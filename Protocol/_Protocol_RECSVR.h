////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		_Protocol_RECSVR.h
// 用途描述：	录音服务通信协议
// 作者：		SUN
// 创建时间：	1999/12/01
// 最后修改时间：2003/12/15
// 版权信息：	Copyright (C) 1999 - 2003 PCSG
// 备注：
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_RECORDSERVER_H__
#define __PROTOCOL_RECORDSERVER_H__
//------------------------------------------------------------------------

//--------------------------
// 录音服务注册 - Total 4
//--------------------------

//--------------------------
// 注册 - 2
//------------
#define TCP_REC2PCS_REGISTER				1			// (0x0001) 注册消息
struct S_TCP_REC2PCS_REGISTER
{
	UC		SysNo;							// 系统代号
	UC		Version;						// 版本号
	UC		RegCode[PCS_DEF_NUMLEN_APP];	// 注册码
	US		nMaxiSensePort;					// 录音服务服务器侦听端口
	char	strMaxiSenseIP[20];				// 录音服务服务器IP地址
};
//------------

//------------
#define TCP_PCS2REC_REGISTER_ACK			1001		// (0x03E9) 注册响应消息
struct S_TCP_PCS2REC_REGISTER_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_BEUSED
											// PCS_DEF_RETCODE_VERSION
											// PCS_DEF_RETCODE_REGCODE
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------

//--------------------------
// 取消注册 - 2
//------------
#define TCP_REC2PCS_UNREGISTER				2			// (0x0002) 取消注册消息
//------------

//------------
#define TCP_PCS2REC_UNREGISTER_ACK			1002		// (0x03EA) 取消注册响应消息
//------------
//--------------------------

//--------------------------
// 录音服务管理 - Total 2
//--------------------------
//------------
#define TCP_REC2PCS_RELOAD_USERLIST_ACK		701			// (0x02BD) 重新加载用户列表响应消息
struct S_TCP_REC2PCS_RELOAD_USERLIST_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2REC_RELOAD_USERLIST			1701		// (0x06A5) 重新加载用户列表消息
//------------
//--------------------------


//--------------------------
// 状态查询 - 4
//------------
#define TCP_REC2PCS_STATUS_QUERY			711			// (0x02C7) 查询状态消息
struct S_TCP_REC2PCS_STATUS_QUERY
{
	UL		AgentID;						// 座席ID
};
//------------

//------------
#define TCP_REC2PCS_USER_STATUS_QUERY		712			// (0x02C8) 查询指定话务员状态消息
struct S_TCP_REC2PCS_USER_STATUS_QUERY
{
	char	strAliasName[50];				// 登录别名
};
//------------

//------------
#define TCP_PCS2REC_STATUS_QUERY_ACK		1711		// (0x06AF) 查询状态响应消息
struct S_TCP_PCS2REC_STATUS_QUERY_ACK
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
#define TCP_PCS2REC_USER_STATUS_QUERY_ACK	1712		// (0x06B0) 查询指定话务员状态响应消息
struct S_TCP_PCS2REC_USER_STATUS_QUERY_ACK
{
	char	strAliasName[50];				// 登录别名
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
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
//--------------------------

//
// 录音索引定义 added by stephen wang 2006-12-08
//
struct S_RECORD_INDEX_NODE					// Total 195 Bytes
{
	UC			ChannelIndex;				// 通道号
	char		StartRecordTime[16];		// 录音开始时间
	UC			CallDirection;				// 通话方向
	UL			RecLength;					// 录音时长
	char		TelNum[25];					// 电话号码
	char		KeyPress[20];				// 用户按键
	char		FilePath[128];				// 录音文件本地路径

};


//------------------------------------------------------------------------------
// Sun added 2010-05-12
// 录音状态通知
//------------
#define TCP_REC2PCS_RECORD_STARTED			721			// (0x02D1) 录音开始通知
struct S_TCP_REC2PCS_RECORD_STARTED
{
	UL		AgentID;						// 座席ID
	char	RecordTime[15];					// 录音开始时间，PK
	char	UserName[20];					// 用户名，PK
	UC		RecordStartType;				// 录音启动控制方式，参见“录音控制方式”
	char	FilePath[128];					// 录音文件全路径
};
//------------

//------------
#define TCP_REC2PCS_RECORD_STOPED			722			// (0x02D2) 录音停止通知
struct S_TCP_REC2PCS_RECORD_STOPED
{
	UL		AgentID;						// 座席ID
	char	RecordTime[15];					// 录音开始时间，PK
	char	UserName[20];					// 用户名，PK
	UC		RecordEndType;					// 录音停止控制方式，参见“录音控制方式”
	UL		RecordDuration;					// 录音时长
	char	FilePath[128];					// 录音文件全路径
};
//------------

//------------
#define TCP_PCS2REC_REC_USERDATA			1721			// (0x06B9) 追加用户数据到最近一次成功录音
struct S_TCP_PCS2REC_REC_USERDATA
{
	UL		AgentID;						// 座席ID
	char	UserData[200];					// 用户数据
};
//------------

//------------
#define TCP_PCS2REC_REC_RECORDDATA			1722			// (0x0088) 录音附加数据消息
struct S_TCP_PCS2REC_REC_RECORDDATA
{
	UL		AgentID;						// 座席ID
	char	AttachData1[16];				// 附加数据项1
	char	AttachData2[16];				// 附加数据项2
	char	AttachData3[32];				// 附加数据项3
	char	AttachData4[32];				// 附加数据项4
	char	AttachData5[256];				// 附加数据项5
};
//------------
// End of Sun added 2010-05-12
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Sun added 2012-04-18
// 录音启动、停止；录音状态改变通知
//------------
#define TCP_PCS2REC_IVRSTART_REC			1723		// (0x0089) IVR请求开始录音
struct S_TCP_PCS2REC_IVRSTART_REC
{
	UC		SysNo;							// IVR系统代号
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// IVR Channel ID who requests the operation
	UL		AgentID;						// 座席ID	
	UC		RecordStartType;				// 录音启动控制方式，参见“录音控制方式”
	UC		CallDirection;					// 通话方向，参见“呼叫类型”
	UL		CallRefID;						// 呼叫标示
	char	ANI[PCS_DEF_NUMLEN_APP];		// 主叫号码
	char	DNIS[PCS_DEF_NUMLEN_APP];		// 被叫号码
	char	strAliasName[50];				// 录音对应的坐席用户ID
	char	FilePath[PCS_DEF_DATALEN_APP];	// IVR录音文件名
	char	AppData[PCS_DEF_DATALEN_APP];	// App数据
	char	AttachData1[16];				// 附加数据项1
	char	AttachData2[16];				// 附加数据项2
	char	AttachData3[32];				// 附加数据项3
	char	AttachData4[32];				// 附加数据项4
	char	AttachData5[256];				// 附加数据项5
};
//------------

//------------
#define TCP_PCS2REC_IVRSTOP_REC				1724			// (0x008A) IVR请求停止录音
struct S_TCP_PCS2REC_IVRSTOP_REC
{
	UC		SysNo;							// IVR系统代号
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// IVR Channel ID who requests the operation
	UL		AgentID;						// 座席ID	
	UC		RecordStopType;					// 录音启动控制方式，参见“录音控制方式”
	UL		CallRefID;						// 呼叫标示
	char	strAliasName[50];				// 录音对应的坐席用户ID
	char	FilePath[PCS_DEF_DATALEN_APP];	// IVR录音文件名
};
//------------
// End of Sun added 2012-04-18
//------------------------------------------------------------------------------
//--------------------------

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_RECORDSERVER_H__
//------------------------------------------------------------------------
