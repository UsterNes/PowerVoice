///////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		_IVRGroup.h
// 用途描述：	IVR通道组类声明
// 作者：		SUN
// 创建时间：	2001/06/29
// 最后修改时间：2002/03/20
// 版权信息：	Copyright (C) 1999 - 2000 Changjiang Xincheng
// 备注：Multi-IVR Program
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if !defined(AFX_IVRGROUP_H__4FE5BF39_B3B6_416C_ABC9_E2ABF666A978__INCLUDED_)
#define AFX_IVRGROUP_H__4FE5BF39_B3B6_416C_ABC9_E2ABF666A978__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "_NodeFunc.h"
#include ".\tapiobj.h"

#ifndef EVENTTABLESIZE
#define EVENTTABLESIZE		5			// 事件表大小
#endif

#ifndef MAX_TRYTIMES
#define MAX_TRYTIMES		3			// 缺省最大尝试次数
#endif

class CIVRChannel;

class CIVRObject
{
public:
	CIVRObject();
	virtual ~CIVRObject();

	BYTE GetObjectType() { return m_bytObjectType; }

protected:
	BYTE	m_bytObjectType;		// 1 - Channel; 2 - Group;
};

class CIVRGroup
	: public CIVRObject
#ifdef CISCO_CCM
	, public CTapiRoutePoint
