#if !defined(AFX_PAGEGROUP_H__5D747F5E_FA5E_4BEF_B929_C8808384BDC6__INCLUDED_)
#define AFX_PAGEGROUP_H__5D747F5E_FA5E_4BEF_B929_C8808384BDC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageGroup.h : header file
//

#include "_Global.h"

/////////////////////////////////////////////////////////////////////////////
// CPageGroup dialog

class CPageGroup : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageGroup)

// Construction
public:
	CPageGroup();
	~CPageGroup();

	DWORD m_Tag;
	BOOL m_changed;

	void Information_ReadIniFile();
	BOOL Information_WriteNewSetting(bool f_apply = false);

// Dialog Data
	//{{AFX_DATA(CPageGroup)
	enum { IDD = IDD_PAGE_GROUP };
	CListBox	m_ivr_grouplist;
	CListBox	m_ivr_channellist;
	CSpinButtonCtrl	m_ivr_channel_spin;
	CSpinButtonCtrl	m_ivr_group_spin;
	CSpinButtonCtrl	m_ivr_group_chspin1;
	CSpinButtonCtrl	m_ivr_group_chspin2;
	UINT	m_ivr_channels;
	UINT	m_ivr_groups;
	CString	m_ivr_channeldn;
	CString	m_ivr_groupcaption;
	BOOL	m_ivr_groupenabled;
	BOOL	m_ivr_groupcti;
	UINT	m_ivr_groupendch;
	UINT	m_ivr_groupstartch;
	BOOL	m_ivr_groupobonly;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageGroup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageGroup)
	afx_msg void OnDeltaposSpnMaxGroups(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpnMaxChannels(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeIvrGrouplist();
	afx_msg void OnSelchangeIvrChlist();
	afx_msg void OnChangeIVR_Parameters();
	afx_msg void OnChangeIVR_TotalGroups();
	afx_msg void OnChangeIVR_TotalChannels();
	afx_msg void OnChangeIVR_GroupStartCHSetting();
	afx_msg void OnChangeIVR_GroupEndCHSetting();
	virtual BOOL OnInitDialog();
	afx_msg void OnModify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool m_autoprocess;

	struct LVS_IVR_Group
	{
		bool Enabled;						// Switch
		bool NeedCTI;						// CTI Switch
		bool blnOBOnly;						// Only used for OB Call
		CString Caption;					// Title
		US StartCH;							// Start Channel
		US EndCH;							// End Channel
	};

	LVS_IVR_Group	m_pGroupInfo[MAX_IVR_GROUP];
	CString		m_pChannelDN[MAX_IVR_CHANNEL];
public:
	bool m_bNeedRestart;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEGROUP_H__5D747F5E_FA5E_4BEF_B929_C8808384BDC6__INCLUDED_)
