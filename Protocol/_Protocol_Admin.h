////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		_Protocol_Admin.h
// 用途描述：	管理控制台通信协议
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
#ifndef __PROTOCOL_ADMIN_H__
#define __PROTOCOL_ADMIN_H__
//------------------------------------------------------------------------

//--------------------------
// 注册 - 2
//------------
#define TCP_ADM2PCS_REGISTER				1			// (0x0001) 注册消息
struct S_TCP_AMD2PCS_REGISTER
{
	UC		Version;						// 版本号
	UC		RegCode[PCS_DEF_NUMLEN_APP];	// 注册码
};
//------------

//------------
#define TCP_PCS2ADM_REGISTER_ACK			1001		// (0x03E9) 注册响应消息
struct S_TCP_PCS2ADM_REGISTER_ACK
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
#define TCP_ADM2PCS_UNREGISTER				2			// (0x0002) 取消注册消息
//------------

//------------
#define TCP_PCS2ADM_UNREGISTER_ACK			1002		// (0x03EA) 取消注册响应消息
//------------
//--------------------------


//--------------------------
// 座席管理 - 6
//------------
#define TCP_ADM2PCS_ADD_AGENT				901			// (0x0385) 新增座席消息
struct S_TCP_ADM2PCS_ADD_AGENT
{
	UL		AgentID;						// 座席ID
	UC		IsMonitor;						// 是否是监控者
};
//------------

//------------
#define TCP_ADM2PCS_DEL_AGENT				902			// (0x0386) 删除座席消息
struct S_TCP_ADM2PCS_DEL_AGENT
{
	UL		AgentID;						// 座席ID
	UC		IsMonitor;						// 是否是监控者
};
//------------

//------------
#define TCP_ADM2PCS_MOD_AGENT				903			// (0x0387) 修改座席消息
struct S_TCP_ADM2PCS_MOD_AGENT
{
	UL		AgentID;						// 座席ID
	UC		IsMonitor;						// 是否是监控者
};
//------------

