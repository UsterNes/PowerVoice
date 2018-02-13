// PCSExtClientSock.cpp: implementation of the CPCSExtClientSock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PowerVoice.h"
#include "PCSExtClientSock.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CPCSExtClientSock

CPCSExtClientSock::CPCSExtClientSock()
: CFastMessageQ(256, PCS_MAX_MESSAGE_SIZE),
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
	m_sndhdr.Sender = USER_IVR;
	m_sndhdr.Receiver = USER_EXT_IVR;
	m_DataQueue.SetMaxLength(BUFFER_OVERFLOW);

	SetMQName(_T("EXT"));

	// Sun added 2007-10-05, [A]
	/// max send time per message = 20 * 50 ms = 1s
	CFastMessageQ::SetMaxRepeatTimes(20);

	// Create sockect
	Create();
}

CPCSExtClientSock::~CPCSExtClientSock()
{
	Close();
}

BOOL CPCSExtClientSock::WaitForStartupToComplete(LPCSTR strServerIP, UINT nServerPort, HWND hWnd)
{
	strncpy(m_strServerIP, strServerIP, sizeof(m_strServerIP)-1);
	m_nServerPort = nServerPort;
	m_hWndOwner = hWnd;

	m_hNeedWait.Create();
	m_shutdown.Create();
	m_shutdown.Reset();
	CMyThread::Start();
	InitSock();

	return TRUE;
}

void CPCSExtClientSock::WaitForShutdownToComplete()
{
	// if we havent already started a shut down, do so...
	if( m_shutdown.GetHANDLE() != INVALID_HANDLE_VALUE )
	{
		m_shutdown.Set();
		if( !Wait(2000) )
			WriteTrace(TraceWarn, "Problem while killing CTI Connection Thread!");
	}

	// Close socket
	if( GetConnected() )
		CloseConnection();
}

int CPCSExtClientSock::Run()
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
						// Sun moved 1 statement 2007-10-05 [A] from [Ln110]
						Sleep(50);

						/// Remove Message
						if( GetRepeatTimes() <= GetMaxRepeatTimes() )
							break;		// Don't remove message
						// Sun ommitted 2007-10-05 [A], [Ln110]
						//else
						//	Sleep(20);
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
					InitSock();
					lv_tickStart = lv_tickNow;
				}
			}

			Sleep(50);
		}
	}
	catch(...)
	{
		WriteTrace(TraceError, _T("CPCSExtClientSock::Run() - Unexpected exception"));
	}
	
	return 0;
}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CPCSExtClientSock, CAsyncSocket)
	//{{AFX_MSG_MAP(CPCSExtClientSock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CPCSExtClientSock member functions
void CPCSExtClientSock::OnConnect(int nErrorCode) 
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

void CPCSExtClientSock::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CloseConnection();

	CAsyncSocket::OnClose(nErrorCode);
}

void CPCSExtClientSock::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	try
	{
		BYTE buff[PACKET_SIZE];

		int nRead = Receive(buff, sizeof(buff));
		if( nRead == SOCKET_ERROR )
		{
			if( GetLastError() != WSAEWOULDBLOCK ) 
				WriteTrace(TraceError, "ERROR in OnReceive(): %d.", GetLastError());
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
		WriteTrace(TraceError, "Exception occurred in CAsyncSocket::OnReceive()!");
		// close the connection.
		CloseConnection();
	}
	
	CAsyncSocket::OnReceive(nErrorCode);
}

