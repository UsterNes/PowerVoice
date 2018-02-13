// PageSystem.cpp : implementation file
//

#include "stdafx.h"
#include "PowerVoice.h"
#include "PageSystem.h"
#include "ConfigBox.h"
#include "MainFrm.h"
#include "BrowsePath.h"
#include "_CommonFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageSystem property page

IMPLEMENT_DYNCREATE(CPageSystem, CPropertyPage)

CPageSystem::CPageSystem() : CPropertyPage(CPageSystem::IDD)
, m_blnAutoCalcCHRT(FALSE)
, m_bNeedRestart(false)
{
	//{{AFX_DATA_INIT(CPageSystem)
	m_sys_dbserver = _T("");
	m_sys_dbname = _T("");
	m_sys_dbuserid = _T("");
	m_sys_dbpassword = _T("");
	m_sys_ctiport = 0;
	m_sys_drport = 0;
	m_sys_recpath = _T("");
	m_sys_voxpath = _T("");
	m_sys_globalcall = FALSE;
	m_sys_DBType = -1;
	m_sys_gc_protocol = _T("");
	m_sys_ch_ans_delay = 0;
	m_sys_ch_reset_delay = 0;
	//}}AFX_DATA_INIT
	m_Tag = NULL;
	m_changed = FALSE;
	m_autoprocess = true;
	m_nLogLevel = 0;
}

CPageSystem::~CPageSystem()
{
}

void CPageSystem::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageSystem)
	DDX_Control(pDX, IDC_CMB_LOG, m_wndLogLevel);
	DDX_Control(pDX, IDC_SYS_DRIP, m_sys_drip);
	DDX_Control(pDX, IDC_SYS_SPN_RESET_DELAY, m_sys_spn_reset_delay);
	DDX_Control(pDX, IDC_SYS_SPN_ANS_DELAY, m_sys_spn_ans_delay);
	DDX_Control(pDX, IDC_SYS_CTIIP, m_sys_ctiip);
	DDX_Text(pDX, IDC_SYS_DBSERVER, m_sys_dbserver);
	DDX_Text(pDX, IDC_SYS_DBNAME, m_sys_dbname);
	DDX_Text(pDX, IDC_SYS_CTIPORT, m_sys_ctiport);
	DDX_Text(pDX, IDC_SYS_RECPATH, m_sys_recpath);
	DDX_Text(pDX, IDC_SYS_VOXPATH, m_sys_voxpath);
	DDX_Check(pDX, IDC_CHK_GC, m_sys_globalcall);
	DDX_Radio(pDX, IDC_OPT_DB_TYPE, m_sys_DBType);
	DDX_Text(pDX, IDC_SYS_DBPassword, m_sys_dbpassword);
	DDX_Text(pDX, IDC_SYS_PROTOCOL, m_sys_gc_protocol);
	DDX_Text(pDX, IDC_SYS_CH_ANS_DELAY, m_sys_ch_ans_delay);
	DDX_Text(pDX, IDC_SYS_CH_RESET_DELAY, m_sys_ch_reset_delay);
	DDX_Text(pDX, IDC_SYS_DRPORT, m_sys_drport);
	DDX_Text(pDX, IDC_SYS_DBUSERID, m_sys_dbuserid);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHK_AUTOCAL_RESETTIME, m_blnAutoCalcCHRT);
}


