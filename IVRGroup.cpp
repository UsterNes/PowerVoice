// IVRGroup.cpp: implementation of the CIVRGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PowerVoice.h"
#include "IVRGroup.h"
#include "MainFrm.h"
#include "PowerVoiceView.h"

#include "_CommonFunc.h"
#include "_Global.h"
#include "ManulCallCom.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CIVRObject::CIVRObject()
 : m_bytObjectType(1)
{
}

CIVRObject::~CIVRObject()
{
}

CIVRGroup::CIVRGroup()
{
	m_bytObjectType = 2;
	m_blnBindFaxResource = TRUE;
	m_bEnabled = FALSE;
	m_bStarted = FALSE;
	m_bNeedCTI = FALSE;
	m_bytGroupNO = 0;
	m_dwRoutePointID = 0;
	m_bytCustID = 0;		// Sun added 2012-09-09
	m_szCaption = _T("");
	ET_First = NULL;
	ET_Second = NULL;
	m_CurrentETID = 0;
	m_AllocFirst = FALSE;
	m_AllocSecond = FALSE;
	m_intStartCH = 0;
	m_intEndCH = 0;
	m_intFlagCH = 0;
	m_intResourcePrjID = 0;
	m_IVRProgramID = DEF_LD_GRP_DEFAULT_ET;
	m_intNextResourcePrjID = 0;
	m_IVRNextProgramID = 0;
	m_nMonitorChannel = -1;
	m_blnMonitor = FALSE;
	m_szIVRNextFileName.Empty();
	m_szIVRFileName.Empty();
	m_szIVRProgramCaption.Empty();
	m_olestrClassID = OLESTR("IVRUserCOM.Program");

	// Sun added 2008-05-29
	m_dwLastCallDistribute = GetTickCount();

	// Sun added 2007-11-04
	m_lngMinGCTIT = 0;

	// 关闭流程节点日志, Sun added 2008-02-19
	m_blnLogSwitchOff = 0;
}

CIVRGroup::~CIVRGroup()
{
	int lv_loop;

	// Stop Group
	StopGroup(TRUE);

	// Free memory
	if(m_AllocFirst && ET_First != NULL)
	{
		delete []ET_First;
	}
	if(m_AllocSecond && ET_Second != NULL)
	{
		delete []ET_Second;
	}
	
	// Deallocate Resource Map
	for( lv_loop = 0; lv_loop < MAXLANGUAGECOUNT; lv_loop++ ) 
	{
		if(m_pResMap[lv_loop] != NULL)
		{
			delete [](m_pResMap[lv_loop]);
		}
	}

	// Notify main window to refresh
	PropertyChanged();
}

void CIVRGroup::SetStarted(BOOL blnStarted) 
{ 
	m_bStarted = blnStarted;

	// Notify main window to refresh
	PropertyChanged(SCR_GROUP_ITEM_GROUPID);
}

void CIVRGroup::Create(UC groupno)
{
	for( int lv_loop = 0; lv_loop < MAXLANGUAGECOUNT; lv_loop++ )
	{
		m_lngResRCN[lv_loop] = 0;
		m_pResMap[lv_loop] = NULL;
	}

	m_bytGroupNO = groupno;
	InitIVRGroup();
	InitDefaultET();

	if( m_bEnabled )
	{
		// Choose One Working Event Table
		if(!Init_SelectET())
			WriteTrace(TraceError, "Can not start State-Machine:%d!", m_IVRProgramID);
		else
			WriteTrace(TraceInfo, "State-Machine ID:%d starting...OK!", m_IVRProgramID);
	}		
}

BOOL CIVRGroup::GetStarted()
{ 
#ifdef CISCO_CCM
	if( GetLineStatus() < ST_READY )
		return FALSE;
#endif

	return m_bStarted;
}

// Sun added 2006-02-26
void CIVRGroup::IncreaseCallCount()
{
	m_DailyCallCount.IncreaseValue();
	m_YTDCallCount.IncreaseValue();
	PropertyChanged(SCR_GROUP_ITEM_CALLCOUNT);
}

//-----------------------------------------------------------------
// Sun added 2004-12-30
BOOL CIVRGroup::GetAutoLoadCallFlow(CTime &f_tmTime, UC *f_bytProgramID, CString &f_strFileName)
{
	f_tmTime = m_NextCallFlow.GetExplodeTime();
	*f_bytProgramID = m_IVRNextProgramID;
	f_strFileName = m_szIVRNextFileName;

	return m_NextCallFlow.GetEnabled();
}

void CIVRGroup::SetAutoLoadCallFlow(BOOL f_blnEnabled, CTime f_tmTime, UC f_bytProgramID, LPCSTR f_strFileName)
{
	m_NextCallFlow.SetEnabled(FALSE);
	m_IVRNextProgramID = f_bytProgramID;
	if( f_strFileName != NULL )
		m_szIVRNextFileName = f_strFileName;
	m_NextCallFlow.SetExplodeTime(f_tmTime);
	m_NextCallFlow.SetEnabled(f_blnEnabled);
}

BOOL CIVRGroup::GetAutoLoadResource(CTime &f_tmTime, US *f_intResourceID)
{
	f_tmTime = m_NextResource.GetExplodeTime();
	*f_intResourceID = m_intNextResourcePrjID;

	return m_NextResource.GetEnabled();
}

void CIVRGroup::SetAutoLoadResource(BOOL f_blnEnabled, CTime f_tmTime, US f_intResourceID)
{
	m_NextResource.SetEnabled(FALSE);
	m_intNextResourcePrjID = f_intResourceID;
	m_NextResource.SetExplodeTime(f_tmTime);
	m_NextResource.SetEnabled(f_blnEnabled);
}

