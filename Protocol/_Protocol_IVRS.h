////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		_Protocol_IVRS.h
// 用途描述：	IVR通信协议
// 作者：		SUN
// 创建时间：	1999/12/01
// 最后修改时间：2004/04/05
// 版权信息：	Copyright (C) 1999 - 2004 PCSG
// 备注：
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_IVRS_H__
#define __PROTOCOL_IVRS_H__
//------------------------------------------------------------------------

//--------------------------
// IVR注册 - Total 4
//--------------------------

//--------------------------
// 注册 - 2
//------------
#define TCP_IVR2PCS_REGISTER				1			// (0x0001) 注册消息
struct S_TCP_IVR2PCS_REGISTER
{
	UC		SysNo;							// 系统代号
	UC		GroupCount;						// 组数量
	US		ChannelCount;					// 通道数量
	UC		Version;						// 版本号
	UC		AutoLogout;						// 断开连接时通道是否自动退出登录，参见“开关常量定义”
	UC		RegCode[PCS_DEF_NUMLEN_APP];	// 注册码
};
//------------

//------------
#define TCP_PCS2IVR_REGISTER_ACK			1001		// (0x03E9) 注册响应消息
struct S_TCP_PCS2IVR_REGISTER_ACK
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
};
//------------
//--------------------------

//--------------------------
// 取消注册 - 2
//------------
#define TCP_IVR2PCS_UNREGISTER				2			// (0x0002) 取消注册消息
//------------

//------------
#define TCP_PCS2IVR_UNREGISTER_ACK			1002		// (0x03EA) 取消注册响应消息
//------------
//--------------------------
//--------------------------


//--------------------------
// IVR设备管理 - Total 4
//--------------------------

//--------------------------
// 通道管理 - 4
//------------
#define TCP_IVR2PCS_CH_OPEN					11			// (0x000B) 设备初始化请求，登录CTI消息
struct S_TCP_IVR2PCS_CH_OPEN
{
	UL		AgentID;						// CTI AgentID
};
//------------

//------------
#define TCP_IVR2PCS_CH_CLOSE				12			// (0x000C) 设备关闭请求消息
struct S_TCP_IVR2PCS_CH_CLOSE
{
	UL		AgentID;						// CTI AgentID
};
//------------

