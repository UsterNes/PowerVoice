////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		_Protocol_Monitor.h
// ��;������	���ͨ��Э��
// ���ߣ�		SUN
// ����ʱ�䣺	1999/12/01
// ����޸�ʱ�䣺2007/07/01
// ��Ȩ��Ϣ��	Copyright (C) 1999 - 2003 PCSG
// ��ע��
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_MONITOR_H__
#define __PROTOCOL_MONITOR_H__
//------------------------------------------------------------------------

//--------------------------
// �������ļ��ע�� - Total 4
//--------------------------

//--------------------------
// ע�� - 2
//------------
#define TCP_MON2PCS_REGISTER				1			// (0x0001) ע����Ϣ
struct S_TCP_MON2PCS_REGISTER
{
	UL		MonitorID;						// �����ID
	UC		Version;						// �汾��
	UC		RegCode[PCS_DEF_NUMLEN_APP];	// ע����
	char	strPassword[20];				// �û�����
};
//------------

//------------
#define TCP_PCS2MON_REGISTER_ACK			1001		// (0x03E9) ע����Ӧ��Ϣ
struct S_TCP_PCS2MON_REGISTER_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_BEUSED
											// PCS_DEF_RETCODE_VERSION
											// PCS_DEF_RETCODE_REGCODE
											// PCS_DEF_RETCODE_PASSWORD
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		MonitorLevel;					// ����û�����
	UL		AssociateAgentID;				// �󶨵���ϯID
	char	strMonitorView[20];				// ����û���Ӧ�Ĳ�ͬ�ӣ���ͬ������"��"����,���������ID
	char	strMonitorSelfView[10];			// �Զ�����ID
	char	strMonitorName[20];				// ���������
	char	strRights[100];					// �����Ȩ��
};
//------------
//--------------------------

//--------------------------
// ȡ��ע�� - 2
//------------
#define TCP_MON2PCS_UNREGISTER				2			// (0x0002) ȡ��ע����Ϣ
//------------

//------------
#define TCP_PCS2MON_UNREGISTER_ACK			1002		// (0x03EA) ȡ��ע����Ӧ��Ϣ
//------------
//--------------------------


//--------------------------
// �������ļ�ض������ - Total 12
//--------------------------

//--------------------------
// ��Ӽ�ض��� - 6
//------------
#define TCP_MON2PCS_ADDMONITOR				201			// (0x00C9) ��Ӽ�ض�����Ϣ
struct S_TCP_MON2PCS_ADDMONITOR
{
	UL		AgentID;						// �����ϯID
};
//------------

//------------
#define TCP_PCS2MON_ADDMONITOR_ACK			1201		// (0x04B1) ��Ӽ�ض�����Ӧ��Ϣ
struct S_TCP_PCS2MON_ADDMONITOR_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		AgentID;						// �����ϯID
	UC		PhoneType;						// �������ͣ�����_pcC_PhoneType
	char	strDN[20];						// DN����
	char	strPosition[20];				// Position ID
	char	strAgentName[20];				// ��ϯ����
	UC		DNStatus;						// ����״̬������_pcC_stDN
	UL		DNStDur;						// ����״̬����ʱ�����룩
	UC		PositionStatus;					// λ��״̬������_pcC_stPOS
	UL		PosStDur;						// λ��״̬����ʱ�����룩
	UC		LeaveReason;					// ��ϯԭ���Զ���
	char	strAliasName[50];				// ��¼����
	//-----------------------------------
	// Sun added 2007-07-28
	char	strRights[50];					// ��ϯȨ��
	UL		LoginID;						// ��¼ID
	UL		UserType;						// ����Ա���ͣ�������Ա����塱��ֵ�����
	char	strUserName[50];				// ����Ա��ʾ����
	char	strUserRights[50];				// ����ԱȨ��
	//-----------------------------------
};
//------------

//------------
#define TCP_MON2PCS_MONITORROUTEMANAGER		202			// (0x00CA) ����·�ɹ����������Ϣ
//------------