#endif
{
public:
	BOOL StopGroup(BOOL f_blnSysShutDown = FALSE);
	BOOL StartGroup(BOOL f_blnSysRecovery = FALSE);
	CIVRGroup();
	virtual ~CIVRGroup();

	// Sun added 2004-12-30
	BOOL GetAutoLoadCallFlow(CTime &f_tmTime, UC *f_bytProgramID, CString &f_strFileName);
	void SetAutoLoadCallFlow(BOOL f_blnEnabled, CTime f_tmTime, UC f_bytProgramID, LPCSTR f_strFileName = NULL);
	BOOL GetAutoLoadResource(CTime &f_tmTime, US *f_intResourceID);
	void SetAutoLoadResource(BOOL f_blnEnabled, CTime f_tmTime, US f_intResourceID);
	void CheckAutomaticTimer(BOOL f_blnResetDailyCallData);

	BOOL ExecuteSQLStatement(CString f_SQL);

	SEventTable		ET_Default[EVENTTABLESIZE];	// 缺省事件状态表
	SEventTable		*ET_First;					// The first event table
	SEventTable		*ET_Second;					// The second event table
	SUserVarDef		m_UVT[2][256];				// 用户定义变量
	UC				m_UserVars[2];				// 用户定义变量数量
	UC				m_TransferRule[2][3];		// 全程转移规则
	_bstr_t			m_olestrClassID;			// COM名称
	BOOL			m_blnLogSwitchOff;			// 关闭流程节点日志, Sun added 2008-02-19

	// Sun added 2004-12-30
	SData1_018		m_Data018_1[2];
	SData2_018		m_Data018_2[2];
	
	// Resource Map
	// Resource Map structure
	struct SResMapDef{
		US			R_ID;				// 资源ID
		char		R_Path[100];		// 资源路径
	};

	SResMapDef		*m_pResMap[MAXLANGUAGECOUNT];		// 资源映像
	long			m_lngResRCN[MAXLANGUAGECOUNT];		// 资源总数

	// Sun added 2006-02-26
	CMutexValueManger m_DailyCallCount;			// 当日呼叫累计
	CMutexValueManger m_YTDCallCount;			// 全部呼叫累计
	void IncreaseCallCount();

	// Load/Refresh resources mapping
	BOOL LoadResourceMap();
	BOOL LoadResourceMap(US f_RID);
	
	// Read IVR Group#
	UC GetGroupNO() { return m_bytGroupNO; }

	// Read & Write enable state
	BOOL GetEnabled() { return m_bEnabled; }
	void SetEnabled(BOOL blnEnabled = TRUE);

	// Read & Write start state
	BOOL GetStarted();
	void SetStarted(BOOL blnStarted = TRUE);

	// Read & Write Channel Caption
	CString GetCaption() { return m_szCaption; }
	void SetCaption(CString szCaption);

	// Read & Write Resource Project ID of Current Group
	US GetResourcePrjID() { return m_intResourcePrjID; }
	void SetResourcePrjID(US intResourcePrjID);

	// Read & Write IVR Program ID
	UC GetIVRProgramID() { return m_IVRProgramID; }
	void SetIVRProgramID(UC bytProgramID);

	// Read & Write IVR Program File Name
	CString GetIVRFileName() { return m_szIVRFileName; }
	void SetIVRFileName(CString szFileName);

	// Read & Write IVR Program Caption
	CString GetIVRProgramCaption() { return m_szIVRProgramCaption; }
	void SetIVRProgramCaption(CString szCaption);

	// Read & Write current ET#
	UC GetCurrentETID() { return (m_CurrentETID); }
	void SetCurrentETID(UC bytCurrentETID) { m_CurrentETID = bytCurrentETID; }

	// Alter ET ( 0 <-> 1 )
	void AlterET() { m_CurrentETID = 1 - m_CurrentETID; }

	// Read & Write root node#
	US GetRootNode(UC bytETID) { return m_RootNodeID[bytETID]; }
	void SetRootNode(UC bytETID, US intRootNode) { m_RootNodeID[bytETID] = intRootNode; }

	// Read & Write record count of current ET
	long GetRCN(UC bytETID) { return m_RCN[bytETID]; }
	void SetRCN(UC bytETID, long lngRCN) { m_RCN[bytETID] = lngRCN; }

	// Read & Write Channel section
	US GetStartCH() { return m_intStartCH; }
	void SetStartCH(US intStartCH);
	US GetEndCH() { return m_intEndCH; }
	void SetEndCH(US intEndCH);
	US GetFlagCH();
	void SetFlagCH(US intValue);
	CIVRChannel *MoveToNextFlagCH();
	
	// Sun added 2002-03-18
	// Read & Write CTI status
	BOOL GetNeedCTI() { return m_bNeedCTI; }
	void SetNeedCTI(BOOL bNeedCTI);
	void InitializeChannels();

	// Sun added 2005-07-10
	BOOL GetIsOBOnly() { return m_bOBOnly; }
	void SetIsOBOnly(BOOL bOBOnly);

	// Sun added 2006-11-30
	BOOL GetIsBindFaxRes() { return m_blnBindFaxResource; }
	void SetIsBindFaxRes(BOOL bIsBind);

	// Sun added 2007-07-10
	DWORD GetCTIRoutePointID() { return m_dwRoutePointID; }
	void SetCTIRoutePointID(DWORD dwRoutePointID);

	// Sun added 2012-09-08
	UC GetCustID() { return m_bytCustID; }
	void SetCustID(UC bytCustID);

	// Read & Write User defined variable number
	UC GetUserVars(UC bytETID) { return m_UserVars[bytETID]; }
	void SetUserVars(UC bytETID, UC bytUserVars) { m_UserVars[bytETID] = bytUserVars; }

	// Sun added 2006-03-05
	// Group is monitorred or not
	BOOL GetIsMonitorred();
	void SetIsMonitorred(BOOL f_Value);
	int GetMonitorChannel();
	BYTE SetMonitorChannel(int f_nChannel = -1);
	BYTE SetUnMonitorChannel(int f_nChannel = -1);

	// Sun added 2007-11-04
	UL GetMinGCTIT();
	void SetMinGCTIT(UL lngSeconds);
	void CalculateGCTIT();

	// Sun added 2012-05-07
	US GetBeforeHookOnNodeID();

	void Create(  UC groupno );				// Create IVR Group#
	BOOL InitIVRGroup();					// Read IVR Group Parameters
	BOOL Init_SelectET();					// Load ET all over function
	void PropertyChanged(UC nItemChanged = 255, BOOL blnRefresh = FALSE);

	// Sun added 2013-01-27 [A]
	/// 发送任务离队消息
	void CancelUMSTasks();

#ifdef CISCO_CCM
	long DistributeCallToChannel(DWORD dwCall);
#endif

	// Sun added 2006-03-05
	BOOL MQ_SendMsg_GroupMonitorAck(BOOL f_blnMonitor, UC f_bytResult);
	BOOL MQ_SendMsg_GroupPropName();
	BOOL MQ_SendMsg_GroupPropCallFlow();
	BOOL MQ_SendMsg_GroupPropResource();
	BOOL MQ_SendMsg_ChannelStart(US f_nChannelID, LPCSTR f_sANI);
	BOOL MQ_SendMsg_ChannelStop(US f_nChannelID);

protected:
	BOOL			m_bEnabled;					// Enable state
	BOOL			m_bStarted;					// Is Group started?
	BOOL			m_bNeedCTI;					// CTI Supported?
	UC				m_bytGroupNO;				// Group#
	CString			m_szCaption;				// Caption of Channel Group
	US				m_intResourcePrjID;			// ID of Resource Project for this Group

	UC				m_IVRProgramID;				// 当前有效流程ID
												/// 0 - Default ET
												/// 1..254 - Load from Database
												/// 255 - Load from Data File
												/// m_szIVRFileName must is valued
	CString			m_szIVRFileName;			// IVR 流程文件名(ID:255)
	CString			m_szIVRProgramCaption;		// IVR 流程描述
												/// (ID:255 - Data File Header)
												/// (ID:1..254 - Database)

	UC				m_CurrentETID;				// 当前事件状态表ID(0 or 1)
	US				m_RootNodeID[2];			// 根节点ID
	long			m_RCN[2];					// 事件表节点数
	BOOL			m_AllocFirst;				// 需要回收第一事件状态表
	BOOL			m_AllocSecond;				// 需要回收第二事件状态表
	US				m_intEndCH;					// 起始通道
	US				m_intStartCH;				// 结束通道
	US				m_intFlagCH;				// 标记通道
	BOOL			m_bOBOnly;					// Only used for OB Call
	BOOL			m_blnBindFaxResource;		// 传真资源是否邦定，默认为true
	DWORD			m_dwRoutePointID;			// 为本组通道分配呼叫的CTI路由点ID, 2007-07-10
	UC				m_bytCustID;				// 租户ID, Sun added 2012-09-08

	BOOL GetIVRProgramName();				// Get the name of IVR program
	BOOL LoadDefaultET();					// Load default Event Table
	BOOL LoadIVRProgram();					// Load new IVR program
	BOOL LoadIVRProgramFromDB();
	BOOL LoadIVRProgramFromFile();

	US NF_000(const UC *f_data2);
	US NF_001(const UC *f_data1);
	US NF_002(const UC f_group, const UC *f_data2);

private:
	BOOL InitDefaultET();

	SData2_000		m_sysNodeData;				// 系统节点数据

	// Sun added 2004-12-30
	CMyTimeBomb		m_NextCallFlow;				// 自动更新Call Flow定时
	CMyTimeBomb		m_NextResource;				// 自动更新资源定时
	US				m_intNextResourcePrjID;		// ID of Next Resource Project for this Group

	UC				m_IVRNextProgramID;			// 下一次要加载的有效流程ID
												/// 0 - Default ET
												/// 1..254 - Load from Database
												/// 255 - Load from Data File
												/// m_szIVRFileName must is valued
	CString			m_szIVRNextFileName;		// 下一次要加载的 IVR 流程文件名(ID:255)

	// Sun added 2006-03-05
	BOOL			m_blnMonitor;				// 是否被监控
	int				m_nMonitorChannel;			// 被监控通道

	// Sun added 2008-05-29
	DWORD			m_dwLastCallDistribute;		// 最近一次呼叫分配TickCount

	// Sun added 2007-11-04
	UL				m_lngMinGCTIT;				// 组通道通话最小间隔

	// Sun added 2007-07-11
	CManulSync		m_QueueLock;
};

#endif // !defined(AFX_IVRGROUP_H__4FE5BF39_B3B6_416C_ABC9_E2ABF666A978__INCLUDED_)