//------------
#define TCP_PCS2ADM_ADD_AGENT_ACK			1901		// (0x076D) 新增座席响应消息
struct S_TCP_PCS2ADM_ADD_AGENT_ACK
{
	UL		AgentID;						// 座席ID
	UC		IsMonitor;						// 是否是监控者
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_BEUSED
											// PCS_DEF_RETCODE_NOLICENSE
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2ADM_DEL_AGENT_ACK			1902		// (0x076E) 删除座席响应消息
struct S_TCP_PCS2ADM_DEL_AGENT_ACK
{
	UL		AgentID;						// 座席ID
	UC		IsMonitor;						// 是否是监控者
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2ADM_MOD_AGENT_ACK			1903		// (0x076F) 修改座席响应消息
struct S_TCP_PCS2ADM_MOD_AGENT_ACK
{
	UL		AgentID;						// 座席ID
	UC		IsMonitor;						// 是否是监控者
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------


//--------------------------
// 用户管理 - 6
//------------
#define TCP_ADM2PCS_ADD_USER				906			// (0x038A) 新增用户消息
struct S_TCP_ADM2PCS_ADD_USER
{
	char	strAliasName[50];				// 登录别名
};
//------------

//------------
#define TCP_ADM2PCS_DEL_USER				907			// (0x038B) 删除用户消息
struct S_TCP_ADM2PCS_DEL_USER
{
	char	strAliasName[50];				// 登录别名
};
//------------

//------------
#define TCP_ADM2PCS_MOD_USER				908			// (0x038C) 修改用户消息
struct S_TCP_ADM2PCS_MOD_USER
{
	char	strAliasName[50];				// 登录别名
};
//------------

//------------
#define TCP_PCS2ADM_ADD_USER_ACK			1906		// (0x0772) 新增用户响应消息
struct S_TCP_PCS2ADM_ADD_USER_ACK
{
	char	strAliasName[50];				// 登录别名
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_BEUSED
											// PCS_DEF_RETCODE_NOLICENSE
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2ADM_DEL_USER_ACK			1907		// (0x0773) 删除用户响应消息
struct S_TCP_PCS2ADM_DEL_USER_ACK
{
	char	strAliasName[50];				// 登录别名
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_BEUSED
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2ADM_MOD_USER_ACK			1908		// (0x0774) 修改用户响应消息
struct S_TCP_PCS2ADM_MOD_USER_ACK
{
	char	strAliasName[50];				// 登录别名
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------


//--------------------------
// 远程控制 - 16
//------------
//------------
#define TCP_ADM2PCS_RELOAD_AGENTLIST		911			// (0x038F) 重新加载座席列表消息
//------------

//------------
#define TCP_ADM2PCS_RELOAD_MONITORLIST		912			// (0x0390) 重新加载监控列表消息
//------------

//------------
#define TCP_ADM2PCS_RELOAD_ACD_SCENARIO		913			// (0x0391) 重新加载排队规则消息
//------------

//------------
#define TCP_ADM2PCS_RELOAD_SYSPARAM			914			// (0x0392) 重新加载系统参数表
//------------

//------------
#define TCP_ADM2PCS_RELOAD_USERLIST			915			// (0x0393) 重新加载用户列表消息
//------------

//------------
#define TCP_ADM2PCS_RELOAD_PBXACDLIST		916			// (0x0394) 重新加载ACD监控列表消息
//------------

//------------
#define TCP_ADM2PCS_RELOAD_USERPARAM		917			// (0x0395) 重新加载用户参数消息
struct S_TCP_ADM2PCS_RELOAD_USERPARAM
{
	char	strAliasName[50];				// 登录别名
};
//------------

//------------
#define TCP_ADM2PCS_RELOAD_CODELIST			918			// (0x0396) 更新代码表消息
struct S_TCP_ADM2PCS_RELOAD_CODELIST
{
	UC		bytCodeListID;					// 代码表ID，参见代码表ID定义
};

/// 代码表ID定义
#define PCS_DEF_CODELIST_COUNTRYCODE		1			// 国家代码表
#define PCS_DEF_CODELIST_CITYCODE			2			// 区号表
#define PCS_DEF_CODELIST_SPECIALTELNO		3			// 特殊电话号码表
#define PCS_DEF_CODELIST_IPPREFIX			4			// IP前缀表
#define PCS_DEF_CODELIST_MOBILEAREA			5			// 手机归属地表
//------------

//------------
#define TCP_PCS2ADM_RELOAD_AGENTLIST_ACK	1911		// (0x0777) 重新加载座席列表响应消息
struct S_TCP_PCS2ADM_RELOAD_AGENTLIST_ACK
{
	US		LoadAgents;						// 加载座席数量
	US		ReclaimAgents;					// 回收座席数量
};
//------------

//------------
#define TCP_PCS2ADM_RELOAD_MONITORLIST_ACK	1912		// (0x0778) 重新加载监控列表响应消息
struct S_TCP_PCS2ADM_RELOAD_MONITORLIST_ACK
{
	US		LoadMonitors;					// 加载监控数量
	US		ReclaimMonitors;				// 回收监控数量
};
//------------

//------------
#define TCP_PCS2ADM_RELOAD_ACD_SCENARIO_ACK	1913		// (0x0779) 重新加载排队规则响应消息
struct S_TCP_PCS2ADM_RELOAD_ACD_SCENARIO_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2ADM_RELOAD_SYSPARAM_ACK		1914		// (0x077A) 重新加载系统参数表响应消息
struct S_TCP_PCS2ADM_RELOAD_SYSPARAM_ACK
{
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2ADM_RELOAD_USERLIST_ACK		1915		// (0x077B) 重新加载用户列表响应消息
struct S_TCP_PCS2ADM_RELOAD_USERLIST_ACK
{
	US		LoadUsers;						// 加载用户数量
	US		ReclaimUsers;					// 回收用户数量
};
//------------

//------------
#define TCP_PCS2ADM_RELOAD_PBXACDLIST_ACK	1916		// (0x077C) 重新加载ACD监控列表响应消息
struct S_TCP_PCS2ADM_RELOAD_PBXACDLIST_ACK
{
	US		LoadACDDNs;						// 加载ACD DN数量
	US		ReclaimACDDNs;					// 回收ACD DN数量
};
//------------

//------------
#define TCP_PCS2ADM_RELOAD_USERPARAM_ACK	1917		// (0x077B) 重新加载用户参数表响应消息
struct S_TCP_PCS2ADM_RELOAD_USERPARAM_ACK
{
	char	strAliasName[50];				// 登录别名
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2ADM_RELOAD_CODELIST_ACK		1918		// (0x077C) 更新代码表响应消息
struct S_TCP_PCS2ADM_RELOAD_CODELIST_ACK
{
	UC		bytCodeListID;					// 代码表ID，参见代码表ID定义
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------


//--------------------------
// 参数管理 - 8
//------------
//------------
#define TCP_ADM2PCS_GET_SYSPARAM			921			// (0x0399) 查询系统参数
struct S_TCP_ADM2PCS_GET_SYSPARAM
{
	char	strParamName[50];				// 参数名
};
//------------

//------------
#define TCP_ADM2PCS_CHG_SYSPARAM			922			// (0x039A) 修改系统参数
struct S_TCP_ADM2PCS_CHG_SYSPARAM
{
	char	strParamName[50];				// 参数名
	char	strParamValue[100];				// 参数值
};
//------------

//------------
#define TCP_ADM2PCS_GET_USERPARAM			923			// (0x039B) 查询用户参数
struct S_TCP_ADM2PCS_GET_USERPARAM
{
	char	strAliasName[50];				// 登录别名
	char	strParamName[50];				// 参数名
};
//------------

//------------
#define TCP_ADM2PCS_CHG_USERPARAM			924			// (0x039C) 修改用户参数
struct S_TCP_ADM2PCS_CHG_USERPARAM
{
	char	strAliasName[50];				// 登录别名
	char	strParamName[50];				// 参数名
	char	strParamValue[100];				// 参数值
};
//------------

//------------
#define TCP_PCS2ADM_GET_SYSPARAM_ACK		1921		// (0x0781) 查询系统参数响应消息
struct S_TCP_PCS2ADM_GET_SYSPARAM_ACK
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
#define TCP_PCS2ADM_CHG_SYSPARAM_ACK		1922		// (0x0782) 修改系统参数响应消息
struct S_TCP_PCS2ADM_CHG_SYSPARAM_ACK
{
	char	strParamName[50];				// 参数名
	char	strParamValue[100];				// 参数值
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
#define TCP_PCS2ADM_GET_USERPARAM_ACK		1923		// (0x0783) 查询用户参数响应消息
struct S_TCP_PCS2ADM_GET_USERPARAM_ACK
{
	char	strAliasName[50];				// 登录别名
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
#define TCP_PCS2ADM_CHG_USERPARAM_ACK		1924		// (0x0784) 修改用户参数响应消息
struct S_TCP_PCS2ADM_CHG_USERPARAM_ACK
{
	char	strAliasName[50];				// 登录别名
	char	strParamName[50];				// 参数名
	char	strParamValue[100];				// 参数值
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------

//--------------------------
// 系统维护 - 2
//------------
#define TCP_ADM2PCS_CONNECTION_RESET		981			// (0x03D5) 连接复位消息
struct S_TCP_ADM2PCS_CONNECTION_RESET
{
	UC		SysType;						// 系统类别
	UC		SysNo;							// 系统代号
};
//------------

//------------
#define TCP_PCS2ADM_CONNECTION_RESET_ACK	1981		// (0x07BD) 连接复位响应消息
struct S_TCP_PCS2ADM_CONNECTION_RESETE_ACK
{
	UC		SysType;						// 系统类别
	UC		SysNo;							// 系统代号
	UC		ErrorCode;						// 返回码，参见“返回码定义”：
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		ThreadID;						// 线程ID
};
//------------
//--------------------------

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_ADMIN_H__
//------------------------------------------------------------------------
