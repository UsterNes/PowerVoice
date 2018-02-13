// DirTreeControl.cpp : implementation file
//

#include "stdafx.h"
#include "DirTreeControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDirTreeControl

CDirTreeControl::CDirTreeControl()
{
	m_strRoot = ""; 
	m_bFiles = false;
}

CDirTreeControl::~CDirTreeControl()
{
	m_imgList.Detach();
}

IMPLEMENT_DYNAMIC(CDirTreeControl, CTreeCtrl)

BEGIN_MESSAGE_MAP(CDirTreeControl, CTreeCtrl)
	//{{AFX_MSG_MAP(CDirTreeControl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDirTreeControl message handlers
BOOL CDirTreeControl::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CTreeCtrl::PreCreateWindow(cs))
		return FALSE;

	cs.style |= TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS;

	return TRUE;
}

BOOL CDirTreeControl::DisplayTree(LPCTSTR strRoot, BOOL bFiles)
{
	/*DWORD dwStyle = GetStyle();   // read the windowstyle
	if ( dwStyle & TVS_EDITLABELS ) 
	{
		// Don't allow the user to edit ItemLabels
		ModifyStyle( TVS_EDITLABELS , 0 );
	}*/
	
	// Display the DirTree with the Rootname e.g. C:\
	// if Rootname == NULL then Display all Drives on this PC
    // First, we need the system-ImageList
	
	DeleteAllItems();

	if ( !GetSysImgList() )
		return FALSE;
    m_bFiles = bFiles;  // if TRUE, Display Path- and Filenames 
	if ( strRoot == NULL || strRoot[0] == '\0' )
	{
		if ( !DisplayDrives() )
			return FALSE;
		m_strRoot = "";
	}
    else
	{
		m_strRoot = strRoot;
		if ( m_strRoot.Right(1) != '\\' )
			m_strRoot += "\\";
		HTREEITEM hParent = AddItem( TVI_ROOT, m_strRoot );
		DisplayPath( hParent, strRoot );
	}
	return TRUE;	
}

BOOL CDirTreeControl::GetSysImgList()
{
	SHFILEINFO shFinfo;
	HIMAGELIST hImgList = NULL;

	if ( GetImageList( TVSIL_NORMAL ) )
		m_imgList.Detach();
	
	hImgList = (HIMAGELIST)SHGetFileInfo( "C:\\",
							  0,
							  &shFinfo,
							  sizeof( shFinfo ),
							  SHGFI_SYSICONINDEX | 
							  SHGFI_SMALLICON );
	if ( !hImgList )
	{
		m_strError = "Cannot retrieve the Handle of SystemImageList!";
		return FALSE;
	}
    
	if ( !m_imgList.Attach( hImgList ) )
	{
		m_strError = "Cannot Attach SystemImageList-Handle";
		return FALSE;
	}
    
	SetImageList( &m_imgList, TVSIL_NORMAL );
	return TRUE;   // OK

}

BOOL CDirTreeControl::DisplayDrives()
{
	//
	// Displaying the Availible Drives on this PC
	// This are the First Items in the TreeCtrl
	//
	DeleteAllItems();
	char  szDrives[128];
	char* pDrive;

	if ( !GetLogicalDriveStrings( sizeof(szDrives), szDrives ) )
	{
		m_strError = "Error Getting Logical DriveStrings!";
		return FALSE;
	}

	pDrive = szDrives;
	while( *pDrive )
	{
		HTREEITEM hParent = AddItem( TVI_ROOT, pDrive );
		if ( FindSubDir( pDrive ) )
			InsertItem( "", 0, 0, hParent );
		pDrive += strlen( pDrive ) + 1;
	}

	return TRUE;
}

