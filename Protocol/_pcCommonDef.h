////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		_pcCommonDef.h
// ��;������   PowerCall��ͨ����
// ���ߣ�		SUN
// ����ʱ�䣺	2002/06/05
// ����޸�ʱ�䣺2006/08/21
// ��Ȩ��Ϣ��	Copyright (C) 1998 - 2004 Proactive Cyber Space
// ��ע��
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PCCOMMONDEF_H__
#define __PCCOMMONDEF_H__
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// ��Ϣ�ͻ���״̬
enum _pcC_stMsgClientStatus
{
	//_pcC_stMsgClientStatus_UNKNOWN = 0,			// ��Ч
	_pcC_stMsgClientStatus_LOGOUT,				// δ��¼
	_pcC_stMsgClientStatus_LOGIN,				// δ��¼
	//_pcC_stMsgClientStatus_READY,				// ����
	//_pcC_stMsgClientStatus_NOTREADY,			// δ����
};
/*
//------------------------------------------------------------------------
// ��Ϣ�ͻ���δ����ԭ��
enum _pcC_stMsgClientReason
{
	_pcC_stMsgClientReason_UNKNOWN = 0,			// Ϊֹ
	_pcC_stMsgClientReason_BUSY,				// æµ
	_pcC_stMsgClientReason_LEAVE,				// �뿪
};
*/
//------------------------------------------------------------------------
// ��Ϣ�ͻ�������
enum _pcC_stMsgClientType
{
	_pcC_stMsgClientType_USEDUSER = 0,			// ������ϯ
	_pcC_stMsgClientType_UNUSEDUSER,			// δ������ϯ
	_pcC_stMsgClientType_MONITOR,				// MONITOR
};

//------------------------------------------------------------------------
// λ��״̬
enum _pcC_stPOS
{
	_pcC_POS_UNKNOWN = 0,					// ��Ч
	_pcC_POS_LOGOUT,						// δ��¼
	_pcC_POS_NOTREADY,						// δ����
	_pcC_POS_AFTERCALL,						// ����
	_pcC_POS_IDLE,							// ����	
	_pcC_POS_RING,							// ����
	_pcC_POS_BUSY,							// æ
	_pcC_POS_HOLD							// ���б���
};

//------------------------------------------------------------------------
// ����״̬
enum _pcC_stDN
{
	_pcC_DN_UNKNOWN = 0,					// ��Ч
	_pcC_DN_DND,							// �����
	_pcC_DN_IDLE,							// ����	
	_pcC_DN_RING,							// ����
	_pcC_DN_OFFHOOK,						// ժ��
	_pcC_DN_INBOUND,						// ����ͨ��
	_pcC_DN_OUTBOUND,						// ����ͨ��
	_pcC_DN_HOLD							// ���б���
};

//------------------------------------------------------------------------
// ��������
enum _pcC_PhoneType
{
	_pcC_PH_TYPE_UNKNOWN = 0,				// �޻���
	_pcC_PH_TYPE_ANA_HAND,					// ģ����ֱ�
	_pcC_PH_TYPE_ANA_EAR,					// ģ�������
	_pcC_PH_TYPE_DGT_HANDLE,				// ���ִ��ֱ� (eg. 2008)
	_pcC_PH_TYPE_DGT_EAR,					// ���ִ����� (eg. 2616)
	_pcC_PH_TYPE_DGT_IP						// IP ����
};

//------------------------------------------------------------------------
// �¼���Դ
enum _pcC_EventSource
{
	_pcC_EVTSRC_UNKNOWN = 0,				// δ֪
	_pcC_EVTSRC_DN,							// ����DN
	_pcC_EVTSRC_POSITION,					// ����Position
	_pcC_EVTSRC_ACD,						// ����ACD DN
	_pcC_EVTSRC_CDN							// ����CDN (Route Point)	
};

