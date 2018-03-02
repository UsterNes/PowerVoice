////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		_Protocol_RECAGT.h
// 用途描述：	录音客户端通信协议
// 作者：		SUN
// 创建时间：	1999/12/01
// 最后修改时间：2006/08/08
// 版权信息：	Copyright (C) 1999 - 2006 PCSG
// 备注：
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_RECORDAGENT_H__
#define __PROTOCOL_RECORDAGENT_H__
//------------------------------------------------------------------------

#ifndef MAX_WAVE_COMPRESS_RATE
#define MAX_WAVE_COMPRESS_RATE		16
#endif

#ifndef SIZE_PER_AUDIO_PACKAGE
#define SIZE_PER_AUDIO_PACKAGE		160
#endif

#ifndef SIZE_AUDIO_FRAME
#define SIZE_AUDIO_FRAME			3200
#endif

#ifndef SIZE_AUDIO_PACKED
#define SIZE_AUDIO_PACKED			200
#endif

// Updated 2006-08-08, from 800 to 8192
#ifndef SIZE_FILE_PACKED
//#define SIZE_FILE_PACKED			800
#define SIZE_FILE_PACKED			800
#endif

// Add 2006-08-08
#ifndef SIZE_MAX_AUDIO_PACKAGE
#define SIZE_MAX_AUDIO_PACKAGE		800
#endif

//--------------------------
// 录音客户端注册 - Total 4
//--------------------------

//--------------------------
// 注册 - 2
//------------
#define TCP_AGT2REC_REGISTER				1			// (0x0001) 注册消息
struct S_TCP_AGT2REC_REGISTER
{
	char	UserName[20];					// 用户名
	char	strPassword[20];				// 用户口令
	char	strAliasName[50];				// 登录别名
	UL		AgentID;						// PowerCall坐席编号，CTI控制时必须提供
	UC		Version;						// 版本号
	UC		RegCode[PCS_DEF_NUMLEN_APP];	// 注册码
};
//------------

//------------
#define TCP_REC2AGT_REGISTER_ACK			1001		// (0x03E9) 注册响应消息
struct S_TCP_REC2AGT_REGISTER_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_BEUSED
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_VERSION
											// PCS_DEF_RETCODE_REGCODE
											// PCS_DEF_RETCODE_PASSWORD
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_NOLICENSE
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UC		bCTIEnabled;					// 是否使用CTI控制
	UC		nMaxSilenceTime;				// 最大静默时间
	UC		nCTIMaxSilenceTime;				// CTI最大静默时间
	char	strDisplayName[50];				// 用户显示名字
	char	strRights[50];					// 权限
	char	strFieldName1[50];				// 附加数据字段名1
	char	strFieldName2[50];				// 附加数据字段名2
	char	strFieldName3[50];				// 附加数据字段名3
	char	strFieldName4[50];				// 附加数据字段名4
	char	strFieldName5[50];				// 附加数据字段名5
};
//------------
//--------------------------

//--------------------------
// 取消注册 - 2
//------------
#define TCP_AGT2REC_UNREGISTER				2			// (0x0002) 取消注册消息
//------------

//------------
#define TCP_REC2AGT_UNREGISTER_ACK			1002		// (0x03EA) 取消注册响应消息
//------------
//--------------------------


//--------------------------
// 录音客户参数设定 - Total 30
//--------------------------
//------------
#define TCP_AGT2REC_CHG_PASSWORD			11			// (0x000B) 客户端修改口令
struct S_TCP_AGT2REC_CHG_PASSWORD
{
	char	strPassword[20];				// 新口令
};

//------------
#define TCP_REC2AGT_CHG_PASSWORD_ACK		1011		// (0x03F3) 修改口令响应消息
struct S_TCP_REC2AGT_CHG_PASSWORD_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_CHG_PARAMETER			12			// (0x000C) 客户端修改个人参数
struct S_TCP_AGT2REC_CHG_PARAMETER
{
	char	strParamName[50];				// 参数名
	char	strParamValue[100];				// 参数值
};
//------------

