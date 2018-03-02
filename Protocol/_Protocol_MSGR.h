////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		_Protocol_MSGR.h
// ��;������	��Ϣ����ͨ��Э��
// ���ߣ�		SUN
// ����ʱ�䣺	1999/12/01
// ����޸�ʱ�䣺2004/12/15
// ��Ȩ��Ϣ��	Copyright (C) 1999 - 2004 PCSG
// ��ע��
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_MSGR_H__
#define __PROTOCOL_MSGR_H__
//------------------------------------------------------------------------

//--------------------------
// ע�� - 2
//------------
#define TCP_MSG2PCS_REGISTER				1			// (0x0001) ע����Ϣ
struct S_TCP_MSG2PCS_REGISTER
{
	UC		Version;						// �汾��
	UC		RegCode[PCS_DEF_NUMLEN_APP];	// ע����
	US		nMSGPort;						// ��Ϣ�����������˿�
	char	strMSGIP[20];					// ��Ϣ������IP��ַ
};
//------------

//------------
#define TCP_PCS2MSG_REGISTER_ACK			1001		// (0x03E9) ע����Ӧ��Ϣ
struct S_TCP_PCS2MSG_REGISTER_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
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
// ȡ��ע�� - 2
//------------
#define TCP_MSG2PCS_UNREGISTER				2			// (0x0002) ȡ��ע����Ϣ
//------------

//------------
#define TCP_PCS2MSG_UNREGISTER_ACK			1002		// (0x03EA) ȡ��ע����Ӧ��Ϣ
//------------
//--------------------------

//--------------------------
// Զ�̿���(ͬRM�й���Ϣ) - 4
//------------
//------------
#define TCP_MSG2PCS_RELOAD_ACD_SCENARIO_ACK	351			// (0x015F) ���¼����Ŷӹ�����Ӧ��Ϣ
struct S_TCP_MSG2PCS_RELOAD_ACD_SCENARIO_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_MSG2PCS_RELOAD_USERLIST_ACK		352			// (0x0160) ���¼����û��б���Ӧ��Ϣ
struct S_TCP_MSG2PCS_RELOAD_USERLIST_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2MSG_RELOAD_ACD_SCENARIO		1351		// (0x0547) ���¼����Ŷӹ�����Ϣ
//------------


//------------
#define TCP_PCS2MSG_RELOAD_USERLIST			1352		// (0x0548) ���¼����û��б���Ϣ
//------------

//--------------------------

//--------------------------
// Sun added 2010-02-12
// ״̬��ѯ - 2
//------------
#define TCP_MSG2PCS_STATUS_QUERY			306			// (0x0132) ��ѯ״̬��Ϣ
struct S_TCP_MSG2PCS_STATUS_QUERY
{
	UL		AgentID;						// ��ϯID
};
//------------

//------------
#define TCP_PCS2MSG_STATUS_QUERY_ACK		1306		// (0x051A) ��ѯ״̬��Ӧ��Ϣ
struct S_TCP_PCS2MSG_STATUS_QUERY_ACK
{
	UL		AgentID;						// ��ϯID
	UC		DNStatus;						// ����״̬������_pcC_stDN
	UL		DNStDur;						// ����״̬����ʱ�����룩
	UC		PositionStatus;					// λ��״̬������_pcC_stPOS
	UL		PosStDur;						// λ��״̬����ʱ�����룩
	UC		LeaveReason;					// ��ϯԭ���Զ���
	char	strAliasName[50];				// ��¼����
};
//------------
//--------------------------

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_MSGR_H__
//------------------------------------------------------------------------
