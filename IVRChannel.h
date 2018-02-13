///////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		IVRChannel.h
// 用途描述：	IVR通道类声明
// 作者：		SUN
// 创建时间：	2001/06/28
// 最后修改时间：2002/03/15
// 版权信息：	Copyright (C) 1999 - 2000 Changjiang Xincheng
// 备注：Multithread
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if !defined(AFX_IVRCHANNEL_H__179953BD_8690_4EA7_B581_23FC2085979C__INCLUDED_)
#define AFX_IVRCHANNEL_H__179953BD_8690_4EA7_B581_23FC2085979C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "ManulCallCom.h"
#include "IVRGroup.h"
#include "channelcontrol.h"
#include "FaxResource.h"
#include "..\CommonClass\PCSCommunicationEvent.h"

#define NODE_RESULT_ERROR		0
#define NODE_RESULT_OK			1

//------------------------------------------------------------------------
// Sun added 2010-01-08
// UMS Timeout Define
#define PCS_TO_UMS_HOOKONSTEP	100		// 挂机后处理节点超时
#define PCS_TO_UMS_EXTMSG		500		// 异步通信超时
#define PCS_TO_UMS_QUERYAGTST	4000	// 查询坐席状态超时
#define PCS_TO_UMS_QUERYRPT		2000	// 查询路有点超时
#define PCS_TO_UMS_ADDTASK		6000	// 添加任务超时
#define PCS_TO_UMS_WAITCONF		16000	// 等待会议磋商完成超时
#define PCS_TO_UMS_MINQUEUE		4		// 最小排队时长（秒）

//------------------------------------------------------------------------
// 电话话叫处理常数定义
#ifndef TELE_HOOKON
#define TELE_HOOKON			0			// 挂机
#endif

#ifndef TELE_HOOKOFF
#define TELE_HOOKOFF		1			// 摘机
#endif

#define TERM_TYPE_PLAY			0
#define TERM_TYPE_GETKEY		1
#define TERM_TYPE_RECORD		2
#define TERM_TYPE_FAX			3
#define TERM_TYPE_SWITCH		4

// Sun added 2008-11-17
#define PCS_MAX_VAR_LEN			255

#include "..\CommonClass\MyThread.h"
#include "ASRPackage\ASRInterface.h"	// Sun added 2008-03-10

// Sun added 2010-07-20
class CMyHookThread : protected CMyThread
{
public:
	CMyHookThread();
	virtual ~CMyHookThread();

	BOOL WaitForStartupToComplete(DWORD dwObj);

protected:
	DWORD m_dwObject;
	virtual int Run();
};