//------------
#define TCP_REC2AGT_CHG_PARAMETER_ACK		1012		// (0x03F4) 客户端修改个人参数响应消息
struct S_TCP_REC2AGT_CHG_PARAMETER_ACK
{
	char	strParamName[50];				// 参数名
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_CHG_SYSPARAM			13			// (0x000D) 管理员端修改系统参数(必须是管理员客户端)
struct S_TCP_AGT2REC_CHG_SYSPARAM
{
	char	strParamName[50];				// 参数名
	char	strParamValue[100];				// 参数值
};
//------------

//------------
#define TCP_REC2AGT_CHG_SYSPARAM_ACK		1013		// (0x03F5) 管理员端修改系统参数响应消息
struct S_TCP_REC2AGT_CHG_SYSPARAM_ACK
{
	char	strParamName[50];				// 参数名
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_ADD_AGENT				14			// (0x000E) 新增座席消息(必须是管理员客户端)
struct S_TCP_AGT2REC_ADD_AGENT
{
	char	UserName[20];					// 用户名
};
//------------

//------------
#define TCP_REC2AGT_ADD_AGENT_ACK			1014		// (0x03F6) 新增座席响应消息
struct S_TCP_REC2AGT_ADD_AGENT_ACK
{
	char	UserName[20];					// 用户名
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_BEUSED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_DEL_AGENT				15			// (0x000F) 删除座席消息(必须是管理员客户端)
struct S_TCP_AGT2REC_DEL_AGENT
{
	char	UserName[20];					// 用户名
};
//------------

//------------
#define TCP_REC2AGT_DEL_AGENT_ACK			1015		// (0x03F7) 删除座席响应消息
struct S_TCP_REC2AGT_DEL_AGENT_ACK
{
	char	UserName[20];					// 用户名
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_MOD_AGENT				16			// (0x0010) 修改座席消息(必须是管理员客户端)
struct S_TCP_AGT2REC_MOD_AGENT
{
	char	UserName[20];					// 用户名
};
//------------

//------------
#define TCP_REC2AGT_MOD_AGENT_ACK			1016		// (0x03F8) 修改座席响应消息
struct S_TCP_REC2AGT_MOD_AGENT_ACK
{
	char	UserName[20];					// 用户名
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_RELOAD_AGENTLIST		17			// (0x0011) 重新加载座席表(必须是管理员客户端)
//------------

//------------
#define TCP_REC2AGT_RELOAD_AGENTLIST_ACK	1017		// (0x03F9) 重新加载座席表响应消息
struct S_TCP_REC2AGT_RELOAD_AGENT_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_RELOAD_SYSPARAM			18			// (0x0018) 重新加载系统参数表(必须是管理员客户端)
//------------

//------------
#define TCP_REC2AGT_RELOAD_SYSPARAM_ACK		1018		// (0x03FA) 重新加载系统参数表响应消息
struct S_TCP_REC2AGT_RELOAD_SYSPARAM_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_UPDATE_GROUPINFO		19			// (0x0019) 更新组信息(必须是管理员客户端)
struct S_TCP_AGT2REC_UPDATE_GROUPINFO
{
	US		GroupNo;						// 组编号
};
//------------

//------------
#define TCP_REC2AGT_UPDATE_GROUPINFO_ACK	1019		// (0x03FB) 更新组信息响应消息
struct S_TCP_REC2AGT_UPDATE_GROUPINFO_ACK
{
	US		GroupNo;						// 组编号
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_ADD_ACCESSPOINT			20			// (0x0014) 增加接入点消息(必须是管理员客户端)
struct S_TCP_AGT2REC_ADD_ACCESSPOINT
{
	UC		AccessPointID;					// 接入点ID, 0 Based
};
//------------

//------------
#define TCP_REC2AGT_ADD_ACCESSPOINT_ACK		1020		// (0x03FC) 增加接入点响应消息
struct S_TCP_REC2AGT_ADD_ACCESSPOINT_ACK
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_LACK_PARAM
											// PCS_DEF_RETCODE_REJECTED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_DEL_ACCESSPOINT			21			// (0x0015) 删除接入点消息(必须是管理员客户端)
struct S_TCP_AGT2REC_DEL_ACCESSPOINT
{
	UC		AccessPointID;					// 接入点ID, 0 Based
};
//------------

//------------
#define TCP_REC2AGT_DEL_ACCESSPOINT_ACK		1021		// (0x03FD) 删除接入点响应消息
struct S_TCP_REC2AGT_DEL_ACCESSPOINT_ACK
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_LACK_PARAM
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_MOD_ACCESSPOINT			22			// (0x0016) 修改接入点消息(必须是管理员客户端)
struct S_TCP_AGT2REC_MOD_ACCESSPOINT
{
	UC		AccessPointID;					// 接入点ID, 0 Based
};
//------------

//------------
#define TCP_REC2AGT_MOD_ACCESSPOINT_ACK		1022		// (0x03FE) 修改接入点响应消息
struct S_TCP_REC2AGT_MOD_ACCESSPOINT_ACK
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_LACK_PARAM
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_RELOAD_APTPARAMLIST		30			// (0x001E) 重新加载接入点参数表(必须是管理员客户端)
struct S_TCP_AGT2REC_RELOAD_APTPARAMLIST
{
	UC		AccessPointID;					// 接入点ID, 0 Based
};
//------------

//------------
#define TCP_REC2AGT_RELOAD_APTPARAMLIST_ACK 1030		// (0x0406) 重新加载接入点参数表响应消息
struct S_TCP_REC2AGT_RELOAD_APTPARAMLIST_ACK
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_CHG_APTPARAM			31			// (0x001F) 修改接入点参数(必须是管理员客户端)
struct S_TCP_AGT2REC_CHG_APTPARAM
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	char	strParamName[50];				// 参数名
	char	strParamValue[100];				// 参数值
};
//------------

//------------
#define TCP_REC2AGT_CHG_APTPARAM_ACK		1031		// (0x0407) 修改接入点参数响应消息
struct S_TCP_REC2AGT_CHG_APTPARAM_ACK
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	char	strParamName[50];				// 参数名
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_LACK_PARAM
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_CHG_APTCHPARAM			32			// (0x0020) 修改接入点通道参数(必须是管理员客户端)
struct S_TCP_AGT2REC_CHG_APTCHPARAM
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	US		ChannelIndex;					// 录音通道编号，1 Based
	char	strParamName[50];				// 参数名
	char	strParamValue[100];				// 参数值
};
//------------

//------------
#define TCP_REC2AGT_CHG_APTCHPARAM_ACK		1032		// (0x0408) 修改接入点通道参数响应消息
struct S_TCP_REC2AGT_CHG_APTCHPARAM_ACK
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	US		ChannelIndex;					// 录音通道编号，1 Based
	char	strParamName[50];				// 参数名
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_LACK_PARAM
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_RELOAD_APTCHPARAMLIST		33			// (0x0021) 重新加载接入点通道参数表(必须是管理员客户端)
struct S_TCP_AGT2REC_RELOAD_APTCHPARAMLIST
{
	UC		AccessPointID;					// 接入点ID, 0 Based
};
//------------

//------------
#define TCP_REC2AGT_RELOAD_APTCHPARAMLIST_ACK 1033		// (0x0409) 重新加载接入点通道参数表响应消息
struct S_TCP_REC2AGT_RELOAD_APTCHPARAMLIST_ACK
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_RELOAD_APLIST		34			// (0x0022) 重新加载接入点(必须是管理员客户端)
/*struct S_TCP_AGT2REC_RELOAD_APIST
{
};*/
//------------

//------------
#define TCP_REC2AGT_RELOAD_APLIST_ACK 1034		// (0x0410) 重新加载接入点响应消息
struct S_TCP_REC2AGT_RELOAD_APLIST_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------


//--------------------------
// 录音控制消息 - Total 18
//--------------------------
//------------
#define TCP_AGT2REC_STARTREC				51			// (0x0033) 客户端开始录音消息
struct S_TCP_AGT2REC_STARTREC
{
	UC		RecordStartType;				// 录音启动控制方式，参见“录音控制方式”
	UC		SamplesPerSec;					// 采样频率(见常数定义)
	UC		BitsPerSample;					// 采样比特(8 or 16)
	UC		Channels;						// 通道数(1 or 2)
};
//------------

//------------
#define TCP_AGT2REC_STARTREC_MON			52			// (0x0034) 班长强制开始录音消息(必须是管理员客户端)
struct S_TCP_AGT2REC_STARTREC_MON
{
	char	UserName[20];					// 用户名
	char	UserData[200];					// 用户数据
};
//------------

//------------
#define TCP_REC2AGT_STARTREC_ACK			1051		// (0x041B) 客户端开始录音响应消息
struct S_TCP_REC2AGT_STARTREC_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_REC2AGT_STARTREC_CMD			1052		// (0x041C) 服务器命令客户端开始录音(CTI控制、班长控制)
struct S_TCP_REC2AGT_STARTREC_CMD
{
	UC		RecordStartType;				// 录音启动控制方式，参见“录音控制方式”
	UC		CallDirection;					// 通话方向，CTI控制时使用，参见“呼叫类型”
	UL		AgentID;						// PowerCall坐席编号，CTI控制时使用
	UL		CallRefID;						// 呼叫标示，CTI控制时使用
	char	TelNo[PCS_DEF_NUMLEN_APP];		// 电话号码，CTI控制时使用
	char	CallInTime[15];					// CTI提供的呼叫时间（TimeStamp），CTI控制时使用
	char	MonitorName[20];				// 班长名，班长控制时使用
	char	UserData[200];					// 用户数据
};
//------------

//------------
#define TCP_REC2AGT_STARTREC_MON_ACK		1053			// (0x041D) 班长强制开始录音响应消息
struct S_TCP_REC2AGT_STARTREC_MON_ACK
{
	char	UserName[20];					// 用户名
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------


//------------
#define TCP_AGT2REC_STOPREC					61			// (0x003D) 客户端停止录音消息
struct S_TCP_AGT2REC_STOPREC
{
	UC		RecordStopType;					// 录音停止控制方式，参见“录音控制方式”
	char	UserData[200];					// 用户数据
};
//------------

//------------
#define TCP_AGT2REC_STOPREC_MON				62			// (0x003E) 班长强制停止录音消息(必须是管理员客户端)
struct S_TCP_AGT2REC_STOPREC_MON
{
	char	UserName[20];					// 用户名
};
//------------

//------------
#define TCP_AGT2REC_APPEND_USERDATA			63			// (0x003F) 追加用户数据到最近一次成功录音
struct S_TCP_AGT2REC_APPEND_USERDATA
{
	char	UserData[200];					// 用户数据
};
//------------

//------------
#define TCP_AGT2REC_ATTACH_RECORDDATA		64			// (0x0040) 录音附加数据消息
struct S_TCP_AGT2REC_ATTACH_RECORDDATA
{
	char	AttachData1[16];				// 附加数据项1
	char	AttachData2[16];				// 附加数据项2
	char	AttachData3[32];				// 附加数据项3
	char	AttachData4[32];				// 附加数据项4
	char	AttachData5[256];				// 附加数据项5
};
//------------

//------------
#define TCP_AGT2REC_SET_SPECIALMARK			65			// (0x0041) 录音特别标记消息
struct S_TCP_AGT2REC_SET_SPECIALMARK
{
	UC		SpecialMark;					// 特别标记
};
//------------

//------------
#define TCP_AGT2REC_VOICEDEVICE_STATUS		66			// (0x0042) 语音设备状态报告消息
struct S_TCP_AGT2REC_VOICEDEVICE_STATUS
{
	UC		VoiceDeviceStatus;				// 语音设备状态，参见开关常量定义
};
//------------

//------------
#define TCP_REC2AGT_STOPREC_ACK				1061		// (0x0425) 客户端停止录音响应消息
struct S_TCP_REC2AGT_STOPREC_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		RecordDuration;					// 录音时长
	char	FilePath[128];					// 录音文件全路径
};
//------------

//------------
#define TCP_REC2AGT_STOPREC_CMD				1062		// (0x0426) 服务器命令客户端停止录音(CTI控制、班长控制)
struct S_TCP_REC2AGT_STOPREC_CMD
{
	UC		RecordStopType;					// 录音停止控制方式，参见“录音控制方式”
	char	MonitorName[20];				// 班长名，班长控制时使用
};
//------------

//------------
#define TCP_REC2AGT_STOPREC_MON_ACK			1063			// (0x0427) 班长强制停止录音响应消息
struct S_TCP_REC2AGT_STOPREC_MON_ACK
{
	char	UserName[20];					// 用户名
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_REC2AGT_USER_ATTACHDATA			1064			// (0x0428) 用户附加录音数据消息
struct S_TCP_REC2AGT_USER_ATTACHDATA
{
	char	UserName[20];					// 被监视用户名
	char	AttachData1[16];				// 附加数据项1
	char	AttachData2[16];				// 附加数据项2
	char	AttachData3[32];				// 附加数据项3
	char	AttachData4[32];				// 附加数据项4
	char	AttachData5[256];				// 附加数据项5
};
//------------

//------------
#define TCP_REC2AGT_SET_SPECIALMARK			1065			// (0x0428) 用户设置特别标记消息
struct S_TCP_REC2AGT_SET_SPECIALMARK
{
	char	UserName[20];					// 被监视用户名
	UC		SpecialMark;					// 特别标记
};
//------------

//------------
#define TCP_REC2AGT_USER_CTICALLINFO		1066			// (0x0429) 被监控用户CTI呼叫数据消息
struct S_TCP_REC2AGT_USER_CTICALLINFO
{
	char	UserName[20];					// 被监视用户名
	UC		CallDirection;					// 通话方向，参见“呼叫类型”
	UL		AgentID;						// PowerCall坐席编号
	UL		CallRefID;						// 呼叫标示
	char	TelNo[PCS_DEF_NUMLEN_APP];		// 电话号码
};
//------------

//------------
#define TCP_REC2AGT_APTCH_CTICALLINFO		1067			// (0x042A) 接入点通道CTI呼叫数据消息
struct S_TCP_REC2AGT_APTCH_CTICALLINFO
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	US		ChannelIndex;					// 录音通道编号，1 Based
	UC		CallDirection;					// 通话方向，参见“呼叫类型”
	UL		AgentID;						// PowerCall坐席编号
	UL		CallRefID;						// 呼叫标示
	char	CallInTime[15];					// 电话呼入/呼出时间
	char	TelNo[PCS_DEF_NUMLEN_APP];		// 电话号码
};
//------------
//--------------------------


//--------------------------
// 监控、监听控制消息 - Total 10
//--------------------------
//------------
#define TCP_AGT2REC_ADDMONITOR				71			// (0x0047) 管理员添加监控对象(必须是管理员客户端)
struct S_TCP_AGT2REC_ADDMONITOR
{
	char	UserName[20];					// 被监控用户名
};
//------------

//------------
#define TCP_AGT2REC_REMOVEMONITOR			72			// (0x0048) 管理员减少监控对象(必须是管理员客户端)
struct S_TCP_AGT2REC_REMOVEMONITOR
{
	char	UserName[20];					// 用户名
};
//------------

//------------
#define TCP_AGT2REC_START_LSN				73			// (0x0049) 管理员开始监听(必须是管理员客户端)
struct S_TCP_AGT2REC_START_LSN
{
	char	UserName[20];					// 被监听用户名
};
//------------

//------------
#define TCP_AGT2REC_STOP_LSN				74			// (0x004A) 管理员停止监听(必须是管理员客户端)
struct S_TCP_AGT2REC_STOP_LSN
{
	char	UserName[20];					// 用户名
};
//------------

//------------
#define TCP_REC2AGT_ADDMONITOR_ACK			1071		// (0x042F) 管理员增加监控响应消息
struct S_TCP_REC2AGT_ADDMONITOR_ACK
{
	char	UserName[20];					// 被监控用户名
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UC		LoginStatus;					// 被监控用户登录状态
	UC		RecordStatus;					// 被监控用户录音状态
	UC		ListenStatus;					// 被监控用户当前受控状态
	UC		MonitorStatus;					// 被监控用户的班长状态
	UC		VoiceDeviceStatus;				// 被监控用户录音设备是否正常
};
//------------

//------------
#define TCP_REC2AGT_REMOVEMONITOR_ACK		1072		// (0x0430) 管理员减少监控响应消息
struct S_TCP_REC2AGT_REMOVEMONITOR_ACK
{
	char	UserName[20];					// 被监控用户名
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_REC2AGT_START_LSN_ACK			1073		// (0x0431) 启动监听应答消息
struct S_TCP_REC2AGT_START_LSN_ACK
{
	char	UserName[20];					// 被监听用户名
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_REC2AGT_STOP_LSN_ACK			1074		// (0x0432) 停止监听应答消息
struct S_TCP_REC2AGT_STOP_LSN_ACK
{
	char	UserName[20];					// 用户名
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_REC2AGT_LSN_STARTED				1075		// (0x0433) 监听开始消息
struct S_TCP_REC2AGT_LSN_STARTED
{
	char	UserName[20];					// 被监听用户名
	UC		RecordStartType;				// 录音启动控制方式，参见“录音控制方式”
	UC		SamplesPerSec;					// 采样频率(见常数定义)
	UC		BitsPerSample;					// 采样比特(8 or 16)
	UC		Channels;						// 通道数(1 or 2)
};
//------------

//------------
#define TCP_REC2AGT_LSN_STOPED				1076		// (0x0434) 监听停止消息
struct S_TCP_REC2AGT_LSN_STOPED
{
	char	UserName[20];					// 被监听用户名
	UC		RecordStartType;				// 录音启动控制方式，参见“录音控制方式”
};
//------------
//--------------------------


//--------------------------
// 文件查询消息 - Total 6
//--------------------------
//------------
#define TCP_AGT2REC_DOWNLOAD_FILE			101			// (0x0065) 请求服务器下传录音文件消息
struct S_TCP_AGT2REC_DOWNLOAD_FILE
{
	char	FilePath[128];					// 录音文件全路径
};
//------------

//------------
#define TCP_AGT2REC_DOWNLOAD_CANCEL			102			// (0x0066) 取消下传录音文件消息
//------------

//------------
#define TCP_REC2AGT_DOWNLOAD_FILE_ACK		1101		// (0x044D) 请求服务器下传录音文件响应消息
struct S_TCP_REC2AGT_DOWNLOAD_FILE_ACK
{
	char	FilePath[128];					// 录音文件全路径
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_REC2AGT_DOWNLOAD_CANCEL_ACK		1102			// (0x044E) 取消下传录音文件响应消息
struct S_TCP_REC2AGT_DOWNLOAD_CANCEL_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_REC2AGT_DOWNLOAD_STARTED		1103			// (0x044F) 文件传输开始消息
struct S_TCP_REC2AGT_DOWNLOAD_STARTED
{
	UDL		FileSize;						// 录音文件大小(字节数)
	char	FilePath[128];					// 录音文件全路径
};
//------------

//------------
#define TCP_REC2AGT_DOWNLOAD_FINISHED		1104			// (0x0450) 文件传输完成消息
struct S_TCP_REC2AGT_DOWNLOAD_FINISHED
{
	char	FilePath[128];					// 录音文件全路径
	UC		HasMoreFiles;					// 是否还有后续文件需要传输，参见“开关常量定义”
};
//------------
//--------------------------


//--------------------------
// 接入点监控消息 - Total 16
//--------------------------
//------------
#define TCP_AGT2REC_APT_ADDMONITOR			121			// (0x0079) 管理员添加接入点监控对象(必须是管理员客户端)
struct S_TCP_AGT2REC_APT_ADDMONITOR
{
	UC		AccessPointID;					// 接入点ID, 0 Based
};
//------------

//------------
#define TCP_AGT2REC_APT_REMOVEMONITOR		122			// (0x007A) 管理员减少接入点监控对象(必须是管理员客户端)
struct S_TCP_AGT2REC_APT_REMOVEMONITOR
{
	UC		AccessPointID;					// 接入点ID, 0 Based
};
//------------

//------------
#define TCP_AGT2REC_APTCH_STARTREC_MON		125			// (0x007D) 班长强制接入点通道开始录音消息(必须是管理员客户端)
struct S_TCP_AGT2REC_APTCH_STARTREC_MON
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	US		ChannelIndex;					// 录音通道编号，1 Based
	char	UserData[200];					// 用户数据
};
//------------

//------------
#define TCP_AGT2REC_APTCH_STOPREC_MON		126			// (0x007E) 班长强制接入点通道停止录音消息(必须是管理员客户端)
struct S_TCP_AGT2REC_APTCH_STOPREC_MON
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	US		ChannelIndex;					// 录音通道编号，1 Based
};
//------------

//------------
#define TCP_AGT2REC_APTCH_START_LSN			127			// (0x007F) 管理员开始监听接入点通道(必须是管理员客户端)
struct S_TCP_AGT2REC_APTCH_START_LSN
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	US		ChannelIndex;					// 录音通道编号，1 Based
};
//------------

//------------
#define TCP_AGT2REC_APTCH_STOP_LSN			128			// (0x0080) 管理员停止监听接入点通道(必须是管理员客户端)
struct S_TCP_AGT2REC_APTCH_STOP_LSN
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	US		ChannelIndex;					// 录音通道编号，1 Based
};
//------------

//------------
#define TCP_REC2AGT_APT_ADDMONITOR_ACK		1121		// (0x0461) 管理员增加接入点监控响应消息
struct S_TCP_REC2AGT_APT_ADDMONITOR_ACK
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UC		LoginStatus;					// 被监控通道登录状态
	US		nChannelCount;					// 通道数量
};
//------------

//------------
#define TCP_REC2AGT_APT_REMOVEMONITOR_ACK	1122		// (0x0462) 管理员减少接入点监控响应消息
struct S_TCP_REC2AGT_APT_REMOVEMONITOR_ACK
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_REC2AGT_APTCH_MONITOR_ACK		1123		// (0x0463) 接入点通道状态初始报告（管理员增加接入点监控后服务器报告各通道状态）
struct S_TCP_REC2AGT_APTCH_MONITOR_ACK
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	US		ChannelIndex;					// 录音通道编号，1 Based
	char	UserName[20];					// 被监视用户名
	UC		LoginStatus;					// 被监控通道登录状态
	UC		RecordStatus;					// 被监控通道录音状态
	UC		ListenStatus;					// 被监控通道当前受控状态
	UC		MonitorStatus;					// 被监控用户的班长状态
	UC		VoiceDeviceStatus;				// 被监控通道是否正常
};
//------------

//------------
#define TCP_REC2AGT_APTCH_STARTREC_MON_ACK	1125		// (0x0465) 班长强制接入点通道开始录音响应消息
struct S_TCP_REC2AGT_APTCH_STARTREC_MON_ACK
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	US		ChannelIndex;					// 录音通道编号，1 Based
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_REC2AGT_APTCH_STOPREC_MON_ACK	1126		// (0x0466) 班长强制接入点通道停止录音响应消息
struct S_TCP_REC2AGT_APTCH_STOPREC_MON_ACK
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	US		ChannelIndex;					// 录音通道编号，1 Based
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_REC2AGT_APTCH_START_LSN_ACK		1127		// (0x0467) 启动接入点通道监听应答消息
// Replaced 2006-08-08
/// From
/*
struct S_TCP_REC2AGT_APTCH_START_LSN_ACK
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	US		ChannelIndex;					// 录音通道编号，1 Based
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
*/
/// To
struct S_TCP_REC2AGT_APTCH_START_LSN_ACK
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	US		ChannelIndex;					// 录音通道编号，1 Based
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UC		FormatTag;						// 语音采集方式ID（比如，1：G.729；2：ADPCM；3： MP3等等）
	char	FilePath[128];					// 录音文件全路径
	UC		HaveMoreFormatDescription;		// 是否还有余下的格式描述串
	US		FormatBufferLen;				// 格式描述长度
	BYTE	FormatDescription[PCS_DEF_FORMAT_DESC_LEN];
};
//------------

//------------
#define TCP_REC2AGT_APTCH_STOP_LSN_ACK		1128		// (0x0468) 停止接入点通道监听应答消息
struct S_TCP_REC2AGT_APTCH_STOP_LSN_ACK
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	US		ChannelIndex;					// 录音通道编号，1 Based
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_REC2AGT_APTCH_LSN_STARTED		1129		// (0x0469) 监听接入点通道开始消息
struct S_TCP_REC2AGT_APTCH_LSN_STARTED
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	US		ChannelIndex;					// 录音通道编号，1 Based
	UC		RecordStartType;				// 录音启动控制方式，参见“录音控制方式”
	UC		SamplesPerSec;					// 采样频率(见常数定义)
	UC		BitsPerSample;					// 采样比特(8 or 16)
	UC		Channels;						// 通道数(1 or 2)
};
//------------

