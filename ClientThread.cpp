/****************************************************************/
/*																*/
/*  CLIENTTHREAD.CPP											*/
/*																*/
/*  Implementation of the Client Thread.						*/
/*	Created when a client logs on to the server and processes	*/
/*  'Send' command's.											*/
/*																*/
/*	This class is a part of the Power Call Server				*/
/*																*/
/*  Programmed by Tony Sun										*/
/*																*/
/*  Last updated: July 4, 2003									*/
/*																*/
/****************************************************************/

#include "stdafx.h"
#include "ClientThread.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define SysCst_WelcomeMessage				"Welcome to PowerVoice Server, session begin...\r\n"

IMPLEMENT_DYNCREATE(CClientThread, CWinThread)

CClientThread::CClientThread()
{
	m_dwCheckTick = GetTickCount();
	m_hWndOwner = NULL;
}


CClientThread::~CClientThread()
{
}


BEGIN_MESSAGE_MAP(CClientThread, CWinThread)
	//{{AFX_MSG_MAP(CClientThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/********************************************************************/
/*																	*/
/* Function name: InitInstance										*/		
/* Description  : Initialize thread									*/
/*																	*/
/********************************************************************/
BOOL CClientThread::InitInstance()
{
	CoInitialize(NULL);

	// Attach the socket handle to a CSocket object.
	// This makes sure that the socket notifications are sent to this thread.
	m_ClientSocket.Attach(m_hSocket);
	m_ClientSocket.AsyncSelect(FD_READ | FD_CLOSE);
	m_ClientSocket.m_pThread = this;

	CString strPeerName;
	UINT nPort;
	m_ClientSocket.GetPeerName(strPeerName, nPort);
	m_strPeerName.Format("%s : %d", strPeerName, nPort);
	
	// Post a message to the main thread so that it can update the number of open connections
	SendStatusToMainWindow(WM_THREADSTART);

	// Send Welcome Message
	m_ClientSocket.SendTextMessage(SysCst_WelcomeMessage);

	return TRUE;
}


/********************************************************************/
/*																	*/
/* Function name: ExitInstance										*/		
/* Description  : Clean up and leave.								*/
/*																	*/
/********************************************************************/
int CClientThread::ExitInstance()
{
	// delete this thread from the linked list
	if( pMainWindow != NULL )
		pMainWindow->m_ThreadList.RemoveThread(this);

	// Post message to the main thread that this socket connection has closed
	SendStatusToMainWindow(WM_THREADCLOSE);

	CoUninitialize();
	return CWinThread::ExitInstance();
}

CString CClientThread::GetPeerName()
{
	return m_strPeerName;
}

void CClientThread::SendStatusToMainWindow(UINT Msg, BYTE UserType, UL ClientID)
{
	// dynamically allocate memory for IP address, port and client type(receiver will delete it!)
	S_WM_WPARAM *lv_pWParam = new S_WM_WPARAM;
	if( lv_pWParam )
	{
		memset(lv_pWParam, 0x00, sizeof(S_WM_WPARAM));
		lv_pWParam->UserType = UserType;
		int nLength = m_strPeerName.GetLength();
		strncpy( lv_pWParam->PeerName, m_strPeerName.GetBuffer(nLength), __min(nLength, sizeof(lv_pWParam->PeerName) - 1) );
		_itoa(ClientID, lv_pWParam->ClientName, 10);
		lv_pWParam->pThread = (DWORD)this;

		/// Post message to the main thread
		::PostMessage(m_hWndOwner, Msg, (WPARAM)lv_pWParam, (LPARAM)m_nThreadID);
	}
}

// Sun updated 2005-04-25
BOOL CClientThread::OnIdle(LONG lCount) 
{
	//---------------------------------------------------
	// Process outbound MQ
	//---------------------------------------------------
	BYTE lv_msg[PCS_MAX_MESSAGE_SIZE];
	size_t lv_len;
	DWORD lv_dwTick;

	lv_dwTick = GetTickCount();

	/// Get Message
	if( !m_ClientSocket.GetMessage(lv_msg, &lv_len) )
	{
		//// Check Connection
		if( (lv_dwTick - m_dwCheckTick) / 1000 >= DEF_SCK_KEEPLIVE_INTERVAL )
		{
			m_dwCheckTick = lv_dwTick;
			if( m_ClientSocket.GetConnectionLength() > DEF_SCK_MAX_REGISTER_DELAY &&
					m_ClientSocket.GetBindingLength() == 0 )
			{
				WriteTrace(TraceWarn, "Client: %s register timeout, and be closed forcely (lcount=%d).", m_strPeerName, lCount);
				m_ClientSocket.CloseConnection();
				return 0;
			}
			else if( !m_ClientSocket.KeepLiveChecking() )
			{
				WriteTrace(TraceWarn, "Client: %s keeplive timeout, and be closed forcely (lcount=%d).", m_strPeerName, lCount);
				m_ClientSocket.CloseConnection();
				return 0;
			}
		}
	
		return CWinThread::OnIdle(lCount);
	}

	/// Send Message
	while( true )
	{
		if( lv_len >= m_ClientSocket.GetMinimumMessageSize() )
		{
			if( !m_ClientSocket.SendControlData(lv_msg, lv_len) )
			{
				/// Remove Message
				//if( m_ClientSocket.GetRepeatTimes() <= m_ClientSocket.GetMaxRepeatTimes() )
				//	break;		// Don't remove message
				// Debuging
				WriteTrace(TraceError, "Socket Error: TCP connection: %s runs error (lcount=%d)!!!", m_strPeerName, lCount);
				m_ClientSocket.CloseConnection();
				return 0;
			}
		}

		//// Remove Message
		m_ClientSocket.RemoveMessage();
		break;
	}

	return m_ClientSocket.GetMQLength();
//	return CWinThread::OnIdle(lCount);
}