//------------
#define TCP_PCS2MON_MONITORROUTEMANAGER_ACK	1202		// (0x04B2) ����·�ɹ����������Ӧ��Ϣ
struct S_TCP_PCS2MON_MONITORROUTEMANAGER_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_MON2PCS_MONITOROBSERVICE		203			// (0x00CB) �����Ⲧ�����������Ϣ
//------------

//------------
#define TCP_PCS2MON_MONITOROBSERVICE_ACK	1203		// (0x04B3) �����Ⲧ�����������Ӧ��Ϣ
struct S_TCP_PCS2MON_MONITOROBSERVICE_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------


//--------------------------
// ���ټ�ض��� - 6
//------------
#define TCP_MON2PCS_REMOVEMONITOR			221			// (0x00DD) ���ټ�ض�����Ϣ
struct S_TCP_MON2PCS_REMOVEMONITOR
{
	UL		AgentID;						// �����ϯID
};
//------------

//------------
#define TCP_PCS2MON_REMOVEMONITOR_ACK		1221		// (0x04C5) ���ټ�ض�����Ӧ��Ϣ
struct S_TCP_PCS2MON_REMOVEMONITOR_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		AgentID;						// �����ϯID
};
//------------

//------------
#define TCP_MON2PCS_REMOVEROUTEMANAGER		222			// (0x00DE) ֹͣ·�ɹ����������Ϣ
//------------

//------------
#define TCP_PCS2MON_REMOVEROUTEMANAGER_ACK	1222		// (0x04C6) ֹͣ·�ɹ����������Ӧ��Ϣ
struct S_TCP_PCS2MON_REMOVEROUTEMANAGER_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_MON2PCS_REMOVEOBSERVICE			223			// (0x00DF) ֹͣ�Ⲧ��������Ϣ
//------------

//------------
#define TCP_PCS2MON_REMOVEOBSERVICE_ACK		1223		// (0x04C7) ֹͣ�Ⲧ��������Ӧ��Ϣ
struct S_TCP_PCS2MON_REMOVEOBSERVICE_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------


//--------------------------
// ��ϵͳ״̬������Ϣ - Total 1
//--------------------------
//------------
#define TCP_PCS2MON_SUBSYS_STATUS_REPORT	1231		// (0x04CF) ��ϵͳ״̬������Ϣ
struct S_TCP_PCS2MON_SUBSYS_STATUS_REPORT
{
	UC		SubSysID;						// ��ϵͳID���μ��������ߡ������߶��塱
	UC		SubSysStatus;					// ��ϵͳ״̬���μ������س������塱
	UC		SubSysVersion;					// ��ϵͳ�汾��
	UC		SubSysIndex;					// ��ϵͳ��ţ�����ϵͳ��
};
//------------
//--------------------------

//--------------------------
// ������Ϣ - Total 2
//--------------------------

//--------------------------
// �޸Ŀ��� - 2
//------------
#define TCP_MON2PCS_CHANGE_PASSWORD			251			// (0x00FA) �޸Ŀ�����Ϣ
struct S_TCP_MON2PCS_CHANGE_PASSWORD
{
	char	strPassword[20];				// �¿���
};
//------------

//------------
#define TCP_PCS2MON_CHANGE_PASSWORD_ACK		1251		// (0x04E3) �޸Ŀ�����Ӧ��Ϣ
struct S_TCP_PCS2MON_CHANGE_PASSWORD_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------

//--------------------------
// ACD ����״̬��ѯ - Total 2
//--------------------------
//------------
#define TCP_MON2PCS_GET_ACDSTATUS			20			// (0x0014) ��ѯACD����״̬��Ϣ
struct S_TCP_MON2PCS_GET_ACDSTATUS
{
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// IVR Channel ID who requests the data
	char	strACDDN[20];					// ACD DN
};
//------------

//------------
#define TCP_PCS2MON_GET_ACDSTATUS_ACK		1020			// (0x03FC) ��ѯACD����״̬��Ӧ��Ϣ
struct S_TCP_PCS2MON_GET_ACDSTATUS_ACK
{
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// IVR Channel ID who requests the data
	char	strACDDN[20];					// ACD DN
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	US		nACDStatus;
	UL		lngLoginAgents;					// Number of login agents
	UL		lngIdleAgents;					// Number of ready agents
	UL		lngTotalCalls;					// Number of calls
	UL		lngReserved;
};
//------------
//--------------------------

