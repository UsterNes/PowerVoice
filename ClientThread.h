#if !defined(AFX_CLIENTTHREAD_H__B7C54BD2_A555_11D0_8996_00AA00B92B2E__INCLUDED_)
#define AFX_CLIENTTHREAD_H__B7C54BD2_A555_11D0_8996_00AA00B92B2E__INCLUDED_

#include "ClientSocket.h"

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CClientThread : public CWinThread
{
	DECLARE_DYNCREATE(CClientThread)

// Operations
public:
	CString GetPeerName();
	void SendStatusToMainWindow(UINT Msg, BYTE UserType = 0, UL ClientID = 0);

	// Used to pass the socket handle from the main thread to this thread.
	SOCKET m_hSocket;
	
	HWND m_hWndOwner;

	// CSocket derived class that handles the connection.
	CClientSocket m_ClientSocket;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_strPeerName;
		
	CClientThread();           // protected constructor used by dynamic creation
	virtual ~CClientThread();

	// Generated message map functions
	//{{AFX_MSG(CClientThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	DWORD m_dwCheckTick;
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTTHREAD_H__B7C54BD2_A555_11D0_8996_00AA00B92B2E__INCLUDED_)
