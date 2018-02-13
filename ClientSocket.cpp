/********************************************************************/
/*																	*/
/*  CLIENTSOCKET.CPP												*/
/*																	*/
/*  Implementation of the Client Socket.							*/
/*	This class is a part of the CClientThread which handles			*/
/*  socket connections. Incomming data is processed in OnReceive	*/
/*																	*/
/*  Programmed by Tony Sun											*/
/*																	*/
/*																	*/
/*  Last updated: July 4, 2003										*/
/*																	*/
/********************************************************************/

#include "stdafx.h"
#include "PowerVoice.h"
#include "MainFrm.h"
#include "ClientSocket.h"
#include "ClientThread.h"
#include "IVRClient_Admin.h"
#include "IVRClient_MSI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CClientSocket::CClientSocket()
 : CFastMessageQ(SOCKET_DEFAULT_MQ_LENGTH, PCS_MAX_MESSAGE_SIZE)
{
	m_tmConnected = CTime::GetCurrentTime();
	m_DataQueue.SetMaxLength(BUFFER_OVERFLOW);
	
	memset(&m_rcvhdr, 0x00, sizeof(m_rcvhdr));
	memset(&m_sndhdr, 0x00, sizeof(m_sndhdr));

	m_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
	m_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
	m_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
	m_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
	m_sndhdr.Sender = USER_IVR;
	m_sndhdr.Receiver = USER_DUMMY;

	memcpy(&m_keeplivemsg, &m_sndhdr, sizeof(SMsgHeader));
	m_keeplivemsg.Receiver = USER_DUMMY;
	m_keeplivemsg.Command = TCP_KEEPLIVE_MSG;
	m_keeplivemsg.PackageLen = sizeof(SMsgHeader);

	m_bytWorkObjType = USER_NULL;
	m_pWorkObjPtr = NULL;
}

CClientSocket::~CClientSocket()
{
	// Clear Buffer
	m_DataQueue.ClearBuffer();
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSocket, CSocket)
	//{{AFX_MSG_MAP(CClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0


// send WM_QUIT message to the thread containing the socket
// to shutdown once the connection is closed.
void CClientSocket::OnClose(int nErrorCode) 
{
	// Unbind with any object
	SetWorkObject();

	// Signal the user interface thread to exit
	m_pThread->PostThreadMessage(WM_QUIT,0,0);

	// Do close procedure
	CSocket::OnClose(nErrorCode);
}


/********************************************************************/
/*																	*/
/* Function name : OnReceive										*/
/* Description   : Called by the framework to notify this socket	*/
/*                 that there is data in the buffer.				*/
/*																	*/
/********************************************************************/
void CClientSocket::OnReceive(int nErrorCode) 
{
	try
	{
		BYTE buff[PACKET_SIZE];

		int nRead = Receive(buff, sizeof(buff));
		if( nRead == SOCKET_ERROR )
		{
			if (GetLastError() != WSAEWOULDBLOCK) 
				WriteTrace(TraceError, "ERROR in OnReceive(): %d at %s", GetLastError(), m_pThread->GetPeerName());
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
				Close();
			}
		}
	}
	catch(...)
	{
		// something bad happened... (DOS attack?)
		WriteTrace(TraceError, "Exception occurred in CSocket::OnReceive() at %s!", m_pThread->GetPeerName());
		// close the connection.
		CloseConnection();
	}
	CSocket::OnReceive(nErrorCode);
}


/********************************************************************/
/*																	*/
/* Function name : CloseConnection									*/
/* Description   : Close connection and signal to end thread		*/
/*																	*/
/********************************************************************/
void CClientSocket::CloseConnection()
{
	// Try to Clear Binding
	SetWorkObject();

	// Clear Buffer Data
	m_DataQueue.Remove();

	Close();

	// tell thread to shutdown
	m_pThread->PostThreadMessage(WM_QUIT,0,0);
}

