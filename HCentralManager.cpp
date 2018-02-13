 /*****************************************************************
 * HCentralManager.cpp: implementation of the CHCentralManager
 * class.
 *
 * Auther: Hamed.M.
 * eMail : 
 *		   
 *****************************************************************/
#include "stdafx.h"
#include "_Global.h"
#include "HCentralManager.h"
#include "PowerVoice.h"
#include "PowerVoiceView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifdef CISCO_CCM

BOOL CHCentralManager::s_threadTerminate = FALSE;

//********************************************************************
//* Construction: CHCentralManager class
//********************************************************************
CHCentralManager::CHCentralManager()
{
	gb_lMngrThread = 0;
	m_pThread = NULL;
	m_pEventThread = NULL;
	m_pDTMFThread = NULL;
	s_threadTerminate = FALSE;
}

//********************************************************************
//* Destruction: CHCentralManager class
//********************************************************************
CHCentralManager::~CHCentralManager()
{
	// Clear Request Queue
	RemoveAllRequests();

	// Terminating Message Thread
	s_threadTerminate = TRUE;
	// Give time to thread to exit
	Sleep(1000);
}

//********************************************************************
//* Seek IVR Channel Object with TAPI line handle
//********************************************************************
CIVRObject *CHCentralManager::FindIVRObjectWithHandle(DWORD dwHandle)
{
	CIVRObject *pObject = NULL;

	if( !pChannelView->m_oIVRCHMap.Lookup(dwHandle, pObject) )
		return NULL;

	return pObject;
}

//********************************************************************
//* Seek IVR Channel Object with TAPI call handle
// bytType = 1, seek channel
// bytType = 2, seek group
//********************************************************************
CIVRObject *CHCentralManager::FindIVRObjectWithCall(DWORD dwHandle, BYTE bytType)
{
	int lv_loop;					// 循环变量
	CIVRObject *pObject = NULL;

	if( bytType & 0x02 )
	{
		for( lv_loop = 0; lv_loop < theApp.m_nIVRGroups; lv_loop++ )
		{
			if( PIVRGroup[lv_loop] )
			{
				if( PIVRGroup[lv_loop]->GetCallHandle() == dwHandle )
				{
					pObject = (CIVRObject *)(PIVRGroup[lv_loop]);
					return pObject;
				}
				// Sun added 2010-07-20
				else if( PIVRGroup[lv_loop]->FindCallListMember(dwHandle) >= 0 )
				{
					pObject = (CIVRObject *)(PIVRGroup[lv_loop]);
					return pObject;
				}
			}
		}
	}

	if( bytType & 0x01 )
	{
		for( lv_loop = 0; lv_loop < theApp.m_nIVRChannels; lv_loop++ )
		{
			if( PIVRCH[lv_loop] )
			{
				if( PIVRCH[lv_loop]->m_oARS.GetCallHandle() == dwHandle ||
					PIVRCH[lv_loop]->m_oARS.GetConfCallHandle() == dwHandle ||
					PIVRCH[lv_loop]->m_oARS.GetConsultCallHandle() == dwHandle )
				{
					pObject = (CIVRObject *)(PIVRCH[lv_loop]);
					return pObject;
				}
			}
		}
	}

	return NULL;
}

//********************************************************************
//* Rematch IVR Channel Object with TAPI line handle
//*
//* Sun added 2010-06-16 [A]
//* 1、判断handle是否属于IVR设备
//* 2、如果是，则查找“IVR设备列表”更新对应关系
//*
//********************************************************************
CIVRChannel *CHCentralManager::RematchIVRObjectWithHandle(DWORD dwHandle)
{
	int lv_loop;					// 循环变量
	CIVRChannel *pObject = NULL;
	DWORD lv_dwLineHandle = 0;

	for( lv_loop = 0; lv_loop < theApp.m_nIVRChannels; lv_loop++ )
	{
		pObject = PIVRCH[lv_loop];
		if( pObject )
		{
			lv_dwLineHandle = pObject->m_oARS.GetLineHandle();
			if( lv_dwLineHandle == dwHandle )
			{
				pChannelView->m_oIVRCHMap.SetAt(lv_dwLineHandle, pObject);
				WriteTrace(TraceWarn, "IVR Channel: %d rematched with TapiLine: 0x%x", pObject->GetLineNO(), dwHandle);

				return pObject;
			}
		}
	}

	return NULL;
}

//********************************************************************
//* Seek Tapi Line Object with TAPI RequestID
//********************************************************************
int CHCentralManager::GetRequesCount()
{
	int lv_value;

	m_bLock.Enter();

	lv_value = GetCount();

	m_bLock.Leave();

	return lv_value;
}

BOOL CHCentralManager::AddRequest(DWORD dwReqID, CTapiLine *f_pLine)
{
	m_bLock.Enter();

	if( f_pLine )
	{
		SetAt(dwReqID, f_pLine);
		WriteTrace(TraceDetail, "Detail - IVR Channel: %d Add tapi request id: 0x%x for line: %d, total: %d", f_pLine->GetDeviceID(), dwReqID, f_pLine->GetLineID(), GetCount());
	}

	m_bLock.Leave();

	return TRUE;
}

void CHCentralManager::RemoveRequest(DWORD dwReqID)
{
	m_bLock.Enter();

	RemoveKey(dwReqID);
	WriteTrace(TraceDetail, "Detail - Remove tapi request id: 0x%x total: %d", dwReqID, GetCount());

	m_bLock.Leave();
}

void CHCentralManager::RemoveAllRequests()
{
	m_bLock.Enter();
	RemoveAll();
	m_bLock.Leave();
}

CTapiLine *CHCentralManager::FindTapiLineWithRequestID(DWORD dwReqID)
{
	CTapiLine *lv_pLine;

	m_bLock.Enter();

	if( !Lookup(dwReqID, lv_pLine) )
		lv_pLine = NULL;
	
	m_bLock.Leave();

	return lv_pLine;
}

