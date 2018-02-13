// PageGroup.cpp : implementation file
//

#include "stdafx.h"
#include "PowerVoice.h"
#include "PageGroup.h"
#include "ConfigBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageGroup property page

IMPLEMENT_DYNCREATE(CPageGroup, CPropertyPage)

CPageGroup::CPageGroup() : CPropertyPage(CPageGroup::IDD)
, m_bNeedRestart(false)
{
	//{{AFX_DATA_INIT(CPageGroup)
	m_ivr_channels = 0;
	m_ivr_groups = 0;
	m_ivr_channeldn = _T("");
	m_ivr_groupcaption = _T("");
	m_ivr_groupenabled = FALSE;
	m_ivr_groupcti = FALSE;
	m_ivr_groupendch = 0;
	m_ivr_groupstartch = 0;
	m_ivr_groupobonly = FALSE;
	//}}AFX_DATA_INIT
	m_Tag = NULL;
	m_changed = FALSE;
	m_autoprocess = true;
}

CPageGroup::~CPageGroup()
{
}

void CPageGroup::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageGroup)
	DDX_Control(pDX, IDC_IVR_GROUPLIST, m_ivr_grouplist);
	DDX_Control(pDX, IDC_IVR_CHLIST, m_ivr_channellist);
	DDX_Control(pDX, IDC_IVR_SPIN2, m_ivr_channel_spin);
	DDX_Control(pDX, IDC_IVR_SPIN1, m_ivr_group_spin);
	DDX_Control(pDX, IDC_IVR_SPIN3, m_ivr_group_chspin1);
	DDX_Control(pDX, IDC_IVR_SPIN4, m_ivr_group_chspin2);
	DDX_Text(pDX, IDC_IVR_CHANNELS, m_ivr_channels);
	DDX_Text(pDX, IDC_IVR_GROUPS, m_ivr_groups);
	DDV_MinMaxUInt(pDX, m_ivr_groups, 0, 256);
	DDX_Text(pDX, IDC_IVR_DN, m_ivr_channeldn);
	DDX_Text(pDX, IDC_IVR_CAPTION, m_ivr_groupcaption);
	DDX_Check(pDX, IDC_GROUPENABLED, m_ivr_groupenabled);
	DDX_Check(pDX, IDC_GROUPCTI, m_ivr_groupcti);
	DDX_Text(pDX, IDC_IVR_ENDCH, m_ivr_groupendch);
	DDX_Text(pDX, IDC_IVR_STARTCH, m_ivr_groupstartch);
	DDX_Check(pDX, IDC_GROUPOBONLY, m_ivr_groupobonly);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageGroup, CPropertyPage)
	//{{AFX_MSG_MAP(CPageGroup)
	ON_NOTIFY(UDN_DELTAPOS, IDC_IVR_SPIN1, OnDeltaposSpnMaxGroups)
	ON_NOTIFY(UDN_DELTAPOS, IDC_IVR_SPIN2, OnDeltaposSpnMaxChannels)
	ON_LBN_SELCHANGE(IDC_IVR_GROUPLIST, OnSelchangeIvrGrouplist)
	ON_LBN_SELCHANGE(IDC_IVR_CHLIST, OnSelchangeIvrChlist)
	ON_EN_CHANGE(IDC_IVR_GROUPS, OnChangeIVR_TotalGroups)
	ON_EN_CHANGE(IDC_IVR_CHANNELS, OnChangeIVR_TotalChannels)
	ON_EN_CHANGE(IDC_IVR_STARTCH, OnChangeIVR_GroupStartCHSetting)
	ON_EN_CHANGE(IDC_IVR_ENDCH, OnChangeIVR_GroupEndCHSetting)
	ON_BN_CLICKED(IDMODIFY, OnModify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageGroup message handlers
BOOL CPageGroup::OnInitDialog() 
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

void CPageGroup::Information_ReadIniFile()
{
	int lv_loop;
	CString sztemp;

	m_autoprocess = true;
	
	// IVR table
	m_ivr_group_spin.SetBase(1);
	m_ivr_group_spin.SetRange(0, MAX_IVR_GROUP);
	m_ivr_group_spin.SetPos(theApp.m_nIVRGroups);
	m_ivr_groups = theApp.m_nIVRGroups;
	m_ivr_channel_spin.SetBase(1);
	m_ivr_channel_spin.SetRange(0, MAX_IVR_CHANNEL);
	m_ivr_channel_spin.SetPos(theApp.m_nIVRChannels);
	m_ivr_channels = theApp.m_nIVRChannels;
		
	for( lv_loop = 0; lv_loop < MAX_IVR_GROUP; lv_loop++ )
	{
		m_pGroupInfo[lv_loop].Enabled = PGroupInfo[lv_loop].Enabled;
		m_pGroupInfo[lv_loop].NeedCTI = PGroupInfo[lv_loop].NeedCTI;
		m_pGroupInfo[lv_loop].StartCH = PGroupInfo[lv_loop].StartCH;
		m_pGroupInfo[lv_loop].EndCH = PGroupInfo[lv_loop].EndCH;
		m_pGroupInfo[lv_loop].Caption = (const char *)PGroupInfo[lv_loop].Caption;

		// Sun added 2005-07-10
		m_pGroupInfo[lv_loop].blnOBOnly = PGroupInfo[lv_loop].blnOBOnly;
	}
	for( lv_loop = 0; lv_loop < MAX_IVR_CHANNEL; lv_loop++ )
		m_pChannelDN[lv_loop] = PChannelDN[lv_loop];

	m_ivr_grouplist.ResetContent();
	m_ivr_channellist.ResetContent();

	for( lv_loop = 0; lv_loop < theApp.m_nIVRGroups; lv_loop++ )
	{
		sztemp.Empty();
		sztemp.Format("Group%2u   %c   CH%3d   CH%3d   %c   ", lv_loop, (m_pGroupInfo[lv_loop].Enabled ? 'Y' : 'N'), m_pGroupInfo[lv_loop].StartCH, m_pGroupInfo[lv_loop].EndCH, (m_pGroupInfo[lv_loop].NeedCTI ? 'C' : ' '));
		sztemp += m_pGroupInfo[lv_loop].Caption;
		m_ivr_grouplist.InsertString(lv_loop, sztemp);
	}
	
	for( lv_loop = 0; lv_loop < theApp.m_nIVRChannels; lv_loop++ )
	{
		sztemp.Empty();
		if( PIVRCH[lv_loop] != NULL )
		{
			m_pChannelDN[lv_loop] = PIVRCH[lv_loop]->GetDeviceNo();
			sztemp.Format("CH%3u    %s", lv_loop, m_pChannelDN[lv_loop]);
		}
		else
			sztemp.Format("CH%3u", lv_loop);

		m_ivr_channellist.InsertString(lv_loop, sztemp);
	}
	
	UpdateData(FALSE);
	m_changed = FALSE;
}

// Save Setting
/// true - save and apply
/// false - save only
BOOL CPageGroup::Information_WriteNewSetting(bool f_apply)
{
	UINT	lv_loop;
	char	lv_buffer[50];
	char	lv_nm_caption[50];
	char	lv_nm_enabled[50];
	char	lv_nm_needcti[50];
	char	lv_nm_startch[50];
	char	lv_nm_endch[50];
	char	lv_nm_channdn[50];
	char	lv_nm_obonly[50];
	UINT	lv_nExtGroups;
	UINT	lv_nExtChannels;
	CString lv_strTemp;
	m_bNeedRestart = FALSE;

	if( this->m_hWnd == NULL )
		return TRUE;

	// Load Waiting Cursor
	HCURSOR hOldCur = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));

	UpdateData(TRUE);

	// IVR Table
	if( m_changed )
	{
		theApp.WriteProfileInt("WORKINFO", "IVRGROUPS", m_ivr_groups);
		theApp.WriteProfileInt("WORKINFO", "IVRCHANNELS", m_ivr_channels);

		lv_nExtGroups = __max(m_ivr_groups, theApp.m_nIVRGroups);
		lv_nExtChannels = __max(m_ivr_channels, theApp.m_nIVRChannels);
		for(lv_loop = 0; lv_loop < lv_nExtGroups; lv_loop++)
		{
			_itoa(lv_loop + 1, lv_buffer, 10);
			sprintf(lv_nm_caption, "GROUP%s_Caption", lv_buffer);
			sprintf(lv_nm_enabled, "GROUP%s_Enabled", lv_buffer);
			sprintf(lv_nm_needcti, "GROUP%s_NeedCTI", lv_buffer);
			sprintf(lv_nm_startch, "GROUP%s_StartCH", lv_buffer);
			sprintf(lv_nm_endch, "GROUP%s_EndCH", lv_buffer);
			sprintf(lv_nm_obonly, "GROUP%s_OBOnly", lv_buffer);
			
			theApp.WriteProfileString("GROUPINFO", lv_nm_caption, m_pGroupInfo[lv_loop].Caption);
			theApp.WriteProfileInt("GROUPINFO", lv_nm_enabled, (m_pGroupInfo[lv_loop].Enabled ? 1 : 0));
			theApp.WriteProfileInt("GROUPINFO", lv_nm_needcti, (m_pGroupInfo[lv_loop].NeedCTI ? 1 : 0));
			theApp.WriteProfileInt("GROUPINFO", lv_nm_startch, m_pGroupInfo[lv_loop].StartCH);
			theApp.WriteProfileInt("GROUPINFO", lv_nm_endch, m_pGroupInfo[lv_loop].EndCH);

			// Sun added 2005-07-10
			theApp.WriteProfileInt("GROUPINFO", lv_nm_obonly, (m_pGroupInfo[lv_loop].blnOBOnly ? 1 : 0));
		}

		for(lv_loop = 0; lv_loop < lv_nExtChannels; lv_loop++)
		{
			_itoa(lv_loop + 1, lv_buffer, 10);
			sprintf(lv_nm_channdn, "CH%s_DN", lv_buffer);
			
			theApp.WriteProfileString("CHANNELINFO", lv_nm_channdn, m_pChannelDN[lv_loop]);
		}

		if( f_apply )
		{
			/// Note: 
			/// 1. Currently only 4 types of properties can be changed dynamically
			/// 2. Change Amount of IVR Groups & IVR Channels
			for(lv_loop = 0; lv_loop < lv_nExtGroups; lv_loop++)
			{
				try
				{
					if( lv_loop < m_ivr_groups )
					{
						if( PIVRGroup[lv_loop] == NULL )
						{
							// Create New Group
							PIVRGroup[lv_loop] = new CIVRGroup;
							PIVRGroup[lv_loop]->Create(lv_loop);
						}

						// Update Group property
						lv_strTemp = (wchar_t *)PGroupInfo[lv_loop].Caption;
						if( m_pGroupInfo[lv_loop].Caption.CompareNoCase(lv_strTemp) != 0 )
						{
							PGroupInfo[lv_loop].Caption = m_pGroupInfo[lv_loop].Caption;
							PIVRGroup[lv_loop]->SetCaption( m_pGroupInfo[lv_loop].Caption );
						}

						if( PGroupInfo[lv_loop].Enabled != m_pGroupInfo[lv_loop].Enabled )
						{
							PGroupInfo[lv_loop].Enabled = m_pGroupInfo[lv_loop].Enabled;
							PIVRGroup[lv_loop]->SetEnabled( PGroupInfo[lv_loop].Enabled );
						}
						if( PGroupInfo[lv_loop].NeedCTI != m_pGroupInfo[lv_loop].NeedCTI )
						{
							PGroupInfo[lv_loop].NeedCTI = m_pGroupInfo[lv_loop].NeedCTI;
							PIVRGroup[lv_loop]->SetNeedCTI( PGroupInfo[lv_loop].NeedCTI );
						}
						if( PGroupInfo[lv_loop].StartCH != m_pGroupInfo[lv_loop].StartCH )
						{
							PGroupInfo[lv_loop].StartCH = m_pGroupInfo[lv_loop].StartCH;
							PIVRGroup[lv_loop]->SetStartCH( PGroupInfo[lv_loop].StartCH );
						}
						if( PGroupInfo[lv_loop].EndCH != m_pGroupInfo[lv_loop].EndCH )
						{
							PGroupInfo[lv_loop].EndCH = m_pGroupInfo[lv_loop].EndCH;
							PIVRGroup[lv_loop]->SetEndCH( PGroupInfo[lv_loop].EndCH );
						}
						if( PGroupInfo[lv_loop].blnOBOnly != m_pGroupInfo[lv_loop].blnOBOnly )
						{
							PGroupInfo[lv_loop].blnOBOnly = m_pGroupInfo[lv_loop].blnOBOnly;
							PIVRGroup[lv_loop]->SetIsOBOnly( PGroupInfo[lv_loop].blnOBOnly );
						}
					}
					else
					{
						PGroupInfo[lv_loop].Caption = "";
						PGroupInfo[lv_loop].Enabled = false;
						PGroupInfo[lv_loop].NeedCTI = false;
						PGroupInfo[lv_loop].StartCH = 0;
						PGroupInfo[lv_loop].EndCH = 0;
						PGroupInfo[lv_loop].blnOBOnly = false;
						if( PIVRGroup[lv_loop] != NULL )
						{
							// Delete expired Group
							delete(PIVRGroup[lv_loop]);
							PIVRGroup[lv_loop] = NULL;
						}
					}
				}
				catch(...)
				{
				}
			}
			theApp.m_nIVRGroups = m_ivr_groups;

			for(lv_loop = 0; lv_loop < lv_nExtChannels; lv_loop++)
			{
				try
				{
					if( lv_loop < m_ivr_channels )
					{
						if( PIVRCH[lv_loop] == NULL )
						{
							PIVRCH[lv_loop] = new CIVRChannel;
							PIVRCH[lv_loop]->Create(lv_loop);
							PIVRCH[lv_loop]->SetMaxPlayTime(theApp.m_nMaxSoundTime);
							PIVRCH[lv_loop]->SetMaxQueueTime(theApp.m_nMaxWaitTime);
							PIVRCH[lv_loop]->SetMaxFaxTime(theApp.m_nMaxFaxTime);
							
							// Sun added 2002-03-15
							PIVRCH[lv_loop]->SetDeviceNo(m_pChannelDN[lv_loop]);

							PIVRCH[lv_loop]->OpenCh();
						}
						else
						{
							// Sun added 2002-03-15
							if( PChannelDN[lv_loop] != m_pChannelDN[lv_loop] )
								PIVRCH[lv_loop]->SetDeviceNo(PChannelDN[lv_loop]);
						}
						PChannelDN[lv_loop] = m_pChannelDN[lv_loop];
					}
					else
					{
						PChannelDN[lv_loop].Empty();
						if( PIVRCH[lv_loop] != NULL )
						{
							delete(PIVRCH[lv_loop]);
							PIVRCH[lv_loop] = NULL;
						}
					}
				}
				catch(...)
				{
				}
			}
			theApp.m_nIVRChannels = m_ivr_channels;

			// Start & Stop All Groups and Channels
			for(lv_loop = 0; lv_loop < theApp.m_nIVRGroups; lv_loop++)
			{
				if( PIVRGroup[lv_loop] != NULL )
				{
					if( PIVRGroup[lv_loop]->GetEnabled() )
						PIVRGroup[lv_loop]->StartGroup(TRUE);
					else
						PIVRGroup[lv_loop]->StopGroup();
				}
			}
		}
		m_changed = FALSE;
	}

	// Resume Cursor
	::SetCursor(hOldCur);

	return TRUE;
}

