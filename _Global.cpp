///////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		_Global.cpp
// 用途描述：	全局变量及函数定义
// 作者：		Tony Sun
// 创建时间：	2000/05/05
// 最后修改时间：2006/10/01
// 版权信息：	Copyright (C) 1999 - 2007 PicaSoft
// 备注：
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
#include "stdafx.h"

#include "_Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//----------------------------------------------------------------------------------------------
SIVR_Group	PGroupInfo[MAX_IVR_GROUP];		// 信道组信息
CString		PChannelDN[MAX_IVR_CHANNEL];	// 信道信息

CIVRGroup	*PIVRGroup[MAX_IVR_GROUP];		// 信道组指针
CIVRChannel	*PIVRCH[MAX_IVR_CHANNEL];		// 信道指针

CString		gb_FN_VoxPath;					// 系统语音目录
CString		gb_FN_RecPath;					// 系统留言目录
CString		gb_FN_TempPath;					// 系统临时目录

int			gb_VoiceVolume;					// 通道声音大小  -10 -- +10
int			gb_RingsPreOffhook;				// 摘机前振铃次数

// Sun added 2009-06-27
BOOL		g_blnAnalogAsynDivert;			// 是否进行异步转接

// Sun added 2003-04-28
CString		gb_NT_Protocol;					// 信令方式
BOOL		gb_NT_UserGlobalCall;			// 是否使用Global Call
BOOL		gb_NT_IncludeD41;				// 是否包括D41卡
BOOL		gb_NT_CaptureABSignal;			// 是否分析AB信令
UINT		gb_NT_ABS_Disconnect;			// AB信令 - Disconnect

UL			glngInvokeID;

// Sun added 2006-10-01
CFaxManager	gb_FaxManager;


#ifdef CISCO_CCM

HLINEAPP	gb_hLineApp;					// TAPI句柄
DWORD		gb_dwgTAPIVersion;				// TAPI版本
UL			gb_lMngrThread;					// To post event messages to central manager
HANDLE		gb_hLineEvent;					// TAPI事件句柄

// Sun added 2010-08-10，会议磋商失败处理方式
/// -1 - 挂断；1 - 继续排队；2 - 做无应答处理；3 - 尝试单步转接
int			gb_nSetupConferenceFailedMethod;

// Sun added 2010-07-31，CTI路由点最大并发呼叫数
int			gb_nRoutePointMaxConcurrentCall;

#endif

DWORD		gb_lngRoutePointCheckTimer;		// Sun added 2008-05-29, RoutePoint设备空闲自检周期（秒）

// Sun added 2011-04-16
// 自动开关（位操作）
UL			gb_lngAutoCloseFlag;			// 默认值0x00FF

// Sun added 2011-04-25
int			gb_nMW_TransFail_Calls;			// 统计转接失败率的最近请求转接电话个数，默认20，取值范围[5..200]
int			gb_nTH_MaxTransFailRate;		// 允许的转接失败率最大值（百分比），默认30，取值范围[1..100]

// Sun added 2013-09-09
int			gb_nMW_CreateOnHookThread;		// 统计连续创建挂机线程失败的次数，默认0，取值范围[0..3]

//----------------------------------------------------------------------------------------------

// Sun added 2006-02-20
BOOL		gb_Enable2DiscDTone;			// 允许第二双音频挂断音
BOOL		gb_Enable2DiscSTone;			// 允许第二单音频挂断音

//----------------------------------------------------------------------------------------------
// 交换机特征音
STone_T				gb_Tone[MAX_CHANNEL_TONES] = 
	{
		{{"TID_DIAL_LCL"},   {TID_DIAL_LCL},   {400,125},  {400,125}, {  0,  0}, {  0,  0}, 0},
		{{"TID_DIAL_INTL"},  {TID_DIAL_INTL},  {402,125},  {402,125}, {  0,  0}, {  0,  0}, 0},
		{{"TID_DIAL_XTRA"},  {TID_DIAL_XTRA},  {401,125},  {401,125}, {  0,  0}, {  0,  0}, 0},
		{{"TID_BUSY1"},      {TID_BUSY1},      {400, 24},  {400, 24}, { 39, 16}, { 39, 16}, 3},
		{{"TID_BUSY2"},      {TID_BUSY2},      {400, 24},  {400, 24}, { 53, 30}, { 53, 30}, 1},
		{{"TID_RNGBK1"},	 {TID_RNGBK1},     {450,150},  {  0,  0}, {130,125}, {580,485}, 0},
		{{"TID_RNGBK2"},	 {TID_RNGBK2},     {450,150},  {450,150}, {130,125}, {580,485}, 0},
		{{"TID_FAX1"},       {TID_FAX1},       {2150,150}, {  0,  0}, { 25,-25}, {  0,  0}, 0},
		{{"TID_FAX2"},       {TID_FAX2},       {1100, 50}, {  0,  0}, { 25,-25}, {  0,  0}, 0},
		{{"TID_DISCON_DT"},  {TID_DISCON_DT},  {450, 32},  {450, 32}, { 32,  2}, { 32,  2}, 1},
		{{"TID_DISCON_ST"},  {TID_DISCON_ST},  {450, 20},  {38,  5},  { 36,  5}, {  1,  0}, 0},
		{{"TID_DISCON_DT2"}, {TID_DISCON_DT2}, {450, 32},  {450, 32}, { 32,  2}, { 32,  2}, 1},
		{{"TID_DISCON_ST2"}, {TID_DISCON_ST2}, {450, 40},  { 50,  8}, { 50,  8}, {  1,  0}, 0}
	};
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
