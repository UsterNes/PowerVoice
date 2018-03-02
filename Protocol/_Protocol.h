////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		_Protocol.h
// ��;������	ͨ��Э��
// ���ߣ�		Tony Sun
// ����ʱ�䣺	1999/12/01
// ����޸�ʱ�䣺2005/12/10
// ��Ȩ��Ϣ��	Copyright (C) 1999 - 2005 PicaSoft
// ��ע��
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// �Զ�����������
//------------------------------------------------------------------------
#ifndef UDL
	#define UDL unsigned __int64
#endif
#ifndef UL
	#define UL unsigned long
#endif
#ifndef US
	#define US unsigned short int
#endif
#ifndef UC
	#define UC unsigned char
#endif
//------------------------------------------------------------------------
//------------------------------------------------------------------------

/*************************************************************************/
/*************************************************************************/

//------------------------------------------------------------------------
// ͨ��Э�鶨��
//------------------------------------------------------------------------

#define PCS_MAX_MESSAGE_SIZE	1024		// ��Ϣ��󳤶�
#define PCS_MAX_DATAFLOW_SIZE	1024		// ��������Ϣ��󳤶�

//------------------------------------------------------------------------
// ��Ϣͷ
//------------------------------------------------------------------------
//
struct SMsgHeader
{
	UC Flag0;					// ��־0
	UC Flag1;					// ��־1
	UC Flag2;					// ��־3
	UC Flag3;					// ��־4
	UC Sender;					// ������
	UC Receiver;				// ������
	US Command;		    		// ������
	US PackageLen;				// ����
	UC TargetSysType;			// Ŀ��ϵͳ����
	UC Reserved1;				// ������AgentЭ��������ReqIndex��1-255��
	UL TargetSysID;				// Ŀ��ϵͳID
};

#define PCS_MSGHDR_FLAG0		0x55		// ��Ϣͷ��־0
#define PCS_MSGHDR_FLAG1		0xAA		// ��Ϣͷ��־1
#define PCS_MSGHDR_FLAG2		0x7F		// ��Ϣͷ��־2
#define PCS_MSGHDR_FLAG3		0xFE		// ��Ϣͷ��־3

//------------------------------------------------------------------------
// �û�����ָ��Ϣ��ͷ�еķ�����&������
//------------------------------------------------------------------------
//
#define	USER_NULL				0			// (0x00) ��Ч

//---------------------------- Core Level
#define USER_CTI 				1			// (0x01) CTI SERVER
#define	USER_PBX				5			// (0x05) PBX
#define	USER_CMS				9			// (0x09) CMS

//---------------------------- Stand Alone Syetem
#define	USER_IVR				20			// (0x14) IVR��ϵͳ
#define	USER_FAX				21			// (0x15) FAX��ϵͳ
#define	USER_DIALER				22			// (0x16) ������ϵͳ
#define	USER_RECORDER			23			// (0x17) ¼����ϵͳ
#define	USER_UMS				32			// (0x20) ͳһ��Ϣ����ģ��

//---------------------------- Service Level
#define	USER_ACD				40			// (0x28) ACD��ϵͳ
#define	USER_OBS				41			// (0x29) �Ⲧ��ϵͳ

//---------------------------- Assitance System
#define USER_DATA 				80			// (0x50) Data Recorder
#define USER_MSG 				81			// (0x51) Messager SERVER
#define USER_PBXCFG				86			// (0x56) PBX Config
#define USER_RECAPT				91			// (0x5B) ¼�������

//---------------------------- Admin Tools
#define	USER_ADMIN				100			// (0x64) ϵͳ�������̨
#define	USER_PROGRAM			101			// (0x65) ���̱༭��ϵͳ
#define	USER_RESOURCE			102			// (0x66) ��Դ������ϵͳ
#define	USER_REPORT				103			// (0x67) ������ϵͳ
#define	USER_OBADMIN			104			// (0x68) �Ⲧ����Ա

