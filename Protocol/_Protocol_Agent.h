////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		_Protocol_Agent.h
// 用途描述：	座席通信协议
// 作者：		SUN
// 创建时间：	1999/12/01
// 最后修改时间：2004/12/15
// 版权信息：	Copyright (C) 1999 - 2004 PCSG
// 备注：
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_AGENT_H__
#define __PROTOCOL_AGENT_H__
//------------------------------------------------------------------------

#include "_pcRightsDef.h"

//--------------------------
// 座席注册 - Total 4
//--------------------------

//--------------------------
// 注册 - 2
//------------
#define TCP_AGT2PCS_REGISTER				1			// (0x0001) 注册消息
struct S_TCP_AGT2PCS_REGISTER
{
	UL		AgentID;						// 座席ID
	UC		Version;						// 版本号
	UC		RegCode[PCS_DEF_NUMLEN_APP];	// 注册码
};
//------------

//------------
#define TCP_PCS2AGT_REGISTER_ACK			1001		// (0x03E9) 注册响应消息
struct S_TCP_PCS2AGT_REGISTER_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_BEUSED
											// PCS_DEF_RETCODE_VERSION
											// PCS_DEF_RETCODE_REGCODE
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UC		PhoneType;						// 话机类型，参照_pcC_PhoneType
	char	strDN[20];						// DN号码
	char	strPosition[20];				// Position ID
	char	strAgentName[20];				// 座席名称
	char	strRights[50];					// 座席权限
};
//------------
//--------------------------

//--------------------------
// 取消注册 - 2
//------------
#define TCP_AGT2PCS_UNREGISTER				2			// (0x0002) 取消注册消息
//------------

//------------
#define TCP_PCS2AGT_UNREGISTER_ACK			1002		// (0x03EA) 取消注册响应消息
//------------
//--------------------------


//--------------------------
// 状态消息 - Total 17
//--------------------------

//--------------------------
// 状态查询 - 2
//------------
#define TCP_AGT2PCS_STATUS_QUERY			11			// (0x000B) 查询状态消息
//------------

//------------
#define TCP_PCS2AGT_STATUS_QUERY_ACK		1011		// (0x03F3) 查询状态响应消息
struct S_TCP_PCS2AGT_STATUS_QUERY_ACK
{
	UC		DNStatus;						// 话机状态，参照_pcC_stDN
	UC		PositionStatus;					// 位置状态，参照_pcC_stPOS
	UC		LeaveReason;					// 离席原因，自定义
};
//------------
//--------------------------

//--------------------------
// 座席登录 - 2
//------------
#define TCP_AGT2PCS_AGENT_LOGIN				12			// (0x000C) 座席登录消息
struct S_TCP_AGT2PCS_AGENT_LOGIN
{
	char	strPassword[20];				// 用户口令
	char	strAliasName[50];				// 登录别名
};
//------------

//------------
#define TCP_PCS2AGT_AGENT_LOGIN_ACK			1012		// (0x03F4) 座席登录响应消息
struct S_TCP_PCS2AGT_AGENT_LOGIN_ACK
{
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
	UL		UserType;						// 话务员类型，“话务员类别定义”中值的组合
	char	strUserName[50];				// 话务员显示名称
	char	strUserRights[50];				// 话务员权限
};
//------------
//--------------------------

//--------------------------
// 座席退出登录 - 2
//------------
#define TCP_AGT2PCS_AGENT_LOGOUT			13			// (0x000D) 座席退出登录消息
//------------

//------------
#define TCP_PCS2AGT_AGENT_LOGOUT_ACK		1013		// (0x03F5) 座席退出登录响应消息
struct S_TCP_PCS2AGT_AGENT_LOGOUT_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_REJECTED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------

//--------------------------
// 修改状态 - 2
//------------
#define TCP_AGT2PCS_STATUS_CHANGE			14			// (0x000E) 修改状态消息
struct S_TCP_AGT2PCS_STATUS_CHANGE
{
	UC		PositionStatus;					// 位置状态，参照_pcC_stPOS
	UC		LeaveReason;					// 离席原因，自定义
	UC		CallDisconnect;					// 是否断开当前通话，参见“开关常量定义”
};
//------------

//------------
#define TCP_PCS2AGT_STATUS_CHANGE_ACK		1014		// (0x03F6) 修改状态响应消息
struct S_TCP_PCS2AGT_STATUS_CHANGE_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------

//--------------------------
// 免打扰设定 - 2
//------------
#define TCP_AGT2PCS_SET_DND_STATUS			15			// (0x000F) 设定免打扰开关消息
struct S_TCP_AGT2PCS_SET_DND_STATUS
{
	UC		SetDNDOn;						// 是否设定免打扰开关，参见“开关常量定义”
};
//------------

//------------
#define TCP_PCS2AGT_SET_DND_STATUS_ACK		1015		// (0x03F7) 设定免打扰开关响应消息
struct S_TCP_PCS2AGT_SET_DND_STATUS_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------

//--------------------------
// 设定及扩展 - 4
//------------
#define TCP_AGT2PCS_SET_CALLFORWARD			16			// (0x0010) 设定呼叫转移消息
struct S_TCP_AGT2PCS_SET_CALLFORWARD
{
	UC		ForwardType;						// 呼叫转移类型，参见“呼叫转移类型定义”
	char	strTelNo[PCS_DEF_NUMLEN_APP];		// 呼叫转移号码
};
//------------

//------------
#define TCP_AGT2PCS_SET_MWA_STATUS			17			// (0x0011) 设定留言等待开关消息
struct S_TCP_AGT2PCS_SET_MWA_STATUS
{
	UC		SetMWAOn;							// 是否设定留言等待开关，参见“开关常量定义”
};
//------------

//------------
#define TCP_AGT2PCS_CHANGE_USER				18			// (0x0012) 快速切换用户消息，返回TCP_PCS2AGT_AGENT_LOGIN_ACK消息
struct S_TCP_AGT2PCS_CHANGE_USER
{
	char	strPassword[20];				// 用户口令
	char	strAliasName[50];				// 登录别名
};
//------------

//------------
#define TCP_PCS2AGT_SET_CALLFORWARD_ACK		1016		// (0x03F8) 设定呼叫转移响应消息
struct S_TCP_PCS2AGT_SET_CALLFORWARD_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------

