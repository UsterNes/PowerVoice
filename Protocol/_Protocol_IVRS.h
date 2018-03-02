////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		_Protocol_IVRS.h
// ��;������	IVRͨ��Э��
// ���ߣ�		SUN
// ����ʱ�䣺	1999/12/01
// ����޸�ʱ�䣺2004/04/05
// ��Ȩ��Ϣ��	Copyright (C) 1999 - 2004 PCSG
// ��ע��
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_IVRS_H__
#define __PROTOCOL_IVRS_H__
//------------------------------------------------------------------------

//--------------------------
// IVRע�� - Total 4
//--------------------------

//--------------------------
// ע�� - 2
//------------
#define TCP_IVR2PCS_REGISTER				1			// (0x0001) ע����Ϣ
struct S_TCP_IVR2PCS_REGISTER
{
	UC		SysNo;							// ϵͳ����
	UC		GroupCount;						// ������
	US		ChannelCount;					// ͨ������
	UC		Version;						// �汾��
	UC		AutoLogout;						// �Ͽ�����ʱͨ���Ƿ��Զ��˳���¼���μ������س������塱
	UC		RegCode[PCS_DEF_NUMLEN_APP];	// ע����
};
//------------

//------------
#define TCP_PCS2IVR_REGISTER_ACK			1001		// (0x03E9) ע����Ӧ��Ϣ
struct S_TCP_PCS2IVR_REGISTER_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_BEUSED
											// PCS_DEF_RETCODE_STATUS
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
#define TCP_IVR2PCS_UNREGISTER				2			// (0x0002) ȡ��ע����Ϣ
//------------

//------------
#define TCP_PCS2IVR_UNREGISTER_ACK			1002		// (0x03EA) ȡ��ע����Ӧ��Ϣ
//------------
//--------------------------
//--------------------------


//--------------------------
// IVR�豸���� - Total 4
//--------------------------

//--------------------------
// ͨ������ - 4
//------------
#define TCP_IVR2PCS_CH_OPEN					11			// (0x000B) �豸��ʼ�����󣬵�¼CTI��Ϣ
struct S_TCP_IVR2PCS_CH_OPEN
{
	UL		AgentID;						// CTI AgentID
};
//------------

//------------
#define TCP_IVR2PCS_CH_CLOSE				12			// (0x000C) �豸�ر�������Ϣ
struct S_TCP_IVR2PCS_CH_CLOSE
{
	UL		AgentID;						// CTI AgentID
};
//------------

