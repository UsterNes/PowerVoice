#if !defined(AFX_DLGAUTOLOADRESOURCE_H__C8D49A30_18EF_43CC_877B_C5000568F0BC__INCLUDED_)
#define AFX_DLGAUTOLOADRESOURCE_H__C8D49A30_18EF_43CC_877B_C5000568F0BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAutoLoadResource.h : header file
//

#include "CheckGroupBox.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoLoadResource dialog

class CDlgAutoLoadResource : public CDialog
{
// Construction
public:
	int m_nselectitem;
	US m_nselectitemdata;
	BOOL m_blnEnabled;
	CDlgAutoLoadResource(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAutoLoadResource)
	enum { IDD = IDD_DLG_AUTO_RESOURCE };
	CComboBox	m_wndlistbox;
	CCheckGroupBox	m_cgbEnabled;
	CTime	m_tmDate;
	CTime	m_tmTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAutoLoadResource)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAutoLoadResource)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL LoadResourceProjectList();
	void SetSeletedItemWithCurrentProject();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAUTOLOADRESOURCE_H__C8D49A30_18EF_43CC_877B_C5000568F0BC__INCLUDED_)
