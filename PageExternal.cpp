// PageExternal.cpp : implementation file
//

#include "stdafx.h"
#include "PowerVoice.h"
#include "PageExternal.h"
#include "ConfigBox.h"
#include "MainFrm.h"
#include ".\pageexternal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageExternal property page

IMPLEMENT_DYNCREATE(CPageExternal, CPropertyPage)

CPageExternal::CPageExternal() : CPropertyPage(CPageExternal::IDD)
, m_bNeedRestart(false)
{
	//{{AFX_DATA_INIT(CPageExternal)
	m_asr_port = 0;
	m_ext_port = 0;
	m_nTTSBufferCount = 0;
	m_nTTSBufferSize = 0;
	m_tts_port = 0;
	m_nTTSVID = 0;
	m_strTTSSN = _T("");
	//}}AFX_DATA_INIT
	m_Tag = NULL;
	m_changed = FALSE;
	m_autoprocess = true;
}

CPageExternal::~CPageExternal()
{
}

void CPageExternal::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageExternal)
	DDX_Control(pDX, IDC_SYS_TTSPITCH, m_wnd_tts_pitch);
	DDX_Control(pDX, IDC_SYS_TTSSPEED, m_wnd_tts_speed);
	DDX_Control(pDX, IDC_SYS_TTSVOLUME, m_wnd_tts_volume);
	DDX_Control(pDX, IDC_SYS_TTSIP, m_tts_ip);
	DDX_Control(pDX, IDC_SYS_EXTIP, m_ext_ip);
	DDX_Control(pDX, IDC_SYS_ASRIP, m_asr_ip);
	DDX_Control(pDX, IDC_EXT_TTS, m_chkTTS);
	DDX_Control(pDX, IDC_EXT_EXTSYS, m_chkEXT);
	DDX_Control(pDX, IDC_EXT_ASR, m_chkASR);
	DDX_Text(pDX, IDC_SYS_ASRPORT, m_asr_port);
	DDX_Text(pDX, IDC_SYS_EXTPORT, m_ext_port);
	DDX_Text(pDX, IDC_SYS_TTSBufferCount, m_nTTSBufferCount);
	DDX_Text(pDX, IDC_SYS_TTSBufferSize, m_nTTSBufferSize);
	DDX_Text(pDX, IDC_SYS_TTSPORT, m_tts_port);
	DDX_Text(pDX, IDC_SYS_TTSVID, m_nTTSVID);
	DDX_Text(pDX, IDC_SYS_TTSSN, m_strTTSSN);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageExternal, CPropertyPage)
	//{{AFX_MSG_MAP(CPageExternal)
	ON_BN_CLICKED(IDC_EXT_EXTSYS, OnChangeExt_Parameters)
	ON_BN_CLICKED(IDC_EXT_TTS, OnChangeExt_Parameters)
	ON_BN_CLICKED(IDC_EXT_ASR, OnChangeExt_Parameters)
	ON_EN_CHANGE(IDC_SYS_TTSPORT, OnChangeExt_Parameters)
	ON_EN_CHANGE(IDC_SYS_EXTPORT, OnChangeExt_Parameters)
	ON_EN_CHANGE(IDC_SYS_TTSSN, OnChangeExt_Parameters)
	ON_EN_CHANGE(IDC_SYS_TTSBufferSize, OnChangeExt_Parameters)
	ON_EN_CHANGE(IDC_SYS_TTSBufferCount, OnChangeExt_Parameters)
	ON_EN_CHANGE(IDC_SYS_TTSVID, OnChangeExt_Parameters)
	ON_EN_CHANGE(IDC_SYS_ASRPORT, OnChangeExt_Parameters)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_SYS_EXTIP, OnChangeExt_Parameters)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_SYS_TTSIP, OnChangeExt_Parameters)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_SYS_ASRIP, OnChangeExt_Parameters)
	//}}AFX_MSG_MAP
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageExternal message handlers
BOOL CPageExternal::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	/// Sun updated 2007-10-28
	/// From
	/*
	m_wnd_tts_speed.SetRange(-500, 500, TRUE);
	m_wnd_tts_speed.SetTicFreq(100);

	m_wnd_tts_volume.SetRange(-20, 20, TRUE);
	m_wnd_tts_volume.SetTicFreq(4);

	m_wnd_tts_pitch.SetRange(-500, 500, TRUE);
	m_wnd_tts_pitch.SetTicFreq(100);
	*/
	/// To
	m_wnd_tts_speed.SetRange(-10, 10, TRUE);
	m_wnd_tts_speed.SetTicFreq(2);

	m_wnd_tts_volume.SetRange(-10, 10, TRUE);
	m_wnd_tts_volume.SetTicFreq(2);

	m_wnd_tts_pitch.SetRange(-10, 10, TRUE);
	m_wnd_tts_pitch.SetTicFreq(2);

	// Read Setting Infomation from INI File
	Information_ReadIniFile();

	// Set Flags
	m_autoprocess = false;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageExternal::Information_ReadIniFile()
{
	m_autoprocess = true;

	// External System
	m_chkEXT.InitCheckBox();
	m_chkEXT.SetCheck(pMainWindow->m_blnEnableEXT);
	m_ext_port = pMainWindow->m_param_extport;
	m_ext_ip.SetWindowText(pMainWindow->m_param_extip);
	
	// TTS Module
	m_chkTTS.InitCheckBox();
	m_chkTTS.SetCheck(pMainWindow->m_blnEnableTTS);
	m_tts_port = pMainWindow->m_TTSServerInfo.m_nServerPort;
	m_tts_ip.SetWindowText(pMainWindow->m_TTSServerInfo.m_strServerIP);
	m_strTTSSN = pMainWindow->m_TTSServerInfo.m_strSerialNO;
	m_nTTSBufferSize = pMainWindow->m_nTTSBufferSize;
	m_nTTSBufferCount = pMainWindow->m_nTTSBufferCount;
	m_nTTSVID = pMainWindow->m_TTSServerInfo.m_nVID;
	m_wnd_tts_speed.SetPos(pMainWindow->m_TTSServerInfo.m_nSpeed);
	m_wnd_tts_volume.SetPos(pMainWindow->m_TTSServerInfo.m_nVolume);
	m_wnd_tts_pitch.SetPos(pMainWindow->m_TTSServerInfo.m_nPitch);

	// ASR Module
	m_chkASR.InitCheckBox();
	m_chkASR.SetCheck(pMainWindow->m_blnEnableASR);
	m_asr_port = pMainWindow->m_param_asrport;
	m_asr_ip.SetWindowText(pMainWindow->m_param_asrip);
	
	UpdateData(FALSE);
	m_changed = FALSE;
}