//--------------------------
// ������Ϣ������Ϣ - Total 4
//--------------------------
// ����Agent���Э��
//--------------------------

//--------------------------
// Զ�̿��� - 8
//--------------------------
// ���¼��ؼ���б���Ϣ
// ����Admin���Э��
//--------------------------

//------------
#define TCP_MON2PCS_RELOAD_ACD_SCENARIO_ACK	271			// (0x010F) ���¼����Ŷӹ�����Ӧ��Ϣ
struct S_TCP_MON2PCS_RELOAD_ACD_SCENARIO_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_MON2PCS_RELOAD_USERLIST_ACK		272			// (0x0110) ���¼����û��б���Ӧ��Ϣ
struct S_TCP_MON2PCS_RELOAD_USERLIST_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_MON2PCS_RELOAD_AGENTLIST_ACK	273			// (0x0111) ���¼�����ϯ�б���Ӧ��Ϣ
struct S_TCP_MON2PCS_RELOAD_AGENTLIST_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2MON_RELOAD_ACD_SCENARIO		1271		// (0x04F7) ���¼����Ŷӹ�����Ϣ
//------------

//------------
#define TCP_PCS2MON_RELOAD_USERLIST			1272		// (0x04F8) ���¼����û��б���Ϣ
//------------

//------------
#define TCP_PCS2MON_RELOAD_AGENTLIST		1273		// (0x04F9) ���¼�����ϯ�б���Ϣ
//--------------------------

//--------------------------
// �������� - 8
//--------------------------
// ����Admin���Э��
//--------------------------


