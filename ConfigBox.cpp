// ConfigBox.cpp : implementation file
//

#include "stdafx.h"
#include "PowerVoice.h"
#include "ConfigBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigBox dialog


CConfigBox::CConfigBox(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigBox)
	//}}AFX_DATA_INIT
	m_pimagelist = NULL;
	m_nActivePage = 0;
}

CConfigBox::~CConfigBox()
{
	if( m_pimagelist )
		delete m_pimagelist;
}

void CConfigBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigBox)
	DDX_Control(pDX, IDC_TREE_OPT, m_tree_opt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigBox, CDialog)
	//{{AFX_MSG_MAP(CConfigBox)
	ON_BN_CLICKED(IDAPPLY, OnApplyClick)
	ON_BN_CLICKED(IDRESET, OnReset)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_OPT, OnSelchangedTreeOpt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigBox message handlers
BOOL CConfigBox::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
//	if( pMsg->message == WM_SYSKEYDOWN && pMsg->wParam > 32 )
  //		if( m_pSheet->CheckKey(pMsg->wParam) )
	//		return TRUE;
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CConfigBox::OnInitDialog() 
{
	// Load Waiting Cursor
	HCURSOR hOldCur = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));

	try
	{
		CDialog::OnInitDialog();
		
		// TODO: Add extra initialization here
		m_pimagelist = new CImageList();
		m_pimagelist->Create(16, 16, TRUE, 3, 0);
		m_pimagelist->Add(theApp.LoadIcon(IDI_OPT_SYSTEM));
		m_pimagelist->Add(theApp.LoadIcon(IDI_OPT_PBX));
		m_pimagelist->Add(theApp.LoadIcon(IDI_OPT_CHANNELS));
		m_pimagelist->Add(theApp.LoadIcon(IDI_OPT_EXTERNAL));

		/// Init Options Tree
		HTREEITEM hItem;
		CString lv_sCaption;

		m_tree_opt.DeleteAllItems();
		m_tree_opt.SetImageList(m_pimagelist, TVSIL_NORMAL);
		lv_sCaption.LoadString(IDS_OPT_SYSTEM);
		hItem = m_tree_opt.InsertItem(lv_sCaption, 0, 0, TVI_ROOT);
		m_tree_opt.SetItemData(hItem, 0);
		lv_sCaption.LoadString(IDS_OPT_PBX);
		hItem = m_tree_opt.InsertItem(lv_sCaption, 1, 1, TVI_ROOT);
		m_tree_opt.SetItemData(hItem, 1);
		lv_sCaption.LoadString(IDS_OPT_CHANNELS);
		hItem = m_tree_opt.InsertItem(lv_sCaption, 2, 2, TVI_ROOT);
		m_tree_opt.SetItemData(hItem, 2);
		lv_sCaption.LoadString(IDS_OPT_EXTERNAL);
		hItem = m_tree_opt.InsertItem(lv_sCaption, 3, 3, TVI_ROOT);
		m_tree_opt.SetItemData(hItem, 3);

		m_pPageSystem.Create(IDD_PAGE_SYS, this);
		m_pPageSystem.m_Tag = (DWORD)this;
		m_pPagePBX.Create(IDD_PAGE_PBX, this);
		m_pPagePBX.m_Tag = (DWORD)this;
		m_pPageGroup.Create(IDD_PAGE_GROUP, this);
		m_pPageGroup.m_Tag = (DWORD)this;
		m_pPageExternal.Create(IDD_PAGE_EXT, this);
		m_pPageExternal.m_Tag = (DWORD)this;

		CRect rcSheet;

		GetDlgItem( IDC_PLACEHOLDER )->GetWindowRect( &rcSheet );
		ScreenToClient( &rcSheet );

		m_pPageSystem.SetWindowPos( NULL, rcSheet.left + 4, rcSheet.top + 8, 0, 0,
		SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE );

		m_pPagePBX.SetWindowPos( NULL, rcSheet.left+ 4, rcSheet.top + 8, 0, 0,
		SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE );

		m_pPageGroup.SetWindowPos( NULL, rcSheet.left + 4, rcSheet.top + 8, 0, 0,
		SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE );

		m_pPageExternal.SetWindowPos( NULL, rcSheet.left + 4, rcSheet.top + 8, 0, 0,
		SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE );

		m_nActivePage = 0;
		ChangePageStyle(m_nActivePage);
		m_tree_opt.SetFocus();

		// Disable Apply Button
		SetApplyEnableState(FALSE);
	}
	catch(...)
	{
	}

	// Resume Cursor
	::SetCursor(hOldCur);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigBox::ChangePageStyle(int f_nIndex)
{
	switch( f_nIndex )
	{
	case 0:						// System
		m_pPageSystem.ShowWindow(SW_SHOW);
		m_pPagePBX.ShowWindow(SW_HIDE);
		m_pPageGroup.ShowWindow(SW_HIDE);
		m_pPageExternal.ShowWindow(SW_HIDE);
		break;

	case 1:						// PBX
		m_pPageSystem.ShowWindow(SW_HIDE);
		m_pPagePBX.ShowWindow(SW_SHOW);
		m_pPageGroup.ShowWindow(SW_HIDE);
		m_pPageExternal.ShowWindow(SW_HIDE);
		break;

	case 2:						// Group
		m_pPageSystem.ShowWindow(SW_HIDE);
		m_pPagePBX.ShowWindow(SW_HIDE);
		m_pPageGroup.ShowWindow(SW_SHOW);
		m_pPageExternal.ShowWindow(SW_HIDE);
		break;

	default:					// External
		m_pPageSystem.ShowWindow(SW_HIDE);
		m_pPagePBX.ShowWindow(SW_HIDE);
		m_pPageGroup.ShowWindow(SW_HIDE);
		m_pPageExternal.ShowWindow(SW_SHOW);
		break;
	}
}

