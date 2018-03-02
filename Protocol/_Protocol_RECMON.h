////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		_Protocol_RECMON.h
// 用途描述：	录音监控通信协议
// 作者：		SUN
// 创建时间：	1999/12/01
// 最后修改时间：2003/12/15
// 版权信息：	Copyright (C) 1999 - 2003 PCSG
// 备注：
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_RECORDMONITOR_H__
#define __PROTOCOL_RECORDMONITOR_H__
//------------------------------------------------------------------------

//--------------------------
// 录音监控注册 - Total 4
//--------------------------

//--------------------------
// 注册 - 2
//------------
#define TCP_MON2REC_REGISTER				1			// (0x0001) 注册消息
struct S_TCP_MON2REC_REGISTER
{
	UC		Version;						// 版本号
	UC		RegCode[PCS_DEF_NUMLEN_APP];	// 注册码
};
//------------

//------------
#define TCP_REC2MON_REGISTER_ACK			1001		// (0x03E9) 注册响应消息
struct S_TCP_REC2MON_REGISTER_ACK
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
#define TCP_MON2REC_UNREGISTER				2			// (0x0002) 取消注册消息
//------------

//------------
#define TCP_REC2MON_UNREGISTER_ACK			1002		// (0x03EA) 取消注册响应消息
//------------
//--------------------------

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_RECORDMONITOR_H__
//------------------------------------------------------------------------
