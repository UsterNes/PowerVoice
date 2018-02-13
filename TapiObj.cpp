/*********************************************************************
 * TapiObj.cpp:
 * implementation of the 
 *							CTapiObj, 
 *							CTapiLine, 
 *							CPhone classes
 * WARNINGS:
 *			  - Just 1 and only 1 instance of these class must be
 *				created.
 *
 * Auther: Hamed.M.
 * eMail : 
 *
 ********************************************************************/
#include "stdafx.h"

#include ".\tapiobj.h"
#include "PowerVoice.h"
#include "_CommonFunc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifdef CISCO_CCM

#ifdef CUCM_MEDIA

// Endpoint 回调函数, Sun added 2013-04-12
// Application can set endpoint callback to receive an indication about operation completions, data
/// transfers, errors, etc. Endpoint callback function type is declared as follows:
/* Parameters：
	hEp - Endpoint handle
	hStream - Rtp stream handle
	dwError - If not 0 (zero), indicates an error
	pData - Endpoint handle
	dwDataSize - Number of bytes received / transfered.
	pUserData - Application data associated with an operation.
	bIsSilence - If set to true, indicates that silence has been detected.
	streamDir - Stream direction. Can be one of the following:
			ToApp = 0x01,         //In,
			ToNwk = 0x02,         //Out
			Both  = 0x03          //In/Out
*/
void WINAPI MediaDriverEpCallback(HANDLE hEp, HANDLE hStream, DWORD dwError, PUCHAR pData, DWORD dwDataSize, LPVOID pUserData, bool bIsSilence, StreamDirection streamDir)
{
	if( dwError != 0 )
	{
		// Error
		WriteTrace(TraceError, "MediaDriverEpCallback Error: 0x%x on Endpoint handle = 0x%x", dwError, hEp);
	}
	else
	{
		// Detail
		WriteTrace(TraceDetail, "MediaDriverEpCallback Detail on Endpoint handle = 0x%x, stream = 0x%x, Data Size = %d, StreamDir = %d, IsSilence = %d", 
			hEp, hStream, dwDataSize, streamDir, bIsSilence);
	}
}

#endif

//********************************************************************
//* Construction: Parent class
//********************************************************************
CTapiObj::CTapiObj() :
 m_hLine(NULL)
 , m_hCall(NULL)
{
	m_appName = THEAPP_NAME;
	// This application is going to use TAPI 2.1
	// For more information about versionning look at MSDN
	m_version = TAPI_CURRENT_VERSION;
	// We are going to place a communication of the type voice
	m_mediaMode = LINEMEDIAMODE_INTERACTIVEVOICE;

	// To receive device capabilities info.
	m_pLineDevCaps = NULL;

	//  To keep the ID of the current device, it can be used later to
	//  to open other devices, in a multiline application. Line class
	//  Starts searching ID's greater than this ID,  So after Finding
	// the first line ID, you can give this 'that value +1' to search
	// for the next device.

	m_ID = 0;
	// The line has not been initialized yet
	m_bInitialized = FALSE;

	// The line has not been opened yet
	m_bOpened = FALSE;

	// Call virtual clear function
	ResetTapiCallData();

	// TAPI variables
	m_hLine = NULL;
	m_hCall = NULL;
	m_dwApiVersion = 0;

	m_strDeviceName = _T("");
	m_dwDeviceID = 0;

	m_stCurrentStatus = ST_NONE;
	memset(&m_extID, NULL, sizeof(LINEEXTENSIONID));
}

//********************************************************************
//* Destruction: Parent class
//********************************************************************
CTapiObj::~CTapiObj()
{
	ShutDown();
}

//********************************************************************
//* Initialize the object data
//********************************************************************
void CTapiObj::ResetTapiCallData()
{
	m_sANI = _T("");
	m_sDNIS = _T("");
	m_sCallerName = _T("");
	m_sCalledName = _T("");
}

//********************************************************************
//* Construction: Exception class, to be thrown!
//********************************************************************
CTapiObj::TEx::TEx(int id, CString res, CString dt, CString tm)
{
	code=id;
	result=res;
	date=dt;
	time=tm;
}

//********************************************************************
//* Misplaced function!
//********************************************************************
void CTapiObj::ThrowErr(int id, CString res)
{
	CTime	tm;
	tm=tm.GetCurrentTime();
	CString stdt,sttm;

	stdt.Format(_T("%d/%d/%d"),tm.GetYear(),tm.GetMonth(),tm.GetDay());
	sttm.Format(_T("%d:%d:%d"),tm.GetHour(),tm.GetMinute(),tm.GetSecond());

	WriteTrace(TraceError, "TapiObj Error - IVR Channel: %d id: %d, res: %s", GetDeviceID(), id, res);

	throw(CTapiObj::TEx(id,res,stdt,sttm));
}

UINT CTapiObj::GetLineID()
{
	return m_ID;
}

DWORD CTapiObj::GetLineHandle()
{
	return (DWORD)m_hLine;
}

DWORD CTapiObj::GetCallHandle()
{
	return (DWORD)m_hCall;
}

void CTapiObj::SetCallHandle(HCALL hNewCall)
{
	m_hCall = hNewCall;
}

CString CTapiObj::GetDeviceName()
{
	return m_strDeviceName;
}

void CTapiObj::SetDeviceName(LPCSTR lpName)
{
	if( m_strDeviceName.CompareNoCase(lpName) != 0 )
		m_strDeviceName = lpName;
}

DWORD CTapiObj::GetDeviceID()
{
	return m_dwDeviceID;
}

void CTapiObj::SetDeviceID(DWORD dwID)
{
	if( m_dwDeviceID != dwID )
		m_dwDeviceID = dwID;
}

STATUS CTapiObj::GetLineStatus()
{
	return m_stCurrentStatus;
}

void CTapiObj::SetLineStatus(STATUS f_stST, LPARAM f_Refer)
{
	if( m_stCurrentStatus != f_stST )
	{
		// Sun added 2010-07-16
		WriteTrace(TraceDetail, _T("TapiObj.cpp, IVR Channel: %d Line:%d status changed from %d to %d"), 
			GetDeviceID(), m_ID, m_stCurrentStatus, f_stST);

		m_stCurrentStatus = f_stST;

		if( gb_lMngrThread )
		{
			//----------------------------------------------------
			// Sun replaced 2010-08-02 [A]
			/// From
			/*
			// Sun updated 2010-07-20
			BOOL lv_blnSend = FALSE;
			int lv_nTimer = 10;		// 数值越大成功率越高
			while( lv_nTimer > 0 )
			{
				lv_blnSend = PostThreadMessage(gb_lMngrThread, WM_TAPI_STATUS_CHNGD, (WPARAM)this, f_Refer);
				if( lv_blnSend )
					break;
				lv_nTimer--;
				Sleep(10);
			}
			if( !lv_blnSend )
			{
				WriteTrace(TraceWarn, "TapiObj.cpp, IVR Channel: %d Line:%d failed to post WM_TAPI_STATUS_CHNGD msg", 
					GetDeviceID(), m_ID);
			}
			*/
			/// To
			if( (f_stST == ST_IBBUSY || f_stST == ST_OBBUSY) && f_Refer > 0 )
			{
				CIVRChannel *pChannel = (CIVRChannel *)f_Refer;
				WriteTrace(TraceDebug, "IVR Channel: %d received TapiRing... Ring..", pChannel->GetLineNO());
				pChannel->NF_CallIn();
			}
			//----------------------------------------------------
		}
	}
}

BOOL CTapiObj::IsInitialized()
{
	return m_bInitialized;
}

BOOL CTapiObj::IsOpened()
{
	return m_bOpened;
}

//********************************************************************
//* Initialize the line
//********************************************************************
long CTapiObj::Init(UINT nID)
{
	long ret = -1;
	CString sr;

	SetLineStatus(ST_INIT);

	// Time to negotiate! for a good device
	LINEEXTENSIONID		lv_extID;
	memset(&lv_extID,NULL,sizeof(LINEEXTENSIONID));
	m_ID = nID;
	ret = lineNegotiateAPIVersion(
		gb_hLineApp,
		m_ID,
		0x00010003,
		0x00030000,
		&m_dwApiVersion,
		&m_extID );
	if( ret !=0 )
	{
		sr.Format(_T("%d"),ret);
		ThrowErr(ret, _T("TapiObj.cpp, Init(), lineNegotiateAPIVersion(), ret=") + sr);
	}
	else
	{
		// Check device capabilities
		ret = GetDevCaps();
		if( ret != 0 )
		{
			sr.Format(_T("%d"), ret);
			ThrowErr(ret,_T("TapiObj.cpp, Init(), GetDevCaps(), ret=") + sr);
		}
	}

	if( ret != 0 )
		SetLineStatus(ST_FAILED);
	else
		m_bInitialized = TRUE;

	return ret;
}

//********************************************************************
//* Asks TAPI for device capabilities
//********************************************************************
long CTapiObj::GetDevCaps()
{
	long ret = -1;
	int  patience = 0;
	DWORD dwRealSize = 0;

	// Free the memory that possibly alocated from last calls
	if( m_pLineDevCaps )
	{
		free(m_pLineDevCaps);
		m_pLineDevCaps = NULL;
	}

	// Try to alocate minimum possible memory
	m_pLineDevCaps = (LINEDEVCAPS*) calloc(1, sizeof(LINEDEVCAPS));
	if( !m_pLineDevCaps )
		return IDS_LINEERR_NOMEM;
	
	// Clean memory
	memset(m_pLineDevCaps, 0x00, sizeof(LINEDEVCAPS));
	m_pLineDevCaps->dwTotalSize = sizeof(LINEDEVCAPS);

	// Run a loop untill enough memory alocated
	do
	{
		ret = lineGetDevCaps(
			gb_hLineApp,
			m_ID,
			m_dwApiVersion,
			0,
			m_pLineDevCaps);

		// Check memory size and realocate, if nessesery
		dwRealSize = m_pLineDevCaps->dwNeededSize;
		if( m_pLineDevCaps->dwTotalSize < dwRealSize )
		{
			free(m_pLineDevCaps);
			m_pLineDevCaps = NULL;
			
			// Try to alocate minimum posible memory
			m_pLineDevCaps = (LINEDEVCAPS*) calloc(1, dwRealSize);
			if( !m_pLineDevCaps )
				return IDS_LINEERR_NOMEM;

			// Clean memory
			memset(m_pLineDevCaps, 0x00, dwRealSize);
			m_pLineDevCaps->dwTotalSize = dwRealSize;
		} 
		else
			break;

		// We'll not encounter unlimited loops
		if( ++patience > 1000 )
		{
			if( m_pLineDevCaps)
				free(m_pLineDevCaps);
			ThrowErr(3, _T("TapiObj.cpp, GetDevCaps(), Recovered from unlimited loop"));
			break;
		};

	} while( true );

	return ret;
}