//------------
#define TCP_REC2AGT_APTCH_LSN_STOPED		1130		// (0x046A) 监听接入点通道停止消息
struct S_TCP_REC2AGT_APTCH_LSN_STOPED
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	US		ChannelIndex;					// 录音通道编号，1 Based
	UC		RecordStartType;				// 录音启动控制方式，参见“录音控制方式”
};
//------------

//------------
// Added 2006-08-08
#define TCP_REC2AGT_FORMATDESC_CONT			1131		// (0x046B) 后续格式描述串
struct S_TCP_REC2AGT_FORMATDESC_CONT
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	US		ChannelIndex;					// 录音通道编号，1 Based
	UC		Index;							// 包顺序，1 Based
	UC		HaveMoreFormatDescription;		// 是否还有余下的格式描述串
	US		FormatBufferLen;				// 格式描述长度
	BYTE	FormatDescription[PCS_DEF_FORMAT_DESC_LEN];
};
//------------
//--------------------------

//--------------------------
// 录音索引传递消息 - Total 4
//--------------------------
//------------
#define TCP_AGT2REC_REQ_RECORDINDEX_UPDATE				201		// (0x00C9) 立即同步索引消息
struct S_TCP_AGT2REC_REQ_RECORDINDEX_UPDATE
{
	UC		AccessPointID;			// 接入点ID, 0 Based.
};
//------------

