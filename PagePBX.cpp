// PagePBX.cpp : implementation file
//

#include "stdafx.h"
#include "PowerVoice.h"
#include "PagePBX.h"
#include "ConfigBox.h"
#include ".\pagepbx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPagePBX property page

IMPLEMENT_DYNCREATE(CPagePBX, CPropertyPage)

CPagePBX::CPagePBX() : CPropertyPage(CPagePBX::IDD)
, m_bNeedRestart(false)
{
	//{{AFX_DATA_INIT(CPagePBX)
	m_pbx_rings = 0;
	m_pbx_soundtime = 0;
	m_pbx_waittime = 0;
	//}}AFX_DATA_INIT
	m_Tag = NULL;
	m_changed = FALSE;
	m_autoprocess = true;
}

CPagePBX::~CPagePBX()
{
}

void CPagePBX::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPagePBX)
	DDX_Control(pDX, IDC_PBX_VOLUMN, m_pbx_volume);
	DDX_Control(pDX, IDC_PBX_SPIN3, m_pbx_spin3);
	DDX_Control(pDX, IDC_PBX_SPIN2, m_pbx_spin2);
	DDX_Control(pDX, IDC_PBX_SPIN1, m_pbx_spin1);
	DDX_Text(pDX, IDC_PBX_RINGS, m_pbx_rings);
	DDX_Text(pDX, IDC_PBX_SOUNDTIME, m_pbx_soundtime);
	DDX_Text(pDX, IDC_PBX_WAITTIME, m_pbx_waittime);
	DDX_Text(pDX, IDC_PBX_FREQ1, m_pbx_freq[0]);
	DDX_Text(pDX, IDC_PBX_FREQ2, m_pbx_freq[1]);
	DDX_Text(pDX, IDC_PBX_FREQ3, m_pbx_freq[2]);
	DDX_Text(pDX, IDC_PBX_FREQ4, m_pbx_freq[3]);
	DDX_Text(pDX, IDC_PBX_FREQ5, m_pbx_freq[4]);
	DDX_Text(pDX, IDC_PBX_FREQ6, m_pbx_freq[5]);
	DDX_Text(pDX, IDC_PBX_FREQ7, m_pbx_freq[6]);
	DDX_Text(pDX, IDC_PBX_FREQ8, m_pbx_freq[7]);
	DDX_Text(pDX, IDC_PBX_FREQ9, m_pbx_freq[8]);
	DDX_Text(pDX, IDC_PBX_FREQ10, m_pbx_freq[9]);
	DDX_Text(pDX, IDC_PBX_FREQ11, m_pbx_freq[10]);
	DDX_Text(pDX, IDC_PBX_DELTA1, m_pbx_delta[0]);
	DDX_Text(pDX, IDC_PBX_DELTA2, m_pbx_delta[1]);
	DDX_Text(pDX, IDC_PBX_DELTA3, m_pbx_delta[2]);
	DDX_Text(pDX, IDC_PBX_DELTA4, m_pbx_delta[3]);
	DDX_Text(pDX, IDC_PBX_DELTA5, m_pbx_delta[4]);
	DDX_Text(pDX, IDC_PBX_DELTA6, m_pbx_delta[5]);
	DDX_Text(pDX, IDC_PBX_DELTA7, m_pbx_delta[6]);
	DDX_Text(pDX, IDC_PBX_DELTA8, m_pbx_delta[7]);
	DDX_Text(pDX, IDC_PBX_DELTA9, m_pbx_delta[8]);
	DDX_Text(pDX, IDC_PBX_DELTA10, m_pbx_delta[9]);
	DDX_Text(pDX, IDC_PBX_DELTA11, m_pbx_delta[10]);
	DDX_Text(pDX, IDC_PBX_FREQB1, m_pbx_freqb[0]);
	DDX_Text(pDX, IDC_PBX_FREQB2, m_pbx_freqb[1]);
	DDX_Text(pDX, IDC_PBX_FREQB3, m_pbx_freqb[2]);
	DDX_Text(pDX, IDC_PBX_FREQB4, m_pbx_freqb[3]);
	DDX_Text(pDX, IDC_PBX_FREQB5, m_pbx_freqb[4]);
	DDX_Text(pDX, IDC_PBX_FREQB6, m_pbx_freqb[5]);
	DDX_Text(pDX, IDC_PBX_FREQB7, m_pbx_freqb[6]);
	DDX_Text(pDX, IDC_PBX_FREQB8, m_pbx_freqb[7]);
	DDX_Text(pDX, IDC_PBX_FREQB9, m_pbx_freqb[8]);
	DDX_Text(pDX, IDC_PBX_FREQB10, m_pbx_freqb[9]);
	DDX_Text(pDX, IDC_PBX_FREQB11, m_pbx_freqb[10]);
	DDX_Text(pDX, IDC_PBX_DELTAB1, m_pbx_deltab[0]);
	DDX_Text(pDX, IDC_PBX_DELTAB2, m_pbx_deltab[1]);
	DDX_Text(pDX, IDC_PBX_DELTAB3, m_pbx_deltab[2]);
	DDX_Text(pDX, IDC_PBX_DELTAB4, m_pbx_deltab[3]);
	DDX_Text(pDX, IDC_PBX_DELTAB5, m_pbx_deltab[4]);
	DDX_Text(pDX, IDC_PBX_DELTAB6, m_pbx_deltab[5]);
	DDX_Text(pDX, IDC_PBX_DELTAB7, m_pbx_deltab[6]);
	DDX_Text(pDX, IDC_PBX_DELTAB8, m_pbx_deltab[7]);
	DDX_Text(pDX, IDC_PBX_DELTAB9, m_pbx_deltab[8]);
	DDX_Text(pDX, IDC_PBX_DELTAB10, m_pbx_deltab[9]);
	DDX_Text(pDX, IDC_PBX_DELTAB11, m_pbx_deltab[10]);
	DDX_Text(pDX, IDC_PBX_ONTIME1, m_pbx_ontime[0]);
	DDX_Text(pDX, IDC_PBX_ONTIME2, m_pbx_ontime[1]);
	DDX_Text(pDX, IDC_PBX_ONTIME3, m_pbx_ontime[2]);
	DDX_Text(pDX, IDC_PBX_ONTIME4, m_pbx_ontime[3]);
	DDX_Text(pDX, IDC_PBX_ONTIME5, m_pbx_ontime[4]);
	DDX_Text(pDX, IDC_PBX_ONTIME6, m_pbx_ontime[5]);
	DDX_Text(pDX, IDC_PBX_ONTIME7, m_pbx_ontime[6]);
	DDX_Text(pDX, IDC_PBX_ONTIME8, m_pbx_ontime[7]);
	DDX_Text(pDX, IDC_PBX_ONTIME9, m_pbx_ontime[8]);
	DDX_Text(pDX, IDC_PBX_ONTIME10, m_pbx_ontime[9]);
	DDX_Text(pDX, IDC_PBX_ONTIME11, m_pbx_ontime[10]);
	DDX_Text(pDX, IDC_PBX_ONDELTA1, m_pbx_ondelta[0]);
	DDX_Text(pDX, IDC_PBX_ONDELTA2, m_pbx_ondelta[1]);
	DDX_Text(pDX, IDC_PBX_ONDELTA3, m_pbx_ondelta[2]);
	DDX_Text(pDX, IDC_PBX_ONDELTA4, m_pbx_ondelta[3]);
	DDX_Text(pDX, IDC_PBX_ONDELTA5, m_pbx_ondelta[4]);
	DDX_Text(pDX, IDC_PBX_ONDELTA6, m_pbx_ondelta[5]);
	DDX_Text(pDX, IDC_PBX_ONDELTA7, m_pbx_ondelta[6]);
	DDX_Text(pDX, IDC_PBX_ONDELTA8, m_pbx_ondelta[7]);
	DDX_Text(pDX, IDC_PBX_ONDELTA9, m_pbx_ondelta[8]);
	DDX_Text(pDX, IDC_PBX_ONDELTA10, m_pbx_ondelta[9]);
	DDX_Text(pDX, IDC_PBX_ONDELTA11, m_pbx_ondelta[10]);
	DDX_Text(pDX, IDC_PBX_OFFTIME1, m_pbx_offtime[0]);
	DDX_Text(pDX, IDC_PBX_OFFTIME2, m_pbx_offtime[1]);
	DDX_Text(pDX, IDC_PBX_OFFTIME3, m_pbx_offtime[2]);
	DDX_Text(pDX, IDC_PBX_OFFTIME4, m_pbx_offtime[3]);
	DDX_Text(pDX, IDC_PBX_OFFTIME5, m_pbx_offtime[4]);
	DDX_Text(pDX, IDC_PBX_OFFTIME6, m_pbx_offtime[5]);
	DDX_Text(pDX, IDC_PBX_OFFTIME7, m_pbx_offtime[6]);
	DDX_Text(pDX, IDC_PBX_OFFTIME8, m_pbx_offtime[7]);
	DDX_Text(pDX, IDC_PBX_OFFTIME9, m_pbx_offtime[8]);
	DDX_Text(pDX, IDC_PBX_OFFTIME10, m_pbx_offtime[9]);
	DDX_Text(pDX, IDC_PBX_OFFTIME11, m_pbx_offtime[10]);
	DDX_Text(pDX, IDC_PBX_OFFDELTA1, m_pbx_offdelta[0]);
	DDX_Text(pDX, IDC_PBX_OFFDELTA2, m_pbx_offdelta[1]);
	DDX_Text(pDX, IDC_PBX_OFFDELTA3, m_pbx_offdelta[2]);
	DDX_Text(pDX, IDC_PBX_OFFDELTA4, m_pbx_offdelta[3]);
	DDX_Text(pDX, IDC_PBX_OFFDELTA5, m_pbx_offdelta[4]);
	DDX_Text(pDX, IDC_PBX_OFFDELTA6, m_pbx_offdelta[5]);
	DDX_Text(pDX, IDC_PBX_OFFDELTA7, m_pbx_offdelta[6]);
	DDX_Text(pDX, IDC_PBX_OFFDELTA8, m_pbx_offdelta[7]);
	DDX_Text(pDX, IDC_PBX_OFFDELTA9, m_pbx_offdelta[8]);
	DDX_Text(pDX, IDC_PBX_OFFDELTA10, m_pbx_offdelta[9]);
	DDX_Text(pDX, IDC_PBX_OFFDELTA11, m_pbx_offdelta[10]);
	DDX_Text(pDX, IDC_PBX_REPEAT1, m_pbx_repeattimes[0]);
	DDX_Text(pDX, IDC_PBX_REPEAT2, m_pbx_repeattimes[1]);
	DDX_Text(pDX, IDC_PBX_REPEAT3, m_pbx_repeattimes[2]);
	DDX_Text(pDX, IDC_PBX_REPEAT4, m_pbx_repeattimes[3]);
	DDX_Text(pDX, IDC_PBX_REPEAT5, m_pbx_repeattimes[4]);
	DDX_Text(pDX, IDC_PBX_REPEAT6, m_pbx_repeattimes[5]);
	DDX_Text(pDX, IDC_PBX_REPEAT7, m_pbx_repeattimes[6]);
	DDX_Text(pDX, IDC_PBX_REPEAT8, m_pbx_repeattimes[7]);
	DDX_Text(pDX, IDC_PBX_REPEAT9, m_pbx_repeattimes[8]);
	DDX_Text(pDX, IDC_PBX_REPEAT10, m_pbx_repeattimes[9]);
	DDX_Text(pDX, IDC_PBX_REPEAT11, m_pbx_repeattimes[10]);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPagePBX, CPropertyPage)
	//{{AFX_MSG_MAP(CPagePBX)
		// NOTE: the ClassWizard will add message map macros here
	ON_NOTIFY(UDN_DELTAPOS, IDC_PBX_SPIN1, OnDeltaposSpnMaxSoundTime)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PBX_SPIN2, OnDeltaposSpnMaxQueueTime)
	ON_EN_CHANGE(IDC_PBX_SOUNDTIME, OnChangePBX_Parameters)
	ON_EN_CHANGE(IDC_PBX_WAITTIME, OnChangePBX_Parameters)
	ON_EN_CHANGE(IDC_PBX_RINGS, OnChangePBX_Parameters)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_PBX_VOLUMN, OnChangePBX_Parameters)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_PBX_FREQ1, IDC_PBX_FREQ11, OnChangePBX_Parameters)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_PBX_DELTA1, IDC_PBX_DELTA11, OnChangePBX_Parameters)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_PBX_FREQB1, IDC_PBX_FREQB11, OnChangePBX_Parameters)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_PBX_DELTAB1, IDC_PBX_DELTAB11, OnChangePBX_Parameters)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_PBX_ONTIME1, IDC_PBX_ONTIME11, OnChangePBX_Parameters)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_PBX_ONDELTA1, IDC_PBX_ONDELTA11, OnChangePBX_Parameters)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_PBX_OFFTIME1, IDC_PBX_OFFTIME11, OnChangePBX_Parameters)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_PBX_OFFDELTA1, IDC_PBX_OFFDELTA11, OnChangePBX_Parameters)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_PBX_REPEAT1, IDC_PBX_REPEAT11, OnChangePBX_Parameters)
	//}}AFX_MSG_MAP
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPagePBX message handlers
BOOL CPagePBX::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pbx_volume.SetRange(-10, 10, TRUE);
	m_pbx_volume.SetTicFreq(2);

	// Read Setting Infomation from INI File
	Information_ReadIniFile();

	// Set Flags
	m_autoprocess = false;
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPagePBX::Information_ReadIniFile()
{
	int lv_loop;

	m_autoprocess = true;

	// PBX table
	for( lv_loop = 0; lv_loop < 10; lv_loop++ )
	{
		m_pbx_freq[lv_loop] = gb_Tone[lv_loop].freq1.freq;
		m_pbx_delta[lv_loop] = gb_Tone[lv_loop].freq1.deviation;
		m_pbx_freqb[lv_loop] = gb_Tone[lv_loop].freq2.freq;
		m_pbx_deltab[lv_loop] = gb_Tone[lv_loop].freq2.deviation;
		m_pbx_ontime[lv_loop] = gb_Tone[lv_loop].on.time;
		m_pbx_ondelta[lv_loop] = gb_Tone[lv_loop].on.deviation;
		m_pbx_offtime[lv_loop] = gb_Tone[lv_loop].off.time;
		m_pbx_offdelta[lv_loop] = gb_Tone[lv_loop].off.deviation;
		m_pbx_repeattimes[lv_loop] = gb_Tone[lv_loop].repcnt;
	}
	m_pbx_freq[lv_loop] = gb_Tone[lv_loop].freq1.freq;
	m_pbx_delta[lv_loop] = gb_Tone[lv_loop].freq1.deviation;
	m_pbx_freqb[lv_loop] = 0;
	m_pbx_deltab[lv_loop] = 0;
	m_pbx_ontime[lv_loop] = gb_Tone[lv_loop].freq2.freq;
	m_pbx_ondelta[lv_loop] = gb_Tone[lv_loop].freq2.deviation;
	m_pbx_offtime[lv_loop] = gb_Tone[lv_loop].on.time;
	m_pbx_offdelta[lv_loop] = gb_Tone[lv_loop].on.deviation;
	m_pbx_repeattimes[lv_loop] = gb_Tone[lv_loop].off.time;
	
	m_pbx_spin3.SetBase(1);
	m_pbx_spin3.SetRange(0, 3);
	m_pbx_spin3.SetPos(gb_RingsPreOffhook);
	m_pbx_rings = gb_RingsPreOffhook;
	m_pbx_spin1.SetBase(10);
	m_pbx_spin1.SetRange(0, 600);
	m_pbx_spin1.SetPos(theApp.m_nMaxSoundTime);
	m_pbx_soundtime = theApp.m_nMaxSoundTime;
	m_pbx_spin2.SetBase(10);
	m_pbx_spin2.SetRange(0, 300);
	m_pbx_spin2.SetPos(theApp.m_nMaxWaitTime);
	m_pbx_waittime = theApp.m_nMaxWaitTime;
	m_pbx_volume.SetPos(gb_VoiceVolume);
	
	UpdateData(FALSE);
	m_changed = FALSE;
}

