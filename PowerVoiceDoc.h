// PowerVoiceDoc.h : interface of the CPowerVoiceDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_POWERVOICEDOC_H__2F7D1334_7FA7_431D_B533_A28D7A4BCE9C__INCLUDED_)
#define AFX_POWERVOICEDOC_H__2F7D1334_7FA7_431D_B533_A28D7A4BCE9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPowerVoiceDoc : public CDocument
{
protected: // create from serialization only
	CPowerVoiceDoc();
	DECLARE_DYNCREATE(CPowerVoiceDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPowerVoiceDoc)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPowerVoiceDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPowerVoiceDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POWERVOICEDOC_H__2F7D1334_7FA7_431D_B533_A28D7A4BCE9C__INCLUDED_)
