////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		_Protocol_RMR.h
// 用途描述：	路由管理通信协议
// 作者：		SUN
// 创建时间：	1999/12/01
// 最后修改时间：2012/06/01
// 版权信息：	Copyright (C) 1999 - 2012 PicaSoft, PCS Group
// 备注：
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_ROUTEMANAGER_H__
#define __PROTOCOL_ROUTEMANAGER_H__
//------------------------------------------------------------------------

//--------------------------
// 路由管理器注册 - Total 4
//--------------------------

//--------------------------
// 注册 - 2
//------------
#define TCP_RMR2PCS_REGISTER				1			// (0x0001) 注册消息
struct S_TCP_RMR2PCS_REGISTER
{
	UC		Version;						// 版本号
	UC		RegCode[PCS_DEF_NUMLEN_APP];	// 注册码
};
//------------

//------------
#define TCP_PCS2RMR_REGISTER_ACK			1001		// (0x03E9) 注册响应消息
struct S_TCP_PCS2RMR_REGISTER_ACK
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
#define TCP_RMR2PCS_UNREGISTER				2			// (0x0002) 取消注册消息
//------------

//------------
#define TCP_PCS2RMR_UNREGISTER_ACK			1002		// (0x03EA) 取消注册响应消息
//------------
//--------------------------


//--------------------------
// 路由管理器对象操作 - Total 6
//--------------------------

//--------------------------
// 添加路由点对象 - 2
//------------
#define TCP_RMR2PCS_ADDROUTEPOINT			301			// (0x012D) 添加路由点对象消息
struct S_TCP_RMR2PCS_ADDROUTEPOINT
{
	US		RoutePointID;					// 路由点组ID
	char	strDeviceID[20];				// 路由号码
	UC		RouteRespondMethod;				// 路由等待处理方式，参照_pcC_RouteRespondMethod
	char	strRouteRespondDestination[20];	// 等待路由目的
	US		MaxConCalls;					// 最大并发呼叫数
};
//------------

//------------
#define TCP_PCS2RMR_ADDROUTEPOINT_ACK		1301		// (0x0515) 添加路由点对象响应消息
struct S_TCP_PCS2RMR_ADDROUTEPOINT_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	US		RoutePointID;					// 路由点组ID
};
//------------
//--------------------------

//--------------------------
// 减少路由点对象 - 2
//------------
#define TCP_RMR2PCS_REMOVEROUTEPOINT		302			// (0x012E) 减少路由点对象消息
struct S_TCP_RMR2PCS_REMOVEROUTEPOINT
{
	US		RoutePointID;					// 路由点组ID
};
//------------

//------------
#define TCP_PCS2RMR_REMOVEROUTEPOINT_ACK	1302		// (0x0516) 减少路由点对象响应消息
struct S_TCP_PCS2RMR_REMOVEROUTEPOINT_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	US		RoutePointID;					// 路由点组ID
};
//------------
//--------------------------

