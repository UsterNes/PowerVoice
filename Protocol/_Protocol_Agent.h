////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		_Protocol_Agent.h
// ��;������	��ϯͨ��Э��
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
#ifndef __PROTOCOL_AGENT_H__
#define __PROTOCOL_AGENT_H__
//------------------------------------------------------------------------

#include "_pcRightsDef.h"

//--------------------------
// ��ϯע�� - Total 4
//--------------------------

//--------------------------
// ע�� - 2
//------------
#define TCP_AGT2PCS_REGISTER				1			// (0x0001) ע����Ϣ
struct S_TCP_AGT2PCS_REGISTER
{
	UL		AgentID;						// ��ϯID
	UC		Version;						// �汾��
	UC		RegCode[PCS_DEF_NUMLEN_APP];	// ע����
};
//------------

//------------
#define TCP_PCS2AGT_REGISTER_ACK			1001		// (0x03E9) ע����Ӧ��Ϣ
struct S_TCP_PCS2AGT_REGISTER_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_BEUSED
											// PCS_DEF_RETCODE_VERSION
											// PCS_DEF_RETCODE_REGCODE
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UC		PhoneType;						// �������ͣ�����_pcC_PhoneType
	char	strDN[20];						// DN����
	char	strPosition[20];				// Position ID
	char	strAgentName[20];				// ��ϯ����
	char	strRights[50];					// ��ϯȨ��
};
//------------
//--------------------------

//--------------------------
// ȡ��ע�� - 2
//------------
#define TCP_AGT2PCS_UNREGISTER				2			// (0x0002) ȡ��ע����Ϣ
//------------

//------------
#define TCP_PCS2AGT_UNREGISTER_ACK			1002		// (0x03EA) ȡ��ע����Ӧ��Ϣ
//------------
//--------------------------


//--------------------------
// ״̬��Ϣ - Total 17
//--------------------------

//--------------------------
// ״̬��ѯ - 2
//------------
#define TCP_AGT2PCS_STATUS_QUERY			11			// (0x000B) ��ѯ״̬��Ϣ
//------------

//------------
#define TCP_PCS2AGT_STATUS_QUERY_ACK		1011		// (0x03F3) ��ѯ״̬��Ӧ��Ϣ
struct S_TCP_PCS2AGT_STATUS_QUERY_ACK
{
	UC		DNStatus;						// ����״̬������_pcC_stDN
	UC		PositionStatus;					// λ��״̬������_pcC_stPOS
	UC		LeaveReason;					// ��ϯԭ���Զ���
};
//------------
//--------------------------

//--------------------------
// ��ϯ��¼ - 2
//------------
#define TCP_AGT2PCS_AGENT_LOGIN				12			// (0x000C) ��ϯ��¼��Ϣ
struct S_TCP_AGT2PCS_AGENT_LOGIN
{
	char	strPassword[20];				// �û�����
	char	strAliasName[50];				// ��¼����
};
//------------

//------------
#define TCP_PCS2AGT_AGENT_LOGIN_ACK			1012		// (0x03F4) ��ϯ��¼��Ӧ��Ϣ
struct S_TCP_PCS2AGT_AGENT_LOGIN_ACK
{
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
	UL		UserType;						// ����Ա���ͣ�������Ա����塱��ֵ�����
	char	strUserName[50];				// ����Ա��ʾ����
	char	strUserRights[50];				// ����ԱȨ��
};
//------------
//--------------------------

//--------------------------
// ��ϯ�˳���¼ - 2
//------------
#define TCP_AGT2PCS_AGENT_LOGOUT			13			// (0x000D) ��ϯ�˳���¼��Ϣ
//------------

//------------
#define TCP_PCS2AGT_AGENT_LOGOUT_ACK		1013		// (0x03F5) ��ϯ�˳���¼��Ӧ��Ϣ
struct S_TCP_PCS2AGT_AGENT_LOGOUT_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_REJECTED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------

//--------------------------
// �޸�״̬ - 2
//------------
#define TCP_AGT2PCS_STATUS_CHANGE			14			// (0x000E) �޸�״̬��Ϣ
struct S_TCP_AGT2PCS_STATUS_CHANGE
{
	UC		PositionStatus;					// λ��״̬������_pcC_stPOS
	UC		LeaveReason;					// ��ϯԭ���Զ���
	UC		CallDisconnect;					// �Ƿ�Ͽ���ǰͨ�����μ������س������塱
};
//------------

//------------
#define TCP_PCS2AGT_STATUS_CHANGE_ACK		1014		// (0x03F6) �޸�״̬��Ӧ��Ϣ
struct S_TCP_PCS2AGT_STATUS_CHANGE_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------

//--------------------------
// ������趨 - 2
//------------
#define TCP_AGT2PCS_SET_DND_STATUS			15			// (0x000F) �趨����ſ�����Ϣ
struct S_TCP_AGT2PCS_SET_DND_STATUS
{
	UC		SetDNDOn;						// �Ƿ��趨����ſ��أ��μ������س������塱
};
//------------

//------------
#define TCP_PCS2AGT_SET_DND_STATUS_ACK		1015		// (0x03F7) �趨����ſ�����Ӧ��Ϣ
struct S_TCP_PCS2AGT_SET_DND_STATUS_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------

//--------------------------
// �趨����չ - 4
//------------
#define TCP_AGT2PCS_SET_CALLFORWARD			16			// (0x0010) �趨����ת����Ϣ
struct S_TCP_AGT2PCS_SET_CALLFORWARD
{
	UC		ForwardType;						// ����ת�����ͣ��μ�������ת�����Ͷ��塱
	char	strTelNo[PCS_DEF_NUMLEN_APP];		// ����ת�ƺ���
};
//------------

//------------
#define TCP_AGT2PCS_SET_MWA_STATUS			17			// (0x0011) �趨���Եȴ�������Ϣ
struct S_TCP_AGT2PCS_SET_MWA_STATUS
{
	UC		SetMWAOn;							// �Ƿ��趨���Եȴ����أ��μ������س������塱
};
//------------

//------------
#define TCP_AGT2PCS_CHANGE_USER				18			// (0x0012) �����л��û���Ϣ������TCP_PCS2AGT_AGENT_LOGIN_ACK��Ϣ
struct S_TCP_AGT2PCS_CHANGE_USER
{
	char	strPassword[20];				// �û�����
	char	strAliasName[50];				// ��¼����
};
//------------

//------------
#define TCP_PCS2AGT_SET_CALLFORWARD_ACK		1016		// (0x03F8) �趨����ת����Ӧ��Ϣ
struct S_TCP_PCS2AGT_SET_CALLFORWARD_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------

