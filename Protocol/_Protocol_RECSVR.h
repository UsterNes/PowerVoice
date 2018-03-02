////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		_Protocol_RECSVR.h
// ��;������	¼������ͨ��Э��
// ���ߣ�		SUN
// ����ʱ�䣺	1999/12/01
// ����޸�ʱ�䣺2003/12/15
// ��Ȩ��Ϣ��	Copyright (C) 1999 - 2003 PCSG
// ��ע��
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_RECORDSERVER_H__
#define __PROTOCOL_RECORDSERVER_H__
//------------------------------------------------------------------------

//--------------------------
// ¼������ע�� - Total 4
//--------------------------

//--------------------------
// ע�� - 2
//------------
#define TCP_REC2PCS_REGISTER				1			// (0x0001) ע����Ϣ
struct S_TCP_REC2PCS_REGISTER
{
	UC		SysNo;							// ϵͳ����
	UC		Version;						// �汾��
	UC		RegCode[PCS_DEF_NUMLEN_APP];	// ע����
	US		nMaxiSensePort;					// ¼����������������˿�
	char	strMaxiSenseIP[20];				// ¼�����������IP��ַ
};
//------------

//------------
#define TCP_PCS2REC_REGISTER_ACK			1001		// (0x03E9) ע����Ӧ��Ϣ
struct S_TCP_PCS2REC_REGISTER_ACK
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
#define TCP_REC2PCS_UNREGISTER				2			// (0x0002) ȡ��ע����Ϣ
//------------

//------------
#define TCP_PCS2REC_UNREGISTER_ACK			1002		// (0x03EA) ȡ��ע����Ӧ��Ϣ
//------------
//--------------------------

//--------------------------
// ¼��������� - Total 2
//--------------------------
//------------
#define TCP_REC2PCS_RELOAD_USERLIST_ACK		701			// (0x02BD) ���¼����û��б���Ӧ��Ϣ
struct S_TCP_REC2PCS_RELOAD_USERLIST_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2REC_RELOAD_USERLIST			1701		// (0x06A5) ���¼����û��б���Ϣ
//------------
//--------------------------


//--------------------------
// ״̬��ѯ - 4
//------------
#define TCP_REC2PCS_STATUS_QUERY			711			// (0x02C7) ��ѯ״̬��Ϣ
struct S_TCP_REC2PCS_STATUS_QUERY
{
	UL		AgentID;						// ��ϯID
};
//------------

//------------
#define TCP_REC2PCS_USER_STATUS_QUERY		712			// (0x02C8) ��ѯָ������Ա״̬��Ϣ
struct S_TCP_REC2PCS_USER_STATUS_QUERY
{
	char	strAliasName[50];				// ��¼����
};
//------------

//------------
#define TCP_PCS2REC_STATUS_QUERY_ACK		1711		// (0x06AF) ��ѯ״̬��Ӧ��Ϣ
struct S_TCP_PCS2REC_STATUS_QUERY_ACK
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
#define TCP_PCS2REC_USER_STATUS_QUERY_ACK	1712		// (0x06B0) ��ѯָ������Ա״̬��Ӧ��Ϣ
struct S_TCP_PCS2REC_USER_STATUS_QUERY_ACK
{
	char	strAliasName[50];				// ��¼����
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
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
//--------------------------

//
// ¼���������� added by stephen wang 2006-12-08
//
struct S_RECORD_INDEX_NODE					// Total 195 Bytes
{
	UC			ChannelIndex;				// ͨ����
	char		StartRecordTime[16];		// ¼����ʼʱ��
	UC			CallDirection;				// ͨ������
	UL			RecLength;					// ¼��ʱ��
	char		TelNum[25];					// �绰����
	char		KeyPress[20];				// �û�����
	char		FilePath[128];				// ¼���ļ�����·��

};


//------------------------------------------------------------------------------
// Sun added 2010-05-12
// ¼��״̬֪ͨ
//------------
#define TCP_REC2PCS_RECORD_STARTED			721			// (0x02D1) ¼����ʼ֪ͨ
struct S_TCP_REC2PCS_RECORD_STARTED
{
	UL		AgentID;						// ��ϯID
	char	RecordTime[15];					// ¼����ʼʱ�䣬PK
	char	UserName[20];					// �û�����PK
	UC		RecordStartType;				// ¼���������Ʒ�ʽ���μ���¼�����Ʒ�ʽ��
	char	FilePath[128];					// ¼���ļ�ȫ·��
};
//------------

//------------
#define TCP_REC2PCS_RECORD_STOPED			722			// (0x02D2) ¼��ֹ֪ͣͨ
struct S_TCP_REC2PCS_RECORD_STOPED
{
	UL		AgentID;						// ��ϯID
	char	RecordTime[15];					// ¼����ʼʱ�䣬PK
	char	UserName[20];					// �û�����PK
	UC		RecordEndType;					// ¼��ֹͣ���Ʒ�ʽ���μ���¼�����Ʒ�ʽ��
	UL		RecordDuration;					// ¼��ʱ��
	char	FilePath[128];					// ¼���ļ�ȫ·��
};
//------------

//------------
#define TCP_PCS2REC_REC_USERDATA			1721			// (0x06B9) ׷���û����ݵ����һ�γɹ�¼��
struct S_TCP_PCS2REC_REC_USERDATA
{
	UL		AgentID;						// ��ϯID
	char	UserData[200];					// �û�����
};
//------------

//------------
#define TCP_PCS2REC_REC_RECORDDATA			1722			// (0x0088) ¼������������Ϣ
struct S_TCP_PCS2REC_REC_RECORDDATA
{
	UL		AgentID;						// ��ϯID
	char	AttachData1[16];				// ����������1
	char	AttachData2[16];				// ����������2
	char	AttachData3[32];				// ����������3
	char	AttachData4[32];				// ����������4
	char	AttachData5[256];				// ����������5
};
//------------
// End of Sun added 2010-05-12
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Sun added 2012-04-18
// ¼��������ֹͣ��¼��״̬�ı�֪ͨ
//------------
#define TCP_PCS2REC_IVRSTART_REC			1723		// (0x0089) IVR����ʼ¼��
struct S_TCP_PCS2REC_IVRSTART_REC
{
	UC		SysNo;							// IVRϵͳ����
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
#define TCP_PCS2REC_IVRSTOP_REC				1724			// (0x008A) IVR����ֹͣ¼��
struct S_TCP_PCS2REC_IVRSTOP_REC
{
	UC		SysNo;							// IVRϵͳ����
	UL		InvokeID;						// Invoke ID
	US		ChannelID;						// IVR Channel ID who requests the operation
	UL		AgentID;						// ��ϯID	
	UC		RecordStopType;					// ¼���������Ʒ�ʽ���μ���¼�����Ʒ�ʽ��
	UL		CallRefID;						// ���б�ʾ
	char	strAliasName[50];				// ¼����Ӧ����ϯ�û�ID
	char	FilePath[PCS_DEF_DATALEN_APP];	// IVR¼���ļ���
};
//------------
// End of Sun added 2012-04-18
//------------------------------------------------------------------------------
//--------------------------

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_RECORDSERVER_H__
//------------------------------------------------------------------------