DWORD CClientSocket::GetConnectionLength()
{
	CTimeSpan lv_duration;
	lv_duration = CTime::GetCurrentTime() - m_tmConnected;

	return lv_duration.GetTotalSeconds();
}

long CClientSocket::GetBindingLength()
{
	long lv_dur = 0;
	CTimeSpan lv_duration;

	m_sync.Enter();

	if( m_bytWorkObjType != USER_NULL )
	{
		lv_duration = CTime::GetCurrentTime() - m_tmBinding;
		lv_dur = lv_duration.GetTotalSeconds();
	}

	m_sync.Leave();

	return lv_dur;
}

// Sun updated 2005-04-25
BOOL CClientSocket::KeepLiveChecking()
{
	try
	{
		if( m_bKeepLiveCount.GetValue() > 1 )
			return FALSE;

		m_bKeepLiveCount.IncreaseValue();
		if( !SendControlData((UC *)&m_keeplivemsg, m_keeplivemsg.PackageLen) )
		{
			WriteTrace(TraceError, "KeepLiveChecking for connection: %s failed to send message!", m_pThread->GetPeerName());
			return FALSE;
		}
	}
	catch(...)
	{
		WriteTrace(TraceError, "KeepLiveChecking on client error!!!");
	}
	return TRUE;
}

/********************************************************************/
/*																	*/
/* Function name : GetWorkObject									*/
/* Description   : Return associated object of given type			*/
/*																	*/
/********************************************************************/
void *CClientSocket::GetWorkObject(const BYTE f_Type)
{
	void *lv_ptr;

	m_sync.Enter();

	if( f_Type == m_bytWorkObjType )
		lv_ptr = m_pWorkObjPtr;
	else
		lv_ptr = NULL;

	m_sync.Leave();

	return lv_ptr;
}

/********************************************************************/
/*																	*/
/* Function name : GetWorkObject									*/
/* Description   : Return associated object and type				*/
/*																	*/
/********************************************************************/
void *CClientSocket::GetWorkObject(BYTE *f_Type)
{
	void *lv_ptr;

	m_sync.Enter();

	lv_ptr = m_pWorkObjPtr;
	*f_Type = m_bytWorkObjType;

	m_sync.Leave();

	return lv_ptr;
}

/********************************************************************/
/*																	*/
/* Function name : GetWorkObjectType								*/
/* Description   : Return associated object's type					*/
/*																	*/
/********************************************************************/
BYTE CClientSocket::GetWorkObjectType()
{
	BYTE lv_type;

	m_sync.Enter();

	lv_type = m_bytWorkObjType;

	m_sync.Leave();

	return lv_type;
}

/********************************************************************/
/*																	*/
/* Function name : ClearWorkObject									*/
/* Description   : Used on destruction of WorkObject				*/
/*																	*/
/********************************************************************/
void CClientSocket::ClearWorkObject()
{
	m_sync.Enter();
	
	if( m_pWorkObjPtr != NULL )
	{
		// Clear binding
		switch( m_bytWorkObjType )
		{
		case USER_PROGRAM:
// Sun added 2013-04-25
#ifndef SMARTAC_LICENSE
			theApp.m_License.DecreaseLicenseUsedClient(PCS_LIC_IVR_ADMIN);
#else
			SMARTAC_LICENSE->DecreaseLicenseUsedClient(PCS_LIC_IVR_ADMIN);
#endif
			break;

		case USER_AGT_MSI:
// Sun added 2013-04-25
#ifndef SMARTAC_LICENSE
			theApp.m_License.DecreaseLicenseUsedClient(PCS_LIC_IVR_MSI);
#else
			SMARTAC_LICENSE->DecreaseLicenseUsedClient(PCS_LIC_IVR_MSI);
#endif
			break;
		}
	}

	m_pWorkObjPtr = NULL;
	m_bytWorkObjType = USER_NULL;

	m_sync.Leave();
}

