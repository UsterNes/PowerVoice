// BrowsePath.cpp : implementation file
//

#include "stdafx.h"
#include "PowerVoice.h"
#include "BrowsePath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBrowsePath dialog


CBrowsePath::CBrowsePath(CWnd* pParent /*=NULL*/, BOOL blnPathBrower /*=TRUE*/)
	: CDialog(CBrowsePath::IDD, pParent),
	m_blnPathBrower(blnPathBrower)
{
	//{{AFX_DATA_INIT(CBrowsePath)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_szCurFile = _T("");
	m_szExt = _T("");
}


void CBrowsePath::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBrowsePath)
	DDX_Control(pDX, IDC_TRE_DIR, m_wndDirList);
	DDX_Control(pDX, IDC_LST_FILEVIEW, m_wndFileList);
	DDX_Control(pDX, IDC_CMB_DRIVERS, m_wndDriverList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBrowsePath, CDialog)
	//{{AFX_MSG_MAP(CBrowsePath)
	ON_CBN_SELCHANGE(IDC_CMB_DRIVERS, OnSelchangeCmbDrivers)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TRE_DIR, OnSelchangedTreDir)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_FILEVIEW, OnDblclkLstFileview)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBrowsePath message handlers

void CBrowsePath::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	POSITION lv_pos;

	m_szCurPath = m_wndDirList.GetFullPath( m_wndDirList.GetSelectedItem() );
	lv_pos = m_wndFileList.GetFirstSelectedItemPosition();
	if( lv_pos )
	{
		int nItem = m_wndFileList.GetNextSelectedItem(lv_pos);
		m_szCurFile = m_szCurPath + m_wndFileList.GetItemText(nItem, 0);
	}

	CDialog::OnOK();
}

BOOL CBrowsePath::OnInitDialog() 
{
	// Load Waiting Cursor
	HCURSOR hOldCur = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString lv_rootdriver = "C:\\";
	int lv_pos;

	InitDriverList();
	m_wndFileList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	// Init File List View
	CString lv_strCaption;

	lv_strCaption.LoadString(IDS_CAP_FILE_NAME);
	m_wndFileList.InsertColumn(0, lv_strCaption, LVCFMT_LEFT, 120);
	lv_strCaption.LoadString(IDS_CAP_FILE_SIZE);
	m_wndFileList.InsertColumn(1, lv_strCaption, LVCFMT_RIGHT, 50);
	lv_strCaption.LoadString(IDS_CAP_FILE_MODIFY_DATE);
	m_wndFileList.InsertColumn(2, lv_strCaption, LVCFMT_LEFT, 120);

	if( m_blnPathBrower )
	{
		if( m_szCurPath == "" )
		{
			if( m_wndDriverList.SelectString(-1, lv_rootdriver) != CB_ERR )
				if( m_wndDirList.DisplayTree(lv_rootdriver, false) )
					m_wndDirList.ExpandTreeItem( m_wndDirList.GetRootItem(), TVE_EXPAND );
		}
		else
		{
			if( (lv_pos = m_szCurPath.Find(':')) > 0 )
			{
				lv_rootdriver = m_szCurPath.Left(lv_pos + 1);
				if( m_wndDriverList.SelectString(-1, lv_rootdriver) != CB_ERR )
					if( m_wndDirList.DisplayTree(lv_rootdriver, false) )
					{
						m_wndDirList.ExpandTreeItem( m_wndDirList.GetRootItem(), TVE_EXPAND );
						m_wndDirList.DisplayPath( m_wndDirList.GetRootItem(), m_szCurPath.Right(m_szCurPath.GetLength() - lv_pos - 2) );
						m_wndDirList.SelectItem(m_wndDirList.FindItemWithFullName(m_szCurPath));
					}
			}
		}
	}
	else
	{
		if( m_szCurFile == "" )
		{
			if( m_wndDriverList.SelectString(-1, lv_rootdriver) != CB_ERR )
				if( m_wndDirList.DisplayTree(lv_rootdriver, false) )
					m_wndDirList.ExpandTreeItem( m_wndDirList.GetRootItem(), TVE_EXPAND );
		}
		else
		{
			TCHAR sFilename[_MAX_PATH] = "";	
			TCHAR sDrive[_MAX_DRIVE] = "";
			TCHAR sDir[_MAX_DIR] = "";
			TCHAR sFname[_MAX_FNAME] = "";
			TCHAR sExt[_MAX_EXT] = "";
			int item;
			CString lv_sPath;

			_tsplitpath(m_szCurFile, sDrive, sDir, sFname, sExt);
			if( sDrive[0] != NULL )
			{
				if( m_wndDriverList.SelectString(-1, sDrive) != CB_ERR )
					if( m_wndDirList.DisplayTree(sDrive, false) )
					{
						lv_sPath.Format("%s%s", sDrive, sDir);
						m_szCurPath = lv_sPath;
						m_wndDirList.ExpandTreeItem( m_wndDirList.GetRootItem(), TVE_EXPAND );
						m_wndDirList.DisplayPath( m_wndDirList.GetRootItem(), sDir+1);
						m_wndDirList.SelectItem(m_wndDirList.FindItemWithFullName(lv_sPath));
					}
			}
			if( m_szExt.CompareNoCase(sExt) == 0 || m_szExt == _T("") )
			{
				lv_sPath.Format("%s%s", sFname, sExt);
				if( FindListCtrlItem(&m_wndFileList, lv_sPath, &item) )
				{
					m_wndFileList.SetHotItem(item);
					m_wndFileList.SetItemState(item, LVIS_SELECTED, LVIS_SELECTED);
				}
			}
		}
	}

	UpdateData(FALSE);

	// Resume Cursor
	::SetCursor(hOldCur);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CBrowsePath::InitDriverList()
{
	char  szDrives[128];
	char* pDrive;

	// Clear List
	m_wndDriverList.ResetContent();
	if ( !GetLogicalDriveStrings( sizeof(szDrives), szDrives ) )
		return FALSE;

	pDrive = szDrives;
	while( *pDrive )
	{
		m_wndDriverList.AddString(pDrive);
		pDrive += strlen( pDrive ) + 1;
	}

	return TRUE;
}

void CBrowsePath::OnSelchangeCmbDrivers() 
{
	// TODO: Add your control notification handler code here
	CString lv_rootdriver;

	// Load Waiting Cursor
	HCURSOR hOldCur = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));
	
	m_wndFileList.DeleteAllItems();
	if( m_wndDriverList.GetCurSel() >= 0 )
	{
		SetRedraw(FALSE);
		m_wndDriverList.GetLBText(m_wndDriverList.GetCurSel(), lv_rootdriver);
		if( m_wndDirList.DisplayTree(lv_rootdriver, false) )
			m_wndDirList.ExpandTreeItem( m_wndDirList.GetRootItem(), TVE_EXPAND );	
		SetRedraw();
		m_wndDirList.Invalidate();
	}
	
	// Resume Cursor
	::SetCursor(hOldCur);
}