//------------------------------------------------------------------------
// ������Դ
enum _pcC_CallSource
{
	_pcC_CALLSRC_INTERNAL = 0,				// �ڲ�ת�Ӽ�����
	_pcC_CALLSRC_CALLCENTER,				// �ⲿ-����������
	_pcC_CALLSRC_LOADBALANCE,				// �ⲿ-������������ƽ�⸺��
	_pcC_CALLSRC_WEB						// �ⲿ-Web
};

//------------------------------------------------------------------------
// ��������
enum _pcC_CallType
{
    _pcC_CALLTYPE_UNKNOWN = 0,
    _pcC_CALLTYPE_INBOUND,
    _pcC_CALLTYPE_OUTBOUND,
    _pcC_CALLTYPE_CONFERENCE
};

//------------------------------------------------------------------------
// ���з�Χ
enum _pcC_CallScale
{
    _pcC_CALLSCALE_UNKNOWN = 0,
    _pcC_CALLSCALE_INTERNAL,
    _pcC_CALLSCALE_LOCAL,
	_pcC_CALLSCALE_NATIONAL,
	_pcC_CALLSCALE_INTERNATIONAL,
    _pcC_CALLSCALE_WEB
};

//------------------------------------------------------------------------
// ��������
enum _pcC_CallOperator
{
    _pcC_CALLOPT_THISPARTY = 0,
    _pcC_CALLOPT_OTHERPARTY,
	_pcC_CALLOPT_THIRDPARTY
};

//------------------------------------------------------------------------
// ���д���ģʽ
enum _pcC_ConsultationMode
{
	_pcC_CONSMODE_GENERIC = 0,
    _pcC_CONSMODE_TRANSFER = 1,
	_pcC_CONSMODE_CONFERENCE = 2
};

//------------------------------------------------------------------------
// ·�ɵȴ�����ʽ
enum _pcC_RouteRespondMethod
{
	_pcC_RRMETHOD_UNKOWN = 0,
	_pcC_RRMETHOD_TRANSFER,
    _pcC_RRMETHOD_IVRACD,
	_pcC_RRMETHOD_RINGBACK,
	_pcC_RRMETHOD_BUSYTONE,
	_pcC_RRMETHOD_MUSIC,
	_pcC_RRMETHOD_RAN
};

//------------------------------------------------------------------------
// ·�ɽ��
enum _pcC_RouteRespondResult
{
	_pcC_RRRESULT_UNKNOWN = 0,
	_pcC_RRRESULT_TIMEOUT,					// ת�ӳ�ʱ����
	_pcC_RRRESULT_FAILED,					// ת��ʧ��
	_pcC_RRRESULT_ABANDON,					// ת����������
	_pcC_RRRESULT_SUCC,						// ת�ӳɹ�
	_pcC_RRRESULT_SUCC_INTIME,				// �ڷ���ˮƽ��ת�ӳɹ�
};

//------------------------------------------------------------------------
// �Ⲧ����״̬
enum _pcC_OutBoundTaskFlag
{
	_pcC_OB_TASK_FLAG_NEW =	'N',			// ������
	_pcC_OB_TASK_FLAG_OVER = 'C',			// ���
	_pcC_OB_TASK_FLAG_PROCESS = 'I',		// ������
	_pcC_OB_TASK_FLAG_DISPATCH = 'D',		// �ѷ���
	_pcC_OB_TASK_FLAG_ERROR = 'E',			// ����
	_pcC_OB_TASK_FLAG_RETRY = 'R',			// ����
	_pcC_OB_TASK_FLAG_OVERDUE = 'O',		// ����
	_pcC_OB_TASK_FLAG_TRANSFER = 'T',		// ת����
	_pcC_OB_TASK_FLAG_PAUSE = 'P'			// ��ͣ

};

