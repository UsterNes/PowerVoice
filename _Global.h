////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		_GLOBAL.h
// 用途描述：	全局类型及变量声明
// 作者：		SUN
// 创建时间：	1999/12/01
// 最后修改时间：2002/04/18
// 版权信息：	Copyright (C) 1999 - 2000 Changjiang Xincheng
// 备注：
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//------------------------------------------------------------------------
#ifndef __GLOBAL_H__
#define __GLOBAL_H__
//------------------------------------------------------------------------

#include "StdAfx.h"

#include "IVRChannel.h"
#include "IVRGroup.h"

// Sun added 2006-10-01
#include "FaxManager.h"

//------------------------------------------------------------------------
// Devices configuration
#define DEV_NONE			0
#define DEV_ANALOG			1
#define DEV_MSI				2
#define DEV_VOICE			3
#define DEV_DIGIT			4
#define DEV_ISDN			5				// ISDN
#define DEV_ICAPI			6				// E1
#define DEV_IP				10				// IP

#define TID_DISCON_DT	90
#define TID_DISCON_ST	91

#define TID_DISCON_DT2	92
#define TID_DISCON_ST2	93

#ifndef MAX_IVR_GROUP
#define MAX_IVR_GROUP		256			// 最大IVR Group
#endif

#ifndef MAX_IVR_CHANNEL
#define MAX_IVR_CHANNEL		512			// 最大IVR Channel
#endif

#ifndef MAX_MSILINES
#define MAX_MSILINES		256			// 最大的座席线路数
#endif

#ifndef MAX_CHANNEL_TONES
#define MAX_CHANNEL_TONES	13			// 最大音频参数组数量
#endif

//------------------------------------------------------------------------
// 缺省系统参数
#define FN_VOXPATH			"C:\\voice\\"		// 提示音目录
#define FN_RECPATH			"C:\\record\\"		// 录音目录
#define FN_TEMPPATH			"C:\\ivrtemp\\"		// IVR临时目录

//------------------------------------------------------------------------
// 画面常数
#define SCR_LIST_ITEM_ALL				255		// 全部项目
#define SCR_GROUP_ITEM_GROUPID			0		// 组号(Group#)
#define SCR_GROUP_ITEM_CAPTION			1		// 标题(Caption)
#define SCR_GROUP_ITEM_PROGRAM			2		// 当前流程ID(CurIVR)
#define SCR_GROUP_ITEM_RESOURCE			3		// 当前资源ID
#define SCR_GROUP_ITEM_STARTCH			4		// 起始通道(Start CH)
#define SCR_GROUP_ITEM_ENDCH			5		// 终止通道(End CH)
#define SCR_GROUP_ITEM_CTI				6		// CTI 连接状态
#define SCR_GROUP_ITEM_CALLCOUNT		7		// 呼叫累计
#define SCR_GROUP_ITEM_LASTLOADTIME		8		// 最后更新时间

#define SCR_CHANNEL_ITEM_CHID			0		// 通道号(CH#)
#define SCR_CHANNEL_ITEM_STATUS			1		// Status
#define SCR_CHANNEL_ITEM_CALLLENGTH		2		// 通话时长
#define SCR_CHANNEL_ITEM_CALLERID		3		// 主叫号码
#define SCR_CHANNEL_ITEM_CALLEEID		4		// 被叫号码
#define SCR_CHANNEL_ITEM_CTI			5		// CTI受控状态
#define SCR_CHANNEL_ITEM_CALLCOUNT		6		// 呼叫累计
#define SCR_CHANNEL_ITEM_EDID			7		// 当前状态表号

//------------------------------------------------------------------------
// 自定义消息
#ifndef MYWM_CHANGE_GROUP				// 修改IVR组信息消息
#define MYWM_CHANGE_GROUP				WM_USER + 101
#endif

#ifndef MYWM_CHANGE_CHANNEL				// 修改IVR信息消息
#define MYWM_CHANGE_CHANNEL				WM_USER + 102
#endif

#ifndef MYWM_IVREVENT					// 修改IVR组信息消息
#define MYWM_IVREVENT					WM_USER + 1001
#endif

//------------------------------------------------------------------------
// Sun added 2011-04-16
// 自动重启原因定义（开关位操作）
/// 默认开的放在低8位
#define PCS_PVC_AUTOCLOSE_MAINTHREAD_ERROR			0x0001			// 主线程吊死
#define PCS_PVC_AUTOCLOSE_WAVEOUT_NOMEM				0x0002			// Wave输出内存耗尽
#define PCS_PVC_AUTOCLOSE_DRIVER_FATALERROR			0x0004			// Driver致命错误
#define PCS_PVC_AUTOCLOSE_CREATETHREAD_ERROR		0x0080			// 连续创建挂机3次线程失败, Sun added 2013-09-09
#define PCS_PVC_AUTOCLOSE_HIGH_TRANSFAIL			0x0010			// 转接失败率过高
#define PCS_PVC_AUTOCLOSE_OPENDEV_WAITTO			0x0020			// 打开设备等待超时
#define PCS_PVC_AUTOCLOSE_CLOSEDEV_WAITTO			0x0040			// 关闭设备等待超时