//********************************************************************
//* Openning the line
//********************************************************************
long CTapiObj::Open()
{
	long ret = -1;

	// Inform parent about a open process start
	SetLineStatus(ST_OPEN);

	// If line has not been initialized yet
	if( !m_bInitialized )
		return ret;

	// Open line, another call to TAPI
	ret = lineOpen(gb_hLineApp,
		m_ID,
		&m_hLine,
		m_dwApiVersion,
		0, 0, LINECALLPRIVILEGE_OWNER,
		m_mediaMode,
		NULL);

	// Check for errors
	if( ret !=0 )
	{
		// LINEERR_RESOURCEUNAVAIL
		WriteTrace(TraceError, _T("TapiObj.cpp, IVR Channel: %d Line:%d Open(), lineOpen return error: 0x%x"), GetDeviceID(), m_ID, ret);
		return ret;
	}

	// I want to receive all messages and events appropriate to me!
	ret = lineSetStatusMessages(
		m_hLine,
		m_pLineDevCaps->dwLineStates,
		0);

	if( ret != 0 )
	{
		WriteTrace(TraceError, _T("TapiObj.cpp, IVR Channel: %d Line:%d Open(), lineSetStatusMessages() return error: 0x%x"), GetDeviceID(), m_ID, ret);
		return ret;
	}

	// Well! We are ready now, everythings OK
	m_bOpened = TRUE;
	SetLineStatus(ST_READY);

	return ret;
}

// Sun added 2008-05-29
//********************************************************************
//* 设备主动自检以判断设备是否有错
//********************************************************************
long CTapiObj::ServiceSelfCheck()
{
	long nRet = 0;

	// 通过调用lineGetDevCaps确认设备正常
	/*
	LPLINEDEVCAPS pDev = new LINEDEVCAPS;
	pDev->dwTotalSize = sizeof(LINEDEVCAPS);
	nRet = lineGetDevCaps(
		gb_hLineApp,
		m_ID,
		m_dwApiVersion,
		0,
		pDev);
	delete pDev;
	if( nRet < 0 )
		return nRet;
	*/

	try
	{
		// 通过调用lineGetLineDevStatus确认设备正常
		LPLINEDEVSTATUS pLineDevStatue = new LINEDEVSTATUS;
		pLineDevStatue->dwTotalSize = sizeof(LINEDEVSTATUS);
		nRet = lineGetLineDevStatus(m_hLine, pLineDevStatue);

#ifndef NDEBUG

		if( nRet == 0 )
		{
			WriteTrace(TraceDetail, "IVR Channel: %d Tapi Line Device(%s) Status : Connected(%d), InService(%d), Locked(%d), MsgWait(%d)",
				GetDeviceID(), m_strDeviceName, 
				pLineDevStatue->dwDevStatusFlags & LINEDEVSTATUSFLAGS_CONNECTED,
				pLineDevStatue->dwDevStatusFlags & LINEDEVSTATUSFLAGS_INSERVICE,
				pLineDevStatue->dwDevStatusFlags & LINEDEVSTATUSFLAGS_LOCKED,
				pLineDevStatue->dwDevStatusFlags & LINEDEVSTATUSFLAGS_MSGWAIT);
		}

#endif

		delete pLineDevStatue;
	}
	catch(...)
	{
	}

	return nRet;
}

// Sun added 2008-05-23
//********************************************************************
//* 重新枚举所有设备，试图查找因各种原因离线的设备
//********************************************************************
long CTapiObj::SearchMeOnLost()
{
	long nRet;
	DWORD	dwSize;
	CString sLineName;
	DWORD dwChannelCnt = 400;

	try
	{
		for( DWORD i = 0;i < dwChannelCnt; i++ )
		{
			LINEEXTENSIONID lineExten;
			ZeroMemory(&lineExten, sizeof(lineExten));
			DWORD dwApiVer=0;
			nRet = lineNegotiateAPIVersion(gb_hLineApp, i, 0x00010003, 0x00030000, &dwApiVer, &lineExten);
			if( nRet == 0 )
			{
				LPLINEDEVCAPS pDev = new LINEDEVCAPS;
				pDev->dwTotalSize = sizeof(LINEDEVCAPS);
				nRet = lineGetDevCaps(gb_hLineApp, i, dwApiVer, 0, pDev);
				if( pDev->dwNeededSize > pDev->dwTotalSize )
				{
					dwSize = pDev->dwNeededSize;
					delete pDev;
					pDev = (LPLINEDEVCAPS)new BYTE[dwSize];
					pDev->dwTotalSize = dwSize;				
					nRet = lineGetDevCaps(gb_hLineApp, i, dwApiVer, 0, pDev);
				}
				if( nRet == 0 )
				{
					sLineName = (LPCTSTR)((LPBYTE)pDev + pDev->dwLineNameOffset);
					//WriteTrace(TraceDetail, "Detail - IVR Channel: %d Enum get TAPI line: %d - %s, media mode: 0x%08X", GetDeviceID(), i, sLineName, pDev->dwMediaModes);

					// 只采用Cisco CTI Port设备
					if( (pDev->dwMediaModes & LINEMEDIAMODE_INTERACTIVEVOICE) == LINEMEDIAMODE_INTERACTIVEVOICE )
					{
						if( m_strDeviceName.CompareNoCase(sLineName) == 0 )
						{
							WriteTrace(TraceWarn, "TapiObj.cpp, IVR Channel: %d Line:%d SearchMeOnLost(), found new lineid %d", GetDeviceID(), m_ID, i);
							m_ID = i;

							delete pDev;
							return 0;
						}
					}
				}
				delete pDev;
			}
		}
	}
	catch(...)
	{
	}

	return -1;
}

//********************************************************************
//* Close the line
//********************************************************************
long CTapiObj::Close()
{
	long ret = -1;

	// Notify parent of a restart event
	SetLineStatus(ST_SHUTDN);

	if( m_bOpened )
	{
		// Drop the line
		DropAllCalls();

		// Close the line
		if( m_hLine )
		{
			ret = lineClose(m_hLine);
			m_hLine = NULL;

			// Check for errors
			if( ret !=0 )
				WriteTrace(TraceError, _T("TapiObj.cpp, IVR Group: %d Line:%d Close(), lineClose return error: 0x%x"), GetDeviceID(), m_ID, ret);
		}

		// TAPI cleanup
		DeallocateAllCalls();

		m_bInitialized = TRUE;
		m_bOpened = FALSE;
	}

	return ret;
}

//********************************************************************
//* Restart the line
//********************************************************************
// -1	- 关闭失败，且不用再打开
// 0	- 正常
// 1	- 延后再打开
// 5	- 需要重新初始化
long CTapiObj::Restart()
{
	// Close First
	long lv_ret = Close();

	if( lv_ret >= 0 )
	{
		// Notify parent of a restart event
		SetLineStatus(ST_RESTART);

		// Sleep 100 more mili seconds, to make sure hardware is ready
		Sleep(200);
	}
	else if( lv_ret == LINEERR_RESOURCEUNAVAIL || lv_ret == LINEERR_NOMEM )
		return -1;

	// Re open the line, Sun updated 2008-05-19
	lv_ret = Open();
	if( lv_ret == LINEERR_RESOURCEUNAVAIL )
	{
		// Should open later
		WriteTrace(TraceWarn, "Warn - IVR Channel: %d TapiLine: %d can't reopen for resource unavailable now, should try it later.", GetDeviceID(), m_ID);
		return 1;
	}
	else if( lv_ret == LINEERR_UNINITIALIZED || lv_ret == LINEERR_REINIT )
	{
		// Should reinitialze, this must process very carefully
		WriteTrace(TraceWarn, "Warn - IVR Channel: %d TapiLine: %d can't reopen, should reinitialize the channel.", GetDeviceID(), m_ID);
		return 5;
	}
	else if( lv_ret == LINEERR_NODEVICE )
	{
		SearchMeOnLost();
	}
	// Sun added 2012-08-21
	else if( lv_ret == LINEERR_INVALAPPHANDLE )
	{
		// Should reinitialze, this must process very carefully
		WriteTrace(TraceWarn, "Warn - IVR Channel: %d TapiLine: %d can't reopen because app handle invalide, should reinitialize the channel.", GetDeviceID(), m_ID);
		return 5;
	}

	return 0;
}

//********************************************************************
//* Shutting down the line
//********************************************************************
void CTapiObj::ShutDown()
{
	// Notify parent of a shutdown event
	SetLineStatus(ST_SHUTDN);

	if( m_bInitialized )
	{
		// Drop the line
		DropAllCalls();

		// Close the line
		if( m_hLine )
		{
			lineClose(m_hLine);
			m_hLine = NULL;
		}

		// TAPI cleanup
		DeallocateAllCalls();

		m_bInitialized = FALSE;
	}

	if( m_pLineDevCaps )
	{
		free( m_pLineDevCaps );
	}
	
	m_pLineDevCaps = NULL;
}


//********************************************************************
//* CTapiRoutePoint class
//********************************************************************

//********************************************************************
//*Constructor: CTapiLine class
//********************************************************************
CTapiRoutePoint::CTapiRoutePoint()
{
	// We are going to place a communication of the type voice
	m_mediaMode = LINEMEDIAMODE_INTERACTIVEVOICE;

	for( int lv_loop = 0; lv_loop < DEF_TAPI_MAX_CALLLIST; lv_loop++ )
	{
		m_hCallList[lv_loop] = 0; 
	}

	// Sun added 2010-07-31
	m_nConcurrentCalls = 0;
}

//********************************************************************
//* Destructor: CTapiLine class
//********************************************************************
CTapiRoutePoint::~CTapiRoutePoint()
{
}

int CTapiRoutePoint::FindCallListMember(DWORD dwCallHandle)
{
	for( int lv_loop = 0; lv_loop < gb_nRoutePointMaxConcurrentCall; lv_loop++ )
	{
		if( m_hCallList[lv_loop] == dwCallHandle )
			return lv_loop;
	}

	return -1;
}

int CTapiRoutePoint::SetCallListMember(DWORD dwCallHandle)
{
	int lv_ret = FindCallListMember(dwCallHandle);
	if( lv_ret >= 0 )
		return lv_ret;

	for( int lv_loop = 0; lv_loop < gb_nRoutePointMaxConcurrentCall; lv_loop++ )
	{
		if( m_hCallList[lv_loop] == 0 )
		{
			m_hCallList[lv_loop] = dwCallHandle;
			
			// Sun added 2010-07-31
			m_nConcurrentCalls++;
			WriteTrace(TraceDetail, "IVR Group: %d id: %d plus call, count = %d", GetDeviceID(), m_ID, m_nConcurrentCalls);

			return lv_loop;
		}
	}
	
	return -1;
}

int CTapiRoutePoint::ClearCallListMember(DWORD dwCallHandle)
{
	int lv_ret = -1;

	for( int lv_loop = 0; lv_loop < gb_nRoutePointMaxConcurrentCall; lv_loop++ )
	{
		if( m_hCallList[lv_loop] == dwCallHandle )
		{
			m_hCallList[lv_loop] = 0;
			lv_ret = lv_loop;

			// Sun added 2010-07-31
			m_nConcurrentCalls--;
			WriteTrace(TraceDetail, "IVR Group: %d id: %d minus call, count = %d", GetDeviceID(), m_ID, m_nConcurrentCalls);
		}
	}

	return lv_ret;
}

//********************************************************************
//* CTapiRoutePoint the routepoint data
//********************************************************************
void CTapiRoutePoint::ResetTapiCallData()
{
	CTapiObj::ResetTapiCallData();
}

//********************************************************************
//* Drop all calls on this device
//********************************************************************
void CTapiRoutePoint::DropAllCalls()
{
	// Drop the line
	if( m_hCall )
		lineDrop(m_hCall, NULL, 0);

	// Sun added 2010-07-20
	for( int lv_loop = 0; lv_loop < DEF_TAPI_MAX_CALLLIST; lv_loop++ )
	{
		m_hCallList[lv_loop] = 0; 
	}
}

