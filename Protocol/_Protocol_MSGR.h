////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		_Protocol_MSGR.h
// 用途描述：	消息服务通信协议
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
#ifndef __PROTOCOL_MSGR_H__
#define __PROTOCOL_MSGR_H__
//------------------------------------------------------------------------

//--------------------------
// 注册 - 2
//------------
#define TCP_MSG2PCS_REGISTER				1			// (0x0001) 注册消息
struct S_TCP_MSG2PCS_REGISTER
{
	UC		Version;						// 版本号
	UC		RegCode[PCS_DEF_NUMLEN_APP];	// 注册码
	US		nMSGPort;						// 消息服务器侦听端口
	char	strMSGIP[20];					// 消息服务器IP地址
};
//------------

//------------
#define TCP_PCS2MSG_REGISTER_ACK			1001		// (0x03E9) 注册响应消息
struct S_TCP_PCS2MSG_REGISTER_ACK
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
#define TCP_MSG2PCS_UNREGISTER				2			// (0x0002) 取消注册消息
//------------

//------------
#define TCP_PCS2MSG_UNREGISTER_ACK			1002		// (0x03EA) 取消注册响应消息
//------------
//--------------------------

//--------------------------
// 远程控制(同RM有关消息) - 4
//------------
//------------
#define TCP_MSG2PCS_RELOAD_ACD_SCENARIO_ACK	351			// (0x015F) 重新加载排队规则响应消息
struct S_TCP_MSG2PCS_RELOAD_ACD_SCENARIO_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_MSG2PCS_RELOAD_USERLIST_ACK		352			// (0x0160) 重新加载用户列表响应消息
struct S_TCP_MSG2PCS_RELOAD_USERLIST_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2MSG_RELOAD_ACD_SCENARIO		1351		// (0x0547) 重新加载排队规则消息
//------------


//------------
#define TCP_PCS2MSG_RELOAD_USERLIST			1352		// (0x0548) 重新加载用户列表消息
//------------

//--------------------------

//--------------------------
// Sun added 2010-02-12
// 状态查询 - 2
//------------
#define TCP_MSG2PCS_STATUS_QUERY			306			// (0x0132) 查询状态消息
struct S_TCP_MSG2PCS_STATUS_QUERY
{
	UL		AgentID;						// 座席ID
};
//------------

//------------
#define TCP_PCS2MSG_STATUS_QUERY_ACK		1306		// (0x051A) 查询状态响应消息
struct S_TCP_PCS2MSG_STATUS_QUERY_ACK
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
//--------------------------

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_MSGR_H__
//------------------------------------------------------------------------