//--------------------------
// 状态查询 - 7
//------------
#define TCP_RMR2PCS_GET_RPTSTATUS_ACK		305			// (0x0131) 查询路由点状态响应消息
struct S_TCP_RMR2PCS_GET_RPTSTATUS_ACK
{
	UC		SysNo;							// IVR系统代号
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// Channel ID who requests the data
	UC		nType;							// 报告对象（Center / RoutePoint / Queue / Group / Team），
											/// 参见_pcC_CallDistributeObject定义
	US		nGroupID;						// 对象ID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
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
#define TCP_RMR2PCS_STATUS_QUERY			306			// (0x0132) 查询状态消息
struct S_TCP_RMR2PCS_STATUS_QUERY
{
	UL		AgentID;						// 座席ID
};
//------------

//------------
#define TCP_RMR2PCS_ACD_STATUS_REPORT		307			// (0x0133) 路由点状态数据报告消息
/// 消息结构同S_TCP_PCS2ALL_ACD_STATUS_REPORT
//------------

//------------
#define TCP_RMR2PCS_QUERY_CQMEMBER_ACK		308			// (0x0134) 查询队列成员响应消息，也适用于UMS队列成员查询
struct S_TCP_RMR2PCS_QUERY_CQMEMBER_ACK
{
	UC		UserType;						// 参见“用户类型指消息包头中的发送者&接收者”
	US		SysNo;							// 用户子系统ID
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// Channel ID who requests the data
	UL		AgentID;						// CTI AgentID
	char	strUserID[20];					// 话务员ID
	UL		CallRefID;						// 呼叫标示，如果为0则系统自动选择
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
	US		nRoutePointID;					// 路由点ID，如果为0则系统自动选择
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

//------------
#define TCP_PCS2RMR_GET_RPTSTATUS			1305		// (0x0519) 查询路由点状态消息
struct S_TCP_PCS2RMR_GET_RPTSTATUS
{
	UC		SysNo;							// IVR系统代号
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// Channel ID who requests the data
	UC		nType;							// 报告对象（Center / RoutePoint / Queue / Group / Team），
											/// 参见_pcC_CallDistributeObject定义
	US		nGroupID;						// 对象ID
};
//------------

//------------
#define TCP_PCS2RMR_STATUS_QUERY_ACK		1306		// (0x051A) 查询状态响应消息
struct S_TCP_PCS2RMR_STATUS_QUERY_ACK
{
	UL		AgentID;						// 座席ID
	UC		DNStatus;						// 话机状态，参照_pcC_stDN
	UL		DNStDur;						// 话机状态保持时长（秒）
	UC		PositionStatus;					// 位置状态，参照_pcC_stPOS
	UL		PosStDur;						// 位置状态保持时长（秒）
	UC		LeaveReason;					// 离席原因，自定义
	char	strAliasName[50];				// 登录别名
};
//------------

//------------
#define TCP_PCS2RMR_QUERY_CQMember			1308		// (0x051C) 查询队列成员消息，也适用于UMS队列成员查询
struct S_TCP_PCS2RMR_QUERY_CQMember
{
	UC		UserType;						// 参见“用户类型指消息包头中的发送者&接收者”
	US		SysNo;							// 用户子系统ID
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// Channel ID who requests the data
	UL		AgentID;						// CTI AgentID
	char	strUserID[20];					// 话务员ID
	UL		CallRefID;						// 呼叫标示，如果为0则系统自动选择
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
};
//------------
//--------------------------


//--------------------------
// 路由控制 - Total 5
//--------------------------
#define TCP_RMR2PCS_ROUTE_QUERY_ACK			311			// (0x0137) 路由请求响应消息
struct S_TCP_RMR2PCS_ROUTE_QUERY_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	US		RoutePointID;					// 路由点组ID
	UL		CallRefID;						// 呼叫标示
	char	InRMTime[15];					// 呼叫首次进入RMR的时间
};
//------------

//------------
#define TCP_RMR2PCS_ROUTE_RESPOND			312			// (0x0138) 路由响应消息
struct S_TCP_RMR2PCS_ROUTE_RESPOND
{
	US		RoutePointID;					// 路由点组ID
	UL		CallRefID;						// 呼叫标示
	US		WaitTime;						// 排队等待时长（秒）
	US		GroupID;						// 座席所在组
	UL		AgentID;						// 电话路由至座席，0表示超时
	UC		TimeoutMethod;					// 超时处理方式，参照_pcC_RouteRespondMethod
	char	strTimeoutDestination[20];		// 超时路由目的
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
	UL		OldCallRefID;					// 旧呼叫标示
};
//------------

//------------
#define TCP_PCS2RMR_ROUTE_QUERY				1311		// (0x051F) 路由请求消息
struct S_TCP_PCS2RMR_ROUTE_QUERY
{
	US		RoutePointID;					// 路由点组ID
	UL		CallRefID;						// 呼叫标示
	char	InRMTime[15];					// 呼叫首次进入RMR的时间
	char	strANI[PCS_DEF_NUMLEN_APP];		// 主叫
	char	strDNIS[PCS_DEF_NUMLEN_APP];	// 被叫
	UL		ANITrunk;						// 主叫中继
	UL		DNISTrunk;						// 被叫中继
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
	UL		OldCallRefID;					// 旧呼叫标示
};
//------------

//------------
#define TCP_PCS2RMR_ROUTE_QUERY_END			1312		// (0x0520) 结束路由请求消息
struct S_TCP_PCS2RMR_ROUTE_QUERY_END
{
	US		RoutePointID;					// 路由点组ID
	UL		CallRefID;						// 呼叫标示
	UC		ResultCode;						// 开关常量定义
};
//------------

//------------
#define TCP_PCS2RMR_ROUTE_RESPOND_ACK		1313		// (0x0521) 重复路由请求消息
struct S_TCP_PCS2RMR_ROUTE_RESPOND_ACK
{
	US		RoutePointID;					// 路由点组ID
	UL		CallRefID;						// 呼叫标示
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------


//--------------------------
// 路由管理外拨 - Total 2
//--------------------------

//--------------------------
// 预览外拨 - 4
//------------
#define TCP_RMR2PCS_PREDICTIVE_DISTRIBUTE_ACK	332				// (0x014C) 预测外拨任务路由响应消息
struct S_TCP_RMR2PCS_PREDICTIVE_DISTRIBUTE_ACK
{
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 任务ID
	UL		AgentID;						// 请求座席ID
	char	strTelNo[PCS_DEF_NUMLEN_APP];	// 外拨电话号码
	char	strType[4];						// 外拨类型（用户自定义）
	char	strData[200];					// 外拨数据（用户自定义）
};
//------------

//------------
#define TCP_PCS2RMR_PREDICTIVE_DISTRIBUTE		1332			// (0x0534) 预测外拨任务路由请求消息
struct S_TCP_PCS2RMR_PREDICTIVE_DISTRIBUTE
{
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 任务ID
	UC		AssignType;						// 分配对象，参照_pcC_CallDistributeObject
	US		TargetID;						// 请求座席组或小组ID
	char	strTelNo[PCS_DEF_NUMLEN_APP];	// 外拨电话号码
	char	strType[4];						// 外拨类型（用户自定义）
	char	strData[200];					// 外拨数据（用户自定义）
};
//------------

//------------
#define TCP_PCS2RMR_PREDICTIVE_AGENT_SEIZED		1333			// (0x0535) 预测外拨座席占用消息
struct S_TCP_PCS2RMR_PREDICTIVE_AGENT_SEIZED
{
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 任务ID
	UL		AgentID;						// 占用座席ID
};
//------------

//------------
#define TCP_PCS2RMR_PREDICTIVE_AGENT_UNSEIZED	1334			// (0x0535) 预测外拨座席取消占用消息
struct S_TCP_PCS2RMR_PREDICTIVE_AGENT_UNSEIZED
{
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 任务ID
	UL		AgentID;						// 占用座席ID
};
//------------
//--------------------------


//--------------------------
// 预测外拨 - 4
//------------
#define TCP_RMR2PCS_OBTASK_DISTRIBUTE_ACK	331				// (0x014B) 外拨任务路由响应消息
struct S_TCP_RMR2PCS_OBTASK_DISTRIBUTE_ACK
{
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 任务ID
	UL		AgentID;						// 请求座席ID
	char	strTelNo[PCS_DEF_NUMLEN_APP];	// 外拨电话号码
	char	strType[4];						// 外拨类型（用户自定义）
	char	strData[200];					// 外拨数据（用户自定义）
};
//------------

//------------
#define TCP_PCS2RMR_OBTASK_DISTRIBUTE		1331			// (0x0533) 外拨任务路由请求消息
struct S_TCP_PCS2RMR_OBTASK_DISTRIBUTE
{
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 任务ID
	UC		AssignType;						// 分配对象，参照_pcC_CallDistributeObject
	UC		SharedTask;						// 是否共享，参见“开关常量定义”
	US		TargetID;						// 请求座席组或小组ID
	char	strTelNo[PCS_DEF_NUMLEN_APP];	// 外拨电话号码
	char	strType[4];						// 外拨类型（用户自定义）
	char	strData[200];					// 外拨数据（用户自定义）
};
//------------
//--------------------------

//--------------------------
// 远程控制 - 4
//------------
//------------
#define TCP_RMR2PCS_RELOAD_ACD_SCENARIO_ACK	351			// (0x015F) 重新加载排队规则响应消息
struct S_TCP_RMR2PCS_RELOAD_ACD_SCENARIO_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_RMR2PCS_UPDATE_USER_ACK			352			// (0x0160) 修改用户响应消息
struct S_TCP_RMR2PCS_UPDATE_USER_ACK
{
	char	strAliasName[50];				// 登录别名
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2RMR_RELOAD_ACD_SCENARIO		1351		// (0x0547) 重新加载排队规则消息
//------------

//------------
#define TCP_PCS2RMR_UPDATE_USER				1352		// (0x0548) 修改用户消息
struct S_TCP_PCS2RMR_UPDATE_USER
{
	char	strAliasName[50];				// 登录别名
};
//------------
//--------------------------


//--------------------------
// UMS处理 - Total 14
//--------------------------

//--------------------------
// 统一排队请求 - 2
//------------
#define TCP_RMR2PCS_UMS_ADDTASK_ACK			3101		// (0x0C1D) 统一入队响应消息
struct S_TCP_RMR2PCS_UMS_ADDTASK_ACK
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
	UC		ErrorCode;								// 返回码，参见“返回码定义”：
													// PCS_DEF_RETCODE_SUCC
													// PCS_DEF_RETCODE_NOTNEED
													// PCS_DEF_RETCODE_OTHERS
													// PCS_DEF_RETCODE_SYSERROR
	US		RoutePointID;							// 进入路由点ID
	US		nRouteData1;							// 路由数据1：队列长度
	US		nRouteData2;							// 路由数据2：登录坐席数
	US		nRouteData3;							// 路由数据3：空闲坐席数
	US		nRouteData4;							// 路由数据4：预计等待时间
	US		nRouteData5;							// 路由数据5：暂时保留
	US		nRouteData6;							// 路由数据5：暂时保留 
	US		nRouteData7;							// 路由数据6：暂时保留
	US		nRouteData8;							// 路由数据7：暂时保留
	US		nRouteData9;							// 路由数据8：暂时保留
};
//------------

//------------
#define TCP_PCS2RMR_UMS_ADDTASK_REQ			3601		// (0x0E11) 统一排队请求消息
struct S_TCP_PCS2RMR_UMS_ADDTASK_REQ
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
	char	strHotLink[100];						// 相关文件路径或连接地址
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------
//--------------------------

//--------------------------
// 统一离队请求 - 2
//------------
#define TCP_RMR2PCS_UMS_REMOVETASK_ACK		3102		// (0x0C1E) 统一离队响应消息
struct S_TCP_RMR2PCS_UMS_REMOVETASK_ACK
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
	US		RoutePointID;							// 路由点ID
	UC		ErrorCode;								// 返回码，参见“返回码定义”：
													// PCS_DEF_RETCODE_SUCC
													// PCS_DEF_RETCODE_NOTEXIST
													// PCS_DEF_RETCODE_STATUS
													// PCS_DEF_RETCODE_NOTNEED
													// PCS_DEF_RETCODE_OTHERS
													// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2RMR_UMS_REMOVETASK_REQ		3602		// (0x0E12) 统一离队消息
struct S_TCP_PCS2RMR_UMS_REMOVETASK_REQ
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
	US		RoutePointID;							// 路由点ID
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
//--------------------------

//--------------------------
// 统一任务分配 - 2
//------------
#define TCP_RMR2PCS_UMS_DISPATCHTASK_REQ	3106		// (0x0C22) 统一分配请求消息
struct S_TCP_RMR2PCS_UMS_DISPATCHTASK_REQ
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
#define TCP_PCS2RMR_UMS_DISPATCHTASK_ACK	3606		// (0x0E16) 统一分配请求响应消息
struct S_TCP_PCS2RMR_UMS_DISPATCHTASK_ACK
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
	US		RoutePointID;							// 路由点ID
	UC		ErrorCode;								// 返回码，参见“返回码定义”：
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
//--------------------------

//--------------------------
// 统一任务处理 - 2
//------------
#define TCP_RMR2PCS_UMS_ACCEPTTASK_ACK		3107		// (0x0C23) 统一分配处理响应消息
struct S_TCP_RMR2PCS_UMS_ACCEPTTASK_ACK
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
	US		RoutePointID;							// 路由点ID
	char	strUserID[20];							// 话务员ID
	UC		ErrorCode;								// 返回码，参见“开关常量定义”
};
//------------

//------------
#define TCP_PCS2RMR_UMS_ACCEPTTASK_REQ		3607		// (0x0E17) 统一分配处理消息
struct S_TCP_PCS2RMR_UMS_ACCEPTASK_REQ
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
	US		RoutePointID;							// 路由点ID
	char	strUserID[20];							// 话务员ID
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
//--------------------------

//--------------------------
// 统一任务取消 - 2
//------------
#define TCP_RMR2PCS_UMS_CANCELTASK_ACK		3108		// (0x0C24) 统一处理取消响应消息
struct S_TCP_RMR2PCS_UMS_CANCELTASK_ACK
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
	US		RoutePointID;							// 路由点ID
	char	strUserID[20];							// 话务员ID
};
//------------

