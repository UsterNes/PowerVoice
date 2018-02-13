////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		_GLOBAL.h
// ��;������	ȫ�����ͼ���������
// ���ߣ�		SUN
// ����ʱ�䣺	1999/12/01
// ����޸�ʱ�䣺2002/04/18
// ��Ȩ��Ϣ��	Copyright (C) 1999 - 2000 Changjiang Xincheng
// ��ע��
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//------------------------------------------------------------------------
#ifndef __GLOBAL_H__
#define __GLOBAL_H__
//------------------------------------------------------------------------

#include "StdAfx.h"

#include "IVRChannel.h"
#include "IVRGroup.h"

// Sun added 2006-10-01
#include "FaxManager.h"

//------------------------------------------------------------------------
// Devices configuration
#define DEV_NONE			0
#define DEV_ANALOG			1
#define DEV_MSI				2
#define DEV_VOICE			3
#define DEV_DIGIT			4
#define DEV_ISDN			5				// ISDN
#define DEV_ICAPI			6				// E1
#define DEV_IP				10				// IP

#define TID_DISCON_DT	90
#define TID_DISCON_ST	91

#define TID_DISCON_DT2	92
#define TID_DISCON_ST2	93

#ifndef MAX_IVR_GROUP
#define MAX_IVR_GROUP		256			// ���IVR Group
#endif

#ifndef MAX_IVR_CHANNEL
#define MAX_IVR_CHANNEL		512			// ���IVR Channel
#endif

#ifndef MAX_MSILINES
#define MAX_MSILINES		256			// ������ϯ��·��
#endif

#ifndef MAX_CHANNEL_TONES
#define MAX_CHANNEL_TONES	13			// �����Ƶ����������
#endif

//------------------------------------------------------------------------
// ȱʡϵͳ����
#define FN_VOXPATH			"C:\\voice\\"		// ��ʾ��Ŀ¼
#define FN_RECPATH			"C:\\record\\"		// ¼��Ŀ¼
#define FN_TEMPPATH			"C:\\ivrtemp\\"		// IVR��ʱĿ¼

//------------------------------------------------------------------------
// ���泣��
#define SCR_LIST_ITEM_ALL				255		// ȫ����Ŀ
#define SCR_GROUP_ITEM_GROUPID			0		// ���(Group#)
#define SCR_GROUP_ITEM_CAPTION			1		// ����(Caption)
#define SCR_GROUP_ITEM_PROGRAM			2		// ��ǰ����ID(CurIVR)
#define SCR_GROUP_ITEM_RESOURCE			3		// ��ǰ��ԴID
#define SCR_GROUP_ITEM_STARTCH			4		// ��ʼͨ��(Start CH)
#define SCR_GROUP_ITEM_ENDCH			5		// ��ֹͨ��(End CH)
#define SCR_GROUP_ITEM_CTI				6		// CTI ����״̬
#define SCR_GROUP_ITEM_CALLCOUNT		7		// �����ۼ�
#define SCR_GROUP_ITEM_LASTLOADTIME		8		// ������ʱ��

#define SCR_CHANNEL_ITEM_CHID			0		// ͨ����(CH#)
#define SCR_CHANNEL_ITEM_STATUS			1		// Status
#define SCR_CHANNEL_ITEM_CALLLENGTH		2		// ͨ��ʱ��
#define SCR_CHANNEL_ITEM_CALLERID		3		// ���к���
#define SCR_CHANNEL_ITEM_CALLEEID		4		// ���к���
#define SCR_CHANNEL_ITEM_CTI			5		// CTI�ܿ�״̬
#define SCR_CHANNEL_ITEM_CALLCOUNT		6		// �����ۼ�
#define SCR_CHANNEL_ITEM_EDID			7		// ��ǰ״̬���

//------------------------------------------------------------------------
// �Զ�����Ϣ
#ifndef MYWM_CHANGE_GROUP				// �޸�IVR����Ϣ��Ϣ
#define MYWM_CHANGE_GROUP				WM_USER + 101
#endif