// Save Setting
/// true - save and apply
/// false - save only
BOOL CPagePBX::Information_WriteNewSetting(bool f_apply)
{
	int		lv_loop;
	CString lv_szTones;
	m_bNeedRestart = FALSE;

	if( this->m_hWnd == NULL )
		return TRUE;

	// Load Waiting Cursor
	HCURSOR hOldCur = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));

	UpdateData(TRUE);

	// PBX Table
	if( m_changed )
	{
		for( lv_loop = 0; lv_loop < 10; lv_loop++ )
		{
			lv_szTones.Empty();
			lv_szTones.Format("{%d, %d},  {%d, %d},  {%d, %d},  {%d, %d}, %d}", 
				m_pbx_freq[lv_loop],
				m_pbx_delta[lv_loop],
				m_pbx_freqb[lv_loop],
				m_pbx_deltab[lv_loop],
				m_pbx_ontime[lv_loop],
				m_pbx_ondelta[lv_loop],
				m_pbx_offtime[lv_loop],
				m_pbx_offdelta[lv_loop],
				m_pbx_repeattimes[lv_loop]);
			theApp.WriteProfileString("SYSTONE", gb_Tone[lv_loop].str, lv_szTones);
		}
		lv_szTones.Empty();
		lv_szTones.Format("{%d, %d},  {%d, %d},  {%d, %d},  {%d, 0}, 0}", 
			m_pbx_freq[lv_loop],
			m_pbx_delta[lv_loop],
			m_pbx_ontime[lv_loop],
			m_pbx_ondelta[lv_loop],
			m_pbx_offtime[lv_loop],
			m_pbx_offdelta[lv_loop],
			m_pbx_repeattimes[lv_loop]);
		theApp.WriteProfileString("SYSTONE", gb_Tone[lv_loop].str, lv_szTones);
		
		theApp.WriteProfileInt("WORKINFO", "RINGSPREOFFHOOK", m_pbx_rings);
		theApp.WriteProfileInt("WORKINFO", "VOICEVOLUME", m_pbx_volume.GetPos());
		theApp.WriteProfileInt("WORKINFO", "MaxSoundTime", m_pbx_soundtime);
		theApp.WriteProfileInt("WORKINFO", "MaxWaitTime", m_pbx_waittime);

		if( f_apply )
		{
			// Goldsun Note: Need remove and add tones for each channel
			for( lv_loop = 0; lv_loop < 10; lv_loop++ )
			{
				if( gb_Tone[lv_loop].freq1.freq != m_pbx_freq[lv_loop] )
				{
                    gb_Tone[lv_loop].freq1.freq = m_pbx_freq[lv_loop];
					m_bNeedRestart = TRUE;
				}
				if( gb_Tone[lv_loop].freq1.deviation != m_pbx_delta[lv_loop] )
				{
					gb_Tone[lv_loop].freq1.deviation = m_pbx_delta[lv_loop];
					m_bNeedRestart = TRUE;
				}
				if( gb_Tone[lv_loop].freq2.freq != m_pbx_freqb[lv_loop] )
				{
					gb_Tone[lv_loop].freq2.freq = m_pbx_freqb[lv_loop];
					m_bNeedRestart = TRUE;
				}
				if( gb_Tone[lv_loop].freq2.deviation != m_pbx_deltab[lv_loop] )
				{
					gb_Tone[lv_loop].freq2.deviation = m_pbx_deltab[lv_loop];
					m_bNeedRestart = TRUE;
				}
				if( gb_Tone[lv_loop].on.time != m_pbx_ontime[lv_loop] )
				{
					gb_Tone[lv_loop].on.time = m_pbx_ontime[lv_loop];
					m_bNeedRestart = TRUE;
				}
				if( gb_Tone[lv_loop].on.deviation != m_pbx_ondelta[lv_loop] )
				{
					gb_Tone[lv_loop].on.deviation = m_pbx_ondelta[lv_loop];
					m_bNeedRestart = TRUE;
				}
				if( gb_Tone[lv_loop].off.time != m_pbx_offtime[lv_loop] )
				{
					gb_Tone[lv_loop].off.time = m_pbx_offtime[lv_loop];
					m_bNeedRestart = TRUE;
				}
				if( gb_Tone[lv_loop].off.deviation != m_pbx_offdelta[lv_loop] )
				{
					gb_Tone[lv_loop].off.deviation = m_pbx_offdelta[lv_loop];
					m_bNeedRestart = TRUE;
				}
				if( gb_Tone[lv_loop].repcnt != m_pbx_repeattimes[lv_loop] )
				{
					gb_Tone[lv_loop].repcnt = m_pbx_repeattimes[lv_loop];
					m_bNeedRestart = TRUE;
				}
			}
			if( gb_Tone[lv_loop].freq1.freq != m_pbx_freq[lv_loop] )
			{
				gb_Tone[lv_loop].freq1.freq = m_pbx_freq[lv_loop];
				m_bNeedRestart = TRUE;
			}
			if( gb_Tone[lv_loop].freq1.deviation != m_pbx_delta[lv_loop] )
			{
				gb_Tone[lv_loop].freq1.deviation = m_pbx_delta[lv_loop];
				m_bNeedRestart = TRUE;
			}
			if( gb_Tone[lv_loop].freq2.freq != m_pbx_ontime[lv_loop] )
			{
				gb_Tone[lv_loop].freq2.freq = m_pbx_ontime[lv_loop];
				m_bNeedRestart = TRUE;
			}
			if( gb_Tone[lv_loop].freq2.deviation != m_pbx_ondelta[lv_loop] )
			{
				gb_Tone[lv_loop].freq2.deviation = m_pbx_ondelta[lv_loop];
				m_bNeedRestart = TRUE;
			}
			if( gb_Tone[lv_loop].on.time != m_pbx_offtime[lv_loop] )
			{
				gb_Tone[lv_loop].on.time = m_pbx_offtime[lv_loop];
				m_bNeedRestart = TRUE;
			}
			if( gb_Tone[lv_loop].on.deviation != m_pbx_offdelta[lv_loop] )
			{
				gb_Tone[lv_loop].on.deviation = m_pbx_offdelta[lv_loop];
				m_bNeedRestart = TRUE;
			}
			if( gb_Tone[lv_loop].off.time != m_pbx_repeattimes[lv_loop] )
			{
				gb_Tone[lv_loop].off.time = m_pbx_repeattimes[lv_loop];
				m_bNeedRestart = TRUE;
			}

			// Goldsun Note: Need set channel parameters for each channel
			if( gb_RingsPreOffhook != m_pbx_rings )
			{
				gb_RingsPreOffhook = m_pbx_rings;
				m_bNeedRestart = TRUE;
			}

			if( gb_VoiceVolume != m_pbx_volume.GetPos() )
			{
				gb_VoiceVolume = m_pbx_volume.GetPos();
				m_bNeedRestart = TRUE;
			}

			theApp.m_nMaxSoundTime = m_pbx_soundtime;
			theApp.m_nMaxWaitTime = m_pbx_waittime;

			for( lv_loop = 0; lv_loop < theApp.m_nIVRChannels; lv_loop++ )
			{
				if( PIVRCH[lv_loop] != NULL )
				{
					PIVRCH[lv_loop]->SetMaxPlayTime(theApp.m_nMaxSoundTime);
					PIVRCH[lv_loop]->SetMaxQueueTime(theApp.m_nMaxWaitTime);
				}
			}
		}
		m_changed = FALSE;
	}

	// Resume Cursor
	::SetCursor(hOldCur);

	return TRUE;
}

void CPagePBX::OnChangePBX_Parameters() 
{
	// TODO: Add your control notification handler code here
	OnChangePBX_Parameters(0);
}

void CPagePBX::OnChangePBX_Parameters(NMHDR* pNMHDR, LRESULT* pResult)
{
	OnChangePBX_Parameters(0);

	*pResult = 0;
}

void CPagePBX::OnChangePBX_Parameters(UINT nControlID) 
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

void CPagePBX::OnDeltaposSpnMaxSoundTime(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	pNMUpDown->iDelta *= 10;

	*pResult = 0;
}

void CPagePBX::OnDeltaposSpnMaxQueueTime(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	pNMUpDown->iDelta *= 10;

	*pResult = 0;
}


void CPagePBX::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	OnChangePBX_Parameters();

	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}