//------------
#define TCP_REC2AGT_END_RECORDINDEX_UPDATE				202		// (0x00CA) 同步索引完成消息
struct S_TCP_REC2AGT_END_RECORDINDEX_UPDATE
{
	UC		AccessPointID;				// 接入点ID, 0 Based.
	UC		ErrorCode;						// 返回码，参见“返回码定义”
																// PCS_DEF_RETCODE_SUCC
																// PCS_DEF_RETCODE_USERINTERRUPT
																// PCS_DEF_RETCODE_OTHERS
																// PCS_DEF_RETCODE_SYSERROR
	UL		RecordCount;					// 实际同步索引条数
};
//------------

//------------
#define TCP_REC2AGT_ON_RECORDINDEX_UPDATE				203		// (0x00CB) 索引同步进度消息
struct S_TCP_REC2AGT_ON_RECORDINDEX_UPDATE
{
	UC		AccessPointID;				// 接入点ID, 0 Based.
	UL		RecordCount;					// 已同步索引条数
};
//------------

//------------
#define TCP_REC2AGT_REQ_RECORDINDEX_UPDATE_ACK				1201		// (0x04B1) 立即同步索引消息返回消息
struct S_TCP_REC2AGT_REQ_RECORDINDEX_UPDATE_ACK
{
	UC		AccessPointID;				// 接入点ID, 0 Based.
	UC		ErrorCode;						// 返回码，参见“返回码定义”
													// PCS_DEF_RETCODE_SUCC
													// PCS_DEF_RETCODE_NOTEXIST
													// PCS_DEF_RETCODE_OTHERS
													// PCS_DEF_RETCODE_SYSERROR
	UL		RecordCount;					// 需同步索引条数
};
//------------
//--------------------------