//--------------------------
// ���������п��� - 2
//--------------------------
//------------
#define TCP_MON2PCS_MONITOR_CALL_CTRL		291			// (0x0123) ��ض˺��п�����Ϣ
struct S_TCP_MON2PCS_MONITOR_CALL_CTRL
{
	UL		InvokeID;						// Invoke ID
	UC		ControlType;					// �������ͣ��μ�_pcC_MonitorControlType
	UL		ControlAgentID;					// ��������ϯID
	UL		CallRefID;						// �����ƺ��б�ʾ�����Ϊ0��ϵͳ�Զ�ѡ��
	UL		HoldRefID;						// ���ֺ��б�־�����Ϊ0��ϵͳ�Զ�ѡ��
	char	strDest[PCS_DEF_NUMLEN_APP];	// Ŀ�����
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------

//------------
#define TCP_PCS2MON_MONITOR_CALL_CTRL_ACK	1291		// (0x050B) ��ض˺��п�����Ӧ��Ϣ
struct S_TCP_PCS2MON_MONITOR_CALL_CTRL_ACK
{
	UL		InvokeID;						// Invoke ID
	UC		ControlType;					// �������ͣ��μ�_pcC_MonitorControlType
	UL		ControlAgentID;					// ��������ϯID
	UL		CallRefID;						// �����ƺ��б�ʾ�����Ϊ0��ϵͳ�Զ�ѡ��
	UL		HoldRefID;						// ���ֺ��б�־�����Ϊ0��ϵͳ�Զ�ѡ��
	char	strDest[PCS_DEF_NUMLEN_APP];	// Ŀ�����
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTSUPPORT
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------


//--------------------------
// UMS���� - Total 12
//--------------------------

//--------------------------
// ��ѯ�����嵥 - 3
//------------
#define TCP_MON2PCS_UMS_QUERYTASKLIST_REQ	3301		// (0x0CE5) ��ѯ���������嵥��Ϣ
struct S_TCP_MON2PCS_UMS_QUERYTASKLIST_REQ
{
	US		RoutePointID;							// ·�ɵ�ID
};
//------------

//------------
#define TCP_PCS2MON_UMS_QUERYTASKLIST_ACK	3801		// (0x0ED9) ��ѯ���������嵥��Ӧ��Ϣ
struct S_TCP_PCS2MON_UMS_QUERYTASKLIST_ACK
{
	US		RoutePointID;							// ·�ɵ�ID
	UC		ErrorCode;								// �����룬�μ��������붨�塱��
													// PCS_DEF_RETCODE_SUCC
													// PCS_DEF_RETCODE_NOTEXIST
													// PCS_DEF_RETCODE_NORIGHTS
													// PCS_DEF_RETCODE_OTHERS
													// PCS_DEF_RETCODE_SYSERROR
	UL		TaskCount;								// ��������
};
//------------

//------------
#define TCP_PCS2MON_UMS_TASK_STATUS_DATA	3802		// (0x0EDA) �����嵥������Ϣ
struct S_TCP_PCS2MON_UMS_TASK_STATUS_DATA
{
	UL		TaskIndex;								// �����ţ�0 based
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
	UC		TaskStatus;								// ����״̬
	UL		WaitTime;								// ���ʱ�䣨�룩
	US		RoutePointID;							// ·�ɵ�ID
	US		GroupID;								// ��ID
	US		TeamID;									// С��ID
	UC		TeamIndex;								// С�����ȼ�
	UC		UserIndex;								// ����Ա���ȼ�
	char	strUserID[20];							// ����Ա
	UC		UMSType;								// UMS�������ͣ��μ�_pcC_UMSTaskTypes
	UC		UMSIndex;								// UMS��ϵͳ���
	UC		CustID;									// �⻧����
	char	strCallingParty[PCS_DEF_NUMLEN_APP];	// ������
	char	strCalledParty[PCS_DEF_NUMLEN_APP];		// ������
	UL		ClassifyCode;							// �������
	char	ServiceCode0;							// ���������
	char	ServiceCode1;							// �����������
	char	ServiceCode2;							// �����������
	char	strTaskTitle[50];						// �������
	char	strTaskContent[200];					// ��������
};
//------------
//--------------------------

//--------------------------
// ͳһ����״̬�ı�֪ͨ - 4
//------------
#define TCP_PCS2MON_UMS_TASK_ADD_NTF		3811		// (0x0EE3) ͳһ���֪ͨ��Ϣ
struct S_TCP_PCS2MON_UMS_TASK_ADD_NTF
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
	UC		UMSType;								// UMS�������ͣ��μ�_pcC_UMSTaskTypes
	UC		UMSIndex;								// UMS��ϵͳ���
	UC		CustID;									// �⻧����
	char	strCallingParty[PCS_DEF_NUMLEN_APP];	// ������
	char	strCalledParty[PCS_DEF_NUMLEN_APP];		// ������
	US		RoutePointID;							// Ĭ��·�ɵ�ID
	UL		ClassifyCode;							// �������
	UL		WaitTime;								// �ѵȴ�ʱ�䣨�룩
	char	ServiceCode0;							// ���������
	char	ServiceCode1;							// �����������
	char	ServiceCode2;							// �����������
	char	strTaskTitle[50];						// �������
	char	strTaskContent[200];					// ��������
};
//------------

//------------
#define TCP_PCS2MON_UMS_TASK_REMOVE_NTF		3812		// (0x0EE4) ͳһ���֪ͨ��Ϣ
struct S_TCP_PCS2MON_UMS_TASK_REMOVE_NTF
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
	US		RoutePointID;							// ·�ɵ�ID
};
//------------

//------------
#define TCP_PCS2MON_UMS_TASK_DISPATCH_NTF	3813		// (0x0EE5) ͳһ���䴦��֪ͨ��Ϣ
struct S_TCP_PCS2MON_UMS_TASK_DISPATCH_NTF
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
	UL		WaitTime;								// �ѵȴ�ʱ�䣨�룩
	US		RoutePointID;							// ·�ɵ�ID
	US		GroupID;								// ��ID
	US		TeamID;									// С��ID
	UC		TeamIndex;								// С�����ȼ�
	UC		UserIndex;								// ����Ա���ȼ�
	char	strUserID[20];							// ����ԱID
};
//------------

//------------
#define TCP_PCS2MON_UMS_TASK_FINISH_NTF		3814		// (0x0EE6) ͳһ�������֪ͨ��Ϣ
struct S_TCP_PCS2MON_UMS_TASK_FINISH_NTF
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
	US		RoutePointID;							// ·�ɵ�ID
	char	strUserID[20];							// ����ԱID
	UL		ReasonCode;								// ���ԭ�����
	char	strRefField1[10];						// ��������ֶ�1
	char	strRefField2[20];						// ��������ֶ�2
	char	strRefField3[20];						// ��������ֶ�3
	char	strRefField4[50];						// ��������ֶ�4
	char	strRefField5[100];						// ��������ֶ�5
};
//------------
//--------------------------

