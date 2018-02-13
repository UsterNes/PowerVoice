#if !defined(AFX_DLGGETPASSWORD_H__D74EC616_58DC_4B52_8CD2_B286D39D7341__INCLUDED_)
#define AFX_DLGGETPASSWORD_H__D74EC616_58DC_4B52_8CD2_B286D39D7341__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGetPassword.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGetPassword dialog

class CDlgGetPassword : public CDialog
{
// Construction
public:
	CDlgGetPassword(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGetPassword)
	enum { IDD = IDD_DLG_GETPASSWORD };
	CString	m_sPassword;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGetPassword)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGetPassword)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGETPASSWORD_H__D74EC616_58DC_4B52_8CD2_B286D39D7341__INCLUDED_)
