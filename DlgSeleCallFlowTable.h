#if !defined(AFX_DLGSELECALLFLOWTABLE_H__FD91075C_55B4_489D_AC2F_DE2A15FFC2F8__INCLUDED_)
#define AFX_DLGSELECALLFLOWTABLE_H__FD91075C_55B4_489D_AC2F_DE2A15FFC2F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSeleCallFlowTable.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSeleCallFlowTable dialog

class CDlgSeleCallFlowTable : public CDialog
{
// Construction
public:
	US m_nselectitemdata;
	int m_nselectitem;
	CStringArray strFileNameArray;
	CDlgSeleCallFlowTable(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSeleCallFlowTable)
	enum { IDD = IDD_SELECALLFLOWTABLE };
	CComboBox	m_wndlistbox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSeleCallFlowTable)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSeleCallFlowTable)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnDblclkCmbCallflow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL LoadCallFlowProjectList();
	void SetSeletedItemWithCurrentProject();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELECALLFLOWTABLE_H__FD91075C_55B4_489D_AC2F_DE2A15FFC2F8__INCLUDED_)
