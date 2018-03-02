////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		_pcReturnCodeDef.h
// ��;������   PowerCall���ش��붨��
// ���ߣ�		SUN
// ����ʱ�䣺	2004/06/09
// ����޸�ʱ�䣺2004/06/09
// ��Ȩ��Ϣ��	Copyright (C) 1998 - 2004 PicaSoft
// ��ע��
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
	pcsRC_Client_StatusError = -10,					// ״̬����
	pcsRC_Client_Conflict = -9,						// ��ͻ

	pcsRC_Client_Timeout = -6,						// ������ʱ
	pcsRC_Client_SocketSendError = -5,				// ���緢�ͳ���
	pcsRC_Client_ConnectionError = -4,				// ���Ӵ���
	pcsRC_Client_MissUserName = -3,					// ȱ���û���
	pcsRC_Client_MissServerPort = -2,				// ȱ�ٷ������˿�
	pcsRC_Client_MissServerIP = -1,					// ȱ�ٷ�������ַ
	pcsRC_OK = 0,									// �ɹ�
	pcsRC_Server_NoExist = 1,						// ������
	pcsRC_Server_BeUsed = 2,						// ��ռ��
	pcsRC_Server_StatusError = 3,					// ״̬��
	pcsRC_Server_NoNeeds = 4,						// ����Ҫ
	pcsRC_Server_WrongVersion =	10,					// �汾����
	pcsRC_Server_WrongRegisterCode = 11,			// ע�������
	pcsRC_Server_PasswordError = 12,				// �����
	pcsRC_Server_NoRights =	13,						// ��Ȩ��
	pcsRC_Server_NoLicense = 14,					// �����������
	pcsRC_Server_LACK_PARAM	= 15,					// ��������
	pcsRC_Server_Rejected =	16,						// (0x10) ���󱻾ܾ� 

	pcsRC_Server_NotSupport = 99,					// (0x63) ��֧��

	pcsRC_Server_OthersError = 100,					// ��������
	
	pcsRC_Server_SystemError = 255					// ϵͳ��
} pcsReturnCode;

//------------------------------------------------------------------------
#endif  //End #ifndef #define __PCRETURNCODEDEF_H__
//------------------------------------------------------------------------