//--------------------------
// 数据流消息 - Total 3
//--------------------------
//------------
#define TCP_AGT2REC_REC_DATAFLOW			901			// (0x0385) 录音数据流
// Replaced 2006-08-08
/// From
/*
struct S_TCP_AGT2REC_REC_DATAFLOW
{
	UL		PackageIndex;					// 包编号
	UC		RecordData[SIZE_AUDIO_PACKED];	// 数据
};
*/
/// To
struct S_TCP_AGT2REC_REC_DATAFLOW
{
	UL		PackageIndex;					// 包编号
	US		DataLength;						// 数据长度
	UC		RecordData[SIZE_MAX_AUDIO_PACKAGE];	// 数据
};
//------------

//------------
#define TCP_REC2AGT_LSN_DATAFLOW			1901		// (0x076D) 监听数据流
// Replaced 2006-08-08
/// From
/*
struct S_TCP_REC2AGT_LSN_DATAFLOW
{
	UL		PackageIndex;					// 包编号
	US		DataLength;						// 数据长度
	UC		ListenData[SIZE_AUDIO_PACKED];	// 数据
};
*/
/// To
struct S_TCP_REC2AGT_LSN_DATAFLOW
{
	UL		PackageIndex;					// 包编号
	US		DataLength;						// 数据长度
	UC		ListenData[SIZE_MAX_AUDIO_PACKAGE];	// 数据
};
//------------