//--------------------------
// ״̬�ı�֪ͨ - 3
//------------
#define TCP_PCS2AGT_AGENT_ALIAS_CHANGE		1017		// (0x03F9) ��ϯ��¼�����ı���Ϣ
struct S_TCP_PCS2AGT_AGENT_ALIAS_CHANGE
{
	UL		AgentID;						// ��ϯID
	char	strAliasName[50];				// ��¼����
	//-----------------------------------
	// Sun added 2007-07-28
	UL		LoginID;						// ��¼ID
	UL		UserType;						// ����Ա���ͣ�������Ա����塱��ֵ�����
	char	strUserName[50];				// ����Ա��ʾ����
	char	strUserRights[50];				// ����ԱȨ��
	//-----------------------------------
};
//------------

//------------
#define TCP_PCS2AGT_POS_STATUS_CHANGE		1018		// (0x03FA) λ��״̬�ı���Ϣ
struct S_TCP_PCS2AGT_POS_STATUS_CHANGE
{
	UL		AgentID;						// ��ϯID
	UC		PositionStatus;					// λ��״̬������_pcC_stPOS
	UC		LeaveReason;					// ��ϯԭ���Զ���
};
//------------

//------------
#define TCP_PCS2AGT_DN_STATUS_CHANGE		1019		// (0x03FB) ����״̬�ı���Ϣ
struct S_TCP_PCS2AGT_DN_STATUS_CHANGE
{
	UL		AgentID;						// ��ϯID
	UC		DNStatus;						// ����״̬������_pcC_stDN
};
//------------
//--------------------------


//--------------------------
// ���ݲ�ѯ - Total 6
//------------
#define TCP_AGT2PCS_AGT_STATUS_QUERY		25			// (0x0019) ��ѯָ����ϯ״̬��Ϣ
struct S_TCP_AGT2PCS_AGT_STATUS_QUERY
{
	UL		AgentID;						// ��ϯID
};
//------------

//------------
#define TCP_AGT2PCS_USER_STATUS_QUERY		26			// (0x001A) ��ѯָ������Ա״̬��Ϣ
struct S_TCP_AGT2PCS_USER_STATUS_QUERY
{
	char	strAliasName[50];				// ��¼����
};
//------------

//------------
#define TCP_AGT2PCS_GET_ACDOBJ_STATUS		27			// (0x001B) ��ѯ�ŶӶ���״̬��Ϣ
struct S_TCP_AGT2PCS_GET_ACDOBJ_STATUS
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// Agent ID who requests the data
	UC		nType;							// �������Center / RoutePoint / Queue / Group / Team����
											/// �μ�_pcC_CallDistributeObject����
	US		nGroupID;						// ����ID
};
//------------

//------------
#define TCP_AGT2PCS_AGENT_PROPERTY_QUERY	28			// (0x001C) ��ѯָ����ϯ������Ϣ
struct S_TCP_AGT2PCS_AGENT_PROPERTY_QUERY
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// AgentID��DN����һ�����ɣ���AgentID����
	char	strDN[PCS_DEF_NUMLEN_APP];		// DN����
};
//------------

//------------
#define TCP_AGT2PCS_USER_PROPERTY_QUERY		29			// (0x001D) ��ѯָ������Ա������Ϣ
struct S_TCP_AGT2PCS_USER_PROPERTY_QUERY
{
	UL		InvokeID;						// Invoke ID
	char	strAliasName[50];				// ��¼�����͹�������һ�����ɣ��ҵ�¼��������
	UL		LoginID;						// ����
};
//------------

//------------
#define TCP_PCS2AGT_AGT_STATUS_QUERY_ACK	1025		// (0x0401) ��ѯָ����ϯ״̬��Ӧ��Ϣ
struct S_TCP_PCS2AGT_AGT_STATUS_QUERY_ACK
{
	UL		AgentID;						// ��ϯID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UC		CustID;							// �⻧ID
	UC		DNStatus;						// ����״̬������_pcC_stDN
	UL		DNStDur;						// ����״̬����ʱ�����룩
	UC		PositionStatus;					// λ��״̬������_pcC_stPOS
	UL		PosStDur;						// λ��״̬����ʱ�����룩
	UC		LeaveReason;					// ��ϯԭ���Զ���
	char	strAliasName[50];				// ��¼����
};
//------------

//------------
#define TCP_PCS2AGT_USER_STATUS_QUERY_ACK	1026		// (0x0402) ��ѯָ������Ա״̬��Ӧ��Ϣ
struct S_TCP_PCS2AGT_USER_STATUS_QUERY_ACK
{
	char	strAliasName[50];				// ��¼����
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UC		HomeSiteID;						// ��������ID
	UC		SiteID;							// ��¼����ID
	UC		CustID;							// �⻧ID
	UL		AgentID;						// ��ϯID
	UC		DNStatus;						// ����״̬������_pcC_stDN
	UL		DNStDur;						// ����״̬����ʱ�����룩
	UC		PositionStatus;					// λ��״̬������_pcC_stPOS
	UL		PosStDur;						// λ��״̬����ʱ�����룩
	UC		LeaveReason;					// ��ϯԭ���Զ���
};
//------------

//------------
#define TCP_PCS2AGT_GET_ACDOBJ_STATUS_ACK	1027		// (0x0403) ��ѯ�ŶӶ���״̬��Ӧ��Ϣ
struct S_TCP_PCS2AGT_GET_ACDOBJ_STATUS_ACK
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// Agent ID who requests the data
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	S_TCP_PCS2ALL_ACD_STATUS_REPORT Data;	// ����
};
//------------

//------------
#define TCP_PCS2AGT_AGENT_PROPERTY_QUERY_ACK 1028		// (0x0404) ��ѯָ����ϯ������Ӧ��Ϣ
struct S_TCP_PCS2AGT_AGENT_PROPERTY_QUERY_ACK
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// AgentID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_LACK_PARAM
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		AgentType;						// ��ϯ���ͣ����ա���ϯ����塱
	UC		PhoneType;						// �������ͣ�����_pcC_PhoneType
	char	strDN[PCS_DEF_NUMLEN_APP];		// DN����
	char	strPosition[PCS_DEF_NUMLEN_APP];  // Position ID
	char	strAgentName[PCS_DEF_NUMLEN_APP]; // ��ϯ����
	UC		CustID;							// ��ϯ�⻧ID
};
//------------

//------------
#define TCP_PCS2AGT_USER_PROPERTY_QUERY_ACK 1029		// (0x0405) ��ѯָ������Ա������Ӧ��Ϣ
struct S_TCP_PCS2AGT_USER_PROPERTY_QUERY_ACK
{
	UL		InvokeID;						// Invoke ID
	char	strAliasName[50];				// ��¼����
	UL		LoginID;						// ����
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_LACK_PARAM
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		UserType;						// �û�����
	char	UserLevel;						// �û�����
	UC		IsAutoUser;						// �Ƿ����������û����μ������س������塱
	char	strUserName[50];				// �û���ʾ��
	UC		AllowLogon;						// �Ƿ������ڱ����ĵ�¼���μ������س������塱
	UC		HomeSiteID;						// ��������ID
	UC		CustID;							// �⻧ID
	UC		LoginSiteID;					// ��¼����ID
	UL		AgentID;						// AgentID����¼״̬����
};
//------------
//--------------------------


