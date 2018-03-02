////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		_pcCommonDef.h
// 用途描述：   PowerCall共通定义
// 作者：		SUN
// 创建时间：	2002/06/05
// 最后修改时间：2006/08/21
// 版权信息：	Copyright (C) 1998 - 2004 Proactive Cyber Space
// 备注：
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PCCOMMONDEF_H__
#define __PCCOMMONDEF_H__
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// 消息客户端状态
enum _pcC_stMsgClientStatus
{
	//_pcC_stMsgClientStatus_UNKNOWN = 0,			// 无效
	_pcC_stMsgClientStatus_LOGOUT,				// 未登录
	_pcC_stMsgClientStatus_LOGIN,				// 未登录
	//_pcC_stMsgClientStatus_READY,				// 就绪
	//_pcC_stMsgClientStatus_NOTREADY,			// 未就绪
};
/*
//------------------------------------------------------------------------
// 消息客户端未就绪原因
enum _pcC_stMsgClientReason
{
	_pcC_stMsgClientReason_UNKNOWN = 0,			// 为止
	_pcC_stMsgClientReason_BUSY,				// 忙碌
	_pcC_stMsgClientReason_LEAVE,				// 离开
};
*/
//------------------------------------------------------------------------
// 消息客户端类型
enum _pcC_stMsgClientType
{
	_pcC_stMsgClientType_USEDUSER = 0,			// 分组坐席
	_pcC_stMsgClientType_UNUSEDUSER,			// 未分组坐席
	_pcC_stMsgClientType_MONITOR,				// MONITOR
};

//------------------------------------------------------------------------
// 位置状态
enum _pcC_stPOS
{
	_pcC_POS_UNKNOWN = 0,					// 无效
	_pcC_POS_LOGOUT,						// 未登录
	_pcC_POS_NOTREADY,						// 未就绪
	_pcC_POS_AFTERCALL,						// 后处理
	_pcC_POS_IDLE,							// 空闲	
	_pcC_POS_RING,							// 震铃
	_pcC_POS_BUSY,							// 忙
	_pcC_POS_HOLD							// 呼叫保持
};

//------------------------------------------------------------------------
// 话机状态
enum _pcC_stDN
{
	_pcC_DN_UNKNOWN = 0,					// 无效
	_pcC_DN_DND,							// 免打扰
	_pcC_DN_IDLE,							// 空闲	
	_pcC_DN_RING,							// 震铃
	_pcC_DN_OFFHOOK,						// 摘机
	_pcC_DN_INBOUND,						// 呼入通话
	_pcC_DN_OUTBOUND,						// 呼出通话
	_pcC_DN_HOLD							// 呼叫保持
};

//------------------------------------------------------------------------
// 话机类型
enum _pcC_PhoneType
{
	_pcC_PH_TYPE_UNKNOWN = 0,				// 无话机
	_pcC_PH_TYPE_ANA_HAND,					// 模拟带手柄
	_pcC_PH_TYPE_ANA_EAR,					// 模拟带耳机
	_pcC_PH_TYPE_DGT_HANDLE,				// 数字带手柄 (eg. 2008)
	_pcC_PH_TYPE_DGT_EAR,					// 数字带耳机 (eg. 2616)
	_pcC_PH_TYPE_DGT_IP						// IP 话机
};

//------------------------------------------------------------------------
// 事件来源
enum _pcC_EventSource
{
	_pcC_EVTSRC_UNKNOWN = 0,				// 未知
	_pcC_EVTSRC_DN,							// 来自DN
	_pcC_EVTSRC_POSITION,					// 来自Position
	_pcC_EVTSRC_ACD,						// 来自ACD DN
	_pcC_EVTSRC_CDN							// 来自CDN (Route Point)	
};