//--------------------------
// 状态改变通知 - 3
//------------
#define TCP_PCS2AGT_AGENT_ALIAS_CHANGE		1017		// (0x03F9) 座席登录别名改变消息
struct S_TCP_PCS2AGT_AGENT_ALIAS_CHANGE
{
	UL		AgentID;						// 座席ID
	char	strAliasName[50];				// 登录别名
	//-----------------------------------
	// Sun added 2007-07-28
	UL		LoginID;						// 登录ID
	UL		UserType;						// 话务员类型，“话务员类别定义”中值的组合
	char	strUserName[50];				// 话务员显示名称
	char	strUserRights[50];				// 话务员权限
	//-----------------------------------
};
//------------

//------------
#define TCP_PCS2AGT_POS_STATUS_CHANGE		1018		// (0x03FA) 位置状态改变消息
struct S_TCP_PCS2AGT_POS_STATUS_CHANGE
{
	UL		AgentID;						// 座席ID
	UC		PositionStatus;					// 位置状态，参照_pcC_stPOS
	UC		LeaveReason;					// 离席原因，自定义
};
//------------

//------------
#define TCP_PCS2AGT_DN_STATUS_CHANGE		1019		// (0x03FB) 话机状态改变消息
struct S_TCP_PCS2AGT_DN_STATUS_CHANGE
{
	UL		AgentID;						// 座席ID
	UC		DNStatus;						// 话机状态，参照_pcC_stDN
};
//------------
//--------------------------


//--------------------------
// 数据查询 - Total 6
//------------
#define TCP_AGT2PCS_AGT_STATUS_QUERY		25			// (0x0019) 查询指定座席状态消息
struct S_TCP_AGT2PCS_AGT_STATUS_QUERY
{
	UL		AgentID;						// 座席ID
};
//------------

//------------
#define TCP_AGT2PCS_USER_STATUS_QUERY		26			// (0x001A) 查询指定话务员状态消息
struct S_TCP_AGT2PCS_USER_STATUS_QUERY
{
	char	strAliasName[50];				// 登录别名
};
//------------

//------------
#define TCP_AGT2PCS_GET_ACDOBJ_STATUS		27			// (0x001B) 查询排队对象状态消息
struct S_TCP_AGT2PCS_GET_ACDOBJ_STATUS
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// Agent ID who requests the data
	UC		nType;							// 报告对象（Center / RoutePoint / Queue / Group / Team），
											/// 参见_pcC_CallDistributeObject定义
	US		nGroupID;						// 对象ID
};
//------------

//------------
#define TCP_AGT2PCS_AGENT_PROPERTY_QUERY	28			// (0x001C) 查询指定座席属性消息
struct S_TCP_AGT2PCS_AGENT_PROPERTY_QUERY
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// AgentID和DN设置一个即可，且AgentID优先
	char	strDN[PCS_DEF_NUMLEN_APP];		// DN号码
};
//------------

//------------
#define TCP_AGT2PCS_USER_PROPERTY_QUERY		29			// (0x001D) 查询指定话务员属性消息
struct S_TCP_AGT2PCS_USER_PROPERTY_QUERY
{
	UL		InvokeID;						// Invoke ID
	char	strAliasName[50];				// 登录别名和工号设置一个即可，且登录别名优先
	UL		LoginID;						// 工号
};
//------------

//------------
#define TCP_PCS2AGT_AGT_STATUS_QUERY_ACK	1025		// (0x0401) 查询指定座席状态响应消息
struct S_TCP_PCS2AGT_AGT_STATUS_QUERY_ACK
{
	UL		AgentID;						// 座席ID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UC		CustID;							// 租户ID
	UC		DNStatus;						// 话机状态，参照_pcC_stDN
	UL		DNStDur;						// 话机状态保持时长（秒）
	UC		PositionStatus;					// 位置状态，参照_pcC_stPOS
	UL		PosStDur;						// 位置状态保持时长（秒）
	UC		LeaveReason;					// 离席原因，自定义
	char	strAliasName[50];				// 登录别名
};
//------------

//------------
#define TCP_PCS2AGT_USER_STATUS_QUERY_ACK	1026		// (0x0402) 查询指定话务员状态响应消息
struct S_TCP_PCS2AGT_USER_STATUS_QUERY_ACK
{
	char	strAliasName[50];				// 登录别名
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UC		HomeSiteID;						// 归属中心ID
	UC		SiteID;							// 登录中心ID
	UC		CustID;							// 租户ID
	UL		AgentID;						// 座席ID
	UC		DNStatus;						// 话机状态，参照_pcC_stDN
	UL		DNStDur;						// 话机状态保持时长（秒）
	UC		PositionStatus;					// 位置状态，参照_pcC_stPOS
	UL		PosStDur;						// 位置状态保持时长（秒）
	UC		LeaveReason;					// 离席原因，自定义
};
//------------

//------------
#define TCP_PCS2AGT_GET_ACDOBJ_STATUS_ACK	1027		// (0x0403) 查询排队对象状态响应消息
struct S_TCP_PCS2AGT_GET_ACDOBJ_STATUS_ACK
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// Agent ID who requests the data
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	S_TCP_PCS2ALL_ACD_STATUS_REPORT Data;	// 数据
};
//------------

//------------
#define TCP_PCS2AGT_AGENT_PROPERTY_QUERY_ACK 1028		// (0x0404) 查询指定座席属性响应消息
struct S_TCP_PCS2AGT_AGENT_PROPERTY_QUERY_ACK
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// AgentID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_LACK_PARAM
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		AgentType;						// 座席类型，参照“座席类别定义”
	UC		PhoneType;						// 话机类型，参照_pcC_PhoneType
	char	strDN[PCS_DEF_NUMLEN_APP];		// DN号码
	char	strPosition[PCS_DEF_NUMLEN_APP];  // Position ID
	char	strAgentName[PCS_DEF_NUMLEN_APP]; // 座席名称
	UC		CustID;							// 座席租户ID
};
//------------

//------------
#define TCP_PCS2AGT_USER_PROPERTY_QUERY_ACK 1029		// (0x0405) 查询指定话务员属性响应消息
struct S_TCP_PCS2AGT_USER_PROPERTY_QUERY_ACK
{
	UL		InvokeID;						// Invoke ID
	char	strAliasName[50];				// 登录别名
	UL		LoginID;						// 工号
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_LACK_PARAM
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		UserType;						// 用户类型
	char	UserLevel;						// 用户级别
	UC		IsAutoUser;						// 是否是真正的用户，参见“开关常量定义”
	char	strUserName[50];				// 用户显示名
	UC		AllowLogon;						// 是否允许在本中心登录，参见“开关常量定义”
	UC		HomeSiteID;						// 归属中心ID
	UC		CustID;							// 租户ID
	UC		LoginSiteID;					// 登录中心ID
	UL		AgentID;						// AgentID，登录状态才有
};
//------------
//--------------------------