class CIVRChannel
	: public CIVRObject
	, protected CMyThread
{
public:
	BOOL ExecuteSQLStatement(CString f_SQL);
	BOOL IVRStatisticsFunction();
	BOOL InsertIVRCDR();

	BOOL			GetNeedCTI();
	BOOL			GetCTIOpened();
	BOOL			GetCTIAvailble();
	void			SetCTIOpened(BOOL blnValue);

	// Sun addded 2006-02-26
	long GetDailyCallCount();
	long GetYTDCallCount();
	void ResetDailyCallCount();
	void ResetYTDCallCount();
	void IncreaseCallCount();
	
	// Sun addded 2005-03-18
	CPCSCommunicationEvent m_CommEvent;
	CPCSCommunicationEvent m_OBEvent;

	// Sun added 2006-09-15
	CTime m_tmOBStartTime;					// 开始拨号时间
	DWORD m_nOBWaitTime;					// 外拨等待时长

	// Sun added 2005-07-10
	CPCSCommunicationEvent m_QueryEvent;

	// Sun added 2012-05-07
	CPCSCommunicationEvent m_HookOnWaitEvent;

	// Sun added 2002-08-06
	long				m_callrefno;

	// Sun added 2002-03-01
	Cchannelcontrol		m_oARS;

	// Sun added 2007-07-12
	CString				m_strTaskID;
	BOOL				m_blnTaskClosed;

	// Sun added 2011-06-25
	CString				m_strRecordTaskID;	// 用于记录数据库，因为m_strTaskID可能中途被清理

	// Sun added 2002-02-28
	char m_szChannelName[50];
	char m_szDTIName[50];
	char m_szFaxName[50];		// Sun added 2012-05-24

	BOOL CTI_ErrorHandler(BOOL f_bError = TRUE);
	void CTI_ClearCall();
	void CTI_UnsetChannelError();
	void CTI_SetChannelError();
	long CTI_SetGCCRN(long f_CRN);
	long CTI_GetGCCRN();
	int ReadDigitsEx(double f_digit = 0);
	int CheckUserPassword(UC *f_uid, UC *f_pwd, US f_comid = 0);
	int UpdateUserPassword(UC *f_uid, UC *f_pwd, US f_comid);
	BOOL SwitchToStationEx(UC f_type = 0, UL f_workno = 0, bool f_directdail = false);
	UC GetOneKey();

	BOOL OpenCh();
	BOOL CloseCh();
	BOOL StartCh();
	BOOL StopCh();

	CIVRChannel();
	virtual ~CIVRChannel();
	CIVRGroup*		m_objGroup;				// Channel belongs to this Group Object

	void Destory();
	US GetExitCode() { return m_ExitCode; };
	void SetExitCode(US f_exitcode);
	void NF_CallIn(UC f_ServiceCode = 0, US f_StartNode = 0);

	// Go and run node
	BOOL NF_GotoNode(US event, US f_node = 0);

	// Create IVR Channel
	void Create( US lineno );

	// Initialize
	BOOL InitCh();

	// Read lineno
	US GetLineNO() { return m_intLineNO; }

	// Read & Write enable state
	BOOL GetEnabled() { return m_bEnabled; }
	void SetEnabled(BOOL blnEnabled = TRUE) { m_bEnabled = blnEnabled; }

	// Read & Write Device No (CTI AgentID)
	CString GetDeviceNo() { return m_sDeviceNo; }
	void SetDeviceNo(CString f_sDeviceNo) { m_sDeviceNo = f_sDeviceNo; }

	// Read & Write Extension#
	CString GetExtensionNo() { return m_sExtensionNo; }
	void SetExtensionNo(LPCSTR f_sDeviceNo) { m_sExtensionNo = f_sDeviceNo; }

	// Read & Write start state
	BOOL GetStarted() { return m_bStarted; }
	void SetStarted(BOOL blnStarted = TRUE);

	// Sun added 2008-05-16
	UL GetChannelCreateDuration();
	UL GetChannelStartDuration();

	// Read In call status
	BOOL GetIsInCall() { return m_bInboundCall; }
	BOOL GetIsOutCall() { return m_bOutboundCall; }
	
	// Sun added 2010-07-22
	BOOL GetIsConferenced();
	void SetIsConferenced(BOOL f_sw);

	// Read & Write Call Reference ID
	UL GetCallRefID() { return m_lngCurCallID;}
	void SetCallRefID(UL nCallID, UC bytEventSource, UC bytCallSource, US nWaitTime, const char *strTimeStamp);

	// Read & Write New Call Reference ID
	UL GetNewCallRefID() { return m_lngNewCallID;}
	void SetNewCallRefID(UL nCallID = 0);

	// Sun added 2011-10-02
	void SetSwitchResult(char f_chValue);

	// Read Open State
	BOOL GetChOpend() { return m_bOpened;}

	// Read In Queue State
	bool GetIsInQueue() { return m_bInQueue; }
	// Read In Queue Tick Time
	DWORD GetQueueTime() { return m_dwQueueTime; }

	// Read & Write Max Waiting Period
	US GetMaxQueueTime() { return m_iMaxQueuedTime; }
	void SetMaxQueueTime(US iMaxQueuedTime = 120) { m_iMaxQueuedTime = iMaxQueuedTime; }
	
	// Read & Write Max Try Times
	UC GetMaxTryTimes() { return m_bytMaxTrys; }
	void SetMaxTryTimes(UC bytMaxTrys = 3) { m_bytMaxTrys = bytMaxTrys; }

	// Read & Write Group#
	UC GetGroupNO() { return m_bytGroupNo; }
	void SetGroupNO(UC bytGroupNO = 0) 
	{ 
		m_bytGroupNo = bytGroupNO; 
	}

	// Read & Write Current ET#
	UC GetCurETID() { return m_cET; }
	void SetCurETID(UC bytETID = 0) { m_cET = bytETID; }

	// Read & Write Channel Status
	UC GetStatus();
	void SetStatus(UC bytStatus = 0);

	// Sun added 2006-03-05
	BOOL GetIsMonitorred();
	void SetIsMonitorred(BOOL f_Value);
	US GetForceNextNode();
	void SetForceNextNode(US f_NodeID);

	// Read & Write Max Play Time
	US GetMaxPlayTime() { return m_intMaxPlayTime; }
	void SetMaxPlayTime(US intMaxPlayTime = 120) 
	{ 
		if( intMaxPlayTime < 5 && intMaxPlayTime > 0 )
			m_intMaxPlayTime = 120;
		else
			m_intMaxPlayTime = intMaxPlayTime; 
	}

	// Sun added 2006-10-01
	US GetMaxFaxTime() { return m_intMaxFaxTime; }
	void SetMaxFaxTime(US intMaxFaxTime = 600) 
	{ 
		if( intMaxFaxTime < 60 && intMaxFaxTime > 0 )
			m_intMaxFaxTime = 600;
		else
			m_intMaxFaxTime = intMaxFaxTime; 
	}

	// Read & Write Channel Time
	DWORD GetBeginTime(void) { return m_dwBegin; }
	
	/// Sun added 2002-05-22
	void GetBeginTime(CTime &f_ctTime) { f_ctTime = m_ctCallInTime; }
	void SetBeginTime();
	UL GetDuration();

	// Sun added 2007-11-04
	UL GetChannelTIT();

	int ProcessTimer(void);							// Time out process

	// Set IVR Channel to Initial State
	BOOL SetHookOn(UC f_oncommand = TELE_HOOKON);

	// Sun added 2010-07-20 [D]
	// 异步挂断
	void AnsySetHookOn(BOOL f_blnAnsy = TRUE);

	// Sun added 2013-01-27 [A]
	/// 发送任务离队消息
	BOOL CancelChannelUMSTask();

	// Sun added 2012-08-02
	BOOL SmartSleep(DWORD dwTimeout, DWORD dwInterval = 20);

	// CallAnswerDelay
	int NF_SetCallAnswerDelay(US f_time);			// 设置应答延时
	US NF_GetCallAnswerDelay();						// 获取应答延时

	// ChannelRestartTime
	int NF_SetChannelRestartTime(US f_time);		// 设置通道复位时间
	US NF_GetChannelRestartTime();					// 获取通道复位时间

	// Program Run
	int NF_SetNextNode(US f_node);					// 设置下一个节点ID
	US NF_GetNextNode();							// 获取下一个节点ID

	// App Data
	UC *NF_GetAppData(void);						// 获取App数据
	void NF_SetAppData(UC *f_pData);				// 设置App数据

	// User Data
	char *NF_GetUserData(void);						// 获取User数据
	void NF_SetUserData(LPCSTR f_pData, int f_nLen);			// 设置User数据

	// Ext Data
	char *NF_GetExtData(void);						// 获取ExtData
	void NF_SetExtData(LPCSTR f_pData, int f_nLen);	// 设置ExtData

	// OB Message Tel No.
	CString NF_GetOBMsgTelNo(void);					// 获取外拨消息设置的号码
	void NF_SetOBMsgTelNo(LPCSTR f_pTelNo);			// 外拨消息设置外拨号码

	// OB ID
	UC NF_GetOBResult();
	UL NF_GetOBPacketID();
	void NF_SetOBPacketID(UL f_nID);
	US NF_GetOBCampaignID();
	void NF_SetOBCampaignID(US f_nID);
	UL NF_GetOBTaskID();
	void NF_SetOBTaskID(UL f_nID);

	// Calling Card
	BYTE NF_GetCCDialProgress();
	BOOL NF_GetCCTalking();
	void NF_SetCCTalking(BOOL f_blnValue);

	// Language ID
	UC NF_GetLanguageID(void);						// 获取语言
	void NF_SetLanguageID(UC f_gid);				// 设置语言

	// Reason for State Changed
	int NF_SetNodeEvent(US f_event);				// 设置状态转移事件
	US NF_GetNodeEvent();							// 获取状态转移事件

	// Get/Set Callerid
	UC *NF_GetCallerID(void);
	void NF_SetCallerID(UC *f_pno);

	// Get/Set Calleeid
	UC *NF_GetCalleeID(void);
	void NF_SetCalleeID(UC *f_pno);

	// Get Amount of Records
	int GetRecordNumber(LPCSTR strFormat, US agentid = 0);
	// Delete Current Record File
	int DeleteRecordFile(char *f_FN);
	// Close Current Record File
	int CloseRecordFile(char *f_FN);

	// Play Voice
	int PlayVoiceEx(US vox_id, UC key = 'A', bool clrdigitbuf = true, int nTimeout = 0);
	int PlayVoiceFile(LPCSTR strFile, UC key = 'A', bool clrdigitbuf = true, int nTimeout = 0);
	// Play TTS Buffer
	int TTSPlayBufferEx(LPSTR pTextData, UC key = 'A', bool clrdigitbuf = true);
	// Play Record
	int PlayRecordEx(CString &f_FN, bool f_blnCloseAfterChecked = false);
	// Get Key Input
	int GetDigitEx(US vox_id, UC digitlen, char *pGetKeys, UC timeout = 10, UC timeinterval = 5, UC key = 'F', bool clrdigitbuf = true);
	int GetDigitEx(LPCSTR strFile, UC digitlen, char *pGetKeys, UC timeout, UC timeinterval, UC key, bool clrdigitbuf);
	// Record to File
	int RecFileEx(US agentid, UC var_filename, LPCSTR appfield1, LPCSTR appfield2, LPCSTR appfield3, US rectime = 120, UC siltime = 5, bool toneon = true, UC recfiletype = 0, UC nMinRecDur = 5, UC nVmsClass = 0, UC nNotifyIntvl = 0, UC var_rectime = 0, US voxid = 0, UC key = 'A', UC playclear = 1, bool blnNotifyPL = false, UC bytUserID = 0);
	// Send Fax File
	int SendFaxEx(US fax_id, US header_id = 0, US from_id = 0, int nRetryTimes = 0);
	// Stop Channel
	BOOL SetChannelStop();

	// Sun added 2012-11-23
	BOOL GetIsAsynPlaying();
	void SetIsAsynPlaying(BOOL f_blnValue);

	// Send Fax
	BOOL SendFaxFile(const char *f_pFilePath, const char *f_strFromNo = NULL, const char *f_strToNo = NULL, const char *f_strHeader = NULL, BYTE *f_pResult = NULL, int nRetryTimes = 0);

	// Receive Fax
	BOOL ReceiveFaxFile(const char *f_pFilePath, char *f_strFromNo = NULL, const char *f_strToNo = NULL, BYTE *f_pResult = NULL, int nRetryTimes = 0);

	void PropertyChanged(BYTE nItemChanged = 255, BOOL blnRefresh = FALSE);

	// Send IVR message to External System
	BOOL SendExtOBResult(UC f_cResult);
	BOOL SendExternMsg(US f_cmd, UC f_sep, const char *f_msg = NULL, int f_nDataLen = 0);

	// Sun added 2003-05-23
	/// Initialize COM Object
	BOOL CreateComObject(LPOLESTR pszProgID);

	BOOL MQ_SendMsg_InitChannel();
	BOOL MQ_SendMsg_DropChannel();

	//------------------------------------------------------------------------
	// Sun added 2007-01-16
	/// IVR 测试标记
	BOOL GetIVRTestFlag();
	void SetIVRTestFlag(BOOL f_blnFlag);

	// 不记录CDR标记
	BOOL GetIVRTestNOCDR();
	void SetIVRTestNOCDR(BOOL f_blnSW);

	// 测试时间
	BOOL GetIVRTestTime(CTime &f_tmTestTime);
	void SetIVRTestTime(LPCSTR f_strTime);
	//------------------------------------------------------------------------

	//------------------------------------------------------------------------
	// Sun added 2007-07-11
	void LockFlagData(BOOL blnLock = TRUE);
	DWORD GetQueueWaitEventID();
	void SetQueueWaitEventData(BYTE *f_pData, size_t nSize);
	void ResetQueueWaitEvent(DWORD dwEventID);
	void SetQueueWaitEvent(DWORD dwEventID);

	int NF_N61_QueueWaiting(DWORD nTimeOut = INFINITE);
	//------------------------------------------------------------------------

protected:
	BOOL			m_bCTIOpened;
	
	BOOL			m_bInCallFlow;		// Sun added 2010-07-20 [E]

	UC				m_cET;				// Current ET(0 or 1)
	DWORD			m_dwBegin;			// 通话开始(Tick Count)
	DWORD			m_dwTickTime;		// 操作定时(Tick Count)
	DWORD			m_dwQueueTime;		// 排队定时(Tick Count)
	DWORD			m_dwWaitAgentAns;	// 等待坐席应答定时(Tick Count)，Sun added 2007-07-15
	DWORD			m_dwIdleTick;		// Idle begin tickcount
	
	/// Sun added 2002-05-22
	CTime			m_ctCallInTime;		// 通话开始时间(CTime)
	DWORD			m_dwWaitBegin;		// 转接等待开始(Tick Count)
	char			m_cCallType;		// 呼叫类型
										// '1' - IVR Call; '2' - Transfer but failed; '3' - Transfer successfully
	CString			m_szTransferDest;	// 转移目的

	UC				m_bytStatus;		// Channel Status
	US				m_ExitCode;			// Return Code
	US				m_iMaxQueuedTime;	// 最长排队等待时间(秒)
	bool			m_bInQueue;			// 进入ACD系统

	int				m_nAnswerTimeOut;	// 是否是通道应答超时，Sun added 2007-05-29
	CFaxResource	*m_pFaxRes;			// 传真资源

	// 信道数据实体(1KBs)
	struct SChData{
		US		nextnode;					// 后续节点ID
		US		visitlog;					// 被访问记录
		UC		callerid[20];				// 主叫号码
		UC		calleeid[20];				// 被叫号码
		US		event;						// 事件
		UC		lastchoice[64];				// 用户最近按键选择
		UC		callintime[14];				// 打入时间
		US		calllength;					// 通话时长
		UC		calltype;					// 呼叫类型
		UC		mainservice;				// 主要业务
		UC		callback;					// 回呼标记
		UC		language;					// 语言编号
		UC		customer[20];				// 客户编号
		UC		password[10];				// Password
		UC		appdata[64];				// App数据
		UL		callrefid;					// 呼叫标识
		UC		submenu;					// 二级菜单 
		UC		thirdmenu;					// 三级菜单
		UL		channelid;					// IVR线路号
		UC		reserved[22];				// 保留，Sun updated 2012-03-09 from [26] to [22]
		UC		userregion[768];			// 用户自定义768字节
	}ChData;						// Channel Data(1KB)

	BOOL			m_bEnabled;			// Is CH enabled?
	BOOL			m_bStarted;			// Is CH started?
	BOOL			m_bOpened;			// Is CH HD opend?
	US				m_intLineNO;		// Line#
	UC				m_bytGroupNo;		// Channel Group#
	US				m_intMaxPlayTime;	// Max Play Time
	UC				m_bytMaxTrys;		// Max Input Try

	// Sun added 2008-05-16
	DWORD			m_dwChCreate;		// 通道创建时间记录
	DWORD			m_dwChStart;		// 通道启动时间记录
	
	// Sun addded 2006-10-01
	US				m_intMaxFaxTime;	// 传真资源最大占用时长（秒）
	long			m_lngFaxResID;		// 传真资源ID

	// Sun added 2002-02-28
	CString			m_sDeviceNo;		// DN#
	CString			m_sExtensionNo;		// Ext#
	
	// Sun added 2002-02-28
	US				m_intSecondParty;	// Second Party
	US				m_intThirdParty;	// Third Party

	// Sun added 2002-02-28
	UL				m_lngCurCallID;		// Current Call ID
	UL				m_lngNewCallID;		// New Call ID
	UL				m_lngCurConfID;		// Current Conference ID
	UL				m_lngNewConfID;		// New Conference ID

	// Sun added 2005-03-22
	UL				m_nOBPacketID;
	UC				m_bytOBResult;
	BOOL			m_blnOBResultSended; 
	CString			m_strOBMsgTelNo;
	US				m_nCampaignID;		// 项目ID
	UL				m_nTaskID;			// 拨号任务ID

	// Sun added 2013-05-20
	CString			m_strGlobalCallRefID;
	CString			m_strCountryCode;
	CString			m_strCityCode;
	CString			m_strPhoneNumber;
	CString			m_strWorkTicketID;

	// Sun added 2005-05-26
	BOOL			m_blnCCTalking;		// Calling Card Talking
	UC				m_bytCCDialProgress;

	CString			m_strExtSN;
	char			m_strUserData[PCS_DEF_DATALEN_USER];
	char			m_strExtData[PCS_MAX_MESSAGE_SIZE];
	UC				m_bytEventSource;
	UC				m_bytCallSource;
	US				m_intCTIWaitTime;	// 之前已经等待时间(秒)

	// Sun added 2012-11-23
	BOOL			m_blnIsAsynPlaying;		// 是否在异步放音

	// Sun added 2008-03-10
	/// ASR Interface
	CASRInterface			m_oASRInt;

	// Sun added 2006-10-01
	/// 释放传真资源
	void ReleaseFaxResource();

	// Read & Write Channel Time
	DWORD GetTickTime() { return m_dwTickTime; }
	void SetTickTime() { m_dwTickTime = GetTickCount(); }

	int NF_SetUserValue(UC f_varid /*变量ID*/, UC *f_value /*变量值*/, int f_len = 0);			// 设置用户变量值
	int NF_SetUserValue(const char* f_varname /*变量名*/, UC *f_value /*变量值*/, int f_len = 0);	// 设置用户变量值
	int NF_GetUserValue(UC f_varid /*变量ID*/, UC *f_value /*变量值*/);			// 读取用户变量值
	int NF_GetUserValueLocation(const char* f_varname /*变量名*/);				// 读取用户变量起始位置
	int NF_GetUserValue(const char* f_varname /*变量名*/, UC *f_value /*变量值*/);			// 读取用户变量值
	int NF_SetAccessLog(UC f_loc);					// 设置访问日志
	bool NF_GetAccessLog(UC f_loc);					// 检查访问日志
	int NF_TransferDefault(UC f_key, US f_parentnode);							// 按照缺省转移规则转移

	US NF_006(const int event, const UC *f_data1, const UC *f_data2);
	US NF_007(const int event, const UC *f_data1, const UC *f_data2);
	US NF_008(const int event, const UC *f_data1, const UC *f_data2);
	US NF_009(const int event, const UC *f_data1, const UC *f_data2);
	US NF_010(const int event, const UC *f_data1, const UC *f_data2);

	US NF_016(const int event, const UC *f_data1, const UC *f_data2);
	US NF_017(const int event, const UC *f_data1, const UC *f_data2);
	US NF_018(const int event, const UC *f_data1, const UC *f_data2);
	US NF_019(const int event, const UC *f_data1, const UC *f_data2);
	US NF_020(const int event, const UC *f_data1, const UC *f_data2);
	US NF_021(const int event, const UC *f_data1, const UC *f_data2);
	US NF_022(const int event, const UC *f_data1, const UC *f_data2);
	US NF_023(const int event, const UC *f_data1, const UC *f_data2);
	US NF_028(const int event, const UC *f_data1, const UC *f_data2);
	
	// Sun added 2008-03-10
	US NF_030(const int event, const UC *f_data1, const UC *f_data2);

	US NF_040(const int event, const UC *f_data1, const UC *f_data2);
	US NF_041(const int event, const UC *f_data1, const UC *f_data2);

	US NF_050(const int event, const UC *f_data1, const UC *f_data2);
	US NF_051(const int event, const UC *f_data1, const UC *f_data2);
	US NF_055(const int event, const UC *f_data1, const UC *f_data2);

	US NF_060(const int event, const UC *f_data1, const UC *f_data2);
	US NF_061(const int event, const UC *f_data1, const UC *f_data2);
	US NF_062(const int event, const UC *f_data1, const UC *f_data2);
	US NF_063(const int event, const UC *f_data1, const UC *f_data2);

	US NF_069(const int event, const UC *f_data1, const UC *f_data2);
	US NF_070(const int event, const UC *f_data1, const UC *f_data2);
	US NF_071(const int event, const UC *f_data1, const UC *f_data2);

	US NF_080(const int event, const UC *f_data1, const UC *f_data2);

	US NF_090(const int event, const UC *f_data1, const UC *f_data2);
	US NF_091(const int event, const UC *f_data1, const UC *f_data2);

	US NF_096(const int event, const UC *f_data1, const UC *f_data2);

	US NF_100(const int event, const UC *f_data1, const UC *f_data2);
	US NF_101(const int event, const UC *f_data1, const UC *f_data2);
	US NF_102(const int event, const UC *f_data1, const UC *f_data2);

	virtual int Run();
	int CallFlowProcess();
	CManualResetEvent m_shutdown;
	CManualResetEvent m_callinEvent;
	HANDLE m_hEventArray[2];			// Event Array for above 2 events

private:
	long NF_Find_WorkNode(US f_NID);
	void NF_EraseLastKey(bool f_clearall = false);
	void NF_RecordLastKey(UC f_key);
	int TerminalAssert(UC type = TERM_TYPE_PLAY, US pnode = 0, UC f_key = 0);
	void ResetChData(BOOL f_blnForce = TRUE);
	BOOL NF_Find_Run();
	void NF_QueueProcess();
	
	UC GetGroupNoWithLineNo( US lineno );
	BOOL GetBreakKey(UC key, char *termchar);		// 解释中断按键
	char *GetResourcePathWithID(US f_rid, char *strDest);	// 根据给定ID获取资源路径
	US GetVoiceFilePlayLength(US f_rid);					// 语音文件时间长度
	BOOL InsertCallBackRecord(CString f_CustomerID, UC f_Flag, const char* f_varname);
	BOOL InsertACCSCDR(CString f_sFields, CString f_sValues);

	// Sun added 2008-01-12, 插入tbIVRCDRDetail表记录
	BOOL Shortcut_InsertIVRCDRDetail();
	BOOL InsertIVRCDRDetail(US f_nNodeID, UC f_bytNodeNo, US f_nNodeStatus, US f_nNodeEvent, LPCSTR f_strNodeInTime, DWORD f_dwNodeTick, US f_nLogLevel, LPCSTR f_strKeyPress);

	// Sun added 2006-10-28
	BOOL InsertFAXCDR(BYTE f_bytFaxDirection, LPCSTR f_sStartTime, LPCSTR f_sCompleteTime, 
							   LPCSTR f_sFromNo, LPCSTR f_sToNo, LPCSTR f_sHeader, LPCSTR f_sExtNo,
							   LPCSTR f_sFilePath, UC f_bytResult,
							   UC f_bytField1, UC f_bytField2, UC f_bytField3);
	// Sun added 2005-05-27
	BOOL UnLinkOBChannel();

	int m_iKeyBufPtr;								// 按键记录指针

	// Sun added 2004-12-30
	SData1_018 m_Data018_1;
	SData2_018 m_Data018_2;

	// Sun added 2007-07-11
	SData1_061 m_Data061_1;
	SData2_061 m_Data061_2;

	// Sun added 2002-04-03
	UC	m_pstatus;
	BOOL m_pstarted;
	DWORD m_pticktime;

	// Sun added 2012-05-07
	int m_nBeforeHookon;

	// Sun added 2003-05-02
	BOOL m_bInboundCall;
	BOOL m_bOutboundCall;
	BOOL m_bCTICallOut;

	// Sun added 2010-07-22
	BOOL m_bConferenced;

	// Sun added 2004-07-26
	int m_nCTITransferAttempt;

	// Sun added 2003-05-09
	CManulSync m_sync;
	US m_nCallAnswerDelay;			// ms
	US m_nChannelRestartTime;		// min

	// Sun added 2013-01-14 dummy buffer in case of COM error
	char m_strDummyBuffer1[PCS_DEF_DATALEN_USER];

	// Sun added 2003-05-23
	CManulCallCom	m_oUc;

	// Sun added 2013-01-14 dummy buffer in case of COM error
	char m_strDummyBuffer2[PCS_DEF_DATALEN_USER];

	// Sun added 2006-02-26
	long m_lngDailyCallCount;						// 当日呼叫累计
	long m_lngYTDCallCount;							// 全部呼叫累计

	// Sun added 2005-05-27
	int m_nLinkChannelNo;

	// Sun added 2004-12-30
	BYTE *m_pTTSData;

	// Sun added 2006-03-05
	BOOL m_blnIsMonitor;
	US m_nForceNextNode;
	char m_strNodeKeyPress[PCS_DEF_DATALEN_APP];

	// Sun added 2007-11-04
	DWORD m_lngLastTalkEndTick;
	UL m_lngChannelTIT;								// 通道通话间隔

	// Sun added 2011-06-16 [B]
	BOOL m_blnCOMInitialized;						// COM环境是否已经初始化

	//------------------------------------------------------------------------
	// Sun added 2007-01-16
	BOOL m_blnIVRTestFlag;
	BOOL m_blnIVRTestNoCDR;
	BOOL m_blnIVRTestTime;
	CTime m_tmIVRTestTime;
	//------------------------------------------------------------------------
	
	// Sun added 2007-07-11
	/// IVR排队中断信号
	/// 等待中断Level1 - CTI等待排队请求回应
	/// 等待中断Level2 - CTI等待排队完成
	/// 等待中断Level3 - 等待磋商完成
	/// 等待中断Level4 - CTI等待坐席接听
	/// 等待中断Level7 - 等待接坐席接听超时（或者拒绝）
	/// 等待中断Level9 - 节点超时
	/// 等待中断Level10 - 等待结束
	/// 等待中断Level13 - 拨号等待响应
	/// 等待中断Level14 - 拨号等待接听
	/// 等待中断Level16 - 错号
	/// 等待中断Level17 - Busy
	/// 等待中断Level18 - 主动离队
	/// 等待中断Level19 - 电话断线
	/// 等待中断Level20 - 时钟检测断线
	CPCSCommunicationEvent m_QueueWaitSyncEvent;

	// Sun added 2007-11-18
	/// 当前CallFlowID，在呼叫建立时时记录，因为呼叫过程中可能会加载流程
	US m_nCurrentCallFlowID;

	// Sun added 2008-01-12
	US m_nCallFlowNodeIndex;						// 流程节点顺序号(1 Based)
	UC m_nCurrentNodeNode;							// 当前流程节点类型编号
	US m_nCurrentNodeLogLevel;						// 当前流程节点日志级别
	CTime m_tmNodeStartTime;						// 流程节点开始时间(Datetime)
	DWORD m_dwNodeStartTick;						// 流程节点开始时间(TickCount)


	US m_nSwitch_DispatchTimes;						// 转接尝试次数
	US m_nSwitch_WorkNo;							// 转接记录-工号
	UL m_nSwitch_AgentID;							// 转接记录-座席ID
	CString m_strSwitch_UserID;						// 转接记录-话务员ID
	CString m_strSwitch_ExtNo;						// 转接记录-分机号
	char m_chSwitch_Result;							// 转接记录-结果代码

	BOOL			m_blnNodeLogOff;				// 关闭流程节点日志, Sun added 2008-02-19

	// Sun addded 2011-10-20, 是否已经记录IVR详单
	BOOL			m_blnCDRRecorded;

	BOOL WaitForStartupToComplete();
	void WaitForShutdownToComplete();

	BOOL MQ_SendMsg_CallClear();
	BOOL MQ_SendMsg_CallOut(LPCSTR strDestNumber);
	BOOL MQ_SendMsg_Singlestep(LPCSTR strDestNumber);
	BOOL MQ_SendMsg_SetCallWaiting(LPCSTR strDN, BOOL blnOnOff = TRUE);
	BOOL MQ_SendMsg_QueryRoutePoint(UC f_nType, US f_nGroupID);
	BOOL MQ_SendMsg_QueryAgentStatus(UC f_nType, UL f_nAgentID, LPCSTR f_strUserID);
	BOOL MQ_SendMsg_StatusReport();
	BOOL MQ_SendMsg_NodeTransfer(US f_NodeID, LPCSTR f_sKeyPress);
	BOOL MQ_SendMsg_QueryQueuePosition(US f_nRoutePointID = 0);
	BOOL MQ_SendMsg_AddQueueTask(UL nRoutePointID);
	BOOL MQ_SendMsg_RemoveQueueTask(LPCSTR strTaskID, UL nRoutePointID, UL nReasonCode);
	BOOL MQ_SendMsg_DispatchTaskReqAck(LPCSTR strTaskID, UL nRoutePointID, UC nErrorCode);
	BOOL MQ_SendMsg_ContinueQueueTask(LPCSTR strTaskID, UL nRoutePointID, UL nReasonCode);
	BOOL MQ_SendMsg_FinishQueueTask(LPCSTR strTaskID, UL nRoutePointID, UL nReasonCode);
	BOOL MQ_SendMsg_PauseQueueTask(LPCSTR strTaskID, UL nRoutePointID, UL nReasonCode);
	
	// Sun added 2012-04-18 [B]
	BOOL MQ_SendMsg_CallConsult(UC bytMode, LPCSTR strDestNumber);
	BOOL MQ_SendMsg_RetrieveCall();
	BOOL MQ_SendMsg_FinishConference();

	// Sun added 2012-04-18 [A]
	BOOL MQ_SendMsg_General_PCS(US f_cmd, const UC *f_msg, int f_nDataLen);

	void FillupCallBindedData(LPSTR f_sAppData, LPSTR f_sUserData);
	void FillupExtReceiveData(SData2_096 *f_pData2, UC *f_sReceiveData);
	int FillupExtSendData(SData1_096 *f_pData1, SData2_096 *f_pData2, LPSTR f_sSendData);
	BOOL ReadTextFile(LPCSTR f_sFileName, CString &f_sText);

private:
//-----------------------------------------------------------------
// System upgraded 2011-12-01 [Tag: ANTIDECEIT]
#ifdef PICADEF_ENABLE_ANTIDECEIT
	// 是否是第一个放音转移节点（通常是欢迎词）
	BOOL m_blnFirstPlayNode;
#endif
//-----------------------------------------------------------------
};

class CIVRCH_Collection : protected CMap<UL, UL, CIVRChannel *, CIVRChannel *> 
{
public:
	CIVRCH_Collection();
	virtual ~CIVRCH_Collection();

	int GetIVRChannelCount();
	BOOL AddAgent(UL f_AgentID, CIVRChannel *f_pAgent);
	void RemoveAgent(UL f_AgentID);
	void RemoveAllAgents();

	CIVRChannel *FindAgentIndexWithID(UL f_AgentID);

private:
	CManulSync		m_bLock;
};

#endif // !defined(AFX_IVRCHANNEL_H__179953BD_8690_4EA7_B581_23FC2085979C__INCLUDED_)
