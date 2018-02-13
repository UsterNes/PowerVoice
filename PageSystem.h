#if !defined(AFX_PAGESYSTEM_H__2E1C0F7E_839D_4EB0_8F90_B73BE8639540__INCLUDED_)
#define AFX_PAGESYSTEM_H__2E1C0F7E_839D_4EB0_8F90_B73BE8639540__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageSystem.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageSystem dialog

class CPageSystem : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageSystem)

// Construction
public:
	CPageSystem();
	~CPageSystem();

	DWORD m_Tag;
	BOOL m_changed;
	UINT m_nLogLevel;

	void Information_ReadIniFile();
	BOOL Information_WriteNewSetting(bool f_apply = false);

// Dialog Data
	//{{AFX_DATA(CPageSystem)
	enum { IDD = IDD_PAGE_SYS };
	CComboBox	m_wndLogLevel;
	CIPAddressCtrl	m_sys_drip;
	CSpinButtonCtrl	m_sys_spn_reset_delay;
	CSpinButtonCtrl	m_sys_spn_ans_delay;
	CIPAddressCtrl	m_sys_ctiip;
	CString	m_sys_dbserver;
	CString	m_sys_dbname;
	UINT	m_sys_ctiport;
	CString	m_sys_recpath;
	CString	m_sys_voxpath;
	BOOL	m_sys_globalcall;
	int		m_sys_DBType;
	CString	m_sys_dbpassword;
	CString	m_sys_gc_protocol;
	UINT	m_sys_ch_ans_delay;
	UINT	m_sys_ch_reset_delay;
	UINT	m_sys_drport;
	CString	m_sys_dbuserid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageSystem)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageSystem)
	afx_msg void OnBrowse1Click();
	afx_msg void OnBrowse2Click();
	virtual BOOL OnInitDialog();
	afx_msg void OnChkGc();
	afx_msg void OnDeltaposSysSpnAnsDelay(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSysSpnResetDelay(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOptDbType();
	afx_msg void OnChangeSYS_Parameters(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeSYS_Parameters(UINT nControlID);
	afx_msg void OnChangeSYS_Parameters();
	afx_msg void OnSelchangeCmbLog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool m_autoprocess;

	void SetDBAccessType();

public:
	// Sun added 2007-11-04，是否自动计算通道复位时间
	BOOL m_blnAutoCalcCHRT;
	bool m_bNeedRestart;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESYSTEM_H__2E1C0F7E_839D_4EB0_8F90_B73BE8639540__INCLUDED_)