//********************************************************************
//* Retrive call info with handle
//********************************************************************
DWORD CHCentralManager::GetCallInfo(DWORD dwCallInfoFlag, HCALL hCall)
{
	DWORD lv_dwSize = 0;
	int lv_loop = 0;
	LINECALLINFO *lpCallInfo = NULL;
	BYTE *lpCallByteData;

	try
	{
		if( hCall )
		{
			lv_dwSize = sizeof(LINECALLINFO) + 1024;

			while( ++lv_loop < 3 )
			{
				lpCallByteData = new BYTE[lv_dwSize];
				lpCallInfo = (LINECALLINFO *)lpCallByteData;
				lpCallInfo->dwTotalSize = lv_dwSize;

				if( lineGetCallInfo(hCall, lpCallInfo) != 0 )
				{
					delete []lpCallByteData;
					return 0;
				}

				if (lpCallInfo->dwTotalSize < lpCallInfo->dwNeededSize )
				{
					lv_dwSize = lpCallInfo->dwNeededSize;
					lpCallInfo = NULL;
					delete []lpCallByteData;
					continue;
				}

				// Exit While Loop
				break;
			}

			if( lpCallInfo )
			{
				WriteTrace(TraceDebug, "TAPI CallInfo - CallState:0x%x, CallID:0x%x, ANI:%s-%s, DNIS:%s-%s, Trunk:0x%x", 
					lpCallInfo->dwCallStates, lpCallInfo->dwCallID,
					(LPCSTR)((LPBYTE)lpCallInfo + lpCallInfo->dwCallerIDOffset),
					(LPCSTR)((LPBYTE)lpCallInfo + lpCallInfo->dwCallerIDNameOffset),
					(LPCSTR)((LPBYTE)lpCallInfo + lpCallInfo->dwCalledIDOffset),
					(LPCSTR)((LPBYTE)lpCallInfo + lpCallInfo->dwCalledIDNameOffset),
					lpCallInfo->dwTrunk);

				CIVRObject *pObject = NULL;
				CIVRChannel *pChannel = NULL;
				char strData[512];
				BYTE lv_CallSource;
				pObject = FindIVRObjectWithHandle(lpCallInfo->hLine);
				if( pObject )
				{
					if( pObject->GetObjectType() == 1 )
					{
						pChannel = (CIVRChannel *)pObject;

						// Call RefID
						if( (dwCallInfoFlag & LINECALLINFOSTATE_CALLID) == LINECALLINFOSTATE_CALLID )
						{
							// Find Call Ref. ID
							strncpy(strData, (LPCTSTR)((LPBYTE)lpCallInfo + lpCallInfo->dwCallerIDOffset), lpCallInfo->dwCallerIDSize);
							switch( lpCallInfo->dwOrigin )
							{
							case LINECALLORIGIN_INTERNAL:
							case LINECALLORIGIN_CONFERENCE:
								lv_CallSource = _pcC_CALLSRC_INTERNAL;
								break;
							case LINECALLORIGIN_EXTERNAL:
								lv_CallSource = _pcC_CALLSRC_CALLCENTER;
								break;
							default:
								lv_CallSource = _pcC_CALLSRC_CALLCENTER;
								break;

							}
							CTime lv_tmCurrent = CTime::GetCurrentTime();
							memset(strData, 0x00, sizeof(strData));
							sprintf(strData, "%s", lv_tmCurrent.Format("%Y%m%d%H%M%S"));
							pChannel->SetCallRefID(lpCallInfo->dwCallID, _pcC_EVTSRC_DN, lv_CallSource, 0, strData);
						}

						// Has ANI ?
						if( (dwCallInfoFlag & LINECALLINFOSTATE_CALLERID) == LINECALLINFOSTATE_CALLERID )
						{
							// Find Caller ID
							memset(strData, 0x00, sizeof(strData));
							strncpy(strData, (LPCTSTR)((LPBYTE)lpCallInfo + lpCallInfo->dwCallerIDOffset), lpCallInfo->dwCallerIDSize);
							pChannel->NF_SetCallerID((UC *)strData);
						}

						// Has DNIS ?
						if( (dwCallInfoFlag & LINECALLINFOSTATE_CALLEDID) == LINECALLINFOSTATE_CALLEDID )
						{
							// Find Called ID
							memset(strData, 0x00, sizeof(strData));
							strncpy(strData, (LPCTSTR)((LPBYTE)lpCallInfo + lpCallInfo->dwCalledIDOffset), lpCallInfo->dwCalledIDSize);
							pChannel->NF_SetCalleeID((UC *)strData);
						}
					}
				}

				// Free Memory
				lpCallInfo = NULL;
				delete []lpCallByteData;
			}
			else
				lv_dwSize = 0;
		}
	}
	catch(...)
	{
	}

	return lv_dwSize;
}

//********************************************************************
//* Will get all messages and process them.
 //*******************************************************************
