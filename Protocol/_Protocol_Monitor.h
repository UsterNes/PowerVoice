////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		_Protocol_Monitor.h
// 用途描述：	监控通信协议
// 作者：		SUN
// 创建时间：	1999/12/01
// 最后修改时间：2007/07/01
// 版权信息：	Copyright (C) 1999 - 2003 PCSG
// 备注：
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_MONITOR_H__
#define __PROTOCOL_MONITOR_H__
//------------------------------------------------------------------------

//--------------------------
// 呼叫中心监控注册 - Total 4
//--------------------------

//--------------------------
// 注册 - 2
//------------
#define TCP_MON2PCS_REGISTER				1			// (0x0001) 注册消息
struct S_TCP_MON2PCS_REGISTER
{
	UL		MonitorID;						// 监控者ID
	UC		Version;						// 版本号
	UC		RegCode[PCS_DEF_NUMLEN_APP];	// 注册码
	char	strPassword[20];				// 用户口令
};
//------------

//------------
#define TCP_PCS2MON_REGISTER_ACK			1001		// (0x03E9) 注册响应消息
struct S_TCP_PCS2MON_REGISTER_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_BEUSED
											// PCS_DEF_RETCODE_VERSION
											// PCS_DEF_RETCODE_REGCODE
											// PCS_DEF_RETCODE_PASSWORD
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		MonitorLevel;					// 监控用户级别
	UL		AssociateAgentID;				// 绑定的座席ID
	char	strMonitorView[20];				// 监控用户对应的不同视，不同的视以"，"隔开,最多三个视ID
	char	strMonitorSelfView[10];			// 自定义视ID
	char	strMonitorName[20];				// 监控者名字
	char	strRights[100];					// 监控者权限
};
//------------
//--------------------------

//--------------------------
// 取消注册 - 2
//------------
#define TCP_MON2PCS_UNREGISTER				2			// (0x0002) 取消注册消息
//------------

//------------
#define TCP_PCS2MON_UNREGISTER_ACK			1002		// (0x03EA) 取消注册响应消息
//------------
//--------------------------


//--------------------------
// 呼叫中心监控对象操作 - Total 12
//--------------------------

//--------------------------
// 添加监控对象 - 6
//------------
#define TCP_MON2PCS_ADDMONITOR				201			// (0x00C9) 添加监控对象消息
struct S_TCP_MON2PCS_ADDMONITOR
{
	UL		AgentID;						// 监控座席ID
};
//------------

//------------
#define TCP_PCS2MON_ADDMONITOR_ACK			1201		// (0x04B1) 添加监控对象响应消息
struct S_TCP_PCS2MON_ADDMONITOR_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		AgentID;						// 监控座席ID
	UC		PhoneType;						// 话机类型，参照_pcC_PhoneType
	char	strDN[20];						// DN号码
	char	strPosition[20];				// Position ID
	char	strAgentName[20];				// 座席名称
	UC		DNStatus;						// 话机状态，参照_pcC_stDN
	UL		DNStDur;						// 话机状态保持时长（秒）
	UC		PositionStatus;					// 位置状态，参照_pcC_stPOS
	UL		PosStDur;						// 位置状态保持时长（秒）
	UC		LeaveReason;					// 离席原因，自定义
	char	strAliasName[50];				// 登录别名
	//-----------------------------------
	// Sun added 2007-07-28
	char	strRights[50];					// 座席权限
	UL		LoginID;						// 登录ID
	UL		UserType;						// 话务员类型，“话务员类别定义”中值的组合
	char	strUserName[50];				// 话务员显示名称
	char	strUserRights[50];				// 话务员权限
	//-----------------------------------
};
//------------

//------------
#define TCP_MON2PCS_MONITORROUTEMANAGER		202			// (0x00CA) 启动路由管理器监控消息
//------------