BEGIN_MESSAGE_MAP(CPageSystem, CPropertyPage)
	//{{AFX_MSG_MAP(CPageSystem)
	ON_BN_CLICKED(IDC_SYS_SEARCH1, OnBrowse1Click)
	ON_BN_CLICKED(IDC_SYS_SEARCH2, OnBrowse2Click)
	ON_CBN_SELCHANGE(IDC_CMB_LOG, OnSelchangeCmbLog)
	ON_BN_CLICKED(IDC_CHK_GC, OnChkGc)
	ON_BN_CLICKED(IDC_CHK_AUTOCAL_RESETTIME, OnChangeSYS_Parameters)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SYS_SPN_ANS_DELAY, OnDeltaposSysSpnAnsDelay)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SYS_SPN_RESET_DELAY, OnDeltaposSysSpnResetDelay)
	ON_EN_CHANGE(IDC_SYS_DBSERVER, OnChangeSYS_Parameters)
	ON_EN_CHANGE(IDC_SYS_DBNAME, OnChangeSYS_Parameters)
	ON_EN_CHANGE(IDC_SYS_DBUSERID, OnChangeSYS_Parameters)
	ON_EN_CHANGE(IDC_SYS_DBPassword, OnChangeSYS_Parameters)
	ON_EN_CHANGE(IDC_SYS_PROTOCOL, OnChangeSYS_Parameters)
	ON_EN_CHANGE(IDC_SYS_CTIPORT, OnChangeSYS_Parameters)
	ON_EN_CHANGE(IDC_SYS_DRPORT, OnChangeSYS_Parameters)
	ON_EN_CHANGE(IDC_SYS_VOXPATH, OnChangeSYS_Parameters)
	ON_EN_CHANGE(IDC_SYS_RECPATH, OnChangeSYS_Parameters)
	ON_EN_CHANGE(IDC_SYS_CH_ANS_DELAY, OnChangeSYS_Parameters)
	ON_EN_CHANGE(IDC_SYS_CH_RESET_DELAY, OnChangeSYS_Parameters)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_SYS_CTIIP, OnChangeSYS_Parameters)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_SYS_DRIP, OnChangeSYS_Parameters)
	ON_BN_CLICKED(IDC_OPT_DB_TYPE, OnOptDbType)
	ON_BN_CLICKED(IDC_OPT_DB_TYPE2, OnOptDbType)
	ON_BN_CLICKED(IDC_OPT_DB_TYPE3, OnOptDbType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageSystem message handlers

BOOL CPageSystem::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	// Read Setting Infomation from INI File
	Information_ReadIniFile();

	// Set Flags
	m_autoprocess = false;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageSystem::Information_ReadIniFile()
{
	char lv_path[100];

	m_autoprocess = true;

	// System table
	/// DB Section
	m_sys_DBType = pMainWindow->m_DBReader.GetDBConnectType();
	m_sys_dbserver = pMainWindow->m_DBReader.GetDataSource();
	m_sys_dbname = pMainWindow->m_DBReader.GetDBName();
	m_sys_dbuserid = pMainWindow->m_DBReader.GetUserName();
	m_sys_dbpassword = pMainWindow->m_DBReader.GetPassword();
	SetDBAccessType();

	// Initialize log level list
	int lv_nIndex, lv_nCurSel = -1;
	m_nLogLevel = theApp.m_nLOG;
	m_wndLogLevel.ResetContent();
	lv_nIndex = m_wndLogLevel.AddString("None");
	if( lv_nIndex >= 0 )
	{
		m_wndLogLevel.SetItemData(lv_nIndex, TraceNone);
		if( m_nLogLevel == TraceNone )
			lv_nCurSel = lv_nIndex;
	}
	lv_nIndex = m_wndLogLevel.AddString("Error");
	if( lv_nIndex >= 0 )
	{
		m_wndLogLevel.SetItemData(lv_nIndex, TraceError);
		if( m_nLogLevel == TraceError )
			lv_nCurSel = lv_nIndex;
	}
	lv_nIndex = m_wndLogLevel.AddString("Warning");
	if( lv_nIndex >= 0 )
	{
		m_wndLogLevel.SetItemData(lv_nIndex, TraceWarn);
		if( m_nLogLevel == TraceWarn )
			lv_nCurSel = lv_nIndex;
	}
	lv_nIndex = m_wndLogLevel.AddString("Normal");
	if( lv_nIndex >= 0 )
	{
		m_wndLogLevel.SetItemData(lv_nIndex, TraceInfo);
		if( m_nLogLevel == TraceInfo )
			lv_nCurSel = lv_nIndex;
	}
	lv_nIndex = m_wndLogLevel.AddString("Debug");
	if( lv_nIndex >= 0 )
	{
		m_wndLogLevel.SetItemData(lv_nIndex, TraceDebug);
		if( m_nLogLevel == TraceDebug )
			lv_nCurSel = lv_nIndex;
	}
	lv_nIndex = m_wndLogLevel.AddString("Detail");
	if( lv_nIndex >= 0 )
	{
		m_wndLogLevel.SetItemData(lv_nIndex, TraceDetail);
		if( m_nLogLevel == TraceDetail )
			lv_nCurSel = lv_nIndex;
	}
	if( lv_nCurSel >= 0 )
		m_wndLogLevel.SetCurSel(lv_nCurSel);

	// Communication Section
	m_sys_ctiport = pMainWindow->m_param_ctiport;
	m_sys_ctiip.SetWindowText(pMainWindow->m_param_ctiip);
	m_sys_drport = pMainWindow->m_param_drport;
	m_sys_drip.SetWindowText(pMainWindow->m_param_drip);
	
	// Protocol Section
	m_sys_gc_protocol = gb_NT_Protocol;
	m_sys_globalcall = gb_NT_UserGlobalCall;
	GetDlgItem(IDC_SYS_PROTOCOL)->EnableWindow(gb_NT_UserGlobalCall);

	// Others
	memset( lv_path, 0x00, sizeof(lv_path) );
	strC2Dos_Path((const char *)gb_FN_VoxPath, lv_path);
	m_sys_voxpath = lv_path;
	memset( lv_path, 0x00, sizeof(lv_path) );
	strC2Dos_Path((const char *)gb_FN_RecPath, lv_path);
	m_sys_recpath = lv_path;
	m_sys_spn_ans_delay.SetBase(100);
	m_sys_spn_ans_delay.SetRange(0, 5000);
	m_sys_spn_ans_delay.SetPos(theApp.m_nCallAnswerDelay);
	m_sys_ch_ans_delay = theApp.m_nCallAnswerDelay;
	m_sys_spn_reset_delay.SetBase(30);
	m_sys_spn_reset_delay.SetRange(0, 1500);
	m_sys_spn_reset_delay.SetPos(theApp.m_nChannelRestartTime);
	m_sys_ch_reset_delay = theApp.m_nChannelRestartTime;

	// Sun added 2007-11-04
	/// 是否自动计算空闲时的通道复位时间
	m_blnAutoCalcCHRT = theApp.m_blnAutoCalcCHRT;

	UpdateData(FALSE);
	m_changed = FALSE;
}

// Save Setting
/// true - save and apply
/// false - save only
BOOL CPageSystem::Information_WriteNewSetting(bool f_apply)
{
	int		lv_loop;
	char	szTmp[32];
	char	lv_path[100];

	m_bNeedRestart = FALSE;

	if( this->m_hWnd == NULL )
		return TRUE;

	// Load Waiting Cursor
	HCURSOR hOldCur = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));

	UpdateData(TRUE);

	// System Table
	if( m_changed )
	{
		/// DB Section
		theApp.WriteProfileInt("SYSEN", "SYSDBACCESSTYPE", m_sys_DBType);
		theApp.WriteProfileString("SYSEN","SYSDBSERVER",m_sys_dbserver);
		theApp.WriteProfileString("SYSEN","SYSDBNAME",m_sys_dbname);
		theApp.WriteProfileString("SYSEN","SYSDBUserName",m_sys_dbuserid);
		theApp.WriteProfileString("SYSEN","SYSDBPASSWORD",m_sys_dbpassword);
		theApp.WriteProfileInt("SYSEN", "LOGLEVEL", m_nLogLevel);

		/// Communication Section
		memset(szTmp, 0x00, sizeof(szTmp));
		m_sys_ctiip.GetWindowText(szTmp, sizeof(szTmp));
		theApp.WriteProfileString("SOCKETINFO","CTIIP", szTmp);
		theApp.WriteProfileInt("SOCKETINFO", "CTIPORT", m_sys_ctiport);
		memset(szTmp, 0x00, sizeof(szTmp));
		m_sys_drip.GetWindowText(szTmp, sizeof(szTmp));
		theApp.WriteProfileString("SOCKETINFO","SYSDBRecorderIP",szTmp);
		theApp.WriteProfileInt("SOCKETINFO", "SYSDBRecorderPort", m_sys_drport);
		
		/// Protocol Section
		theApp.WriteProfileInt("SYSEN", "NT_GLOBALCALL", (m_sys_globalcall ? 1: 0));
		theApp.WriteProfileString("SYSEN", "NT_PROTOCOL", m_sys_gc_protocol);

		/// Others
		memset( lv_path, 0x00, sizeof(lv_path) );
		strDos2C_Path((const char *)m_sys_voxpath, lv_path);
		m_sys_voxpath = lv_path;
		theApp.WriteProfileString("SYSDIR", "FN_VoxPath", m_sys_voxpath);
		memset( lv_path, 0x00, sizeof(lv_path) );
		strDos2C_Path((const char *)m_sys_recpath, lv_path);
		m_sys_recpath = lv_path;
		theApp.WriteProfileString("SYSDIR", "FN_RecPath", m_sys_recpath);
		theApp.WriteProfileInt("WORKINFO", "CallAnswerDelay", m_sys_ch_ans_delay);
		theApp.WriteProfileInt("WORKINFO", "ChannelRestartTime", m_sys_ch_reset_delay);
				
		/// LOG Section
		theApp.m_nLOG = m_nLogLevel;
		SetTraceLevel(theApp.m_nLOG);

		/// DB Section
		pMainWindow->m_DBReader.SetDBConnectType( m_sys_DBType );
		pMainWindow->m_DBReader.SetDataSource( m_sys_dbserver );
		pMainWindow->m_DBReader.SetDBName( m_sys_dbname );
		pMainWindow->m_DBReader.SetUserName( m_sys_dbuserid );
		pMainWindow->m_DBReader.SetPassword( m_sys_dbpassword );

		theApp.m_nChannelRestartTime = m_sys_ch_reset_delay;
		theApp.m_nCallAnswerDelay = m_sys_ch_ans_delay;

		// Sun added 2007-11-04
		/// 是否自动计算空闲时的通道复位时间
		theApp.m_blnAutoCalcCHRT = m_blnAutoCalcCHRT;
		theApp.WriteProfileInt("WORKINFO", "IsAutoCalcChannelRestartTime", theApp.m_blnAutoCalcCHRT);

		if( f_apply )
		{
			/// Others
			gb_FN_VoxPath = AddTailBackslashToPath(m_sys_voxpath);
			gb_FN_RecPath = AddTailBackslashToPath(m_sys_recpath);

			/// Communication Section
			if( pMainWindow->m_param_ctiport != m_sys_ctiport )
			{
				pMainWindow->m_param_ctiport = m_sys_ctiport;
				m_bNeedRestart = TRUE;
			}
			memset(szTmp, 0x00, sizeof(szTmp));
			m_sys_ctiip.GetWindowText(szTmp, sizeof(szTmp));
			if( pMainWindow->m_param_ctiip.CompareNoCase(szTmp) != 0 )
			{
				pMainWindow->m_param_ctiip = szTmp;
				m_bNeedRestart = TRUE;
			}

			if( pMainWindow->m_param_drport != m_sys_drport )
			{
				pMainWindow->m_param_drport = m_sys_drport;
				m_bNeedRestart = TRUE;
			}
			memset(szTmp, 0x00, sizeof(szTmp));
			m_sys_drip.GetWindowText(szTmp, sizeof(szTmp));
			if( pMainWindow->m_param_drip.CompareNoCase(szTmp) != 0 )
			{
				pMainWindow->m_param_drip = szTmp;
				m_bNeedRestart = TRUE;
			}

			/// Protocol Section
			if( gb_NT_Protocol != m_sys_gc_protocol )
			{
				gb_NT_Protocol = m_sys_gc_protocol;
				m_bNeedRestart = TRUE;
			}
			if( gb_NT_UserGlobalCall != m_sys_globalcall )
			{
				gb_NT_UserGlobalCall = m_sys_globalcall;
				m_bNeedRestart = TRUE;
			}

			for( lv_loop = 0; lv_loop < theApp.m_nIVRChannels; lv_loop++ )
			{
				if( PIVRCH[lv_loop] != NULL )
				{
					PIVRCH[lv_loop]->NF_SetChannelRestartTime(theApp.m_nChannelRestartTime);
					PIVRCH[lv_loop]->NF_SetCallAnswerDelay(theApp.m_nCallAnswerDelay);
				}
			}
		}
		m_changed = FALSE;
	}

	// Resume Cursor
	::SetCursor(hOldCur);
	
	return TRUE;
}

