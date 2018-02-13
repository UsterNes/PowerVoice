#if !defined(AFX_DLGAUTOLOADCALLFLOW_H__D98C03C6_5B99_46E7_88A3_1BC0FF17266F__INCLUDED_)
#define AFX_DLGAUTOLOADCALLFLOW_H__D98C03C6_5B99_46E7_88A3_1BC0FF17266F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAutoLoadCallFlow.h : header file
//

#include "CheckGroupBox.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoLoadCallFlow dialog

class CDlgAutoLoadCallFlow : public CDialog
{
// Construction
public:
	int m_nselectitem;
	US m_nselectitemdata;
	BOOL m_blnEnabled;
	CStringArray strFileNameArray;
	CDlgAutoLoadCallFlow(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAutoLoadCallFlow)
	enum { IDD = IDD_DLG_AUTO_CALLFLOW };
	CComboBox	m_wndlistbox;
	CCheckGroupBox	m_cgbEnabled;
	CTime	m_tmDate;
	CTime	m_tmTime;
	CString	m_strFileName;
	int		m_optSource;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAutoLoadCallFlow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAutoLoadCallFlow)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBtnBrowse();
	afx_msg void OnOptSource();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL LoadCallFlowProjectList();
	void SetSeletedItemWithCurrentProject();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAUTOLOADCALLFLOW_H__D98C03C6_5B99_46E7_88A3_1BC0FF17266F__INCLUDED_)