//------------
#define TCP_PCS2MON_MONITORROUTEMANAGER_ACK	1202		// (0x04B2) 启动路由管理器监控响应消息
struct S_TCP_PCS2MON_MONITORROUTEMANAGER_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_MON2PCS_MONITOROBSERVICE		203			// (0x00CB) 启动外拨服务器监控消息
//------------

//------------
#define TCP_PCS2MON_MONITOROBSERVICE_ACK	1203		// (0x04B3) 启动外拨服务器监控响应消息
struct S_TCP_PCS2MON_MONITOROBSERVICE_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------


//--------------------------
// 减少监控对象 - 6
//------------
#define TCP_MON2PCS_REMOVEMONITOR			221			// (0x00DD) 减少监控对象消息
struct S_TCP_MON2PCS_REMOVEMONITOR
{
	UL		AgentID;						// 监控座席ID
};
//------------

//------------
#define TCP_PCS2MON_REMOVEMONITOR_ACK		1221		// (0x04C5) 减少监控对象响应消息
struct S_TCP_PCS2MON_REMOVEMONITOR_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		AgentID;						// 监控座席ID
};
//------------

//------------
#define TCP_MON2PCS_REMOVEROUTEMANAGER		222			// (0x00DE) 停止路由管理器监控消息
//------------

//------------
#define TCP_PCS2MON_REMOVEROUTEMANAGER_ACK	1222		// (0x04C6) 停止路由管理器监控响应消息
struct S_TCP_PCS2MON_REMOVEROUTEMANAGER_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_MON2PCS_REMOVEOBSERVICE			223			// (0x00DF) 停止外拨服务监控消息
//------------

//------------
#define TCP_PCS2MON_REMOVEOBSERVICE_ACK		1223		// (0x04C7) 停止外拨服务监控响应消息
struct S_TCP_PCS2MON_REMOVEOBSERVICE_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------


//--------------------------
// 子系统状态报告消息 - Total 1
//--------------------------
//------------
#define TCP_PCS2MON_SUBSYS_STATUS_REPORT	1231		// (0x04CF) 子系统状态报告消息
struct S_TCP_PCS2MON_SUBSYS_STATUS_REPORT
{
	UC		SubSysID;						// 子系统ID，参见“接收者、发送者定义”
	UC		SubSysStatus;					// 子系统状态，参见“开关常量定义”
	UC		SubSysVersion;					// 子系统版本号
	UC		SubSysIndex;					// 子系统编号（多子系统）
};
//------------
//--------------------------

//--------------------------
// 管理消息 - Total 2
//--------------------------

//--------------------------
// 修改口令 - 2
//------------
#define TCP_MON2PCS_CHANGE_PASSWORD			251			// (0x00FA) 修改口令消息
struct S_TCP_MON2PCS_CHANGE_PASSWORD
{
	char	strPassword[20];				// 新口令
};
//------------

//------------
#define TCP_PCS2MON_CHANGE_PASSWORD_ACK		1251		// (0x04E3) 修改口令响应消息
struct S_TCP_PCS2MON_CHANGE_PASSWORD_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------

//--------------------------
// ACD 队列状态查询 - Total 2
//--------------------------
//------------
#define TCP_MON2PCS_GET_ACDSTATUS			20			// (0x0014) 查询ACD队列状态消息
struct S_TCP_MON2PCS_GET_ACDSTATUS
{
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// IVR Channel ID who requests the data
	char	strACDDN[20];					// ACD DN
};
//------------

//------------
#define TCP_PCS2MON_GET_ACDSTATUS_ACK		1020			// (0x03FC) 查询ACD队列状态响应消息
struct S_TCP_PCS2MON_GET_ACDSTATUS_ACK
{
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// IVR Channel ID who requests the data
	char	strACDDN[20];					// ACD DN
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	US		nACDStatus;
	UL		lngLoginAgents;					// Number of login agents
	UL		lngIdleAgents;					// Number of ready agents
	UL		lngTotalCalls;					// Number of calls
	UL		lngReserved;
};
//------------
//--------------------------

