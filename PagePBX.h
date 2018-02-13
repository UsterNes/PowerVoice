#if !defined(AFX_PAGEPBX_H__7F907F63_C01D_4802_8E73_D3B31874EDDD__INCLUDED_)
#define AFX_PAGEPBX_H__7F907F63_C01D_4802_8E73_D3B31874EDDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PagePBX.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPagePBX dialog

class CPagePBX : public CPropertyPage
{
	DECLARE_DYNCREATE(CPagePBX)

// Construction
public:
	CPagePBX();
	~CPagePBX();

	DWORD m_Tag;
	BOOL m_changed;

	void Information_ReadIniFile();
	BOOL Information_WriteNewSetting(bool f_apply = false);

// Dialog Data
	//{{AFX_DATA(CPagePBX)
	enum { IDD = IDD_PAGE_PBX };
	CSliderCtrl	m_pbx_volume;
	CSpinButtonCtrl	m_pbx_spin3;
	CSpinButtonCtrl	m_pbx_spin2;
	CSpinButtonCtrl	m_pbx_spin1;
	UINT	m_pbx_rings;
	UINT	m_pbx_soundtime;
	UINT	m_pbx_waittime;
	int		m_pbx_delta[11];
	int		m_pbx_deltab[11];
	int		m_pbx_freq[11];
	int		m_pbx_freqb[11];
	int		m_pbx_offdelta[11];
	int		m_pbx_offtime[11];
	int		m_pbx_ondelta[11];
	int		m_pbx_ontime[11];
	int		m_pbx_repeattimes[11];
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPagePBX)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPagePBX)
		// NOTE: the ClassWizard will add member functions here
	afx_msg void OnDeltaposSpnMaxSoundTime(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpnMaxQueueTime(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangePBX_Parameters(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangePBX_Parameters(UINT nControlID);
	afx_msg void OnChangePBX_Parameters();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool	m_autoprocess;
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	bool m_bNeedRestart;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEPBX_H__7F907F63_C01D_4802_8E73_D3B31874EDDD__INCLUDED_)
