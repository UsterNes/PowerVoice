// DataRecorderConnect.cpp : implementation file
//

#include "stdafx.h"
#include "DataRecorderConnect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataRecorderConnect
CDataRecorderConnect::CDataRecorderConnect()
: CFastMessageQ(64, PCS_MAX_MESSAGE_SIZE),
  m_nServerPort(0),
  m_bConnected(FALSE),
  m_hWndOwner(NULL)
{
	memset(&m_rcvhdr, 0x00, sizeof(m_rcvhdr));
	memset(&m_sndhdr, 0x00, sizeof(m_sndhdr));
	memset(m_strServerIP, 0x00, sizeof(m_strServerIP));
	m_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
	m_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
	m_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
	m_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
	m_sndhdr.Receiver = USER_DATA;
	m_DataQueue.SetMaxLength(BUFFER_OVERFLOW);
	SetMQName(_T("DataRecorder"));

	// Sun added 2007-05-22, [A]
	m_tmRecTime = CTime::GetCurrentTime();

	// Create sockect
	Create();
}

CDataRecorderConnect::~CDataRecorderConnect()
{
	Close();
	RemoveAllQueuedSQLStatements();
}

// Sun added 2004-12-09
/// 安全删除所有语句
void CDataRecorderConnect::RemoveAllQueuedSQLStatements()
{
	BYTE lv_msg[PCS_MAX_MESSAGE_SIZE];
	size_t lv_len;

	try
	{
		// Send message
		while( GetMessage(lv_msg, &lv_len) )
		{
			WriteToFile((LPCSTR)(lv_msg+sizeof(SMsgHeader)), lv_len - sizeof(SMsgHeader));
			RemoveMessage();
		}
	}
	catch(...)
	{
		WriteTrace(TraceError, _T("CDataRecorderConnect::RemoveAllQueuedSQLStatements() - Unexpected exception"));
	}
}

void CDataRecorderConnect::SetSenderParty(UC bytSender)
{
	m_sndhdr.Sender = bytSender;
}

void CDataRecorderConnect::ChangeQueueSize(int nNewSize)
{
	if( GetMQMaxLength() != nNewSize )
		ReclaimQueueLength(nNewSize, PCS_MAX_MESSAGE_SIZE);
}

BOOL CDataRecorderConnect::WaitForStartupToComplete(LPCSTR strServerIP, UINT nServerPort, HWND hWnd)
{
	strncpy(m_strServerIP, strServerIP, sizeof(m_strServerIP)-1);
	m_nServerPort = nServerPort;
	m_hWndOwner = hWnd;

	// Sun added 2005-09-08
	if( nServerPort > 0 )
	{
		m_hNeedWait.Create();
		m_shutdown.Create();
		m_shutdown.Reset();
		CMyThread::Start();
		InitSock();
	}

	return TRUE;
}

void CDataRecorderConnect::WaitForShutdownToComplete()
{
	// if we havent already started a shut down, do so...
	if( m_shutdown.GetHANDLE() != INVALID_HANDLE_VALUE )
	{
		m_shutdown.Set();
		if( !Wait(2000) )
			WriteTrace(TraceWarn, "Problem while killing Data Recorder Thread!");
	}

	// Close socket
	if( GetConnected() )
		CloseConnection();
}

