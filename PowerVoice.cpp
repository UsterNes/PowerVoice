// PowerVoice.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PowerVoice.h"

#include "HyperLink.h"
#include "BtnST.h"

#include "MainFrm.h"
#include "PowerVoiceDoc.h"
#include "PowerVoiceView.h"
#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Omit the following line if test with db
//#define PCS_LOCAL_APP_TEST

/////////////////////////////////////////////////////////////////////////////
// CPowerVoiceApp
BEGIN_MESSAGE_MAP(CPowerVoiceApp, CWinApp)
	//{{AFX_MSG_MAP(CPowerVoiceApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPowerVoiceApp construction

CPowerVoiceApp::CPowerVoiceApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_sVersionData = _T("Unknown Version");
	m_sFileVersion = _T("1.0.0.1");				// Sun added 2009-01-09
	m_bytSiteID = 0;							// Sun added 2012-09-08
	m_nSysNo = 0;
	m_blnChannelRichRefresh = TRUE;
	m_blnAutoChLogout = TRUE;
	m_sAppPath = _T("");
	m_nLOG = 0;
	m_hPROID = 0;
	m_nIVRBoardStart = 1;
	m_nDTIBoardStart = 1;					// Start # of DTI Resource Board，Sun added 2012-05-24
	m_nFaxBoardStart = 0;					// Start # of Fax Resource Board，Sun added 2012-05-24
	m_nIVRChannelSPAN = 0;					// Continuous # of IVR Channels，Sun added 2012-06-19
	m_blnIVRFaxAlternant = FALSE;			// Whether IVR & FAX Spans apear alternantly，Sun added 2012-06-19
	m_nIVRGroups = 1;
	m_nIVRChannels = 1;
	m_nMaxSoundTime = 180;
	m_nMaxFaxTime = 600;
	m_nMaxCallLength = 3600;
	m_nMaxWaitTime = 60;
	m_nCallAnswerDelay = 0;
	m_nCTIOnHookDelay = 0;
	m_bytCTIAnsyOnHook = 1;					// Sun added 2013-02-18

	// FAX远端挂断延时（秒）
	theApp.m_nFaxDiscDelay = 0;

	m_nMaxBeforeHookOnSteps = 0;
	m_nChannelRestartTime = 0;
	m_blnAutoCalcCHRT = TRUE;
	m_nMaxTryTimes = 3;
	m_nCallTransferMaxTryTimes = 3;		// Sun added 2010-08-02 [B]
	m_nAnalogFlashTM = 50;
	m_nIVRBoardCount = 0;
	m_nDRMQLength = 64;

	m_strThisMacAddress = _T("");
	m_strThisHDSerialNumber = _T("");

	memset(m_sDefaultProfilePath, 0x00, sizeof(m_sDefaultProfilePath));
	memset(m_sCustProfilePath, 0x00, sizeof(m_sCustProfilePath));

	for( int lv_loop = 0; lv_loop < MAX_IVR_GROUP; lv_loop++ )
		m_hBoardHandles[lv_loop] = -1;

// System upgraded 2011-12-01 [Tag: ANTIDECEIT]
#ifdef PICADEF_ENABLE_ANTIDECEIT
	// 防欺诈标志是否激活
	m_bANDT_Avtive = FALSE;
	// 防欺诈呼叫计数器
	m_nANDT_CallCounter = 0;
#endif

}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPowerVoiceApp object

CPowerVoiceApp theApp;

// Global Objects
CThreadSysMain			g_thSystemMain;
CFastMessageQ			g_MQCTIOnHookEvents;

#ifdef INTEL_DIALOGIC
CThreadProcessGCEvents	g_thProcessGCEvents;
#endif

#ifdef CISCO_CCM
CHCentralManager		g_thProcessTAPIEvents;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPowerVoiceApp initialization

BOOL CPowerVoiceApp::InitInstance()
{
	HANDLE RID = OpenMutex(1, TRUE, THEAPP_NAME);
	if( RID > 0)
	{
		AfxMessageBox(IDS_MSG_E_ALREADY_RUN);
		return FALSE;
	}

	m_hPROID = CreateMutex(NULL, TRUE, THEAPP_NAME);

	// 初始化系统组件环境
#if _WIN32_WINNT >= 0x0400
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
#else
	CoInitialize(NULL);
#endif

	// CG: The following block was added by the Splash Screen component.
\
	{
\
		CCommandLineInfo cmdInfo;
\
		ParseCommandLine(cmdInfo);
\

\
		CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
\
	}

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	InitSystemBuffer();

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CPowerVoiceDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CPowerVoiceView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Main Title
	CString lv_sztile;
	lv_sztile.Format("%s %s", THEAPP_NAME, m_sVersionData);
	m_pMainWnd->SetWindowText(lv_sztile);

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CButtonST		m_OK;
	CHyperLink		m_Link;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDC_LINK, m_Link);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CPowerVoiceApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CPowerVoiceApp message handlers
BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString lv_sTMP, lv_sString;

	// Dialog Title
	lv_sString.LoadString(IDS_ABOUTME_CAPTION);
	lv_sTMP.Format("%s %s", lv_sString, THEAPP_NAME);
	SetWindowText(lv_sTMP);

	// INIT BUTTONS
	m_OK.SetIcon ( IDI_MyOK );
	m_OK.AddToolTip ( IDS_CAP_BTN_OK );
	m_OK.SetBtnCursor(IDC_HAND);

	// Version Info
	lv_sString.Format("%s %s", THEAPP_NAME, theApp.m_sVersionData);

	SetDlgItemText(IDC_VERSION, lv_sString);

	lv_sString.LoadString(IDS_MAILADDRESS);
	m_Link.SetWindowText( lv_sString );
	m_Link.SetURL ( lv_sString );
	m_Link.SetLinkCursor ( AfxGetApp()->LoadCursor ( IDC_HAND ) );
	lv_sString.LoadString(IDS_MAILTOTIP);
	m_Link.SetToolTip( lv_sString );
	m_Link.SetUnderline();
	//m_Link.SetItalic();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPowerVoiceApp::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following lines were added by the Splash Screen component.
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;

	return CWinApp::PreTranslateMessage(pMsg);
}

int CPowerVoiceApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	CoUninitialize();

	return CWinApp::ExitInstance();
}

int CPowerVoiceApp::InitSystemBuffer()
{
	int lv_loop;

	// Clear IVR Group Info
	memset(PGroupInfo, 0, sizeof(PGroupInfo));
	
	// Clear IVR Group
	for(lv_loop = 0; lv_loop < MAX_IVR_GROUP; lv_loop++)
	{
		PIVRGroup[lv_loop] = NULL;
	}

	// Clear IVR Channel
	for(lv_loop = 0; lv_loop < MAX_IVR_CHANNEL; lv_loop++)
	{
		PChannelDN[lv_loop].Empty();
		PIVRCH[lv_loop] = NULL;
	}

	// Get App.Path
	TCHAR sFilename[_MAX_PATH] = "";	
	TCHAR sDrive[_MAX_DRIVE] = "";
	TCHAR sDir[_MAX_DIR] = "";
	TCHAR sFname[_MAX_FNAME] = "";
	TCHAR sExt[_MAX_EXT] = "";

	GetModuleFileName(NULL, sFilename, _MAX_PATH);
	_tsplitpath(sFilename, sDrive, sDir, sFname, sExt);

	// Version Info
	sprintf(sExt, "%s.exe", sFname);
	UpdateVersionInformation(sExt);
	
	// Get INI profile path(App.Path & IVRS.INI)
	/// 保存系统默认配置文件/注册表位置
	strcpy(m_sDefaultProfilePath, theApp.m_pszProfileName);
	/// 应用程序指定的配置文件位置
	m_sAppPath.Format("%s%s", sDrive, sDir);
	sprintf(m_sCustProfilePath, "%sIVRS.INI", m_sAppPath);

// System upgraded 2011-12-01 [Tag: ANTIDECEIT]
#ifdef PICADEF_ENABLE_ANTIDECEIT
	// 获取防欺诈标志
	GetAntiDeceitEnableFlag();
#endif

	SetCustProfilePath();

	return 1;
}

BOOL CPowerVoiceApp::SetCustProfilePath()
{
	if( strlen(m_sCustProfilePath) > 3 )
	{
		free((void*)theApp.m_pszProfileName);
		theApp.m_pszProfileName=_tcsdup((const char*)m_sCustProfilePath);

		return TRUE;
	}

	return FALSE;
}

BOOL CPowerVoiceApp::ResetProfilePath()
{
	if( strlen(m_sDefaultProfilePath) > 3 )
	{
		free((void*)theApp.m_pszProfileName);
		theApp.m_pszProfileName=_tcsdup((const char*)m_sDefaultProfilePath);

		return TRUE;
	}

	return FALSE;
}

//-----------------------------------------------------------------
// System upgraded 2011-12-01 [Tag: ANTIDECEIT]
#ifdef PICADEF_ENABLE_ANTIDECEIT

