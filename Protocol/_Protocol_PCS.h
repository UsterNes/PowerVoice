////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		_Protocol_PCS.h
// ��;������	ͨ��Э��
// ���ߣ�		Simon
// ����ʱ�䣺	2010/05/04
// ����޸�ʱ�䣺2010/05/04
// ��Ȩ��Ϣ��	Copyright (C) 1999 - 2010 PicaSoft
// ��ע��
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_PCS_H__
#define __PROTOCOL_PCS_H__

// �¼���ϵͳ״̬����
#define TCP_PCS2PCS_SYSTEMSTATUSREPORT_EVT		11004		
struct S_TCP_PCS2PCS_SYSTEMSTATUSREPORT_EVT
{
	UC		PCSID;								// Base 0
	UC		PCSStatus;							// �ο���PCS_PCSStatus
	UL		PCSStatusDuration;					// ��ǰ״̬����ʱ��
	UC		CCMStatus;							// CCM״̬��0��Ч��1��Ч
	UC		RMStatus;							// RM״̬��PCS_RMStatus
	US		ValidAGTCount;						// ��Чע����ϯ��
	UC		Reserved[5];						// ����16�ֽ�
};

// ���󣺽ӹ�ϵͳ����ԾPCS����Ҫ��Է��ӹܣ��Է����봦��Standby״̬��TakeOver��handoff
#define TCP_PCS2PCS_TAKEOVER_REQ				11005
struct S_TCP_PCS2PCS_TAKEOVER_REQ
{
	UC		Reason;								// ����
};

// ��Ӧ���ӹ�ϵͳ
#define TCP_PCS2PCS_TAKEOVER_ACK				11006
struct S_TCP_PCS2PCS_TAKEOVER_ACK
{
	UC		ErrorCode;							// �������
};

// ������������������Ҷ�˵PCS1������ʱ����PCS2����Ҫ��PCS1����������Ϣ���ܵ���PCS1 TCP_PCS2PCS_TAKEOVER_REQ��Ϣ�ͳ���
#define TCP_PCS2PCS_REBOOT_REQ					11007	
struct S_TCP_PCS2PCS_REBOOT_REQ
{
	UC		Reason;								// ����
};

// ��Ӧ����������
#define TCP_PCS2PCS_REBOOT_ACK					11008		
struct S_TCP_PCS2PCS_REBOOT_ACK
{
	UC		ErrorCode;							// �������
};

// ACD���
// �¼����������
#define TCP_PCS2PCS_ADDTASKSTART_EVT			11009		
struct S_TCP_PCS2PCS_ADDTASKSTART_EVT
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];	// ͳһ��Ϣ����ID
	S_TCP_PCS2RMR_UMS_ADDTASK_REQ sAddtaskREQ;	// ԭ�����Ϣ��'S_TCP_PCS2RMR_UMS_ADDTASK_REQ'
	UCHAR	TaskType;							// 0-UMS����,1-��������
	S_TCP_PCS2RMR_ROUTE_QUERY sRouteQuery;		// ԭ�����Ϣ��'S_TCP_PCS2RMR_ROUTE_QUERY'
	// �ǻ�ԾPCS�յ�·������,�����󷢸���ԾPCS����.��ԾPCS��¼Զ��PCS�����ĺ���ID,�Ծ�������RM������Ϣ�Ĵ���ʽ
};

// �¼��������Ѿ����
#define TCP_PCS2PCS_ADDEDTASKCOMPLETION_EVT		11010
struct S_TCP_PCS2PCS_ADDEDTASKCOMPLETION_EVT
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
};
// �¼��������Ѿ��Ƴ�
#define TCP_PCS2PCS_REMOVETASK_EVT				11013
struct S_TCP_PCS2PCS_REMOVETASK_EVT
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
};

// ����RMѡ��ı䣨���ɻ�ԾPCS����
#define TCP_PCS2PCS_RM_SELECTCHANGE_REQ			11011
struct S_TCP_PCS2PCS_RM_SELECTCHANGE_REQ
{
	UC		Reason;								// ����
	UC		ActiveID;							// ��ԾRM����PCSID
};

// ��Ӧ��RMѡ��ı�
#define TCP_PCS2PCS_RM_SELECTCHANGE_ACK			11012		
struct S_TCP_PCS2PCS_RM_SELECTCHANGE_ACK
{
	UC		ErrorCode;							// �������
	UC		ActiveID;							// ��ԾRM����PCSID
};
// Simon added 2012-02-29 
// �¼�����ϯ����״̬�ı�
#define TCP_PCS2PCS_AGENTCONNECTIONSTATUS_EVT				11014
struct S_TCP_PCS2PCS_AGENTCONNECTIONSTATUS_EVT
{
	UL		AgentID;
	UC		Online;								// ��ϯ����״̬ 0���ӶϿ�,1��������
};

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_PCS_H__
//------------------------------------------------------------------------