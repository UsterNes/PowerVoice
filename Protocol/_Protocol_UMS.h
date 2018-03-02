////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		_Protocol_UMS.h
// 用途描述：	统一消息通信协议
// 作者：		SUN
// 创建时间：	2006/05/10
// 最后修改时间：2006/05/12
// 版权信息：	Copyright (C) 2005 - 2006 PicaSoft
// 备注：
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_UNIFYMESSAGESYSTEM_H__
#define __PROTOCOL_UNIFYMESSAGESYSTEM_H__
//------------------------------------------------------------------------

//--------------------------
// UMS接入模块注册 - Total 4
//--------------------------

//--------------------------
// 注册 - 1
//------------
#define TCP_UMS2PCS_REGISTER				1			// (0x0001) 注册消息
struct S_TCP_UMS2PCS_REGISTER
{
	UC		UMSType;						// UMS接入类型，参见_pcC_UMSTaskTypes
	UC		UMSIndex;						// UMS子系统编号
	UC		Version;						// 版本号
	UL		MaxConTask;						// 最大并发任务数
	UC		RegCode[PCS_DEF_NUMLEN_APP];	// 注册码
};
//------------

//------------
#define TCP_PCS2UMS_REGISTER_ACK			1001		// (0x03E9) 注册响应消息
struct S_TCP_PCS2UMS_REGISTER_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
												// PCS_DEF_RETCODE_SUCC
												// PCS_DEF_RETCODE_NOTEXIST
												// PCS_DEF_RETCODE_BEUSED
												// PCS_DEF_RETCODE_VERSION
												// PCS_DEF_RETCODE_REGCODE
												// PCS_DEF_RETCODE_OTHERS
												// PCS_DEF_RETCODE_SYSERROR
	UC		SiteID;							// 中心ID
	UC		RMOnlineStatus;					// RouteManager在线状态
};
//------------
//--------------------------

//--------------------------
// 取消注册 - 2
//------------
#define TCP_UMS2PCS_UNREGISTER				2			// (0x0002) 取消注册消息
//------------

//------------
#define TCP_PCS2UMS_UNREGISTER_ACK			1002		// (0x03EA) 取消注册响应消息
//------------
//--------------------------


//--------------------------
// UMS处理 - Total 12
//--------------------------

//--------------------------
// 排队请求 - 2
//------------
#define TCP_UMS2PCS_ADDTASK_REQ				3001		// (0x0BB9) 排队请求消息
struct S_TCP_UMS2PCS_ADDTASK_REQ
{
	UL		InvokeID;									// Invoke ID
	UL		TaskID;										// Task ID
	UC		CustID;										// 租户代码
	char	strCallingParty[PCS_DEF_NUMLEN_APP];		// 发起者
	char	strCalledParty[PCS_DEF_NUMLEN_APP];			// 接收者
	US		RoutePointID;								// 默认路由点ID
	UL		ClassifyCode;								// 分类代码
	UL		WaitTime;									// 已等待时间（秒）
	char	ServiceCode0;								// 主服务代码
	char	ServiceCode1;								// 二级服务代码
	char	ServiceCode2;								// 三级服务代码
	char	strTaskTitle[50];							// 任务标题
	char	strTaskContent[200];						// 任务描述
	char	strHotLink[100];							// 相关文件路径或连接地址
	char	strAppData[PCS_DEF_DATALEN_APP];			// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];			// User Data
};
//------------