//------------------------------------------------------------------------
// �Ⲧִ�н��
enum _pcC_OutBoundCallResult
{
	_pcC_OB_RESULT_SUCC = '0',				// '0' - �ɹ�
	_pcC_OB_RESULT_NO_ANS = '1',			// '1' - ��Ӧ��
	_pcC_OB_RESULT_DEST_BUSY = '2',			// '2' - �Է�æ
	_pcC_OB_RESULT_ABANDON = '3',			// '3' - ����
	_pcC_OB_RESULT_DEST_WRONG = '6',		// '6' - �����
	_pcC_OB_RESULT_POWER_OFF = '7',			// '7' - �ػ�
	_pcC_OB_RESULT_NOT_REACH = '8',			// '8' - ���ڷ���Χ
	_pcC_OB_RESULT_STOP_SVR = '9',			// '9' - Ƿ��ͣ��
	_pcC_OB_RESULT_FAX_TONE = 'F',			// 'F' - ����
	_pcC_OB_RESULT_IVR_TONE = 'A',			// 'A' - �Զ�����
	_pcC_OB_RESULT_MODEM_TONE = 'M',		// 'M' - Modem
	_pcC_OB_RESULT_OP_TIMEOUT = 'T',		// 'T' - Timeout
	_pcC_OB_RESULT_SYSTEM_ERROR = 'S',		// 'S' - System Error
	_pcC_OB_RESULT_OTHERS = 'O'				// 'O' - ����
};

//------------------------------------------------------------------------
// ͳһ��Ϣ��������
enum _pcC_UMSTaskTypes
{
	_pcC_UMS_TASK_TYPE_UNKNOWN = 0,			// δ֪
	_pcC_UMS_TASK_TYPE_IBCALL,				// ����绰
	_pcC_UMS_TASK_TYPE_OBCALL,				// �����绰
	_pcC_UMS_TASK_TYPE_GENERAL = 10,		// ͨ������
	_pcC_UMS_TASK_TYPE_EMAIL,				// �����ʼ�����
	_pcC_UMS_TASK_TYPE_FAX,					// ��������
	_pcC_UMS_TASK_TYPE_SMS,					// ��������
	_pcC_UMS_TASK_TYPE_VMS,					// ��������
	_pcC_UMS_TASK_TYPE_CB,					// �ز�����
	_pcC_UMS_TASK_TYPE_BBS,					// BBS����
	_pcC_UMS_TASK_TYPE_CHAT,				// Web Chat����
	_pcC_UMS_TASK_TYPE_WEBCALL,				// Web Call����
	_pcC_UMS_TASK_TYPE_WEBCOLLAB,			// Web Эͬ����
	_pcC_UMS_TASK_TYPE_VIDEO				// ��Ƶ����
};

//------------------------------------------------------------------------
// �෽�����¼�����
enum _pcC_MultiPartyOperationEvent
{
	_pcC_MULTIOP_DUMMY = '0',				// δ����
	_pcC_MULTIOP_TpTRANSFER = '1',			// This Party Transfer
	_pcC_MULTIOP_OpTRANSFER = '2',			// Other Party Transfer
	_pcC_MULTIOP_TpCONFERENCE = '3',		// This Party Conference
	_pcC_MULTIOP_OpCONFERENCE = '4',		// Other Party Conference
	_pcC_MULTIOP_TpTransfered = '5',		// This Party Transfered Successfully
	_pcC_MULTIOP_OpConferenced = '6',		// Other Party Conferenced Successfully
	_pcC_MULTIOP_TpConferenced = '7',		// This Party Conferenced Successfully
	_pcC_MULTIOP_OpTransfered = '8',		// Other Party Transfered Successfully
	_pcC_MULTIOP_TpSlowTransfered = 'C',	// This Party Slow Transfered Successfully
	_pcC_MULTIOP_TpFastTransfered = 'F',	// This Party Fast Transfered Successfully
};

//------------------------------------------------------------------------
// ����������
enum _pcC_CallDistributeObject
{
	_pcC_CDOBJ_CENTER = 0,					// ������ȫ����ϯ
	_pcC_CDOBJ_GROUP = 1,					// ��ϯ��
	_pcC_CDOBJ_TEAM = 2,					// ��ϯС��
	_pcC_CDOBJ_AGENT = 3,					// ��ϯ
	_pcC_CDOBJ_DN = 4,						// ����ֻ�, Sun added 2011-12-20
	_pcC_CDOBJ_ACD = 6,						// ����ֻ���, Sun added 2011-12-20
	_pcC_CDOBJ_QUEUE = 7,					// ����
	_pcC_CDOBJ_ROUTEPOINT = 8,				// ·�ɵ�
	_pcC_CDOBJ_DEPARTMENT = 11,				// ����
	_pcC_CDOBJ_CUST = 12,					// �⻧
	_pcC_CDOBJ_GLOBAL = 15					// ��������
};

