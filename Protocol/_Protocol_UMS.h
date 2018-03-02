////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		_Protocol_UMS.h
// ��;������	ͳһ��Ϣͨ��Э��
// ���ߣ�		SUN
// ����ʱ�䣺	2006/05/10
// ����޸�ʱ�䣺2006/05/12
// ��Ȩ��Ϣ��	Copyright (C) 2005 - 2006 PicaSoft
// ��ע��
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_UNIFYMESSAGESYSTEM_H__
#define __PROTOCOL_UNIFYMESSAGESYSTEM_H__
//------------------------------------------------------------------------

//--------------------------
// UMS����ģ��ע�� - Total 4
//--------------------------

//--------------------------
// ע�� - 1
//------------
#define TCP_UMS2PCS_REGISTER				1			// (0x0001) ע����Ϣ
struct S_TCP_UMS2PCS_REGISTER
{
	UC		UMSType;						// UMS�������ͣ��μ�_pcC_UMSTaskTypes
	UC		UMSIndex;						// UMS��ϵͳ���
	UC		Version;						// �汾��
	UL		MaxConTask;						// ��󲢷�������
	UC		RegCode[PCS_DEF_NUMLEN_APP];	// ע����
};
//------------

//------------
#define TCP_PCS2UMS_REGISTER_ACK			1001		// (0x03E9) ע����Ӧ��Ϣ
struct S_TCP_PCS2UMS_REGISTER_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
												// PCS_DEF_RETCODE_SUCC
												// PCS_DEF_RETCODE_NOTEXIST
												// PCS_DEF_RETCODE_BEUSED
												// PCS_DEF_RETCODE_VERSION
												// PCS_DEF_RETCODE_REGCODE
												// PCS_DEF_RETCODE_OTHERS
												// PCS_DEF_RETCODE_SYSERROR
	UC		SiteID;							// ����ID
	UC		RMOnlineStatus;					// RouteManager����״̬
};
//------------
//--------------------------

//--------------------------
// ȡ��ע�� - 2
//------------
#define TCP_UMS2PCS_UNREGISTER				2			// (0x0002) ȡ��ע����Ϣ
//------------

//------------
#define TCP_PCS2UMS_UNREGISTER_ACK			1002		// (0x03EA) ȡ��ע����Ӧ��Ϣ
//------------
//--------------------------


//--------------------------
// UMS���� - Total 12
//--------------------------

//--------------------------
// �Ŷ����� - 2
//------------
#define TCP_UMS2PCS_ADDTASK_REQ				3001		// (0x0BB9) �Ŷ�������Ϣ
struct S_TCP_UMS2PCS_ADDTASK_REQ
{
	UL		InvokeID;									// Invoke ID
	UL		TaskID;										// Task ID
	UC		CustID;										// �⻧����
	char	strCallingParty[PCS_DEF_NUMLEN_APP];		// ������
	char	strCalledParty[PCS_DEF_NUMLEN_APP];			// ������
	US		RoutePointID;								// Ĭ��·�ɵ�ID
	UL		ClassifyCode;								// �������
	UL		WaitTime;									// �ѵȴ�ʱ�䣨�룩
	char	ServiceCode0;								// ���������
	char	ServiceCode1;								// �����������
	char	ServiceCode2;								// �����������
	char	strTaskTitle[50];							// �������
	char	strTaskContent[200];						// ��������
	char	strHotLink[100];							// ����ļ�·�������ӵ�ַ
	char	strAppData[PCS_DEF_DATALEN_APP];			// Application Data
	char	strUserData[PCS_DEF_DATALEN_USER];			// User Data
};
//------------