//--------------------------
// 文字消息服务消息 - Total 4
//--------------------------
// 采用Agent相关协议
//--------------------------

//--------------------------
// 远程控制 - 8
//--------------------------
// 重新加载监控列表消息
// 采用Admin相关协议
//--------------------------

//------------
#define TCP_MON2PCS_RELOAD_ACD_SCENARIO_ACK	271			// (0x010F) 重新加载排队规则响应消息
struct S_TCP_MON2PCS_RELOAD_ACD_SCENARIO_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_MON2PCS_RELOAD_USERLIST_ACK		272			// (0x0110) 重新加载用户列表响应消息
struct S_TCP_MON2PCS_RELOAD_USERLIST_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_MON2PCS_RELOAD_AGENTLIST_ACK	273			// (0x0111) 重新加载座席列表响应消息
struct S_TCP_MON2PCS_RELOAD_AGENTLIST_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2MON_RELOAD_ACD_SCENARIO		1271		// (0x04F7) 重新加载排队规则消息
//------------

//------------
#define TCP_PCS2MON_RELOAD_USERLIST			1272		// (0x04F8) 重新加载用户列表消息
//------------

//------------
#define TCP_PCS2MON_RELOAD_AGENTLIST		1273		// (0x04F9) 重新加载座席列表消息
//--------------------------

//--------------------------
// 参数管理 - 8
//--------------------------
// 采用Admin相关协议
//--------------------------


