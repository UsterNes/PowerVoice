////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		_Protocol_OBS.h
// 用途描述：	外拨服务通信协议
// 作者：		SUN
// 创建时间：	1999/12/01
// 最后修改时间：2003/09/20
// 版权信息：	Copyright (C) 1999 - 2003 PCSG
// 备注：
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_OBSERVICE_H__
#define __PROTOCOL_OBSERVICE_H__
//------------------------------------------------------------------------

//--------------------------
// 注册 - 2
//------------
#define TCP_OBS2PCS_REGISTER				1			// (0x0001) 注册消息
struct S_TCP_OBS2PCS_REGISTER
{
	UC		Version;						// 版本号
	UC		RegCode[PCS_DEF_NUMLEN_APP];	// 注册码
};
//------------

//------------
#define TCP_PCS2OBS_REGISTER_ACK			1001		// (0x03E9) 注册响应消息
struct S_TCP_PCS2OBS_REGISTER_ACK
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
#define TCP_OBS2PCS_UNREGISTER				2			// (0x0002) 取消注册消息
//------------

//------------
#define TCP_PCS2OBS_UNREGISTER_ACK			1002		// (0x03EA) 取消注册响应消息
//------------
//--------------------------


//--------------------------
// 预览外拨 - 7
//------------
#define TCP_OBS2PCS_AGENTTASK_REQUEST		401			// (0x0191) 指定座席外拨任务请求消息
struct S_TCP_OBS2PCS_AGENTTASK_REQUEST
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
#define TCP_OBS2PCS_GROUPTASK_REQUEST		402			// (0x0192) 外拨任务分配请求消息
struct S_TCP_OBS2PCS_GROUPTASK_REQUEST
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

//------------
#define TCP_OBS2PCS_TASK_CANCEL				411			// (0x019B) 外拨任务取消消息
struct S_TCP_OBS2PCS_TASK_CANCEL	
{
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 任务ID
	UL		AgentID;						// 请求座席ID
};
//------------

//------------
#define TCP_PCS2OBS_TASK_REQUEST_ACK		1401			// (0x0579) 外拨任务请求响应消息
struct S_TCP_PCS2OBS_TASK_REQUEST_ACK
{
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 任务ID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		TargetID;						// 请求座席ID、座席组或小组ID
};
//------------

//------------
#define TCP_PCS2OBS_TASK_EXECUTE			1402			// (0x057A) 外拨任务开始执行消息
struct S_TCP_PCS2OBS_TASK_EXECUTE
{
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 任务ID
	UL		AgentID;						// 请求座席ID
};
//------------

//------------
#define TCP_PCS2OBS_TASK_FINISHED			1403			// (0x057B) 外拨任务完成消息
struct S_TCP_PCS2OBS_TASK_FINISHED
{
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 任务ID
	UC		ResultCode;						// 外拨执行结果，参照_pcC_OutBoundCallResult
	UC		TryTimes;						// 尝试次数
	UL		AgentID;						// 请求座席ID
};
//------------

//------------
#define TCP_PCS2OBS_TASK_CANCEL_ACK			1411			// (0x0583) 外拨任务取消响应消息
struct S_TCP_PCS2OBS_TASK_CANCEL_ACK
{
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 任务ID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		AgentID;						// 请求座席ID
};
//------------
//--------------------------


//--------------------------
// 预测外拨 - 8
//------------
#define TCP_OBS2PCS_PREDICTIVE_AGENTTASK_REQUEST	431		// (0x1AF) 占用座席消息
struct S_TCP_OBS2PCS_PREDICTIVE_AGENTTASK_REQUEST
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
#define TCP_OBS2PCS_PREDICTIVE_GROUPTASK_REQUEST	432			// (0x01B0) 占用座席(组)消息
struct S_TCP_OBS2PCS_PREDICTIVE_GROUPTASK_REQUEST
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
#define TCP_OBS2PCS_PREDICTIVE_CANCELTASK			441			// (0x01B9) 预览外拨任务取消消息
struct S_TCP_OBS2PCS_PREDICTIVE_CANCELTASK	
{
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 任务ID
	UL		AgentID;						// 请求座席ID
};
//------------

//------------
#define TCP_OBS2PCS_PREDICTIVE_TRANSFERTASK			442			// (0x01BA) 外拨电话接入消息
struct S_TCP_OBS2PCS_PREDICTIVE_TRANSFERTASK
{
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 任务ID
	UL		AgentID;						// 请求座席ID
	UL		ControlAgentID;					// 外拨座席ID
	UL		CallRefID;						// 呼叫标示，如果为0则系统自动选择
	UC		EventSource;					// 事件来源，参照_pcC_EventSource，如果为0则系统自动选择
	char	strTelNo[PCS_DEF_NUMLEN_APP];	// 外拨电话号码
	char	strType[4];						// 外拨类型（用户自定义）
	char	strData[200];					// 外拨数据（用户自定义）
};
//------------

//------------
#define TCP_PCS2OBS_PREDICTIVE_TASK_REQUEST_ACK		1431			// (0x0597) 预测外拨任务请求响应消息
struct S_TCP_PCS2OBS_PREDICTIVE_TASK_REQUEST_ACK
{
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 任务ID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		TargetID;						// 请求座席ID、座席组或小组ID
};
//------------

//------------
#define TCP_PCS2OBS_PREDICTIVE_CANCELTASKACK		1441			// (0x05A1) 预测外拨任务取消响应消息
struct S_TCP_PCS2OBS_PREDICTIVE_CANCELTASKACK
{
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 任务ID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		AgentID;						// 请求座席ID
};
//------------

//------------
#define TCP_PCS2OBS_PREDICTIVE_TRANSFERTASK_ACK		1442			// (0x052A) 外拨电话接入消息
struct S_TCP_PCS2OBS_PREDICTIVE_TRANSFERTASK_ACK
{
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 任务ID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		AgentID;						// 请求座席ID
};
//------------

//------------
/// 收到座席发送的开始任务消息后, Power Call Server向发送OB Server发送任务完成消息
#define TCP_PCS2OBS_PREDICTIVE_FINISHTASK			1443			// (0x052B) 任务完成消息
struct S_TCP_PCS2OBS_PREDICTIVE_FINISHTASK
{
	US		CampaignID;						// 项目ID
	UL		TaskID;							// 任务ID
	UL		AgentID;						// 请求座席ID
};
//------------
//--------------------------


//--------------------------
// 状态查询 - 2
//------------
#define TCP_OBS2PCS_STATUS_QUERY			491			// (0x01EB) 查询状态消息
struct S_TCP_OBS2PCS_STATUS_QUERY
{
	UL		AgentID;						// 查询座席ID
};
//------------

//------------
#define TCP_PCS2OBS_STATUS_QUERY_ACK		1491		// (0x05D3) 查询状态响应消息
struct S_TCP_PCS2OBS_STATUS_QUERY_ACK
{
	UL		AgentID;						// 查询座席ID
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
#endif  //End #ifndef __PROTOCOL_OBSERVICE_H__
//------------------------------------------------------------------------