// Add an item into list control
int CBrowsePath::InsertNewFileItem(int nItem, CString strFileName, DWORD strFileLength,CTime& FileTime)
{
    CListCtrl& control = m_wndFileList;
	
	DWORD lv_filelength;
	CString lv_strLen;
	CString lv_strTim;

	lv_filelength = (strFileLength - 1)/ 1024 + 1;
	lv_strLen.Format("%d KB", lv_filelength);

    lv_strTim=FileTime.Format("%Y-%m-%d %H:%M");

	int item = control.InsertItem(nItem, _T(strFileName));
	control.SetItemText(item,1, lv_strLen);
	control.SetItemText(item,2,lv_strTim);
	control.EnsureVisible(item,true);
    
	return 1;
}

void CBrowsePath::OnSelchangedTreDir(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int lv_loop, lv_pos;
	CString lv_strExt;

	m_wndDirList.OnFolderSelchanged(pNMHDR, pResult);
	m_wndFileList.DeleteAllItems();
	for( lv_loop = 0; lv_loop < m_wndDirList.strFileNameArray.GetSize(); lv_loop++ )
	{
		lv_pos = m_wndDirList.strFileNameArray[lv_loop].ReverseFind('.');
		if( lv_pos > 0 )
			lv_strExt = m_wndDirList.strFileNameArray[lv_loop].Mid(lv_pos);
		else
			lv_strExt = _T("");
		if( m_szExt == _T("") || m_szExt.CompareNoCase(lv_strExt) == 0 )
			InsertNewFileItem(lv_loop, m_wndDirList.strFileNameArray[lv_loop], m_wndDirList.strFileLengthArray[lv_loop], m_wndDirList.FileTimeArray[lv_loop]);
	}
	m_wndFileList.Update(TRUE);

	*pResult = 0;
}

// Find item from list
BOOL CBrowsePath::FindListCtrlItem(CListCtrl *f_list, CString f_name, int *f_index)
{
	int lv_Start, lv_End;

	lv_End = f_list->GetItemCount();

	for( lv_Start = 0; lv_Start <= lv_End; lv_Start++)
	{
		if( f_name.CompareNoCase( f_list->GetItemText(lv_Start, 0) ) == 0 ) 
		{
			*f_index = lv_Start;
			return TRUE;
		}
	}

	return FALSE;
}

void CBrowsePath::OnDblclkLstFileview(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if( !m_blnPathBrower )
		OnOK();

	*pResult = 0;
}
