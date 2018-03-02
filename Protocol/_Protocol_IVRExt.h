////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		_Protocol_IVRExt.h
// 用途描述：	IVR与外部接口模块通信协议
// 作者：		STEVEN
// 创建时间：	2005/03/19
// 最后修改时间：2005/03/21
// 版权信息：	Copyright (C) 1999 - 2005 PCSG
// 备注：
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_IVREXT_H__
#define __PROTOCOL_IVREXT_H__
//------------------------------------------------------------------------

//--------------------------
// 注册 - 2
//------------
#define TCP_IVR2EXT_REGISTER				1			// (0x0001) 注册消息
struct S_TCP_IVR2EXT_REGISTER
{
	US		ChannelCount;								// 通道数量
};
//------------

//------------
#define TCP_EXT2IVR_REGISTER_ACK			1001		// (0x03E9) 注册响应消息
struct S_TCP_EXT2IVR_REGISTER_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
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
#define TCP_IVR2EXT_UNREGISTER				2			// (0x0002) 取消注册消息
//------------

//------------
#define TCP_EXT2IVR_UNREGISTER_ACK			1002		// (0x03EA) 取消注册响应消息
struct S_TCP_EXT2IVR_UNREGISTER_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
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
// 外拨控制消息 - 11
//------------
#define TCP_EXT2IVR_TASK_ADD				0x11		// 增加拨号任务
struct S_TCP_EXT2IVR_TASK_ADD
{
	UL		PacketID;						// 消息ID
	US		GroupNO;						// IVR组号
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 拨号任务ID
	UC		TelNoLength;					// 电话号码长度
	UC		ServiceCode;					// 服务代码，'0'-'9', 'a'-'z', 'A'-'Z'有效，其它不记录
	US		NotesLength;					// 备注长度
	US		StartNode;						// 进入IVR流程节点ID，如果小于256则从根节点开始
};
//------------

//------------
#define TCP_EXT2IVR_TASK_REMOVE				0x12		// 删除拨号任务
struct S_TCP_EXT2IVR_TASK_REMOVE
{
	UL		PacketID;						// 消息ID
	US		GroupNO;						// IVR组号
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 拨号任务ID
};
//------------

//------------
#define TCP_EXT2IVR_TASK_REMOVE_ALL			0x13		// 删除所有任务
struct S_TCP_EXT2IVR_TASK_REMOVE_ALL
{
	UL		PacketID;						// 消息ID
	US		GroupNO;						// IVR组号
};
//------------

//------------
#define TCP_EXT2IVR_TASK_QUERY				0x14		// 查询拨号任务状态
struct S_TCP_EXT2IVR_TASK_QUERY
{
	UL		PacketID;						// 消息ID
	US		GroupNO;						// IVR组号
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 拨号任务ID
};
//------------

//------------
#define TCP_EXT2IVR_TASK_LIST_QUERY			0x15		// 查询拨号任务列表
struct S_TCP_EXT2IVR_TASK_LIST_QUERY
{
	UL		PacketID;						// 消息ID
	US		GroupNO;						// IVR组号
};
//------------			

//------------
#define TCP_IVR2EXT_TAST_ADD_ACK			0x111		// 增加拨号任务应答
struct S_TCP_IVR2EXT_TAST_ADD_ACK
{
	UL		PacketID;						// 消息ID
	US		GroupNO;						// IVR组号
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 拨号任务ID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NOTSUPPORT
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	US		TaskPos;						// 任务的位置
};
//------------

//------------
#define TCP_IVR2EXT_TAST_REMOVE_ACK			0x112		// 删除拨号任务应答
struct S_TCP_IVR2EXT_TAST_REMOVE_ACK
{
	UL		PacketID;						// 消息ID
	US		GroupNO;						// IVR组号
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 拨号任务ID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NOTSUPPORT
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	US		TaskPos;						// 任务的位置
};
//------------

//------------
#define TCP_IVR2EXT_TAST_REMOVE_ALL_ACK		0x113		// 删除所有任务应答
struct S_TCP_IVR2EXT_TAST_REMOVE_ALL_ACK
{
	UL		PacketID;						// 消息ID
	US		GroupNO;						// IVR组号
	US		TaskCount;						// 删除任务数量
};
//------------

//------------
#define TCP_IVR2EXT_TASK_QUERY_ACK			0x114		// 查询拨号任务状态
struct S_TCP_IVR2EXT_TASK_QUERY_ACK
{
	UL		PacketID;						// 消息ID
	US		GroupNO;						// IVR组号
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 拨号任务ID
	UC		TaskStatus;						// 任务状态，参见_pcC_OutBoundTaskFlag
	US		Duration;						// 当前状态持续时间
};
//------------

//------------
#define TCP_IVR2EXT_TASK_LIST_QUERY_ACK		0x115		// 查询拨号任务列表
struct S_TCP_IVR2EXT_TASK_LIST_QUERY_ACK
{
	UL		PacketID;						// 消息ID
	US		GroupNO;						// IVR组号
	US		TaskCount;						// 当前活动任务总数
	UL		FinishTask;						// 本日累计完成任务
	UL		SuccTask;						// 本日累计成功任务
	US		AveLine;						// 任务平均通道占用时间
};
//------------

//------------
#define TCP_IVR2EXT_TASK_RESULT				0x120		// 拨号任务执行结果
struct S_TCP_IVR2EXT_TASK_RESULT
{
	UL		PacketID;						// 消息ID
	US		GroupNO;						// IVR组号
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 拨号任务ID
	UC		TaskResult;						// 任务执行结果，参见_pcC_OutBoundCallResult
	UL		CallRefID;						// 呼叫标示
};
//------------
//--------------------------

//--------------------------
// 通用数据包结构
/// IVR -> Ext 命令字 [2000 - 2999]
/// Ext -> IVR 命令字 [3000 - 3999]
struct S_TCP_IVREXT_PACKGET
{
	UL PacketID;							// 消息ID
	US ChannelNO;							// 通道号
	UC Seperator;							// 分隔符
	US VariableLength;						// 变量长度
	US NotesLength;							// 备注长度
};
//--------------------------

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_IVREXT_H__
//------------------------------------------------------------------------