//--------------------------
// ͳһ����ȡ�� - 5
//------------
#define TCP_MON2PCS_UMS_CANCELTASK_CMD		3321		// (0x0CF9) ͳһ����ȡ��������Ϣ
struct S_TCP_MON2PCS_UMS_CANCELTASK_CMD
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
	UL		ReasonCode;								// ���ԭ�����
	char	strRefField1[10];						// ��������ֶ�1
	char	strRefField2[20];						// ��������ֶ�2
	char	strRefField3[20];						// ��������ֶ�3
	char	strRefField4[50];						// ��������ֶ�4
	char	strRefField5[100];						// ��������ֶ�5
};
//------------

//------------
#define TCP_MON2PCS_UMS_CANCELAGTTASK_CMD	3322		// (0x0CFA) ��ϯȫ������ȡ��������Ϣ
struct S_TCP_MON2PCS_UMS_CANCELAGTTASK_CMD
{
	char	strUserID[20];							// ����ԱID
	UL		ReasonCode;								// ���ԭ�����
	char	strRefField1[10];						// ��������ֶ�1
	char	strRefField2[20];						// ��������ֶ�2
	char	strRefField3[20];						// ��������ֶ�3
	char	strRefField4[50];						// ��������ֶ�4
	char	strRefField5[100];						// ��������ֶ�5
};
//------------

//------------
#define TCP_PCS2MON_UMS_CANCELTASK_ACK		3821		// (0x0EED) ͳһ����ȡ��������Ӧ��Ϣ
struct S_TCP_PCS2MON_UMS_CANCELTASK_ACK
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
	UC		ErrorCode;								// �����룬�μ��������붨�塱��
													// PCS_DEF_RETCODE_SUCC
													// PCS_DEF_RETCODE_NOTEXIST
													// PCS_DEF_RETCODE_NORIGHTS
													// PCS_DEF_RETCODE_OTHERS
													// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2MON_UMS_CANCELAGTTASK_ACK	3822		// (0x0EEE) ��ϯȫ������ȡ��������Ӧ��Ϣ
struct S_TCP_PCS2MON_UMS_CANCELAGTTASK_ACK
{
	char	strUserID[20];							// ����ԱID
	UC		ErrorCode;								// �����룬�μ��������붨�塱��
													// PCS_DEF_RETCODE_SUCC
													// PCS_DEF_RETCODE_NOTEXIST
													// PCS_DEF_RETCODE_NORIGHTS
													// PCS_DEF_RETCODE_OTHERS
													// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2MON_UMS_TASK_CANCEL_NTF		3823		// (0x0EEF) ͳһ����ȡ��֪ͨ��Ϣ
struct S_TCP_PCS2MON_UMS_TASK_CANCEL_NTF
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
	US		RoutePointID;							// ·�ɵ�ID
	char	strUserID[20];							// ����ԱID
	UL		ReasonCode;								// ���ԭ�����
	char	strRefField1[10];						// ��������ֶ�1
	char	strRefField2[20];						// ��������ֶ�2
	char	strRefField3[20];						// ��������ֶ�3
	char	strRefField4[50];						// ��������ֶ�4
	char	strRefField5[100];						// ��������ֶ�5
};
//------------
//--------------------------


//--------------------------
// ��ϯ״̬�޸���Ϣ - Total 12
//--------------------------
//------------
#define TCP_MON2PCS_AGENT_LOGIN_REQ			241			// (0x00F1) ��ϯ��¼������Ϣ
struct S_TCP_MON2PCS_AGENT_LOGIN_REQ
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// �����ϯID
	UL		LoginID;						// ��¼ID�����Ϊ0����ʹ�õ�¼����
	char	strAliasName[50];				// ��¼����
	char	strPassword[20];				// �û�����
};
//------------