//------------------------------------------------------------------------
// 呼叫来源
enum _pcC_CallSource
{
	_pcC_CALLSRC_INTERNAL = 0,				// 内部转接及其它
	_pcC_CALLSRC_CALLCENTER,				// 外部-本呼叫中心
	_pcC_CALLSRC_LOADBALANCE,				// 外部-其它呼叫中心平衡负载
	_pcC_CALLSRC_WEB						// 外部-Web
};

//------------------------------------------------------------------------
// 呼叫类型
enum _pcC_CallType
{
    _pcC_CALLTYPE_UNKNOWN = 0,
    _pcC_CALLTYPE_INBOUND,
    _pcC_CALLTYPE_OUTBOUND,
    _pcC_CALLTYPE_CONFERENCE
};

//------------------------------------------------------------------------
// 呼叫范围
enum _pcC_CallScale
{
    _pcC_CALLSCALE_UNKNOWN = 0,
    _pcC_CALLSCALE_INTERNAL,
    _pcC_CALLSCALE_LOCAL,
	_pcC_CALLSCALE_NATIONAL,
	_pcC_CALLSCALE_INTERNATIONAL,
    _pcC_CALLSCALE_WEB
};

//------------------------------------------------------------------------
// 操作主体
enum _pcC_CallOperator
{
    _pcC_CALLOPT_THISPARTY = 0,
    _pcC_CALLOPT_OTHERPARTY,
	_pcC_CALLOPT_THIRDPARTY
};

//------------------------------------------------------------------------
// 呼叫磋商模式
enum _pcC_ConsultationMode
{
	_pcC_CONSMODE_GENERIC = 0,
    _pcC_CONSMODE_TRANSFER = 1,
	_pcC_CONSMODE_CONFERENCE = 2
};

//------------------------------------------------------------------------
// 路由等待处理方式
enum _pcC_RouteRespondMethod
{
	_pcC_RRMETHOD_UNKOWN = 0,
	_pcC_RRMETHOD_TRANSFER,
    _pcC_RRMETHOD_IVRACD,
	_pcC_RRMETHOD_RINGBACK,
	_pcC_RRMETHOD_BUSYTONE,
	_pcC_RRMETHOD_MUSIC,
	_pcC_RRMETHOD_RAN
};

//------------------------------------------------------------------------
// 路由结果
enum _pcC_RouteRespondResult
{
	_pcC_RRRESULT_UNKNOWN = 0,
	_pcC_RRRESULT_TIMEOUT,					// 转接超时放弃
	_pcC_RRRESULT_FAILED,					// 转接失败
	_pcC_RRRESULT_ABANDON,					// 转接主动放弃
	_pcC_RRRESULT_SUCC,						// 转接成功
	_pcC_RRRESULT_SUCC_INTIME,				// 在服务水平内转接成功
};

//------------------------------------------------------------------------
// 外拨任务状态
enum _pcC_OutBoundTaskFlag
{
	_pcC_OB_TASK_FLAG_NEW =	'N',			// 新任务
	_pcC_OB_TASK_FLAG_OVER = 'C',			// 完成
	_pcC_OB_TASK_FLAG_PROCESS = 'I',		// 处理中
	_pcC_OB_TASK_FLAG_DISPATCH = 'D',		// 已分派
	_pcC_OB_TASK_FLAG_ERROR = 'E',			// 错误
	_pcC_OB_TASK_FLAG_RETRY = 'R',			// 重试
	_pcC_OB_TASK_FLAG_OVERDUE = 'O',		// 过期
	_pcC_OB_TASK_FLAG_TRANSFER = 'T',		// 转接中
	_pcC_OB_TASK_FLAG_PAUSE = 'P'			// 暂停

};