/// 定时例行检查
void CIVRGroup::CheckAutomaticTimer(BOOL f_blnResetDailyCallData)
{
	BOOL lv_retval;
	CString lv_sCaption;
	int lv_loop;

	/// Sun added 2008-05-23
#ifdef CISCO_CCM

	long lv_lngret;

	/// To Recovery from Line Error
	if( GetLineHandle() == NULL )
	{
		// Sun added 2008-06-26 [A], 避免多次初始化TAPI
		if( pMainWindow->m_bRunning )
		// End of 2008-06-26 [A]
		{
			WriteTrace(TraceWarn, "Warn - IVRGroup: %d (%s) is unavailable now, should try recovery from error.", m_bytGroupNO, m_strDeviceName);
			lv_lngret = Restart();
			if( lv_lngret == 0 )
			{
				// Add group into device list
				pChannelView->m_oIVRCHMap.SetAt(CTapiRoutePoint::GetLineHandle(), this);
				WriteTrace(TraceWarn, "IVR Group: %d bind with TapiLine: 0x%x.", GetGroupNO(), CTapiRoutePoint::GetLineHandle());
			}
			else if( lv_lngret == 5 )
			{
				pMainWindow->m_bRunning = FALSE;

				// Inform Main-Form to Reinitialize Channels
				pMainWindow->PostMessage(WM_SYS_REINIT_DEVICE);
			}
		}
	}
	else
	{
		/// Sun added 2008-05-29
		/// RoutePoint设备空闲自检，默认15秒
		if( GetTickCount() - m_dwLastCallDistribute > gb_lngRoutePointCheckTimer * (DWORD)1000 )
		{
			// Sun added 2010-01-08 [C]
			/// 避免频繁检查
			m_dwLastCallDistribute = GetTickCount();

			lv_lngret = CTapiRoutePoint::ServiceSelfCheck();
			if( lv_lngret != 0 )
			{
				WriteTrace(TraceError, "CTI RoutePoint Error - System auto checker found platform error 0x%x on group: %d!", 
					lv_lngret, m_bytGroupNO);

				if( lv_lngret == LINEERR_UNINITIALIZED )
				{
					// Sun replaced 2010-07-16
					/// From
					// 关闭组，下次自检时会重启
					//StopGroup(FALSE);
					/// To
					// 关闭对象，下次自检时会重启
					pChannelView->m_oIVRCHMap.RemoveKey(CTapiRoutePoint::GetLineHandle());
					WriteTrace(TraceWarn, "IVR Group: %d unbind with TapiLine: 0x%x.", GetGroupNO(), CTapiRoutePoint::GetLineHandle());

					CTapiRoutePoint::Close();
				}
				// Sun added 2010-07-16
				else if( lv_lngret == LINEERR_INVALLINEHANDLE )
				{
					// 关闭对象，下次自检时会重启
					pChannelView->m_oIVRCHMap.RemoveKey(CTapiRoutePoint::GetLineHandle());
					WriteTrace(TraceWarn, "IVR Group: %d unbind with TapiLine: 0x%x.", GetGroupNO(), CTapiRoutePoint::GetLineHandle());

					CTapiRoutePoint::Close();
				}
			}
		}
	}

#endif

	/// Auto reload Call Flow
	if( m_NextCallFlow.CheckTimer() )
	{
		SetIVRProgramID(m_IVRNextProgramID);
		if( m_IVRNextProgramID == DEF_LD_GRP_ET_FILE )
			SetIVRFileName(m_szIVRNextFileName);
				
		// Reload Call Flow
		lv_retval = Init_SelectET();
		if( lv_retval )
		{
			PGroupInfo[m_bytGroupNO].IVRProgramID = GetIVRProgramID();
			lv_sCaption.Format("GROUP%u_IVRProgramID", m_bytGroupNO + 1);
			theApp.WriteProfileInt("GROUPINFO", lv_sCaption, GetIVRProgramID());

			if( GetIVRProgramID() == DEF_LD_GRP_ET_FILE )
			{
				lv_sCaption.Format("GROUP%u_IVRProgramFN", m_bytGroupNO + 1);
				theApp.WriteProfileString("GROUPINFO", lv_sCaption, m_szIVRNextFileName);
			}

		}
		else
		{
			SetIVRProgramID(PGroupInfo[m_bytGroupNO].IVRProgramID);
		}
	}

	// Auto reload Resource
	if( m_NextResource.CheckTimer() )
	{
		lv_retval = LoadResourceMap(m_intNextResourcePrjID);
		if( lv_retval )
			SetResourcePrjID(m_intNextResourcePrjID);
	}

	// Sun added 2006-02-26
	if( f_blnResetDailyCallData )
	{
		m_DailyCallCount.SetValue(0);
		PropertyChanged(SCR_GROUP_ITEM_CALLCOUNT);

		for( lv_loop = m_intStartCH; lv_loop <= m_intEndCH; lv_loop++ )
		{
			if( PIVRCH[lv_loop] != NULL )
				PIVRCH[lv_loop]->ResetDailyCallCount();
		}
	}
}
// End of Sun added 2004-12-30
//-----------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// 运行查询语句
BOOL CIVRGroup::ExecuteSQLStatement(CString f_SQL)
{
	return pMainWindow->m_sckDataRecorder.AddMessage(f_SQL);
}
//----------------------------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// CIVRGroup message handlers
//
//----------------------------------------------------------------------------------------------
// 初始化 - 缺省 IVR 工作流程
BOOL CIVRGroup::InitDefaultET()
{
	int	lv_index;
	SData1_020 lv_data1_020;
	SData2_020 lv_data2_020;
	SData1_023 lv_data1_023;
	SData2_023 lv_data2_023;
	SData1_040 lv_data1_040;
	SData2_040 lv_data2_040;
	SData1_060 lv_data1_060;
	SData2_060 lv_data2_060;
	
	// Node 0 -- Get CallID
	lv_index = 0;
	ET_Default[lv_index].cStatus = LN_IDLE;			// 取主叫号码
	memset(ET_Default[lv_index].Data1, 0, sizeof(ET_Default[lv_index].Data1));
	memset(ET_Default[lv_index].Data2, 0, sizeof(ET_Default[lv_index].Data2));
	ET_Default[lv_index].NodeNo = 0;				// angetcid(Get Call NO)
	
	// Node 1 -- Play welcome
	memset(&lv_data1_023,0,sizeof(SData1_023));
	memset(&lv_data2_023,0,sizeof(SData2_023));

	lv_data1_023.breakkey = 'A';		// All break
	lv_data1_023.log = 0;				// No log
	lv_data1_023.playclear = 0;			// No clear
	lv_data1_023.timeout = 0;			// No timeout
	
	lv_data2_023.nd_goto = LN_GET_STATIONNO;
	lv_data2_023.nd_parent = LN_IDLE;
	lv_data2_023.vox_play = 101;

	lv_index = 1;
	ET_Default[lv_index].cStatus = LN_PLAY_WELCOME;			// 播放欢迎词
	memcpy(ET_Default[lv_index].Data1, (UC *)&lv_data1_023, sizeof(ET_Default[lv_index].Data1));
	memcpy(ET_Default[lv_index].Data2, (UC *)&lv_data2_023, sizeof(ET_Default[lv_index].Data2));
	ET_Default[lv_index].NodeNo = 23;				// NF_023
	
	// Node 2 -- Switch to agency
	memset(&lv_data1_060,0,sizeof(SData1_060));
	memset(&lv_data2_060,0,sizeof(SData2_060));
	
	//lv_data1_060.switchtype = 0;		// Automactic transfer
	lv_data1_060.switchtype = 2;		// User enter
	lv_data1_060.getlength = 3;			// 3 charaters
	lv_data1_060.timeout = 100;			// Timeout over 100s
	lv_data1_060.looptimes = 3;			// Loop 3 times
	
	lv_data2_060.vox_op = 102;			// Please dial agency
	lv_data2_060.vox_sw = 103;			// Pls wait Switching
	lv_data2_060.vox_wt = 104;			// Pls wait agency to idle
	lv_data2_060.vox_busy = 105;		// Agency is busy
	lv_data2_060.vox_nobody = 106;		// Nobody on duty
	lv_data2_060.vox_ok = 107;			// Switch successfully, No.? agency helps U
	lv_data2_060.nd_parent = LN_PLAY_WELCOME;
	lv_data2_060.nd_busy = LN_GET_STATIONNO;		// Loop
	lv_data2_060.nd_nobody = LN_PLAY_NOMSI;
	lv_data2_060.nd_ok = LN_SPEECH;

	lv_index = 2;
	ET_Default[lv_index].cStatus = LN_GET_STATIONNO;		// 转分机号码
	memcpy(ET_Default[lv_index].Data1, (UC *)&lv_data1_060, sizeof(ET_Default[lv_index].Data1));
	memcpy(ET_Default[lv_index].Data2, (UC *)&lv_data2_060, sizeof(ET_Default[lv_index].Data2));
	ET_Default[lv_index].NodeNo = 60;				// NF_060
	
	// Node 3 -- No MSI please leave message
	memset(&lv_data1_040,0,sizeof(SData1_040));
	memset(&lv_data2_040,0,sizeof(SData2_040));

	lv_data1_040.breakkey = 'A';			// All break
	lv_data1_040.playclear = 0;				// No clear
	lv_data1_040.rectime = 120;				// Record 2 minutes
	
	lv_data2_040.vox_op = 108;				// Pls leave a message
	lv_data2_040.nd_parent = LN_GET_STATIONNO;
	lv_data2_040.nd_child = LN_PLAY_ONHOOK;

	lv_index = 3;
	ET_Default[lv_index].cStatus = LN_PLAY_NOMSI;			// 座席没有上班，投诉请录音。
	memcpy(ET_Default[lv_index].Data1, (UC *)&lv_data1_040, sizeof(ET_Default[lv_index].Data1));
	memcpy(ET_Default[lv_index].Data2, (UC *)&lv_data2_040, sizeof(ET_Default[lv_index].Data2));
	ET_Default[lv_index].NodeNo = 40;				// NF_040

	// Node 4 -- Play and hook on
	memset(&lv_data1_020,0,sizeof(SData1_020));
	memset(&lv_data2_020,0,sizeof(SData2_020));

	lv_data1_020.playclear = 1;			// Play clear
	
	lv_data2_020.nd_parent = LN_PLAY_NOMSI;
	lv_data2_020.vox_play = 109;				// Thank U for your visit

	lv_index = 4;
	ET_Default[lv_index].cStatus = LN_PLAY_ONHOOK;			// 挂机退出
	memcpy(ET_Default[lv_index].Data1, (UC *)&lv_data1_020, sizeof(ET_Default[lv_index].Data1));
	memcpy(ET_Default[lv_index].Data2, (UC *)&lv_data2_020, sizeof(ET_Default[lv_index].Data2));
	ET_Default[lv_index].NodeNo = 20;				// NF_020
	
	return TRUE;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// 装载 - 缺省 IVR 工作流程
BOOL CIVRGroup::LoadDefaultET()
{
	// 流程ID
	m_IVRProgramID = DEF_LD_GRP_DEFAULT_ET;

	// 根节点
	SetRootNode(m_CurrentETID, LN_PLAY_WELCOME);

	switch(m_CurrentETID)
	{
	case 0:
		// Free allocated buffer
		if(m_AllocFirst)
		{
			delete []ET_First;
		}

		// Set to default
		ET_First = ET_Default;
		m_AllocFirst = FALSE;
		break;

	default:
		// Free allocated buffer
		if(m_AllocSecond)
		{
			delete []ET_Second;
		}

		// Set to default
		ET_Second = ET_Default;
		m_AllocSecond = FALSE;
		break;
	}
	
	// Write down RCN
	SetRCN(m_CurrentETID, EVENTTABLESIZE - 1);
	
	// Switch ETID to another one
	AlterET();

	return TRUE;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// 生成状态转换矩阵
// 返回：TRUE - 成功；FALSE - 失败
BOOL CIVRGroup::LoadIVRProgram()
{
	BOOL lv_retval;

	// Load Call Flow from DB or IVR File
	if( m_IVRProgramID == DEF_LD_GRP_ET_FILE )
	{
		lv_retval = LoadIVRProgramFromFile();
	}
	else
	{
		if( (lv_retval = LoadIVRProgramFromDB()) )
			GetIVRProgramName();
	}

	return lv_retval;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
BOOL CIVRGroup::GetIVRProgramName()
{
	CString lv_pname;
	_RecordsetPtr	lv_RS;

	lv_pname.Empty();
	try
	{
		char lv_query[200];
		memset(lv_query, 0x00, sizeof(lv_query));
		sprintf(lv_query, 
			"select P_Name from tbRefIVR where P_ID=%u and P_Type='P'", m_IVRProgramID);
		_bstr_t bstrSQL = lv_query;

		lv_RS.CreateInstance(__uuidof(Recordset));
		lv_RS->CursorType = adOpenStatic;

		// 返回结果集
		lv_RS->CursorLocation = adUseClient;
		if( !pMainWindow->m_DBReader.OpenADORecordset(lv_RS, bstrSQL) )
			return FALSE;
	}
	catch(...)
	{
		WriteTrace(TraceError, "Read tbRefIVR table (P_ID = %d) fialed!", m_IVRProgramID);
		return FALSE;
	}
	
	if( lv_RS->RecordCount > 0 )
	{
		if( lv_RS->Fields->Item["P_Name"]->ActualSize > 0 )
		{
			lv_pname = lv_RS->Fields->Item["P_Name"]->Value.bstrVal;
			lv_pname.TrimRight();
		}
	}
	lv_RS->Close();
	
	SetIVRProgramCaption(lv_pname);
	return TRUE;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
void CIVRGroup::SetIVRProgramID(UC bytProgramID)
{ 
	if( m_IVRProgramID == bytProgramID )
		return;

	m_IVRProgramID = bytProgramID;
	PropertyChanged(SCR_GROUP_ITEM_PROGRAM);
}

void CIVRGroup::SetResourcePrjID(US intResourcePrjID)
{ 
	if( m_intResourcePrjID == intResourcePrjID )
		return;

	m_intResourcePrjID = intResourcePrjID;
	PropertyChanged(SCR_GROUP_ITEM_RESOURCE);

	/*
	// Sun added 2002-12-06
	/// Update Node Data
	CString lv_query;
	if( m_IVRProgramID > DEF_LD_GRP_DEFAULT_ET )
	{
		m_sysNodeData.ResourceProject = intResourcePrjID;
		lv_query.Empty();
		lv_query.Format("UPDATE tbIVRProgram SET N_Data2 = '%s' WHERE P_ID=%u AND N_ID=1", 
			m_sysNodeData, m_IVRProgramID);
		ExecuteSQLStatement(lv_query);
	}
	// End of Add
	*/
}

void CIVRGroup::SetEnabled(BOOL blnEnabled) 
{ 
	if( m_bEnabled == blnEnabled )
		return;

	m_bEnabled = blnEnabled;
	PropertyChanged(SCR_GROUP_ITEM_GROUPID);
}

void CIVRGroup::SetNeedCTI(BOOL bNeedCTI)
{ 
	/// Note: Connection could be opened and closed automatically by system thread
	m_bNeedCTI = bNeedCTI;
	PropertyChanged(SCR_GROUP_ITEM_CTI);
}

// Sun added 2005-07-10
void CIVRGroup::SetIsOBOnly(BOOL bOBOnly)
{
	if( bOBOnly != m_bOBOnly )
		m_bOBOnly = bOBOnly;
}

// Sun added 2006-11-30
void CIVRGroup::SetIsBindFaxRes(BOOL bIsBind)
{
	if( bIsBind != m_blnBindFaxResource )
		m_blnBindFaxResource = bIsBind;
}

// Sun added 2007-07-10
void CIVRGroup::SetCTIRoutePointID(DWORD dwRoutePointID)
{
	if( dwRoutePointID != m_dwRoutePointID )
		m_dwRoutePointID = dwRoutePointID;
}

// Sun added 2012-09-08
void CIVRGroup::SetCustID(UC bytCustID)
{
	if( bytCustID != m_bytCustID )
		m_bytCustID = bytCustID;
}

//--------------------------------------------------
// Sun added 2006-03-05
// Group is monitorred or not
BOOL CIVRGroup::GetIsMonitorred()
{
	return m_blnMonitor;
}

void CIVRGroup::SetIsMonitorred(BOOL f_Value)
{
	if( m_blnMonitor != f_Value )
	{
		m_blnMonitor = f_Value;
		MQ_SendMsg_GroupMonitorAck(f_Value, PCS_DEF_RETCODE_SUCC);
		if( !f_Value )
			SetMonitorChannel(-1);
	}
}

int CIVRGroup::GetMonitorChannel()
{
	return m_nMonitorChannel;
}

BYTE CIVRGroup::SetMonitorChannel(int f_nChannel)
{
	BYTE lv_retval = PCS_DEF_RETCODE_SUCC;
	CIVRChannel *lv_pAgent = NULL;

	if( m_nMonitorChannel != f_nChannel )
	{
		if( m_nMonitorChannel >= GetStartCH() && m_nMonitorChannel <= GetEndCH() )
		{
			lv_pAgent = PIVRCH[m_nMonitorChannel];
			if( lv_pAgent )
				lv_pAgent->SetIsMonitorred(FALSE);
			m_nMonitorChannel = -1;
		}

		if( f_nChannel >= GetStartCH() && f_nChannel <= GetEndCH() )
		{
			lv_pAgent = PIVRCH[f_nChannel];
			if( lv_pAgent )
				lv_pAgent->SetIsMonitorred(TRUE);
			m_nMonitorChannel = f_nChannel;
		}
		else if( f_nChannel != -1 )
			lv_retval = PCS_DEF_RETCODE_NOTEXIST;
	}
	else
		lv_retval = PCS_DEF_RETCODE_NOTNEED;

	return lv_retval;
}

BYTE CIVRGroup::SetUnMonitorChannel(int f_nChannel)
{
	BYTE lv_retval = PCS_DEF_RETCODE_SUCC;
	CIVRChannel *lv_pAgent = NULL;

	if( m_nMonitorChannel == f_nChannel )
	{
		if( m_nMonitorChannel >= GetStartCH() && m_nMonitorChannel <= GetEndCH() )
		{
			lv_pAgent = PIVRCH[m_nMonitorChannel];
			if( lv_pAgent )
				lv_pAgent->SetIsMonitorred(FALSE);
			m_nMonitorChannel = -1;
		}
		else
			lv_retval = PCS_DEF_RETCODE_NOTEXIST;
	}
	else
		lv_retval = PCS_DEF_RETCODE_NOTNEED;

	return lv_retval;
}
//--------------------------------------------------

void CIVRGroup::InitializeChannels()
{
	int lv_loop;

	if( GetStarted() )
	{
		try
		{
			for( lv_loop = m_intStartCH; lv_loop <= m_intEndCH; lv_loop++ )
			{
				if( PIVRCH[lv_loop] != NULL )
				{
					PIVRCH[lv_loop]->MQ_SendMsg_InitChannel();
					Sleep(20);
				}
			}
		}
		catch(...)
		{
			WriteTrace(TraceError, "InitializeChannels() in group: %d error on channel index: %d!", m_bytGroupNO, lv_loop);
		}
	}
}

// Sun added 2013-01-27 [A]
/// 发送任务离队消息
void CIVRGroup::CancelUMSTasks()
{
	int lv_loop;

	if( GetStarted() )
	{
		try
		{
			for( lv_loop = m_intStartCH; lv_loop <= m_intEndCH; lv_loop++ )
			{
				if( PIVRCH[lv_loop] != NULL )
				{
					if( PIVRCH[lv_loop]->CancelChannelUMSTask() )
						Sleep(10);
				}
			}
		}
		catch(...)
		{
			WriteTrace(TraceError, "CancelUMSTasks() in group: %d error on channel index: %d!", m_bytGroupNO, lv_loop);
		}
	}
}

void CIVRGroup::SetCaption(CString szCaption) 
{ 
	if( m_szCaption == szCaption )
		return;

	m_szCaption = szCaption;
	PropertyChanged(SCR_GROUP_ITEM_CAPTION);
}

void CIVRGroup::SetIVRFileName(CString szFileName) 
{ 
	if( m_szIVRFileName == szFileName )
		return;

	m_szIVRFileName = szFileName;
	PropertyChanged(SCR_GROUP_ITEM_PROGRAM);
}

void CIVRGroup::SetIVRProgramCaption(CString szCaption) 
{ 
	if( m_szIVRProgramCaption == szCaption )
		return;

	m_szIVRProgramCaption = szCaption;
	PropertyChanged(SCR_GROUP_ITEM_PROGRAM);
}

void CIVRGroup::SetStartCH(US intStartCH)
{ 
	if( m_intStartCH == intStartCH )
		return;

	US lv_nOldValue = m_intStartCH;
	m_intStartCH = intStartCH;
	if( m_intStartCH > m_intEndCH )
		m_intEndCH = m_intStartCH;

	if( lv_nOldValue > m_intEndCH )
		lv_nOldValue = m_intEndCH;

	// Sun added 2006-01-06
	if( lv_nOldValue > m_intStartCH )
	{
		for( int lv_loop = m_intStartCH; lv_loop < lv_nOldValue; lv_loop++ )
		{
			if( PIVRCH[lv_loop] )
				PIVRCH[lv_loop]->InitCh();
		}
	}

	m_intFlagCH = intStartCH;
	PropertyChanged(SCR_GROUP_ITEM_STARTCH);
}

void CIVRGroup::SetEndCH(US intEndCH) 
{ 
	if( m_intEndCH == intEndCH )
		return;

	int lv_nOldValue = (int)m_intEndCH;
	m_intEndCH = intEndCH;
	if( m_intStartCH > m_intEndCH )
		m_intStartCH = m_intEndCH;

	if( lv_nOldValue + 1 < m_intStartCH )
		lv_nOldValue = m_intStartCH - 1;

	// Sun added 2006-01-06
	if( lv_nOldValue < m_intEndCH )
	{
		for( int lv_loop = lv_nOldValue + 1; lv_loop <= m_intEndCH; lv_loop++ )
		{
			if( PIVRCH[lv_loop] )
				PIVRCH[lv_loop]->InitCh();
		}
	}

	PropertyChanged(SCR_GROUP_ITEM_ENDCH);
}

US CIVRGroup::GetFlagCH()
{
	if( m_intFlagCH > m_intEndCH || m_intEndCH < m_intStartCH )
		m_intFlagCH = m_intStartCH;

	return m_intFlagCH;
}

void CIVRGroup::SetFlagCH(US intValue)
{
	m_intFlagCH = intValue;
}

CIVRChannel *CIVRGroup::MoveToNextFlagCH()
{
	US lv_loop;
	US lv_index;
	CIVRChannel *lv_pAgent, *lv_pCH = NULL;

	m_QueueLock.Enter();

	/// 找下一个空闲通道
	lv_index = GetFlagCH();

	try
	{
		for( lv_loop = m_intStartCH; lv_loop <= m_intEndCH; lv_loop++ )
		{
			if( (lv_pAgent = PIVRCH[lv_index]) != NULL )
			{
				if( lv_pAgent->GetStarted() )
				{
#ifdef CISCO_CCM
				if( lv_pAgent->m_oARS.GetLineStatus() == ST_READY )				
#endif
					if( lv_pAgent->GetStatus() == IVR_IDLE && lv_pAgent->NF_GetOBMsgTelNo() == _T("") )
						lv_pCH = lv_pAgent;
				}
			}

			/// Next Channel
			lv_index++;
			if( lv_index > m_intEndCH )
				lv_index = m_intStartCH;

			if( lv_pCH )
			{
				// Sun added 2008-12-30 [A]
				lv_pCH->SetStatus(IVR_W_ANSWER);

				SetFlagCH(lv_index);
				break;
			}
		}
	}
	catch(...)
	{
	}

	m_QueueLock.Leave();

	if( !lv_pCH )
		WriteTrace(TraceWarn, "Warn - There is no idle channel in Group: %d, ivr channel should be added!", GetGroupNO());

	return lv_pCH;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// 由数据库读取节点数据，生成状态转换矩阵
BOOL CIVRGroup::LoadIVRProgramFromDB()
{
	HRESULT hr;											// 测试返回值
	_RecordsetPtr	lv_RS;
	SEventTable *lv_ET;									// 临时事件表指针
	
	// 通过 ADO 访问流程文件
	try
	{
		char lv_query[200];
		memset(lv_query, 0x00, sizeof(lv_query));

#ifdef PVC_USE_MULTI_NODE_FIELDS
		sprintf(lv_query, 
			"select * from tbIVRProgramNew where P_ID=%u order by N_ID", 
			m_IVRProgramID);
#else
		sprintf(lv_query, 
			"select N_ID, N_NO, N_Data1, N_Data2 from tbIVRProgram where P_ID=%u order by N_ID", 
			m_IVRProgramID);
#endif

		_bstr_t bstrSQL = lv_query;
		lv_RS.CreateInstance(__uuidof(Recordset));
		lv_RS->CursorType = adOpenStatic;

		// 返回结果集
		lv_RS->CursorLocation = adUseClient;
		if( !pMainWindow->m_DBReader.OpenADORecordset(lv_RS, bstrSQL) )
			return FALSE;
	}
	catch(_com_error e)
	{
#ifdef PVC_USE_MULTI_NODE_FIELDS
		WriteTrace(TraceError, "Read table tbIVRProgramNew failed! P_ID = %d, Error Msg:%s", 
			m_IVRProgramID, e.ErrorMessage());
#else
		WriteTrace(TraceError, "Read table tbIVRProgram failed! P_ID = %d, Error Msg:%s", 
			m_IVRProgramID, e.ErrorMessage());
#endif
		return FALSE;
	}
	
	try
	{
		hr = lv_RS->MoveFirst();
		if(FAILED(hr))
		{
			WriteTrace(TraceError, "Recordset MoveFirst failed! P_ID = %d", m_IVRProgramID);
			return FALSE;
		}
	}
	catch(...)
	{
		WriteTrace(TraceError, "Recordset MoveFirst error! P_ID = %d", m_IVRProgramID);
		lv_RS->Close();				// 关闭表
		return FALSE;
	}
	
	// 记录数量
	long lv_RCN = lv_RS->RecordCount;
	if(lv_RCN < 3)
	{
		WriteTrace(TraceError, "IVR program file corrupt! P_ID = %d", m_IVRProgramID);
		return FALSE;
	}

	//----------------------
	// 生成全程控制规则
	//----------------------
	US			lv_nid = 0;				// 节点ID 
	UC			lv_nno = 255;			// 节点编号
	UC			lv_ndata1[DEF_NODE_DATA_SMALL_LEN];		// 节点数据段1
	UC			lv_ndata2[DEF_NODE_DATA_BIG_LEN];		// 节点数据段2

	try
	{
		// 取数据
		if( lv_RS->Fields->Item["N_ID"]->ActualSize > 0 )
			lv_nid = lv_RS->Fields->Item["N_ID"]->Value.iVal;
		if( lv_RS->Fields->Item["N_NO"]->ActualSize > 0 )
			lv_nno = lv_RS->Fields->Item["N_NO"]->Value.bVal;
		// Don't need data1
#ifdef PVC_USE_MULTI_NODE_FIELDS
		pMainWindow->m_DBReader.GotherIVRNodeDataBig(lv_RS, lv_ndata2);
#else
		if( lv_RS->Fields->Item["N_Data2"]->ActualSize > 0 )
			memcpy(lv_ndata2, lv_RS->Fields->Item["N_Data2"]->Value.pbVal,DEF_NODE_DATA_BIG_LEN);
#endif
	}
	catch(...)
	{
		WriteTrace(TraceError, "IVR program read failed! P_ID = %d, N_ID = %d", m_IVRProgramID, lv_nid);
		return FALSE;
	}

	if(lv_nno != 0)
	{
		WriteTrace(TraceError, "IVR program read failed! P_ID = %d, N_ID = %d", m_IVRProgramID, lv_nid);
		return FALSE;
	}
	
	NF_000((UC *)lv_ndata2);
	lv_RCN--;
	lv_RS->MoveNext();
	//----------------------

	//----------------------
	// 用户变量定义
	//----------------------
	try
	{
		// 取数据
		if( lv_RS->Fields->Item["N_ID"]->ActualSize > 0 )
			lv_nid = lv_RS->Fields->Item["N_ID"]->Value.iVal;
		if( lv_RS->Fields->Item["N_NO"]->ActualSize > 0 )
			lv_nno = lv_RS->Fields->Item["N_NO"]->Value.bVal;

#ifdef PVC_USE_MULTI_NODE_FIELDS
		pMainWindow->m_DBReader.GotherIVRNodeDataSmall(lv_RS, lv_ndata1);
#else
		if( lv_RS->Fields->Item["N_Data1"]->ActualSize > 0 )
			memcpy(lv_ndata1, lv_RS->Fields->Item["N_Data1"]->Value.pbVal, DEF_NODE_DATA_SMALL_LEN);
#endif
		// Don't need data2
	}
	catch(...)
	{
		WriteTrace(TraceError, "IVR program read failed! P_ID = %d, N_ID = %d", m_IVRProgramID, lv_nid);
		return FALSE;
	}

	if(NF_001((UC *)lv_ndata1) <= MAXSYSNODEID)
	{

		lv_RS->MoveNext();
		lv_RCN--;
		UC lv_group = 0;

		try
		{
			// 取数据
			if( lv_RS->Fields->Item["N_ID"]->ActualSize > 0 )
				lv_nid = lv_RS->Fields->Item["N_ID"]->Value.iVal;
			if( lv_RS->Fields->Item["N_NO"]->ActualSize > 0 )
				lv_nno = lv_RS->Fields->Item["N_NO"]->Value.bVal;
			// Don't need data1
#ifdef PVC_USE_MULTI_NODE_FIELDS
			pMainWindow->m_DBReader.GotherIVRNodeDataBig(lv_RS, lv_ndata2);
#else
			if( lv_RS->Fields->Item["N_Data2"]->ActualSize > 0 )
				memcpy(lv_ndata2, lv_RS->Fields->Item["N_Data2"]->Value.pbVal,DEF_NODE_DATA_BIG_LEN);
#endif
		}
		catch(...)
		{
			WriteTrace(TraceError, "IVR program read failed! P_ID = %d, N_ID = %d", m_IVRProgramID, lv_nid);
			return FALSE;
		}

		while(NF_002(lv_group, (UC *)lv_ndata2)  <= MAXSYSNODEID)
		{
			lv_RS->MoveNext();
			lv_group++;
			lv_RCN--;
			if(lv_RCN <= 0 || lv_group > MAXSYSNODEID / 4)
			{
				WriteTrace(TraceError, "IVR program read failed! P_ID = %d, N_ID = %d", m_IVRProgramID, lv_nid);
				return 0;
			}
			try
			{
				// 取数据
				if( lv_RS->Fields->Item["N_ID"]->ActualSize > 0 )
					lv_nid = lv_RS->Fields->Item["N_ID"]->Value.iVal;
				if( lv_RS->Fields->Item["N_NO"]->ActualSize > 0 )
					lv_nno = lv_RS->Fields->Item["N_NO"]->Value.bVal;
				// Don't need data1
#ifdef PVC_USE_MULTI_NODE_FIELDS
				pMainWindow->m_DBReader.GotherIVRNodeDataBig(lv_RS, lv_ndata2);
#else
				if( lv_RS->Fields->Item["N_Data2"]->ActualSize > 0 )
					memcpy(lv_ndata2, lv_RS->Fields->Item["N_Data2"]->Value.pbVal,DEF_NODE_DATA_BIG_LEN);
#endif
			}
			catch(...)
			{
				WriteTrace(TraceError, "IVR program read failed! P_ID = %d, N_ID = %d", m_IVRProgramID, lv_nid);
				return FALSE;
			}
		} // End of while
	} // End of if

	lv_RCN--;
	//----------------------
	
	//----------------------
	// 生成事件表
	//----------------------
	//1. 分配存储空间
	lv_ET = new SEventTable[lv_RCN];
	if(lv_ET == NULL)
		return FALSE;
	memset(lv_ET,0,sizeof(SEventTable) * lv_RCN);

	//2. 遍历全部节点
	long lv_node;				// 节点ID
	for(lv_node = 0; lv_node < lv_RCN; lv_node++)
	{
		lv_RS->MoveNext();
		try
		{
			// 取数据
			if( lv_RS->Fields->Item["N_ID"]->ActualSize > 0 )
				lv_nid = lv_RS->Fields->Item["N_ID"]->Value.iVal;
			if( lv_RS->Fields->Item["N_NO"]->ActualSize > 0 )
				lv_nno = lv_RS->Fields->Item["N_NO"]->Value.bVal;
#ifdef PVC_USE_MULTI_NODE_FIELDS
			pMainWindow->m_DBReader.GotherIVRNodeDataSmall(lv_RS, lv_ndata1);
			pMainWindow->m_DBReader.GotherIVRNodeDataBig(lv_RS, lv_ndata2);
#else
			if( lv_RS->Fields->Item["N_Data1"]->ActualSize > 0 )
				memcpy(lv_ndata1, lv_RS->Fields->Item["N_Data1"]->Value.pbVal, DEF_NODE_DATA_SMALL_LEN);
			if( lv_RS->Fields->Item["N_Data2"]->ActualSize > 0 )
				memcpy(lv_ndata2, lv_RS->Fields->Item["N_Data2"]->Value.pbVal, DEF_NODE_DATA_BIG_LEN);
#endif
		}
		catch(...)
		{
			WriteTrace(TraceError, "IVR program read failed! P_ID = %d, N_ID = %d", m_IVRProgramID, lv_nid);
			return FALSE;
		}

		(lv_ET + lv_node)->cStatus = lv_nid;
		memcpy((UC *)(lv_ET + lv_node)->Data1, (UC *)lv_ndata1, DEF_NODE_DATA_SMALL_LEN);
		memcpy((UC *)(lv_ET + lv_node)->Data2, (UC *)lv_ndata2, DEF_NODE_DATA_BIG_LEN);
		
		// 处理函数
		(lv_ET + lv_node)->NodeNo = lv_nno;

		//---------------------------------------------------
		// Sun added 2004-12-30
		if( m_sysNodeData.nd_SysSendData == lv_nid )
		{
			memcpy((UC *)(&m_Data018_1[m_CurrentETID]), (UC *)lv_ndata1, DEF_NODE_DATA_SMALL_LEN);
			memcpy((UC *)(&m_Data018_2[m_CurrentETID]), (UC *)lv_ndata2, DEF_NODE_DATA_BIG_LEN);
		}
		//---------------------------------------------------

	} // End of loop

	// 关闭表
	lv_RS->Close();
	
	//3. 确定流程存储对象
	if(m_CurrentETID == 0)
	{
		/// 回收旧存储空间
		if(m_AllocFirst)
		{
			delete []ET_First;
		}
	
		/// 指向新存储空间
		ET_First = lv_ET;
		m_AllocFirst = TRUE;
	}
	else
	{
		/// 回收旧存储空间
		if(m_AllocSecond)
		{
			delete []ET_Second;
		}
	
		/// 指向新存储空间
		ET_Second = lv_ET;
		m_AllocSecond = TRUE;
	}
	//----------------------

	// Write down RCN
	SetRCN(m_CurrentETID, lv_RCN - 1);

	/// Switch ETID to another one
	AlterET();

	//----------------------

	// 成功返回
	WriteTrace(TraceInfo, "IVR group: %d updated call flow %d from DB.", m_bytGroupNO, m_IVRProgramID);
	return TRUE;
}
//----------------------------------------------------------------------------------------------

// Sun added 2012-05-07
US CIVRGroup::GetBeforeHookOnNodeID()
{
	return m_sysNodeData.nd_BeforeHookOn;
}

//----------------------------------------------------------------------------------------------
// 由流程文件读取节点数据，生成状态转换矩阵
BOOL CIVRGroup::LoadIVRProgramFromFile()
{
	FILE *lv_CallFlow;
    SCFFileHeader lv_Header;
    SCFFileRecord lv_Record;
	int lv_nRead;
	long lv_node, lv_RCN;
	BOOL lv_retval = FALSE;
	CString lv_sCaption;

	US			lv_nid;				// 节点ID 
	UC			lv_nno;				// 节点编号
	UC			lv_ndata1[DEF_NODE_DATA_SMALL_LEN];		// 节点数据段1
	UC			lv_ndata2[DEF_NODE_DATA_BIG_LEN];		// 节点数据段2
	SEventTable *lv_ET;									// 临时事件表指针

	if( m_szIVRFileName.IsEmpty() )
		return FALSE;

	try
	{
		// Open File
		if( (lv_CallFlow = fopen(m_szIVRFileName, "rb")) == NULL )
			return FALSE;

		TRACE("sizeof(SCFFileHeader):%d, sizeof(SCFFileRecord):%d, sizeof(SData2_101):%d\n", sizeof(SCFFileHeader), sizeof(SCFFileRecord), sizeof(SData2_101));
		if( !feof( lv_CallFlow ) )
		{
			memset( &lv_Header, 0x00, sizeof(SCFFileHeader) );
			lv_nRead = fread((void *)&lv_Header, sizeof(SCFFileHeader), 1, lv_CallFlow);
		}

		lv_RCN = lv_Header.P_RCN;
		if( lv_RCN >= 3 )
		{
			// Set Caption
			//lv_sCaption.Format("%s", (char *)lv_Header.P_Description);
			//if( lv_sCaption.IsEmpty() )
				SetIVRProgramCaption(m_szIVRFileName);
			//else
			//	SetIVRProgramCaption(lv_sCaption);
				
			memset( &lv_Record, 0x00, sizeof(SCFFileRecord) );
			lv_nRead = fread(&lv_Record, sizeof(SCFFileRecord), 1, lv_CallFlow);

			if( lv_Record.N_ID == 1 && lv_Record.N_NO == 0 )
			{
				//----------------------
				// 生成全程控制规则
				//----------------------
				memcpy(lv_ndata2, lv_Record.N_Data2, DEF_NODE_DATA_BIG_LEN);
				NF_000((UC *)lv_ndata2);
				lv_RCN--;
				
				//----------------------
				// 用户变量定义
				//----------------------
				memset( &lv_Record, 0x00, sizeof(SCFFileRecord) );
				lv_nRead = fread(&lv_Record, sizeof(SCFFileRecord), 1, lv_CallFlow);
				if( lv_nRead > 0 )
				{
					memcpy(lv_ndata1, lv_Record.N_Data1, DEF_NODE_DATA_SMALL_LEN);
					lv_RCN--;
					if( NF_001((UC *)lv_ndata1) <= MAXSYSNODEID )
					{
						UC lv_group = 0;

						while( lv_nRead > 0 )
						{
							memset( &lv_Record, 0x00, sizeof(SCFFileRecord) );
							lv_nRead = fread(&lv_Record, sizeof(SCFFileRecord), 1, lv_CallFlow);
							lv_RCN--;
							memcpy(lv_ndata2, lv_Record.N_Data2, DEF_NODE_DATA_BIG_LEN);
							if(NF_002(lv_group, (UC *)lv_ndata2)  <= MAXSYSNODEID)
								lv_group++;
							else
								break;
						}
					}

					//----------------------
					// 生成事件表
					//----------------------
					//1. 分配存储空间
					lv_ET = new SEventTable[lv_RCN];
					if(lv_ET == NULL)
					{
						fclose(lv_CallFlow);
						return FALSE;
					}
					memset(lv_ET,0,sizeof(SEventTable) * lv_RCN);

					lv_nid = 0;
					for(lv_node = 0; lv_node < lv_RCN; lv_node++)
					{
						if( feof( lv_CallFlow ) ||  lv_nRead == 0 )
							break;

						memset( &lv_Record, 0x00, sizeof(SCFFileRecord) );
						lv_nRead = fread(&lv_Record, sizeof(SCFFileRecord), 1, lv_CallFlow);

						// Checking Data File
						if( lv_nid > lv_Record.N_ID )
						{
							WriteTrace(TraceError, "Call flow file: %s is BAD!", m_szIVRFileName);
							fclose(lv_CallFlow);
							return FALSE;
						}

						// 取数据
						lv_nid = lv_Record.N_ID;
						lv_nno = lv_Record.N_NO;
						memcpy(lv_ndata1, lv_Record.N_Data1, DEF_NODE_DATA_SMALL_LEN);
						memcpy(lv_ndata2, lv_Record.N_Data2, DEF_NODE_DATA_BIG_LEN);

						(lv_ET + lv_node)->cStatus = lv_nid;
						memcpy((UC *)(lv_ET + lv_node)->Data1, (UC *)lv_ndata1, DEF_NODE_DATA_SMALL_LEN);
						memcpy((UC *)(lv_ET + lv_node)->Data2, (UC *)lv_ndata2, DEF_NODE_DATA_BIG_LEN);
		
						// 处理函数
						(lv_ET + lv_node)->NodeNo = lv_nno;
					}

					//3. 确定流程存储对象
					if(m_CurrentETID == 0)
					{
						/// 回收旧存储空间
						if(m_AllocFirst)
						{
							delete []ET_First;
						}
					
						/// 指向新存储空间
						ET_First = lv_ET;
						m_AllocFirst = TRUE;
					}
					else
					{
						/// 回收旧存储空间
						if(m_AllocSecond)
						{
							delete []ET_Second;
						}
					
						/// 指向新存储空间
						ET_Second = lv_ET;
						m_AllocSecond = TRUE;
					}
					//----------------------

					// Write down RCN
					SetRCN(m_CurrentETID, lv_RCN - 1);

					/// Switch ETID to another one
					AlterET();

					//----------------------
					WriteTrace(TraceInfo, "IVR group: %d loaded call flow from file: %s.", m_bytGroupNO, m_szIVRFileName);

					lv_retval = TRUE;
				}
			}
		}

		fclose(lv_CallFlow);
	}
	catch(...)
	{
		WriteTrace(TraceError, "IVR group: %d failed to load call flow from file: %s.", m_bytGroupNO, m_szIVRFileName);
		return FALSE;
	}

	// 返回
	return lv_retval;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Select an avialable Event Table for IVRs 
// SUCC:Return 1;ERROR:Return 0
// Principle:
//		1. Choose the default ET if no pointed
//		2. Otherwise, get ET from program file/table
//		3. Still choose the default ET if meet any exception
//		4. Store the final result according to current ETID 
BOOL CIVRGroup::Init_SelectET()
{
	//char errmsg[MAXMSGLEN] = "";
	BOOL retval = FALSE;

	if(m_IVRProgramID == DEF_LD_GRP_DEFAULT_ET)
	{
		// Load default ET
		LoadDefaultET();
	}
	else
	{
		// Load ET from program file/table
		if(!LoadIVRProgram())
		{
			// Something wrong: Load default ET
			LoadDefaultET();
			WriteTrace(TraceError, "Group:%d - ET:%d load failed! Default ET loaded!", m_bytGroupNO, m_IVRProgramID);
		}
		else
		{
			WriteTrace(TraceInfo, "Group:%d - New ET:%d loaded successfully!", m_bytGroupNO, m_IVRProgramID);
			retval = TRUE;
		}
	}

	// Notify main window to refresh
	PropertyChanged(SCR_GROUP_ITEM_PROGRAM);

	// Sun added 2006-03-05
	if( GetIsMonitorred() )
		MQ_SendMsg_GroupPropCallFlow();

	return retval;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// 全程转移规则
// 返回：下个节点
US CIVRGroup::NF_000(const UC *f_data2)
{
	UC	lv_ETID;						// 当前事件表
	US	lv_newResID;					// 资源项目ID

	// Get parameters
	memcpy((UC *)&m_sysNodeData, f_data2, sizeof(m_sysNodeData));
	
	lv_ETID = GetCurrentETID();
	m_TransferRule[lv_ETID][0] = m_sysNodeData.key_repeat;
	m_TransferRule[lv_ETID][1] = m_sysNodeData.key_return;
	m_TransferRule[lv_ETID][2] = m_sysNodeData.key_root;
	m_RootNodeID[lv_ETID] = m_sysNodeData.nd_root;
	lv_newResID = m_sysNodeData.ResourceProject;
	
	// 关闭流程节点日志, Sun added 2008-02-19
	m_blnLogSwitchOff = (m_sysNodeData.LogSwitchOff > 0 );

	// Sun added 2002-12-06
	/// Whether Update Resource
	if( lv_newResID != m_intResourcePrjID )
	{
		m_intResourcePrjID = lv_newResID;
		LoadResourceMap();
	}

	// Sun added 2004-12-30
	memset((UC *)(&m_Data018_1[lv_ETID]), 0x00, DEF_NODE_DATA_SMALL_LEN);
	memset((UC *)(&m_Data018_2[lv_ETID]), 0x00, DEF_NODE_DATA_BIG_LEN);

	return 2;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// 用户Buffer定义日志
// 返回：下个节点
US CIVRGroup::NF_001(const UC *f_data1)
{
	
	UC	lv_ETID;						// 当前事件表
	SData1_001 lv_data1;				// 节点数据1

	// Get parameters
	lv_ETID = GetCurrentETID();
	memcpy((UC *)&lv_data1, f_data1, sizeof(lv_data1));

	// Sun added 2002-05-21
	for( int lv_loop = 0; lv_loop < 256; lv_loop++ )
		memset(&m_UVT[lv_ETID][lv_loop], 0x00, sizeof(SUserVarDef));

	m_UserVars[lv_ETID] = lv_data1.uservars;

	if(m_UserVars[lv_ETID] > 0)
		return 3;
	else
		return m_RootNodeID[lv_ETID];
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// 用户Buffer定义变量
// 返回：下个节点
US CIVRGroup::NF_002(const UC f_group, const UC *f_data2)
{
	
	UC	lv_ETID;						// 当前事件表
	UC	lv_loop;						// 循环变量
	US	lv_nextnode;					// 下一个节点
	SData2_002 lv_data2;				// 节点数据2

	lv_ETID = GetCurrentETID();
	if(f_group > 251)
	{
		return m_RootNodeID[lv_ETID];
	}

	// Get parameters
	memcpy((UC *)&lv_data2, f_data2, sizeof(lv_data2));

	for(lv_loop = 0;lv_loop < 4;lv_loop++)
	{
		memcpy(&m_UVT[lv_ETID][f_group * 4 + lv_loop], &lv_data2.uservar[lv_loop], sizeof(SUserVarDef));
	}

	if(f_group * 4 + 4 >= m_UserVars[lv_ETID])
	{
		lv_nextnode = m_RootNodeID[lv_ETID];
	}
	else
	{
		lv_nextnode = 4 + f_group;
	}

	return lv_nextnode;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Read IVR Group Parameters from DB
BOOL CIVRGroup::InitIVRGroup()
{
	int lv_groupno = m_bytGroupNO;

	m_szCaption = (wchar_t *)PGroupInfo[lv_groupno].Caption;
	m_bEnabled = PGroupInfo[lv_groupno].Enabled;
	m_bNeedCTI = PGroupInfo[lv_groupno].NeedCTI;
	m_intStartCH = PGroupInfo[lv_groupno].StartCH;
	m_intEndCH = PGroupInfo[lv_groupno].EndCH;
	m_intFlagCH = m_intStartCH;
	m_IVRProgramID = PGroupInfo[lv_groupno].IVRProgramID;
	m_intResourcePrjID = 0;		// System Resource

	// Sun added 2005-07-10
	m_bOBOnly = PGroupInfo[lv_groupno].blnOBOnly;

	// Sun added 2006-11-30
	m_blnBindFaxResource = PGroupInfo[lv_groupno].blnBindFaxResource;

	// Sun added 2002-02-18
	m_szIVRFileName = (wchar_t *)PGroupInfo[lv_groupno].IVRFileName;

	// Sun added 2012-09-08
	SetCustID(PGroupInfo[lv_groupno].bytCustID);

	// Sun added 2002-12-06
	memset((UC *)&m_sysNodeData, 0x00, sizeof(m_sysNodeData));

	// Notify main window to refresh
	PropertyChanged();
	
	return TRUE;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Load/Refresh resources mapping
BOOL CIVRGroup::LoadResourceMap()
{
	if( !LoadResourceMap(m_intResourcePrjID) )
	{
		// load Resource from "m_intResourcePrjID.csv" file
		/// L_ID, R_ID, "R_PATH" 
		/// Must order by L_ID, R_ID

		// Open File
		FILE *lv_ResTable;
		int lv_lang, lv_rid;
		char lv_sPath[_MAX_PATH];
		char lpszFileName[_MAX_PATH];

		sprintf(lpszFileName, "%s%d.csv", theApp.m_sAppPath, m_intResourcePrjID);
		WriteTrace(TraceInfo, "Group: %s will load resources from file: %s", m_szCaption, lpszFileName);

		try
		{
			if( (lv_ResTable = fopen(lpszFileName, "rt")) != NULL )
			{
				int lv_newlang = -1, lv_index = 0;
				SResMapDef lv_ResTemp[4096];
				while( !feof( lv_ResTable ) )
				{
					memset(lv_sPath, 0x00, sizeof(lv_sPath));
					if( fscanf( lv_ResTable, "%d,%d,", &lv_lang, &lv_rid) < 2 )
						continue;
					if( ferror( lv_ResTable ) )
						break;
					if( fgets( lv_sPath, _MAX_PATH, lv_ResTable ) == NULL )
						continue;
					if( ferror( lv_ResTable ) )
						break;

					if( lv_lang < 0 || lv_rid < 0 )
						continue;

					// 分配内存以便保存资源映像表
					if( lv_newlang < lv_lang )
					{
						if( lv_newlang >= 0 )
						{
							if( m_pResMap[lv_newlang] != NULL )
							{
								delete [](m_pResMap[lv_newlang]);
								m_pResMap[lv_newlang] = NULL;
							}
							if( lv_index > 0 )
							{
								m_pResMap[lv_newlang] = new SResMapDef[lv_index];
								if(m_pResMap[lv_newlang] == NULL)
									return FALSE;
								memcpy(m_pResMap[lv_newlang], lv_ResTemp, sizeof(SResMapDef) * lv_index);
							}
							m_lngResRCN[lv_newlang] = lv_index - 1;
							lv_index = 0;
						}
						lv_newlang = lv_lang;
					}

					// Store Values
					lv_ResTemp[lv_index].R_ID = lv_rid;
					/// 特殊处理: '\' -> '\\'
					memset(lv_ResTemp[lv_index].R_Path, 0x00, 100);
					
					// Sun debug 2004-12-30
					//strDos2C_Path(strRTrim(lv_sPath), lv_ResTemp[lv_index].R_Path);
					strDos2C_Path(lv_sPath, lv_ResTemp[lv_index].R_Path);

					lv_index++;

					// COM Interface Name
					if( lv_rid == m_sysNodeData.MainCOM && lv_lang == 0 )
						m_olestrClassID = strRTrim(lv_sPath);
				}

				if( lv_newlang >= 0 )
				{
					if( m_pResMap[lv_newlang] != NULL )
					{
						delete [](m_pResMap[lv_newlang]);
						m_pResMap[lv_newlang] = NULL;
					}
					if( lv_index > 0 )
					{
						m_pResMap[lv_newlang] = new SResMapDef[lv_index];
						if(m_pResMap[lv_newlang] == NULL)
							return FALSE;
						memcpy(m_pResMap[lv_newlang], lv_ResTemp, sizeof(SResMapDef) * lv_index);
					}
					m_lngResRCN[lv_newlang] = lv_index - 1;
					lv_index = 0;
				}

				fclose(lv_ResTable);

				// Modify Resource Caption
				PropertyChanged(SCR_GROUP_ITEM_RESOURCE);
			}
		}
		catch(...)
		{
			if( lv_ResTable )
				fclose(lv_ResTable);

			WriteTrace(TraceError, "IVR group: %d failed to load resource from file: %s!", m_bytGroupNO, lpszFileName);
			return FALSE;
		}
	}

	return TRUE;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Load/Refresh resources mapping
BOOL CIVRGroup::LoadResourceMap(US f_RID)
{
	_RecordsetPtr	lv_RS;
	char lv_path[100];
	int lv_loop, lv_subloop;				// 循环遍历
	
	for( lv_loop = 0; lv_loop < MAXLANGUAGECOUNT; lv_loop++ ) 
	{
		// 通过 ADO 访问资源文件
		try
		{
			char lv_query[200];
			memset(lv_query, 0x00, sizeof(lv_query));
			sprintf(lv_query, 
				"select R_ID, R_Path from tbResource "
				"where P_ID=%u AND L_ID=%u order by R_ID", 
				f_RID, lv_loop);
			_bstr_t bstrSQL = lv_query;

			lv_RS.CreateInstance(__uuidof(Recordset));
			lv_RS->CursorType = adOpenStatic;

			// 返回结果集
			lv_RS->CursorLocation = adUseClient;
			if( !pMainWindow->m_DBReader.OpenADORecordset(lv_RS, bstrSQL) )
				return FALSE;
		}
		catch(...)
		{
			WriteTrace(TraceError, "Read table tbResource failed! R_ID = %d, L_ID = %d", f_RID, lv_loop);
			return FALSE;
		}
	
		// 分配内存以便保存资源映像表
		if(m_pResMap[lv_loop] != NULL)
		{
			delete [](m_pResMap[lv_loop]);
			m_pResMap[lv_loop] = NULL;
		}
		m_lngResRCN[lv_loop] = lv_RS->RecordCount;

		if( m_lngResRCN[lv_loop] > 0 )
		{
			m_pResMap[lv_loop] = new SResMapDef[m_lngResRCN[lv_loop]];
			if(m_pResMap[lv_loop] == NULL)
				return FALSE;
			memset(m_pResMap[lv_loop], 0x00, sizeof(SResMapDef) * m_lngResRCN[lv_loop]);
		}
	
		// 保存资源映像表
		for(lv_subloop = 0; lv_subloop < m_lngResRCN[lv_loop]; lv_subloop++)
		{
			// 读取保存
			(m_pResMap[lv_loop] + lv_subloop)->R_ID = lv_RS->Fields->Item["R_ID"]->Value.iVal;

			try
			{
				if( lv_RS->Fields->Item["R_Path"]->ActualSize > 0 )
				{
					_bstr_t lv_str = lv_RS->Fields->Item["R_Path"]->Value.bstrVal;
					strncpy(lv_path, (const char *)lv_str, 100);

					// COM Interface Name
					if( (m_pResMap[lv_loop] + lv_subloop)->R_ID == m_sysNodeData.MainCOM && lv_loop == 0 )
						m_olestrClassID = strRTrim(lv_path);
				}
			}
			catch(...)
			{
			}
			
			// 特殊处理: '\' -> '\\'
			strDos2C_Path(lv_path, (m_pResMap[lv_loop] + lv_subloop)->R_Path);

			// 下一条记录
			lv_RS->MoveNext();
		}
	
		// 关闭表
		lv_RS->Close();
		m_lngResRCN[lv_loop]--;			// Sun added 2001-05-24 because former probabaly cause a out of bound error
	}
	
	// Modify Resource Caption
	PropertyChanged(SCR_GROUP_ITEM_RESOURCE);
	WriteTrace(TraceInfo, "IVR group: %d updated resource: %d from DB!", m_bytGroupNO, f_RID);

	// Sun added 2006-03-05
	if( GetIsMonitorred() )
		MQ_SendMsg_GroupPropResource();

	return TRUE;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Send windows user message on codition of attributes changed
void CIVRGroup::PropertyChanged(UC nItemChanged, BOOL blnRefresh)
{
	if( pMainWindow->m_bRunning )
		::PostMessage(pGroupView->m_hWnd, MYWM_CHANGE_GROUP, m_bytGroupNO, nItemChanged);
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Sun added 2007-11-04
UL CIVRGroup::GetMinGCTIT()
{
	return m_lngMinGCTIT;
}

void CIVRGroup::SetMinGCTIT(UL lngSeconds)
{
	m_lngMinGCTIT = lngSeconds;
}

// 计算＂组通道通话最小间隔＂
void CIVRGroup::CalculateGCTIT()
{
	US lv_loop;
	UL lv_nMinGCTIT = 0, lv_nChTIT;

	for( lv_loop = m_intStartCH; lv_loop <= m_intEndCH; lv_loop++ )
	{
		if( PIVRCH[lv_loop] != NULL )
		{
			lv_nChTIT = PIVRCH[lv_loop]->GetChannelTIT();
			if( lv_nChTIT > 0 )
			{
				if( lv_nMinGCTIT > lv_nChTIT || lv_nMinGCTIT == 0 )
					lv_nMinGCTIT = lv_nChTIT;
			}
		}
	}

	SetMinGCTIT(lv_nMinGCTIT);
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Start the Group
BOOL CIVRGroup::StartGroup(BOOL f_blnSysRecovery)
{
	int lv_loop;

	// Sun added 2008-05-29
	m_dwLastCallDistribute = GetTickCount();

	if( GetEnabled() && !GetStarted() )
	{
#ifdef CISCO_CCM
		if( m_dwRoutePointID > 0 && CTapiRoutePoint::IsInitialized() )
		{
			if( CTapiRoutePoint::Open() != 0 )
			{
				WriteTrace(TraceError, "Error - Failed to open group: %d!", GetGroupNO());
				return FALSE;
			}

			// Add group into device list
			pChannelView->m_oIVRCHMap.SetAt(CTapiRoutePoint::GetLineHandle(), this);
			WriteTrace(TraceWarn, "IVR Group: %d bind with TapiLine: 0x%x.", GetGroupNO(), CTapiRoutePoint::GetLineHandle());
		}
#endif

		// Sun move it front of open channels
		SetStarted(TRUE);

		// Sun added 2006-02-26
		if( !f_blnSysRecovery )
		{
			m_DailyCallCount.SetValue(0);
			m_YTDCallCount.SetValue(0);
			PropertyChanged(SCR_GROUP_ITEM_CALLCOUNT);
		}
	}

	// Sun moved 2007-11-04, from 'if( GetEnabled() && !GetStarted() )'
	/// 防止重新划分通道时，新分配来的通道没有被启动
	if( GetEnabled() )
	{
		try
		{
			for( lv_loop = m_intStartCH; lv_loop <= m_intEndCH; lv_loop++ )
			{
				if( PIVRCH[lv_loop] != NULL )
				{
					// Sun added condition 2007-11-04
					if( !PIVRCH[lv_loop]->GetStarted() )
					{
						if( PIVRCH[lv_loop]->StartCh() )
							Sleep(10);
					}
				}
			}
		}
		catch(...)
		{
			return FALSE;
		}
	}

	return TRUE;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Stop the Group
BOOL CIVRGroup::StopGroup(BOOL f_blnSysShutDown)
{
	int lv_loop;

	// Sun replaced 2007-11-04
	// 避免在修改组Ｅｎａｂｌｅ属性时，无法关闭组和下属通道
	/// From
	//if( GetEnabled() && GetStarted() )
	// To
	if( GetStarted() )
	{
		// Sun move it fron of close channels
		SetStarted(FALSE);

#ifdef CISCO_CCM
		if( m_dwRoutePointID > 0 && CTapiRoutePoint::IsOpened() )
		{
			if( f_blnSysShutDown )
			{
				// Remove group from device list
				pChannelView->m_oIVRCHMap.RemoveKey(CTapiRoutePoint::GetLineHandle());
				WriteTrace(TraceWarn, "IVR Group: %d unbind with TapiLine: 0x%x.", GetGroupNO(), CTapiRoutePoint::GetLineHandle());

				CTapiRoutePoint::ShutDown();
			}
			else
			{
				// Sun added 2007-12-12
				// Remove group from device list
				pChannelView->m_oIVRCHMap.RemoveKey(CTapiRoutePoint::GetLineHandle());
				WriteTrace(TraceWarn, "IVR Group: %d unbind with TapiLine: 0x%x.", GetGroupNO(), CTapiRoutePoint::GetLineHandle());
				
				CTapiRoutePoint::Close();
			}
		}
#endif

		try
		{
			for( lv_loop = m_intStartCH; lv_loop <= m_intEndCH; lv_loop++ )
			{
				if( PIVRCH[lv_loop] != NULL )
				{
					// Sun added condition 2007-11-04
					if( PIVRCH[lv_loop]->GetStarted() )
						PIVRCH[lv_loop]->StopCh();
				}
			}
		}
		catch(...)
		{
			return FALSE;
		}
	}

	return TRUE;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Sun added 2007-07-11
#ifdef CISCO_CCM

// 按轮询次序将来电分派给空闲通道
long CIVRGroup::DistributeCallToChannel(DWORD dwCall)
{
	long ret = -1;

	// Sun added 2010-08-05
	if( !pMainWindow->m_bRunning )
		return -1;

	// Sun added 2011-12-01
	if( !GetStarted() )
		return -1;

	// Sun added 2008-05-29
	m_dwLastCallDistribute = GetTickCount();

	CIVRChannel *lv_pCH = MoveToNextFlagCH();

	if( lv_pCH )
	{
		ret = CTapiRoutePoint::RedirectCall(dwCall, lv_pCH->GetExtensionNo());
		if( ret > 0 )
			lv_pCH->m_oARS.SetDistributeRequest((DWORD)ret);
		else
			WriteTrace(TraceError, "IVR Group: %d failed to DistributeCall: 0x%x To IVR Channel: %d, error = 0x%x.", GetGroupNO(), dwCall, lv_pCH->GetLineNO(), ret);
	}
	//---------------------------------------------
	// Sun added 2010-07-20 [C]
	else
	{
		CTapiRoutePoint::DropCall();
	}
	//---------------------------------------------

	return ret;
}

#endif
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Sun added 2006-03-05
BOOL CIVRGroup::MQ_SendMsg_GroupMonitorAck(BOOL f_blnMonitor, UC f_bytResult)
{
	BOOL lv_retval = FALSE;
	CClientThread *lv_pThread = (CClientThread *)(pMainWindow->m_IVRAdmin.GetConnected());
	if( lv_pThread == NULL )
		return FALSE;

	try
	{
		SMsgHeader lv_sndhdr;
		S_TCP_IVR2ADM_GROUP_MON_ACK lv_sndbdy;
		BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

		lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
		lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
		lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
		lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
		lv_sndhdr.Sender = USER_IVR;
		lv_sndhdr.Receiver = USER_CTI;

		memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));
		if( f_blnMonitor )
			lv_sndhdr.Command = TCP_IVR2ADM_GROUP_MON_ACK;
		else
			lv_sndhdr.Command = TCP_IVR2ADM_GROUP_UNMON_ACK;
		lv_sndhdr.PackageLen = sizeof(lv_sndhdr) + sizeof(lv_sndbdy);

		lv_sndbdy.GroupID = m_bytGroupNO;
		lv_sndbdy.ErrorCode = f_bytResult;

		// Send Message
		memcpy(lv_sndmsg, &lv_sndhdr, sizeof(lv_sndhdr));
		memcpy(lv_sndmsg+sizeof(lv_sndhdr), &lv_sndbdy, sizeof(lv_sndbdy));

		lv_retval = lv_pThread->m_ClientSocket.AddMessage(lv_sndmsg, lv_sndhdr.PackageLen);

		if( lv_retval && f_blnMonitor )
			MQ_SendMsg_GroupPropName();
	}
	catch(...)
	{
		lv_retval = FALSE;
		WriteTrace(TraceError, "IVR Group:%d failed when call MQ_SendMsg_GroupMonitorAck()", m_bytGroupNO);
	}

	return lv_retval;
}

BOOL CIVRGroup::MQ_SendMsg_GroupPropName()
{
	BOOL lv_retval = FALSE;
	CClientThread *lv_pThread = (CClientThread *)(pMainWindow->m_IVRAdmin.GetConnected());
	if( lv_pThread == NULL )
		return FALSE;

	try
	{
		SMsgHeader lv_sndhdr;
		S_TCP_IVR2ADM_GROUP_PROP_NAME lv_sndbdy;
		BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

		lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
		lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
		lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
		lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
		lv_sndhdr.Sender = USER_IVR;
		lv_sndhdr.Receiver = USER_CTI;

		memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));
		lv_sndhdr.Command = TCP_IVR2ADM_GROUP_PROP_NAME;
		lv_sndhdr.PackageLen = sizeof(lv_sndhdr) + sizeof(lv_sndbdy);

		lv_sndbdy.GroupID = m_bytGroupNO;
		lv_sndbdy.CTIEnabled = (m_bNeedCTI ? 1 : 0);
		lv_sndbdy.StartChannel = GetStartCH();
		lv_sndbdy.EndChannel = GetEndCH();
		lv_sndbdy.CallFlowID = m_IVRProgramID;
		lv_sndbdy.ResourceID = m_intResourcePrjID;
		lv_sndbdy.DailyCallCount = (UL)(m_DailyCallCount.GetValue());
		lv_sndbdy.YTDCallCount = (UL)(m_YTDCallCount.GetValue());
		strncpy(lv_sndbdy.GroupName, (LPCSTR)m_szCaption, sizeof(lv_sndbdy.GroupName)-1);
		strncpy(lv_sndbdy.CallFlowName, (LPCSTR)m_szIVRProgramCaption, sizeof(lv_sndbdy.CallFlowName)-1);

		// Send Message
		memcpy(lv_sndmsg, &lv_sndhdr, sizeof(lv_sndhdr));
		memcpy(lv_sndmsg+sizeof(lv_sndhdr), &lv_sndbdy, sizeof(lv_sndbdy));

		lv_retval = lv_pThread->m_ClientSocket.AddMessage(lv_sndmsg, lv_sndhdr.PackageLen);
	}
	catch(...)
	{
		lv_retval = FALSE;
		WriteTrace(TraceError, "IVR Group:%d failed when call MQ_SendMsg_GroupPropName()", m_bytGroupNO);
	}

	return lv_retval;
}

BOOL CIVRGroup::MQ_SendMsg_GroupPropCallFlow()
{
	BOOL lv_retval = FALSE;
	CClientThread *lv_pThread = (CClientThread *)(pMainWindow->m_IVRAdmin.GetConnected());
	if( lv_pThread == NULL )
		return FALSE;

	try
	{
		SMsgHeader lv_sndhdr;
		S_TCP_IVR2ADM_GROUP_PROP_CALLFLOW lv_sndbdy;
		BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

		lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
		lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
		lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
		lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
		lv_sndhdr.Sender = USER_IVR;
		lv_sndhdr.Receiver = USER_CTI;

		memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));
		lv_sndhdr.Command = TCP_IVR2ADM_GROUP_PROP_CALLFLOW;
		lv_sndhdr.PackageLen = sizeof(lv_sndhdr) + sizeof(lv_sndbdy);

		lv_sndbdy.GroupID = m_bytGroupNO;
		lv_sndbdy.CallFlowID = m_IVRProgramID;
		strncpy(lv_sndbdy.CallFlowName, (LPCSTR)m_szIVRProgramCaption, sizeof(lv_sndbdy.CallFlowName)-1);

		// Send Message
		memcpy(lv_sndmsg, &lv_sndhdr, sizeof(lv_sndhdr));
		memcpy(lv_sndmsg+sizeof(lv_sndhdr), &lv_sndbdy, sizeof(lv_sndbdy));

		lv_retval = lv_pThread->m_ClientSocket.AddMessage(lv_sndmsg, lv_sndhdr.PackageLen);
	}
	catch(...)
	{
		lv_retval = FALSE;
		WriteTrace(TraceError, "IVR Group:%d failed when call MQ_SendMsg_GroupPropCallFlow()", m_bytGroupNO);
	}

	return lv_retval;
}

BOOL CIVRGroup::MQ_SendMsg_GroupPropResource()
{
	BOOL lv_retval = FALSE;
	CClientThread *lv_pThread = (CClientThread *)(pMainWindow->m_IVRAdmin.GetConnected());
	if( lv_pThread == NULL )
		return FALSE;

	try
	{
		SMsgHeader lv_sndhdr;
		S_TCP_IVR2ADM_GROUP_PROP_RESOURCE lv_sndbdy;
		BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

		lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
		lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
		lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
		lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
		lv_sndhdr.Sender = USER_IVR;
		lv_sndhdr.Receiver = USER_CTI;

		memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));
		lv_sndhdr.Command = TCP_IVR2ADM_GROUP_PROP_RESOURCE;
		lv_sndhdr.PackageLen = sizeof(lv_sndhdr) + sizeof(lv_sndbdy);

		lv_sndbdy.GroupID = m_bytGroupNO;
		lv_sndbdy.ResourceID = m_intResourcePrjID;

		// Send Message
		memcpy(lv_sndmsg, &lv_sndhdr, sizeof(lv_sndhdr));
		memcpy(lv_sndmsg+sizeof(lv_sndhdr), &lv_sndbdy, sizeof(lv_sndbdy));

		lv_retval = lv_pThread->m_ClientSocket.AddMessage(lv_sndmsg, lv_sndhdr.PackageLen);
	}
	catch(...)
	{
		lv_retval = FALSE;
		WriteTrace(TraceError, "IVR Group:%d failed when call MQ_SendMsg_GroupPropResource()", m_bytGroupNO);
	}

	return lv_retval;
}

BOOL CIVRGroup::MQ_SendMsg_ChannelStart(US f_nChannelID, LPCSTR f_sANI)
{
	BOOL lv_retval = FALSE;
	CClientThread *lv_pThread = (CClientThread *)(pMainWindow->m_IVRAdmin.GetConnected());
	if( lv_pThread == NULL )
		return FALSE;

	try
	{
		SMsgHeader lv_sndhdr;
		S_TCP_IVR2ADM_CH_ST_START lv_sndbdy;
		BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

		lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
		lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
		lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
		lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
		lv_sndhdr.Sender = USER_IVR;
		lv_sndhdr.Receiver = USER_CTI;

		memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));
		lv_sndhdr.Command = TCP_IVR2ADM_CH_ST_START;
		lv_sndhdr.PackageLen = sizeof(lv_sndhdr) + sizeof(lv_sndbdy);

		lv_sndbdy.GroupID = m_bytGroupNO;
		lv_sndbdy.ChannelID = f_nChannelID;
		strcpy(lv_sndbdy.strANI, f_sANI);

		// Send Message
		memcpy(lv_sndmsg, &lv_sndhdr, sizeof(lv_sndhdr));
		memcpy(lv_sndmsg+sizeof(lv_sndhdr), &lv_sndbdy, sizeof(lv_sndbdy));

		lv_retval = lv_pThread->m_ClientSocket.AddMessage(lv_sndmsg, lv_sndhdr.PackageLen);
	}
	catch(...)
	{
		lv_retval = FALSE;
		WriteTrace(TraceError, "IVR Group:%d failed when call MQ_SendMsg_ChannelStart(%d, %s)", m_bytGroupNO, f_nChannelID, f_sANI);
	}

	return lv_retval;
}

BOOL CIVRGroup::MQ_SendMsg_ChannelStop(US f_nChannelID)
{
	BOOL lv_retval = FALSE;
	CClientThread *lv_pThread = (CClientThread *)(pMainWindow->m_IVRAdmin.GetConnected());
	if( lv_pThread == NULL )
		return FALSE;

	try
	{
		SMsgHeader lv_sndhdr;
		S_TCP_IVR2ADM_CH_ST_STOP lv_sndbdy;
		BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

		lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
		lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
		lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
		lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
		lv_sndhdr.Sender = USER_IVR;
		lv_sndhdr.Receiver = USER_CTI;

		memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));
		lv_sndhdr.Command = TCP_IVR2ADM_CH_ST_STOP;
		lv_sndhdr.PackageLen = sizeof(lv_sndhdr) + sizeof(lv_sndbdy);

		lv_sndbdy.GroupID = m_bytGroupNO;
		lv_sndbdy.ChannelID = f_nChannelID;

		// Send Message
		memcpy(lv_sndmsg, &lv_sndhdr, sizeof(lv_sndhdr));
		memcpy(lv_sndmsg+sizeof(lv_sndhdr), &lv_sndbdy, sizeof(lv_sndbdy));

		lv_retval = lv_pThread->m_ClientSocket.AddMessage(lv_sndmsg, lv_sndhdr.PackageLen);
	}
	catch(...)
	{
		lv_retval = FALSE;
		WriteTrace(TraceError, "IVR Group:%d failed when call MQ_SendMsg_ChannelStop(%d)", m_bytGroupNO, f_nChannelID);
	}

	return lv_retval;
}
//----------------------------------------------------------------------------------------------