//------------
#define TCP_PCS2RMR_UMS_CANCELTASK_REQ		3608		// (0x0E18) 统一处理取消消息
struct S_TCP_PCS2RMR_UMS_CANCELTASK_REQ
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
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------
//--------------------------

//--------------------------
// 统一任务完成 - 2
//------------
#define TCP_RMR2PCS_UMS_FINISHTASK_ACK		3109		// (0x0C25) 统一处理完成响应消息
struct S_TCP_RMR2PCS_UMS_FINISHTASK_ACK
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
	US		RoutePointID;							// 路由点ID
	char	strUserID[20];							// 话务员ID
};
//------------

//------------
#define TCP_PCS2RMR_UMS_FINISHTASK_REQ		3609		// (0x0E19) 统一处理完成消息
struct S_TCP_PCS2RMR_UMS_FINISHTASK_REQ
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
	US		RoutePointID;							// 路由点ID
	char	strUserID[20];							// 话务员
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
//--------------------------

//--------------------------
// 批量任务取消 - 2
//------------
#define TCP_RMR2PCS_UMS_AGTTASK_CANCEL_ACK	3110		// (0x0C26) 坐席全部处理取消响应消息
struct S_TCP_RMR2PCS_UMS_AGTTASK_CANCEL_ACK
{
	char	strUserID[20];							// 话务员ID
	UL		TaskCount;								// 任务数量
};
//------------

