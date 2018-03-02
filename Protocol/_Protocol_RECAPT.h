////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		_Protocol_RECAPT.h
// 用途描述：	录音接入点通信协议
// 作者：		SUN
// 最后修改人：Stephen Wang
// 创建时间：	2006/01/11
// 最后修改时间：2006/12/06
// 版权信息：	Copyright (C) 1999 - 2006 PCSG
// 备注：
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_RECORDACCESSPOINT_H__
#define __PROTOCOL_RECORDACCESSPOINT_H__
//------------------------------------------------------------------------

//--------------------------
// 录音接入点注册 - Total 4
//--------------------------

//--------------------------
// 注册 - 2
//------------
#define TCP_APT2REC_REGISTER				1			// (0x0001) 注册消息
struct S_TCP_APT2REC_REGISTER
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	UC		Version;						// 版本号
	UC		RegCode[PCS_DEF_NUMLEN_APP];	// 注册码
};
//------------

//------------
#define TCP_REC2APT_REGISTER_ACK			1001		// (0x03E9) 注册响应消息
struct S_TCP_REC2APT_REGISTER_ACK
{
	UC		AccessPointID;					// 接入点ID, 0 Based
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_BEUSED
											// PCS_DEF_RETCODE_VERSION
											// PCS_DEF_RETCODE_REGCODE
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UC		FirstRecordStartType;			// 首选录音控制方式，参见“录音控制方式”
	UC		SecondRecordStartType;			// 备用录音控制方式，参见“录音控制方式”
	US		nChannelCount;					// 通道数量
	char	strAccessPointName[50];			// 接入点名字
};
//------------
//--------------------------

//--------------------------
// 取消注册 - 2
//------------
#define TCP_APT2REC_UNREGISTER				2			// (0x0002) 取消注册消息
//------------

//------------
#define TCP_REC2APT_UNREGISTER_ACK			1002		// (0x03EA) 取消注册响应消息
//------------
//--------------------------

//--------------------------
// 文件查询消息 - Total 6
//--------------------------
//------------
#define TCP_REC2APT_DOWNLOAD_FILE			101			// (0x0065) 请求AP下传录音文件消息
struct S_TCP_REC2APT_DOWNLOAD_FILE
{
	char	FilePath[128];					// 录音文件全路径
	char	RequirAdmin[20];				// 请求人
};
//------------

//------------
#define TCP_REC2APT_DOWNLOAD_CANCEL			102			// (0x0066) 取消下传录音文件消息
struct S_TCP_REC2APT_DOWNLOAD_CANCEL
{
	int		TransferID;						// 文件传输ID
};

//------------