int CDataRecorderConnect::Run()
{
	BYTE lv_msg[PCS_MAX_MESSAGE_SIZE];
	size_t lv_len;
	DWORD lv_tickStart, lv_tickNow;
	
	try
	{
		lv_tickStart = GetTickCount();
		while( !m_shutdown.Wait(0) )
		{
			if( WaitForSingleObject(m_shutdown.GetHANDLE(), 0) == WAIT_OBJECT_0 )
				break;

			if( GetConnected() )
			{
				// Send message
				while( GetMessage(lv_msg, &lv_len) && !m_shutdown.Wait(0) )
				{
					if( !SendControlData(lv_msg, lv_len) )
					{
						// Sun moved 1 statement 2007-10-05 [B] from [Ln142]
						Sleep(50);

						/// Remove Message
						if( GetRepeatTimes() <= GetMaxRepeatTimes() )
							break;		// Don't remove message
						else
						{
							// Sun ommitted 2007-10-05 [B], [Ln142]
							//Sleep(50);

							// Sun added 2004-11-05
							WriteToFile((LPCSTR)(lv_msg+sizeof(SMsgHeader)), lv_len - sizeof(SMsgHeader));
						}
					}
					RemoveMessage();
				}
			}
			else
			{
				// Recovery connection
				lv_tickNow = GetTickCount();

				//--------------------------------------------
				// Sun added 2007-05-22, [B]
				if( lv_tickNow < lv_tickStart )
					lv_tickStart = lv_tickNow;
				//--------------------------------------------
				else if( lv_tickNow - lv_tickStart > SOCKET_CONNECT_RETRY_INTERVAL )
				{
					WriteTrace(TraceInfo, "SysInfo - prepare to recovery DataRecorder connection.");
					if( !InitSock() )
						WriteTrace(TraceWarn, "SysWarn - failed to initialize socket error code = %d.", GetLastError());

					lv_tickStart = lv_tickNow;
				}
			}

			Sleep(100);
		}
	}
	catch(...)
	{
		WriteTrace(TraceError, _T("CDataRecorderConnect::Run() - Unexpected exception"));
	}
	
	WriteTrace(TraceInfo, "SysInfo - CDataRecorderConnect thread exited!");
	return 0;
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CDataRecorderConnect, CAsyncSocket)
	//{{AFX_MSG_MAP(CDataRecorderConnect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CDataRecorderConnect member functions

void CDataRecorderConnect::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( nErrorCode == 0 )
	{
		AsyncSelect(FD_READ | FD_CLOSE);
		SetConnected(TRUE);
		m_hNeedWait.Set();
	}
	
	CAsyncSocket::OnConnect(nErrorCode);
}

void CDataRecorderConnect::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CloseConnection();
	
	CAsyncSocket::OnClose(nErrorCode);
}

void CDataRecorderConnect::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	try
	{
		BYTE buff[PACKET_SIZE];

		// Sun added 2007-05-22, [A]
		m_tmRecTime = CTime::GetCurrentTime();

		int nRead = Receive(buff, sizeof(buff));
		if( nRead == SOCKET_ERROR )
		{
			if( GetLastError() != WSAEWOULDBLOCK ) 
				WriteTrace(TraceError, "ERROR in CDataRecorderConnect::OnReceive(): %d.", GetLastError());
		}
		else
		{
			if( m_DataQueue.Append(buff, nRead) > 0 )
			{
				GetRequests();
			}
			else
			{
				// No Message
				CloseConnection();
			}
		}
	}
	catch(...)
	{
		// something bad happened... (DOS attack?)
		WriteTrace(TraceError, "Exception occurred in CDataRecorderConnect::OnReceive()!");
		// close the connection.
		CloseConnection();
	}
	
	CAsyncSocket::OnReceive(nErrorCode);
}

BOOL CDataRecorderConnect::InitSock()
{
	// Connect server
	try
	{
		if( Connect(m_strServerIP, m_nServerPort) == 0 
			&& GetLastError() != WSAEWOULDBLOCK )
			return FALSE;

		// Sun added 2008-05-16
		m_tmRecTime = CTime::GetCurrentTime();
	}
	catch(...)
	{
		WriteTrace(TraceError, "SysError - Unexpect error occurred in CDataRecorderConnect::InitSock()!");
		return FALSE;
	}

	// Wait connection established
	m_hNeedWait.Reset();
	if( m_hNeedWait.Wait(SOCKET_WAIT_TIMEOUT) == false )
		return FALSE;

	return TRUE;
}

/********************************************************************/
/*																	*/
/* Function name : CloseConnection									*/
/* Description   : Close connection and signal to end thread		*/
/*																	*/
/********************************************************************/
void CDataRecorderConnect::CloseConnection()
{
	// Clear Buffer
	m_DataQueue.Remove();

	Close();

	SetConnected(FALSE);

	// Create Socket again
	Create();
}

