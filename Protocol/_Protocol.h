////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		_Protocol.h
// 用途描述：	通信协议
// 作者：		Tony Sun
// 创建时间：	1999/12/01
// 最后修改时间：2005/12/10
// 版权信息：	Copyright (C) 1999 - 2005 PicaSoft
// 备注：
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// 自定义数据类型
//------------------------------------------------------------------------
#ifndef UDL
	#define UDL unsigned __int64
#endif
#ifndef UL
	#define UL unsigned long
#endif
#ifndef US
	#define US unsigned short int
#endif
#ifndef UC
	#define UC unsigned char
#endif
//------------------------------------------------------------------------
//------------------------------------------------------------------------

/*************************************************************************/
/*************************************************************************/

//------------------------------------------------------------------------
// 通信协议定义
//------------------------------------------------------------------------

#define PCS_MAX_MESSAGE_SIZE	1024		// 消息最大长度
#define PCS_MAX_DATAFLOW_SIZE	1024		// 数据流消息最大长度

//------------------------------------------------------------------------
// 消息头
//------------------------------------------------------------------------
//
struct SMsgHeader
{
	UC Flag0;					// 标志0
	UC Flag1;					// 标志1
	UC Flag2;					// 标志3
	UC Flag3;					// 标志4
	UC Sender;					// 发送者
	UC Receiver;				// 接收者
	US Command;		    		// 命令字
	US PackageLen;				// 包长
	UC TargetSysType;			// 目标系统类型
	UC Reserved1;				// 保留，Agent协议中用作ReqIndex（1-255）
	UL TargetSysID;				// 目标系统ID
};

#define PCS_MSGHDR_FLAG0		0x55		// 消息头标志0
#define PCS_MSGHDR_FLAG1		0xAA		// 消息头标志1
#define PCS_MSGHDR_FLAG2		0x7F		// 消息头标志2
#define PCS_MSGHDR_FLAG3		0xFE		// 消息头标志3

//------------------------------------------------------------------------
// 用户类型指消息包头中的发送者&接收者
//------------------------------------------------------------------------
//
#define	USER_NULL				0			// (0x00) 无效

//---------------------------- Core Level
#define USER_CTI 				1			// (0x01) CTI SERVER
#define	USER_PBX				5			// (0x05) PBX
#define	USER_CMS				9			// (0x09) CMS

//---------------------------- Stand Alone Syetem
#define	USER_IVR				20			// (0x14) IVR子系统
#define	USER_FAX				21			// (0x15) FAX子系统
#define	USER_DIALER				22			// (0x16) 拨号子系统
#define	USER_RECORDER			23			// (0x17) 录音子系统
#define	USER_UMS				32			// (0x20) 统一消息接入模块

//---------------------------- Service Level
#define	USER_ACD				40			// (0x28) ACD子系统
#define	USER_OBS				41			// (0x29) 外拨子系统

//---------------------------- Assitance System
#define USER_DATA 				80			// (0x50) Data Recorder
#define USER_MSG 				81			// (0x51) Messager SERVER
#define USER_PBXCFG				86			// (0x56) PBX Config
#define USER_RECAPT				91			// (0x5B) 录音接入点

//---------------------------- Admin Tools
#define	USER_ADMIN				100			// (0x64) 系统管理控制台
#define	USER_PROGRAM			101			// (0x65) 流程编辑子系统
#define	USER_RESOURCE			102			// (0x66) 资源管理子系统
#define	USER_REPORT				103			// (0x67) 报表子系统
#define	USER_OBADMIN			104			// (0x68) 外拨管理员

//---------------------------- Supervisor
#define	USER_MON_CTI			120			// (0x78) 呼叫中心监控
#define	USER_MON_REC			121			// (0x79) 录音监控
#define	USER_MON_OBS			122			// (0x7A) 外拨监控

//---------------------------- Agent
#define USER_TST_CTI 			140			// (0x8C) CTI Test
#define USER_AGT_CTI 			141			// (0x8D) 座席
#define USER_AGT_REC 			142			// (0x8E) 录音客户端
#define USER_AGT_MSG 			143			// (0x8F) 消息客户端
#define USER_AGT_MSI 			144			// (0x90) 座席卡客户端

//---------------------------- External System
#define USER_EXT_IVR 			201			// (0xC9) IVR外部接口模块
#define USER_EXT_EMAIL 			202			// (0xCA) 电子邮件模块
#define USER_EXT_SMS 			203			// (0xCB) 短信模块
#define USER_EXT_FAX 			204			// (0xCC) FAX接入模块

#define	USER_DUMMY				255			// (0xFF) Dummy System

//------------------------------------------------------------------------
// 消息定义
//------------------------------------------------------------------------
//
//------------------------------------------
// 共同消息
//------------------------------------------
#define TCP_PCS2ALL_TEXTMSG					0			// (0x0000) 文字信息

