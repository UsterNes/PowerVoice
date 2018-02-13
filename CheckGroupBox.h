#if !defined(AFX_CHECKGROUPBOX_H__81566BB9_7606_4925_B5A7_1A28B0BAFC28__INCLUDED_)
#define AFX_CHECKGROUPBOX_H__81566BB9_7606_4925_B5A7_1A28B0BAFC28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CheckGroupBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCheckGroupBox window
/*
//ʹ��˵��:
//		1.���� InitCheckBox()
//		2.��ʼ����ɵ���SetCheck����
//��ע��
//		��ע�����˳���¼������ϼ�(����ָChild����Parent)����InitCheckBox��SetCheck����������
//Create By Stephen Wang
//2004-4-26
*/
class CCheckGroupBox : public CButton
{
	DECLARE_DYNAMIC(CCheckGroupBox)
// Construction
public:
	CCheckGroupBox();
	BOOL InitCheckBox();


// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckGroupBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	int GetCheck();
	void SetCheck(int nCheck);
	virtual ~CCheckGroupBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCheckGroupBox)
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	afx_msg void OnClicked() ;
	afx_msg LRESULT OnGetCheck(WPARAM, LPARAM);
	afx_msg LRESULT OnSetCheck(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()
private:
	void CheckControls();
	CButton m_CheckTitle;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKGROUPBOX_H__81566BB9_7606_4925_B5A7_1A28B0BAFC28__INCLUDED_)