/********************************************************************/
/*																	*/
/* Function name : GetWorkObjectType								*/
/* Description   : Return associated object and type				*/
/*																	*/
/********************************************************************/
void CClientSocket::SetWorkObject(void *f_prt, BYTE f_Type)
{
	CIVRClient_MSI *lv_pMSI;
	UL lv_ClientID = 0;
	CString lv_strName;

	m_sync.Enter();
	
	if( m_pWorkObjPtr != NULL )
	{
		// Clear binding
		switch( m_bytWorkObjType )
		{
		case USER_PROGRAM:
			pMainWindow->m_IVRAdmin.SetDisconnected();
			lv_ClientID = 0;

// Sun added 2013-04-25
#ifndef SMARTAC_LICENSE
			theApp.m_License.DecreaseLicenseUsedClient(PCS_LIC_IVR_ADMIN);
#else
			SMARTAC_LICENSE->DecreaseLicenseUsedClient(PCS_LIC_IVR_ADMIN);
#endif
			break;

		case USER_AGT_MSI:
			lv_pMSI = (CIVRClient_MSI *)m_pWorkObjPtr;
			lv_ClientID = lv_pMSI->GetAgentID();
			lv_pMSI->SetDisconnected();

// Sun added 2013-04-25
#ifndef SMARTAC_LICENSE
			theApp.m_License.DecreaseLicenseUsedClient(PCS_LIC_IVR_MSI);
#else
			SMARTAC_LICENSE->DecreaseLicenseUsedClient(PCS_LIC_IVR_MSI);
#endif
			break;
		}

		// Told MainWindow about it
		if( m_bytWorkObjType != 0 )
			m_pThread->SendStatusToMainWindow(WM_CLIENTOFFLINE, m_bytWorkObjType);
	}

	if( f_Type == USER_NULL || f_prt == NULL )
	{
		m_pWorkObjPtr = NULL;
		m_bytWorkObjType = USER_NULL;
	}
	else
	{
		// New binding
		switch( f_Type )
		{
		case USER_PROGRAM:
			pMainWindow->m_IVRAdmin.SetConnected((PVOID)m_pThread);
			lv_ClientID = 0;
			lv_strName = _T("Admin");

// Sun added 2013-04-25
#ifndef SMARTAC_LICENSE
			theApp.m_License.IncreaseLicenseUsedClient(PCS_LIC_IVR_ADMIN);
#else
			SMARTAC_LICENSE->IncreaseLicenseUsedClient(PCS_LIC_IVR_ADMIN);
#endif
			break;

		case USER_AGT_MSI:
			lv_pMSI = (CIVRClient_MSI *)f_prt;
			lv_ClientID = lv_pMSI->GetAgentID();
			lv_strName.Format("Client:%d", lv_ClientID);
			lv_pMSI->SetConnected((PVOID)m_pThread);

// Sun added 2013-04-25
#ifndef SMARTAC_LICENSE
			theApp.m_License.IncreaseLicenseUsedClient(PCS_LIC_IVR_MSI);
#else
			SMARTAC_LICENSE->IncreaseLicenseUsedClient(PCS_LIC_IVR_MSI);
#endif
			break;
		}
		
		SetMQName(lv_strName);
		m_pWorkObjPtr = f_prt;
		m_bytWorkObjType = f_Type;
		m_tmBinding = CTime::GetCurrentTime();

		// Told MainWindow about it
		m_pThread->SendStatusToMainWindow(WM_CLIENTONLINE, f_Type, lv_ClientID);
	}

	m_sync.Leave();
}


/********************************************************************/
/*																	*/
/* Function name : GetMinimumMessageSize							*/
/* Description   : A valid message length must larger than it		*/
/*																	*/
/********************************************************************/
size_t CClientSocket::GetMinimumMessageSize() const
{
   return sizeof(SMsgHeader);
}

