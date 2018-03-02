////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		_Protocol_RMR.h
// ��;������	·�ɹ���ͨ��Э��
// ���ߣ�		SUN
// ����ʱ�䣺	1999/12/01
// ����޸�ʱ�䣺2012/06/01
// ��Ȩ��Ϣ��	Copyright (C) 1999 - 2012 PicaSoft, PCS Group
// ��ע��
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_ROUTEMANAGER_H__
#define __PROTOCOL_ROUTEMANAGER_H__
//------------------------------------------------------------------------

//--------------------------
// ·�ɹ�����ע�� - Total 4
//--------------------------

//--------------------------
// ע�� - 2
//------------
#define TCP_RMR2PCS_REGISTER				1			// (0x0001) ע����Ϣ
struct S_TCP_RMR2PCS_REGISTER
{
	UC		Version;						// �汾��
	UC		RegCode[PCS_DEF_NUMLEN_APP];	// ע����
};
//------------

//------------
#define TCP_PCS2RMR_REGISTER_ACK			1001		// (0x03E9) ע����Ӧ��Ϣ
struct S_TCP_PCS2RMR_REGISTER_ACK
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
#define TCP_RMR2PCS_UNREGISTER				2			// (0x0002) ȡ��ע����Ϣ
//------------

//------------
#define TCP_PCS2RMR_UNREGISTER_ACK			1002		// (0x03EA) ȡ��ע����Ӧ��Ϣ
//------------
//--------------------------


//--------------------------
// ·�ɹ������������ - Total 6
//--------------------------

//--------------------------
// ���·�ɵ���� - 2
//------------
#define TCP_RMR2PCS_ADDROUTEPOINT			301			// (0x012D) ���·�ɵ������Ϣ
struct S_TCP_RMR2PCS_ADDROUTEPOINT
{
	US		RoutePointID;					// ·�ɵ���ID
	char	strDeviceID[20];				// ·�ɺ���
	UC		RouteRespondMethod;				// ·�ɵȴ�����ʽ������_pcC_RouteRespondMethod
	char	strRouteRespondDestination[20];	// �ȴ�·��Ŀ��
	US		MaxConCalls;					// ��󲢷�������
};
//------------

//------------
#define TCP_PCS2RMR_ADDROUTEPOINT_ACK		1301		// (0x0515) ���·�ɵ������Ӧ��Ϣ
struct S_TCP_PCS2RMR_ADDROUTEPOINT_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	US		RoutePointID;					// ·�ɵ���ID
};
//------------
//--------------------------

//--------------------------
// ����·�ɵ���� - 2
//------------
#define TCP_RMR2PCS_REMOVEROUTEPOINT		302			// (0x012E) ����·�ɵ������Ϣ
struct S_TCP_RMR2PCS_REMOVEROUTEPOINT
{
	US		RoutePointID;					// ·�ɵ���ID
};
//------------

//------------
#define TCP_PCS2RMR_REMOVEROUTEPOINT_ACK	1302		// (0x0516) ����·�ɵ������Ӧ��Ϣ
struct S_TCP_PCS2RMR_REMOVEROUTEPOINT_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	US		RoutePointID;					// ·�ɵ���ID
};
//------------
//--------------------------