//------------------------------------------------------------------------
// ����ת�����Ͷ���
enum _pcC_CallForwardType
{
	_pcC_CFWT_ALL = 0,						// ת�����е绰
	_pcC_CFWT_BUSY,							// ��æת��
	_pcC_CFWT_NOANS,						// ��Ӧ��ת��
	_pcC_CFWT_BUSY_NOANS					// ��æ����Ӧ��ת��
};

//------------------------------------------------------------------------
// �Ŷӷ�ʽ
enum _pcC_QueueType
{
	_pcC_FIRST_AVALIABLE = 1,				// ֪����
	_pcC_MAX_IDEL_TIME,						// �����ʱ��(�ϴ�ͨ�����ۼ�)
	_pcC_MIN_CALL_DENAITY,					// ��λʱ����Сͨ���ܶ�(����)
	_pcC_MIN_WORKLOAD,						// ��λʱ����С��������(ͨ��ʱ��+����ʱ��)
	_pcC_LONGEST_IDEL,						// �����м�ʱ(���ۼ�)
	_pcC_LONGEST_CALL_INTERVAL				// ���ͨ�����(���ۼ�)
};

//------------------------------------------------------------------------
// �������
enum _pcC_OverFlow
{	
	_pcC_MAX_LENGTH = 1,					// ���г���
	_pcC_MAX_WAITE_TIME,					// ��ȴ�ʱ��
	_pcC_MIN_LOGIN_COUNT,					// ��С��¼��ϯ��
	_pcC_MIN_IDLE_COUNT						// ��С������
};

//------------------------------------------------------------------------
// ��ض˺��п�������
enum _pcC_MonitorControlType
{	
	_pcC_MONCTRL_Logout = 0,				// ǿ��ע��
	_pcC_MONCTRL_SingleStepTran = 1,		// ����ת��
	_pcC_MONCTRL_SingleStepConf = 2,		// ��������
	_pcC_MONCTRL_ConsultTran = 3,			// ����ת��
	_pcC_MONCTRL_ConsultConf = 4,			// ���̻���
	_pcC_MONCTRL_CompleteTran = 5,			// ת�����
	_pcC_MONCTRL_CompleteConf = 6,			// �������
	_pcC_MONCTRL_HoldCall = 7,				// ǿ�Ʊ���
	_pcC_MONCTRL_RetrieveCall = 8,			// ǿ��ʰ��
	_pcC_MONCTRL_ClearCall = 9,				// ǿ�ƹҶ�
};

// Simon Added 2010-05-04
// PCS״̬���壺
typedef enum _tag_PCSStatus
{
	PCS_PS_Init,				// ��ʼ��
	PCS_PS_Active,				// ��Ծ
	PCS_PS_Standby,				// �������󱸣�
	PCS_PS_Disconnected,		// �Ͽ�(δ֪)
	PCS_PS_Closed,				// �Ѿ��ر�
}PCS_PCSStatus;

// RM״̬���壺
typedef enum _tag_RMStatus
{
	PCS_RMS_Init,				// ��ʼ��
	PCS_RMS_InService,			// ������
	PCS_RMS_Disconnected,		// �Ͽ�(δ֪)
	PCS_RMS_Closed,				// �Ѿ��ر�
}PCS_RMStatus;

//------------------------------------------------------------------------
// ��ϯ�����
#define PCS_DEF_AGTTYP_USER					1			// ����Ա��ϯ
#define PCS_DEF_AGTTYP_IVR					2			// IVR�豸
#define PCS_DEF_AGTTYP_DIALER				3			// �Ⲧ�豸

