///////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		_IVRCh.cpp
// ��;������	IVRͨ����ʵ��
// ���ߣ�		SUN
// ����ʱ�䣺	2001/06/28
// ����޸�ʱ�䣺2003/05/05
// ��Ȩ��Ϣ��	Copyright (C) 1999 - 2003 PCS
// ��ע��Multithread
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
#include "stdafx.h"
#include "PowerVoice.h"
#include "MainFrm.h"
#include "PowerVoiceView.h"
#include "IVRChannel.h"

#include "_CommonFunc.h"
#include "_Global.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Sun added 2006-02-08
#define DEF_IVR_RECFILE_ADDNEW_VOX					"Group%u\\a%u_c%u_%s.vox"
#define DEF_IVR_RECFILE_ADDNEW_WAV					"Group%u\\a%u_c%u_%s.wav"

#ifdef INTEL_DIALOGIC
// Dialiog supports vox and wav
#define DEF_IVR_RECFILE_FORMAT_NEW					"%sGroup%u\\a%u_c*.*"
#define DEF_IVR_RECFILE_FORMAT_CLOSED				"%sGroup%u\\da%u_c*.*"
#define DEF_IVR_RECFILE_FORMAT_DELETED				"%sGroup%u\\xa%u_c*.*"

#else
// Cisco only supports wave format
#define DEF_IVR_RECFILE_FORMAT_NEW					"%sGroup%u\\a%u_c*.wav"
#define DEF_IVR_RECFILE_FORMAT_CLOSED				"%sGroup%u\\da%u_c*.wav"
#define DEF_IVR_RECFILE_FORMAT_DELETED				"%sGroup%u\\xa%u_c*.wav"

#endif

// Sun added 2008-05-25
#define DEF_IVR_RECFILE_FORMAT_ASR					"%sGroup%u\\asr\\g%s_c%u_%s.wav"

// Sun added 2006-11-30
#define DEF_IVR_FAXFILE_FORMAT_AUTO					"%sGroup%u\\%s_c%u.tif"


// User DLL Declare
typedef HRESULT (WINAPI *Userdllfunc)(const int channel); 

static UINT MyHookThread(LPVOID pParam);


static UINT MyHookThread(LPVOID pParam)
{
	if( pParam )
	{
		CIVRChannel *lv_pChannel = (CIVRChannel *)pParam;
		WriteTrace(TraceDebug, "IVR Channel: %d - IVR Hook thread started", lv_pChannel->GetLineNO());
		lv_pChannel->SetHookOn();
		WriteTrace(TraceDebug, "IVR Channel: %d - IVR Hook thread stoped", lv_pChannel->GetLineNO());
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyHookThread::CMyHookThread()
{
	m_dwObject = 0;
}

CMyHookThread::~CMyHookThread()
{
}

//==========================================================
// Thread member functions
BOOL CMyHookThread::WaitForStartupToComplete(DWORD dwObj)
{
	m_dwObject = dwObj;
	CMyThread::Start(0, true);		// Auto Delete

	return TRUE;
}

int CMyHookThread::Run()
{
	if( m_dwObject )
	{
		CIVRChannel *lv_pChannel = (CIVRChannel *)m_dwObject;

		WriteTrace(TraceDebug, "IVR Channel: %d - IVR Hook thread started", lv_pChannel->GetLineNO());
		lv_pChannel->SetHookOn();
		WriteTrace(TraceDebug, "IVR Channel: %d - IVR Hook thread stoped", lv_pChannel->GetLineNO());
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////
// CIVRCH_Collection Construction/Destruction
//////////////////////////////////////////////////////////////////////
CIVRCH_Collection::CIVRCH_Collection()
{
}

CIVRCH_Collection::~CIVRCH_Collection()
{
	RemoveAllAgents();
}

void CIVRCH_Collection::RemoveAllAgents()
{
	m_bLock.Enter();
	RemoveAll();
	m_bLock.Leave();
}

int CIVRCH_Collection::GetIVRChannelCount()
{
	int lv_value;

	m_bLock.Enter();

	lv_value = (int)GetCount();

	m_bLock.Leave();

	return lv_value;
}

BOOL CIVRCH_Collection::AddAgent(UL f_AgentID, CIVRChannel *f_pAgent)
{
	m_bLock.Enter();

	if( f_pAgent )
		SetAt(f_AgentID, f_pAgent);

	m_bLock.Leave();

	return TRUE;
}

void CIVRCH_Collection::RemoveAgent(UL f_AgentID)
{
	m_bLock.Enter();

	RemoveKey(f_AgentID);

	m_bLock.Leave();
}

CIVRChannel *CIVRCH_Collection::FindAgentIndexWithID(UL f_AgentID)
{
	CIVRChannel *lv_pAgent;

	m_bLock.Enter();

	if( !Lookup(f_AgentID, lv_pAgent) )
		lv_pAgent = NULL;
	
	m_bLock.Leave();

	return lv_pAgent;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CIVRChannel::CIVRChannel()
{
	m_bytObjectType = 1;

	// Sun added 2004-12-30
	m_pTTSData = NULL;
	m_pFaxRes = NULL;

	m_bEnabled = FALSE;
	m_bStarted = FALSE;
	m_bOpened = FALSE;
	m_intLineNO = 0;
	m_bytGroupNo = 0;
	m_cET = 0;
	m_bytStatus = IVR_IDLE;
	m_intMaxPlayTime = 120;				// 2 Mins
	m_iMaxQueuedTime = 30;				// 0.5 Mins
	m_bytMaxTrys = MAX_TRYTIMES;
	m_objGroup = NULL;
	memset(&ChData, 0, sizeof(ChData));

	// Sun added 2006-10-01
	m_intMaxFaxTime = 600;				// 10 Mins
	m_lngFaxResID = -1;

	// Sun added 2002-02-28
	memset(m_szChannelName, 0x00, sizeof(m_szChannelName));
	memset(m_szDTIName, 0x00, sizeof(m_szDTIName));
	memset(m_szFaxName, 0x00, sizeof(m_szFaxName));
	memset(m_strNodeKeyPress, 0x00, sizeof(m_strNodeKeyPress));
	
	m_sDeviceNo.Empty();				// DN#
	m_sExtensionNo.Empty();				// Ext#

	// Call related data
	m_callrefno = 0;
	m_intSecondParty = 0;				// Second Party
	m_intThirdParty = 0;				// Third Party
	m_lngCurCallID = 0;					// Current Call ID
	m_lngNewCallID = 0;					// New Call ID
	m_lngCurConfID = 0;					// Current Conference ID
	m_lngNewConfID = 0;					// New Conference ID

	// Sun added 2002-04-03
	m_pstatus = IVR_IDLE;
	m_pstarted = FALSE;
	m_pticktime = GetTickCount();
	m_bInboundCall = FALSE;
	m_bOutboundCall = FALSE;
	m_bCTICallOut = FALSE;
	m_nCallAnswerDelay = 100;			// ms
	m_nChannelRestartTime = 0;			// min

	m_nLinkChannelNo = -1;
	m_bCTIOpened = FALSE;
	m_blnIsMonitor = FALSE;
	m_nForceNextNode = 0;

	// Sun added 2006-02-26
	m_lngDailyCallCount = 0;
	m_lngYTDCallCount = 0;

	// Sun added 2007-05-29
	m_nAnswerTimeOut = 0;

	// Sun added 2012-05-07
	m_nBeforeHookon = 0;

	// Sun added 2005-03-18
	m_CommEvent.Create();
	m_OBEvent.Create();
	m_QueryEvent.Create();
	m_HookOnWaitEvent.Create();

	// Sun added 2007-07-11
	m_QueueWaitSyncEvent.Create();

	// Sun added 2007-11-04
	m_lngChannelTIT = 0;
	m_lngLastTalkEndTick = 0;

	// Sun added 2007-11-18
	m_nCurrentCallFlowID = 0;

	// Sun added 2008-01-12
	m_nCallFlowNodeIndex = 0;
	m_nCurrentNodeNode = 0;
	m_nCurrentNodeLogLevel = 0;

	// Sun added 2008-05-16
	m_dwChCreate = m_dwChStart = GetTickCount();

	// Sun added 2008-02-19
	m_blnNodeLogOff = 0;

	// Sun added 2010-07-20 [E]
	m_bInCallFlow = FALSE;

	// Sun added 2010-07-22
	m_bConferenced = FALSE;

	// Sun added 2011-06-16 [B]
	m_blnCOMInitialized = FALSE;

	// Sun addded 2011-10-20, �Ƿ��Ѿ���¼IVR�굥
	m_blnCDRRecorded = TRUE;

	// Sun added 2012-11-23
	m_blnIsAsynPlaying = FALSE;
}

CIVRChannel::~CIVRChannel()
{
	WaitForShutdownToComplete();

	Destory();

	// Notify main window to refresh
	// Sun added 2011-06-27
	if( !pMainWindow->m_bSystemAutoShutDown )
		PropertyChanged();
}

BOOL CIVRChannel::CreateComObject(LPOLESTR pszProgID)
{
	BOOL lv_retval;

	try
	{
		lv_retval = (m_oUc.CreateObjectEx(pszProgID) == NOERROR);
	}
	catch(_com_error e)
	{
		WriteTrace(TraceError, "Error - IVR Channel: %d Error Msg:%s, CreateComObject(%s) failed!", 
			m_intLineNO, e.ErrorMessage(), pszProgID);

		lv_retval = FALSE;
	}
	catch(...)
	{
		WriteTrace(TraceError, "Error - IVR Channel: %d CreateComObject(%s) unexpected error!", 
			m_intLineNO, pszProgID);

		lv_retval = FALSE;
	}

	return( lv_retval );
}

//----------------------------------------------------------------
// Sun addded 2006-02-26
long CIVRChannel::GetDailyCallCount()
{
	return m_lngDailyCallCount;
}

long CIVRChannel::GetYTDCallCount()
{
	return m_lngYTDCallCount;
}

void CIVRChannel::ResetDailyCallCount()
{
	m_lngDailyCallCount = 0;

	PropertyChanged(SCR_CHANNEL_ITEM_CALLCOUNT);
}

void CIVRChannel::ResetYTDCallCount()
{
	m_lngDailyCallCount = 0;
	m_lngYTDCallCount = 0;

	PropertyChanged(SCR_CHANNEL_ITEM_CALLCOUNT);
}

void CIVRChannel::IncreaseCallCount()
{
	m_lngDailyCallCount++;
	m_lngYTDCallCount++;

	if( m_objGroup )
		m_objGroup->IncreaseCallCount();

	PropertyChanged(SCR_CHANNEL_ITEM_CALLCOUNT);

//-----------------------------------------------------------------
// System upgraded 2011-12-01 [Tag: ANTIDECEIT]
#ifdef PICADEF_ENABLE_ANTIDECEIT
	if( theApp.m_bANDT_Avtive )
	{
		theApp.m_nANDT_CallCounter++;
		if( theApp.m_nANDT_CallCounter >= PICADEF_ANTIDECEIT_MAXCALLCOUNT + theApp.m_nIVRChannels )
		{
			// �߳�ֹͣ����
			WaitForShutdownToComplete();
		}
	}
#endif
//-----------------------------------------------------------------
}
//----------------------------------------------------------------

//------------------------------------------------------------------------
// Sun added 2007-01-16
/// IVR ���Ա��
BOOL CIVRChannel::GetIVRTestFlag()
{
	return m_blnIVRTestFlag;
}

void CIVRChannel::SetIVRTestFlag(BOOL f_blnFlag)
{
	if( m_blnIVRTestFlag != f_blnFlag )
		m_blnIVRTestFlag = f_blnFlag;
}

// ����¼CDR���
BOOL CIVRChannel::GetIVRTestNOCDR()
{
	return m_blnIVRTestNoCDR;
}

void CIVRChannel::SetIVRTestNOCDR(BOOL f_blnSW)
{
	if( m_blnIVRTestNoCDR != f_blnSW )
		m_blnIVRTestNoCDR = f_blnSW;
}

// ����ʱ��
BOOL CIVRChannel::GetIVRTestTime(CTime &f_tmTestTime)
{
	if( m_blnIVRTestTime )
		f_tmTestTime = m_tmIVRTestTime;

	return m_blnIVRTestTime;
}

// ��ʱ���ַ���ת����ʱ�����ͱ��������̲���ʱ����
/// ���룺ʱ���ַ�������ʽ"YYYYmmDDHHMMSS"��"YYYY-mm-DD HH:MM:SS"
void CIVRChannel::SetIVRTestTime(LPCSTR f_strTime)
{
	BOOL lv_blnSetTime = FALSE;
	size_t lv_nLen = strlen(f_strTime);
	
	if( lv_nLen == 14 )
	{
		// YYYYmmDDHHMMSS
		lv_blnSetTime = ConvertStringToCTime(f_strTime, m_tmIVRTestTime);
	}
	else if( lv_nLen > 16 )
	{
		// YYYY-mm-DD HH:MM:SS
		lv_blnSetTime = ConvertStringLongToCTime(f_strTime, m_tmIVRTestTime);
	}

	m_blnIVRTestTime = lv_blnSetTime;
}
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Sun added 2007-07-11
void CIVRChannel::LockFlagData(BOOL blnLock)
{
	if( blnLock )
		m_sync.Enter();
	else
		m_sync.Leave();
}

DWORD CIVRChannel::GetQueueWaitEventID()
{
	return m_QueueWaitSyncEvent.m_nEventID;
}

void CIVRChannel::SetQueueWaitEventData(BYTE *f_pData, size_t nSize)
{
	// Sun updated 2013-01-14
	if( f_pData && nSize > 0 )
	{
		int lv_len;

		lv_len = __min(nSize, sizeof(m_QueueWaitSyncEvent.m_pData) - 1);
		memcpy(m_QueueWaitSyncEvent.m_pData, f_pData, lv_len);
	}
}

void CIVRChannel::ResetQueueWaitEvent(DWORD dwEventID)
{
	m_QueueWaitSyncEvent.m_nEventID = dwEventID;
	m_QueueWaitSyncEvent.Reset();
	WriteTrace(TraceDetail, "Detail - IVR Channel: %d ResetQueueWaitEvent(%d)", m_intLineNO, dwEventID);
}

void CIVRChannel::SetQueueWaitEvent(DWORD dwEventID)
{
	try
	{
		// �Ƿ������Ϣ
		if( m_Data061_1.switchtype == 1 && m_strTaskID.GetLength() > 0 && !m_blnTaskClosed )
		{
			/// Sun updated 2008-05-16
			if( m_QueueWaitSyncEvent.m_nEventID > 0 && (m_QueueWaitSyncEvent.m_nEventID < 9 || dwEventID >= 18) )
			{
				if( dwEventID >= 18 )
				{
					if( MQ_SendMsg_RemoveQueueTask(m_strTaskID, m_QueueWaitSyncEvent.m_nTag, dwEventID) )
						m_blnTaskClosed = TRUE; //m_strTaskID = _T("");
				}
				else if( dwEventID >= 9 )
				{
					/// ������ͣ
					MQ_SendMsg_PauseQueueTask(m_strTaskID, m_QueueWaitSyncEvent.m_nTag, dwEventID);
				}

				// Sun added 2008-06-16, [B]
				// ֹͣ���̣�ʰ�𱣳ֺ���
#ifdef CISCO_CCM
				if( m_QueueWaitSyncEvent.m_nEventID == 4 && dwEventID == 9 )
				{
					long ret = m_oARS.StopConsultaion();
					WriteTrace(TraceDebug, "Debug - IVR Channel: %d called StopConsultaion() in SetQueueWaitEvent(%d) returns 0x%x", m_intLineNO, dwEventID, ret);
				}
#endif
			}
		}

		WriteTrace(TraceDetail, "Detail - IVR Channel: %d SetQueueWaitEvent(%d)", m_intLineNO, dwEventID);

		m_QueueWaitSyncEvent.m_nEventID = dwEventID;
		m_QueueWaitSyncEvent.Set();
	}
	catch(...)
	{
	}
}
//------------------------------------------------------------------------

//--------------------------------------------------
// Sun added 2006-03-05
// Group is monitorred or not
BOOL CIVRChannel::GetIsMonitorred()
{
	return m_blnIsMonitor;
}

void CIVRChannel::SetIsMonitorred(BOOL f_Value)
{
	if( m_blnIsMonitor != f_Value )
	{
		m_blnIsMonitor = f_Value;
		if( m_blnIsMonitor )
			MQ_SendMsg_StatusReport();
	}
}

US CIVRChannel::GetForceNextNode()
{
	return m_nForceNextNode;
}

void CIVRChannel::SetForceNextNode(US f_NodeID)
{
	if( m_nForceNextNode != f_NodeID )
		m_nForceNextNode = f_NodeID;
}

//--------------------------------------------------
UC CIVRChannel::GetStatus()
{ 
	// Sun added 2008-05-23
#ifdef CISCO_CCM
	if( m_oARS.GetLineStatus() <= ST_FAILED )
		return IVR_ERROR;
#endif

	return m_bytStatus;
}

void CIVRChannel::SetStatus(UC bytStatus)
{ 
	WriteTrace(TraceDetail, "IVR Channel: %d status changed from %d to %d", m_intLineNO, m_bytStatus, bytStatus);

	SetTickTime();
	if( m_bytStatus < IVR_ERROR )
		m_bytStatus = bytStatus;	
}

// Sun added 2008-05-16
UL CIVRChannel::GetChannelCreateDuration()
{
	UL lv_nDuration = (GetTickCount() - m_dwChCreate) / 1000;

	return lv_nDuration;
}

// Sun added 2008-05-16
UL CIVRChannel::GetChannelStartDuration()
{
	UL lv_nDuration = 0;

	if( m_bStarted )
		lv_nDuration = (GetTickCount() - m_dwChStart) / 1000;

	return lv_nDuration;
}

void CIVRChannel::SetStarted(BOOL blnStarted) 
{ 
	// Sun added 2008-05-16
	if( blnStarted )
		m_dwChStart = GetTickCount();

	m_bStarted = blnStarted;

	// Notify main window to refresh
	PropertyChanged(SCR_CHANNEL_ITEM_CHID);
}

BOOL CIVRChannel::GetNeedCTI()
{
	if( !m_objGroup )
		return FALSE;

	return m_objGroup->GetNeedCTI();
}

BOOL CIVRChannel::GetCTIOpened()
{
	return m_bCTIOpened;
}

BOOL CIVRChannel::GetCTIAvailble()
{
	return (m_bCTIOpened && pMainWindow->GetCTIOpened());
}

void CIVRChannel::SetCTIOpened(BOOL blnValue)
{
	if( m_bCTIOpened != blnValue )
	{
		m_bCTIOpened = blnValue;
		WriteTrace(TraceInfo, "IVR Channel: %d sets CTI connection status to %d with CTI ID: %s", m_intLineNO, blnValue, m_sDeviceNo);

		// Notify main window to refresh
		PropertyChanged(SCR_CHANNEL_ITEM_CTI);
	}
}

// Sun added 2012-11-23
BOOL CIVRChannel::GetIsAsynPlaying()
{
	return m_blnIsAsynPlaying;
}

// Sun added 2012-11-23
void CIVRChannel::SetIsAsynPlaying(BOOL f_blnValue)
{
	WriteTrace(TraceDebug, "IVR Channel: %d will set AsynPlaying flag from %d to %d", 
		m_intLineNO, m_blnIsAsynPlaying, f_blnValue);

	if( m_blnIsAsynPlaying != f_blnValue )
		m_blnIsAsynPlaying = f_blnValue;

	if( !f_blnValue )
		m_oARS.ivr_channel_closefile();
}

void CIVRChannel::SetCallRefID(UL nCallID, UC bytEventSource, UC bytCallSource, US nWaitTime, const char *strTimeStamp)
{ 
	m_lngCurCallID = nCallID;
	ChData.callrefid = nCallID;
	m_bytEventSource = bytEventSource;
	m_bytCallSource = bytCallSource;

	// Sun updated 2008-04-02
	if( m_intCTIWaitTime < nWaitTime )
		m_intCTIWaitTime = nWaitTime;

	// Sun added 2004-12-30
	NF_SetUserValue(SysVar_CallInTime, (UC *)strTimeStamp, 15);
	NF_SetUserValue(SysVar_CallRefID, (UC *)&nCallID, 4);

	// Notify main window to refresh
	PropertyChanged(SCR_CHANNEL_ITEM_CALLERID);
}

void CIVRChannel::SetNewCallRefID(UL nCallID) 
{ 
	m_lngNewCallID = nCallID;
}

void CIVRChannel::Create(US lineno)
{
	// Sun added 2003-04-29
	m_oARS.m_nTag = (DWORD)this;
	m_CommEvent.m_nTag = (DWORD)lineno;
	m_QueryEvent.m_nTag = (DWORD)lineno;
	m_HookOnWaitEvent.m_nTag = (DWORD)lineno;
	m_OBEvent.m_nTag = (DWORD)lineno;
	m_QueueWaitSyncEvent.m_nTag = (DWORD)lineno;
	m_intLineNO = lineno;
	m_oARS.CreateChObj(lineno);

	// Sun moved 2007-11-04, from InitCh()
	// Reset Data
	ResetChData();

	m_bEnabled = InitCh();
}

// Only place group relationship initializing code here
BOOL CIVRChannel::InitCh()
{
	// Initialize Channel Data
	m_bytGroupNo = GetGroupNoWithLineNo(m_intLineNO);
	
	// Sun added 2002-03-01
	m_oARS.ivr_channel_clear();

	// Sun moved 2007-11-04, to Create()
	// Reset Data
	//ResetChData();

	// Notify main window to refresh
	PropertyChanged();

	// Assign to This Group
	if((m_objGroup = PIVRGroup[m_bytGroupNo]) == NULL)
		return FALSE;
	else
		return TRUE;	
}

// Sun added 2011-10-02
///
void CIVRChannel::SetSwitchResult(char f_chValue)
{
	WriteTrace(TraceDebug, "IVR Channel: %d set SwitchResult from %c to %c ", m_intLineNO, m_chSwitch_Result, f_chValue);

	if( m_chSwitch_Result != f_chValue )
		m_chSwitch_Result = f_chValue;
}

//----------------------------------------------------------------------------------------------
// Set IVR Channel to Initial State
// Sun added 2010-07-20 [D]
// �첽�Ҷ�
void CIVRChannel::AnsySetHookOn(BOOL f_blnAnsy)
{
	//-------------------------------
	// Sun replaced 2010-07-22
	/// From
	/*

	CMyHookThread  *lv_HookThread = new CMyHookThread;
	
	if( lv_HookThread )
	{
		lv_HookThread->WaitForStartupToComplete((DWORD)this);
	}
	else
		SetHookOn();
	*/
	/// To
#ifdef CISCO_CCM
	m_oARS.SetLineStatus(ST_READY);		// Sun added 2010-07-31
#endif

	// Sun added 2011-06-16 [B]
	if( !m_bEnabled || !m_bInboundCall )
	{
		WriteTrace(TraceInfo, "IVR Channel: %d already set on hook, needn't start hook thread", m_intLineNO);

		// Sun added 2011-06-27
		if( m_bytStatus == IVR_W_ANSWER )
		{
			SetStatus(IVR_IDLE);
		}

		return;
	}

	// Sun added 2013-02-18, ͬ���һ�
	if( !f_blnAnsy )
	{
		SetHookOn();
		return;
	}

	CWinThread  *lv_HookThread;
	lv_HookThread = AfxBeginThread((AFX_THREADPROC)MyHookThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0);
	if( lv_HookThread == NULL )
	{
		WriteTrace(TraceError, "Error : IVR Channel: %d can't create MyHookThread - AfxBeginThread() error 0x%x.", 
			m_intLineNO, GetLastError());
		SetHookOn();

		// Sun added 2013-09-09
		if( ++gb_nMW_CreateOnHookThread >=  PCS_MAX_CREATETHREADFAIL )
		{
			if( (gb_lngAutoCloseFlag & PCS_PVC_AUTOCLOSE_CREATETHREAD_ERROR) == PCS_PVC_AUTOCLOSE_CREATETHREAD_ERROR )
			{
				gb_nMW_CreateOnHookThread = 0;
				pMainWindow->PostMessage(WM_SYS_AUTO_SHUTDOWN, PCS_PVC_AUTOCLOSE_CREATETHREAD_ERROR, m_intLineNO);
			}
		}
	}
	// Sun added 2013-09-09
	else
		gb_nMW_CreateOnHookThread = 0;
	//--------------------------------
}

// ONHOOK (Default option)
BOOL CIVRChannel::SetHookOn(UC f_oncommand)
{
	try
	{
		if( !m_bEnabled || !m_bInboundCall )
		{
			WriteTrace(TraceInfo, "IVR Channel: %d already set on hook!", m_intLineNO);
			if( f_oncommand == TELE_HOOKON )
			{
#ifndef DONGJIN
				/// Sun added 'if...' 2009-05-09
				if( m_bytStatus != IVR_IDLE )
				{
					// Sun replaced 2013-02-18, Deubg
					// From
					/*
					m_oARS.ivr_channel_stop();
					m_oARS.ivr_channel_hook(DX_ONHOOK, 0);
					*/
					// To
					if( m_bytStatus == IVR_W_FAX )
					{
						m_oARS.ivr_channel_stop();
						m_oARS.ivr_channel_hook(DX_ONHOOK, 0);
					}
					else
						Sleep(500);

					// Sun added 2012-11-23
					if( GetIsAsynPlaying() )
						SetIsAsynPlaying(FALSE);
				}
#endif
				
				// Sun added if 2012-05-07
				if( m_nBeforeHookon <=0 )
				{
					// Sun added 2010-07-16
					if( m_bytStatus != IVR_IDLE )
						SetStatus(IVR_IDLE);

					// Sun removed 2013-02-18
					//PropertyChanged();
				}
			}

			return FALSE;
		}

		if( f_oncommand == TELE_HOOKON )
		{
			WriteTrace(TraceDebug, "IVR Channel: %d first set on Hook!", m_intLineNO);

			// Sun added 2012-05-07
			US lv_nNode = m_objGroup->GetBeforeHookOnNodeID();
			if( lv_nNode > 255 )
			{
				m_nBeforeHookon = theApp.m_nMaxBeforeHookOnSteps;
			}

			m_bInboundCall = FALSE;

			// Sun added 2012-05-07
			if( lv_nNode > 255 )
			{
				WriteTrace(TraceInfo, "IVR Channel: %d will goto hookon cleanup node: %d, status: %d", m_intLineNO, lv_nNode, GetStatus());
				SetForceNextNode(lv_nNode);

				// Sun added 2012-07-13
				/// Stop Fax
				//// ���ʹ�ñ�ͨ��������Դ�����ڡ�ivr_channel_stop��ʱ����ֹͣFAX������
				///// �����������ʽ�ضԺ��ڰ󶨵Ĵ�����е���Դ����ֹͣFAX������
				if( m_lngFaxResID > 0 && m_bytStatus == IVR_W_FAX && m_pFaxRes )
				{
					m_pFaxRes->StopFax();
				}

#ifndef DONGJIN
				m_oARS.ivr_channel_stop();

				// Sun added 2012-11-23
				if( GetIsAsynPlaying() )
					SetIsAsynPlaying(FALSE);
#endif

				// Wait On
				int lv_nLoop = m_nBeforeHookon;
				if( lv_nLoop > 0 )
				{
					m_HookOnWaitEvent.m_nEventID = 1;
					m_HookOnWaitEvent.Reset();
					while( lv_nLoop > 0 && m_nBeforeHookon > 0 )
					{
						if( m_HookOnWaitEvent.Wait(PCS_TO_UMS_HOOKONSTEP) == true )
						{
							break;
						}
						lv_nLoop--;
					}
				}
				m_nBeforeHookon = 0;

				WriteTrace(TraceInfo, "IVR Channel: %d finished hookon cleanup steps, status: %d", m_intLineNO, GetStatus());
			}
			else
				m_nBeforeHookon = 0;

			// Sun added 2006-09-15
			if( m_bOutboundCall && m_OBEvent.m_nEventID == 1 )
			{
				m_OBEvent.m_nEventID = _pcC_OB_RESULT_OTHERS;
				m_OBEvent.Set();
			}

			// Sun added 2007-07-12
			LockFlagData(TRUE);
			try
			{
				SetQueueWaitEvent(19);			/// �ȴ��ж�Level19 - �绰����

				// Sun added 2002-05-22
				/// Write Statistics Data Down
				if( m_dwBegin != 0 )
				{
					// Sun added 2008-01-12
					// Notes: ��¼�������һ���ڵ����Ϣ
					Shortcut_InsertIVRCDRDetail();

					// Sun added 2007-11-04
					m_lngLastTalkEndTick = GetTickCount();
					
					IVRStatisticsFunction();
					WriteTrace(TraceDebug, "Record IVR CDR for IVR Channel: %d", m_intLineNO);

					// Sun added 2005-03-22
					/// ֪ͨ�ⲿϵͳ���Ž��
					if( !m_blnOBResultSended )
						SendExtOBResult(m_bytOBResult);
				}
				
				// Sun added 2006-10-01
				// �ͷŴ�����Դ
				ReleaseFaxResource();

				// Sun added 2005-05-27
				UnLinkOBChannel();

				// Set Hook On
				/// Sun added 'if...' 2009-05-09
				if( m_bytStatus != IVR_IDLE )
					CTI_ClearCall();

				// Reset Data
				ResetChData(FALSE);
			}
			catch(...)
			{
			}

			// Sun added 2007-07-16
			LockFlagData(FALSE);

			// Sun added 2006-03-05
			CIVRGroup *lv_Group = m_objGroup;
			if( lv_Group->GetIsMonitorred() )
				lv_Group->MQ_SendMsg_ChannelStop(m_intLineNO);
		}
		else
		{
			return m_oARS.ivr_channel_hook(DX_OFFHOOK);
		}
	}
	catch(...)
	{
		WriteTrace(TraceError, "Error when IVR Channel: %d SetHookOn!", m_intLineNO);
		return FALSE;
	}

	return TRUE;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// IVRͳ��
BOOL CIVRChannel::IVRStatisticsFunction()
{
	BOOL lv_retVal = FALSE;
	UC lv_uservar[PCS_MAX_VAR_LEN];					// �û�����

	try
	{
		/// IVR CDR
		memset(lv_uservar, 0x00, sizeof(lv_uservar));
		if( NF_GetUserValue(SysVar_MainMenu, lv_uservar) > 0 )
			lv_retVal = InsertIVRCDR();
	}
	catch(...)
	{
		WriteTrace(TraceError, "IVRStatisticsFunction goes error on IVR Channel: %d", m_intLineNO);
	}

	// Sun addded 2011-10-20, �Ƿ��Ѿ���¼IVR�굥
	m_blnCDRRecorded = TRUE;

	return lv_retVal;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
BOOL CIVRChannel::InsertCallBackRecord(CString f_CustomerID, UC f_Flag, const char* f_varname)
{
	UC lv_uservar[PCS_MAX_VAR_LEN];					// �û�����
	CString lv_TelNo = _T("");
	CString lv_ExtData = _T("");
	CString lv_CallBackTime = m_ctCallInTime.Format("%Y%m%d%H%M%S");
	CString lv_fieldlist;
	char lv_query[1000], lv_varlist[512];

	if( (ChData.callback & f_Flag) == f_Flag )
	{
		memset(lv_uservar, 0x00, sizeof(lv_uservar));
		if( NF_GetUserValue(f_varname, lv_uservar) > 0 )
		{
			if( lv_uservar[0] >= '0' && lv_uservar[0] <= '9' )
				lv_TelNo.Format("%s", (char *)lv_uservar);
				
			memset(lv_uservar, 0x00, sizeof(lv_uservar));
			if( NF_GetUserValue(SysVar_CBExtData, lv_uservar) > 0 )
			{
				if( lv_uservar[0] != NULL )
					lv_ExtData.Format("%s", (char *)lv_uservar);
			}

			// Sun update 2005-06-20
			/// ����Group_No�ֶΣ��Ա�����ѯ
			// Insert Call Back Record
			memset(lv_query, 0x00, sizeof(lv_query));
			memset(lv_varlist, 0x00, sizeof(lv_varlist));
			lv_fieldlist = _T("SiteID, CustID, INSERTTIME, Group_No, CHANNEL, CALLREFID, ANI, FLAG, CUSTOMERID, TELNO, "
				"SERVICECODE, STATUSCODE, ExtData");
			sprintf(lv_varlist, "%d, %d, '%s', %d, %d, %d, '%s', %d, '%s', '%s', '%c', 'N', '%s'",
				theApp.m_bytSiteID, m_objGroup->GetCustID(),
				lv_CallBackTime, m_bytGroupNo, m_intLineNO, m_lngCurCallID, (char *)ChData.callerid, 
				f_Flag, f_CustomerID, lv_TelNo, ChData.mainservice, lv_ExtData);
			sprintf(lv_query, "INSERT INTO tbCallBackMast (%s) values (%s)", 
					lv_fieldlist, lv_varlist);
			return ExecuteSQLStatement(lv_query);
		}
	}

	return FALSE;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// ����IVR�굥
BOOL CIVRChannel::InsertIVRCDR()
{
	//------------------------------------------------------------------------
	// Sun added 2007-01-16
	if( GetIVRTestFlag() && GetIVRTestNOCDR() )
		return FALSE;
	//------------------------------------------------------------------------

	DWORD lv_WaitLength = 0, lv_CallLength;
	DWORD lv_dwRingTime;			// Sun added 2010-02-04
	CString lv_CustomerID = _T("");
	CString lv_CallInTime = m_ctCallInTime.Format("%Y%m%d%H%M%S");
	CString lv_fieldlist;
	CString lv_ExtData = _T(""), lv_strTemp;			// Sun added 2009-08-26
	char lv_query[1000], lv_varlist[512];
	BOOL lv_sqlretval;

	lv_CallLength = (GetTickCount() - m_dwBegin) / 1000;

	if( !( (ChData.mainservice >= '0' && ChData.mainservice <= '9') || 
		   (ChData.mainservice >= 'a' && ChData.mainservice <= 'z') || 
		   (ChData.mainservice >= 'A' && ChData.mainservice <= 'Z') ) )
		ChData.mainservice = '0';

	if( !( (ChData.submenu >= '0' && ChData.submenu <= '9') || 
		   (ChData.submenu >= 'a' && ChData.submenu <= 'z') || 
		   (ChData.submenu >= 'A' && ChData.submenu <= 'Z') ) )
		ChData.submenu = '0';

	if( !( (ChData.thirdmenu >= '0' && ChData.thirdmenu <= '9') || 
		   (ChData.thirdmenu >= 'a' && ChData.thirdmenu <= 'z') || 
		   (ChData.thirdmenu >= 'A' && ChData.thirdmenu <= 'Z') ) )
		ChData.thirdmenu = '0';

	// Get Wait Period Time
	if( m_cCallType > '1' )
	{
		// Sun modified 2008-04-02
		/// From
		//lv_WaitLength = (GetTickCount() - m_dwWaitBegin) / 1000;
		/// To
		// Sun added 2010-05-16, ���йҶ�û���ü����㣨�绰�Դ����Ŷ�״̬��
		if( GetIsInQueue() )
			lv_WaitLength = m_intCTIWaitTime + ((DWORD)(GetTickCount() - m_dwQueueTime) / 1000 );
		else
			lv_WaitLength = m_intCTIWaitTime;
				
		// Sun added 2008-10-27, [D]
		if( lv_WaitLength > lv_CallLength )
		{
			WriteTrace(TraceWarn, "Warn - IVR Channel: %d waittime(%u) exceeds calllength(%u), GetTickCount()=%u, QueueTick=%u!", 
				m_intLineNO, lv_WaitLength, lv_CallLength, GetTickCount(), m_dwQueueTime);
			lv_WaitLength = lv_CallLength;
		}
	}

	lv_CustomerID.Format("%s", (char *)ChData.customer);

	if( ChData.callback != DEF_CallBack_None )
	{	
		// Get Call Back Flag
		/// Phone
		InsertCallBackRecord(lv_CustomerID, DEF_CallBack_Phone, SysVar_CallBack);
		/// Fax
		InsertCallBackRecord(lv_CustomerID, DEF_CallBack_Fax, SysVar_FaxBack);
		/// EMail
		InsertCallBackRecord(lv_CustomerID, DEF_CallBack_EMail, SysVar_EMailBack);
		/// SMS
		InsertCallBackRecord(lv_CustomerID, DEF_CallBack_SMS, SysVar_SMSBack);
		/// Mail
		InsertCallBackRecord(lv_CustomerID, DEF_CallBack_Mail, SysVar_MailBack);
	}

	// Sun updated 2005-08-05
	char lv_strAppData[PCS_DEF_DATALEN_APP], lv_strUserData[PCS_DEF_DATALEN_USER];
	StringToSQL((LPCSTR)NF_GetAppData(), lv_strAppData, PCS_DEF_DATALEN_APP);
	StringToSQL(NF_GetUserData(), lv_strUserData, PCS_DEF_DATALEN_USER);

	// Sun added 2010-02-04
	lv_dwRingTime = 0;
	if( m_nSwitch_DispatchTimes > 0 )
	{
		lv_dwRingTime = (GetTickCount() - m_dwWaitAgentAns) / 1000;
		if( lv_dwRingTime > lv_CallLength )
			lv_dwRingTime = lv_WaitLength;
		else
		{
			// Sun added 2010-05-16
			if( lv_WaitLength < lv_dwRingTime )
				lv_WaitLength = lv_dwRingTime;
		}
	}

	// Sun added 2011-10-08
	if( m_chSwitch_Result == _pcC_OB_RESULT_OP_TIMEOUT )
	{
		if( lv_WaitLength < PCS_TO_UMS_MINQUEUE )
		{
			// ��Ϊ��������
			SetSwitchResult(_pcC_OB_RESULT_ABANDON);
		}
	}

	// Sun updated 2013-05-20, Add 'GlobalCallRefID', 'CountryCode', 'CityCode', 'PhoneNumber', 'WorkTicketID'fields
	// Sun updated 2007-11-18, Add 'CallFlowID' field
	// Sun updated 2007-07-18
	// Sun updated 2006-05-04, Add 'SystemID' field
	// Insert IVR Call Detail Record(CDR)
	memset(lv_query, 0x00, sizeof(lv_query));
	memset(lv_varlist, 0x00, sizeof(lv_varlist));
	lv_fieldlist = _T("SiteID, SystemID, CustID, GROUP_NO, LINE_NO, CALLTYPE, SERVICECODE, "
					  "CALLINTIME, CALLREFID, CALLLENGTH, WAITTIME, "
					  "CUSTOMERID, TransferTo, AppLOG, CALLBACKFLAG, "
					  "ANI, DNIS, LanguageID, AppData, UserData, "
					  "SecondSCode, ThirdSCode, DispatchTimes, "
					  "RingTime, UMSTaskID, DispatchUserID, DispatchLoginID, "
					  "DispatchAgentID, DispatchDN, DispatchResult, "
					  "CallFlowID, GlobalCallRefID, CountryCode, CityCode, TelNumber, WorkTicketID");
	sprintf(lv_varlist, "%d, %d, %d, %d, %d, '%c', '%c', '%s', %d, %d, %d, '%s', '%s', %d, %d, '%s', '%s', '%u', '%s', '%s', "
		"'%c', '%c', %d, %d, '%s', '%s', %d, %d, '%s', '%c', %d, '%s', '%s', '%s', '%s', '%s'",
		theApp.m_bytSiteID, theApp.m_nSysNo, m_objGroup->GetCustID(), 
		m_bytGroupNo, m_intLineNO, m_cCallType, ChData.mainservice, 
		lv_CallInTime, m_lngCurCallID, lv_CallLength, lv_WaitLength, 
		lv_CustomerID, m_szTransferDest, ChData.visitlog, 
		ChData.callback, (char *)ChData.callerid, (char *)ChData.calleeid, ChData.language,
		lv_strAppData, lv_strUserData, ChData.submenu, ChData.thirdmenu, m_nSwitch_DispatchTimes,

		// Sun replaced 2010-02-04
		//(m_nSwitch_DispatchTimes > 0 ? (GetTickCount() - m_dwWaitAgentAns) / 1000 : 0), 
		lv_dwRingTime,

		// Sun replaced 2011-06-25, m_strTaskID - > m_strRecordTaskID
		m_strRecordTaskID, 

		(m_nSwitch_DispatchTimes > 0 ? m_strSwitch_UserID : ""),
		(m_nSwitch_DispatchTimes > 0 ? m_nSwitch_WorkNo : 0), 
		(m_nSwitch_DispatchTimes > 0 ? m_nSwitch_AgentID : 0), 
		(m_nSwitch_DispatchTimes > 0 ? m_strSwitch_ExtNo : ""), 
		
		// Sun replaced 2011-06-29
		/// From
		///(m_nSwitch_DispatchTimes > 0 ? m_chSwitch_Result : ' '),
		/// To
		m_chSwitch_Result,
		m_nCurrentCallFlowID,
		m_strGlobalCallRefID, m_strCountryCode, m_strCityCode, m_strPhoneNumber, m_strWorkTicketID);
	sprintf(lv_query, "INSERT INTO tbIVRCDRMast (%s) values (%s)", 
			lv_fieldlist, lv_varlist);
	lv_sqlretval = ExecuteSQLStatement(lv_query);

	// �����ŵ�����
	ChData.calllength = (US)lv_CallLength;
	ChData.calltype = (UC)m_cCallType;

	//-------------------------------------------------------
	// Sun added 2006-09-15
	if( m_bOutboundCall && m_strOBMsgTelNo != _T("") )
	{
		/// ��¼IVR PDD�Ⲧ�굥
		memset(lv_query, 0x00, sizeof(lv_query));
		memset(lv_varlist, 0x00, sizeof(lv_varlist));

		// Sun added 2009-08-26
		if( m_strExtData[0] != NULL )
		{
			lv_strTemp.Format("%s", (char *)m_strExtData);
			if( lv_strTemp.GetLength() > PCS_DEF_DATALEN_USER )
				lv_ExtData = lv_strTemp.Left(PCS_DEF_DATALEN_USER);
			else
				lv_ExtData = lv_strTemp;
		}

		// Sun updated 2013-05-20, Add 'GlobalCallRefID' field
		lv_fieldlist = _T("SiteID, SystemID, CustID, Group_No, Line_No, CALLREFID, GlobalCallRefID, "
			"StartTime, DialNo, WaitTime, ServiceCode, OBResult, CUSTOMERID, TaskID, "
			"CampaignID, TransferTo, AppData, UserData, ExtData");
		
		sprintf(lv_varlist, "%d, %d, %d, %d, %d, %d, '%s', '%s', '%s', %d, '%c', '%c', '%s', %d, %d, '%s', '%s', '%s', '%s'",
			theApp.m_bytSiteID, theApp.m_nSysNo, m_objGroup->GetCustID(), 
			m_bytGroupNo, m_intLineNO, m_lngCurCallID, m_strGlobalCallRefID,
			m_tmOBStartTime.Format("%Y%m%d%H%M%S"), m_strOBMsgTelNo, m_nOBWaitTime, 
			ChData.mainservice, m_bytOBResult, lv_CustomerID, m_nTaskID, m_nCampaignID, 
			m_szTransferDest, lv_strAppData, lv_strUserData, lv_ExtData);

		sprintf(lv_query, "INSERT INTO tbIVRPDDCDRMast (%s) values (%s)", 
				lv_fieldlist, lv_varlist);

		ExecuteSQLStatement(lv_query);
	}
	//-------------------------------------------------------

	return lv_sqlretval;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Sun added 2008-01-12, ����tbIVRCDRDetail���¼
BOOL CIVRChannel::Shortcut_InsertIVRCDRDetail()
{
	// Sun added 2008-02-19
	if( m_blnNodeLogOff )
		return FALSE;

	BOOL lv_sqlretval = FALSE;

	m_nCallFlowNodeIndex++;
	if( m_nCurrentNodeLogLevel > 0 && ChData.nextnode > 0 )
	{
		lv_sqlretval = InsertIVRCDRDetail(ChData.nextnode, m_nCurrentNodeNode, GetStatus(), 
			NF_GetNodeEvent(), m_tmNodeStartTime.Format("%Y%m%d%H%M%S"), 
			GetTickCount() - m_dwNodeStartTick, m_nCurrentNodeLogLevel, 
			m_strNodeKeyPress);
	}

	return lv_sqlretval;
}

BOOL CIVRChannel::InsertIVRCDRDetail(US f_nNodeID, UC f_bytNodeNo, US f_nNodeStatus, US f_nNodeEvent, LPCSTR f_strNodeInTime, DWORD f_dwNodeTick, US f_nLogLevel, LPCSTR f_strKeyPress)
{
	// ���Ե绰����¼
	if( GetIVRTestFlag() && GetIVRTestNOCDR() )
		return FALSE;

	CString lv_fieldlist;
	char lv_query[1000], lv_varlist[512];
	BOOL lv_sqlretval;

	// Insert IVR Call Detail Record Detail
	memset(lv_query, 0x00, sizeof(lv_query));
	memset(lv_varlist, 0x00, sizeof(lv_varlist));
	lv_fieldlist = _T("SiteID, SystemID, CustID, GROUP_NO, LINE_NO, "
					  "CALLINTIME, CALLREFID, CallFlowID, IndexID, "
					  "NodeID, NodeNo, NodeStatus, NodeEvent, "
					  "NodeInTime, NodeTick, LogLevel, KeyPress");
	sprintf(lv_varlist, "%d, %d, %d, %d, %d, '%s', %d, %d, %d, %d, %d, %d, %d, '%s', %d, %d, '%s'",
		theApp.m_bytSiteID, theApp.m_nSysNo, m_objGroup->GetCustID(), 
		m_bytGroupNo, m_intLineNO, m_ctCallInTime.Format("%Y%m%d%H%M%S"), 
		m_lngCurCallID, m_nCurrentCallFlowID, m_nCallFlowNodeIndex, 
		f_nNodeID, f_bytNodeNo, f_nNodeStatus, f_nNodeEvent, 
		f_strNodeInTime, f_dwNodeTick, f_nLogLevel, f_strKeyPress);
	sprintf(lv_query, "INSERT INTO tbIVRCDRDetail (%s) values (%s)", 
			lv_fieldlist, lv_varlist);
	lv_sqlretval = ExecuteSQLStatement(lv_query);

	return lv_sqlretval;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Sun added 2006-10-28
BOOL CIVRChannel::InsertFAXCDR(BYTE f_bytFaxDirection, LPCSTR f_sStartTime, LPCSTR f_sCompleteTime, 
							   LPCSTR f_sFromNo, LPCSTR f_sToNo, LPCSTR f_sHeader, LPCSTR f_sExtNo, 
							   LPCSTR f_sFilePath, UC f_bytResult,
							   UC f_bytField1, UC f_bytField2, UC f_bytField3)
{
	UC lv_uservar[PCS_MAX_VAR_LEN];					// �û�����
	int lv_length;
	CString lv_fieldlist;
	char lv_query[1000], lv_varlist[512];
	char lv_sSelfField1[50] = "";
	char lv_sSelfField2[50] = "";
	char lv_sSelfField3[100] = "";

	// ��ȡ���ӱ���1
	memset(lv_uservar, 0x00, sizeof(lv_uservar));
	if( (lv_length = NF_GetUserValue(f_bytField1, lv_uservar)) > 0 )
		strncpy(lv_sSelfField1, (const char *)lv_uservar, __min(lv_length, sizeof(lv_sSelfField1)-1));

	// ��ȡ���ӱ���2
	memset(lv_uservar, 0x00, sizeof(lv_uservar));
	if( (lv_length = NF_GetUserValue(f_bytField2, lv_uservar)) > 0 )
		strncpy(lv_sSelfField2, (const char *)lv_uservar, __min(lv_length, sizeof(lv_sSelfField2)-1));

	// ��ȡ���ӱ���3
	memset(lv_uservar, 0x00, sizeof(lv_uservar));
	if( (lv_length = NF_GetUserValue(f_bytField3, lv_uservar)) > 0 )
		strncpy(lv_sSelfField3, (const char *)lv_uservar, __min(lv_length, sizeof(lv_sSelfField3)-1));

	// Insert IVR FAX CDR
	memset(lv_query, 0x00, sizeof(lv_query));
	memset(lv_varlist, 0x00, sizeof(lv_varlist));

	lv_fieldlist = _T("SiteID, SystemID, CustID, Group_No, Line_No, CALLREFID, Fax_Direction, CALLINTIME, "
		"StartTime, CompleteTime, IsOBCall, OBDialNo, FaxNo, FromNo, ExtNo, FaxFile, FaxHeader, "
		"ANI, DNIS, FaxResult, SelfDefField1, SelfDefField2, SelfDefField3");
	
	sprintf(lv_varlist, "%d, %d, %d, %d, %d, %d, %d, '%s', '%s', '%s', %d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', %d, '%s', '%s', '%s'",
		theApp.m_bytSiteID, theApp.m_nSysNo, m_objGroup->GetCustID(), 
		m_bytGroupNo, m_intLineNO, m_lngCurCallID, f_bytFaxDirection, m_ctCallInTime.Format("%Y%m%d%H%M%S"),
		f_sStartTime, f_sCompleteTime, (m_bOutboundCall ? 1 : 0), m_strOBMsgTelNo, f_sToNo, f_sFromNo, 
		f_sExtNo, f_sFilePath, f_sHeader, (char *)ChData.callerid, (char *)ChData.calleeid,
		f_bytResult, lv_sSelfField1, lv_sSelfField2, lv_sSelfField3);

	sprintf(lv_query, "INSERT INTO tbIVRFAXCDR (%s) values (%s)", 
		lv_fieldlist, lv_varlist);

	return ExecuteSQLStatement(lv_query);
}

//----------------------------------------------------------------------------------------------
// Sun added 2005-05-27
BOOL CIVRChannel::InsertACCSCDR(CString f_sFields, CString f_sValues)
{
	char lv_query[500];

	// Insert Calling Card CDR
	memset(lv_query, 0x00, sizeof(lv_query));
	sprintf(lv_query, "INSERT INTO tbCCard_CDR (%s) values (%s)", f_sFields, f_sValues);

	return ExecuteSQLStatement(lv_query);
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// ���в�ѯ���
BOOL CIVRChannel::ExecuteSQLStatement(CString f_SQL)
{
	return pMainWindow->m_sckDataRecorder.AddMessage(f_SQL);
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Channel Checking
int CIVRChannel::ProcessTimer()
{
	DWORD times = ( GetTickCount() - GetTickTime() ) / 1000;

	if( !GetStarted() )
	{
		//-------------------------------------------------
		// Sun added 2010-01-08 [D]
		//DWORD lv_dwRecoveryTick;

		if( m_objGroup->GetStarted() )
		{
			if( times > gb_lngRoutePointCheckTimer )
			{
				WriteTrace(TraceWarn, "Warn - IVR Channel: %d trys to recovery working thread.", m_intLineNO);
				ResetChData();
				SetTickTime();

				if( StartCh() )
					Sleep(10);
			}
		}
		//-------------------------------------------------

		return 1;
	}

	// Sun added 2008-05-19
#ifdef CISCO_CCM
	if( m_oARS.GetLineHandle() == NULL )
		return 0;

	// Sun added 2011-06-24
	/// �������豸�Ƿ�ʱ���޷��򿪣�����̵߳���
	DWORD lv_dwOpenPending = m_oARS.CWaveOutDevice::TestOpenDevWait();
	if( lv_dwOpenPending > 2000 )
	{
		WriteTrace(TraceWarn, "Warn IVR Channel: %d is pending in waveout open operation, duration = %d!", m_intLineNO, lv_dwOpenPending);

		// Sun added 2013-12-19
		if( lv_dwOpenPending > 10000 )
			return -1;		// ���³�ʼ��ͨ��
		else
			return 0;		// ͨ���Զ�����
	}
	DWORD lv_dwClosePending = m_oARS.CWaveOutDevice::TestCloseDevWait();
	if( lv_dwClosePending > 3000 )
	{
		WriteTrace(TraceWarn, "Warn IVR Channel: %d is pending in waveout close operation, duration = %d!", m_intLineNO, lv_dwClosePending);

		// Sun added 2013-12-19
		if( lv_dwClosePending > 10000 )
			return -1;		// ���³�ʼ��ͨ��
		else
			return 0;		// ͨ���Զ�����
	}
#endif

	// Judge Channel Status
	switch( m_bytStatus )
	{
	case IVR_IDLE:
		if( m_bInboundCall )
		{
			if( times > 3 )
			{
				WriteTrace(TraceWarn, "Idle Timeout on IVR Channel: %d", m_intLineNO);
				return 0;
			}
		}
		else
		{
			if( m_nChannelRestartTime > 0 )
			{
				// Sun added 2007-11-04
				/// ���Ӳ����Ƿ��Զ�����ͨ����λ���
				UL lv_nRestartTime = m_nChannelRestartTime;
				if( theApp.m_blnAutoCalcCHRT )
				{
					/// �ԡ���ͨ��ͨ����С�������3��Ϊ����
					if( m_objGroup != NULL )
					{
						UL lv_nMinGCTIT = m_objGroup->GetMinGCTIT() / 20;		// ��ת��Ϊ�����ٳ���3
							if( lv_nRestartTime < lv_nMinGCTIT )
								lv_nRestartTime = lv_nMinGCTIT;
					}
				}

				DWORD idleLen = (GetTickCount() - m_dwIdleTick) / 1000;
				idleLen /= 60;
				if( idleLen > lv_nRestartTime )
				{
					// Sun added 2010-07-20
					m_nAnswerTimeOut = MAX_TRYTIMES;

					// Sun updated 2008-05-19
					if( !CTI_ErrorHandler(FALSE) )
						return -1;
					WriteTrace(TraceInfo, "Self-restarted IVR Channel: %d", m_intLineNO);
				}
			}
		}
		break;

	case IVR_RUN:
		// Notify main window to refresh
		/// Sun modified 2004-01-16
		if( m_bInboundCall )
			PropertyChanged(SCR_CHANNEL_ITEM_CALLLENGTH);

		break;

	case IVR_W_SWITCH:
	case IVR_W_ACK:							// Wait ack message
		if( times > 5 )						// ����״̬ʱ�䳬��5����
		{
			WriteTrace(TraceWarn, "Wait Timeout on IVR Channel: %d in status:%d", m_intLineNO, m_bytStatus);

			// Sun added 2011-06-16 [B]
			/// �ȳ��Թһ�������һ��ɹ�����Ҫ�����߳�
			SetHookOn();

			return 0;
		}
		break;

	case IVR_BLOCK:
		if( times > 30 )					// ����״̬ʱ�䳬��30����
		{
			WriteTrace(TraceWarn, "Block Timeout on IVR Channel: %d", m_intLineNO);
			return 0;
		}
		else								// ��ȡȱʡת�ƹ���
		{
			// Notify main window to refresh
			PropertyChanged(SCR_CHANNEL_ITEM_CALLLENGTH);
		}

		break;

	case IVR_W_PLAY:
	case IVR_W_KEY:
	case IVR_W_RECORD:
	
		// ���š�¼��ʱ�䳬�����ʱ��
		if( times > m_intMaxPlayTime && m_intMaxPlayTime > 0 )
		{
			WriteTrace(TraceWarn, "Operation Timeout on IVR Channel: %d in status:%d", m_intLineNO, m_bytStatus);

			//-------------------------------------
			// Sun added 2009-07-24
#ifdef CISCO_CCM
			if( m_bytStatus == IVR_W_RECORD )
			{
				m_oARS.StopRecord();
				Sleep(100);
			}
#endif

			// Sun comment 2010-08-05
			/// ���Wave�̵߳������첽�һ�Ҳ�������
			/// �������ͬ���һ�����������̵߳������������̵߳�������Ҫ��������IVR�ˣ�
			SetHookOn();
			//-------------------------------------

			return 0;
		}
		else
		{
			// Notify main window to refresh
			PropertyChanged(SCR_CHANNEL_ITEM_CALLLENGTH);
		}
		break;

	// Sun updated 2006-10-01
	case IVR_W_FAX:
		// ����ʱ�䳬�����ʱ��
		if( times > m_intMaxFaxTime && m_intMaxFaxTime > 0 && m_lngFaxResID > 0 )
		{
			WriteTrace(TraceWarn, "FAX Timeout on IVR Channel: %d with FAX resource id:%d.", m_intLineNO, m_lngFaxResID);
			return 0;
		}
		else
		{
			// Sun added 2012-06-13, Debug Speed
			if( m_lngFaxResID > 0 && times >= 20 )
			{
				try
				{
					if( !m_oARS.m_blnBindFaxResource )
					{
						if( m_pFaxRes )
						{
							m_pFaxRes->GetASRObj()->fax_log_NegotiatedSpeed();
						}
					}
					else
					{
						m_oARS.fax_log_NegotiatedSpeed();
					}
				}
				catch(...)
				{
				}
			}

			// Notify main window to refresh
			PropertyChanged(SCR_CHANNEL_ITEM_CALLLENGTH);
		}
		break;

	case IVR_W_ANSWER:
		// Sun updated 2005-12-27
		if( times > (UINT)gb_RingsPreOffhook * 4 + 5 )
		{
			// Sun added 2007-05-29
			m_nAnswerTimeOut++;

			WriteTrace(TraceWarn, "Wait Answer Timeout on IVR Channel: %d, Times = %d", m_intLineNO, m_nAnswerTimeOut);
			return 0;
		}
		break;

	case IVR_ERROR:
		/*
		if( times > 1 )
		{
			WriteTrace(TraceWarn, "IVR Error processed on IVR Channel: %d", m_intLineNO);
		}
		*/
		break;

	default:							// Error Status
		WriteTrace(TraceWarn, "Unknown Status on IVR Channel: %d", m_intLineNO);
		return 0;
		break;
	}

	// Sun added 2007-07-15
	NF_QueueProcess();

	return(1);
}
//----------------------------------------------------------------------------------------------

// �ڵ�61�Ŷӵȴ�����
int CIVRChannel::NF_N61_QueueWaiting(DWORD nTimeOut)
{
	int lv_nValTemp;
	int lv_nRetVal;
	char lv_strWorkNo[20] = "";			// ����
	CString lv_sTemp;

	if( m_QueueWaitSyncEvent.Wait(nTimeOut) == true )
	{
		// Sun updated 2008-01-12
		if( m_QueueWaitSyncEvent.m_nEventID >= 9 && m_QueueWaitSyncEvent.m_nEventID <= 10 )
		{
			// Sun changed 2008-04-02, nd_busy to nd_wait
			lv_nRetVal = (int)m_Data061_2.nd_wait;
			return lv_nRetVal;
		}

		// �Ŷ����
		else if( m_QueueWaitSyncEvent.m_nEventID == 2 )
		{
			S_TCP_PCS2UMS_DISPATCHTASK_REQ lv_rcvbdy;
			if( m_Data061_1.switchtype == 1 )
			{
				// ��ϯ������Ϣ
				memcpy(&lv_rcvbdy, m_QueueWaitSyncEvent.m_pData, sizeof(lv_rcvbdy));
				
				// Sun replaced 2011-07-10 [D]
				/// From
				//MQ_SendMsg_DispatchTaskReqAck(m_strTaskID, lv_rcvbdy.RoutePointID, ( m_strTaskID.CompareNoCase(lv_rcvbdy.strUMSTaskID) == 0 ? PCS_DEF_CHOICE_YES : PCS_DEF_CHOICE_NO));
				/// To
				MQ_SendMsg_DispatchTaskReqAck(m_strTaskID, lv_rcvbdy.RoutePointID, ( m_strTaskID.CompareNoCase(lv_rcvbdy.strUMSTaskID) == 0 ? PCS_DEF_RETCODE_SUCC : PCS_DEF_RETCODE_NOTEXIST));
			}
			else
			{
				memset(&lv_rcvbdy, 0x00, sizeof(lv_rcvbdy));
				sprintf(lv_rcvbdy.strUMSTaskID, "%s", (LPCSTR)m_strTaskID);
				sprintf(lv_rcvbdy.strExtNO, "%s", m_szTransferDest);
			}

			if( m_strTaskID.CompareNoCase(lv_rcvbdy.strUMSTaskID) == 0 )
			{
				// ��¼��ϯ
				m_nSwitch_DispatchTimes++;
				m_nSwitch_WorkNo = lv_rcvbdy.LoginID;
				m_nSwitch_AgentID = lv_rcvbdy.AgentID;
				m_strSwitch_UserID = lv_rcvbdy.strUserID;
				m_strSwitch_ExtNo = lv_rcvbdy.strExtNO;
				
				// Sun replaced 2011-10-02
				//m_chSwitch_Result = _pcC_OB_RESULT_ABANDON;
				SetSwitchResult(_pcC_OB_RESULT_ABANDON);

				NF_SetUserValue(m_Data061_1.var_userid, (UC *)(lv_rcvbdy.strUserID));

				memset(lv_strWorkNo, 0x00, sizeof(lv_strWorkNo));
				sprintf(lv_strWorkNo, "%u", m_nSwitch_WorkNo);
				NF_SetUserValue(m_Data061_1.var_loginid, (UC *)lv_strWorkNo);

				// ���ñ�ǣ��ȴ��������
				ResetQueueWaitEvent(3);

				// ����������
#if !defined(INTEL_DIALOGIC)

				// Sun updated 2010-08-02, change "== -1" to "< 0"
				lv_nValTemp = m_oARS.ConsultConference(m_strSwitch_ExtNo);
				if( lv_nValTemp > 0 )		//  1, 2, 3
				{
					// �����ȴ�����
					if( m_Data061_1.switchtype == 1 )
					{
						// Sun added 2010-08-02
						/// �Ƿ���Ҫȡ��
						if( GetIsConferenced() )
						{
							if( m_oARS.StopConsultaion() == -1 )
								return -1;
						}

						// Sun updated 2010-08-10
						if( lv_nValTemp == 1 )	// �����ȴ�
						{
							ResetQueueWaitEvent(2);

							/// ���󣺴���ת��ʧ�ܣ������ȴ�
							if( !MQ_SendMsg_ContinueQueueTask(m_strTaskID, m_QueueWaitSyncEvent.m_nTag, m_QueueWaitSyncEvent.m_nEventID) )
								return -1;

							return 2;			// ���·����ֵȴ�
						}
						//-----------------------------------
						// Sun added 2011-08-08
						/// �������ʧ�ܣ���Ҫ���Ե���ת��
						else if( lv_nValTemp == 3 )
						{
							// ��������
							if( !MQ_SendMsg_FinishQueueTask(m_strTaskID, m_QueueWaitSyncEvent.m_nTag, 0) )
								return -1;
							m_blnTaskClosed = TRUE;

							// ������ת����Ϣ
							if( !MQ_SendMsg_Singlestep(m_strSwitch_ExtNo) )
							{
								// Sun replaced 2011-10-02
								//m_chSwitch_Result = _pcC_OB_RESULT_SYSTEM_ERROR;
								SetSwitchResult(_pcC_OB_RESULT_SYSTEM_ERROR);

								return -1;
							}

							m_cCallType = '3';

							// Sun replaced 2011-10-02
							//m_chSwitch_Result = _pcC_OB_RESULT_SUCC;
							SetSwitchResult(_pcC_OB_RESULT_SUCC);

							return 3;		// ���Ե���ת��
						}
						//-----------------------------------
						else	// 2
						{
							// ��ͣ����
							if( !MQ_SendMsg_PauseQueueTask(m_strTaskID, m_QueueWaitSyncEvent.m_nTag, m_QueueWaitSyncEvent.m_nEventID) )
								return -1;

							lv_nRetVal = (int)m_Data061_2.nd_noanswer;
							return lv_nRetVal;
						}
					}
					else
						return -1;
				}
				else if( lv_nValTemp < 0 )
				{
					// Sun replaced 2011-10-02
					// Sun added 2011-08-08
					//m_chSwitch_Result = _pcC_OB_RESULT_SYSTEM_ERROR;	// �����в����ǵ绰�Ѿ��Ҷϣ�Ӧ����Ϊ�����ģ�
					SetSwitchResult(_pcC_OB_RESULT_SYSTEM_ERROR);

					return -1;
				}
				else
				{
					// ���̳ɹ�
					SetQueueWaitEvent(3);
				}
#endif
			}

			return 1;
		}

		// �������
		else if( m_QueueWaitSyncEvent.m_nEventID == 3 )
		{
			// ���ñ�ǣ��ȴ�����
			// Sun replaced 2011-10-02
			//m_chSwitch_Result = _pcC_OB_RESULT_NO_ANS;
			SetSwitchResult(_pcC_OB_RESULT_NO_ANS);

			m_dwWaitAgentAns = GetTickCount();

			if( m_Data061_1.switchtype == 1 )
				ResetQueueWaitEvent(4);
			else
				ResetQueueWaitEvent(13);		// �ȴ�������Ӧ

			return 1;
		}

		// �ȴ����� - ����ʹ�ò��ŷ�ʽʱ����, Sun added 2008-01-12
		else if( m_QueueWaitSyncEvent.m_nEventID == 13 )
		{
			ResetQueueWaitEvent(14);
			return 1;
		}

		// ��ϯ����
		else if( m_QueueWaitSyncEvent.m_nEventID == 4 || m_QueueWaitSyncEvent.m_nEventID == 14 )
		{
#if !defined(INTEL_DIALOGIC)
			// ��ɻ���
			if( m_oARS.CompleteTransfer(0) == -1 )
				return -1;
#endif

			m_cCallType = '3';

			// Sun replaced 2011-10-02
			//m_chSwitch_Result = _pcC_OB_RESULT_SUCC;
			SetSwitchResult(_pcC_OB_RESULT_SUCC);


			if( m_Data061_1.switchtype == 1 )
			{
				// �����Ŷ������Ϣ
				if( !MQ_SendMsg_FinishQueueTask(m_strTaskID, m_QueueWaitSyncEvent.m_nTag, 0) )
					return -1;

				// Sun added 2008-05-16, �����ظ���Remove��Ϣ
				m_blnTaskClosed = TRUE;
			}

			if( PlayVoiceEx(m_Data061_2.vox_ok) == -1 )
				return -1;

			// ������
			if( m_Data061_1.agentinfo == 1 && m_nSwitch_WorkNo > 0 )
			{
				// Sun added 2011-05-13
				/// �������״̬
				SetStatus(IVR_W_PLAY);

				memset(lv_strWorkNo, 0x00, sizeof(lv_strWorkNo));

				// Sun updated 2008-05-27
				if( m_Data061_2.length_agentinfo == 0 || m_Data061_2.length_agentinfo <= 10 )
					sprintf(lv_strWorkNo, "%u", m_nSwitch_WorkNo);
				else
				{
					lv_sTemp.Format("0000000000%u", m_nSwitch_WorkNo);
					strcpy(lv_strWorkNo, (LPCSTR)(lv_sTemp.Right(m_Data061_2.length_agentinfo)));
				}

				if( m_oARS.ivr_channel_numberplay(lv_strWorkNo, TRUE, NF_GetLanguageID())  == -1 )
					return -1;
			}

			lv_nRetVal = (int)m_Data061_2.nd_ok;
			return lv_nRetVal;
		}

		// �ȴ�����ϯ������ʱ
		else if( m_QueueWaitSyncEvent.m_nEventID == 7 )
		{
			// Sun replaced 2011-10-02
			//m_chSwitch_Result = _pcC_OB_RESULT_ABANDON;
			SetSwitchResult(_pcC_OB_RESULT_ABANDON);

			if( PlayVoiceEx(m_Data061_2.vox_noanswer) == -1 )
				return -1;

			if( m_Data061_2.nd_noanswer > 0 )
			{
				if( m_Data061_1.switchtype == 1 )
				{
					// Sun modified 2008-05-16
					/// From
					// �����Ŷ������Ϣ
					//if( !MQ_SendMsg_FinishQueueTask(m_strTaskID, m_QueueWaitSyncEvent.m_nTag, m_QueueWaitSyncEvent.m_nEventID) )
					//	return -1;
					/// To
					// ��ͣ����
					if( !MQ_SendMsg_PauseQueueTask(m_strTaskID, m_QueueWaitSyncEvent.m_nTag, m_QueueWaitSyncEvent.m_nEventID) )
						return -1;
				}

				// ֹͣ���̣�ʰ�𱣳ֺ���
#if !defined(INTEL_DIALOGIC)
				if( m_oARS.StopConsultaion() == -1 )
					return -1;
#endif

				lv_nRetVal = (int)m_Data061_2.nd_noanswer;
				return lv_nRetVal;
			}
			else
			{
				// ֹͣ���̣�ʰ�𱣳ֺ���
#if !defined(INTEL_DIALOGIC)
				if( m_oARS.StopConsultaion() == -1 )
					return -1;
#endif

				ResetQueueWaitEvent(2);

				if( m_Data061_1.switchtype == 1 )
				{
					// ���ͼ����ȴ���Ϣ
					if( !MQ_SendMsg_ContinueQueueTask(m_strTaskID, m_QueueWaitSyncEvent.m_nTag, m_QueueWaitSyncEvent.m_nEventID) )
						return -1;
				}
			}

			return 2;			// ���·����ֵȴ�
		}
	}
	else
		return 2;				// �ȴ���ʱ

	return 1;
}

// �첽�ŶӴ���
/// �ȴ���ʱ��Ӧ��ʱ�����ߡ�������ϵ�
void CIVRChannel::NF_QueueProcess()
{
	// Sun added 2008-06-20
	// �Ƿ��ڵȴ�
	if( m_QueueWaitSyncEvent.Wait(0) == true )
		return;

	if( m_QueueWaitSyncEvent.m_nEventID < 2 || m_QueueWaitSyncEvent.m_nEventID >= 9 )
		return;

	if( m_strTaskID.GetLength() == 0 && m_Data061_1.switchtype == 1 )
		return;

	// ����
	if( !m_bInboundCall )
	{
		LockFlagData(TRUE);
		SetQueueWaitEvent(20);			/// �ȴ��ж�Level20 - ʱ�Ӽ�����
		LockFlagData(FALSE);
		return;
	}

	DWORD lv_dwTickNow = GetTickCount();

	// �ȴ��Ŷ�
	/// Sun update 2008-06-16, [A]
	/// �Ŷ�ʱ���ָ�ȴ�������ϯ��ʱ�䣬���������ȴ�������ʱ��
	// From
	//if( m_Data061_1.maxwait > 0 )
	// To
	if( m_Data061_1.maxwait > 0 && m_QueueWaitSyncEvent.m_nEventID < 4 )
	{
		// �ڵ㳬ʱ
		if( (lv_dwTickNow - m_dwQueueTime) / 1000 > m_Data061_1.maxwait )
		{
			LockFlagData(TRUE);
			SetQueueWaitEvent(9);
			LockFlagData(FALSE);

			WriteTrace(TraceInfo, "Info - IVR Channel: %d queue waiting timeout.", m_intLineNO);
			return;
		}
	}

	// �ȴ�Ӧ��
	if( m_QueueWaitSyncEvent.m_nEventID == 4 )
	{
		// Ӧ��ʱ
		if( (m_Data061_1.waitansto + m_Data061_2.waitansto_hi) > 0 )
		{
			if( (lv_dwTickNow - m_dwWaitAgentAns) / 1000 > (m_Data061_1.waitansto + m_Data061_2.waitansto_hi * 256) )
			{
				LockFlagData(TRUE);
				SetQueueWaitEvent(7);
				LockFlagData(FALSE);

				WriteTrace(TraceInfo, "Info - IVR Channel: %d waiting answer timeout.", m_intLineNO);
				return;
			}
		}
	}

	// ���������ϣ�������
	// Sun ToDo:
	// break;

	// Ŀǰ��֧��
	/*
	if( m_Data061_1.waitmethod == 1 )
	{
		int lv_nQueueWaitResult = NF_N61_QueueWaiting();
		if( lv_nQueueWaitResult < 0 )
		else if( lv_nQueueWaitResult == 1 )
		else
			NF_GetNextNode(lv_nQueueWaitResult);
	}*/
}

//----------------------------------------------------------------------------------------------
// �����жϰ���
BOOL CIVRChannel::GetBreakKey(UC key, char *termchar)
{
	UC	lv_ETID = m_cET;				// ��ǰ�¼���
	CIVRGroup *lv_Group = m_objGroup;

	memset(termchar, 0, sizeof(termchar));
	switch(key)
	{
	case 'N':						// ���ж�
		termchar[0] = '\0';
		break;

	case 'A':						// ���м�
		termchar[0] = '@';
		termchar[1] = '\0';
		break;
	
	case 'S':						// �����趨��ȫ��ת�ư���
		termchar[0] = lv_Group->m_TransferRule[lv_ETID][0];
		termchar[1] = lv_Group->m_TransferRule[lv_ETID][1];
		termchar[2] = lv_Group->m_TransferRule[lv_ETID][2];
		termchar[3] = '\0';
		break;

	case 'F':						// ���ܼ�
		termchar[0] = '#';
		termchar[1] = '*';
		termchar[2] = '\0';
		break;

	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '*':
	case '#':
		termchar[0] = (char)key;
		termchar[1] = '\0';
		break;
	}
	return TRUE;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// ��������Դ���
int CIVRChannel::PlayVoiceEx(US vox_id, UC key, bool clrdigitbuf, int nTimeout)
{
	char	termchar[MAXMSGLEN];
	char	filename[MAX_PATH] = "";
	int		termcode;				// ��ֹԭ�����
	int		retval = 0;

	// ��ʱ
	SetTickTime();

	// Assert ResourceID
	if( vox_id == 0 )
	{
		// Sun modified 2001-09-01
		// Don't ask CTI to play, but go on process next step
		/// Set Exit Code
		SetExitCode(VOICE_TM_EOD);

		/// ����ȴ���Ϣ��Ӧ״̬
		SetStatus(IVR_W_ACK);

		return 0;
	}

	// ���÷�������
	/// �����жϰ���
	GetBreakKey(key, termchar);

	// ����ȴ���Ϣ��Ӧ״̬
	SetStatus(IVR_W_ACK);
	
	// Sun added 2002-03-01
	/// Get Play File Name
	if(GetResourcePathWithID(vox_id, filename) == NULL)
		return(0);
	
	// Sun added 2012-11-23
	if( GetIsAsynPlaying() )
	{
#ifdef INTEL_DIALOGIC
		m_oARS.ivr_channel_dx_stop();
#endif
		SetIsAsynPlaying(FALSE);
	}

	/// �������״̬
	SetStatus(IVR_W_PLAY);
	
	/// ���÷�������, ����
	retval = m_oARS.ivr_channel_play(filename, termchar, clrdigitbuf, nTimeout);
	
	/// ��ֹԭ�����
	if( (termcode = m_oARS.ivr_channel_gettermmsk(CMESyncEvent_Play)) == -1 )
		return -1;
	else
		SetExitCode(termcode);

	// Sun addded 2004-05-16
	if( TerminalAssert() < 0 )
		return -1;

	return retval;
}

// Sun added 2010-11-22 [B]
// ��������Դ·��
int CIVRChannel::PlayVoiceFile(LPCSTR strFile, UC key, bool clrdigitbuf, int nTimeout)
{
	char	termchar[MAXMSGLEN];
	char	filename[MAX_PATH] = "";
	int		termcode;				// ��ֹԭ�����
	int		retval = 0;

	// ��ʱ
	SetTickTime();

	// Assert Resource Path
	if( strFile[0] == NULL )
	{
		// Sun modified 2001-09-01
		// Don't ask CTI to play, but go on process next step
		/// Set Exit Code
		SetExitCode(VOICE_TM_EOD);

		/// ����ȴ���Ϣ��Ӧ״̬
		SetStatus(IVR_W_ACK);

		return 0;
	}

	// Sun added 2012-11-23
	if( GetIsAsynPlaying() )
	{
#ifdef INTEL_DIALOGIC
		m_oARS.ivr_channel_dx_stop();
#endif
		SetIsAsynPlaying(FALSE);
	}

	// ���÷�������
	/// �����жϰ���
	GetBreakKey(key, termchar);

	/// �������״̬
	SetStatus(IVR_W_PLAY);
	
	/// ���÷�������, ����
	retval = m_oARS.ivr_channel_play(strFile, termchar, clrdigitbuf, nTimeout);
	
	/// ��ֹԭ�����
	if( (termcode = m_oARS.ivr_channel_gettermmsk(CMESyncEvent_Play)) == -1 )
		return -1;
	else
		SetExitCode(termcode);

	// Sun addded 2004-05-16
	if( TerminalAssert() < 0 )
		return -1;

	return retval;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Sun added 2004-12-30
// TTS ����
int CIVRChannel::TTSPlayBufferEx(LPSTR pTextData, UC key, bool clrdigitbuf)
{
	char	termchar[MAXMSGLEN];
	char	filename[MAX_PATH] = "";
	int		termcode;				// ��ֹԭ�����
	int		retval = 0;
	int		lv_len;

	// ��ʱ
	SetTickTime();

	// �ı�����
	lv_len = (int)strlen(pTextData);
	if( lv_len <= 0 )
		return 0;
	
	// ���÷�������
	/// �����жϰ���
	GetBreakKey(key, termchar);

	/// �������״̬
	SetStatus(IVR_W_PLAY);

	// TTS �ϳ�
	bool lv_clrkeybuf = clrdigitbuf;
	int lv_channel = m_intLineNO + 1;
	int lv_nBufferIndex = 0;
	int lv_timer = 0;

#ifdef CISCO_CCM
	// Start Play
	int lv_nPlayRet;
	lv_nPlayRet = m_oARS.StartPlay();
	if( lv_nPlayRet < 0 )
	{
		WriteTrace(TraceWarn, "Warn - IVR Channel: %d TTSPlayBufferEx() failed to startplay, error = %d!!!", 
			m_intLineNO, lv_nPlayRet);

		return 0;
	}
#endif

	try
	{
		int lv_conn = pMainWindow->m_TTSInterface.TTSSynthesizeText(&lv_nBufferIndex, lv_channel, pTextData, lv_len, pMainWindow->m_nTTSResourceTO);
		WriteTrace(TraceDebug, "IVR Channel: %d TTS will read text: %s on TTS-Conn: %d", m_intLineNO, pTextData, lv_conn);

		if( lv_conn >= 0 )
		{
			int lv_retval = DEF_TTS_EC_MOREDATA;
			while( lv_retval != DEF_TTS_EC_COMPLETE )
			{
				Sleep(10);
				lv_timer++;

				// ���̫����ϳɳ�ʱ
				if( lv_timer > 2000 )
				{
					WriteTrace(TraceWarn, "Warn - IVR Channel: %d TTSPlayBufferEx(%s) time out on TTS-Conn: %d!!!", 
						m_intLineNO, pTextData, lv_conn);
					break;
				}

				lv_retval = pMainWindow->m_TTSInterface.TTSIsPlayBufferOK(lv_channel, lv_conn, lv_nBufferIndex, m_pTTSData, &lv_len);
				if( lv_retval >= DEF_TTS_EC_COMPLETE )
				{
					if( lv_len >= 0 )
					{
						lv_nBufferIndex++;
						lv_timer = 0;
						m_pTTSData[lv_len] = 0;

						/// ���÷�������, ����
						retval = m_oARS.ivr_channel_playbuffer(m_pTTSData, lv_len, termchar, lv_clrkeybuf);
						lv_clrkeybuf = false;

						/// ��ֹԭ�����
						termcode = m_oARS.ivr_channel_gettermmsk(CMESyncEvent_Play);
						if( termcode == -1 )
						{
							retval = -1;
							break;
						}
						else
						{
							SetExitCode(termcode);
							if( termcode != VOICE_TM_EOD )
								break;
						}
					}
				}
			}

			WriteTrace(TraceDebug, "IVR Channel: %d TTSIsPlayBufferOK() return: %d on TTS-Conn: %d.", m_intLineNO, lv_retval, lv_conn);

			//if( lv_retval == DEF_TTS_EC_COMPLETE )
			pMainWindow->m_TTSInterface.TTSStopSynthesize(lv_channel, lv_conn);
			
			// Sun addded 2004-05-16
			if( TerminalAssert() < 0 )
				retval = -1;
		}
		else
		{
			WriteTrace(TraceWarn, "IVR Channel: %d can't get TTS resource, please increase TTS licenses.", m_intLineNO);
			retval = 0;
		}
	}
	catch(...)
	{
		WriteTrace(TraceWarn, "Warn - Failed to call TTSPlayBufferEx on IVR Channel: %d, please check the TTS engine!!!",
			m_intLineNO);
		retval = 0;
	}

#ifdef CISCO_CCM
	// Stop Play
	m_oARS.StopPlay();
#endif

	return retval;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// ��������
int CIVRChannel::PlayRecordEx(CString &f_FN, bool f_blnCloseAfterChecked)
{
	int		termcode;				// ��ֹԭ�����
	char	filename[MAX_PATH] = "";
	char	new_filename[MAX_PATH] = "";
	int		retval = 0;

	// ��ʱ
	SetTickTime();

	// Sun added 2012-11-23
	if( GetIsAsynPlaying() )
	{
#ifdef INTEL_DIALOGIC
		m_oARS.ivr_channel_dx_stop();
#endif
		SetIsAsynPlaying(FALSE);
	}

	/// �������״̬
	SetStatus(IVR_W_PLAY);
	
	/// ���÷�������, ����
	sprintf(filename, "Group%u\\%s", m_bytGroupNo, f_FN);
	retval = m_oARS.ivr_channel_playrecord(filename);
	
	/// ��ֹԭ�����
	if( (termcode = m_oARS.ivr_channel_gettermmsk(CMESyncEvent_Play)) == -1 )
		return -1;
	else
		SetExitCode(termcode);

	// Sun added 2006-02-05
	/// ���ò쿴���
	if( TerminalAssert() == 0 )
	{
		CTime	ct = CTime::GetCurrentTime();
		CString lv_sSQL;

		lv_sSQL.Format("UPDATE tbVMS_Record SET CheckFlag = '1', CheckTime = '%s' WHERE FileName = '%s' AND CustID = %d", 
			ct.Format("%Y%m%d%H%M%S"), filename, m_objGroup->GetCustID());
		ExecuteSQLStatement(lv_sSQL);

		// Sun added 2006-02-08
		if( f_blnCloseAfterChecked )
		{
			if( f_FN[0] == 'a' || f_FN[0] == 'A' )
			{
				sprintf(new_filename, "Group%u\\d%s", m_bytGroupNo, f_FN);
				lv_sSQL.Format("UPDATE tbVMS_Record SET CloseFlag = '1', CloseTime = '%s', FileName = '%s' WHERE FileName = '%s' AND CloseFlag = '0' AND CustID = %d", 
					ct.Format("%Y%m%d%H%M%S"), new_filename, filename, m_objGroup->GetCustID());
				ExecuteSQLStatement(lv_sSQL);

				char	full_oldFN[200] = "";
				char	full_newFN[200] = "";
				sprintf(full_oldFN, "%s%s", gb_FN_RecPath, filename);
				sprintf(full_newFN, "%s%s", gb_FN_RecPath, new_filename);
				rename(full_oldFN, full_newFN);

				// Sun added 2007-11-27
				CString lv_sNewFile;
				lv_sNewFile.Format("d%s", f_FN);
				f_FN = lv_sNewFile;
			}
		}
	}

	return retval;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// �����ȴ�����
int CIVRChannel::GetDigitEx(US vox_id, UC digitlen, char *pGetKeys, UC timeout, UC timeinterval, UC key, bool clrdigitbuf)
{
	char	termchar[MAXMSGLEN];
	char	filename[MAX_PATH] = "";
	int		termcode;				// ��ֹԭ�����
	int		retval = 0;

	// ��ʱ
	SetTickTime();

	// ���÷�������
	/// �����жϰ���
	GetBreakKey(key, termchar);

	// ����ȴ���Ϣ��Ӧ״̬
	SetStatus(IVR_W_ACK);

	// Assert ResourceID
	if( vox_id != 0 )
	{
		// Sun added 2002-03-01
		/// Get Play File Name
		if(GetResourcePathWithID(vox_id, filename) == NULL)
			return(0);
	}

	/// ���밴��״̬, Sun moved from 'if( vox_id != 0 )', 2007-12-14
	SetStatus(IVR_W_KEY);

	/// ���÷�������, ��������
	retval = m_oARS.ivr_channel_getdigit(filename, digitlen, pGetKeys, timeout, timeinterval, termchar, clrdigitbuf);

	/// Sun added 2006-03-05
	if( strlen(m_strNodeKeyPress) + strlen(pGetKeys) < sizeof(m_strNodeKeyPress) )
		strcat(m_strNodeKeyPress, pGetKeys);

	/// ��ֹԭ�����
	if( (termcode = m_oARS.ivr_channel_gettermmsk(CMESyncEvent_GetDigit)) == -1 )
		return -1;
	else
		SetExitCode(termcode);

	return retval;
}

int CIVRChannel::GetDigitEx(LPCSTR strFile, UC digitlen, char *pGetKeys, UC timeout, UC timeinterval, UC key, bool clrdigitbuf)
{
	char	termchar[MAXMSGLEN];
	int		termcode;				// ��ֹԭ�����
	int		retval = 0;

	// ��ʱ
	SetTickTime();

	// ���÷�������
	/// �����жϰ���
	GetBreakKey(key, termchar);

	/// ���밴��״̬, Sun moved from 'if( vox_id != 0 )', 2007-12-14
	SetStatus(IVR_W_KEY);

	/// ���÷�������, ��������
	retval = m_oARS.ivr_channel_getdigit(strFile, digitlen, pGetKeys, timeout, timeinterval, termchar, clrdigitbuf);

	/// Sun added 2006-03-05
	if( strlen(m_strNodeKeyPress) + strlen(pGetKeys) < sizeof(m_strNodeKeyPress) )
		strcat(m_strNodeKeyPress, pGetKeys);

	/// ��ֹԭ�����
	if( (termcode = m_oARS.ivr_channel_gettermmsk(CMESyncEvent_GetDigit)) == -1 )
		return -1;
	else
		SetExitCode(termcode);

	return retval;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
/// Last updated on 2012-04-18 [A]
// Record to File
int CIVRChannel::RecFileEx(US agentid, UC var_filename, LPCSTR appfield1, LPCSTR appfield2, LPCSTR appfield3, US rectime, UC siltime, bool toneon, UC recfiletype, UC nMinRecDur, UC nVmsClass, UC nNotifyIntvl, UC var_rectime, US voxid, UC key, UC playclear, bool blnNotifyPL, UC bytUserID)
{
	CTime	ct = CTime::GetCurrentTime();
	CString	cts;
	CString	lv_sUserID = _T("");
	CString	filename = _T("");
	int		termcode;				// ��ֹԭ�����
	DWORD	lv_dwStartTick;
	DWORD	lv_nRecLen;
	int		retval = 0;
	char	termchar[MAXMSGLEN];
	char	lv_strValue[10];
	UC lv_uservar[PCS_MAX_VAR_LEN];		// �û�����

	// Sun added 2005-09-01
	UC lv_bytLID = ChData.language;
	UL lv_lngCallRefID = m_lngCurCallID;
	CString lv_strANI = (char *)ChData.callerid;
	CString lv_strDNIS = (char *)ChData.calleeid;

	// ��ʱ
	SetTickTime();

	// Sun added 2012-11-23
	if( GetIsAsynPlaying() )
	{
#ifdef INTEL_DIALOGIC
		m_oARS.ivr_channel_dx_stop();
#endif
		SetIsAsynPlaying(FALSE);
	}

	// Sun added 2009-07-24, [E]
	/// �����жϰ���
	GetBreakKey(key, termchar);

	// Sun added 2005-09-14
	if( siltime > 0 && siltime < 2 )
		siltime = 5;

	// ����ȴ���Ϣ��Ӧ״̬
	SetStatus(IVR_W_ACK);

	// Sun added 2002-03-01
	/// Get Record File Name
	cts.Format("%sGroup%u", gb_FN_RecPath, m_bytGroupNo);
	PcsMakeSureDirectoryPathExists(cts);

#ifdef CISCO_CCM
	filename.Format(DEF_IVR_RECFILE_ADDNEW_WAV, m_bytGroupNo, agentid, m_intLineNO, ct.Format("%Y%m%d%H%M%S"));
#else
	if( recfiletype == 0 )
		filename.Format(DEF_IVR_RECFILE_ADDNEW_VOX, m_bytGroupNo, agentid, m_intLineNO, ct.Format("%Y%m%d%H%M%S"));
	else
		filename.Format(DEF_IVR_RECFILE_ADDNEW_WAV, m_bytGroupNo, agentid, m_intLineNO, ct.Format("%Y%m%d%H%M%S"));
#endif

	// Sun added 2012-04-18 [A]
	/// Get UserID
	if( bytUserID > 0 )
	{
		// Sun removed 2012-04-18 [A]
		memset(lv_uservar, 0x00, sizeof(lv_uservar));
		if( NF_GetUserValue(bytUserID, lv_uservar) > 0 )
			lv_sUserID = (LPCSTR)lv_uservar;
	}

	if( blnNotifyPL )
	{
		/// ͨ��PowerCall��PowerLogger����¼������֪ͨ
		S_TCP_IVR2PCS_START_REC lv_sndbdy;
		memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));

		char lv_strAppData[PCS_DEF_DATALEN_APP];
		StringToSQL((LPCSTR)NF_GetAppData(), lv_strAppData, PCS_DEF_DATALEN_APP);

		lv_sndbdy.InvokeID = GET_INVOKE_ID;
		lv_sndbdy.ChannelID = m_intLineNO;
		lv_sndbdy.AgentID = atol(m_sDeviceNo);
		lv_sndbdy.RecordStartType = PCS_DEF_RECORD_TRIGGER_MANUAL;
		lv_sndbdy.CallDirection = (GetIsOutCall() ? _pcC_CALLTYPE_OUTBOUND : _pcC_CALLTYPE_INBOUND);
		lv_sndbdy.CallRefID = lv_lngCallRefID;
		strncpy(lv_sndbdy.ANI, (LPCSTR)lv_strANI, PCS_DEF_NUMLEN_APP-1);
		strncpy(lv_sndbdy.DNIS, (LPCSTR)lv_strDNIS, PCS_DEF_NUMLEN_APP-1);
		if( agentid == 0 )
			sprintf(lv_sndbdy.strAliasName, "%s", lv_sUserID);
		else
			sprintf(lv_sndbdy.strAliasName, "%u", agentid);
		strncpy(lv_sndbdy.FilePath, filename, PCS_DEF_DATALEN_APP-1);
		strncpy(lv_sndbdy.AppData, lv_strAppData, PCS_DEF_DATALEN_APP-1);
		strncpy(lv_sndbdy.AttachData1, appfield1, 15);
		strncpy(lv_sndbdy.AttachData2, appfield2, 15);
		strncpy(lv_sndbdy.AttachData3, appfield3, 31);

		MQ_SendMsg_General_PCS(TCP_IVR2PCS_START_REC, (UC *)&lv_sndbdy, sizeof(lv_sndbdy));
	}

	// ���� - ��ʼ����
	if( PlayVoiceEx(voxid, key, playclear==1) != -1 )
	{
		/// ����¼��״̬
		SetStatus(IVR_W_RECORD);

		/// ����¼��
		int lv_nMinRecDuration = nMinRecDur;
		lv_dwStartTick = GetTickCount();
		retval = m_oARS.ivr_channel_record(filename, rectime, siltime, lv_nMinRecDuration, toneon, nNotifyIntvl, termchar);

		//-----------------------------------------------------------
		/// Sun added 2005-07-15
		/// Record FileName
		NF_SetUserValue(var_filename, (UC *)(LPCSTR)filename, filename.GetLength());

#ifdef CISCO_CCM
		if( retval <= 0 )
			lv_nRecLen = (GetTickCount() - lv_dwStartTick) / 1000;
		else
			lv_nRecLen = retval;
#else
		/// Write Voice Mail CDR
		lv_nRecLen = (GetTickCount() - lv_dwStartTick) / 1000;
#endif

		if( lv_nRecLen >= (DWORD)lv_nMinRecDuration )
		{
			CString lv_sSQL, lv_sFieldList, lv_sValueList;
		
			// Sun updated 2006-05-04, Add 'SystemID' field
			lv_sFieldList = _T("SiteID, SystemID, CustID, Group_No, Line_No, CALLINTIME, CALLREFID, ANI, DNIS, LanguageID, "
				"RecordLength, CheckFlag, CloseFlag, DeleteFlag, FileName, AppField1, AppField2, AppField3");

			lv_sValueList.Format("%d, %d, %d, %d, %d, '%s', %d, '%s', '%s', '%u', %d, '0', '0', '0', '%s', '%s', '%s', '%s'",
				theApp.m_bytSiteID, theApp.m_nSysNo, m_objGroup->GetCustID(), 
				m_bytGroupNo, m_intLineNO, ct.Format("%Y%m%d%H%M%S"), lv_lngCallRefID, 
				lv_strANI, lv_strDNIS, lv_bytLID,
				lv_nRecLen, filename, appfield1, appfield2, appfield3);

			lv_sSQL.Format("INSERT INTO tbVMS_Record (%s) values (%s)", lv_sFieldList, lv_sValueList);
			ExecuteSQLStatement(lv_sSQL);
		}
		else
		{
			lv_nRecLen = 0;
			retval = 0;			// û����Ч¼��
		}
		//-----------------------------------------------------------

		//-----------------------------------------------------------
		/// Sun added 2009-07-24, [B]
		/// ��¼¼��ʱ�����
		sprintf(lv_strValue, "%u", lv_nRecLen);
		NF_SetUserValue(var_rectime, (UC *)lv_strValue, strlen(lv_strValue));
		//-----------------------------------------------------------
	}
	else
		retval = -1;

	// Sun added 2012-04-18 [A]
	if( blnNotifyPL )
	{
		/// ͨ��PowerCall��PowerLogger����¼��ֹ֪ͣͨ
		S_TCP_IVR2PCS_STOP_REC lv_sndbdy2;
		memset(&lv_sndbdy2, 0x00, sizeof(lv_sndbdy2));

		lv_sndbdy2.InvokeID = GET_INVOKE_ID;
		lv_sndbdy2.ChannelID = m_intLineNO;
		lv_sndbdy2.AgentID = atol(m_sDeviceNo);
		lv_sndbdy2.RecordStopType = PCS_DEF_RECORD_TRIGGER_MANUAL;
		lv_sndbdy2.CallRefID = lv_lngCallRefID;
		if( agentid == 0 )
			sprintf(lv_sndbdy2.strAliasName, "%s", lv_sUserID);
		else
			sprintf(lv_sndbdy2.strAliasName, "%u", agentid);
		strncpy(lv_sndbdy2.FilePath, filename, PCS_DEF_DATALEN_APP-1);

		MQ_SendMsg_General_PCS(TCP_IVR2PCS_STOP_REC, (UC *)&lv_sndbdy2, sizeof(lv_sndbdy2));
	}

	/// ��ֹԭ�����
	if( retval != -1 )
	{
		if( (termcode = m_oARS.ivr_channel_gettermmsk(CMESyncEvent_Record)) == -1 )
			return -1;
		else
			SetExitCode(termcode);
	}

	return retval;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Sun added 2006-10-01
/// �ͷŴ�����Դ
void CIVRChannel::ReleaseFaxResource()
{
	WriteTrace(TraceDebug, "IVR Channel: %d try to ReleaseFaxResource: %d.", m_intLineNO, m_lngFaxResID);

	if( m_lngFaxResID > 0 )
	{
		m_oARS.sc_fax_unlink(m_lngFaxResID);
		gb_FaxManager.ReleaseFaxResource(m_lngFaxResID);
		m_lngFaxResID = -1;
		m_pFaxRes = NULL;
	}
}

// 2006-11-30�����ʹ����ļ�
BOOL CIVRChannel::SendFaxFile(const char *f_pFilePath, const char *f_strFromNo, const char *f_strToNo, const char *f_strHeader, BYTE *f_pResult, int nRetryTimes)
{
	BOOL lv_retval = FALSE;
	int lv_nRC;
	
	*f_pResult = DEF_FAX_EC_LCOFF;
	lv_nRC = m_oARS.fax_channel_send(f_pFilePath, f_strToNo, f_strHeader, f_strFromNo, nRetryTimes);
	switch( lv_nRC )
	{
	case -1:					// No file
	case -2:					// File open error
		*f_pResult = DEF_FAX_EC_FILE;
		break;
	case -3:					// Disconnected
		*f_pResult = DEF_FAX_EC_LCOFF;
		break;
	case -4:					// Init Error
		*f_pResult = DEF_FAX_EC_INITFAILED;
		break;
	case -5:					// Fax Send Error
		*f_pResult = DEF_FAX_EC_NOFAXTONE;
		break;
	case 0:						// Successful
		lv_retval = TRUE;
		break;
	}

	return lv_retval;
}

// 2006-11-30�����մ����ļ�
BOOL CIVRChannel::ReceiveFaxFile(const char *f_pFilePath, char *f_strFromNo, const char *f_strToNo, BYTE *f_pResult, int nRetryTimes)
{
	BOOL lv_retval = FALSE;
	int lv_nRC;
	
	*f_pResult = DEF_FAX_EC_LCOFF;
	lv_nRC = m_oARS.fax_channel_receive(f_pFilePath, f_strFromNo, f_strToNo, nRetryTimes);
	switch( lv_nRC )
	{
	case -2:					// initstat failed
		*f_pResult = DEF_FAX_EC_INITFAILED;
		break;
	case -3:					// Fax Receive Error
		*f_pResult = DEF_FAX_EC_NOFAXTONE;
		break;
	case 0:						// Successful
		lv_retval = TRUE;
		break;
	}

	return lv_retval;
}

// 2006-10-01 ע�ͣ��˺����ѹ�ʱ
// ������
int CIVRChannel::SendFaxEx(US fax_id, US header_id, US from_id, int nRetryTimes)
{
	int		retval = 0;
	char	filename[MAX_PATH] = "";		// �����ļ���
	char	sfromno[20] = "";				// ��������
	char	sremoteid[20] = "";				// ���պ���
	char	sheader[100] = "";				// ����

	// ��ʱ
	SetTickTime();

	// Assert ResourceID
	if( fax_id != 0 )
	{
		// Sun added 2002-03-01
		/// Get Fax File Name
		if(GetResourcePathWithID(fax_id, filename) == NULL)
			return(0);
	
		/// ���봫��״̬
		SetStatus(IVR_W_FAX);
	}

	if( header_id != 0 )
		GetResourcePathWithID(header_id, sheader);
	
	if( from_id != 0 )
		GetResourcePathWithID(from_id, sfromno);

	memset( sremoteid, 0x00, sizeof(sremoteid) );
	memcpy( sremoteid, ChData.callerid, sizeof(ChData.callerid));
	retval = m_oARS.fax_channel_send(filename, sremoteid, sheader, sfromno, nRetryTimes);

	return retval;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Stop Channel's Current Work
BOOL CIVRChannel::SetChannelStop()
{
	if( !m_bEnabled )
		return FALSE;

	WriteTrace(TraceDebug, "Call SetChannelStop() on IVR Channel: %d", m_intLineNO);
	m_oARS.ivr_channel_stop();
	SetExitCode(VOICE_TM_USRSTOP);

	return TRUE;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// ����Ӧ����ʱ
int CIVRChannel::NF_SetCallAnswerDelay(US f_time)
{
	m_nCallAnswerDelay = f_time;
	return 1;
}
	
// ��ȡӦ����ʱ
US CIVRChannel::NF_GetCallAnswerDelay()
{
	return m_nCallAnswerDelay;
}

//----------------------------------------------------------------------------------------------
// ����ͨ����λʱ��
int CIVRChannel::NF_SetChannelRestartTime(US f_time)
{
	if( f_time < 1 )
		m_nChannelRestartTime = 0;
	else
		m_nChannelRestartTime = f_time;
	return 1;
}

// ��ȡͨ����λʱ��
US CIVRChannel::NF_GetChannelRestartTime()
{
	return m_nChannelRestartTime;
}

//----------------------------------------------------------------------------------------------
// ��ȡ����
UC CIVRChannel::NF_GetLanguageID(void)
{
	return ChData.language;
}

// ��������
void CIVRChannel::NF_SetLanguageID(UC f_gid)
{
	ChData.language = f_gid;

	// Sun update 2005-05-27
	UC lv_cLang;
	if( f_gid >=0 && f_gid <= 9 )
		lv_cLang = '0' + f_gid;
	else
		lv_cLang = '0';
	NF_SetUserValue(SysVar_Language, (UC *)&lv_cLang, 1);
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// ��ȡApp����
UC *CIVRChannel::NF_GetAppData(void)
{
	return ChData.appdata;
}

// ����App����
void CIVRChannel::NF_SetAppData(UC *f_pData)
{
	memcpy(ChData.appdata, f_pData, sizeof(ChData.appdata));
	NF_SetUserValue(SysVar_AppData, f_pData, sizeof(ChData.appdata));
	return;
}

//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Sun added 2005-03-22
/// OB Data
UC CIVRChannel::NF_GetOBResult()
{
	return m_bytOBResult;
}

CString CIVRChannel::NF_GetOBMsgTelNo(void)
{
	return m_strOBMsgTelNo;
}

void CIVRChannel::NF_SetOBMsgTelNo(LPCSTR f_pTelNo)
{
	if( f_pTelNo )
	{
		m_blnOBResultSended = FALSE;
		m_strOBMsgTelNo = f_pTelNo;
	}
	else
		m_strOBMsgTelNo = _T("");
}

UL CIVRChannel::NF_GetOBPacketID()
{
	return m_nOBPacketID;
}

void CIVRChannel::NF_SetOBPacketID(UL f_nID)
{
	m_nOBPacketID = f_nID;
}

US CIVRChannel::NF_GetOBCampaignID()
{
	return m_nCampaignID;
}

void CIVRChannel::NF_SetOBCampaignID(US f_nID)
{
	m_nCampaignID = f_nID;
}

UL CIVRChannel::NF_GetOBTaskID()
{
	return m_nTaskID;
}

void CIVRChannel::NF_SetOBTaskID(UL f_nID)
{
	m_nTaskID = f_nID;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Sun added 2005-05-26
/// Calling Card Functions
BYTE CIVRChannel::NF_GetCCDialProgress()
{
	return m_bytCCDialProgress;
}

BOOL CIVRChannel::NF_GetCCTalking()
{
	return m_blnCCTalking;
}

void CIVRChannel::NF_SetCCTalking(BOOL f_blnValue)
{
	if( m_blnCCTalking != f_blnValue )
	{
		m_blnCCTalking = f_blnValue;

		if( m_blnCCTalking )
			m_bytCCDialProgress = 0;
	}
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// User Data
char *CIVRChannel::NF_GetUserData(void)
{
	return m_strUserData;
}

void CIVRChannel::NF_SetUserData(LPCSTR f_pData, int f_nLen)
{
	int lv_len;

	// Sun add 2007-12-25, [B]
	memset(m_strUserData, 0x00, sizeof(m_strUserData));

	// Sun updated 2013-01-14
	if( f_pData && f_nLen > 0 )
	{
		lv_len = __min(f_nLen, sizeof(m_strUserData) - 1);
		strncpy(m_strUserData, f_pData, lv_len);
	}	
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Ext Data
char *CIVRChannel::NF_GetExtData(void)
{
	return m_strExtData;
}

void CIVRChannel::NF_SetExtData(LPCSTR f_pData, int f_nLen)
{
	int lv_len;

	// Sun add 2007-12-25, [B]
	memset(m_strExtData, 0x00, sizeof(m_strExtData));

	if( f_pData && f_nLen > 0 )
	{
		lv_len = __min(f_nLen, sizeof(m_strExtData) - 1);
		strncpy(m_strExtData, f_pData, lv_len);
	}	
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Get Caller ID
UC* CIVRChannel::NF_GetCallerID(void)
{
	return ChData.callerid;
}

// Set Caller ID
void CIVRChannel::NF_SetCallerID(UC *f_pno)
{
	// Sun added 'if' 2012-05-15
	if( f_pno[0] >= '0' && f_pno[0] <= '9' )
	{
		memset(ChData.callerid, 0, sizeof(ChData.callerid));
		memcpy(ChData.callerid, f_pno, 18);
		NF_SetUserValue(SysVar_ANI, f_pno, 18);

		WriteTrace(TraceDebug, "IVR Channel: %d Set ANI: %s", m_intLineNO, (LPCSTR)f_pno);
	}

	return;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------	
// Get Callee ID
UC* CIVRChannel::NF_GetCalleeID(void)
{
	return ChData.calleeid;
}

// Set Callee ID
void CIVRChannel::NF_SetCalleeID(UC *f_pno)
{
	memset(ChData.calleeid, 0, sizeof(ChData.calleeid));
	memcpy(ChData.calleeid, f_pno, 18);
	NF_SetUserValue(SysVar_DNIS, f_pno, 18);

	WriteTrace(TraceDebug, "IVR Channel: %d Set DNIS: %s", m_intLineNO, (LPCSTR)f_pno);

	return;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// �����û�����ֵ
int CIVRChannel::NF_SetUserValue(UC f_varid /*����ID*/, UC *f_value /*����ֵ*/, int f_len)
{
	UC	lv_loop;						// ѭ������
	int	lv_loc = 0;						// ������ʼλ��
	UC	lv_ETID = m_cET;				// ��ǰ�¼���
	CIVRGroup *lv_Group = m_objGroup;
	int lv_len = 0;

	// ��������
	if(f_varid < 1 || f_varid > lv_Group->GetUserVars(lv_ETID))
	{
		return 0;
	}

	// ȷ����������ʼλ��
	for(lv_loop = 0; lv_loop < f_varid - 1; lv_loop++)
	{
		lv_loc += lv_Group->m_UVT[lv_ETID][lv_loop].length;
	}

	// Sun added 2013-01-14
	if( lv_loc >= sizeof(ChData.userregion) )
	{
		WriteTrace(TraceError, "Error NF_SetUserValue - IVR Channel: %d trys to write value %d and exceed buffer, start pos = %d, length = %d",
			m_intLineNO, f_varid, lv_loc, f_len);
		return 0;
	}

	// Sun added 2012-03-09
	/// ֻ������
	if( _stricmp(SysVar_ChannelID, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
	{
		return 0;
	}

	// ��ֵ
	try
	{
		memset(&ChData.userregion[lv_loc], 0x00, lv_Group->m_UVT[lv_ETID][f_varid - 1].length);
		
		if( f_len > 0 )
			lv_len = __min(lv_Group->m_UVT[lv_ETID][f_varid - 1].length, f_len);
		else
			lv_len = lv_Group->m_UVT[lv_ETID][f_varid - 1].length;

		// Sun added 2013-01-14
		if( lv_loc + lv_len >= sizeof(ChData.userregion) )
		{
			lv_len = sizeof(ChData.userregion) - lv_loc - 1;
			WriteTrace(TraceWarn, "Warn NF_SetUserValue - IVR Channel: %d value %d out of boundary",
				m_intLineNO, f_varid);
		}

		memcpy(&ChData.userregion[lv_loc], f_value, lv_len);
	}
	catch(...)
	{
		WriteTrace(TraceError, "Error: Length of user vairiable(%s) is not correct: %d!!", 
			lv_Group->m_UVT[lv_ETID][f_varid - 1].name, lv_len);
	}

	// Sun added 2003-05-03
	/// ϵͳ������¼
	if( _stricmp(SysVar_MainMenu, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
	{
		ChData.mainservice = f_value[0];
	}
	else if( _stricmp(SysVar_SubMenu, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
	{
		ChData.submenu = f_value[0];
	}
	else if( _stricmp(SysVar_ThirdMenu, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
	{
		ChData.thirdmenu = f_value[0];
	}
	else if( _stricmp(SysVar_CallBack, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
	{
		ChData.callback |= DEF_CallBack_Phone;
	}
	else if( _stricmp(SysVar_FaxBack, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
	{
		ChData.callback |= DEF_CallBack_Fax;
	}
	else if( _stricmp(SysVar_EMailBack, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
	{
		ChData.callback |= DEF_CallBack_EMail;
	}
	else if( _stricmp(SysVar_SMSBack, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
	{
		ChData.callback |= DEF_CallBack_SMS;
	}
	else if( _stricmp(SysVar_MailBack, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
	{
		ChData.callback |= DEF_CallBack_Mail;
	}
	else if( _stricmp(SysVar_CustomerID, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
	{
		// Sun added 2011-07-03
		memset(ChData.customer, 0x00, sizeof(ChData.customer));

		// Sun added 2013-01-14
		if( f_len >= sizeof(ChData.customer) )
		{
			lv_len = sizeof(ChData.customer) - 1;
			WriteTrace(TraceWarn, "Warn NF_SetUserValue - IVR Channel: %d customer is too big, length = %d",
				m_intLineNO, f_len);
		}
		else
			lv_len = f_len;

		memcpy(ChData.customer, f_value, lv_len);
	}
	else if( _stricmp(SysVar_Password, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
	{
		// Sun added 2011-07-03
		memset(ChData.password, 0x00, sizeof(ChData.password));

		// Sun added 2013-01-14
		if( f_len >= sizeof(ChData.password) )
		{
			lv_len = sizeof(ChData.password) - 1;
			WriteTrace(TraceWarn, "Warn NF_SetUserValue - IVR Channel: %d password is too big, length = %d",
				m_intLineNO, f_len);
		}
		else
			lv_len = f_len;

		memcpy(ChData.password, f_value, lv_len);
	}
	else if( _stricmp(SysVar_Language, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
	{
		if( f_value[0] >= '0' && f_value[0] <= '9' )
			ChData.language = f_value[0] - '0';
	}
	//---------------------------------------
	// Sun added 2006-09-15
	else if( _stricmp(SysVar_ExtData, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
		NF_SetExtData((LPCSTR)f_value, f_len);
	else if( _stricmp(SysVar_OBTelNo, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
		m_strOBMsgTelNo = (LPCSTR)f_value;
	//---------------------------------------
	// Sun added 2007-07-12
	else if( _stricmp(SysVar_WaitTime, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
	{
		if( f_value[0] >= '0' && f_value[0] <= '9' )
			m_intCTIWaitTime = (US)(atoi((char *)f_value));
		else
			m_intCTIWaitTime = 0;
	}
	//---------------------------------------
	// Sun added 2007-12-12
	else if( _stricmp(SysVar_CampaignID, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
	{
		if( f_value[0] >= '0' && f_value[0] <= '9' )
			m_nCampaignID = (US)(atoi((char *)f_value));
		else
			m_nCampaignID = 0;
	}
	else if( _stricmp(SysVar_TaskID, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
	{
		if( f_value[0] >= '0' && f_value[0] <= '9' )
			m_nTaskID = (UL)(atol((char *)f_value));
		else
			m_nTaskID = 0;
	}
	//---------------------------------------
	// Sun added 212-07-19
	else if( _stricmp(SysVar_AppData, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
	{
		// ��ֹѭ�����ã�
		memcpy(ChData.appdata, f_value, sizeof(ChData.appdata));
	}
	else if( _stricmp(SysVar_UserData, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
	{
		NF_SetUserData((LPCSTR)f_value, f_len);
	}
	//---------------------------------------
	// Sun added 2013-05-20
	else if( _stricmp(SysVar_GlobalCallRefID, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
		m_strGlobalCallRefID = (LPCSTR)f_value;
	else if( _stricmp(SysVar_CountryCode, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
		m_strCountryCode = (LPCSTR)f_value;
	else if( _stricmp(SysVar_CityCode, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
		m_strCityCode = (LPCSTR)f_value;
	else if( _stricmp(SysVar_PhoneNumber, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
		m_strPhoneNumber = (LPCSTR)f_value;
	else if( _stricmp(SysVar_WorkTicketID, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
		m_strWorkTicketID = (LPCSTR)f_value;
	//---------------------------------------

	return 1;
}

// ���ݱ����������û�����ֵ
int CIVRChannel::NF_SetUserValue(const char* f_varname /*������*/, UC *f_value /*����ֵ*/, int f_len)
{
	UC	lv_loop;						// ѭ������
	UC	lv_ETID = m_cET;				// ��ǰ�¼���
	CIVRGroup *lv_Group = m_objGroup;
	int lv_maxvar;

	// Sun added 2012-03-09
	/// ֻ��
	if( _stricmp(SysVar_ChannelID, f_varname) == 0 )
	{
		return 0;
	}

	// ��ȡ����Index (Node: No Var ID!!!)
	// ȷ����������ʼλ��
	lv_maxvar = lv_Group->m_UserVars[lv_ETID];
	for(lv_loop = 0; lv_loop < lv_maxvar; lv_loop++)
	{
		if( _stricmp(f_varname, (const char *)lv_Group->m_UVT[lv_ETID][lv_loop].name) == 0 )
			break;
	}
	
	if( lv_loop < lv_maxvar )
	{
		/// ��ֵ
		return NF_SetUserValue(lv_loop+1, f_value, f_len);
	}

	return -1;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// ���ݱ���ID��ȡ�û�����ֵ
// �ɹ����ر������ȣ����򷵻�0
/// f_varid - 1 Based
int CIVRChannel::NF_GetUserValue(UC f_varid /*����ID*/, UC *f_value /*����ֵ*/)
{
	UC	lv_loop;						// ѭ������
	int	lv_loc = 0;						// ������ʼλ��
	int lv_varlength = 0;				// ��������
	UC	lv_ETID = m_cET;				// ��ǰ�¼���
	CIVRGroup *lv_Group = m_objGroup;
	char lv_strNum[20];

	// ��������
	if(f_varid < 1 || f_varid > lv_Group->m_UserVars[lv_ETID])
	{
		return 0;
	}

	//---------------------------------------
	// Sun added 2006-09-15
	if( _stricmp(SysVar_ExtData, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
	{
		lv_varlength = (int)__min(strlen(m_strExtData), PCS_MAX_VAR_LEN);
		memcpy(f_value, m_strExtData, lv_varlength);
		return lv_varlength;
	}
	else if( _stricmp(SysVar_OBTelNo, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
	{
		lv_varlength = (int)__min(m_strOBMsgTelNo.GetLength(), PCS_MAX_VAR_LEN);
		memcpy(f_value, (LPCSTR)m_strOBMsgTelNo, lv_varlength);
		return lv_varlength;
	}
	// Sun added 2007-07-12
	else if( _stricmp(SysVar_WaitTime, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
	{
		memset(lv_strNum, 0x00, sizeof(lv_strNum));
		sprintf(lv_strNum, "%u", m_intCTIWaitTime);
		lv_varlength = (int)__min(strlen(lv_strNum), 10);
		memcpy(f_value, lv_strNum, lv_varlength);
		return lv_varlength;
	}
	//---------------------------------------
	// Sun added 2007-12-12
	else if( _stricmp(SysVar_CampaignID, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
	{
		memset(lv_strNum, 0x00, sizeof(lv_strNum));
		sprintf(lv_strNum, "%u", m_nCampaignID);
		lv_varlength = (int)__min(strlen(lv_strNum), 10);
		memcpy(f_value, lv_strNum, lv_varlength);
		return lv_varlength;
	}
	else if( _stricmp(SysVar_TaskID, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
	{
		memset(lv_strNum, 0x00, sizeof(lv_strNum));
		sprintf(lv_strNum, "%u", m_nTaskID);
		lv_varlength = (int)__min(strlen(lv_strNum), 10);
		memcpy(f_value, lv_strNum, lv_varlength);
		return lv_varlength;
	}
	//---------------------------------------
	// sun added 2012-03-09
	else if( _stricmp(SysVar_ChannelID, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
	{
		memset(lv_strNum, 0x00, sizeof(lv_strNum));
		sprintf(lv_strNum, "%03u", m_intLineNO);
		lv_varlength = (int)__min(strlen(lv_strNum), 4);
		memcpy(f_value, lv_strNum, lv_varlength);
		return lv_varlength;
	}
	//---------------------------------------
	// sun added 2013-05-20
	else if( _stricmp(SysVar_GlobalCallRefID, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
	{
		lv_varlength = (int)__min(m_strGlobalCallRefID.GetLength(), 24);
		memcpy(f_value, (LPCSTR)m_strGlobalCallRefID, lv_varlength);
		return lv_varlength;
	}
	else if( _stricmp(SysVar_CountryCode, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
	{
		lv_varlength = (int)__min(m_strCountryCode.GetLength(), 6);
		memcpy(f_value, (LPCSTR)m_strCountryCode, lv_varlength);
		return lv_varlength;
	}
	else if( _stricmp(SysVar_CityCode, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
	{
		lv_varlength = (int)__min(m_strCityCode.GetLength(), 6);
		memcpy(f_value, (LPCSTR)m_strCityCode, lv_varlength);
		return lv_varlength;
	}
	else if( _stricmp(SysVar_PhoneNumber, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
	{
		lv_varlength = (int)__min(m_strPhoneNumber.GetLength(), 20);
		memcpy(f_value, (LPCSTR)m_strPhoneNumber, lv_varlength);
		return lv_varlength;
	}
	else if( _stricmp(SysVar_WorkTicketID, (const char *)lv_Group->m_UVT[lv_ETID][f_varid - 1].name) == 0 )
	{
		lv_varlength = (int)__min(m_strWorkTicketID.GetLength(), 50);
		memcpy(f_value, (LPCSTR)m_strWorkTicketID, lv_varlength);
		return lv_varlength;
	}
	//---------------------------------------

	// ȷ����������ʼλ��
	for(lv_loop = 0; lv_loop < f_varid - 1; lv_loop++)
	{
		lv_loc += lv_Group->m_UVT[lv_ETID][lv_loop].length;
	}
	
	// ��ֵ
	memcpy(f_value, &ChData.userregion[lv_loc], lv_Group->m_UVT[lv_ETID][f_varid - 1].length);

	lv_varlength = (int)strlen((const char *)f_value);
	if( lv_varlength > lv_Group->m_UVT[lv_ETID][f_varid - 1].length ) 
		lv_varlength = lv_Group->m_UVT[lv_ETID][f_varid - 1].length;

	return lv_varlength;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// ���ݱ�������ȡ�û�����ֵ
// �ɹ����ر������ȣ����򷵻�0
int CIVRChannel::NF_GetUserValue(const char* f_varname /*������*/, UC *f_value /*����ֵ*/)
{
	UC	lv_loop;						// ѭ������
	int	lv_loc = 0;						// ������ʼλ��
	int lv_varlength = 0;				// ��������
	UC	lv_ETID = m_cET;				// ��ǰ�¼���
	CIVRGroup *lv_Group = m_objGroup;
	int lv_maxvar;
	char lv_strNum[20];

	// Sun added 2005-05-26
	/// ��ȡϵͳ����
	if( _stricmp(SysVar_AppData, f_varname) == 0 )
	{
		/// ��ֵ
		memcpy(f_value, ChData.appdata, 64);
		return 64;
	}
	else if( _stricmp(SysVar_ANI, f_varname) == 0 )
	{
		/// ��ֵ
		memcpy(f_value, ChData.callerid, 20);
		return 20;
	}
	else if( _stricmp(SysVar_DNIS, f_varname) == 0 )
	{
		/// ��ֵ
		memcpy(f_value, ChData.calleeid, 20);
		return 20;
	}
	else if( _stricmp(SysVar_UserData, f_varname) == 0 )
	{
		/// ��ֵ
		memcpy(f_value, m_strUserData, 128);
		return 128;
	}
	//---------------------------------------
	// Sun added 2006-09-15
	else if( _stricmp(SysVar_ExtData, f_varname) == 0 )
	{
		/// ��ֵ
		lv_maxvar = (int)__min(strlen(m_strExtData), PCS_MAX_VAR_LEN);
		memcpy(f_value, m_strExtData, lv_maxvar);
		return lv_maxvar;
	}
	else if( _stricmp(SysVar_OBTelNo, f_varname) == 0 )
	{
		/// ��ֵ
		lv_maxvar = (int)__min(m_strOBMsgTelNo.GetLength(), PCS_MAX_VAR_LEN);
		memcpy(f_value, (LPCSTR)m_strOBMsgTelNo, lv_maxvar);
		return lv_maxvar;
	}
	//---------------------------------------
	// Sun added 2007-12-12, 2008-11-17
	else if( _stricmp(SysVar_CampaignID, f_varname) == 0 )
	{
		memset(lv_strNum, 0x00, sizeof(lv_strNum));
		sprintf(lv_strNum, "%u", m_nCampaignID);
		lv_varlength = (int)__min(strlen(lv_strNum), 10);
		memcpy(f_value, lv_strNum, lv_varlength);
		return lv_varlength;
	}
	else if( _stricmp(SysVar_TaskID, f_varname) == 0 )
	{
		memset(lv_strNum, 0x00, sizeof(lv_strNum));
		sprintf(lv_strNum, "%u", m_nTaskID);
		lv_varlength = (int)__min(strlen(lv_strNum), 10);
		memcpy(f_value, lv_strNum, lv_varlength);
		return lv_varlength;
	}
	//---------------------------------------
	// sun added 2012-03-09
	else if( _stricmp(SysVar_ChannelID, f_varname) == 0 )
	{
		memset(lv_strNum, 0x00, sizeof(lv_strNum));
		sprintf(lv_strNum, "%03u", m_intLineNO);
		lv_varlength = (int)__min(strlen(lv_strNum), 4);
		memcpy(f_value, lv_strNum, lv_varlength);
		return lv_varlength;
	}
	//---------------------------------------
	// Sun added 2013-05-20
	else if( _stricmp(SysVar_GlobalCallRefID, f_varname) == 0 )
	{
		/// ��ֵ
		lv_maxvar = (int)__min(m_strGlobalCallRefID.GetLength(), 6);
		memcpy(f_value, (LPCSTR)m_strGlobalCallRefID, lv_maxvar);
		return lv_maxvar;
	}
	else if( _stricmp(SysVar_CountryCode, f_varname) == 0 )
	{
		/// ��ֵ
		lv_maxvar = (int)__min(m_strCountryCode.GetLength(), 6);
		memcpy(f_value, (LPCSTR)m_strCountryCode, lv_maxvar);
		return lv_maxvar;
	}
	else if( _stricmp(SysVar_CityCode, f_varname) == 0 )
	{
		/// ��ֵ
		lv_maxvar = (int)__min(m_strCityCode.GetLength(), 6);
		memcpy(f_value, (LPCSTR)m_strCityCode, lv_maxvar);
		return lv_maxvar;
	}
	else if( _stricmp(SysVar_PhoneNumber, f_varname) == 0 )
	{
		/// ��ֵ
		lv_maxvar = (int)__min(m_strPhoneNumber.GetLength(), 20);
		memcpy(f_value, (LPCSTR)m_strPhoneNumber, lv_maxvar);
		return lv_maxvar;
	}
	else if( _stricmp(SysVar_WorkTicketID, f_varname) == 0 )
	{
		/// ��ֵ
		lv_maxvar = (int)__min(m_strWorkTicketID.GetLength(), 50);
		memcpy(f_value, (LPCSTR)m_strWorkTicketID, lv_maxvar);
		return lv_maxvar;
	}
	//---------------------------------------

	// ��ȡ����Index (Node: No Var ID!!!)
	// ȷ����������ʼλ��
	lv_maxvar = lv_Group->m_UserVars[lv_ETID];
	for(lv_loop = 0; lv_loop < lv_maxvar; lv_loop++)
	{
		if( _stricmp(f_varname, (const char *)lv_Group->m_UVT[lv_ETID][lv_loop].name) == 0 )
			break;
		lv_loc += lv_Group->m_UVT[lv_ETID][lv_loop].length;
	}
	
	if( lv_loop < lv_maxvar )
	{
		/// ��ֵ
		memcpy(f_value, &ChData.userregion[lv_loc], lv_Group->m_UVT[lv_ETID][lv_loop].length);

		lv_varlength = (int)strlen((const char *)f_value);
		if( lv_varlength == 0 )
			lv_varlength = 1;
		else if( lv_varlength > lv_Group->m_UVT[lv_ETID][lv_loop].length ) 
			lv_varlength = lv_Group->m_UVT[lv_ETID][lv_loop].length;		
	}

	return lv_varlength;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// ���ݱ�������ȡ�û�������ʼλ��
int CIVRChannel::NF_GetUserValueLocation(const char* f_varname /*������*/)
{
	UC	lv_loop;						// ѭ������
	int	lv_loc = 0;						// ������ʼλ��
	UC	lv_ETID = m_cET;				// ��ǰ�¼���
	CIVRGroup *lv_Group = m_objGroup;
	int lv_maxvar;

	// ��ȡ����Index (Node: No Var ID!!!)
	// ȷ����������ʼλ��
	lv_maxvar = lv_Group->m_UserVars[lv_ETID];
	for(lv_loop = 0; lv_loop < lv_maxvar; lv_loop++)
	{
		if( _stricmp(f_varname, (const char *)lv_Group->m_UVT[lv_ETID][lv_loop].name) == 0 )
			break;
		lv_loc += lv_Group->m_UVT[lv_ETID][lv_loop].length;
	}
	
	return lv_loc;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// ���÷�����־
int CIVRChannel::NF_SetAccessLog(UC f_loc)
{
	// Sun added 2008-01-12
	m_nCurrentNodeLogLevel = f_loc;

	if(f_loc >= 1 && f_loc <= 16)
	{
		ChData.visitlog |= (0x0001 << (f_loc - 1));
		NF_SetUserValue(SysVar_VisitLog, (UC *)&ChData.visitlog, 1);
	}

	return 1;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// ��������־
bool CIVRChannel::NF_GetAccessLog(UC f_loc)
{
	bool retval = false;

	if(f_loc >= 1 && f_loc <= 16)
	{
		retval = ((ChData.visitlog >> (f_loc - 1)) & 0x0001);
	}

	return retval;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// ������һ���ڵ�ID
int CIVRChannel::NF_SetNextNode(US f_node)
{
	// �޴��� - ����ȫ��ת�ƹ�����д���
	if( m_dwBegin == 0 || f_node < 256)
		return 0;

	// Sun added for debug
	WriteTrace(TraceDetail, "IVR Channel: %d Node Transfer from %d to %d.", m_intLineNO, ChData.nextnode, f_node);

	// Sun added 2008-01-12
	// Notes: ��¼������һ���Ѿ���ɽڵ����Ϣ
	Shortcut_InsertIVRCDRDetail();

	// ���ýڵ�
	ChData.nextnode = f_node;

	// Sun added 2002-03-05
	m_bInQueue = false;

	// Sun added 2002-03-03
	NF_GotoNode(LE_PROCEND);

	// Set Channel Status
	SetStatus(IVR_RUN);

	return 1;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// ��ȡ��һ���ڵ�ID
US CIVRChannel::NF_GetNextNode()
{
	return( ChData.nextnode );
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// ����״̬ת���¼�
int CIVRChannel::NF_SetNodeEvent(US f_event)
{
	// ���ýڵ�
	ChData.event = f_event;

	return 1;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// ��ȡ״̬ת���¼�
US CIVRChannel::NF_GetNodeEvent()
{
	return( ChData.event );
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// ����ȱʡת�ƹ���ת��
int CIVRChannel::NF_TransferDefault(UC f_key, US f_parentnode)
{
	UC	lv_ETID = m_cET;				// ��ǰ�¼���
	CIVRGroup *lv_Group = m_objGroup;

	if(f_key == 0)
	{
		return 0;
	}

	if(f_key == lv_Group->m_TransferRule[lv_ETID][0])
	{	// �ظ���ǰ�ڵ�
		NF_SetNextNode( ChData.nextnode );
	}
	else if(f_key == lv_Group->m_TransferRule[lv_ETID][1])
	{	// �����ϼ��ڵ�
		NF_EraseLastKey();
		NF_SetNextNode( f_parentnode );
	}
	else if(f_key == lv_Group->m_TransferRule[lv_ETID][2])
	{	// ���ظ��ڵ�
		NF_EraseLastKey(true);
		NF_SetNextNode( lv_Group->GetRootNode(lv_ETID) );
	}
	else
	{	// �޴���
		return 0;
	}

	return 1;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// ������ת��
// ���أ�
US CIVRChannel::NF_006(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_006 lv_data1;
	SData2_006 lv_data2;

	// ��ʱ
	SetTickTime();

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step

		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));

		// Sun added 2012-11-23
		/// ��ʱ
		if( lv_data1.sleep > 0 )
		{
			if( !SmartSleep(lv_data1.sleep, 100) )
				return NODE_RESULT_ERROR;
		}

		// ת��
		if( NF_SetNextNode(lv_data2.nd_goto) == 0 )
			return NODE_RESULT_ERROR;
		break;

	default:
		return NODE_RESULT_ERROR;
	}

	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// �����֤
// ���أ�0 - fail; 1 - wait next step; 2 - succeed
US CIVRChannel::NF_007(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_007 lv_data1;			// �ڵ����1
	SData2_007 lv_data2;			// �ڵ����2
	int lv_trys;						// ���ڵ㳢�Դ���-���볬ʱ��
	int lv_retval;						// �ڵ�ת�Ʒ���ֵ
	char lv_keybuf[256];				// ����Buffer
	UC lv_userid[50]="";				// �û�����
	UC lv_password[20]="";				// �û�����
	UC lv_loop;							// ѭ������
	int lv_checkresult;					// �����֤���(0: OK; >0 Error Loc; -1: COM Error)	
	UC lv_nodetrys=0;					// �ڵ㳢�Դ���
	UC lv_sysresult[1];

	SetTickTime();

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step
		
		// Clear static buffer
		memset(lv_userid, 0, sizeof(lv_userid));
		memset(lv_password, 0, sizeof(lv_password));

		// Get parameters
		memcpy((UC *)&lv_sysresult, lv_sysresult, sizeof(lv_sysresult));
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));

		// Sun added 2002-05-24
		// ��¼�û�����
		if( NF_GetUserValue(lv_data1.var_result, lv_sysresult) > 0 )
		{
			if( lv_sysresult[0] == 49 )
			{
				// ת�ɹ��ڵ�
				if( NF_SetNextNode(lv_data2.nd_succeed) == 0 )
					return NODE_RESULT_ERROR;
				return NODE_RESULT_OK;
			}
		}

		/// ������־
		NF_SetAccessLog(lv_data1.log);
	
		for( lv_nodetrys = 0; lv_nodetrys < lv_data1.maxtrytime; lv_nodetrys++ )
		{
			/// Sun added 2002-05-15
			if( lv_data1.maxuserid > 0 )
			{
				for( lv_trys = 0; lv_trys < m_bytMaxTrys; lv_trys++ )
				{
					// Ҫ���û����������ʾ����
					memset(lv_keybuf, 0x00, sizeof(lv_keybuf));
					if( GetDigitEx(lv_data2.vox_userid, lv_data1.maxuserid, lv_keybuf, lv_data1.timeout, 5, lv_data1.key_term, true) == -1 )
						return NODE_RESULT_ERROR;

					// No Entry
					if (lv_keybuf[0] == NULL && lv_data1.maxuserid > 0)
						continue;

					// �ж������ж�
					memcpy(lv_userid, lv_keybuf, sizeof(lv_userid));
					lv_retval = TerminalAssert(TERM_TYPE_GETKEY, lv_data2.nd_parent, lv_keybuf[0]);
					if( lv_retval == 5 )
					{	// Try Again
						continue;
					}
					else if( lv_retval == 0 )
					{	// Go on
						break;
					}
					else if( lv_retval == 1 )
					{	// Default transfer
						return NODE_RESULT_OK;
					}
					else if( lv_retval < 0 )
						return NODE_RESULT_ERROR;
				}

				if( lv_trys >= m_bytMaxTrys )
				{	// �ڵ�ʧ��
					if( NF_SetNextNode(lv_data2.nd_fail) == 0 )
						return NODE_RESULT_ERROR;
					return NODE_RESULT_OK;
				}

				/// �����û�����-���ֻ��һλ�жϺ��������ȫ��ת��
				//// ���ʧ���򷵻ظ��ڵ�
				if(lv_userid[0] == lv_data1.key_term)
				{
					if(!NF_TransferDefault(lv_userid[0], lv_data2.nd_parent))
					{
						/// ���ظ��ڵ�
						if(!NF_SetNextNode(lv_data2.nd_parent))
							return NODE_RESULT_ERROR;
					}
					return NODE_RESULT_OK;
				}
				else
				{	/// �����û�����
					for(lv_loop=1;lv_loop<lv_data1.maxuserid;lv_loop++)
					{
						if(lv_userid[lv_loop] == lv_data1.key_term)
							break;
					}
					lv_userid[lv_loop] = 0;

					// ��¼�û�����
					NF_SetUserValue(lv_data1.var_userid, lv_userid, lv_loop);
				}
			}

			if( lv_data1.maxpassword > 0 )
			{
				for( lv_trys = 0; lv_trys < m_bytMaxTrys; lv_trys++ )
				{
					
					// ��ȡ�û�����
					memset(lv_keybuf, 0x00, sizeof(lv_keybuf));
					if( GetDigitEx(lv_data2.vox_password, lv_data1.maxpassword, lv_keybuf, lv_data1.timeout, 5, lv_data1.key_term, true) == -1 )
						return NODE_RESULT_ERROR;

					// No Entry
					if (lv_keybuf[0] == NULL && lv_data1.maxpassword > 0)
						continue;

					// �ж������ж�
					memcpy(lv_password, lv_keybuf, sizeof(lv_password));
					lv_retval = TerminalAssert(TERM_TYPE_GETKEY, lv_data2.nd_parent, lv_keybuf[0]);
					if( lv_retval == 5 )
					{	// Try Again
						continue;
					}
					else if( lv_retval == 0 )
					{	// Go on
						break;
					}
					else if( lv_retval == 1 )
					{	// Default transfer
						return NODE_RESULT_OK;
					}
					else if( lv_retval < 0 )
						return NODE_RESULT_ERROR;
				}
			
				if( lv_trys >= m_bytMaxTrys )
				{	// �ڵ�ʧ��
					if( NF_SetNextNode(lv_data2.nd_fail) == 0 )
						return NODE_RESULT_ERROR;
					return NODE_RESULT_OK;
				}

				/// �����û�����
				for(lv_loop=0;lv_loop<lv_data1.maxpassword;lv_loop++)
				{
					if(lv_password[lv_loop] == lv_data1.key_term)
						break;
				}
				lv_password[lv_loop] = 0;

				// ��¼�û�����
				NF_SetUserValue(lv_data1.var_password, lv_password, lv_loop);
			}

			// ��֤���
			WriteTrace(TraceDebug, "Debug - IVR Channel: %d will verify user [%s] [%s]", m_intLineNO, lv_userid, lv_password); 
			lv_checkresult = CheckUserPassword(lv_userid, lv_password, lv_data2.com_iid);
			switch( lv_checkresult )
			{
			case 0:			// Succeed!
				
				// ��¼�û����� -  try times
				NF_SetUserValue(lv_data1.var_trytime, &lv_nodetrys, 1);

				// ��¼�û�����
				lv_sysresult[0] = 49;			// '1'
				NF_SetUserValue(lv_data1.var_result, lv_sysresult);
				
				// ת�ɹ��ڵ�
				if( NF_SetNextNode(lv_data2.nd_succeed) == 0 )
					return NODE_RESULT_ERROR;
				return NODE_RESULT_OK;

				break;

			case -1:		// COM Error
				return NODE_RESULT_ERROR;
				break;
			
			default:		// Check failed
				
				// ����-������������(Note: <= not <)
				if( lv_nodetrys < lv_data1.maxtrytime )
				{
					if(PlayVoiceEx(lv_data2.vox_tryagain) == -1)
						return NODE_RESULT_ERROR;
				}
			}
		}
			
		if( lv_nodetrys >= lv_data1.maxtrytime )
		{	
			// ��¼�û�����
			lv_sysresult[0] = 48;			// '0'
			NF_SetUserValue(lv_data1.var_result, lv_sysresult);

			// �ڵ�ʧ��
			if( NF_SetNextNode(lv_data2.nd_fail) == 0 )
				return NODE_RESULT_ERROR;
			return NODE_RESULT_OK;
		}

		break;

	default:
		return NODE_RESULT_ERROR;
	}
		
	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// �޸Ŀ���
// ���أ�
US CIVRChannel::NF_008(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_008 lv_data1;
	SData2_008 lv_data2;
	int lv_trys;						// ���ڵ㳢�Դ���-���볬ʱ��
	int lv_retval;						// �ڵ�ת�Ʒ���ֵ
	char lv_keybuf[256];				// ����Buffer
	UC lv_password[20]="";				// �û�����
	UC lv_confirm[20]="";				// ȷ�Ͽ���
	UC lv_loop;							// ѭ������
	int lv_checkresult;					// �����֤���(0: OK; >0 Error Loc; -1: COM Error)	
	UC lv_nodetrys=0;					// �ڵ㳢�Դ���

	// ��ʱ
	SetTickTime();

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step
		
		// Clear static buffer
		memset(lv_confirm, 0, sizeof(lv_confirm));
		memset(lv_password, 0, sizeof(lv_password));

		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));
		
		/// ������־
		NF_SetAccessLog(lv_data1.log);

		for( lv_nodetrys = 0; lv_nodetrys < lv_data1.maxtrytime; lv_nodetrys++ )
		{
			/// Sun added 2002-05-15
			if( lv_data1.maxpassword <= 0 )
			{
				break;
			}
			else
			{
				for( lv_trys = 0; lv_trys < m_bytMaxTrys; lv_trys++ )
				{
					// Ҫ���û������¿�����ʾ����
					memset(lv_keybuf, 0x00, sizeof(lv_keybuf));
					if( GetDigitEx(lv_data2.vox_password, lv_data1.maxpassword, lv_keybuf, lv_data1.timeout, 5, lv_data1.key_term, true) == -1 )
						return NODE_RESULT_ERROR;

					// No Entry
					if (lv_keybuf[0] == NULL)
						continue;
								
					// �ж������ж�
					memcpy(lv_password, lv_keybuf, sizeof(lv_password));
					lv_retval = TerminalAssert(TERM_TYPE_GETKEY, lv_data2.nd_parent, lv_keybuf[0]);
					if( lv_retval == 5 )
					{	// Try Again
						continue;
					}
					else if( lv_retval == 0 )
					{	// Go on
						break;
					}
					else if( lv_retval == 1 )
					{	// Default transfer
						return NODE_RESULT_OK;
					}
					else if( lv_retval < 0 )
						return NODE_RESULT_ERROR;
				}

				if( lv_trys >= m_bytMaxTrys )
				{	// �ڵ�ʧ��
					if( NF_SetNextNode(lv_data2.nd_fail) == 0 )
						return NODE_RESULT_ERROR;
					return NODE_RESULT_OK;
				}

				/// �����¿���-���ֻ��һλ�жϺ��������ȫ��ת��
				//// ���ʧ���򷵻ظ��ڵ�
				if(lv_password[0] == lv_data1.key_term)
				{
					if(!NF_TransferDefault(lv_password[0], lv_data2.nd_parent))
					{
						/// ���ظ��ڵ�
						if(!NF_SetNextNode(lv_data2.nd_parent))
							return NODE_RESULT_ERROR;
					}
					return NODE_RESULT_OK;
				}
				else
				{	/// �����¿���
					for(lv_loop=1;lv_loop<lv_data1.maxpassword;lv_loop++)
					{
						if(lv_password[lv_loop] == lv_data1.key_term)
							break;
					}
					lv_password[lv_loop] = 0;
				}
			
				for( lv_trys = 0; lv_trys < m_bytMaxTrys; lv_trys++ )
				{
					// �ٴζ�ȡ�û�����
					memset(lv_keybuf, 0x00, sizeof(lv_keybuf));
					if( GetDigitEx(lv_data2.vox_confirm, lv_data1.maxpassword, lv_keybuf, lv_data1.timeout, 5, lv_data1.key_term, true) == -1 )
						return NODE_RESULT_ERROR;

					// No Entry
					if (lv_keybuf[0] == NULL)
						continue;

					// �ж������ж�
					memcpy(lv_confirm, lv_keybuf, sizeof(lv_confirm));
					lv_retval = TerminalAssert(TERM_TYPE_GETKEY, lv_data2.nd_parent, lv_keybuf[0]);
					if( lv_retval == 5 )
					{	// Try Again
						continue;
					}
					else if( lv_retval == 0 )
					{	// Go on
						break;
					}
					else if( lv_retval == 1 )
					{	// Default transfer
						return NODE_RESULT_OK;
					}
					else if( lv_retval < 0 )
						return NODE_RESULT_ERROR;
				}
			
				if( lv_trys >= m_bytMaxTrys )
				{	// �ڵ�ʧ��
					if( NF_SetNextNode(lv_data2.nd_fail) == 0 )
						return NODE_RESULT_ERROR;
					return NODE_RESULT_OK;
				}

				/// �����û�����
				for(lv_loop=0;lv_loop<lv_data1.maxpassword;lv_loop++)
				{
					if(lv_confirm[lv_loop] == lv_data1.key_term)
						break;
				}
				lv_confirm[lv_loop] = 0;
			
				// �Ƚ����ε������Ƿ�һ��
				if( _stricmp((const char *)lv_password, (const char *)lv_confirm) == 0 )
				{
					// ��¼�û�����
					NF_SetUserValue(lv_data1.var_password, lv_password, lv_loop);
					break;		// Exit For
				}

				// ��һ��
				if(PlayVoiceEx(lv_data2.vox_tryagain) == -1)
				{
					return NODE_RESULT_ERROR;
				}
			} // If Max
		} // End of For

		if( lv_nodetrys >= lv_data1.maxtrytime )
		{	// Max time reached
			// �ڵ�ʧ��
			if( NF_SetNextNode(lv_data2.nd_fail) == 0 )
				return NODE_RESULT_ERROR;
			return NODE_RESULT_OK;
		}
		else
		{
			// Get User ID from User Variable
			if( lv_data1.maxpassword > 0 )
			{
				if( ChData.customer[0] > 0 )
				{	
					// Record New Password
					lv_checkresult = UpdateUserPassword(ChData.customer, lv_password, lv_data2.com_iid);
					// ��¼�û�����
					NF_SetUserValue(lv_data1.var_result, (UC *)&lv_checkresult);
					switch( lv_checkresult )
					{
					case 0:			// Succeed!
						
						// ��¼�û����� -  try times
						NF_SetUserValue(lv_data1.var_trytime, &lv_nodetrys, 1);
						
						// ת�ɹ��ڵ�
						if(PlayVoiceEx(lv_data2.vox_succeed) == -1)
						{
							return NODE_RESULT_ERROR;
						}
						if( NF_SetNextNode(lv_data2.nd_succeed) == 0 )
							return NODE_RESULT_ERROR;
						return NODE_RESULT_OK;

						break;

					default:		// COM Error
						return NODE_RESULT_ERROR;
					}
				}
				/// Can't find User ID, then goto fail node
				//// �ڵ�ʧ��
				if( NF_SetNextNode(lv_data2.nd_fail) == 0 )
					return NODE_RESULT_ERROR;
				return NODE_RESULT_OK;
			}
		}

		// ת�ɹ��ڵ�
		if( NF_SetNextNode(lv_data2.nd_succeed) == 0 )
			return NODE_RESULT_ERROR;
		return NODE_RESULT_OK;

		break;

	default:
		return NODE_RESULT_ERROR;
	}
		
	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// ʱ���֧
// ���أ�
US CIVRChannel::NF_009(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_009 lv_data1;
	SData2_009 lv_data2;
	CTime lv_tmTest, lv_now;
	int lv_weekday, lv_loop;
	long lv_nowmin, lv_startmin, lv_endmin;
	US lv_node = 0;

	lv_now = CTime::GetCurrentTime();

	//------------------------------------------------------------------------
	// Sun added 2007-01-16
	if( GetIVRTestFlag() )
		if( GetIVRTestTime(lv_tmTest) )
			lv_now = lv_tmTest;
	//------------------------------------------------------------------------

	// ��ʱ
	SetTickTime();

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step
		
		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));
		
		/// ������־
		NF_SetAccessLog(lv_data1.log);

		//---------------------------------------------------------------
		// Sun test 2011-12-02
		WriteTrace(TraceDebug, "Debug - IVR Channel: %d NF_009 Data: WorkDay(0x%x), WorkTime(0x%x), Now is %s", 
			m_intLineNO, lv_data2.workday, lv_data2.worktime, lv_now.Format("%Y-%m-%d %H:%M"));
		for( lv_loop = 0; lv_loop < 6; lv_loop++ )
		{
			memcpy(&lv_node, &lv_data2.nd_timesec1 + lv_loop, 2);
			WriteTrace(TraceDebug, "Debug - IVR Channel: %d NF_009 Data: TimeSlot: %d enabled=%d, from %d:%d to %d:%d, Node: %d", 
				m_intLineNO, lv_loop, GetOneBit(lv_data2.worktime, lv_loop), 
				lv_data2.timesec[lv_loop].StartHH, lv_data2.timesec[lv_loop].StartMM,
				lv_data2.timesec[lv_loop].EndHH, lv_data2.timesec[lv_loop].EndMM,
				lv_node);
		}
		//---------------------------------------------------------------

		// ϵͳ�����ж�
		lv_weekday = lv_now.GetDayOfWeek() - 1;			// 0 (Sunday) - 6 (Saturday) Bit
		if( !GetOneBit(lv_data2.workday, lv_weekday) )
		{
			/// ת����Ϣ�սڵ�
			if( NF_SetNextNode(lv_data2.nd_sparetime) == 0 )
				return NODE_RESULT_ERROR;
			return NODE_RESULT_OK;
		}
		
		// ������
		/// ʱ����ж�
		lv_nowmin = lv_now.GetHour() * 60 + lv_now.GetMinute();
		for( lv_loop = 0; lv_loop < 6; lv_loop++ )
		{
			if( !GetOneBit(lv_data2.worktime, lv_loop) )
				break;
			lv_startmin = lv_data2.timesec[lv_loop].StartHH * 60 + lv_data2.timesec[lv_loop].StartMM;
			lv_endmin = lv_data2.timesec[lv_loop].EndHH * 60 + lv_data2.timesec[lv_loop].EndMM;
			if( lv_startmin <= lv_nowmin && lv_nowmin <= lv_endmin )
			{
				break;			// Exit Loop
			}
		}
		lv_loop %= 6;
		/// ת����Ӧʱ��νڵ�
		memcpy(&lv_node, &lv_data2.nd_timesec1 + lv_loop, 2);
		if( NF_SetNextNode(lv_node) == 0 )
			return NODE_RESULT_ERROR;
		return NODE_RESULT_OK;

		break;

	default:
		return NODE_RESULT_ERROR;
	}
		
	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// �������趨
// ���أ�
US CIVRChannel::NF_010(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_010 lv_data1;
	SData2_010 lv_data2;
	CTime lv_tmTest, lv_now = CTime::GetCurrentTime();
	CTimeSpan lv_pasedTime;
	CTime lv_tmStart, lv_tmEnd;
	int lv_nStartYear, lv_nStartMonth;
	int lv_nEndYear, lv_nEndMonth;
	int lv_days, lv_index1, lv_index2;

	//------------------------------------------------------------------------
	// Sun added 2007-01-16
	if( GetIVRTestFlag() )
		if( GetIVRTestTime(lv_tmTest) )
			lv_now = lv_tmTest;
	//------------------------------------------------------------------------

	// ��ʱ
	SetTickTime();

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step
		
		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));
		
		/// ������־
		NF_SetAccessLog(lv_data1.log);

		// ���ڷ�Χ�ж�
		lv_nStartYear = lv_data1.startyear + 2000;
		lv_nStartMonth = lv_data1.startmonth % 13;
		lv_nEndYear = lv_nStartYear;
		lv_nEndMonth = lv_nStartMonth + lv_data1.monthcount;
		if( lv_nEndMonth > 12 )
		{
            lv_nEndYear += (lv_nEndMonth / 13);
            lv_nEndMonth = lv_nEndMonth % 13 + 1;
		}

		// ������Ч����֤
		if( lv_nStartYear > 2200 )
			lv_nStartYear = 2008;
		if( lv_nStartMonth <= 0  || lv_nStartMonth > 12 )
			lv_nStartMonth = 1;
		if( lv_nEndYear > 2200 )
			lv_nEndYear = lv_nStartYear;
		if( lv_nEndMonth <= 0  || lv_nEndMonth > 12 )
			lv_nStartMonth = lv_nStartMonth;

		lv_tmStart = CTime::CTime(lv_nStartYear, lv_nStartMonth, 1, 0, 0, 0);
		lv_tmEnd = CTime::CTime(lv_nEndYear, lv_nEndMonth, 1, 0, 0, 0);

		if( lv_tmStart > lv_now || lv_now >= lv_tmEnd )
		{
			/// ת�����趨�ڵ�
			if( NF_SetNextNode(lv_data2.nd_daysec[2]) == 0 )
				return NODE_RESULT_ERROR;
			return NODE_RESULT_OK;
		}

		// �����ա��ڼ����ж�
		lv_pasedTime = lv_now - lv_tmStart;
		lv_days = (int)lv_pasedTime.GetDays();
		lv_index1 = lv_days / 8;
		lv_index2 = lv_days % 8;

		if( GetOneBit(lv_data2.daytype[lv_index1], lv_index2) )
		{
			/// ת��ڼ��սڵ�
			if( NF_SetNextNode(lv_data2.nd_daysec[1]) == 0 )
				return NODE_RESULT_ERROR;
		}
		else
		{
			/// ת�빤���սڵ�
			if( NF_SetNextNode(lv_data2.nd_daysec[0]) == 0 )
				return NODE_RESULT_ERROR;
		}
		break;

	default:
		return NODE_RESULT_ERROR;
	}
		
	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Sun added 2004-12-30
// ������֧
// ���أ�
US CIVRChannel::NF_016(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_016 lv_data1;
	SData2_016 lv_data2;
	UC lv_uservar[PCS_MAX_VAR_LEN];					// �û�����
	CString lv_sFormat, lv_sComp1, lv_sComp2;
	BOOL lv_result;
	int lv_strlen, lv_nCompLen;

	// ��ʱ
	SetTickTime();

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step

		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));

		/// ������־
		NF_SetAccessLog(lv_data1.log);

		/// ȡ����ֵ
		memset(lv_uservar, 0x00, sizeof(lv_uservar));
		NF_GetUserValue(lv_data1.var_id, lv_uservar);
		lv_sFormat = (LPCSTR)lv_uservar;

		/// ����
		switch( lv_data1.convert )
		{
		case DEF_NODE016_CONVERT_LEFT:
			lv_strlen = lv_sFormat.GetLength();
			if( lv_data1.param1 < lv_strlen )
				lv_strlen = lv_data1.param1;
			lv_sComp1 = lv_sFormat.Left(lv_strlen);
			break;
		case DEF_NODE016_CONVERT_RIGHT:
			lv_strlen = lv_sFormat.GetLength();
			if( lv_data1.param1 < lv_strlen )
				lv_strlen = lv_data1.param1;
			lv_sComp1 = lv_sFormat.Right(lv_data1.param1);
			break;
		case DEF_NODE016_CONVERT_MID:
			lv_strlen = lv_sFormat.GetLength();
			if( lv_data1.param1 < lv_strlen )
			{
				if( lv_data1.param1 + lv_data1.param2 < lv_strlen )
					lv_strlen = lv_data1.param2;
				else
					lv_strlen -= lv_data1.param1;
				lv_sComp1 = lv_sFormat.Mid(lv_data1.param1, lv_strlen);
			}
			else
				lv_sComp1 = _T("");
			break;

		default:
			lv_sComp1 = lv_sFormat;
		}

		// Sun updated 2007-11-27
		/// From
		//lv_sComp2 = (LPCSTR)lv_data2.var_value;
		// To
		if( lv_data2.var_value[0] == ':' )
		{
			if( lv_data2.var_value[1] != ':' )
			{
				/// IVR Variable
				lv_sFormat = (const char *)(lv_data2.var_value + 1);
				memset(lv_uservar, 0x00, sizeof(lv_uservar));
				NF_GetUserValue((const char *)lv_sFormat, lv_uservar);
				lv_sComp2 = (LPCSTR)lv_uservar;
			}
			else			// "::" -> ":"
				lv_sComp2 = (LPCSTR)(lv_data2.var_value + 1);
		}
		else
			lv_sComp2 = (LPCSTR)lv_data2.var_value;

		WriteTrace(TraceDebug, "IVR Channel: %d NF_016: compare (%s, %s)", m_intLineNO, lv_sComp1, lv_sComp2);

		/// �Ƚ�
		lv_result = FALSE;
		if( lv_data1.convert == DEF_NODE016_CONVERT_LEN )
		{
			// Sun added 2005-06-20
			lv_strlen = lv_sComp1.GetLength();
			lv_nCompLen = atoi((LPCSTR)lv_sComp2);
			switch( lv_data1.logic )
			{
			case DEF_NODE016_LOGIC_BIGGER:
				if( lv_strlen > lv_nCompLen )
					lv_result = TRUE;
				break;
			case DEF_NODE016_LOGIC_SMALLER:
				if( lv_strlen < lv_nCompLen )
					lv_result = TRUE;
				break;
			case DEF_NODE016_LOGIC_EB:
				if( lv_strlen >= lv_nCompLen )
					lv_result = TRUE;
				break;
			case DEF_NODE016_LOGIC_ES:
				if( lv_strlen <= lv_nCompLen )
					lv_result = TRUE;
				break;
			case DEF_NODE016_LOGIC_NE:
				if( lv_strlen != lv_nCompLen )
					lv_result = TRUE;
				break;
			default:
				if( lv_strlen == lv_nCompLen )
					lv_result = TRUE;
			}
		}
		else
		{
			switch( lv_data1.logic )
			{
			case DEF_NODE016_LOGIC_BIGGER:
				if( lv_sComp1 > lv_sComp2 )
					lv_result = TRUE;
				break;
			case DEF_NODE016_LOGIC_SMALLER:
				if( lv_sComp1 < lv_sComp2 )
					lv_result = TRUE;
				break;
			case DEF_NODE016_LOGIC_EB:
				if( lv_sComp1 >= lv_sComp2 )
					lv_result = TRUE;
				break;
			case DEF_NODE016_LOGIC_ES:
				if( lv_sComp1 <= lv_sComp2 )
					lv_result = TRUE;
				break;
			case DEF_NODE016_LOGIC_NE:
				if( lv_sComp1 != lv_sComp2 )
					lv_result = TRUE;
				break;
			case DEF_NODE016_LOGIC_LIKE:
				if( lv_sComp1.Find( lv_sComp2 ) >= 0 )
					lv_result = TRUE;
				break;
			default:
				if( lv_sComp1 == lv_sComp2 )
					lv_result = TRUE;
			}
		}

		// ת��
		if( lv_result )
		{
			if(NF_SetNextNode(lv_data2.nd_succ) == 0)
				return NODE_RESULT_ERROR;
		}
		else
		{
			if(NF_SetNextNode(lv_data2.nd_fail) == 0)
				return NODE_RESULT_ERROR;
		}
		
		break;

	default:
		return NODE_RESULT_ERROR;
	}
		
	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// ѡ���������
// ���أ�
US CIVRChannel::NF_017(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_017 lv_data1;
	SData2_017 lv_data2;
	char lv_keybuf[256];				// ����Buffer
	UC lv_langid[2]="";					// ��������ID
	int lv_loop, lv_nodetrys, lv_retval;

	// ��ʱ
	SetTickTime();

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step
		
		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));
		
		for( lv_nodetrys = 0; lv_nodetrys < lv_data1.maxtrytime; lv_nodetrys++ )
		{
			// ��ʾ���� - Ҫ���û�ѡ���������
			memset(lv_keybuf, 0x00, sizeof(lv_keybuf));
			if( GetDigitEx(lv_data2.vox_play, 1, lv_keybuf, lv_data1.timeout, 5) == -1 )
				return NODE_RESULT_ERROR;

			// No Entry
			if (lv_keybuf[0] == NULL)
				continue;

			// ��¼����ѡ����
			for( lv_loop = 0; lv_loop < MAXLANGUAGECOUNT; lv_loop++ )
			{
				if( lv_data2.lang[lv_loop] > 0 && lv_data2.lang[lv_loop] == lv_keybuf[0] )
				{
					// ��������
					NF_SetLanguageID((UC)lv_loop);

					// ��¼�û�����
					memcpy(lv_langid, lv_keybuf, sizeof(lv_langid));
					NF_SetUserValue(lv_data1.var_lang, lv_langid, 1);

					// ת�ɹ��ڵ�
					if( NF_SetNextNode(lv_data2.nd_succ) == 0 )
						return NODE_RESULT_ERROR;
					return NODE_RESULT_OK;
				}
			}

			// �ж������ж�
			lv_retval = TerminalAssert(TERM_TYPE_GETKEY, lv_data2.nd_parent, lv_keybuf[0]);
			if( lv_retval == 1 )
			{	// Default transfer
				return NODE_RESULT_OK;
			}
			else if( lv_retval < 0 )
				return NODE_RESULT_ERROR;
		}

		// תʧ�ܽڵ�
		if( NF_SetNextNode(lv_data2.nd_fail) == 0 )
			return NODE_RESULT_ERROR;
		return NODE_RESULT_OK;

		break;

	default:
		return NODE_RESULT_ERROR;
	}
		
	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Sun added 2004-12-30
// ��������
// ���أ�
US CIVRChannel::NF_018(const int event, const UC *f_data1, const UC *f_data2)
{
	switch(event)
	{
	case LE_PROCEND:					// must be the first Step

		// ��ʱ
		SetTickTime();

		// Get parameters
		memcpy((UC *)&m_Data018_1, f_data1, DEF_NODE_DATA_SMALL_LEN);
		memcpy((UC *)&m_Data018_2, f_data2, DEF_NODE_DATA_BIG_LEN);
		
		// Sun added 2012-07-19
		char lv_strAppData[PCS_DEF_DATALEN_APP];
		char lv_strUserData[PCS_DEF_DATALEN_USER];
		try
		{
			memset(lv_strAppData, 0x00, sizeof(lv_strAppData));
			memset(lv_strUserData, 0x00, sizeof(lv_strUserData));
			FillupCallBindedData(lv_strAppData, lv_strUserData);
		}
		catch(...)
		{
		}

		// ת�ɹ��ڵ�
		if( NF_SetNextNode(m_Data018_2.nd_child) == 0 )
			return NODE_RESULT_ERROR;
		break;

	default:
		return NODE_RESULT_ERROR;
	}

	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// �޲���
// ���أ�
US CIVRChannel::NF_019(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_019 lv_data1;
	SData2_019 lv_data2;

	// ��ʱ
	SetTickTime();

	// Get parameters
	memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
	memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step
		// Sun added 2007-07-11
		/// �Ƿ����
		if(	lv_data1.leavequeue == 1 )
			SetQueueWaitEvent(18);		/// �ȴ��ж�Level18 - �������

		if( lv_data2.delaytime > 0 )
		{
			// Sun added 2012-11-23
			/// ��ʱ
			if( !SmartSleep(lv_data2.delaytime) )
				return NODE_RESULT_ERROR;
		}
		break;

	default:
		return NODE_RESULT_ERROR;
	}
	
	// Sun added 2004-12-30 & updated 2005-05-26
	if( m_bytStatus > IVR_BLOCK && !m_blnCCTalking )
		SetStatus(IVR_BLOCK);

	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// �����һ�
// ���أ�
US CIVRChannel::NF_020(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_020 lv_data1;
	SData2_020 lv_data2;

	// ��ʱ
	SetTickTime();

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step
		
		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));
		
		/// ������־
		NF_SetAccessLog(lv_data1.log);

		// Sun added 2012-05-07
		if( m_HookOnWaitEvent.m_nEventID > 0 )
			m_nBeforeHookon = 0;
		if( m_HookOnWaitEvent.m_nEventID == 1 )
		{
			m_HookOnWaitEvent.m_nEventID = 0;
			m_HookOnWaitEvent.Set();
		}

		// ����
		if(PlayVoiceEx(lv_data2.vox_play, 'N', lv_data1.playclear==1) == -1)
			return NODE_RESULT_ERROR;
	
		// �һ�����
		return NODE_RESULT_ERROR;
		break;

	default:
		return NODE_RESULT_ERROR;
	}
		
	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// ��������
// ���أ�
US CIVRChannel::NF_021(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_021 lv_data1;
	SData2_021 lv_data2;
	int lv_retval;						// �ڵ�ת�Ʒ���ֵ
	UC lv_uservar[PCS_MAX_VAR_LEN];		// �û�����
	int lv_varlength;					// �û���������
	char filename[MAX_PATH] = "";		// ��Դ·��
	_bstr_t lv_inputstr="";				// COM���ò���
	BSTR lv_outputstr;					// COM���÷��ؽ��
	_bstr_t lv_comiid="";				// COM�ӿ�ID
	HRESULT hr = 0;						// ���Է���ֵ
	int lv_Lp;							// Loop
	UC lv_pt;							// ��������
	BOOL lv_blnReadLetter;				// �Ƿ񲥷���ĸ
	CString lv_sYYYYMMDD;
	int lv_nYear, ln_nMonth, ln_nDay;

	SetTickTime();

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step
		
		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));

		/// ������־
		NF_SetAccessLog(lv_data1.log);
	
		// ǰ����������
		if(PlayVoiceEx(lv_data2.vox_pred, lv_data1.breakkey, lv_data1.playclear==1) == -1)
			return NODE_RESULT_ERROR;

		// ��ȡ������ΪCOM���ò���
		lv_varlength = NF_GetUserValue(lv_data1.usevar, lv_uservar);
		lv_retval = 0;

		// ��ʼ��COM���ò���
		lv_outputstr=SysAllocString((OLECHAR *)"000000000000000000000000000000000000000000000000000000000000");

		if( lv_varlength > 0 )
		{
			if( lv_data2.com_iid > 0 )
			{
				// ȡ����Դ·��
				if(GetResourcePathWithID(lv_data2.com_iid, filename) == NULL)
				{
					// Sun added 2004-12-30
					SysFreeString(lv_outputstr);

					// ֱ��ת�ӽڵ�
					if( NF_SetNextNode(lv_data2.nd_child) == 0 )
						return NODE_RESULT_ERROR;
					return NODE_RESULT_OK;
				}
				else
				{
					// ȡ��COM�ӿ�ID
					lv_comiid = ( const char* )filename;

					// ����COM
					VARIANT lv_R;
					VariantInit(&lv_R);

					lv_inputstr = ( const char* )lv_uservar;

					try
					{
						if( m_oUc.IsCreateObject() )
						{
							hr = m_oUc.Invoke(DISPATCH_METHOD, &lv_R, NULL, NULL,
								(BSTR)lv_comiid, TEXT("s&s"),
								(BSTR)lv_inputstr,
								(BSTR FAR *)&lv_outputstr);
						}	
					}
					catch(...)
					{
						// Sun added 2004-12-30
						SysFreeString(lv_outputstr);

						// ֱ��ת�ӽڵ�
						if( NF_SetNextNode(lv_data2.nd_child) == 0 )
							return NODE_RESULT_ERROR;
						return NODE_RESULT_OK;
					}

					if(FAILED(hr))
					{
						// Sun added 2004-12-30
						SysFreeString(lv_outputstr);

						// ֱ��ת�ӽڵ�
						if( NF_SetNextNode(lv_data2.nd_child) == 0 )
							return NODE_RESULT_ERROR;
						return NODE_RESULT_OK;
					}
				}
			}
			else
			{
				for(lv_Lp = 0 ; lv_Lp < lv_varlength; lv_Lp++)
				{
					lv_outputstr[lv_Lp] = lv_uservar[lv_Lp];
				}
				lv_outputstr[lv_Lp] = 0;
			}

			// ���ݲ���
			//-----------------------------
			// Sun added 2010-10-25
			/// ��ʱ
			SetTickTime();
			/// �������״̬
			SetStatus(IVR_W_PLAY);
           //-----------------------------

			// ���������ж�
			lv_pt = lv_data1.playtype;

			// Sun added 2005-06-20
			if( lv_pt >= 16)
			{
				// ��������
				lv_pt-=16;

				// ȡ���ꡢ�¡���
				lv_sYYYYMMDD = lv_outputstr;
				lv_nYear = ln_nMonth = ln_nDay = 0;
				if( lv_sYYYYMMDD.GetLength() >= 6 )
				{
					lv_nYear = atoi((LPCSTR)lv_sYYYYMMDD.Left(4));
					ln_nMonth = atoi((LPCSTR)lv_sYYYYMMDD.Mid(4, 2));
				}
				if( lv_sYYYYMMDD.GetLength() >= 8 )
					ln_nDay = atoi((LPCSTR)lv_sYYYYMMDD.Mid(6, 2));

				// ��
				if( lv_nYear > 0 && lv_nYear < 3000 )
				{
					if( (lv_retval = m_oARS.ivr_channel_numberplay((LPCSTR)lv_sYYYYMMDD.Left(4), FALSE, NF_GetLanguageID())) == -1 )
						return NODE_RESULT_ERROR;

					memset(filename, 0x00, sizeof(filename));
					sprintf(filename, "SYS_%d_YEAR", NF_GetLanguageID());
					if( m_oARS.ivr_channel_play(filename) == -1 )
						return NODE_RESULT_ERROR;
				}

				// ��
				if( ln_nMonth > 0 && ln_nMonth <= 12 )
				{
					memset(filename, 0x00, sizeof(filename));
					switch( ln_nMonth )
					{
					case 1:
						sprintf(filename, "SYS_%d_JANUARY", NF_GetLanguageID());
						break;
					case 2:
						sprintf(filename, "SYS_%d_FEBRERY", NF_GetLanguageID());
						break;
					case 3:
						sprintf(filename, "SYS_%d_MARCH", NF_GetLanguageID());
						break;
					case 4:
						sprintf(filename, "SYS_%d_APRIL", NF_GetLanguageID());
						break;
					case 5:
						sprintf(filename, "SYS_%d_MAY", NF_GetLanguageID());
						break;
					case 6:
						sprintf(filename, "SYS_%d_JUNE", NF_GetLanguageID());
						break;
					case 7:
						sprintf(filename, "SYS_%d_JULY", NF_GetLanguageID());
						break;
					case 8:
						sprintf(filename, "SYS_%d_AUGUEST", NF_GetLanguageID());
						break;
					case 9:
						sprintf(filename, "SYS_%d_SEPTEMBER", NF_GetLanguageID());
						break;
					case 10:
						sprintf(filename, "SYS_%d_OCTOBER", NF_GetLanguageID());
						break;
					case 11:
						sprintf(filename, "SYS_%d_NOVEMBER", NF_GetLanguageID());
						break;
					case 12:
						sprintf(filename, "SYS_%d_DECEMBER", NF_GetLanguageID());
						break;
					}

					if( m_oARS.ivr_channel_play(filename) == -1 )
						return NODE_RESULT_ERROR;
				}

				// ��
				if( ln_nDay > 0 && ln_nDay <= 31 )
				{
					if( m_oARS.ivr_channel_digitplay(ln_nDay, 0, NF_GetLanguageID()) == -1 )
						return NODE_RESULT_ERROR;
				}
			}
			else
			{
				if( lv_pt >= 8)
				{
					// GOLDSUN ���ź���
					// Ŀǰ��֧��
					lv_pt-=8;
				}

				if( lv_pt >= 4 )
				{
					// ������ĸ
					lv_pt = 2;
					lv_blnReadLetter = TRUE;
				}
				else
					lv_blnReadLetter = FALSE;

				// Sun modified 2002-05-24
				// ��������
				double lv_digits;
				_bstr_t lv_tempstr;
				lv_tempstr = lv_outputstr;
				lv_digits = atof((const char *)lv_tempstr) + 0.00001; // Sun updated 2008-01-18

				switch( lv_pt )
				{
				case 2:			// ����
					if( (lv_retval = m_oARS.ivr_channel_numberplay((char *)lv_tempstr, lv_blnReadLetter, NF_GetLanguageID())) == -1 )
						return NODE_RESULT_ERROR;
					break;

				case 1:			// ���
					if( (lv_retval = m_oARS.ivr_channel_moneyplay(lv_digits, NF_GetLanguageID())) == -1 )
						return NODE_RESULT_ERROR;
					break;

				case 0:			// ����, Sun updated 2008-04-23
					if( (lv_retval = m_oARS.ivr_channel_digitplay(lv_digits, 4, NF_GetLanguageID())) == -1 )
						return NODE_RESULT_ERROR;
					break;
				}
			}
		}
		else
		{
			// ֱ��ת�ӽڵ�
			if( NF_SetNextNode(lv_data2.nd_child) == 0 )
				return NODE_RESULT_ERROR;
			return NODE_RESULT_OK;
		}
		
		// Sun added 2004-12-30
		SysFreeString(lv_outputstr);
		
		if( lv_retval != 1 )
		{
			// ֱ��ת�ӽڵ�
			if( NF_SetNextNode(lv_data2.nd_child) == 0 )
				return NODE_RESULT_ERROR;
			return NODE_RESULT_OK;
		}
		
		// ������������
		if(PlayVoiceEx(lv_data2.vox_succ, lv_data1.breakkey, false) == -1)
			return NODE_RESULT_ERROR;

		if( NF_SetNextNode(lv_data2.nd_child) == 0 )
			return NODE_RESULT_ERROR;
		return NODE_RESULT_OK;			

		break;

	default:
		return NODE_RESULT_ERROR;
	}
		
	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// �����ȴ�����
// ���أ�
US CIVRChannel::NF_022(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_022 lv_data1;
	SData2_022 lv_data2;
	int lv_trys;						// ���ڵ㳢�Դ���-���볬ʱ��
	int lv_retval = -1;					// �ڵ�ת�Ʒ���ֵ
	char lv_keybuf[256];				// ����Buffer
	US lv_nextnode;
	UC lv_bytMaxTrys;					// ����Դ���, Sun added 2007-03-20
		
	// ��ʱ
	SetTickTime();

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step
		
		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));

		/// ������־
		NF_SetAccessLog(lv_data1.log);
	
		if( lv_data1.getlength > 0 )
		{
			// Sun added 2007-03-20
			lv_bytMaxTrys = lv_data1.maxtrytime;
			if( lv_bytMaxTrys == 0 )
				lv_bytMaxTrys = m_bytMaxTrys;

			memset(lv_keybuf, 0x00, sizeof(lv_keybuf));
			for( lv_trys = 0; lv_trys < lv_bytMaxTrys; lv_trys++ )
			{
				// Sun added 2004-12-30
				// ȱʡת�ƹ���
				if( lv_retval == 0 && lv_data1.getlength == 1 && lv_keybuf[0] > 0 )
				{
					if( !NF_TransferDefault(lv_keybuf[0], lv_data2.nd_parent) == 0 )
						return NODE_RESULT_OK;
				}

				lv_nextnode = 0;

				// �������ܰ���
				memset(lv_keybuf, 0x00, sizeof(lv_keybuf));
				if( GetDigitEx(lv_data2.vox_play, lv_data1.getlength, lv_keybuf, lv_data1.timeout, lv_data1.maxinterval, lv_data1.breakkey, lv_data1.playclear==1) == -1 )
					return NODE_RESULT_ERROR;

				// No Entry
				if( lv_keybuf[0] == NULL && lv_data1.getlength > 0 )
					continue;

				// ��¼���
				NF_SetUserValue(lv_data1.var_key, (UC *)lv_keybuf, strlen(lv_keybuf));
				WriteTrace(TraceDetail, "NT_022: IVR Channel: %d got key input [%s] recorded into value [%d]", m_intLineNO, lv_keybuf, lv_data1.var_key);
			
				// �ж������ж�
				lv_retval = TerminalAssert(TERM_TYPE_GETKEY, lv_data2.nd_parent, lv_keybuf[0]);
				if( lv_retval == 5 )
				{	// Try Again
					continue;
				}
				else if( lv_retval == 0 )
				{	
					if( lv_data1.getlength == 1 )
					{
						if( lv_keybuf[0] == '0' )
						{
							lv_nextnode = lv_data2.nd_key0;
							if( lv_nextnode == 0 )
								continue;
						}
						else if( lv_keybuf[0] == '1' )
						{
							lv_nextnode = lv_data2.nd_key1;
							if( lv_nextnode == 0 )
								continue;
						}
						else if( lv_keybuf[0] == '2' )
						{
							lv_nextnode = lv_data2.nd_key2;
							if( lv_nextnode == 0 )
								continue;
						}
						else if( lv_keybuf[0] == '3' )
						{
							lv_nextnode = lv_data2.nd_key3;
							if( lv_nextnode == 0 )
								continue;
						}
						else if( lv_keybuf[0] == '4' )
						{
							lv_nextnode = lv_data2.nd_key4;
							if( lv_nextnode == 0 )
								continue;
						}
						else if( lv_keybuf[0] == '5' )
						{
							lv_nextnode = lv_data2.nd_key5;
							if( lv_nextnode == 0 )
								continue;
						}
						else if( lv_keybuf[0] == '6' )
						{
							lv_nextnode = lv_data2.nd_key6;
							if( lv_nextnode == 0 )
								continue;
						}
						else if( lv_keybuf[0] == '7' )
						{
							lv_nextnode = lv_data2.nd_key7;
							if( lv_nextnode == 0 )
								continue;
						}
						else if( lv_keybuf[0] == '8' )
						{
							lv_nextnode = lv_data2.nd_key8;
							if( lv_nextnode == 0 )
								continue;
						}
						else if( lv_keybuf[0] == '9' )
						{
							lv_nextnode = lv_data2.nd_key9;
							if( lv_nextnode == 0 )
								continue;
						}
						else if( lv_keybuf[0] == '*' )
						{
							lv_nextnode = lv_data2.nd_keyA;
							if( lv_nextnode == 0 )
								continue;
						}
						else if( lv_keybuf[0] == '#' )
						{
							lv_nextnode = lv_data2.nd_keyB;
							if( lv_nextnode == 0 )
								continue;
						}
					}

					// Go on
					break;
				}
				else if( lv_retval == 1 )
				{	// Default transfer
					return NODE_RESULT_OK;
				}
				else if( lv_retval < 0 )
					return NODE_RESULT_ERROR;
			} // End of For Loop

			if( lv_trys >= lv_bytMaxTrys )
			{	// �ڵ�ʧ�ܲ�������
				if(PlayVoiceEx(lv_data2.vox_nodefail, 'N') == -1)
					return NODE_RESULT_ERROR;
				if( NF_SetNextNode(lv_data2.nd_nodefail) == 0 )
					return NODE_RESULT_ERROR;
				return NODE_RESULT_OK;
			}
		}

		// ת��
		if( lv_data1.getlength == 1 )
			lv_retval = NF_SetNextNode(lv_nextnode);
		else
			lv_retval = NF_SetNextNode(lv_data2.nd_key0);
	
		// ȱʡת�ƹ���
		if(lv_retval == 0)
		{
			if( NF_TransferDefault(lv_keybuf[0], lv_data2.nd_parent) == 0 )
			{
				/// �ظ����ڵ�
				if(!NF_SetNextNode(NF_GetNextNode()))
					return NODE_RESULT_ERROR;
			}
		}
		break;

	default:
		return NODE_RESULT_ERROR;
	}

	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Sun updaed 2006-02-10
// ����ת��
// ���أ�
US CIVRChannel::NF_023(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_023 lv_data1;
	SData2_023 lv_data2;
	UC lv_uservar[PCS_MAX_VAR_LEN];		// �û�����
	int lv_varlength;					// �û���������
	US lv_nVoiceID;

#ifdef PICADEF_ENABLE_ANTIDECEIT
	char lv_keybuf[256];				// ����Buffer
	BOOL lv_bSysGetSpecialKeyIn = FALSE;
#endif
	
	// ��ʱ
	SetTickTime();

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step

		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));

		/// ������־
		NF_SetAccessLog(lv_data1.log);

		// Sun added 2006-02-10
		lv_nVoiceID = lv_data2.vox_play;
		lv_varlength = 0;
		if( lv_data1.var_play > 0 )
		{
			memset(lv_uservar, 0x00, sizeof(lv_uservar));
			lv_varlength = NF_GetUserValue(lv_data1.var_play, lv_uservar);
			if( lv_varlength > 0 )
			{
				// Sun added 2010-11-22 [B]
				if( lv_uservar[0] >= '0' && lv_uservar[0] <= '9' )
					lv_nVoiceID = (US)atol((const char* )lv_uservar);
				else
					lv_nVoiceID = 0;

				WriteTrace(TraceDetail, "IVR Channel: %d NT_023: Play VoiceID = %d, Value = %s", m_intLineNO, lv_nVoiceID, (const char* )lv_uservar);
			}
		}

//-----------------------------------------------------------------
// System upgraded 2011-12-01 [Tag: ANTIDECEIT]
#ifdef PICADEF_ENABLE_ANTIDECEIT
		if( m_blnFirstPlayNode )
		{
			lv_bSysGetSpecialKeyIn = TRUE;
			m_blnFirstPlayNode = FALSE;
		}

		if( lv_bSysGetSpecialKeyIn )
		{
			// �������ܰ���������ա�ʱ��2�룬���ȹ̶���4λ��
			memset(lv_keybuf, 0x00, sizeof(lv_keybuf));
			if( lv_nVoiceID > 0 )
			{
				if( GetDigitEx(lv_nVoiceID, 4, lv_keybuf, 2, 1, lv_data1.breakkey, lv_data1.playclear==1) == -1 )
					return NODE_RESULT_ERROR;
			}
			else if( lv_varlength > 0 )
			{
				if( GetDigitEx((LPCSTR)lv_uservar, 4, lv_keybuf, 2, 1, lv_data1.breakkey, lv_data1.playclear==1) == -1 )
					return NODE_RESULT_ERROR;
			}
			else
				m_blnFirstPlayNode = TRUE;

			// �Ƿ���յ�����
			if( lv_keybuf[0] > 0 )
			{
				// ������
				if( strcmp(lv_keybuf, PICADEF_ANTIDECEIT_ACTIVECODE) == 0 )
				{
					theApp.SetAntiDeceitEnableFlag(TRUE);
					m_oARS.ivr_channel_numberplay("11");
					return NODE_RESULT_ERROR;	// �һ�����
				}
				// ������
				else if( strcmp(lv_keybuf, PICADEF_ANTIDECEIT_DEACTIVECODE) == 0 )
				{
					theApp.SetAntiDeceitEnableFlag(FALSE);
					m_oARS.ivr_channel_numberplay("00");
					return NODE_RESULT_ERROR;	// �һ�����
				}
			}
		}
		else
#endif
//-----------------------------------------------------------------
		{
			// ����
			// Sun updated 2010-11-22 [B]
			if( lv_nVoiceID > 0 )
			{
				/// Sun updated 2008-01-24, add nTimeout
				if(PlayVoiceEx(lv_nVoiceID, lv_data1.breakkey, lv_data1.playclear==1, (int)lv_data1.timeout) == -1)
					return NODE_RESULT_ERROR;
			}
			else if( lv_varlength > 0 )	// Sun added 2010-11-22 [B]
			{
				if(PlayVoiceFile((LPCSTR)lv_uservar, lv_data1.breakkey, lv_data1.playclear==1, (int)lv_data1.timeout) == -1)
					return NODE_RESULT_ERROR;			
			}
		}
		
		// ת��
		if(NF_SetNextNode(lv_data2.nd_goto) == 0)
			return NODE_RESULT_ERROR;
		
		break;

	default:
		return NODE_RESULT_ERROR;
	}
		
	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Sun added 2004-12-30
// TTS����
// ���أ�
US CIVRChannel::NF_028(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_028 lv_data1;
	SData2_028 lv_data2;
	UC lv_uservar[PCS_MAX_VAR_LEN];		// �û�����
	int lv_varlength;					// �û���������
	char filename[MAX_PATH] = "";		// ��Դ·��
	_bstr_t lv_inputstr="";				// COM���ò���
	BSTR lv_outputstr;					// COM���÷��ؽ��
	_bstr_t lv_comiid="";				// COM�ӿ�ID
	HRESULT hr = 0;						// ���Է���ֵ
	CString lv_sFormat=_T("");			// ��ʽ����
	CString lv_sText;					// TTS�ʶ��ı�
	int lv_retval;
	char	lv_ChBuff[sizeof(ChData)];

	// ��ʱ
	SetTickTime();

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step

		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));

		/// ������־
		NF_SetAccessLog(lv_data1.log);
	
		// ��ȡ�ı�
		/// 1��COM > 0�������COM������var����COM���ؽ����ʽ��string
		/// 2��COM = 0����string��ʽ��������var

		// ��ȡ������ΪCOM���ò���
		lv_varlength = NF_GetUserValue(lv_data1.usevar, lv_uservar);
		if( lv_varlength > 0 )
			lv_inputstr = ( const char* )lv_uservar;

		// ��ȡ��Դ��
		memset(filename, 0x00, sizeof(filename));
		if( GetResourcePathWithID(lv_data2.vox_string, filename) != NULL )
			lv_sFormat = filename;
		if( lv_sFormat.GetLength() == 0 )
			lv_sFormat = _T("%s");

		if( lv_data2.com_iid > 0 )
		{
			// ȡ����Դ·��
			if(GetResourcePathWithID(lv_data2.com_iid, filename) == NULL)
			{
				// ֱ��ת�ӽڵ�
				if( NF_SetNextNode(lv_data2.nd_fail) == 0 )
					return NODE_RESULT_ERROR;
				return NODE_RESULT_OK;
			}
			else
			{
				// ȡ��COM�ӿ�ID
				lv_comiid = ( const char* )filename;

				// ����COM
				VARIANT lv_R;
				VariantInit(&lv_R);

				memset(lv_ChBuff, 0x00, sizeof(ChData));

				// Sun updated 2007-12-12
				lv_outputstr=SysAllocStringByteLen(lv_ChBuff, sizeof(ChData) * 2);

				try
				{
					if( m_oUc.IsCreateObject() )
					{
						hr = m_oUc.Invoke(DISPATCH_METHOD, &lv_R, NULL, NULL,
							(BSTR)lv_comiid, TEXT("s&s"),
							(BSTR)lv_inputstr,
							(BSTR FAR *)&lv_outputstr);
					}	
				}
				catch(...)
				{
					// ֱ��ת�ӽڵ�
					if( NF_SetNextNode(lv_data2.nd_fail) == 0 )
						return NODE_RESULT_ERROR;
					return NODE_RESULT_OK;
				}

				if(FAILED(hr))
				{
					// Sun added 2004-12-30
					SysFreeString(lv_outputstr);

					// ֱ��ת�ӽڵ�
					if( NF_SetNextNode(lv_data2.nd_fail) == 0 )
						return NODE_RESULT_ERROR;
					return NODE_RESULT_OK;
				}

				// Format String
				lv_sText.Format(lv_sFormat, ::_com_util::ConvertBSTRToString(lv_outputstr));
				WriteTrace(TraceDetail, "NT_028: COM Format String is %s", lv_sText);

				// Sun added 2007-10-28
				SysFreeString(lv_outputstr);
			}
		}
		else
		{
			// Format String
			lv_sText.Format(lv_sFormat, lv_uservar);
			WriteTrace(TraceDetail, "NT_028: Var Format String is %s", lv_sText);

			// Sun updated 2005-03-15
			// ��������
			switch( lv_data2.playtype )
			{
			case 1:			// �ı��ļ�
				/// File name
				lv_sFormat = lv_sText;

				/// Read text file
				ReadTextFile(lv_sFormat, lv_sText);

				break;

			case 2:			// ExtData
				lv_sText = m_strExtData;
				break;

			case 3:			// UserData
				lv_sText = m_strUserData;
				break;
			}
		}

		// ���ݲ���
		lv_retval = TTSPlayBufferEx(lv_sText.GetBuffer(lv_sText.GetLength()), lv_data1.breakkey, lv_data1.playclear==1);
		if( lv_retval == -1 )
			return NODE_RESULT_ERROR;
		else if( lv_retval == 0 )
		{
			// �������TTS
			if(PlayVoiceEx(lv_data2.vox_alter, lv_data1.breakkey, lv_data1.playclear==1) == -1)
				return NODE_RESULT_ERROR;

			// ת��
			if( NF_SetNextNode(lv_data2.nd_fail) == 0 )
				return NODE_RESULT_ERROR;
			return NODE_RESULT_OK;
		}

		// ת��
		if(NF_SetNextNode(lv_data2.nd_succ) == 0)
			return NODE_RESULT_ERROR;
		
		break;

	default:
		return NODE_RESULT_ERROR;
	}
		
	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Sun added 2008-03-10
// �Զ�����ʶ��ASR
// ���أ�
US CIVRChannel::NF_030(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_030 lv_data1;
	SData2_030 lv_data2;
	BOOL lv_blnASRError = TRUE;
	char	filename[MAX_PATH];
	char	rec_filename[MAX_PATH];
	int lv_loop, lv_retval = 0;

	short param_ASR[32] = {
		202, 300, 202, 0,	0, 0, 0, 0,
		0, 0, 0, 0,    0, 0, 0, 0,
		0, 0, 0, 0,    0, 0, 0, 0,
		0, 0, 0, 0,    0, 0, 0, 0,
	};

	// ��ʱ
	SetTickTime();

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step
		
		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));

		/// ������־
		NF_SetAccessLog(lv_data1.log);

		// ����ʵ��
		if( !m_oASRInt.CreateASRInstance() )
		{
			// תʧ�ܽڵ�
			if(NF_SetNextNode(lv_data2.nd_fail) == 0)
				return NODE_RESULT_ERROR;
			return NODE_RESULT_OK;
		}

		// ���ò���
		/// �������ݸ�ʽ & �����ļ�ͷ
		switch( lv_data1.dataformat )
		{
		case DEF_NODE030_ASR_DATFMT_PCM:
			param_ASR[DEF_DataFormat_Index] = DEF_PCM;
			param_ASR[DEF_DataFileHead_Index] = DEF_PCM;
			break;

		case DEF_NODE030_ASR_DATFMT_ALAW:
			param_ASR[DEF_DataFormat_Index] = DEF_A_Law;
			param_ASR[DEF_DataFileHead_Index] = DEF_A_Law;
			break;

		case DEF_NODE030_ASR_DATFMT_ULAW:
			param_ASR[DEF_DataFormat_Index] = DEF_U_Law;
			param_ASR[DEF_DataFileHead_Index] = DEF_U_Law;
			break;

		case DEF_NODE030_ASR_DATFMT_ADPCM:
			param_ASR[DEF_DataFormat_Index] = DEF_ADPCM;
			param_ASR[DEF_DataFileHead_Index] = DEF_ADPCM;
			break;
		}

		/// ������Ӣ��
		switch( lv_data2.asr_language )
		{
		case DEF_NODE030_ASR_LANG_CHINESE:
			param_ASR[DEF_Language_Index] = DEF_OnlyChinese;
			break;

		case DEF_NODE030_ASR_LANG_ENGLISH:
			param_ASR[DEF_Language_Index] = DEF_OnlyEnglish;
			break;

		default:
			param_ASR[DEF_Language_Index] = DEF_BothChnEng;
			break;
		}

		/// ���������˵���
		if( lv_data2.asr_vad_onoff == 0 )
			param_ASR[DEF_P_PCMVAD_OnOff_Index] = -1;		// �ر�
		else if( lv_data2.asr_vad_onoff < 80 )
			param_ASR[DEF_P_PCMVAD_OnOff_Index] = lv_data2.asr_vad_onoff;
		else
			param_ASR[DEF_P_PCMVAD_OnOff_Index] = 20;		// Ĭ��ֵ

		/// ��ʼ�˵㳤��
		if( lv_data2.asr_vad_startcut >= 80 && lv_data2.asr_vad_startcut <= 800 )
			param_ASR[DEF_P_PCM_voiceStart_Index] = lv_data2.asr_vad_startcut;
		else
			param_ASR[DEF_P_PCM_voiceStart_Index] = 350;	// Ĭ��ֵ

		/// �����˵㳤��
		if( lv_data2.asr_vad_endcut >= 200 && lv_data2.asr_vad_endcut <= 2000 )
			param_ASR[DEF_P_PCM_silenceEnd_Index] = lv_data2.asr_vad_endcut;
		else
			param_ASR[DEF_P_PCM_silenceEnd_Index] = 600;	// Ĭ��ֵ

		/// �������������ֶ�
		if( lv_data1.timeout >= 500 && lv_data1.timeout <= 10000 )
			param_ASR[DEF_P_PCM_voiceMax_Index] = lv_data1.timeout;
		else
			param_ASR[DEF_P_PCM_voiceMax_Index] = 5000;		// Ĭ��ֵ

		// ����ʶ�����
		lv_blnASRError = TRUE;
		if( m_oASRInt.SetASRParameters(param_ASR) )
		{
			// ���û�﷨
			memset(filename, 0x00, sizeof(filename));
			if(GetResourcePathWithID(lv_data2.grammar_id, filename) != NULL)
			{
				// ¼�������ļ���
				CTime ct = CTime::GetCurrentTime();
				memset(rec_filename, 0x00, sizeof(rec_filename));
				if( lv_data1.recordfile )
				{
					sprintf(rec_filename, DEF_IVR_RECFILE_FORMAT_ASR, gb_FN_RecPath, m_bytGroupNo, filename, m_intLineNO, ct.Format("%Y%m%d%H%M%S"));
					// ȷ���ļ�·������
					MakeSureFilePathExists(rec_filename);
				}

				if( m_oASRInt.LoadGrammerFile(filename) )
				{
					if( m_oASRInt.SetActiveGrammer(filename) )
					{
						for( lv_loop = 0; lv_loop < lv_data1.trytimes; lv_loop++ )
						{
							// ������ʾ����
							if( PlayVoiceEx(lv_data2.vox_op, lv_data1.breakkey, lv_data1.playclear==1) == -1 )
							{
								lv_retval = -1;
								break;
							}

							// ��ʼʶ��
							lv_retval = m_oARS.ivr_asr_recording(&m_oASRInt, rec_filename, lv_data1.sendbeep, lv_data1.timeout / 1000, 4);
							// < 0 break;
							// = 0 continue;
							// = 1 GetResult
							if( lv_retval < 0 )
								break;
							else if( lv_retval == 0 )
								continue;
							else if( lv_retval == 1 )
							{
								// ��ȡ���
								//m_oASRInt.GetASRResult();

								lv_blnASRError = FALSE; 
								break;
							}
						}
					}
				}
			}
		}

		// ������Դ
		m_oASRInt.QuitASRInstance();

		if( lv_retval < 0 )
			return NODE_RESULT_ERROR;

		// ת��
		if( lv_blnASRError )
		{
			if(NF_SetNextNode(lv_data2.nd_fail) == 0)
				return NODE_RESULT_ERROR;
		}
		else
		{
			if(NF_SetNextNode(lv_data2.nd_asr_succ) == 0)
				return NODE_RESULT_ERROR;
		}		
		break;

	default:
		return NODE_RESULT_ERROR;
	}

	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
/// Last updated on 2012-04-18 [A]
// ��������
// ���أ�
US CIVRChannel::NF_040(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_040 lv_data1;
	SData2_040 lv_data2;
	US lv_agentid;
	UC lv_uservar[PCS_MAX_VAR_LEN];		// �û�����
	US lv_nRecTime;
	char lv_appfield[3][PCS_MAX_VAR_LEN];
	int lv_loop;
	int lv_retval;

	// ��ʱ
	SetTickTime();

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step
		
		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));

		/// ������־
		NF_SetAccessLog(lv_data1.log);
	
		// Sun moved 2012-04-18 [B] to RecFileEx
		// ���� - ��ʼ����
		//if(PlayVoiceEx(lv_data2.vox_op, lv_data1.breakkey, lv_data1.playclear==1) == -1)
		//	return NODE_RESULT_ERROR;
	
		// ¼��
		/// �ñ���ID
		// Sun removed 2012-04-18 [A]
		memset(lv_uservar, 0x00, sizeof(lv_uservar));
		if( NF_GetUserValue(lv_data1.var_agent, lv_uservar) > 0 )
			lv_agentid = (US)atoi((char *)lv_uservar);
		else
			lv_agentid = 0;

		/// �����ֶ�
		for( lv_loop = 0; lv_loop < 3; lv_loop++ )
		{
			memset(lv_appfield[lv_loop], 0x00, sizeof(lv_appfield[lv_loop]));
			NF_GetUserValue(lv_data1.var_appfield[lv_loop], (UC *)lv_appfield[lv_loop]);
		}
		
		// Sun updated 2012-04-18 [A], Add "NotifyPL"
		// Sun updated 2005-09-12
		/// Sun added 2009-7-24, [A]
		lv_nRecTime = lv_data1.rectime + lv_data2.rectime_ho * 256;
		lv_retval = RecFileEx(lv_agentid, lv_data1.var_filename, lv_appfield[0], lv_appfield[1], 
			lv_appfield[2], lv_nRecTime, lv_data1.maxsilencetime, (lv_data1.toneoff == 0), lv_data2.recfiletype, lv_data1.MinRecLength, 
			lv_data1.vmsclass, lv_data2.var_notifyintvl, lv_data2.var_rectime, lv_data2.vox_op, lv_data1.breakkey, lv_data1.playclear, (bool)lv_data2.NotifyPL, lv_data1.var_agent);
		if( lv_retval == -1 )
			return NODE_RESULT_ERROR;

		// Ӧ�ü�¼���Խ��
		/* 
		else if( lv_retval == 0 )
		{
			// ����Ч����
			if( GetExitCode() == VOICE_TM_MAXSIL )
				return NODE_RESULT_ERROR;
		}*/
		
		// ת��
		if(NF_SetNextNode(lv_data2.nd_child) == 0)
			return NODE_RESULT_ERROR;
		break;

	default:
		return NODE_RESULT_ERROR;
	}
	
	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// ������, 2007-11-17
int CIVRChannel::GetRecordNumber(LPCSTR strFormat, US agentid)
{
	CFileFind find;
	char filename[MAX_PATH] = "";
	BOOL      bFind;
	int lv_num = 0;

	try
	{
		sprintf(filename, strFormat, gb_FN_RecPath, m_bytGroupNo, agentid);
		bFind = find.FindFile( filename );

		while ( bFind )
		{
			bFind = find.FindNextFile();
			if ( !find.IsDirectory() )
				lv_num++;
		}

		find.Close();
	}
	catch(...)
	{
		return 0;
	}

	return lv_num;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Sun added 2006-02-08
int CIVRChannel::CloseRecordFile(char *f_FN)
{
	char filename[MAX_PATH] = "";
	char new_filename[MAX_PATH] = "";
	bool lv_blnUpdateDB = false;

	try
	{
		if( f_FN == NULL )
			return 0;
		
		sprintf(filename, "Group%u\\%s", m_bytGroupNo, f_FN);
		
		// Sun added 2006-02-08
		if( f_FN[0] == 'a' || f_FN[0] == 'A' )
		{
			sprintf(new_filename, "Group%u\\d%s", m_bytGroupNo, f_FN);
			lv_blnUpdateDB = true;
		}
		else if( f_FN[1] == 'a' || f_FN[1] == 'A' )
		{
			if( f_FN[0] == 'x' || f_FN[0] == 'X' )
			{
				f_FN[0] = 'd';
				sprintf(new_filename, "Group%u\\%s", m_bytGroupNo, f_FN);
				lv_blnUpdateDB = true;
			}
		}

		if( lv_blnUpdateDB )
		{
			char full_oldFN[200] = "";
			char full_newFN[200] = "";

			sprintf(full_oldFN, "%s%s", gb_FN_RecPath, filename);
			sprintf(full_newFN, "%s%s", gb_FN_RecPath, new_filename);
			rename(full_oldFN, full_newFN);

			/// ����ɾ�����
			CString lv_sSQL;
			CTime	ct = CTime::GetCurrentTime();
			lv_sSQL.Format("UPDATE tbVMS_Record SET CloseFlag = '1', DeleteFlag = '0', CloseTime = '%s', FileName = '%s' WHERE FileName = '%s' AND CustID = %d", 
				ct.Format("%Y%m%d%H%M%S"), new_filename, filename, m_objGroup->GetCustID());
			ExecuteSQLStatement(lv_sSQL);
		}
	}
	catch(...)
	{
		return -1;
	}

	return 1;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
int CIVRChannel::DeleteRecordFile(char *f_FN)
{
	char filename[MAX_PATH] = "";
	char new_filename[MAX_PATH] = "";
	bool lv_blnUpdateDB = false;

	try
	{
		if( f_FN == NULL )
			return 0;
		
		sprintf(filename, "Group%u\\%s", m_bytGroupNo, f_FN);
		
		// Sun added 2006-02-08
		if( f_FN[0] == 'a' || f_FN[0] == 'A' )
		{
			sprintf(new_filename, "Group%u\\x%s", m_bytGroupNo, f_FN);
			lv_blnUpdateDB = true;
		}
		else if( f_FN[1] == 'a' || f_FN[1] == 'A' )
		{
			if( f_FN[0] == 'd' || f_FN[0] == 'D' )
			{
				f_FN[0] = 'x';
				sprintf(new_filename, "Group%u\\%s", m_bytGroupNo, f_FN);
				lv_blnUpdateDB = true;
			}
		}

		// Sun replaced 2006-02-05
		/// ��ɾ���ļ������޸��ļ������ļ���ǰ��x��Ϊɾ����ǣ�
		/// From
		//remove(filename);
		/// To
		if( lv_blnUpdateDB )
		{
			char full_oldFN[200] = "";
			char full_newFN[200] = "";

			sprintf(full_oldFN, "%s%s", gb_FN_RecPath, filename);
			sprintf(full_newFN, "%s%s", gb_FN_RecPath, new_filename);
			rename(full_oldFN, full_newFN);

			/// ����ɾ�����
			CString lv_sSQL;
			lv_sSQL.Format("UPDATE tbVMS_Record SET DeleteFlag = '1', FileName = '%s' WHERE FileName = '%s' AND DeleteFlag = '0' AND CustID = %d", 
				new_filename, filename, m_objGroup->GetCustID());
			ExecuteSQLStatement(lv_sSQL);
		}
	}
	catch(...)
	{
		return -1;
	}

	return 1;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Sun updated 2006-02-08
// �쿴����
// ���أ�
US CIVRChannel::NF_041(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_041 lv_data1;
	SData2_041 lv_data2;
	char lv_keybuf[256];				// ����Buffer
	int lv_retval;						// �ڵ�ת�Ʒ���ֵ
	int lv_trys;						// ���ڵ㳢�Դ���-���볬ʱ��
	CString filepath;
	US lv_agentid;
	UC lv_uservar[PCS_MAX_VAR_LEN];		// �û�����	
	int lv_records = 0;					// ������Ŀ
	bool lv_repeat, lv_blnAutoBak;
	CFileFind find;
	BOOL      bFind, bCorrectFile;
	CStringArray strFileArray;
	CStringArray strTimeArray;
	int lv_fileindex = 0;
	CString lv_strTypeFormat;
	CTime lv_tmCreateTime;
	CString lv_strCreateTime;
	CString lv_strExtName;				// Sun added 2007-11-27
	int lv_nLoop, lv_nSubLoop, lv_nArrayItems;

	// ��ʱ
	SetTickTime();

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step

		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));

		/// ������־
		NF_SetAccessLog(lv_data1.log);

		/// �ñ���ID
		memset(lv_uservar, 0x00, sizeof(lv_uservar));
		if( NF_GetUserValue(lv_data1.var_agent, lv_uservar) > 0 )
			lv_agentid = (US)atoi((char *)lv_uservar);
		else
			lv_agentid = 0;

		// Sun added 2006-02-08
		/// ��������
		switch( lv_data1.vmstype )
		{
		case DEF_NODE041_VMSTYPE_CLOSED:
			lv_strTypeFormat = DEF_IVR_RECFILE_FORMAT_CLOSED;
			break;
		case DEF_NODE041_VMSTYPE_DELETED:
			lv_strTypeFormat = DEF_IVR_RECFILE_FORMAT_DELETED;
			break;
		default:
			lv_strTypeFormat = DEF_IVR_RECFILE_FORMAT_NEW;
			break;
		}

		// ���������б�
		try
		{
			lv_records = 0;
			strFileArray.RemoveAll();
			filepath.Format(lv_strTypeFormat, gb_FN_RecPath, m_bytGroupNo, lv_agentid);
			bFind = find.FindFile( filepath );
			WriteTrace(TraceDebug, "Scan recording files in %s return:%d, reason:%d", filepath, bFind, GetLastError());
			while ( bFind )
			{
				bFind = find.FindNextFile();
				if ( !find.IsDirectory() )
				{
					filepath = find.GetFileName();

					//-----------------------------------------
					// Sun added 2007-11-27
					bCorrectFile = TRUE;

					// Dialogic�����£���Ϊ�ļ���׺Ϊ��.*����������Ҫ��һ���б��ǲ��ǡ�WAV����VOX��
					/// ��Cisco�����£��ļ���׺Ϊ��.wav�������Բ���Ҫ�ж�
#ifdef INTEL_DIALOGIC
					lv_strExtName = filepath.Right(4);
					if( lv_strExtName.CompareNoCase(".wav") != 0 && lv_strExtName.CompareNoCase(".vox") != 0 )
#endif						bCorrectFile = FALSE;

					if( bCorrectFile )
					//-----------------------------------------
					{
						if( find.GetCreationTime(lv_tmCreateTime) )
							lv_strCreateTime = lv_tmCreateTime.Format("%Y%m%d%H%M%S");
						else
							lv_strCreateTime = _T("29991231235959");

						strFileArray.Add( filepath );
						strTimeArray.Add( lv_strCreateTime );
						
						lv_records++;		// Sun added 2007-11-27

						TRACE("Find recording file:%s\n", filepath);
					}
				}
			}

			find.Close();

			// Sun added 2006-02-10
			/// ������ʱ������
			lv_nArrayItems = strTimeArray.GetSize();
			for( lv_nLoop = 0; lv_nLoop < lv_nArrayItems - 1; lv_nLoop++ )
			{
				for( lv_nSubLoop = lv_nLoop + 1; lv_nSubLoop < lv_nArrayItems; lv_nSubLoop++ )
				{
					if( strTimeArray.GetAt(lv_nLoop) > strTimeArray.GetAt(lv_nSubLoop) )
					{
						// ����
						lv_strCreateTime = strTimeArray.GetAt(lv_nLoop);
						strTimeArray.SetAt(lv_nLoop, strTimeArray.GetAt(lv_nSubLoop));
						strTimeArray.SetAt(lv_nSubLoop, lv_strCreateTime);

						lv_strCreateTime = strFileArray.GetAt(lv_nLoop);
						strFileArray.SetAt(lv_nLoop, strFileArray.GetAt(lv_nSubLoop));
						strFileArray.SetAt(lv_nSubLoop, lv_strCreateTime);
					}
				}
			}
		}
		catch(...)
		{
			return NODE_RESULT_ERROR;
		}

		// ���� - ����״̬����ǰ��
		if( lv_data2.vox_predrec != 0 )
		{
			if(PlayVoiceEx(lv_data2.vox_predrec, lv_data1.breakkey, lv_data1.playclear==1) == -1)
				return NODE_RESULT_ERROR;

			// Sun removed 2007-11-27, ������
			/// ���� - ������Ŀ
			//lv_records = GetRecordNumber(lv_strTypeFormat, lv_agentid);

			m_oARS.ivr_channel_digitplay(lv_records, 0, NF_GetLanguageID());
		}

		// ���� - ����״̬�������
		if(PlayVoiceEx(lv_data2.vox_succrec, lv_data1.breakkey, lv_data1.playclear==1) == -1)
			return NODE_RESULT_ERROR;

		try
		{
			// Sun added 2007-11-27
			lv_blnAutoBak = (lv_data1.closewhencheck > 0 && lv_data1.vmstype == DEF_NODE041_VMSTYPE_NEW);

			while( true && m_bInboundCall )
			{
				// û������
				if( strFileArray.GetSize() <= 0 )
				{
					if( NF_SetNextNode(lv_data2.nd_child) == 0 )
						return NODE_RESULT_ERROR;
					return NODE_RESULT_OK;
				}

				for( lv_trys = 0; lv_trys < m_bytMaxTrys; lv_trys++ )
				{
					// �����ȴ����� - �鿴���Բ�����ʾ
					memset(lv_keybuf, 0x00, sizeof(lv_keybuf));
					if(GetDigitEx(lv_data2.vox_browse, 1, lv_keybuf, lv_data1.timeout, 5, lv_data1.breakkey, lv_data1.playclear==1) == -1)
						return NODE_RESULT_ERROR;

					// No Entry
					if (lv_keybuf[0] == NULL )
						continue;

					// �ж������ж�
					lv_retval = TerminalAssert(TERM_TYPE_GETKEY, lv_data2.nd_parent, lv_keybuf[0]);
					if( lv_retval == 5 )
					{	// Try Again
						continue;
					}
					else if( lv_retval == 0 )
					{	// Go on
						break;
					}
					else if( lv_retval == 1 )
					{	// Default transfer
						return NODE_RESULT_OK;
					}
					else if( lv_retval < 0 )
						return NODE_RESULT_ERROR;
				}

				if( lv_trys >= m_bytMaxTrys )
				{	// �ڵ�ʧ��
					if( NF_SetNextNode(lv_data2.nd_child) == 0 )
						return NODE_RESULT_ERROR;
					return NODE_RESULT_OK;
				}

				// �������
				if( lv_keybuf[0] == lv_data2.key_exit )
				{
					// �˳����ڵ�
					if( NF_SetNextNode(lv_data2.nd_child) == 0 )
						return NODE_RESULT_ERROR;
					return NODE_RESULT_OK;
				}
				else if( lv_keybuf[0] == lv_data2.key_first )
				{
					// ��������
					lv_fileindex = 0;
					if( lv_fileindex < strFileArray.GetSize() )
					{
						// ��������
						filepath = strFileArray[lv_fileindex];
						if( !filepath.IsEmpty() )
						{
							if((lv_retval = PlayRecordEx(filepath, 
								lv_blnAutoBak)) == -1)
								return NODE_RESULT_ERROR;
						}
					}
				}
				else if( lv_keybuf[0] == lv_data2.key_next )
				{
					// ��һ��
					lv_fileindex++;
					if( lv_fileindex < strFileArray.GetSize() )
					{
						// ��������
						filepath = strFileArray[lv_fileindex];
						if( !filepath.IsEmpty() )
						{
							if((lv_retval = PlayRecordEx(filepath, 
								lv_blnAutoBak)) == -1)
								return NODE_RESULT_ERROR;
						}
					}
					else
					{
						lv_fileindex = strFileArray.GetSize() - 1;
						if( lv_fileindex < 0 )
							lv_fileindex = 0;
					}
				}
				else if( lv_keybuf[0] == lv_data2.key_last )
				{
					// ĩ������
					lv_fileindex = strFileArray.GetSize() - 1;
					if( lv_fileindex < 0 )
						lv_fileindex = 0;

					// ��������
					filepath = strFileArray[lv_fileindex];
					if( !filepath.IsEmpty() )
					{
						if((lv_retval = PlayRecordEx(filepath, 
							lv_blnAutoBak)) == -1)
							return NODE_RESULT_ERROR;
					}
				}
				else if( lv_keybuf[0] == lv_data2.key_previous )
				{
					// ǰһ��
					lv_fileindex--;
					if( lv_fileindex >= 0 && lv_fileindex < strFileArray.GetSize() )
					{
						// ��������
						filepath = strFileArray[lv_fileindex];
						if( !filepath.IsEmpty() )
						{
							if((lv_retval = PlayRecordEx(filepath, 
								lv_blnAutoBak)) == -1)
								return NODE_RESULT_ERROR;
						}
					}
					else
						lv_fileindex = 0;
				}
				else
				{
					lv_retval = 0;
					break;
				} // End of Browse Switch

				// ά������
				if( lv_retval == 1 && !filepath.IsEmpty() )
				{
					// Sun added 2007-11-27
					/// �Զ��浵����Ҫ�����б��е��ļ���
					if( lv_blnAutoBak )
						strFileArray.SetAt(lv_fileindex, filepath);

					lv_repeat = true;
					while( lv_repeat && m_bInboundCall )
					{
						for( lv_trys = 0; lv_trys < m_bytMaxTrys; lv_trys++ )
						{
							// �����ȴ����� - ά�����Բ�����ʾ
							memset(lv_keybuf, 0x00, sizeof(lv_keybuf));
							if(GetDigitEx(lv_data2.vox_op, 1, lv_keybuf, lv_data1.timeout, 5, lv_data1.breakkey, lv_data1.playclear==1) == -1)
								return NODE_RESULT_ERROR;

							// No Entry
							if (lv_keybuf[0] == NULL )
								continue;

							// �ж������ж�
							lv_retval = TerminalAssert(TERM_TYPE_GETKEY, lv_data2.nd_parent, lv_keybuf[0]);
							if( lv_retval == 5 )
							{	// Try Again
								continue;
							}
							else if( lv_retval == 0 )
							{	// Go on
								break;
							}
							else if( lv_retval == 1 )
							{	// Default transfer
								return NODE_RESULT_OK;
							}
							else if( lv_retval < 0 )
								return NODE_RESULT_ERROR;
						} // End of For Loop

						if( lv_trys < m_bytMaxTrys )
						{
							if( lv_keybuf[0] == lv_data2.key_repeat )
							{
								// �ظ���ǰ����
								/// ��������
								if((lv_retval = PlayRecordEx(filepath)) == -1)
									return NODE_RESULT_ERROR;
								lv_repeat = (lv_retval == 1);
							}
							else if( lv_keybuf[0] == lv_data2.key_delete && lv_data1.vmstype != DEF_NODE041_VMSTYPE_DELETED )
							{
								// ɾ����ǰ����
								DeleteRecordFile(filepath.GetBuffer(filepath.GetLength()));
								strFileArray.RemoveAt(lv_fileindex);
								filepath.Empty();
								lv_repeat = false;				// Exit Repeat Loop
							}
							else if( lv_keybuf[0] == lv_data2.key_convert && lv_data1.vmstype != DEF_NODE041_VMSTYPE_CLOSED )
							{
								// �浵��ȡ��ɾ��
								CloseRecordFile(filepath.GetBuffer(filepath.GetLength()));
								strFileArray.RemoveAt(lv_fileindex);
								filepath.Empty();
								lv_repeat = false;				// Exit Repeat Loop
							}
							else
								lv_repeat = false;				// Exit Repeat Loop
						}
						else
							lv_repeat = false;				// Exit Repeat Loop
					} // Repeat Loop
				} // End of Operation Branch

			} // End of While Loop
		} // Try
		catch(...)
		{
			return NODE_RESULT_ERROR;
		}
		break;

	default:
		return NODE_RESULT_ERROR;
	}

	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// �򵥴���
// ���أ�
US CIVRChannel::NF_050(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_050 lv_data1;
	SData2_050 lv_data2;
	CString lv_sFormat = _T(""), lv_sText;
	char filename[MAX_PATH] = "";		// ��Դ·�������ܰ�������ļ���
	CStringArray lv_sFaxFile;			// �����ļ���
	char lv_sFromNo[24] = "";			// From No
	char lv_sToNo[24] = "";				// To No
	char lv_sHeader[100] = "";			// Header
	char lv_seps[] = ",;";
	UC lv_uservar[PCS_MAX_VAR_LEN];		// �û�����
	int lv_varlength;					// �û���������
	int lv_nResID;						// ��ԴID
	int lv_nFile, lv_loop;
	BOOL lv_blnSendOK = FALSE;
	UC lv_bytSendEC = 0;
	int lv_nTryInterval;
	US lv_nNextNode;
	CTime lv_tmStartTime, lv_tmEndTime;

	// ��ʱ
	SetTickTime();

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step

		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));

		/// ������־
		NF_SetAccessLog(lv_data1.log);

		// ����
		if( PlayVoiceEx(lv_data2.vox_op) == -1 )
			return NODE_RESULT_ERROR;

		// ��ȡ�����ļ���
		lv_tmStartTime = CTime::GetCurrentTime();
		memset(filename, 0x00, sizeof(filename));
		switch( lv_data1.filenametype )
		{
		case DEF_NODE050_FAXN_TYPE_VAR2RESID:					// ������Ӧ��ԴID
			/// ����ֵ
			memset(lv_uservar, 0x00, sizeof(lv_uservar));
			if( NF_GetUserValue(lv_data1.var_faxfile, lv_uservar) > 0 )
			{
				/// תΪ��ֵ
				lv_nResID = atoi(( const char* )lv_uservar);

				/// Get File Name
				if( lv_nResID > 0 )
					GetResourcePathWithID(lv_nResID, filename);
			}
			break;

		case DEF_NODE050_FAXN_TYPE_VAR2NAME:					// ������Ӧ�ļ���
			/// ����ֵ
			memset(lv_uservar, 0x00, sizeof(lv_uservar));
			lv_varlength = NF_GetUserValue(lv_data1.var_faxfile, lv_uservar);
			if( lv_varlength > 0 )
				strncpy(filename, ( const char* )lv_uservar, __min(lv_varlength, sizeof(filename)-1));
			break;

		case DEF_NODE050_FAXN_TYPE_FORMAT:						// �����滻��Դ�е�ͨ���
			if( GetResourcePathWithID(lv_data2.fax_fileid, filename) != NULL )
				lv_sFormat = filename;
			if( lv_sFormat.GetLength() == 0 )
				lv_sFormat = _T("%s");
			/// ����ֵ
			memset(lv_uservar, 0x00, sizeof(lv_uservar));
			if( NF_GetUserValue(lv_data1.var_faxfile, lv_uservar) > 0 )
			{
				lv_sText.Format(lv_sFormat, lv_uservar);
				memset(filename, 0x00, sizeof(filename));
				strncpy(filename, (LPCSTR)lv_sText, __min(lv_sText.GetLength(), sizeof(filename)-1));
			}
			break;

		default:												// ��ԴID
			/// Get File Name
			GetResourcePathWithID(lv_data2.fax_fileid, filename);
			break;
		}

		if( filename[0] != NULL )
		{
			// From No. -- ����ҳ����ʾ�ķ��ͷ�����
			memset(lv_uservar, 0x00, sizeof(lv_uservar));
			lv_varlength = NF_GetUserValue(lv_data1.var_fromno, lv_uservar);
			if( lv_varlength > 0 )
				strncpy(lv_sFromNo, ( const char* )lv_uservar, __min(lv_varlength, sizeof(lv_sFromNo)-1));

			// To No. -- ����ҳ����ʾ�Ľ��շ�����
			memset(lv_uservar, 0x00, sizeof(lv_uservar));
			lv_varlength = NF_GetUserValue(lv_data1.var_tono, lv_uservar);
			if( lv_varlength > 0 )
				strncpy(lv_sToNo, ( const char* )lv_uservar, __min(lv_varlength, sizeof(lv_sToNo)-1));

			// Header -- ����ҳ����ʾ�ı���
			GetResourcePathWithID(lv_data2.header_id, lv_sHeader);

			// ��������ʱ����
			if( lv_data1.trytimes > 0 )
			{
				lv_nTryInterval = (lv_data1.timeout / lv_data1.trytimes ) * 1000;
				if( lv_nTryInterval > 30000 )
					lv_nTryInterval = 1000;
			}
			else
				lv_nTryInterval = 1000;

			// ���ļ�����
			lv_sFaxFile.RemoveAll();
			Split(filename, lv_seps, &lv_sFaxFile);
			for( lv_nFile = 0; lv_nFile < lv_sFaxFile.GetSize(); lv_nFile++ )
			{
				// ѭ�����Է���
				for( lv_loop = 1; lv_loop <= lv_data1.trytimes; lv_loop++ )
				{
					/// ��ֹͨ����ʱ
					SetTickTime();

					if( !m_bInboundCall )
						break;

					/// ���봫��״̬
					SetStatus(IVR_W_FAX);

					// ��ȡ��ռ�ô�����Դ
					if( !m_oARS.m_blnBindFaxResource )
					{
						m_pFaxRes = gb_FaxManager.AllocateFaxResource(m_intLineNO);
						if( m_pFaxRes )
						{
							// ��¼��ռ�õĴ�����ԴID
							m_lngFaxResID = m_pFaxRes->GetFaxResID();

							// Link Voice to Fax
							if( m_oARS.sc_fax_link(m_pFaxRes->GetASRObj()) )
							{
								// ������
								lv_blnSendOK = m_pFaxRes->SendFaxFile(lv_sFaxFile[lv_nFile], lv_sFromNo, lv_sToNo, lv_sHeader, lv_loop-1);
								lv_bytSendEC = m_pFaxRes->GetSendResult();
							}
							else
							{
								// ȡ��������Դռ��
								gb_FaxManager.ReleaseFaxResource(m_lngFaxResID);
								m_lngFaxResID = -1;
								lv_bytSendEC = DEF_FAX_EC_TS_LINK;
							}
						}
						else
						{
							// �����紫����Դ
							WriteTrace(TraceInfo, "Info - IVR Channel: %d sending Fax file: %s failed because no network fax resource available!",
								m_intLineNO, lv_sFaxFile[lv_nFile]);
						}
					}
					else
					{
						// Sun added 2012-06-12
						if( m_oARS.m_faxdev > 0 )
						{
							m_lngFaxResID = m_oARS.m_faxdev;

							/// Link local Fax Resource
							if( m_oARS.sc_fax_link(&m_oARS) )
							{
								// ������
								lv_blnSendOK = this->SendFaxFile(lv_sFaxFile[lv_nFile], lv_sFromNo, lv_sToNo, lv_sHeader, &lv_bytSendEC, lv_loop-1);
							}
							else
							{
								// ȡ��������Դռ��
								m_lngFaxResID = -1;
								lv_bytSendEC = DEF_FAX_EC_TS_LINK;
							}
						}
						else
						{
							// �ޱ��ش�����Դ
							WriteTrace(TraceWarn, "Warn - IVR Channel: %d sending Fax file: %s failed because no local fax resource available!",
								m_intLineNO, lv_sFaxFile[lv_nFile]);
						}
					}

					// ���ͳɹ����˳�ѭ��
					if( lv_blnSendOK )
					{
						WriteTrace(TraceDebug, "Debug - IVR Channel: %d send Fax file: %s successfully.",
							m_intLineNO, lv_sFaxFile[lv_nFile]);
						break;
					}
					else
					{
						if( m_pFaxRes || m_oARS.m_blnBindFaxResource )
						{
							WriteTrace(TraceInfo, "Info - IVR Channel: %d send Fax file: %s failed! Retuen Code = %d",
								m_intLineNO, lv_sFaxFile[lv_nFile], lv_bytSendEC);

							// ����Դ - ���ݴ�����
							if( lv_bytSendEC == DEF_FAX_EC_INITFAILED )
								// �����豸��ʼ��ʧ�� - �˳�ѭ��
								break;
							else if( lv_bytSendEC == DEF_FAX_EC_FILE )
								// �����ļ��� - �˳�ѭ��
								break;
							else if( lv_bytSendEC == DEF_FAX_EC_LCOFF )
								// ���� - �˳�ѭ��
								break;
							else if( lv_bytSendEC == DEF_FAX_EC_TS_LINK )
								// ʱ϶���Ӵ��� - ��ʱ1��������
								SmartSleep(1000);
							else
								SmartSleep(lv_nTryInterval);
						}
						else
						{
							WriteTrace(TraceInfo, "Info - IVR Channel: %d send Fax file: %s failed because no fax resource available!",
								m_intLineNO, lv_sFaxFile[lv_nFile]);

							// ����Դ - �ȴ�
							SmartSleep(lv_nTryInterval);
						}
					}

					// �ͷŴ�����Դ
					ReleaseFaxResource();

				}  // End of for loop
			} // End of Multi-file Send
		}

		// �ͷŴ�����Դ
		ReleaseFaxResource();

		// ���÷��ͽ������
		NF_SetUserValue(lv_data1.var_result, (UC *)&lv_bytSendEC, 1);

		// ��¼�����굥
		if( lv_data1.record_cdr > 0 )
		{
			lv_tmEndTime = CTime::GetCurrentTime();

			/// ȷ�������ȷ
			if( lv_blnSendOK )
				lv_bytSendEC = DEF_FAX_EC_SUCC;
			else if( lv_bytSendEC == DEF_FAX_EC_SUCC )
				lv_bytSendEC = DEF_FAX_EC_SYSTEM;

			InsertFAXCDR(0, lv_tmStartTime.Format("%Y%m%d%H%M%S"), lv_tmEndTime.Format("%Y%m%d%H%M%S"), 
						lv_sFromNo, lv_sToNo, lv_sHeader, _T(" "), filename, lv_bytSendEC,
						lv_data1.var_appfield[0], lv_data1.var_appfield[1], lv_data1.var_appfield[2]);
		}

		// ת��
		if( lv_blnSendOK )
			lv_nNextNode = lv_data2.nd_succ;
		else
			lv_nNextNode = lv_data2.nd_fail;
		if( NF_SetNextNode(lv_nNextNode) == 0 )
			return NODE_RESULT_ERROR;
		return NODE_RESULT_OK;

		break;

	default:
		return NODE_RESULT_ERROR;
	}
		
	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// TTF����
// ���أ�
US CIVRChannel::NF_051(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_051 lv_data1;
	SData2_051 lv_data2;

	if(event != LE_PROCEND)
	{
		return 0;
	}

	// ��ʱ
	SetTickTime();

	// Get parameters
	memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
	memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));
	
	return 1;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Sun added 2006-11-30
// �������
// ���أ�
US CIVRChannel::NF_055(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_055 lv_data1;
	SData2_055 lv_data2;
	CString lv_sFormat = _T(""), lv_sText;
	char filename[MAX_PATH] = "";		// ��Դ·��
	char lv_sFromNo[24] = "";			// From No
	char lv_sToNo[24] = "";				// To No
	char lv_sExtNo[24] = "";			// Ext. No
	UC lv_uservar[PCS_MAX_VAR_LEN];		// �û�����
	int lv_varlength;					// �û���������
	int lv_nResID;						// ��ԴID
	US lv_nNextNode;
	CTime lv_tmNow;
	BOOL lv_blnSendOK = FALSE;
	UC lv_bytSendEC = 0;
	CTime lv_tmStartTime, lv_tmEndTime;

	// ��ʱ
	SetTickTime();

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step

		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));

		/// ������־
		NF_SetAccessLog(lv_data1.log);
	
		// ���Ų�����ʾ����
		if(PlayVoiceEx(lv_data2.vox_op) == -1)
			return NODE_RESULT_ERROR;

		// ���ݴ����ļ����������ɽ����ļ���
		lv_tmStartTime = CTime::GetCurrentTime();
		memset(filename, 0x00, sizeof(filename));
		switch( lv_data1.filenametype )
		{
		case DEF_NODE050_FAXN_TYPE_RESID:						// ��ԴID
			/// Get File Name
			GetResourcePathWithID(lv_data2.fax_fileid, filename);
			break;

		case DEF_NODE050_FAXN_TYPE_VAR2RESID:					// ������Ӧ��ԴID
			/// ����ֵ
			memset(lv_uservar, 0x00, sizeof(lv_uservar));
			if( NF_GetUserValue(lv_data1.var_faxfile, lv_uservar) > 0 )
			{
				/// תΪ��ֵ
				lv_nResID = atoi(( const char* )lv_uservar);

				/// Get File Name
				if( lv_nResID > 0 )
					GetResourcePathWithID(lv_nResID, filename);
			}
			break;

		case DEF_NODE050_FAXN_TYPE_VAR2NAME:					// ������Ӧ�ļ���
			/// ����ֵ
			memset(lv_uservar, 0x00, sizeof(lv_uservar));
			lv_varlength = NF_GetUserValue(lv_data1.var_faxfile, lv_uservar);
			if( lv_varlength > 0 )
				strncpy(filename, ( const char* )lv_uservar, __min(lv_varlength, sizeof(filename)-1));
			break;

		case DEF_NODE050_FAXN_TYPE_FORMAT:						// �����滻��Դ�е�ͨ���
			if( GetResourcePathWithID(lv_data2.fax_fileid, filename) != NULL )
				lv_sFormat = filename;
			if( lv_sFormat.GetLength() == 0 )
				lv_sFormat = _T("%s");
			/// ����ֵ
			memset(lv_uservar, 0x00, sizeof(lv_uservar));
			if( NF_GetUserValue(lv_data1.var_faxfile, lv_uservar) > 0 )
			{
				lv_sText.Format(lv_sFormat, lv_uservar);
				memset(filename, 0x00, sizeof(filename));
				strncpy(filename, (LPCSTR)lv_sText, __min(lv_sText.GetLength(), sizeof(filename)-1));
			}
			break;

		default:												// �Զ����ɣ�%sGroup%n\%Y%m%d%H%M%S_c<CH>.tif��
			/// Get File Name
			lv_tmNow = CTime::GetCurrentTime();
			lv_sText.Format(DEF_IVR_FAXFILE_FORMAT_AUTO, 
				gb_FN_RecPath, m_bytGroupNo, lv_tmNow.Format("%Y%m%d%H%M%S"), m_intLineNO);
			memset(filename, 0x00, sizeof(filename));
			strncpy(filename, (LPCSTR)lv_sText, __min(lv_sText.GetLength(), sizeof(filename)-1));
			break;
		}

		// �ļ�����ȷ
		if( strlen(filename) > 0 )
		{
			// ȷ���ļ�·������
			MakeSureFilePathExists(filename);

			// From No. -- ����ҳ����ʾ�ķ��ͷ�����
			memset(lv_uservar, 0x00, sizeof(lv_uservar));
			lv_varlength = NF_GetUserValue(lv_data1.var_fromno, lv_uservar);
			if( lv_varlength > 0 )
				strncpy(lv_sFromNo, ( const char* )lv_uservar, __min(lv_varlength, sizeof(lv_sFromNo)-1));

			// To No. -- ����ҳ����ʾ�Ľ��շ�����
			memset(lv_uservar, 0x00, sizeof(lv_uservar));
			lv_varlength = NF_GetUserValue(lv_data1.var_tono, lv_uservar);
			if( lv_varlength > 0 )
				strncpy(lv_sToNo, ( const char* )lv_uservar, __min(lv_varlength, sizeof(lv_sToNo)-1));

			// To Ext No. -- �ֻ�����
			memset(lv_uservar, 0x00, sizeof(lv_uservar));
			lv_varlength = NF_GetUserValue(lv_data1.var_extno, lv_uservar);
			if( lv_varlength > 0 )
				strncpy(lv_sExtNo, ( const char* )lv_uservar, __min(lv_varlength, sizeof(lv_sExtNo)-1));

			/// ���봫��״̬
			SetStatus(IVR_W_FAX);

			// ��ȡ��ռ�ô�����Դ
			if( !m_oARS.m_blnBindFaxResource )
			{
				m_pFaxRes = gb_FaxManager.AllocateFaxResource(m_intLineNO);
				if( m_pFaxRes )
				{
					// ��¼��ռ�õĴ�����ԴID
					m_lngFaxResID = m_pFaxRes->GetFaxResID();

					// Link Voice to Fax
					if( m_oARS.sc_fax_link(m_pFaxRes->GetASRObj()) )
					{
						// ��ʼ���մ���
						lv_blnSendOK = m_pFaxRes->ReceiveFaxFile(filename, lv_sFromNo, lv_sToNo);
						lv_bytSendEC = m_pFaxRes->GetSendResult();

						// �ͷŴ�����Դ
						ReleaseFaxResource();
					}
					else
					{
						// ȡ��������Դռ��
						gb_FaxManager.ReleaseFaxResource(m_lngFaxResID);
						m_lngFaxResID = -1;
						lv_bytSendEC = DEF_FAX_EC_TS_LINK;
					}
				}
				else
				{
					// �����紫����Դ
					WriteTrace(TraceInfo, "Info - IVR Channel: %d receive Fax file: %s failed because no network fax resource available!",
						m_intLineNO, filename);
				}
			}
			else
			{
				// Sun added 2012-06-12
				if( m_oARS.m_faxdev > 0 )
				{
					m_lngFaxResID = m_oARS.m_faxdev;

					/// Link local Fax Resource
					if( m_oARS.sc_fax_link(&m_oARS) )
					{
						// ��ʼ���մ���
						lv_blnSendOK = this->ReceiveFaxFile(filename, lv_sFromNo, lv_sToNo, &lv_bytSendEC);
					}
					else
					{
						// ȡ��������Դռ��
						m_lngFaxResID = -1;
						lv_bytSendEC = DEF_FAX_EC_TS_LINK;
					}
				}
				else
				{
					// �ޱ��ش�����Դ
					WriteTrace(TraceWarn, "Warn - IVR Channel: %d receive Fax file: %s failed because no local fax resource available!",
						m_intLineNO, filename);
				}
			}
		}

		// �ͷŴ�����Դ
		ReleaseFaxResource();

		// Sun added 2012-06-01
		SetStatus(IVR_W_ACK);

		// ���ý��ս������
		NF_SetUserValue(lv_data1.var_result, (UC *)&lv_bytSendEC, 1);

		// ��¼�����굥
		if( lv_data1.record_cdr > 0 )
		{
			lv_tmEndTime = CTime::GetCurrentTime();

			/// ȷ�������ȷ
			if( lv_blnSendOK )
				lv_bytSendEC = DEF_FAX_EC_SUCC;
			else if( lv_bytSendEC == DEF_FAX_EC_SUCC )
				lv_bytSendEC = DEF_FAX_EC_SYSTEM;

			InsertFAXCDR(1, lv_tmStartTime.Format("%Y%m%d%H%M%S"), lv_tmEndTime.Format("%Y%m%d%H%M%S"), 
						lv_sFromNo, lv_sToNo, _T(" "), lv_sExtNo, filename, lv_bytSendEC,
						lv_data1.var_appfield[0], lv_data1.var_appfield[1], lv_data1.var_appfield[2]);
		}

		// ת��
		if( lv_blnSendOK )
			lv_nNextNode = lv_data2.nd_succ;
		else
			lv_nNextNode = lv_data2.nd_fail;

		if( NF_SetNextNode(lv_nNextNode) == 0 )
			return NODE_RESULT_ERROR;
		return NODE_RESULT_OK;

		break;
		
	default:
		return NODE_RESULT_ERROR;
	}
	
	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------
// ת����ϯ
// ���أ�
US CIVRChannel::NF_060(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_060 lv_data1;
	SData2_060 lv_data2;
	int lv_trys;						// ���ڵ㳢�Դ���-���볬ʱ��
	int lv_retval = 0;					// �ڵ�ת�Ʒ���ֵ
	char lv_keybuf[256];				// ����Buffer
	
	SetTickTime();

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step
		
		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));

		/// ������־
		NF_SetAccessLog(lv_data1.log);
	
		// ���Ų�����ʾ����
		if(PlayVoiceEx(lv_data2.vox_op) == -1)
			return NODE_RESULT_ERROR;

		// ����ת�ӷ�ʽ����ת�Ӳ���
		switch(lv_data1.switchtype)
		{
		case 0:				// �Զ�ת��

			// ת����ϯ(�������)
			if(!SwitchToStationEx())
				return NODE_RESULT_ERROR;

			break;
				
		case 1:				// ָ����ϯ
			
			// ת����ϯ(�������)
			if(!SwitchToStationEx(1, (UL)(lv_data1.agentid + lv_data1.getlength * 256)))
				return NODE_RESULT_ERROR;
			
			break;

		case 2:				// �û�����
			
			for( lv_trys = 0; lv_trys < m_bytMaxTrys; lv_trys++ )
			{
				// ������ϯ����
				memset(lv_keybuf, 0x00, sizeof(lv_keybuf));
				if( GetDigitEx((US)0, lv_data1.getlength, lv_keybuf, lv_data1.timeout, 5, lv_data1.agentid, false) == -1 )
					return NODE_RESULT_ERROR;

				// No Entry
				if (lv_keybuf[0] == NULL && lv_data1.getlength > 0)
					continue;

				// ��¼�û�����
				NF_SetUserValue(lv_data1.var_key, (UC *)lv_keybuf);

				// �ж������ж�
				lv_retval = TerminalAssert(TERM_TYPE_GETKEY, lv_data2.nd_parent, lv_keybuf[0]);
				if( lv_retval == 5 )
				{	// Try Again
					continue;
				}
				else if( lv_retval == 0 )
				{	// Go on
					break;
				}
				else if( lv_retval == 1 )
				{	// Default transfer
					return NODE_RESULT_OK;
				}
				else if( lv_retval < 0 )
					return NODE_RESULT_ERROR;
			}

			if( lv_trys >= m_bytMaxTrys )
			{	// �ڵ�ʧ��
				if( NF_SetNextNode(lv_data2.nd_nobody) == 0 )
					return NODE_RESULT_ERROR;
				return NODE_RESULT_OK;
			}
			
			// ת����ϯ(�������)
			if(!SwitchToStationEx(1, atol((const char*)lv_keybuf)))
				return NODE_RESULT_ERROR;
			
			break;

		default:			// ��������
			
			return NODE_RESULT_ERROR;
			break;

		} // Switch

		// Sun added 2002-04-06
		if( GetCTIAvailble() )
		{
			if( NF_SetNextNode(lv_data2.nd_ok) == 0 )
				return NODE_RESULT_ERROR;
			return NODE_RESULT_OK;
		}
		
		m_dwQueueTime = GetTickCount();
		m_bInQueue = true;
		while( true && m_bInboundCall )
		{
			// Switch Music Play End
			/// Then Play Waiting Music
			if( m_oARS.ivr_channel_getstate(LINESTATUS_HOOKST) == VOICE_LS_HOOKST_ONHOOK )
				return NODE_RESULT_ERROR;

			if( m_oARS.ivr_channel_getstate() == VOICE_LS_DEVST_IDLE )
			{
				while( true && m_bInboundCall )
				{
					lv_retval = GetExitCode();
					if( lv_retval == VOICE_TM_USRSTOP || lv_retval >= MSI_TM_NOMSI )
						break;
					if( PlayVoiceEx(lv_data2.vox_wt, 'N') == -1 )
						return NODE_RESULT_ERROR;
					Sleep(20);
				}
			}

			// Sun added 2002-04-18
			/// Wait Stop Channel Finished
			Sleep(20);

			if( lv_retval == VOICE_TM_USRSTOP )
			{
				if( PlayVoiceEx(lv_data2.vox_busy) == -1 )
					return NODE_RESULT_ERROR;

				if( NF_SetNextNode(lv_data2.nd_busy) == 0 )
					return NODE_RESULT_ERROR;
			}

			lv_retval = TerminalAssert(TERM_TYPE_SWITCH, lv_data2.nd_parent);
			switch(lv_retval)
			{
			case 1:				// Nobody
				if( PlayVoiceEx(lv_data2.vox_nobody) == -1 )
					return NODE_RESULT_ERROR;
				if( NF_SetNextNode(lv_data2.nd_nobody) == 0 )
					return NODE_RESULT_ERROR;
				return NODE_RESULT_OK;

				break;
			
			case 2:				// Busy
				if( PlayVoiceEx(lv_data2.vox_busy) == -1 )
					return NODE_RESULT_ERROR;
				if( NF_SetNextNode(lv_data2.nd_busy) == 0 )
					return NODE_RESULT_ERROR;
				return NODE_RESULT_OK;

				break;

			case 3:				// Answer
				if( PlayVoiceEx(lv_data2.vox_sw) == -1 )
					return NODE_RESULT_ERROR;
				
				// continue while loop
				break;

			case 4:				// No Answer
				if( PlayVoiceEx(lv_data2.vox_busy) == -1 )
					return NODE_RESULT_ERROR;
				if( NF_SetNextNode(lv_data2.nd_busy) == 0 )
					return NODE_RESULT_ERROR;
				return NODE_RESULT_OK;

				break;

			case 5:				// OK
				if( PlayVoiceEx(lv_data2.vox_ok) == -1 )
					return NODE_RESULT_ERROR;

				if( NF_SetNextNode(lv_data2.nd_ok) == 0 )
					return NODE_RESULT_ERROR;
				return NODE_RESULT_OK;

				break;

			default:
				return NODE_RESULT_ERROR;
			} // End of Switch
		} // End of While
	
		break;

	default:
		return NODE_RESULT_ERROR;
	}
	
	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// ת����ϯ��
// ���أ�
US CIVRChannel::NF_061(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_061 lv_data1;
	SData2_061 lv_data2;
	UL lv_nDeviceNumber;
	char lv_sACDDN[PCS_DEF_NUMLEN_APP];
	UC lv_uservar[PCS_MAX_VAR_LEN];		// �û�����
	int lv_varlength;					// �û���������
	int lv_nPlay = 0;
	int lv_nTimes = 0;
	char filename[MAX_PATH] = "";		// ��Դ·��
	BOOL lv_bNeedWait;					// ��Ҫ�ȴ�
	int lv_nQueueWaitResult;

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step
		
		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));
		memcpy((UC *)&m_Data061_1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&m_Data061_2, f_data2, sizeof(lv_data2));

		/// ������־
		NF_SetAccessLog(lv_data1.log);

		// ���Ų�����ʾ����
		if( PlayVoiceEx(lv_data2.vox_op) == -1 )
			return NODE_RESULT_ERROR;

		// CTIת�����ж�CTI�Ƿ�����
		if( lv_data1.switchtype == 1 )
		{
			if( !GetCTIAvailble() )
			{
				if( PlayVoiceEx(lv_data2.vox_noanswer) == -1 )
					return NODE_RESULT_ERROR;
				if( NF_SetNextNode(lv_data2.nd_noanswer) == 0 )
					return NODE_RESULT_ERROR;
				return NODE_RESULT_OK;
			}
		}
				
		// ��ȡת��Ŀ���ӱ����л�ȡת��Ŀ��
		memset(lv_sACDDN, 0x00, sizeof(lv_sACDDN));
		lv_nDeviceNumber = 0;
		if( lv_data1.usevar > 0 )
		{
			memset(lv_uservar, 0x00, sizeof(lv_uservar));
			lv_varlength = NF_GetUserValue(lv_data1.usevar, lv_uservar);
			if( lv_varlength > 0 )
			{
				if( lv_data1.toacd > 0 )
					strncpy(lv_sACDDN, ( const char* )lv_uservar, sizeof(lv_sACDDN) - 1);
				else
					lv_nDeviceNumber = atol(( const char* )lv_uservar);
			}
		}
		else
		{
			if( lv_data1.toacd > 0 )
				strncpy(lv_sACDDN, ( const char* )lv_data2.acddn, sizeof(lv_sACDDN) - 1);
			else	// Route Point ID
				lv_nDeviceNumber = lv_data2.routepointid;
		}

		// Queue Init
		if( lv_data1.toacd > 0 )
			m_szTransferDest = lv_sACDDN;
		else
			m_szTransferDest.Format("%u", lv_nDeviceNumber);
		m_cCallType = '2';							// ��������ת�ӱ��
		m_dwWaitBegin = GetTickCount() - m_intCTIWaitTime * 1000;			// �ۼ��Ŷ�ʱ��
		m_dwQueueTime = GetTickCount();				// �����Ŷ�ʱ��
		m_bInQueue = true;
		m_nSwitch_WorkNo = 0;
		m_nSwitch_AgentID = 0;
		m_strSwitch_UserID = _T("");
		m_strSwitch_ExtNo = _T("");

		// Sun replaced 2011-10-02
		//m_chSwitch_Result = _pcC_OB_RESULT_ABANDON;
		SetSwitchResult(_pcC_OB_RESULT_ABANDON);

		SetExitCode(VOICE_TM_Dummy);

		/// ���õȴ��ж�Level1
		m_QueueWaitSyncEvent.m_nTag = lv_nDeviceNumber;
		memset(m_QueueWaitSyncEvent.m_pData, 0x00, sizeof(m_QueueWaitSyncEvent.m_pData));
		ResetQueueWaitEvent(1);

		// ע�⣺Ŀǰ��֧�ַ�CISCO�Ĳ��Ŵ���ת��
		if( lv_data1.switchtype == 0 )
		{
			// Sun ToDo: Not support by now, should enhance later
#if !defined(CISCO_CCM) && !defined(CISCO_CME)
			return NODE_RESULT_ERROR;
#endif
		}

		// �����Ŷ����󣬵ȴ���Ӧ
		lv_bNeedWait = TRUE;
		if( lv_data1.switchtype == 1 )
		{
			// CTIת�Ӳ�֧��ACD���루�޷��ж�ACD״̬��
			/// Sun updated 2008-05-16
			if( m_strTaskID.GetLength() > 0 )
			{
				// ��������
				if( !MQ_SendMsg_ContinueQueueTask(m_strTaskID, lv_nDeviceNumber, 0) )
					goto ERROR_EXIT;
			}
			else
			{
				// �½�����
				if( !MQ_SendMsg_AddQueueTask(lv_nDeviceNumber) )
					goto ERROR_EXIT;
			}

			// Sun replaced 2011-10-02
			// Sun added 2011-06-29
			//m_chSwitch_Result = _pcC_OB_RESULT_OP_TIMEOUT;
			SetSwitchResult(_pcC_OB_RESULT_OP_TIMEOUT);
			
			// Sun updated 2010-01-08 [A], Wait(2000) -> PCS_TO_UMS_ADDTASK
			if( m_QueueWaitSyncEvent.Wait(PCS_TO_UMS_ADDTASK) == true )
			{
				if( m_QueueWaitSyncEvent.m_nEventID >= 10 )
				{
					// �����ȴ�
					goto ERROR_EXIT;
				}

				// ��ȡ��Ӧ����
				S_TCP_PCS2UMS_ADDTASK_ACK lv_rcvbdy;
				memcpy(&lv_rcvbdy, m_QueueWaitSyncEvent.m_pData, sizeof(lv_rcvbdy));
				if( (lv_rcvbdy.ErrorCode == PCS_DEF_RETCODE_SUCC || lv_rcvbdy.ErrorCode == PCS_DEF_RETCODE_NOTNEED) &&
					lv_rcvbdy.strUMSTaskID[0] != NULL )
				{
					// Sun replaced 2011-10-02
					// Sun added 2011-06-29
					//m_chSwitch_Result =_pcC_OB_RESULT_ABANDON;
					SetSwitchResult(_pcC_OB_RESULT_ABANDON);

					m_strTaskID = lv_rcvbdy.strUMSTaskID;
					m_blnTaskClosed = FALSE;

					// Sun added 2011-06-25
					m_strRecordTaskID = m_strTaskID;

					/// ���õȴ��ж�Level2
					if( m_QueueWaitSyncEvent.m_nEventID < 2 )
					{
						memset(m_QueueWaitSyncEvent.m_pData, 0x00, sizeof(m_QueueWaitSyncEvent.m_pData));
						ResetQueueWaitEvent(2);
					}

					WriteTrace(TraceDebug, "Debug - IVR Channel: %d UMS Task Ack Data1=%d, Data2=%d, Data3=%d, Data4=%d", 
						m_intLineNO, lv_rcvbdy.nRouteData1, lv_rcvbdy.nRouteData2, lv_rcvbdy.nRouteData3, lv_rcvbdy.nRouteData4);

					// ��½��ϯ��
					if( lv_rcvbdy.nRouteData2 == 0 )
					{
						if( PlayVoiceEx(lv_data2.vox_nobody) == -1 )
							goto ERROR_EXIT;
						if( lv_data2.nd_nobody > 0 )
						{
							// ֹͣ�ȴ�
							SetQueueWaitEvent(10);

							if( NF_SetNextNode(lv_data2.nd_nobody) == 0 )
								goto ERROR_EXIT;
							goto OK_EXIT;							
						}
					}
					// ������ϯ��
					else if( lv_rcvbdy.nRouteData3 == 0 )
					{
						if( lv_data2.nd_busy > 0 )
						{
							if( PlayVoiceEx(lv_data2.vox_busy) == -1 )
								goto ERROR_EXIT;

							// ֹͣ�ȴ�
							SetQueueWaitEvent(10);

							if( NF_SetNextNode(lv_data2.nd_busy) == 0 )
								goto ERROR_EXIT;
							goto OK_EXIT;
						}

						// ����EWT
						if( lv_data1.readEWT > 0 )
						{
							// �������EWTʱ��
							short lv_nRealEWT;
							/// ǰ���ļ����ų���
							lv_nRealEWT = (short)(lv_rcvbdy.nRouteData4 - GetVoiceFilePlayLength(lv_data2.vox_busy));
							/// ʱ�䲥�ų���
							if( lv_nRealEWT > 600 )
								lv_nRealEWT -= 4;
							else if( lv_nRealEWT > 60 )
								lv_nRealEWT -= 3;
							else
								lv_nRealEWT -= 2;
							if( lv_nRealEWT > 5 )
							{
								if( PlayVoiceEx(lv_data2.vox_busy) == -1 )
									goto ERROR_EXIT;

								if(	m_oARS.ivr_channel_timeplay(lv_nRealEWT, lv_data1.readEWT, NF_GetLanguageID()) == -1 )
									goto ERROR_EXIT;
							}
							else
							{
								// �ȴ�ʱ��ܶ�
								lv_bNeedWait = FALSE;
								if( PlayVoiceEx(lv_data2.vox_sw) == -1 )
									goto ERROR_EXIT;
							}
						}
						else
						{
							if( PlayVoiceEx(lv_data2.vox_busy) == -1 )
								goto ERROR_EXIT;
						}
					}
					else
					{
						// ����Ҫ�ȴ�
						lv_bNeedWait = FALSE;
						if( PlayVoiceEx(lv_data2.vox_sw) == -1 )
							goto ERROR_EXIT;
					}
				}
				else
				{
					// Sun added 2011-06-25
					/// ������������ʧ�ܣ�m_strTaskID������ǿ�
					/// ����Ǽ�������ʧ�ܣ������ڣ��޷�����������m_strTaskID�ÿգ��´ξͲ�������������Ϣ�����Ƿ�����������Ϣ
					if( lv_rcvbdy.ErrorCode == PCS_DEF_RETCODE_NOTEXIST )
						m_strTaskID = _T("");

					// Sun replaced 2011-10-02
					// Sun added 2011-06-29
					//m_chSwitch_Result =_pcC_OB_RESULT_SYSTEM_ERROR;
					SetSwitchResult(_pcC_OB_RESULT_SYSTEM_ERROR);

					// �����Ŷ�ʧ��
					SetQueueWaitEvent(0);			// ����Ҫ�Ƴ��Ŷ�����
					if( PlayVoiceEx(lv_data2.vox_noanswer) == -1 )
						goto ERROR_EXIT;
					if( NF_SetNextNode(lv_data2.nd_noanswer) == 0 )
						goto ERROR_EXIT;
					goto OK_EXIT;
				}
			}
			else
			{
				// ���������Ӧ��ʱ
				SetQueueWaitEvent(0);			// ����Ҫ�Ƴ��Ŷ�����
				if( PlayVoiceEx(lv_data2.vox_noanswer) == -1 )
					goto ERROR_EXIT;
				if( NF_SetNextNode(lv_data2.nd_noanswer) == 0 )
					goto ERROR_EXIT;
				goto OK_EXIT;
			}
		}
		else
		{
			// ����ת�� - ֱ�ӽ���Level2�ж�
			SetQueueWaitEvent(2);
		}

		// Wait idle agent
		/// �ȴ���ʽ: 0 - ����������1 - �첽������
		if( lv_data1.waitmethod == 0 || !lv_bNeedWait )
		{
			lv_nTimes = 0;
			while( m_bInboundCall )
			{
				// ���Ŵ���
				lv_nTimes++;
				if( lv_data1.looptimes > 0 && lv_nTimes > lv_data1.looptimes )
				{
					// ֹͣ�ȴ�
					SetQueueWaitEvent(10);

					// Sun removed 2008-04-02
                    //if( PlayVoiceEx(lv_data2.vox_busy) == -1 )
					//	return NODE_RESULT_ERROR;

					// Sun fixed 2008-04-02
					if( NF_SetNextNode(lv_data2.nd_wait) == 0 )
						goto ERROR_EXIT;
					goto OK_EXIT;

					break;
				}

				// 5 �����ϲŷ���
				if( lv_data1.maxwait > 5 || lv_data1.maxwait == 0 )
				{
					/// Get Play File Name
					if( lv_data2.vox_wt > 0 && GetQueueWaitEventID() == 2 )
					{
						if( GetResourcePathWithID(lv_data2.vox_wt, filename) != NULL )
						{
							/// �������״̬
							SetStatus(IVR_W_PLAY);

							// Sun added 'if'  2008-04-02
							int termcode = m_oARS.ivr_channel_gettermmsk(CMESyncEvent_Play);
							if( lv_nTimes <= 1 || termcode == VOICE_TM_EOD )
							{
								m_oARS.ivr_channel_gettermmsk(CMESyncEvent_Play);
								lv_nPlay = m_oARS.ivr_channel_async_play(filename, NULL, true, true);
								if( lv_nPlay == -1 )
									goto ERROR_EXIT;
							}
						}
					}
				}

				// �����ȴ��ж�
				while( m_bInboundCall )
				{
					lv_nQueueWaitResult = NF_N61_QueueWaiting();

					//WriteTrace(TraceDebug, "IVR Channel: %d NF_N61_QueueWaiting() = %d", m_intLineNO, lv_nQueueWaitResult);

					if( lv_nQueueWaitResult < 0 )
						goto ERROR_EXIT;
					else if( lv_nQueueWaitResult == 1 )
						continue;
					else if( lv_nQueueWaitResult == 2 )
						break;
					else if( lv_nQueueWaitResult == 3 )
					{
						// Sun added 2011-08-08
						/// ����ת�ӣ�����ʱ
						SmartSleep(5000);

						goto ERROR_EXIT;
					}
					else
					{
						if( NF_SetNextNode((US)lv_nQueueWaitResult) == 0 )
							goto ERROR_EXIT;
						goto OK_EXIT;
					}
				}
			}

			if( !m_bInboundCall )
				goto ERROR_EXIT;
		}
		else
		{
			// �첽������
			if( NF_SetNextNode(lv_data2.nd_wait) == 0 )
				goto ERROR_EXIT;
			goto OK_EXIT;
		}

		break;

	default:
		return NODE_RESULT_ERROR;
	}
	
OK_EXIT:
	// Sun added 2008-04-02
	m_bInQueue = false;
	m_intCTIWaitTime += ((DWORD)(GetTickCount() - m_dwQueueTime) / 1000 );
	return NODE_RESULT_OK;

ERROR_EXIT:
	// Sun added 2008-04-02
	m_bInQueue = false;
	m_intCTIWaitTime += ((DWORD)(GetTickCount() - m_dwQueueTime) / 1000 );
	return NODE_RESULT_ERROR;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// ����绰����
// Last updated on 2012-04-18 [B]
// ���أ�
US CIVRChannel::NF_062(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_062 lv_data1;
	SData2_062 lv_data2;
	CString lv_sDialNo = _T("");
	UC lv_uservar[PCS_MAX_VAR_LEN];		// �û�����
	int lv_varlength;					// �û���������
	CString lv_sFormat, lv_sTemp;
	BOOL lv_blnOB = FALSE;				// �Ƿ��Ⲧ�ɹ�
	CIVRChannel *lv_pChannel = NULL;
	UC lv_btyOBResult;
	int lv_nTO, lv_nLoop;
	char filename[MAX_PATH] = "";		// ��Դ·��
	int lv_nPlay = 0;

	// ��ʱ
	SetTickTime();

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step

		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));

		/// ������־
		NF_SetAccessLog(lv_data1.log);

		// ���Ų�����ʾ����
		if( PlayVoiceEx(lv_data2.vox_op) == -1 )
			return NODE_RESULT_ERROR;

		// CTI�������ж�CTI�Ƿ�����������תʧ�ܽڵ㣨����ϵͳ������ʾ����
		if( lv_data1.switchtype == 1 )
		{
			if( !GetCTIAvailble() )
			{
				if( PlayVoiceEx(lv_data2.vox_syserror) == -1 )
					return NODE_RESULT_ERROR;
				if( NF_SetNextNode(lv_data2.nd_failed) == 0 )
					return NODE_RESULT_ERROR;
				return NODE_RESULT_OK;
			}
		}
				
		// ��ȡ����Ŀ���ӱ����л�ȡ����Ŀ��
		if( lv_data1.usevar > 0 )
		{
			memset(lv_uservar, 0x00, sizeof(lv_uservar));
			lv_varlength = NF_GetUserValue(lv_data1.usevar, lv_uservar);
			if( lv_varlength > 0 )
				lv_sDialNo = (LPCSTR)lv_uservar;
		}
		else
		{
			if( lv_data2.DialNo[0] == ':' )
			{
				if( lv_data2.DialNo[1] != ':' )
				{
					/// IVR Variable
					lv_sFormat = (const char *)(lv_data2.DialNo + 1);
					memset(lv_uservar, 0x00, sizeof(lv_uservar));
					NF_GetUserValue((const char *)lv_sFormat, lv_uservar);
					lv_sDialNo = (LPCSTR)lv_uservar;
				}
				else			// "::" -> ":"
					lv_sDialNo = (LPCSTR)(lv_data2.DialNo + 1);
			}
			else
				lv_sDialNo = (LPCSTR)lv_data2.DialNo;
		}

		// Queue Init
		m_szTransferDest = lv_sDialNo;
		m_cCallType = '2';							// ��������ת�ӱ��
		m_dwWaitBegin = GetTickCount();				// �ȴ��Ŷ�ʱ��

		// ��Ӳ���ǰ׺
		if( lv_data2.predial[0] != 0 )
		{
			lv_sFormat = (LPCSTR)lv_data2.predial;
			lv_sTemp.Format("%s%s", lv_sFormat, lv_sDialNo);
			lv_sDialNo = lv_sTemp;
		}

		// Sun replaced 2011-10-02
		//m_chSwitch_Result = _pcC_OB_RESULT_ABANDON;
		SetSwitchResult(_pcC_OB_RESULT_ABANDON);

		SetExitCode(VOICE_TM_Dummy);

		// ע�⣺Ŀǰ��֧�ַ�CISCO�Ĳ��Ŵ��̻��飬����ʹ��CTI��ʽ
		if( lv_data1.switchtype == 0 )
		{
			// Sun ToDo: Not support by now, should enhance later
#if !defined(CISCO_CCM) && !defined(CISCO_CME)
			WriteTrace(TraceError, "Error - IVR Channel: %d NF_062 - call flow setting is wrong! Not supported D card conference.", m_intLineNO);
			return NODE_RESULT_ERROR;
#endif
		}

		WriteTrace(TraceInfo, "Info - IVR Channel: %d NF_062 - will conference number %s.", m_intLineNO, lv_sDialNo);

		for( lv_nLoop = 0; lv_nLoop < lv_data1.looptimes; lv_nLoop++ )
		{
#if defined(CISCO_CCM) || defined(CISCO_CME)

			// ���鷽ʽ�Ⲧ������Ҫ�����ͨ����Դ
			lv_blnOB = FALSE;
			if( m_oARS.ConsultConference(lv_sDialNo) != -1 )
			{
				// �ȴ�����������
				ResetQueueWaitEvent(13);
				lv_nTO = 0;
				while( m_bInboundCall )
				{
					if( m_QueueWaitSyncEvent.Wait(1000) == true )
					{
						if( m_QueueWaitSyncEvent.m_nEventID <= 14 )
						{
							// ���̳ɹ�
							lv_pChannel = this;
							lv_blnOB = TRUE;

							lv_btyOBResult = _pcC_OB_RESULT_NO_ANS;
							ResetQueueWaitEvent(14);
						}
						else
						{
							lv_btyOBResult = _pcC_OB_RESULT_DEST_BUSY;
							WriteTrace(TraceInfo, "Info - IVR Channel: %d NF_062 - destination:%s is busy or error!", m_intLineNO, lv_sDialNo);
						}

						break;
					}
					else
					{
						lv_nTO++;
						if( lv_nTO > lv_data1.timeout )
						{
							lv_btyOBResult = _pcC_OB_RESULT_OP_TIMEOUT;
							WriteTrace(TraceWarn, "Warn - IVR Channel: %d NF_062 - wait ConsultConference finished timeout!", m_intLineNO);
							break;
						}
					}
				}
			}
			else
			{
				lv_btyOBResult = _pcC_OB_RESULT_SYSTEM_ERROR;
				WriteTrace(TraceWarn, "Warn - IVR Channel: %d NF_062 - wait ConsultConference(%s) failed!", m_intLineNO, lv_sDialNo);
			}

#else

			// CTI��Ϣ����
			ResetQueueWaitEvent(13);
			if( !MQ_SendMsg_CallConsult(_pcC_CONSMODE_CONFERENCE, (LPCSTR)lv_sDialNo) )
			{
				lv_btyOBResult = _pcC_OB_RESULT_SYSTEM_ERROR;
				WriteTrace(TraceWarn, "Warn - IVR Channel: %d NF_062 - wait MQ_SendMsg_CallConsult(%s) failed!", m_intLineNO, lv_sDialNo);
			}
			else
			{
				// �ȴ�����������
				lv_nTO = 0;
				while( m_bInboundCall )
				{
					if( m_QueueWaitSyncEvent.Wait(1000) == true )
					{
						if( m_QueueWaitSyncEvent.m_nEventID <= 14 )
						{
							// ���̳ɹ�
							lv_pChannel = this;
							lv_blnOB = TRUE;

							lv_btyOBResult = _pcC_OB_RESULT_NO_ANS;
							ResetQueueWaitEvent(14);
						}
						else
						{
							lv_btyOBResult = _pcC_OB_RESULT_DEST_BUSY;
							WriteTrace(TraceInfo, "Info - IVR Channel: %d NF_062 - destination:%s is busy or error!", m_intLineNO, lv_sDialNo);
						}

						break;
					}
					else
					{
						lv_nTO++;
						if( lv_nTO > lv_data1.timeout )
						{
							lv_btyOBResult = _pcC_OB_RESULT_OP_TIMEOUT;
							WriteTrace(TraceWarn, "Warn - IVR Channel: %d NF_062 - wait MQ_SendMsg_CallConsult finished timeout!", m_intLineNO);
							break;
						}
					}
				}
			}
#endif

			if( !lv_blnOB )
			{
				ResetQueueWaitEvent(0);

#if defined(CISCO_CCM) || defined(CISCO_CME)
				// ֹͣ���̣�ʰ�𱣳ֺ��У����û�г�������Դ���������
				if( lv_btyOBResult != _pcC_OB_RESULT_SYSTEM_ERROR )
					m_oARS.StopConsultaion();
#else
				if( lv_btyOBResult != _pcC_OB_RESULT_SYSTEM_ERROR )
				{
					MQ_SendMsg_RetrieveCall();
				}
#endif
				SmartSleep(theApp.m_nCTITransferDelay);
			}
			else
				break;	// Exit for Loop

		} // End of Loop

		SetSwitchResult(lv_btyOBResult);

		if( !lv_blnOB )
		{
			if( lv_btyOBResult == _pcC_OB_RESULT_SYSTEM_ERROR )
			{
				// ϵͳ������ʾ����
				if( PlayVoiceEx(lv_data2.vox_syserror) == -1 )
					return NODE_RESULT_ERROR;
			}
			else
			{
				// ����ʧ����ʾ����
				if( PlayVoiceEx(lv_data2.vox_noconf) == -1 )
					return NODE_RESULT_ERROR;
			}

			// תʧ�ܽڵ�
			if( NF_SetNextNode(lv_data2.nd_failed) == 0 )
				return NODE_RESULT_ERROR;
		}
		else
		{
			// ������������ʾ����
			if( PlayVoiceEx(lv_data2.vox_sw) == -1 )
				return NODE_RESULT_ERROR;

			// �ȴ�����
			/// �ȴ�����
			if( lv_data2.vox_wt > 0 )
			{
				if( GetResourcePathWithID(lv_data2.vox_wt, filename) != NULL )
				{
					/// �������״̬
					SetStatus(IVR_W_PLAY);

					// Sun added 'if'  2008-04-02
					m_oARS.ivr_channel_gettermmsk(CMESyncEvent_Play);
					if( m_oARS.ivr_channel_async_play(filename, NULL, true, true) == -1 )
						return NODE_RESULT_ERROR;
					lv_nPlay = 1;
				}
			}

#ifdef INTEL_DIALOGIC

			if( MQ_SendMsg_FinishConference() )
			{

#endif

#ifdef CISCO_CCM

			if( m_oARS.CompleteTransfer(0) == 0 )
			{

#endif

#ifdef CISCO_CME

			if( 1 )		// ToDoCME
			{

#endif

		// 4���ȴ���ͨ
				lv_varlength = 0;
				while( lv_varlength < lv_data1.waitansto * 10 )
				{
					SetTickTime();
					Sleep(100);
					lv_varlength++;

					if( m_QueueWaitSyncEvent.Wait(0) )
					{
						if( m_QueueWaitSyncEvent.m_nEventID == 14 )
						{
							lv_varlength = 0;
							lv_btyOBResult = _pcC_OB_RESULT_SUCC;

							// �ȴ�ͨ������
							ResetQueueWaitEvent(15);
						}
						else if( m_QueueWaitSyncEvent.m_nEventID == 17 )
						{
							lv_btyOBResult = _pcC_OB_RESULT_DEST_BUSY;
							WriteTrace(TraceWarn, "Warn - IVR Channel: %d NF_062 - dial to busy.", m_intLineNO);
						}
						else
						{
							lv_btyOBResult = _pcC_OB_RESULT_OTHERS;
							WriteTrace(TraceWarn, "Warn - IVR Channel: %d NF_062 - wait answer failed! StatusID=%d", m_intLineNO, m_QueueWaitSyncEvent.m_nEventID);
						}

						break;
					}

					/// ���йһ�
					if( !m_bInboundCall )
					{
						lv_btyOBResult = _pcC_OB_RESULT_ABANDON;
						WriteTrace(TraceWarn, "Warn - IVR Channel: %d NF_062 - call abandoned! StatusID=%d", m_intLineNO, m_QueueWaitSyncEvent.m_nEventID);
						break;
					}
				}
			}
			else
				lv_btyOBResult = _pcC_OB_RESULT_SYSTEM_ERROR;

			SetSwitchResult(lv_btyOBResult);

			/// Stop Play
			if( lv_nPlay == 1 )
				m_oARS.ivr_channel_stop_play();

			if( !m_bInboundCall || m_QueueWaitSyncEvent.m_nEventID == 19 )
			{
				return NODE_RESULT_ERROR;
			}

			if( lv_btyOBResult == _pcC_OB_RESULT_SUCC )
			{
				// �ɹ���ʾ����
				if( PlayVoiceEx(lv_data2.vox_ok) == -1 )
					return NODE_RESULT_ERROR;

				// ת�ɹ��ڵ�
				if( NF_SetNextNode(lv_data2.nd_ok) == 0 )
					return NODE_RESULT_ERROR;
			}
			else
			{
				if( m_QueueWaitSyncEvent.m_nEventID > 10 && m_QueueWaitSyncEvent.m_nEventID < 19 )
				{
					ResetQueueWaitEvent(0);

#ifdef INTEL_DIALOGIC
					// ȡ������
					MQ_SendMsg_RetrieveCall();
#endif

#ifdef CISCO_CCM
					// ֹͣ���̣�ʰ�𱣳ֺ���
					m_oARS.StopConsultaion();
#endif

#ifdef CISCO_CME
				// TODOCME
#endif
				}

				if( lv_btyOBResult == _pcC_OB_RESULT_SYSTEM_ERROR )
				{
					// ϵͳ������ʾ����
					if( PlayVoiceEx(lv_data2.vox_syserror) == -1 )
						return NODE_RESULT_ERROR;

					// תʧ�ܽڵ�
					if( NF_SetNextNode(lv_data2.nd_failed) == 0 )
						return NODE_RESULT_ERROR;
				}
				else
				{
					// ��Ӧ����ʾ����
					if( PlayVoiceEx(lv_data2.vox_noans) == -1 )
						return NODE_RESULT_ERROR;

					// ��Ӧ��ת�ڵ�ID
					if( NF_SetNextNode(lv_data2.nd_noans) == 0 )
						return NODE_RESULT_ERROR;
				}
			}
		}

		break;
		
	default:
		return NODE_RESULT_ERROR;
	}
	
	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// ��ǿת����ϯ��
// ���أ�
US CIVRChannel::NF_063(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_063 lv_data1;
	SData2_063 lv_data2;

	if(event != LE_PROCEND)
	{
		return 0;
	}

	// ��ʱ
	SetTickTime();

	// Get parameters
	memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
	memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));

	
	return 1;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Sun updated 2005-08-15
// ת����ֻ�
// ���أ�
US CIVRChannel::NF_069(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_069 lv_data1;
	SData2_069 lv_data2;
	UL lv_nDeviceNumber;
	UC lv_uservar[PCS_MAX_VAR_LEN];		// �û�����
	int lv_varlength;					// �û���������
	UC lv_bytMaxTrys;					// Sun added 2012-04-18 [C]

	// ��ʱ
	SetTickTime();

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step

		if( m_bytStatus == IVR_W_SWITCH )
			Sleep(50);

		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));

		/// ������־
		NF_SetAccessLog(lv_data1.log);
	
		// ���Ų�����ʾ����
		if(PlayVoiceEx(lv_data2.vox_op) == -1)
			return NODE_RESULT_ERROR;

		// Sun added 2005-08-15
		if( lv_data1.usevar == 0 )
			lv_nDeviceNumber = lv_data1.vagency;
		else
		{
			lv_varlength = NF_GetUserValue(lv_data1.usevar, lv_uservar);
			if( lv_varlength > 0 )
				lv_nDeviceNumber = atol(( const char* )lv_uservar);
			else
				lv_nDeviceNumber = 0;
		}

		// ����ת�ӷ�ʽ����ת�Ӳ���
		switch(lv_data1.switchtype)
		{
		case 0:				// ����
			// ת����ϯ(�������)
			if(!SwitchToStationEx(1, lv_nDeviceNumber, true))
				return NODE_RESULT_ERROR;

			if( NF_SetNextNode(lv_data2.nd_child) == 0 )
				return NODE_RESULT_ERROR;

			break;

		default:			// CTI
			// ת����ϯ(�������)
			if(!SwitchToStationEx(1, lv_nDeviceNumber))
				return NODE_RESULT_ERROR;

			//----------------------------------------
			// Sun updated 2012-04-18 [C]
			if( lv_data2.tryinterval > 0 )
				SmartSleep(lv_data2.tryinterval);
			//----------------------------------------
			else if( theApp.m_nCTITransferDelay > 0 )
				SmartSleep(theApp.m_nCTITransferDelay);

			// 0 do nothing but wait cti to clear call
			if( lv_data2.nd_child > 0 || theApp.m_nCTIOnHookDelay == 0 )
			{
				if( NF_SetNextNode(lv_data2.nd_child) == 0 )
					return NODE_RESULT_ERROR;
			}
			else
			{
				// Sun added 2012-04-18 [C]
				lv_bytMaxTrys = lv_data2.maxtry;
				if( lv_bytMaxTrys == 0 )
					lv_bytMaxTrys = m_bytMaxTrys;

				// Only Try 3 times
				if( m_nCTITransferAttempt++ > lv_bytMaxTrys )
				{
					m_nCTITransferAttempt = 0;
					WriteTrace(TraceWarn, "IVR Channel: %d failed to transfer to %d", m_intLineNO, lv_data1.vagency);
					return NODE_RESULT_ERROR;
				}
			}
		}

		break;
		
	default:
		return NODE_RESULT_ERROR;
	}
	
	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Sun added 2005-07-10, and updated 2005-08-15
// ��ѯ·�ɵ�
// ���أ�
US CIVRChannel::NF_070(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_070 lv_data1;
	SData2_070 lv_data2;
	US lv_nNextNode = 0, lv_CH;
	UL lv_nDataValue;
	DWORD lv_tickA;
	BOOL lv_result;
	S_TCP_PCS2IVR_GET_RPTSTATUS_ACK lv_rcvbdy;
	char	filename[MAX_PATH] = "";
	char	lv_datastring[200];
	_bstr_t lv_inputstr="";				// COM���ò���
	_bstr_t lv_methodname;
	BSTR lv_com_uservars;
	char lv_ChBuff[sizeof(ChData)];
	VARIANT lv_R;						// COM���÷���ֵ
	HRESULT hr;							// ���Է���ֵ
	UC lv_nType;
	US lv_nRoutePointID;
	UC lv_uservar[PCS_MAX_VAR_LEN];		// �û�����
	int lv_varlength;					// �û���������

	// ��ʱ
	SetTickTime();

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step

		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));

		/// ������־
		NF_SetAccessLog(lv_data1.log);

		//---------------------------------------------
		// Sun added 2005-08-15
		switch( lv_data1.querytype )
		{
		case 1:					// ����
			lv_nType = _pcC_CDOBJ_QUEUE;
			break;
		
		case 2:					// ��
			lv_nType = _pcC_CDOBJ_GROUP;
			break;

		case 3:					// С��
			lv_nType = _pcC_CDOBJ_TEAM;
			break;

		default:				// ·�ɵ�
			lv_nType = _pcC_CDOBJ_ROUTEPOINT;
			break;
		}

		if( lv_data1.usevar == 0 )
			lv_nRoutePointID = lv_data2.routepointid;
		else
		{
			lv_varlength = NF_GetUserValue(lv_data1.usevar, lv_uservar);
			if( lv_varlength > 0 )
				lv_nRoutePointID = atoi(( const char* )lv_uservar);
			else
				lv_nRoutePointID = 0;
		}

		/// ���Ͳ�ѯ��Ϣ
		lv_tickA = GetTickCount();
		m_QueryEvent.Reset();
		MQ_SendMsg_QueryRoutePoint(lv_nType, lv_nRoutePointID);
		while( true )
		{
			if( m_QueryEvent.Wait(PCS_TO_UMS_QUERYRPT) == true )
			{
				//// Receive Data
				memcpy(&lv_rcvbdy, m_QueryEvent.m_pData, sizeof(lv_rcvbdy));

				if( lv_rcvbdy.ErrorCode != PCS_DEF_RETCODE_SUCC 
					|| lv_rcvbdy.nGroupID != lv_nRoutePointID 
					|| lv_rcvbdy.nType != lv_nType )
				{
					WriteTrace(TraceInfo, "IVR Channel: %d receives error code: %d in node NF_070. Get RPTID = %d [type = %d], Query RPTID = %d [type = %d]", 
						m_intLineNO, lv_rcvbdy.ErrorCode, lv_rcvbdy.nGroupID, lv_rcvbdy.nType, lv_nRoutePointID, lv_nType);
					lv_nNextNode = lv_data2.nd_fail;
					break;
				}

				//// Get COM Name with ID
				if( lv_data2.com_iid > 0 )
				{
					if( m_oUc.IsCreateObject() )
					{
						memset(filename, 0x00, sizeof(filename));
						if(GetResourcePathWithID(lv_data2.com_iid, filename) != NULL)
						{
							///// ���� COM
							lv_methodname = filename;
							memcpy(lv_ChBuff, &ChData, sizeof(ChData));
							lv_com_uservars=SysAllocStringByteLen(lv_ChBuff, sizeof(ChData));
							try
							{
								VariantInit(&lv_R);
								lv_CH = GetLineNO();
								memset(lv_datastring, 0x00, sizeof(lv_datastring));
								sprintf( lv_datastring, "%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;", 
									lv_rcvbdy.nType, lv_rcvbdy.nGroupID,
									lv_rcvbdy.nCallsWait, lv_rcvbdy.nCallsAveWaitTime,
									lv_rcvbdy.nCallsMaxWaitTime, lv_rcvbdy.nCallsTotal,
									lv_rcvbdy.nCallsTransfer, lv_rcvbdy.nCallsSucc,
									lv_rcvbdy.nCallsSuccInTime, lv_rcvbdy.nAgentSum,
									lv_rcvbdy.nAgentIdle, lv_rcvbdy.nAgentBusy, 
									lv_rcvbdy.nAgentNotReady, lv_rcvbdy.nAgentAfterCall, 
									lv_rcvbdy.nAgentLogout, lv_rcvbdy.nEstimatedWaitTime );
								lv_inputstr = ( const char* )lv_datastring;
								hr = m_oUc.Invoke(DISPATCH_METHOD, &lv_R, NULL, NULL, 
									lv_methodname, TEXT("is&s"), 
									lv_CH,
									(BSTR)lv_inputstr,
									(BSTR FAR *)&lv_com_uservars);
								if(FAILED(hr))
								{
									SysFreeString(lv_com_uservars);
									return NODE_RESULT_ERROR;
								}
							}
							catch(_com_error e)
							{
								WriteTrace(TraceError, "Error - Error Msg:%s, Invoke(%s) failed!", 
									e.ErrorMessage(), lv_methodname);

								SysFreeString(lv_com_uservars);
								return NODE_RESULT_ERROR;
							}

							memcpy(&ChData, (const char *)lv_com_uservars, sizeof(ChData));
							SysFreeString(lv_com_uservars);
						}
					}
				}

				//// ������ֵ
				switch( lv_data1.paramindex )
				{
				case 0:					// �Ŷӵȴ�����
					lv_nDataValue = lv_rcvbdy.nCallsWait;
					break;

				case 1:					// ƽ���ȴ�ʱ��
					lv_nDataValue = lv_rcvbdy.nCallsAveWaitTime;
					break;

				case 2:					// ��ȴ�ʱ��
					lv_nDataValue = lv_rcvbdy.nCallsMaxWaitTime;
					break;

				case 3:					// ����������ϯ��
					lv_nDataValue = lv_rcvbdy.nAgentIdle;
					break;

				case 4:					// ͨ����ϯ��
					lv_nDataValue = lv_rcvbdy.nAgentBusy;
					break;

				case 5:					// δ������ϯ��
					lv_nDataValue = lv_rcvbdy.nAgentNotReady;
					break;

				case 6:					// ������ϯ��
					lv_nDataValue = lv_rcvbdy.nAgentAfterCall;
					break;

				case 7:					// δ������������ϯ��
					lv_nDataValue = lv_rcvbdy.nAgentNotReady + lv_rcvbdy.nAgentAfterCall;
					break;

				// Sun added 2007-04-15
				case 8:					// ����Ԥ��ȴ�ʱ��(EWT)
					lv_nDataValue = lv_rcvbdy.nEstimatedWaitTime;
					break;

				//----------------------------------------------------
				// Sun added 2008-05-16
				case 9:					// ��ϯ����
					lv_nDataValue = lv_rcvbdy.nAgentSum;
					break;

				case 10:				// ��¼��ϯ��
					lv_nDataValue = lv_rcvbdy.nAgentSum - lv_rcvbdy.nAgentLogout;
					break;

				case 11:				// δ��¼��ϯ��
					lv_nDataValue = lv_rcvbdy.nAgentLogout;
					break;

				case 12:				// ��ϯ�����ʣ�n%��
					lv_nDataValue = (UL)(lv_rcvbdy.nAgentSum - lv_rcvbdy.nAgentLogout) * 1000;
					if( lv_rcvbdy.nAgentSum > 0 )
						lv_nDataValue = (UL)((lv_nDataValue / lv_rcvbdy.nAgentSum + 5 ) / 10);
					else
						lv_nDataValue = 0;
					break;

				case 13:				// ��ϯ�����ʣ�n%��
					lv_nDataValue = (UL)lv_rcvbdy.nAgentIdle * 1000;
					if( lv_rcvbdy.nAgentSum > lv_rcvbdy.nAgentLogout )
						lv_nDataValue = (UL)((lv_nDataValue / (lv_rcvbdy.nAgentSum - lv_rcvbdy.nAgentLogout) + 5 ) / 10);
					else
						lv_nDataValue = 0;
					break;

				case 14:				// ������������
					lv_nDataValue = lv_rcvbdy.nCallsTotal;
					break;

				case 15:				// ����ת����
					lv_nDataValue = lv_rcvbdy.nCallsTransfer;
					break;

				case 16:				// ����ת�ӳɹ���
					lv_nDataValue = lv_rcvbdy.nCallsSucc;
					break;

				case 17:				// ���м�ʱת����
					lv_nDataValue = lv_rcvbdy.nCallsSuccInTime;
					break;

				case 18:				// �����ʣ�n%��
					lv_nDataValue = (lv_rcvbdy.nCallsTransfer - lv_rcvbdy.nCallsSucc) * 1000;
					if( lv_rcvbdy.nCallsTransfer > 0 )
						lv_nDataValue = (UL)((lv_nDataValue / lv_rcvbdy.nCallsTransfer + 5 ) / 10);
					else
						lv_nDataValue = 0;
					break;

				case 19:				// ����ˮƽ��n%��
					lv_nDataValue = lv_rcvbdy.nCallsSuccInTime * 1000;
					if( lv_rcvbdy.nCallsTransfer > 0 )
						lv_nDataValue = (UL)((lv_nDataValue / lv_rcvbdy.nCallsTransfer + 5 ) / 10);
					else
						lv_nDataValue = 100;
					break;
				//----------------------------------------------------

				default:
					lv_nDataValue = 0;
				}
				
				// Sun added 2007-04-15
				//// ��¼��ѯ���
				memset(filename, 0x00, sizeof(filename));
				sprintf(filename, "%u", lv_nDataValue);
				NF_SetUserValue(lv_data1.var_result, (UC *)filename); 

				//// �Ƚ�
				lv_result = FALSE;
				switch( lv_data1.logic )
				{
				case DEF_NODE016_LOGIC_BIGGER:
					if( lv_nDataValue > lv_data2.comparedvalue )
						lv_result = TRUE;
					break;
				case DEF_NODE016_LOGIC_SMALLER:
					if( lv_nDataValue < lv_data2.comparedvalue )
						lv_result = TRUE;
					break;
				case DEF_NODE016_LOGIC_EB:
					if( lv_nDataValue >= lv_data2.comparedvalue )
						lv_result = TRUE;
					break;
				case DEF_NODE016_LOGIC_ES:
					if( lv_nDataValue <= lv_data2.comparedvalue )
						lv_result = TRUE;
					break;
				case DEF_NODE016_LOGIC_NE:
					if( lv_nDataValue != lv_data2.comparedvalue )
						lv_result = TRUE;
					break;
				default:
					if( lv_nDataValue == lv_data2.comparedvalue )
						lv_result = TRUE;
				}
				WriteTrace(TraceDebug, "IVR Channel: %d NF_070: compare (%d, %d)", m_intLineNO, lv_nDataValue, lv_data2.comparedvalue);

				//// ת�ƽڵ�
				if( lv_result )
					lv_nNextNode = lv_data2.nd_yes;
				else
					lv_nNextNode = lv_data2.nd_no;
				break;
			}

			/// ��ֹͨ����ʱ
			SetTickTime();

			/// �Ƿ�ʱ
			if( (GetTickCount() - lv_tickA) / 1000 > lv_data1.timeout )
			{
				lv_nNextNode = lv_data2.nd_fail;
				break;
			}
		}

		// ת�ƽڵ�
		if( NF_SetNextNode(lv_nNextNode) == 0 )
			return NODE_RESULT_ERROR;
		
		break;

	default:
		return NODE_RESULT_ERROR;
	}
		
	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Sun added 2005-08-15
// ��ѯ��ϯ״̬
// ���أ�
US CIVRChannel::NF_071(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_071 lv_data1;
	SData2_071 lv_data2;
	US lv_nNextNode = 0;
	BOOL lv_blnFirst, lv_blnSecond, lv_result;
	DWORD lv_tickA;
	S_TCP_PCS2IVR_GET_AGENTSTATUS_ACK lv_rcvbdy;
	UL lv_nAgentID = 0;
	CString lv_strUserID = _T("");
	UC lv_uservar[PCS_MAX_VAR_LEN];		// �û�����
	int lv_varlength;					// �û���������

	// ��ʱ
	SetTickTime();

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step

		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));

		/// ������־
		NF_SetAccessLog(lv_data1.log);

		if( lv_data1.querytype == 0 || lv_data1.querytype == 2 )
		{
			// Agent ID or Work No
			if( lv_data1.usevar == 0 )
				lv_nAgentID = lv_data2.agentid;
			else
			{
				lv_varlength = NF_GetUserValue(lv_data1.usevar, lv_uservar);
				if( lv_varlength > 0 )
					lv_nAgentID = atoi(( const char* )lv_uservar);
			}

			if( lv_nAgentID == 0 )
			{
				WriteTrace(TraceInfo, "IVR Channel: %d invalid paramter for agentid in NF_071().", m_intLineNO);

				// ת�ƽڵ�
				if( NF_SetNextNode(lv_data2.nd_fail) == 0 )
					return NODE_RESULT_ERROR;

				return NODE_RESULT_OK;
			}
		}
		else
		{
			// User ID
			if( lv_data1.usevar != 0 )
			{
				lv_varlength = NF_GetUserValue(lv_data1.usevar, lv_uservar);
				if( lv_varlength > 0 )
					lv_strUserID = ( const char* )lv_uservar;
			}

			if( lv_strUserID == _T("") )
			{
				WriteTrace(TraceInfo, "IVR Channel: %d invalid paramter for userid in NF_071().", m_intLineNO);

				// ת�ƽڵ�
				if( NF_SetNextNode(lv_data2.nd_fail) == 0 )
					return NODE_RESULT_ERROR;

				return NODE_RESULT_OK;
			}
		}
		
		/// ���Ͳ�ѯ��Ϣ
		lv_tickA = GetTickCount();
		m_QueryEvent.Reset();
		MQ_SendMsg_QueryAgentStatus(lv_data1.querytype, lv_nAgentID, (LPCSTR)lv_strUserID);
		while( true )
		{
			if( m_QueryEvent.Wait(PCS_TO_UMS_QUERYAGTST) == true )
			{
				//// Receive Data
				memcpy(&lv_rcvbdy, m_QueryEvent.m_pData, sizeof(lv_rcvbdy));

				if( lv_rcvbdy.ErrorCode != PCS_DEF_RETCODE_SUCC 
					|| lv_rcvbdy.UserQuery != lv_data1.querytype )
				{
					WriteTrace(TraceInfo, "IVR Channel: %d receives error code: %d in node NF_071. Get AgentID = %d, UserID = %s, Query AgentID = %d, UserID = %s", 
						m_intLineNO, lv_rcvbdy.ErrorCode, lv_rcvbdy.AgentID, lv_rcvbdy.strAliasName, lv_nAgentID, lv_strUserID);
					lv_nNextNode = lv_data2.nd_fail;
					break;
				}

				//// First Condition (DN)
				lv_blnFirst = FALSE;
				switch( lv_data1.dn_logic )
				{
				case DEF_NODE016_LOGIC_BIGGER:
					if( lv_rcvbdy.DNStatus > lv_data1.dn_status )
						lv_blnFirst = TRUE;
					break;
				case DEF_NODE016_LOGIC_SMALLER:
					if( lv_rcvbdy.DNStatus < lv_data1.dn_status )
						lv_blnFirst = TRUE;
					break;
				case DEF_NODE016_LOGIC_EB:
					if( lv_rcvbdy.DNStatus >= lv_data1.dn_status )
						lv_blnFirst = TRUE;
					break;
				case DEF_NODE016_LOGIC_ES:
					if( lv_rcvbdy.DNStatus <= lv_data1.dn_status )
						lv_blnFirst = TRUE;
					break;
				case DEF_NODE016_LOGIC_NE:
					if( lv_rcvbdy.DNStatus != lv_data1.dn_status )
						lv_blnFirst = TRUE;
					break;
				default:
					if( lv_rcvbdy.DNStatus == lv_data1.dn_status )
						lv_blnFirst = TRUE;
				}

				//// Second Condition (POS)
				lv_blnSecond = FALSE;
				switch( lv_data1.pos_logic )		// Sun debug 2006-07-25
				{
				case DEF_NODE016_LOGIC_BIGGER:
					if( lv_rcvbdy.PositionStatus > lv_data1.pos_status )
						lv_blnSecond = TRUE;
					break;
				case DEF_NODE016_LOGIC_SMALLER:
					if( lv_rcvbdy.PositionStatus < lv_data1.pos_status )
						lv_blnSecond = TRUE;
					break;
				case DEF_NODE016_LOGIC_EB:
					if( lv_rcvbdy.PositionStatus >= lv_data1.pos_status )
						lv_blnSecond = TRUE;
					break;
				case DEF_NODE016_LOGIC_ES:
					if( lv_rcvbdy.PositionStatus <= lv_data1.pos_status )
						lv_blnSecond = TRUE;
					break;
				case DEF_NODE016_LOGIC_NE:
					if( lv_rcvbdy.PositionStatus != lv_data1.pos_status )
						lv_blnSecond = TRUE;
					break;
				default:
					if( lv_rcvbdy.PositionStatus == lv_data1.pos_status )
						lv_blnSecond = TRUE;
				}

				//// Ŀ��״̬
				switch( lv_data1.conditions )
				{
				case DEF_NODE071_CONDITION_FIRST:
					lv_result = lv_blnFirst;
					break;

				case DEF_NODE071_CONDITION_SECOND:
					lv_result = lv_blnSecond;
					break;

				case DEF_NODE071_CONDITION_BOTH:
					lv_result = (lv_blnFirst && lv_blnSecond);
					break;

				case DEF_NODE071_CONDITION_EITHER:
					lv_result = (lv_blnFirst || lv_blnSecond);
					break;

				default:
					lv_result = TRUE;
					break;
				}
				
				//// ת�ƽڵ�
				if( lv_result )
					lv_nNextNode = lv_data2.nd_yes;
				else
					lv_nNextNode = lv_data2.nd_no;
				break;
			}

			/// ��ֹͨ����ʱ
			SetTickTime();

			/// �Ƿ�ʱ
			if( (GetTickCount() - lv_tickA) / 1000 > lv_data1.timeout )
			{
				lv_nNextNode = lv_data2.nd_fail;
				break;
			}
		}

		// ת�ƽڵ�
		if( NF_SetNextNode(lv_nNextNode) == 0 )
			return NODE_RESULT_ERROR;
		
		break;

	default:
		return NODE_RESULT_ERROR;
	}
		
	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// GOLDSUN need enhanced
// ����ACD�Ŷ�
// ���أ�
US CIVRChannel::NF_080(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_080 lv_data1;
	SData2_080 lv_data2;
	int lv_retval;						// �ڵ�ת�Ʒ���ֵ

	// ��ʱ
	SetTickTime();
	m_dwQueueTime = GetTickCount();
	m_bInQueue = true;

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step
		
		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));

		/// ������־
		NF_SetAccessLog(lv_data1.log);
	
		// ���������Ŷӵȴ�����
		do
		{
			if((lv_retval = PlayVoiceEx(lv_data2.vox_wait, 'N')) == -1)
				return NODE_RESULT_ERROR;
		}
		while(lv_retval && m_bInboundCall );
		break;

		// �ж������ж�
		if(TerminalAssert(TERM_TYPE_SWITCH) == 5)
		{
			// ͨ�� - �ͷ�IVR
			//ResetChData();
			if( NF_SetNextNode(lv_data2.nd_child) == 0 )
				return NODE_RESULT_ERROR;
			return NODE_RESULT_OK;
		}
		break;

	default:
		return NODE_RESULT_ERROR;
	}

	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// �������ߺ���
// ���أ�
US CIVRChannel::NF_090(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_090 lv_data1;
	SData2_090 lv_data2;
	US lv_nNextNode;
	UC lv_uservar[PCS_MAX_VAR_LEN];		// �û�����
	int lv_varlength;					// �û���������
	char lv_sTemp[50];
	char lv_sSetANI[50];				// Sun added 2012-06-26
	char filename[MAX_PATH] = "";			// ��Դ·��
	UC lv_loop, lv_subloop, lv_dialresult;
	CString lv_sPredial, lv_sPhoneNo, lv_sExtNo;
	TCHAR lv_char;
	char	lv_ChBuff[sizeof(ChData)];
	_bstr_t lv_methodname;
	BSTR lv_com_uservars;
	BSTR lv_outputstr;					// COM���÷��ؽ��String
	VARIANT lv_R;						// COM���÷���ֵ
	HRESULT hr;							// ���Է���ֵ
	int lv_nResult;
	DWORD lv_dwOBStartTick;
	BOOL lv_blnAsyncDial, lv_blnAsyncDTMF;					// Sun added 2010-12-09
	BOOL lv_blnExtAsyncDial, lv_blnExtAsyncDTMF;			// Sun added 2010-12-09

	// Sun added 2010-12-09
	lv_blnAsyncDial = FALSE;
	lv_blnAsyncDTMF = FALSE;
	lv_blnExtAsyncDial = TRUE;
	lv_blnExtAsyncDTMF = TRUE;

	// Sun added 2010-12-01 [B]
	int lv_nPreFlag, lv_nPauseLoop, lv_nPauseSubLoop;
	CString lv_strMidString;
	CStringArray strArrMain;
	CStringArray strArrExt;

	// ��ʱ
	SetTickTime();

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step
		
		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));

		// ������־
		NF_SetAccessLog(lv_data1.log);

		m_bOutboundCall = TRUE;

		// Sun added 2006-09-15
		m_tmOBStartTime = CTime::GetCurrentTime();
		lv_dwOBStartTick = GetTickCount();

		// ȡ���Ⲧ���룬�����˷Ƿ��ַ���������ֻ���
		lv_sPredial = (char *)lv_data2.predial;
		/// 1. ȡ���Ⲧ����
		switch( lv_data1.numbertype )
		{
		case 1:				// ����
			lv_varlength = NF_GetUserValue(lv_data1.usevar, lv_uservar);
			if( lv_varlength > 0 )
				lv_sPhoneNo = (char *)lv_uservar;
			else
				lv_sPhoneNo = _T("");
			break;

		case 2:				// �Ⲧ��Ϣ
			lv_sPhoneNo = m_strOBMsgTelNo;
			break;

		case 3:				// COM
			lv_sPhoneNo = _T("");
			if( lv_data2.com_iid > 0 )
			{
				if( m_oUc.IsCreateObject() )
				{
					// ȡ����Դ·��
					if(GetResourcePathWithID(lv_data2.com_iid, filename) != NULL)
					{
						// ��ʼ��COM���ò���
						lv_outputstr=SysAllocString((OLECHAR *)"000000000000000000000000000000000000000000000000000000000000");

						// ȡ��COM�ӿ�ID
						lv_methodname = filename;
						memcpy(lv_ChBuff, &ChData, sizeof(ChData));
						lv_com_uservars=SysAllocStringByteLen(lv_ChBuff, sizeof(ChData));

						try
						{
							VariantInit(&lv_R);
							hr = m_oUc.Invoke(DISPATCH_METHOD, &lv_R, NULL, NULL,
								(BSTR)lv_methodname, TEXT("&s&s"),
								(BSTR FAR *)&lv_com_uservars,
								(BSTR FAR *)&lv_outputstr);
							SysFreeString(lv_com_uservars);

							if( SUCCEEDED(hr) )
								lv_sPhoneNo = lv_outputstr;
						}
						catch(_com_error e)
						{
							WriteTrace(TraceError, "Error - Error Msg:%s, Invoke(%s) failed!", 
								e.ErrorMessage(), lv_methodname);

							SysFreeString(lv_com_uservars);
						}

						// Sun added 2004-12-30
						SysFreeString(lv_outputstr);
					}
				}
			}
			break;

		default:			// �̶��趨
			lv_sPhoneNo = (char *)lv_data2.phoneno;
			break;
		}
		/// 2. ���˷Ƿ��ַ���������ֻ���
		lv_nPreFlag = 0;			// Sun added 2010-12-01 [B]
		lv_strMidString = _T("");	// Sun added 2010-12-01 [B]
		strArrMain.RemoveAll();		// Sun added 2010-12-01 [B]
		strArrExt.RemoveAll();		// Sun added 2010-12-01 [B]
		lv_sExtNo = _T("");
		lv_subloop = 0;
		memset(lv_sTemp, 0x00, sizeof(lv_sTemp));
		lv_varlength = lv_sPhoneNo.GetLength();

		/// Sun added 2010-12-09
		/// ��CTI��ͨ�б�
		if( lv_varlength > 0 )
		{
			if( lv_data1.connecttype != 1 )
			{
				lv_char = lv_sPhoneNo.GetAt(0);
				if( lv_char == 'T' || lv_char == 't' )
				{
					lv_blnAsyncDial = FALSE;
					lv_blnAsyncDTMF = FALSE;
				}
				else if( lv_char == 'H' || lv_char == 'h' )
				{
					lv_blnAsyncDial = FALSE;
					lv_blnAsyncDTMF = TRUE;
				}
				else if( lv_char == 'Y' || lv_char == 'y' )
				{
					lv_blnAsyncDial = TRUE;
					lv_blnAsyncDTMF = TRUE;
				}
			}
			else
			{
				lv_blnAsyncDial = TRUE;
				lv_blnAsyncDTMF = TRUE;
			}
		}

		for( lv_loop = 0; lv_loop < lv_varlength; lv_loop++ )
		{
			lv_char = lv_sPhoneNo.GetAt(lv_loop);
			if( lv_data1.dialtype == 0 )
			{
				if( lv_char == '-' )
				{
					lv_sExtNo = lv_sPhoneNo.Right(lv_varlength - lv_loop -1);
					lv_sPhoneNo = lv_sTemp;
					lv_subloop = 0;
					break;
				}
				// Sun replaced 2010-12-01 [B]
				/// From
				//else if( (lv_char >= '0' && lv_char <= '9') || lv_char == ',' || lv_char == ';' )
				/// To
				else if( strchr(DEF_IVR_DTMF_CHARSET, lv_char) != NULL )
				{ // �Ϸ�DTMF
					if( lv_nPreFlag <= 1 )
					{	// ����������
						lv_sTemp[lv_subloop++] = lv_char;
						lv_nPreFlag = 1;
					}
					else
					{
						if( lv_nPreFlag == 3 )
						{
							strArrMain.Add(lv_strMidString);
							WriteTrace(TraceDetail, "IVR Channel: %d dial MainNumber add pause section: %d - '%s' ", 
								m_intLineNO, strArrMain.GetSize(), lv_strMidString);
							lv_strMidString = _T("");
						}

						// ������ʱ�����
						lv_strMidString = lv_strMidString + lv_char;
						lv_nPreFlag = 2;
					}
				}
				else if( lv_char == DEF_IVR_DIAL_LONGPAUSE || lv_char == DEF_IVR_DIAL_MIDPAUSE || lv_char == DEF_IVR_DIAL_SHORTPAUSE )
				{ // ��ʱ�ָ���
					if( lv_nPreFlag == 2 )	// ǰһ����DTMF
					{
						strArrMain.Add(lv_strMidString);
						WriteTrace(TraceDetail, "IVR Channel: %d dial MainNumber add pause section: %d - '%s' ", 
							m_intLineNO, strArrMain.GetSize(), lv_strMidString);
						lv_strMidString = _T("");
					}
					lv_strMidString = lv_strMidString + lv_char;
					lv_nPreFlag = 3;
				}
			}
			else
			{
				if( lv_char == '-' )
				{
					lv_sExtNo = lv_sPhoneNo.Right(lv_varlength - lv_loop -1);
					lv_sPhoneNo = lv_sTemp;
					lv_subloop = 0;
					break;
				}
				// Sun replaced 2010-12-01 [A]
				/// From
				//else if( lv_char >= '0' && lv_char <= '9' )
				/// To
				if( strchr(DEF_IVR_DTMF_CHARSET, lv_char) != NULL )
					lv_sTemp[lv_subloop++] = lv_char;
			}
		}

		// Sun added 2010-12-01 [B]
		if( lv_strMidString.GetLength() > 0 )
		{
			strArrMain.Add(lv_strMidString);
			WriteTrace(TraceDetail, "IVR Channel: %d dial MainNumber add last pause section: %d - '%s' ", 
				m_intLineNO, strArrMain.GetSize(), lv_strMidString);
			lv_strMidString = _T("");
		}

		if( lv_subloop > 0 )
		{	// �޷ֻ�
			lv_sPhoneNo = lv_sTemp;
		}
		else
		{
			/// �������˷ֻ�
			lv_nPreFlag = 0;			// Sun added 2010-12-01 [B]
			lv_varlength = lv_sExtNo.GetLength();
			if( lv_varlength > 0 )
			{
				// Sun added 2010-12-09
				if( lv_data1.connecttype != 1 )
				{
					lv_char = lv_sExtNo.GetAt(0);
					if( lv_char == 'T' || lv_char == 't' )
					{
						lv_blnExtAsyncDial = FALSE;
						lv_blnExtAsyncDTMF = FALSE;
					}
					else if( lv_char == 'H' || lv_char == 'h' )
					{
						lv_blnExtAsyncDial = FALSE;
						lv_blnExtAsyncDTMF = TRUE;
					}
					else if( lv_char == 'Y' || lv_char == 'y' )
					{
						lv_blnExtAsyncDial = TRUE;
						lv_blnExtAsyncDTMF = TRUE;
					}
				}
				else
				{
					lv_blnExtAsyncDial = TRUE;
					lv_blnExtAsyncDTMF = TRUE;
				}

				lv_subloop = 0;
				memset(lv_sTemp, 0x00, sizeof(lv_sTemp));
				for( lv_loop = 0; lv_loop < lv_varlength; lv_loop++ )
				{
					lv_char = lv_sExtNo.GetAt(lv_loop);

					// Sun replaced 2010-12-01 [B]
					/// From
					//if( lv_char >= '0' && lv_char <= '9' )
					//	lv_sTemp[lv_subloop++] = lv_char;
					/// To
					if( strchr(DEF_IVR_DTMF_CHARSET, lv_char) != NULL )
					{ // �Ϸ�DTMF
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
									m_intLineNO, strArrExt.GetSize(), lv_strMidString);
								lv_strMidString = _T("");
							}

							// ������ʱ�����
							lv_strMidString = lv_strMidString + lv_char;
							lv_nPreFlag = 2;
						}
					}
					else if( lv_char == DEF_IVR_DIAL_LONGPAUSE || lv_char == DEF_IVR_DIAL_MIDPAUSE || lv_char == DEF_IVR_DIAL_SHORTPAUSE )
					{ // ��ʱ�ָ���
						if( lv_nPreFlag == 2 )	// ǰһ����DTMF
						{
							strArrExt.Add(lv_strMidString);
							WriteTrace(TraceDetail, "IVR Channel: %d dial ExtNumber add pause section: %d - '%s' ", 
								m_intLineNO, strArrExt.GetSize(), lv_strMidString);
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
						m_intLineNO, strArrExt.GetSize(), lv_strMidString);
					lv_strMidString = _T("");
				}
			}
		}

		// Sun added 2012-06-26
		memset(lv_sSetANI, 0x00, sizeof(lv_sSetANI));
		if( lv_data2.setANI > 0 )
		{
			// ȡ����Դ·��
			memset(filename, 0x00, sizeof(filename));
			if(GetResourcePathWithID(lv_data2.setANI, filename) != NULL)
				strncpy(lv_sSetANI, filename, sizeof(lv_sSetANI) - 1);
		}

		// Sun added 2010-12-01
		WriteTrace(TraceDebug, "IVR Channel: %d will dial Number: %s set ANI: %s with %d pauses, Ext: %s with %d pauses", 
			m_intLineNO, lv_sPhoneNo, lv_sSetANI, strArrMain.GetSize(), lv_sExtNo, strArrExt.GetSize());
		WriteTrace(TraceDebug, "IVR Channel: %d will dial Number AsyncDial: %d, AsyncDTMF: %d, ExtAsyncDial: %d, ExtAsyncDTMF: %d", 
			m_intLineNO, lv_blnAsyncDial, lv_blnAsyncDTMF, lv_blnExtAsyncDial, lv_blnExtAsyncDTMF);

		//-------------------------------------------------------
		// Sun added 2012-07-04
		/// Show ANI & DNIS
		char lv_strCallee[50];
		memset(lv_strCallee, 0x00, sizeof(lv_strCallee));
		if( lv_sExtNo.GetLength() > 0 )
			sprintf(lv_strCallee, "%s-%s", lv_sPhoneNo, lv_sExtNo);
		else
			sprintf(lv_strCallee, "%s", lv_sPhoneNo);
		NF_SetCalleeID((UC *)lv_strCallee);
		if( lv_sSetANI[0] != 0 )
			NF_SetCallerID((UC *)lv_sSetANI);
		PropertyChanged(SCR_CHANNEL_ITEM_CALLERID);
		//-------------------------------------------------------

		// Sun added 2005-05-27
		if( m_blnCCTalking )
			m_bytCCDialProgress = 1;

		// ���������ͽ��в���
		m_bytOBResult = _pcC_OB_RESULT_OTHERS;
		if( lv_sPhoneNo.GetLength() > 0 || strArrMain.GetSize() > 0 )
		{
			memset(lv_sTemp, 0x00, sizeof(lv_sTemp));
			sprintf(lv_sTemp, "%s%s", lv_sPredial, lv_sPhoneNo);

			/// �첽�Ⲧ�¼�
			m_OBEvent.m_nEventID = 1;
			m_OBEvent.Reset();
			
			// Sun added 2012-01-17
			/// �Ƿ�ǿ��ժ��
			if( lv_data1.explictoffhook )
			{
				m_oARS.ivr_channel_hook(DX_OFFHOOK, 0, true);
			}

			for( lv_loop = 1; lv_loop <= lv_data1.trytimes; lv_loop++ )
			{
				/// ��ֹͨ����ʱ
				SetTickTime();

				// Sun added 2006-09-15
				if( !m_bInboundCall )
					break;

				if( lv_data1.dialtype == 0 )
				{
					/// ����������
					/// Sun replaced 2010-12-09
					/// From
					//if( lv_data1.connecttype == 1 )
					/// To
					if( lv_blnAsyncDial )
					{
						// �첽
						lv_nResult = m_oARS.ivr_channel_dailout(lv_sTemp, lv_sSetANI, lv_data1.timeout, true, &m_callrefno);
						if( lv_nResult == VOICE_OBT_FAILED )
							break;		// Exit for loop
					}
					else
					{
						// ͬ��
						m_callrefno = 0;
						lv_nResult = m_oARS.ivr_channel_dailout(lv_sTemp, lv_sSetANI, lv_data1.timeout, false, &m_callrefno);
						if( lv_nResult == VOICE_OBT_FAILED )
							break;		// Exit for loop
						else
						{
							if( lv_nResult == VOICE_OBT_SUCC )
								m_bytOBResult = _pcC_OB_RESULT_SUCC;
							else if( lv_nResult == VOICE_OBT_FAX )
								m_bytOBResult = _pcC_OB_RESULT_FAX_TONE;
							else if( lv_nResult == VOICE_OBT_NOANSWER )
								m_bytOBResult = _pcC_OB_RESULT_NO_ANS;
							else if( lv_nResult == VOICE_OBT_BUSY )
								m_bytOBResult = _pcC_OB_RESULT_DEST_BUSY;
							else if( lv_nResult == VOICE_OBT_BREAK )
								m_bytOBResult = _pcC_OB_RESULT_ABANDON;
							else if( lv_nResult == VOICE_OBT_STATUS )
								m_bytOBResult = _pcC_OB_RESULT_DEST_WRONG;
							else if( lv_nResult == VOICE_OBT_NORINGBACK )
								m_bytOBResult = _pcC_OB_RESULT_NOT_REACH;
							else if( lv_nResult == VOICE_OBT_NORINGBACK )
								m_bytOBResult = _pcC_OB_RESULT_NOT_REACH;
						}
					}
				}
				else
				{
					/// CTI����
					//// ģ���豸����ǰ��ժ��
					if( m_oARS.m_nDevType == DEV_ANALOG )
						m_oARS.ivr_channel_hook(DX_OFFHOOK);

					//// �����Ⲧ��Ϣ - ֻ��������
					if( !MQ_SendMsg_CallOut(lv_sTemp) )
						break;
					m_bCTICallOut = TRUE;
				}

				/// Sun added 2010-12-01 [B]
				/// ��ʱ������DTMF
				for( lv_nPauseLoop = 0; lv_nPauseLoop < strArrMain.GetSize(); lv_nPauseLoop++ )
				{
					lv_strMidString = strArrMain.GetAt(lv_nPauseLoop);
					lv_char = lv_strMidString.GetAt(0);
					if( strchr(DEF_IVR_DTMF_CHARSET, lv_char) != NULL )
					{
						// ����DTMF
						if( m_oARS.ivr_channel_sendDTMF((LPCSTR)lv_strMidString, lv_blnAsyncDTMF) == -1 )
							return NODE_RESULT_ERROR;
					}
					else
					{
						// ��ʱ
						for( lv_nPauseSubLoop = 0; lv_nPauseSubLoop < lv_strMidString.GetLength(); lv_nPauseSubLoop++ )
						{
							lv_char = lv_strMidString.GetAt(lv_nPauseSubLoop);
							if( lv_char == DEF_IVR_DIAL_LONGPAUSE )
							{
								SmartSleep(1000);
							}
							else if( lv_char == DEF_IVR_DIAL_MIDPAUSE )
							{
								Sleep(500);
							}
							else if( lv_char == DEF_IVR_DIAL_SHORTPAUSE )
							{
								Sleep(200);
							}

							/// ��ֹͨ����ʱ
							SetTickTime();

							// Sun added 2012-07-04
							/// ���йһ�
							if( !m_bInboundCall )
								break;
						}
					}
				}

				/// ��ͨ�б�
				if( lv_data1.connecttype == 1 && 
					GetCTIOpened() )
				{
					if( m_OBEvent.Wait(lv_data1.timeout * 1000) == true )
					{
						if( m_OBEvent.m_nEventID >= _pcC_OB_RESULT_SUCC && m_OBEvent.m_nEventID <= _pcC_OB_RESULT_OTHERS )
							m_bytOBResult = (UC)m_OBEvent.m_nEventID;
					}
				}
				else
					m_bytOBResult = _pcC_OB_RESULT_SUCC;

				/// �Ƿ�����
				if( m_bytOBResult == _pcC_OB_RESULT_SUCC || 
					m_bytOBResult == VOICE_OBT_FAX ||
					m_bytOBResult == _pcC_OB_RESULT_OTHERS )
					break;				// Exit for loop
				else if( lv_loop + 1 <= lv_data1.trytimes )
				{
					// �һ� & ����
					m_oARS.ivr_channel_hook(DX_ONHOOK);
					SmartSleep(2000);
				}
			}
		}

		// Sun added 2006-09-15
		m_nOBWaitTime = (GetTickCount() - lv_dwOBStartTick) / 1000;

		// ���Ž��
		if( m_bytOBResult == _pcC_OB_RESULT_SUCC && m_bInboundCall )
		{
			/// ���ֻ���
			if( lv_sExtNo.GetLength() > 0 )
			{
				//// ��ʱ
				if( lv_data1.extdelay > 0 && lv_data1.extdelay < 60 )
					SmartSleep((int)lv_data1.extdelay * 1000);

				//// DTMF����
				if( m_oARS.ivr_channel_sendDTMF((LPCSTR)lv_sExtNo, lv_blnExtAsyncDial) == -1 )
					return NODE_RESULT_ERROR;
			}

			/// Sun added 2010-12-01 [B]
			if( strArrExt.GetSize() > 0 )
			{
				/// ��ʱ������DTMF
				for( lv_nPauseLoop = 0; lv_nPauseLoop < strArrExt.GetSize(); lv_nPauseLoop++ )
				{
					lv_strMidString = strArrExt.GetAt(lv_nPauseLoop);
					lv_char = lv_strMidString.GetAt(0);
					if( strchr(DEF_IVR_DTMF_CHARSET, lv_char) != NULL )
					{
						// ����DTMF
						if( m_oARS.ivr_channel_sendDTMF((LPCSTR)lv_strMidString, lv_blnExtAsyncDTMF) == -1 )
							return NODE_RESULT_ERROR;
					}
					else
					{
						// ��ʱ
						for( lv_nPauseSubLoop = 0; lv_nPauseSubLoop < lv_strMidString.GetLength(); lv_nPauseSubLoop++ )
						{
							lv_char = lv_strMidString.GetAt(lv_nPauseSubLoop);
							if( lv_char == DEF_IVR_DIAL_LONGPAUSE )
							{
								SmartSleep(1000);
							}
							else if( lv_char == DEF_IVR_DIAL_MIDPAUSE )
							{
								Sleep(500);
							}
							else if( lv_char == DEF_IVR_DIAL_SHORTPAUSE )
							{
								Sleep(200);
							}

							/// ��ֹͨ����ʱ
							SetTickTime();
						}
					}
				}
			}

			lv_dialresult = lv_loop;
			lv_nNextNode = lv_data2.nd_succ;

			// Sun added 2005-05-27
			if( m_blnCCTalking )
				m_bytCCDialProgress = 2;
		}
		else
		{
			lv_dialresult = 0;
			lv_nNextNode = lv_data2.nd_fail;

			// Sun added 2005-05-27
			if( m_blnCCTalking )
				m_bytCCDialProgress = 3;
		}

		// ��������
		/// ��¼����
		NF_SetUserValue(lv_data1.resultvar, &lv_dialresult, 1); 

		// ת�ƽڵ�
		if( NF_SetNextNode(lv_nNextNode) == 0 )
			return NODE_RESULT_ERROR;

		break;

	default:
		return NODE_RESULT_ERROR;
	}

	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Sun updated 2007-12-12
// Sun added 2005-05-26
// Calling Card
// ���أ�
US CIVRChannel::NF_091(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_091 lv_data1;
	SData2_091 lv_data2;
	UC lv_uservar[255];					// �û�����
	int lv_varlength;					// �û���������
	char	filename[MAX_PATH] = "";
	HRESULT hr;							// ���Է���ֵ
	VARIANT lv_R;						// COM���÷���ֵ
	_bstr_t lv_methodname;
	_bstr_t lv_cardno = "";				// COM���ò���
	_bstr_t lv_telno = "";				// COM���ò���
	_bstr_t lv_connecttime = "";		// COM���ò���
	_bstr_t lv_connectlength = "";		// COM���ò���
	BSTR lv_outputstr;					// COM���÷��ؽ��String
	CString lv_sHHMMSS = "";
	DWORD lv_nTotalSec = 0;
	int lv_nHour, lv_nMin, lv_nSec;
	BOOL lv_blnOB = FALSE;				// �Ƿ��Ⲧ�ɹ�
	CIVRChannel *lv_pChannel = NULL;
	UC lv_btyOBResult;
	DWORD lv_dwDialTick = 0, lv_dwDialLength;
	DWORD lv_dwConnectTick = 0, lv_dwConnLength = 0;
	CString lv_strDialTime = _T(""), lv_strConnTime = _T("");
	CString lv_strTelNo, lv_strConnectLength;
	UC lv_byeCallScale;
	double lv_fltFee = 0;
	BOOL lv_blnTimeOut;

#ifdef INTEL_DIALOGIC
	CIVRGroup *lv_pGroup;
	int lv_nChannelNo;
#endif

	// ��ʱ
	SetTickTime();

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step
		
		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));

		/// ������־
		NF_SetAccessLog(lv_data1.log);
	
		// ȡ�ÿ��ź͵绰����
		lv_varlength = NF_GetUserValue(lv_data1.var_cardno, lv_uservar);
		if( lv_varlength > 0 )
			lv_cardno = ( const char* )lv_uservar;
		else
			return NODE_RESULT_ERROR;
		lv_varlength = NF_GetUserValue(lv_data1.var_telno, lv_uservar);
		if( lv_varlength > 2 )
		{
			lv_telno = ( const char* )lv_uservar;
			lv_strTelNo = ( const char* )lv_uservar;
		}
		else
			return NODE_RESULT_ERROR;

		/// 1�������ͨ��ʱ��
		if( lv_data2.com_talklength > 0 )
		{
			if( m_oUc.IsCreateObject() )
			{
				// ȡ����Դ·��
				if(GetResourcePathWithID(lv_data2.com_talklength, filename) != NULL)
				{
					// ��ʼ��COM���ò���
					lv_outputstr=SysAllocString((OLECHAR *)"000000000000000000000000000000000000000000000000000000000000");

					// ȡ��COM�ӿ�ID
					lv_methodname = filename;

					// ����COM
					try
					{
						VariantInit(&lv_R);
						hr = m_oUc.Invoke(DISPATCH_METHOD, &lv_R, NULL, NULL,
							(BSTR)lv_methodname, TEXT("ss&s"),
							(BSTR)lv_cardno, (BSTR)lv_telno,
							(BSTR FAR *)&lv_outputstr);

						if( SUCCEEDED(hr) )
							lv_sHHMMSS = lv_outputstr;
					}
					catch(_com_error e)
					{
						WriteTrace(TraceError, "Error - Error Msg:%s, Invoke(%s) failed!", 
							e.ErrorMessage(), lv_methodname);
					}
					SysFreeString(lv_outputstr);

					// ���ſ�ͨ��ʱ��
					if( lv_sHHMMSS.GetLength() == 6 )
					{
						lv_nHour = atoi((const char *)lv_sHHMMSS.Left(2));
						lv_nMin = atoi((const char *)lv_sHHMMSS.Mid(2, 2));
						lv_nSec = atoi((const char *)lv_sHHMMSS.Right(2));
						lv_nTotalSec = lv_nHour * 3600 + lv_nMin * 60 + lv_nSec;
						WriteTrace(TraceDebug, "IVR Channel: %d will play time: [%d:%d:%d].", 
							m_intLineNO, lv_nHour, lv_nMin, lv_nSec);

						if( lv_data1.talklentype > 0 && lv_nTotalSec > 0 )
						{
							/// ǰ����������
							if( PlayVoiceEx(lv_data2.vox_talklen) == -1 )
								return NODE_RESULT_ERROR;

							/// ����ʱ��
							if( m_oARS.ivr_channel_timeplay(lv_nTotalSec, lv_data1.talklentype, NF_GetLanguageID()) == -1 )
								return NODE_RESULT_ERROR;
						}
					}
					else
						lv_nTotalSec = atoi((const char *)lv_sHHMMSS);
				}
			}
		}

		// 1�������ϲ���ʹ��
		if( lv_nTotalSec > 0 && lv_nTotalSec < 60 )
		{
			/// ���ſ��������Ч��ʾ
			if( PlayVoiceEx(lv_data2.vox_noservice) == -1 )
				return NODE_RESULT_ERROR;

			// ת�ƽڵ�
			if( NF_SetNextNode(lv_data2.nd_child) == 0 )
				return NODE_RESULT_ERROR;

			return NODE_RESULT_OK;
		}

		// 2���Ⲧ
#ifdef INTEL_DIALOGIC

		/// Ѱ���Ⲧͨ��
		if( lv_data1.obgroup != m_objGroup->GetGroupNO() )
		{
			if( lv_data1.obgroup < theApp.m_nIVRGroups )
			{
				if( (lv_pGroup = PIVRGroup[lv_data1.obgroup]) != NULL )
				{
					lv_nChannelNo = m_intLineNO + lv_pGroup->GetStartCH() - m_objGroup->GetStartCH();
					if( lv_nChannelNo <= lv_pGroup->GetEndCH() )
						lv_pChannel = PIVRCH[lv_nChannelNo];
					else
						WriteTrace(TraceWarn, "Warn - IVR Channel: %d NF_091 - out of channel: %d in group: %d!", m_intLineNO, lv_nChannelNo, lv_data1.obgroup);
				}
			}
			else
				WriteTrace(TraceWarn, "Warn - IVR Channel: %d NF_091 - out of group: %d!", m_intLineNO, lv_data1.obgroup);
		}
		else
			WriteTrace(TraceWarn, "Warn - IVR Channel: %d NF_091 - can't use the same group (%d) of channels for OB call!", m_intLineNO, lv_data1.obgroup);

		/// ��ʼ�Ⲧ
		SetTickTime();
		if( lv_pChannel )
		{
			if( lv_pChannel->GetStarted() )
			{
				if( lv_pChannel->GetStatus() == IVR_IDLE && 
					lv_pChannel->NF_GetOBMsgTelNo() == _T("") )
				{
					/// �����Ⲧ��Ϣ
					//lv_pChannel->NF_SetOBPacketID(0);
					//lv_pChannel->NF_SetOBCampaignID(0);
					//lv_pChannel->NF_SetOBTaskID(0);
					lv_pChannel->NF_SetOBMsgTelNo(lv_strTelNo);

					/// Callout Flow
					lv_pChannel->NF_CallIn();

					/// Wait OB channel started
					lv_pChannel->NF_SetCCTalking(TRUE);
					lv_varlength = 0;
					while( lv_varlength < 100 )
					{
						SetTickTime();
						Sleep(20);
						lv_varlength++;

						if( lv_pChannel->NF_GetCCDialProgress() > 0 )
						{
							lv_blnOB = TRUE;
							break;
						}
					}

					if( !lv_blnOB )
						lv_pChannel->NF_SetCCTalking(FALSE);
				}
				else
					WriteTrace(TraceWarn, "Warn - IVR Channel: %d NF_091 - OB channel: %d is used!", m_intLineNO, lv_nChannelNo);
			}
			else
				WriteTrace(TraceWarn, "Warn - IVR Channel: %d NF_091 - OB channel: %d is not started!", m_intLineNO, lv_nChannelNo);
		}
		else
			WriteTrace(TraceWarn, "Warn - IVR Channel: %d NF_091 - can't find OB channel!", m_intLineNO);

#endif

#ifdef CISCO_CCM

		// ���鷽ʽ�Ⲧ������Ҫ�����ͨ����Դ
		lv_blnOB = FALSE;
		if( m_oARS.ConsultConference(lv_strTelNo) != -1 )
		{
			// �ȴ�����������
			ResetQueueWaitEvent(13);
			if( m_QueueWaitSyncEvent.Wait(PCS_TO_UMS_WAITCONF) == true )
			{
				if( m_QueueWaitSyncEvent.m_nEventID <= 14 )
				{
					lv_pChannel = this;
					lv_blnOB = TRUE;

					lv_btyOBResult = _pcC_OB_RESULT_NO_ANS;
					ResetQueueWaitEvent(14);
				}
				else
					WriteTrace(TraceInfo, "Info - IVR Channel: %d NF_091 - destination:%s is busy or error!", m_intLineNO, lv_strTelNo);
			}
			else
			{
				lv_btyOBResult = _pcC_OB_RESULT_OTHERS;
				WriteTrace(TraceWarn, "Warn - IVR Channel: %d NF_091 - wait ConsultConference finished timeout!", m_intLineNO);
			}

			if( !lv_blnOB )
			{
				// ֹͣ���̣�ʰ�𱣳ֺ���
				ResetQueueWaitEvent(0);
				m_oARS.StopConsultaion();
			}
		}

#endif

		// 3������
		if( lv_blnOB )
		{

#ifdef INTEL_DIALOGIC

			if( m_oARS.sc_channel_link(&lv_pChannel->m_oARS) )
			{
				m_nLinkChannelNo = lv_nChannelNo;
#endif

#ifdef CISCO_CCM

			if( m_oARS.CompleteTransfer(0) != -1 )
			{

#endif

#ifdef CISCO_CME

			if( 1 )		// ToDoCME
			{

#endif

		// 4���ȴ���ͨ
				lv_varlength = 0;
				lv_dwDialTick = GetTickCount();
				lv_strDialTime = CTime::GetCurrentTime().Format("%Y%m%d%H%M%S");
				while( lv_varlength < lv_data1.timeout * 10 )
				{
					SetTickTime();
					Sleep(100);
					lv_varlength++;

#ifdef INTEL_DIALOGIC

					if( lv_pChannel->NF_GetCCDialProgress() > 1 )
					{
						if( lv_pChannel->NF_GetCCDialProgress() < 3 )
						{
							lv_varlength = 0;
							lv_dwConnectTick = GetTickCount();
							lv_strConnTime = CTime::GetCurrentTime().Format("%Y%m%d%H%M%S");
						}
						break;
					}

#else		// CISCO_CCM or CISCO_CME

					if( m_QueueWaitSyncEvent.Wait(0) )
					{
						if( m_QueueWaitSyncEvent.m_nEventID == 14 )
						{
							lv_varlength = 0;
							lv_dwConnectTick = GetTickCount();
							lv_strConnTime = CTime::GetCurrentTime().Format("%Y%m%d%H%M%S");
							lv_btyOBResult = _pcC_OB_RESULT_SUCC;
						}
						else if( m_QueueWaitSyncEvent.m_nEventID == 17 )
						{
							lv_btyOBResult = _pcC_OB_RESULT_DEST_BUSY;
							WriteTrace(TraceWarn, "Warn - IVR Channel: %d NF_091 - dial to busy.", m_intLineNO);
						}
						else
						{
							lv_btyOBResult = _pcC_OB_RESULT_OTHERS;
							WriteTrace(TraceWarn, "Warn - IVR Channel: %d NF_091 - wait answer failed! StatusID=%d", m_intLineNO, m_QueueWaitSyncEvent.m_nEventID);
						}

						break;
					}

#endif

					/// ���йһ�
					if( !m_bInboundCall )
					{
#ifndef INTEL_DIALOGIC
						lv_btyOBResult = _pcC_OB_RESULT_ABANDON;
						WriteTrace(TraceWarn, "Warn - IVR Channel: %d NF_091 - call abandoned! StatusID=%d", m_intLineNO, m_QueueWaitSyncEvent.m_nEventID);
#endif
						break;
					}
				}


		// 5��ͨ��
				if( lv_varlength == 0 )
				{

#ifdef INTEL_DIALOGIC

					lv_btyOBResult = lv_pChannel->NF_GetOBResult();
					lv_dwDialLength = (GetTickCount() - lv_dwDialTick) / 1000;
					lv_blnTimeOut = FALSE;
					while( lv_pChannel->NF_GetCCDialProgress() == 2 )
					{
						SetTickTime();
						Sleep(100);

						/// ���йһ�
						if( lv_pChannel->NF_GetCCTalking() == FALSE || lv_pChannel->GetStatus() < IVR_RUN  )
							break;

#else		// CISCO_CCM or CISCO_CME

					// �ȴ�ͨ������
					ResetQueueWaitEvent(15);

					lv_dwDialLength = (GetTickCount() - lv_dwDialTick) / 1000;
					lv_blnTimeOut = FALSE;
					while( !m_QueueWaitSyncEvent.Wait(0) )
					{
						SetTickTime();
						Sleep(100);

#endif

						/// ���йһ�
						if( !m_bInboundCall )
							break;

						/// �Ʒ���ֹ�һ�
						lv_dwConnLength = (GetTickCount() - lv_dwConnectTick) / 1000;
						if( lv_nTotalSec > 0 )
						{
							if( lv_nTotalSec < lv_dwConnLength )
								break;

							/// ���ų�ʱ����
							if( !lv_blnTimeOut && lv_data1.remindminute > 0 )
							{
								if( lv_nTotalSec - lv_dwConnLength <= lv_data1.remindminute * 60 )
								{
									lv_blnTimeOut = TRUE;

									//------------------------------
									// Sun added 2005-08-23
									/// ��ʱ����
									m_oARS.sc_channel_unlink(&lv_pChannel->m_oARS);
									//------------------------------

									/// ��������ͨ�� n ����
									if( PlayVoiceEx(lv_data2.vox_timeout) == -1 )
										return NODE_RESULT_ERROR;

									if( m_oARS.ivr_channel_digitplay(lv_data1.remindminute, 0, NF_GetLanguageID()) == -1 )
										return NODE_RESULT_ERROR;
									memset(filename, 0x00, sizeof(filename));
									sprintf(filename, "SYS_%d_MINUTE", NF_GetLanguageID());
									if( m_oARS.ivr_channel_play(filename) == -1 )
										return NODE_RESULT_ERROR;

									//------------------------------
									// Sun added 2005-08-23
									/// ���´���
									m_oARS.sc_channel_link(&lv_pChannel->m_oARS);
									//------------------------------
								}
							}
						}
					}

			// 6����¼����
					CString lv_fieldlist, lv_varlist ;
					
					/// Fields
					lv_fieldlist = _T("StartTime, ChannelID, CardNo, ANI, DNIS, DialFlag, DialTime, WaitTime, "
						"CallScale, ConnectTime, ConnectLength, Fee");
				
					/// Values
					if( lv_dwConnectTick > 0 )
						lv_dwConnLength = (GetTickCount() - lv_dwConnectTick) / 1000;
					else
						lv_dwConnLength = 0;

					if( lv_strTelNo.Left(2) == _T("00") )
						lv_byeCallScale = _pcC_CALLSCALE_INTERNATIONAL;
					else if( lv_strTelNo.Left(1) == _T("0") )
						lv_byeCallScale = _pcC_CALLSCALE_NATIONAL;
					else
						lv_byeCallScale = _pcC_CALLSCALE_LOCAL;

					/// ���㻰��
					if( lv_dwConnLength > 0 && lv_data2.com_billing > 0 )
					{
						if( m_oUc.IsCreateObject() )
						{
							// ȡ����Դ·��
							memset(filename, 0x00, sizeof(filename));
							if(GetResourcePathWithID(lv_data2.com_billing, filename) != NULL)
							{
								// ��ʼ��COM���ò���
								lv_outputstr=SysAllocString((OLECHAR *)"000000000000000000000000000000000000000000000000000000000000");
								lv_connecttime = (_bstr_t)lv_strConnTime;
								lv_strConnectLength.Format("%u", lv_dwConnLength);
								lv_connectlength = (_bstr_t)lv_strConnectLength;

								// ȡ��COM�ӿ�ID
								lv_methodname = filename;

								// ����COM
								try
								{
									VariantInit(&lv_R);
									hr = m_oUc.Invoke(DISPATCH_METHOD, &lv_R, NULL, NULL,
										(BSTR)lv_methodname, TEXT("sss&s"),
										(BSTR)lv_telno, (BSTR)lv_connecttime, (BSTR)lv_connectlength,
										(BSTR FAR *)&lv_outputstr);

									if( SUCCEEDED(hr) )
									{
										lv_strConnectLength = lv_outputstr;
										lv_fltFee = atof((LPCSTR)lv_strConnectLength) + 0.00001;  // Sun updated 2008-01-18
										WriteTrace(TraceDebug, "Debug - Channel: %d run NF_091() got CC CDR len:%s fee: %s", 
											m_intLineNO, (LPCSTR)lv_connectlength, (LPCSTR)lv_strConnectLength);
									}
								}
								catch(_com_error e)
								{
									WriteTrace(TraceError, "Error - Error Msg:%s, Invoke(%s) failed!", 
										e.ErrorMessage(), lv_methodname);
								}
								SysFreeString(lv_outputstr);
							}
						}
					}

					CString lv_strTelNo;
					lv_strTelNo = SeperateTelNo(lv_telno);

					lv_varlist.Format("'%s', %d, '%s', '%s', '%s', '%c', '%s', %d, %d, '%s', %d, %f",
						m_ctCallInTime.Format("%Y%m%d%H%M%S"), m_intLineNO, (LPCSTR)lv_cardno,
						(char *)ChData.callerid, (LPCSTR)lv_strTelNo, (char)lv_btyOBResult, 
						lv_strDialTime, lv_dwDialLength, lv_byeCallScale,
						lv_strConnTime, lv_dwConnLength, lv_fltFee);

					InsertACCSCDR(lv_fieldlist, lv_varlist);
				}
			}

#ifdef INTEL_DIALOGIC
			lv_pChannel->NF_SetCCTalking(FALSE);
			lv_pChannel->SetHookOn();
#endif

#ifdef CISCO_CCM
			// ֹͣ���̣�ʰ�𱣳ֺ���
			if( m_QueueWaitSyncEvent.m_nEventID > 10 )
			{
				ResetQueueWaitEvent(0);
				m_oARS.StopConsultaion();
			}
#endif

#ifdef CISCO_CME
			// TODOCME
#endif

			// ת�ƽڵ�
			if( NF_SetNextNode(lv_data2.nd_child) == 0 )
				return NODE_RESULT_ERROR;
		}
		else
		{
			WriteTrace(TraceWarn, "Warn - IVR Channel: %d NF_091 - CompleteTransfer failed! StatusID=%d", m_intLineNO, m_QueueWaitSyncEvent.m_nEventID);

			// Sun added 2007-12-12
			// ת�ƽڵ�
			if( NF_SetNextNode(lv_data2.nd_child) == 0 )
				return NODE_RESULT_ERROR;
		}

		break;

	default:
		return NODE_RESULT_ERROR;
	}

	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Sun added 2005-03-16
// �첽ͨ�Žڵ�
// ���أ�
US CIVRChannel::NF_096(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_096 lv_data1;
	SData2_096 lv_data2;
	US lv_nNextNode;
	int lv_nDataLen, lv_nPlay = 0;
	char lv_pData[PCS_MAX_MESSAGE_SIZE];
	char filename[MAX_PATH] = "";
	BOOL lv_blnSendMsg;
	DWORD lv_tickA;
	
	// ��ʱ
	SetTickTime();

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step
		
		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));

		/// ������־
		NF_SetAccessLog(lv_data1.log);
	
		if( pMainWindow->m_sckExternalServer.GetConnected() )
		{
			/// ��֯��Ϣ����
			lv_nDataLen = FillupExtSendData(&lv_data1, &lv_data2, lv_pData);

			/// �ȴ���Ӧ
			lv_blnSendMsg = TRUE;
			lv_tickA = GetTickCount();

			// 5 �����ϲŷ���
			if( lv_data1.timeout >= 5 )
			{
				/// Get Play File Name
				if( lv_data2.vox_wt > 0 )
				{
					if( GetResourcePathWithID(lv_data2.vox_wt, filename) != NULL )
					{
						lv_nPlay = m_oARS.ivr_channel_async_play(filename, NULL, true, true);
						if( lv_nPlay == -1 )
							return NODE_RESULT_ERROR;

						// Sun added 2012-11-23
						SetIsAsynPlaying(TRUE);
					}
				}
			}

			while( true )
			{
				/// ������Ϣ
				if( lv_blnSendMsg )
				{
					lv_blnSendMsg = FALSE;
					m_CommEvent.m_nEventID = lv_data2.command;
					m_CommEvent.Reset();
					if( !SendExternMsg(lv_data2.command, lv_data1.seperator, lv_pData, lv_nDataLen) )
					{
						lv_nNextNode = lv_data2.nd_timeout;
						break;
					}
				}

				if( m_CommEvent.Wait(PCS_TO_UMS_EXTMSG) == true )
				{
					/// Stop Play
					if( lv_nPlay == 1 && lv_data1.carryonasynplay == 0 )
					{
						m_oARS.ivr_channel_stop_play();

						// Sun added 2012-11-23
						SetIsAsynPlaying(FALSE);
					}

					/// ��������
					//// Receive Variables
					FillupExtReceiveData(&lv_data2, m_CommEvent.m_pData);
					
					//// Ext Data
					switch( lv_data1.extdata )
					{
					case 1:						// �����ļ�
						memset(filename, 0x00, sizeof(filename));
						sprintf(filename, "%s%s%s.txt", gb_FN_TempPath, lv_data2.fileprefix, m_strExtSN);
						WriteBufferToTextFile(filename, m_strExtData);
						break;

					case 2:						// ��¼������
						NF_SetUserValue(lv_data1.extvar, (UC *)m_strExtData); 
						WriteTrace(TraceDetail, "NT_096: New Value [%d] is %s", lv_data1.extvar, m_strExtData);
						break;

					case 3:						// ��¼��AppData
						NF_SetAppData((UC *)m_strExtData);
						WriteTrace(TraceDetail, "NT_096: New AppData is %s", m_strExtData);
						break;

					case 4:						// ��¼��UserData
						NF_SetUserData(m_strExtData, PCS_DEF_DATALEN_USER);
						WriteTrace(TraceDetail, "NT_096: New UserData is %s", m_strExtData);
						break;

					case 5:						// TTS
						if( TTSPlayBufferEx(m_strExtData, 'N', true) == -1 )
							return NODE_RESULT_ERROR;
						break;
					}

					lv_nNextNode = lv_data2.nd_child;
					break;
				}

				/// ��ֹͨ����ʱ
				SetTickTime();

				/// Sun added 2012-04-21
				// ���߼��
				if( !m_bInboundCall )
					return NODE_RESULT_ERROR;

				if( m_oARS.m_nDevType == DEV_ANALOG )
					if( m_oARS.ivr_channel_getstate(LINESTATUS_HOOKST) == VOICE_LS_HOOKST_ONHOOK )
						return NODE_RESULT_ERROR;

				/// �Ƿ�ʱ
				if( (GetTickCount() - lv_tickA) / 1000 > lv_data1.timeout )
				{
					/// Stop Play
					if( lv_nPlay == 1 && lv_data1.carryonasynplay == 0 )
						m_oARS.ivr_channel_stop_play();

					lv_nNextNode = lv_data2.nd_timeout;
					break;
				}
			}
		}
		else
			lv_nNextNode = lv_data2.nd_timeout;

		if( m_oARS.m_nDevType == DEV_ANALOG )
			if( m_oARS.ivr_channel_getstate(LINESTATUS_HOOKST) == VOICE_LS_HOOKST_ONHOOK )
				return NODE_RESULT_ERROR;

		/// ת�ƽڵ�
		if( NF_SetNextNode(lv_nNextNode) == 0 )
			return NODE_RESULT_ERROR;

		break;

	default:
		return NODE_RESULT_ERROR;
	}

	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// �û�DLL
// ���أ�
US CIVRChannel::NF_100(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_100 lv_data1;
	SData2_100 lv_data2;

	char	filename[MAX_PATH] = "";
	HRESULT hr;							// ���Է���ֵ
	HINSTANCE    DIHinst = NULL;
	Userdllfunc  DirectUserDllFunc = NULL; 

	// ��ʱ
	SetTickTime();

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step

		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));

		/// ������־
		NF_SetAccessLog(lv_data1.log);
	
		// Get DLL Name with ID
		if( lv_data2.dll_fid > 0 )
		{
			memset(filename, 0x00, sizeof(filename));
			if(GetResourcePathWithID(lv_data2.dll_fid, filename) != NULL)
			{
				DIHinst = AfxLoadLibrary(filename);
				if( DIHinst != NULL )
				{
					DirectUserDllFunc = (Userdllfunc)GetProcAddress(DIHinst, "Userdllfunc");
					AfxFreeLibrary(DIHinst); 
				}

				if( DirectUserDllFunc != NULL )
				{
					hr = DirectUserDllFunc((int)GetLineNO());
					if(FAILED(hr))
					{
						return NODE_RESULT_ERROR;
					}
				}
			}
		}

		// ת��
		if(NF_SetNextNode(lv_data2.nd_child) == 0)
			return NODE_RESULT_ERROR;
	
		break;

	default:
		return NODE_RESULT_ERROR;
	}
		
	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// �û�COM
// ���أ�
US CIVRChannel::NF_101(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_101 lv_data1;
	SData2_101 lv_data2;
	
	HRESULT hr;							// ���Է���ֵ
	VARIANT lv_R;						// COM���÷���ֵ
	US		lv_CH, lv_ChildNode;
	UC		lv_LanguageID;
	char	filename[MAX_PATH] = "";
	_bstr_t lv_methodname;
	BSTR lv_com_uservars;
	char lv_ChBuff[sizeof(ChData)];

	// ��ʱ
	SetTickTime();

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step

		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));

		/// ������־
		NF_SetAccessLog(lv_data1.log);
		lv_ChildNode = ChData.nextnode;	
		lv_LanguageID = NF_GetLanguageID();

		// Get COM Name with ID
		if(lv_data2.com_iid > 0 )
		{
			if( m_oUc.IsCreateObject() )
			{
				memset(filename, 0x00, sizeof(filename));
				if(GetResourcePathWithID(lv_data2.com_iid, filename) != NULL)
				{
					/// COM Name
					WriteTrace(TraceDetail, "IVR Channel: %d NT_102 will call COM Name: %s", m_intLineNO, filename);

					/// ���� COM
					lv_methodname = filename;
					memcpy(lv_ChBuff, &ChData, sizeof(ChData));
					lv_com_uservars=SysAllocStringByteLen(lv_ChBuff, sizeof(ChData));
					try
					{
						VariantInit(&lv_R);
						lv_CH = GetLineNO();
						hr = m_oUc.Invoke(DISPATCH_METHOD, &lv_R, NULL, NULL, 
							lv_methodname, TEXT("i&s"), 
							lv_CH,
							(BSTR FAR *)&lv_com_uservars);
						if(FAILED(hr))
						{
							/// Failed call COM
							WriteTrace(TraceWarn, "IVR Channel: %d NT_101 failed to call COM Name: %s", m_intLineNO, filename);

							SysFreeString(lv_com_uservars);
							return NODE_RESULT_ERROR;
						}
					}
					catch(_com_error e)
					{
						WriteTrace(TraceError, "Error - Error Msg:%s, Invoke(%s) failed!", 
							e.ErrorMessage(), lv_methodname);

						SysFreeString(lv_com_uservars);
						return NODE_RESULT_ERROR;
					}

					memcpy(&ChData, (const char *)lv_com_uservars, sizeof(ChData));
					SysFreeString(lv_com_uservars);

					// Change Language?
					if( ChData.language != lv_LanguageID )
						NF_SetLanguageID(ChData.language);
					TRACE("Set New Language ID:%d\n", ChData.language);
				}
			}
		}

		// ת��
		/// If user did not change call flow,
		/// we will go to default node.
		if( lv_ChildNode == ChData.nextnode )
			lv_ChildNode = 0;
		else
			lv_ChildNode = ChData.nextnode;
		if( lv_ChildNode < 256 )
			lv_ChildNode = lv_data2.nd_child;
		if(NF_SetNextNode(lv_ChildNode) == 0)
			return NODE_RESULT_ERROR;
		
		break;

	default:
		return NODE_RESULT_ERROR;
	}
		
	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Sun updated 2004-12-30
// ��¼����
// ���أ�
US CIVRChannel::NF_102(const int event, const UC *f_data1, const UC *f_data2)
{
	SData1_102 lv_data1;
	SData2_102 lv_data2;
	
	HRESULT hr;							// ���Է���ֵ
	VARIANT lv_R;						// COM���÷���ֵ
	char	filename[MAX_PATH] = "";
	char	lv_ChBuff[sizeof(ChData)];
	_bstr_t lv_methodname;
	BSTR lv_com_uservars;
	BSTR lv_outputstr;					// COM���÷��ؽ��String
	US lv_childenode;
	int lv_strlen;
	CString lv_sFormat, lv_sComp, lv_sTMP;

	// ��ʱ
	SetTickTime();

	switch(event)
	{
	case LE_PROCEND:					// must be the first Step

		// Get parameters
		memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));
		memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));

		/// ������־
		NF_SetAccessLog(lv_data1.log);

		// User Variable
		lv_childenode = 0;
		if( lv_data1.var_chg > 0 )
		{
			// Get COM Name with ID
			if( lv_data2.com_iid > 0 &&  m_oUc.IsCreateObject() )
			{
				// ��ʼ��COM���ò���
				memset(filename, 0x00, sizeof(filename));
				NF_GetUserValue(lv_data1.var_chg, (UC *)filename);
				wchar_t lv_wcValue[100];
				mbstowcs(lv_wcValue, filename, strlen(filename)+1);
				lv_outputstr=SysAllocString(lv_wcValue);
				lv_sFormat = lv_outputstr;
				WriteTrace(TraceDetail, "IVR Channel: %d NT_102: Old Value [%d] is %s", m_intLineNO, lv_data1.var_chg, lv_sFormat);

				if(GetResourcePathWithID(lv_data2.com_iid, filename) != NULL)
				{
					/// COM Name
					WriteTrace(TraceDetail, "IVR Channel: %d NT_102 will call COM Name: %s", m_intLineNO, filename);

					/// ���� COM
					lv_methodname = filename;
					memcpy(lv_ChBuff, &ChData, sizeof(ChData));
					lv_com_uservars=SysAllocStringByteLen(lv_ChBuff, sizeof(ChData));

					try
					{
						VariantInit(&lv_R);
						hr = m_oUc.Invoke(DISPATCH_METHOD, &lv_R, NULL, NULL,
							(BSTR)lv_methodname, TEXT("&s&s"),
							(BSTR FAR *)&lv_com_uservars,
							(BSTR FAR *)&lv_outputstr);
						
						// Sun removed 2008-05-05
						//SysFreeString(lv_com_uservars);

						if(FAILED(hr))
						{
							/// Failed call COM
							WriteTrace(TraceWarn, "IVR Channel: %d NT_102 failed to call COM Name: %s", m_intLineNO, filename);

							SysFreeString(lv_outputstr);			// Sun added 2003-05-14
							SysFreeString(lv_com_uservars);			// Sun added 2008-05-05

							return NODE_RESULT_ERROR;
						}
					}
					catch(_com_error e)
					{
						WriteTrace(TraceError, "Error - IVR Channel: %d Error Msg:%s, Invoke(%s) failed!", 
							m_intLineNO, e.ErrorMessage(), lv_methodname);

						SysFreeString(lv_outputstr);			// Sun added 2003-05-14
						SysFreeString(lv_com_uservars);

						return NODE_RESULT_ERROR;
					}

					// ����ֵ
					/// Variable
					//lv_strlen = SysStringLen(lv_outputstr);
					//if( lv_strlen > 100 )
					//	lv_strlen = 100;
					lv_sFormat = lv_outputstr;
					strcpy(filename, (LPCSTR)lv_sFormat);
					//strBSTRCpy(filename, lv_outputstr, lv_strlen);

					// Sun added 2008-05-05
					memcpy(&ChData, (const char *)lv_com_uservars, sizeof(ChData));
					SysFreeString(lv_com_uservars);

					/// Transfer to Node
					lv_childenode = (US)lv_R.iVal;
				}

				// ��¼���
				SysFreeString(lv_outputstr);			// Sun moved ahead 2003-05-14
				NF_SetUserValue(lv_data1.var_chg, (UC *)filename);
				WriteTrace(TraceDetail, "IVR Channel: %d NT_102: New Value is %s", m_intLineNO, lv_sFormat);
			}
			else
			{
				// Set with default value
				if( lv_data2.value[0] == ':' )
				{
					/// IVR Variable
					UC lv_uservar[PCS_MAX_VAR_LEN];		// �û�����

					// Sun updated 2005-08-25
					int lv_nPos;
					lv_sTMP = (const char *)(lv_data2.value+1);
					lv_nPos = lv_sTMP.FindOneOf("-, ;+");
					if( lv_nPos > 0 )
					{
						/// First Value
						lv_sComp = lv_sTMP.Left(lv_nPos);
						memset(lv_uservar, 0x00, sizeof(lv_uservar));
						NF_GetUserValue((const char *)lv_sComp, lv_uservar);
						lv_sFormat = (LPCSTR)lv_uservar;

						/// Second Value
						if( lv_sTMP.GetAt(lv_nPos) == '+' && lv_sTMP.GetAt(lv_nPos+1) == '+' )
						{
							// ++
							lv_nPos = atoi(lv_sFormat);
							lv_nPos++;
							lv_sFormat.Format("%u", lv_nPos);
						}
						else
						{
							// cat-string
							if( lv_sTMP.GetAt(lv_nPos+1) == ':' )
							{
								lv_sComp = lv_sTMP.Mid(lv_nPos+2);
								memset(lv_uservar, 0x00, sizeof(lv_uservar));
								NF_GetUserValue((const char *)lv_sComp, lv_uservar);
								lv_sFormat += (LPCSTR)lv_uservar;
							}
							else
							{
								lv_sComp = lv_sTMP.Mid(lv_nPos+1);
								lv_sFormat += lv_sComp;
							}
						}
					}
					else
					{
						lv_sComp = lv_sTMP;
						memset(lv_uservar, 0x00, sizeof(lv_uservar));
						NF_GetUserValue((const char *)lv_sComp, lv_uservar);
						lv_sFormat = (LPCSTR)lv_uservar;
					}
				}
				else
					lv_sFormat = (LPCSTR)lv_data2.value;

				// Sun IVR Test
				WriteTrace(TraceDetail, "IVR Channel: %d NT_102: will record [%d] with value %s", m_intLineNO, lv_data1.var_chg, lv_sFormat);

				/// ����
				switch( lv_data1.convert )
				{
				case DEF_NODE016_CONVERT_LEFT:
					lv_strlen = lv_sFormat.GetLength();
					if( lv_data1.param1 < lv_strlen )
						lv_strlen = lv_data1.param1;
					lv_sComp = lv_sFormat.Left(lv_strlen);
					break;
				case DEF_NODE016_CONVERT_RIGHT:
					lv_strlen = lv_sFormat.GetLength();
					if( lv_data1.param1 < lv_strlen )
						lv_strlen = lv_data1.param1;
					lv_sComp = lv_sFormat.Right(lv_data1.param1);
					break;
				case DEF_NODE016_CONVERT_MID:
					lv_strlen = lv_sFormat.GetLength();
					if( lv_data1.param1 < lv_strlen )
					{
						if( lv_data1.param1 + lv_data1.param2 < lv_strlen )
							lv_strlen = lv_data1.param2;
						else
							lv_strlen -= lv_data1.param1;
						lv_sComp = lv_sFormat.Mid(lv_data1.param1, lv_strlen);
					}
					else
						lv_sComp = _T("");
					break;
				default:
					lv_sComp = lv_sFormat;
				}

				NF_SetUserValue(lv_data1.var_chg, (UC *)lv_sComp.GetBuffer(lv_sComp.GetLength()), lv_sComp.GetLength());
				WriteTrace(TraceDetail, "IVR Channel: %d NT_102: New Value is %s", m_intLineNO, lv_sComp);
			}
		}

		// ת��
		if( lv_childenode < 256 )
			lv_childenode = lv_data2.nd_child;
		if( NF_SetNextNode(lv_childenode) == 0 )
			return NODE_RESULT_ERROR;
		
		break;

	default:
		return NODE_RESULT_ERROR;
	}
		
	return NODE_RESULT_OK;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Goto the specificated node and run it precess
BOOL CIVRChannel::NF_GotoNode(US event, US f_node)
{
	// if f_node differs from ChData.nextnode,
	// reset it. Thus Messager can control IVR flow.
	if( f_node != 0 && f_node != NF_GetNextNode() )
	{
		// ���ýڵ�
		ChData.nextnode = f_node;
	}

	// Set state changing reason
	NF_SetNodeEvent(event);

	return TRUE;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Get Group# with specified Line#
UC CIVRChannel::GetGroupNoWithLineNo(US lineno)
{
	UC lv_groupno = 0;
	int lv_loop;

	for(lv_loop = 0; lv_loop < theApp.m_nIVRGroups; lv_loop++)
	{
		if(lineno >= PGroupInfo[lv_loop].StartCH && lineno <= PGroupInfo[lv_loop].EndCH)
		{
			lv_groupno = lv_loop;
			break;
		}
	}

	return(lv_groupno);
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------

void CIVRChannel::NF_CallIn(UC f_ServiceCode, US f_StartNode)
{
	US lv_root = 256;

	// Select Current ET
	CIVRGroup *lv_Group = m_objGroup;

	// Sun added 2002-07-18
	if( !lv_Group->GetStarted() )
		return;

	// Sun added for M18 Error
	if( m_bInboundCall )
	{
		if( !NF_GetCCTalking() )
		{
			WriteTrace(TraceError, "IVR Channel: %d callin in error status: %d, previous call processing seems not end.", 
				m_intLineNO, m_bytStatus);
		}

		return;
	}

	// Sun added 2012-11-23
	if( GetIsAsynPlaying() )
	{
		WriteTrace(TraceWarn, "IVR Channel: %d callin but AsynPlaying flag is error!", m_intLineNO);

#ifdef INTEL_DIALOGIC
		m_oARS.ivr_channel_dx_stop();
#endif

		SetIsAsynPlaying(FALSE);
	}

	m_cET = 1 - lv_Group->GetCurrentETID();

	// Sun added 2007-11-18
	/// ��¼ʹ�õ�CallFlowID
	m_nCurrentCallFlowID = lv_Group->GetIVRProgramID();

	// Sun added 2008-01-12
	m_nCallFlowNodeIndex = 0;

	// Sun added 2004-12-30
	memcpy(&m_Data018_1, (UC *)(&lv_Group->m_Data018_1[m_cET]), DEF_NODE_DATA_SMALL_LEN);
	memcpy(&m_Data018_2, (UC *)(&lv_Group->m_Data018_2[m_cET]), DEF_NODE_DATA_BIG_LEN);

	// Sun added 2007-07-11
	memset(&m_Data061_1, 0x00, DEF_NODE_DATA_SMALL_LEN);
	memset(&m_Data061_2, 0x00, DEF_NODE_DATA_BIG_LEN);

	// Reset Tick
	SetTickTime();

	// Switch to waiting answer state
	SetStatus(IVR_W_ANSWER);

	// Start Timing Talk
	SetBeginTime();

	// Sun added 2003-05-02
	m_bInboundCall = TRUE;

	// Sun added 2009-02-28, [B]
	m_QueueWaitSyncEvent.m_nEventID = 0;

	// Sun added 2006-02-26
	IncreaseCallCount();

	// Sun addded 2011-10-20, �Ƿ��Ѿ���¼IVR�굥
	m_blnCDRRecorded = FALSE;

	// Sun added 2002-05-27
	if( !GetCTIAvailble() )
	{
		// Automatic Call Ref. ID.
		m_lngCurCallID = m_bytGroupNo * 256 + m_intLineNO;
	}

	// Goto root node
	if( f_StartNode >= 256 )
		lv_root = f_StartNode;
	else
	{
		// Sun modified 2002-05-23
		/// Assume 256 as the default Root Node.
		/// If node 256 exists, it must be able to transfer to the exact Root (eg. MainMenu)
		/// That's to say, 256 is the physical root,
		/// while the node id defined in No.1 node is logic root.
		if( NF_Find_WorkNode(lv_root) < 0 )
		{	/// 256 not exist then begin with the logic root
			lv_root = lv_Group->GetRootNode(m_cET);
		}
	}

	NF_GotoNode(LE_CALLIN, lv_root);	// LE_CALLIN

	// Set Service Code
	if( (f_ServiceCode >= '0' && f_ServiceCode <= '9') ||
		(f_ServiceCode >= 'A' && f_ServiceCode <= 'Z') ||
		(f_ServiceCode >= 'a' && f_ServiceCode <= 'z') )
		NF_SetUserValue(SysVar_MainMenu, &f_ServiceCode, 1);

	// Notify main window to refresh
	m_callinEvent.Set();
	PropertyChanged();

	return;
}

//----------------------------------------------------------------------------------------------
// �����ض��ڵ�
long CIVRChannel::NF_Find_WorkNode(US f_NID)
{
	UC	lv_ETID = m_cET;								// ��ǰ�¼���
	SEventTable *lv_ET;									// ��ʱ�¼���ָ��
	long lv_Start, lv_End, lv_Mid;
	CIVRGroup *lv_Group = m_objGroup;

	// ȷ�������¼���
	if( lv_ETID == 0 )
	{
		lv_ET = lv_Group->ET_First;
	}
	else
	{
		lv_ET = lv_Group->ET_Second;
	}

	// �����¼�����(���ַ�����)
	lv_Start = 0;
	lv_End = lv_Group->GetRCN(lv_ETID);

	while(lv_Start <= lv_End)
	{
		lv_Mid = (US)((lv_Start + lv_End) / 2);
		if(f_NID == lv_ET[lv_Mid].cStatus) 
		{
			return(lv_Mid);		// Exit Loop
		}
		if(f_NID > lv_ET[lv_Mid].cStatus) 
		{
			lv_Start = lv_Mid + 1;
		}
		else
		{
			lv_End = lv_Mid - 1;
		}
	}

	return -1;
}
//----------------------------------------------------------------------------------------------

// Find node then run it
BOOL CIVRChannel::NF_Find_Run()
{
	if( GetStatus() <= IVR_BLOCK )	// || m_oARS.ivr_channel_getstate(LINESTATUS_HOOKST) == VOICE_LS_HOOKST_ONHOOK )
		return FALSE;

	// Sun updated 2012-05-07
	if( !m_bInboundCall && m_nBeforeHookon <= 0)
		return FALSE;

	UC	lv_ETID = m_cET;								// ��ǰ�¼���
	SEventTable *lv_ET;									// ��ʱ�¼���ָ��
	US lv_cstatus, lv_event;
	CIVRGroup *lv_Group = m_objGroup;
	long lv_Mid;
	US lv_retval;

	// Next Status node
	lv_cstatus = NF_GetNextNode();
	lv_event = NF_GetNodeEvent();

	// Sun added 2012-05-07
	if( m_nBeforeHookon > 0 )
		m_nBeforeHookon--;

	// Sun added 2006-03-05
	if( GetForceNextNode() >= 256 )
	{
		lv_cstatus = GetForceNextNode();
		SetForceNextNode(0);
		ChData.nextnode = lv_cstatus;
	}

	WriteTrace(TraceDebug, "IVR Channel: %d will process event: %d on status: %d, Before Hook On Step: %d", 
		m_intLineNO, lv_event, lv_cstatus, (m_nBeforeHookon > 0 ? theApp.m_nMaxBeforeHookOnSteps - m_nBeforeHookon : 0));

	// ȷ�������¼���
	if( lv_ETID == 0 )
		lv_ET = lv_Group->ET_First;
	else
		lv_ET = lv_Group->ET_Second;

	// �����¼�����(���ַ�����)
	if( (lv_Mid = NF_Find_WorkNode(lv_cstatus)) < 0 )
		return FALSE;

	// Sun added 2002-03-01
	if( lv_event == LE_CALLIN )
	{
		// Add Call Flow initializition code here
		if( m_oARS.m_nDevType == DEV_ANALOG )
		{
			/// Off Hook
			SetHookOn(TELE_HOOKOFF);
		}
		else
		{
			/// Sun added 2003-05-09
			//// Delay call flow
			if( m_nCallAnswerDelay > 0 )
				Sleep(m_nCallAnswerDelay);
		}

		/// Set event to processable event
		if( m_dwBegin == 0 )
		{
			WriteTrace(TraceWarn, "Warning - IVR Channel: %d reset before answered.", m_intLineNO);
			return FALSE;
		}

		/// Sun added 2004-08-24
		//// Maxium Call Length
		if( theApp.m_nMaxCallLength > 0 )
		{
			if( GetDuration() > theApp.m_nMaxCallLength )
			{
				WriteTrace(TraceWarn, "Warning - IVR Channel: %d is busy for too much time.", m_intLineNO);
				return FALSE;
			}
		}

		// Sun added 2006-03-05
		if( lv_Group->GetIsMonitorred() )
			lv_Group->MQ_SendMsg_ChannelStart(m_intLineNO, (char *)NF_GetCallerID());

		// Sun added 2007-05-29
		m_nAnswerTimeOut = 0;
		
		lv_event = LE_PROCEND;
		SetStatus(IVR_RUN);
		PropertyChanged();
	}

	try
	{
		// Sun added 2006-03-05
		if( GetIsMonitorred() )
			MQ_SendMsg_NodeTransfer(lv_cstatus, m_strNodeKeyPress);
		memset(m_strNodeKeyPress, 0x00, sizeof(m_strNodeKeyPress));

		// Sun added 2008-01-12
		m_nCurrentNodeNode = lv_ET[lv_Mid].NodeNo;
		m_tmNodeStartTime = CTime::GetCurrentTime();
		m_dwNodeStartTick = GetTickCount();

	// Sun added 2013-04-12
#ifdef CUCM_MEDIA
		m_oARS.EndPointOpen();
#endif

		switch( lv_ET[lv_Mid].NodeNo )
		{
			case 6:
				lv_retval = NF_006(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;
			case 7:
				lv_retval = NF_007(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;
			case 8:
				lv_retval = NF_008(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;
			case 9:
				lv_retval = NF_009(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;
			case 10:
				lv_retval = NF_010(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;

			// Sun added 2004-12-30
			case 16:
				lv_retval = NF_016(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;
				
			case 17:
				lv_retval = NF_017(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;
			case 18:
				lv_retval = NF_018(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;
			case 19:
				lv_retval = NF_019(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;
			case 20:
				lv_retval = NF_020(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;
			case 21:
				lv_retval = NF_021(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;
			case 22:
				lv_retval = NF_022(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;
			case 23:
				lv_retval = NF_023(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;

			// Sun added 2004-12-30
			case 28:
				lv_retval = NF_028(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;
			
			// Sun added 2008-03-10
			case 30:
				lv_retval = NF_030(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;

			case 40:
				lv_retval = NF_040(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;
			case 41:
				lv_retval = NF_041(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;
			
			case 50:
				lv_retval = NF_050(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;
			case 51:
				lv_retval = NF_051(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;
			
			// Sun added 2006-11-30
			case 55:
				lv_retval = NF_055(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;

			case 60:
				lv_retval = NF_060(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;
			case 61:
				lv_retval = NF_061(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;
			case 62:
				lv_retval = NF_062(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;
			case 63:
				lv_retval = NF_063(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;
			
			case 69:
				lv_retval = NF_069(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;

			// Sun added 2005-07-10
			case 70:
				lv_retval = NF_070(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;
			
			// Sun added 2005-08-15
			case 71:
				lv_retval = NF_071(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;

			case 90:
				lv_retval = NF_090(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;

			// Sun added 2005-05-26
			case 91:
				lv_retval = NF_091(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;
				
			// Sun added 2005-03-16
			case 96:
				lv_retval = NF_096(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;

			case 100:
				lv_retval = NF_100(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;
			case 101:
				lv_retval = NF_101(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;
			case 102:
				lv_retval = NF_102(lv_event, lv_ET[lv_Mid].Data1, lv_ET[lv_Mid].Data2);
				break;
			
			default:
				lv_retval = NODE_RESULT_ERROR;
				break;
		}
	}
	catch(...)
	{
		lv_retval = NODE_RESULT_ERROR;
	}

	return(lv_retval == NODE_RESULT_OK);
}

void CIVRChannel::ResetChData(BOOL f_blnForce)
{
	// Sun added 2011-10-20
	if( !f_blnForce )
	{
		if( !m_blnCDRRecorded )
		{
			WriteTrace(TraceInfo, "IVR Channel: %d can't execute Reset Channel Data before CDR is recorded!", m_intLineNO);
			return;
		}
	}

	/// Sun added 2002-05-22
	m_cCallType = '1';
	m_ctCallInTime = NULL;
	m_dwWaitBegin = 0;
	m_szTransferDest = _T("");
	m_strTaskID = _T("");
	m_blnTaskClosed = TRUE;

	// Sun added 2011-06-25
	m_strRecordTaskID = _T("");

	SetStatus(IVR_IDLE);
	m_ExitCode = VOICE_TM_Dummy;
	m_dwBegin = 0;
	m_dwTickTime = 0;

	// Sun removed 2008-10-27, [D]
	//m_dwQueueTime = 0;

	m_dwWaitAgentAns = 0;
	m_iKeyBufPtr = 0;
	m_bInQueue = false;
	
	// Sun moved 2007-05-29 to "if( !m_blnAnswerTimeOut )"
	//memset(&ChData, 0, sizeof(ChData));

	// Sun updated 2007-05-29
	if( m_nAnswerTimeOut == 0 )
	{
		memset(&ChData, 0, sizeof(ChData));

		m_lngCurCallID = 0;					// Current Call ID

		memset(m_strUserData, 0x00, sizeof(m_strUserData));
		memset(m_strExtData, 0x00, sizeof(m_strExtData));

		m_bytEventSource = 0;
		m_bytCallSource = 0;
		m_intCTIWaitTime = 0;
	}

	// Sun added 2005-03-23
	m_OBEvent.m_nEventID = 0;
	m_OBEvent.Reset();

	// Sun added 2012-07-04
	if( f_blnForce )
	{
		if( m_callrefno != 0 )
		{
			WriteTrace(TraceInfo, "IVR Channel: %d trys to force clear GC callrefid 0x%x!", m_intLineNO, m_callrefno);
			m_callrefno = 0;
		}
	}

	// Sun added 2002-03-01
	m_intSecondParty = 0;				// Second Party
	m_intThirdParty = 0;				// Third Party

	// Sun moved 2007-05-29 to "if( !m_blnAnswerTimeOut )"
	//m_lngCurCallID = 0;					// Current Call ID

	m_lngNewCallID = 0;					// New Call ID
	m_lngCurConfID = 0;					// Current Conference ID
	m_lngNewConfID = 0;					// New Conference ID

	// Sun added 2005-03-22
	m_blnOBResultSended = TRUE;
	m_bytOBResult = _pcC_OB_RESULT_OTHERS;
	m_strOBMsgTelNo = _T("");
	m_nOBPacketID = 0;
	m_nCampaignID = 0;
	m_nTaskID = 0;

	// Sun added 2013-05-20
	m_strGlobalCallRefID = _T("");
	m_strCountryCode = _T("");
	m_strCityCode = _T("");
	m_strPhoneNumber = _T("");
	m_strWorkTicketID = _T("");

	// Sun added 2006-09-15
	m_nOBWaitTime = 0;

	// Sun added 2005-05-26
	m_bOutboundCall = FALSE;
	m_bCTICallOut = FALSE;
	m_blnCCTalking = FALSE;
	m_bytCCDialProgress = 0;
	m_nLinkChannelNo = -1;

	// Sun added 2010-07-22
	m_bConferenced = FALSE;

	m_strExtSN = _T("");
	memset(m_strNodeKeyPress, 0x00, sizeof(m_strNodeKeyPress));
	
	// Sun moved 2007-05-29 to "if( !m_blnAnswerTimeOut )"
	//memset(m_strUserData, 0x00, sizeof(m_strUserData));
	//memset(m_strExtData, 0x00, sizeof(m_strExtData));
	//m_bytEventSource = 0;
	//m_bytCallSource = 0;
	//m_intCTIWaitTime = 0;

	m_nCTITransferAttempt = 0;
	m_nForceNextNode = 0;

	// Sun added 2003-05-08
	m_dwIdleTick = GetTickCount();

	// Sun added 2006-09-18
	m_bInboundCall = FALSE;
	m_bOutboundCall = FALSE;

	//------------------------------------------------------------------------
	// Sun added 2007-01-16
	m_blnIVRTestFlag = FALSE;
	m_blnIVRTestNoCDR = FALSE;
	m_blnIVRTestTime = FALSE;
	//------------------------------------------------------------------------

	// Sun added 2007-07-18
	m_nSwitch_DispatchTimes = 0;
	m_nSwitch_WorkNo = 0;
	m_nSwitch_AgentID = 0;
	m_strSwitch_UserID = _T("");
	m_strSwitch_ExtNo = _T("");
	m_chSwitch_Result = _pcC_OB_RESULT_SUCC;

	// Sun added 2012-05-07
	m_nBeforeHookon = 0;

	// Sun removed 2013-02-18
	// Notify main window to refresh
	//PropertyChanged();

//-----------------------------------------------------------------
// System upgraded 2011-12-01 [Tag: ANTIDECEIT]
#ifdef PICADEF_ENABLE_ANTIDECEIT
	// �Ƿ��ǵ�һ������ת�ƽڵ㣨ͨ���ǻ�ӭ�ʣ�
	m_blnFirstPlayNode = TRUE;
#endif
//-----------------------------------------------------------------

	WriteTrace(TraceDebug, "IVR Channel: %d Reset Channel Data!", m_intLineNO);
}

void CIVRChannel::Destory()
{
	// Sun added 2003-04-29
	m_oARS.m_nTag = 0;

	// Sun added 2002-03-01
	CloseCh();

	StopCh();
}

// Sun added 2010-07-22
BOOL CIVRChannel::GetIsConferenced()
{
	return m_bConferenced;
}

void CIVRChannel::SetIsConferenced(BOOL f_sw)
{
	if( m_bConferenced != f_sw )
		m_bConferenced = f_sw;
}

// Sun added 2007-11-04
UL CIVRChannel::GetChannelTIT()
{ 
	if( m_lngChannelTIT == 0 )
	{
		// û�����κ���
		UL lv_nChannelTIT = 0;
		
		if( m_lngLastTalkEndTick > 0 )
		{
			// ����һ�κ��У������ϴκ��н�������ʱ��
			lv_nChannelTIT = (GetTickCount() - m_lngLastTalkEndTick) / 1000;
		}
		else // ����û�к��У�����ͨ����������ʱ��
			lv_nChannelTIT = GetChannelCreateDuration();

		return lv_nChannelTIT;
	}

	return m_lngChannelTIT;
}

void CIVRChannel::SetBeginTime()
{
	m_dwBegin = GetTickCount();

	// Sun added 2007-11-04
	/// ���ο�ʼ���ϴν���֮ʱ���
	if( m_lngLastTalkEndTick > 0 )
		m_lngChannelTIT =  (m_dwBegin - m_lngLastTalkEndTick) / 1000;

	// Sun added 2008-10-27, [C]
	m_cCallType = '1';
	m_dwQueueTime = GetTickCount();
	// End of Sun added 2008-10-27, [C]

	/// Sun added 2002-05-22
	m_ctCallInTime = CTime::GetCurrentTime();
	memcpy(ChData.callintime, (const char *)m_ctCallInTime.Format("%Y%m%d%H%M%S"), sizeof(ChData.callintime));
}

UL CIVRChannel::GetDuration() 
{
	if( GetStatus() < IVR_RUN )
		return 0;
	else if( m_dwBegin > 0 )
		return((GetTickCount() - m_dwBegin) / 1000);
	
	return 0;
}

// Sun added 2002-03-01
BOOL CIVRChannel::OpenCh()
{
	if( !m_bEnabled )
		return FALSE;

	// Sun added 2006-11-30
	m_oARS.m_blnBindFaxResource = m_objGroup->GetIsBindFaxRes();

	// Sun added 2002-02-28
	m_bOpened = FALSE;
	if( m_oARS.ivr_channel_open(m_szChannelName, m_szDTIName, m_szFaxName) )
		m_bOpened = TRUE;

	return m_bOpened;
}

// Sun added 2002-03-01
BOOL CIVRChannel::CloseCh()
{
	if( !m_bEnabled )
		return FALSE;

	ResetChData();
	if( !m_oARS.ivr_channel_close() )
		return FALSE;

	m_bOpened = FALSE;
	return TRUE;
}

BOOL CIVRChannel::StartCh()
{
	if( !m_bEnabled )
		return FALSE;

	if( !m_bOpened )
		return FALSE;

	// Sun added 2004-12-30
	try
	{
		if( m_pTTSData )
			delete m_pTTSData;
		m_pTTSData = new BYTE[pMainWindow->m_nTTSBufferSize * 1024 + 1];
	}
	catch(...)
	{
	}

	// Sun added 2006-02-26
	ResetYTDCallCount();

	// Sun added 2011-06-15
	WriteTrace(TraceInfo, "StartCh - IVR Channel: %d will be started", m_intLineNO);

	WaitForStartupToComplete();
	
	return TRUE;
}

BOOL CIVRChannel::StopCh()
{
	if( !m_bEnabled )
		return FALSE;

	if( GetCTIAvailble() )
		MQ_SendMsg_DropChannel();

	// Sun added 2010-01-08 [C]
#ifdef CISCO_CCM
	m_oARS.ivr_channel_stop_play();
#endif

	WaitForShutdownToComplete();

	// Sun added 2004-12-30
	try
	{
		if( m_pTTSData )
		{
			delete m_pTTSData;
			m_pTTSData = NULL;
		}
	}
	catch(...)
	{
	}
	
	// Sun added 2011-06-15
	WriteTrace(TraceInfo, "StopCh - IVR Channel: %d be stopped", m_intLineNO);

	return TRUE;
}

long CIVRChannel::CTI_GetGCCRN()
{
	return m_callrefno;
}

long CIVRChannel::CTI_SetGCCRN(long f_CRN)
{
	// Sun removed lock 2012-07-04
	//m_sync.Enter();

	if( m_callrefno != f_CRN )
	{
		m_callrefno = f_CRN;
		WriteTrace(TraceDebug, "IVR Channel: %d set GC CRN: 0x%x", m_intLineNO, f_CRN);
	}
	//m_sync.Leave();

	return 0;
}

void CIVRChannel::CTI_ClearCall()
{
	// Sun added 2008-06-20
	/// Cisco TAPI 4.2 [QIVR080616]
#ifdef CISCO_CCM
	BOOL lv_nNeedRestart = FALSE;

	// ����-�����У�û����ɣ�������ͨ����λ
	if( strlen((char *)ChData.callerid) > 4 && m_oARS.GetConfCallHandle() > 0 && m_chSwitch_Result > _pcC_OB_RESULT_SUCC )
	{
		// ���г��ȴ���4λ
		WriteTrace(TraceDebug, "Debug - IVR Channel: %d - CTI_ClearCall() ANIleng=%d, ConfID=0x%x, Switch Result: %c",  
			m_intLineNO, strlen((char *)ChData.callerid), m_oARS.GetConfCallHandle(), m_chSwitch_Result);

		if( m_chSwitch_Result == _pcC_OB_RESULT_NO_ANS )
		{
			// �ڲ����Խ������Ҫ�������٣�
			/// ���ߺ��룬����ʱ�Ҷϣ�m_chSwitch_Result = _pcC_OB_RESULT_ABANDON����Ϊ���յ�����IDLE��Ϣ
			/// ���ߺ��룬����ʱ�Ҷϣ�m_chSwitch_Result = _pcC_OB_RESULT_NO_ANS����Ϊ���յ�DISC��Ϣ
			lv_nNeedRestart = TRUE;
		}
	}
	
#endif

	m_oARS.ivr_channel_stop();
	m_oARS.ivr_channel_hook(DX_ONHOOK, m_callrefno);

	WriteTrace(TraceDebug, "Debug - IVR Channel: %d finished CTI_ClearCall() callrefno = 0x%x, CallRefID = 0x%x", m_intLineNO, m_callrefno, GetCallRefID());

	// Sun added 2005-06-16
	/// CTI Clear Call
	if( m_bOutboundCall && m_bCTICallOut && GetCallRefID() > 0 )
		MQ_SendMsg_CallClear();

	// Sun added 2012-07-04
	CTI_SetGCCRN(0);

	// Sun added 2008-06-20
	/// Cisco TAPI 4.2 [QIVR080616]
#ifdef CISCO_CCM
	if( lv_nNeedRestart )
	{
		//Sleep(1000);		// Wait DropCall reply finish
		WriteTrace(TraceInfo, "Info - IVR Channel: %d (%s) will restart for releasing consultation call.", GetLineNO(), m_oARS.GetDeviceName());
		m_oARS.ivr_channel_restart();
	}
#endif
}

// Sun added 2005-01-20
void CIVRChannel::CTI_UnsetChannelError()
{
	m_bytStatus = IVR_IDLE;					// ����ֱ�Ӹ�������ֵ
	ResetChData();
	// ע��ͨ��
	if( GetCTIAvailble() )
		MQ_SendMsg_InitChannel();
}

// Sun added 2005-01-20
void CIVRChannel::CTI_SetChannelError()
{
	SetStatus(IVR_ERROR);
	WriteTrace(TraceWarn, "IVR Channel: %d set to error status!!!", m_intLineNO);
	PropertyChanged(SCR_CHANNEL_ITEM_STATUS);

	// IVR Channel Logout
	if( GetCTIAvailble() )
		MQ_SendMsg_DropChannel();
}

BOOL CIVRChannel::CTI_ErrorHandler(BOOL f_bError)
{
	// Reopen Channel
	if( f_bError )
		WriteTrace(TraceWarn, "IVR Channel: %d ErrorHandler(%s, %s)!", m_intLineNO, m_szChannelName, m_szDTIName);

	try
	{
#ifdef INTEL_DIALOGIC

		// Sun updated 2004-12-31
		WriteTrace(TraceInfo, "IVR Channel: %d will close %s in error handler!", m_intLineNO, m_szChannelName);

		// Sun added 2012-07-13
		if( m_bInCallFlow )
		{
			WriteTrace(TraceWarn, "IVR Channel: %d checked call flow thread pending!", m_intLineNO);

			// ǿ����ֹ�̣߳����̻߳���һ��ʱ���Ĭ��15�룩��������
			WaitForShutdownToComplete();
		}

		if( !m_oARS.ivr_channel_close() )
		{
			ResetChData();

			// Mark the Channel as Error
			CTI_SetChannelError();
			return TRUE;
		}

		WriteTrace(TraceInfo, "IVR Channel: %d will open %s in error handler!", m_intLineNO, m_szChannelName);

		ResetChData();
		m_oARS.ivr_channel_open(m_szChannelName, m_szDTIName, m_szFaxName);

		WriteTrace(TraceInfo, "IVR Channel: %d opened %s in error handler!", m_intLineNO, m_szChannelName);

		// Sun added 2007-05-29
		m_nAnswerTimeOut = 0;

#endif

#ifdef CISCO_CCM
		WriteTrace(TraceInfo, "IVR Channel: %d will restart %s in error handler, answer TO times = %d!", m_intLineNO, m_szChannelName, m_nAnswerTimeOut);

		ResetChData();

		//---------------------------------------------
		// Sun added 2010-07-20 [E]
		if( m_bInCallFlow )
		{
			WriteTrace(TraceWarn, "IVR Channel: %d checked call flow thread pending!", m_intLineNO);

			// Sun added 2011-06-24
			/// �������豸�Ƿ�ʱ���޷��򿪣�����̵߳���
			DWORD lv_dwOpenPending = m_oARS.CWaveOutDevice::TestOpenDevWait();
			if( lv_dwOpenPending > 3000 )
			{
				WriteTrace(TraceError, "Error IVR Channel: %d thread pending in waveout open operation, duration = %d!", m_intLineNO, lv_dwOpenPending);

				if( (gb_lngAutoCloseFlag & PCS_PVC_AUTOCLOSE_OPENDEV_WAITTO) == PCS_PVC_AUTOCLOSE_OPENDEV_WAITTO )
				{
					pMainWindow->PostMessage(WM_SYS_AUTO_SHUTDOWN, PCS_PVC_AUTOCLOSE_OPENDEV_WAITTO, m_intLineNO);
				}
				return FALSE;
			}
			/// �������豸�Ƿ�ʱ���޷��رգ�����̵߳���
			DWORD lv_dwClosePending = m_oARS.CWaveOutDevice::TestCloseDevWait();
			if( lv_dwClosePending > 5000 )
			{
				WriteTrace(TraceError, "Error IVR Channel: %d thread pending in waveout close operation, duration = %d!", m_intLineNO, lv_dwClosePending);

				if( (gb_lngAutoCloseFlag & PCS_PVC_AUTOCLOSE_CLOSEDEV_WAITTO) == PCS_PVC_AUTOCLOSE_CLOSEDEV_WAITTO )
				{
					pMainWindow->PostMessage(WM_SYS_AUTO_SHUTDOWN, PCS_PVC_AUTOCLOSE_CLOSEDEV_WAITTO, m_intLineNO);
				}
				return FALSE;
			}

			// Sun added 2010-08-02 [C]
			CTI_ClearCall();

			// ǿ����ֹ�̣߳����̻߳���һ��ʱ���Ĭ��15�룩��������
			WaitForShutdownToComplete();

			// Sun added 2011-05-13
			if( !m_oARS.ivr_channel_restart() )
				return FALSE;
		}
		else if( m_nAnswerTimeOut >= MAX_TRYTIMES )
		//---------------------------------------------
		{
			m_nAnswerTimeOut = 0;
			if( !m_oARS.ivr_channel_restart() )
				return FALSE;
		}
		WriteTrace(TraceInfo, "IVR Channel: %d restarted %s in error handler!", m_intLineNO, m_szChannelName);

		// Sun added 2010-07-16
		PropertyChanged(SCR_CHANNEL_ITEM_STATUS, TRUE);
#endif

#ifdef CISCO_CME
		WriteTrace(TraceInfo, "IVR Channel: %d will restart %s in error handler!", m_intLineNO, m_szChannelName);
		ResetChData();
		WriteTrace(TraceInfo, "IVR Channel: %d restarted %s in error handler!", m_intLineNO, m_szChannelName);

		// Sun added 2007-05-29
		m_nAnswerTimeOut = 0;
#endif
	}
	catch(...)
	{
		WriteTrace(TraceError, "IVR Channel: %d - CTI_ErrorHandler(%s, %s) Error!!!", m_intLineNO, m_szChannelName, m_szDTIName);
	}

	return TRUE;
}

// Send windows user message on codition of attributes changed
void CIVRChannel::PropertyChanged(BYTE nItemChanged, BOOL blnRefresh)
{
	DWORD lv_temptick;
	bool lv_blnSendMsg;

	lv_temptick = GetTickCount();
	if( !theApp.m_blnChannelRichRefresh )
	{
		if( nItemChanged == 255 || nItemChanged == SCR_CHANNEL_ITEM_CTI || nItemChanged == SCR_CHANNEL_ITEM_CHID )
			lv_blnSendMsg = true;
		else
			lv_blnSendMsg = false;
	}
	else
	{
		lv_blnSendMsg = false;
		switch( nItemChanged )
		{
		case 255:
		case SCR_CHANNEL_ITEM_CHID:
		case SCR_CHANNEL_ITEM_CALLERID:
		case SCR_CHANNEL_ITEM_CALLEEID:
		case SCR_CHANNEL_ITEM_CTI:
		case SCR_CHANNEL_ITEM_EDID:
		case SCR_CHANNEL_ITEM_CALLCOUNT:
			lv_blnSendMsg = true;
			break;
		case SCR_CHANNEL_ITEM_CALLLENGTH:
			if( (lv_temptick - m_pticktime) / 1000 > 0 )
			{
				m_pticktime = lv_temptick;
				lv_blnSendMsg = true;
			}
			break;
		case SCR_CHANNEL_ITEM_STATUS:
			if( m_pstatus != GetStatus() || m_pstarted != m_bStarted )
			{
				m_pstatus = GetStatus();
				lv_blnSendMsg = true;
			}
			break;
		}
	}
	if( lv_blnSendMsg || blnRefresh )
	{
		m_pstarted = m_bStarted;
		::PostMessage(pChannelView->m_hWnd, MYWM_CHANGE_CHANNEL, m_intLineNO, m_bytGroupNo * 256 + nItemChanged);
	}
}

// ����ת��
BOOL CIVRChannel::SwitchToStationEx(UC f_type, UL f_workno, bool f_directdail)
{
	CString lv_szNumber;

	if( f_type != 0 && f_workno < 0 )
		return FALSE;
	
	// ��ʱ
	SetTickTime();

	// ����ȴ�ת��״̬
	SetStatus(IVR_W_SWITCH);

	// Sun added 2002-05-22
	m_nSwitch_DispatchTimes++;
	m_cCallType = '2';
	m_dwWaitBegin = GetTickCount() - m_intCTIWaitTime * 1000;			// �ۼ��Ŷ�ʱ��;
	m_dwWaitAgentAns = GetTickCount();

	// Sun added 2002-04-05
	lv_szNumber.Format("%u", f_workno);

	// Sun added 2002-05-22
	m_szTransferDest = lv_szNumber;

	if( !GetCTIAvailble() || f_directdail )
	{
		// Sun updated 2007-06-14
		switch( m_oARS.ivr_channel_divert(lv_szNumber.GetBuffer(lv_szNumber.GetLength()), m_callrefno) )
		{
		case 0:
			SetExitCode(MSI_TM_MSIBUSY);
			break;
		case 1:
			SetExitCode(MSI_TM_ANSWER);
			break;
		default:
			SetExitCode(MSI_TM_ERROR);
		}
	}
	else
	{
		// Ask CTI Service to transfer call
		DWORD lv_CallLength = (GetTickCount() - m_dwBegin) / 1000;
		ChData.calllength = (US)lv_CallLength;

		if( MQ_SendMsg_Singlestep(lv_szNumber) )
			m_cCallType = '3';
	}

	return TRUE;
}

void CIVRChannel::SetExitCode(US f_exitcode)
{ 
	m_ExitCode = f_exitcode;

	// Sun added 2002-05-22
	if( f_exitcode == MSI_TM_ANSWER )
		m_cCallType = '3';

	// Sun added 2002-04-18
	/// To stop Channel
	if( m_ExitCode >= MSI_TM_NOMSI && m_ExitCode < MSI_TM_Dummy )
	{
		if( m_oARS.m_nDevType == DEV_ANALOG )
		{
			if( m_oARS.ivr_channel_getstate(LINESTATUS_HOOKST) == VOICE_LS_HOOKST_OFFHOOK )
				if( m_oARS.ivr_channel_getstate(LINESTATUS_DEVST) == VOICE_LS_DEVST_PLAY )
				{
					SetChannelStop();
					//// Reload Value
					m_ExitCode = f_exitcode;
				}
		}
	}
};

//----------------------------------------------------------------------------------------------
// �ж������ж�
// 0 : �����ж�
// 1 : �����ж�
// 2 : ¼��
// 3 : ����
// 4 : ת����ϯ
// Note: ����ȱʡת�ƹ����ж�
// Return : -1 --- Error Occur
//			0  --- Go On
//			1  --- Default transfer succeed
//          5  --- Try Again
int CIVRChannel::TerminalAssert(UC type, US pnode, UC f_key)
{
	int lv_reco;						// ������������ֵ-�ж�����
	int lv_retval = -1;					// �ڵ�ת�Ʒ���ֵ
	
	switch( type )
	{
	case TERM_TYPE_PLAY:				// Play Voice
		
		// �ж������ж�
		lv_reco = GetExitCode();
		switch(lv_reco)
		{
		case VOICE_TM_MAXDTMF:			// ��󰴼���ֹ
		case VOICE_TM_DIGIT:			// �жϰ�����ֹ
		case VOICE_TM_EOD:				// �������
			// ����
			lv_retval = 0;
			break;

		case VOICE_TM_LCOFF:			// ����
			break;

		case VOICE_TM_USRSTOP:			// Function stopped by user
			// ����
			lv_retval = 1;
			break;

		// Sun added 2008-01-24
		case VOICE_TM_MAXTIME:			// ���ʱ����ֹ
			// ����
			lv_retval = 0;
			break;
		}

		break;

	case TERM_TYPE_GETKEY:				// Get Key

		// �ж������ж�
		lv_reco = GetExitCode();
		switch(lv_reco)
		{
		case VOICE_TM_MAXTIME:			// ���ʱ����ֹ
		case VOICE_TM_IDDTIME:			// ��󰴼����
			// Please try it again
			lv_retval = 5;
			break;

		case VOICE_TM_MAXDTMF:			// ��󰴼���ֹ
			// ����
			lv_retval = 0;
			break;

		case VOICE_TM_DIGIT:			// �жϰ�����ֹ
			
			// ȫ��ת�ƹ���
			lv_retval = NF_TransferDefault(f_key, pnode);

			// ����		
			break;

		case VOICE_TM_USRSTOP:			// Function stopped by user
			// ����
			lv_retval = 1;
			break;
		}

		// Sun added 2001-09-07
		if( lv_retval == 0 )
		{
			// ��¼�˰���
			NF_RecordLastKey(f_key);
		}

		break;

	case TERM_TYPE_RECORD:
	case TERM_TYPE_FAX:
		break;

	case TERM_TYPE_SWITCH:

		// �ж������ж�
		lv_reco = GetExitCode();
		switch(lv_reco)
		{
		case MSI_TM_NOMSI:		// No MSI exist
			lv_retval = 1;
			break;

		case MSI_TM_MSIBUSY:
		case MSI_TM_GROUPBUSY:
		case MSI_TM_ALLMSIBUSY:
			lv_retval = 2;		// Busy
			break;

		case MSI_TM_MSIOK:
		case MSI_TM_GROUPOK:
		case MSI_TM_ANYMSIOK:
			lv_retval = 3;		// Ready
			break;

		case MSI_TM_NOANSWER:
			lv_retval = 4;		// Ready but no answer
			break;

		case MSI_TM_ANSWER:
			lv_retval = 5;		// Answer
			break;
		}

		break;
	}

	return(lv_retval);
}

//----------------------------------------------------------------------------------------------
// ��֤�û�����
// return: -1		error occured
//			0		user legal
//			other	user illegal (present password error location)
int CIVRChannel::CheckUserPassword(UC *f_uid, UC *f_pwd, US f_comid)
{
	HRESULT hr;							// ���Է���ֵ
	VARIANT lv_R;						// COM���÷���ֵ
	UC lv_checkresult = 0;				// �����֤���(0:�ɹ�; ����:����λ��)
	_bstr_t lv_comiid="";				// COM�ӿ�ID
	char filename[MAX_PATH] = "";		// ��Դ·��

	if( !m_oUc.IsCreateObject() )
		return -1;

	// ����COM�����ݿ�
	_bstr_t lv_com_userid = (char *)f_uid;
	_bstr_t lv_com_password = (char *)f_pwd;
	
	lv_comiid = OLESTR("UserLogin");
	if( f_comid > 0 )
	{
		// ȡ����Դ·��
		if(GetResourcePathWithID(f_comid, filename) != NULL)
		{
			// ȡ��COM�ӿ�ID
			lv_comiid = ( const char* )filename;
		}
	}

	try
	{
		VariantInit(&lv_R);
		hr = m_oUc.Invoke(DISPATCH_METHOD, &lv_R, NULL, NULL,
			(BSTR)lv_comiid, TEXT("ss"),
			(BSTR)lv_com_userid,
			(BSTR)lv_com_password);

		if(FAILED(hr))
			return -1;
	}
	catch(_com_error e)
	{
		WriteTrace(TraceError, "IVR Channel: %d Error - Error Msg:%s, Invoke(%s) failed!", 
			m_intLineNO, e.ErrorMessage(), lv_comiid);

		return -1;
	}

	// Check the result
	lv_checkresult = lv_R.bVal;

	return((int)lv_checkresult);
}

//----------------------------------------------------------------------------------------------
// �����û�����
// return: 
//			0		update successfully
//			other	error
int CIVRChannel::UpdateUserPassword(UC *f_uid, UC *f_pwd, US f_comid)
{
	HRESULT hr;							// ���Է���ֵ
	VARIANT lv_R;						// COM���÷���ֵ
	UC lv_checkresult = 0;				// �����֤���(0:�ɹ�; ����:����)
	_bstr_t lv_comiid="";				// COM�ӿ�ID
	char filename[MAX_PATH] = "";		// ��Դ·��

	if( !m_oUc.IsCreateObject() )
		return -1;

	// ����COM�����ݿ�
	_bstr_t lv_com_userid = (char *)f_uid;
	_bstr_t lv_com_password = (char *)f_pwd;
	
	lv_comiid = OLESTR("ChangePassword");
	if( f_comid > 0 )
	{
		// ȡ����Դ·��
		if(GetResourcePathWithID(f_comid, filename) != NULL)
		{
			// ȡ��COM�ӿ�ID
			lv_comiid = ( const char* )filename;
		}
	}

	try
	{
		VariantInit(&lv_R);
		hr = m_oUc.Invoke(DISPATCH_METHOD, &lv_R, NULL, NULL,
			(BSTR)lv_comiid, TEXT("ss"),
			(BSTR)lv_com_userid,
			(BSTR)lv_com_password);
		
		if(FAILED(hr))
			return -1;
	}
	catch(_com_error e)
	{
		WriteTrace(TraceError, "IVR Channel: %d Error - Error Msg:%s, Invoke(%s) failed!", 
			m_intLineNO, e.ErrorMessage(), lv_comiid);

		return -1;
	}
	
	// Check the result
	lv_checkresult = lv_R.bVal;

	return((int)lv_checkresult);
}

// ������������
int CIVRChannel::ReadDigitsEx(double f_digit)
{
	char	lv_digitbuf[MAXMSGLEN] = "";
	int		termcode;				// ��ֹԭ�����
	int		retval = 0;

	// Doulbe => String
	// _gcvt(f_digit, 10, lv_digitbuf);

	// ����ȴ���Ϣ��Ӧ״̬
	SetStatus(IVR_W_ACK);

	// Sun added 2012-11-23
	if( GetIsAsynPlaying() )
	{
#ifdef INTEL_DIALOGIC
		m_oARS.ivr_channel_dx_stop();
#endif
		SetIsAsynPlaying(FALSE);
	}

	retval = m_oARS.ivr_channel_digitplay(f_digit, 0, NF_GetLanguageID());
	
	/// ��ֹԭ�����
	if( (termcode = m_oARS.ivr_channel_gettermmsk(CMESyncEvent_Play)) == -1 )
		return -1;
	else
		SetExitCode(termcode);

	return retval;
}

//----------------------------------------------------------------------------------------------
// ��¼�˰���
void CIVRChannel::NF_RecordLastKey(UC f_key)
{
	// 0 - 63
	if( m_iKeyBufPtr < sizeof(ChData.lastchoice) )
		ChData.lastchoice[m_iKeyBufPtr++] = f_key;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// �������
void CIVRChannel::NF_EraseLastKey(bool f_clearall)
{
	if(	f_clearall )
	{
		m_iKeyBufPtr = 0;
	}
	else
	{
		if( m_iKeyBufPtr > 0 )
			m_iKeyBufPtr--;
	}
	memset(ChData.lastchoice + m_iKeyBufPtr, 0, sizeof(ChData.lastchoice) - m_iKeyBufPtr);		
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// ���ݸ���ID��ȡ��Դ·��
//		����:	f_rid	-	��ԴID
//				strDest	-	��Դ·�������ַ
//		����ֵ:	NULL	-	û���ҵ�
//				����	-	��Դ·��
char *CIVRChannel::GetResourcePathWithID(US f_rid, char *strDest)
{
	int lv_Len = 99;
	long lv_Start, lv_End, lv_Mid;
	CIVRGroup *lv_Group = m_objGroup;
	int lv_LID = NF_GetLanguageID();

	try
	{
		if( lv_Group->m_pResMap[lv_LID] != NULL )
		{
			// ���ַ�����
			lv_Start = 0;
			lv_End = lv_Group->m_lngResRCN[lv_LID];

			while(lv_Start <= lv_End)
			{
				lv_Mid = (US)((lv_Start + lv_End) / 2);
				if(f_rid == (lv_Group->m_pResMap[lv_LID][lv_Mid]).R_ID) 
				{	// �ҵ���! Sun 2001-06-20
					//if( lv_Len > sizeof(strDest) - 1 )
					//	lv_Len = sizeof(strDest) - 1;
					strncpy(strDest, (lv_Group->m_pResMap[lv_LID][lv_Mid]).R_Path, lv_Len);

					// Sun removed 2004-12-30
					//strRTrim(strDest);

					return strDest;
				}
				if(f_rid > (lv_Group->m_pResMap[lv_LID][lv_Mid]).R_ID) 
				{
					lv_Start = lv_Mid + 1;
				}
				else
				{
					lv_End = lv_Mid - 1;
				}
			}
		}
	}
	catch(...)
	{
	}
	
	// û���ҵ�
	return NULL;
}

// Sun added 2007-07-13
/// �����ļ�ʱ�䳤��
US CIVRChannel::GetVoiceFilePlayLength(US f_rid)
{
	US		lv_nPlayLen = 0;
	char	filename[MAX_PATH];
	__int64 lv_iLen;
	CString	szMent;
	char	*strp;

	memset(filename, 0x00, sizeof(filename));
	if(GetResourcePathWithID(f_rid, filename) != NULL)
	{
		// Full File Path
		strp = strrchr(filename, '.');
		if( strp == NULL )
		{
#ifdef INTEL_DIALOGIC
			szMent.Format("%s%s.vox", gb_FN_VoxPath, filename);
#else
			szMent.Format("%s%s.wav", gb_FN_VoxPath, filename);
#endif
		}
		else
		{
			szMent.Format("%s%s", gb_FN_VoxPath, filename);
		}

		// ������������
		if( GetFileLen((LPCSTR)szMent, &lv_iLen) )
		{
			szMent.MakeLower();
			if( szMent.Right(3) == "vox" )
			{
				/// Vox - 4 KB per Second
				lv_nPlayLen = (US)(lv_iLen / 4096 );
			}
			else
			{
				/// Wav - 16 KB per Second
				lv_nPlayLen = (US)(lv_iLen / 16384 );
			}
		}
	}

	return lv_nPlayLen;
}
//----------------------------------------------------------------------------------------------

//==========================================================
// Thread member functions
BOOL CIVRChannel::WaitForStartupToComplete()
{
	m_shutdown.Create();
	m_shutdown.Reset();
	m_callinEvent.Create();
	m_callinEvent.Reset();
	m_hEventArray[0] = m_shutdown.GetHANDLE();			// highest priority
	m_hEventArray[1] = m_callinEvent.GetHANDLE();

	// Sun added 2011-06-24
	if( CMyThread::GetHandle() != 0 )
	{
		CMyThread::CloseThreadHandle();
		WriteTrace(TraceWarn, "IVR Channel: %d - IVR Channel Thread handle is not closed, should be closed before restarting. InCallFlow = %d, COMInited = %d!", 
			m_intLineNO, m_bInCallFlow, m_blnCOMInitialized);
	}

	CMyThread::Start();

	return TRUE;
}

void CIVRChannel::WaitForShutdownToComplete()
{
	// if we havent already started a shut down, do so...
	if( m_shutdown.GetHANDLE() != INVALID_HANDLE_VALUE )
	{
		m_shutdown.Set();

		DWORD lv_dwWaitDur;
		// Sun added 2011-06-27
		if( pMainWindow->m_bSystemAutoShutDown )
			lv_dwWaitDur = 10;
		else
			lv_dwWaitDur = 200;

		if( !Wait(lv_dwWaitDur) )
		{
			WriteTrace(TraceWarn, "IVR Channel: %d - Problem while killing IVR Channel Thread, InCallFlow = %d, COMInited = %d!", 
				m_intLineNO, m_bInCallFlow, m_blnCOMInitialized);
			
			// Sun added 2010-07-20
			try
			{
				TerminateThread(CMyThread::GetHandle(), 0);

				// Sun added 2011-06-15
				// Destory COM
				if( m_bInCallFlow )
				{
					m_oUc.ClearDispatchObject();

					// Sun added 2011-06-16 [B]
					if( m_blnCOMInitialized )
					{
						CoUninitialize();
						m_blnCOMInitialized = FALSE;
					}

					ResetChData(FALSE);
				}
			}
			catch(...)
			{
			}

			// Sun added 2011-06-15
			m_bInCallFlow = FALSE;

			// Sun added 2011-06-15
			SetStarted(FALSE);

			// Sun added 2011-06-24
			if( CMyThread::GetHandle() != 0 )
			{
				CMyThread::CloseThreadHandle();
				WriteTrace(TraceWarn, "IVR Channel: %d - IVR Channel Thread handle was forcely closed!", 
					m_intLineNO);
			}

			// Sun added 2011-06-15
			WriteTrace(TraceWarn, "IVR Channel: %d - Forcely IVR finished processing call flow and IVR thread", m_intLineNO);
		}
	}
}


int CIVRChannel::CallFlowProcess()
{
	// Sun added 2010-07-20 [E]
	m_bInCallFlow = TRUE;

	// Note: initialize user COM object
	try
	{
		// Sun added 2011-06-15 for debug
		WriteTrace(TraceDetail, "IVR Channel: %d - IVR start call flow and begin init COM 1, ComInited = %d", m_intLineNO, m_blnCOMInitialized);

		// Sun added 2011-06-16 [B]
		if( !m_blnCOMInitialized )
		{
			CoInitialize(NULL);
			m_blnCOMInitialized = TRUE;
		}
		else
		{
			WriteTrace(TraceWarn, "Warn IVR Channel: %d - ComInit Status is wrong %d", m_intLineNO, m_blnCOMInitialized);
		}

		// Sun added 2011-06-15 for debug
		WriteTrace(TraceDetail, "IVR Channel: %d - IVR start call flow and begin init COM 2", m_intLineNO);

		CreateComObject(m_objGroup->m_olestrClassID);
	}
	catch(...)
	{
		WriteTrace(TraceError, "Error - IVR Channel: %d CoInitialize() unexpected error!", 
			m_intLineNO);
	}

	// Sun added 2008-02-19
	m_blnNodeLogOff = m_objGroup->m_blnLogSwitchOff;

	WriteTrace(TraceDebug, "IVR Channel: %d - IVR begin to process call flow", m_intLineNO);

	m_callinEvent.Reset();
	while( !m_shutdown.Wait(0) )
	{
		if( WaitForSingleObject(m_shutdown.GetHANDLE(), 0) == WAIT_OBJECT_0 )
			break;

		// Process State Event
		if( !NF_Find_Run() && m_nBeforeHookon <= 0 )
			break;

		// Sun added 2004-05-17 
		/// This is need if Main Thread don't check channels
		PropertyChanged(SCR_CHANNEL_ITEM_CALLLENGTH);
	}

	// Sun added 2011-06-15 for debug
	WriteTrace(TraceDetail, "IVR Channel: %d - IVR finished call flow and begin Destory COM 1", m_intLineNO);

	// Destory COM
	m_oUc.ClearDispatchObject();

	// Sun added 2011-06-15 for debug
	WriteTrace(TraceDetail, "IVR Channel: %d - IVR finished call flow and begin Destory COM 2, ComInited = %d", m_intLineNO, m_blnCOMInitialized);

	SetHookOn();

	// Sun added 2011-06-15 for debug
	WriteTrace(TraceDetail, "IVR Channel: %d - IVR finished call flow and begin Destory COM 3, ComInited = %d", m_intLineNO, m_blnCOMInitialized);

	// Sun updated 2011-06-16 [B]
	if( m_blnCOMInitialized )
	{
		CoUninitialize();
		m_blnCOMInitialized = FALSE;
	}

	// Sun added 2011-10-20
	int lv_nDur = 0;
	while( lv_nDur++ < 200 )
	{
		// Sun updated 2012-07-04
		/// From
		//if( m_blnCDRRecorded )
		/// To
		if( m_blnCDRRecorded && CTI_GetGCCRN() == 0 )
			break;

		Sleep(20);
	}

	// Sun updated 2011-07-03
	/// From
	/*
	// Sun added 2005-07-28
	/// To reset channel status from switch to idle
	if( m_bytStatus == IVR_W_SWITCH )
		ResetChData(FALSE);
	*/
	/// To
	ResetChData(FALSE);

	// Sun added 2010-07-20 [E]
	m_bInCallFlow = FALSE;

	// Sun added 2012-11-23
	if( GetIsAsynPlaying() )
	{
#ifdef INTEL_DIALOGIC
		m_oARS.ivr_channel_dx_stop();
#endif
		SetIsAsynPlaying(FALSE);
	}

	// Sun added 2013-02-18
	PropertyChanged();

	WriteTrace(TraceDebug, "IVR Channel: %d - IVR finished processing call flow", m_intLineNO);

	return 1;
}

int CIVRChannel::Run()
{
	DWORD Event = -1;
	DWORD times;

	// Sun moved 2011-06-16 from [Mark_20110616_001]
	//CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	WriteTrace(TraceInfo, "IVR Channel: %d - IVR thread started", m_intLineNO);
	SetStarted(TRUE);

	while( !m_shutdown.Wait(0) )
	{
		try
		{
			Event = WaitForMultipleObjects(2, m_hEventArray, FALSE, 0);
			if( Event == WAIT_OBJECT_0 )
				break;
			else if( Event == WAIT_OBJECT_0 + 1 )
			{
				if( !CallFlowProcess() )
					break;
			}
			// Sun added 2009-08-26
			else if( m_bytStatus == IVR_RUN )
			{
				times = GetTickCount() - GetTickTime();
				if( times > 3000 )
				{
					WriteTrace(TraceWarn, "Warn - IVR Channel: %d thread is running in error status: %d", m_intLineNO, m_bytStatus);
					ResetChData();
				}
			}

			Sleep(50);
		}
		catch(...)
		{
			WriteTrace(TraceError, "CIVRChannel::Run() - Unexpected exception, IVR Channel: %d", m_intLineNO);
		}
	}

	// Sun moved 2011-06-16 from [Mark_20110616_002]
	//CoUninitialize();

	SetStarted(FALSE);
	WriteTrace(TraceInfo, "IVR Channel: %d - IVR thread stoped", m_intLineNO);
	return 0;
}

BOOL CIVRChannel::MQ_SendMsg_InitChannel()
{
	BOOL lv_retval;

	try
	{
		SMsgHeader lv_sndhdr;
		S_TCP_IVR2PCS_CH_OPEN lv_sndbdy;
		BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

		lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
		lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
		lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
		lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
		lv_sndhdr.Sender = USER_IVR;
		lv_sndhdr.Receiver = USER_CTI;

		memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));
		lv_sndhdr.Command = TCP_IVR2PCS_CH_OPEN;
		lv_sndhdr.PackageLen = sizeof(lv_sndhdr) + sizeof(lv_sndbdy);

		lv_sndbdy.AgentID = atol(m_sDeviceNo);

		// Send Message
		memcpy(lv_sndmsg, &lv_sndhdr, sizeof(lv_sndhdr));
		memcpy(lv_sndmsg+sizeof(lv_sndhdr), &lv_sndbdy, sizeof(lv_sndbdy));

		// Sun updated 2010-05-18 [V7.1.8.3]
		lv_retval = pMainWindow->MQ_HA_SendMsg(lv_sndmsg, lv_sndhdr.PackageLen, TRUE);
	}
	catch(...)
	{
		lv_retval = FALSE;
		WriteTrace(TraceError, "IVR Channel: %d failed when call MQ_SendMsg_InitChannel(%s)", m_intLineNO, m_sDeviceNo);
	}

	return lv_retval;
}

BOOL CIVRChannel::MQ_SendMsg_DropChannel()
{
	BOOL lv_retval;

	try
	{
		SMsgHeader lv_sndhdr;
		S_TCP_IVR2PCS_CH_CLOSE lv_sndbdy;
		BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

		lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
		lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
		lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
		lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
		lv_sndhdr.Sender = USER_IVR;
		lv_sndhdr.Receiver = USER_CTI;

		memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));
		lv_sndhdr.Command = TCP_IVR2PCS_CH_CLOSE;
		lv_sndhdr.PackageLen = sizeof(lv_sndhdr) + sizeof(lv_sndbdy);

		lv_sndbdy.AgentID = atol(m_sDeviceNo);

		// Send Message
		memcpy(lv_sndmsg, &lv_sndhdr, sizeof(lv_sndhdr));
		memcpy(lv_sndmsg+sizeof(lv_sndhdr), &lv_sndbdy, sizeof(lv_sndbdy));

		// Sun updated 2010-05-18 [V7.1.8.3]
		lv_retval = pMainWindow->MQ_HA_SendMsg(lv_sndmsg, lv_sndhdr.PackageLen, TRUE);
	}
	catch(...)
	{
		lv_retval = FALSE;
		WriteTrace(TraceError, "IVR Channel: %d failed when call MQ_SendMsg_DropChannel(%s)", m_intLineNO, m_sDeviceNo);
	}

	return lv_retval;
}

BOOL CIVRChannel::MQ_SendMsg_SetCallWaiting(LPCSTR strDN, BOOL blnOnOff)
{
	BOOL lv_retval;

	try
	{
		SMsgHeader lv_sndhdr;
		S_TCP_IVR2PCS_CALL_MSGWAITING lv_sndbdy;
		BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

		lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
		lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
		lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
		lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
		lv_sndhdr.Sender = USER_IVR;
		lv_sndhdr.Receiver = USER_CTI;

		memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));
		lv_sndhdr.Command = TCP_IVR2PCS_SET_MSGWAITING;
		lv_sndhdr.PackageLen = sizeof(lv_sndhdr) + sizeof(lv_sndbdy);

		lv_sndbdy.AgentID = atol(m_sDeviceNo);
		strncpy(lv_sndbdy.strDN, strDN, sizeof(lv_sndbdy.strDN)-1);
		if( blnOnOff )
			lv_sndbdy.SetMWADOn = PCS_DEF_CHOICE_YES;
		else
			lv_sndbdy.SetMWADOn = PCS_DEF_CHOICE_NO;

		// Send Message
		memcpy(lv_sndmsg, &lv_sndhdr, sizeof(lv_sndhdr));
		memcpy(lv_sndmsg+sizeof(lv_sndhdr), &lv_sndbdy, sizeof(lv_sndbdy));

		// Sun updated 2010-05-18 [V7.1.8.3]
		lv_retval = pMainWindow->MQ_HA_SendMsg(lv_sndmsg, lv_sndhdr.PackageLen);
	}
	catch(...)
	{
		lv_retval = FALSE;
		WriteTrace(TraceError, "IVR Channel: %d failed when call MQ_SendMsg_SetCallWaiting(%s)", m_intLineNO, m_sDeviceNo);
	}

	return lv_retval;
}

// Sun added 2012-04-18 [B]
BOOL CIVRChannel::MQ_SendMsg_CallConsult(UC bytMode, LPCSTR strDestNumber)
{
	BOOL lv_retval;

	try
	{
		SMsgHeader lv_sndhdr;
		S_TCP_IVR2PCS_CALL_CONSULTATION lv_sndbdy;
		BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

		lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
		lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
		lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
		lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
		lv_sndhdr.Sender = USER_IVR;
		lv_sndhdr.Receiver = USER_CTI;

		memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));
		lv_sndhdr.Command = TCP_IVR2PCS_CALL_CONSULTATION;
		lv_sndhdr.PackageLen = sizeof(lv_sndhdr) + sizeof(lv_sndbdy);

		lv_sndbdy.AgentID = atol(m_sDeviceNo);
		//lv_sndbdy.CallRefID, lv_sndbdy.EventSource User Default Value
		lv_sndbdy.ConsultMode = bytMode;
		lv_sndbdy.CallScale = (UC)_pcC_CALLSCALE_INTERNAL;
		strcpy(lv_sndbdy.strCalledParty, strDestNumber);

		// Orgnize App Data & User Data
		FillupCallBindedData(lv_sndbdy.strAppData, lv_sndbdy.strUserData);

		// Send Message
		memcpy(lv_sndmsg, &lv_sndhdr, sizeof(lv_sndhdr));
		memcpy(lv_sndmsg+sizeof(lv_sndhdr), &lv_sndbdy, sizeof(lv_sndbdy));

		// Sun updated 2010-05-18 [V7.1.8.3]
		lv_retval = pMainWindow->MQ_HA_SendMsg(lv_sndmsg, lv_sndhdr.PackageLen);
	}
	catch(...)
	{
		lv_retval = FALSE;
		WriteTrace(TraceError, "IVR Channel: %d failed when call MQ_SendMsg_CallConsult(%s)", m_intLineNO, strDestNumber);
	}

	return lv_retval;
}

// Sun added 2012-04-18 [B]
BOOL CIVRChannel::MQ_SendMsg_RetrieveCall()
{
	BOOL lv_retval;

	try
	{
		SMsgHeader lv_sndhdr;
		S_TCP_IVR2PCS_CALL_RETRIEVECALL lv_sndbdy;
		BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

		lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
		lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
		lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
		lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
		lv_sndhdr.Sender = USER_IVR;
		lv_sndhdr.Receiver = USER_CTI;

		memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));
		lv_sndhdr.Command = TCP_IVR2PCS_CALL_RETRIEVECALL;
		lv_sndhdr.PackageLen = sizeof(lv_sndhdr) + sizeof(lv_sndbdy);

		lv_sndbdy.AgentID = atol(m_sDeviceNo);
		//lv_sndbdy.CallRefID, lv_sndbdy.EventSource User Default Value

		// Send Message
		memcpy(lv_sndmsg, &lv_sndhdr, sizeof(lv_sndhdr));
		memcpy(lv_sndmsg+sizeof(lv_sndhdr), &lv_sndbdy, sizeof(lv_sndbdy));

		// Sun updated 2010-05-18 [V7.1.8.3]
		lv_retval = pMainWindow->MQ_HA_SendMsg(lv_sndmsg, lv_sndhdr.PackageLen);
	}
	catch(...)
	{
		lv_retval = FALSE;
		WriteTrace(TraceError, "IVR Channel: %d failed when call MQ_SendMsg_RetrieveCall()", m_intLineNO);
	}

	return lv_retval;
}

// Sun added 2012-04-18 [B]
BOOL CIVRChannel::MQ_SendMsg_FinishConference()
{
	BOOL lv_retval;

	try
	{
		SMsgHeader lv_sndhdr;
		S_TCP_IVR2PCS_CALL_CONFERENCE lv_sndbdy;
		BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

		lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
		lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
		lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
		lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
		lv_sndhdr.Sender = USER_IVR;
		lv_sndhdr.Receiver = USER_CTI;

		memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));
		lv_sndhdr.Command = TCP_IVR2PCS_CALL_CONFERENCE;
		lv_sndhdr.PackageLen = sizeof(lv_sndhdr) + sizeof(lv_sndbdy);

		lv_sndbdy.AgentID = atol(m_sDeviceNo);
		//lv_sndbdy.CallRefID, lv_sndbdy.EventSource User Default Value

		// Send Message
		memcpy(lv_sndmsg, &lv_sndhdr, sizeof(lv_sndhdr));
		memcpy(lv_sndmsg+sizeof(lv_sndhdr), &lv_sndbdy, sizeof(lv_sndbdy));

		// Sun updated 2010-05-18 [V7.1.8.3]
		lv_retval = pMainWindow->MQ_HA_SendMsg(lv_sndmsg, lv_sndhdr.PackageLen);
	}
	catch(...)
	{
		lv_retval = FALSE;
		WriteTrace(TraceError, "IVR Channel: %d failed when call MQ_SendMsg_FinishConference()", m_intLineNO);
	}

	return lv_retval;
}

BOOL CIVRChannel::MQ_SendMsg_Singlestep(LPCSTR strDestNumber)
{
	BOOL lv_retval;

	try
	{
		SMsgHeader lv_sndhdr;
		S_TCP_IVR2PCS_CALL_SINGLESTEPTRANS lv_sndbdy;
		BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

		lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
		lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
		lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
		lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
		lv_sndhdr.Sender = USER_IVR;
		lv_sndhdr.Receiver = USER_CTI;

		memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));
		lv_sndhdr.Command = TCP_IVR2PCS_CALL_SINGLESTEPTRANS;
		lv_sndhdr.PackageLen = sizeof(lv_sndhdr) + sizeof(lv_sndbdy);

		lv_sndbdy.AgentID = atol(m_sDeviceNo);
		//lv_sndbdy.CallRefID, lv_sndbdy.EventSource User Default Value
		lv_sndbdy.CallScale = (UC)_pcC_CALLSCALE_INTERNAL;
		strcpy(lv_sndbdy.strCalledParty, strDestNumber);

		// Orgnize App Data & User Data
		FillupCallBindedData(lv_sndbdy.strAppData, lv_sndbdy.strUserData);

		// Send Message
		memcpy(lv_sndmsg, &lv_sndhdr, sizeof(lv_sndhdr));
		memcpy(lv_sndmsg+sizeof(lv_sndhdr), &lv_sndbdy, sizeof(lv_sndbdy));

		// Sun updated 2010-05-18 [V7.1.8.3]
		lv_retval = pMainWindow->MQ_HA_SendMsg(lv_sndmsg, lv_sndhdr.PackageLen);
	}
	catch(...)
	{
		lv_retval = FALSE;
		WriteTrace(TraceError, "IVR Channel: %d failed when call MQ_SendMsg_Singlestep(%s)", m_intLineNO, strDestNumber);
	}

	return lv_retval;
}

// Sun added 2005-03-24
BOOL CIVRChannel::MQ_SendMsg_CallOut(LPCSTR strDestNumber)
{
	BOOL lv_retval;

	try
	{
		SMsgHeader lv_sndhdr;
		S_TCP_IVR2PCS_CALL_CALLOUT lv_sndbdy;
		BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

		lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
		lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
		lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
		lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
		lv_sndhdr.Sender = USER_IVR;
		lv_sndhdr.Receiver = USER_CTI;

		memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));
		lv_sndhdr.Command = TCP_IVR2PCS_CALL_CALLOUT;
		lv_sndhdr.PackageLen = sizeof(lv_sndhdr) + sizeof(lv_sndbdy);

		lv_sndbdy.AgentID = atol(m_sDeviceNo);
		lv_sndbdy.CallScale = (UC)_pcC_CALLSCALE_LOCAL;
		strcpy(lv_sndbdy.strCalledParty, strDestNumber);

		// Orgnize App Data & User Data
		FillupCallBindedData(lv_sndbdy.strAppData, lv_sndbdy.strUserData);

		// Send Message
		memcpy(lv_sndmsg, &lv_sndhdr, sizeof(lv_sndhdr));
		memcpy(lv_sndmsg+sizeof(lv_sndhdr), &lv_sndbdy, sizeof(lv_sndbdy));

		// Sun updated 2010-05-18 [V7.1.8.3]
		lv_retval = pMainWindow->MQ_HA_SendMsg(lv_sndmsg, lv_sndhdr.PackageLen);
	}
	catch(...)
	{
		lv_retval = FALSE;
		WriteTrace(TraceError, "IVR Channel: %d failed when call MQ_SendMsg_CallOut(%s)", m_intLineNO, strDestNumber);
	}

	return lv_retval;
}

BOOL CIVRChannel::MQ_SendMsg_CallClear()
{
	BOOL lv_retval;

	try
	{
		SMsgHeader lv_sndhdr;
		S_TCP_IVR2PCS_CALL_CLEARCALL lv_sndbdy;
		BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

		lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
		lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
		lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
		lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
		lv_sndhdr.Sender = USER_IVR;
		lv_sndhdr.Receiver = USER_CTI;

		memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));
		lv_sndhdr.Command = TCP_IVR2PCS_CALL_CLEARCALL;
		lv_sndhdr.PackageLen = sizeof(lv_sndhdr) + sizeof(lv_sndbdy);

		lv_sndbdy.AgentID = atol(m_sDeviceNo);
		lv_sndbdy.CallRefID = 0;
		lv_sndbdy.EventSource = 0;

		// Send Message
		memcpy(lv_sndmsg, &lv_sndhdr, sizeof(lv_sndhdr));
		memcpy(lv_sndmsg+sizeof(lv_sndhdr), &lv_sndbdy, sizeof(lv_sndbdy));

		// Sun updated 2010-05-18 [V7.1.8.3]
		lv_retval = pMainWindow->MQ_HA_SendMsg(lv_sndmsg, lv_sndhdr.PackageLen);
	}
	catch(...)
	{
		lv_retval = FALSE;
		WriteTrace(TraceError, "IVR Channel: %d failed when call MQ_SendMsg_CallClear()", m_intLineNO);
	}

	return lv_retval;
}

// Sun added 2007-04-15
BOOL CIVRChannel::MQ_SendMsg_QueryQueuePosition(US f_nRoutePointID)
{
	BOOL lv_retval;

	try
	{
		SMsgHeader lv_sndhdr;
		S_TCP_IVR2PCS_GET_CALL_EWT lv_sndbdy;
		BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

		lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
		lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
		lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
		lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
		lv_sndhdr.Sender = USER_IVR;
		lv_sndhdr.Receiver = USER_CTI;

		memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));
		lv_sndhdr.Command = TCP_IVR2PCS_GET_CALL_EWT;
		lv_sndhdr.PackageLen = sizeof(lv_sndhdr) + sizeof(lv_sndbdy);

		m_QueryEvent.m_nEventID = GET_INVOKE_ID;
		lv_sndbdy.InvokeID = m_QueryEvent.m_nEventID;
		lv_sndbdy.ChannelID = m_intLineNO;
		lv_sndbdy.AgentID = atol(m_sDeviceNo);
		lv_sndbdy.CallRefID = 0;

		// Send Message
		memcpy(lv_sndmsg, &lv_sndhdr, sizeof(lv_sndhdr));
		memcpy(lv_sndmsg+sizeof(lv_sndhdr), &lv_sndbdy, sizeof(lv_sndbdy));

		// Sun updated 2010-05-18 [V7.1.8.3]
		lv_retval = pMainWindow->MQ_HA_SendMsg(lv_sndmsg, lv_sndhdr.PackageLen);
	}
	catch(...)
	{
		lv_retval = FALSE;
		WriteTrace(TraceError, "IVR Channel: %d failed when call MQ_SendMsg_QueryQueuePosition(%d)", m_intLineNO, f_nRoutePointID);
	}

	return lv_retval;
}

// Sun added 2005-08-15
BOOL CIVRChannel::MQ_SendMsg_QueryAgentStatus(UC f_nType, UL f_nAgentID, LPCSTR f_strUserID)
{
	BOOL lv_retval;

	try
	{
		SMsgHeader lv_sndhdr;
		S_TCP_IVR2PCS_GET_AGENTSTATUS lv_sndbdy;
		BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

		lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
		lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
		lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
		lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
		lv_sndhdr.Sender = USER_IVR;
		lv_sndhdr.Receiver = USER_CTI;

		memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));
		lv_sndhdr.Command = TCP_IVR2PCS_GET_AGENTSTATUS;
		lv_sndhdr.PackageLen = sizeof(lv_sndhdr) + sizeof(lv_sndbdy);

		m_QueryEvent.m_nEventID = GET_INVOKE_ID;
		lv_sndbdy.InvokeID = m_QueryEvent.m_nEventID;
		lv_sndbdy.ChannelID = m_intLineNO;
		lv_sndbdy.UserQuery = f_nType;
		lv_sndbdy.AgentID = f_nAgentID;
		strncpy(lv_sndbdy.strAliasName, f_strUserID, sizeof(lv_sndbdy.strAliasName) - 1);

		// Send Message
		memcpy(lv_sndmsg, &lv_sndhdr, sizeof(lv_sndhdr));
		memcpy(lv_sndmsg+sizeof(lv_sndhdr), &lv_sndbdy, sizeof(lv_sndbdy));

		// Sun updated 2010-05-18 [V7.1.8.3]
		lv_retval = pMainWindow->MQ_HA_SendMsg(lv_sndmsg, lv_sndhdr.PackageLen);
	}
	catch(...)
	{
		lv_retval = FALSE;
		WriteTrace(TraceError, "IVR Channel: %d failed when call MQ_SendMsg_QueryAgentStatus(%d, %d, %s)", m_intLineNO, f_nType, f_nAgentID, f_strUserID);
	}

	return lv_retval;
}

// Sun added 2005-07-10, and updated 2005-08-15
BOOL CIVRChannel::MQ_SendMsg_QueryRoutePoint(UC f_nType, US f_nGroupID)
{
	BOOL lv_retval;

	try
	{
		SMsgHeader lv_sndhdr;
		S_TCP_IVR2PCS_GET_RPTSTATUS lv_sndbdy;
		BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

		lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
		lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
		lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
		lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
		lv_sndhdr.Sender = USER_IVR;
		lv_sndhdr.Receiver = USER_CTI;

		memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));
		lv_sndhdr.Command = TCP_IVR2PCS_GET_RPTSTATUS;
		lv_sndhdr.PackageLen = sizeof(lv_sndhdr) + sizeof(lv_sndbdy);

		m_QueryEvent.m_nEventID = GET_INVOKE_ID;
		lv_sndbdy.InvokeID = m_QueryEvent.m_nEventID;
		lv_sndbdy.ChannelID = m_intLineNO;
		lv_sndbdy.nType = f_nType;
		lv_sndbdy.nGroupID = f_nGroupID;

		// Send Message
		memcpy(lv_sndmsg, &lv_sndhdr, sizeof(lv_sndhdr));
		memcpy(lv_sndmsg+sizeof(lv_sndhdr), &lv_sndbdy, sizeof(lv_sndbdy));

		// Sun updated 2010-05-18 [V7.1.8.3]
		lv_retval = pMainWindow->MQ_HA_SendMsg(lv_sndmsg, lv_sndhdr.PackageLen);
	}
	catch(...)
	{
		lv_retval = FALSE;
		WriteTrace(TraceError, "IVR Channel: %d failed when call MQ_SendMsg_QueryRoutePoint(%d, %d)", m_intLineNO, f_nType, f_nGroupID);
	}

	return lv_retval;
}

// Sun added 2006-03-05
BOOL CIVRChannel::MQ_SendMsg_StatusReport()
{
	BOOL lv_retval;

	try
	{
		SMsgHeader lv_sndhdr;
		S_TCP_IVR2ADM_CH_ST_REPORT lv_sndbdy;
		BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

		lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
		lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
		lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
		lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
		lv_sndhdr.Sender = USER_IVR;
		lv_sndhdr.Receiver = USER_CTI;

		memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));
		lv_sndhdr.Command = TCP_IVR2ADM_CH_ST_REPORT;
		lv_sndhdr.PackageLen = sizeof(lv_sndhdr) + sizeof(lv_sndbdy);

		lv_sndbdy.GroupID = m_bytGroupNo;
		lv_sndbdy.ChannelID = m_intLineNO;
		lv_sndbdy.NodeID = NF_GetNextNode();
		lv_sndbdy.DailyCallCount = GetDailyCallCount();
		lv_sndbdy.YTDCallCount = GetYTDCallCount();

		// Send Message
		memcpy(lv_sndmsg, &lv_sndhdr, sizeof(lv_sndhdr));
		memcpy(lv_sndmsg+sizeof(lv_sndhdr), &lv_sndbdy, sizeof(lv_sndbdy));

		// Sun updated 2010-05-18 [V7.1.8.3]
		lv_retval = pMainWindow->MQ_HA_SendMsg(lv_sndmsg, lv_sndhdr.PackageLen);
	}
	catch(...)
	{
		lv_retval = FALSE;
		WriteTrace(TraceError, "IVR Channel: %d failed when call MQ_SendMsg_StatusReport()", m_intLineNO);
	}

	return lv_retval;
}

// Sun added 2006-03-05
BOOL CIVRChannel::MQ_SendMsg_NodeTransfer(US f_NodeID, LPCSTR f_sKeyPress)
{
	BOOL lv_retval;

	try
	{
		SMsgHeader lv_sndhdr;
		S_TCP_IVR2ADM_NODE_TRANSFER lv_sndbdy;
		BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

		lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
		lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
		lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
		lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
		lv_sndhdr.Sender = USER_IVR;
		lv_sndhdr.Receiver = USER_CTI;

		memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));
		lv_sndhdr.Command = TCP_IVR2ADM_NODE_TRANSFER;
		lv_sndhdr.PackageLen = sizeof(lv_sndhdr) + sizeof(lv_sndbdy);

		lv_sndbdy.GroupID = m_bytGroupNo;
		lv_sndbdy.ChannelID = m_intLineNO;
		lv_sndbdy.NodeID = f_NodeID;
		strcpy(lv_sndbdy.KeyPress, f_sKeyPress);

		// Send Message
		memcpy(lv_sndmsg, &lv_sndhdr, sizeof(lv_sndhdr));
		memcpy(lv_sndmsg+sizeof(lv_sndhdr), &lv_sndbdy, sizeof(lv_sndbdy));

		// Sun updated 2010-05-18 [V7.1.8.3]
		lv_retval = pMainWindow->MQ_HA_SendMsg(lv_sndmsg, lv_sndhdr.PackageLen);
	}
	catch(...)
	{
		lv_retval = FALSE;
		WriteTrace(TraceError, "IVR Channel: %d failed when call MQ_SendMsg_NodeTransfer(%d, %s)", m_intLineNO, f_NodeID, f_sKeyPress);
	}

	return lv_retval;
}

// Sun added 2007-07-12
BOOL CIVRChannel::MQ_SendMsg_AddQueueTask(UL nRoutePointID)
{
	BOOL lv_retval;

	try
	{
		SMsgHeader lv_sndhdr;
		S_TCP_UMS2PCS_ADDTASK_REQ lv_sndbdy;
		BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

		lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
		lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
		lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
		lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
		lv_sndhdr.Sender = USER_IVR;
		lv_sndhdr.Receiver = USER_CTI;

		m_strTaskID = _T("");
		memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));
		lv_sndhdr.Command = TCP_UMS2PCS_ADDTASK_REQ;
		lv_sndhdr.PackageLen = sizeof(lv_sndhdr) + sizeof(lv_sndbdy);

		//lv_sndbdy.InvokeID = GET_INVOKE_ID;
		lv_sndbdy.InvokeID = GetCallRefID();

		lv_sndbdy.TaskID = atol(m_sDeviceNo);
		lv_sndbdy.CustID = m_objGroup->GetCustID();			// Sun updated 2012-09-08
		strncpy(lv_sndbdy.strCallingParty, (char *)ChData.callerid, sizeof(lv_sndbdy.strCallingParty) - 1);
		strncpy(lv_sndbdy.strCalledParty, (char *)ChData.calleeid, sizeof(lv_sndbdy.strCalledParty) - 1);
		lv_sndbdy.RoutePointID = (US)nRoutePointID;
		lv_sndbdy.WaitTime = m_intCTIWaitTime;
		lv_sndbdy.ServiceCode0 = ChData.mainservice;

		// Orgnize App Data & User Data
		FillupCallBindedData(lv_sndbdy.strAppData, lv_sndbdy.strUserData);

		// Send Message
		memcpy(lv_sndmsg, &lv_sndhdr, sizeof(lv_sndhdr));
		memcpy(lv_sndmsg+sizeof(lv_sndhdr), &lv_sndbdy, sizeof(lv_sndbdy));

		// Sun updated 2010-05-18 [V7.1.8.3]
		lv_retval = pMainWindow->MQ_HA_SendMsg(lv_sndmsg, lv_sndhdr.PackageLen);
	}
	catch(...)
	{
		lv_retval = FALSE;
		WriteTrace(TraceError, "IVR Channel: %d failed when call MQ_SendMsg_AddQueueTask(%d)", m_intLineNO, nRoutePointID);
	}

	return lv_retval;
}

BOOL CIVRChannel::MQ_SendMsg_RemoveQueueTask(LPCSTR strTaskID, UL nRoutePointID, UL nReasonCode)
{
	BOOL lv_retval;

	if( strTaskID[0] == NULL )
		return FALSE;

	try
	{
		SMsgHeader lv_sndhdr;
		S_TCP_UMS2PCS_REMOVETASK_REQ lv_sndbdy;
		BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

		lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
		lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
		lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
		lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
		lv_sndhdr.Sender = USER_IVR;
		lv_sndhdr.Receiver = USER_CTI;

		memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));
		lv_sndhdr.Command = TCP_UMS2PCS_REMOVETASK_REQ;
		lv_sndhdr.PackageLen = sizeof(lv_sndhdr) + sizeof(lv_sndbdy);

		//lv_sndbdy.InvokeID = GET_INVOKE_ID;
		lv_sndbdy.InvokeID = GetCallRefID();

		lv_sndbdy.TaskID = atol(m_sDeviceNo);
		strncpy(lv_sndbdy.strUMSTaskID,strTaskID, PCS_DEF_LEN_GCREF);
		lv_sndbdy.RoutePointID = (US)nRoutePointID;
		lv_sndbdy.ReasonCode = (US)nReasonCode;

		// Orgnize App Data & User Data
		FillupCallBindedData(lv_sndbdy.strAppData, lv_sndbdy.strUserData);

		// Send Message
		memcpy(lv_sndmsg, &lv_sndhdr, sizeof(lv_sndhdr));
		memcpy(lv_sndmsg+sizeof(lv_sndhdr), &lv_sndbdy, sizeof(lv_sndbdy));

		// Sun updated 2010-05-18 [V7.1.8.3]
		lv_retval = pMainWindow->MQ_HA_SendMsg(lv_sndmsg, lv_sndhdr.PackageLen);
	}
	catch(...)
	{
		lv_retval = FALSE;
		WriteTrace(TraceError, "IVR Channel: %d failed when call MQ_SendMsg_RemoveQueueTask(%s, %d, %d)", 
			m_intLineNO, strTaskID, nRoutePointID, nReasonCode);
	}

	return lv_retval;
}

//  Sun added 2008-05-16
BOOL CIVRChannel::MQ_SendMsg_PauseQueueTask(LPCSTR strTaskID, UL nRoutePointID, UL nReasonCode)
{
	BOOL lv_retval;

	if( strTaskID[0] == NULL )
		return FALSE;

	try
	{
		SMsgHeader lv_sndhdr;
		S_TCP_UMS2PCS_PAUSETASK_REQ lv_sndbdy;
		BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

		lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
		lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
		lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
		lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
		lv_sndhdr.Sender = USER_IVR;
		lv_sndhdr.Receiver = USER_CTI;

		memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));
		lv_sndhdr.Command = TCP_UMS2PCS_PAUSETASK_REQ;
		lv_sndhdr.PackageLen = sizeof(lv_sndhdr) + sizeof(lv_sndbdy);

		//lv_sndbdy.InvokeID = GET_INVOKE_ID;
		lv_sndbdy.InvokeID = GetCallRefID();

		lv_sndbdy.TaskID = atol(m_sDeviceNo);
		strncpy(lv_sndbdy.strUMSTaskID, strTaskID, PCS_DEF_LEN_GCREF);
		lv_sndbdy.RoutePointID = (US)nRoutePointID;
		lv_sndbdy.ReasonCode = (US)nReasonCode;

		// Send Message
		memcpy(lv_sndmsg, &lv_sndhdr, sizeof(lv_sndhdr));
		memcpy(lv_sndmsg+sizeof(lv_sndhdr), &lv_sndbdy, sizeof(lv_sndbdy));

		// Sun updated 2010-05-18 [V7.1.8.3]
		lv_retval = pMainWindow->MQ_HA_SendMsg(lv_sndmsg, lv_sndhdr.PackageLen);
	}
	catch(...)
	{
		lv_retval = FALSE;
		WriteTrace(TraceError, "IVR Channel: %d failed when call MQ_SendMsg_PauseQueueTask(%s, %d, %d)", 
			m_intLineNO, strTaskID, nRoutePointID, nReasonCode);
	}

	return lv_retval;
}

BOOL CIVRChannel::MQ_SendMsg_DispatchTaskReqAck(LPCSTR strTaskID, UL nRoutePointID, UC nErrorCode)
{
	BOOL lv_retval;

	if( strTaskID[0] == NULL )
		return FALSE;

	try
	{
		SMsgHeader lv_sndhdr;
		S_TCP_UMS2PCS_DISPATCHTASK_REQ_ACK lv_sndbdy;
		BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

		lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
		lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
		lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
		lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
		lv_sndhdr.Sender = USER_IVR;
		lv_sndhdr.Receiver = USER_CTI;

		memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));
		lv_sndhdr.Command = TCP_UMS2PCS_DISPATCHTASK_REQ_ACK;
		lv_sndhdr.PackageLen = sizeof(lv_sndhdr) + sizeof(lv_sndbdy);

		lv_sndbdy.TaskID = atol(m_sDeviceNo);
		strncpy(lv_sndbdy.strUMSTaskID,strTaskID, PCS_DEF_LEN_GCREF);
		lv_sndbdy.RoutePointID = (US)nRoutePointID;
		lv_sndbdy.ErrorCode = nErrorCode;

		// Send Message
		memcpy(lv_sndmsg, &lv_sndhdr, sizeof(lv_sndhdr));
		memcpy(lv_sndmsg+sizeof(lv_sndhdr), &lv_sndbdy, sizeof(lv_sndbdy));

		// Sun updated 2010-05-18 [V7.1.8.3]
		lv_retval = pMainWindow->MQ_HA_SendMsg(lv_sndmsg, lv_sndhdr.PackageLen);
	}
	catch(...)
	{
		lv_retval = FALSE;
		WriteTrace(TraceError, "IVR Channel: %d failed when call MQ_SendMsg_DispatchTaskReqAck(%s, %d, %d)", 
			m_intLineNO, strTaskID, nRoutePointID, nErrorCode);
	}

	return lv_retval;
}

BOOL CIVRChannel::MQ_SendMsg_ContinueQueueTask(LPCSTR strTaskID, UL nRoutePointID, UL nReasonCode)
{
	BOOL lv_retval;

	if( strTaskID[0] == NULL )
		return FALSE;

	try
	{
		SMsgHeader lv_sndhdr;
		S_TCP_UMS2PCS_CONTINUETASK_REQ lv_sndbdy;
		BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

		lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
		lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
		lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
		lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
		lv_sndhdr.Sender = USER_IVR;
		lv_sndhdr.Receiver = USER_CTI;

		memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));
		lv_sndhdr.Command = TCP_UMS2PCS_CONTINUETASK_REQ;
		lv_sndhdr.PackageLen = sizeof(lv_sndhdr) + sizeof(lv_sndbdy);

		//lv_sndbdy.InvokeID = GET_INVOKE_ID;
		lv_sndbdy.InvokeID = GetCallRefID();

		lv_sndbdy.TaskID = atol(m_sDeviceNo);
		strncpy(lv_sndbdy.strUMSTaskID,strTaskID, PCS_DEF_LEN_GCREF);
		lv_sndbdy.RoutePointID = (US)nRoutePointID;
		lv_sndbdy.ReasonCode = (US)nReasonCode;

		// Send Message
		memcpy(lv_sndmsg, &lv_sndhdr, sizeof(lv_sndhdr));
		memcpy(lv_sndmsg+sizeof(lv_sndhdr), &lv_sndbdy, sizeof(lv_sndbdy));

		// Sun updated 2010-05-18 [V7.1.8.3]
		lv_retval = pMainWindow->MQ_HA_SendMsg(lv_sndmsg, lv_sndhdr.PackageLen);
	}
	catch(...)
	{
		lv_retval = FALSE;
		WriteTrace(TraceError, "IVR Channel: %d failed when call MQ_SendMsg_ContinueQueueTask(%s, %d, %d)", 
			m_intLineNO, strTaskID, nRoutePointID, nReasonCode);
	}

	return lv_retval;
}

BOOL CIVRChannel::MQ_SendMsg_FinishQueueTask(LPCSTR strTaskID, UL nRoutePointID, UL nReasonCode)
{
	BOOL lv_retval;

	if( strTaskID[0] == NULL )
		return FALSE;

	try
	{
		SMsgHeader lv_sndhdr;
		S_TCP_UMS2PCS_REMOVETASK_REQ lv_sndbdy;
		BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

		lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
		lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
		lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
		lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
		lv_sndhdr.Sender = USER_IVR;
		lv_sndhdr.Receiver = USER_CTI;

		memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));
		lv_sndhdr.Command = TCP_UMS2PCS_REMOVETASK_REQ;
		lv_sndhdr.PackageLen = sizeof(lv_sndhdr) + sizeof(lv_sndbdy);

		//lv_sndbdy.InvokeID = GET_INVOKE_ID;
		lv_sndbdy.InvokeID = GetCallRefID();

		lv_sndbdy.TaskID = atol(m_sDeviceNo);
		strncpy(lv_sndbdy.strUMSTaskID,strTaskID, PCS_DEF_LEN_GCREF);
		lv_sndbdy.RoutePointID = (US)nRoutePointID;
		lv_sndbdy.ReasonCode = (US)nReasonCode;

		// Orgnize App Data & User Data
		FillupCallBindedData(lv_sndbdy.strAppData, lv_sndbdy.strUserData);

		// Send Message
		memcpy(lv_sndmsg, &lv_sndhdr, sizeof(lv_sndhdr));
		memcpy(lv_sndmsg+sizeof(lv_sndhdr), &lv_sndbdy, sizeof(lv_sndbdy));

		// Sun updated 2010-05-18 [V7.1.8.3]
		lv_retval = pMainWindow->MQ_HA_SendMsg(lv_sndmsg, lv_sndhdr.PackageLen);
	}
	catch(...)
	{
		lv_retval = FALSE;
		WriteTrace(TraceError, "IVR Channel: %d failed when call MQ_SendMsg_FinishQueueTask(%s, %d, %d)", 
			m_intLineNO, strTaskID, nRoutePointID, nReasonCode);
	}

	return lv_retval;
}

// Sun added 2012-04-18 [A]
/// ��PCS������Ϣͨ�ù���
BOOL CIVRChannel::MQ_SendMsg_General_PCS(US f_cmd, const UC *f_msg, int f_nDataLen)
{
//	BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];
//	S_TCP_IVREXT_PACKGET *lv_pbdyhdr;
//	US lv_nLen;

	BOOL lv_retval;

	try
	{
		SMsgHeader lv_sndhdr;
		BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];
		memset(lv_sndmsg, 0x00, PCS_MAX_MESSAGE_SIZE);

		lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
		lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
		lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
		lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
		lv_sndhdr.Sender = USER_IVR;
		lv_sndhdr.Receiver = USER_CTI;

		lv_sndhdr.Command = f_cmd;
		lv_sndhdr.PackageLen = sizeof(lv_sndhdr) + f_nDataLen;

		memcpy(lv_sndmsg, &lv_sndhdr, sizeof(lv_sndhdr));
		memcpy(lv_sndmsg+sizeof(lv_sndhdr), f_msg, f_nDataLen);

		lv_retval = pMainWindow->MQ_HA_SendMsg(lv_sndmsg, lv_sndhdr.PackageLen);
	}
	catch(...)
	{
		lv_retval = FALSE;
		WriteTrace(TraceError, "IVR Channel: %d failed when call MQ_SendMsg_General_PCS, Cmd=%d, DataLen=%d", 
			m_intLineNO, f_cmd, f_nDataLen);
	}

	return lv_retval;
}

// Sun added 2004-12-30
BOOL CIVRChannel::ReadTextFile(LPCSTR f_sFileName, CString &f_sText)
{
	char pBuffer[DEF_TTS_CST_MAX_INBUF_SIZE+1];
	HANDLE hFile;

	// Sun added 2012-11-23
	if( GetIsAsynPlaying() )
	{
#ifdef INTEL_DIALOGIC
		m_oARS.ivr_channel_dx_stop();
#endif
		SetIsAsynPlaying(FALSE);
	}

	try
	{
		hFile = CreateFile( f_sFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if( hFile == INVALID_HANDLE_VALUE )
		{
			WriteTrace(TraceWarn, "IVR Channel: %d cannot open file : %s!!!", m_intLineNO, f_sFileName);
			return FALSE;
		}
		DWORD dwFileLen = GetFileSize( hFile, NULL );
		if( dwFileLen <= 0 )
			return FALSE;

		DWORD dwRead;

		if( dwFileLen > DEF_TTS_CST_MAX_INBUF_SIZE )
			dwFileLen = DEF_TTS_CST_MAX_INBUF_SIZE;
		ReadFile( hFile, pBuffer, dwFileLen, &dwRead, NULL );
		pBuffer[dwRead] = '\0';

		f_sText = pBuffer;
	}
	catch(...)
	{
		WriteTrace(TraceError, "IVR Channel: %d failed to read from file: %s!!!", m_intLineNO, f_sFileName);
		return FALSE;
	}

	return TRUE;
}

// Sun added 2004-12-30
void CIVRChannel::FillupCallBindedData(LPSTR f_sAppData, LPSTR f_sUserData)
{
	int lv_nAppLen, lv_nUserLen, lv_loop;
	UC lv_uservar[PCS_MAX_VAR_LEN];		// �û�����
	int lv_varlength;					// �û���������

	lv_nAppLen = lv_nUserLen = 0;
	for( lv_loop = 0; lv_loop < 15; lv_loop++ )
	{
		if( m_Data018_2.typeflags[lv_loop] == 0 )
			break;

		lv_varlength = NF_GetUserValue(m_Data018_2.valueid[lv_loop], lv_uservar);
		if( m_Data018_2.prefix1[lv_loop] > 0 )
			lv_varlength++;
		if( m_Data018_2.prefix2[lv_loop] > 0 )
			lv_varlength++;

		if( m_Data018_2.typeflags[lv_loop] == 1 )
		{
			if( lv_varlength + lv_nAppLen + 1 < PCS_DEF_DATALEN_APP )
			{
				if( m_Data018_2.prefix1[lv_loop] > 0 )
				{
					f_sAppData[lv_nAppLen++] = m_Data018_2.prefix1[lv_loop];
					lv_varlength--;
				}
				if( m_Data018_2.prefix2[lv_loop] > 0 )
				{
					f_sAppData[lv_nAppLen++] = m_Data018_2.prefix2[lv_loop];
					lv_varlength--;
				}
				if( lv_varlength > 0 )
				{
					memcpy(f_sAppData+lv_nAppLen, lv_uservar, lv_varlength);
					lv_nAppLen += lv_varlength;
				}
				f_sAppData[lv_nAppLen++] = m_Data018_1.seperator;
			}
		}
		else
		{
			if( lv_varlength + lv_nUserLen + 1 < PCS_DEF_DATALEN_USER )
			{
				if( m_Data018_2.prefix1[lv_loop] > 0 )
				{
					f_sUserData[lv_nUserLen++] = m_Data018_2.prefix1[lv_loop];
					lv_varlength--;
				}
				if( m_Data018_2.prefix2[lv_loop] > 0 )
				{
					f_sUserData[lv_nUserLen++] = m_Data018_2.prefix2[lv_loop];
					lv_varlength--;
				}
				if( lv_varlength > 0 )
				{
					memcpy(f_sUserData+lv_nUserLen, lv_uservar, lv_varlength);
					lv_nUserLen += lv_varlength;
				}
				f_sUserData[lv_nUserLen++] = m_Data018_1.seperator;
			}
		}
	}

	// If app or user data is empty, try to fill them with old data
	if( lv_nAppLen == 0 )
		memcpy(f_sAppData, ChData.appdata, sizeof(ChData.appdata));
	else
		// Sun added 2011-04-08
		NF_SetAppData((UC *)f_sAppData);


	if( lv_nUserLen == 0 )
		memcpy(f_sUserData, m_strUserData, PCS_DEF_DATALEN_USER);
	else
		// Sun added 2011-04-08
		NF_SetUserData(f_sUserData, PCS_DEF_DATALEN_USER);
}

// Sun added 2005-03-22
void CIVRChannel::FillupExtReceiveData(SData2_096 *f_pData2, UC *f_sReceiveData)
{
	int lv_ptr, lv_loop = 0;
	UC lv_uservar[PCS_MAX_VAR_LEN];		// �û�����
	int lv_varlength;					// �û���������
	S_TCP_IVREXT_PACKGET *lv_pbdyhdr;
	UC *lv_buffer;

	lv_pbdyhdr = (S_TCP_IVREXT_PACKGET *)f_sReceiveData;
	lv_buffer = f_sReceiveData + sizeof(S_TCP_IVREXT_PACKGET);
	lv_ptr = 0;

	// ����SessionID
	m_strExtSN.Format("%u_%u", lv_pbdyhdr->ChannelNO, lv_pbdyhdr->PacketID);

	try
	{
		// ������չ����
		NF_SetExtData((LPCSTR)(lv_buffer + lv_pbdyhdr->VariableLength), lv_pbdyhdr->NotesLength);

		for( lv_loop = 0; lv_loop < 10; lv_loop++ )
		{
			if( f_pData2->var_receive[lv_loop] == 0 || lv_ptr >= lv_pbdyhdr->VariableLength )
				break;

			/// ����һ������
			lv_varlength = 0;
			memset(lv_uservar, 0x00, sizeof(lv_uservar));
			while( lv_ptr < lv_pbdyhdr->VariableLength )
			{
				if( lv_buffer[lv_ptr] == lv_pbdyhdr->Seperator )
				{
					lv_ptr++;
					break;
				}
				else
				{
					lv_uservar[lv_varlength++] = lv_buffer[lv_ptr];
					lv_ptr++;
				}
			}
			// Sun updated 2012-04-21
			// From
			//if( lv_varlength > 0 )
			// To
			if( lv_varlength >= 0 )
			{
				CString lv_sTemp;

				NF_SetUserValue(f_pData2->var_receive[lv_loop], lv_uservar, lv_varlength);
				lv_sTemp = (LPCSTR)lv_uservar;
				WriteTrace(TraceDetail, "IVR Channel: %d set variable:%d: with value %s", m_intLineNO, f_pData2->var_receive[lv_loop], lv_sTemp);
			}
		}
	}
	catch(...)
	{
		WriteTrace(TraceError, "IVR Channel: %d FillupExtReceiveData error at loop %d", m_intLineNO, lv_loop);
	}
}

// Sun added 2005-03-18
int CIVRChannel::FillupExtSendData(SData1_096 *f_pData1, SData2_096 *f_pData2, LPSTR f_sSendData)
{
	int lv_nOutputLen, lv_loop = 0;
	UC lv_uservar[PCS_MAX_VAR_LEN];		// �û�����
	int lv_varlength;					// �û���������

	lv_nOutputLen = 0;

	try
	{
		for( lv_loop = 0; lv_loop < 10; lv_loop++ )
		{
			if( f_pData2->var_send[lv_loop] == 0 )
				break;

			memset(lv_uservar, 0x00, sizeof(lv_uservar));
			lv_varlength = NF_GetUserValue(f_pData2->var_send[lv_loop], lv_uservar);
			if( lv_varlength + lv_nOutputLen + 1 < PCS_MAX_MESSAGE_SIZE )
			{
				if( lv_varlength > 0 )
				{
					memcpy(f_sSendData+lv_nOutputLen, lv_uservar, lv_varlength);
					lv_nOutputLen += lv_varlength;
				}
				f_sSendData[lv_nOutputLen++] = f_pData1->seperator;
			}
			else
			{
				WriteTrace(TraceWarn, "IVR Channel: %d FillupExtSendData too long at loop %d, var length = %d", m_intLineNO, lv_loop, lv_varlength);
				break;
			}
		}
	}
	catch(...)
	{
		WriteTrace(TraceError, "IVR Channel: %d FillupExtSendData error at loop %d", m_intLineNO, lv_loop);
	}

	return lv_nOutputLen;
}

// Sun added 2005-03-18
BOOL CIVRChannel::SendExternMsg(US f_cmd, UC f_sep, const char *f_msg, int f_nDataLen)
{
	BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];
	S_TCP_IVREXT_PACKGET *lv_pbdyhdr;
	US lv_nLen;

	lv_nLen = sizeof(S_TCP_IVREXT_PACKGET);
	lv_pbdyhdr = (S_TCP_IVREXT_PACKGET *)lv_sndmsg;
	lv_pbdyhdr->PacketID = GET_INVOKE_ID;
	lv_pbdyhdr->ChannelNO = m_intLineNO;
	lv_pbdyhdr->Seperator = f_sep;
	lv_pbdyhdr->VariableLength = (US)f_nDataLen;
	lv_pbdyhdr->NotesLength = 0;	
	
	memcpy(lv_sndmsg+lv_nLen, f_msg, f_nDataLen);
	lv_nLen += (US)f_nDataLen;

	return( pMainWindow->m_sckExternalServer.SendProtocolData(f_cmd, lv_nLen, lv_sndmsg) );
}

// Sun added 2005-03-22
BOOL CIVRChannel::SendExtOBResult(UC f_cResult)
{
	S_TCP_IVR2EXT_TASK_RESULT lv_sndbdy;

	lv_sndbdy.PacketID = GET_INVOKE_ID;		// NF_GetOBPacketID()
	lv_sndbdy.GroupNO = m_bytGroupNo;
	lv_sndbdy.CampaignID = m_nCampaignID;
	lv_sndbdy.TaskID = m_nTaskID;
	lv_sndbdy.TaskResult = (UC)f_cResult;
	lv_sndbdy.CallRefID = m_lngCurCallID;

	// ���ñ��
	m_blnOBResultSended = TRUE;

	return( pMainWindow->m_sckExternalServer.SendProtocolData(TCP_IVR2EXT_TASK_RESULT, sizeof(lv_sndbdy), (UC *)&lv_sndbdy) );
}

// Sun added 2005-05-27
BOOL CIVRChannel::UnLinkOBChannel()
{
	CIVRChannel *lv_pChannel;

	if( m_nLinkChannelNo >= 0 )
	{
		if( m_nLinkChannelNo < theApp.m_nIVRChannels )
		{
			WriteTrace(TraceDebug, "Unlink info - IVR Channel: %d and channel: %d", m_intLineNO, m_nLinkChannelNo);

			lv_pChannel = PIVRCH[m_nLinkChannelNo];
			m_nLinkChannelNo = -1;
			if( lv_pChannel )
			{
				lv_pChannel->NF_SetCCTalking(FALSE);
				lv_pChannel->SetHookOn();
				m_oARS.sc_channel_unlink(&lv_pChannel->m_oARS);
			}
		}
	}

	return TRUE;
}

// Sun added 2012-08-02
BOOL CIVRChannel::SmartSleep(DWORD dwTimeout, DWORD dwInterval)
{
	DWORD lv_time = 0;
	while( lv_time < dwTimeout )
	{
		if( !m_bInboundCall )
		{
			return FALSE;
		}
		else
			Sleep(dwInterval);
		
		lv_time += dwInterval;
	}

	return TRUE;
}

// Sun added 2013-01-27 [A]
/// �������������Ϣ
BOOL CIVRChannel::CancelChannelUMSTask()
{
	BOOL lv_ret = FALSE;

	if( m_strTaskID.GetLength() > 0 && !m_blnTaskClosed )
	{
		if( MQ_SendMsg_RemoveQueueTask(m_strTaskID, m_QueueWaitSyncEvent.m_nTag, 19) )
		{
			m_blnTaskClosed = TRUE; //m_strTaskID = _T("");
			lv_ret = TRUE;
		}
	}

	return lv_ret;
}