size_t CClientSocket::GetMessageSize()
{
	size_t messageSize = 0;

	// Read message header
	memcpy(&m_rcvhdr, m_DataQueue.GetBuffer(), sizeof(SMsgHeader) );

	// Verify message
	/// Right Message Flag
	if( _mbsnbcmp((BYTE *)&m_rcvhdr, (BYTE *)&m_sndhdr, 4) == 0 )
	{
		/// Right Receiver
		if( m_rcvhdr.Receiver == USER_IVR )
		{
			/// Right Sender
			switch( m_rcvhdr.Sender )
			{
			case USER_PROGRAM:				// 流程编辑子系统
			case USER_AGT_MSI:				// 座席卡客户端
				messageSize = m_rcvhdr.PackageLen;
				break;
			}
		}
	}
   
	return messageSize;
}

/********************************************************************/
/*																	*/
/* Function name: GetRequests										*/
/* Description  : Parse complete Request line						*/
/*																	*/
/********************************************************************/
void CClientSocket::GetRequests()
{
	bool done;
	int lv_proc;

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
				SendTextMessage(_T("Invalid message format, connection will be closed!!!"), TRUE);
			}
			else
			{
				if( used >= messageSize )
				{
					// We have a whole, distinct, message
					lv_proc = ProcessRequests();
					if( lv_proc != 1 )
					{
						if( lv_proc == 0 )
						{
							// Send Error Message and Close Connetion
							SendTextMessage(_T("Unknown Message or abuse, connection will be closed!!!"), TRUE);
						}
						else
							CloseConnection();
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
					WriteTrace(TraceWarn, "Error: Buffer too small for peer %s!", m_pThread->GetPeerName());
					// Send Error Message and Close Connetion
					SendTextMessage(_T("Buffer too small, connection will be closed!!!"), TRUE);
				}
			}
		}
	}
	while( !done );
}


/********************************************************************/
/*																	*/
/* Function name : SendTextMessage									*/
/* Description   : Send error information							*/
/*																	*/
/********************************************************************/
BOOL CClientSocket::SendTextMessage(LPCTSTR lpszMessage, BOOL blnClose)
{
	BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];
	int lv_len = __min(lstrlen(lpszMessage), PCS_MAX_MESSAGE_SIZE - sizeof(SMsgHeader) - 1);

	m_sndhdr.Receiver = USER_DUMMY;
	m_sndhdr.Command = TCP_PCS2ALL_TEXTMSG;
	m_sndhdr.PackageLen = sizeof(SMsgHeader) + lv_len + 1;
	
	// put the data into the socket...
	memset(lv_sndmsg, 0x00, sizeof(lv_sndmsg));
	memcpy(lv_sndmsg, &m_sndhdr, sizeof(SMsgHeader));
	memcpy(lv_sndmsg+sizeof(SMsgHeader), lpszMessage, lv_len);
	SendControlData(lv_sndmsg, m_sndhdr.PackageLen);

	if( blnClose )
	{
		WriteTrace(TraceWarn, "ERROR Message: %s Peer: %s", lpszMessage, m_pThread->GetPeerName());

		CloseConnection();
	}

	return TRUE;
}

/********************************************************************/
/*																	*/
/* Function name : SendServerTimeMessage							*/
/* Description   : Send server time									*/
/*																	*/
/********************************************************************/
BOOL CClientSocket::SendServerTimeMessage()
{
	BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

	S_TCP_PCS2ALL_GET_SERVERTIME_ACK lv_sndbdy;
	memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));

	m_sndhdr.Command = TCP_PCS2ALL_GET_SERVERTIME_ACK;
	m_sndhdr.PackageLen = sizeof(SMsgHeader) + sizeof(lv_sndbdy);
	
	// Get server time information
	TIME_ZONE_INFORMATION lv_struTimeZone; 
	GetTimeZoneInformation(&lv_struTimeZone);
	lv_sndbdy.lngTimeZoneBias = lv_struTimeZone.Bias;

	CTime lv_tmNow = CTime::GetCurrentTime();
	strcpy(lv_sndbdy.strDate, lv_tmNow.Format("%Y-%m-%d"));
	strcpy(lv_sndbdy.strTime, lv_tmNow.Format("%H:%M:%S"));

	// Put the data into the socket...
	memcpy(lv_sndmsg, &m_sndhdr, sizeof(SMsgHeader));
	memcpy(lv_sndmsg+sizeof(SMsgHeader), &lv_sndbdy, sizeof(lv_sndbdy));
	SendControlData(lv_sndmsg, m_sndhdr.PackageLen);

	return TRUE;
}