void CHCentralManager::ProcessMessage(MSG msg)
{
	long ret = 0;

	if( pMainWindow->m_bShutdown )
		return;

	switch( msg.message )
	{
	// New DTMF code pressed by user
	case WM_TAPI_DTMF:
		if( msg.lParam > 0 && msg.wParam > 0 )
		{
			CIVRChannel *pChannel = (CIVRChannel *)msg.lParam;
			BYTE lv_bytChSt = pChannel->GetStatus();
			if( lv_bytChSt == IVR_W_PLAY || lv_bytChSt == IVR_W_KEY || lv_bytChSt == IVR_W_RECORD )
			{
				char lv_chKey = (char)msg.wParam;
				pChannel->m_oARS.TestTPTCondition(lv_chKey);
			}
		}
		break;

	// Line status changed, do appropriate job.
	case WM_TAPI_STATUS_CHNGD:
		//----------------------------------------------------
		// Sun removed 2010-08-02 [A]
		/*
		if( msg.wParam > 0 )
		{
			// If TAPI Failed, cleanup and exit
			CTapiLine *lv_pLine = (CTapiLine *)msg.wParam;
			STATUS lv_nCurrST = lv_pLine->GetLineStatus();
			if( lv_nCurrST == ST_FAILED )
			{
				WriteTrace(TraceError, "IVR Channel: %d TAPI failed event.", lv_pLine->GetDeviceID());
			}
			else if( (lv_nCurrST == ST_IBBUSY || lv_nCurrST == ST_OBBUSY) && msg.lParam > 0 )
			{
				// Call Answered, Start IVR Call Flow
				CIVRChannel *pChannel = (CIVRChannel *)msg.lParam;
				WriteTrace(TraceDebug, "IVR Channel: %d received TapiRing... Ring..", pChannel->GetLineNO());
				pChannel->NF_CallIn();
			}
			else
			{
				WriteTrace(TraceDetail, _T("IVR Channel: %d received line status changed msg to: %d"), 
					lv_pLine->GetDeviceID(), lv_nCurrST);
			}
		}
		else if( (STATUS)msg.lParam == ST_FAILED )
		{
			WriteTrace(TraceError, "Error - WM_TAPI_STATUS_CHNGD got TAPI failed event.");
		}
		else
		{
			WriteTrace(TraceError, "Error - WM_TAPI_STATUS_CHNGD event with NULL TapiLine!");
		}
		*/
		//----------------------------------------------------
		break;

	// Line Event triggerred
	case WM_TAPI_LINEEVENT:
		{
			LINEMESSAGE stLineMsg;
			DWORD		dwTimeout = 10;			// Miliseconds
			CIVRObject  *pObject = NULL;
			CIVRChannel *pChannel = NULL;
			CIVRGroup   *pGroup = NULL;
			BOOL		lv_blnSetHookOn;

			if( !gb_hLineApp )
				return;

			// Give me message information
			ret = lineGetMessage(gb_hLineApp, &stLineMsg, dwTimeout);

			// Check errors
			if ( ret != 0 )
			{
				WriteTrace(TraceError, _T("CHCentralManager::ProcessMessage(), Err: Can't get message. Err:[%x]"), ret);
				return;
			}

			WriteTrace(TraceDetail, _T("Tapi Event Detail - Event 0x%x, Device: 0x%x, Param1: 0x%x, Param2: 0x%x, Param3: 0x%x"),
				stLineMsg.dwMessageID, stLineMsg.hDevice, stLineMsg.dwParam1, stLineMsg.dwParam2, stLineMsg.dwParam3);

			switch( stLineMsg.dwMessageID )
			{
			case LINE_ADDRESSSTATE:
				WriteTrace(TraceDetail, "消息：LINE_ADDRESSSTATE");
				break;

			case LINE_APPNEWCALL:
				WriteTrace(TraceDetail, "消息：LINE_APPNEWCALL, Device Handle = 0x%x Call Handle = 0x%x", stLineMsg.hDevice, stLineMsg.dwParam2);
				if( stLineMsg.dwParam3 == LINECALLPRIVILEGE_OWNER )
				{
					// Find IVR Channel
					pObject = FindIVRObjectWithHandle(stLineMsg.hDevice);
					if( pObject )
					{
						if( pObject->GetObjectType() == 1 )
						{
							pChannel = (CIVRChannel *)pObject;
							
							// Sun updated 2009-02-28, [A]
							/// From
							// pChannel->m_oARS.SetCallHandle((HCALL)stLineMsg.dwParam2);
							/// To
							if( pChannel->GetStatus() == IVR_W_ANSWER )
								pChannel->m_oARS.SetCallHandle((HCALL)stLineMsg.dwParam2);
							//-------------------------
							// Sun added 2012-12-26
							else if( pChannel->GetIsOutCall() )
								pChannel->m_oARS.SetCallHandle((HCALL)stLineMsg.dwParam2);
							//-------------------------
							else
								pChannel->m_oARS.SetConsultCallHandle((HCALL)stLineMsg.dwParam2);
						}
						else
						{
							pGroup = (CIVRGroup *)pObject;
							pGroup->SetCallHandle((HCALL)stLineMsg.dwParam2);

							// Sun added 2010-07-20
							if( pGroup->SetCallListMember(stLineMsg.dwParam2) < 0 )
							{
								WriteTrace(TraceError, "Error - IVR Group: %d RoutePoint can't add call handle to list!", pGroup->GetGroupNO());
							}
						}
					}
					// Sun added 2011-06-16 [A]
					else
					{
						WriteTrace(TraceWarn, "Tapi Warn - received no owner LINE_APPNEWCALL message, object=0x%x, call=0x%x, type=0x%x",  stLineMsg.hDevice, stLineMsg.dwParam2, stLineMsg.dwParam3);
					}
				}
				break;

			// New information about the call is available
			case LINE_CALLINFO:
				WriteTrace(TraceDetail, "消息：LINE_CALLINFO");
				// Notes: we do it on CallOffer event instead!
				//GetCallInfo(stLineMsg.dwParam1, (HCALL)stLineMsg.hDevice);
				break;

			// Something has changed!
			case LINE_CALLSTATE : 
				// Find IVR Channel
				pObject = FindIVRObjectWithCall(stLineMsg.hDevice, 0x03);
				if( pObject )
				{
					if( pObject->GetObjectType() == 1 )
					{
						pChannel = (CIVRChannel *)pObject;

						// What's the change?
						switch( stLineMsg.dwParam1 )
						{
							// Line is IDLE, so Time to clean up
						case LINECALLSTATE_IDLE:
							WriteTrace(TraceDetail, "IVR Channel: %d TapiMsg: LINECALLSTATE_IDLE", pChannel->GetLineNO());
							// Cleanup and restart, to receive next call
							//pChannel->m_oARS.Restart();

							// Sun updated 2007-12-12, add 'if...'
							if( stLineMsg.hDevice == pChannel->m_oARS.GetCallHandle() )
							{
								// Sun added 2010-07-22
								/// 磋商过程中主叫挂机
								if( pChannel->GetIsConferenced() && stLineMsg.hDevice != pChannel->m_oARS.GetConsultCallHandle() )
								{
									pChannel->SetIsConferenced(FALSE);
									pChannel->AnsySetHookOn();
								}
								else
								{
									pChannel->m_oARS.DropCall();
									pChannel->m_oARS.DeallocateCall();				// Sun added 2008-06-16
									pChannel->m_oARS.SetLineStatus(ST_READY);
								}
							}
							else
							{
								// Sun added 2008-06-16, and removed 2013-07-05
								// 有这一句电话不能拾起
								//pChannel->m_oARS.DeallocateCall(stLineMsg.hDevice);
							}
							break;

							// Don't make mistake!, it is not ringing!
						case LINECALLSTATE_OFFERING:
							WriteTrace(TraceDebug, "IVR Channel: %d TapiMsg: LINECALLSTATE_OFFERING", pChannel->GetLineNO());

							// Sun added 'if condition' 2008-12-30 [B]
							if( pChannel->m_oARS.GetLineStatus() == ST_READY )
							{
								pChannel->m_oARS.SetCallHandle((HCALL)stLineMsg.hDevice);
								GetCallInfo(0xffffffff, (HCALL)stLineMsg.hDevice);
								pChannel->m_oARS.SetLineStatus(ST_OFFER);
								// Accept the Call
								pChannel->m_oARS.AcceptCall();
							}
							else
							{
								// Sun added 2008-12-30 [B]
								WriteTrace(TraceWarn, "Warn - IVR Channel: %d TapiMsg: LINECALLSTATE_OFFERING in Error Status: %d", pChannel->GetLineNO(), pChannel->m_oARS.GetLineStatus());
							}
							break;

							// Call has been accepted
						case LINECALLSTATE_ACCEPTED:
							WriteTrace(TraceDetail, "IVR Channel: %d TapiMsg: LINECALLSTATE_ACCEPTED", pChannel->GetLineNO());
							break;

							// Line is ready to receive voice, Hi!
						case LINECALLSTATE_CONNECTED:
							WriteTrace(TraceDebug, "IVR Channel: %d TapiMsg: LINECALLSTATE_CONNECTED, LINECONNECTEDMODE=0x%x, CH Status=%d, Line Status=%d Call:0x%x", 
								pChannel->GetLineNO(), stLineMsg.dwParam2, pChannel->GetStatus(), pChannel->m_oARS.GetLineStatus(), pChannel->m_oARS.GetCallHandle());
							// SUN 2007-10-20, 会议时也可能有接通消息
							/// 如果这时MonitorDigit可能失败
							/// Sun updated 2010-07-16
							/// From
							//if( !pChannel->GetIsInCall() )
							// To
							if( pChannel->GetStatus() == IVR_W_ANSWER || !pChannel->GetIsInCall() )
							{
								// Start listening to touchtones from user
								if( pChannel->m_oARS.MonitorDigits() == 0 )
								{
									if( pChannel->m_oARS.GetLineStatus() == ST_RING )
										pChannel->m_oARS.SetLineStatus(ST_IBBUSY, (LPARAM)pChannel);
									else
										pChannel->m_oARS.SetLineStatus(ST_OBBUSY, (LPARAM)pChannel);
								}
								else
								{
									// Sun updated 2008-06-12
									/// From
									//pChannel->m_oARS.Restart();
									/// To
									//WriteTrace(TraceWarn, "Warn - IVR Channel: %d (%s) will restart.", pChannel->GetLineNO(), pChannel->m_oARS.GetDeviceName());
									//pChannel->m_oARS.ivr_channel_restart();

									if( pChannel->m_oARS.GetLineStatus() == ST_RING )
										WriteTrace(TraceWarn, "Warn - IVR Channel: %d (%s) failed to listen DTMF on ringing.", pChannel->GetLineNO(), pChannel->m_oARS.GetDeviceName());
								}
							}
							//------------------------------------------
							// Sun added 2012-12-26
							else if( pChannel->GetIsOutCall() && pChannel->m_oARS.GetLineStatus() == ST_RINGBK )
							{
								if( !pChannel->m_oARS.MonitorDigits() == 0 )
									WriteTrace(TraceWarn, "Warn - IVR Channel: %d (%s) failed to listen DTMF on ringing-back.", pChannel->GetLineNO(), pChannel->m_oARS.GetDeviceName());

								// Notes: f_Refer must be Null to avoid re-calling NF_CallIn()
								pChannel->m_oARS.SetLineStatus(ST_OBBUSY);
							}
							//------------------------------------------
							else
							{
								// Sun added 2007-12-12 && updated 2009-02-28 [B]
								pChannel->m_oARS.MonitorDigits(stLineMsg.hDevice);
								if( pChannel->GetQueueWaitEventID() >= 14 && pChannel->GetQueueWaitEventID() != 19 )
									pChannel->SetQueueWaitEvent(14);

								// Sun added 2012-04-18 [B]
								DWORD lv_dwLineDevStatus, lv_dwFeature;
								lv_dwLineDevStatus = pChannel->m_oARS.PrintLineStatus(&lv_dwFeature);
								if( lv_dwLineDevStatus == LINEDEVSTATUSFLAGS_CONNECTED + LINEDEVSTATUSFLAGS_INSERVICE )
								{
									if( (lv_dwFeature & LINECALLFEATURE_ANSWER) == LINECALLFEATURE_ANSWER )
									{
										//pChannel->m_oARS.AcceptCall();
										//pChannel->m_oARS.AnswerCall();
									}
								}
							}
							break;

							// One Party added into conference
						case LINECALLSTATE_CONFERENCED:
							WriteTrace(TraceDebug, "IVR Channel: %d TapiMsg: LINECALLSTATE_CONFERENCED [con-call: 0x%x, insert-call: 0x%x]", pChannel->GetLineNO(), stLineMsg.dwParam2, stLineMsg.hDevice);
							// Sun added 2007-12-12, add 'if...'
							if( stLineMsg.dwParam2 > 0 )
							{
								pChannel->m_oARS.SetConfCallHandle(stLineMsg.dwParam2);

								// Sun added 2010-07-22
								pChannel->SetIsConferenced(TRUE);
							}
							if( pChannel->m_oARS.GetCallHandle() != stLineMsg.hDevice )
								pChannel->m_oARS.SetConsultCallHandle(stLineMsg.hDevice);
							break;

							// The remote party has disconnected from the call
							// Bye!
						case LINECALLSTATE_DISCONNECTED:
							WriteTrace(TraceDebug, "IVR Channel: %d TapiMsg: LINECALLSTATE_DISCONNECTED, LINEDISCONNECTMODE=0x%x, CallID=0x%x, ConsCallID=0x%x, ConfID=0x%x", 
								pChannel->GetLineNO(), stLineMsg.dwParam2, 
								pChannel->m_oARS.GetCallHandle(), pChannel->m_oARS.GetConsultCallHandle(), pChannel->m_oARS.GetConfCallHandle());

							// Sun added 2007-12-12
							lv_blnSetHookOn = TRUE;
							//if( pChannel->GetQueueWaitEventID() >= 13 && stLineMsg.hDevice == pChannel->m_oARS.GetConsultCallHandle() )
							if( stLineMsg.hDevice == pChannel->m_oARS.GetConsultCallHandle() )
							{
								switch( stLineMsg.dwParam2 )
								{
								// 已测试：
								case LINEDISCONNECTMODE_BUSY:
								case LINEDISCONNECTMODE_BADADDRESS:

								// 未测试：
								case LINEDISCONNECTMODE_REJECT:
								case LINEDISCONNECTMODE_NOANSWER:
								case LINEDISCONNECTMODE_UNREACHABLE:
								case LINEDISCONNECTMODE_UNAVAIL:

									if( pChannel->GetQueueWaitEventID() >= 13 )
										pChannel->SetQueueWaitEvent(17);

									lv_blnSetHookOn = FALSE;
									break;

								case LINEDISCONNECTMODE_NORMAL:
									lv_blnSetHookOn = FALSE;
									break;

								default:
									break;
								}
							}

							if( lv_blnSetHookOn )
							{
								// Don't restart here
								//-----------------------------------------
								/// Sun updated 2010-07-20 [D]
								/// From
								//pChannel->SetHookOn();
								/// To
								pChannel->SetIsConferenced(FALSE);		// Sun added 2010-07-22
								pChannel->AnsySetHookOn();
								//-----------------------------------------
							}
							break;

						//----------------------------------------------------------
						// Sun added 2007-12-12
						case LINECALLSTATE_DIALTONE:
							WriteTrace(TraceDebug, "IVR Channel: %d TapiMsg: LINECALLSTATE_DIALTONE, LINEDIALTONEMODE=0x%x", pChannel->GetLineNO(), stLineMsg.dwParam2);
							break;

						case LINECALLSTATE_DIALING:
							WriteTrace(TraceDetail, "IVR Channel: %d TapiMsg: LINECALLSTATE_DIALING", pChannel->GetLineNO());

							// Sun added 2012-12-26
							if( pChannel->m_oARS.GetLineStatus() == ST_READY )
							{
								pChannel->m_oARS.SetCallHandle((HCALL)stLineMsg.hDevice);
								pChannel->m_oARS.SetLineStatus(ST_DIAL);
							}

							break;

						case LINECALLSTATE_PROCEEDING:				// 允许完成会议
							WriteTrace(TraceDetail, "IVR Channel: %d TapiMsg: LINECALLSTATE_PROCEEDING", pChannel->GetLineNO());
							if( pChannel->GetQueueWaitEventID() >= 13 )
								pChannel->SetQueueWaitEvent(13);

							break;

						case LINECALLSTATE_RINGBACK:				// 允许完成会议
							WriteTrace(TraceDetail, "IVR Channel: %d TapiMsg: LINECALLSTATE_RINGBACK", pChannel->GetLineNO());

							// Sun added 2012-12-26
							if( pChannel->m_oARS.GetLineStatus() == ST_DIAL )
								pChannel->m_oARS.SetLineStatus(ST_RINGBK);

							break;

						case LINECALLSTATE_BUSY:
							WriteTrace(TraceDebug, "IVR Channel: %d TapiMsg: LINECALLSTATE_BUSY, LINEBUSYMODE=0x%x", pChannel->GetLineNO(), stLineMsg.dwParam2);
							if( pChannel->GetQueueWaitEventID() >= 13 )
								pChannel->SetQueueWaitEvent(17);
							break;
						//----------------------------------------------------------
						}
					}
					else
					{
						pGroup = (CIVRGroup *)pObject;

						// What's the change?
						switch( stLineMsg.dwParam1 )
						{
							// Line is IDLE, so Time to clean up
						case LINECALLSTATE_IDLE:
							WriteTrace(TraceDetail, "IVR Group消息：LINECALLSTATE_IDLE CallID=0x%x", stLineMsg.hDevice);

							if( pGroup->DropCall(stLineMsg.hDevice) != 0 )
								pGroup->DeallocateCall(stLineMsg.hDevice);		// Sun added 2008-06-16

							// Sun added 2010-07-20
							pGroup->ClearCallListMember(stLineMsg.hDevice);
							break;

							// Don't make mistake!, it is not ringing!
						case LINECALLSTATE_OFFERING:
							WriteTrace(TraceDebug, "IVR Group消息：LINECALLSTATE_OFFERING CallID=0x%x", stLineMsg.hDevice);
							pGroup->SetCallHandle((HCALL)stLineMsg.hDevice);

							// Accept the Call
							pGroup->DistributeCallToChannel(stLineMsg.hDevice);
							break;

							// The remote party has disconnected from the call
							// Bye!
						case LINECALLSTATE_DISCONNECTED:
							WriteTrace(TraceDebug, "IVR Group消息：LINECALLSTATE_DISCONNECTED CallID=0x%x", stLineMsg.hDevice);
							break;
						}
					}
				}
				else
				{
					// Sun added 2008-06-12
					WriteTrace(TraceWarn, "Tapi Warn - received no owner call state message, object=0x%x, state=0x%x",  stLineMsg.hDevice, stLineMsg.dwParam1);
				}
				break;

			// Close the line
			case LINE_CLOSE:
				WriteTrace(TraceDebug, "消息：LINE_CLOSE");
				
				// Sun added 2008-05-23
				if( pMainWindow->m_bRunning )
				{
					pObject = FindIVRObjectWithHandle(stLineMsg.hDevice);
					if( pObject )
					{
						if( pObject->GetObjectType() == 1 )
						{
							pChannel = (CIVRChannel *)pObject;

							// Remove group from device list
							pChannelView->m_oIVRCHMap.RemoveKey(pChannel->m_oARS.GetLineHandle());
							WriteTrace(TraceInfo, "IVR Channel: %d unbind with TapiLine: 0x%x in LINE_CLOSE MSG.", pChannel->GetLineNO(), pChannel->m_oARS.GetLineHandle());

							pChannel->m_oARS.Close();
							pChannel->PropertyChanged(SCR_CHANNEL_ITEM_STATUS);
						}
						else
						{
							pGroup = (CIVRGroup *)pObject;

							// Remove group from device list
							pChannelView->m_oIVRCHMap.RemoveKey(pGroup->GetLineHandle());
							WriteTrace(TraceWarn, "IVR Group: %d unbind with TapiLine: 0x%x in LINE_CLOSE MSG.", pGroup->GetGroupNO(), pGroup->GetLineHandle());

							pGroup->Close();
							pGroup->PropertyChanged(SCR_GROUP_ITEM_GROUPID);
						}
					}
				}
				break;

			case LINE_CREATE:
				WriteTrace(TraceDebug, "消息：LINE_CREATE");
				break;

			case LINE_DEVSPECIFIC:
				WriteTrace(TraceDetail, "消息：LINE_DEVSPECIFIC");
				break;

			case LINE_DEVSPECIFICFEATURE:
				WriteTrace(TraceDetail, "消息：LINE_DEVSPECIFICFEATURE");
				break;

			case LINE_GATHERDIGITS:
				WriteTrace(TraceDetail, "消息：LINE_GATHERDIGITS");
				break;

			case LINE_GENERATE:
				WriteTrace(TraceDetail, "消息：LINE_GENERATE");
				break;

			case LINE_LINEDEVSTATE:
				WriteTrace(TraceDetail, "消息：LINE_LINEDEVSTATE");
				// Find IVR Channel
				pObject = FindIVRObjectWithHandle(stLineMsg.hDevice);
				if( pObject )
				{
					if( pObject->GetObjectType() == 1 )
					{
						pChannel = (CIVRChannel *)pObject;

						// Ringing, Please answer the call
						if( stLineMsg.dwParam1 == LINEDEVSTATE_RINGING)
						{
							// Hey, dady! It's ringing
							pChannel->m_oARS.SetLineStatus(ST_RING);

							// Should I pick the phone Up?
							if ( stLineMsg.dwParam3 >= (DWORD)gb_RingsPreOffhook )
							{
								// Answer Call
								pChannel->m_oARS.AnswerCall();
							}
						}
						// Connetion is broken
						else if( stLineMsg.dwParam1 == LINEDEVSTATE_OUTOFSERVICE )
						{
							WriteTrace(TraceWarn, "Warn - IVR Channel: %d TAPI is out of service", pChannel->GetLineNO());

							// Inform CTI
							pChannel->m_oARS.SetLineStatus(ST_INIT);
							pChannel->PropertyChanged(SCR_CHANNEL_ITEM_STATUS, TRUE);
						}
						// Connetion resume
						else if( stLineMsg.dwParam1 == LINEDEVSTATE_INSERVICE )
						{
							WriteTrace(TraceInfo, "Warn - IVR Channel: %d TAPI is in service", pChannel->GetLineNO());

							// Inform CTI
							pChannel->m_oARS.SetLineStatus(ST_READY);
							pChannel->PropertyChanged(SCR_CHANNEL_ITEM_STATUS, TRUE);
						}
					}
					else
					{
						pGroup = (CIVRGroup *)pObject;
						pGroup->SetCallHandle((HCALL)stLineMsg.dwParam2);

						// Connetion is broken
						if( stLineMsg.dwParam1 == LINEDEVSTATE_OUTOFSERVICE )
						{
							WriteTrace(TraceWarn, "Warn - group: %d is out of service", pGroup->GetGroupNO());

							// Inform CTI
							pGroup->SetLineStatus(ST_INIT);
							pGroup->PropertyChanged(SCR_GROUP_ITEM_GROUPID, TRUE);
						}
						// Connetion resume
						else if( stLineMsg.dwParam1 == LINEDEVSTATE_INSERVICE )
						{
							WriteTrace(TraceInfo, "Warn - group: %d is in service", pGroup->GetGroupNO());

							// Inform CTI
							pGroup->SetLineStatus(ST_READY);
							pGroup->PropertyChanged(SCR_GROUP_ITEM_GROUPID);
						}
					}
				}
				else
				{
					// Sun added 2008-06-12
					/// Refind Line
					if( stLineMsg.dwParam1 == LINEDEVSTATE_INSERVICE )
					{
						//lineGetLineDevStatus stLineMsg.hDevice
						WriteTrace(TraceError, "Error ToDo: should refind device handle: 0x%x!!!", stLineMsg.hDevice);

						// Sun added 2011-06-16 [A]
						pChannel = RematchIVRObjectWithHandle(stLineMsg.hDevice);
						if( pChannel )
						{
							WriteTrace(TraceInfo, "Info - IVR Channel: %d TAPI is in service", pChannel->GetLineNO());

							// Inform CTI
							pChannel->m_oARS.SetLineStatus(ST_READY);
							pChannel->PropertyChanged(SCR_CHANNEL_ITEM_STATUS, TRUE);
						}
					}
				}
				break;

			// Touch tone digit arrived!
			case LINE_MONITORDIGITS : 
				WriteTrace(TraceDetail, "消息：LINE_MONITORDIGITS，Key:'%c'", stLineMsg.dwParam1);
				// Find IVR Channel
				pChannel = (CIVRChannel *)FindIVRObjectWithCall(stLineMsg.hDevice);
				if( pChannel )
				{

					// Process digits
					if( stLineMsg.dwParam2 == LINEDIGITMODE_DTMF )
					{
						pChannel->m_oARS.PushKey(LOBYTE(stLineMsg.dwParam1));
						// Post Thread Message for TPT
						if( m_pDTMFThread )
						{
							// Sun updated 2010-07-20
							BOOL lv_blnSend = FALSE;
							int lv_nTimer = 5;		// 数值越大成功率越高
							while( lv_nTimer > 0 )
							{
								lv_blnSend = PostThreadMessage(m_pDTMFThread->m_nThreadID, WM_TAPI_DTMF, stLineMsg.dwParam1, (LPARAM)pChannel);
								if( lv_blnSend )
									break;
								lv_nTimer--;
								Sleep(10);
							}
							if( !lv_blnSend )
							{
								WriteTrace(TraceWarn, "IVR Channel: %d Line:%d failed to post WM_TAPI_DTMF msg:'%c'", 
									pChannel->GetLineNO(), stLineMsg.dwParam1);
							}
						}
					}
				}
				break;

			case LINE_MONITORMEDIA:
				WriteTrace(TraceDetail, "消息：LINE_MONITORMEDIA");
				break;

			// Tone arrived, any computer is trying to connect?
			case LINE_MONITORTONE : 
				WriteTrace(TraceDetail, "消息：LINE_MONITORTONE");
				// ?
				break;

			case LINE_REMOVE:
				WriteTrace(TraceDebug, "消息：LINE_REMOVE");
				break;

			// Any detectable error(s)?
			case LINE_REPLY : 
				WriteTrace(TraceDetail, "消息：LINE_REPLY, RequestID = 0x%x, Result = 0x%x", stLineMsg.dwParam1, stLineMsg.dwParam2);
				// Possibly an error occured
				// Handle errors, and stop later calls or so...
				{
					// Sun added 2010-08-04
					CSingleLock lv_MySingleLock(&m_ReplyLock);
					lv_MySingleLock.Lock(2000);

					CTapiLine *lv_pTapiLine = FindTapiLineWithRequestID(stLineMsg.dwParam1);
					if( lv_pTapiLine )
					{
						if( stLineMsg.dwParam1 == lv_pTapiLine->GetDistributeRequest() )
						{
							if( (long)stLineMsg.dwParam2 < 0 )
							{
								WriteTrace(TraceWarn, "Warn - IVR Channel: %d call redirect failed for line: %d, RequestID = 0x%x, Result = 0x%x", lv_pTapiLine->GetDeviceID(), lv_pTapiLine->GetLineID(), stLineMsg.dwParam1, stLineMsg.dwParam2);

								// Resart Line
								// Sun replaced 2011-06-16 [A]
								/// From
								//lv_pTapiLine->Restart();
								/// To
								/// 需要更新IVR通道查询列表
								try
								{
									if( lv_pTapiLine->GetDeviceID() >= 0 && lv_pTapiLine->GetDeviceID() < theApp.m_nIVRChannels )
									{
										pChannel = PIVRCH[lv_pTapiLine->GetDeviceID()];
										if( pChannel )
											pChannel->m_oARS.ivr_channel_restart();
									}
								}
								catch(...)
								{
								}
							}

							lv_pTapiLine->SetDistributeRequest(0);
						}
						else
						{
							lv_pTapiLine->StopWaitCurrentRequest(stLineMsg.dwParam2);
							if( stLineMsg.dwParam2 > 0 )
							{
								WriteTrace(TraceWarn, "Warn - IVR Channel: %d call got REPLY RequestID = 0x%x, Result = 0x%x", lv_pTapiLine->GetDeviceID(), stLineMsg.dwParam1, stLineMsg.dwParam2);
							}
							else
							{
								WriteTrace(TraceDebug, "Debug - IVR Channel: %d call got REPLY RequestID = 0x%x, Result = 0x%x", lv_pTapiLine->GetDeviceID(), stLineMsg.dwParam1, stLineMsg.dwParam2);
							}
						}
					}
				}
				break;

			case LINE_REQUEST:
				WriteTrace(TraceDetail, "消息：LINE_REQUEST");
				break;

			case LINEERR_OPERATIONUNAVAIL:
				WriteTrace(TraceDetail, "消息：LINEERR_OPERATIONUNAVAIL");
				break;

			default:
				WriteTrace(TraceError, _T("Unknown Taip event [%x] "), stLineMsg.dwMessageID);
				break;
			}
		}
		break;

	default : break;
	}
}