void CTapiRoutePoint::DeallocateAllCalls()
{
	// TAPI cleanup
	if( m_hCall )
	{
		lineDeallocateCall(m_hCall);
		m_hCall = NULL;
	}
}

long CTapiRoutePoint::DeallocateCall(DWORD dwCallHandle)
{
	HCALL lv_hCall;

	if( dwCallHandle == 0 )
		lv_hCall = m_hCall;
	else
		lv_hCall = (HCALL)dwCallHandle;

	if( lv_hCall )
	{
		lineDeallocateCall(lv_hCall);

		if( lv_hCall == m_hCall )
			m_hCall = NULL;
	}

	return 0;
}

long CTapiRoutePoint::DropCall(DWORD dwCallHandle)
{
	long ret = -1;

	HCALL lv_hCall;

	if( dwCallHandle == 0 )
		lv_hCall = m_hCall;
	else
		lv_hCall = (HCALL)dwCallHandle;

	// Drop the line
	if( lv_hCall )
	{
		ret = lineDrop(lv_hCall, NULL, 0);
		if( ret == 0 )
		{
			lineDeallocateCall(lv_hCall);
			if( lv_hCall == m_hCall )
				m_hCall = NULL;
			ResetTapiCallData();
		}
	}

	return ret;
}

long CTapiRoutePoint::RedirectCall(DWORD dwCallHandle, LPCSTR lpszDestAddress)
{
	long ret = -1;

	HCALL lv_hCall;

	if( dwCallHandle == 0 )
		lv_hCall = m_hCall;
	else
		lv_hCall = (HCALL)dwCallHandle;

	// Drop the line
	if( lv_hCall )
	{
		ret = lineRedirect(lv_hCall, lpszDestAddress, 0);
	}

	return ret;
}


//********************************************************************
//* CTapiLine class
//********************************************************************

//********************************************************************
//*Constructor: CTapiLine class
//********************************************************************
CTapiLine::CTapiLine()
{
	// We are going to place a communication of the type voice
	m_mediaMode = LINEMEDIAMODE_INTERACTIVEVOICE | LINEMEDIAMODE_AUTOMATEDVOICE;

	// TAPI variables
	m_hConsultCall = NULL;
	m_hConferenceCall = NULL;
	m_intRingCnt = 1;
	m_nKeyPtr = 0;

	m_dwDistributeReuestID = 0;
	m_PlaySyncEvent.Create();
	m_CallSyncEvent.Create();
}

//********************************************************************
//* Destructor: CTapiLine class
//********************************************************************
CTapiLine::~CTapiLine()
{
}

//********************************************************************
//* Initialize the line data
//********************************************************************
void CTapiLine::ResetTapiCallData()
{
	CTapiObj::ResetTapiCallData();

	SetTerminateCode(VOICE_TM_Dummy);
	memset(m_tpt, 0x00, sizeof(DV_TPT) * DEF_TAPI_MAX_TPTCNT);
	ClearKeyBuffer();
}

void CTapiLine::ClearKeyBuffer()
{
	m_keyLock.Enter();
	m_nKeyPtr = 0;
	m_nKeyReadPtr = 0;
	m_dwLastKeyTick = 0;
	m_dwTotalKeyTick = 0;
	m_dwIntervalTick = 0;
	memset(m_sKeyBuffer, 0x00, DEF_TAPI_MAX_KEYBUFFER);
	m_keyLock.Leave();
}

int CTapiLine::GetKeyBufferLength()
{
	int lv_nLength = 0;

	m_keyLock.Enter();
	if( m_nKeyPtr > m_nKeyReadPtr )
		lv_nLength = m_nKeyPtr - m_nKeyReadPtr;
	else if( m_nKeyPtr < m_nKeyReadPtr )
		lv_nLength = DEF_TAPI_MAX_KEYBUFFER - m_nKeyReadPtr + m_nKeyPtr;
	m_keyLock.Leave();

	return lv_nLength;
}

int CTapiLine::PushKey(char chKey)
{
	m_keyLock.Enter();
	if( (m_nKeyPtr + 1) % DEF_TAPI_MAX_KEYBUFFER == m_nKeyReadPtr )
	{
		WriteTrace(TraceWarn, "IVR Channel: %d TAPI Key Buffer is Full!", GetDeviceID());		
	}
	else
	{
		m_sKeyBuffer[m_nKeyPtr] = chKey;
		m_nKeyPtr++;
		if( m_nKeyPtr >= DEF_TAPI_MAX_KEYBUFFER )
			m_nKeyPtr = 0;
	}
	m_keyLock.Leave();
	m_dwLastKeyTick = GetTickCount();

	return m_nKeyPtr;
}

int CTapiLine::PopKey(char *chKey, int nLength)
{
	int lv_nLen = 0, lv_loop;

	lv_nLen = __min(GetKeyBufferLength(), nLength);
	
	m_keyLock.Enter();
	try
	{
		if( lv_nLen > 0 )
		{
			for( lv_loop = 0; lv_loop < lv_nLen; lv_loop++ )
			{
				chKey[lv_loop] = m_sKeyBuffer[m_nKeyReadPtr];
				m_nKeyReadPtr++;
				if( m_nKeyReadPtr >= DEF_TAPI_MAX_KEYBUFFER )
					m_nKeyReadPtr = 0;
			}
		}
	}
	catch(...)
	{
		WriteTrace(TraceError, "Error - IVR Channel: %d CTapiLine::PopKey(), key buffer is too small to hold the string.", GetDeviceID());
		lv_nLen = lv_loop;
	}
	m_keyLock.Leave();

	return lv_nLen;
}

char CTapiLine::GetLastKey()
{
	char lv_chKey = 0;

	if( GetKeyBufferLength() > 0 )
	{
		m_keyLock.Enter();
		if( m_nKeyPtr == 0 )
			lv_chKey = m_sKeyBuffer[DEF_TAPI_MAX_KEYBUFFER - 1];
		else
			lv_chKey = m_sKeyBuffer[m_nKeyPtr - 1];
		m_keyLock.Leave();
	}

	return lv_chKey;
}

char CTapiLine::GetFirstKey()
{
	char lv_chKey = 0;

	if( GetKeyBufferLength() > 0 )
	{
		m_keyLock.Enter();
		lv_chKey = m_sKeyBuffer[m_nKeyReadPtr];
		m_keyLock.Leave();
	}

	return lv_chKey;
}

// nIndex - 0 based position
char CTapiLine::GetKeyIndex(int nIndex)
{
	char lv_chKey = 0;
	int lv_nPos, lv_nLen;

	lv_nLen = GetKeyBufferLength();
	if( lv_nLen > 0 && lv_nLen > nIndex )
	{
		m_keyLock.Enter();
		lv_nPos = (m_nKeyReadPtr + nIndex) % DEF_TAPI_MAX_KEYBUFFER;
		lv_chKey = m_sKeyBuffer[lv_nPos];
		m_keyLock.Leave();
	}

	return lv_chKey;
}

DWORD CTapiLine::GetConsultCallHandle()
{
	return (DWORD)m_hConsultCall;
}

void CTapiLine::SetConsultCallHandle(HCALL hNewCall)
{
	m_hConsultCall = hNewCall;
}

DWORD CTapiLine::GetConfCallHandle()
{
	return (DWORD)m_hConferenceCall;
}

void CTapiLine::SetConfCallHandle(HCALL hNewCall)
{
	m_hConferenceCall = hNewCall;
	WriteTrace(TraceDetail, "TapiObj.cpp, IVR Channel: %d Line:%d SetConfCallHandle(0x%x)", GetDeviceID(), m_ID, m_hConferenceCall);
}


//********************************************************************
//* Returns current TAPI request ID
//********************************************************************
DWORD CTapiLine::GetCurrentRequestID()
{
	return m_CallSyncEvent.m_nEventID;
}

//********************************************************************
//* Start waiting ack for call reuest
//********************************************************************
void CTapiLine::AddCurrentRequest(DWORD dwEventID)
{
	m_CallSyncEvent.m_nTag = 0;
	m_CallSyncEvent.m_nEventID = dwEventID;
	g_thProcessTAPIEvents.AddRequest(dwEventID, this);
	m_CallSyncEvent.Reset();
}

bool CTapiLine::StartWaitRequest(DWORD dwTimeout)
{
	return( m_CallSyncEvent.Wait(dwTimeout) );
}

//********************************************************************
//* Stop waiting ack for call reuest
//********************************************************************
void CTapiLine::StopWaitCurrentRequest(DWORD dwResult)
{
	g_thProcessTAPIEvents.RemoveRequest(m_CallSyncEvent.m_nEventID);

	m_CallSyncEvent.m_nTag = dwResult;
	m_CallSyncEvent.m_nEventID = 0;
	m_CallSyncEvent.Set();
}

//********************************************************************
//* 设置路由点呼叫分配请求
//********************************************************************
void CTapiLine::SetDistributeRequest(DWORD dwRequestID)
{
	// Remove previous one
	if( m_dwDistributeReuestID > 0 )
	{
		g_thProcessTAPIEvents.RemoveRequest(m_dwDistributeReuestID);
	}

	// Add new one
	if( dwRequestID > 0 )
	{
		m_dwDistributeReuestID = dwRequestID;
		g_thProcessTAPIEvents.AddRequest(dwRequestID, this);
	}
}

//********************************************************************
//* Returns wave device ID binding with the line
//********************************************************************
DWORD CTapiLine::GetLineWavOutDevID()
{
	return m_dwWaveOutID;
}

DWORD CTapiLine::GetLineWavInDevID()
{
	return m_dwWaveInID;
}

//********************************************************************
//* Initialize the line, (~ 200 lines)
//********************************************************************
long CTapiLine::Init(UINT nID, DWORD dwDeviceID)
{
	// Line initialization is done.
	m_PlaySyncEvent.m_nTag = nID;
	m_CallSyncEvent.m_nTag = nID;

	// Sun added 2009-01-09
	CTapiObj::SetDeviceID(dwDeviceID);

	return( CTapiObj::Init(nID) );
}

//********************************************************************
//* Set number of rings to pickup the line after
//********************************************************************
void CTapiLine::SetRingCount(int nRingCnt)
{
	if( m_intRingCnt != nRingCnt )
	{
		m_intRingCnt = nRingCnt;
	
		// Pickup after ? rings
		if( m_hLine )
			lineSetNumRings(m_hLine, 0, m_intRingCnt);
	}
}