BOOL CDataRecorderConnect::SendControlData(const BYTE *pData, size_t dataLength)
{
	// send data and keep message pump running...
	if( !m_bConnected )
		return FALSE;

	// Sun updated 2007-10-05, [B]
	/// From
	//return CAsyncSocket::Send(pData, dataLength, 0);
	// To
	return( CAsyncSocket::Send(pData, dataLength, 0) != SOCKET_ERROR );
}

BOOL CDataRecorderConnect::GetConnected()
{
	return m_bConnected;
}

void CDataRecorderConnect::SetConnected(BOOL f_sw)
{
	if( m_bConnected != f_sw )
	{
		m_bConnected = f_sw;

		// Post message to the main thread
		if( m_hWndOwner && m_sndhdr.Sender )
		{
			S_WM_WPARAM *lv_pWParam = new S_WM_WPARAM;
			if( lv_pWParam )
			{
				memset(lv_pWParam, 0x00, sizeof(S_WM_WPARAM));
				lv_pWParam->UserType = USER_DATA;
				sprintf(lv_pWParam->PeerName, "%s : %d", m_strServerIP, m_nServerPort);
				lv_pWParam->pThread = 0;

				/// Post message to the main thread
				::PostMessage(m_hWndOwner, (m_bConnected ? WM_CLIENTONLINE : WM_CLIENTOFFLINE), 
					(WPARAM)lv_pWParam, (LPARAM)GetThreadID());
			}
		}
	}
}

/********************************************************************/
/*																	*/
/* Function name: IsSockTimeOut										*/
/* Description  : Assert Socket connection validation.				*/
/*																	*/
/********************************************************************/
// Sun added 2007-05-22, [A]
BOOL CDataRecorderConnect::IsSockTimeOut()
{
	try
	{
		if( GetConnected() )
		{
			CTime lv_curTime = CTime::GetCurrentTime();
			CTimeSpan lv_tmspan;
			lv_tmspan = lv_curTime - m_tmRecTime;
			if( lv_tmspan.GetTotalSeconds() >  DEF_DRC_SCK_MAXKEEPLIVE_INTERVAL )
			{
				CloseConnection();
				WriteTrace(TraceError, "Error - DataRecorder socket is time out");
				return TRUE;
			}
		}
	}
	catch(...)
	{
		WriteTrace(TraceError, _T("DataRecorde::IsSockTimeOut() - CloseConnection Unexpected exception"));
	}

	return FALSE;
}

/********************************************************************/
/*																	*/
/* Function name: AddMessage										*/
/* Description  : Override CFastMessageQ::AddMessage().				*/
/*																	*/
/********************************************************************/
int CDataRecorderConnect::AddMessage(CString f_strSQL)
{
	BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];
	int lv_len = f_strSQL.GetLength();

	if( lv_len + sizeof(SMsgHeader) >= PCS_MAX_MESSAGE_SIZE )
		return 0;

	if( m_nServerPort == 0 )
	{
		// Sun added 2005-09-08
		WriteToFile(f_strSQL, lv_len);

		return 0;
	}

	int lv_retval;

	m_bLock.Enter();

	try
	{
		m_sndhdr.Command = TCP_ALL2DBR_EXECUTE_SQL;
		m_sndhdr.PackageLen = lv_len + sizeof(SMsgHeader);

		memcpy(lv_sndmsg, &m_sndhdr, sizeof(SMsgHeader));
		memcpy(lv_sndmsg+sizeof(SMsgHeader), f_strSQL.GetBuffer(lv_len), lv_len);
		lv_retval = CFastMessageQ::AddMessage(lv_sndmsg, m_sndhdr.PackageLen);
	}
	catch(...)
	{
		WriteTrace(TraceError, "DR AddMessage runs error!");
		lv_retval = 0;
	}

	m_bLock.Leave();

	// Sun added 2004-11-05
	if( !lv_retval )
		WriteToFile(f_strSQL, lv_len);

	return lv_retval;
}