//********************************************************************
//* Wakes up all managers!
//********************************************************************
BOOL CHCentralManager::WakeUp()
{
	try
	{
		s_threadTerminate = FALSE;

		// Begin a message thread so that others can communicate us
		m_pThread = AfxBeginThread(
			MessageThread,
			(LPVOID)this,
			THREAD_PRIORITY_BELOW_NORMAL,
			0,
			CREATE_SUSPENDED,
			0);
		// If we can't run manager
		if (!m_pThread)
		{
			gb_lMngrThread = 0;
			return FALSE;
		}
		// Set global variable
		gb_lMngrThread = m_pThread->m_nThreadID;
		m_pThread->m_bAutoDelete = FALSE;
		
		// Run a message thread to get TAPI Events
		m_pEventThread = AfxBeginThread(
			TapiEventThread,
			(LPVOID)this,
			THREAD_PRIORITY_NORMAL,
			0,
			CREATE_SUSPENDED,
			0);
		// If we can't run manager
		if (!m_pEventThread)
		{
			delete m_pThread;
			m_pThread = NULL;
			return FALSE;
		}
		m_pEventThread->m_bAutoDelete = FALSE;
		
		// Run a message thread to get DTMF Messages
		m_pDTMFThread = AfxBeginThread(
			DTMFThread,
			(LPVOID)this,
			THREAD_PRIORITY_NORMAL,
			0,
			CREATE_SUSPENDED,
			0);
		// If we can't run manager
		if (!m_pDTMFThread) return FALSE;
		m_pDTMFThread->m_bAutoDelete = FALSE;

		// Start Thread
		m_pThread->ResumeThread();
		m_pEventThread->ResumeThread();
		m_pDTMFThread->ResumeThread();

		// Handle line & phone exceptions
	}
	catch( CTapiObj::TEx e )
	{
		WriteTrace(TraceError, "line manager. source: Central Manager - id: %d, res: %s", e.code, e.result);

		// Sun removed 2010-08-02 [A]
		//if( gb_lMngrThread )
		//	PostThreadMessage(gb_lMngrThread, WM_TAPI_STATUS_CHNGD, 0, ST_FAILED);

		return FALSE;
		// Handle other exceptions
	}
	catch( ... )
	{
		WriteTrace(TraceError, "line manager. source: Central Manager: Unknown error in waking up");

		// Sun removed 2010-08-02 [A]
		//if( gb_lMngrThread )
		//	PostThreadMessage(gb_lMngrThread, WM_TAPI_STATUS_CHNGD, 0, ST_FAILED);

		return FALSE;
	}

	return TRUE;
}

