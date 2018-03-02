// MyCursorManager.cpp: implementation of the CMyCursorManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyCursorManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyCursorManager::CMyCursorManager()
 :	m_hOldCursor(0),
	m_hCursorDefault(0),
	m_hCursorWait(0),
	m_hCursorHand(0),
	m_hCursorHelp(0)
{
}

CMyCursorManager::~CMyCursorManager()
{
}

void CMyCursorManager::LoadStapleCursors()
{
	m_hCursorDefault = ::LoadCursor(NULL, IDC_ARROW);
	m_hCursorWait = ::LoadCursor(NULL, IDC_WAIT);

#if(WINVER >= 0x0500)
	m_hCursorHand = ::LoadCursor(NULL, IDC_HAND);
#endif

#if(WINVER >= 0x0500)
	m_hCursorHelp = ::LoadCursor(NULL, IDC_HELP);
#endif
}

HCURSOR CMyCursorManager::SetCustomerCursor(WORD nResID)
{
	HCURSOR lv_cursor = NULL;

	try
	{
		m_hOldCursor = ::SetCursor(::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(nResID)));
	}
	catch(...)
	{
		TRACE("LoadCursor(Resource ID: %d) failed!!!\n", nResID);
	}

	return lv_cursor;
}

void CMyCursorManager::ResumeToOld()
{
	SetCursorWithHander(m_hOldCursor);
}

void CMyCursorManager::ChangeToDefaultCursor()
{
	SetCursorWithHander(m_hCursorDefault);
}

void CMyCursorManager::ChangeToWaitCursor()
{
	SetCursorWithHander(m_hCursorWait);
}

void CMyCursorManager::ChangeToHandCursor()
{
	SetCursorWithHander(m_hCursorHand);
}

void CMyCursorManager::ChangeToHelpCursor()
{
	SetCursorWithHander(m_hCursorHelp);
}

BOOL CMyCursorManager::SetCursorWithHander(HCURSOR hCursor)
{
	if( hCursor )
		if( (m_hOldCursor = ::SetCursor(hCursor)) != NULL )
			return TRUE;

	return FALSE;
}