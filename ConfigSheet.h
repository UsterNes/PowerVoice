#if !defined(AFX_CONFIGSHEET_H__F2FC00E1_5B3C_4DF8_8C7A_73B7C9209B0E__INCLUDED_)
#define AFX_CONFIGSHEET_H__F2FC00E1_5B3C_4DF8_8C7A_73B7C9209B0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigSheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigSheet

class CConfigSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CConfigSheet)

// Construction
public:
	CConfigSheet();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigSheet)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL CheckKey(char cAccel);
	virtual ~CConfigSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CConfigSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGSHEET_H__F2FC00E1_5B3C_4DF8_8C7A_73B7C9209B0E__INCLUDED_)