void CPageGroup::OnDeltaposSpnMaxGroups(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	pNMUpDown->iDelta *= 1;

	*pResult = 0;
}

void CPageGroup::OnDeltaposSpnMaxChannels(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	pNMUpDown->iDelta *= 1;

	*pResult = 0;
}

void CPageGroup::OnSelchangeIvrGrouplist() 
{
	// TODO: Add your control notification handler code here
	int lv_loop;

	m_autoprocess = true;
	UpdateData(TRUE);
	lv_loop = m_ivr_grouplist.GetCurSel();
	
	m_ivr_groupcaption = m_pGroupInfo[lv_loop].Caption;
	m_ivr_groupenabled = m_pGroupInfo[lv_loop].Enabled;
	m_ivr_groupcti = m_pGroupInfo[lv_loop].NeedCTI;
	m_ivr_group_chspin1.SetBase(1);
	m_ivr_group_chspin1.SetRange(0, m_pGroupInfo[lv_loop].EndCH);
	m_ivr_group_chspin1.SetPos(m_pGroupInfo[lv_loop].StartCH);
	m_ivr_groupstartch = m_pGroupInfo[lv_loop].StartCH;
	m_ivr_group_chspin2.SetBase(1);
	m_ivr_group_chspin2.SetRange(m_pGroupInfo[lv_loop].StartCH, m_ivr_channels - 1);
	m_ivr_group_chspin2.SetPos(m_pGroupInfo[lv_loop].EndCH);
	m_ivr_groupendch = m_pGroupInfo[lv_loop].EndCH;

	// Sun added 2005-07-10
	m_ivr_groupobonly = m_pGroupInfo[lv_loop].blnOBOnly;
	
	UpdateData(FALSE);
	m_autoprocess = false;
}