#ifndef MYWM_CHANGE_CHANNEL				// �޸�IVR��Ϣ��Ϣ
#define MYWM_CHANGE_CHANNEL				WM_USER + 102
#endif

#ifndef MYWM_IVREVENT					// �޸�IVR����Ϣ��Ϣ
#define MYWM_IVREVENT					WM_USER + 1001
#endif

//------------------------------------------------------------------------
// Sun added 2011-04-16
// �Զ�����ԭ���壨����λ������
/// Ĭ�Ͽ��ķ��ڵ�8λ
#define PCS_PVC_AUTOCLOSE_MAINTHREAD_ERROR			0x0001			// ���̵߳���
#define PCS_PVC_AUTOCLOSE_WAVEOUT_NOMEM				0x0002			// Wave����ڴ�ľ�
#define PCS_PVC_AUTOCLOSE_DRIVER_FATALERROR			0x0004			// Driver��������
#define PCS_PVC_AUTOCLOSE_CREATETHREAD_ERROR		0x0080			// ���������һ�3���߳�ʧ��, Sun added 2013-09-09
#define PCS_PVC_AUTOCLOSE_HIGH_TRANSFAIL			0x0010			// ת��ʧ���ʹ���
#define PCS_PVC_AUTOCLOSE_OPENDEV_WAITTO			0x0020			// ���豸�ȴ���ʱ
#define PCS_PVC_AUTOCLOSE_CLOSEDEV_WAITTO			0x0040			// �ر��豸�ȴ���ʱ

// Sun added 2013-09-09
#define PCS_MAX_CREATETHREADFAIL					3				// ����̴߳�������ʧ�ܴ���

//------------------------------------------------------------------------
// Agent Status Define
#define PCS_AgentAfterCallWork			100
#define PCS_AgentLogin					101
#define PCS_AgentLogout					102
#define PCS_AgentNotReady				103
#define PCS_AgentReady					104
#define PCS_AgentOtherWork				105
#define PCS_AgentInService				106
#define PCS_AgentBusy					107

//------------------------------------------------------------------------
// Other constants
#define DEF_LD_PARAM_FROM_INI		0
#define DEF_LD_PARAM_FROM_DB		1

#define DEF_LD_GRP_DEFAULT_ET		0
#define DEF_LD_GRP_ET_FILE			255
//------------------------------------------------------------------------


//------------------------------------------------------------------------
// Sun added 2007-01-16
// �������ݸ�ʽ��������
/// ����������ָ���
#define DEF_IVR_TEST_FIELD_DELIMIT	";"
/// ���Ա���ַ���
#define DEF_IVR_TEST_FLAG			":IVRTestFlag"

/// ����¼CDR
#define DEF_IVR_TEST_NOCDR			":NOCDR"

/// ����ʱ��
#define DEF_IVR_TEST_SET_TIME		":Time="

/// ��������
#define DEF_IVR_TEST_SET_ANI		":ANI="

/// ���Ա���
#define DEF_IVR_TEST_SET_DNIS		":DNIS="
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Sun added 2010-12-01 [A]
/// DTMF�ַ���
#define DEF_IVR_DTMF_CHARSET		"0123456789*#,;&!abcdABCD"

#define DEF_IVR_PLAY_LONGPAUSE		'-'				// 500ms
#define DEF_IVR_PLAY_SHORTPAUSE		','				// 200ms

#define DEF_IVR_DIAL_LONGPAUSE		'+'				// �Ӻţ�1000ms
#define DEF_IVR_DIAL_MIDPAUSE		'.'				// �㣨��ţ���500ms
#define DEF_IVR_DIAL_SHORTPAUSE		':'				// ð�ţ�200ms
//------------------------------------------------------------------------

