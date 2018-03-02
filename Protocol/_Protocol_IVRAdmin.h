////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		_Protocol_IVRAdmin.h
// ��;������	IVRͨ�Ź���ԱЭ��
// ���ߣ�		SUN
// ����ʱ�䣺	2006/03/05
// ����޸�ʱ�䣺2006/03/05
// ��Ȩ��Ϣ��	Copyright (C) 2003 - 2006 PicaSoft
// ��ע��
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_IVRSADMIN_H__
#define __PROTOCOL_IVRSADMIN_H__
//------------------------------------------------------------------------

//--------------------------
// IVR Adminע�� - Total 4
//--------------------------

//--------------------------
// ע�� - 2
//------------
#define TCP_ADM2IVR_REGISTER				1			// (0x0001) ע����Ϣ
struct S_TCP_ADM2IVR_REGISTER
{
	char	strUserID[50];					// �û���
	char	strPassword[20];				// ����
	UC		Version;						// �汾��
	UC		RegCode[PCS_DEF_NUMLEN_APP];	// ע����
};
//------------

//------------
#define TCP_IVR2ADM_REGISTER_ACK			1001		// (0x03E9) ע����Ӧ��Ϣ
struct S_TCP_IVR2ADM_REGISTER_ACK
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
	UC		SysNo;							// ϵͳ����
	UC		GroupCount;						// ������
	US		ChannelCount;					// ͨ������
	UC		Version;						// �汾��
};
//------------
//--------------------------

//--------------------------
// ȡ��ע�� - 2
//------------
#define TCP_ADM2IVR_UNREGISTER				2			// (0x0002) ȡ��ע����Ϣ
//------------

//------------
#define TCP_IVR2ADM_UNREGISTER_ACK			1002		// (0x03EA) ȡ��ע����Ӧ��Ϣ
//------------
//--------------------------
//--------------------------


//--------------------------
// IVR �����Ϣ - Total 22
//--------------------------

