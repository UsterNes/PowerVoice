// DlgGetPassword.cpp : implementation file
//

#include "stdafx.h"
#include "PowerVoice.h"
#include "DlgGetPassword.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGetPassword dialog

CDlgGetPassword::CDlgGetPassword(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGetPassword::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGetPassword)
	m_sPassword = _T("********");
	//}}AFX_DATA_INIT
}


void CDlgGetPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGetPassword)
	DDX_Text(pDX, IDC_TXT_PASSWORD, m_sPassword);
	DDV_MaxChars(pDX, m_sPassword, 20);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGetPassword, CDialog)
	//{{AFX_MSG_MAP(CDlgGetPassword)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGetPassword message handlers

void CDlgGetPassword::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	if(	m_sPassword == theApp.GetProfileString("System", "ConsolePassword", _T("PCS")) )
	{
		CDialog::OnOK();
		DestroyWindow();
	}
	else
	{
		AfxMessageBox(IDS_MSG_WRONG_PWD);
		GetDlgItem(IDC_TXT_PASSWORD)->SetFocus();
	}
}
