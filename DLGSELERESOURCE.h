#if !defined(AFX_DLGSELERESOURCE_H__E1D119DA_83FA_401F_9F1F_039D3C5CC977__INCLUDED_)
#define AFX_DLGSELERESOURCE_H__E1D119DA_83FA_401F_9F1F_039D3C5CC977__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLGSELERESOURCE.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDLGSELERESOURCE dialog

class CDLGSELERESOURCE : public CDialog
{
// Construction
public:
	US m_nselectitemdata;
	int m_nselectitem;
	CDLGSELERESOURCE(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDLGSELERESOURCE)
	enum { IDD = IDD_SELERESOURCE };
	CComboBox	m_wndlistbox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDLGSELERESOURCE)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDLGSELERESOURCE)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkCmbResource();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL LoadResourceProjectList();
	void SetSeletedItemWithCurrentProject();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELERESOURCE_H__E1D119DA_83FA_401F_9F1F_039D3C5CC977__INCLUDED_)
