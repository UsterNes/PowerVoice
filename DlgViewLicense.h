#if !defined(AFX_DLGVIEWLICENSE1_H__A4DE8FC1_96AB_475D_B484_0D8B9A17B65D__INCLUDED_)
#define AFX_DLGVIEWLICENSE1_H__A4DE8FC1_96AB_475D_B484_0D8B9A17B65D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgViewLicense.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgViewLicense dialog

class CDlgViewLicense : public CDialog
{
// Construction
public:
	CDlgViewLicense(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgViewLicense)
	enum { IDD = IDD_DLG_LICENSEINFO };
	CString	m_strLicense1;
	CString	m_strLicense2;
	CString	m_strLicDateTime;
	CString	m_strProductName;
	CString	m_strLicServerHDSN;
	CString	m_strLicServerMAC;
	CString	m_strLicUID;
	CString	m_strThisServerHDSN;
	CString	m_strThisServerMAC;
	CString	m_strLicense3;
	CString	m_strLicense4;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgViewLicense)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgViewLicense)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVIEWLICENSE1_H__A4DE8FC1_96AB_475D_B484_0D8B9A17B65D__INCLUDED_)