//------------
#define TCP_PCS2UMS_ADDTASK_ACK				3501		// (0x0DAD) 入队响应消息
struct S_TCP_PCS2UMS_ADDTASK_ACK
{
	UL		InvokeID;								// Invoke ID
	UL		TaskID;									// Task ID
	UC		ErrorCode;								// 返回码，参见“返回码定义”：
													// PCS_DEF_RETCODE_SUCC
													// PCS_DEF_RETCODE_NOTEXIST
													// PCS_DEF_RETCODE_STATUS
													// PCS_DEF_RETCODE_NOTNEED
													// PCS_DEF_RETCODE_REJECTED
													// PCS_DEF_RETCODE_OTHERS
													// PCS_DEF_RETCODE_SYSERROR
	US		RoutePointID;							// 进入路由点ID
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
	char	strAcceptTime[15];						// 任务生成时间，"YYYYMMDDHHMMSS\0"
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
//--------------------------

//--------------------------
// 离队请求 - 2
//------------
#define TCP_UMS2PCS_REMOVETASK_REQ			3002		// (0x0BBA) 离队消息
struct S_TCP_UMS2PCS_REMOVETASK_REQ
{
	UL		InvokeID;								// Invoke ID
	UL		TaskID;									// Task ID
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

//------------
#define TCP_PCS2UMS_REMOVETASK_ACK			3502		// (0x0DAE) 离队响应消息
struct S_TCP_PCS2UMS_REMOVETASK_ACK
{
	UL		InvokeID;								// Invoke ID
	UL		TaskID;									// Task ID
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

//--------------------------
// 继续等待消息 - 1
//------------
#define TCP_UMS2PCS_CONTINUETASK_REQ		3003		// (0x0BBB) 继续排队消息
struct S_TCP_UMS2PCS_CONTINUETASK_REQ
{
	UL		InvokeID;							// Invoke ID
	UL		TaskID;								// Task ID
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];	// 统一消息任务ID
	US		RoutePointID;						// 路由点ID
	UL		ReasonCode;							// 继续原因代码
};

//--------------------------
// 任务暂停 - 2
//------------
#define TCP_UMS2PCS_PAUSETASK_REQ			3004		// (0x0BBC) 任务暂停消息
struct S_TCP_UMS2PCS_PAUSETASK_REQ
{
	UL		InvokeID;								// Invoke ID
	UL		TaskID;									// Task ID
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
	US		RoutePointID;							// 路由点ID
	UL		ReasonCode;								// 离队原因代码
};
//------------

//------------
#define TCP_PCS2UMS_PAUSETASK_ACK			3504		// (0x0DB0) 任务暂停响应消息
struct S_TCP_PCS2UMS_PAUSETASK_ACK
{
	UL		InvokeID;								// Invoke ID
	UL		TaskID;									// Task ID
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

//--------------------------
// UMS分配请求消息 - 2
//------------
#define TCP_PCS2UMS_DISPATCHTASK_REQ			3510		// (0x0DB6) 分配请求消息
struct S_TCP_PCS2UMS_DISPATCHTASK_REQ
{
	UL		TaskID;								// Task ID
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];	// 统一消息任务ID
	UL		WaitTime;							// 已等待时间（秒）
	US		RoutePointID;						// 路由点ID
	US		GroupID;							// 组ID
	US		TeamID;								// 小组ID
	UC		TeamIndex;							// 小组优先级
	UC		UserIndex;							// 话务员优先级
	char	strUserID[20];						// 话务员ID
	US		LoginID;							// 话务员工号
	UL		AgentID;							// 座席ID
	char	strExtNO[PCS_DEF_NUMLEN_APP];		// 分机号码
	char	strIP[PCS_DEF_NUMLEN_APP];			// 分机IP地址
};
//------------

//------------
#define TCP_UMS2PCS_DISPATCHTASK_REQ_ACK		3010	// (0x0BC2)分配请求响应消息
struct S_TCP_UMS2PCS_DISPATCHTASK_REQ_ACK
{
	UL		TaskID;								// Task ID
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];	// 统一消息任务ID
	US		RoutePointID;						// 路由点ID
	UC		ErrorCode;							// 返回码，参见“返回码定义”：
													// PCS_DEF_RETCODE_SUCC
													// PCS_DEF_RETCODE_NOTEXIST
													// PCS_DEF_RETCODE_STATUS
													// PCS_DEF_RETCODE_NOTNEED
													// PCS_DEF_RETCODE_NORIGHTS
													// PCS_DEF_RETCODE_NOTSUPPORT
													// PCS_DEF_RETCODE_OTHERS
													// PCS_DEF_RETCODE_SYSERROR
};
//--------------------------
// UMS任务状态改变消息 - 3
//------------
#define TCP_PCS2UMS_TASKST_DISPACTH				3511	// (0x0DB7) 分配处理消息
struct S_TCP_PCS2UMS_TASKST_DISPACTH
{
	UL		TaskID;								// Task ID
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];	// 统一消息任务ID
	US		RoutePointID;						// 路由点ID
	char	strUserID[20];						// 话务员
	UC		DispacthResult;						// 参见_pcC_OutBoundCallResult
													// _pcC_OB_RESULT_SUCC = '0',				// '0' - 成功
													// _pcC_OB_RESULT_NO_ANS = '1',				// '1' - 无应答
													// _pcC_OB_RESULT_DEST_BUSY = '2',			// '2' - 对方忙
													// _pcC_OB_RESULT_ABANDON = '3',			// '3' - 放弃
};

//------------

//------------
#define TCP_PCS2UMS_TASKST_CANCEL			3512		// (0x0DB8) 处理取消消息
struct S_TCP_PCS2UMS_TASKST_CANCEL
{
	UL		TaskID;									// Task ID
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
};
//------------

//------------
#define TCP_PCS2UMS_TASKST_FINISH			3513		// (0x0DB9) 处理完成消息
struct S_TCP_PCS2UMS_TASKST_FINISH
{
	UL		TaskID;									// Task ID
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
};
//------------
//--------------------------

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_UNIFYMESSAGESYSTEM_H__
//------------------------------------------------------------------------
