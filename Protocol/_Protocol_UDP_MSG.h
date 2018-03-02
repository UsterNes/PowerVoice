////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		_Protocol_UDP_MSG.h
// 用途描述：	消息通信UDP协议
// 作者：		SUN
// 创建时间：	2002/06/05
// 最后修改时间：2004/02/28
// 版权信息：	Copyright (C) 1998 - 2004 PicaSoft
// 备注：
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif
//------------------------------------------------------------------------
#ifndef __PROTOCOL_UDP_MSG_H__
#define __PROTOCOL_UDP_MSG_H__
//------------------------------------------------------------------------

//--------------------------
// 常量定义 - 2
//------------
#ifndef MAXHEADLEN
	#define MAXHEADLEN			50			// 最大的信息显示长度(<MAX_PACKET_LEN - 16)
#endif

#ifndef MAXMSGLEN
	#define MAXMSGLEN			200			// 最大的信息显示长度(<MAX_PACKET_LEN - 16)
#endif

//------------------------------------------
// 消息类型 - 4
//-----------------------------------------
#define	PKGTYP_AllNOTE			10			// 全体
#define	PKGTYP_AGENTNOTE		1			// 座席
#define	PKGTYP_TEAMNOTE			2			// 小组
#define	PKGTYP_GROUPNOTE		3			// 组
//------------
//--------------------------

//--------------------------
// 结构体 - 2
//------------
struct SMemberListItem
{
	UL		AgentID;				// Agent ID
	char	UserID[20];				// User ID
	UC		AgentStatus;			// 座席状态
};

struct SMemberTreeItem
{
	US		GroupID;				// Group ID
	char	GroupName[50];			// Group Name
	US		TeamID;					// Team	ID
	char	TeamName[50];			// Team Name
	char	UserID[20];				// User ID
	char	UserName[50];			// User Name
};
//------------
//--------------------------

//------------------------------------------------------------------------
// 通信协议定义 - 2
//------------------------------------------------------------------------
// 消息头
struct SUDPMsgHeader
{
	UC Sender;					// 发送者
	UC Receiver;				// 接收者
	US PackageLen;				// 包长
	US Command;		    		// 命令字
};

//------------------------------------------------------------------------
#define UDP_ICQ_MSG							100			//文字聊天消息

#define UDP_ACD_INFROMATION_MSG				200			//路由点状态数据报告消息
//------------------------------------------------------------------------
// 通信消息包结构定义
struct SUDPPackage
{
    SUDPMsgHeader	MsgHeader;					// 消息包头
	UL				ResourceID;					// 发送源编号
	char			ResourceName[50];			// 发送源
	UL				TargetID;					// 接收源编号
	char			TargetName[50];				// 接收源名称
	UC				MessageType;		    	// 消息类型
    char			Title[MAXHEADLEN];		// 标题
	char			Content[MAXMSGLEN];		// 内容
};

//------------------------------------------------------------------------
// 
struct SACDPackage
{
	SUDPMsgHeader	MsgHeader;					// 消息包头
	S_TCP_PCS2ALL_ACD_STATUS_REPORT MsgBody;	// 消息包
	/*
	US		nGroupID;				// 路由点组ID，0表示整个中心
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
	US		nAgentIdle;				// 空闲座席数
	US		nAgentBusy;				// 繁忙座席数
	US		nAgentNotReady;			// 未就绪座席数
	US		nAgentAfterCall;		// 后处理座席数
	US		nAgentLogout;			// 未登录座席数
	*/
};

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_MSGR_H__
//------------------------------------------------------------------------
