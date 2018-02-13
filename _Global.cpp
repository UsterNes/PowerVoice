///////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		_Global.cpp
// ��;������	ȫ�ֱ�������������
// ���ߣ�		Tony Sun
// ����ʱ�䣺	2000/05/05
// ����޸�ʱ�䣺2006/10/01
// ��Ȩ��Ϣ��	Copyright (C) 1999 - 2007 PicaSoft
// ��ע��
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
#include "stdafx.h"

#include "_Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//----------------------------------------------------------------------------------------------
SIVR_Group	PGroupInfo[MAX_IVR_GROUP];		// �ŵ�����Ϣ
CString		PChannelDN[MAX_IVR_CHANNEL];	// �ŵ���Ϣ

CIVRGroup	*PIVRGroup[MAX_IVR_GROUP];		// �ŵ���ָ��
CIVRChannel	*PIVRCH[MAX_IVR_CHANNEL];		// �ŵ�ָ��

CString		gb_FN_VoxPath;					// ϵͳ����Ŀ¼
CString		gb_FN_RecPath;					// ϵͳ����Ŀ¼
CString		gb_FN_TempPath;					// ϵͳ��ʱĿ¼

int			gb_VoiceVolume;					// ͨ��������С  -10 -- +10
int			gb_RingsPreOffhook;				// ժ��ǰ�������

// Sun added 2009-06-27
BOOL		g_blnAnalogAsynDivert;			// �Ƿ�����첽ת��

// Sun added 2003-04-28
CString		gb_NT_Protocol;					// ���ʽ
BOOL		gb_NT_UserGlobalCall;			// �Ƿ�ʹ��Global Call
BOOL		gb_NT_IncludeD41;				// �Ƿ����D41��
BOOL		gb_NT_CaptureABSignal;			// �Ƿ����AB����
UINT		gb_NT_ABS_Disconnect;			// AB���� - Disconnect

UL			glngInvokeID;

// Sun added 2006-10-01
CFaxManager	gb_FaxManager;


#ifdef CISCO_CCM

HLINEAPP	gb_hLineApp;					// TAPI���
DWORD		gb_dwgTAPIVersion;				// TAPI�汾
UL			gb_lMngrThread;					// To post event messages to central manager
HANDLE		gb_hLineEvent;					// TAPI�¼����

// Sun added 2010-08-10���������ʧ�ܴ���ʽ
/// -1 - �Ҷϣ�1 - �����Ŷӣ�2 - ����Ӧ����3 - ���Ե���ת��
int			gb_nSetupConferenceFailedMethod;

// Sun added 2010-07-31��CTI·�ɵ���󲢷�������
int			gb_nRoutePointMaxConcurrentCall;

#endif

DWORD		gb_lngRoutePointCheckTimer;		// Sun added 2008-05-29, RoutePoint�豸�����Լ����ڣ��룩

// Sun added 2011-04-16
// �Զ����أ�λ������
UL			gb_lngAutoCloseFlag;			// Ĭ��ֵ0x00FF

// Sun added 2011-04-25
int			gb_nMW_TransFail_Calls;			// ͳ��ת��ʧ���ʵ��������ת�ӵ绰������Ĭ��20��ȡֵ��Χ[5..200]
int			gb_nTH_MaxTransFailRate;		// �����ת��ʧ�������ֵ���ٷֱȣ���Ĭ��30��ȡֵ��Χ[1..100]

// Sun added 2013-09-09
int			gb_nMW_CreateOnHookThread;		// ͳ�����������һ��߳�ʧ�ܵĴ�����Ĭ��0��ȡֵ��Χ[0..3]

//----------------------------------------------------------------------------------------------

// Sun added 2006-02-20
BOOL		gb_Enable2DiscDTone;			// ����ڶ�˫��Ƶ�Ҷ���
BOOL		gb_Enable2DiscSTone;			// ����ڶ�����Ƶ�Ҷ���

//----------------------------------------------------------------------------------------------
// ������������
STone_T				gb_Tone[MAX_CHANNEL_TONES] = 
	{
		{{"TID_DIAL_LCL"},   {TID_DIAL_LCL},   {400,125},  {400,125}, {  0,  0}, {  0,  0}, 0},
		{{"TID_DIAL_INTL"},  {TID_DIAL_INTL},  {402,125},  {402,125}, {  0,  0}, {  0,  0}, 0},
		{{"TID_DIAL_XTRA"},  {TID_DIAL_XTRA},  {401,125},  {401,125}, {  0,  0}, {  0,  0}, 0},
		{{"TID_BUSY1"},      {TID_BUSY1},      {400, 24},  {400, 24}, { 39, 16}, { 39, 16}, 3},
		{{"TID_BUSY2"},      {TID_BUSY2},      {400, 24},  {400, 24}, { 53, 30}, { 53, 30}, 1},
		{{"TID_RNGBK1"},	 {TID_RNGBK1},     {450,150},  {  0,  0}, {130,125}, {580,485}, 0},
		{{"TID_RNGBK2"},	 {TID_RNGBK2},     {450,150},  {450,150}, {130,125}, {580,485}, 0},
		{{"TID_FAX1"},       {TID_FAX1},       {2150,150}, {  0,  0}, { 25,-25}, {  0,  0}, 0},
		{{"TID_FAX2"},       {TID_FAX2},       {1100, 50}, {  0,  0}, { 25,-25}, {  0,  0}, 0},
		{{"TID_DISCON_DT"},  {TID_DISCON_DT},  {450, 32},  {450, 32}, { 32,  2}, { 32,  2}, 1},
		{{"TID_DISCON_ST"},  {TID_DISCON_ST},  {450, 20},  {38,  5},  { 36,  5}, {  1,  0}, 0},
		{{"TID_DISCON_DT2"}, {TID_DISCON_DT2}, {450, 32},  {450, 32}, { 32,  2}, { 32,  2}, 1},
		{{"TID_DISCON_ST2"}, {TID_DISCON_ST2}, {450, 40},  { 50,  8}, { 50,  8}, {  1,  0}, 0}
	};
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