void CPageSystem::OnSelchangeCmbLog() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	int lv_nIndex = m_wndLogLevel.GetCurSel();
	if( lv_nIndex >= 0 )
	{
		UINT lv_nTmpLog = (UINT)m_wndLogLevel.GetItemData(lv_nIndex);
		if( lv_nTmpLog != m_nLogLevel )
		{
			m_nLogLevel = lv_nTmpLog;
			OnChangeSYS_Parameters();
		}
	}
}

void CPageSystem::OnChangeSYS_Parameters() 
{
	OnChangeSYS_Parameters(0);
}

void CPageSystem::OnChangeSYS_Parameters(NMHDR* pNMHDR, LRESULT* pResult)
{
	OnChangeSYS_Parameters(0);

	*pResult = 0;
}

void CPageSystem::OnChangeSYS_Parameters(UINT nControlID) 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if( m_autoprocess )
		return;

	m_changed = TRUE;

	// Enable Apply Button
	if( m_Tag )
		((CConfigBox *)m_Tag)->SetApplyEnableState(TRUE);
}

void CPageSystem::OnBrowse1Click()
{
	UpdateData(TRUE);
	
	CBrowsePath lv_dlg;
	
	lv_dlg.m_szCurPath = m_sys_voxpath;
	if( lv_dlg.DoModal() == IDOK )
	{
		m_sys_voxpath = AddTailBackslashToPath(lv_dlg.m_szCurPath);
		OnChangeSYS_Parameters();		
	}

	UpdateData(FALSE);
}