//--------------------------
// ����������˳���Ϣ - 18
//------------
#define TCP_IVR2ADM_GROUP_MON_ACK				501			// (0x01F5) IVR����������Ӧ��Ϣ
struct S_TCP_IVR2ADM_GROUP_MON_ACK
{
	UC		GroupID;						// IVR Group ID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_IVR2ADM_GROUP_UNMON_ACK				502			// (0x01F6) IVR���ؽ�����Ӧ��Ϣ
struct S_TCP_IVR2ADM_GROUP_UNMON_ACK
{
	UC		GroupID;						// IVR Group ID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_IVR2ADM_GROUP_PROP_NAME				503			// (0x01F7) IVR�����Ա�����Ϣ
struct S_TCP_IVR2ADM_GROUP_PROP_NAME
{
	UC		GroupID;						// IVR Group ID
	UC		CTIEnabled;						// �Ƿ�ʹ��CTI
	US		StartChannel;					// ��ʼͨ��
	US		EndChannel;						// ��ֹͨ��
	UC		CallFlowID;						// Call Flow ID
	US		ResourceID;						// Resource ID
	UL		DailyCallCount;					// �����ۼƺ�������
	UL		YTDCallCount;					// ȫ�������ۼ�����
	char	GroupName[50];					// IVR Group Name
	char	CallFlowName[50];				// Call Flow Name
	char	ResourceName[50];				// Resource Name
};
//------------

//------------
#define TCP_IVR2ADM_GROUP_PROP_CALLFLOW			504			// (0x01F8) IVR�����̸ı���Ϣ
struct S_TCP_IVR2ADM_GROUP_PROP_CALLFLOW
{
	UC		GroupID;						// IVR Group ID
	UC		CallFlowID;						// Call Flow ID
	char	CallFlowName[50];				// Call Flow Name
};
//------------

//------------
#define TCP_IVR2ADM_GROUP_PROP_RESOURCE			505			// (0x01F9) IVR����Դ�ı���Ϣ
struct S_TCP_IVR2ADM_GROUP_PROP_RESOURCE
{
	UC		GroupID;						// IVR Group ID
	US		ResourceID;						// Resource ID
	char	ResourceName[50];				// Resource Name
};
//------------

//------------
#define TCP_IVR2ADM_GROUP_TIME_CALLFLOW_ACK		506			// (0x01FA) IVR�鶨ʱ����������Ӧ��Ϣ
struct S_TCP_IVR2ADM_GROUP_TIME_CALLFLOW_ACK
{
	UC		GroupID;						// IVR Group ID
	UC		CallFlowID;						// Call Flow ID
	char	LoadTime[15];					// ��ʱ����ʱ�䣬YYYYMMDDHHMMSS
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_IVR2ADM_GROUP_TIME_RESOURCE_ACK		507			// (0x01FB) IVR�鶨ʱ������Դ��Ӧ��Ϣ
struct S_TCP_IVR2ADM_GROUP_TIME_RESOURCE_ACK
{
	UC		GroupID;						// IVR Group ID
	UC		ResourceID;						// Call Flow ID
	char	LoadTime[15];					// ��ʱ����ʱ�䣬YYYYMMDDHHMMSS
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_IVR2ADM_CH_ST_REPORT				508			// (0x01FC) IVRͨ��״̬������Ϣ
struct S_TCP_IVR2ADM_CH_ST_REPORT
{
	UC		GroupID;						// IVR Group ID
	US		ChannelID;						// IVR Channel ID
	US		NodeID;							// Transfer to Node ID
	UL		DailyCallCount;					// �����ۼƺ�������
	UL		YTDCallCount;					// ȫ�������ۼ�����
};
//------------

//------------
#define TCP_IVR2ADM_CH_MON_ACK					510			// (0x01FE) IVRͨ�����������Ӧ��Ϣ
struct S_TCP_IVR2ADM_CH_MON_ACK
{
	UC		GroupID;						// IVR Group ID
	US		ChannelID;						// IVR Channel ID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_IVR2ADM_CH_UNMON_ACK				511			// (0x01FF) IVRͨ����ؽ�����Ӧ��Ϣ
struct S_TCP_IVR2ADM_CH_UNMON_ACK
{
	UC		GroupID;						// IVR Group ID
	US		ChannelID;						// IVR Channel ID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_ADM2IVR_GROUP_MON					1501		// (0x05DD) IVR����������Ϣ
struct S_TCP_ADM2IVR_GROUP_MON
{
	UC		GroupID;						// IVR Group ID
};
//------------

//------------
#define TCP_ADM2IVR_GROUP_UNMON					1502		// (0x05DE) IVR���ؽ�����Ϣ
struct S_TCP_ADM2IVR_GROUP_UNMON
{
	UC		GroupID;						// IVR Group ID
};
//------------

//------------
#define TCP_ADM2IVR_GROUP_LOAD_CALLFLOW			1504		// (0x05E0) IVR�����������Ϣ
struct S_TCP_ADM2IVR_GROUP_LOAD_CALLFLOW
{
	UC		GroupID;						// IVR Group ID
	UC		CallFlowID;						// Call Flow ID
};
//------------

//------------
#define TCP_ADM2IVR_GROUP_LOAD_RESOURCE			1505		// (0x05E1) IVR�������Դ��Ϣ
struct S_TCP_ADM2IVR_GROUP_LOAD_RESOURCE
{
	UC		GroupID;						// IVR Group ID
	UC		ResourceID;						// Call Flow ID
};
//------------

//------------
#define TCP_ADM2IVR_GROUP_TIME_CALLFLOW			1506		// (0x05E2) IVR�鶨ʱ����������Ϣ
struct S_TCP_ADM2IVR_GROUP_TIME_CALLFLOW
{
	UC		GroupID;						// IVR Group ID
	UC		CallFlowID;						// Call Flow ID
	char	LoadTime[15];					// ��ʱ����ʱ�䣬YYYYMMDDHHMMSS
};
//------------

//------------
#define TCP_ADM2IVR_GROUP_TIME_RESOURCE			1507		// (0x05E3) IVR�鶨ʱ������Դ��Ϣ
struct S_TCP_ADM2IVR_GROUP_TIME_RESOURCE
{
	UC		GroupID;						// IVR Group ID
	UC		ResourceID;						// Call Flow ID
	char	LoadTime[15];					// ��ʱ����ʱ�䣬YYYYMMDDHHMMSS
};
//------------

//------------
#define TCP_ADM2IVR_CH_MON						1510		// (0x05E6) IVRͨ�����������Ϣ
struct S_TCP_ADM2IVR_CH_MON
{
	UC		GroupID;						// IVR Group ID
	US		ChannelID;						// IVR Channel ID
};
//------------

//------------
#define TCP_ADM2IVR_CH_UNMON					1511		// (0x05E7) IVRͨ����ؽ�����Ϣ
struct S_TCP_ADM2IVR_CH_UNMON
{
	UC		GroupID;						// IVR Group ID
	US		ChannelID;						// IVR Channel ID
};
//------------
//--------------------------

//--------------------------
// ͨ��״̬��Ϣ - 2
//------------
#define TCP_IVR2ADM_CH_ST_START					521			// (0x0209) IVRͨ���������״̬��Ϣ
struct S_TCP_IVR2ADM_CH_ST_START
{
	UC		GroupID;						// IVR Group ID
	US		ChannelID;						// IVR Channel ID
	char	strANI[PCS_DEF_NUMLEN_APP];		// ���к���
};

//------------
#define TCP_IVR2ADM_CH_ST_STOP					522			// (0x0209) IVRͨ����������״̬��Ϣ
struct S_TCP_IVR2ADM_CH_ST_STOP
{
	UC		GroupID;						// IVR Group ID
	US		ChannelID;						// IVR Channel ID
	US		CallLength;						// ͨ��ʱ�����룩
};
//------------

//--------------------------

//--------------------------
// ������ת��������Ϣ - 2
//------------
#define TCP_IVR2ADM_NODE_TRANSFER				531			// (0x0213) IVRͨ��������ת��Ϣ
struct S_TCP_IVR2ADM_NODE_TRANSFER
{
	UC		GroupID;						// IVR Group ID
	US		ChannelID;						// IVR Channel ID
	US		NodeID;							// Transfer to Node ID
	char	KeyPress[PCS_DEF_DATALEN_APP];	// Key Pressed in previous node
};
//------------

//------------
#define TCP_ADM2IVR_NODE_TRANSFER				1531		// (0x05FB) IVRͨ��������ת������Ϣ
struct S_TCP_ADM2IVR_NODE_TRANSFER
{
	UC		GroupID;						// IVR Group ID
	US		ChannelID;						// IVR Channel ID
	US		NodeID;							// Transfer to Node ID
};
//------------
//--------------------------

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_IVRADMIN_H__
//------------------------------------------------------------------------
