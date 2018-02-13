////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		_NodeFunc.h
// 用途描述：	节点例行处理函数声明
// 作者：		SUN
// 创建时间：	2001/03/21
// 最后修改时间：2012/05/07
// 版权信息：	Copyright (C) 1999 - 2012 Changjiang Xincheng, PicaSoft
// 备注：
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//------------------------------------------------------------------------
#ifndef __NODEFUNC_H_
#define __NODEFUNC_H_
//------------------------------------------------------------------------

#ifndef MAXSYSNODEID
#define MAXSYSNODEID		255			// 最大的系统节点ID
#endif

// Sun added 2003-04-25
#define MAXLANGUAGECOUNT	10			// 最大服务语言种数

//------------------------------------------------------------------------
// Sun added 2002-05-22
/// 系统变量
#define SysVar_MainMenu						"MainMenu"				// 业务类型
#define SysVar_SubMenu						"SubMenu"				// 业务二级菜单
#define SysVar_ThirdMenu					"ThirdMenu"				// 业务三级菜单
#define SysVar_Language						"Language"				// 服务语言代码
#define SysVar_CallBack						"CallBack"				// 回拨标记
#define SysVar_FaxBack						"FaxBack"				// FOD标记
#define SysVar_EMailBack					"EMailBack"				// EMail标记
#define SysVar_SMSBack						"SMSBack"				// SMS标记
#define SysVar_MailBack						"MailBack"				// Mail标记
#define SysVar_CBExtData					"CBExtData"				// 回拨扩展数据
#define SysVar_WaitTime						"WaitTime"				// 计时标记
#define SysVar_CustomerID					"CustomerID"			// 客户编号
#define SysVar_Password						"Password"				// Password
#define SysVar_CallInTime					"CallInTime"			// 呼入时间
#define SysVar_CallRefID					"CallRefID"				// 呼叫标识
#define SysVar_ANI							"ANI"					// 主叫号码
#define SysVar_DNIS							"DNIS"					// 被叫号码
#define SysVar_VisitLog						"VisitLog"				// 节点访问日志
#define SysVar_AppData						"AppData"				// 应用数据
#define SysVar_UserData						"UserData"				// 用户数据
#define SysVar_ExtData						"ExtData"				// 扩展数据
#define SysVar_OBTelNo						"OBTelNo"				// 外拨号码

// Sun added 2012-03-09
/// 只读变量，定义时不需要分配空间，只要有名字即可使用
#define SysVar_ChannelID					"ChannelID"				// IVR通道号

// Sun added 2007-12-12
#define SysVar_CampaignID					"CampaignID"			// 外拨行动ID
#define SysVar_TaskID						"TaskID"				// 外拨任务ID

// Sun added 2013-05-20
#define SysVar_GlobalCallRefID				"GlobalCallRefID"			// 全局呼叫标示，char(24)
#define SysVar_CountryCode					"CountryCode"			// 国家代码，char(6)
#define SysVar_CityCode						"CityCode"				// 区号，char(6)
#define SysVar_PhoneNumber					"PhoneNumber"			// 电话号码，char(20)
#define SysVar_WorkTicketID					"WorkTicketID"			// 工单ID，char(50)


//------------------------------------------------------------------------

#define DEF_CallBack_None					0x00
#define DEF_CallBack_Phone					0x01
#define DEF_CallBack_Fax					0x02
#define DEF_CallBack_EMail					0x04
#define DEF_CallBack_SMS					0x08
#define DEF_CallBack_Mail					0x10

// 事件组结构
#define DEF_NODE_DATA_SMALL_LEN				13
#define DEF_NODE_DATA_BIG_LEN				64
struct SEventTable{
	int cStatus;							// 节点ID
	UC  Data1[DEF_NODE_DATA_SMALL_LEN];		// 数据部分1
	UC	Data2[DEF_NODE_DATA_BIG_LEN];		// 数据部分2
	UC	NodeNo;								// 节点类型(处理函数)
};

