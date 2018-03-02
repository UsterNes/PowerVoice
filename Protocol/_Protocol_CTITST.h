////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		_Protocol_CTITST.h
// ��;������	CTI���Կͻ���ͨ��Э��
// ���ߣ�		SUN
// ����ʱ�䣺	1999/12/01
// ����޸�ʱ�䣺2003/10/10
// ��Ȩ��Ϣ��	Copyright (C) 1999 - 2003 PCSG
// ��ע��
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_CTITST_H__
#define __PROTOCOL_CTITST_H__
//------------------------------------------------------------------------

//--------------------------
// ע�� - 2
//------------
#define TCP_TST2PCS_REGISTER				1			// (0x0001) ע����Ϣ
struct S_TCP_TST2PCS_REGISTER
{
	UC		Version;						// �汾��
	UC		RegCode[PCS_DEF_NUMLEN_APP];	// ע����
	UC		DeviceType;						// �豸���ͣ�����_pcC_EventSource
	char	strDeviceID[PCS_DEF_NUMLEN_APP];		// �豸��
};
//------------

//------------
#define TCP_PCS2TST_REGISTER_ACK			1001		// (0x03E9) ע����Ӧ��Ϣ
struct S_TCP_PCS2TST_REGISTER_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_VERSION
											// PCS_DEF_RETCODE_REGCODE
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------

//--------------------------
// ȡ��ע�� - 2
//------------
#define TCP_TST2PCS_UNREGISTER				2			// (0x0002) ȡ��ע����Ϣ
//------------

//------------
#define TCP_PCS2TST_UNREGISTER_ACK			1002		// (0x03EA) ȡ��ע����Ӧ��Ϣ
//------------
//--------------------------

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_CTITST_H__
//------------------------------------------------------------------------