//********************************************************************
//* Does reverse of wakeup! Freezes everythings
//********************************************************************
void CHCentralManager::SleepAll()
{
	if( m_pThread )
	{	
		// If thread already started
		// Terminate Message Thread
		s_threadTerminate = TRUE;
		// Sleep while thread exits
		if( WaitForSingleObject(m_pThread->m_hThread, 300 ) == WAIT_TIMEOUT)
		{
			TerminateThread(m_pThread->m_hThread, 0);
		}
		// Sleep while thread exits
		if( WaitForSingleObject(m_pEventThread->m_hThread, 300 ) == WAIT_TIMEOUT)
		{
			TerminateThread(m_pEventThread->m_hThread, 0);
		}
		// Sleep while thread exits
		if( WaitForSingleObject(m_pDTMFThread->m_hThread, 300) == WAIT_TIMEOUT)
		{
			TerminateThread(m_pDTMFThread->m_hThread, 0);
		}

		try
		{
			delete m_pThread;
			m_pThread = NULL;
			gb_lMngrThread = 0;

			delete m_pEventThread;
			m_pEventThread = NULL;

			delete m_pDTMFThread;
			m_pDTMFThread = NULL;
		}
		catch(...)
		{
		}
	}
}

//********************************************************************
//* A thread to receive message from other managers
//********************************************************************
//* A pointer to this thread will be available globaly, so that others
//* can access central manager.
//* TAPI classes (line & phone) will send their message to this thread
//* either.
//********************************************************************
UINT CHCentralManager::MessageThread(LPVOID pParam)
{
	CHCentralManager* pCM = (CHCentralManager*) pParam;
	int lv_loop;					// 循环变量
	BYTE lv_bytChSt;
	MSG msg;

	WriteTrace(TraceInfo, _T("SysInfo - MessageThread Started."));
	while( !s_threadTerminate )
	{
		if( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
		{
			// New Message arrived -> process it please!
			pCM->ProcessMessage(msg);
		}
		else
		{
			// Channel Check Timer
			for( lv_loop = 0; lv_loop < theApp.m_nIVRChannels; lv_loop++ )
			{
				if( PIVRCH[lv_loop] )
				{
					lv_bytChSt = PIVRCH[lv_loop]->GetStatus();
					if( lv_bytChSt == IVR_W_PLAY || lv_bytChSt == IVR_W_KEY || lv_bytChSt == IVR_W_RECORD )
					{
						PIVRCH[lv_loop]->m_oARS.TestTPTCondition();
					}
				}
			}

			// Check message queue every 10 ms
			Sleep(10);
		}
	}

	WriteTrace(TraceInfo, _T("SysInfo - MessageThread Stopped."));
	return 0;
}

//*******************************************************************
//* A thread to receive TAPI line event to this thread
//*******************************************************************
UINT CHCentralManager::TapiEventThread(LPVOID pParam)
{
	CHCentralManager* pCM = (CHCentralManager*) pParam;
	MSG msg;
	CString sz;

	WriteTrace(TraceInfo, _T("SysInfo - TapiEventThread Started."));
	while( !s_threadTerminate )
	{
		switch( WaitForSingleObject(gb_hLineEvent, 10) )
		{
		// Have we got any events?
		case WAIT_OBJECT_0:
			// I did not catch errors here
			try
			{
				msg.message = WM_TAPI_LINEEVENT;
				pCM->ProcessMessage(msg);

			}
			catch( CTapiObj::TEx e )
			{
				sz.Format(_T("TapiEventThread Err: [code:%d],")
					_T("[date:%s],")
					_T("[time:%s],")
					_T("[reason:%s]\n"), e.code, e.date, e.time, e.result);
				WriteTrace(TraceError, sz);
			}
			catch(...)
			{
				WriteTrace(TraceError, _T("TapiEventThread Error!!!"));
			}
			break;
		
		// Sure we have nothing to process now
		case WAIT_TIMEOUT:
			break;

		// Go on waiting and processing
		default: continue;
		}
	}

	WriteTrace(TraceInfo, _T("SysInfo - TapiEventThread Stopped."));
	return 0;
}

//********************************************************************
//* A thread to receive message from DTMF manager
//********************************************************************
//* A thread to receive DTMF messages. Don't try to mix this message
//* thread with MessageThread. The reason I separated these two is
//* to be able to detect and respond to line messages more quickly and
//* independent to current status of the other thread. Think about a 
//* chicky user who pressed 100 digits one after the other and then 
//* disconnected, calling parent's ProcessMessage() function directly, 
//* make us busy the line untill all button process completed and then
//* process disconnect message.(It's a queue). But separating these 2
//* ensures that line messages will be processed as soon as comming.
//********************************************************************
UINT CHCentralManager::DTMFThread (LPVOID pParam)
{
	CHCentralManager* pCM = (CHCentralManager*) pParam;
	MSG msg;

	WriteTrace(TraceInfo, _T("SysInfo - DTMFThread Started."));
	while( !s_threadTerminate )
	{
		if( PeekMessage(&msg,NULL,0,0,PM_REMOVE) )
		{
			// New Message arrived -> process it please!
			pCM->ProcessMessage(msg);
		}
		else
		{
			// Check message queue every 10 ms
			Sleep(10);
		}
	}

	WriteTrace(TraceInfo, _T("SysInfo - DTMFThread Stopped."));
	return 0;
}
#endif

/*********************************************************************
 * End of Central manager :)
 ********************************************************************/