//********************************************************************
//* Openning the line
//********************************************************************
long CTapiLine::Open()
{
	long ret = CTapiObj::Open();

	if( ret == 0 )
	{
		// Pickup after ? rings
		lineSetNumRings(m_hLine, 0, m_intRingCnt);

		// Get the binding wave or RTP device id
		ret = GetWaveID();
		if( ret != 0 )
		{
			// Resume previous status
			m_bOpened = FALSE;
			SetLineStatus(ST_OPEN);

			WriteTrace(TraceError, _T("TapiObj.cpp, IVR Channel: %d Line:%d Open(), GetWaveID() return error: 0x%x"), GetDeviceID(), m_ID, ret);
			return ret;
		}
		//-------------------------------------
		// Sun added 2010-07-20 [A]
		else
		{
			if( m_dwWaveOutID == 0 || m_dwWaveInID == 0 )
			{
				// Resume previous status
				m_bOpened = FALSE;
				SetLineStatus(ST_OPEN);
				WriteTrace(TraceError, _T("TapiObj.cpp, IVR Channel: %d Line:%d Open(), can't GetWaveID() WaveOutID=%d, WaveInID=%d"), 
					GetDeviceID(), m_ID, m_dwWaveOutID, m_dwWaveInID);

				return 1;
			}
		}
		//-------------------------------------

#ifdef CUCM_MEDIA
		// Sun added 2013-03-06
		/// Subscribe for media stream events for relevant devices using Cisco lineDevSpecific extension
		CCiscoLineDevSpecificSetStatusMsgs lv_StatusMsgs;

		lv_StatusMsgs.m_DevSpecificStatusMsgsFlag = DEVSPECIFIC_MEDIA_STREAM;			// CALL_DEVSPECIFIC_RTP_EVENTS
		long lv_lngRet = lineDevSpecific(m_hLine, 0, 0, lv_StatusMsgs.lpParams(), lv_StatusMsgs.dwSize());
		if( lv_lngRet < 0 )
		{
			WriteTrace(TraceError, _T("TapiObj.cpp, IVR Channel: %d Line:%d Open(), lineDevSpecific(DEVSPECIFIC_MEDIA_STREAM) return error: 0x%x"), GetDeviceID(), m_ID, lv_lngRet);
		}

		//-------------------------------------
		// Sun added 2013-04-12
		/// 打开Media通道，
		//// 不同于Wave Driver的是，这里是事先打开；Wave Driver是使用时才打开。
		PRTPENDPOINTCALLBACK lv_pCallback = MediaDriverEpCallback;
		// ToApp = 0x01,         //In,
		// ToNwk = 0x02,         //Out
		// Both  = 0x03          //In/Out
		/*m_hMediaEpOut = EpOpenById(m_dwWaveOutID, ToNwk, lv_pCallback);
		if( m_hMediaEpOut == NULL )
		{
			WriteTrace(TraceError, _T("TapiObj.cpp, IVR Channel: %d Line:%d Open(), EpOpenById(CISCO_OUT: %d) return error: 0x%x"), 
				GetDeviceID(), m_ID, m_dwWaveOutID, EpApiGetLastError());
		}
		else
		{
			WriteTrace(TraceInfo, _T("TapiObj.cpp, IVR Channel: %d Line:%d Open(), EpOpenById(CISCO_OUT: %d) succeeded, Endpoint handle = 0x%x"), 
				GetDeviceID(), m_ID, m_dwWaveOutID, m_hMediaEpOut);
		}*/

		m_hMediaEpIn = EpOpenById(m_dwWaveInID, ToApp, lv_pCallback);
		if( m_hMediaEpIn == NULL )
		{
			WriteTrace(TraceError, _T("TapiObj.cpp, IVR Channel: %d Line:%d Open(), EpOpenById(CISCO_IN: %d) return error: 0x%x"), 
				GetDeviceID(), m_ID, m_dwWaveInID, EpApiGetLastError());
		}
		else
		{
			WriteTrace(TraceInfo, _T("TapiObj.cpp, IVR Channel: %d Line:%d Open(), EpOpenById(CISCO_IN: %d) succeeded"), 
				GetDeviceID(), m_ID, m_dwWaveInID);
		}
		//-------------------------------------

#endif

	}

	return ret;
}

//********************************************************************
//* Close the line
//********************************************************************
// Sun added 2013-04-12
BOOL CTapiLine::EndPointOpen()
{
#ifdef CUCM_MEDIA

	PRTPENDPOINTCALLBACK lv_pCallback = MediaDriverEpCallback;
	// ToApp = 0x01,         //In,
	// ToNwk = 0x02,         //Out
	// Both  = 0x03          //In/Out
	m_hMediaEpOut = EpOpenById(m_dwWaveOutID, Both, lv_pCallback);
	if( m_hMediaEpOut == NULL )
	{
		WriteTrace(TraceError, _T("TapiObj.cpp, IVR Channel: %d Line:%d Open(), EpOpenById(CISCO_OUT: %d) return error: 0x%x"), 
			GetDeviceID(), m_ID, m_dwWaveOutID, EpApiGetLastError());
		return FALSE;
	}
	else
	{
		WriteTrace(TraceInfo, _T("TapiObj.cpp, IVR Channel: %d Line:%d Open(), EpOpenById(CISCO_OUT: %d) succeeded, Endpoint handle = 0x%x"), 
			GetDeviceID(), m_ID, m_dwWaveOutID, m_hMediaEpOut);
	}

#endif

	return TRUE;
}

//********************************************************************
//* Close the line
//********************************************************************
// Sun added 2013-04-12
long CTapiLine::Close()
{
#ifdef CUCM_MEDIA
	if( m_hMediaEpIn != NULL )
	{
		if( !EpClose(m_hMediaEpIn) )
		{
			WriteTrace(TraceError, _T("TapiObj.cpp, IVR Channel: %d Line:%d Close(), EpClose(CISCO_IN: %d) return error: 0x%x"), 
				GetDeviceID(), m_ID, m_dwWaveInID, EpApiGetLastError());
		}
		else
			m_hMediaEpIn = NULL;
	}

	if( m_hMediaEpOut != NULL )
	{
		if( !EpClose(m_hMediaEpOut) )
		{
			WriteTrace(TraceError, _T("TapiObj.cpp, IVR Channel: %d Line:%d Close(), EpClose(CISCO_OUT: %d) return error: 0x%x"), 
				GetDeviceID(), m_ID, m_dwWaveOutID, EpApiGetLastError());
		}
		else
			m_hMediaEpOut = NULL;
	}

	// Reset the flag anyway
	CWaveInDevice::m_bDevOpen = FALSE;
	CWaveOutDevice::m_bDevOpen = FALSE;

#endif

	return CTapiObj::Close();
}

//********************************************************************
//* Shutting down the line
//********************************************************************
void CTapiLine::ShutDown()
{
	if( m_bInitialized )
	{
		SetDistributeRequest(0);

		if( m_CallSyncEvent.m_nEventID > 0 )
			g_thProcessTAPIEvents.RemoveRequest(m_CallSyncEvent.m_nEventID);
	}

	CTapiObj::ShutDown();
	SetTerminateCode(VOICE_TM_NORMTERM);
}

//********************************************************************
//* Drop all calls on this device
//********************************************************************
void CTapiLine::DropAllCalls()
{
	// Drop the line
	if( m_hConferenceCall )
		lineDrop(m_hConferenceCall, NULL, 0);
	if( m_hConsultCall )
		lineDrop(m_hConsultCall, NULL, 0);
	if( m_hCall )
		lineDrop(m_hCall, NULL, 0);

}

void CTapiLine::DeallocateAllCalls()
{
	long ret=0;

	WriteTrace(TraceDetail, "TapiObj.cpp, IVR Channel: %d Line:%d calls DeallocateAllCalls(). Conf=0x%x, Cons=0x%x, Call=0x%x", GetDeviceID(), m_ID, m_hConferenceCall, m_hConsultCall, m_hCall);

	// TAPI cleanup
	if( m_hConferenceCall )
	{
		ret = lineDeallocateCall(m_hConferenceCall);
		if( ret < 0 )
			WriteTrace(TraceDebug, "TapiObj.cpp, IVR Channel: %d Line:%d lineDeallocateCall(m_hConferenceCall:0x%x) ret 0x%x", GetDeviceID(), m_ID, m_hConferenceCall, ret);

		m_hConferenceCall = NULL;
	}
	if( m_hConsultCall )
	{
		ret = lineDeallocateCall(m_hConsultCall);
		if( ret < 0 )
			WriteTrace(TraceDebug, "TapiObj.cpp, IVR Channel: %d Line:%d lineDeallocateCall(m_hConsultCall:0x%x) ret 0x%x", GetDeviceID(), m_ID, m_hConsultCall, ret);

		m_hConsultCall = NULL;
	}
	if( m_hCall )
	{
		ret = lineDeallocateCall(m_hCall);
		if( ret < 0 )
			WriteTrace(TraceDebug, "TapiObj.cpp, IVR Channel: %d Line:%d lineDeallocateCall(m_hCall:0x%x) ret 0x%x", GetDeviceID(), m_ID, m_hCall, ret);

		m_hCall = NULL;
	}
}

//********************************************************************
//* Tries to get the ID of the wave or RTP out/in device
//********************************************************************
long CTapiLine::GetWaveID()
{
	VARSTRING	*pVs;
	LONG		ret=0;
	DWORD		dwWaveDev;
	DWORD		dwSize;
	CString		strDevClass;
	int			patience;

	//---------------------------------------------------------------------------
	for( int lv_loop = 0; lv_loop < 2; lv_loop++ )
	{
		// allocate memory for structure
		pVs = (VARSTRING *) calloc (1, 1024);
		memset(pVs, 0x00, 1024);
		// set structure size
		pVs->dwTotalSize = 1024; //sizeof(VARSTRING);

		if( lv_loop == 0 )
		{
// Sun added 2013-03-06
#ifdef CUCM_MEDIA
			strDevClass = _T("ciscowave/out");
#else
			strDevClass = _T("wave/out");
#endif
		}
		else
		{
// Sun added 2013-03-06
#ifdef CUCM_MEDIA
			strDevClass = _T("ciscowave/in");
#else
			strDevClass = _T("wave/in");
#endif
		}

		patience = 0;
		do
		{
			// We have not more patience, stop the loop
			if( ++patience > 100)
			{
				WriteTrace(TraceError, _T("TapiObj.cpp, IVR Channel: %d Line:%d GetWaveID(), Recovered from unlimited loop"), GetDeviceID(), m_ID);
				return -1;
			}

			// Get wave/out ID
			ret = lineGetID(
				m_hLine, 
				0L, 
				0L, 
				LINECALLSELECT_LINE,		// LINECALLSELECT_CALL, LINECALLSELECT_LINE
				pVs, 
				(LPCSTR)strDevClass);

			// check errors
			if( ret ) 
			{
				free( pVs );
				WriteTrace(TraceError, _T("TapiObj.cpp, IVR Channel: %d Line:%d lineGetID error: 0x%x"), GetDeviceID(), m_ID, ret);
				return -1;
			}

			// reallocate and try again
			if( pVs->dwTotalSize < pVs->dwNeededSize )
			{
				dwSize = pVs->dwNeededSize;
				free( pVs );
				pVs = (VARSTRING *) calloc(1, dwSize);
				pVs->dwTotalSize = dwSize;
				continue;
			} // end if (need more space)
			break; // success 
		} while (TRUE);

		// copy wave id
		dwWaveDev = (DWORD) *((DWORD *)((LPSTR)pVs + pVs->dwStringOffset));
		free (pVs);

		if( lv_loop == 0 )
			m_dwWaveOutID = dwWaveDev;
		else
		{
			m_dwWaveInID = dwWaveDev;

			CCtrlWaveIn::SetMinDiskBuffer(0);
			CCtrlWaveIn::SetMaxDiskBuffer(640);
			CCtrlWaveIn::SetMaxSilenceTime(5);
			CCtrlWaveIn::SetActiveThreshold(500);
		}
	}

	return 0;
}

//------------------------------------------------------------------
// Call Control routines
//------------------------------------------------------------------
long CTapiLine::AcceptCall()
{
	long ret = -1;

	// Notes: The Cisco TSP does not support user-user information.
	if( m_hCall )
	{
		ret = lineAccept(m_hCall, NULL, 0);
		ClearKeyBuffer();
	}

	return ret;
}