// Sun added 2013-09-09
#define PCS_MAX_CREATETHREADFAIL					3				// 最大线程创建连续失败次数

//------------------------------------------------------------------------
// Agent Status Define
#define PCS_AgentAfterCallWork			100
#define PCS_AgentLogin					101
#define PCS_AgentLogout					102
#define PCS_AgentNotReady				103
#define PCS_AgentReady					104
#define PCS_AgentOtherWork				105
#define PCS_AgentInService				106
#define PCS_AgentBusy					107

//------------------------------------------------------------------------
// Other constants
#define DEF_LD_PARAM_FROM_INI		0
#define DEF_LD_PARAM_FROM_DB		1

#define DEF_LD_GRP_DEFAULT_ET		0
#define DEF_LD_GRP_ET_FILE			255
//------------------------------------------------------------------------


//------------------------------------------------------------------------
// Sun added 2007-01-16
// 测试数据格式常量定义
/// 测试数据项分隔符
#define DEF_IVR_TEST_FIELD_DELIMIT	";"
/// 测试标记字符串
#define DEF_IVR_TEST_FLAG			":IVRTestFlag"

/// 不记录CDR
#define DEF_IVR_TEST_NOCDR			":NOCDR"

/// 测试时间
#define DEF_IVR_TEST_SET_TIME		":Time="

/// 测试主叫
#define DEF_IVR_TEST_SET_ANI		":ANI="

/// 测试被叫
#define DEF_IVR_TEST_SET_DNIS		":DNIS="
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Sun added 2010-12-01 [A]
/// DTMF字符集
#define DEF_IVR_DTMF_CHARSET		"0123456789*#,;&!abcdABCD"

#define DEF_IVR_PLAY_LONGPAUSE		'-'				// 500ms
#define DEF_IVR_PLAY_SHORTPAUSE		','				// 200ms

#define DEF_IVR_DIAL_LONGPAUSE		'+'				// 加号：1000ms
#define DEF_IVR_DIAL_MIDPAUSE		'.'				// 点（句号）：500ms
#define DEF_IVR_DIAL_SHORTPAUSE		':'				// 冒号：200ms
//------------------------------------------------------------------------

//---------------------------------------------------------------------
// IVR Group
struct SIVR_Group
{
	bool Enabled;						// Switch
	bool NeedCTI;						// CTI Switch
	_bstr_t Caption;					// Title
	US StartCH;							// Start Channel
	US EndCH;							// End Channel
	UC IVRProgramID;					// Current Program ID
	_bstr_t IVRFileName;				// IVR File Name
	bool blnOBOnly;						// Only used for OB Call
	bool blnBindFaxResource;			// 传真资源是否邦定，默认为true
	DWORD dwRoutePointID;				// Tapi CTI RoutePointID
	UC bytCustID;						// Sun added 2012-09-08, 租户ID
};

//------------------------------------------------------------------------
// PerfectCall Call Analysis Data Structure
struct SFreq_T{
	int   freq;          // frequency Hz
	int   deviation;     // deviation in Hz
};

struct SState_T{
	int   time;          // time in 10ms
	int   deviation;     // deviation in ms
};

struct STone_T{
	char *		str;        // informational string
	int			tid;        // tone id
	SFreq_T		freq1;      // frequency 1
	SFreq_T		freq2;      // frequency 2
	SState_T	on;         // on time
	SState_T	off;        // off time
	int			repcnt;     // repitition count
};

//---------------------------------------------------------------------
// 系统工作状态
enum ENUM_SYSTEM_STATUS
{
	SYS_DEAD = 0,					// 没有启动 
	SYS_STARTING,					// 正在启动
	SYS_STARTED,					// 正常工作
	SYS_ERROR,						// 出错
	SYS_CLOSING,					// 正在关闭
	SYS_PENDING,					// 挂起
};

//---------------------------------------------------------------------
// 缺省流程状态(节点编号)
enum ENUM_LINE_STATUS
{
	LN_IDLE = 0,					// 空闲, 呼入, 获取主叫号码
	LN_STARTING,					// 启动流程
	LN_PLAY,						// 放音状态
	LN_SPEECH,						// 通话状态
	LN_RECORD = 9,					// 录音状态

	LN_PLAY_WELCOME = 10,			// 播放欢迎词, 提示用户直播分机号码，查号请拨0
	LN_GET_STATIONNO,				// 获取分机号码, 0 —— 接线员（开始计时）
	LN_PLAY_BUSY,					// 播放座席忙, 提示用户拨打另外一个分机号码
	LN_PLAY_NOMSI,					// 无座席工作, 请留言
	LN_PLAY_ONHOOK,					// 播放退出音
	LN_GOTO_ACD,					// 转入排队系统