void CPageGroup::OnChangeIVR_Parameters() 
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

void CPageGroup::OnChangeIVR_TotalGroups()
{
	if( m_autoprocess )
		return;

	m_autoprocess = true;
	US lv_loop, lv_oldGroups = m_ivr_groups;
	CString sztemp;
	UpdateData(TRUE);

	// Sun added 2006-05-10
	if( m_ivr_groups > MAX_IVR_GROUP )
		m_ivr_groups = MAX_IVR_GROUP;

	if( lv_oldGroups > m_ivr_groups )
	{
		for( lv_loop = lv_oldGroups - 1; lv_loop >= m_ivr_groups; lv_loop-- )
		{
			m_pGroupInfo[lv_loop].Enabled = false;
			m_pGroupInfo[lv_loop].NeedCTI = false;
			m_pGroupInfo[lv_loop].Caption.Empty();
			m_pGroupInfo[lv_loop].StartCH = 0;
			m_pGroupInfo[lv_loop].EndCH = 0;
			m_pGroupInfo[lv_loop].blnOBOnly = false;
			m_ivr_grouplist.DeleteString(lv_loop);	
		}
	}
	else if( lv_oldGroups < m_ivr_groups )
	{
		for( lv_loop = lv_oldGroups; lv_loop < m_ivr_groups; lv_loop++ )
		{
			m_pGroupInfo[lv_loop].Enabled = false;
			m_pGroupInfo[lv_loop].NeedCTI = false;
			m_pGroupInfo[lv_loop].Caption = _T("Unknown");
			m_pGroupInfo[lv_loop].StartCH = 0;
			m_pGroupInfo[lv_loop].EndCH = 0;
			m_pGroupInfo[lv_loop].blnOBOnly = false;
			sztemp.Empty();
			sztemp.Format("Group%2u   %c   CH%3d   CH%3d   %c   ", lv_loop, (m_pGroupInfo[lv_loop].Enabled ? 'Y' : 'N'), m_pGroupInfo[lv_loop].StartCH, m_pGroupInfo[lv_loop].EndCH, (m_pGroupInfo[lv_loop].NeedCTI ? 'C' : ' '));
			sztemp += m_pGroupInfo[lv_loop].Caption;
			m_ivr_grouplist.InsertString(lv_loop, sztemp);
		}
	}

	UpdateData(FALSE);
	m_autoprocess = false;
	if( lv_oldGroups != m_ivr_groups )
		OnChangeIVR_Parameters();
}