long CTapiLine::AnswerCall()
{
	long ret = -1;

	// Notes: The Cisco TSP does not support user-user information.
	if( m_hCall )
		ret = lineAnswer(m_hCall, NULL, 0);

	return ret;
}

long CTapiLine::MonitorDigits(DWORD dwCallHandle)
{
	long ret = -1;
	HCALL lv_hCall;

	if( dwCallHandle == 0 )
		lv_hCall = m_hCall;
	else
		lv_hCall = (HCALL)dwCallHandle;

	if( lv_hCall )
	{
		ret = lineMonitorDigits(lv_hCall, LINEDIGITMODE_DTMF);
		if( ret )
			WriteTrace(TraceError, "TapiObj.cpp, IVR Channel: %d Line:%d Unable to listen for user tones (Call:0x%x), error: 0x%x", 
				GetDeviceID(), m_ID, lv_hCall, ret);
	}
	else
	{
		WriteTrace(TraceWarn, "TapiObj.cpp, IVR Channel: %d Line:%d - No Call to listen for user tones", 
			GetDeviceID(), m_ID);
	}


	return ret;
}

long CTapiLine::DeallocateCall(DWORD dwCallHandle)
{
	long ret = 0;
	HCALL lv_hCall;

	WriteTrace(TraceDetail, "TapiObj.cpp, IVR Channel: %d Line:%d calls DeallocateCall(0x%x). Conf=0x%x, Cons=0x%x, Call=0x%x", GetDeviceID(), m_ID, dwCallHandle, m_hConferenceCall, m_hConsultCall, m_hCall);

	if( dwCallHandle == 0 )
		lv_hCall = m_hCall;
	else
		lv_hCall = (HCALL)dwCallHandle;

	if( lv_hCall )
	{
		ret = lineDeallocateCall(lv_hCall);
		if( ret < 0 )
			WriteTrace(TraceDebug, "TapiObj.cpp, IVR Channel: %d Line:%d lineDeallocateCall(Call:0x%x) ret 0x%x", GetDeviceID(), m_ID, lv_hCall, ret);

		if( lv_hCall == m_hConferenceCall )
			m_hConferenceCall = NULL;

		if( lv_hCall == m_hConsultCall )
			m_hConsultCall = NULL;

		if( lv_hCall == m_hCall )
			m_hCall = NULL;
	}

	return 0;
}

// *****不能用*****
long CTapiLine::EnumCallsOnLine()
{
	long lv_nCalls = 0;
	long nRet = 0;
	DWORD dwSize;

	try
	{
		LPLINECALLLIST pCallList = new LINECALLLIST;
		pCallList->dwTotalSize = sizeof(LINECALLLIST);
		nRet = lineGetNewCalls(m_hLine, 0, LINECALLSELECT_LINE, pCallList);
		if( pCallList->dwNeededSize > pCallList->dwTotalSize )
		{
			dwSize = pCallList->dwNeededSize;
			delete pCallList;
			pCallList = (LPLINECALLLIST)new BYTE[dwSize];
			pCallList->dwTotalSize = dwSize;				
			nRet = lineGetNewCalls(m_hLine, 0, LINECALLSELECT_LINE, pCallList);
		}

		if( nRet == 0 )
		{
			lv_nCalls = pCallList->dwCallsNumEntries;
			for( long lv_loop = 0; lv_loop < lv_nCalls; lv_loop++ )
			{
				WriteTrace(TraceDebug, "IVR Channel: %d CallList: %d (0x%x)", GetDeviceID(), lv_loop+1, *((HCALL *)((LPBYTE)pCallList + pCallList->dwCallsOffset)+lv_loop) );
			}
		}
		delete pCallList;
	}
	catch(...)
	{
	}

	return lv_nCalls;
}

long CTapiLine::DropCall(DWORD dwCallHandle)
{
	long ret = -1;

	WriteTrace(TraceDetail, "TapiObj.cpp, IVR Channel: %d Line:%d calls DropCall(0x%x). Conf=0x%x, Cons=0x%x, Call=0x%x", GetDeviceID(), m_ID, dwCallHandle, m_hConferenceCall, m_hConsultCall, m_hCall);

	if( dwCallHandle == 0 )
	{
		// Drop the line
		if( m_hConferenceCall )
		{
			ret = lineDrop(m_hConferenceCall, NULL, 0);
			if( ret == 0 )
			{
				lineDeallocateCall(m_hConferenceCall);
				m_hConferenceCall = NULL;
			}
			else if( ret < 0 )
				WriteTrace(TraceDebug, "TapiObj.cpp, IVR Channel: %d Line:%d lineDrop(m_hConferenceCall:0x%x) ret 0x%x", GetDeviceID(), m_ID, m_hConferenceCall, ret);
		}

		if( m_hCall )
		{
			if( m_hConsultCall )
			{
				ret = lineDrop(m_hConsultCall, NULL, 0);
				//WriteTrace(TraceDebug, "AAAAAA TapiObj.cpp, IVR Channel: %d Line:%d lineDrop(m_hConsultCall:0x%x) ret 0x%x", GetDeviceID(), m_ID, m_hConsultCall, ret);
				if( ret == 0 )
				{
					lineDeallocateCall(m_hConsultCall);
					m_hConsultCall = NULL;
				}
				else if( ret < 0 )
					WriteTrace(TraceDebug, "TapiObj.cpp, IVR Channel: %d Line:%d lineDrop(m_hConsultCall:0x%x) ret 0x%x", GetDeviceID(), m_ID, m_hConsultCall, ret);
			}

			ret = lineDrop(m_hCall, NULL, 0);
			if( ret == 0 )
			{
				lineDeallocateCall(m_hCall);
				m_hCall = NULL;
				ResetTapiCallData();
			}
			else if( ret < 0 )
				WriteTrace(TraceDebug, "TapiObj.cpp, IVR Channel: %d Line:%d lineDrop(m_hCall:0x%x) ret 0x%x", GetDeviceID(), m_ID, m_hCall, ret);
		}
	}
	else
	{
		ret = lineDrop(dwCallHandle, NULL, 0);
		if( ret == 0 )
		{
			lineDeallocateCall(dwCallHandle);
		}
		else if( ret < 0 )
			WriteTrace(TraceDebug, "TapiObj.cpp, IVR Channel: %d Line:%d lineDrop(Call:0x%x) ret 0x%x", GetDeviceID(), m_ID, dwCallHandle, ret);
	}

	return ret;
}

long CTapiLine::MakeCall(LPCSTR lpszDigits, int nNoAnsTimeOut)
{
	long ret = -1;

	// Drop the line
	if( m_hLine )
	{
		LINECALLPARAMS lv_Param;
		memset(&lv_Param, 0x00, sizeof(lv_Param));
		lv_Param.dwTotalSize = sizeof(lv_Param);
		lv_Param.dwNoAnswerTimeout = (DWORD)nNoAnsTimeOut;
		ret = lineMakeCall(m_hLine, &m_hCall, lpszDigits, 0, &lv_Param);
	}

	return ret;
}

long CTapiLine::SingleStepTransfer(LPCSTR lpszDigits)
{
	long ret = -1;

	// Drop the line
	if( m_hCall )
		ret = lineBlindTransfer(m_hCall, lpszDigits, 0);

	return ret;
}

// Notes: 2 types of consultation transfer
/// 1. Hold -> MakeCall -> Complete Transfer
/// 2. Setup Transfer -> Dial -> Complete Transfer
/// Now we use the second type in IVRS
long CTapiLine::ConsultTransfer(LPCSTR lpszDigits, int nNoAnsTimeOut)
{
	long ret = -1;

	// Drop the line
	if( m_hCall )
	{
		LINECALLPARAMS lv_Param;
		memset(&lv_Param, 0x00, sizeof(lv_Param));
		lv_Param.dwTotalSize = sizeof(lv_Param);
		lv_Param.dwNoAnswerTimeout = (DWORD)nNoAnsTimeOut;

		// Sun added 2010-08-04
		CSingleLock lv_MySingleLock(&g_thProcessTAPIEvents.m_ReplyLock);
		lv_MySingleLock.Lock(2000);

		ret = lineSetupTransfer(m_hCall, &m_hConsultCall, &lv_Param);
		if( ret > 0 )
		{
			// Sun added 2010-08-04
			AddCurrentRequest((DWORD)ret);
			lv_MySingleLock.Unlock();

			// Setup transfer successfully
			if( StartWaitRequest() )
			{
				// Consultation OK
				ret = (long)m_CallSyncEvent.m_nTag;
				if( ret == 0 )
				{
					// Sun added 2010-08-04
					lv_MySingleLock.Lock(2000);

					ret = lineDial(m_hConsultCall, lpszDigits, 0);
					if( ret > 0 )
					{
						// Sun added 2010-08-04
						AddCurrentRequest((DWORD)ret);
						lv_MySingleLock.Unlock();

						if( StartWaitRequest() )
							ret = (long)m_CallSyncEvent.m_nTag;
						else
							ret = -1;
					}
				}
			}
			else
				ret = -1;
		}
	}

	return ret;
}