// Sun added for Send Message Easyly
BOOL CDataRecorderConnect::SendProtocolData(US f_nCmd, US f_nLen, UC *f_pData)
{
	BOOL lv_retval;
	SMsgHeader lv_sndhdr;
	BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

	lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
	lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
	lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
	lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
	lv_sndhdr.Sender = m_sndhdr.Sender;
	lv_sndhdr.Receiver = USER_DATA;
	lv_sndhdr.Command = f_nCmd;
	lv_sndhdr.PackageLen = f_nLen + sizeof(SMsgHeader);

	try
	{
		// put the data into the sending buffer
		memset(lv_sndmsg, 0x00, sizeof(lv_sndmsg));
		memcpy(lv_sndmsg, &lv_sndhdr, sizeof(SMsgHeader));
		if( f_nLen > 0 && f_pData )
			memcpy(lv_sndmsg+sizeof(SMsgHeader), f_pData, f_nLen);

		lv_retval = SendControlData(lv_sndmsg, lv_sndhdr.PackageLen);
	}
	catch(...)
	{
		lv_retval = FALSE;
		WriteTrace(TraceError, "Socket - Put the data into the sending buffer failed!");
	}

	return( lv_retval );
}

/********************************************************************/
/*																	*/
/* Function name : GetMinimumMessageSize							*/
/* Description   : A valid message length must larger than it		*/
/*																	*/
/********************************************************************/
size_t CDataRecorderConnect::GetMinimumMessageSize() const
{
   return sizeof(SMsgHeader);
}

size_t CDataRecorderConnect::GetMessageSize()
{
	size_t messageSize = 0;

	// Read message header
	memcpy(&m_rcvhdr, m_DataQueue.GetBuffer(), sizeof(SMsgHeader) );

	// Verify message
	/// Right Message Flag
	if( _mbsnbcmp((BYTE *)&m_rcvhdr, (BYTE *)&m_sndhdr, 4) == 0 )
	{
		/// Right Receiver & Right Sender
		if( m_rcvhdr.Sender == USER_DATA )
			messageSize = m_rcvhdr.PackageLen;
	}
   
	return messageSize;
}
/********************************************************************/
/*																	*/
/* Function name: GetRequests										*/
/* Description  : Parse complete Request line						*/
/*																	*/
/********************************************************************/
void CDataRecorderConnect::GetRequests()
{
	bool done;

	do
	{
		done = true;

		const size_t used = m_DataQueue.Length();

		if( used >= GetMinimumMessageSize() )
		{
			const size_t messageSize = GetMessageSize();

			if( messageSize < GetMinimumMessageSize() )
			{
				// Send Error Message and Close Connetion
				WriteTrace(TraceError, "Invalid message format, connection will be closed!!!");
				CloseConnection();
			}
			else
			{
				if( used >= messageSize )
				{
					// We have a whole, distinct, message
					if( !ProcessRequests() )
					{
						// Send Error Message
						WriteTrace(TraceWarn, "Unknown Message or abuse!");
						m_DataQueue.Remove();
					}
					else
					{
						if( used > messageSize )
						{
							done = false;
							m_DataQueue.Remove(messageSize);
						}
						else
							m_DataQueue.Remove();
					}
				}
				else if( messageSize > (size_t)m_DataQueue.GetMaxLength() )
				{
					// Send Error Message and Close Connetion
					WriteTrace(TraceWarn, "Error: Buffer too small!");
					CloseConnection();
				}
			}
		}
	}
	while( !done );
}

/********************************************************************/
/*																	*/
/* Function name: ProcessRequests									*/
/* Description  : Parse and execute commands from client.			*/
/*																	*/
/********************************************************************/
int CDataRecorderConnect::ProcessRequests()
{
	/// Process message directly
	CString lv_strMsg;

	switch( m_rcvhdr.Command )
	{
	// Keep Live Message
	case TCP_KEEPLIVE_MSG:
		SendProtocolData(TCP_KEEPLIVE_ACK);
		break;
	}

	return 1;
}