void CDirTreeControl::DisplayPath(HTREEITEM hParent, LPCTSTR strPath)
{
	//
	// Displaying the Path in the TreeCtrl
	//
	CStringArray strDirArray;
	CFileFind find;
	CString   strPathFiles = strPath;
	BOOL      bFind;
	
	if ( strPathFiles.Right(1) != "\\" )
		strPathFiles += "\\";
	strPathFiles += "*.*";

	// Sun Added 2001-06-05
	strFilePathArray.RemoveAll();
	strFileNameArray.RemoveAll();
	strFileLengthArray.RemoveAll();

	bFind = find.FindFile( strPathFiles );

	while ( bFind )
	{
		bFind = find.FindNextFile();
		if ( find.IsDirectory() && !find.IsDots() )
		{		
			strDirArray.Add( find.GetFilePath() );
		}
		if ( !find.IsDirectory() )
		{
			//sunxd 2001-6-1 14:00
			strFilePathArray.Add( find.GetFilePath() );
           	strFileNameArray.Add( find.GetFileName() );
			strFileLengthArray.Add( find.GetLength() );

			CTime filetime;
			find.GetLastWriteTime(filetime);
            FileTimeArray.Add(filetime);
		}
	}
    
	SetRedraw( FALSE );
	CWaitCursor wait;
    
	for ( int i = 0; i < strDirArray.GetSize(); i++ )
	{
			HTREEITEM hItem = AddItem( hParent, strDirArray.GetAt(i) );
			if ( FindSubDir( strDirArray.GetAt(i) ) )
				InsertItem( "", 0, 0, hItem );
	}

	if ( m_bFiles )
	{
		for ( i = 0; i < strFilePathArray.GetSize(); i++ )
		{
			HTREEITEM hItem = AddItem( hParent, strFilePathArray.GetAt(i) );
			
		}
	}
    
	SetRedraw( TRUE );
}

HTREEITEM CDirTreeControl::AddItem(HTREEITEM hParent, LPCTSTR strPath)
{
	// Adding the Item to the TreeCtrl with the current Icons
	SHFILEINFO shFinfo;
	int iIcon, iIconSel;
    CString    strTemp = strPath;
    
	if ( strTemp.Right(1) != '\\' )
		 strTemp += "\\";
	if ( !SHGetFileInfo( strTemp,
						0,
						&shFinfo,
						sizeof( shFinfo ),
						SHGFI_ICON | 
					    SHGFI_SMALLICON ) )
	{
		m_strError = "Error Gettting SystemFileInfo!";
		return NULL;
	}

	iIcon = shFinfo.iIcon;

	if ( !SHGetFileInfo( strTemp,
						0,
						&shFinfo,
						sizeof( shFinfo ),
						SHGFI_ICON | SHGFI_OPENICON |
					    SHGFI_SMALLICON ) )
	{
		m_strError = "Error Gettting SystemFileInfo!";
		return NULL;
	}

	iIconSel = shFinfo.iIcon;

	if ( strTemp.Right(1) == "\\" )
		strTemp.SetAt( strTemp.GetLength() - 1, '\0' );
	
	if ( hParent == TVI_ROOT )
		return InsertItem( strTemp, iIcon, iIconSel, hParent );
	
	return InsertItem( GetSubPath( strTemp ), iIcon, iIconSel, hParent );

}

LPCTSTR CDirTreeControl::GetSubPath(LPCTSTR strPath)
{
    // getting the last SubPath from a PathString
	// e.g. C:\temp\readme.txt
	// the result = readme.txt
	static CString strTemp;
	int     iPos;

	strTemp = strPath;
	if ( strTemp.Right(1) == '\\' )
		 strTemp.SetAt( strTemp.GetLength() - 1, '\0' );
	iPos = strTemp.ReverseFind( '\\' );
	if ( iPos != -1 )
	    strTemp = strTemp.Mid( iPos + 1);

	return (LPCTSTR)strTemp;

}