// Sun updated 2013-07-25 to support append externsion after dialing finished
// Used in reading work No. transfer
long CTapiLine::ConsultConference(LPCSTR lpszDigits, int nNoAnsTimeOut)
{
	long ret = -1;
	int	patience;

	// Sun added 2013-07-25
	char lv_sTemp[50];
	UC lv_loop, lv_subloop;
	CString lv_sPredial, lv_sPhoneNo, lv_sExtNo;
	TCHAR lv_char;
	int lv_nPreFlag, lv_nPauseLoop, lv_nPauseSubLoop;
	CString lv_strMidString;
	CStringArray strArrMain;
	CStringArray strArrExt;
	int lv_varlength = 0;				// 变量长度

	if( m_hCall )
	{
		LINECALLPARAMS lv_Param;
		memset(&lv_Param, 0x00, sizeof(lv_Param));
		lv_Param.dwTotalSize = sizeof(lv_Param);
		lv_Param.dwNoAnswerTimeout = (DWORD)nNoAnsTimeOut;

		// Sun added 2010-08-02 [B]
		patience = 0;
		do
		{
			// We have not more patience, stop the loop
			if( ++patience > theApp.m_nCallTransferMaxTryTimes )
			{
				WriteTrace(TraceError, "Error - IVR Channel: %d TapiLine: %d function ConsultConference() failed after MaxTry", 
					GetDeviceID(), m_ID);
				ret = -1;
				break;
			}

			try
			{
				// Sun added 2010-08-04
				CSingleLock lv_MySingleLock(&g_thProcessTAPIEvents.m_ReplyLock);
				lv_MySingleLock.Lock(2000);

				m_hConferenceCall = 0;
				m_hConsultCall = 0;
				ret = lineSetupConference(m_hCall, NULL, &m_hConferenceCall, &m_hConsultCall, 3, &lv_Param);
				if( ret > 0 )
				{
					// Sun added 2010-08-04
					AddCurrentRequest((DWORD)ret);
					lv_MySingleLock.Unlock();

					// Setup transfer successfully
					if( StartWaitRequest() )
					{
						ret = (long)m_CallSyncEvent.m_nTag;
						WriteTrace(TraceDetail, "Detail - IVR Channel: %d TapiLine: %d function ConsultConference() called lineSetupConference() retrun: 0x%x on Call: 0x%x got ConferenceCall: 0x%x, ConsultCall: 0x%x, try time = %d", 
							GetDeviceID(), m_ID, ret, m_hCall, m_hConferenceCall, m_hConsultCall, patience);

						// Consultation OK
						if( ret == 0 || m_hConsultCall > 0 )		// LINEERR_INVALLINESTATE可能也是成功
						{
							// Sun added 2010-08-04
							lv_MySingleLock.Lock(2000);

							//-----------------------------------------------------
							// Sun replaced 2013-07-25
							/// 分析号码：拆分主号码、延时号码段、分机等
							/// From
							//ret = lineDial(m_hConsultCall, lpszDigits, 0);
							/// To
							lv_sPhoneNo = lpszDigits;
							lv_nPreFlag = 0;			// Sun added 2010-12-01 [B]
							lv_strMidString = _T("");	// Sun added 2010-12-01 [B]
							strArrMain.RemoveAll();		// Sun added 2010-12-01 [B]
							strArrExt.RemoveAll();		// Sun added 2010-12-01 [B]
							lv_sExtNo = _T("");
							lv_subloop = 0;
							memset(lv_sTemp, 0x00, sizeof(lv_sTemp));
							lv_varlength = lv_sPhoneNo.GetLength();

							// 过滤非法字符，分离出分机号
							for( lv_loop = 0; lv_loop < lv_varlength; lv_loop++ )
							{
								lv_char = lv_sPhoneNo.GetAt(lv_loop);
								if( lv_char == '-' )
								{
									lv_sExtNo = lv_sPhoneNo.Right(lv_varlength - lv_loop -1);
									lv_sPhoneNo = lv_sTemp;
									lv_subloop = 0;
									break;
								}
								else if( strchr(DEF_IVR_DTMF_CHARSET, lv_char) != NULL )
								{	// 合法DTMF
									if( lv_nPreFlag <= 1 )
									{	// 记入主号码
										lv_sTemp[lv_subloop++] = lv_char;
										lv_nPreFlag = 1;
									}
									else
									{
										if( lv_nPreFlag == 3 )
										{
											strArrMain.Add(lv_strMidString);
											WriteTrace(TraceDetail, "IVR Channel: %d dial MainNumber add pause section: %d - '%s' ", 
												GetDeviceID(), strArrMain.GetSize(), lv_strMidString);
											lv_strMidString = _T("");
										}

										// 记入延时号码段
										lv_strMidString = lv_strMidString + lv_char;
										lv_nPreFlag = 2;
									}
								}
								else if( lv_char == DEF_IVR_DIAL_LONGPAUSE || lv_char == DEF_IVR_DIAL_MIDPAUSE || lv_char == DEF_IVR_DIAL_SHORTPAUSE )
								{ // 延时分隔符
									if( lv_nPreFlag == 2 )	// 前一个是DTMF
									{
										strArrMain.Add(lv_strMidString);
										WriteTrace(TraceDetail, "IVR Channel: %d dial MainNumber add pause section: %d - '%s' ", 
											GetDeviceID(), strArrMain.GetSize(), lv_strMidString);
										lv_strMidString = _T("");
									}
									lv_strMidString = lv_strMidString + lv_char;
									lv_nPreFlag = 3;
								}
							}

							// Sun added 2010-12-01 [B]
							if( lv_strMidString.GetLength() > 0 )
							{
								strArrMain.Add(lv_strMidString);
								WriteTrace(TraceDetail, "IVR Channel: %d dial MainNumber add last pause section: %d - '%s' ", 
									GetDeviceID(), strArrMain.GetSize(), lv_strMidString);
								lv_strMidString = _T("");
							}

							if( lv_subloop > 0 )
							{	// 无分机
								lv_sPhoneNo = lv_sTemp;
							}
							else
							{
								/// 继续过滤分机
								lv_nPreFlag = 0;			// Sun added 2010-12-01 [B]
								lv_varlength = lv_sExtNo.GetLength();
								if( lv_varlength > 0 )
								{
									lv_subloop = 0;
									memset(lv_sTemp, 0x00, sizeof(lv_sTemp));
									for( lv_loop = 0; lv_loop < lv_varlength; lv_loop++ )
									{
										lv_char = lv_sExtNo.GetAt(lv_loop);

										if( strchr(DEF_IVR_DTMF_CHARSET, lv_char) != NULL )
										{ // 合法DTMF
											if( lv_nPreFlag <= 1 )
											{
												lv_sTemp[lv_subloop++] = lv_char;
												lv_nPreFlag = 1;
											}
											else
											{
												if( lv_nPreFlag == 3 )
												{
													strArrExt.Add(lv_strMidString);
													WriteTrace(TraceDetail, "IVR Channel: %d dial ExtNumber add pause section: %d - '%s' ", 
														GetDeviceID(), strArrExt.GetSize(), lv_strMidString);
													lv_strMidString = _T("");
												}

												// 记入延时号码段
												lv_strMidString = lv_strMidString + lv_char;
												lv_nPreFlag = 2;
											}
										}
										else if( lv_char == DEF_IVR_DIAL_LONGPAUSE || lv_char == DEF_IVR_DIAL_MIDPAUSE || lv_char == DEF_IVR_DIAL_SHORTPAUSE )
										{ // 延时分隔符
											if( lv_nPreFlag == 2 )	// 前一个是DTMF
											{
												strArrExt.Add(lv_strMidString);
												WriteTrace(TraceDetail, "IVR Channel: %d dial ExtNumber add pause section: %d - '%s' ", 
													GetDeviceID(), strArrExt.GetSize(), lv_strMidString);
												lv_strMidString = _T("");
											}
											lv_strMidString = lv_strMidString + lv_char;
											lv_nPreFlag = 3;
										}
									}
									lv_sExtNo = lv_sTemp;
									if( lv_strMidString.GetLength() > 0 )
									{
										strArrExt.Add(lv_strMidString);
										WriteTrace(TraceDetail, "IVR Channel: %d dial ExtNumber add pause section: %d - '%s' ", 
											GetDeviceID(), strArrExt.GetSize(), lv_strMidString);
										lv_strMidString = _T("");
									}
								}
							}

							WriteTrace(TraceDebug, "IVR Channel: %d will dial Number: %s with %d pauses, Ext: %s with %d pauses", 
								GetDeviceID(), lv_sPhoneNo, strArrMain.GetSize(), lv_sExtNo, strArrExt.GetSize());

							ret = lineDial(m_hConsultCall, (LPCSTR)lv_sPhoneNo, 0);
							//-----------------------------------------------------

							WriteTrace(TraceDebug, "TapiObj.cpp, IVR Channel: %d Line:%d lineDial(m_hConsultCall:0x%x) ret 0x%x", GetDeviceID(), m_ID, m_hConsultCall, ret);
							if( ret > 0 )
							{
								// Sun added 2010-08-04
								AddCurrentRequest((DWORD)ret);
								lv_MySingleLock.Unlock();

								if( StartWaitRequest() )
								{
									ret = (long)m_CallSyncEvent.m_nTag;
									if( ret == 0 )
									{
										// 拨号成功:0
										//-----------------------------------------------------
										// Sun added 2013-07-25
										/// 拨分机号
										if( lv_sExtNo.GetLength() > 0 )
										{
											/// 延时及发送DTMF
											for( lv_nPauseLoop = 0; lv_nPauseLoop < strArrMain.GetSize(); lv_nPauseLoop++ )
											{
												lv_strMidString = strArrMain.GetAt(lv_nPauseLoop);
												lv_char = lv_strMidString.GetAt(0);
												if( strchr(DEF_IVR_DTMF_CHARSET, lv_char) != NULL )
												{
													// 发送DTMF
													if( SendDTFM((LPCSTR)lv_strMidString) != 0 )
													{
														WriteTrace(TraceWarn, "TapiObj.cpp, IVR Channel: %d Line:%d SendDTMF(%s) failed", GetDeviceID(), m_ID, lv_strMidString);
														return ret;
													}
												}
												else
												{
													// 延时
													for( lv_nPauseSubLoop = 0; lv_nPauseSubLoop < lv_strMidString.GetLength(); lv_nPauseSubLoop++ )
													{
														lv_char = lv_strMidString.GetAt(lv_nPauseSubLoop);
														if( lv_char == DEF_IVR_DIAL_LONGPAUSE )
														{
															Sleep(1000);
														}
														else if( lv_char == DEF_IVR_DIAL_MIDPAUSE )
														{
															Sleep(500);
														}
														else if( lv_char == DEF_IVR_DIAL_SHORTPAUSE )
														{
															Sleep(200);
														}

														/// 防止通道超时
														//SetTickTime();

														// Sun added 2012-07-04
														/// 主叫挂机
														//if( !m_bInboundCall )
														//	break;
													}
												}
											}

											//// DTMF拨号
											if( SendDTFM((LPCSTR)lv_sExtNo) != 0 )
											{
												WriteTrace(TraceWarn, "TapiObj.cpp, IVR Channel: %d Line:%d SendDTMF(%s) Ext failed", GetDeviceID(), m_ID, lv_sExtNo);
												return ret;
											}

											/// Sun added 2010-12-01 [B]
											if( strArrExt.GetSize() > 0 )
											{
												/// 延时及发送DTMF
												for( lv_nPauseLoop = 0; lv_nPauseLoop < strArrExt.GetSize(); lv_nPauseLoop++ )
												{
													lv_strMidString = strArrExt.GetAt(lv_nPauseLoop);
													lv_char = lv_strMidString.GetAt(0);
													if( strchr(DEF_IVR_DTMF_CHARSET, lv_char) != NULL )
													{
														// 发送DTMF
														if( SendDTFM((LPCSTR)lv_strMidString) != 0 )
														{
															WriteTrace(TraceWarn, "TapiObj.cpp, IVR Channel: %d Line:%d SendDTMF(%s) ExtTail failed", GetDeviceID(), m_ID, lv_strMidString);
															return ret;
														}
													}
													else
													{
														// 延时
														for( lv_nPauseSubLoop = 0; lv_nPauseSubLoop < lv_strMidString.GetLength(); lv_nPauseSubLoop++ )
														{
															lv_char = lv_strMidString.GetAt(lv_nPauseSubLoop);
															if( lv_char == DEF_IVR_DIAL_LONGPAUSE )
															{
																Sleep(1000);
															}
															else if( lv_char == DEF_IVR_DIAL_MIDPAUSE )
															{
																Sleep(500);
															}
															else if( lv_char == DEF_IVR_DIAL_SHORTPAUSE )
															{
																Sleep(200);
															}

															/// 防止通道超时
															//SetTickTime();
														}
													}
												}
											}
										}
										//-----------------------------------------------------

										break;
									}
									else
									{
										// 拨号失败
										ret = -1;
										break;
									}
								}
								else
								{
									WriteTrace(TraceError, "Error - IVR Channel: %d TapiLine: %d function ConsultConference() called lineDial(0x%x, %s) operation time out", 
										GetDeviceID(), m_ID, m_hConsultCall, lpszDigits);
									ret = -1;
									break;
								}
							}
							else
							{
								// Sun added 2010-08-04
								lv_MySingleLock.Unlock();

								// ret < 0
								WriteTrace(TraceError, "Error - IVR Channel: %d TapiLine: %d function ConsultConference() called lineDial(0x%x, %s) failed error code = %d", 
									GetDeviceID(), m_ID, m_hConsultCall, lpszDigits, ret);
								break;
							}
						}
						//----------------------------------------
						// Sun added 2010-08-02 [B]
						else if( ret == LINEERR_INVALLINESTATE )
						{
							// Sun added 2011-08-08
							DWORD lv_dwLineDevStatus = 0;

							// Sun added 2011-08-08
							lv_dwLineDevStatus = PrintLineStatus();

							/// 磋商失败就挂断，避免影响扩大化（权宜之计）
							ret = gb_nSetupConferenceFailedMethod;		// 1 继续； 2 无应答处理；-1 挂断；3 - 尝试单步转接

							//--------------------------------------------------
							// Sun added 2011-08-08
							if( gb_nSetupConferenceFailedMethod == 3 )
							{
								if( lv_dwLineDevStatus != (LINEDEVSTATUSFLAGS_CONNECTED + LINEDEVSTATUSFLAGS_INSERVICE) )
									ret = -1;		// 状态不对则挂断
							}
							//--------------------------------------------------
							break;
						}
						else
						{
							// 其他失败原因
							ret = -1;
							break;
						}
						//----------------------------------------
					}
					else
					{
						WriteTrace(TraceError, "Error - IVR Channel: %d TapiLine: %d function ConsultConference() called lineSetupConference() on Call: 0x%x operation time out, try time = %d", 
							GetDeviceID(), m_ID, m_hCall, patience);
						ret = -1;
						break;
					}
				}
				else
				{
					// Sun added 2010-08-04
					lv_MySingleLock.Unlock();

					// ret < 0
					WriteTrace(TraceError, "Error - IVR Channel: %d TapiLine: %d function ConsultConference() called lineSetupConference() on Call: 0x%x failed error code = %d, try time = %d", 
						GetDeviceID(), m_ID, m_hCall, ret, patience);
					break;
				}
			}
			catch(...)
			{
			}
		} while (TRUE);
	}

	return ret;
}