//------------------------------------------------------------------------
// 外拨执行结果
enum _pcC_OutBoundCallResult
{
	_pcC_OB_RESULT_SUCC = '0',				// '0' - 成功
	_pcC_OB_RESULT_NO_ANS = '1',			// '1' - 无应答
	_pcC_OB_RESULT_DEST_BUSY = '2',			// '2' - 对方忙
	_pcC_OB_RESULT_ABANDON = '3',			// '3' - 放弃
	_pcC_OB_RESULT_DEST_WRONG = '6',		// '6' - 号码错
	_pcC_OB_RESULT_POWER_OFF = '7',			// '7' - 关机
	_pcC_OB_RESULT_NOT_REACH = '8',			// '8' - 不在服务范围
	_pcC_OB_RESULT_STOP_SVR = '9',			// '9' - 欠费停机
	_pcC_OB_RESULT_FAX_TONE = 'F',			// 'F' - 传真
	_pcC_OB_RESULT_IVR_TONE = 'A',			// 'A' - 自动语音
	_pcC_OB_RESULT_MODEM_TONE = 'M',		// 'M' - Modem
	_pcC_OB_RESULT_OP_TIMEOUT = 'T',		// 'T' - Timeout
	_pcC_OB_RESULT_SYSTEM_ERROR = 'S',		// 'S' - System Error
	_pcC_OB_RESULT_OTHERS = 'O'				// 'O' - 其它
};

//------------------------------------------------------------------------
// 统一消息任务类型
enum _pcC_UMSTaskTypes
{
	_pcC_UMS_TASK_TYPE_UNKNOWN = 0,			// 未知
	_pcC_UMS_TASK_TYPE_IBCALL,				// 呼入电话
	_pcC_UMS_TASK_TYPE_OBCALL,				// 呼出电话
	_pcC_UMS_TASK_TYPE_GENERAL = 10,		// 通用任务
	_pcC_UMS_TASK_TYPE_EMAIL,				// 电子邮件任务
	_pcC_UMS_TASK_TYPE_FAX,					// 传真任务
	_pcC_UMS_TASK_TYPE_SMS,					// 短信任务
	_pcC_UMS_TASK_TYPE_VMS,					// 留言任务
	_pcC_UMS_TASK_TYPE_CB,					// 回拨任务
	_pcC_UMS_TASK_TYPE_BBS,					// BBS任务
	_pcC_UMS_TASK_TYPE_CHAT,				// Web Chat任务
	_pcC_UMS_TASK_TYPE_WEBCALL,				// Web Call任务
	_pcC_UMS_TASK_TYPE_WEBCOLLAB,			// Web 协同任务
	_pcC_UMS_TASK_TYPE_VIDEO				// 视频任务
};

//------------------------------------------------------------------------
// 多方操作事件定义
enum _pcC_MultiPartyOperationEvent
{
	_pcC_MULTIOP_DUMMY = '0',				// 未定义
	_pcC_MULTIOP_TpTRANSFER = '1',			// This Party Transfer
	_pcC_MULTIOP_OpTRANSFER = '2',			// Other Party Transfer
	_pcC_MULTIOP_TpCONFERENCE = '3',		// This Party Conference
	_pcC_MULTIOP_OpCONFERENCE = '4',		// Other Party Conference
	_pcC_MULTIOP_TpTransfered = '5',		// This Party Transfered Successfully
	_pcC_MULTIOP_OpConferenced = '6',		// Other Party Conferenced Successfully
	_pcC_MULTIOP_TpConferenced = '7',		// This Party Conferenced Successfully
	_pcC_MULTIOP_OpTransfered = '8',		// Other Party Transfered Successfully
	_pcC_MULTIOP_TpSlowTransfered = 'C',	// This Party Slow Transfered Successfully
	_pcC_MULTIOP_TpFastTransfered = 'F',	// This Party Fast Transfered Successfully
};