void CPageGroup::OnChangeIVR_TotalChannels()
{
	if( m_autoprocess )
		return;

	m_autoprocess = true;
	US lv_loop, lv_oldChannels = m_ivr_channels;
	CString sztemp;
	UpdateData(TRUE);

	// Sun added 2006-05-10
	if( m_ivr_channels > MAX_IVR_CHANNEL )
		m_ivr_channels = MAX_IVR_CHANNEL;

	if( lv_oldChannels > m_ivr_channels )
	{
		for( lv_loop = lv_oldChannels - 1; lv_loop >= m_ivr_channels; lv_loop-- )
		{
			m_pChannelDN[lv_loop].Empty();
			m_ivr_channellist.DeleteString(lv_loop);	
		}
	}
	else if( lv_oldChannels < m_ivr_channels )
	{
		for( lv_loop = lv_oldChannels; lv_loop < m_ivr_channels; lv_loop++ )
		{
			m_pChannelDN[lv_loop].Empty();
			sztemp.Empty();
			sztemp.Format("CH%3u    %s", lv_loop, m_pChannelDN[lv_loop]);
			m_ivr_channellist.InsertString(lv_loop, sztemp);
		}
	}

	UpdateData(FALSE);
	m_autoprocess = false;
	if( lv_oldChannels != m_ivr_channels )
		OnChangeIVR_Parameters();
}

