// MyCursorManager.h: interface for the CMyCursorManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYCURSORMANAGER_H__599DEF75_9D58_435C_937B_BFEA13399F6F__INCLUDED_)
#define AFX_MYCURSORMANAGER_H__599DEF75_9D58_435C_937B_BFEA13399F6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyCursorManager  
{
public:
	CMyCursorManager();
	virtual ~CMyCursorManager();

	void LoadStapleCursors();
	HCURSOR SetCustomerCursor(WORD nResID);

	void ResumeToOld();
	void ChangeToDefaultCursor();
	void ChangeToWaitCursor();
	void ChangeToHandCursor();
	void ChangeToHelpCursor();
	
protected:
	BOOL SetCursorWithHander(HCURSOR hCursor);

	HCURSOR m_hOldCursor;
	HCURSOR m_hCursorDefault;
	HCURSOR m_hCursorWait;
	HCURSOR m_hCursorHand;
	HCURSOR m_hCursorHelp;

};

#endif // !defined(AFX_MYCURSORMANAGER_H__599DEF75_9D58_435C_937B_BFEA13399F6F__INCLUDED_)