/************************************************************************/
/*																		*/
/* Function name : SendControlData										*/
/* Description   : Send specified data to client						*/
/*																		*/
/************************************************************************/
BOOL CClientSocket::SendControlData(const BYTE *pData, size_t dataLength)
{
	// send data and keep message pump running...
	if( m_hSocket == INVALID_SOCKET)
		return FALSE;

	int nType = 0;
	int nTypeLen = sizeof(int);
	if( !GetSockOpt(SO_TYPE, &nType, &nTypeLen) )
		return FALSE;
	if( nType != SOCK_STREAM )
		return FALSE;

	try
	{
		CSocketFile sockFile(this, FALSE);
		sockFile.Write(pData, dataLength);
		sockFile.Flush();
	}
	catch(...)
	{
		return FALSE;
	}
	return TRUE;
}

/********************************************************************/
/*																	*/
/* Function name: AddMessage										*/
/* Description  : Override CFastMessageQ::AddMessage().				*/
/*																	*/
/********************************************************************/
int CClientSocket::AddMessage(const BYTE *f_data, size_t f_len, DWORD f_Tag)
{
	int lv_retval = CFastMessageQ::AddMessage(f_data, f_len, f_Tag);
	m_pThread->PostThreadMessage(WM_TIMER, 0, 0);
	return lv_retval;
}

/********************************************************************/
/*																	*/
/* Function name: ProcessRequests									*/
/* Description  : Parse and execute commands from client.			*/
/*																	*/
/********************************************************************/
int CClientSocket::ProcessRequests()
{
	/////////////////////////////////
	// Keep Live Message Process
	/////////////////////////////////
	if( m_rcvhdr.Command == TCP_KEEPLIVE_ACK )
	{
		m_bKeepLiveCount.SetValue(0);
		return 1;
	}

	// Answer the peer
	m_sndhdr.Receiver = m_rcvhdr.Sender;

	// Sun added 2005-12-11
	/////////////////////////////////
	// Get Server Time Message Process
	/////////////////////////////////
	if( m_rcvhdr.Command == TCP_ALL2PCS_GET_SERVERTIME )
	{
		SendServerTimeMessage();
		return 1;
	}

	int lv_retval;

	// Process Received Message
	switch( m_rcvhdr.Sender )
	{
	case USER_PROGRAM:				// 流程编辑子系统
		lv_retval = ProcessMsg_Admin();
		break;

	case USER_AGT_MSI:				// 座席卡客户端
		lv_retval = ProcessMsg_Client();
		break;

	default:
		lv_retval = 0;
	}

	return lv_retval;
}