//------------
#define TCP_PCS2IVR_CH_OPEN_ACK				1011		// (0x03F3) 设备初始化响应消息
struct S_TCP_PCS2IVR_CH_OPEN_ACK
{
	UL		AgentID;						// CTI AgentID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2IVR_CH_CLOSE_ACK			1012		// (0x03F4) 设备关闭响应消息
struct S_TCP_PCS2IVR_CH_CLOSE_ACK
{
	UL		AgentID;						// CTI AgentID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------


//--------------------------
// IVR 呼叫控制 - Total 10
//--------------------------

//------------
// 电话外拨
//------------
#define TCP_IVR2PCS_CALL_CALLOUT			51			// (0x0033) 电话外拨消息
struct S_TCP_IVR2PCS_CALL_CALLOUT
{
	UL		AgentID;								// CTI AgentID
	UC		CallScale;								// 呼叫范围，参照_pcC_CallScale
	char	strCalledParty[PCS_DEF_NUMLEN_APP];		// 外拨号码
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------

//------------
// 电话挂断
//------------
#define TCP_IVR2PCS_CALL_CLEARCALL			52			// (0x0034) 电话挂断请求消息
struct S_TCP_IVR2PCS_CALL_CLEARCALL
{
	UL		AgentID;								// CTI AgentID
	UL		CallRefID;								// 呼叫标示，如果为0则系统自动选择
	UC		EventSource;							// 事件来源，参照_pcC_EventSource，如果为0则系统自动选择
};
//------------

//------------
// 电话应答
//------------
#define TCP_IVR2PCS_CALL_ANSWERCALL			53			// (0x0035) 电话应答请求消息
struct S_TCP_IVR2PCS_CALL_ANSWERCALL
{
	UL		AgentID;								// CTI AgentID
	UL		CallRefID;								// 呼叫标示，如果为0则系统自动选择
	UC		EventSource;							// 事件来源，参照_pcC_EventSource，如果为0则系统自动选择
};
//------------

//------------
// 电话保持
//------------
#define TCP_IVR2PCS_CALL_HOLDCALL			54			// (0x0036) 电话保持请求消息
struct S_TCP_IVR2PCS_CALL_HOLDCALL
{
	UL		AgentID;								// CTI AgentID
	UL		CallRefID;								// 呼叫标示，如果为0则系统自动选择
	UC		EventSource;							// 事件来源，参照_pcC_EventSource，如果为0则系统自动选择
};
//------------

//------------
// 电话拾起
//------------
#define TCP_IVR2PCS_CALL_RETRIEVECALL		55			// (0x0037) 电话拾起请求消息
struct S_TCP_IVR2PCS_CALL_RETRIEVECALL
{
	UL		AgentID;								// CTI AgentID
	UL		CallRefID;								// 呼叫标示，如果为0则系统自动选择
	UC		EventSource;							// 事件来源，参照_pcC_EventSource，如果为0则系统自动选择
};
//------------

//------------
// 呼叫磋商
//------------
#define TCP_IVR2PCS_CALL_CONSULTATION		61			// (0x003D) 呼叫磋商消息
struct S_TCP_IVR2PCS_CALL_CONSULTATION
{
	UL		AgentID;								// CTI AgentID
	UL		CallRefID;								// 呼叫标示，如果为0则系统自动选择
	UC		EventSource;							// 事件来源，参照_pcC_EventSource，如果为0则系统自动选择
	UC		ConsultMode;							// 呼叫磋商模式，参照_pcC_ConsultationMode
	UC		CallScale;								// 呼叫范围，参照_pcC_CallScale
	char	strCalledParty[PCS_DEF_NUMLEN_APP];		// 外拨号码
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------

//------------
// 电话转接
//------------
#define TCP_IVR2PCS_CALL_TRANSFER			62			// (0x003E) 电话转接消息
struct S_TCP_IVR2PCS_CALL_TRANSFER
{
	UL		AgentID;								// CTI AgentID
	UL		HoldRefID;								// 保持呼叫标志，如果为0则系统自动选择
	UL		CallRefID;								// 呼叫标示，如果为0则系统自动选择
	UC		EventSource;							// 事件来源，参照_pcC_EventSource，如果为0则系统自动选择
};
//------------

//------------
// 电话会议
//------------
#define TCP_IVR2PCS_CALL_CONFERENCE			63			// (0x003F) 电话会议消息
struct S_TCP_IVR2PCS_CALL_CONFERENCE
{
	UL		AgentID;								// CTI AgentID
	UL		HoldRefID;								// 保持呼叫标志，如果为0则系统自动选择
	UL		CallRefID;								// 呼叫标示，如果为0则系统自动选择
	UC		EventSource;							// 事件来源，参照_pcC_EventSource，如果为0则系统自动选择
};
//------------

//------------
// 单步转接
//------------
#define TCP_IVR2PCS_CALL_SINGLESTEPTRANS	64			// (0x0040) 单步转接消息
struct S_TCP_IVR2PCS_CALL_SINGLESTEPTRANS
{
	UL		AgentID;								// CTI AgentID
	UL		CallRefID;								// 呼叫标示，如果为0则系统自动选择
	UC		EventSource;							// 事件来源，参照_pcC_EventSource，如果为0则系统自动选择
	UC		CallScale;								// 呼叫范围，参照_pcC_CallScale
	char	strCalledParty[PCS_DEF_NUMLEN_APP];		// 外拨号码
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------

//------------
#define TCP_IVR2PCS_SET_MSGWAITING			119			// (0x0077) 设置留言等待指示开关消息
struct S_TCP_IVR2PCS_CALL_MSGWAITING
{
	UL		AgentID;								// CTI AgentID
	char	strDN[20];								// DN号码
	UC		SetMWADOn;								// 是否设定留言等待指示开关，参见“开关常量定义”
};
//------------
//--------------------------

//--------------------------
// ACD 队列状态查询 - Total 8
//--------------------------
//------------
#define TCP_IVR2PCS_GET_ACDSTATUS			20			// (0x0014) 查询ACD队列状态消息
struct S_TCP_IVR2PCS_GET_ACDSTATUS
{
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// IVR Channel ID who requests the data
	char	strACDDN[20];					// ACD DN
};
//------------

//------------
#define TCP_IVR2PCS_GET_RPTSTATUS			21			// (0x0015) 查询路由点状态消息
struct S_TCP_IVR2PCS_GET_RPTSTATUS
{
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// IVR Channel ID who requests the data
	UC		nType;							// 报告对象（Center / RoutePoint / Queue / Group / Team），
											/// 参见_pcC_CallDistributeObject定义
	US		nGroupID;						// 对象ID
};
//------------

//------------
#define TCP_IVR2PCS_GET_AGENTSTATUS			22			// (0x0016) 查询座席状态消息
struct S_TCP_IVR2PCS_GET_AGENTSTATUS
{
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// IVR Channel ID who requests the data
	UC		UserQuery;						// 查询类型，0:Agent; 1:UserID; 2:LoginID;
	UL		AgentID;						// 座席ID 或 登录工号
	char	strAliasName[50];				// 登录别名
};
//------------

// Sun added 2007-04-15
//------------
#define TCP_IVR2PCS_GET_CALL_EWT			23			// (0x0017) 某呼叫在特定路由点的EWT数值
struct S_TCP_IVR2PCS_GET_CALL_EWT
{
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// IVR Channel ID who requests the data
	UL		AgentID;						// CTI AgentID
	UL		CallRefID;						// 呼叫标示，如果为0则系统自动选择
};
//------------

//------------
#define TCP_IVR2PCS_CALL_PREQUEUE_REQ		30			// (0x001E) 呼叫预排队请求
struct S_TCP_IVR2PCS_CALL_PREQUEUE_REQ
{
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// IVR Channel ID who requests the data
	UL		AgentID;						// CTI AgentID
	UL		CallRefID;						// 呼叫标示，如果为0则系统自动选择
	US		nRoutePointID;					// 路由点ID
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------

//------------
#define TCP_PCS2IVR_GET_ACDSTATUS_ACK		1020			// (0x03FC) 查询ACD队列状态响应消息
struct S_TCP_PCS2IVR_GET_ACDSTATUS_ACK
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

//------------
#define TCP_PCS2IVR_GET_RPTSTATUS_ACK		1021			// (0x03FD) 查询路由点状态响应消息
struct S_TCP_PCS2IVR_GET_RPTSTATUS_ACK
{
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// IVR Channel ID who requests the data
	UC		nType;							// 报告对象（Center / RoutePoint / Queue / Group / Team），
											/// 参见_pcC_CallDistributeObject定义
	US		nGroupID;						// 对象ID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS (RM不在线)
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	/// Call Data
	US		nCallsWait;				// 排队等待长度
	US		nCallsAveWaitTime;		// 排队平均等待时长
	US		nCallsMaxWaitTime;		// 排队最长等待时长
	UL		nCallsTotal;			// 队列来电数量(周期累计)
	UL		nCallsTransfer;			// 队列转接数(周期累计)
	UL		nCallsSucc;				// 队列转接成功数(周期累计)
	UL		nCallsSuccInTime;		// 队列及时转接数(周期累计)
	