	LN_CALLOUT = 50,				// 外拨状态

};

//---------------------------------------------------------------------
// 系统事件
enum ENUM_LINE_EVENT
{
	LE_CALLIN = 0,					// 电话呼入	
	LE_RINGS,						// 震铃
	LE_PLAYEND,						// 播放结束
	LE_KEYEND,						// 按键结束
	LE_RECEND,						// 录音结束
	LE_PROCEND,						// 处理结束
	LE_HOOKEND,						// 摘挂机结束
	LE_QUERYEND,					// 查询结束
	LE_MSIACK,						// 座席应答
	LE_TIMEOUT,						// 处理超时
	LE_DUMMY = 255,					// 假事件
};

//---------------------------------------------------------------------
// IVR信道状态
enum ENUM_IVRCH_STATE
{
	IVR_IDLE = 0,					// 信道空闲
	IVR_BLOCK,						// 信道阻塞
	IVR_RUN,						// 信道正常运行
	IVR_W_ACK,						// 等待消息应答
	IVR_W_PLAY,						// 等待放音结束
	IVR_W_KEY,						// 等待按键结束
	IVR_W_RECORD,					// 等待录音结束
	IVR_W_FAX,						// 等待传真结束
	IVR_W_SWITCH,					// 等待转接
	IVR_W_ANSWER,					// 等待接听
	IVR_ERROR = 255					// 信道出错
};

//---------------------------------------------------------------------
// 外部变量
extern SIVR_Group	PGroupInfo[MAX_IVR_GROUP];		// 信道组信息
extern CString		PChannelDN[MAX_IVR_CHANNEL];	// 信道信息

extern CIVRGroup	*PIVRGroup[MAX_IVR_GROUP];		// 信道组指针
extern CIVRChannel	*PIVRCH[MAX_IVR_CHANNEL];		// 信道指针

// Sun added 2006-02-20
extern BOOL			gb_Enable2DiscDTone;			// 允许第二双音频挂断音
extern BOOL			gb_Enable2DiscSTone;			// 允许第二单音频挂断音

extern STone_T		gb_Tone[MAX_CHANNEL_TONES];		// 电话系统特征音

extern CString		gb_FN_VoxPath;					// 系统语音目录
extern CString		gb_FN_RecPath;					// 系统留言目录
extern CString		gb_FN_TempPath;					// 系统临时目录

extern int			gb_VoiceVolume;					// 通道声音大小  -10 -- +10
extern int			gb_RingsPreOffhook;				// 摘机前振铃次数

// Sun added 2009-06-27
extern	BOOL		g_blnAnalogAsynDivert;			// 是否进行异步转接

// Sun added 2003-04-28
extern BOOL			gb_NT_UserGlobalCall;			// 是否使用Global Call
extern CString		gb_NT_Protocol;					// 信令方式
extern BOOL			gb_NT_IncludeD41;				// 是否包括D41卡
extern BOOL			gb_NT_CaptureABSignal;			// 是否分析AB信令
extern UINT			gb_NT_ABS_Disconnect;			// AB信令 - Disconnect

extern UL			glngInvokeID;

// Sun added 2006-10-01
extern CFaxManager	gb_FaxManager;


#ifdef CISCO_CCM

extern HLINEAPP		gb_hLineApp;					// TAPI句柄
extern DWORD		gb_dwgTAPIVersion;				// TAPI版本
extern UL			gb_lMngrThread;					// To post event messages to central manager
extern HANDLE		gb_hLineEvent;					// TAPI事件句柄

// Sun added 2010-08-10，会议磋商失败处理方式
/// -1 - 挂断；1 - 继续排队；2 - 做无应答处理；3 - 尝试单步转接
extern int			gb_nSetupConferenceFailedMethod;

// Sun added 2010-07-31，CTI路由点最大并发呼叫数
extern int			gb_nRoutePointMaxConcurrentCall;

#endif

extern DWORD		gb_lngRoutePointCheckTimer;		// Sun added 2008-05-29, RoutePoint设备空闲自检周期（秒）

#define GET_INVOKE_ID								(++glngInvokeID) % 0x999999 + 1

// Sun added 2011-04-16
// 自动开关（位操作）
extern UL			gb_lngAutoCloseFlag;			// 默认值0x00FF

// Sun added 2011-04-25
extern int			gb_nMW_TransFail_Calls;			// 统计转接失败率的最近请求转接电话个数，默认20，取值范围[5..200]
extern int			gb_nTH_MaxTransFailRate;		// 允许的转接失败率最大值（百分比），默认30，取值范围[1..100]

// Sun added 2013-09-09
extern int			gb_nMW_CreateOnHookThread;		// 统计连续创建挂机线程失败的次数，默认0，取值范围[0..3]

long sig_hdlr( unsigned long );

//------------------------------------------------------------------------
#endif  //End #ifndef _GLOBAL_H
//------------------------------------------------------------------------