//---------------------------------------------------------------------
// IVR Group
struct SIVR_Group
{
	bool Enabled;						// Switch
	bool NeedCTI;						// CTI Switch
	_bstr_t Caption;					// Title
	US StartCH;							// Start Channel
	US EndCH;							// End Channel
	UC IVRProgramID;					// Current Program ID
	_bstr_t IVRFileName;				// IVR File Name
	bool blnOBOnly;						// Only used for OB Call
	bool blnBindFaxResource;			// ������Դ�Ƿ���Ĭ��Ϊtrue
	DWORD dwRoutePointID;				// Tapi CTI RoutePointID
	UC bytCustID;						// Sun added 2012-09-08, �⻧ID
};

//------------------------------------------------------------------------
// PerfectCall Call Analysis Data Structure
struct SFreq_T{
	int   freq;          // frequency Hz
	int   deviation;     // deviation in Hz
};

struct SState_T{
	int   time;          // time in 10ms
	int   deviation;     // deviation in ms
};

struct STone_T{
	char *		str;        // informational string
	int			tid;        // tone id
	SFreq_T		freq1;      // frequency 1
	SFreq_T		freq2;      // frequency 2
	SState_T	on;         // on time
	SState_T	off;        // off time
	int			repcnt;     // repitition count
};

//---------------------------------------------------------------------
// ϵͳ����״̬
enum ENUM_SYSTEM_STATUS
{
	SYS_DEAD = 0,					// û������ 
	SYS_STARTING,					// ��������
	SYS_STARTED,					// ��������
	SYS_ERROR,						// ����
	SYS_CLOSING,					// ���ڹر�
	SYS_PENDING,					// ����
};

//---------------------------------------------------------------------
// ȱʡ����״̬(�ڵ���)
enum ENUM_LINE_STATUS
{
	LN_IDLE = 0,					// ����, ����, ��ȡ���к���
	LN_STARTING,					// ��������
	LN_PLAY,						// ����״̬
	LN_SPEECH,						// ͨ��״̬
	LN_RECORD = 9,					// ¼��״̬

	LN_PLAY_WELCOME = 10,			// ���Ż�ӭ��, ��ʾ�û�ֱ���ֻ����룬����벦0
	LN_GET_STATIONNO,				// ��ȡ�ֻ�����, 0 ���� ����Ա����ʼ��ʱ��
	LN_PLAY_BUSY,					// ������ϯæ, ��ʾ�û���������һ���ֻ�����
	LN_PLAY_NOMSI,					// ����ϯ����, ������
	LN_PLAY_ONHOOK,					// �����˳���
	LN_GOTO_ACD,					// ת���Ŷ�ϵͳ

	LN_CALLOUT = 50,				// �Ⲧ״̬

};

//---------------------------------------------------------------------
// ϵͳ�¼�
enum ENUM_LINE_EVENT
{
	LE_CALLIN = 0,					// �绰����	
	LE_RINGS,						// ����
	LE_PLAYEND,						// ���Ž���
	LE_KEYEND,						// ��������
	LE_RECEND,						// ¼������
	LE_PROCEND,						// �������
	LE_HOOKEND,						// ժ�һ�����
	LE_QUERYEND,					// ��ѯ����
	LE_MSIACK,						// ��ϯӦ��
	LE_TIMEOUT,						// ����ʱ
	LE_DUMMY = 255,					// ���¼�
};

//---------------------------------------------------------------------
// IVR�ŵ�״̬
enum ENUM_IVRCH_STATE
{
	IVR_IDLE = 0,					// �ŵ�����
	IVR_BLOCK,						// �ŵ�����
	IVR_RUN,						// �ŵ���������
	IVR_W_ACK,						// �ȴ���ϢӦ��
	IVR_W_PLAY,						// �ȴ���������
	IVR_W_KEY,						// �ȴ���������
	IVR_W_RECORD,					// �ȴ�¼������
	IVR_W_FAX,						// �ȴ��������
	IVR_W_SWITCH,					// �ȴ�ת��
	IVR_W_ANSWER,					// �ȴ�����
	IVR_ERROR = 255					// �ŵ�����
};

