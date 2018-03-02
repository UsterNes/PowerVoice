////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		_Protocol_OBS.h
// ��;������	�Ⲧ����ͨ��Э��
// ���ߣ�		SUN
// ����ʱ�䣺	1999/12/01
// ����޸�ʱ�䣺2003/09/20
// ��Ȩ��Ϣ��	Copyright (C) 1999 - 2003 PCSG
// ��ע��
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_OBSERVICE_H__
#define __PROTOCOL_OBSERVICE_H__
//------------------------------------------------------------------------

//--------------------------
// ע�� - 2
//------------
#define TCP_OBS2PCS_REGISTER				1			// (0x0001) ע����Ϣ
struct S_TCP_OBS2PCS_REGISTER
{
	UC		Version;						// �汾��
	UC		RegCode[PCS_DEF_NUMLEN_APP];	// ע����
};
//------------

//------------
#define TCP_PCS2OBS_REGISTER_ACK			1001		// (0x03E9) ע����Ӧ��Ϣ
struct S_TCP_PCS2OBS_REGISTER_ACK
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
#define TCP_OBS2PCS_UNREGISTER				2			// (0x0002) ȡ��ע����Ϣ
//------------

//------------
#define TCP_PCS2OBS_UNREGISTER_ACK			1002		// (0x03EA) ȡ��ע����Ӧ��Ϣ
//------------
//--------------------------


//--------------------------
// Ԥ���Ⲧ - 7
//------------
#define TCP_OBS2PCS_AGENTTASK_REQUEST		401			// (0x0191) ָ����ϯ�Ⲧ����������Ϣ
struct S_TCP_OBS2PCS_AGENTTASK_REQUEST
{
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ����ID
	UL		AgentID;						// ������ϯID
	char	strTelNo[PCS_DEF_NUMLEN_APP];	// �Ⲧ�绰����
	char	strType[4];						// �Ⲧ���ͣ��û��Զ��壩
	char	strData[200];					// �Ⲧ���ݣ��û��Զ��壩
};
//------------

//------------
#define TCP_OBS2PCS_GROUPTASK_REQUEST		402			// (0x0192) �Ⲧ�������������Ϣ
struct S_TCP_OBS2PCS_GROUPTASK_REQUEST
{
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ����ID
	UC		AssignType;						// ������󣬲���_pcC_CallDistributeObject
	UC		SharedTask;						// �Ƿ����μ������س������塱
	US		TargetID;						// ������ϯ���С��ID
	char	strTelNo[PCS_DEF_NUMLEN_APP];	// �Ⲧ�绰����
	char	strType[4];						// �Ⲧ���ͣ��û��Զ��壩
	char	strData[200];					// �Ⲧ���ݣ��û��Զ��壩
};
//------------

//------------
#define TCP_OBS2PCS_TASK_CANCEL				411			// (0x019B) �Ⲧ����ȡ����Ϣ
struct S_TCP_OBS2PCS_TASK_CANCEL	
{
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ����ID
	UL		AgentID;						// ������ϯID
};
//------------

//------------
#define TCP_PCS2OBS_TASK_REQUEST_ACK		1401			// (0x0579) �Ⲧ����������Ӧ��Ϣ
struct S_TCP_PCS2OBS_TASK_REQUEST_ACK
{
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ����ID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		TargetID;						// ������ϯID����ϯ���С��ID
};
//------------

//------------
#define TCP_PCS2OBS_TASK_EXECUTE			1402			// (0x057A) �Ⲧ����ʼִ����Ϣ
struct S_TCP_PCS2OBS_TASK_EXECUTE
{
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ����ID
	UL		AgentID;						// ������ϯID
};
//------------

//------------
#define TCP_PCS2OBS_TASK_FINISHED			1403			// (0x057B) �Ⲧ���������Ϣ
struct S_TCP_PCS2OBS_TASK_FINISHED
{
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ����ID
	UC		ResultCode;						// �Ⲧִ�н��������_pcC_OutBoundCallResult
	UC		TryTimes;						// ���Դ���
	UL		AgentID;						// ������ϯID
};
//------------

//------------
#define TCP_PCS2OBS_TASK_CANCEL_ACK			1411			// (0x0583) �Ⲧ����ȡ����Ӧ��Ϣ
struct S_TCP_PCS2OBS_TASK_CANCEL_ACK
{
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ����ID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		AgentID;						// ������ϯID
};
//------------
//--------------------------