/********************************************************************/
/*																	*/
/* Function name: ProcessMsg_Admin									*/
/* Description  : Porcess Message from IVR Admin.					*/
/*																	*/
/********************************************************************/
int CClientSocket::ProcessMsg_Admin()
{
	BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

	switch( m_rcvhdr.Command )
	{
	/////////////////////////////////
	// Assign Message Process
	/////////////////////////////////
	case TCP_ADM2IVR_REGISTER:						// 注册消息

		S_TCP_ADM2IVR_REGISTER lv_rcvbdy;
		S_TCP_IVR2ADM_REGISTER_ACK lv_sndbdy;

		/// Message Analysis
		if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
			return 0;
		memcpy(&lv_rcvbdy, m_DataQueue.GetBuffer()+sizeof(SMsgHeader), sizeof(lv_rcvbdy));
		
		/// Prepare Sending Message
		m_sndhdr.Command = TCP_IVR2ADM_REGISTER_ACK;
		m_sndhdr.PackageLen = sizeof(SMsgHeader) + sizeof(lv_sndbdy);
		memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));

		/// Verify Version
		if( pMainWindow->m_nVerAdmin > lv_rcvbdy.Version )
		{
			///////////////////////////////////////////////
			//// Invalid Version -> Send Error Message
			///////////////////////////////////////////////

			///// Prepare Sending Message
			lv_sndbdy.ErrorCode = PCS_DEF_RETCODE_VERSION;
		}
		else
		{
			//// ToDo: Verify Register Code
			// if error return DEF_AGT_REGISTER_RETCODE_REGCODE
			//...

			if( pMainWindow->m_IVRAdmin.GetConnected() )
			{
				///////////////////////////////////////////////
				//// IVR Admin is already used -> Send Error Message
				///////////////////////////////////////////////

				///// Prepare Sending Message
				lv_sndbdy.ErrorCode = PCS_DEF_RETCODE_BEUSED;
			}
			else
			{
				///////////////////////////////////////////////
				//// IVR Admin registered successfully
				///////////////////////////////////////////////

				///// Monitor Register
				pMainWindow->m_IVRAdmin.SetVersion( lv_rcvbdy.Version );
				SetWorkObject((void *)&pMainWindow->m_IVRAdmin, USER_PROGRAM);

				///// Prepare Sending Message
				lv_sndbdy.ErrorCode = PCS_DEF_RETCODE_SUCC;
			}
		}

		//// Send Message
		memcpy(lv_sndmsg, &m_sndhdr, sizeof(SMsgHeader));
		memcpy(lv_sndmsg+sizeof(SMsgHeader), &lv_sndbdy, sizeof(lv_sndbdy));
		SendControlData(lv_sndmsg, m_sndhdr.PackageLen);

		break;

	/////////////////////////////////
	// Deassign Message Process
	/////////////////////////////////
	case TCP_ADM2IVR_UNREGISTER:					// 取消注册消息

		///// IVR Admin Unregister
		SetWorkObject();

		/// Prepare Sending Message
		m_sndhdr.Command = TCP_IVR2ADM_UNREGISTER_ACK;
		m_sndhdr.PackageLen = sizeof(SMsgHeader);

		/// Send Message
		memcpy(lv_sndmsg, &m_sndhdr, sizeof(SMsgHeader));
		SendControlData(lv_sndmsg, m_sndhdr.PackageLen);

		return 2;
		break;

	/////////////////////////////////
	// Operation Message Process
	/////////////////////////////////
	default:
		try
		{
			if( pMainWindow->m_IVRAdmin.GetConnected() != (void *)m_pThread )
			{
				// Send Error Message and Close Connetion
				SendTextMessage(_T("Operation Message: IVR Admin binding is obsolete, connection will be closed!!!"));
				return 2;
			}
			else
			{
				// Process message
				return( pMainWindow->m_IVRAdmin.ProcessTCPMessage( m_rcvhdr, 
						m_DataQueue.GetBuffer() + sizeof(SMsgHeader) ) );
			}
		}
		catch(...)
		{
			WriteTrace(TraceError, "Error - IVR Admin message process, pointer for peer %s is invalid!", m_pThread->GetPeerName());
		}

		break;
	}

	return 1;
}

/********************************************************************/
/*																	*/
/* Function name: ProcessMsg_Client									*/
/* Description  : Porcess Message from IVR Client.					*/
/*																	*/
/********************************************************************/
int CClientSocket::ProcessMsg_Client()
{	
	return 1;
}