//------------
#define TCP_REC2AGT_DOWNLOAD_DATAFLOW		1902		// (0x076E) 文件下传数据流
struct S_TCP_REC2AGT_DOWNLOAD_DATAFLOW
{
	US		DataLength;						// 数据长度
	UC		FileData[SIZE_FILE_PACKED];		// 数据
};
//------------
//--------------------------


//--------------------------
// 状态改变通知消息 - Total 4
//--------------------------
//------------
#define TCP_REC2AGT_ACCESSPOINT_STATUS		1996		// (0x07CC) 接入点状况报告消息
struct S_TCP_REC2AGT_ACCESSPOINT_STATUS
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	UC		AccessPointStatus;				// 设备状态，参见“开关常量定义”
	UC		Version;						// 版本号
	US		nChannelCount;					// 通道数量
	char	strAccessPointName[50];			// 接入点名字
};
//------------

//------------
#define TCP_REC2AGT_ACCESSPOINT_CH_STATUS	1997		// (0x07CD) 接入点通道状况报告消息
struct S_TCP_REC2AGT_ACCESSPOINT_CH_STATUS
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	US		ChannelIndex;					// 录音通道编号，1 Based
	char	UserName[20];					// 被监视用户名
	UC		StatusID;						// 状态标识
	UC		StatusValue;					// 状态值
};
//------------