//--------------------------
// ��ϯ�Ⲧ - Total 5
//--------------------------

//--------------------------
// Ԥ���Ⲧ - 5
//------------
#define TCP_AGT2PCS_ACCEPTOBTASK			31			// (0x001F) �����Ⲧ������Ϣ
struct S_TCP_AGT2PCS_ACCEPTOBTASK
{
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ����ID
};
//------------

//------------
#define TCP_AGT2PCS_EXECUTEOBTASK			32			// (0x0020) ��ʼִ���Ⲧ������Ϣ
struct S_TCP_AGT2PCS_EXECUTEOBTASK
{
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ����ID
};
//------------

//------------
#define TCP_AGT2PCS_FINISHOBTASK			33			// (0x0021) �Ⲧ���������Ϣ
struct S_TCP_AGT2PCS_FINISHOBTASK
{
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ����ID
	UC		ResultCode;						// �Ⲧִ�н��������_pcC_OutBoundCallResult
	UC		TryTimes;						// ���Դ���
};
//------------

//------------
#define TCP_PCS2AGT_ASSIGNOBTASK			1031		// (0x0407) �����Ⲧ������Ϣ
struct S_TCP_PCS2AGT_ASSIGNOBTASK
{
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ����ID
	UC		SharedTask;						// �Ƿ����μ������س������塱
	char	strTelNo[PCS_DEF_NUMLEN_APP];	// �Ⲧ�绰����
	char	strType[4];						// �Ⲧ���ͣ��û��Զ��壩
	char	strData[200];					// �Ⲧ���ݣ��û��Զ��壩
};
//------------

//------------
#define TCP_PCS2AGT_CANCELOBTASK			1032		// (0x0408) ȡ���Ⲧ������Ϣ
struct S_TCP_PCS2AGT_CANCELOBTASK
{
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ����ID
};
//------------
//--------------------------


//--------------------------
// Ԥ���Ⲧ - 5
//------------
#define TCP_AGT2PCS_PREDICTIVE_ACCEPTTASK			41			// (0x0029) ����Ԥ���Ⲧ������Ϣ
struct S_TCP_AGT2PCS_PREDICTIVE_ACCEPTTASK
{
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ����ID
	UC		nFlag;							// �Ƿ���ܣ��μ������س������塱
};
//------------

//------------
/// ��ϯ�����Ⲧ�绰ʱ,��PCS���Ϳ�ʼ������Ϣ
#define TCP_AGT2PCS_PREDICTIVE_BEGINTASK			42			// (0x002A) ��ʼִ���Ⲧ������Ϣ
struct S_TCP_AGT2PCS_PREDICTIVE_BEGINTASK
{
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ����ID
};
//------------

//------------
#define TCP_PCS2AGT_PREDICTIVE_ASSIGNTASK			1041		// (0x411) ��ϯ�Ⲧ֪ͨ��Ϣ
struct S_TCP_PCS2AGT_PREDICTIVE_ASSIGNTASK
{
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ����ID
	char	strTelNo[PCS_DEF_NUMLEN_APP];	// �Ⲧ�绰����
	char	strType[4];						// �Ⲧ���ͣ��û��Զ��壩
	char	strData[200];					// �Ⲧ���ݣ��û��Զ��壩
};
//------------

//------------
#define TCP_PCS2AGT_PREDICTIVE_CANCELTASK			1042		// (0x0412) ȡ��Ԥ���Ⲧ������Ϣ
struct S_TCP_PCS2AGT_PREDICTIVE_CANCELTASK
{
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ����ID
};
//------------

//------------
#define TCP_PCS2AGT_PREDICTIVE_TRANSFERTASK			1043		// (0x0413) �Ⲧ�绰������Ϣ
struct S_TCP_PCS2AGT_PREDICTIVE_TRANSFERTASK
{
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ����ID
	char	strTelNo[PCS_DEF_NUMLEN_APP];	// �Ⲧ�绰����
	char	strType[4];						// �Ⲧ���ͣ��û��Զ��壩
	char	strData[200];					// �Ⲧ���ݣ��û��Զ��壩
};
//------------
//--------------------------


//--------------------------
// ������Ϣ Total 29
//--------------------------