// 停止磋商，拾起被保持呼叫
long CTapiLine::StopConsultaion()
{
	long ret = -1;
	HCALL lv_hCall;

	// Sun added 2012-04-18 [B]
	lv_hCall = m_hCall;
	WriteTrace(TraceDetail, "TapiObj.cpp, IVR Channel: %d Line:%d calls StopConsultaion(). Conf=0x%x, Cons=0x%x, Call=0x%x", GetDeviceID(), m_ID, m_hConferenceCall, m_hConsultCall, m_hCall);

	// Sun added 2010-08-04
	CSingleLock lv_MySingleLock(&g_thProcessTAPIEvents.m_ReplyLock);

	// 停止磋商
	if( m_hConsultCall )
	{
		WriteTrace(TraceDetail, "TapiObj.cpp, IVR Channel: %d Line:%d calls lineDrop(Cons=0x%x)", GetDeviceID(), m_ID, m_hConsultCall);

		lv_MySingleLock.Lock(2000);
		ret = lineDrop(m_hConsultCall, NULL, 0);
		if( ret == 0 )
		{
			// Sun added 2013-07-05
			lv_MySingleLock.Unlock();

			lineDeallocateCall(m_hConsultCall);
			m_hConsultCall = NULL;
		}
		// Sun added 2013-07-05
		else
		{
			AddCurrentRequest((DWORD)ret);
			lv_MySingleLock.Unlock();
			if( StartWaitRequest() )
			{
				ret = (long)m_CallSyncEvent.m_nTag;
				if( ret == 0 )
				{
					//WriteTrace(TraceDetail, "TapiObj.cpp, IVR Channel: %d Line:%d calls lineDeallocateCall(Call=0x%x)", GetDeviceID(), m_ID, m_hConsultCall);
					//ret = lineDeallocateCall(m_hConsultCall);
					Sleep(100);
				}
			}
		}
	}

	// 停止会议
	/*
	if( m_hConferenceCall )
	{
		WriteTrace(TraceDetail, "TapiObj.cpp, IVR Channel: %d Line:%d calls lineDrop(Conf=0x%x)", GetDeviceID(), m_ID, m_hConferenceCall);

		lv_MySingleLock.Lock(2000);
		ret = lineDrop(m_hConferenceCall, NULL, 0);
		if( ret == 0 )
		{
			// Sun added 2013-07-05
			lv_MySingleLock.Unlock();

			lineDeallocateCall(m_hConferenceCall);
			m_hConsultCall = NULL;
		}
		// Sun added 2013-07-05
		else
		{
			AddCurrentRequest((DWORD)ret);
			lv_MySingleLock.Unlock();
			if( StartWaitRequest() )
			{
				ret = (long)m_CallSyncEvent.m_nTag;
			}
		}
	}
	*/

	// 拾起被保持呼叫
	if( lv_hCall )
	{
		WriteTrace(TraceDetail, "TapiObj.cpp, IVR Channel: %d Line:%d calls lineUnhold(Call=0x%x)", GetDeviceID(), m_ID, lv_hCall);
		lv_MySingleLock.Lock(2000);
		ret = lineUnhold(lv_hCall);
		if( ret > 0 )
		{
			// Sun added 2010-08-04
			AddCurrentRequest((DWORD)ret);
			lv_MySingleLock.Unlock();

			if( StartWaitRequest() )
			{
				ret = (long)m_CallSyncEvent.m_nTag;

				//-----------------------------------------
				// Sun test 2012-04-18 [B]
				if( ret == 0 )
				{
					lv_MySingleLock.Lock(2000);
					ret = lineHold(m_hCall);
					if( ret > 0 )
					{
						AddCurrentRequest((DWORD)ret);
						lv_MySingleLock.Unlock();
						if( StartWaitRequest() )
						{
							ret = (long)m_CallSyncEvent.m_nTag;
							if( ret == 0 )
							{
								lv_MySingleLock.Lock(2000);
								ret = lineUnhold(m_hCall);
								if( ret > 0 )
								{
									AddCurrentRequest((DWORD)ret);
									lv_MySingleLock.Unlock();
									if( StartWaitRequest() )
									{
										ret = (long)m_CallSyncEvent.m_nTag;
									}
								}
							}
						}
					}
				}
				//-----------------------------------------
			}
			else
				ret = -1;
		}
		else
			ret = -1;
	}
	else
		ret = -1;

	return ret;
}

long CTapiLine::CompleteTransfer(BOOL blnTransfer)
{
	long ret = -1;

	// Drop the line
	if( m_hCall && m_hConsultCall )
	{
		// Sun added 2010-08-04
		CSingleLock lv_MySingleLock(&g_thProcessTAPIEvents.m_ReplyLock);
		lv_MySingleLock.Lock(2000);

		if( blnTransfer )
			ret = lineCompleteTransfer(m_hCall, m_hConsultCall, NULL, LINETRANSFERMODE_TRANSFER);
		else
		{
			// Cisco TAPI don't support this function
			//ret = lineCompleteTransfer(m_hCall, m_hConsultCall, &m_hConferenceCall, LINETRANSFERMODE_CONFERENCE);
			ret = lineAddToConference(m_hConferenceCall, m_hConsultCall);
		}
			
		if( ret > 0 )
		{
			// Sun added 2010-08-04
			AddCurrentRequest((DWORD)ret);
			lv_MySingleLock.Unlock();

			WriteTrace(TraceDetail, "Detail - IVR Channel: %d CTapiLine::CompleteTransfer(%d) returns 0x%x", GetDeviceID(), blnTransfer, ret);

			// Complete Transfer or Conference successfully
			if( StartWaitRequest() )
				ret = (long)m_CallSyncEvent.m_nTag;
			else
				ret = -1;
		}
		else
		{
			WriteTrace(TraceWarn, "Warn - IVR Channel: %d CTapiLine::CompleteTransfer(%d) returns 0x%x!", GetDeviceID(), blnTransfer, ret);
			ret = -1;
		}
	}

	return ret;
}

DWORD CTapiLine::PrintLineStatus(DWORD *f_pdwFeature)
{
	DWORD lv_dwLineDevStatus = 0;

	// Sun added 2010-08-05
	LPLINEDEVSTATUS pLineDevStatue = new LINEDEVSTATUS;
	pLineDevStatue->dwTotalSize = sizeof(LINEDEVSTATUS);
	if( lineGetLineDevStatus(m_hLine, pLineDevStatue) == 0 )
	{
		// Sun added 2011-08-08
		lv_dwLineDevStatus = pLineDevStatue->dwDevStatusFlags;
		if( f_pdwFeature )
			*f_pdwFeature = pLineDevStatue->dwLineFeatures;

		WriteTrace(TraceDebug, "IVR Channel: %d PrintLineStatus() StatusFlags = 0x%x, Features = 0x%x",
			GetDeviceID(), 
			pLineDevStatue->dwDevStatusFlags,
			pLineDevStatue->dwLineFeatures);
	}
	delete pLineDevStatue;

	return lv_dwLineDevStatus;
}

// Initiates the generation of the specified digits on
// the specified call as out-of-band tones, Null for 
long CTapiLine::SendDTFM(LPCSTR lpszDigits)
{
	long ret = -1;

	if( m_hCall )
	{
		// Cisco TSP does not support dwDuration.
		/// 0 - Succeed
		/// < 0 - Failed
		ret = lineGenerateDigits(m_hCall, LINEDIGITMODE_DTMF, lpszDigits, 0);
	}

	return ret;
}

// The tone is a beep, as used to announce the beginning of a recording
long CTapiLine::SendBeep()
{
	long ret = -1;

	if( m_hCall )
	{
		// Cisco TSP does not support dwDuration.
		ret = lineGenerateTone(m_hCall, LINETONEMODE_BEEP, 0, 0, NULL);
	}

	return ret;
}

UL CTapiLine::GetTerminateCode()
{
	if( m_PlaySyncEvent.Wait(0) )
		return m_PlaySyncEvent.m_nEventID;

	return VOICE_TM_Dummy;
}

void CTapiLine::SetTerminateCode(UL lngTermCode)
{
	m_PlaySyncEvent.m_nEventID = lngTermCode;
	if( lngTermCode == VOICE_TM_Dummy )
		m_PlaySyncEvent.Reset();
	else
		m_PlaySyncEvent.Set();

	WriteTrace(TraceDetail, "IVR Channel: %d TAPI line:%d terminate code = %d", GetDeviceID(), m_ID, lngTermCode);
}

void CTapiLine::ClearTPTParam()
{
	memset(m_tpt, 0x00, sizeof(DV_TPT) * DEF_TAPI_MAX_TPTCNT);
}

void CTapiLine::SetTPTParam(DV_TPT *pTPT, int nCount)
{
	int lv_nCount;

	SetTerminateCode(VOICE_TM_Dummy);
	if( nCount < DEF_TAPI_MAX_TPTCNT )
		lv_nCount = nCount;
	else
		lv_nCount = DEF_TAPI_MAX_TPTCNT;

	m_dwTotalKeyTick = m_dwLastKeyTick = m_dwIntervalTick = GetTickCount();
	memcpy(m_tpt, pTPT, sizeof(DV_TPT) * lv_nCount);
}

