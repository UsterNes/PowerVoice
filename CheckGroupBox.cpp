// CheckGroupBox.cpp : implementation file
//

#include "stdafx.h"
#include "CheckGroupBox.h"
#include <list>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNAMIC( CCheckGroupBox, CButton )
/////////////////////////////////////////////////////////////////////////////
// CCheckGroupBox
#define ID_CHECK_TITLE		0xFFFF

CCheckGroupBox::CCheckGroupBox()
{
}

CCheckGroupBox::~CCheckGroupBox()
{
}


BEGIN_MESSAGE_MAP(CCheckGroupBox, CButton)
	//{{AFX_MSG_MAP(CCheckGroupBox)
	ON_WM_ENABLE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_COMMAND( ID_CHECK_TITLE , OnClicked)
	ON_MESSAGE(BM_GETCHECK, OnGetCheck)
	ON_MESSAGE(BM_SETCHECK, OnSetCheck)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckGroupBox message handlers


BOOL CCheckGroupBox::InitCheckBox()
{
	CString strText;
	GetWindowText(strText);
	SetWindowText(_T(""));

	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(GetFont());
	CSize czText = dc.GetTextExtent(strText);
	dc.SelectObject(pOldFont);
	// Add some space for the checkbox and at the end
	czText.cx += 30;

	// Move the checkbox on top of the groupbox
	CRect rc;
	GetWindowRect(rc);	
	this->ScreenToClient(rc);
	rc.left += 5;
	rc.right = rc.left + czText.cx;
	rc.bottom = rc.top + czText.cy;
	
	m_CheckTitle.Create(strText, BS_AUTOCHECKBOX | WS_CHILD  , rc, this, ID_CHECK_TITLE);
	m_CheckTitle.SetFont(GetFont(), true);
	m_CheckTitle.ShowWindow(SW_SHOW);
	
	return TRUE;
	
}

void CCheckGroupBox::CheckControls()
{
	int nCheck = m_CheckTitle.GetCheck();
	CRect rcGroupbox;
	GetWindowRect(rcGroupbox);
	list<CCheckGroupBox*> mapChkableGroupWnds;

	// Get first child control
	CWnd* pWnd = GetParent()->GetWindow(GW_CHILD);
	
	CRect rcWnd, rcTest;

	while (pWnd)
	{
		pWnd->GetWindowRect(rcWnd);

		if (rcTest.IntersectRect(rcGroupbox, rcWnd) && 
			(rcTest != rcGroupbox) &&
			pWnd != this && pWnd != &m_CheckTitle)
		{
			if(pWnd->IsKindOf(RUNTIME_CLASS(CCheckGroupBox)))
			{
				mapChkableGroupWnds.push_back((CCheckGroupBox*)pWnd);
			}
			pWnd->EnableWindow(nCheck == 1 && m_CheckTitle.IsWindowEnabled());
		}
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}
	
	list<CCheckGroupBox*>::iterator iter;
	for(iter=mapChkableGroupWnds.begin(); iter!=mapChkableGroupWnds.end(); ++iter)
	{
		(*iter)->SetCheck((*iter)->GetCheck());
	}
}

void CCheckGroupBox::SetCheck(int nCheck)
{
	if(::IsWindow(m_CheckTitle.m_hWnd))
	{
		m_CheckTitle.SetCheck(nCheck);
		CheckControls();
	}

}

int CCheckGroupBox::GetCheck()
{
	if(::IsWindow(m_CheckTitle.m_hWnd))
	{
		return m_CheckTitle.GetCheck();
	}
	return -1;
}

void CCheckGroupBox::OnClicked() 
{
	CheckControls();
	UINT style = m_CheckTitle.GetButtonStyle();
	/*if(style == BS_AUTORADIOBUTTON)
	{
		CWnd* pWnd = GetParent()->GetWindow(GW_CHILD);
		CRect rcWnd, rcTest;

		while (pWnd)
		{
			if(pWnd->IsKindOf(RUNTIME_CLASS(CCheckGroupBox)))
			{
				CCheckGroupBox* pT = (CCheckGroupBox*)pWnd;
				if(pT->GetGroupID() == m_nGroupID && pT != this)
				{
					pT->SetCheck(0);
				}
			}
			pWnd = pWnd->GetWindow(GW_HWNDNEXT);
		}		
	}*/
	
	::SendMessage(GetParent()->m_hWnd,
					WM_COMMAND, 
					MAKEWPARAM(::GetDlgCtrlID(m_hWnd), BN_CLICKED),
					(LPARAM)m_hWnd);
	
}

LRESULT CCheckGroupBox::OnGetCheck(WPARAM wp, LPARAM lp)
{
	return GetCheck();
}

LRESULT CCheckGroupBox::OnSetCheck(WPARAM wp, LPARAM lp)
{
	SetCheck((int)wp);
	return 0;
}

void CCheckGroupBox::OnEnable(BOOL bEnable) 
{
	CButton::OnEnable(bEnable);
	
	// TODO: Add your message handler code here
	m_CheckTitle.EnableWindow(bEnable);
	CheckControls();
	
}

void CCheckGroupBox::OnSetFocus(CWnd* pOldWnd) 
{
	CButton::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	m_CheckTitle.SetFocus();
	
}