//--------------------------
// �绰�Ⲧ - 1
//------------
#define TCP_AGT2PCS_CALL_CALLOUT			51			// (0x0033) �绰�Ⲧ��Ϣ
struct S_TCP_AGT2PCS_CALL_CALLOUT
{
	UC		CallScale;								// ���з�Χ������_pcC_CallScale
	char	strCalledParty[PCS_DEF_NUMLEN_APP];		// �Ⲧ����
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------
//--------------------------

//--------------------------
// �绰���� - 1
//------------
#define TCP_PCS2AGT_CALL_CALLIN				1051		// (0x041B) �绰������Ϣ
struct S_TCP_PCS2AGT_CALL_CALLIN
{
	UL		AgentID;						// ��ϯID
	UL		CallRefID;						// ���б�ʾ
	char	TimeStamp[15];					// "YYYYMMDDHHMMSS\0"
	UC		EventSource;					// �¼���Դ������_pcC_EventSource
	UC		CallSource;						// ������Դ��Դ������_pcC_CallSource
	US		RoutePoint;						// �绰�����·�ɵ�
	US		GroupID;						// �绰�������ϯ����
	US		WaitTime;						// �Ŷӵȴ�ʱ�䣨�룩
	UL		TrunkID;						// �м̺�
	char	strANI[PCS_DEF_NUMLEN_APP];		// ����
	char	strDNIS[PCS_DEF_NUMLEN_APP];	// ����
	char	strOtherParty[PCS_DEF_NUMLEN_APP];		// �ڲ����жԷ�DN
	char	strThirdParty[PCS_DEF_NUMLEN_APP];		// ת�ӷ�����
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------
//--------------------------

//--------------------------
// �绰�Ҷ� - 3
//------------
#define TCP_AGT2PCS_CALL_CLEARCALL			52			// (0x0034) �绰�Ҷ�������Ϣ
struct S_TCP_AGT2PCS_CALL_CLEARCALL
{
	UL		CallRefID;						// ���б�ʾ�����Ϊ0��ϵͳ�Զ�ѡ��
	UC		EventSource;					// �¼���Դ������_pcC_EventSource�����Ϊ0��ϵͳ�Զ�ѡ��
};
//------------

//------------
#define TCP_PCS2AGT_CALL_CALLCLEARED		1052		// (0x041C) �绰�Ҷ�֪ͨ��Ϣ
struct S_TCP_PCS2AGT_CALL_CALLCLEARED
{
	UL		AgentID;						// ��ϯID
	UL		CallRefID;						// ���б�ʾ
	char	TimeStamp[15];					// "YYYYMMDDHHMMSS\0"
	UC		EventSource;					// �¼���Դ������_pcC_EventSource
	UC		Operator;						// �������壬����_pcC_CallOperator
	char	strPeerParty[PCS_DEF_NUMLEN_APP];		// �Է�DN
};
//------------

//------------
#define TCP_PCS2AGT_CALL_UNRINGING			1056		// (0x0420) �绰����֪ͨ��Ϣ
struct S_TCP_PCS2AGT_CALL_UNRINGING
{
	UL		AgentID;						// ��ϯID
	UL		CallRefID;						// ���б�ʾ
	char	TimeStamp[15];					// "YYYYMMDDHHMMSS\0"
	UC		EventSource;					// �¼���Դ������_pcC_EventSource
	UC		Operator;						// �������壬����_pcC_CallOperator
	char	strPeerParty[PCS_DEF_NUMLEN_APP];		// �Է�DN
};
//------------
//--------------------------

//--------------------------
// �绰Ӧ�� - 2
//------------
#define TCP_AGT2PCS_CALL_ANSWERCALL			53			// (0x0035) �绰Ӧ��������Ϣ
struct S_TCP_AGT2PCS_CALL_ANSWERCALL
{
	UL		CallRefID;						// ���б�ʾ�����Ϊ0��ϵͳ�Զ�ѡ��
	UC		EventSource;					// �¼���Դ������_pcC_EventSource�����Ϊ0��ϵͳ�Զ�ѡ��
};
//------------

//------------
#define TCP_PCS2AGT_CALL_CALLANSWERED		1053		// (0x041D) �绰Ӧ��֪ͨ��Ϣ
struct S_TCP_PCS2AGT_CALL_CALLANSWERED
{
	UL		AgentID;						// ��ϯID
	UL		CallRefID;						// ���б�ʾ
	char	TimeStamp[15];					// "YYYYMMDDHHMMSS\0"
	UC		EventSource;					// �¼���Դ������_pcC_EventSource
	UC		Operator;						// �������壬����_pcC_CallOperator
	char	strPeerParty[PCS_DEF_NUMLEN_APP];		// �Է�DN
};
//------------
//--------------------------

//--------------------------
// �绰���� - 6
//------------
#define TCP_AGT2PCS_CALL_HOLDCALL			54			// (0x0036) �绰����������Ϣ
struct S_TCP_AGT2PCS_CALL_HOLDCALL
{
	UL		CallRefID;						// ���б�ʾ�����Ϊ0��ϵͳ�Զ�ѡ��
	UC		EventSource;					// �¼���Դ������_pcC_EventSource�����Ϊ0��ϵͳ�Զ�ѡ��
};
//------------

//------------
#define TCP_PCS2AGT_CALL_CALLHELD			1054		// (0x041E) �绰����֪ͨ��Ϣ
struct S_TCP_PCS2AGT_CALL_CALLHELD
{
	UL		AgentID;						// ��ϯID
	UL		CallRefID;						// ���б�ʾ
	char	TimeStamp[15];					// "YYYYMMDDHHMMSS\0"
	UC		EventSource;					// �¼���Դ������_pcC_EventSource
	UC		Operator;						// �������壬����_pcC_CallOperator
	char	strPeerParty[PCS_DEF_NUMLEN_APP];		// �Է�DN
};
//------------

//------------
#define TCP_AGT2PCS_CALL_RETRIEVECALL		55			// (0x0037) �绰ʰ��������Ϣ
struct S_TCP_AGT2PCS_CALL_RETRIEVECALL
{
	UL		CallRefID;						// ���б�ʾ�����Ϊ0��ϵͳ�Զ�ѡ��
	UC		EventSource;					// �¼���Դ������_pcC_EventSource�����Ϊ0��ϵͳ�Զ�ѡ��
};
//------------

//------------
#define TCP_PCS2AGT_CALL_CALLRETRIEVED		1055		// (0x041F) �绰ʰ��֪ͨ��Ϣ
struct S_TCP_PCS2AGT_CALL_CALLRETRIEVED
{
	UL		AgentID;						// ��ϯID
	UL		CallRefID;						// ���б�ʾ
	char	TimeStamp[15];					// "YYYYMMDDHHMMSS\0"
	UC		EventSource;					// �¼���Դ������_pcC_EventSource
	UC		Operator;						// �������壬����_pcC_CallOperator
	char	strPeerParty[PCS_DEF_NUMLEN_APP];		// �Է�DN
};
//------------

//------------
#define TCP_AGT2PCS_CALL_RECONNECTCALL		56			// (0x0038) �绰��������������Ϣ
struct S_TCP_AGT2PCS_CALL_RECONNECTCALL
{
	UL		CallRefID;						// ���б�ʾ�����Ϊ0��ϵͳ�Զ�ѡ��
	UC		EventSource;					// �¼���Դ������_pcC_EventSource�����Ϊ0��ϵͳ�Զ�ѡ��
};

//------------
//--------------------------

//------------
// Simon Added 2011-12-15 ���ӵ绰�м���Ϣ֪ͨ
#define TCP_PCS2AGT_CALL_TRUNKINFO		1057		// (0x0421) �绰�м���Ϣ֪ͨ��Ϣ
struct S_TCP_PCS2AGT_CALL_TRUNKINFO
{
	UL		AgentID;						// ��ϯID
	UL		CallRefID;						// ���б�ʾ
	char	TrunkGroup[20];					// �м����
	char	TrunkMember[20];				// �м̳�Ա
    char	TimeStamp[15];                  // "YYYYMMDDHHMMSS\0"
    char	strANI[PCS_DEF_NUMLEN_APP];     // ����
    char	strDNIS[PCS_DEF_NUMLEN_APP];	// ����
    char	strOtherParty[PCS_DEF_NUMLEN_APP];         // �ڲ����жԷ�DN
    char	strThirdParty[PCS_DEF_NUMLEN_APP];         // ת�ӷ�����
    char	strAppData[PCS_DEF_DATALEN_APP];           // Application Data
    char	strUserData[PCS_DEF_DATALEN_USER];         // User Data
};
//------------
//--------------------------

//--------------------------
// �������� - 10
//------------
#define TCP_AGT2PCS_CALL_CONSULTATION		61			// (0x003D) ���д�����Ϣ
struct S_TCP_AGT2PCS_CALL_CONSULTATION
{
	UL		CallRefID;						// ���б�ʾ�����Ϊ0��ϵͳ�Զ�ѡ��
	UC		EventSource;					// �¼���Դ������_pcC_EventSource�����Ϊ0��ϵͳ�Զ�ѡ��
	UC		ConsultMode;					// ���д���ģʽ������_pcC_ConsultationMode
	UC		CallScale;						// ���з�Χ������_pcC_CallScale
	char	strCalledParty[PCS_DEF_NUMLEN_APP];		// �Ⲧ����
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------

//------------
#define TCP_AGT2PCS_CALL_TRANSFER			62			// (0x003E) �绰ת����Ϣ
struct S_TCP_AGT2PCS_CALL_TRANSFER
{
	UL		HoldRefID;						// ���ֺ��б�־�����Ϊ0��ϵͳ�Զ�ѡ��
	UL		CallRefID;						// ���б�ʾ�����Ϊ0��ϵͳ�Զ�ѡ��
	UC		EventSource;					// �¼���Դ������_pcC_EventSource�����Ϊ0��ϵͳ�Զ�ѡ��
};
//------------

//------------

//------------
#define TCP_AGT2PCS_CALL_CONFERENCE			63			// (0x003F) �绰������Ϣ
struct S_TCP_AGT2PCS_CALL_CONFERENCE
{
	UL		HoldRefID;						// ���ֺ��б�־�����Ϊ0��ϵͳ�Զ�ѡ��
	UL		CallRefID;						// ���б�ʾ�����Ϊ0��ϵͳ�Զ�ѡ��
	UC		EventSource;					// �¼���Դ������_pcC_EventSource�����Ϊ0��ϵͳ�Զ�ѡ��
};
//------------

//------------
#define TCP_AGT2PCS_CALL_SINGLESTEPTRANS	64			// (0x0040) ����ת����Ϣ
struct S_TCP_AGT2PCS_CALL_SINGLESTEPTRANS
{
	UL		CallRefID;						// ���б�ʾ�����Ϊ0��ϵͳ�Զ�ѡ��
	UC		EventSource;					// �¼���Դ������_pcC_EventSource�����Ϊ0��ϵͳ�Զ�ѡ��
	UC		CallScale;						// ���з�Χ������_pcC_CallScale
	char	strCalledParty[PCS_DEF_NUMLEN_APP];		// �Ⲧ����
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------

//------------
#define TCP_PCS2AGT_CALL_NETWORKREACHED		1061		// (0x0425) �Ⲧͨ������֪ͨ��Ϣ
struct S_TCP_PCS2AGT_CALL_NETWORKREACHED
{
	UL		AgentID;						// ��ϯID
	UL		CallRefID;						// ���б�ʾ
	char	TimeStamp[15];					// "YYYYMMDDHHMMSS\0"
	UC		EventSource;					// �¼���Դ������_pcC_EventSource
	char	strPeerParty[PCS_DEF_NUMLEN_APP];		// �Է�DN
};
//------------

//------------
#define TCP_PCS2AGT_CALL_TRANSFERED			1062		// (0x0426) ת�����֪ͨ��Ϣ
struct S_TCP_PCS2AGT_CALL_TRANSFERED
{
	UL		AgentID;						// ��ϯID
	UL		CallRefID;						// ���б�ʾ
	char	TimeStamp[15];					// "YYYYMMDDHHMMSS\0"
	UC		EventSource;					// �¼���Դ������_pcC_EventSource
	char	strOtherParty[PCS_DEF_NUMLEN_APP];		// ���жԷ�DN
	char	strThirdParty[PCS_DEF_NUMLEN_APP];		// ת�ӷ�����
};
//------------

//------------
#define TCP_PCS2AGT_CALL_CONFERENCED		1063		// (0x0427) �������֪ͨ��Ϣ
struct S_TCP_PCS2AGT_CALL_CONFERENCED
{
	UL		AgentID;						// ��ϯID
	UL		CallRefID;						// ���б�ʾ
	char	TimeStamp[15];					// "YYYYMMDDHHMMSS\0"
	UC		EventSource;					// �¼���Դ������_pcC_EventSource
	char	strOtherParty[PCS_DEF_NUMLEN_APP];		// ���жԷ�DN
	char	strThirdParty[PCS_DEF_NUMLEN_APP];		// ���鷽����
};
//------------

//------------
#define TCP_PCS2AGT_CALL_DESTINATIONBUSY	1064		// (0x0428) �Ⲧ��æ֪ͨ��Ϣ
struct S_TCP_PCS2AGT_CALL_DESTINATIONBUSY
{
	UL		AgentID;						// ��ϯID
	UL		CallRefID;						// ���б�ʾ
	char	TimeStamp[15];					// "YYYYMMDDHHMMSS\0"
	UC		EventSource;					// �¼���Դ������_pcC_EventSource
	char	strPeerParty[PCS_DEF_NUMLEN_APP];		// �Է�DN
};
//------------

//------------
#define TCP_PCS2AGT_CALL_DESTINATIONINVALID	1065		// (0x0429) �ⲦĿ����Ч֪ͨ��Ϣ
struct S_TCP_PCS2AGT_CALL_DESTINATIONINVALID
{
	UL		AgentID;						// ��ϯID
	UL		CallRefID;						// ���б�ʾ
	char	TimeStamp[15];					// "YYYYMMDDHHMMSS\0"
	UC		EventSource;					// �¼���Դ������_pcC_EventSource
	char	strPeerParty[PCS_DEF_NUMLEN_APP];		// �Է�DN
};
//------------

//------------
#define TCP_PCS2AGT_CALL_CONSULTATION_INIT	1066		// (0x042A) ת�ӡ������ʼ�������Ϣ
struct S_TCP_PCS2AGT_CALL_CONSULTATION_INIT
{
	UL		AgentID;						// ��ϯID
	UL		CallRefID;						// ���б�ʾ
	UL		HoldRefID;						// ���ֺ��б�־
	char	TimeStamp[15];					// "YYYYMMDDHHMMSS\0"
	UC		EventSource;					// �¼���Դ������_pcC_EventSource
	UC		ConsultMode;					// ���д���ģʽ������_pcC_ConsultationMode
	char	strPeerParty[PCS_DEF_NUMLEN_APP];		// �Է�DN
};
//------------
//--------------------------

//--------------------------
// פ����ȡ�أ���չ���ܣ�����ʹ��Softphone�� - 5
//------------
#define TCP_AGT2PCS_CALL_PARKING			71			// (0x0047) ����פ������
struct S_TCP_AGT2PCS_CALL_PARKING
{
	UL		CallRefID;						// ���б�ʾ�����Ϊ0��ϵͳ�Զ�ѡ��
	US		ParkedRoutePoint;				// פ���绰��·�ɵ�ID
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------

//------------
#define TCP_AGT2PCS_CALL_FETCHING			72			// (0x0048) ����ȡ������
struct S_TCP_AGT2PCS_CALL_FETCHING
{
	UL		CallRefID;						// ���б�ʾ
	US		ParkedRoutePoint;				// פ���绰��·�ɵ�ID
};
//------------

//------------
#define TCP_PCS2AGT_CALL_PARKING_ACK		1071		// (0x042F) ����פ����Ӧ��Ϣ
struct S_TCP_PCS2AGT_CALL_PARKING_ACK
{
	UL		CallRefID;						// ���б�ʾ
	US		ParkedRoutePoint;				// פ���绰��·�ɵ�ID
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
#define TCP_PCS2AGT_CALL_FETCHING_ACK		1072		// (0x0430) ����ȡ����Ӧ��Ϣ
struct S_TCP_PCS2AGT_CALL_FETCHING_ACK
{
	US		ParkedRoutePoint;				// פ���绰��·�ɵ�ID
	UL		CallRefID;						// ���б�ʾ
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_NOTSUPPORT
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2AGT_CALL_PARK_MISSED		1073		// (0x0431) פ�����ж�ʧ��Ϣ
struct S_TCP_PCS2AGT_CALL_PARK_MISSED
{
	UL		CallRefID;						// ���б�ʾ
	US		ParkedRoutePoint;				// פ���绰��·�ɵ�ID
};
//------------
//--------------------------

//--------------------------
// ������չ���ܣ�����ʹ��Softphone - 5
//------------
#define TCP_AGT2PCS_SEND_DTMF				81			// (0x0051) ����DTMF������Ϣ
struct S_TCP_AGT2PCS_SEND_DTMF
{
	UL		CallRefID;						// ���б�ʾ�����Ϊ0��ϵͳ�Զ�ѡ��
	UC		EventSource;					// �¼���Դ������_pcC_EventSource�����Ϊ0��ϵͳ�Զ�ѡ��
	char	strDTMF[PCS_DEF_NUMLEN_APP];	// ��Ҫ���͵�DTMF��
};
//------------

//------------
#define TCP_AGT2PCS_TELNO_PARSE				82			// (0x0052) �绰�������������Ϣ
struct S_TCP_AGT2PCS_TELNO_PARSE
{
	UL		InvokeID;						// ���ñ��
	UC		CallType;						// �������ͣ���Ҫ���ֺ���/����������_pcC_CallType
	UC		blnConnected;					// �Ƿ��ͨ�������������������ṩ���μ������س������塱
	char	strOrigNo[PCS_DEF_NUMLEN_APP];	// ԭʼ����
};
//------------

//------------
#define TCP_PCS2AGT_TELNO_PARSE_ACK			1082		// (0x043A) �绰���������Ӧ��Ϣ
struct S_TCP_PCS2AGT_TELNO_PARSE_ACK
{
	UL		InvokeID;						// ���ñ��
	UC		CallType;						// �������ͣ���Ҫ���ֺ���/����������_pcC_CallType
	UC		blnConnected;					// �Ƿ��ͨ�������������������ṩ���μ������س������塱
	char	strOrigNo[PCS_DEF_NUMLEN_APP];	// ԭʼ����
	UC		bytCallScale;					// ������������з�Χ�������������μ�_pcC_CallScale
	char	cIPIndex;						// ���������IPǰ׺��ţ���������
	char	strCountryCode[8];				// ������������Ҵ���
	char	strCityCode[8];					// �������������
	char	strPureNo[PCS_DEF_NUMLEN_APP];	// �������������
};
//------------

//------------
// Simon Added 2011-12-15
#define TCP_AGT2PCS_CALL_PICKUP				83			// (0x0053) ������д���
struct S_TCP_AGT2PCS_CALL_PICKUP
{
	UC		bytScale;						// ����Ŀ�� �ο�:_pcC_CallDistributeObject
	char	strDestination[50];				// Ŀ�����
};

#define TCP_PCS2AGT_CALL_PICKUP_ACK			1083		// (0x0453) ��Ӧ���д���
struct S_TCP_PCS2AGT_CALL_PICKUP_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST: ָ���Ĵ���Ŀ�겻����
											// PCS_DEF_RETCODE_BEUSED: ����ϯ��������ͨ�������ܴ��������绰
											// PCS_DEF_RETCODE_STATUS: ָ���Ĵ���Ŀ��û�п��Դ���ĵ绰���磺�ֻ���������״̬������û�������������ϯ�ȣ�
											// PCS_DEF_RETCODE_VERSION: �˰汾��֧�ֱ�����
											// PCS_DEF_RETCODE_NORIGHTS: ��ϯ�޴���Ȩ��
											// PCS_DEF_RETCODE_LACK_PARAM: ��������
											// PCS_DEF_RETCODE_REJECTED: ϵͳ�ܾ�����
											// PCS_DEF_RETCODE_NOTSUPPORT: ��������֧�ֱ�����
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		CallRefID;						// ������ĺ��б�ʾ
	char	strPickupDN[PCS_DEF_NUMLEN_APP];	// ������ķֻ���
};
//------------
//--------------------------


//--------------------------
// ���� - 1
//------------
#define TCP_PCS2AGT_CALL_FUNCTION_ACK		1091		// (0x0443) ���п�����Ӧ��Ϣ
struct S_TCP_PCS2AGT_CALL_FUNCTION_ACK
{
	US		FunctionCode;					// ���ܴ���
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_LACK_PARAM
											// PCS_DEF_RETCODE_NOTSUPPORT
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------


//--------------------------
// ������Ϣ - Total 14
//--------------------------

//--------------------------
// �޸Ŀ��� - 2
//------------
#define TCP_AGT2PCS_CHANGE_PASSWORD			101			// (0x0065) �޸Ŀ�����Ϣ
struct S_TCP_AGT2PCS_CHANGE_PASSWORD
{
	char	strPassword[20];				// �¿���
};
//------------

//------------
#define TCP_PCS2AGT_CHANGE_PASSWORD_ACK		1101		// (0x044D) �޸Ŀ�����Ӧ��Ϣ
struct S_TCP_PCS2AGT_CHANGE_PASSWORD_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------

//--------------------------
// �û��������� - 6
//------------
#define TCP_AGT2PCS_GET_AGTPARAM			102			// (0x0066) ��ѯ�û�����
struct S_TCP_AGT2PCS_GET_AGTPARAM
{
	char	strParamName[50];				// ������
};
//------------

//------------
#define TCP_AGT2PCS_CHG_AGTPARAM			103			// (0x0067) �޸��û�����
struct S_TCP_AGT2PCS_CHG_AGTPARAM
{
	char	strParamName[50];				// ������
	char	strParamValue[100];				// ����ֵ
};
//------------

//------------
#define TCP_PCS2AGT_GET_AGTPARAM_ACK		1102		// (0x044E) ��ѯ�û�������Ӧ��Ϣ
struct S_TCP_PCS2AGT_GET_AGTPARAM_ACK
{
	char	strParamName[50];				// ������
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	char	strParamValue[100];				// ����ֵ
};
//------------

//------------
#define TCP_PCS2AGT_CHG_AGTPARAM_ACK		1103		// (0x044F) �޸��û�������Ӧ��Ϣ
struct S_TCP_PCS2AGT_CHG_AGTPARAM_ACK
{
	char	strParamName[50];				// ������
	char	strParamValue[100];				// ����ֵ
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2PCS_GET_LEAVEREASONS		104			// (0x0068) �����ȡ�����б�
//------------

//------------
#define TCP_PCS2AGT_LEAVEREASONS_LIST		1104		// (0x0450) �����б�
struct S_TCP_PCS2AGT_LEAVEREASONS_LIST
{
	UC		ReasonCount;					// ��������
	UC		ReasonValue[10];				// ���ɱ��0-9
	char	ReasonName[10][20];				// ��������0-9
};
//------------

//--------------------------
//Simon Added 2011-12-15
#define TCP_AGT2PCS_RESET_OBANI				105			// (0x0069) ����OB���к�������
struct S_TCP_AGT2PCS_RESET_OBANI
{
	char	strANI[50];				// ���к���
};

//------------
//--------------------------



//--------------------------
// �೤ǿ�� - 2
//------------
#define TCP_AGT2PCS_MONITOR_BREAK			111			// (0x006F) �೤ǿ����Ϣ
struct S_TCP_AGT2PCS_MONITOR_BREAK
{
	UL		ControlAgentID;					// ��������ϯID
};
//------------

//------------
#define TCP_PCS2AGT_MONITOR_BREAK_ACK		1111		// (0x0457) �೤ǿ����Ӧ��Ϣ
struct S_TCP_PCS2AGT_MONITOR_BREAK_ACK
{
	UL		ControlAgentID;					// ��������ϯID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------

//--------------------------
// �೤���� - 2
//------------
#define TCP_AGT2PCS_MONITOR_HOLDUP			112			// (0x0070) �೤������Ϣ
struct S_TCP_AGT2PCS_MONITOR_HOLDUP
{
	UL		ControlAgentID;					// ��������ϯID
};
//------------

//------------
#define TCP_PCS2AGT_MONITOR_HOLDUP_ACK		1112		// (0x0458) �೤������Ӧ��Ϣ
struct S_TCP_PCS2AGT_MONITOR_HOLDUP_ACK
{
	UL		ControlAgentID;					// ��������ϯID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------

//--------------------------
// �೤ǿ�� - 2
//------------
#define TCP_AGT2PCS_MONITOR_INSERT			113			// (0x0071) �೤ǿ����Ϣ
struct S_TCP_AGT2PCS_MONITOR_INSERT
{
	UL		ControlAgentID;					// ��������ϯID
};
//------------

//------------
#define TCP_PCS2AGT_MONITOR_INSERT_ACK		1113		// (0x0459) �೤ǿ����Ӧ��Ϣ
struct S_TCP_PCS2AGT_MONITOR_INSERT_ACK
{
	UL		ControlAgentID;					// ��������ϯID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------

//--------------------------
// �೤ǿ��ע�� - 2
//------------
#define TCP_AGT2PCS_MONITOR_LOGOUT			114			// (0x0072) �೤ǿ��ע����Ϣ
struct S_TCP_AGT2PCS_MONITOR_LOGOUT
{
	UL		ControlAgentID;					// ��������ϯID
};
//------------

//------------
#define TCP_PCS2AGT_MONITOR_LOGOUT_ACK		1114		// (0x045A) �೤ǿ��ע����Ӧ��Ϣ
struct S_TCP_PCS2AGT_MONITOR_LOGOUT_ACK
{
	UL		ControlAgentID;					// ��������ϯID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------

//--------------------------
// ������Ϣ������Ϣ - Total 4
//--------------------------
//------------
#define TCP_AGT2PCS_MSGRSTART_ACK			121			// (0x0079) ��Ϣ����������Ӧ��Ϣ
struct S_TCP_AGT2PCS_MSGRSTART_ACK
{
	UC		MessagerClientStarted;			// ��Ϣ����ͻ����Ƿ��������μ������س������塱
};
//------------

//------------
#define TCP_AGT2PCS_MSGRSTOP_ACK			122			// (0x007A) ��Ϣ����ֹͣ��Ӧ��Ϣ
//------------

//------------
#define TCP_PCS2AGT_MSGRSTART				1121		// (0x0461) ��Ϣ����������Ϣ������ӦȨ�޲Ż��յ�����Ϣ
struct S_TCP_PCS2AGT_MSGRSTART
{
	US		nMSGPort;						// ��Ϣ�����������˿�
	char	strMSGRIP[20];					// ��Ϣ������IP��ַ
};
//------------

//------------
#define TCP_PCS2AGT_MSGRSTOP				1122		// (0x0462) ��Ϣ����ֹͣ��Ϣ
//------------
//--------------------------

//--------------------------
// ¼��������Ϣ - Total 8
//--------------------------
//------------
#define TCP_AGT2PCS_MAXISTART_ACK			131			// (0x0083) ¼������������Ӧ��Ϣ
struct S_TCP_AGT2PCS_MAXISTART_ACK
{
	UC		MaxiSenseClientStarted;			// ¼������ͻ����Ƿ��������μ������س������塱
};
//------------

//------------
#define TCP_AGT2PCS_MAXISTOP_ACK			132			// (0x0084) ¼������ֹͣ��Ӧ��Ϣ
//------------

//------------
#define TCP_PCS2AGT_MAXISTART				1131		// (0x046B) ¼������������Ϣ������ӦȨ�޲Ż��յ�����Ϣ
struct S_TCP_PCS2AGT_MAXISTART
{
	US		nMaxiSensePort;					// ¼����������������˿�
	char	strMaxiSenseIP[20];				// ¼�����������IP��ַ
};
//------------

//------------
#define TCP_PCS2AGT_MAXISTOP				1132		// (0x046C) ¼������ֹͣ��Ϣ
//------------

//------------------------------------------------------------------------------
// Sun added 2010-05-12
// ¼��״̬֪ͨ
//------------
#define TCP_AGT2PCS_REC_USERDATA			135			// (0x0087) ׷���û����ݵ����һ�γɹ�¼��
struct S_TCP_AGT2PCS_REC_USERDATA
{
	char	UserData[200];					// �û�����
};
//------------

//------------
#define TCP_AGT2PCS_REC_RECORDDATA			136			// (0x0088) ¼������������Ϣ
struct S_TCP_AGT2PCS_REC_RECORDDATA
{
	char	AttachData1[16];				// ����������1
	char	AttachData2[16];				// ����������2
	char	AttachData3[32];				// ����������3
	char	AttachData4[32];				// ����������4
	char	AttachData5[256];				// ����������5
};
//------------

//------------
#define TCP_PCS2AGT_RECORD_STARTED			1135		// (0x046F) ¼����ʼ֪ͨ
struct S_TCP_PCS2AGT_RECORD_STARTED
{
	char	RecordTime[15];					// ¼����ʼʱ�䣬PK
	char	UserName[20];					// �û�����PK
	UC		RecordStartType;				// ¼���������Ʒ�ʽ���μ���¼�����Ʒ�ʽ��
	char	FilePath[128];					// ¼���ļ�ȫ·��
};
//------------

//------------
#define TCP_PCS2AGT_RECORD_STOPED			1136		// (0x0470) ¼��ֹ֪ͣͨ
struct S_TCP_PCS2AGT_RECORD_STOPED
{
	char	RecordTime[15];					// ¼����ʼʱ�䣬PK
	char	UserName[20];					// �û�����PK
	UC		RecordEndType;					// ¼��ֹͣ���Ʒ�ʽ���μ���¼�����Ʒ�ʽ��
	UL		RecordDuration;					// ¼��ʱ��
	char	FilePath[128];					// ¼���ļ�ȫ·��
};
//------------
// End of Sun added 2010-05-12
//------------------------------------------------------------------------------
//--------------------------


//--------------------------
// UMS���� - Total 13
//--------------------------

//--------------------------
// ͳһ������� - 2
//------------
#define TCP_AGT2PCS_UMS_DISPATCHTASK_ACK	3201		// (0x0C81) ͳһ���������Ӧ��Ϣ
struct S_TCP_AGT2PCS_UMS_DISPATCHTASK_ACK
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
	US		RoutePointID;							// ·�ɵ�ID
	UC		ErrorCode;								// �����룬�μ������س������塱
	UL		ReasonCode;								// ���ԭ�����
	char	strRefField1[10];						// ��������ֶ�1
	char	strRefField2[20];						// ��������ֶ�2
	char	strRefField3[20];						// ��������ֶ�3
	char	strRefField4[50];						// ��������ֶ�4
	char	strRefField5[100];						// ��������ֶ�5
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------

//------------
#define TCP_PCS2AGT_UMS_DISPATCHTASK_NTY	3701		// (0x0E75) ͳһ���������Ϣ
struct S_TCP_PCS2AGT_UMS_DISPATCHTASK_NTY
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
	UC		UMSType;								// UMS�������ͣ��μ�_pcC_UMSTaskTypes
	UC		UMSIndex;								// UMS��ϵͳ���
	UC		CustID;									// �⻧����
	US		RoutePointID;							// ·�ɵ�ID
	US		GroupID;								// ��ID
	US		TeamID;									// С��ID
	UC		TeamIndex;								// С�����ȼ�
	UC		UserIndex;								// ����Ա���ȼ�
	char	strCallingParty[PCS_DEF_NUMLEN_APP];	// ������
	char	strCalledParty[PCS_DEF_NUMLEN_APP];		// ������
	UL		ClassifyCode;							// �������
	UL		WaitTime;								// �ѵȴ�ʱ�䣨�룩
	char	ServiceCode0;							// ���������
	char	ServiceCode1;							// �����������
	char	ServiceCode2;							// �����������
	char	strTaskTitle[50];						// �������
	char	strTaskContent[200];					// ��������
	char	strHotLink[100];						// ����ļ�·�������ӵ�ַ
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------
//--------------------------

//--------------------------
// ͳһ����ȡ�� - 3
//------------
#define TCP_AGT2PCS_UMS_ABANDONTASK_REQ		3202		// (0x0C82) ͳһ���������Ϣ
struct S_TCP_AGT2PCS_UMS_ABANDONTASK_REQ
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
	UL		ReasonCode;								// ���ԭ�����
	char	strRefField1[10];						// ��������ֶ�1
	char	strRefField2[20];						// ��������ֶ�2
	char	strRefField3[20];						// ��������ֶ�3
	char	strRefField4[50];						// ��������ֶ�4
	char	strRefField5[100];						// ��������ֶ�5
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------

//------------
#define TCP_AGT2PCS_UMS_CANCELTASK_ACK		3203		// (0x0C83) ͳһ����ȡ����Ӧ��Ϣ
struct S_TCP_AGT2PCS_UMS_CANCELTASK_ACK
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
};
//------------

//------------
#define TCP_PCS2AGT_UMS_CANCELTASK_NTY		3702		// (0x0E76) ͳһ����ȡ����Ϣ
struct S_TCP_PCS2AGT_UMS_CANCELTASK_NTY
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
	US		RoutePointID;							// ·�ɵ�ID
};
//------------
//--------------------------

//--------------------------
// ͳһ������� - 2
//------------
#define TCP_AGT2PCS_UMS_FINISHTASK_REQ		3205		// (0x0C85) ͳһ���������Ϣ
struct S_TCP_AGT2PCS_UMS_FINISHTASK_REQ
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
	UL		ReasonCode;								// ���ԭ�����
	char	strRefField1[10];						// ��������ֶ�1
	char	strRefField2[20];						// ��������ֶ�2
	char	strRefField3[20];						// ��������ֶ�3
	char	strRefField4[50];						// ��������ֶ�4
	char	strRefField5[100];						// ��������ֶ�5
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------

//------------
#define TCP_PCS2AGT_UMS_FINISHTASK_NTY		3705		// (0x0E79) ͳһ���������Ӧ��Ϣ
struct S_TCP_PCS2AGT_UMS_FINISHTASK_NTY
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
	US		RoutePointID;							// ·�ɵ�ID
};
//------------
//--------------------------

//--------------------------
// ��������ȡ�� - 3
//------------
#define TCP_AGT2PCS_UMS_ABANDONALLTASK_REQ	3208		// (0x0C88) ȫ�����������Ϣ
struct S_TCP_AGT2PCS_UMS_ABANDONALLTASK_REQ
{
	UL		ReasonCode;								// ���ԭ�����
	char	strRefField1[10];						// ��������ֶ�1
	char	strRefField2[20];						// ��������ֶ�2
	char	strRefField3[20];						// ��������ֶ�3
	char	strRefField4[50];						// ��������ֶ�4
	char	strRefField5[100];						// ��������ֶ�5
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------

//------------
#define TCP_AGT2PCS_UMS_CANCELALLTASK_ACK	3209		// (0x0C89) ȫ������ȡ����Ӧ��Ϣ
struct S_TCP_AGT2PCS_UMS_CANCELALLTASK_ACK
{
	UL		TaskCount;								// ��������
};
//------------

//------------
#define TCP_PCS2AGT_UMS_CANCELALLTASK_NTY	3708		// (0x0E7C) ȫ������ȡ����Ϣ
struct S_TCP_PCS2AGT_UMS_CANCELALLTASK_NTY
{
	UL		TaskCount;								// ��������
};
//------------
//--------------------------

//--------------------------
// ��ѯ�����嵥 - 3
//------------
#define TCP_AGT2PCS_UMS_QUERYTASKLIST_REQ	3211		// (0x0C8B) ��ѯ��ϯ�����嵥��Ϣ
//------------

//------------
#define TCP_PCS2AGT_UMS_QUERYTASKLIST_ACK	3711		// (0x0E7F) ��ѯ��ϯ�����嵥��Ӧ��Ϣ
struct S_TCP_PCS2AGT_UMS_QUERYTASKLIST_ACK
{
	UL		TaskCount;								// ��������
};
//------------

//------------
#define TCP_PCS2AGT_UMS_TASK_DATA			3712		// (0x0E78) ͳһ����������Ϣ
struct S_TCP_PCS2AGT_UMS_TASK_DATA
{
	UL		TaskIndex;								// �����ţ�0 based
	US		TaskStatus;								// ͳһ��Ϣ����״̬
	S_TCP_PCS2AGT_UMS_DISPATCHTASK_NTY TaskData;
};
//------------
//--------------------------

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_AGENT_H__
//------------------------------------------------------------------------