//--------------------------
// 第三方呼叫控制 - 2
//--------------------------
//------------
#define TCP_MON2PCS_MONITOR_CALL_CTRL		291			// (0x0123) 监控端呼叫控制消息
struct S_TCP_MON2PCS_MONITOR_CALL_CTRL
{
	UL		InvokeID;						// Invoke ID
	UC		ControlType;					// 控制类型，参见_pcC_MonitorControlType
	UL		ControlAgentID;					// 被控制座席ID
	UL		CallRefID;						// 被控制呼叫标示，如果为0则系统自动选择
	UL		HoldRefID;						// 保持呼叫标志，如果为0则系统自动选择
	char	strDest[PCS_DEF_NUMLEN_APP];	// 目标号码
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------

//------------
#define TCP_PCS2MON_MONITOR_CALL_CTRL_ACK	1291		// (0x050B) 监控端呼叫控制响应消息
struct S_TCP_PCS2MON_MONITOR_CALL_CTRL_ACK
{
	UL		InvokeID;						// Invoke ID
	UC		ControlType;					// 控制类型，参见_pcC_MonitorControlType
	UL		ControlAgentID;					// 被控制座席ID
	UL		CallRefID;						// 被控制呼叫标示，如果为0则系统自动选择
	UL		HoldRefID;						// 保持呼叫标志，如果为0则系统自动选择
	char	strDest[PCS_DEF_NUMLEN_APP];	// 目标号码
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTSUPPORT
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------


//--------------------------
// UMS处理 - Total 12
//--------------------------

//--------------------------
// 查询任务清单 - 3
//------------
#define TCP_MON2PCS_UMS_QUERYTASKLIST_REQ	3301		// (0x0CE5) 查询队列任务清单消息
struct S_TCP_MON2PCS_UMS_QUERYTASKLIST_REQ
{
	US		RoutePointID;							// 路由点ID
};
//------------

//------------
#define TCP_PCS2MON_UMS_QUERYTASKLIST_ACK	3801		// (0x0ED9) 查询队列任务清单响应消息
struct S_TCP_PCS2MON_UMS_QUERYTASKLIST_ACK
{
	US		RoutePointID;							// 路由点ID
	UC		ErrorCode;								// 返回码，参见“返回码定义”：
													// PCS_DEF_RETCODE_SUCC
													// PCS_DEF_RETCODE_NOTEXIST
													// PCS_DEF_RETCODE_NORIGHTS
													// PCS_DEF_RETCODE_OTHERS
													// PCS_DEF_RETCODE_SYSERROR
	UL		TaskCount;								// 任务数量
};
//------------

//------------
#define TCP_PCS2MON_UMS_TASK_STATUS_DATA	3802		// (0x0EDA) 任务清单数据消息
struct S_TCP_PCS2MON_UMS_TASK_STATUS_DATA
{
	UL		TaskIndex;								// 任务编号，0 based
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
	UC		TaskStatus;								// 任务状态
	UL		WaitTime;								// 入队时间（秒）
	US		RoutePointID;							// 路由点ID
	US		GroupID;								// 组ID
	US		TeamID;									// 小组ID
	UC		TeamIndex;								// 小组优先级
	UC		UserIndex;								// 话务员优先级
	char	strUserID[20];							// 话务员
	UC		UMSType;								// UMS接入类型，参见_pcC_UMSTaskTypes
	UC		UMSIndex;								// UMS子系统编号
	UC		CustID;									// 租户代码
	char	strCallingParty[PCS_DEF_NUMLEN_APP];	// 发起者
	char	strCalledParty[PCS_DEF_NUMLEN_APP];		// 接收者
	UL		ClassifyCode;							// 分类代码
	char	ServiceCode0;							// 主服务代码
	char	ServiceCode1;							// 二级服务代码
	char	ServiceCode2;							// 三级服务代码
	char	strTaskTitle[50];						// 任务标题
	char	strTaskContent[200];					// 任务描述
};
//------------
//--------------------------

//--------------------------
// 统一任务状态改变通知 - 4
//------------
#define TCP_PCS2MON_UMS_TASK_ADD_NTF		3811		// (0x0EE3) 统一入队通知消息
struct S_TCP_PCS2MON_UMS_TASK_ADD_NTF
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
	UC		UMSType;								// UMS接入类型，参见_pcC_UMSTaskTypes
	UC		UMSIndex;								// UMS子系统编号
	UC		CustID;									// 租户代码
	char	strCallingParty[PCS_DEF_NUMLEN_APP];	// 发起者
	char	strCalledParty[PCS_DEF_NUMLEN_APP];		// 接收者
	US		RoutePointID;							// 默认路由点ID
	UL		ClassifyCode;							// 分类代码
	UL		WaitTime;								// 已等待时间（秒）
	char	ServiceCode0;							// 主服务代码
	char	ServiceCode1;							// 二级服务代码
	char	ServiceCode2;							// 三级服务代码
	char	strTaskTitle[50];						// 任务标题
	char	strTaskContent[200];					// 任务描述
};
//------------

//------------
#define TCP_PCS2MON_UMS_TASK_REMOVE_NTF		3812		// (0x0EE4) 统一离队通知消息
struct S_TCP_PCS2MON_UMS_TASK_REMOVE_NTF
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
	US		RoutePointID;							// 路由点ID
};
//------------

//------------
#define TCP_PCS2MON_UMS_TASK_DISPATCH_NTF	3813		// (0x0EE5) 统一分配处理通知消息
struct S_TCP_PCS2MON_UMS_TASK_DISPATCH_NTF
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
	UL		WaitTime;								// 已等待时间（秒）
	US		RoutePointID;							// 路由点ID
	US		GroupID;								// 组ID
	US		TeamID;									// 小组ID
	UC		TeamIndex;								// 小组优先级
	UC		UserIndex;								// 话务员优先级
	char	strUserID[20];							// 话务员ID
};
//------------

//------------
#define TCP_PCS2MON_UMS_TASK_FINISH_NTF		3814		// (0x0EE6) 统一处理完成通知消息
struct S_TCP_PCS2MON_UMS_TASK_FINISH_NTF
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
	US		RoutePointID;							// 路由点ID
	char	strUserID[20];							// 话务员ID
	UL		ReasonCode;								// 离队原因代码
	char	strRefField1[10];						// 任务参照字段1
	char	strRefField2[20];						// 任务参照字段2
	char	strRefField3[20];						// 任务参照字段3
	char	strRefField4[50];						// 任务参照字段4
	char	strRefField5[100];						// 任务参照字段5
};
//------------
//--------------------------

