#if !defined(AFX_CONFIGBOX_H__32B3714E_2F99_442E_8A0A_3FA3F4AC214F__INCLUDED_)
#define AFX_CONFIGBOX_H__32B3714E_2F99_442E_8A0A_3FA3F4AC214F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigBox.h : header file
//

#include "PageSystem.h"
#include "PagePBX.h"
#include "PageGroup.h"
#include "PageExternal.h"

/////////////////////////////////////////////////////////////////////////////
// CConfigBox dialog

class CConfigBox : public CDialog
{
// Construction
public:
	CConfigBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfigBox();

	void SetApplyEnableState(BOOL f_bEnabled = TRUE);

// Dialog Data
	//{{AFX_DATA(CConfigBox)
	enum { IDD = IDD_CONFIG };
	CTreeCtrl	m_tree_opt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList		*m_pimagelist;

	void ChangePageStyle(int f_nIndex);

	// Generated message map functions
	//{{AFX_MSG(CConfigBox)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnApplyClick();
	afx_msg void OnReset();
	afx_msg void OnSelchangedTreeOpt(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CPageSystem			m_pPageSystem;
	CPagePBX			m_pPagePBX;
	CPageGroup			m_pPageGroup;
	CPageExternal		m_pPageExternal;

	int					m_nActivePage;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGBOX_H__32B3714E_2F99_442E_8A0A_3FA3F4AC214F__INCLUDED_)