//------------
#define TCP_APT2REC_DOWNLOAD_FILE_ACK		1101		// (0x044D) 请求AP下传录音文件响应消息
struct S_TCP_APT2REC_DOWNLOAD_FILE_ACK
{
	char	FilePath[128];					// 录音文件全路径
	char	RequirAdmin[20];				// 请求人用户名
	int		TransferID;						// 传输时的文件ID，由AP给定
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
#define TCP_APT2REC_DOWNLOAD_CANCEL_ACK		1102			// (0x044E) 取消下传录音文件响应消息
struct S_TCP_APT2REC_DOWNLOAD_CANCEL_ACK
{
	int		TransferID;						// 文件传输ID
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_APT2REC_DOWNLOAD_STARTED		1103			// (0x044F) 文件传输开始消息
struct S_TCP_APT2REC_DOWNLOAD_STARTED
{
	int		TransferID;						// 文件传输ID
	UDL		FileSize;						// 录音文件大小(字节数)
};
//------------

//------------
#define TCP_APT2REC_DOWNLOAD_FINISHED		1104			// (0x0450) 文件传输完成消息
struct S_TCP_APT2REC_DOWNLOAD_FINISHED
{
	int		TransferID;						// 文件传输ID
	UC		HasMoreFiles;					// 是否还有后续文件需要传输，参见“开关常量定义”
};
//------------
//--------------------------



//--------------------------
// 录音通道管理 - Total 5
//--------------------------
//------------
#define TCP_APT2REC_CH_OPEN					201			// (0x00C9) 录音通道初始化请求消息
struct S_TCP_APT2REC_CH_OPEN
{
	US		ChannelIndex;					// 录音通道编号，1 Based
	UC		ChannelStatus;					// 录音通道设备状态，参见“设备状态定义”
};
//------------

//------------
#define TCP_APT2REC_CH_CLOSE				202			// (0x00CA) 录音通道关闭请求消息
struct S_TCP_APT2REC_CH_CLOSE
{
	US		ChannelIndex;					// 录音通道编号，1 Based
};
//------------

//------------
#define TCP_APT2REC_CH_STATUS				220		// (0x00DC) 通道状况报告消息
struct S_TCP_APT2REC_CH_STATUS
{
	US		ChannelIndex;					// 录音通道编号，1 Based
	UC		StatusID;						// 状态标识
	UC		StatusValue;					// 状态值
};
//------------

//------------
#define TCP_REC2APT_CH_OPEN_ACK				1201		// (0x04B1) 录音通道初始化响应消息
struct S_TCP_REC2APT_CH_OPEN_ACK
{
	US		ChannelIndex;					// 录音通道编号，1 Based
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		AgentID;						// PowerCall坐席编号，CTI控制时必须提供
	UC		bMustLogin;						// 只有登录后才录音，否则全程录音
	char	strChannelName[20];				// 录音通道名字（全局唯一）
	UC		FirstRecordStartType;			// 首选录音控制方式，参见“录音控制方式”
	UC		SecondRecordStartType;			// 备用录音控制方式，参见“录音控制方式”
};
//------------

//------------
#define TCP_REC2APT_CH_CLOSE_ACK			1202		// (0x04B2) 录音通道关闭响应消息
struct S_TCP_REC2APT_CH_CLOSE_ACK
{
	US		ChannelIndex;					// 录音通道编号，1 Based
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------


//--------------------------
// 录音参数管理 - Total 6
//--------------------------
//------------
#define TCP_APT2REC_GET_SYSPARAM			221			// (0x00DD) 查询系统参数请求消息
struct S_TCP_APT2REC_GET_SYSPARAM
{
	char	strParamName[50];				// 参数名
	char	strTimeStamp[15];				// 时间戳
};
//------------

//------------
#define TCP_APT2REC_GET_CHPARAM				222			// (0x00DE) 查询通道参数请求消息
struct S_TCP_APT2REC_GET_CHPARAM
{
	US		ChannelIndex;					// 录音通道编号，1 Based
	char	strParamName[50];				// 参数名
};
//------------

//------------
// Added 2006-12-06
#define TCP_APT2REC_UPDATE_SYSPARAM			223		// (0x00DF) 更新系统参数消息
struct S_TCP_APT2REC_UPDATE_SYSPARAM
{
	char	strParamName[50];				// 参数名
	char	strParamValue[100];				// 参数值
	char	strTimeStamp[15];				// 时间戳
};
//------------

//------------
#define TCP_APT2REC_UPDATE_SYSPARAM_ACK		224		// (0x00E0) 更新系统参数消息响应消息
struct S_TCP_APT2REC_UPDATE_SYSPARAM_ACK
{
	char	strParamName[50];			// 参数名
	UC		ErrorCode;					// 返回码，参见“返回码定义”：
										// PCS_DEF_RETCODE_SUCC
										// PCS_DEF_RETCODE_NOTEXIST
										// PCS_DEF_RETCODE_LACK_PARAM
										// PCS_DEF_RETCODE_OTHERS
										// PCS_DEF_RETCODE_SYSERROR
	char	strTimeStamp[15];			// 时间戳
};
//------------


//------------
#define TCP_REC2APT_GET_SYSPARAM_ACK		1221		// (0x04C5) 查询系统参数响应消息
struct S_TCP_REC2APT_GET_SYSPARAM_ACK
{
	char	strParamName[50];				// 参数名
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	char	strParamValue[100];				// 参数值
};
//------------

//------------
#define TCP_REC2APT_GET_CHPARAM_ACK			1222		// (0x04C6) 查询通道参数响应消息
struct S_TCP_REC2APT_GET_CHPARAM_ACK
{
	US		ChannelIndex;					// 录音通道编号，1 Based
	char	strParamName[50];				// 参数名
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_LACK_PARAM
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	char	strParamValue[100];				// 参数值
};
//------------

//------------
#define TCP_APT2REC_GET_CHPARAM_ACK			1223		// (0x04C7) 查询通道参数响应消息
struct S_TCP_APT2REC_GET_CHPARAM_ACK
{
	US		ChannelIndex;					// 录音通道编号，1 Based
	char	strParamName[50];				// 参数名
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_LACK_PARAM
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	char	strParamValue[100];				// 参数值
};
//------------

// Added 2006-12-06
#define TCP_REC2APT_UPDATE_SYSPARAM_ACK		1224		// (0x04C8) 更新系统参数消息响应消息
struct S_TCP_REC2APT_UPDATE_SYSPARAM_ACK
{
	char	strParamName[50];			// 参数名
	UC		ErrorCode;					// 返回码，参见“返回码定义”：
										// PCS_DEF_RETCODE_SUCC
										// PCS_DEF_RETCODE_NOTEXIST
										// PCS_DEF_RETCODE_LACK_PARAM
										// PCS_DEF_RETCODE_OTHERS
										// PCS_DEF_RETCODE_SYSERROR
	char	strTimeStamp[15];			// 时间戳
};
//------------

//------------
#define TCP_REC2APT_UPDATE_SYSPARAM			1225		// (0x04C9) 更新系统参数消息
struct S_TCP_REC2APT_UPDATE_SYSPARAM
{
	char	strParamName[50];				// 参数名
	char	strParamValue[100];				// 参数值
	char	strTimeStamp[15];				// 时间戳
};
//------------

//--------------------------


//--------------------------
// 录音控制消息 - Total 9
//--------------------------
//------------
#define TCP_APT2REC_STARTREC				251			// (0x00FB) 通道开始录音请求消息
// Replaced 2006-08-08
/// From
/*
struct S_TCP_APT2REC_STARTREC
{
	US		ChannelIndex;					// 录音通道编号，1 Based
	UC		RecordStartType;				// 录音启动控制方式，参见“录音控制方式”
	UC		SamplesPerSec;					// 采样频率(见常数定义)
	UC		BitsPerSample;					// 采样比特(8 or 16)
	UC		Channels;						// 通道数(1 or 2)
	char	strANI[PCS_DEF_NUMLEN_APP];		// 电话号码，板卡获取
};
*/
/// To
struct S_TCP_APT2REC_STARTREC
{
	US		ChannelIndex;					// 录音通道编号，1 Based
	UC		RecordStartType;				// 录音启动控制方式，参见"录音控制方式"
	UC		FormatTag;						// 语音采集方式ID（比如，1：G.729；2：ADPCM；3： MP3等等）
	char	strANI[PCS_DEF_NUMLEN_APP];		// 电话号码，板卡获取
	char	FilePath[128];					// 本地录音文件全路径
	UC		HaveMoreFormatDescription;		// 是否还有余下的格式描述串
	US		FormatBufferLen;				// 格式描述长度
	BYTE	FormatDescription[PCS_DEF_FORMAT_DESC_LEN];	
											// 这个是格式描述的二进制流，比如说FormatTag 为ADPCM的时候，就是一个WAVEFORMATEX结构体实例，如果为MP3 时，则是一个MP3格式描述的结构体实例）
};
//------------

//------------
// Added 2006-08-08
#define TCP_APT2REC_FORMATDESC_CONT			252			// (0x00FC) 后续格式描述串
struct S_TCP_APT2REC_FORMATDESC_CONT
{
	US		ChannelIndex;					// 录音通道编号，1 Based
	UC		Index;							// 包顺序，1 Based
	UC		HaveMoreFormatDescription;		// 是否还有余下的格式描述串
	US		FormatBufferLen;				// 格式描述长度
	BYTE	FormatDescription[PCS_DEF_FORMAT_DESC_LEN];
};
//------------

//------------
#define TCP_APT2REC_STOPREC					261			// (0x0105) 通道停止录音请求消息
struct S_TCP_APT2REC_STOPREC
{
	US		ChannelIndex;					// 录音通道编号，1 Based
	UC		RecordStopType;					// 录音停止控制方式，参见“录音控制方式”
	UL		RecordLength;					// 总录音长度 added by stephen wang 2006-08-21
	char	KeyPressData[200];				// 按键数据
};
//------------

//------------
// Added 2006-08-08
#define TCP_APT2REC_GET_RECDATA_ACK			271			// (0x010F) 请求AP传输语音数据的响应消息
struct S_TCP_APT2REC_GET_RECDATA_ACK
{
	US		ChannelIndex;					// 录音通道编号，1 Based
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_REC2APT_STARTREC_ACK			1251		// (0x04E3) 通道开始录音响应消息
// Replaced 2006-08-08
/// From
/*
struct S_TCP_REC2APT_STARTREC_ACK
{
	US		ChannelIndex;					// 录音通道编号，1 Based
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
*/
/// To
struct S_TCP_REC2APT_STARTREC_ACK
{
	US		ChannelIndex;					// 录音通道编号，1 Based
	UC		ErrorCode;						// 返回码，参见"返回码定义"
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UC		IsLocalSave;					// 是否本地存储
};
//------------

//------------
#define TCP_REC2APT_STARTREC_CMD			1252		// (0x04E4) 服务器命令通道开始录音(CTI控制、班长控制、用户手动控制)
struct S_TCP_REC2APT_STARTREC_CMD
{
	US		ChannelIndex;					// 录音通道编号，1 Based
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
#define TCP_REC2APT_STOPREC_ACK				1261		// (0x04ED) 通道停止录音响应消息
struct S_TCP_REC2APT_STOPREC_ACK
{
	US		ChannelIndex;					// 录音通道编号，1 Based
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
#define TCP_REC2APT_STOPREC_CMD				1262		// (0x04EE) 服务器命令通道停止录音(CTI控制、班长控制、用户手动控制)
struct S_TCP_REC2APT_STOPREC_CMD
{
	US		ChannelIndex;					// 录音通道编号，1 Based
	UC		RecordStopType;					// 录音停止控制方式，参见“录音控制方式”
	char	MonitorName[20];				// 班长名，班长控制时使用
};
//------------

//------------
#define TCP_REC2APT_CTI_CALLHELD				1263		// (0x04EF) 电话保持消息
struct S_TCP_REC2APT_CTI_CALLHELD
{
	US		ChannelIndex;					// 录音通道编号，1 Based
};
//------------
//--------------------------



//------------
// Added 2006-08-08
#define TCP_REC2APT_GET_RECDATA_REQ			1271		// (0x04F7) 请求AP传输语音数据消息
struct S_TCP_REC2APT_GET_RECDATA_REQ
{
	US		ChannelIndex;					// 录音通道编号，1 Based
};
//------------


//------------
// Added 2006-08-22
#define TCP_REC2APT_CANCEL_RECDATA_TRANS	1272		// (0x04F8)	取消请求AP传输语音数据消息
struct S_TCP_REC2APT_CANCEL_RECDATA_TRANS
{
	US		ChannelIndex;					// 录音通道编号，1 Based
};
//------------
//--------------------------


//--------------------------
// 录音索引传递管理消息 - Total 4
//--------------------------
//------------
#define TCP_APT2REC_REQ_RECORDINDEX_UPDATE_ACK	301		// (0x012D) Server向AP发送传输录音索引请求响应消息
struct S_TCP_APT2REC_REQ_RECORDINDEX_UPDATE_ACK
{
	UC		AccessPointID;					// 接入点ID, 0 Based.
	UC		ErrorCode;						// 返回码，参见“返回码定义”
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		RecordCount;					// 需同步索引条数
};
//------------

//------------
#define TCP_APT2REC_END_RECORDINDEX_UPDATE	302			// (0x012E) 索引发送结束消息
struct S_TCP_APT2REC_END_RECORDINDEX_UPDATE
{
	UC		AccessPointID;					// 接入点ID, 0 Based.
	UC		ErrorCode;						// 返回码，参见“返回码定义”
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_USERINTERRUPT
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		RecordCount;					// 实际同步索引条数
};
//------------

//------------
#define TCP_APT2REC_RECORDINDEX_UPDATE		303			// (0x012F) 录音索引数据包消息
struct S_TCP_APT2REC_RECORDINDEX_UPDATE
{
	UC		AccessPointID;					// 接入点ID, 0 Based.
	UL		PackageNo;						// 录音索引包号
	US		DataLength;						// 录音索引包数据长度（最后一个包可能不满足最大长度）
	UC		HaveMoreData;					// 还有后续数据否？
	UC		RecordIndexBuffer[SIZE_FILE_PACKED];	// 录音索引Buffer
};
//------------

//------------
#define TCP_REC2APT_REQ_RECORDINDEX_UPDATE	1301		// (0x0515) Server向AP发送传输录音索引请求消息
struct S_TCP_REC2APT_REQ_RECORDINDEX_UPDATE
{
	UC		AccessPointID;					// 接入点ID, 0 Based.
};
//------------
//--------------------------



//--------------------------
// 数据流消息 - Total 2
//--------------------------
//------------
#define TCP_APT2REC_REC_DATAFLOW			951			// (0x03B7) 通道录音数据流
// Replaced 2006-08-08
/// From
/*
struct S_TCP_APT2REC_REC_DATAFLOW
{
	US		ChannelIndex;					// 录音通道编号，1 Based
	UL		PackageIndex;					// 包编号
	US		DataLength;						// 数据长度
	UC		RecordData[SIZE_AUDIO_PACKED];	// 数据
};
*/
/// To
struct S_TCP_APT2REC_REC_DATAFLOW
{
	US		ChannelIndex;					// 录音通道编号，1 Based
	UL		PackageIndex;					// 包编号
	US		DataLength;						// 数据长度
	UC		RecordData[SIZE_MAX_AUDIO_PACKAGE];	// 数据
};
//------------

//------------
// Added 2006-12-06
#define TCP_APT2REC_DOWNLOAD_DATAFLOW		952		// (0x03B8) 文件下传数据流
struct S_TCP_APT2REC_DOWNLOAD_DATAFLOW
{
	int		TransferID;						// 文件传输ID
	UL		PackageNo;						// 文件包序号 （1-n）
	US		DataLength;						// 数据长度
	UC		FileData[SIZE_FILE_PACKED];		// 数据
};
//------------
//--------------------------



//--------------------------
// 状态通知消息 - Total 1
//--------------------------
//------------
#define TCP_REC2APT_USER_STATUS				1999		// (0x07CF) 通道用户状态改变通知
struct S_TCP_REC2APT_USER_STATUS
{
	US		ChannelIndex;					// 录音通道编号，1 Based
	char	UserName[20];					// 被监视用户名
	UC		StatusID;						// 状态标识
	UC		StatusValue;					// 状态值
};
//--------------------------


//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_RECORDACCESSPOINT_H__
//------------------------------------------------------------------------