//--------------------------
// 座席外拨 - Total 5
//--------------------------

//--------------------------
// 预览外拨 - 5
//------------
#define TCP_AGT2PCS_ACCEPTOBTASK			31			// (0x001F) 接受外拨任务消息
struct S_TCP_AGT2PCS_ACCEPTOBTASK
{
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 任务ID
};
//------------

//------------
#define TCP_AGT2PCS_EXECUTEOBTASK			32			// (0x0020) 开始执行外拨任务消息
struct S_TCP_AGT2PCS_EXECUTEOBTASK
{
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 任务ID
};
//------------

//------------
#define TCP_AGT2PCS_FINISHOBTASK			33			// (0x0021) 外拨任务结束消息
struct S_TCP_AGT2PCS_FINISHOBTASK
{
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 任务ID
	UC		ResultCode;						// 外拨执行结果，参照_pcC_OutBoundCallResult
	UC		TryTimes;						// 尝试次数
};
//------------

//------------
#define TCP_PCS2AGT_ASSIGNOBTASK			1031		// (0x0407) 分配外拨任务消息
struct S_TCP_PCS2AGT_ASSIGNOBTASK
{
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 任务ID
	UC		SharedTask;						// 是否共享，参见“开关常量定义”
	char	strTelNo[PCS_DEF_NUMLEN_APP];	// 外拨电话号码
	char	strType[4];						// 外拨类型（用户自定义）
	char	strData[200];					// 外拨数据（用户自定义）
};
//------------

//------------
#define TCP_PCS2AGT_CANCELOBTASK			1032		// (0x0408) 取消外拨任务消息
struct S_TCP_PCS2AGT_CANCELOBTASK
{
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 任务ID
};
//------------
//--------------------------


//--------------------------
// 预测外拨 - 5
//------------
#define TCP_AGT2PCS_PREDICTIVE_ACCEPTTASK			41			// (0x0029) 接受预测外拨任务消息
struct S_TCP_AGT2PCS_PREDICTIVE_ACCEPTTASK
{
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 任务ID
	UC		nFlag;							// 是否接受，参见“开关常量定义”
};
//------------

//------------
/// 座席接听外拨电话时,向PCS发送开始任务消息
#define TCP_AGT2PCS_PREDICTIVE_BEGINTASK			42			// (0x002A) 开始执行外拨任务消息
struct S_TCP_AGT2PCS_PREDICTIVE_BEGINTASK
{
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 任务ID
};
//------------

//------------
#define TCP_PCS2AGT_PREDICTIVE_ASSIGNTASK			1041		// (0x411) 座席外拨通知消息
struct S_TCP_PCS2AGT_PREDICTIVE_ASSIGNTASK
{
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 任务ID
	char	strTelNo[PCS_DEF_NUMLEN_APP];	// 外拨电话号码
	char	strType[4];						// 外拨类型（用户自定义）
	char	strData[200];					// 外拨数据（用户自定义）
};
//------------

//------------
#define TCP_PCS2AGT_PREDICTIVE_CANCELTASK			1042		// (0x0412) 取消预测外拨任务消息
struct S_TCP_PCS2AGT_PREDICTIVE_CANCELTASK
{
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 任务ID
};
//------------

//------------
#define TCP_PCS2AGT_PREDICTIVE_TRANSFERTASK			1043		// (0x0413) 外拨电话接入消息
struct S_TCP_PCS2AGT_PREDICTIVE_TRANSFERTASK
{
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 任务ID
	char	strTelNo[PCS_DEF_NUMLEN_APP];	// 外拨电话号码
	char	strType[4];						// 外拨类型（用户自定义）
	char	strData[200];					// 外拨数据（用户自定义）
};
//------------
//--------------------------


//--------------------------
// 呼叫消息 Total 29
//--------------------------