void CConfigBox::OnApplyClick() 
{
	// TODO: Add your control notification handler code here
	// Save Setting and Apply New parameters
	CString str;
	BOOL lv_bNeedRestart = FALSE;

	try
	{
		if( m_pPageSystem.Information_WriteNewSetting(true) && 
			m_pPagePBX.Information_WriteNewSetting(true) && 
			m_pPageGroup.Information_WriteNewSetting(true) &&
			m_pPageExternal.Information_WriteNewSetting(true) )
		{
			lv_bNeedRestart = ( m_pPageSystem.m_bNeedRestart 
							|| m_pPagePBX.m_bNeedRestart 
							|| m_pPageGroup.m_bNeedRestart 
							|| m_pPageExternal.m_bNeedRestart );
			if( lv_bNeedRestart )
			{
				str.LoadString(IDS_MSG_NEED_RESTART);
				AfxMessageBox(str, MB_ICONEXCLAMATION | MB_OK);
			}
			else
			{
				str.LoadString(IDS_MSG_NEEDNOT_RESTART);
				AfxMessageBox(str, MB_ICONINFORMATION | MB_OK);
			}
		}
		else
		{
			str.LoadString(IDS_MSG_WRONG_NEWSETTING);
			AfxMessageBox(str, MB_ICONEXCLAMATION | MB_OK);
		}

		// Disable Apply Button
		SetApplyEnableState(FALSE);
	}
	catch(...)
	{
	}
}

void CConfigBox::OnOK() 
{
	// TODO: Add extra validation here
	// Save Setting but don't active them at present
	// Sun replaced 2007-10-30
	// From
	/*
	CString str;

	try
	{
		if( m_pPageSystem.Information_WriteNewSetting(false) && 
			m_pPagePBX.Information_WriteNewSetting(false) && 
			m_pPageGroup.Information_WriteNewSetting(false) && 
			m_pPageExternal.Information_WriteNewSetting(false) )
		{
			str.LoadString(IDS_MSG_SAVED_NEWSETTING);
			AfxMessageBox(str, MB_ICONINFORMATION | MB_OK);
		}
		else
		{
			str.LoadString(IDS_MSG_UNSAVED_NEWSETTING);
			AfxMessageBox(str, MB_ICONEXCLAMATION | MB_OK);
		}

		// Exit Dialog
		CDialog::OnOK();
	}
	catch(...)
	{
	}
	*/
	// To
	//-----------------------
	OnApplyClick();

	// Exit Dialog
	CDialog::OnOK();
	//-----------------------
}

void CConfigBox::OnCancel() 
{
	// TODO: Add extra validation here
	// Save Setting but don't active them at present
	CString str;

	str.LoadString(IDS_MSG_Q_DISCARD_CHANGES);

	if( m_pPageSystem.m_changed || 
		m_pPagePBX.m_changed || 
		m_pPageGroup.m_changed ||
		m_pPageExternal.m_changed )
		if(AfxMessageBox(str, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION | MB_DEFBUTTON1) == IDNO)
			return;

	// Exit Dialog
	CDialog::OnCancel();
}

void CConfigBox::OnReset() 
{
	// TODO: Add your control notification handler code here
	
	switch( m_nActivePage )
	{
	case 0:
		if( m_pPageSystem.m_changed )
			m_pPageSystem.Information_ReadIniFile();
		break;
	case 1:
		if( m_pPagePBX.m_changed )
			m_pPagePBX.Information_ReadIniFile();
		break;
	case 2:
		if( m_pPageGroup.m_changed )
			m_pPageGroup.Information_ReadIniFile();
		break;
	case 3:
		if( m_pPageExternal.m_changed )
			m_pPageExternal.Information_ReadIniFile();
		break;
	}

	if( ! (m_pPageSystem.m_changed || 
			m_pPagePBX.m_changed || 
			m_pPageGroup.m_changed || 
			m_pPageExternal.m_changed ) )
	{
		// Disable Apply Button
		SetApplyEnableState(FALSE);
	}
}

void CConfigBox::SetApplyEnableState(BOOL f_bEnabled)
{
	GetDlgItem(IDAPPLY)->EnableWindow(f_bEnabled);
}

void CConfigBox::OnSelchangedTreeOpt(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;

	if( hItem != NULL )
	{
		m_nActivePage = m_tree_opt.GetItemData(hItem);
		ChangePageStyle(m_nActivePage);
	}

	*pResult = 0;
}