void CPageGroup::OnChangeIVR_GroupStartCHSetting() 
{
	// TODO: Add your control notification handler code here
	if( m_autoprocess )
		return;

	m_autoprocess = true;
	UpdateData(TRUE);
	m_ivr_group_chspin2.SetRange(m_ivr_groupstartch, m_ivr_channels - 1);
	if( m_ivr_groupendch < m_ivr_groupstartch )
	{
		m_ivr_group_chspin2.SetPos(m_ivr_groupstartch);
		m_ivr_groupendch = m_ivr_groupstartch;
	}
	UpdateData(FALSE);
	m_autoprocess = false;
}

void CPageGroup::OnChangeIVR_GroupEndCHSetting() 
{
	// TODO: Add your control notification handler code here
	if( m_autoprocess )
		return;

	m_autoprocess = true;
	UpdateData(TRUE);
	m_ivr_group_chspin1.SetRange(0, m_ivr_groupendch);
	if( m_ivr_groupendch < m_ivr_groupstartch )
	{
		m_ivr_group_chspin1.SetPos(m_ivr_groupendch);
		m_ivr_groupstartch = m_ivr_groupendch;
	}
	UpdateData(FALSE);
	m_autoprocess = false;
}

void CPageGroup::OnSelchangeIvrChlist() 
{
	// TODO: Add your control notification handler code here
	int lv_loop;

	m_autoprocess = true;
	UpdateData(TRUE);
	lv_loop = m_ivr_channellist.GetCurSel();
	m_ivr_channeldn = m_pChannelDN[lv_loop];
	UpdateData(FALSE);
	m_autoprocess = false;
}