// Keep Live Message
#define TCP_KEEPLIVE_ACK					3			// (0x0003) Keep Live Acknowledge
#define TCP_KEEPLIVE_MSG					1003		// (0x03EB) Keep Live

// 消息包裹，主要用于同步数据时的大量琐碎消息，Simon added 2010-05-04
#define TCP_MESSAGEPACKED					1004		// (0x3EC) 消息包裹
// 变长消息，包含许多完整消息，接收后展开逐条处理

// CTI Status Report Message
#define TCP_PCS2ALL_CTILinkStart			10001		// (0x2711) CTI Link Connection is OK
#define TCP_PCS2ALL_CTILinkStop				10002		// (0x2712) CTI Link Connection is Down
struct S_TCP_PCS2ALL_CTILinkShift
{
	char strDeviceID[PCS_DEF_NUMLEN_APP];				// 设备ID, PBX表示交换机连接
};

// ACD Status Report Message
#define TCP_PCS2ALL_ACD_STATUS_REPORT		10011		// (0x271B) 路由点状态数据报告消息
struct S_TCP_PCS2ALL_ACD_STATUS_REPORT
{
	// Sun added 2004-12-30
	UC		nType;					// 报告对象（Center / RoutePoint / Queue / Group / Team），
									/// 参见_pcC_CallDistributeObject定义
	US		nGroupID;				// 对象ID
	
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
	// Sun added 2004-09-14
	/// Reason Data
	US		nReason0;				// 选择事由代码0的人数
	US		nReason1;				// 选择事由代码1的人数
	US		nReason2;				// 选择事由代码2的人数
	US		nReason3;				// 选择事由代码3的人数
	US		nReason4;				// 选择事由代码4的人数
	US		nReason5;				// 选择事由代码5的人数
	US		nReason6;				// 选择事由代码6的人数
	US		nReason7;				// 选择事由代码7的人数
	US		nReason8;				// 选择事由代码8的人数
	US		nReason9;				// 选择事由代码9的人数
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	// Sun added 2007-04-15
	US		nEstimatedWaitTime;		// 预计等待时间
	
	// 2010-4-12 Defined nReserved0 - nReserved2
	US		nATT;					// IB平均通话时长
	US		nACW;					// IB平均后处理时长
	US		nASA;					// IB平均应答速度

	// 2010-5-13 Defined nReserved3 - nReserved7
	US		nOBATT;					// OB平均通话时长
	US		nOBACW;					// OB平均后处理时长
	US		nOBAveWaitTime;			// OB平均等待时长
	UL		nOBCallTotal;			// OB呼叫总数(周期累计)
	UL		nOBConnect;				// OB接通次数(周期累计)

	UL		nReserved8;
	UL		nReserved9;
	//----------------------------------------------------------------
};

// 子系统状态报告消息
#define TCP_PCS2ALL_SUBSYS_STATUS_REPORT	1008		// (0x03F0) 子系统状态报告消息
struct S_TCP_PCS2ALL_SUBSYS_STATUS_REPORT
{
	UC		SubSysID;						// 子系统ID，参见“接收者、发送者定义”
	UC		SubSysStatus;					// 子系统状态，PCS参见“PCS状态定义”，其他系统参见“开关常量定义”
	UC		SubSysVersion;					// 子系统版本号
	UC		SubSysIndex;					// 子系统编号（多子系统）
	UC		SubSysType;						// 子系统类型
	UC		Reserved[3];					// 保留
};


// Sun added 2005-12-11
//--------------------------
// 服务器时钟查询 - 2
//------------
#define TCP_ALL2PCS_GET_SERVERTIME			9			// (0x0009) 查询服务器时钟消息
//------------

//------------
#define TCP_PCS2ALL_GET_SERVERTIME_ACK		1009		// (0x03F1) 查询服务器时钟响应消息
struct S_TCP_PCS2ALL_GET_SERVERTIME_ACK
{
	UL		lngTimeZoneBias;		// 时区偏差(分钟)
	char	strDate[12];			// 日期，"yyyy-mm-dd"
	char	strTime[10];			// 时间，"HH:MM:SS"
};

// Sun added 2005-12-10
//--------------------------
// 统计数据查询 - 2
//------------
#define TCP_ALL2PCS_QUERY_WORKDATA			10			// (0x000A) 统计数据查询消息
struct S_TCP_ALL2PCS_QUERY_WORKDATA
{
	char strAliasName[50];								// 登录别名
};
//------------

//------------
#define TCP_PCS2ALL_QUERY_WORKDATA_ACK		1010		// (0x03F2) 统计数据查询响应消息
// Statistics data for user
struct S_TCP_PCS2ALL_USER_ST_REPORT
{
	char strAliasName[50];								// 登录别名