//------------
#define TCP_PCS2RMR_UMS_AGTTASK_CANCEL_REQ	3610		// (0x0E1A) 坐席全部处理取消消息
struct S_TCP_PCS2RMR_UMS_AGTTASK_CANCEL_REQ
{
	char	strUserID[20];							// 话务员ID
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
//--------------------------

//--------------------------
//------------
// Simon added 2010-05-04
#define TCP_PCS2RMR_UPDATE_REALTIMEPARAM_REQ 3621		// (0x0E25) 实时数据参数更新 请求
struct S_TCP_PCS2RMR_UPDATE_REALTIMEPARAM_REQ
{
	UC		bReportRealtimeData;					// 是否报告实时数据
	UC		bRecordRTD;								// 是否记录实时数据
	UC		Reserved[16];							// 保留
};

#define TCP_RMR2PCS_UPDATE_REALTIMEPARAM_ACK 3121		// (0x0C31) 实时数据参数更新 回应
struct S_TCP_RMR2PCS_UPDATE_REALTIMEPARAM_ACK
{
	UC		ErrorCode;								// 错误代码
};

#define TCP_RMR2PCS_INSERVICE_EVT			3122		// (0x0C32) RM 进入服务状态
#define TCP_RMR2RMR_OUTOFSERVICE_EVT		3123		// (0x0C33) RM 离开服务状态(暂不使用)
//------------
//--------------------------

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_ROUTEMANAGER_H__
//------------------------------------------------------------------------