void CPageGroup::OnModify() 
{
	// TODO: Add your control notification handler code here
	int lv_loop;
	CString sztemp;
	bool lv_bDNDataChanged = false;
	bool lv_bDataChanged = false;

	UpdateData(TRUE);

	lv_loop = m_ivr_channellist.GetCurSel();
	if( lv_loop >= 0 )
	{
		if( m_pChannelDN[lv_loop] != m_ivr_channeldn )
		{
			m_pChannelDN[lv_loop] = m_ivr_channeldn;
			sztemp.Format("CH%3u    %s", lv_loop, m_pChannelDN[lv_loop]);
			m_ivr_channellist.DeleteString(lv_loop);
			m_ivr_channellist.InsertString(lv_loop, sztemp);
			m_ivr_channellist.SetCurSel(lv_loop);
			lv_bDNDataChanged = true;
		}
	}
	
	lv_loop = m_ivr_grouplist.GetCurSel();
	if( lv_loop >= 0 )
	{
		if( m_pGroupInfo[lv_loop].Caption != m_ivr_groupcaption )
		{
			m_pGroupInfo[lv_loop].Caption = m_ivr_groupcaption;
			lv_bDataChanged = true;
		}
		if( m_pGroupInfo[lv_loop].Enabled != (m_ivr_groupenabled == 1) )
		{
			m_pGroupInfo[lv_loop].Enabled = (m_ivr_groupenabled == 1);
			lv_bDataChanged = true;
		}
		if( m_pGroupInfo[lv_loop].NeedCTI != (m_ivr_groupcti == 1) )
		{
			m_pGroupInfo[lv_loop].NeedCTI = (m_ivr_groupcti == 1);
			lv_bDataChanged = true;
		}
		if( m_pGroupInfo[lv_loop].StartCH != m_ivr_groupstartch )
		{
			m_pGroupInfo[lv_loop].StartCH = m_ivr_groupstartch;
			lv_bDataChanged = true;
		}
		if( m_pGroupInfo[lv_loop].EndCH != m_ivr_groupendch )
		{
			m_pGroupInfo[lv_loop].EndCH = m_ivr_groupendch;
			lv_bDataChanged = true;
		}

		// Sun added 2005-07-10
		if( m_pGroupInfo[lv_loop].blnOBOnly != (m_ivr_groupobonly == 1) )
		{
			m_pGroupInfo[lv_loop].blnOBOnly = (m_ivr_groupobonly == 1);
			lv_bDataChanged = true;
		}

		if( lv_bDataChanged )
		{
			sztemp.Format("Group%2u   %c   CH%3d   CH%3d   %c   ", lv_loop, (m_pGroupInfo[lv_loop].Enabled ? 'Y' : 'N'), m_pGroupInfo[lv_loop].StartCH, m_pGroupInfo[lv_loop].EndCH, (m_pGroupInfo[lv_loop].NeedCTI ? 'C' : ' '));
			sztemp += m_pGroupInfo[lv_loop].Caption;
			m_ivr_grouplist.DeleteString(lv_loop);
			m_ivr_grouplist.InsertString(lv_loop, sztemp);
			m_ivr_grouplist.SetCurSel(lv_loop);
		}
	}

	UpdateData(FALSE);

	if( lv_bDataChanged || lv_bDNDataChanged )
		OnChangeIVR_Parameters();
}