// Save Setting
/// true - save and apply
/// false - save only
BOOL CPageExternal::Information_WriteNewSetting(bool f_apply)
{
	char	szTmp[32];
	m_bNeedRestart = FALSE;

	if( this->m_hWnd == NULL )
		return TRUE;

	// Load Waiting Cursor
	HCURSOR hOldCur = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));

	UpdateData(TRUE);

	if( m_changed )
	{
		/// External
		theApp.WriteProfileInt("SOCKETINFO", "SupportEXT", m_chkEXT.GetCheck());
		memset(szTmp, 0x00, sizeof(szTmp));
		m_ext_ip.GetWindowText(szTmp, sizeof(szTmp));
		theApp.WriteProfileString("SOCKETINFO", "EXTIP", szTmp);
		theApp.WriteProfileInt("SOCKETINFO", "EXTPORT", m_ext_port);

		/// TTS
		theApp.WriteProfileInt("TTS", "SupportTTS", m_chkTTS.GetCheck());
		memset(szTmp, 0x00, sizeof(szTmp));
		m_tts_ip.GetWindowText(szTmp, sizeof(szTmp));
		theApp.WriteProfileString("TTS", "ServerIP", szTmp);
		theApp.WriteProfileInt("TTS", "ServerPort", m_tts_port);
		theApp.WriteProfileInt("TTS", "BufferCount", m_nTTSBufferCount);
		theApp.WriteProfileInt("TTS", "BufferSize", m_nTTSBufferSize);
		theApp.WriteProfileString("TTS", "SerialNO", m_strTTSSN);
		theApp.WriteProfileInt("TTS", "VID", m_nTTSVID);
		theApp.WriteProfileInt("TTS", "Speed", m_wnd_tts_speed.GetPos());
		theApp.WriteProfileInt("TTS", "Volume", m_wnd_tts_volume.GetPos());
		theApp.WriteProfileInt("TTS", "Pitch", m_wnd_tts_pitch.GetPos());

		/// ASR
		theApp.WriteProfileInt("ASR", "SupportASR", m_chkASR.GetCheck());
		memset(szTmp, 0x00, sizeof(szTmp));
		m_asr_ip.GetWindowText(szTmp, sizeof(szTmp));
		theApp.WriteProfileString("ASR", "ServerIP", szTmp);
		theApp.WriteProfileInt("ASR", "ServerPort", m_asr_port);

		if( f_apply )
		{
			/// External
			if( pMainWindow->m_blnEnableEXT != m_chkEXT.GetCheck() )
			{
				pMainWindow->m_blnEnableEXT = m_chkEXT.GetCheck();
				m_bNeedRestart = TRUE;
			}
			if( pMainWindow->m_param_extport != m_ext_port )
			{
				pMainWindow->m_param_extport = m_ext_port;
				m_bNeedRestart = TRUE;
			}
			memset(szTmp, 0x00, sizeof(szTmp));
			m_ext_ip.GetWindowText(szTmp, sizeof(szTmp));
			if( pMainWindow->m_param_extip.CompareNoCase(szTmp) != 0 )
			{
				pMainWindow->m_param_extip = szTmp;
				m_bNeedRestart = TRUE;
			}

			/// TTS
			if( pMainWindow->m_blnEnableTTS != m_chkTTS.GetCheck() )
			{
				pMainWindow->m_blnEnableTTS = m_chkTTS.GetCheck();
				m_bNeedRestart = TRUE;
			}
			if( pMainWindow->m_TTSServerInfo.m_nServerPort != m_tts_port )
			{
				pMainWindow->m_TTSServerInfo.m_nServerPort = m_tts_port;
				m_bNeedRestart = TRUE;
			}
			memset(szTmp, 0x00, sizeof(szTmp));
			m_tts_ip.GetWindowText(szTmp, sizeof(szTmp));
			if( pMainWindow->m_TTSServerInfo.m_strServerIP.CompareNoCase(szTmp) != 0 )
			{
				pMainWindow->m_TTSServerInfo.m_strServerIP = szTmp;
				m_bNeedRestart = TRUE;
			}
			if( pMainWindow->m_nTTSBufferSize != (SHORT)m_nTTSBufferSize )
			{
				pMainWindow->m_nTTSBufferSize = (SHORT)m_nTTSBufferSize;
				m_bNeedRestart = TRUE;
			}
			if( pMainWindow->m_nTTSBufferCount != (SHORT)m_nTTSBufferCount )
			{
				pMainWindow->m_nTTSBufferCount = (SHORT)m_nTTSBufferCount;
				m_bNeedRestart = TRUE;
			}
			if( pMainWindow->m_TTSServerInfo.m_strSerialNO != m_strTTSSN )
			{
				pMainWindow->m_TTSServerInfo.m_strSerialNO = m_strTTSSN;
				m_bNeedRestart = TRUE;
			}
			if( pMainWindow->m_TTSServerInfo.m_nVID != (SHORT)m_nTTSVID )
			{
				pMainWindow->m_TTSServerInfo.m_nVID = (SHORT)m_nTTSVID;
				m_bNeedRestart = TRUE;
			}
			if( pMainWindow->m_TTSServerInfo.m_nSpeed != m_wnd_tts_speed.GetPos() )
			{
				pMainWindow->m_TTSServerInfo.m_nSpeed = m_wnd_tts_speed.GetPos();
				m_bNeedRestart = TRUE;
			}
			if( pMainWindow->m_TTSServerInfo.m_nVolume != m_wnd_tts_volume.GetPos() )
			{
				pMainWindow->m_TTSServerInfo.m_nVolume = m_wnd_tts_volume.GetPos();
				m_bNeedRestart = TRUE;
			}
			if( pMainWindow->m_TTSServerInfo.m_nPitch != m_wnd_tts_pitch.GetPos() )
			{
				pMainWindow->m_TTSServerInfo.m_nPitch = m_wnd_tts_pitch.GetPos();
				m_bNeedRestart = TRUE;
			}

			/// ASR
			if( pMainWindow->m_blnEnableASR != m_chkASR.GetCheck() )
			{
				pMainWindow->m_blnEnableASR = m_chkASR.GetCheck();
				m_bNeedRestart = TRUE;
			}
			if( pMainWindow->m_param_asrport != m_asr_port )
			{
				pMainWindow->m_param_asrport = m_asr_port;
				m_bNeedRestart = TRUE;
			}
			memset(szTmp, 0x00, sizeof(szTmp));
			m_asr_ip.GetWindowText(szTmp, sizeof(szTmp));
			if( pMainWindow->m_param_asrip.CompareNoCase(szTmp) != 0 )
			{
				pMainWindow->m_param_asrip = szTmp;
				m_bNeedRestart = TRUE;
			}
		}
		m_changed = FALSE;
	}

	// Resume Cursor
	::SetCursor(hOldCur);

	return TRUE;
}

void CPageExternal::OnChangeExt_Parameters(NMHDR* pNMHDR, LRESULT* pResult)
{
	OnChangeExt_Parameters();

	*pResult = 0;
}

void CPageExternal::OnChangeExt_Parameters() 
{
	// TODO: Add your control notification handler code here
	if( m_autoprocess )
		return;

	m_changed = TRUE;

	// Enable Apply Button
	if( m_Tag )
		((CConfigBox *)m_Tag)->SetApplyEnableState(TRUE);
}

void CPageExternal::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	OnChangeExt_Parameters();

	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}