BOOL CDirTreeControl::FindSubDir(LPCTSTR strPath)
{
	//
	// Are there subDirs ?
	//
	CFileFind find;
	CString   strTemp = strPath;
	BOOL      bFind;

	if (_tcslen(strPath) == 3 )
	{
		if (strPath[1] == ':' && strPath[2] =='\\')
			strTemp += "*.*";
		else
			strTemp += "\\*.*";
	}
	else strTemp += "\\*.*";
	
	bFind = find.FindFile( strTemp );
		
	while ( bFind )
	{
		bFind = find.FindNextFile();

		if ( find.IsDirectory() && !find.IsDots() )
		{
			return TRUE;
		}
		if ( !find.IsDirectory() && m_bFiles && !find.IsHidden() )
			return TRUE;
	}

	return FALSE;
}

// 返回指定列表项目的完整路经(尾'\')
CString CDirTreeControl::GetFullPath(HTREEITEM hItem)
{
	// get the Full Path of the item
	CString strReturn;
	CString strTemp;
	HTREEITEM hParent = hItem;

	strReturn.Empty();
	while ( hParent )
	{
		
		strTemp = GetItemText( hParent );
		strTemp += "\\";
		strReturn = strTemp + strReturn;
		hParent = GetParentItem( hParent );
	}
    if ( strReturn.Right(1) != '\\' )
		strReturn += "\\";

    return strReturn;
}

// 展开选中的目录
void CDirTreeControl::ExpandTreeItem(HTREEITEM hItem, UINT nCode)
{
	CString strPath;
	
	if ( nCode == TVE_EXPAND )
	{
		HTREEITEM hChild = GetChildItem( hItem );
		while ( hChild )
		{
			DeleteItem( hChild );
			hChild = GetChildItem( hItem );
		}
        
		strPath = GetFullPath( hItem );
		DisplayPath( hItem, strPath );
	}
	Expand(hItem, nCode);
}

// 选中时自动展开
void CDirTreeControl::OnFolderSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	// TODO: Add your control notification handler code here
	SetRedraw(FALSE);
	ExpandTreeItem( pNMTreeView->itemNew.hItem, TVE_EXPAND );

	SetRedraw();
	*pResult = 0;
}

//******************************************************************************
//目的：根据完整名称（路径）查找项目
HTREEITEM CDirTreeControl::FindItemWithFullName(LPCTSTR strPath)
{
	HTREEITEM lv_item = GetRootItem();
	
	CString strFound, strTemp, strReturn = strPath;
	int lv_pos1, lv_pos2;
	
	if ( strReturn.Right(1) != '\\' )
		strReturn += "\\";
	
	lv_pos1 = strReturn.Find(":\\");
	if( lv_pos1 < 0 )
		lv_pos1 = 0;

	lv_pos2 = strReturn.Find("\\", lv_pos1 + 2);
	if( lv_pos2 < lv_pos1 )
		return NULL;

	while( lv_item != NULL)
	{
		strTemp = strReturn.Left(lv_pos2 + 1);			// Include '\\'
		strFound = GetFullPath(lv_item);
		if( strFound.CompareNoCase( strTemp ) == 0 )
		{
			lv_pos1 = lv_pos2;
			lv_pos2 = strReturn.Find("\\", lv_pos1 + 1);
			if( lv_pos2 < lv_pos1 )
				return lv_item;
			else
			{
				ExpandTreeItem(lv_item, TVE_EXPAND);
				lv_item = GetNextItem(lv_item, TVGN_CHILD);
			}
		}
		else
			lv_item = GetNextItem(lv_item, TVGN_NEXTVISIBLE);
	}
	return NULL;
}

//******************************************************************************
//目的：返回当前高亮显示的项目
HTREEITEM CDirTreeControl::GetPointItem(CPoint f_point)
{
	HTREEITEM lv_item = GetRootItem();
	RECT lv_rect;
	
	while( lv_item != NULL)
	{
		GetItemRect(lv_item, &lv_rect, TRUE);
		if( f_point.y >= lv_rect.top && f_point.y <= lv_rect.bottom )
		{
			return lv_item;
		}
		lv_item = GetNextItem(lv_item, TVGN_NEXTVISIBLE);
	}
	return NULL;
}
//******************************************************************************