//------------
#define TCP_PCS2IVR_CH_OPEN_ACK				1011		// (0x03F3) �豸��ʼ����Ӧ��Ϣ
struct S_TCP_PCS2IVR_CH_OPEN_ACK
{
	UL		AgentID;						// CTI AgentID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2IVR_CH_CLOSE_ACK			1012		// (0x03F4) �豸�ر���Ӧ��Ϣ
struct S_TCP_PCS2IVR_CH_CLOSE_ACK
{
	UL		AgentID;						// CTI AgentID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------


//--------------------------
// IVR ���п��� - Total 10
//--------------------------

//------------
// �绰�Ⲧ
//------------
#define TCP_IVR2PCS_CALL_CALLOUT			51			// (0x0033) �绰�Ⲧ��Ϣ
struct S_TCP_IVR2PCS_CALL_CALLOUT
{
	UL		AgentID;								// CTI AgentID
	UC		CallScale;								// ���з�Χ������_pcC_CallScale
	char	strCalledParty[PCS_DEF_NUMLEN_APP];		// �Ⲧ����
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------

//------------
// �绰�Ҷ�
//------------
#define TCP_IVR2PCS_CALL_CLEARCALL			52			// (0x0034) �绰�Ҷ�������Ϣ
struct S_TCP_IVR2PCS_CALL_CLEARCALL
{
	UL		AgentID;								// CTI AgentID
	UL		CallRefID;								// ���б�ʾ�����Ϊ0��ϵͳ�Զ�ѡ��
	UC		EventSource;							// �¼���Դ������_pcC_EventSource�����Ϊ0��ϵͳ�Զ�ѡ��
};
//------------

//------------
// �绰Ӧ��
//------------
#define TCP_IVR2PCS_CALL_ANSWERCALL			53			// (0x0035) �绰Ӧ��������Ϣ
struct S_TCP_IVR2PCS_CALL_ANSWERCALL
{
	UL		AgentID;								// CTI AgentID
	UL		CallRefID;								// ���б�ʾ�����Ϊ0��ϵͳ�Զ�ѡ��
	UC		EventSource;							// �¼���Դ������_pcC_EventSource�����Ϊ0��ϵͳ�Զ�ѡ��
};
//------------

//------------
// �绰����
//------------
#define TCP_IVR2PCS_CALL_HOLDCALL			54			// (0x0036) �绰����������Ϣ
struct S_TCP_IVR2PCS_CALL_HOLDCALL
{
	UL		AgentID;								// CTI AgentID
	UL		CallRefID;								// ���б�ʾ�����Ϊ0��ϵͳ�Զ�ѡ��
	UC		EventSource;							// �¼���Դ������_pcC_EventSource�����Ϊ0��ϵͳ�Զ�ѡ��
};
//------------

//------------
// �绰ʰ��
//------------
#define TCP_IVR2PCS_CALL_RETRIEVECALL		55			// (0x0037) �绰ʰ��������Ϣ
struct S_TCP_IVR2PCS_CALL_RETRIEVECALL
{
	UL		AgentID;								// CTI AgentID
	UL		CallRefID;								// ���б�ʾ�����Ϊ0��ϵͳ�Զ�ѡ��
	UC		EventSource;							// �¼���Դ������_pcC_EventSource�����Ϊ0��ϵͳ�Զ�ѡ��
};
//------------

//------------
// ���д���
//------------
#define TCP_IVR2PCS_CALL_CONSULTATION		61			// (0x003D) ���д�����Ϣ
struct S_TCP_IVR2PCS_CALL_CONSULTATION
{
	UL		AgentID;								// CTI AgentID
	UL		CallRefID;								// ���б�ʾ�����Ϊ0��ϵͳ�Զ�ѡ��
	UC		EventSource;							// �¼���Դ������_pcC_EventSource�����Ϊ0��ϵͳ�Զ�ѡ��
	UC		ConsultMode;							// ���д���ģʽ������_pcC_ConsultationMode
	UC		CallScale;								// ���з�Χ������_pcC_CallScale
	char	strCalledParty[PCS_DEF_NUMLEN_APP];		// �Ⲧ����
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------

//------------
// �绰ת��
//------------
#define TCP_IVR2PCS_CALL_TRANSFER			62			// (0x003E) �绰ת����Ϣ
struct S_TCP_IVR2PCS_CALL_TRANSFER
{
	UL		AgentID;								// CTI AgentID
	UL		HoldRefID;								// ���ֺ��б�־�����Ϊ0��ϵͳ�Զ�ѡ��
	UL		CallRefID;								// ���б�ʾ�����Ϊ0��ϵͳ�Զ�ѡ��
	UC		EventSource;							// �¼���Դ������_pcC_EventSource�����Ϊ0��ϵͳ�Զ�ѡ��
};
//------------

//------------
// �绰����
//------------
#define TCP_IVR2PCS_CALL_CONFERENCE			63			// (0x003F) �绰������Ϣ
struct S_TCP_IVR2PCS_CALL_CONFERENCE
{
	UL		AgentID;								// CTI AgentID
	UL		HoldRefID;								// ���ֺ��б�־�����Ϊ0��ϵͳ�Զ�ѡ��
	UL		CallRefID;								// ���б�ʾ�����Ϊ0��ϵͳ�Զ�ѡ��
	UC		EventSource;							// �¼���Դ������_pcC_EventSource�����Ϊ0��ϵͳ�Զ�ѡ��
};
//------------

//------------
// ����ת��
//------------
#define TCP_IVR2PCS_CALL_SINGLESTEPTRANS	64			// (0x0040) ����ת����Ϣ
struct S_TCP_IVR2PCS_CALL_SINGLESTEPTRANS
{
	UL		AgentID;								// CTI AgentID
	UL		CallRefID;								// ���б�ʾ�����Ϊ0��ϵͳ�Զ�ѡ��
	UC		EventSource;							// �¼���Դ������_pcC_EventSource�����Ϊ0��ϵͳ�Զ�ѡ��
	UC		CallScale;								// ���з�Χ������_pcC_CallScale
	char	strCalledParty[PCS_DEF_NUMLEN_APP];		// �Ⲧ����
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------

//------------
#define TCP_IVR2PCS_SET_MSGWAITING			119			// (0x0077) �������Եȴ�ָʾ������Ϣ
struct S_TCP_IVR2PCS_CALL_MSGWAITING
{
	UL		AgentID;								// CTI AgentID
	char	strDN[20];								// DN����
	UC		SetMWADOn;								// �Ƿ��趨���Եȴ�ָʾ���أ��μ������س������塱
};
//------------
//--------------------------

//--------------------------
// ACD ����״̬��ѯ - Total 8
//--------------------------
//------------
#define TCP_IVR2PCS_GET_ACDSTATUS			20			// (0x0014) ��ѯACD����״̬��Ϣ
struct S_TCP_IVR2PCS_GET_ACDSTATUS
{
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// IVR Channel ID who requests the data
	char	strACDDN[20];					// ACD DN
};
//------------

//------------
#define TCP_IVR2PCS_GET_RPTSTATUS			21			// (0x0015) ��ѯ·�ɵ�״̬��Ϣ
struct S_TCP_IVR2PCS_GET_RPTSTATUS
{
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// IVR Channel ID who requests the data
	UC		nType;							// �������Center / RoutePoint / Queue / Group / Team����
											/// �μ�_pcC_CallDistributeObject����
	US		nGroupID;						// ����ID
};
//------------

//------------
#define TCP_IVR2PCS_GET_AGENTSTATUS			22			// (0x0016) ��ѯ��ϯ״̬��Ϣ
struct S_TCP_IVR2PCS_GET_AGENTSTATUS
{
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// IVR Channel ID who requests the data
	UC		UserQuery;						// ��ѯ���ͣ�0:Agent; 1:UserID; 2:LoginID;
	UL		AgentID;						// ��ϯID �� ��¼����
	char	strAliasName[50];				// ��¼����
};
//------------

// Sun added 2007-04-15
//------------
#define TCP_IVR2PCS_GET_CALL_EWT			23			// (0x0017) ĳ�������ض�·�ɵ��EWT��ֵ
struct S_TCP_IVR2PCS_GET_CALL_EWT
{
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// IVR Channel ID who requests the data
	UL		AgentID;						// CTI AgentID
	UL		CallRefID;						// ���б�ʾ�����Ϊ0��ϵͳ�Զ�ѡ��
};
//------------

//------------
#define TCP_IVR2PCS_CALL_PREQUEUE_REQ		30			// (0x001E) ����Ԥ�Ŷ�����
struct S_TCP_IVR2PCS_CALL_PREQUEUE_REQ
{
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// IVR Channel ID who requests the data
	UL		AgentID;						// CTI AgentID
	UL		CallRefID;						// ���б�ʾ�����Ϊ0��ϵͳ�Զ�ѡ��
	US		nRoutePointID;					// ·�ɵ�ID
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------

//------------
#define TCP_PCS2IVR_GET_ACDSTATUS_ACK		1020			// (0x03FC) ��ѯACD����״̬��Ӧ��Ϣ
struct S_TCP_PCS2IVR_GET_ACDSTATUS_ACK
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

//------------
#define TCP_PCS2IVR_GET_RPTSTATUS_ACK		1021			// (0x03FD) ��ѯ·�ɵ�״̬��Ӧ��Ϣ
struct S_TCP_PCS2IVR_GET_RPTSTATUS_ACK
{
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// IVR Channel ID who requests the data
	UC		nType;							// �������Center / RoutePoint / Queue / Group / Team����
											/// �μ�_pcC_CallDistributeObject����
	US		nGroupID;						// ����ID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS (RM������)
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	/// Call Data
	US		nCallsWait;				// �Ŷӵȴ�����
	US		nCallsAveWaitTime;		// �Ŷ�ƽ���ȴ�ʱ��
	US		nCallsMaxWaitTime;		// �Ŷ���ȴ�ʱ��
	UL		nCallsTotal;			// ������������(�����ۼ�)
	UL		nCallsTransfer;			// ����ת����(�����ۼ�)
	UL		nCallsSucc;				// ����ת�ӳɹ���(�����ۼ�)
	UL		nCallsSuccInTime;		// ���м�ʱת����(�����ۼ�)
	