//--------------------------
// ״̬��ѯ - 7
//------------
#define TCP_RMR2PCS_GET_RPTSTATUS_ACK		305			// (0x0131) ��ѯ·�ɵ�״̬��Ӧ��Ϣ
struct S_TCP_RMR2PCS_GET_RPTSTATUS_ACK
{
	UC		SysNo;							// IVRϵͳ����
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// Channel ID who requests the data
	UC		nType;							// �������Center / RoutePoint / Queue / Group / Team����
											/// �μ�_pcC_CallDistributeObject����
	US		nGroupID;						// ����ID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
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
#define TCP_RMR2PCS_STATUS_QUERY			306			// (0x0132) ��ѯ״̬��Ϣ
struct S_TCP_RMR2PCS_STATUS_QUERY
{
	UL		AgentID;						// ��ϯID
};
//------------

//------------
#define TCP_RMR2PCS_ACD_STATUS_REPORT		307			// (0x0133) ·�ɵ�״̬���ݱ�����Ϣ
/// ��Ϣ�ṹͬS_TCP_PCS2ALL_ACD_STATUS_REPORT
//------------

//------------
#define TCP_RMR2PCS_QUERY_CQMEMBER_ACK		308			// (0x0134) ��ѯ���г�Ա��Ӧ��Ϣ��Ҳ������UMS���г�Ա��ѯ
struct S_TCP_RMR2PCS_QUERY_CQMEMBER_ACK
{
	UC		UserType;						// �μ����û�����ָ��Ϣ��ͷ�еķ�����&�����ߡ�
	US		SysNo;							// �û���ϵͳID
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// Channel ID who requests the data
	UL		AgentID;						// CTI AgentID
	char	strUserID[20];					// ����ԱID
	UL		CallRefID;						// ���б�ʾ�����Ϊ0��ϵͳ�Զ�ѡ��
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
	US		nRoutePointID;					// ·�ɵ�ID�����Ϊ0��ϵͳ�Զ�ѡ��
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

//------------
#define TCP_PCS2RMR_GET_RPTSTATUS			1305		// (0x0519) ��ѯ·�ɵ�״̬��Ϣ
struct S_TCP_PCS2RMR_GET_RPTSTATUS
{
	UC		SysNo;							// IVRϵͳ����
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// Channel ID who requests the data
	UC		nType;							// �������Center / RoutePoint / Queue / Group / Team����
											/// �μ�_pcC_CallDistributeObject����
	US		nGroupID;						// ����ID
};
//------------

//------------
#define TCP_PCS2RMR_STATUS_QUERY_ACK		1306		// (0x051A) ��ѯ״̬��Ӧ��Ϣ
struct S_TCP_PCS2RMR_STATUS_QUERY_ACK
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

//------------
#define TCP_PCS2RMR_QUERY_CQMember			1308		// (0x051C) ��ѯ���г�Ա��Ϣ��Ҳ������UMS���г�Ա��ѯ
struct S_TCP_PCS2RMR_QUERY_CQMember
{
	UC		UserType;						// �μ����û�����ָ��Ϣ��ͷ�еķ�����&�����ߡ�
	US		SysNo;							// �û���ϵͳID
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// Channel ID who requests the data
	UL		AgentID;						// CTI AgentID
	char	strUserID[20];					// ����ԱID
	UL		CallRefID;						// ���б�ʾ�����Ϊ0��ϵͳ�Զ�ѡ��
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
};
//------------
//--------------------------


//--------------------------
// ·�ɿ��� - Total 5
//--------------------------
#define TCP_RMR2PCS_ROUTE_QUERY_ACK			311			// (0x0137) ·��������Ӧ��Ϣ
struct S_TCP_RMR2PCS_ROUTE_QUERY_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	US		RoutePointID;					// ·�ɵ���ID
	UL		CallRefID;						// ���б�ʾ
	char	InRMTime[15];					// �����״ν���RMR��ʱ��
};
//------------

//------------
#define TCP_RMR2PCS_ROUTE_RESPOND			312			// (0x0138) ·����Ӧ��Ϣ
struct S_TCP_RMR2PCS_ROUTE_RESPOND
{
	US		RoutePointID;					// ·�ɵ���ID
	UL		CallRefID;						// ���б�ʾ
	US		WaitTime;						// �Ŷӵȴ�ʱ�����룩
	US		GroupID;						// ��ϯ������
	UL		AgentID;						// �绰·������ϯ��0��ʾ��ʱ
	UC		TimeoutMethod;					// ��ʱ����ʽ������_pcC_RouteRespondMethod
	char	strTimeoutDestination[20];		// ��ʱ·��Ŀ��
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
	UL		OldCallRefID;					// �ɺ��б�ʾ
};
//------------

//------------
#define TCP_PCS2RMR_ROUTE_QUERY				1311		// (0x051F) ·��������Ϣ
struct S_TCP_PCS2RMR_ROUTE_QUERY
{
	US		RoutePointID;					// ·�ɵ���ID
	UL		CallRefID;						// ���б�ʾ
	char	InRMTime[15];					// �����״ν���RMR��ʱ��
	char	strANI[PCS_DEF_NUMLEN_APP];		// ����
	char	strDNIS[PCS_DEF_NUMLEN_APP];	// ����
	UL		ANITrunk;						// �����м�
	UL		DNISTrunk;						// �����м�
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
	UL		OldCallRefID;					// �ɺ��б�ʾ
};
//------------

//------------
#define TCP_PCS2RMR_ROUTE_QUERY_END			1312		// (0x0520) ����·��������Ϣ
struct S_TCP_PCS2RMR_ROUTE_QUERY_END
{
	US		RoutePointID;					// ·�ɵ���ID
	UL		CallRefID;						// ���б�ʾ
	UC		ResultCode;						// ���س�������
};
//------------

//------------
#define TCP_PCS2RMR_ROUTE_RESPOND_ACK		1313		// (0x0521) �ظ�·��������Ϣ
struct S_TCP_PCS2RMR_ROUTE_RESPOND_ACK
{
	US		RoutePointID;					// ·�ɵ���ID
	UL		CallRefID;						// ���б�ʾ
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------


//--------------------------
// ·�ɹ����Ⲧ - Total 2
//--------------------------

//--------------------------
// Ԥ���Ⲧ - 4
//------------
#define TCP_RMR2PCS_PREDICTIVE_DISTRIBUTE_ACK	332				// (0x014C) Ԥ���Ⲧ����·����Ӧ��Ϣ
struct S_TCP_RMR2PCS_PREDICTIVE_DISTRIBUTE_ACK
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
#define TCP_PCS2RMR_PREDICTIVE_DISTRIBUTE		1332			// (0x0534) Ԥ���Ⲧ����·��������Ϣ
struct S_TCP_PCS2RMR_PREDICTIVE_DISTRIBUTE
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
#define TCP_PCS2RMR_PREDICTIVE_AGENT_SEIZED		1333			// (0x0535) Ԥ���Ⲧ��ϯռ����Ϣ
struct S_TCP_PCS2RMR_PREDICTIVE_AGENT_SEIZED
{
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ����ID
	UL		AgentID;						// ռ����ϯID
};
//------------

//------------
#define TCP_PCS2RMR_PREDICTIVE_AGENT_UNSEIZED	1334			// (0x0535) Ԥ���Ⲧ��ϯȡ��ռ����Ϣ
struct S_TCP_PCS2RMR_PREDICTIVE_AGENT_UNSEIZED
{
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ����ID
	UL		AgentID;						// ռ����ϯID
};
//------------
//--------------------------


//--------------------------
// Ԥ���Ⲧ - 4
//------------
#define TCP_RMR2PCS_OBTASK_DISTRIBUTE_ACK	331				// (0x014B) �Ⲧ����·����Ӧ��Ϣ
struct S_TCP_RMR2PCS_OBTASK_DISTRIBUTE_ACK
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
#define TCP_PCS2RMR_OBTASK_DISTRIBUTE		1331			// (0x0533) �Ⲧ����·��������Ϣ
struct S_TCP_PCS2RMR_OBTASK_DISTRIBUTE
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
//--------------------------

//--------------------------
// Զ�̿��� - 4
//------------
//------------
#define TCP_RMR2PCS_RELOAD_ACD_SCENARIO_ACK	351			// (0x015F) ���¼����Ŷӹ�����Ӧ��Ϣ
struct S_TCP_RMR2PCS_RELOAD_ACD_SCENARIO_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_RMR2PCS_UPDATE_USER_ACK			352			// (0x0160) �޸��û���Ӧ��Ϣ
struct S_TCP_RMR2PCS_UPDATE_USER_ACK
{
	char	strAliasName[50];				// ��¼����
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2RMR_RELOAD_ACD_SCENARIO		1351		// (0x0547) ���¼����Ŷӹ�����Ϣ
//------------

//------------
#define TCP_PCS2RMR_UPDATE_USER				1352		// (0x0548) �޸��û���Ϣ
struct S_TCP_PCS2RMR_UPDATE_USER
{
	char	strAliasName[50];				// ��¼����
};
//------------
//--------------------------


//--------------------------
// UMS���� - Total 14
//--------------------------

//--------------------------
// ͳһ�Ŷ����� - 2
//------------
#define TCP_RMR2PCS_UMS_ADDTASK_ACK			3101		// (0x0C1D) ͳһ�����Ӧ��Ϣ
struct S_TCP_RMR2PCS_UMS_ADDTASK_ACK
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
	UC		ErrorCode;								// �����룬�μ��������붨�塱��
													// PCS_DEF_RETCODE_SUCC
													// PCS_DEF_RETCODE_NOTNEED
													// PCS_DEF_RETCODE_OTHERS
													// PCS_DEF_RETCODE_SYSERROR
	US		RoutePointID;							// ����·�ɵ�ID
	US		nRouteData1;							// ·������1�����г���
	US		nRouteData2;							// ·������2����¼��ϯ��
	US		nRouteData3;							// ·������3��������ϯ��
	US		nRouteData4;							// ·������4��Ԥ�Ƶȴ�ʱ��
	US		nRouteData5;							// ·������5����ʱ����
	US		nRouteData6;							// ·������5����ʱ���� 
	US		nRouteData7;							// ·������6����ʱ����
	US		nRouteData8;							// ·������7����ʱ����
	US		nRouteData9;							// ·������8����ʱ����
};
//------------

//------------
#define TCP_PCS2RMR_UMS_ADDTASK_REQ			3601		// (0x0E11) ͳһ�Ŷ�������Ϣ
struct S_TCP_PCS2RMR_UMS_ADDTASK_REQ
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
	char	strHotLink[100];						// ����ļ�·�������ӵ�ַ
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------
//--------------------------

//--------------------------
// ͳһ������� - 2
//------------
#define TCP_RMR2PCS_UMS_REMOVETASK_ACK		3102		// (0x0C1E) ͳһ�����Ӧ��Ϣ
struct S_TCP_RMR2PCS_UMS_REMOVETASK_ACK
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
	US		RoutePointID;							// ·�ɵ�ID
	UC		ErrorCode;								// �����룬�μ��������붨�塱��
													// PCS_DEF_RETCODE_SUCC
													// PCS_DEF_RETCODE_NOTEXIST
													// PCS_DEF_RETCODE_STATUS
													// PCS_DEF_RETCODE_NOTNEED
													// PCS_DEF_RETCODE_OTHERS
													// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2RMR_UMS_REMOVETASK_REQ		3602		// (0x0E12) ͳһ�����Ϣ
struct S_TCP_PCS2RMR_UMS_REMOVETASK_REQ
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
	US		RoutePointID;							// ·�ɵ�ID
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
//--------------------------

//--------------------------
// ͳһ������� - 2
//------------
#define TCP_RMR2PCS_UMS_DISPATCHTASK_REQ	3106		// (0x0C22) ͳһ����������Ϣ
struct S_TCP_RMR2PCS_UMS_DISPATCHTASK_REQ
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
#define TCP_PCS2RMR_UMS_DISPATCHTASK_ACK	3606		// (0x0E16) ͳһ����������Ӧ��Ϣ
struct S_TCP_PCS2RMR_UMS_DISPATCHTASK_ACK
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
	US		RoutePointID;							// ·�ɵ�ID
	UC		ErrorCode;								// �����룬�μ��������붨�塱��
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
//--------------------------

//--------------------------
// ͳһ������ - 2
//------------
#define TCP_RMR2PCS_UMS_ACCEPTTASK_ACK		3107		// (0x0C23) ͳһ���䴦����Ӧ��Ϣ
struct S_TCP_RMR2PCS_UMS_ACCEPTTASK_ACK
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
	US		RoutePointID;							// ·�ɵ�ID
	char	strUserID[20];							// ����ԱID
	UC		ErrorCode;								// �����룬�μ������س������塱
};
//------------

//------------
#define TCP_PCS2RMR_UMS_ACCEPTTASK_REQ		3607		// (0x0E17) ͳһ���䴦����Ϣ
struct S_TCP_PCS2RMR_UMS_ACCEPTASK_REQ
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
	US		RoutePointID;							// ·�ɵ�ID
	char	strUserID[20];							// ����ԱID
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
//--------------------------

//--------------------------
// ͳһ����ȡ�� - 2
//------------
#define TCP_RMR2PCS_UMS_CANCELTASK_ACK		3108		// (0x0C24) ͳһ����ȡ����Ӧ��Ϣ
struct S_TCP_RMR2PCS_UMS_CANCELTASK_ACK
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
	US		RoutePointID;							// ·�ɵ�ID
	char	strUserID[20];							// ����ԱID
};
//------------