//--------------------------
// 电话外拨 - 1
//------------
#define TCP_AGT2PCS_CALL_CALLOUT			51			// (0x0033) 电话外拨消息
struct S_TCP_AGT2PCS_CALL_CALLOUT
{
	UC		CallScale;								// 呼叫范围，参照_pcC_CallScale
	char	strCalledParty[PCS_DEF_NUMLEN_APP];		// 外拨号码
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------
//--------------------------

//--------------------------
// 电话呼入 - 1
//------------
#define TCP_PCS2AGT_CALL_CALLIN				1051		// (0x041B) 电话呼入消息
struct S_TCP_PCS2AGT_CALL_CALLIN
{
	UL		AgentID;						// 座席ID
	UL		CallRefID;						// 呼叫标示
	char	TimeStamp[15];					// "YYYYMMDDHHMMSS\0"
	UC		EventSource;					// 事件来源，参照_pcC_EventSource
	UC		CallSource;						// 呼叫来源来源，参照_pcC_CallSource
	US		RoutePoint;						// 电话进入的路由点
	US		GroupID;						// 电话进入的座席组编号
	US		WaitTime;						// 排队等待时间（秒）
	UL		TrunkID;						// 中继号
	char	strANI[PCS_DEF_NUMLEN_APP];		// 主叫
	char	strDNIS[PCS_DEF_NUMLEN_APP];	// 被叫
	char	strOtherParty[PCS_DEF_NUMLEN_APP];		// 内部呼叫对方DN
	char	strThirdParty[PCS_DEF_NUMLEN_APP];		// 转接方号码
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------
//--------------------------

//--------------------------
// 电话挂断 - 3
//------------
#define TCP_AGT2PCS_CALL_CLEARCALL			52			// (0x0034) 电话挂断请求消息
struct S_TCP_AGT2PCS_CALL_CLEARCALL
{
	UL		CallRefID;						// 呼叫标示，如果为0则系统自动选择
	UC		EventSource;					// 事件来源，参照_pcC_EventSource，如果为0则系统自动选择
};
//------------

//------------
#define TCP_PCS2AGT_CALL_CALLCLEARED		1052		// (0x041C) 电话挂断通知消息
struct S_TCP_PCS2AGT_CALL_CALLCLEARED
{
	UL		AgentID;						// 座席ID
	UL		CallRefID;						// 呼叫标示
	char	TimeStamp[15];					// "YYYYMMDDHHMMSS\0"
	UC		EventSource;					// 事件来源，参照_pcC_EventSource
	UC		Operator;						// 操作主体，参照_pcC_CallOperator
	char	strPeerParty[PCS_DEF_NUMLEN_APP];		// 对方DN
};
//------------

//------------
#define TCP_PCS2AGT_CALL_UNRINGING			1056		// (0x0420) 电话放弃通知消息
struct S_TCP_PCS2AGT_CALL_UNRINGING
{
	UL		AgentID;						// 座席ID
	UL		CallRefID;						// 呼叫标示
	char	TimeStamp[15];					// "YYYYMMDDHHMMSS\0"
	UC		EventSource;					// 事件来源，参照_pcC_EventSource
	UC		Operator;						// 操作主体，参照_pcC_CallOperator
	char	strPeerParty[PCS_DEF_NUMLEN_APP];		// 对方DN
};
//------------
//--------------------------

//--------------------------
// 电话应答 - 2
//------------
#define TCP_AGT2PCS_CALL_ANSWERCALL			53			// (0x0035) 电话应答请求消息
struct S_TCP_AGT2PCS_CALL_ANSWERCALL
{
	UL		CallRefID;						// 呼叫标示，如果为0则系统自动选择
	UC		EventSource;					// 事件来源，参照_pcC_EventSource，如果为0则系统自动选择
};
//------------

//------------
#define TCP_PCS2AGT_CALL_CALLANSWERED		1053		// (0x041D) 电话应答通知消息
struct S_TCP_PCS2AGT_CALL_CALLANSWERED
{
	UL		AgentID;						// 座席ID
	UL		CallRefID;						// 呼叫标示
	char	TimeStamp[15];					// "YYYYMMDDHHMMSS\0"
	UC		EventSource;					// 事件来源，参照_pcC_EventSource
	UC		Operator;						// 操作主体，参照_pcC_CallOperator
	char	strPeerParty[PCS_DEF_NUMLEN_APP];		// 对方DN
};
//------------
//--------------------------

//--------------------------
// 电话保持 - 6
//------------
#define TCP_AGT2PCS_CALL_HOLDCALL			54			// (0x0036) 电话保持请求消息
struct S_TCP_AGT2PCS_CALL_HOLDCALL
{
	UL		CallRefID;						// 呼叫标示，如果为0则系统自动选择
	UC		EventSource;					// 事件来源，参照_pcC_EventSource，如果为0则系统自动选择
};
//------------

//------------
#define TCP_PCS2AGT_CALL_CALLHELD			1054		// (0x041E) 电话保持通知消息
struct S_TCP_PCS2AGT_CALL_CALLHELD
{
	UL		AgentID;						// 座席ID
	UL		CallRefID;						// 呼叫标示
	char	TimeStamp[15];					// "YYYYMMDDHHMMSS\0"
	UC		EventSource;					// 事件来源，参照_pcC_EventSource
	UC		Operator;						// 操作主体，参照_pcC_CallOperator
	char	strPeerParty[PCS_DEF_NUMLEN_APP];		// 对方DN
};
//------------

//------------
#define TCP_AGT2PCS_CALL_RETRIEVECALL		55			// (0x0037) 电话拾起请求消息
struct S_TCP_AGT2PCS_CALL_RETRIEVECALL
{
	UL		CallRefID;						// 呼叫标示，如果为0则系统自动选择
	UC		EventSource;					// 事件来源，参照_pcC_EventSource，如果为0则系统自动选择
};
//------------

//------------
#define TCP_PCS2AGT_CALL_CALLRETRIEVED		1055		// (0x041F) 电话拾起通知消息
struct S_TCP_PCS2AGT_CALL_CALLRETRIEVED
{
	UL		AgentID;						// 座席ID
	UL		CallRefID;						// 呼叫标示
	char	TimeStamp[15];					// "YYYYMMDDHHMMSS\0"
	UC		EventSource;					// 事件来源，参照_pcC_EventSource
	UC		Operator;						// 操作主体，参照_pcC_CallOperator
	char	strPeerParty[PCS_DEF_NUMLEN_APP];		// 对方DN
};
//------------

//------------
#define TCP_AGT2PCS_CALL_RECONNECTCALL		56			// (0x0038) 电话重新连接请求消息
struct S_TCP_AGT2PCS_CALL_RECONNECTCALL
{
	UL		CallRefID;						// 呼叫标示，如果为0则系统自动选择
	UC		EventSource;					// 事件来源，参照_pcC_EventSource，如果为0则系统自动选择
};

//------------
//--------------------------

//------------
// Simon Added 2011-12-15 增加电话中继信息通知
#define TCP_PCS2AGT_CALL_TRUNKINFO		1057		// (0x0421) 电话中继信息通知消息
struct S_TCP_PCS2AGT_CALL_TRUNKINFO
{
	UL		AgentID;						// 座席ID
	UL		CallRefID;						// 呼叫标示
	char	TrunkGroup[20];					// 中继组号
	char	TrunkMember[20];				// 中继成员
    char	TimeStamp[15];                  // "YYYYMMDDHHMMSS\0"
    char	strANI[PCS_DEF_NUMLEN_APP];     // 主叫
    char	strDNIS[PCS_DEF_NUMLEN_APP];	// 被叫
    char	strOtherParty[PCS_DEF_NUMLEN_APP];         // 内部呼叫对方DN
    char	strThirdParty[PCS_DEF_NUMLEN_APP];         // 转接方号码
    char	strAppData[PCS_DEF_DATALEN_APP];           // Application Data
    char	strUserData[PCS_DEF_DATALEN_USER];         // User Data
};
//------------
//--------------------------

//--------------------------
// 三方操作 - 10
//------------
#define TCP_AGT2PCS_CALL_CONSULTATION		61			// (0x003D) 呼叫磋商消息
struct S_TCP_AGT2PCS_CALL_CONSULTATION
{
	UL		CallRefID;						// 呼叫标示，如果为0则系统自动选择
	UC		EventSource;					// 事件来源，参照_pcC_EventSource，如果为0则系统自动选择
	UC		ConsultMode;					// 呼叫磋商模式，参照_pcC_ConsultationMode
	UC		CallScale;						// 呼叫范围，参照_pcC_CallScale
	char	strCalledParty[PCS_DEF_NUMLEN_APP];		// 外拨号码
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------

//------------
#define TCP_AGT2PCS_CALL_TRANSFER			62			// (0x003E) 电话转接消息
struct S_TCP_AGT2PCS_CALL_TRANSFER
{
	UL		HoldRefID;						// 保持呼叫标志，如果为0则系统自动选择
	UL		CallRefID;						// 呼叫标示，如果为0则系统自动选择
	UC		EventSource;					// 事件来源，参照_pcC_EventSource，如果为0则系统自动选择
};
//------------

//------------

//------------
#define TCP_AGT2PCS_CALL_CONFERENCE			63			// (0x003F) 电话会议消息
struct S_TCP_AGT2PCS_CALL_CONFERENCE
{
	UL		HoldRefID;						// 保持呼叫标志，如果为0则系统自动选择
	UL		CallRefID;						// 呼叫标示，如果为0则系统自动选择
	UC		EventSource;					// 事件来源，参照_pcC_EventSource，如果为0则系统自动选择
};
//------------

//------------
#define TCP_AGT2PCS_CALL_SINGLESTEPTRANS	64			// (0x0040) 单步转接消息
struct S_TCP_AGT2PCS_CALL_SINGLESTEPTRANS
{
	UL		CallRefID;						// 呼叫标示，如果为0则系统自动选择
	UC		EventSource;					// 事件来源，参照_pcC_EventSource，如果为0则系统自动选择
	UC		CallScale;						// 呼叫范围，参照_pcC_CallScale
	char	strCalledParty[PCS_DEF_NUMLEN_APP];		// 外拨号码
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------

//------------
#define TCP_PCS2AGT_CALL_NETWORKREACHED		1061		// (0x0425) 外拨通道建立通知消息
struct S_TCP_PCS2AGT_CALL_NETWORKREACHED
{
	UL		AgentID;						// 座席ID
	UL		CallRefID;						// 呼叫标示
	char	TimeStamp[15];					// "YYYYMMDDHHMMSS\0"
	UC		EventSource;					// 事件来源，参照_pcC_EventSource
	char	strPeerParty[PCS_DEF_NUMLEN_APP];		// 对方DN
};
//------------

//------------
#define TCP_PCS2AGT_CALL_TRANSFERED			1062		// (0x0426) 转接完成通知消息
struct S_TCP_PCS2AGT_CALL_TRANSFERED
{
	UL		AgentID;						// 座席ID
	UL		CallRefID;						// 呼叫标示
	char	TimeStamp[15];					// "YYYYMMDDHHMMSS\0"
	UC		EventSource;					// 事件来源，参照_pcC_EventSource
	char	strOtherParty[PCS_DEF_NUMLEN_APP];		// 呼叫对方DN
	char	strThirdParty[PCS_DEF_NUMLEN_APP];		// 转接方号码
};
//------------

//------------
#define TCP_PCS2AGT_CALL_CONFERENCED		1063		// (0x0427) 会议完成通知消息
struct S_TCP_PCS2AGT_CALL_CONFERENCED
{
	UL		AgentID;						// 座席ID
	UL		CallRefID;						// 呼叫标示
	char	TimeStamp[15];					// "YYYYMMDDHHMMSS\0"
	UC		EventSource;					// 事件来源，参照_pcC_EventSource
	char	strOtherParty[PCS_DEF_NUMLEN_APP];		// 呼叫对方DN
	char	strThirdParty[PCS_DEF_NUMLEN_APP];		// 会议方号码
};
//------------

//------------
#define TCP_PCS2AGT_CALL_DESTINATIONBUSY	1064		// (0x0428) 外拨遇忙通知消息
struct S_TCP_PCS2AGT_CALL_DESTINATIONBUSY
{
	UL		AgentID;						// 座席ID
	UL		CallRefID;						// 呼叫标示
	char	TimeStamp[15];					// "YYYYMMDDHHMMSS\0"
	UC		EventSource;					// 事件来源，参照_pcC_EventSource
	char	strPeerParty[PCS_DEF_NUMLEN_APP];		// 对方DN
};
//------------

//------------
#define TCP_PCS2AGT_CALL_DESTINATIONINVALID	1065		// (0x0429) 外拨目的无效通知消息
struct S_TCP_PCS2AGT_CALL_DESTINATIONINVALID
{
	UL		AgentID;						// 座席ID
	UL		CallRefID;						// 呼叫标示
	char	TimeStamp[15];					// "YYYYMMDDHHMMSS\0"
	UC		EventSource;					// 事件来源，参照_pcC_EventSource
	char	strPeerParty[PCS_DEF_NUMLEN_APP];		// 对方DN
};
//------------

//------------
#define TCP_PCS2AGT_CALL_CONSULTATION_INIT	1066		// (0x042A) 转接、会议初始化完成消息
struct S_TCP_PCS2AGT_CALL_CONSULTATION_INIT
{
	UL		AgentID;						// 座席ID
	UL		CallRefID;						// 呼叫标示
	UL		HoldRefID;						// 保持呼叫标志
	char	TimeStamp[15];					// "YYYYMMDDHHMMSS\0"
	UC		EventSource;					// 事件来源，参照_pcC_EventSource
	UC		ConsultMode;					// 呼叫磋商模式，参照_pcC_ConsultationMode
	char	strPeerParty[PCS_DEF_NUMLEN_APP];		// 对方DN
};
//------------
//--------------------------

//--------------------------
// 驻留、取回（扩展功能，必须使用Softphone） - 5
//------------
#define TCP_AGT2PCS_CALL_PARKING			71			// (0x0047) 呼叫驻留请求
struct S_TCP_AGT2PCS_CALL_PARKING
{
	UL		CallRefID;						// 呼叫标示，如果为0则系统自动选择
	US		ParkedRoutePoint;				// 驻留电话的路由点ID
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------

//------------
#define TCP_AGT2PCS_CALL_FETCHING			72			// (0x0048) 呼叫取回请求
struct S_TCP_AGT2PCS_CALL_FETCHING
{
	UL		CallRefID;						// 呼叫标示
	US		ParkedRoutePoint;				// 驻留电话的路由点ID
};
//------------

//------------
#define TCP_PCS2AGT_CALL_PARKING_ACK		1071		// (0x042F) 呼叫驻留响应消息
struct S_TCP_PCS2AGT_CALL_PARKING_ACK
{
	UL		CallRefID;						// 呼叫标示
	US		ParkedRoutePoint;				// 驻留电话的路由点ID
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
#define TCP_PCS2AGT_CALL_FETCHING_ACK		1072		// (0x0430) 呼叫取回响应消息
struct S_TCP_PCS2AGT_CALL_FETCHING_ACK
{
	US		ParkedRoutePoint;				// 驻留电话的路由点ID
	UL		CallRefID;						// 呼叫标示
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_NOTSUPPORT
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2AGT_CALL_PARK_MISSED		1073		// (0x0431) 驻留呼叫丢失消息
struct S_TCP_PCS2AGT_CALL_PARK_MISSED
{
	UL		CallRefID;						// 呼叫标示
	US		ParkedRoutePoint;				// 驻留电话的路由点ID
};
//------------
//--------------------------

//--------------------------
// 其它扩展功能，必须使用Softphone - 5
//------------
#define TCP_AGT2PCS_SEND_DTMF				81			// (0x0051) 发送DTMF请求消息
struct S_TCP_AGT2PCS_SEND_DTMF
{
	UL		CallRefID;						// 呼叫标示，如果为0则系统自动选择
	UC		EventSource;					// 事件来源，参照_pcC_EventSource，如果为0则系统自动选择
	char	strDTMF[PCS_DEF_NUMLEN_APP];	// 需要发送的DTMF串
};
//------------

//------------
#define TCP_AGT2PCS_TELNO_PARSE				82			// (0x0052) 电话号码解析请求消息
struct S_TCP_AGT2PCS_TELNO_PARSE
{
	UL		InvokeID;						// 调用编号
	UC		CallType;						// 呼叫类型，主要区分呼入/呼出，参照_pcC_CallType
	UC		blnConnected;					// 是否接通，分析呼出号码是需提供，参见“开关常量定义”
	char	strOrigNo[PCS_DEF_NUMLEN_APP];	// 原始号码
};
//------------

//------------
#define TCP_PCS2AGT_TELNO_PARSE_ACK			1082		// (0x043A) 电话号码解析响应消息
struct S_TCP_PCS2AGT_TELNO_PARSE_ACK
{
	UL		InvokeID;						// 调用编号
	UC		CallType;						// 呼叫类型，主要区分呼入/呼出，参照_pcC_CallType
	UC		blnConnected;					// 是否接通，分析呼出号码是需提供，参见“开关常量定义”
	char	strOrigNo[PCS_DEF_NUMLEN_APP];	// 原始号码
	UC		bytCallScale;					// 解析结果：呼叫范围（仅呼出），参见_pcC_CallScale
	char	cIPIndex;						// 解析结果：IP前缀编号（仅呼出）
	char	strCountryCode[8];				// 解析结果：国家代码
	char	strCityCode[8];					// 解析结果：区号
	char	strPureNo[PCS_DEF_NUMLEN_APP];	// 解析结果：号码
};
//------------

//------------
// Simon Added 2011-12-15
#define TCP_AGT2PCS_CALL_PICKUP				83			// (0x0053) 请求呼叫代答
struct S_TCP_AGT2PCS_CALL_PICKUP
{
	UC		bytScale;						// 操作目标 参考:_pcC_CallDistributeObject
	char	strDestination[50];				// 目标号码
};

#define TCP_PCS2AGT_CALL_PICKUP_ACK			1083		// (0x0453) 回应呼叫代答
struct S_TCP_PCS2AGT_CALL_PICKUP_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST: 指定的代答目标不存在
											// PCS_DEF_RETCODE_BEUSED: 本座席话机正在通话，不能代答其他电话
											// PCS_DEF_RETCODE_STATUS: 指定的代答目标没有可以代答的电话（如：分机不在振铃状态；组中没有正在振铃的座席等）
											// PCS_DEF_RETCODE_VERSION: 此版本不支持本操作
											// PCS_DEF_RETCODE_NORIGHTS: 座席无代答权限
											// PCS_DEF_RETCODE_LACK_PARAM: 参数不足
											// PCS_DEF_RETCODE_REJECTED: 系统拒绝操作
											// PCS_DEF_RETCODE_NOTSUPPORT: 交换机不支持本操作
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		CallRefID;						// 被代答的呼叫标示
	char	strPickupDN[PCS_DEF_NUMLEN_APP];	// 被代答的分机号
};
//------------
//--------------------------


//--------------------------
// 其他 - 1
//------------
#define TCP_PCS2AGT_CALL_FUNCTION_ACK		1091		// (0x0443) 呼叫控制响应消息
struct S_TCP_PCS2AGT_CALL_FUNCTION_ACK
{
	US		FunctionCode;					// 功能代码
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_LACK_PARAM
											// PCS_DEF_RETCODE_NOTSUPPORT
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------


//--------------------------
// 管理消息 - Total 14
//--------------------------

//--------------------------
// 修改口令 - 2
//------------
#define TCP_AGT2PCS_CHANGE_PASSWORD			101			// (0x0065) 修改口令消息
struct S_TCP_AGT2PCS_CHANGE_PASSWORD
{
	char	strPassword[20];				// 新口令
};
//------------

//------------
#define TCP_PCS2AGT_CHANGE_PASSWORD_ACK		1101		// (0x044D) 修改口令响应消息
struct S_TCP_PCS2AGT_CHANGE_PASSWORD_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------

//--------------------------
// 用户参数管理 - 6
//------------
#define TCP_AGT2PCS_GET_AGTPARAM			102			// (0x0066) 查询用户参数
struct S_TCP_AGT2PCS_GET_AGTPARAM
{
	char	strParamName[50];				// 参数名
};
//------------

//------------
#define TCP_AGT2PCS_CHG_AGTPARAM			103			// (0x0067) 修改用户参数
struct S_TCP_AGT2PCS_CHG_AGTPARAM
{
	char	strParamName[50];				// 参数名
	char	strParamValue[100];				// 参数值
};
//------------

//------------
#define TCP_PCS2AGT_GET_AGTPARAM_ACK		1102		// (0x044E) 查询用户参数响应消息
struct S_TCP_PCS2AGT_GET_AGTPARAM_ACK
{
	char	strParamName[50];				// 参数名
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	char	strParamValue[100];				// 参数值
};
//------------

//------------
#define TCP_PCS2AGT_CHG_AGTPARAM_ACK		1103		// (0x044F) 修改用户参数响应消息
struct S_TCP_PCS2AGT_CHG_AGTPARAM_ACK
{
	char	strParamName[50];				// 参数名
	char	strParamValue[100];				// 参数值
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2PCS_GET_LEAVEREASONS		104			// (0x0068) 请求获取事由列表
//------------

//------------
#define TCP_PCS2AGT_LEAVEREASONS_LIST		1104		// (0x0450) 事由列表
struct S_TCP_PCS2AGT_LEAVEREASONS_LIST
{
	UC		ReasonCount;					// 事由数量
	UC		ReasonValue[10];				// 事由编号0-9
	char	ReasonName[10][20];				// 事由名称0-9
};
//------------

//--------------------------
//Simon Added 2011-12-15
#define TCP_AGT2PCS_RESET_OBANI				105			// (0x0069) 重置OB主叫号码推送
struct S_TCP_AGT2PCS_RESET_OBANI
{
	char	strANI[50];				// 主叫号码
};

//------------
//--------------------------



//--------------------------
// 班长强拆 - 2
//------------
#define TCP_AGT2PCS_MONITOR_BREAK			111			// (0x006F) 班长强拆消息
struct S_TCP_AGT2PCS_MONITOR_BREAK
{
	UL		ControlAgentID;					// 被控制座席ID
};
//------------

//------------
#define TCP_PCS2AGT_MONITOR_BREAK_ACK		1111		// (0x0457) 班长强拆响应消息
struct S_TCP_PCS2AGT_MONITOR_BREAK_ACK
{
	UL		ControlAgentID;					// 被控制座席ID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------

//--------------------------
// 班长拦截 - 2
//------------
#define TCP_AGT2PCS_MONITOR_HOLDUP			112			// (0x0070) 班长拦截消息
struct S_TCP_AGT2PCS_MONITOR_HOLDUP
{
	UL		ControlAgentID;					// 被控制座席ID
};
//------------

//------------
#define TCP_PCS2AGT_MONITOR_HOLDUP_ACK		1112		// (0x0458) 班长拦截响应消息
struct S_TCP_PCS2AGT_MONITOR_HOLDUP_ACK
{
	UL		ControlAgentID;					// 被控制座席ID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------

//--------------------------
// 班长强插 - 2
//------------
#define TCP_AGT2PCS_MONITOR_INSERT			113			// (0x0071) 班长强插消息
struct S_TCP_AGT2PCS_MONITOR_INSERT
{
	UL		ControlAgentID;					// 被控制座席ID
};
//------------

//------------
#define TCP_PCS2AGT_MONITOR_INSERT_ACK		1113		// (0x0459) 班长强插响应消息
struct S_TCP_PCS2AGT_MONITOR_INSERT_ACK
{
	UL		ControlAgentID;					// 被控制座席ID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------

//--------------------------
// 班长强制注销 - 2
//------------
#define TCP_AGT2PCS_MONITOR_LOGOUT			114			// (0x0072) 班长强制注销消息
struct S_TCP_AGT2PCS_MONITOR_LOGOUT
{
	UL		ControlAgentID;					// 被控制座席ID
};
//------------

//------------
#define TCP_PCS2AGT_MONITOR_LOGOUT_ACK		1114		// (0x045A) 班长强制注销响应消息
struct S_TCP_PCS2AGT_MONITOR_LOGOUT_ACK
{
	UL		ControlAgentID;					// 被控制座席ID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------

//--------------------------
// 文字消息服务消息 - Total 4
//--------------------------
//------------
#define TCP_AGT2PCS_MSGRSTART_ACK			121			// (0x0079) 消息服务启动响应消息
struct S_TCP_AGT2PCS_MSGRSTART_ACK
{
	UC		MessagerClientStarted;			// 消息服务客户端是否启动，参见“开关常量定义”
};
//------------

//------------
#define TCP_AGT2PCS_MSGRSTOP_ACK			122			// (0x007A) 消息服务停止响应消息
//------------

//------------
#define TCP_PCS2AGT_MSGRSTART				1121		// (0x0461) 消息服务启动消息，有响应权限才会收到此消息
struct S_TCP_PCS2AGT_MSGRSTART
{
	US		nMSGPort;						// 消息服务器侦听端口
	char	strMSGRIP[20];					// 消息服务器IP地址
};
//------------

//------------
#define TCP_PCS2AGT_MSGRSTOP				1122		// (0x0462) 消息服务停止消息
//------------
//--------------------------

//--------------------------
// 录音服务消息 - Total 8
//--------------------------
//------------
#define TCP_AGT2PCS_MAXISTART_ACK			131			// (0x0083) 录音服务启动响应消息
struct S_TCP_AGT2PCS_MAXISTART_ACK
{
	UC		MaxiSenseClientStarted;			// 录音服务客户端是否启动，参见“开关常量定义”
};
//------------

//------------
#define TCP_AGT2PCS_MAXISTOP_ACK			132			// (0x0084) 录音服务停止响应消息
//------------

//------------
#define TCP_PCS2AGT_MAXISTART				1131		// (0x046B) 录音服务启动消息，有响应权限才会收到此消息
struct S_TCP_PCS2AGT_MAXISTART
{
	US		nMaxiSensePort;					// 录音服务服务器侦听端口
	char	strMaxiSenseIP[20];				// 录音服务服务器IP地址
};
//------------

//------------
#define TCP_PCS2AGT_MAXISTOP				1132		// (0x046C) 录音服务停止消息
//------------

//------------------------------------------------------------------------------
// Sun added 2010-05-12
// 录音状态通知
//------------
#define TCP_AGT2PCS_REC_USERDATA			135			// (0x0087) 追加用户数据到最近一次成功录音
struct S_TCP_AGT2PCS_REC_USERDATA
{
	char	UserData[200];					// 用户数据
};
//------------

//------------
#define TCP_AGT2PCS_REC_RECORDDATA			136			// (0x0088) 录音附加数据消息
struct S_TCP_AGT2PCS_REC_RECORDDATA
{
	char	AttachData1[16];				// 附加数据项1
	char	AttachData2[16];				// 附加数据项2
	char	AttachData3[32];				// 附加数据项3
	char	AttachData4[32];				// 附加数据项4
	char	AttachData5[256];				// 附加数据项5
};
//------------

//------------
#define TCP_PCS2AGT_RECORD_STARTED			1135		// (0x046F) 录音开始通知
struct S_TCP_PCS2AGT_RECORD_STARTED
{
	char	RecordTime[15];					// 录音开始时间，PK
	char	UserName[20];					// 用户名，PK
	UC		RecordStartType;				// 录音启动控制方式，参见“录音控制方式”
	char	FilePath[128];					// 录音文件全路径
};
//------------

//------------
#define TCP_PCS2AGT_RECORD_STOPED			1136		// (0x0470) 录音停止通知
struct S_TCP_PCS2AGT_RECORD_STOPED
{
	char	RecordTime[15];					// 录音开始时间，PK
	char	UserName[20];					// 用户名，PK
	UC		RecordEndType;					// 录音停止控制方式，参见“录音控制方式”
	UL		RecordDuration;					// 录音时长
	char	FilePath[128];					// 录音文件全路径
};
//------------
// End of Sun added 2010-05-12
//------------------------------------------------------------------------------
//--------------------------


//--------------------------
// UMS处理 - Total 13
//--------------------------

//--------------------------
// 统一任务分派 - 2
//------------
#define TCP_AGT2PCS_UMS_DISPATCHTASK_ACK	3201		// (0x0C81) 统一任务分派响应消息
struct S_TCP_AGT2PCS_UMS_DISPATCHTASK_ACK
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
	US		RoutePointID;							// 路由点ID
	UC		ErrorCode;								// 返回码，参见“开关常量定义”
	UL		ReasonCode;								// 离队原因代码
	char	strRefField1[10];						// 任务参照字段1
	char	strRefField2[20];						// 任务参照字段2
	char	strRefField3[20];						// 任务参照字段3
	char	strRefField4[50];						// 任务参照字段4
	char	strRefField5[100];						// 任务参照字段5
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------

//------------
#define TCP_PCS2AGT_UMS_DISPATCHTASK_NTY	3701		// (0x0E75) 统一任务分派消息
struct S_TCP_PCS2AGT_UMS_DISPATCHTASK_NTY
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
	UC		UMSType;								// UMS接入类型，参见_pcC_UMSTaskTypes
	UC		UMSIndex;								// UMS子系统编号
	UC		CustID;									// 租户代码
	US		RoutePointID;							// 路由点ID
	US		GroupID;								// 组ID
	US		TeamID;									// 小组ID
	UC		TeamIndex;								// 小组优先级
	UC		UserIndex;								// 话务员优先级
	char	strCallingParty[PCS_DEF_NUMLEN_APP];	// 发起者
	char	strCalledParty[PCS_DEF_NUMLEN_APP];		// 接收者
	UL		ClassifyCode;							// 分类代码
	UL		WaitTime;								// 已等待时间（秒）
	char	ServiceCode0;							// 主服务代码
	char	ServiceCode1;							// 二级服务代码
	char	ServiceCode2;							// 三级服务代码
	char	strTaskTitle[50];						// 任务标题
	char	strTaskContent[200];					// 任务描述
	char	strHotLink[100];						// 相关文件路径或连接地址
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------
//--------------------------

//--------------------------
// 统一任务取消 - 3
//------------
#define TCP_AGT2PCS_UMS_ABANDONTASK_REQ		3202		// (0x0C82) 统一任务放弃消息
struct S_TCP_AGT2PCS_UMS_ABANDONTASK_REQ
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
	UL		ReasonCode;								// 离队原因代码
	char	strRefField1[10];						// 任务参照字段1
	char	strRefField2[20];						// 任务参照字段2
	char	strRefField3[20];						// 任务参照字段3
	char	strRefField4[50];						// 任务参照字段4
	char	strRefField5[100];						// 任务参照字段5
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------

//------------
#define TCP_AGT2PCS_UMS_CANCELTASK_ACK		3203		// (0x0C83) 统一任务取消响应消息
struct S_TCP_AGT2PCS_UMS_CANCELTASK_ACK
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
};
//------------

//------------
#define TCP_PCS2AGT_UMS_CANCELTASK_NTY		3702		// (0x0E76) 统一任务取消消息
struct S_TCP_PCS2AGT_UMS_CANCELTASK_NTY
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
	US		RoutePointID;							// 路由点ID
};
//------------
//--------------------------

//--------------------------
// 统一任务完成 - 2
//------------
#define TCP_AGT2PCS_UMS_FINISHTASK_REQ		3205		// (0x0C85) 统一任务完成消息
struct S_TCP_AGT2PCS_UMS_FINISHTASK_REQ
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
	UL		ReasonCode;								// 离队原因代码
	char	strRefField1[10];						// 任务参照字段1
	char	strRefField2[20];						// 任务参照字段2
	char	strRefField3[20];						// 任务参照字段3
	char	strRefField4[50];						// 任务参照字段4
	char	strRefField5[100];						// 任务参照字段5
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------

//------------
#define TCP_PCS2AGT_UMS_FINISHTASK_NTY		3705		// (0x0E79) 统一任务完成响应消息
struct S_TCP_PCS2AGT_UMS_FINISHTASK_NTY
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
	US		RoutePointID;							// 路由点ID
};
//------------
//--------------------------

//--------------------------
// 批量任务取消 - 3
//------------
#define TCP_AGT2PCS_UMS_ABANDONALLTASK_REQ	3208		// (0x0C88) 全部任务放弃消息
struct S_TCP_AGT2PCS_UMS_ABANDONALLTASK_REQ
{
	UL		ReasonCode;								// 离队原因代码
	char	strRefField1[10];						// 任务参照字段1
	char	strRefField2[20];						// 任务参照字段2
	char	strRefField3[20];						// 任务参照字段3
	char	strRefField4[50];						// 任务参照字段4
	char	strRefField5[100];						// 任务参照字段5
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------

//------------
#define TCP_AGT2PCS_UMS_CANCELALLTASK_ACK	3209		// (0x0C89) 全部任务取消响应消息
struct S_TCP_AGT2PCS_UMS_CANCELALLTASK_ACK
{
	UL		TaskCount;								// 任务数量
};
//------------

//------------
#define TCP_PCS2AGT_UMS_CANCELALLTASK_NTY	3708		// (0x0E7C) 全部任务取消消息
struct S_TCP_PCS2AGT_UMS_CANCELALLTASK_NTY
{
	UL		TaskCount;								// 任务数量
};
//------------
//--------------------------

//--------------------------
// 查询任务清单 - 3
//------------
#define TCP_AGT2PCS_UMS_QUERYTASKLIST_REQ	3211		// (0x0C8B) 查询坐席任务清单消息
//------------

//------------
#define TCP_PCS2AGT_UMS_QUERYTASKLIST_ACK	3711		// (0x0E7F) 查询坐席任务清单响应消息
struct S_TCP_PCS2AGT_UMS_QUERYTASKLIST_ACK
{
	UL		TaskCount;								// 任务数量
};
//------------

//------------
#define TCP_PCS2AGT_UMS_TASK_DATA			3712		// (0x0E78) 统一任务数据消息
struct S_TCP_PCS2AGT_UMS_TASK_DATA
{
	UL		TaskIndex;								// 任务编号，0 based
	US		TaskStatus;								// 统一消息任务状态
	S_TCP_PCS2AGT_UMS_DISPATCHTASK_NTY TaskData;
};
//------------
//--------------------------

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_AGENT_H__
//------------------------------------------------------------------------