//------------------------------------------------------------------------
// ����Ա�����
#define PCS_DEF_USERTYP_INBOUND				0x00000001	// ���봦��
#define PCS_DEF_USERTYP_OUTBOUND			0x00000002	// ��������
#define PCS_DEF_USERTYP_WEB					0x00000004	// Web Call & ��Ƶ������
#define PCS_DEF_USERTYP_EMAIL				0x00000008	// EMail����
#define PCS_DEF_USERTYP_FAX					0x00000010	// FAX����
#define PCS_DEF_USERTYP_SMS					0x00000020	// SMS����
#define PCS_DEF_USERTYP_VMS					0x00000040	// VMS����
#define PCS_DEF_USERTYP_CALLBACK			0x00000080	// �ز���¼����
#define PCS_DEF_USERTYP_BBS					0x00000100	// BBS & Chat������

//------------------------------------------------------------------------
// �����붨��
#define PCS_DEF_RETCODE_SUCC				0			// (0x00) �ɹ�
#define PCS_DEF_RETCODE_NOTEXIST			1			// (0x01) ��ϯ������
#define PCS_DEF_RETCODE_BEUSED				2			// (0x02) �Ѿ�ռ��
#define PCS_DEF_RETCODE_STATUS				3			// (0x03) ״̬��
#define PCS_DEF_RETCODE_NOTNEED				4			// (0x04) ����Ҫ
#define PCS_DEF_RETCODE_VERSION				10			// (0x0A) �汾����
#define PCS_DEF_RETCODE_REGCODE				11			// (0x0B) ע�������
#define PCS_DEF_RETCODE_PASSWORD			12			// (0x0C) �����
#define PCS_DEF_RETCODE_NORIGHTS			13			// (0x0D) ��Ȩ��
#define PCS_DEF_RETCODE_NOLICENSE			14			// (0x0E) �����������
#define PCS_DEF_RETCODE_LACK_PARAM			15			// (0x0F) ��������
#define PCS_DEF_RETCODE_REJECTED			16			// (0x10) ���󱻾ܾ� 
#define PCS_DEF_RETCODE_NOTSUPPORT			99			// (0x63) ��֧��
#define PCS_DEF_RETCODE_OTHERS				100			// (0x64) ��������
#define PCS_DEF_RETCODE_SYSERROR			255			// (0xFF) ϵͳ��

//------------------------------------------------------------------------
// Agent Working Detail Record Status
#define PCS_AWDR_EVENT_CLOSE_LOG			'L'			// Login ��������
#define PCS_AWDR_EVENT_CLOSE_RDY			'R'			// Ready ��������
#define PCS_AWDR_EVENT_LOGOUT				'0'			// Logout
#define PCS_AWDR_EVENT_LOGIN				'1'			// Login
#define PCS_AWDR_EVENT_READY				'2'			// Ready
#define PCS_AWDR_EVENT_NOTREADY				'3'			// Not Ready
#define PCS_AWDR_EVENT_AFTERCALL			'4'			// After Call Work (IB or Default)
#define PCS_AWDR_EVENT_TP_RING				'5'			// This party ringing
#define PCS_AWDR_EVENT_OP_RING				'6'			// Other party ringing
#define PCS_AWDR_EVENT_OB_AFTERCALL			'7'			// After Call Work (OB)
#define PCS_AWDR_EVENT_AVAILABLE			'A'			// Available (Agent Ready & Phone Idle)
#define PCS_AWDR_EVENT_IBCALL				'I'			// In bound call
#define PCS_AWDR_EVENT_OBCALL				'O'			// Out bound call
#define PCS_AWDR_EVENT_IBHOLD				'H'			// In bound hold
#define PCS_AWDR_EVENT_OBHOLD				'Q'			// Out bound hold
#define PCS_AWDR_EVENT_PICKUP				'P'			// Call Pickup, Sun added 2011-12-20

//------------------------------------------------------------------------
// ���س�������
#define PCS_DEF_CHOICE_NO					0			// Off / No / Disnabled
#define PCS_DEF_CHOICE_YES					1			// On / Yes / Enabled
#define PCS_DEF_UNNORMAL_EXIT				2			// �������˳�

//------------------------------------------------------------------------
// ���ݳ��ȳ�������
#define PCS_DEF_LEN_GCREF					24			// length of global call ref id
#define PCS_DEF_NUMLEN_APP					32			// Tel. Number data length
#define PCS_DEF_DATALEN_APP					128			// Application data length
#define PCS_DEF_DATALEN_USER				256			// User data length
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// ¼��ϵͳ��������
//------------------------------------------------------------------------

//
// ¼����ʽ���� added by stephen wang 2006-08-21
//
enum _pcC_RecFormatTag
{
	_pcC_RFT_UNKNOW = 0,						// δ֪��ʽ
	_pcC_RFT_G729,								// G.729��ʽ
	_pcC_RFT_ADPCM,								// ADPCM��ʽ
};


//
// ¼���洢��ʽ���� added by stephen wang 2006-12-12
//
enum _pcC_APRecSaveMode
{
	_pcC_AP_RSM_UNKNOW = 0,						// δ֪��ʽ
	_pcC_AP_RSM_NETWORK,						// ���紫��
	_pcC_AP_RSM_LOCAL,							// AP���ر���
	_pcC_AP_RSM_LOCAL_AND_NO_REALTIMEDTA,		// AP���ر��棨ֱ�ӱ����ļ���
	_pcC_AP_RSM_LOCAL_AND_NETWORK,				// ���ؼ�����
};

// Simon Added 2011-06-28
// ö�٣�ACD����ģʽ For Avaya
enum _pcC_ACDControlMode
{
	_pcC_AM_NONE=0,				//ʲôҲ����
	_pcC_AM_ALLREADLY,			//�Զ�����������ϯ
	_pcC_AM_SYNCH,				//����ACDͬ��
};

#define _pcC_AP_PARAM_TIME_SYNC						(_T("TimeToSync"))				// ��������ʱ��ͬ��ʱ��
#define _pcC_AP_PARAM_FILE_SAVE_MODE				(_T("FileSaveMethode"))			// �ļ����淽ʽ
#define _pcC_AP_PARAM_FILE_SAVE_TYPE				(_T("FilesSaveType"))			// �ļ������ʽ
#define _pcC_AP_PARAM_FILE_CONTROL_TYPE				(_T("RecControlType"))			// AP���Ʒ�ʽ
#define _pcC_AP_PARAM_FILE_REC_INDEX_SYNC_TIME		(_T("RecordIndexSyncTime"))		// APδ�ϴ���¼��������־

//
// ¼�����Ʒ�ʽ
//
#define PCS_DEF_RECORD_TRIGGER_UNKNOWN		0			// δ֪
#define PCS_DEF_RECORD_TRIGGER_VOICE		1			// ��ѹ����
#define PCS_DEF_RECORD_TRIGGER_MANUAL		2			// �ֶ�����(�ͻ�����)
#define PCS_DEF_RECORD_TRIGGER_API			3			// API����
#define PCS_DEF_RECORD_TRIGGER_MONITOR		4			// �೤����
#define PCS_DEF_RECORD_TRIGGER_VOLTAGE		5			// ��ѹ����
#define PCS_DEF_RECORD_TRIGGER_DTMF			6			// ���
#define PCS_DEF_RECORD_TRIGGER_CTI			9			// CTI����

//
// ����Ƶ��
//
#define PCS_DEF_SAMPLE_FREQ_POOR			0			// 8000 Hz
#define PCS_DEF_SAMPLE_FREQ_LOW				1			// 11025 Hz
#define PCS_DEF_SAMPLE_FREQ_NORMAL			2			// 22050 Hz
#define PCS_DEF_SAMPLE_FREQ_HIGH			3			// 44100 Hz

//
// ¼���ļ�ͷ����
//
#define PCS_DEF_FORMAT_DESC_LEN				512			// Bytes

//------------------------------------------------------------------------
#endif  //End #ifndef #define __PCCOMMONDEF_H__
//------------------------------------------------------------------------