//--------------------------
// Ԥ���Ⲧ - 8
//------------
#define TCP_OBS2PCS_PREDICTIVE_AGENTTASK_REQUEST	431		// (0x1AF) ռ����ϯ��Ϣ
struct S_TCP_OBS2PCS_PREDICTIVE_AGENTTASK_REQUEST
{
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ����ID
	UL		AgentID;						// ������ϯID
	char	strTelNo[PCS_DEF_NUMLEN_APP];	// �Ⲧ�绰����
	char	strType[4];						// �Ⲧ���ͣ��û��Զ��壩
	char	strData[200];					// �Ⲧ���ݣ��û��Զ��壩
};
//------------

//------------
#define TCP_OBS2PCS_PREDICTIVE_GROUPTASK_REQUEST	432			// (0x01B0) ռ����ϯ(��)��Ϣ
struct S_TCP_OBS2PCS_PREDICTIVE_GROUPTASK_REQUEST
{
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ����ID
	UC		AssignType;						// ������󣬲���_pcC_CallDistributeObject
	US		TargetID;						// ������ϯ���С��ID
	char	strTelNo[PCS_DEF_NUMLEN_APP];	// �Ⲧ�绰����
	char	strType[4];						// �Ⲧ���ͣ��û��Զ��壩
	char	strData[200];					// �Ⲧ���ݣ��û��Զ��壩
};
//------------

//------------
#define TCP_OBS2PCS_PREDICTIVE_CANCELTASK			441			// (0x01B9) Ԥ���Ⲧ����ȡ����Ϣ
struct S_TCP_OBS2PCS_PREDICTIVE_CANCELTASK	
{
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ����ID
	UL		AgentID;						// ������ϯID
};
//------------

//------------
#define TCP_OBS2PCS_PREDICTIVE_TRANSFERTASK			442			// (0x01BA) �Ⲧ�绰������Ϣ
struct S_TCP_OBS2PCS_PREDICTIVE_TRANSFERTASK
{
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ����ID
	UL		AgentID;						// ������ϯID
	UL		ControlAgentID;					// �Ⲧ��ϯID
	UL		CallRefID;						// ���б�ʾ�����Ϊ0��ϵͳ�Զ�ѡ��
	UC		EventSource;					// �¼���Դ������_pcC_EventSource�����Ϊ0��ϵͳ�Զ�ѡ��
	char	strTelNo[PCS_DEF_NUMLEN_APP];	// �Ⲧ�绰����
	char	strType[4];						// �Ⲧ���ͣ��û��Զ��壩
	char	strData[200];					// �Ⲧ���ݣ��û��Զ��壩
};
//------------

//------------
#define TCP_PCS2OBS_PREDICTIVE_TASK_REQUEST_ACK		1431			// (0x0597) Ԥ���Ⲧ����������Ӧ��Ϣ
struct S_TCP_PCS2OBS_PREDICTIVE_TASK_REQUEST_ACK
{
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ����ID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		TargetID;						// ������ϯID����ϯ���С��ID
};
//------------

//------------
#define TCP_PCS2OBS_PREDICTIVE_CANCELTASKACK		1441			// (0x05A1) Ԥ���Ⲧ����ȡ����Ӧ��Ϣ
struct S_TCP_PCS2OBS_PREDICTIVE_CANCELTASKACK
{
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ����ID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		AgentID;						// ������ϯID
};
//------------

//------------
#define TCP_PCS2OBS_PREDICTIVE_TRANSFERTASK_ACK		1442			// (0x052A) �Ⲧ�绰������Ϣ
struct S_TCP_PCS2OBS_PREDICTIVE_TRANSFERTASK_ACK
{
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ����ID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		AgentID;						// ������ϯID
};
//------------

//------------
/// �յ���ϯ���͵Ŀ�ʼ������Ϣ��, Power Call Server����OB Server�������������Ϣ
#define TCP_PCS2OBS_PREDICTIVE_FINISHTASK			1443			// (0x052B) ���������Ϣ
struct S_TCP_PCS2OBS_PREDICTIVE_FINISHTASK
{
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ����ID
	UL		AgentID;						// ������ϯID
};
//------------
//--------------------------


//--------------------------
// ״̬��ѯ - 2
//------------
#define TCP_OBS2PCS_STATUS_QUERY			491			// (0x01EB) ��ѯ״̬��Ϣ
struct S_TCP_OBS2PCS_STATUS_QUERY
{
	UL		AgentID;						// ��ѯ��ϯID
};
//------------

//------------
#define TCP_PCS2OBS_STATUS_QUERY_ACK		1491		// (0x05D3) ��ѯ״̬��Ӧ��Ϣ
struct S_TCP_PCS2OBS_STATUS_QUERY_ACK
{
	UL		AgentID;						// ��ѯ��ϯID
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
#endif  //End #ifndef __PROTOCOL_OBSERVICE_H__
//------------------------------------------------------------------------