//------------
#define TCP_REC2AGT_STORAGE_STATUS			1998		// (0x07CE) 服务器存储状况报告消息
struct S_TCP_REC2AGT_STORAGE_STATUS
{
	UC		StorageID;						// 设备标识
	UC		StorageIndex;					// 设备序号（适用于多网络备份设备）
	UC		StorageStatus;					// 设备状态
};

/// 设备标识定义
#define PCS_DEF_DEV_ID_LOG					0
#define PCS_DEF_DEV_ID_INDEX				1
#define PCS_DEF_DEV_ID_DATA					2
#define PCS_DEF_DEV_ID_LOCAL_BAK			3
#define PCS_DEF_DEV_ID_NET_BAK				4
#define PCS_DEF_DEV_ID_TAPE					5

/// 设备状态定义
#define PCS_DEF_DEV_ST_ERROR				0
#define PCS_DEF_DEV_ST_WARNING				1
#define PCS_DEF_DEV_ST_NOTICE				2
#define PCS_DEF_DEV_ST_FINE					3
#define PCS_DEF_DEV_ST_UNKNOWN				255
//------------

//------------
#define TCP_REC2AGT_USER_STATUS				1999		// (0x07CF) 用户状态改变通知
struct S_TCP_REC2AGT_USER_STATUS
{
	char	UserName[20];					// 被监视用户名
	UC		StatusID;						// 状态标识
	UC		StatusValue;					// 状态值
};

/// 状态标识定义
#define PCS_REC_ST_ID_LOGIN					0
#define PCS_REC_ST_ID_RECORD				1
#define PCS_REC_ST_ID_LISTEN				2
#define PCS_REC_ST_ID_MONITOR				3
#define PCS_REC_ST_ID_VOICEDEVICE			4
#define PCS_REC_ST_ID_CHANNELOPEN			5
#define PCS_REC_ST_ID_CTIBINDING			6
//------------
//--------------------------

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_RECORDAGENT_H__
//------------------------------------------------------------------------
