#if !defined(AFX_BROWSEPATH_H__167F5C7E_940B_4A0A_895E_E9307CC2A080__INCLUDED_)
#define AFX_BROWSEPATH_H__167F5C7E_940B_4A0A_895E_E9307CC2A080__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BrowsePath.h : header file
//

#include "DirTreeControl.h"

/////////////////////////////////////////////////////////////////////////////
// CBrowsePath dialog

class CBrowsePath : public CDialog
{
// Construction
public:
	CBrowsePath(CWnd* pParent = NULL, BOOL blnPathBrower = TRUE);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBrowsePath)
	enum { IDD = IDD_BROWSEPATH };
	CDirTreeControl	m_wndDirList;
	CListCtrl		m_wndFileList;
	CComboBox		m_wndDriverList;
	CString			m_szCurPath;
	CString			m_szCurFile;
	CString			m_szExt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBrowsePath)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBrowsePath)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmbDrivers();
	afx_msg void OnSelchangedTreDir(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkLstFileview(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
private:
	BOOL m_blnPathBrower;

	BOOL InitDriverList();
	BOOL FindListCtrlItem(CListCtrl *f_list, CString f_name, int *f_index);
	int InsertNewFileItem(int nItem, CString strFileName, DWORD strFileLength, CTime& FileTime);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BROWSEPATH_H__167F5C7E_940B_4A0A_895E_E9307CC2A080__INCLUDED_)
