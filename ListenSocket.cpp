/****************************************************************/
/*																*/
/*  LISTENSOCKET.CPP											*/
/*																*/
/*  Implementation of the Listen Socket.						*/
/*  The server listens for connections. When a new connection	*/
/*  is requested, the server accepts the connection and then	*/
/*  creates a connect thread to handle the connection.			*/
/*																*/
/*	This class is a part of the Power Call Server				*/
/*																*/
/*  Programmed by Tony Sun										*/
/*																*/
/*  Last updated: July 4, 2003									*/
/*																*/
/****************************************************************/

#include "stdafx.h"
#include "MainFrm.h"
#include "ListenSocket.h"
#include "ClientThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CListenSocket::CListenSocket()
{
	m_pWndDlg = NULL;
}

CListenSocket::~CListenSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CListenSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CListenSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0


/********************************************************************/
/*																	*/
/* Function name : OnAccept											*/		
/* Description   : Called by the framework to notify this listening	*/
/*				   socket that it can accept pending connection		*/
/*				   requests by calling the Accept member function.	*/
/*																	*/
/********************************************************************/
void CListenSocket::OnAccept(int nErrorCode) 
{
	try
	{
		if( nErrorCode == 0 )
		{
			// New connection is being established
			CSocket sock;

			// Accept the connection using a temp CSocket object.
			Accept(sock);

			if( !pMainWindow->m_bRunning )
			{
				sock.Close();
				return;
			}

			// Create a thread to handle the connection. The thread is created suspended so that we can
			// set variables in CConnectThread before it starts executing.
			CClientThread* pThread = (CClientThread*)AfxBeginThread(RUNTIME_CLASS(CClientThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
			if (!pThread)
			{
				sock.Close();
				WriteTrace(TraceError, "Could not create thread!!");
				return;
			}
			
			CMainFrame* pDlg = (CMainFrame*) m_pWndDlg;

			// set members of CClientThread.m_socket
			pThread->m_hWndOwner = m_pWndDlg->GetSafeHwnd();

			// Pass the socket to the thread by passing the socket handle. You cannot pass
			// a CSocket object across threads.	
			pThread->m_hSocket = sock.Detach();

			// since everything is successful, add the thread to our list
			pDlg->m_ThreadList.AddThread(pThread);

			// Set receive timeout
			int lv_Timeout = 5000;
			setsockopt(pThread->m_hSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&lv_Timeout, sizeof(lv_Timeout));

			// Now start the thread.
			pThread->ResumeThread();
		}
		else
		{
			WriteTrace(TraceError, "CListenSocket::OnAccept(%d) error!!!", nErrorCode);
		}

		CAsyncSocket::OnAccept(nErrorCode);
	}
	catch(...)
	{
		WriteTrace(TraceError, "CListenSocket::OnAccept(%d) catch one error: 0x%X", nErrorCode, GetLastError());
	}
}
