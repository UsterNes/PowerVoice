////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		_Protocol_PCS.h
// 用途描述：	通信协议
// 作者：		Simon
// 创建时间：	2010/05/04
// 最后修改时间：2010/05/04
// 版权信息：	Copyright (C) 1999 - 2010 PicaSoft
// 备注：
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_PCS_H__
#define __PROTOCOL_PCS_H__

// 事件：系统状态报告
#define TCP_PCS2PCS_SYSTEMSTATUSREPORT_EVT		11004		
struct S_TCP_PCS2PCS_SYSTEMSTATUSREPORT_EVT
{
	UC		PCSID;								// Base 0
	UC		PCSStatus;							// 参考：PCS_PCSStatus
	UL		PCSStatusDuration;					// 当前状态持续时间
	UC		CCMStatus;							// CCM状态：0无效，1有效
	UC		RMStatus;							// RM状态：PCS_RMStatus
	US		ValidAGTCount;						// 有效注册座席数
	UC		Reserved[5];						// 补足16字节
};

// 请求：接管系统（活跃PCS发起，要求对方接管，对方必须处在Standby状态）TakeOver，handoff
#define TCP_PCS2PCS_TAKEOVER_REQ				11005
struct S_TCP_PCS2PCS_TAKEOVER_REQ
{
	UC		Reason;								// 理由
};

// 回应：接管系统
#define TCP_PCS2PCS_TAKEOVER_ACK				11006
struct S_TCP_PCS2PCS_TAKEOVER_ACK
{
	UC		ErrorCode;							// 错误代码
};

// 请求：重新启动（当大家都说PCS1有问题时，由PCS2发起要求PCS1重启，此消息可能导致PCS1 TCP_PCS2PCS_TAKEOVER_REQ消息送出）
#define TCP_PCS2PCS_REBOOT_REQ					11007	
struct S_TCP_PCS2PCS_REBOOT_REQ
{
	UC		Reason;								// 理由
};

// 回应：重新启动
#define TCP_PCS2PCS_REBOOT_ACK					11008		
struct S_TCP_PCS2PCS_REBOOT_ACK
{
	UC		ErrorCode;							// 错误代码
};

// ACD相关
// 事件：添加任务
#define TCP_PCS2PCS_ADDTASKSTART_EVT			11009		
struct S_TCP_PCS2PCS_ADDTASKSTART_EVT
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];	// 统一消息任务ID
	S_TCP_PCS2RMR_UMS_ADDTASK_REQ sAddtaskREQ;	// 原添加消息体'S_TCP_PCS2RMR_UMS_ADDTASK_REQ'
	UCHAR	TaskType;							// 0-UMS任务,1-呼叫任务
	S_TCP_PCS2RMR_ROUTE_QUERY sRouteQuery;		// 原添加消息体'S_TCP_PCS2RMR_ROUTE_QUERY'
	// 非活跃PCS收到路由请求,将请求发给活跃PCS处理.活跃PCS记录远程PCS发来的呼叫ID,以决定后续RM发来消息的处理方式
};

// 事件：任务已经添加
#define TCP_PCS2PCS_ADDEDTASKCOMPLETION_EVT		11010
struct S_TCP_PCS2PCS_ADDEDTASKCOMPLETION_EVT
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
};
// 事件：任务已经移除
#define TCP_PCS2PCS_REMOVETASK_EVT				11013
struct S_TCP_PCS2PCS_REMOVETASK_EVT
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// 统一消息任务ID
};

// 请求：RM选择改变（仅由活跃PCS发起）
#define TCP_PCS2PCS_RM_SELECTCHANGE_REQ			11011
struct S_TCP_PCS2PCS_RM_SELECTCHANGE_REQ
{
	UC		Reason;								// 理由
	UC		ActiveID;							// 活跃RM所属PCSID
};

// 回应：RM选择改变
#define TCP_PCS2PCS_RM_SELECTCHANGE_ACK			11012		
struct S_TCP_PCS2PCS_RM_SELECTCHANGE_ACK
{
	UC		ErrorCode;							// 错误代码
	UC		ActiveID;							// 活跃RM所属PCSID
};
// Simon added 2012-02-29 
// 事件：坐席连接状态改变
#define TCP_PCS2PCS_AGENTCONNECTIONSTATUS_EVT				11014
struct S_TCP_PCS2PCS_AGENTCONNECTIONSTATUS_EVT
{
	UL		AgentID;
	UC		Online;								// 坐席连接状态 0连接断开,1连接正常
};

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_PCS_H__
//------------------------------------------------------------------------