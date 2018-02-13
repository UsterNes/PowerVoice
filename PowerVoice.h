// PowerVoice.h : main header file for the POWERVOICE application
//

#if !defined(AFX_POWERVOICE_H__1FD692BE_FC40_4F6A_A604_AAF5744CE042__INCLUDED_)
#define AFX_POWERVOICE_H__1FD692BE_FC40_4F6A_A604_AAF5744CE042__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "stdafx.h"
#include "resource.h"       // main symbols

#include "_Global.h"		// Global Definition
#include "ThreadProcessGCEvents.h"
#include "HCentralManager.h"
#include "ThreadSysMain.h"
#include "..\CommonClass\MessageQ.h"
#include "..\CommonClass\MySysInfo.h"
#include "ProductLicense.h"

// For Function: SetMenuStatus
// nStatus:
//		0 -- Initializing (Not Ready)
//		1 -- Stoped
//		2 -- During Loading IVR Program
//		3 -- During Start/Stop/ReStart Group(s)
//	    4 -- Started
//		9 -- No Current Group
#define MAIN_FRM_STATUS_INIT		0
#define MAIN_FRM_STATUS_STOP		1
#define MAIN_FRM_STATUS_LOAD		2
#define MAIN_FRM_STATUS_RUN			3
#define MAIN_FRM_STATUS_START		4
#define MAIN_FRM_STATUS_NOCUR		9

// Definition of Icon Index
#define	ICON_Error					0
#define	ICON_Stop					1
#define	ICON_Ready					2
#define	ICON_Start					3

// Definition of Status Bar Index
#define STATUS_BAR_Operation		0
#define STATUS_BAR_SYS				1
#define STATUS_BAR_DB				2
#define STATUS_BAR_COMM				3
#define STATUS_BAR_HARDWARE			4
#define STATUS_BAR_PROGRESS			5

// Sun added 2013-04-25
#ifdef SMARTAC_LICENSE
#define THEAPP_NAME					"PowerVoiceServer"
#else
#define THEAPP_NAME					"PowerVoice Server"
#endif

/////////////////////////////////////////////////////////////////////////////
// CPowerVoiceApp:
// See PowerVoice.cpp for the implementation of this class
//

class CPowerVoiceApp : public CWinApp
{
public:
	CPowerVoiceApp();

	// Sun added 2004-11-20
	// Product License Object
	CProductLicense m_License;
	CString m_strThisMacAddress;			// ����MAC��ַ
	CString	m_strThisHDSerialNumber;		// ����Ӳ�����к�

	// System Infomation Object
	CMySysInfo				m_SysInfo;

	UC		m_bytSiteID;					// Sun added 2012-09-08
	UC		m_nSysNo;						// ϵͳ����
	BOOL	m_blnChannelRichRefresh;		// �Ƿ�ͨ������ˢ����Ļ������ֻ�ڽ�ͨ�͹Ҷ�ʱˢ��
	BOOL	m_blnAutoChLogout;				// �Ƿ��Զ��˳���¼
	CString	m_sAppPath;						// ϵͳ����·��
	CString	m_sVersionData;					// Version Info
	CString m_sFileVersion;					// Sun added 2009-01-09, File Version
	UINT	m_nLOG;							// ϵͳ��־����
	HANDLE	m_hPROID;						// Only start a application	at one time
	UC m_nIVRBoardStart;					// Start # of IVR Board
	UC m_nDTIBoardStart;					// Start # of DTI Resource Board��Sun added 2012-05-24
	UC m_nFaxBoardStart;					// Start # of Fax Resource Board��Sun added 2012-05-24
	UC m_nIVRChannelSPAN;					// Continuous # of IVR Channels��Sun added 2012-06-19
	BOOL m_blnIVRFaxAlternant;				// Whether IVR & FAX Spans apear alternantly��Sun added 2012-06-19
	UC m_nIVRGroups;						// Amount of IVR Groups
	US m_nIVRChannels;						// Amount of IVR Channels
	US m_nMaxSoundTime;						// Maximum time of a sound file
	UL m_nMaxCallLength;					// Maximum time of a call
	US m_nMaxWaitTime;						// Maximum time of waiting in Queue
	UC m_nMaxTryTimes;						// Maximum atempt times when entry
	UC m_nCallTransferMaxTryTimes;			// TAPI�������ʧ������Դ���
	US m_nCTITransferDelay;					// CTIת����ʱ(ms)
	US m_nCTIOnHookDelay;					// CTI�һ���ʱ(ms)
	long m_nFaxDiscDelay;					// FAXԶ�˹Ҷ���ʱ���룩
	UC m_bytCTIAnsyOnHook;					// Sun added 2013-02-18, CTI�һ��첽�һ�[1, 0..1]
	
	// Sun added 2012-05-07
	int m_nMaxBeforeHookOnSteps;			// �һ�ǰ������ڵ���

	// Sun added 2006-10-01
	US m_nMaxFaxTime;						// Maximum time of FAX Receiving or Sending

	// Sun added 2004-11-20
	UINT m_nDRMQLength;						// DataRecorder��Ϣ���г���
	long m_hBoardHandles[MAX_IVR_GROUP];	// Handles of IVR boards
	US m_nIVRBoardCount;					// Total IVR boards
	int m_nAnalogFlashTM;					// Flash����ʱ��(*10 ms)

	//----------------------------------------------------------------
	int m_nCallAnswerDelay;					// ͨ��Ӧ����ʱ
	int m_nChannelRestartTime;				// ͨ����λʱ��
	BOOL m_blnAutoCalcCHRT;					// �Զ��������ʱ��ͨ����λʱ�䣬2007-11-04

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	BOOL SetCustProfilePath();
	BOOL ResetProfilePath();

// System upgraded 2011-12-01 [Tag: ANTIDECEIT]
#ifdef PICADEF_ENABLE_ANTIDECEIT

	// ����թ��־�Ƿ񼤻�
	BOOL			m_bANDT_Avtive;
	// ����թ���м�����
	int				m_nANDT_CallCounter;

	// ��ȡ����թ��־
	void GetAntiDeceitEnableFlag();
	// ���÷���թ��־
	void SetAntiDeceitEnableFlag(BOOL f_blnSW);

#endif

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPowerVoiceApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CPowerVoiceApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int				InitSystemBuffer();
	void			UpdateVersionInformation(LPCTSTR lpszModuleName);

	TCHAR m_sDefaultProfilePath[_MAX_EXT];
	TCHAR m_sCustProfilePath[_MAX_EXT];
};

extern CPowerVoiceApp theApp;
extern CThreadSysMain			g_thSystemMain;
extern CFastMessageQ			g_MQCTIOnHookEvents;

#ifdef INTEL_DIALOGIC
extern CThreadProcessGCEvents	g_thProcessGCEvents;
#endif

#ifdef CISCO_CCM
extern CHCentralManager			g_thProcessTAPIEvents;
#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POWERVOICE_H__1FD692BE_FC40_4F6A_A604_AAF5744CE042__INCLUDED_)