// 获取防欺诈标志
void CPowerVoiceApp::GetAntiDeceitEnableFlag()
{
	BOOL lv_blnChanged = FALSE;
	CString lv_sTemp;

	// Default Value
	m_bANDT_Avtive = FALSE;

	if( _stricmp(theApp.m_pszProfileName, m_sDefaultProfilePath) != 0 )
	{
		lv_blnChanged = ResetProfilePath();
	}

	lv_sTemp = GetProfileString(PICADEF_ANTIDECEIT_SECTION, PICADEF_ANTIDECEIT_ENTRY_CODE, "0");
	if( lv_sTemp == _T("1") )
		m_bANDT_Avtive = TRUE;

	// 恢复原有设置
	if( lv_blnChanged )
	{
		SetCustProfilePath();
	}

	return;
}

// 设置防欺诈标志
void CPowerVoiceApp::SetAntiDeceitEnableFlag(BOOL f_blnSW)
{
	BOOL lv_blnChanged = FALSE;
	CString lv_sTemp;

	if( _stricmp(theApp.m_pszProfileName, m_sDefaultProfilePath) != 0 )
	{
		lv_blnChanged = ResetProfilePath();
	}

	m_bANDT_Avtive = f_blnSW;
	if( f_blnSW )
		lv_sTemp = _T("1");
	else
		lv_sTemp = _T("0");
	WriteProfileString(PICADEF_ANTIDECEIT_SECTION, PICADEF_ANTIDECEIT_ENTRY_CODE, lv_sTemp);

	// 恢复原有设置
	if( lv_blnChanged )
	{
		SetCustProfilePath();
	}

	return;
}

#endif
//-----------------------------------------------------------------

void CPowerVoiceApp::UpdateVersionInformation(LPCTSTR lpszModuleName)
{
	BYTE		*pDataBlock = NULL;
	DWORD FAR	*translation;
	DWORD FAR	*buffer;     
	DWORD		dwHandle;
	UINT		nBytes;
	
	// WinAPI wants non-const arguments
	LPTSTR lpszExeName = const_cast<LPTSTR>(lpszModuleName); 

	char szName[512];       // StringFileInfo data block.
	char szData[256];

	// get the size of the information block
	nBytes = (UINT)GetFileVersionInfoSize(lpszExeName, &dwHandle);

	if (nBytes)
	{
		pDataBlock = new BYTE[nBytes];

		// get the block for the version information
		if (GetFileVersionInfo(lpszExeName, dwHandle, nBytes, pDataBlock))
		{
			if (VerQueryValue(pDataBlock, "\\VarFileInfo\\Translation", (VOID FAR * FAR *)&translation, (UINT FAR *)&nBytes)) 
			{
				// The Private Build Version for this application
				wsprintf(szName, "\\StringFileInfo\\%04x%04x\\PrivateBuild", LOWORD(*translation), HIWORD(*translation));

				if (VerQueryValue(pDataBlock, szName, (VOID FAR * FAR *)&buffer, (UINT FAR *)&nBytes)) 
				{
					lstrcpy(szData, (char far *)buffer);
					m_sVersionData = szData;
					m_sVersionData.TrimRight();
					m_sVersionData.TrimLeft();
				}

				// The File Version for this application
				wsprintf(szName, "\\StringFileInfo\\%04x%04x\\FileVersion", LOWORD(*translation), HIWORD(*translation));

				if (VerQueryValue(pDataBlock, szName, (VOID FAR * FAR *)&buffer, (UINT FAR *)&nBytes)) 
				{
					lstrcpy(szData, (char far *)buffer);
					m_sFileVersion = szData;
					m_sFileVersion.TrimRight();
					m_sFileVersion.TrimLeft();
				}
			}
			else 
			{
				// sorry ...
			}
		}
		if (pDataBlock)
			delete[] pDataBlock;
	}

//-------------------------------------------------
#ifdef INTEL_DIALOGIC
	// Dialogic Compliant Voice Card

#ifndef DONGJIN
		m_sVersionData += _T(" on Dialogic");
#else
		m_sVersionData += _T(" on Dongjin");
#endif

#else
	// Cisco
#ifdef CISCO_CCM

	// Sun added 2013-03-06
#ifdef CUCM_MEDIA
		m_sVersionData += _T(" on Cisco CUCM MediaDriver");
#else
		m_sVersionData += _T(" on Cisco CCM WaveDriver");
#endif

#else
		m_sVersionData += _T(" on Cisco CME");
#endif

#endif
//-------------------------------------------------

#ifdef PVC_USE_MULTI_NODE_FIELDS
		m_sVersionData += _T("(N)");
#endif

//-------------------------------------------------
// Sun added 2007-10-28
		m_sVersionData += _T("\nSupports TTS Engine: ");
#ifdef USE_SINOVOICE_TTS
		m_sVersionData += _T("SinoVoice jTTS");
#else
		m_sVersionData += _T("iFly TTS");
#endif
//-------------------------------------------------
}
