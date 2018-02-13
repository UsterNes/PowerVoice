#if !defined(AFX_PAGEEXTERNAL_H__72252648_45B4_4F8A_9523_74201F1C1B6A__INCLUDED_)
#define AFX_PAGEEXTERNAL_H__72252648_45B4_4F8A_9523_74201F1C1B6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageExternal.h : header file
//

//-------------------------------------------
// Sun added 2005-03-17
//-------------------------------------------
#include "CheckGroupBox.h"

/////////////////////////////////////////////////////////////////////////////
// CPageExternal dialog

class CPageExternal : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageExternal)

// Construction
public:
	CPageExternal();
	~CPageExternal();

	DWORD m_Tag;
	BOOL m_changed;

	void Information_ReadIniFile();
	BOOL Information_WriteNewSetting(bool f_apply = false);

// Dialog Data
	//{{AFX_DATA(CPageExternal)
	enum { IDD = IDD_PAGE_EXT };
	CSliderCtrl	m_wnd_tts_pitch;
	CSliderCtrl	m_wnd_tts_speed;
	CSliderCtrl	m_wnd_tts_volume;
	CIPAddressCtrl	m_tts_ip;
	CIPAddressCtrl	m_ext_ip;
	CIPAddressCtrl	m_asr_ip;
	CCheckGroupBox	m_chkTTS;
	CCheckGroupBox	m_chkEXT;
	CCheckGroupBox	m_chkASR;
	UINT	m_asr_port;
	UINT	m_ext_port;
	UINT	m_nTTSBufferCount;
	UINT	m_nTTSBufferSize;
	UINT	m_tts_port;
	UINT	m_nTTSVID;
	CString	m_strTTSSN;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageExternal)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageExternal)
	afx_msg void OnChangeExt_Parameters();
	afx_msg void OnChangeExt_Parameters(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool	m_autoprocess;
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	bool m_bNeedRestart;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEEXTERNAL_H__72252648_45B4_4F8A_9523_74201F1C1B6A__INCLUDED_)