//------------------------------------------------------------------------
// 节点000：系统节点
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_000{
	UC		reserved1[10];		// 保留
    UC		Languages;          // 语言数量
    UC		MajorVer;           // 主版本
    UC		MinorVer;           // 次版本
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_000{
	UC		key_repeat;			// 重复当前节点按键
	UC		key_return;			// 回到上一级节点按键
	UC		key_root;			// 回到主菜单按键
	UC		reserved1;			// 保留
    US		ResourceProject;	// 资源项目ID sun 2002-12-03
    US		MainCOM;			// 流程COM组件资源编号 sun 2002-12-03
	UC		LogSwitchOff;		// 节点日志全局控制开关, sun added 2008-02-19
    UC		reserved2[39];		// 保留 sun 2002-12-03
    
	US		nd_parent;			// 父节点ID
	US		nd_root;			// 主菜单(根)ID
	US		nd_SysSendData;		// 系统缺省发送数据格式定义节点 sun 2004-12-30
	US		nd_BeforeHookOn;	// 挂机前转节点，Sun added 2012-05-07
	UC		reserved3[8];		// 保留
};

//------------------------------------------------------------------------
// 节点001：变量总数
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_001{
	UC		reserved1[11];		// 保留
	UC		uservars;			// 用户定义变量数, 0-255
    UC      reserved2;
};

//------------------------------------------------------------------------
// 节点002：变量清单
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_002{
	UC		reserved1[13];		// 保留
};
/// 用户定义变量结构
struct SUserVarDef{
	UC		length;				// 变量长度0-255
	UC		type;				// 变量数据类型
	UC		name[14];			// 变量名称
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_002{
	SUserVarDef uservar[4];		// 用户定义变量0-3
};

//------------------------------------------------------------------------
// 节点006：无条件转移
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_006{
	US		sleep;				// 延时（sleep），单位为毫秒
	UC		reserved1[11];		// 保留
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_006{
	US		nd_goto;			// 跳转节点ID
	UC		reserved1[46];		// 保留
	US		nd_parent;			// 父节点ID
	UC		reserved2[14];		// 保留
};

//------------------------------------------------------------------------
// 节点007：身份验证
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_007{
	UC		timeout;			// 节点超时(秒)
	UC		maxuserid;			// 用户号码最大长度
	UC		maxpassword;		// 用户口令最大长度
	UC		key_term;			// 输入终止符, 按键
	UC		maxtrytime;			// 最大尝试次数
	UC		log;				// 被访问日志
	UC		var_trytime;		// 验证次数记录(0 - maxtrytime)
	UC		var_result;			// 验证结果记录
	UC		var_userid;			// 用户号码记录
	UC		var_password;		// 用户口令记录
	UC		reserved1[3];		// 保留
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_007{
	US		vox_userid;			// 提示用户输入号码播放语音
	US		vox_password;		// 提示用户输入口令播放语音
	US		vox_tryagain;		// 提示用户重新输入播放语音
	UC		reserved1[10];		// 保留
	US		com_iid;			// COM接口ID
	UC		reserved2[30];		// 保留
	US		nd_parent;			// 父节点ID
	US		nd_succeed;			// 成功转节点ID
	US		nd_fail;			// 失败转节点ID
	UC		reserved3[10];		// 保留
};

//------------------------------------------------------------------------
// 节点008：修改口令
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_008{
	UC		timeout;			// 节点超时(秒)
	UC		reserved1;			// 保留
	UC		maxpassword;		// 用户口令最大长度
	UC		key_term;			// 输入终止符, 按键
	UC		maxtrytime;			// 最大尝试次数
	UC		log;				// 被访问日志
	UC		var_trytime;		// 尝试次数记录(0 - maxtrytime)
	UC		var_result;			// 验证结果记录
	UC		reserved2;			// 保留
	UC		var_password;		// 用户口令记录
	UC		reserved3[3];		// 保留
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_008{
	US		vox_password;		// 提示用户输入新口令播放语音
	US		vox_confirm;		// 提示用户再次确认播放语音
	US		vox_tryagain;		// 两次不一致重新输入播放语音
	US		vox_succeed;		// 提示用户修改成功播放语音
	UC		reserved1[8];		// 保留
	US		com_iid;			// COM接口ID
	UC		reserved2[30];		// 保留
	US		nd_parent;			// 父节点ID
	US		nd_succeed;			// 成功转节点ID
	US		nd_fail;			// 失败转节点ID
	UC		reserved3[10];		// 保留
};

//------------------------------------------------------------------------
// 节点009：时间分支
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_009{
	UC		reserved1[5];		// 保留
	UC		log;				// 被访问日志
	UC		reserved2[7];		// 保留
};
/// 时间段结构
struct STimePeriod{
	UC		StartHH;			// 开始小时
	UC		StartMM;			// 开始分钟
	UC		EndHH;				// 终止小时
	UC		EndMM;				// 终止分钟
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_009{
	UC		workday;			// 工作日安排, Bit操作, 0-6位有效,0:休息日;1:工作日
	UC		worktime;			// 工作日时间段安排, Bit操作, 0-5位有效,0:无效;1:有效
	STimePeriod timesec[6];		// 时间段1-6
	UC		reserved1[22];		// 保留
	US		nd_parent;			// 父节点ID
	US		nd_sparetime;		// 休息日转节点ID
	US		nd_timesec1;		// 时间段1转节点ID
	US		nd_timesec2;		// 时间段2转节点ID
	US		nd_timesec3;		// 时间段3转节点ID
	US		nd_timesec4;		// 时间段4转节点ID
	US		nd_timesec5;		// 时间段5转节点ID
	US		nd_timesec6;		// 时间段6转节点ID
};

//------------------------------------------------------------------------
// Sun added 2004-04-30
// 节点010：工作日设定
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_010{
    UC		maincalendar;		// 是否是主日历，不可见，系统识别
    UC		startyear;			// 开始年份，YY
    UC		startmonth;			// 开始月，1-12
    UC		monthcount;			// 共几个月（最大12个月）
    UC		reserved1;			// 保留
    UC		log;				// 被访问日志
	UC		reserved2[7];		// 保留
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_010{
    UC		daytype[48];		// 位操作: 0 - 工作日；1 - 节假日
    US		nd_parent;			// 父节点ID
    US		nd_daysec[3];		// 转节点ID
    UC		reserved1[8];		// 保留
};

//------------------------------------------------------------------------
// Sun added 2004-12-30
// 节点016：条件分支
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_016{
    UC		reserved1[5];		// 保留
    UC		log;                // 被访问日志
	UC		var_id;				// 变量名
    UC		logic;              // 逻辑运算符
    UC		convert;            // 转换公式
    UC		param1;             // 参数1
    UC		param2;             // 参数2
    UC		reserved2[2];       // 保留
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_016{
    UC		var_value[48];      // 变量值
    US		nd_parent;	        // 父节点ID
    US		nd_succ;	        // 条件满足转移节点ID
    US		nd_fail;			// 条件不满足转移节点ID
    UC		reserved3[10];      // 保留
};

/// 逻辑运算符定义
#define DEF_NODE016_LOGIC_EQUE			0			// =
#define DEF_NODE016_LOGIC_BIGGER		1			// >
#define DEF_NODE016_LOGIC_SMALLER		2			// <
#define DEF_NODE016_LOGIC_EB			3			// >=
#define DEF_NODE016_LOGIC_ES			4			// <=
#define DEF_NODE016_LOGIC_NE			5			// <>
#define DEF_NODE016_LOGIC_LIKE			6			// Like

/// 转换公式定义
#define DEF_NODE016_CONVERT_NONE		0			// Do Nothing
#define DEF_NODE016_CONVERT_LEFT		1			// Left(n)
#define DEF_NODE016_CONVERT_RIGHT		2			// Right(n)
#define DEF_NODE016_CONVERT_MID			3			// Mid(m, n)
#define DEF_NODE016_CONVERT_LEN			4           // Len()

//------------------------------------------------------------------------
// Sun added 2003-04-24
// 节点017：服务语言选择
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_017{
	UC		timeout;			// 节点超时(秒)
	UC		maxtrytime;			// 最大尝试次数
	UC		var_lang;			// 验证结果记录
	UC		reserved1[10];		// 保留
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_017{
	US		vox_play;			// 播放语音
	UC		lang[MAXLANGUAGECOUNT];			// 语言对应按键
	UC		reserved1[36];		// 保留
	US		nd_parent;			// 父节点ID
	US		nd_succ;			// 成功节点ID
	US		nd_fail;			// 失败节点ID
	UC		reserved2[10];		// 保留
};

//------------------------------------------------------------------------
// Sun updated 2004-12-30
// 节点018：发送数据
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_018{
    UC		seperator;			// 分隔符
	UC		reserved1[12];		// 保留
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_018{
    UC		typeflags[15];		// 标记：AppData/UserData
    UC		prefix1[15];        // 变量前缀1
    UC		prefix2[15];		// 变量前缀2
    UC		valueid[15];		// 变量ID
	US		nd_parent;			// 父节点ID
	US		nd_child;			// 子节点ID
};

//------------------------------------------------------------------------
// 节点019：无操作
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_019{
	UC		reserved1[12];		// 保留
	UC		leavequeue;			// 是否离队
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_019{
	US		delaytime;			// 延时时间(ms)
	UC		reserved1[46];		// 保留
	US		nd_parent;			// 父节点ID
	UC		reserved2[14];		// 保留
};

//------------------------------------------------------------------------
// 节点020：放音挂机
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_020{
	UC		reserved1;			// 保留
	UC		playclear;			// 放音清空标志: 0 -- 不清空;1 -- 清空
	UC		reserved2[3];		// 保留
	UC		log;				// 被访问日志
	UC		reserved3[7];		// 保留
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_020{
	US		vox_play;			// 播放语音
	UC		reserved1[46];		// 保留
	US		nd_parent;			// 父节点ID
	UC		reserved2[14];		// 保留
};

//------------------------------------------------------------------------
// 节点021：放音继续
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_021{
	UC		reserved1;			// 保留
	UC		playclear;			// 放音清空标志: 0 -- 不清空;1 -- 清空
	UC		playtype;			// 播放类型(可组合):0数字1金额2号码4字母8汉字16日期
	UC		usevar;				// 使用变量ID: 0 --不使用；1 -- 255 变量ID
	UC		breakkey;			// 中断按键
	UC		log;				// 被访问日志
	UC		reserved3[7];		// 保留
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_021{
	US		vox_pred;			// 前导播放语音
	US		vox_succ;			// 后续播放语音
	UC		reserved1[12];		// 保留
	US		com_iid;			// COM接口ID
	UC		reserved2[30];		// 保留
	US		nd_parent;			// 父节点ID
	US		nd_child;			// 子节点ID
	UC		reserved3[12];		// 保留
};

//------------------------------------------------------------------------
// 节点022：放音等待按键
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_022{
	UC		timeout;			// 节点超时(秒)
	UC		playclear;			// 放音清空标志: 0 -- 不清空;1 -- 清空
	UC		getlength;			// 用户输入长度
	UC		maxinterval;		// 按键最大间隔(秒)
	UC		breakkey;			// 中断按键
	UC		log;				// 被访问日志
	UC		var_key;			// 用户按键记录, 1-255变量ID
	UC		maxtrytime;			// 最大尝试次数
	UC		reserved1[5];		// 保留
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_022{
	US		vox_play;			// 播放语音
	US		vox_nodefail;		// 节点失败播放语音[V2]
	UC		reserved1[28];		// 保留
	US		nd_parent;			// 父节点ID
	US		nd_key0;			// 子节点ID: 0
	US		nd_key1;			// 子节点ID: 1
	US		nd_key2;			// 子节点ID: 2
	US		nd_key3;			// 子节点ID: 3
	US		nd_key4;			// 子节点ID: 4
	US		nd_key5;			// 子节点ID: 5
	US		nd_key6;			// 子节点ID: 6
	US		nd_key7;			// 子节点ID: 7
	US		nd_key8;			// 子节点ID: 8
	US		nd_key9;			// 子节点ID: 9
	US		nd_keyA;			// 子节点ID: *
	US		nd_keyB;			// 子节点ID: #
	US		nd_nodefail;		// 子节点ID: 失败转入节点[V2]
	UC		reserved2[4];		// 保留
};

//------------------------------------------------------------------------
// 节点023：放音转移
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_023{
	UC		timeout;			// 节点超时(秒)
	UC		playclear;			// 放音清空标志: 0 -- 不清空;1 -- 清空
	UC		breakkey;			// 中断按键
	UC		reserved1[2];		// 保留
	UC		log;				// 被访问日志
	UC		var_play;			// 放音, 1-255变量ID
	UC		reserved2[6];		// 保留
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_023{
	US		vox_play;			// 播放语音
	UC		reserved1[46];		// 保留
	US		nd_parent;			// 父节点ID
	US		nd_goto;			// 转移节点ID
	UC		reserved2[12];		// 保留
};

//------------------------------------------------------------------------
// Sun added 2004-12-30
// 节点028：TTS放音
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_028{
	UC		timeout;			// 节点超时(秒)
	UC		playclear;			// 放音清空标志: 0 -- 不清空;1 -- 清空
	UC		usevar;				// 使用变量ID: 0 --不使用；1 -- 255 变量ID
	UC		breakkey;			// 中断按键
	UC		reserved1;			// 保留
	UC		log;				// 被访问日志
	UC		reserved2[7];		// 保留
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_028{
	US		vox_string;			// 播放字符串，支持"%s"插入子串
	US		vox_alter;			// TTS不可用时替代语音
	UC		playtype;			// 播放类型：
                                /// 0 - 字符串
                                /// 1 - 文本文件（vox_string格式化后作为文件路径）
                                /// 2 - ExtData
                                /// 3 - UserData
	UC		reserved1[11];		// 保留
	US		com_iid;			// COM接口ID
	UC		reserved2[30];		// 保留
	US		nd_parent;			// 父节点ID
	US		nd_succ;			// TTS播放成功转移节点ID
	US		nd_fail;			// TTS播放失败转移节点ID
	UC		reserved3[10];		// 保留
};

//------------------------------------------------------------------------
// 节点030：自动语音识别
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_030{
	US		timeout;			// 节点超时（毫秒500-10000，默认：5000）
	UC		playclear;			// 放音清空标志: 0 -- 不清空;1 -- 清空
	UC		trytimes;           // 尝试次数
	UC		dataformat;         // 语音数据格式
	UC		log;				// 被访问日志
	UC		var_key;			// 用户按键记录变量, 0 --不记录；1-255变量ID
	UC		var_asr;			// 识别结果记录变量ID: 0 --不记录；1 -- 255 变量ID
	UC		recordfile;			// 录音是否保存在文件中，0 --不保存；1 保存（文件名自动生成）
	UC		breakkey;			// 中断按键
	UC		sendbeep;			// 识别前是否发出Beep声，0 -- 不发；1 -- 发
	UC		reserved1[2];		// 保留
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_030{
	US		vox_op;				// 操作提示语音
	US		grammar_id;			// 语法文件(资源ID)
	UC		reserved1[24];		// 保留
	US		asr_language;		// ASR参数-语言
	US		asr_vad_onoff;		// ASR参数-端点检测开关（0-关，1-80开，默认：20）
	US		asr_vad_startcut;	// ASR参数-起始端点长度（毫秒，80-800，默认：350）
	US		asr_vad_endcut;		// ASR参数-结束端点长度（毫秒，200-2000，默认：600）
	UC		reserved2[12];		// 保留
	US		nd_parent;			// 父节点ID
	US		nd_key_break;		// 按键中断转节点ID
	US		nd_asr_succ;		// ASR成功转节点ID
	US		nd_fail;			// 失败转节点ID
	UC		reserved3[8];		// 保留
};

/// ASR参数-语音数据格式
#define DEF_NODE030_ASR_DATFMT_PCM		1			// PCM, 偏移44个字节
#define DEF_NODE030_ASR_DATFMT_ALAW		2			// A-Law, 偏移58个字节
#define DEF_NODE030_ASR_DATFMT_ULAW		3			// U-Law, 偏移58个字节
#define DEF_NODE030_ASR_DATFMT_ADPCM	4			// ADPCM, 偏移60个字节

/// ASR参数-语言
#define DEF_NODE030_ASR_LANG_CHINESE	1			// 仅中文
#define DEF_NODE030_ASR_LANG_ENGLISH	2			// 仅英文
#define DEF_NODE030_ASR_LANG_CHN_ENG	3			// 中英文

//------------------------------------------------------------------------
// 节点040：建立留言
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_040{
	UC		rectime;			// 录音时长(秒)
	UC		playclear;			// 放音清空标志: 0 -- 不清空;1 -- 清空
	UC		breakkey;			// 中断按键
	UC		var_agent;			// Agent ID
	UC		var_filename;       // 录音文件名记录变量
	UC		log;				// 被访问日志
	UC		var_appfield[3];	// 应用数据项
	UC		maxsilencetime;		// 最大静音时长(秒)
    UC		vmsclass;           // 留言分类[2007-02-28]
    UC		MinRecLength;       // 最短录音时长(秒)[2007-03-20]
    UC		toneoff;			// 不播放录音开始提示[2009-07-24]，0 - 播放；1 - 不播放
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_040{
	US		vox_op;				// 录音前提示语音
    UC		recfiletype;        // 录音文件类型[2007-06-28]: 0 - vox; 1 - wav
    UC		rectime_ho;         // 录音时长高8位
	UC		var_notifyintvl;	// 提示间隔(秒)[2009-07-24]，0 - 不提示
	UC		var_rectime;		// 录音实际时长记录变量(秒)[2009-07-24]
	UC		NotifyPL;			// 是否通知PL录音系统[2012-04-18] [A]
	UC		reserved1[41];		// 保留
	US		nd_parent;			// 父节点ID
	US		nd_child;			// 子节点ID
	UC		reserved2[12];		// 保留
};

/// 留言分类定义
#define DEF_NODE040_VMSCLASS_UNKNOWN	0			// 未知
#define DEF_NODE040_VMSCLASS_ALL		1           // 公共
#define DEF_NODE040_VMSCLASS_GROUP		2           // 组
#define DEF_NODE040_VMSCLASS_EXT		3           // 分机
#define DEF_NODE040_VMSCLASS_USER		4           // 用户

//------------------------------------------------------------------------
// 节点041：察看留言
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_041{
	UC		timeout;			// 节点超时(秒)
	UC		playclear;			// 放音清空标志: 0 -- 不清空;1 -- 清空
	UC		breakkey;			// 中断按键
	UC		var_agent;			// Agent ID
	UC		reserved1;			// 保留
	UC		log;				// 被访问日志
    UC		vmstype;			// 察看留言类型
    UC		closewhencheck;		// 察看后自动变为关闭状态
	UC		reserved2[5];		// 保留
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_041{
	US		vox_predrec;		// 留言状态报告前导
	US		vox_succrec;		// 留言状态报告后续
	US		vox_browse;			// 浏览提示语音
	US		vox_op;				// 操作提示语音
	UC		key_first;			// 听第一条留言按键
	UC		key_previous;		// 前一条留言按键
	UC		key_next;			// 听下一条留言按键
	UC		key_last;			// 听最后一条留言按键
	UC		key_repeat;			// 重听当前留言按键
	UC		key_delete;			// 删除当前留言按键
	UC		key_exit;			// 退出本节点按键
	UC		key_convert;		// 转换类型按键
	UC		reserved1[32];		// 保留
	US		nd_parent;			// 父节点ID
	US		nd_child;			// 子节点ID
	UC		reserved2[12];		// 保留
};

/// 察看留言类型定义
#define DEF_NODE041_VMSTYPE_NEW			0			// 新留言，可删除、存档
#define DEF_NODE041_VMSTYPE_CLOSED		1           // 存档留言，可删除
#define DEF_NODE041_VMSTYPE_DELETED		2           // 删除留言，可取消删除变为存档

//------------------------------------------------------------------------
/// 构造传真文件名
#define DEF_NODE050_FAXN_TYPE_AUTO		0			// 自动生成
#define DEF_NODE050_FAXN_TYPE_RESID		1           // 资源ID
#define DEF_NODE050_FAXN_TYPE_VAR2RESID	2           // 变量对应资源ID
#define DEF_NODE050_FAXN_TYPE_VAR2NAME	3           // 变量对应文件名
#define DEF_NODE050_FAXN_TYPE_FORMAT	4           // 变量替换资源中的通配符

// 节点050：简单传真
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_050{
	US		timeout;			// 节点超时(秒)
	UC		filenametype;		// 传真文件名类型: 
								/// 1 -- 资源ID
								/// 2 -- 变量对应资源ID
								/// 3 -- 变量对应文件名
								/// 4 -- 变量替换资源中的通配符
								/// 一次多个发送多个文件用分号分割，但最多5个
	UC		trytimes;           // 尝试次数
	UC		record_cdr;			// 是否记录传真发送详单: 0 --不记录；1 -- 记录
	UC		log;				// 被访问日志
    UC		var_faxfile;        // 传真文件使用变量ID: 0 --不使用；1 -- 255 变量ID
	UC		var_fromno;         // 发出号码使用变量ID: 0 --不使用；1 -- 255 变量ID
	UC		var_tono;			// 接收号码使用变量ID: 0 --不使用；1 -- 255 变量ID
    UC		var_result;         // 结果记录变量ID: 0 --不使用；1 -- 255 变量ID
	UC		var_appfield[3];	// 应用数据项
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_050{
	US		vox_op;				// 操作提示语音
	US		fax_fileid;			// 传真文件(资源ID)
	US		header_id;			// 传真标题(资源ID)
	UC		reserved1[42];		// 保留
	US		nd_parent;			// 父节点ID
	US		nd_succ;			// 成功转节点ID
	US		nd_fail;			// 失败转节点ID
	UC		reserved2[10];		// 保留
};

//------------------------------------------------------------------------
// 节点051：TTF传真
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_051{
	US		timeout;			// 节点超时(秒)
	UC		reserved1[3];		// 保留
	UC		log;				// 被访问日志
	UC		reserved2[7];		// 保留
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_051{
	US		vox_op;				// 操作提示语音
	US		fax_logo;			// LOGO文件(资源ID)
	US		fax_format;			// 表格格式文件(资源ID)
	US		header_id;			// 传真标题(资源ID)
	US		from_id;			// 发出号码(资源ID)
	UC		reserved1[6];		// 保留
	US		com_iid;			// COM接口ID
	UC		reserved2[30];		// 保留
	US		nd_parent;			// 父节点ID
	US		nd_child;			// 子节点ID
	UC		reserved3[12];		// 保留
};

//------------------------------------------------------------------------
// 节点055：传真接收
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_055{
	US		timeout;			// 节点超时(秒)
	UC		filenametype;		// 传真文件名类型: 
								/// 0 -- 自动生成（$Record\Group%n\%Y%m%d%H%M%S_c<CH>.tif）
								/// 1 -- 资源ID
								/// 2 -- 变量对应资源ID
								/// 3 -- 变量对应文件名
								/// 4 -- 变量替换资源中的通配符
	UC		var_faxfile;        // 传真文件使用变量ID: 0 --不使用；1 -- 255 变量ID
	UC		record_cdr;			// 是否记录传真接收详单: 0 --不记录；1 -- 记录
	UC		log;				// 被访问日志
	UC		var_fromno;         // [get]发出号码记录变量ID: 0 --不使用；1 -- 255 变量ID
	UC		var_tono;			// 接收号码记录变量ID: 0 --不使用；1 -- 255 变量ID
	UC		var_extno;			// 分机号码记录变量ID: 0 --不使用；1 -- 255 变量ID
    UC		var_result;         // 结果记录变量ID: 0 --不使用；1 -- 255 变量ID
	UC		var_appfield[3];	// 应用数据项记录变量
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_055{
	US		vox_op;				// 操作提示语音
	US		fax_fileid;			// 传真文件(资源ID)
	UC		reserved1[44];		// 保留
	US		nd_parent;			// 父节点ID
	US		nd_succ;			// 成功转节点ID
	US		nd_fail;			// 失败转节点ID
	UC		reserved2[10];		// 保留
};

//------------------------------------------------------------------------
// 节点060：转接座席
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_060{
	UC		timeout;			// 节点超时(秒)
	UC		switchtype;			// 转接方式(0：自动传接；1：指定座席；2：用户输入)
	UC		agentid;			// 指定座席ID, 方式1时 / 中断按键, 方式2时
	UC		getlength;			// 用户输入长度, 转接方式为2时有效；转接方式为1时为指定座席ID的高位
	UC		looptimes;			// 等待循环播放次数
	UC		log;				// 被访问日志
	UC		var_key;			// 用户按键记录, 转接方式为2时有效，1-255变量ID
	UC		agentinfo;			// 是否宣读座席信息, 0 不宣读；1 宣读
	UC		reserved1[5];		// 保留
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_060{
	US		vox_op;				// 操作提示语音
	US		vox_sw;				// 转接提示语音
	US		vox_wt;				// 等待循环播放语音
	US		vox_nobody;			// 没有上班提示语音
	US		vox_busy;			// 座席忙提示语音
	US		vox_noanswer;		// 座席无应答提示语音
	US		vox_ok;				// 座席转接成功
	UC		length_agentinfo;	// 座席信息（如：工号）位数，默认4位，不足在前面补0，0采用原始长度
	UC		reserved1[33];		// 保留
	US		nd_parent;			// 父节点ID
	US		nd_nobody;			// 没有上班转节点ID
	US		nd_busy;			// 座席忙转节点ID
	US		nd_noanswer;		// 座席无应答转节点ID
	US		nd_ok;				// 转接成功转节点ID
	UC		reserved2[6];		// 保留
};

//------------------------------------------------------------------------
// Sun added 2004-12-30
// 节点061：转接座席组
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_061{
    US		maxwait;			// 等待超时(秒)，0 表示无限等待
    UC		toacd;				// 转接ACD或是RoutePoint，0 － RoutePoint；1 － ACD
	UC		usevar;				// 使用变量ID: 0 --不使用；1 -- 255 变量ID
    UC		looptimes;          // 等待循环播放次数，0 表示无限循环
    UC		log;				// 被访问日志
    UC		agentinfo;			// 是否宣读座席信息, 0 不宣读；1 宣读
	UC		readEWT;			// 是否播报预测等待时间, 0 不宣读；1 宣读-秒；2 宣读-时分秒
	UC		switchtype;         // 转接方式: 0 - 拨号；1 - CTI
	UC		waitmethod;         // 等待方式: 0 - 播放语音；1 - 异步走流程
	UC		var_userid;			// 转接坐席ID记录变量ID: 0 --不记录；1 -- 255 变量ID
	UC		var_loginid;		// 转接坐席工号记录变量ID: 0 --不记录；1 -- 255 变量ID
    UC		waitansto;			// 等待坐席应答超时(秒)，0 表示无限等待
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_061{
	US		vox_op;				// 操作提示语音-节点开始（转接前）
	US		vox_sw;				// 转接提示语音-转接响应后（报EWT）
	US		vox_wt;				// 等待循环播放语音
	US		vox_nobody;			// 没有上班提示语音-转接响应后
	US		vox_busy;			// 座席组全忙提示语音-转接响应后
	US		vox_noanswer;		// 座席无应答提示语音
	US		vox_ok;				// 座席组转接成功
    US		routepointid;		// 转接的路由点编号
    UC		acddn[20];			// 转接的ACD号码
	UC		length_agentinfo;	// 座席信息（如：工号）位数，默认4位，不足在前面补0，0采用原始长度
	UC		reserved1[10];		// 保留
	UC		waitansto_hi;       // 保存等待座席应答超时的高8位
	US		nd_parent;			// 父节点ID
	US		nd_nobody;			// 没有上班转节点ID
	US		nd_busy;			// 座席组全忙转节点ID
	US		nd_noanswer;		// 座席无应答转节点ID（如果为0则继续等待）
	US		nd_ok;				// 转接成功转节点ID
	US		nd_wait;			// 等待流程入口节点号
	UC		reserved2[4];		// 保留
};

//------------------------------------------------------------------------
// 节点062：发起会议，2012-04-18 [B]
/// 节点数据结构-Data1(03-0F/13b)
/*
struct SData1_062{
	UC		reserved1[3];		// 保留
	UC		usevar;				// 使用变量ID: 0 --不使用；1 -- 255 变量ID
	UC		looptimes;			// 等待循环播放次数
	UC		log;				// 被访问日志
	UC		reserved2[7];		// 保留
};
*/
struct SData1_062{
    UC		timeout;			// 节点超时(秒)
    UC		reserved1;			// 保留
	UC		usevar;				// 使用变量ID: 0 --不使用；1 -- 255 变量ID
    UC		looptimes;          // 等待循环播放次数，0 表示使用系统默认次数
    UC		switchtype;			// 转接方式: 0 - 拨号（仅TAPI环境有效）；1 - CTI
    UC		log;				// 被访问日志
    UC		waitansto;			// 等待坐席应答超时(秒)，0 表示无限等待
	UC		reserved2[6];		// 保留
};
/// 节点数据结构-Data2(30-6F/64b)
/*
struct SData2_062{
	US		vox_op;				// 操作提示语音
	US		vox_sw;				// 转接提示语音
	US		vox_wt;				// 等待循环播放语音
	US		vox_nobody;			// 没有上班提示语音
	US		vox_busy;			// 座席全忙提示语音
	US		vox_ok;				// 座席转接成功
	UC		reserved1[36];		// 保留
	US		nd_parent;			// 父节点ID
	US		nd_nobody;			// 没有上班转节点ID
	US		nd_busy;			// 座席全忙转节点ID
	US		nd_ok;				// 转接成功转节点ID
	UC		reserved2[8];		// 保留
};
*/
struct SData2_062{
	US		vox_op;				// 操作提示语音
	US		vox_sw;				// 转接提示语音
	US		vox_wt;				// 等待循环播放语音
	US		vox_noconf;			// 磋商失败提示语音
	US		vox_noans;			// 无应答提示语音
	US		vox_ans;			// 成功应答提示语音
	US		vox_ok;				// 完成会议提示语音
	US		vox_syserror;		// 系统错误提示语音
	UC		DialNo[24];			// 会议号码，也可以是:变量名
	UC		predial[6];			// 拨号前缀
	UC		reserved1[2];		// 保留
	US		nd_parent;			// 父节点ID
	US		nd_noans;			// 无应答转节点ID
	US		nd_failed;			// 会议失败转节点ID
	US		nd_ok;				// 会议成功转节点ID
	UC		reserved2[8];		// 保留
};

//------------------------------------------------------------------------
// 节点063：增强转接座席组
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_063{
	UC		reserved1[3];		// 保留
	UC		usevar;				// 使用变量ID: 0 --不使用；1 -- 255 变量ID
	UC		looptimes;			// 等待循环播放次数
	UC		log;				// 被访问日志
	UC		reserved2[7];		// 保留
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_063{
	US		vox_op;				// 操作提示语音
	US		vox_sw;				// 转接提示语音
	US		vox_wt;				// 等待循环播放语音
	US		vox_nobody;			// 没有上班提示语音
	US		vox_busy;			// 座席全忙提示语音
	US		vox_ok;				// 座席转接成功
	UC		reserved1[36];		// 保留
	US		nd_parent;			// 父节点ID
	US		nd_nobody;			// 没有上班转节点ID
	US		nd_busy;			// 座席全忙转节点ID
	US		nd_ok;				// 转接成功转节点ID
	UC		reserved2[8];		// 保留
};

//------------------------------------------------------------------------
// 节点066：建立会议
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_066{
	US		maxwait;			// 等待超时(秒)，0 表示无限等待
	UC		waitansto;			// 等待坐席应答超时(秒)，0 表示无限等待
	UC		usevar;				// 使用变量ID: 0 --不使用；1 -- 255 变量ID
	UC		looptimes;			// 等待循环播放次数
	UC		log;				// 被访问日志
	UC		switchtype;         // 会议方式: 0 - 拨号；1 - CTI
	UC		var_answerdn;		// 会议应答号码记录变量ID: 0 --不记录；1 -- 255 变量ID
	UC		reserved1[5];		// 保留
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_066{
	US		vox_op;				// 操作提示语音
	US		vox_wt;				// 等待循环播放语音
	US		vox_busy;			// 目标忙提示语音
	US		vox_noanswer;		// 目标无应答提示语音
	US		vox_ok;				// 目标成功应答提示语音
	UC		reserved1[38];		// 保留
	US		nd_parent;			// 父节点ID
	US		nd_busy;			// 目标忙转节点ID
	US		nd_noanswer;		// 目标无应答转节点ID
	US		nd_ok;				// 会议成功转节点ID
	UC		reserved2[8];		// 保留
};

//------------------------------------------------------------------------
// 节点069：转虚拟分机
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_069{
	UC		reserved1[3];		// 保留
	UC		usevar;				// 使用变量ID: 0 --不使用；1 -- 255 变量ID
	UC		switchtype;         // 转接方式: 0 - 拨号；1 - CTI
	UC		log;				// 被访问日志
	UL		vagency;			// 虚拟分机号码
	UC		reserved3[3];		// 保留
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_069{
	US		vox_op;				// 操作提示语音
    US		maxtry;				// 最大尝试次数，2012-04-18 [C]
    US		tryinterval;		// 尝试间隔，2012-04-18 [C]
	UC		reserved1[42];		// 保留
	US		nd_parent;			// 父节点ID
	US		nd_child;			// 子节点ID
	UC		reserved2[12];		// 保留
};

//------------------------------------------------------------------------
// 节点070：查询路由点
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_070{
    UC		timeout;			// 节点超时(秒)
	UC		reserved1;			// 保留
	UC		usevar;				// 使用变量ID: 0 --不使用；1 -- 255 变量ID
    UC		paramindex;			// 参数编号
    UC		logic;				// 逻辑运算符
    UC		log;				// 被访问日志
	UC		querytype;          // 查询类型，0 - 路由点；1 - 队列；2 - 组；3 - 小组
    UC		var_result;			// 查询结果记录变量: 0 -- 不使用；1 -- 255 变量ID，[2007-04-12]
    UC		reserved2[5];		// 保留
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_070{
    US		routepointid;		// 路由点编号
    US		comparedvalue;		// 参考值
    UC		reserved1[12];		// 保留
    US		com_iid;			// COM接口ID
    UC		reserved2[30];		// 保留
    US		nd_parent;			// 父节点ID
    US		nd_yes;				// 满足条件转节点ID
    US		nd_no;				// 不满足条件转节点ID
    US		nd_fail;			// 查询失败节点ID
    UC		reserved3[8];		// 保留
};

//------------------------------------------------------------------------
// 节点071：查询座席状态
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_071{
    UC		timeout;			// 节点超时(秒)
	UC		usevar;				// 使用变量ID: 0 --不使用；1 -- 255 变量ID
    UC		dn_status;			// DN状态编号
	UC		pos_status;			// POS状态编号
    UC		dn_logic;			// DN逻辑运算符
	UC		pos_logic;			// POS逻辑运算符
    UC		log;				// 被访问日志
	UC		querytype;          // 查询类型，0 - Agent; 1 - User (必须使用变量确定用户名); 2 - 工号
	UC		conditions;			// 条件，参见“条件定义”
    UC		reserved1[4];		// 保留
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_071{
    UL		agentid;			// 座席编号
    UC		reserved1[44];		// 保留
    US		nd_parent;			// 父节点ID
    US		nd_yes;				// 满足条件转节点ID
    US		nd_no;				// 不满足条件转节点ID
    US		nd_fail;			// 查询失败节点ID
    UC		reserved2[8];		// 保留
};

/// 条件定义
#define DEF_NODE071_CONDITION_NONE		0
#define DEF_NODE071_CONDITION_FIRST		1
#define DEF_NODE071_CONDITION_SECOND	2
#define DEF_NODE071_CONDITION_BOTH		3
#define DEF_NODE071_CONDITION_EITHER	4

//------------------------------------------------------------------------
// 节点080：进入ACD排队
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_080{
	UC		reserved1[3];		// 保留
	UC		usevar;				// 使用变量ID: 0 --不使用；1 -- 255 变量ID
	UC		reserved2;			// 保留
	UC		log;				// 被访问日志
	US		maxwaittime;		// 最长排队等待时间(秒)
	UC		reserved3[5];		// 保留
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_080{
	US		vox_wait;			// 排队等待语音
	UC		reserved1[14];		// 保留
	US		com_iid;			// COM接口ID
	UC		reserved2[30];		// 保留
	US		nd_parent;			// 父节点ID
	US		nd_child;			// 子节点ID
	UC		reserved3[12];		// 保留
};

//------------------------------------------------------------------------
// 节点090：呼叫外线号码
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_090{
	UC		timeout;			// 外拨超时，秒
    UC		numbertype;         // 号码类型: 0 -- 固定设定；1 -- 变量；2 -- 外拨消息；3 -- COM
    UC		dialtype;           // 拨号类型: 0 -- 语音卡拨号；1 -- CTI拨号
    UC		connecttype;        // 拨号接通判别: 0 -- 语音卡判别；1 -- CTI消息判别
    UC		trytimes;           // 尝试次数
	UC		log;				// 被访问日志
    UC		extdelay;           // 拨分机前延时，秒
    UC		usevar;             // 使用变量ID: 0 --不使用；1 -- 255 变量ID
    UC		resultvar;          // 结果记录变量ID: 0 --不使用；1 -- 255 变量ID
    UC		resultinform;       // 结果是否通知外部系统
	UC		explictoffhook;     // 是否强制摘机，Sun added 2012-01-17
    UC		reserved1[2];       // 保留
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_090{
    UC		predial[14];        // 拨号前缀
    UC		phoneno[32];        // 被叫号码，号码类型为0时有效
	US		com_iid;			// COM接口ID
	US		nd_parent;			// 父节点ID
	US		nd_succ;			// 成功转节点ID
	US		nd_fail;			// 失败转节点ID
    US		setANI;				// 显式设置主叫号码（资源ID），Sun added 2012-06-26
	UC		reserved1[8];		// 保留
};

//------------------------------------------------------------------------
// 节点091：Calling Card
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_091{
	UC		timeout;			// 节点超时，秒
    UC		talklentype;        // 可通话时长播报方式: 0 -- 不需要播报；1 -- 时分秒；2 -- 分秒；3 -- 分（四舍五入）；4 -- 秒
	UC		obgroup;			// 外拨组号
	UC		remindminute;		// 通话时间剩余多少分钟提醒
	UC		reserved1;			// 保留
	UC		log;				// 被访问日志
    UC		reserved2;			// 保留
    UC		var_cardno;			// 卡号使用变量ID: 0 --不使用；1 -- 255 变量ID
    UC		var_telno;			// 外拨号码使用变量ID: 0 --不使用；1 -- 255 变量ID
    UC		var_connectlength;	// 通话时长记录变量ID: 0 --不记录；1 -- 255 变量ID
    UC		reserved3[3];		// 保留
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_091{
    US		vox_talklen;		// 播报可通话时长前导语音（系统自动根据talklentype播报，时间单位使用系统语音资源）
    US		vox_timeout;		// 通话超时提醒语音
	US		vox_noservice;		// 余额不足或卡无效提示语音
	UC		reserved1[38];		// 保留
    US		com_talklength;		// 预先计算可通话时长COM接口ID（返回时分秒），播报方式大于0时有效
    US		com_billing;		// 计费COM接口ID（返回本次通话金额）
	US		nd_parent;			// 父节点ID
	US		nd_child;			// 子节点ID
	UC		reserved2[12];		// 保留
};

//------------------------------------------------------------------------
// 节点096：异步通信节点
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_096{
    UC		timeout;			// 等待超时，秒
    UC		seperator;			// 分隔符
    UC		extdata;			// 扩展数据处理方式: 0 -- 不处理；1 -- 保存文件；2 -- 记录到变量；3 -- 记录到AppData；4 -- 记录到UserData；5 -- TTS；
	UC		extvar;				// extdata记录变量
    UC		reserved1;			// 保留
    UC		log;				// 被访问日志
	UC		carryonasynplay;	// 是否收到通信响应是否继续异步放音：默认为0-不继续
    UC		reserved2[6];		// 保留
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_096{
    US		command;			// 命令代码
    US		vox_wt;				// 等待循环播放语音
    UC		var_send[10];		// 发送变量ID
    UC		var_receive[10];	// 接收变量ID
	UC		fileprefix[2];		// 文件名前缀
    UC		reserved1[22];		// 保留
    US		nd_parent;			// 父节点ID
    US		nd_child;			// 子节点ID
    US		nd_timeout;			// 超时节点ID
    UC		reserved2[10];		// 保留
};

//------------------------------------------------------------------------
// 节点100：用户DLL
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_100{
	UC		reserved1[5];		// 保留
	UC		log;				// 被访问日志
	UC		reserved2[7];		// 保留
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_100{
	US		dll_fid;			// DLL文件ID
	UC		reserved1[46];		// 保留
	US		nd_parent;			// 父节点ID
	US		nd_child;			// 子节点ID
	UC		reserved2[12];		// 保留
};

//------------------------------------------------------------------------
// 节点101：用户COM
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_101{
	UC		reserved1[5];		// 保留
	UC		log;				// 被访问日志
	UC		reserved2[7];		// 保留
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_101{
	UC		reserved1[16];		// 保留
	US		com_iid;			// COM接口ID
	UC		reserved2[30];		// 保留
	US		nd_parent;			// 父节点ID
	US		nd_child;			// 子节点ID
	UC		reserved3[12];		// 保留
};

//------------------------------------------------------------------------
// Sun update 2004-12-30
// 节点102：记录变量
/// 节点数据结构-Data1(03-0F/13b)
struct SData1_102{
    UC		reserved1[5];		// 保留
    UC		log;                // 被访问日志
    UC		var_chg;	        // 记录变量ID: 0 --不记录；1 -- 255 变量ID
    UC		convert;            // 转换公式
    UC		param1;             // 参数1
    UC		param2;             // 参数2
    UC		reserved2[3];       // 保留
};
/// 节点数据结构-Data2(30-6F/64b)
struct SData2_102{
	UC		reserved1[16];		// 保留
	US		com_iid;			// COM接口ID
	UC		value[24];			// 变量值
	UC		reserved2[6];		// 保留
	US		nd_parent;			// 父节点ID
	US		nd_child;			// 子节点ID
	UC		reserved3[12];		// 保留
};

//------------------------------------------------------------------------
// Sun added 2002-05-23
// Ver 2 Mend 1
// Exchange File Header Definition
// 512 Bytes
struct SCFFileHeader{
    UC		P_ID_Name[10];		// 分割标记                  (10)
    UC		P_ID;				// 项目ID                    (1)
    UC		P_Type_Name[10];	// 分割标记                  (10)
    UC		P_Type;				// 项目类型                  (1)
    UC		P_Name_Name[10];	// 分割标记                  (10)
    UC		P_Name[50];			// 项目名称                  (50)
    UC		P_Description_Name[10];	// 分割标记               (10)
    UC		P_Description[200];	// 项目描述文字              (200)
    UC		P_Version_Name[10];	// 分割标记                  (10)
    UC		P_Version[10];		// 项目版本号                (10)
    UC		P_Auther_Name[10];	// 分割标记                  (10)
    UC		P_Auther[50];		// 项目作者                  (50)
    UC		P_User_Name[10];	// 分割标记                  (10)
    UC		P_User[50];			// 项目用户                  (50)
    UC		P_CreateTime_Name[10];	// 分割标记                (10)
    UC		P_CreateTime[8];	// 项目创建时间              (8)
    UC		P_ModifyTime_Name[10];	// 分割标记                (10)
    UC		P_ModifyTime[8];	// 项目最后修改时间          (8)
    UC		P_RCN_Name[10];		// 分割标记                  (10)
    US		P_RCN;				// 记录数                    (2)
	US		P_RCN1;				// 记录数                    (2)
    UC		P_Reserved_Name[10];	// 分割标记                  (10)
    UC		Reserved[20];		// 填充保留                  (20)
};

// Exchange File Record Definition
// 128 Bytes
struct SCFFileRecord{
    US		N_ID;				// 节点ID                    (2)
    UC		N_NO;				// 节点编号                  (1)
    UC		N_Description[32];	// 节点描述文字              (32)
    UC		N_Page;				// 节点所在页码              (1)
    US		N_Left;				// 节点左坐标                (2)
    US		N_Top;				// 节点上坐标                (2)
    US		N_Height;			// 节点高度                  (2)
    US		N_Width;			// 节点宽度                  (2)
    UC		N_Data1[DEF_NODE_DATA_SMALL_LEN];	// 节点数据段1               (13)
    UC		N_Data2[DEF_NODE_DATA_BIG_LEN];		// 节点数据段2               (64)
    UC		Reserved[7];		// 保留                      (7)
};

//------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------