	// Call Data
	US lngIBCalls;					// IB电话数
	US lngOBCalls;					// OB电话数
	US lngIBSucc;					// IB成功电话数
	US lngOBSucc;					// OB成功电话数
	UL lngIBTalkLength;				// IB通话时长
	UL lngOBTalkLength;				// OB通话时长
	UL lngIBWaitLength;				// IB等待时长
	UL lngOBWaitLength;				// OB等待时长

	// Status Data
	US nLoginTimes;					// 登录次数
	UL nLoginLength;				// 登录时长
	US nReadyTimes;					// 就绪次数
	UL nReadyLength;				// 就绪时长
	UL nAvailableLength;			// 可用时长
	US nIBACWTimes;					// IB后处理次数
	UL nIBACWLength;				// IB后处理时长
	US nOBACWTimes;					// OB后处理次数
	UL nOBACWLength;				// OB后处理时长
	US nIBHoldTimes;				// IB保持次数
	UL nIBHoldLength;				// IB保持时长
	US nOBHoldTimes;				// OB保持次数
	UL nOBHoldLength;				// OB保持时长
	US nReasonTimes0;				// 事由0次数
	UL nReasonLength0;				// 事由0时长
	US nReasonTimes1;				// 事由1次数
	UL nReasonLength1;				// 事由1时长
	US nReasonTimes2;				// 事由2次数
	UL nReasonLength2;				// 事由2时长
	US nReasonTimes3;				// 事由3次数
	UL nReasonLength3;				// 事由3时长
	US nReasonTimes4;				// 事由4次数
	UL nReasonLength4;				// 事由4时长
	US nReasonTimes5;				// 事由5次数
	UL nReasonLength5;				// 事由5时长
	US nReasonTimes6;				// 事由6次数
	UL nReasonLength6;				// 事由6时长
	US nReasonTimes7;				// 事由7次数
	UL nReasonLength7;				// 事由7时长
	US nReasonTimes8;				// 事由8次数
	UL nReasonLength8;				// 事由8时长
	US nReasonTimes9;				// 事由9次数
	UL nReasonLength9;				// 事由9时长
};
//------------

#define TCP_ALL2DBR_EXECUTE_SQL				11000		// (0x2AF8) 执行SQL语句


// PCS选择改变放在公共协议里面，不需要为每个协议添加此消息
#define TCP_ALL2PCS_PCSSELECTCHANGED_EVT	7			// (0x0007) PCS选择改变
struct S_TCP_ALL2PCS_PCSSELECTCHANGED_EVT
{
	UC		PCS0Status;						// 参考：PCS_PCSStatus
	UC		PCS1Status;						// 参考：PCS_PCSStatus
	UC		ActivePCSID;					// 活跃PCSID（-1表示没有活跃PCS）
};


//------------------------------------------
// 1、录音服务通信协议
//------------------------------------------
#if PCS_APP_IDENTIFICATION & (0x0001 << 1 )
#include "_Protocol_RECSVR.h"
#endif

//------------------------------------------
// 2、座席消息
//------------------------------------------
#if PCS_APP_IDENTIFICATION & (0x0001 << 2 )
#include "_Protocol_Agent.h"
#endif

//------------------------------------------
// 3、呼叫中心监控消息
//------------------------------------------
#if PCS_APP_IDENTIFICATION & (0x0001 << 3 )
#include "_Protocol_Monitor.h"
#endif

//------------------------------------------
// 4、路由管理消息
//------------------------------------------
#if PCS_APP_IDENTIFICATION & (0x0001 << 4 )
#include "_Protocol_RMR.h"
#endif

//------------------------------------------
// 5、外拨服务消息
//------------------------------------------
#if PCS_APP_IDENTIFICATION & (0x0001 << 5 )
#include "_Protocol_OBS.h"
#endif

//------------------------------------------
// 6、管理控制台消息
//------------------------------------------
#if PCS_APP_IDENTIFICATION & (0x0001 << 6 )
#include "_Protocol_Admin.h"
#endif

//------------------------------------------
// 7、CTI测试客户端通信协议
//------------------------------------------
#if PCS_APP_IDENTIFICATION & (0x0001 << 7 )
#include "_Protocol_CTITST.h"
#endif

//------------------------------------------
// 8、IVR通信协议
//------------------------------------------
#if PCS_APP_IDENTIFICATION & (0x0001 << 8 )
#include "_Protocol_IVRS.h"
#endif

//------------------------------------------
// 9、消息服务通信协议
//------------------------------------------
#if PCS_APP_IDENTIFICATION & (0x0001 << 9 )
#include "_Protocol_MSGR.h"
#endif

//------------------------------------------
// 10、UMS通信协议
//------------------------------------------
#if PCS_APP_IDENTIFICATION & (0x0001 << 10 )
#include "_Protocol_UMS.h"
#endif

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_H__
//------------------------------------------------------------------------
