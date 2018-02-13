#if !defined(AFX_DIRTREECONTROL_H__7CB661F6_5054_11D5_AEFB_0050BACB0109__INCLUDED_)
#define AFX_DIRTREECONTROL_H__7CB661F6_5054_11D5_AEFB_0050BACB0109__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// DirTreeControl.h : header file
//
#include "afxtempl.h"

/////////////////////////////////////////////////////////////////////////////
// CDirTreeControl window

class CDirTreeControl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CDirTreeControl)

// Construction
public:
	CDirTreeControl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDirTreeControl)
	//}}AFX_VIRTUAL

// Implementation
public:
	CString GetFullPath(HTREEITEM hItem);
	BOOL DisplayTree(LPCTSTR strRoot, BOOL bFiles = false);
	LPCTSTR GetSubPath(LPCTSTR strPath);
	virtual ~CDirTreeControl();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDirTreeControl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:

public:                           //sunxd 2001-6-1 14:00
	HTREEITEM FindItemWithFullName(LPCTSTR strPath);
	HTREEITEM GetPointItem(CPoint f_point);
	HTREEITEM m_hitemDrop;
	void ExpandTreeItem(HTREEITEM hItem, UINT nCode);
	CArray<CTime,CTime&> FileTimeArray;
	CStringArray strFilePathArray;
	CStringArray strFileNameArray;
	CDWordArray strFileLengthArray;

	BOOL DisplayDrives();
	BOOL m_bFiles;
	BOOL FindSubDir( LPCTSTR strPath);
	BOOL GetSysImgList();
	
	void DisplayPath(HTREEITEM hParent, LPCTSTR strPath);
	HTREEITEM AddItem(HTREEITEM hParent, LPCTSTR strPath);
	CImageList m_imgList;
	CString m_strRoot;
	CString m_strError;
	//{{AFX_MSG(CDirTreeControl)
	//}}AFX_MSG
	void OnFolderSelchanged(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIRTREECONTROL_H__7CB661F6_5054_11D5_AEFB_0050BACB0109__INCLUDED_)