	/// Agent Data
	US		nAgentSum;				// ��ϯ����
	US		nAgentIdle;				// ����������ϯ��
	US		nAgentBusy;				// ͨ����ϯ��
	US		nAgentNotReady;			// δ������ϯ��
	US		nAgentAfterCall;		// ������ϯ��
	US		nAgentLogout;			// δ��¼��ϯ��

	//----------------------------------------------------------------
	// Sun added 2007-04-15
	US		nEstimatedWaitTime;				// Ԥ�Ƶȴ�ʱ��
	
	US		nReserved0;
	US		nReserved1;
	US		nReserved2;
	US		nReserved3;
	US		nReserved4;
	US		nReserved5;
	UL		nReserved6;
	UL		nReserved7;
	UL		nReserved8;
	UL		nReserved9;
	//----------------------------------------------------------------
};
//------------

//------------
#define TCP_PCS2IVR_GET_AGENTSTATUS_ACK		1022			// (0x03FE) ��ѯ��ϯ״̬��Ӧ��Ϣ
struct S_TCP_PCS2IVR_GET_AGENTSTATUS_ACK
{
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// IVR Channel ID who requests the data
	UC		UserQuery;						// �Ƿ��ѯ�û����μ������س������塱
	UL		AgentID;						// ��ϯID	
	char	strAliasName[50];				// ��¼����
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UC		DNStatus;						// ����״̬������_pcC_stDN
	UL		DNStDur;						// ����״̬����ʱ�����룩
	UC		PositionStatus;					// λ��״̬������_pcC_stPOS
	UL		PosStDur;						// λ��״̬����ʱ�����룩
	UC		LeaveReason;					// ��ϯԭ���Զ���	
};
//------------

// Sun added 2007-04-15
//------------
#define TCP_PCS2IVR_GET_CALL_EWT_ACK		1023			// (0x03FF) ��ѯ�������ض�·�ɵ�EWT��Ӧ��Ϣ
struct S_TCP_PCS2IVR_GET_CALL_EWT_ACK
{
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// IVR Channel ID who requests the data
	UL		CallRefID;						// ���б�ʾ
	US		nRoutePointID;					// ·�ɵ�ID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	US		nQueuePosition;					// �Ŷ�λ��
	US		nEstimatedWaitTime;				// Ԥ�Ƶȴ�ʱ�䣨�룩
	US		nReserved0;
	US		nReserved1;
};
//------------

//------------
#define TCP_PCS2IVR_CALL_PREQUEUE_REQ		1030			// (0x0406) ����Ԥ�Ŷ�������Ӧ��Ϣ
struct S_TCP_PCS2IVR_CALL_PREQUEUE_REQ
{
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// IVR Channel ID who requests the data
	UL		CallRefID;						// ���б�ʾ�����Ϊ0��ϵͳ�Զ�ѡ��
	US		nRoutePointID;					// ·�ɵ�ID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	US		nQueuePosition;					// �Ŷ�λ��
	US		nEstimatedWaitTime;				// Ԥ�Ƶȴ�ʱ�䣨�룩
	US		nReserved0;
	US		nReserved1;
};
//------------
//--------------------------

//------------------------------------------------------------------------------
// Sun added 2012-04-18 [A]
// ¼��������ֹͣ��¼��״̬�ı�֪ͨ
//------------
#define TCP_IVR2PCS_START_REC				131			// (0x0083) ����ʼ¼��
struct S_TCP_IVR2PCS_START_REC
{
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// IVR Channel ID who requests the operation
	UL		AgentID;						// ��ϯID	
	UC		RecordStartType;				// ¼���������Ʒ�ʽ���μ���¼�����Ʒ�ʽ��
	UC		CallDirection;					// ͨ�����򣬲μ����������͡�
	UL		CallRefID;						// ���б�ʾ
	char	ANI[PCS_DEF_NUMLEN_APP];		// ���к���
	char	DNIS[PCS_DEF_NUMLEN_APP];		// ���к���
	char	strAliasName[50];				// ¼����Ӧ����ϯ�û�ID
	char	FilePath[PCS_DEF_DATALEN_APP];	// IVR¼���ļ���
	char	AppData[PCS_DEF_DATALEN_APP];	// App����
	char	AttachData1[16];				// ����������1
	char	AttachData2[16];				// ����������2
	char	AttachData3[32];				// ����������3
	char	AttachData4[32];				// ����������4
	char	AttachData5[256];				// ����������5
};
//------------

//------------
#define TCP_IVR2PCS_STOP_REC				132			// (0x0084) ����ֹͣ¼��
struct S_TCP_IVR2PCS_STOP_REC
{
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// IVR Channel ID who requests the operation
	UL		AgentID;						// ��ϯID	
	UC		RecordStopType;					// ¼���������Ʒ�ʽ���μ���¼�����Ʒ�ʽ��
	UL		CallRefID;						// ���б�ʾ
	char	strAliasName[50];				// ¼����Ӧ����ϯ�û�ID
	char	FilePath[PCS_DEF_DATALEN_APP];	// IVR¼���ļ���
};
//------------

//------------
#define TCP_IVR2PCS_REC_USERDATA			135			// (0x0087) ׷���û����ݵ���ͨ�����һ�γɹ�¼��
struct S_TCP_IVR2PCS_REC_USERDATA
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// ��ϯID	
	char	UserData[200];					// �û�����
};
//------------

//------------
#define TCP_IVR2PCS_REC_RECORDDATA			136			// (0x0088) ¼������������Ϣ
struct S_TCP_IVR2PCS_REC_RECORDDATA
{
	UL		InvokeID;						// Invoke ID
	UL		AgentID;						// ��ϯID	
	char	AttachData1[16];				// ����������1
	char	AttachData2[16];				// ����������2
	char	AttachData3[32];				// ����������3
	char	AttachData4[32];				// ����������4
	char	AttachData5[256];				// ����������5
};
//------------

//------------
// 1135		// (0x046F) ¼����ʼ֪ͨ¼����ʼ֪ͨ
// 1136		// (0x0470) ¼��ֹ֪ͣͨ
/// ͬAgent��Ϣ
//------------
// End of Sun added 2012-04-18 [A]
//------------------------------------------------------------------------------

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_IVRS_H__
//------------------------------------------------------------------------