// 检测中断条件
/// 执行时机：
/// 1、断线
/// 2、收到新按键
/// 3、按键计时
/// 4、录音静默
BOOL CTapiLine::TestTPTCondition(char chLastKey)
{
	// 已经中断做过判断，尚未处理完毕
	if( GetTerminateCode() != VOICE_TM_Dummy )
		return FALSE;

	BOOL lv_ret = FALSE;
	UINT lv_termcode = 0;
    DWORD lv_NowTick = GetTickCount();

	for( int lv_loop = 0; lv_loop < DEF_TAPI_MAX_TPTCNT; lv_loop++ )
	{
		if( m_tpt[lv_loop].tp_type == IO_CONT || m_tpt[lv_loop].tp_type == IO_EOT )
		{
			// Test the condition
			switch( m_tpt[lv_loop].tp_termno )
			{ 
			case DX_LCOFF:				// 断线
				if( GetLineStatus() <= ST_READY )
				{
					lv_termcode = VOICE_TM_LCOFF;
					lv_ret = TRUE;
				}
				break;

			case DX_MAXDTMF:			// 最大按键
				if( GetKeyBufferLength() >= m_tpt[lv_loop].tp_length && chLastKey > 0 )
				{
					lv_termcode = VOICE_TM_MAXDTMF;
					lv_ret = TRUE;
				}
				break;

			case DX_DIGMASK:			// 指定按键
				if( GetDigitMask(chLastKey) & m_tpt[lv_loop].tp_length )
				{
					lv_termcode = VOICE_TM_DIGIT;
					lv_ret = TRUE;
				}
				break;

			case DX_IDDTIME:			// 最大按键间隔
				if( m_dwLastKeyTick > lv_NowTick )
					m_dwLastKeyTick = lv_NowTick;
				else if( (lv_NowTick - m_dwLastKeyTick) / 100 >= m_tpt[lv_loop].tp_length )
				{
					lv_termcode = VOICE_TM_IDDTIME;
					lv_ret = TRUE;
				}
				break;

			case DX_MAXTIME:			// 最大时间终止
				if( m_dwTotalKeyTick > lv_NowTick )
					m_dwTotalKeyTick = lv_NowTick;
				else if( (lv_NowTick - m_dwTotalKeyTick) / 100 >= m_tpt[lv_loop].tp_length )
				{
					lv_termcode = VOICE_TM_MAXTIME;
					lv_ret = TRUE;
				}
				break;

			case DX_MAXSIL:				// 最大静默时间
				// Recording Control
				if( CCtrlWaveIn::IsRecording() )
				{
					if( CCtrlWaveIn::GetIsSilence() )
					{
						if( CCtrlWaveIn::GetMaxSilenceTime() > 0 )
						{
							if( CCtrlWaveIn::GetSilenceLength() > (DWORD)CCtrlWaveIn::GetMaxSilenceTime() * 1000 )
							{
								/// Stop Recording
								lv_termcode = VOICE_TM_MAXSIL;
								lv_ret = TRUE;
							}
						}
					}
				}
				break;

			// Sun added 2009-07-24, [C]
			case DX_MAXNOSIL:			// 最大非静默时间，录音Beep间隔
				if( m_tpt[lv_loop].tp_length > 0 )
				{
					if( m_dwIntervalTick > lv_NowTick )
						m_dwIntervalTick = lv_NowTick;
					else if( (lv_NowTick - m_dwIntervalTick) / 100 >= m_tpt[lv_loop].tp_length )
					{
						m_dwIntervalTick = lv_NowTick;
						WriteTrace(TraceDetail, "Detail - IVR Channel: %d TapiLine: %d TPT interval reached.", GetDeviceID(), m_ID);

						// Recording Control
						if( CCtrlWaveIn::IsRecording() )
						{
							/// Don't Stop Recording, but play beep only
							lv_termcode = VOICE_TM_MAXNOSIL;
							SendBeep();
						}
					}
				}
				break;
			}

			// Last condition passed
			if( m_tpt[lv_loop].tp_type == IO_EOT || lv_ret )
				break;
		}
		else
			break;
	}

	if( lv_ret )
	{
		ClearTPTParam();

		// Stop Playing or Recording
		if( IsPlaying() )
			CFileVoiceOut::StopPlay();

		if( IsRecording() )
			CCtrlWaveIn::StopRecord();

		SetTerminateCode(lv_termcode);
	}

	return lv_ret;
}

//------------------------------------------------------------------
// Voice routines
//------------------------------------------------------------------
void CTapiLine::BufferSub()
{
	// Play next group of frames
	CFileVoiceOut::BufferSub();

	if( CFileVoiceOut::GetPlayedBytes() >= CFileVoiceOut::GetFileBytes() && !CFileVoiceOut::m_blnLoopPlay )
		SetTerminateCode(VOICE_TM_EOD);
}


// 同步或异步播放文件，支持循环播放
long CTapiLine::PlayFile(LPCSTR lpszFilePath, UINT nMaxLength, BOOL blnAsync, BOOL blnLoopPlay)
{
	long ret = -1;

	if( m_dwWaveOutID > 0 )
	{
		WriteTrace(TraceDetail, "Tony Debug 2010-07-18: IVR Channel: %d Play 1", GetDeviceID());

		if( !CFileVoiceOut::PlayFile(lpszFilePath, blnLoopPlay) )
			SetTerminateCode(VOICE_TM_ERROR);
		else
		{
			/// 异步->同步
			if( blnAsync )
			{
				// return success without actual result
				ret = 0;
				return ret;
			}
			else
			{
				// Sun added 2013-08-20
				if( GetTerminateCode() == VOICE_TM_USRSTOP )
				{
					CFileVoiceOut::CloseFile();
					WriteTrace(TraceInfo, "Info - IVR Channel: %d terminated when playing file", GetDeviceID());
					ret = -1;
					return ret;
				}

				DWORD lv_testTick = GetTickCount();
				WriteTrace(TraceDetail, "Tony Debug 2010-07-18: IVR Channel: %d Play 2", GetDeviceID());

				// Sun added 2010-08-02 [C]
				DWORD lv_dwWait;
				if( nMaxLength <= 0 )
					lv_dwWait = INFINITE;
				else
					lv_dwWait = nMaxLength * 1000;

				// Sun replaced 2010-08-02 [C]
				/// INFINITE -> lv_dwWait
				if( m_PlaySyncEvent.Wait(lv_dwWait) == true )
				{
					// return success with actual result
					ret = m_PlaySyncEvent.m_nEventID;

					// Wait Wave Thread Exit
					Sleep(100);
				}

				WriteTrace(TraceDetail, "Tony Debug 2010-07-18: IVR Channel: %d Play 3: wait %d s", GetDeviceID(), (GetTickCount() - lv_testTick) / 1000);
			}
		}

		// Close Playing File
		CFileVoiceOut::CloseFile();
	}
	else
	{
		WriteTrace(TraceError, "Error - IVR Channel: %d line: %d failed to call CTapiLine::PlayFile(%s), no wave out resource!", GetDeviceID(), m_ID, lpszFilePath);
	}

	return ret;
}

// 同步或异步内存放音，
/// 注意:调用前必须手工开始(StartPlay)播放，结束后必须手工停止(StopPlay)播放
long CTapiLine::PlayBuffer(BYTE *pData, UINT nSize, UINT nMaxLength, BOOL blnAsync)
{
	long ret = -1;

	if( m_dwWaveOutID > 0 )
	{
		if( !CFileVoiceOut::PlayBuffer(pData, nSize) )
			SetTerminateCode(VOICE_TM_ERROR);
		else
		{
			/// 异步->同步
			if( blnAsync )
			{
				// return success without actual result
				ret = 0;
			}
			else
			{
				// Sun added 2010-08-02 [C]
				DWORD lv_dwWait;
				if( nMaxLength <= 0 )
					lv_dwWait = INFINITE;
				else
					lv_dwWait = nMaxLength * 1000;

				// Sun replaced 2010-08-02 [C]
				/// INFINITE -> lv_dwWait
				if( m_PlaySyncEvent.Wait(lv_dwWait) == true )
				{
					// return success with actual result
					ret = m_PlaySyncEvent.m_nEventID;
				}
			}
		}
	}
	else
	{
		WriteTrace(TraceError, "Error - IVR Channel: %d line: %d failed to call CTapiLine::PlayBuffer(), no wave out resource!", GetDeviceID(), m_ID);
	}

	return ret;
}

BOOL CTapiLine::StopPlay()
{
	// File Play Stop
	if( CFileVoiceOut::IsPlaying() )
	{
		WriteTrace(TraceDetail, "IVR Channel: %d line: %d trys to stop playing.", GetDeviceID(), m_ID);

		ClearTPTParam();
		BOOL lv_retval = CFileVoiceOut::StopPlay();
		SetTerminateCode(VOICE_TM_USRSTOP);
		return lv_retval;
	}

	return TRUE;
}

// 如果nDigitCnt = 0,则取全部按键
// 返回值：实际取得按键数，异步调用返回0
int CTapiLine::GetDigit(char *pszDigit, int nDigitCnt, UINT nMaxLength, BOOL blnAsync)
{
	int lv_nCount = 0;

	/// 异步->同步
	try
	{
		if( !blnAsync )
		{
			// Sun added 2010-08-02 [C]
			DWORD lv_dwWait;
			if( nMaxLength <= 0 )
				lv_dwWait = INFINITE;
			else
				lv_dwWait = nMaxLength * 1000;

			// Sun replaced 2010-08-02 [C]
			/// INFINITE -> lv_dwWait
			if( m_PlaySyncEvent.Wait(lv_dwWait) == true )
			{
				if( nDigitCnt == 0 )
					nDigitCnt = GetKeyBufferLength();

				if( nDigitCnt > 0 )
					lv_nCount = PopKey(pszDigit, nDigitCnt);
				pszDigit[lv_nCount] = NULL;

				// Wait Wave Thread Exit
				Sleep(100);
			}
		}
	}
	catch(...)
	{
		WriteTrace(TraceError, "Error - IVR Channel: %d CTapiLine::GetDigit() memory copy failed!", GetDeviceID());
	}

	return lv_nCount;
}

long CTapiLine::RecordFile(LPCSTR lpszFilePath, UINT nMaxLength, BOOL blnAsync)
{
	long ret = -1;

	if( m_dwWaveInID > 0 )
	{
		CCtrlWaveIn::SetRecordFileName(lpszFilePath);

		if( !CCtrlWaveIn::StartRecord() )
			SetTerminateCode(VOICE_TM_ERROR);
		else
		{
			/// 异步->同步
			if( blnAsync )
			{
				// return success without actual result
				ret = 0;
			}
			else
			{
				// Sun added 2010-08-02 [C]
				DWORD lv_dwWait;
				if( nMaxLength <= 0 )
					lv_dwWait = INFINITE;
				else
					lv_dwWait = nMaxLength * 1000;

				// Sun replaced 2010-08-02 [C]
				/// INFINITE -> lv_dwWait
				if( m_PlaySyncEvent.Wait(lv_dwWait) == true )
				{
					// return success with actual result
					ret = m_PlaySyncEvent.m_nEventID;

					// Wait Wave Thread Exit
					Sleep(100);
				}
			}
		}
	}
	else
	{
		WriteTrace(TraceError, "Error - IVR Channel: %d line: %d failed to call CTapiLine::RecordFile(%s), no wave in resource!", GetDeviceID(), m_ID, lpszFilePath);
	}

	return ret;
}

BOOL CTapiLine::StopRecord()
{
	// Recording Stop
	if( CCtrlWaveIn::IsRecording() )
	{
		ClearTPTParam();
		return CCtrlWaveIn::StopRecord();
	}

	return TRUE;
}

#endif