//------------
#define TCP_PCS2RMR_UMS_CANCELTASK_REQ		3608		// (0x0E18) ͳһ����ȡ����Ϣ
struct S_TCP_PCS2RMR_UMS_CANCELTASK_REQ
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
	char	strAppData[PCS_DEF_DATALEN_APP];		// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];		// User Data
};
//------------
//--------------------------

//--------------------------
// ͳһ������� - 2
//------------
#define TCP_RMR2PCS_UMS_FINISHTASK_ACK		3109		// (0x0C25) ͳһ���������Ӧ��Ϣ
struct S_TCP_RMR2PCS_UMS_FINISHTASK_ACK
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
	US		RoutePointID;							// ·�ɵ�ID
	char	strUserID[20];							// ����ԱID
};
//------------

//------------
#define TCP_PCS2RMR_UMS_FINISHTASK_REQ		3609		// (0x0E19) ͳһ���������Ϣ
struct S_TCP_PCS2RMR_UMS_FINISHTASK_REQ
{
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
	US		RoutePointID;							// ·�ɵ�ID
	char	strUserID[20];							// ����Ա
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
//--------------------------

//--------------------------
// ��������ȡ�� - 2
//------------
#define TCP_RMR2PCS_UMS_AGTTASK_CANCEL_ACK	3110		// (0x0C26) ��ϯȫ������ȡ����Ӧ��Ϣ
struct S_TCP_RMR2PCS_UMS_AGTTASK_CANCEL_ACK
{
	char	strUserID[20];							// ����ԱID
	UL		TaskCount;								// ��������
};
//------------

//------------
#define TCP_PCS2RMR_UMS_AGTTASK_CANCEL_REQ	3610		// (0x0E1A) ��ϯȫ������ȡ����Ϣ
struct S_TCP_PCS2RMR_UMS_AGTTASK_CANCEL_REQ
{
	char	strUserID[20];							// ����ԱID
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
//--------------------------

//--------------------------
//------------
// Simon added 2010-05-04
#define TCP_PCS2RMR_UPDATE_REALTIMEPARAM_REQ 3621		// (0x0E25) ʵʱ���ݲ������� ����
struct S_TCP_PCS2RMR_UPDATE_REALTIMEPARAM_REQ
{
	UC		bReportRealtimeData;					// �Ƿ񱨸�ʵʱ����
	UC		bRecordRTD;								// �Ƿ��¼ʵʱ����
	UC		Reserved[16];							// ����
};

#define TCP_RMR2PCS_UPDATE_REALTIMEPARAM_ACK 3121		// (0x0C31) ʵʱ���ݲ������� ��Ӧ
struct S_TCP_RMR2PCS_UPDATE_REALTIMEPARAM_ACK
{
	UC		ErrorCode;								// �������
};

#define TCP_RMR2PCS_INSERVICE_EVT			3122		// (0x0C32) RM �������״̬
#define TCP_RMR2RMR_OUTOFSERVICE_EVT		3123		// (0x0C33) RM �뿪����״̬(�ݲ�ʹ��)
//------------
//--------------------------

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_ROUTEMANAGER_H__
//------------------------------------------------------------------------
