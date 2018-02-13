// PowerVoiceView.h : interface of the CPowerVoiceView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_POWERVOICEVIEW_H__AF180BCF_0D77_4638_9DED_C5325FFFA78B__INCLUDED_)
#define AFX_POWERVOICEVIEW_H__AF180BCF_0D77_4638_9DED_C5325FFFA78B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>
#include <afxtempl.h>
#include <afxcview.h>

#include "IVRGroup.h"

class CPowerVoiceDoc;

class CPowerVoiceView : public CListView
{
protected: // create from serialization only
	CPowerVoiceView();
	DECLARE_DYNCREATE(CPowerVoiceView)

// Attributes
public:
	CPowerVoiceDoc* GetDocument();

#ifdef INTEL_DIALOGIC
	CMap<DWORD, DWORD, CIVRChannel*, CIVRChannel*> m_oIVRCHMap;
#endif

#ifdef CISCO_CCM
	CMap<DWORD, DWORD, CIVRObject*, CIVRObject*> m_oIVRCHMap;
#endif

protected:
	CImageList *m_pimagelist;
	int				m_nCurrentItem;

// Operations
public:

	void Screen_ShowIVRChannelInfo(CIVRGroup *f_Group);
	BOOL Screen_InsertChannel(CIVRChannel *f_Channel);
	BOOL Screen_DeleteChannel(WPARAM f_ChannelNo);
	BOOL Screen_ChangeListItemStatus(WPARAM f_ChannelNo);
	BOOL Screen_ChangeListItemCallLength(WPARAM f_ChannelNo);
	BOOL Screen_ChangeListItemCTIStatus(WPARAM f_ChannelNo);
	BOOL Screen_ChangeListItemCallerID(WPARAM f_ChannelNo);
	BOOL Screen_ChangeListItemCallCount(WPARAM f_ChannelNo);
	BOOL Screen_ChangeViewMode(bool f_bListMode);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPowerVoiceView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPowerVoiceView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPowerVoiceView)
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:

	void InitMainForm();
	BOOL FindListCtrlItem(CListCtrl *f_list, CString f_name, int *f_index);

#ifdef INTEL_DIALOGIC
	void Sys_IVREventAnalysis(void);
#endif

public:
	afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuchReset();
	afx_msg void OnMenuchRestart();
	afx_msg void OnMenuchObtes();
};

#ifndef _DEBUG  // debug version in PowerVoiceView.cpp
inline CPowerVoiceDoc* CPowerVoiceView::GetDocument()
   { return (CPowerVoiceDoc*)m_pDocument; }
#endif

// Views' Pointer
extern CPowerVoiceView *pChannelView;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POWERVOICEVIEW_H__AF180BCF_0D77_4638_9DED_C5325FFFA78B__INCLUDED_)
