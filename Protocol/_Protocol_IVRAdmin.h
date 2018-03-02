////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		_Protocol_IVRAdmin.h
// 用途描述：	IVR通信管理员协议
// 作者：		SUN
// 创建时间：	2006/03/05
// 最后修改时间：2006/03/05
// 版权信息：	Copyright (C) 2003 - 2006 PicaSoft
// 备注：
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_IVRSADMIN_H__
#define __PROTOCOL_IVRSADMIN_H__
//------------------------------------------------------------------------

//--------------------------
// IVR Admin注册 - Total 4
//--------------------------

//--------------------------
// 注册 - 2
//------------
#define TCP_ADM2IVR_REGISTER				1			// (0x0001) 注册消息
struct S_TCP_ADM2IVR_REGISTER
{
	char	strUserID[50];					// 用户名
	char	strPassword[20];				// 口令
	UC		Version;						// 版本号
	UC		RegCode[PCS_DEF_NUMLEN_APP];	// 注册码
};
//------------

//------------
#define TCP_IVR2ADM_REGISTER_ACK			1001		// (0x03E9) 注册响应消息
struct S_TCP_IVR2ADM_REGISTER_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_BEUSED
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_VERSION
											// PCS_DEF_RETCODE_REGCODE
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UC		SysNo;							// 系统代号
	UC		GroupCount;						// 组数量
	US		ChannelCount;					// 通道数量
	UC		Version;						// 版本号
};
//------------
//--------------------------

//--------------------------
// 取消注册 - 2
//------------
#define TCP_ADM2IVR_UNREGISTER				2			// (0x0002) 取消注册消息
//------------

//------------
#define TCP_IVR2ADM_UNREGISTER_ACK			1002		// (0x03EA) 取消注册响应消息
//------------
//--------------------------
//--------------------------


//--------------------------
// IVR 监控消息 - Total 22
//--------------------------