	/// Agent Data
	US		nAgentSum;				// 座席总数
	US		nAgentIdle;				// 就绪空闲座席数
	US		nAgentBusy;				// 通话座席数
	US		nAgentNotReady;			// 未就绪座席数
	US		nAgentAfterCall;		// 后处理座席数
	US		nAgentLogout;			// 未登录座席数

	//----------------------------------------------------------------
	// Sun added 2007-04-15
	US		nEstimatedWaitTime;				// 预计等待时间
	
	US		nReserved0;
	US		nReserved1;
	US		nReserved2;
	US		nReserved3;
	US		nReserved4;
	US		nReserved5;
	UL		nReserved6;
	UL		nReserved7;
	UL		nReserved8;
	UL		nReserved9;
	//----------------------------------------------------------------
};
//------------

//------------
#define TCP_PCS2IVR_GET_AGENTSTATUS_ACK		1022			// (0x03FE) 查询座席状态响应消息
struct S_TCP_PCS2IVR_GET_AGENTSTATUS_ACK
{
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// IVR Channel ID who requests the data
	UC		UserQuery;						// 是否查询用户，参见“开关常量定义”
	UL		AgentID;						// 座席ID	
	char	strAliasName[50];				// 登录别名
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UC		DNStatus;						// 话机状态，参照_pcC_stDN
	UL		DNStDur;						// 话机状态保持时长（秒）
	UC		PositionStatus;					// 位置状态，参照_pcC_stPOS
	UL		PosStDur;						// 位置状态保持时长（秒）
	UC		LeaveReason;					// 离席原因，自定义	
};
//------------

// Sun added 2007-04-15
//------------
#define TCP_PCS2IVR_GET_CALL_EWT_ACK		1023			// (0x03FF) 查询呼叫在特定路由点EWT响应消息
struct S_TCP_PCS2IVR_GET_CALL_EWT_ACK
{
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// IVR Channel ID who requests the data
	UL		CallRefID;						// 呼叫标示
	US		nRoutePointID;					// 路由点ID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	US		nQueuePosition;					// 排队位置
	US		nEstimatedWaitTime;				// 预计等待时间（秒）
	US		nReserved0;
	US		nReserved1;
};
//------------

//------------
#define TCP_PCS2IVR_CALL_PREQUEUE_REQ		1030			// (0x0406) 呼叫预排队请求响应消息
struct S_TCP_PCS2IVR_CALL_PREQUEUE_REQ
{
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// IVR Channel ID who requests the data
	UL		CallRefID;						// 呼叫标示，如果为0则系统自动选择
	US		nRoutePointID;					// 路由点ID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	US		nQueuePosition;					// 排队位置
	US		nEstimatedWaitTime;				// 预计等待时间（秒）
	US		nReserved0;
	US		nReserved1;
};
//------------
//--------------------------

//------------------------------------------------------------------------------
// Sun added 2012-04-18 [A]
// 录音启动、停止；录音状态改变通知
//------------
#define TCP_IVR2PCS_START_REC				131			// (0x0083) 请求开始录音
struct S_TCP_IVR2PCS_START_REC
{
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
#define TCP_IVR2PCS_STOP_REC				132			// (0x0084) 请求停止录音
struct S_TCP_IVR2PCS_STOP_REC
{
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// IVR Channel ID who requests the operation
	UL		AgentID;						// 座席ID	
	UC		RecordStopType;					// 录音启动控制方式，参见“录音控制方式”
	UL		CallRefID;						// 呼叫标示
	char	strAliasName[50];				// 录音对应的坐席用户ID
	char	FilePath[PCS_DEF_DATALEN_APP];	// IVR录音文件名
};
//------------

//------------
#define TCP_IVR2PCS_REC_USERDATA			135			// (0x0087) 追加用户数据到本通道最近一次成功录音
struct S_TCP_IVR2PCS_REC_USERDATA
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// 座席ID	
	char	UserData[200];					// 用户数据
};
//------------

//------------
#define TCP_IVR2PCS_REC_RECORDDATA			136			// (0x0088) 录音附加数据消息
struct S_TCP_IVR2PCS_REC_RECORDDATA
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// 座席ID	
	char	AttachData1[16];				// 附加数据项1
	char	AttachData2[16];				// 附加数据项2
	char	AttachData3[32];				// 附加数据项3
	char	AttachData4[32];				// 附加数据项4
	char	AttachData5[256];				// 附加数据项5
};
//------------

//------------
// 1135		// (0x046F) 录音开始通知录音开始通知
// 1136		// (0x0470) 录音停止通知
/// 同Agent消息
//------------
// End of Sun added 2012-04-18 [A]
//------------------------------------------------------------------------------

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_IVRS_H__
//------------------------------------------------------------------------
