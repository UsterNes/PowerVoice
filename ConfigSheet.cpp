// ConfigSheet.cpp : implementation file
//

#include "stdafx.h"
#include "ConfigSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigSheet

IMPLEMENT_DYNAMIC(CConfigSheet, CPropertySheet)

CConfigSheet::CConfigSheet()
{
}

CConfigSheet::~CConfigSheet()
{
}


BEGIN_MESSAGE_MAP(CConfigSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CConfigSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigSheet message handlers

BOOL CConfigSheet::CheckKey(char cAccel)
{
	CTabCtrl *pTab = GetTabControl();

	int n = pTab->GetItemCount();
		
	char buf[80], shortcut[3];

	TC_ITEM tcItem;
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = buf;
	shortcut[0] = _T('&');
	shortcut[2] = _T('\0');
	for( int i = 0; i < n; i++ )
	{
		tcItem.cchTextMax = 79;
		pTab->GetItem( i, &tcItem );
		shortcut[1] = cAccel;
  
		// convert string characters into uppercase characters
		short nCount = 0;
		while( buf[nCount] != '\0' )
		{
			buf[nCount] = toupper(buf[nCount]);
			++nCount;
		}

		if( strstr( buf, shortcut ) ) 
		{
			SetActivePage( i );
			return TRUE;
		}
	} 
	return FALSE;
}

BOOL CConfigSheet::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_SYSKEYDOWN && pMsg->wParam > 32 )
	{
		if( CheckKey(pMsg->wParam) )
		  return TRUE;

		return(GetParent()->GetParent()->PreTranslateMessage(pMsg));
	}
	
	return CPropertySheet::PreTranslateMessage(pMsg);
}