//------------------------------------------------------------------------
// 话务分配对象
enum _pcC_CallDistributeObject
{
	_pcC_CDOBJ_CENTER = 0,					// 本中心全体座席
	_pcC_CDOBJ_GROUP = 1,					// 座席组
	_pcC_CDOBJ_TEAM = 2,					// 座席小组
	_pcC_CDOBJ_AGENT = 3,					// 座席
	_pcC_CDOBJ_DN = 4,						// 物理分机, Sun added 2011-12-20
	_pcC_CDOBJ_ACD = 6,						// 物理分机组, Sun added 2011-12-20
	_pcC_CDOBJ_QUEUE = 7,					// 队列
	_pcC_CDOBJ_ROUTEPOINT = 8,				// 路由点
	_pcC_CDOBJ_DEPARTMENT = 11,				// 部门
	_pcC_CDOBJ_CUST = 12,					// 租户
	_pcC_CDOBJ_GLOBAL = 15					// 所有中心
};

//------------------------------------------------------------------------
// 呼叫转移类型定义
enum _pcC_CallForwardType
{
	_pcC_CFWT_ALL = 0,						// 转移所有电话
	_pcC_CFWT_BUSY,							// 遇忙转移
	_pcC_CFWT_NOANS,						// 无应答转移
	_pcC_CFWT_BUSY_NOANS					// 遇忙、无应答转移
};

//------------------------------------------------------------------------
// 排队方式
enum _pcC_QueueType
{
	_pcC_FIRST_AVALIABLE = 1,				// 知更鸟
	_pcC_MAX_IDEL_TIME,						// 最长空闲时间(上次通话后累计)
	_pcC_MIN_CALL_DENAITY,					// 单位时间最小通话密度(次数)
	_pcC_MIN_WORKLOAD,						// 单位时间最小工作负荷(通话时长+后处理时长)
	_pcC_LONGEST_IDEL,						// 最大空闲计时(不累计)
	_pcC_LONGEST_CALL_INTERVAL				// 最大通话间隔(不累计)
};

//------------------------------------------------------------------------
// 溢出规则
enum _pcC_OverFlow
{	
	_pcC_MAX_LENGTH = 1,					// 队列长度
	_pcC_MAX_WAITE_TIME,					// 最长等待时间
	_pcC_MIN_LOGIN_COUNT,					// 最小登录坐席数
	_pcC_MIN_IDLE_COUNT						// 最小空闲数
};

//------------------------------------------------------------------------
// 监控端呼叫控制类型
enum _pcC_MonitorControlType
{	
	_pcC_MONCTRL_Logout = 0,				// 强制注销
	_pcC_MONCTRL_SingleStepTran = 1,		// 单步转接
	_pcC_MONCTRL_SingleStepConf = 2,		// 单步会议
	_pcC_MONCTRL_ConsultTran = 3,			// 磋商转接
	_pcC_MONCTRL_ConsultConf = 4,			// 磋商会议
	_pcC_MONCTRL_CompleteTran = 5,			// 转接完成
	_pcC_MONCTRL_CompleteConf = 6,			// 会议完成
	_pcC_MONCTRL_HoldCall = 7,				// 强制保持
	_pcC_MONCTRL_RetrieveCall = 8,			// 强制拾起
	_pcC_MONCTRL_ClearCall = 9,				// 强制挂断
};

// Simon Added 2010-05-04
// PCS状态定义：
typedef enum _tag_PCSStatus
{
	PCS_PS_Init,				// 初始化
	PCS_PS_Active,				// 活跃
	PCS_PS_Standby,				// 就绪（后备）
	PCS_PS_Disconnected,		// 断开(未知)
	PCS_PS_Closed,				// 已经关闭
}PCS_PCSStatus;

// RM状态定义：
typedef enum _tag_RMStatus
{
	PCS_RMS_Init,				// 初始化
	PCS_RMS_InService,			// 服务中
	PCS_RMS_Disconnected,		// 断开(未知)
	PCS_RMS_Closed,				// 已经关闭
}PCS_RMStatus;

//------------------------------------------------------------------------
// 座席类别定义
#define PCS_DEF_AGTTYP_USER					1			// 话务员座席
#define PCS_DEF_AGTTYP_IVR					2			// IVR设备
#define PCS_DEF_AGTTYP_DIALER				3			// 外拨设备