//---------------------------- Supervisor
#define	USER_MON_CTI			120			// (0x78) �������ļ��
#define	USER_MON_REC			121			// (0x79) ¼�����
#define	USER_MON_OBS			122			// (0x7A) �Ⲧ���

//---------------------------- Agent
#define USER_TST_CTI 			140			// (0x8C) CTI Test
#define USER_AGT_CTI 			141			// (0x8D) ��ϯ
#define USER_AGT_REC 			142			// (0x8E) ¼���ͻ���
#define USER_AGT_MSG 			143			// (0x8F) ��Ϣ�ͻ���
#define USER_AGT_MSI 			144			// (0x90) ��ϯ���ͻ���

//---------------------------- External System
#define USER_EXT_IVR 			201			// (0xC9) IVR�ⲿ�ӿ�ģ��
#define USER_EXT_EMAIL 			202			// (0xCA) �����ʼ�ģ��
#define USER_EXT_SMS 			203			// (0xCB) ����ģ��
#define USER_EXT_FAX 			204			// (0xCC) FAX����ģ��

#define	USER_DUMMY				255			// (0xFF) Dummy System

//------------------------------------------------------------------------
// ��Ϣ����
//------------------------------------------------------------------------
//
//------------------------------------------
// ��ͬ��Ϣ
//------------------------------------------
#define TCP_PCS2ALL_TEXTMSG					0			// (0x0000) ������Ϣ

// Keep Live Message
#define TCP_KEEPLIVE_ACK					3			// (0x0003) Keep Live Acknowledge
#define TCP_KEEPLIVE_MSG					1003		// (0x03EB) Keep Live

// ��Ϣ��������Ҫ����ͬ������ʱ�Ĵ���������Ϣ��Simon added 2010-05-04
#define TCP_MESSAGEPACKED					1004		// (0x3EC) ��Ϣ����
// �䳤��Ϣ���������������Ϣ�����պ�չ����������

// CTI Status Report Message
#define TCP_PCS2ALL_CTILinkStart			10001		// (0x2711) CTI Link Connection is OK
#define TCP_PCS2ALL_CTILinkStop				10002		// (0x2712) CTI Link Connection is Down
struct S_TCP_PCS2ALL_CTILinkShift
{
	char strDeviceID[PCS_DEF_NUMLEN_APP];				// �豸ID, PBX��ʾ����������
};

// ACD Status Report Message
#define TCP_PCS2ALL_ACD_STATUS_REPORT		10011		// (0x271B) ·�ɵ�״̬���ݱ�����Ϣ
struct S_TCP_PCS2ALL_ACD_STATUS_REPORT
{
	// Sun added 2004-12-30
	UC		nType;					// �������Center / RoutePoint / Queue / Group / Team����
									/// �μ�_pcC_CallDistributeObject����
	US		nGroupID;				// ����ID
	
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
	// Sun added 2004-09-14
	/// Reason Data
	US		nReason0;				// ѡ�����ɴ���0������
	US		nReason1;				// ѡ�����ɴ���1������
	US		nReason2;				// ѡ�����ɴ���2������
	US		nReason3;				// ѡ�����ɴ���3������
	US		nReason4;				// ѡ�����ɴ���4������
	US		nReason5;				// ѡ�����ɴ���5������
	US		nReason6;				// ѡ�����ɴ���6������
	US		nReason7;				// ѡ�����ɴ���7������
	US		nReason8;				// ѡ�����ɴ���8������
	US		nReason9;				// ѡ�����ɴ���9������
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	// Sun added 2007-04-15
	US		nEstimatedWaitTime;		// Ԥ�Ƶȴ�ʱ��
	
	// 2010-4-12 Defined nReserved0 - nReserved2
	US		nATT;					// IBƽ��ͨ��ʱ��
	US		nACW;					// IBƽ������ʱ��
	US		nASA;					// IBƽ��Ӧ���ٶ�

	// 2010-5-13 Defined nReserved3 - nReserved7
	US		nOBATT;					// OBƽ��ͨ��ʱ��
	US		nOBACW;					// OBƽ������ʱ��
	US		nOBAveWaitTime;			// OBƽ���ȴ�ʱ��
	UL		nOBCallTotal;			// OB��������(�����ۼ�)
	UL		nOBConnect;				// OB��ͨ����(�����ۼ�)

	UL		nReserved8;
	UL		nReserved9;
	//----------------------------------------------------------------
};

// ��ϵͳ״̬������Ϣ
#define TCP_PCS2ALL_SUBSYS_STATUS_REPORT	1008		// (0x03F0) ��ϵͳ״̬������Ϣ
struct S_TCP_PCS2ALL_SUBSYS_STATUS_REPORT
{
	UC		SubSysID;						// ��ϵͳID���μ��������ߡ������߶��塱
	UC		SubSysStatus;					// ��ϵͳ״̬��PCS�μ���PCS״̬���塱������ϵͳ�μ������س������塱
	UC		SubSysVersion;					// ��ϵͳ�汾��
	UC		SubSysIndex;					// ��ϵͳ��ţ�����ϵͳ��
	UC		SubSysType;						// ��ϵͳ����
	UC		Reserved[3];					// ����
};


// Sun added 2005-12-11
//--------------------------
// ������ʱ�Ӳ�ѯ - 2
//------------
#define TCP_ALL2PCS_GET_SERVERTIME			9			// (0x0009) ��ѯ������ʱ����Ϣ
//------------

//------------
#define TCP_PCS2ALL_GET_SERVERTIME_ACK		1009		// (0x03F1) ��ѯ������ʱ����Ӧ��Ϣ
struct S_TCP_PCS2ALL_GET_SERVERTIME_ACK
{
	UL		lngTimeZoneBias;		// ʱ��ƫ��(����)
	char	strDate[12];			// ���ڣ�"yyyy-mm-dd"
	char	strTime[10];			// ʱ�䣬"HH:MM:SS"
};

// Sun added 2005-12-10
//--------------------------
// ͳ�����ݲ�ѯ - 2
//------------
#define TCP_ALL2PCS_QUERY_WORKDATA			10			// (0x000A) ͳ�����ݲ�ѯ��Ϣ
struct S_TCP_ALL2PCS_QUERY_WORKDATA
{
	char strAliasName[50];								// ��¼����
};
//------------

//------------
#define TCP_PCS2ALL_QUERY_WORKDATA_ACK		1010		// (0x03F2) ͳ�����ݲ�ѯ��Ӧ��Ϣ
// Statistics data for user
struct S_TCP_PCS2ALL_USER_ST_REPORT
{
	char strAliasName[50];								// ��¼����

	// Call Data
	US lngIBCalls;					// IB�绰��
	US lngOBCalls;					// OB�绰��
	US lngIBSucc;					// IB�ɹ��绰��
	US lngOBSucc;					// OB�ɹ��绰��
	UL lngIBTalkLength;				// IBͨ��ʱ��
	UL lngOBTalkLength;				// OBͨ��ʱ��
	UL lngIBWaitLength;				// IB�ȴ�ʱ��
	UL lngOBWaitLength;				// OB�ȴ�ʱ��

	// Status Data
	US nLoginTimes;					// ��¼����
	UL nLoginLength;				// ��¼ʱ��
	US nReadyTimes;					// ��������
	UL nReadyLength;				// ����ʱ��
	UL nAvailableLength;			// ����ʱ��
	US nIBACWTimes;					// IB�������
	UL nIBACWLength;				// IB����ʱ��
	US nOBACWTimes;					// OB�������
	UL nOBACWLength;				// OB����ʱ��
	US nIBHoldTimes;				// IB���ִ���
	UL nIBHoldLength;				// IB����ʱ��
	US nOBHoldTimes;				// OB���ִ���
	UL nOBHoldLength;				// OB����ʱ��
	US nReasonTimes0;				// ����0����
	UL nReasonLength0;				// ����0ʱ��
	US nReasonTimes1;				// ����1����
	UL nReasonLength1;				// ����1ʱ��
	US nReasonTimes2;				// ����2����
	UL nReasonLength2;				// ����2ʱ��
	US nReasonTimes3;				// ����3����
	UL nReasonLength3;				// ����3ʱ��
	US nReasonTimes4;				// ����4����
	UL nReasonLength4;				// ����4ʱ��
	US nReasonTimes5;				// ����5����
	UL nReasonLength5;				// ����5ʱ��
	US nReasonTimes6;				// ����6����
	UL nReasonLength6;				// ����6ʱ��
	US nReasonTimes7;				// ����7����
	UL nReasonLength7;				// ����7ʱ��
	US nReasonTimes8;				// ����8����
	UL nReasonLength8;				// ����8ʱ��
	US nReasonTimes9;				// ����9����
	UL nReasonLength9;				// ����9ʱ��
};
//------------

#define TCP_ALL2DBR_EXECUTE_SQL				11000		// (0x2AF8) ִ��SQL���


// PCSѡ��ı���ڹ���Э�����棬����ҪΪÿ��Э����Ӵ���Ϣ
#define TCP_ALL2PCS_PCSSELECTCHANGED_EVT	7			// (0x0007) PCSѡ��ı�
struct S_TCP_ALL2PCS_PCSSELECTCHANGED_EVT
{
	UC		PCS0Status;						// �ο���PCS_PCSStatus
	UC		PCS1Status;						// �ο���PCS_PCSStatus
	UC		ActivePCSID;					// ��ԾPCSID��-1��ʾû�л�ԾPCS��
};


//------------------------------------------
// 1��¼������ͨ��Э��
//------------------------------------------
#if PCS_APP_IDENTIFICATION & (0x0001 << 1 )
#include "_Protocol_RECSVR.h"
#endif

//------------------------------------------
// 2����ϯ��Ϣ
//------------------------------------------
#if PCS_APP_IDENTIFICATION & (0x0001 << 2 )
#include "_Protocol_Agent.h"
#endif

//------------------------------------------
// 3���������ļ����Ϣ
//------------------------------------------
#if PCS_APP_IDENTIFICATION & (0x0001 << 3 )
#include "_Protocol_Monitor.h"
#endif

//------------------------------------------
// 4��·�ɹ�����Ϣ
//------------------------------------------
#if PCS_APP_IDENTIFICATION & (0x0001 << 4 )
#include "_Protocol_RMR.h"
#endif

//------------------------------------------
// 5���Ⲧ������Ϣ
//------------------------------------------
#if PCS_APP_IDENTIFICATION & (0x0001 << 5 )
#include "_Protocol_OBS.h"
#endif

//------------------------------------------
// 6���������̨��Ϣ
//------------------------------------------
#if PCS_APP_IDENTIFICATION & (0x0001 << 6 )
#include "_Protocol_Admin.h"
#endif

//------------------------------------------
// 7��CTI���Կͻ���ͨ��Э��
//------------------------------------------
#if PCS_APP_IDENTIFICATION & (0x0001 << 7 )
#include "_Protocol_CTITST.h"
#endif

//------------------------------------------
// 8��IVRͨ��Э��
//------------------------------------------
#if PCS_APP_IDENTIFICATION & (0x0001 << 8 )
#include "_Protocol_IVRS.h"
#endif

//------------------------------------------
// 9����Ϣ����ͨ��Э��
//------------------------------------------
#if PCS_APP_IDENTIFICATION & (0x0001 << 9 )
#include "_Protocol_MSGR.h"
#endif

//------------------------------------------
// 10��UMSͨ��Э��
//------------------------------------------
#if PCS_APP_IDENTIFICATION & (0x0001 << 10 )
#include "_Protocol_UMS.h"
#endif

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_H__
//------------------------------------------------------------------------