//--------------------------
// 统一任务取消 - 5
//------------
#define TCP_MON2PCS_UMS_CANCELTASK_CMD		3321		// (0x0CF9) 统一任务取消控制消息
struct S_TCP_MON2PCS_UMS_CANCELTASK_CMD
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
	UL		ReasonCode;								// 离队原因代码
	char	strRefField1[10];						// 任务参照字段1
	char	strRefField2[20];						// 任务参照字段2
	char	strRefField3[20];						// 任务参照字段3
	char	strRefField4[50];						// 任务参照字段4
	char	strRefField5[100];						// 任务参照字段5
};
//------------

//------------
#define TCP_MON2PCS_UMS_CANCELAGTTASK_CMD	3322		// (0x0CFA) 坐席全部任务取消控制消息
struct S_TCP_MON2PCS_UMS_CANCELAGTTASK_CMD
{
	char	strUserID[20];							// 话务员ID
	UL		ReasonCode;								// 离队原因代码
	char	strRefField1[10];						// 任务参照字段1
	char	strRefField2[20];						// 任务参照字段2
	char	strRefField3[20];						// 任务参照字段3
	char	strRefField4[50];						// 任务参照字段4
	char	strRefField5[100];						// 任务参照字段5
};
//------------

//------------
#define TCP_PCS2MON_UMS_CANCELTASK_ACK		3821		// (0x0EED) 统一任务取消控制响应消息
struct S_TCP_PCS2MON_UMS_CANCELTASK_ACK
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
	UC		ErrorCode;								// 返回码，参见“返回码定义”：
													// PCS_DEF_RETCODE_SUCC
													// PCS_DEF_RETCODE_NOTEXIST
													// PCS_DEF_RETCODE_NORIGHTS
													// PCS_DEF_RETCODE_OTHERS
													// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2MON_UMS_CANCELAGTTASK_ACK	3822		// (0x0EEE) 坐席全部任务取消控制响应消息
struct S_TCP_PCS2MON_UMS_CANCELAGTTASK_ACK
{
	char	strUserID[20];							// 话务员ID
	UC		ErrorCode;								// 返回码，参见“返回码定义”：
													// PCS_DEF_RETCODE_SUCC
													// PCS_DEF_RETCODE_NOTEXIST
													// PCS_DEF_RETCODE_NORIGHTS
													// PCS_DEF_RETCODE_OTHERS
													// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2MON_UMS_TASK_CANCEL_NTF		3823		// (0x0EEF) 统一处理取消通知消息
struct S_TCP_PCS2MON_UMS_TASK_CANCEL_NTF
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
	US		RoutePointID;							// 路由点ID
	char	strUserID[20];							// 话务员ID
	UL		ReasonCode;								// 离队原因代码
	char	strRefField1[10];						// 任务参照字段1
	char	strRefField2[20];						// 任务参照字段2
	char	strRefField3[20];						// 任务参照字段3
	char	strRefField4[50];						// 任务参照字段4
	char	strRefField5[100];						// 任务参照字段5
};
//------------
//--------------------------


//--------------------------
// 座席状态修改消息 - Total 12
//--------------------------
//------------
#define TCP_MON2PCS_AGENT_LOGIN_REQ			241			// (0x00F1) 座席登录控制消息
struct S_TCP_MON2PCS_AGENT_LOGIN_REQ
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// 监控座席ID
	UL		LoginID;						// 登录ID，如果为0，则使用登录别名
	char	strAliasName[50];				// 登录别名
	char	strPassword[20];				// 用户口令
};
//------------