void CPageSystem::OnBrowse2Click()
{
	UpdateData(TRUE);
	
	CBrowsePath lv_dlg;
	
	lv_dlg.m_szCurPath = m_sys_recpath;
	if( lv_dlg.DoModal() == IDOK )
	{
		m_sys_recpath = AddTailBackslashToPath(lv_dlg.m_szCurPath);
		OnChangeSYS_Parameters();
	}

	UpdateData(FALSE);
}

void CPageSystem::OnChkGc() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	OnChangeSYS_Parameters();
	GetDlgItem(IDC_SYS_PROTOCOL)->EnableWindow(m_sys_globalcall);	
}

void CPageSystem::SetDBAccessType()
{
	CString lv_sCaption;

	if( m_sys_DBType == 0 )
	{
	// OLE DB
		lv_sCaption.LoadString( IDS_CAP_SET_DBSERVER );
		SetDlgItemText(IDC_SYS_LBL_DB_SERVER, lv_sCaption);
		GetDlgItem(IDC_SYS_LBL_DB_SERVER)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SYS_DBSERVER)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SYS_LBL_DB_DBNAME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SYS_DBNAME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SYS_LBL_DB_USERID)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SYS_LBL_DB_USERID)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SYS_DBUSERID)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SYS_LBL_DB_PWD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SYS_DBPassword)->ShowWindow(SW_HIDE);
	}
	else if( m_sys_DBType == 1 )
	{
	// ODBC
		lv_sCaption.LoadString( IDS_CAP_SET_DSN );
		SetDlgItemText(IDC_SYS_LBL_DB_SERVER, lv_sCaption);
		GetDlgItem(IDC_SYS_LBL_DB_SERVER)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SYS_DBSERVER)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SYS_LBL_DB_DBNAME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SYS_DBNAME)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SYS_LBL_DB_USERID)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SYS_DBUSERID)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SYS_LBL_DB_PWD)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_SYS_DBPassword)->ShowWindow(SW_SHOW);
	}
	else
	{
	// None
		GetDlgItem(IDC_SYS_LBL_DB_SERVER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SYS_DBSERVER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SYS_LBL_DB_DBNAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SYS_DBNAME)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SYS_LBL_DB_USERID)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SYS_DBUSERID)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SYS_LBL_DB_PWD)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SYS_DBPassword)->ShowWindow(SW_HIDE);
	}
}

void CPageSystem::OnOptDbType() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	SetDBAccessType();
	OnChangeSYS_Parameters();
}

void CPageSystem::OnDeltaposSysSpnAnsDelay(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	pNMUpDown->iDelta *= m_sys_spn_ans_delay.GetBase();

	*pResult = 0;
}

void CPageSystem::OnDeltaposSysSpnResetDelay(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	pNMUpDown->iDelta *= m_sys_spn_reset_delay.GetBase();

	*pResult = 0;
}
