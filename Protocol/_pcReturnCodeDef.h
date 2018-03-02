////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		_pcReturnCodeDef.h
// 用途描述：   PowerCall返回代码定义
// 作者：		SUN
// 创建时间：	2004/06/09
// 最后修改时间：2004/06/09
// 版权信息：	Copyright (C) 1998 - 2004 PicaSoft
// 备注：
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PCRETURNCODEDEF_H__
#define __PCRETURNCODEDEF_H__
//------------------------------------------------------------------------

typedef enum pcsReturnCode
{
	pcsRC_Client_StatusError = -10,					// 状态错误
	pcsRC_Client_Conflict = -9,						// 冲突

	pcsRC_Client_Timeout = -6,						// 操作超时
	pcsRC_Client_SocketSendError = -5,				// 网络发送出错
	pcsRC_Client_ConnectionError = -4,				// 连接错误
	pcsRC_Client_MissUserName = -3,					// 缺少用户名
	pcsRC_Client_MissServerPort = -2,				// 缺少服务器端口
	pcsRC_Client_MissServerIP = -1,					// 缺少服务器地址
	pcsRC_OK = 0,									// 成功
	pcsRC_Server_NoExist = 1,						// 不存在
	pcsRC_Server_BeUsed = 2,						// 已占用
	pcsRC_Server_StatusError = 3,					// 状态错
	pcsRC_Server_NoNeeds = 4,						// 不需要
	pcsRC_Server_WrongVersion =	10,					// 版本错误
	pcsRC_Server_WrongRegisterCode = 11,			// 注册码错误
	pcsRC_Server_PasswordError = 12,				// 口令错
	pcsRC_Server_NoRights =	13,						// 无权限
	pcsRC_Server_NoLicense = 14,					// 许可数量不足
	pcsRC_Server_LACK_PARAM	= 15,					// 参数不足
	pcsRC_Server_Rejected =	16,						// (0x10) 请求被拒绝 

	pcsRC_Server_NotSupport = 99,					// (0x63) 不支持

	pcsRC_Server_OthersError = 100,					// 其它错误
	
	pcsRC_Server_SystemError = 255					// 系统错
} pcsReturnCode;

//------------------------------------------------------------------------
#endif  //End #ifndef #define __PCRETURNCODEDEF_H__
//------------------------------------------------------------------------