//------------
#define TCP_PCS2UMS_ADDTASK_ACK				3501		// (0x0DAD) �����Ӧ��Ϣ
struct S_TCP_PCS2UMS_ADDTASK_ACK
{
	UL		InvokeID;								// Invoke ID
	UL		TaskID;									// Task ID
	UC		ErrorCode;								// �����룬�μ��������붨�塱��
													// PCS_DEF_RETCODE_SUCC
													// PCS_DEF_RETCODE_NOTEXIST
													// PCS_DEF_RETCODE_STATUS
													// PCS_DEF_RETCODE_NOTNEED
													// PCS_DEF_RETCODE_REJECTED
													// PCS_DEF_RETCODE_OTHERS
													// PCS_DEF_RETCODE_SYSERROR
	US		RoutePointID;							// ����·�ɵ�ID
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
	char	strAcceptTime[15];						// ��������ʱ�䣬"YYYYMMDDHHMMSS\0"
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
//--------------------------

//--------------------------
// ������� - 2
//------------
#define TCP_UMS2PCS_REMOVETASK_REQ			3002		// (0x0BBA) �����Ϣ
struct S_TCP_UMS2PCS_REMOVETASK_REQ
{
	UL		InvokeID;								// Invoke ID
	UL		TaskID;									// Task ID
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

//------------
#define TCP_PCS2UMS_REMOVETASK_ACK			3502		// (0x0DAE) �����Ӧ��Ϣ
struct S_TCP_PCS2UMS_REMOVETASK_ACK
{
	UL		InvokeID;								// Invoke ID
	UL		TaskID;									// Task ID
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

//--------------------------
// �����ȴ���Ϣ - 1
//------------
#define TCP_UMS2PCS_CONTINUETASK_REQ		3003		// (0x0BBB) �����Ŷ���Ϣ
struct S_TCP_UMS2PCS_CONTINUETASK_REQ
{
	UL		InvokeID;							// Invoke ID
	UL		TaskID;								// Task ID
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];	// ͳһ��Ϣ����ID
	US		RoutePointID;						// ·�ɵ�ID
	UL		ReasonCode;							// ����ԭ�����
};

//--------------------------
// ������ͣ - 2
//------------
#define TCP_UMS2PCS_PAUSETASK_REQ			3004		// (0x0BBC) ������ͣ��Ϣ
struct S_TCP_UMS2PCS_PAUSETASK_REQ
{
	UL		InvokeID;								// Invoke ID
	UL		TaskID;									// Task ID
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
	US		RoutePointID;							// ·�ɵ�ID
	UL		ReasonCode;								// ���ԭ�����
};
//------------

//------------
#define TCP_PCS2UMS_PAUSETASK_ACK			3504		// (0x0DB0) ������ͣ��Ӧ��Ϣ
struct S_TCP_PCS2UMS_PAUSETASK_ACK
{
	UL		InvokeID;								// Invoke ID
	UL		TaskID;									// Task ID
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

//--------------------------
// UMS����������Ϣ - 2
//------------
#define TCP_PCS2UMS_DISPATCHTASK_REQ			3510		// (0x0DB6) ����������Ϣ
struct S_TCP_PCS2UMS_DISPATCHTASK_REQ
{
	UL		TaskID;								// Task ID
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];	// ͳһ��Ϣ����ID
	UL		WaitTime;							// �ѵȴ�ʱ�䣨�룩
	US		RoutePointID;						// ·�ɵ�ID
	US		GroupID;							// ��ID
	US		TeamID;								// С��ID
	UC		TeamIndex;							// С�����ȼ�
	UC		UserIndex;							// ����Ա���ȼ�
	char	strUserID[20];						// ����ԱID
	US		LoginID;							// ����Ա����
	UL		AgentID;							// ��ϯID
	char	strExtNO[PCS_DEF_NUMLEN_APP];		// �ֻ�����
	char	strIP[PCS_DEF_NUMLEN_APP];			// �ֻ�IP��ַ
};
//------------

//------------
#define TCP_UMS2PCS_DISPATCHTASK_REQ_ACK		3010	// (0x0BC2)����������Ӧ��Ϣ
struct S_TCP_UMS2PCS_DISPATCHTASK_REQ_ACK
{
	UL		TaskID;								// Task ID
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];	// ͳһ��Ϣ����ID
	US		RoutePointID;						// ·�ɵ�ID
	UC		ErrorCode;							// �����룬�μ��������붨�塱��
													// PCS_DEF_RETCODE_SUCC
													// PCS_DEF_RETCODE_NOTEXIST
													// PCS_DEF_RETCODE_STATUS
													// PCS_DEF_RETCODE_NOTNEED
													// PCS_DEF_RETCODE_NORIGHTS
													// PCS_DEF_RETCODE_NOTSUPPORT
													// PCS_DEF_RETCODE_OTHERS
													// PCS_DEF_RETCODE_SYSERROR
};
//--------------------------
// UMS����״̬�ı���Ϣ - 3
//------------
#define TCP_PCS2UMS_TASKST_DISPACTH				3511	// (0x0DB7) ���䴦����Ϣ
struct S_TCP_PCS2UMS_TASKST_DISPACTH
{
	UL		TaskID;								// Task ID
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];	// ͳһ��Ϣ����ID
	US		RoutePointID;						// ·�ɵ�ID
	char	strUserID[20];						// ����Ա
	UC		DispacthResult;						// �μ�_pcC_OutBoundCallResult
													// _pcC_OB_RESULT_SUCC = '0',				// '0' - �ɹ�
													// _pcC_OB_RESULT_NO_ANS = '1',				// '1' - ��Ӧ��
													// _pcC_OB_RESULT_DEST_BUSY = '2',			// '2' - �Է�æ
													// _pcC_OB_RESULT_ABANDON = '3',			// '3' - ����
};

//------------

//------------
#define TCP_PCS2UMS_TASKST_CANCEL			3512		// (0x0DB8) ����ȡ����Ϣ
struct S_TCP_PCS2UMS_TASKST_CANCEL
{
	UL		TaskID;									// Task ID
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
};
//------------

//------------
#define TCP_PCS2UMS_TASKST_FINISH			3513		// (0x0DB9) ���������Ϣ
struct S_TCP_PCS2UMS_TASKST_FINISH
{
	UL		TaskID;									// Task ID
	char	strUMSTaskID[PCS_DEF_LEN_GCREF];		// ͳһ��Ϣ����ID
};
//------------
//--------------------------

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_UNIFYMESSAGESYSTEM_H__
//------------------------------------------------------------------------