//------------------------------------------------------------------------
// 话务员类别定义
#define PCS_DEF_USERTYP_INBOUND				0x00000001	// 呼入处理
#define PCS_DEF_USERTYP_OUTBOUND			0x00000002	// 呼出处理
#define PCS_DEF_USERTYP_WEB					0x00000004	// Web Call & 视频请求处理
#define PCS_DEF_USERTYP_EMAIL				0x00000008	// EMail处理
#define PCS_DEF_USERTYP_FAX					0x00000010	// FAX处理
#define PCS_DEF_USERTYP_SMS					0x00000020	// SMS处理
#define PCS_DEF_USERTYP_VMS					0x00000040	// VMS处理
#define PCS_DEF_USERTYP_CALLBACK			0x00000080	// 回拨记录处理
#define PCS_DEF_USERTYP_BBS					0x00000100	// BBS & Chat请求处理

//------------------------------------------------------------------------
// 返回码定义
#define PCS_DEF_RETCODE_SUCC				0			// (0x00) 成功
#define PCS_DEF_RETCODE_NOTEXIST			1			// (0x01) 座席不存在
#define PCS_DEF_RETCODE_BEUSED				2			// (0x02) 已经占用
#define PCS_DEF_RETCODE_STATUS				3			// (0x03) 状态错
#define PCS_DEF_RETCODE_NOTNEED				4			// (0x04) 不需要
#define PCS_DEF_RETCODE_VERSION				10			// (0x0A) 版本错误
#define PCS_DEF_RETCODE_REGCODE				11			// (0x0B) 注册码错误
#define PCS_DEF_RETCODE_PASSWORD			12			// (0x0C) 口令错
#define PCS_DEF_RETCODE_NORIGHTS			13			// (0x0D) 无权限
#define PCS_DEF_RETCODE_NOLICENSE			14			// (0x0E) 许可数量不足
#define PCS_DEF_RETCODE_LACK_PARAM			15			// (0x0F) 参数不足
#define PCS_DEF_RETCODE_REJECTED			16			// (0x10) 请求被拒绝 
#define PCS_DEF_RETCODE_NOTSUPPORT			99			// (0x63) 不支持
#define PCS_DEF_RETCODE_OTHERS				100			// (0x64) 其它错误
#define PCS_DEF_RETCODE_SYSERROR			255			// (0xFF) 系统错

//------------------------------------------------------------------------
// Agent Working Detail Record Status
#define PCS_AWDR_EVENT_CLOSE_LOG			'L'			// Login 辅助处理
#define PCS_AWDR_EVENT_CLOSE_RDY			'R'			// Ready 辅助处理
#define PCS_AWDR_EVENT_LOGOUT				'0'			// Logout
#define PCS_AWDR_EVENT_LOGIN				'1'			// Login
#define PCS_AWDR_EVENT_READY				'2'			// Ready
#define PCS_AWDR_EVENT_NOTREADY				'3'			// Not Ready
#define PCS_AWDR_EVENT_AFTERCALL			'4'			// After Call Work (IB or Default)
#define PCS_AWDR_EVENT_TP_RING				'5'			// This party ringing
#define PCS_AWDR_EVENT_OP_RING				'6'			// Other party ringing
#define PCS_AWDR_EVENT_OB_AFTERCALL			'7'			// After Call Work (OB)
#define PCS_AWDR_EVENT_AVAILABLE			'A'			// Available (Agent Ready & Phone Idle)
#define PCS_AWDR_EVENT_IBCALL				'I'			// In bound call
#define PCS_AWDR_EVENT_OBCALL				'O'			// Out bound call
#define PCS_AWDR_EVENT_IBHOLD				'H'			// In bound hold
#define PCS_AWDR_EVENT_OBHOLD				'Q'			// Out bound hold
#define PCS_AWDR_EVENT_PICKUP				'P'			// Call Pickup, Sun added 2011-12-20