//---------------------------------------------------------------------
// �ⲿ����
extern SIVR_Group	PGroupInfo[MAX_IVR_GROUP];		// �ŵ�����Ϣ
extern CString		PChannelDN[MAX_IVR_CHANNEL];	// �ŵ���Ϣ

extern CIVRGroup	*PIVRGroup[MAX_IVR_GROUP];		// �ŵ���ָ��
extern CIVRChannel	*PIVRCH[MAX_IVR_CHANNEL];		// �ŵ�ָ��

// Sun added 2006-02-20
extern BOOL			gb_Enable2DiscDTone;			// ����ڶ�˫��Ƶ�Ҷ���
extern BOOL			gb_Enable2DiscSTone;			// ����ڶ�����Ƶ�Ҷ���

extern STone_T		gb_Tone[MAX_CHANNEL_TONES];		// �绰ϵͳ������

extern CString		gb_FN_VoxPath;					// ϵͳ����Ŀ¼
extern CString		gb_FN_RecPath;					// ϵͳ����Ŀ¼
extern CString		gb_FN_TempPath;					// ϵͳ��ʱĿ¼

extern int			gb_VoiceVolume;					// ͨ��������С  -10 -- +10
extern int			gb_RingsPreOffhook;				// ժ��ǰ�������

// Sun added 2009-06-27
extern	BOOL		g_blnAnalogAsynDivert;			// �Ƿ�����첽ת��

// Sun added 2003-04-28
extern BOOL			gb_NT_UserGlobalCall;			// �Ƿ�ʹ��Global Call
extern CString		gb_NT_Protocol;					// ���ʽ
extern BOOL			gb_NT_IncludeD41;				// �Ƿ����D41��
extern BOOL			gb_NT_CaptureABSignal;			// �Ƿ����AB����
extern UINT			gb_NT_ABS_Disconnect;			// AB���� - Disconnect

extern UL			glngInvokeID;

// Sun added 2006-10-01
extern CFaxManager	gb_FaxManager;


#ifdef CISCO_CCM

extern HLINEAPP		gb_hLineApp;					// TAPI���
extern DWORD		gb_dwgTAPIVersion;				// TAPI�汾
extern UL			gb_lMngrThread;					// To post event messages to central manager
extern HANDLE		gb_hLineEvent;					// TAPI�¼����

// Sun added 2010-08-10���������ʧ�ܴ���ʽ
/// -1 - �Ҷϣ�1 - �����Ŷӣ�2 - ����Ӧ����3 - ���Ե���ת��
extern int			gb_nSetupConferenceFailedMethod;

// Sun added 2010-07-31��CTI·�ɵ���󲢷�������
extern int			gb_nRoutePointMaxConcurrentCall;

#endif

extern DWORD		gb_lngRoutePointCheckTimer;		// Sun added 2008-05-29, RoutePoint�豸�����Լ����ڣ��룩

#define GET_INVOKE_ID								(++glngInvokeID) % 0x999999 + 1

// Sun added 2011-04-16
// �Զ����أ�λ������
extern UL			gb_lngAutoCloseFlag;			// Ĭ��ֵ0x00FF

// Sun added 2011-04-25
extern int			gb_nMW_TransFail_Calls;			// ͳ��ת��ʧ���ʵ��������ת�ӵ绰������Ĭ��20��ȡֵ��Χ[5..200]
extern int			gb_nTH_MaxTransFailRate;		// �����ת��ʧ�������ֵ���ٷֱȣ���Ĭ��30��ȡֵ��Χ[1..100]

// Sun added 2013-09-09
extern int			gb_nMW_CreateOnHookThread;		// ͳ�����������һ��߳�ʧ�ܵĴ�����Ĭ��0��ȡֵ��Χ[0..3]

long sig_hdlr( unsigned long );

//------------------------------------------------------------------------
#endif  //End #ifndef _GLOBAL_H
//------------------------------------------------------------------------