BOOL CPCSExtClientSock::InitSock()
{
	// Connect server
	try
	{
		if( Connect(m_strServerIP, m_nServerPort) == 0 
			&& GetLastError() != WSAEWOULDBLOCK )
			return FALSE;
	}
	catch(...)
	{
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
void CPCSExtClientSock::CloseConnection()
{
	// Clear Buffer
	m_DataQueue.Remove();

	Close();

	SetConnected(FALSE);

	// Create Socket again
	Create();
}

BOOL CPCSExtClientSock::SendControlData(const BYTE *pData, size_t dataLength)
{
	// send data and keep message pump running...
	if( !m_bConnected )
		return FALSE;

	// Sun updated 2007-10-05, [A]
	/// From
	//return CAsyncSocket::Send(pData, dataLength, 0);
	// To
	return( CAsyncSocket::Send(pData, dataLength, 0) != SOCKET_ERROR );
}

BOOL CPCSExtClientSock::GetConnected()
{
	return m_bConnected;
}

void CPCSExtClientSock::SetConnected(BOOL f_sw)
{
	if( m_bConnected != f_sw )
	{
		m_bConnected = f_sw;

		// Post message to the main thread
		if( m_hWndOwner && !pMainWindow->m_bShutdown )
		{
			S_WM_WPARAM *lv_pWParam = new S_WM_WPARAM;
			if( lv_pWParam )
			{
				memset(lv_pWParam, 0x00, sizeof(S_WM_WPARAM));
				lv_pWParam->UserType = USER_EXT_IVR;
				sprintf(lv_pWParam->PeerName, "%s : %d", m_strServerIP, m_nServerPort);
				lv_pWParam->pThread = 0;

				/// Post message to the main thread
				::PostMessage(m_hWndOwner, (m_bConnected ? WM_CLIENTONLINE : WM_CLIENTOFFLINE), 
					(WPARAM)lv_pWParam, (LPARAM)GetThreadID());
			}
		}
	}
}

BOOL CPCSExtClientSock::SendProtocolData(US f_nCmd, US f_nLen, UC *f_pData)
{
	BOOL lv_retval;
	SMsgHeader lv_sndhdr;
	BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

	lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
	lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
	lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
	lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
	lv_sndhdr.Sender = USER_IVR;
	lv_sndhdr.Receiver = USER_EXT_IVR;
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
/* Function name: AddMessage										*/
/* Description  : Override CFastMessageQ::AddMessage().				*/
/*																	*/
/********************************************************************/
int CPCSExtClientSock::AddMessage(const BYTE *f_data, size_t f_len, DWORD f_Tag)
{
	if( !m_bConnected )
		return 0;

	return CFastMessageQ::AddMessage(f_data, f_len, f_Tag);
}

/********************************************************************/
/*																	*/
/* Function name : GetMinimumMessageSize							*/
/* Description   : A valid message length must larger than it		*/
/*																	*/
/********************************************************************/
size_t CPCSExtClientSock::GetMinimumMessageSize() const
{
   return sizeof(SMsgHeader);
}

size_t CPCSExtClientSock::GetMessageSize()
{
	size_t messageSize = 0;

	// Read message header
	memcpy(&m_rcvhdr, m_DataQueue.GetBuffer(), sizeof(SMsgHeader) );

	// Verify message
	/// Right Message Flag
	if( _mbsnbcmp((BYTE *)&m_rcvhdr, (BYTE *)&m_sndhdr, 4) == 0 )
	{
		/// Right Receiver & Right Sender
		if( (m_rcvhdr.Receiver == USER_IVR || m_rcvhdr.Receiver == USER_DUMMY ) 
			&& m_rcvhdr.Sender == USER_EXT_IVR )
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
void CPCSExtClientSock::GetRequests()
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
int CPCSExtClientSock::ProcessRequests()
{
	/// Process message directly
	CString lv_strMsg;

	switch( m_rcvhdr.Command )
	{
	// Keep Live Message
	case TCP_KEEPLIVE_MSG:
		SendProtocolData(TCP_KEEPLIVE_ACK);
		break;

	// 注册响应消息
	case TCP_EXT2IVR_REGISTER_ACK:
		{
			S_TCP_EXT2IVR_REGISTER_ACK lv_rcvbdy;
			
			/// Message Analysis
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, m_DataQueue.GetBuffer()+sizeof(SMsgHeader), sizeof(lv_rcvbdy));
			
			switch( lv_rcvbdy.ErrorCode )
			{
			case PCS_DEF_RETCODE_BEUSED:
				lv_strMsg = _T("IVR Ext failed to register, some others has already registered on Server!");
				break;
			case PCS_DEF_RETCODE_VERSION:
				lv_strMsg = _T("IVR Ext failed to register, check the version of the appliction!");
				break;
			case PCS_DEF_RETCODE_REGCODE:
				lv_strMsg = _T("IVR Ext failed to register, check the register code!");
				break;
			case PCS_DEF_RETCODE_STATUS:
				lv_strMsg = _T("IVR Ext register OK, however the Server is pending!");
				break;
			case PCS_DEF_RETCODE_OTHERS:
				lv_strMsg = _T("IVR Ext failed to register for an unknown error, call the software vendor!");
				break;
			case PCS_DEF_RETCODE_SYSERROR:
				lv_strMsg = _T("IVR Ext failed to register for a system error, call the software vendor!");
				break;
			case PCS_DEF_RETCODE_SUCC:
				lv_strMsg.Format("IVR Ext successfully registered on Server [%s : %d]", m_strServerIP, m_nServerPort);
				break;
			}

			WriteTrace(TraceInfo, lv_strMsg);
		}
		break;

	// 取消注册响应消息
	case TCP_EXT2IVR_UNREGISTER_ACK:
		lv_strMsg = _T("IVR Ext successfully unregistered from Server.");
		WriteTrace(TraceInfo, lv_strMsg);
		break;

	//--------------------------------------------------------------
	// 外拨控制消息
	//--------------------------------------------------------------
	// 增加拨号任务
	case TCP_EXT2IVR_TASK_ADD:
		{
			S_TCP_EXT2IVR_TASK_ADD *lv_pbdy;
			S_TCP_IVR2EXT_TAST_ADD_ACK lv_sndbdy;
			CIVRGroup *lv_pGroup;
			CIVRChannel *lv_pCH;
			char lv_szData[PCS_MAX_MESSAGE_SIZE];
			BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];
			
			/// Message Analysis
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(S_TCP_EXT2IVR_TASK_ADD) )
				return 0;
			lv_pbdy = (S_TCP_EXT2IVR_TASK_ADD *)(m_DataQueue.GetBuffer()+sizeof(SMsgHeader));
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(S_TCP_EXT2IVR_TASK_ADD) + lv_pbdy->TelNoLength + lv_pbdy->NotesLength )
				return 0;

			WriteTrace(TraceDebug, "Receive OB task: %d-%d for group:%d.", 
				lv_pbdy->CampaignID, lv_pbdy->TaskID, lv_pbdy->GroupNO);

			/// Prepare Send Message
			m_sndhdr.Command = TCP_IVR2EXT_TAST_ADD_ACK;
			m_sndhdr.PackageLen = sizeof(SMsgHeader) + sizeof(lv_sndbdy);
			memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));

			lv_sndbdy.PacketID = lv_pbdy->PacketID;
			lv_sndbdy.GroupNO = lv_pbdy->GroupNO;
			lv_sndbdy.CampaignID = lv_pbdy->CampaignID;
			lv_sndbdy.TaskID = lv_pbdy->TaskID;
			lv_sndbdy.ErrorCode = PCS_DEF_RETCODE_NOTEXIST;

			/// 找到组
			if( lv_pbdy->GroupNO >= 0 && lv_pbdy->GroupNO < theApp.m_nIVRGroups )
			{
				if( (lv_pGroup = PIVRGroup[lv_pbdy->GroupNO]) != NULL )
				{
					/// 找空闲通道
					lv_pCH = lv_pGroup->MoveToNextFlagCH();
					if( lv_pCH == NULL )
						lv_sndbdy.ErrorCode = PCS_DEF_RETCODE_STATUS;
					else
					{
						lv_sndbdy.ErrorCode = PCS_DEF_RETCODE_SUCC;

						/// 设置外拨信息
						lv_pCH->NF_SetOBPacketID(lv_pbdy->PacketID);
						lv_pCH->NF_SetOBCampaignID(lv_pbdy->CampaignID);
						lv_pCH->NF_SetOBTaskID(lv_pbdy->TaskID);

						memcpy(lv_szData, 
							m_DataQueue.GetBuffer()+sizeof(SMsgHeader)+sizeof(S_TCP_EXT2IVR_TASK_ADD),
							lv_pbdy->TelNoLength);
						lv_szData[lv_pbdy->TelNoLength] = NULL;
						lv_pCH->NF_SetOBMsgTelNo(lv_szData);

						WriteTrace(TraceDetail, ".......OB task: %d-%d for group:%d OB TelNum:%s, assign IVR Channel: %d.", 
							lv_pbdy->CampaignID, lv_pbdy->TaskID, lv_pbdy->GroupNO, lv_szData, lv_pCH->GetLineNO());

						memcpy(lv_szData, 
							m_DataQueue.GetBuffer()+sizeof(SMsgHeader)+sizeof(S_TCP_EXT2IVR_TASK_ADD)+lv_pbdy->TelNoLength,
							lv_pbdy->NotesLength);
						lv_szData[lv_pbdy->NotesLength] = NULL;
						lv_pCH->NF_SetExtData(lv_szData, lv_pbdy->NotesLength);

						/// Callout Flow
						lv_pCH->NF_CallIn(lv_pbdy->ServiceCode, lv_pbdy->StartNode);
					}
				}
			}

			//// Send Message
			memcpy(lv_sndmsg, &m_sndhdr, sizeof(SMsgHeader));
			memcpy(lv_sndmsg+sizeof(SMsgHeader), &lv_sndbdy, sizeof(lv_sndbdy));
			SendControlData(lv_sndmsg, m_sndhdr.PackageLen);
		}
		break;

	// 删除拨号任务
	case TCP_EXT2IVR_TASK_REMOVE:
		break;

	// 删除所有任务
	case TCP_EXT2IVR_TASK_REMOVE_ALL:
		break;

	// 查询拨号任务状态
	case TCP_EXT2IVR_TASK_QUERY:
		break;

	// 查询拨号任务列表
	case TCP_EXT2IVR_TASK_LIST_QUERY:
		break;

	//--------------------------------------------------------------
	// 通用数据包结构
	//--------------------------------------------------------------
	default:
		{
			S_TCP_IVREXT_PACKGET *lv_pbdy;
			CIVRChannel *lv_pAgent = NULL;
			
			/// Message Analysis
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(S_TCP_IVREXT_PACKGET) )
				return 0;
			lv_pbdy = (S_TCP_IVREXT_PACKGET *)(m_DataQueue.GetBuffer()+sizeof(SMsgHeader));
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(S_TCP_IVREXT_PACKGET) + lv_pbdy->NotesLength + lv_pbdy->VariableLength )
				return 0;
	
			WriteTrace(TraceDebug, "Receive external msg: %d, length:%d, IVR Channel: %d.", 
				m_rcvhdr.Command, m_rcvhdr.PackageLen, lv_pbdy->ChannelNO);

			try
			{
				if( lv_pbdy->ChannelNO >= 0 && lv_pbdy->ChannelNO < theApp.m_nIVRChannels )
				{
					lv_pAgent = PIVRCH[lv_pbdy->ChannelNO];
					if( lv_pAgent )
						if( lv_pAgent->GetStarted() )
						{
							memcpy(lv_pAgent->m_CommEvent.m_pData, 
								m_DataQueue.GetBuffer() + sizeof(SMsgHeader), 
								m_rcvhdr.PackageLen - sizeof(SMsgHeader));
							lv_pAgent->m_CommEvent.Set();
						}
				}
			}
			catch(...)
			{
				WriteTrace(TraceError, "Sock process general external msg: %d goes error on IVR Channel: %d", 
					m_rcvhdr.Command, lv_pbdy->ChannelNO);
			}
		}
		break;
	}

	return 1;
}