//------------------------------------------------------------------------
// 开关常量定义
#define PCS_DEF_CHOICE_NO					0			// Off / No / Disnabled
#define PCS_DEF_CHOICE_YES					1			// On / Yes / Enabled
#define PCS_DEF_UNNORMAL_EXIT				2			// 非正常退出

//------------------------------------------------------------------------
// 数据长度常量定义
#define PCS_DEF_LEN_GCREF					24			// length of global call ref id
#define PCS_DEF_NUMLEN_APP					32			// Tel. Number data length
#define PCS_DEF_DATALEN_APP					128			// Application data length
#define PCS_DEF_DATALEN_USER				256			// User data length
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// 录音系统常数定义
//------------------------------------------------------------------------

//
// 录音格式定义 added by stephen wang 2006-08-21
//
enum _pcC_RecFormatTag
{
	_pcC_RFT_UNKNOW = 0,						// 未知格式
	_pcC_RFT_G729,								// G.729格式
	_pcC_RFT_ADPCM,								// ADPCM格式
};


//
// 录音存储方式定义 added by stephen wang 2006-12-12
//
enum _pcC_APRecSaveMode
{
	_pcC_AP_RSM_UNKNOW = 0,						// 未知格式
	_pcC_AP_RSM_NETWORK,						// 网络传输
	_pcC_AP_RSM_LOCAL,							// AP本地保存
	_pcC_AP_RSM_LOCAL_AND_NO_REALTIMEDTA,		// AP本地保存（直接保存文件）
	_pcC_AP_RSM_LOCAL_AND_NETWORK,				// 本地加网络
};

// Simon Added 2011-06-28
// 枚举：ACD控制模式 For Avaya
enum _pcC_ACDControlMode
{
	_pcC_AM_NONE=0,				//什么也不做
	_pcC_AM_ALLREADLY,			//自动就绪所有座席
	_pcC_AM_SYNCH,				//与软ACD同步
};

#define _pcC_AP_PARAM_TIME_SYNC						(_T("TimeToSync"))				// 服务器间时间同步时间
#define _pcC_AP_PARAM_FILE_SAVE_MODE				(_T("FileSaveMethode"))			// 文件保存方式
#define _pcC_AP_PARAM_FILE_SAVE_TYPE				(_T("FilesSaveType"))			// 文件保存格式
#define _pcC_AP_PARAM_FILE_CONTROL_TYPE				(_T("RecControlType"))			// AP控制方式
#define _pcC_AP_PARAM_FILE_REC_INDEX_SYNC_TIME		(_T("RecordIndexSyncTime"))		// AP未上传的录音索引日志

//
// 录音控制方式
//
#define PCS_DEF_RECORD_TRIGGER_UNKNOWN		0			// 未知
#define PCS_DEF_RECORD_TRIGGER_VOICE		1			// 声压启动
#define PCS_DEF_RECORD_TRIGGER_MANUAL		2			// 手动启动(客户控制)
#define PCS_DEF_RECORD_TRIGGER_API			3			// API控制
#define PCS_DEF_RECORD_TRIGGER_MONITOR		4			// 班长控制
#define PCS_DEF_RECORD_TRIGGER_VOLTAGE		5			// 电压启动
#define PCS_DEF_RECORD_TRIGGER_DTMF			6			// 码控
#define PCS_DEF_RECORD_TRIGGER_CTI			9			// CTI控制

//
// 采样频率
//
#define PCS_DEF_SAMPLE_FREQ_POOR			0			// 8000 Hz
#define PCS_DEF_SAMPLE_FREQ_LOW				1			// 11025 Hz
#define PCS_DEF_SAMPLE_FREQ_NORMAL			2			// 22050 Hz
#define PCS_DEF_SAMPLE_FREQ_HIGH			3			// 44100 Hz

//
// 录音文件头长度
//
#define PCS_DEF_FORMAT_DESC_LEN				512			// Bytes

//------------------------------------------------------------------------
#endif  //End #ifndef #define __PCCOMMONDEF_H__
//------------------------------------------------------------------------