//--------------------------
// 监控启动、退出消息 - 18
//------------
#define TCP_IVR2ADM_GROUP_MON_ACK				501			// (0x01F5) IVR组监控启动响应消息
struct S_TCP_IVR2ADM_GROUP_MON_ACK
{
	UC		GroupID;						// IVR Group ID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_IVR2ADM_GROUP_UNMON_ACK				502			// (0x01F6) IVR组监控结束响应消息
struct S_TCP_IVR2ADM_GROUP_UNMON_ACK
{
	UC		GroupID;						// IVR Group ID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_IVR2ADM_GROUP_PROP_NAME				503			// (0x01F7) IVR组属性报告消息
struct S_TCP_IVR2ADM_GROUP_PROP_NAME
{
	UC		GroupID;						// IVR Group ID
	UC		CTIEnabled;						// 是否使用CTI
	US		StartChannel;					// 开始通道
	US		EndChannel;						// 终止通道
	UC		CallFlowID;						// Call Flow ID
	US		ResourceID;						// Resource ID
	UL		DailyCallCount;					// 当日累计呼叫数量
	UL		YTDCallCount;					// 全部呼叫累计数量
	char	GroupName[50];					// IVR Group Name
	char	CallFlowName[50];				// Call Flow Name
	char	ResourceName[50];				// Resource Name
};
//------------

//------------
#define TCP_IVR2ADM_GROUP_PROP_CALLFLOW			504			// (0x01F8) IVR组流程改变消息
struct S_TCP_IVR2ADM_GROUP_PROP_CALLFLOW
{
	UC		GroupID;						// IVR Group ID
	UC		CallFlowID;						// Call Flow ID
	char	CallFlowName[50];				// Call Flow Name
};
//------------

//------------
#define TCP_IVR2ADM_GROUP_PROP_RESOURCE			505			// (0x01F9) IVR组资源改变消息
struct S_TCP_IVR2ADM_GROUP_PROP_RESOURCE
{
	UC		GroupID;						// IVR Group ID
	US		ResourceID;						// Resource ID
	char	ResourceName[50];				// Resource Name
};
//------------

//------------
#define TCP_IVR2ADM_GROUP_TIME_CALLFLOW_ACK		506			// (0x01FA) IVR组定时加载流程响应消息
struct S_TCP_IVR2ADM_GROUP_TIME_CALLFLOW_ACK
{
	UC		GroupID;						// IVR Group ID
	UC		CallFlowID;						// Call Flow ID
	char	LoadTime[15];					// 定时加载时间，YYYYMMDDHHMMSS
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_IVR2ADM_GROUP_TIME_RESOURCE_ACK		507			// (0x01FB) IVR组定时加载资源响应消息
struct S_TCP_IVR2ADM_GROUP_TIME_RESOURCE_ACK
{
	UC		GroupID;						// IVR Group ID
	UC		ResourceID;						// Call Flow ID
	char	LoadTime[15];					// 定时加载时间，YYYYMMDDHHMMSS
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_IVR2ADM_CH_ST_REPORT				508			// (0x01FC) IVR通道状态报告消息
struct S_TCP_IVR2ADM_CH_ST_REPORT
{
	UC		GroupID;						// IVR Group ID
	US		ChannelID;						// IVR Channel ID
	US		NodeID;							// Transfer to Node ID
	UL		DailyCallCount;					// 当日累计呼叫数量
	UL		YTDCallCount;					// 全部呼叫累计数量
};
//------------

//------------
#define TCP_IVR2ADM_CH_MON_ACK					510			// (0x01FE) IVR通道监控启动响应消息
struct S_TCP_IVR2ADM_CH_MON_ACK
{
	UC		GroupID;						// IVR Group ID
	US		ChannelID;						// IVR Channel ID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_IVR2ADM_CH_UNMON_ACK				511			// (0x01FF) IVR通道监控结束响应消息
struct S_TCP_IVR2ADM_CH_UNMON_ACK
{
	UC		GroupID;						// IVR Group ID
	US		ChannelID;						// IVR Channel ID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_ADM2IVR_GROUP_MON					1501		// (0x05DD) IVR组监控启动消息
struct S_TCP_ADM2IVR_GROUP_MON
{
	UC		GroupID;						// IVR Group ID
};
//------------

//------------
#define TCP_ADM2IVR_GROUP_UNMON					1502		// (0x05DE) IVR组监控结束消息
struct S_TCP_ADM2IVR_GROUP_UNMON
{
	UC		GroupID;						// IVR Group ID
};
//------------

//------------
#define TCP_ADM2IVR_GROUP_LOAD_CALLFLOW			1504		// (0x05E0) IVR组加载流程消息
struct S_TCP_ADM2IVR_GROUP_LOAD_CALLFLOW
{
	UC		GroupID;						// IVR Group ID
	UC		CallFlowID;						// Call Flow ID
};
//------------

//------------
#define TCP_ADM2IVR_GROUP_LOAD_RESOURCE			1505		// (0x05E1) IVR组加载资源消息
struct S_TCP_ADM2IVR_GROUP_LOAD_RESOURCE
{
	UC		GroupID;						// IVR Group ID
	UC		ResourceID;						// Call Flow ID
};
//------------

//------------
#define TCP_ADM2IVR_GROUP_TIME_CALLFLOW			1506		// (0x05E2) IVR组定时加载流程消息
struct S_TCP_ADM2IVR_GROUP_TIME_CALLFLOW
{
	UC		GroupID;						// IVR Group ID
	UC		CallFlowID;						// Call Flow ID
	char	LoadTime[15];					// 定时加载时间，YYYYMMDDHHMMSS
};
//------------

//------------
#define TCP_ADM2IVR_GROUP_TIME_RESOURCE			1507		// (0x05E3) IVR组定时加载资源消息
struct S_TCP_ADM2IVR_GROUP_TIME_RESOURCE
{
	UC		GroupID;						// IVR Group ID
	UC		ResourceID;						// Call Flow ID
	char	LoadTime[15];					// 定时加载时间，YYYYMMDDHHMMSS
};
//------------

//------------
#define TCP_ADM2IVR_CH_MON						1510		// (0x05E6) IVR通道监控启动消息
struct S_TCP_ADM2IVR_CH_MON
{
	UC		GroupID;						// IVR Group ID
	US		ChannelID;						// IVR Channel ID
};
//------------

//------------
#define TCP_ADM2IVR_CH_UNMON					1511		// (0x05E7) IVR通道监控结束消息
struct S_TCP_ADM2IVR_CH_UNMON
{
	UC		GroupID;						// IVR Group ID
	US		ChannelID;						// IVR Channel ID
};
//------------
//--------------------------

//--------------------------
// 通道状态消息 - 2
//------------
#define TCP_IVR2ADM_CH_ST_START					521			// (0x0209) IVR通道进入服务状态消息
struct S_TCP_IVR2ADM_CH_ST_START
{
	UC		GroupID;						// IVR Group ID
	US		ChannelID;						// IVR Channel ID
	char	strANI[PCS_DEF_NUMLEN_APP];		// 主叫号码
};

//------------
#define TCP_IVR2ADM_CH_ST_STOP					522			// (0x0209) IVR通道结束服务状态消息
struct S_TCP_IVR2ADM_CH_ST_STOP
{
	UC		GroupID;						// IVR Group ID
	US		ChannelID;						// IVR Channel ID
	US		CallLength;						// 通话时长（秒）
};
//------------

//--------------------------

//--------------------------
// 流程跳转、控制消息 - 2
//------------
#define TCP_IVR2ADM_NODE_TRANSFER				531			// (0x0213) IVR通道流程跳转消息
struct S_TCP_IVR2ADM_NODE_TRANSFER
{
	UC		GroupID;						// IVR Group ID
	US		ChannelID;						// IVR Channel ID
	US		NodeID;							// Transfer to Node ID
	char	KeyPress[PCS_DEF_DATALEN_APP];	// Key Pressed in previous node
};
//------------

//------------
#define TCP_ADM2IVR_NODE_TRANSFER				1531		// (0x05FB) IVR通道流程跳转控制消息
struct S_TCP_ADM2IVR_NODE_TRANSFER
{
	UC		GroupID;						// IVR Group ID
	US		ChannelID;						// IVR Channel ID
	US		NodeID;							// Transfer to Node ID
};
//------------
//--------------------------

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_IVRADMIN_H__
//------------------------------------------------------------------------