//------------
#define TCP_MON2PCS_AGENT_LOGOUT_REQ		242			// (0x00F2) ��ϯ�˳���¼������Ϣ
struct S_TCP_MON2PCS_AGENT_LOGOUT_REQ
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// �����ϯID
};
//------------

//------------
#define TCP_MON2PCS_AGENT_STATUS_CHG_REQ	243			// (0x00F3) ��ϯ�޸�״̬������Ϣ
struct S_TCP_MON2PCS_AGENT_STATUS_CHG_REQ
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// �����ϯID
	UC		PositionStatus;					// λ��״̬������_pcC_stPOS
	UC		LeaveReason;					// ��ϯԭ���Զ���
	UC		CallDisconnect;					// �Ƿ�Ͽ���ǰͨ�����μ������س������塱
};
//------------

//------------
#define TCP_MON2PCS_AGENT_SET_CALLFWD		244			// (0x00F4) ��ϯ�趨����ת������Ϣ
struct S_TCP_MON2PCS_AGENT_SET_CALLFWD
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// �����ϯID
	UC		ForwardType;					// ����ת�����ͣ��μ�������ת�����Ͷ��塱
	char	strTelNo[PCS_DEF_NUMLEN_APP];	// ����ת�ƺ���
};
//------------

//------------
#define TCP_MON2PCS_AGENT_SET_MWA_ST		245			// (0x00F5) ��ϯ�趨���Եȴ����ؿ�����Ϣ
struct S_TCP_MON2PCS_AGENT_SET_MWA_ST
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// �����ϯID
	UC		SetMWAOn;						// �Ƿ��趨���Եȴ����أ��μ������س������塱
};
//------------

//------------
#define TCP_MON2PCS_GET_LEAVEREASONS		246			// (0x00F6) �����ȡ�����б�
//------------

//------------
#define TCP_PCS2MON_AGENT_LOGIN_ACK			1241		// (0x04D9) ��ϯ��¼������Ӧ��Ϣ
struct S_TCP_PCS2MON_AGENT_LOGIN_ACK
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// �����ϯID
	UL		LoginID;						// ��¼ID
	char	strAliasName[50];				// ��¼����
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_BEUSED
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_PASSWORD
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_LACK_PARAM
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UC		PhoneType;						// �������ͣ�����_pcC_PhoneType
	char	strDN[20];						// DN����
	char	strPosition[20];				// Position ID
	char	strRights[50];					// ��ϯȨ��
	UL		UserType;						// ����Ա���ͣ�������Ա����塱��ֵ�����
	char	strUserName[50];				// ����Ա��ʾ����
	char	strUserRights[50];				// ����ԱȨ��
};
//------------

//------------
#define TCP_PCS2MON_AGENT_LOGOUT_ACK		1242		// (0x04DA) ��ϯ�˳���¼������Ӧ��Ϣ
struct S_TCP_PCS2MON_AGENT_LOGOUT_ACK
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// �����ϯID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_REJECTED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2MON_AGENT_STATUS_CHG_ACK	1243		// (0x04DB) ��ϯ�޸�״̬������Ӧ��Ϣ
struct S_TCP_PCS2MON_AGENT_STATUS_CHG_ACK
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// �����ϯID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2MON_AGENT_SET_CALLFWD_ACK	1244		// (0x04DC) ��ϯ�趨����ת�ƿ�����Ӧ��Ϣ
struct S_TCP_PCS2MON_AGENT_SET_CALLFWD_ACK
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// �����ϯID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2MON_AGENT_SET_MWA_ST_ACK	1245		// (0x04DD) ��ϯ�趨���Եȴ����ؿ�����Ӧ��Ϣ
struct S_TCP_PCS2MON_AGENT_SET_MWA_ST_ACK
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// �����ϯID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2MON_LEAVEREASONS_LIST		1246		// (0x04DE) �����б�
struct S_TCP_PCS2MON_LEAVEREASONS_LIST
{
	UC		ReasonCount;					// ��������
	UC		ReasonValue[10];				// ���ɱ��0-9
	char	ReasonName[10][20];				// ��������0-9
};
//------------
//--------------------------

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_MONITOR_H__
//------------------------------------------------------------------------