//------------
#define TCP_MON2PCS_AGENT_LOGOUT_REQ		242			// (0x00F2) 座席退出登录控制消息
struct S_TCP_MON2PCS_AGENT_LOGOUT_REQ
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// 监控座席ID
};
//------------

//------------
#define TCP_MON2PCS_AGENT_STATUS_CHG_REQ	243			// (0x00F3) 座席修改状态控制消息
struct S_TCP_MON2PCS_AGENT_STATUS_CHG_REQ
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// 监控座席ID
	UC		PositionStatus;					// 位置状态，参照_pcC_stPOS
	UC		LeaveReason;					// 离席原因，自定义
	UC		CallDisconnect;					// 是否断开当前通话，参见“开关常量定义”
};
//------------

//------------
#define TCP_MON2PCS_AGENT_SET_CALLFWD		244			// (0x00F4) 座席设定呼叫转移制消息
struct S_TCP_MON2PCS_AGENT_SET_CALLFWD
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// 监控座席ID
	UC		ForwardType;					// 呼叫转移类型，参见“呼叫转移类型定义”
	char	strTelNo[PCS_DEF_NUMLEN_APP];	// 呼叫转移号码
};
//------------

//------------
#define TCP_MON2PCS_AGENT_SET_MWA_ST		245			// (0x00F5) 座席设定留言等待开关控制消息
struct S_TCP_MON2PCS_AGENT_SET_MWA_ST
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// 监控座席ID
	UC		SetMWAOn;						// 是否设定留言等待开关，参见“开关常量定义”
};
//------------

//------------
#define TCP_MON2PCS_GET_LEAVEREASONS		246			// (0x00F6) 请求获取事由列表
//------------

//------------
#define TCP_PCS2MON_AGENT_LOGIN_ACK			1241		// (0x04D9) 座席登录控制响应消息
struct S_TCP_PCS2MON_AGENT_LOGIN_ACK
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// 监控座席ID
	UL		LoginID;						// 登录ID
	char	strAliasName[50];				// 登录别名
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_BEUSED
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_PASSWORD
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_LACK_PARAM
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UC		PhoneType;						// 话机类型，参照_pcC_PhoneType
	char	strDN[20];						// DN号码
	char	strPosition[20];				// Position ID
	char	strRights[50];					// 座席权限
	UL		UserType;						// 话务员类型，“话务员类别定义”中值的组合
	char	strUserName[50];				// 话务员显示名称
	char	strUserRights[50];				// 话务员权限
};
//------------

//------------
#define TCP_PCS2MON_AGENT_LOGOUT_ACK		1242		// (0x04DA) 座席退出登录控制响应消息
struct S_TCP_PCS2MON_AGENT_LOGOUT_ACK
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// 监控座席ID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_REJECTED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2MON_AGENT_STATUS_CHG_ACK	1243		// (0x04DB) 座席修改状态控制响应消息
struct S_TCP_PCS2MON_AGENT_STATUS_CHG_ACK
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// 监控座席ID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2MON_AGENT_SET_CALLFWD_ACK	1244		// (0x04DC) 座席设定呼叫转移控制响应消息
struct S_TCP_PCS2MON_AGENT_SET_CALLFWD_ACK
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// 监控座席ID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2MON_AGENT_SET_MWA_ST_ACK	1245		// (0x04DD) 座席设定留言等待开关控制响应消息
struct S_TCP_PCS2MON_AGENT_SET_MWA_ST_ACK
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// 监控座席ID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2MON_LEAVEREASONS_LIST		1246		// (0x04DE) 事由列表
struct S_TCP_PCS2MON_LEAVEREASONS_LIST
{
	UC		ReasonCount;					// 事由数量
	UC		ReasonValue[10];				// 事由编号0-9
	char	ReasonName[10][20];				// 事由名称0-9
};
//------------
//--------------------------

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_MONITOR_H__
//------------------------------------------------------------------------
