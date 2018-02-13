// PCSClientSock.cpp : implementation file
//

#include "stdafx.h"
#include "PowerVoice.h"
#include "PCSClientSock.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPCSClientSock
CString GetPCSStatusName(PCS_PCSStatus f_Status)
{
	CString lv_sName = _T("");

	switch( f_Status )
	{
	case PCS_PS_Init:
		lv_sName = _T("Init");
		break;
	case PCS_PS_Active:
		lv_sName = _T("Active");
		break;
	case PCS_PS_Standby:
		lv_sName = _T("Standby");
		break;
	case PCS_PS_Disconnected:
		lv_sName = _T("Disc");
		break;
	case PCS_PS_Closed:
		lv_sName = _T("Closed");
		break;
	default:
		lv_sName = _T("Unknown");
		break;
	}

	return lv_sName;
}

/// 查询UMS接入模块的名称
CString GetUMSModuleName(UC f_nIndex)
{
	CString lv_sCaption;

	switch( f_nIndex )
	{
	case _pcC_UMS_TASK_TYPE_IBCALL:
		lv_sCaption.LoadString(IDS_CAP_UMS_IBCALL);
		break;

	case _pcC_UMS_TASK_TYPE_OBCALL:
		lv_sCaption.LoadString(IDS_CAP_UMS_OBCALL);
		break;

	case _pcC_UMS_TASK_TYPE_GENERAL:
		lv_sCaption.LoadString(IDS_CAP_UMS_GENERAL);
		break;

	case _pcC_UMS_TASK_TYPE_EMAIL:
		lv_sCaption.LoadString(IDS_CAP_UMS_EMAIL);
		break;

	case _pcC_UMS_TASK_TYPE_FAX:
		lv_sCaption.LoadString(IDS_CAP_UMS_FAX);
		break;

	case _pcC_UMS_TASK_TYPE_SMS:
		lv_sCaption.LoadString(IDS_CAP_UMS_SMS);
		break;

	case _pcC_UMS_TASK_TYPE_VMS:
		lv_sCaption.LoadString(IDS_CAP_UMS_VMS);
		break;

	case _pcC_UMS_TASK_TYPE_CB:
		lv_sCaption.LoadString(IDS_CAP_UMS_CALLBACK);
		break;

	case _pcC_UMS_TASK_TYPE_BBS:
		lv_sCaption.LoadString(IDS_CAP_UMS_BBS);
		break;

	case _pcC_UMS_TASK_TYPE_CHAT:
		lv_sCaption.LoadString(IDS_CAP_UMS_WEBCHAT);
		break;

	case _pcC_UMS_TASK_TYPE_WEBCALL:
		lv_sCaption.LoadString(IDS_CAP_UMS_WEBCALL);
		break;

	case _pcC_UMS_TASK_TYPE_WEBCOLLAB:
		lv_sCaption.LoadString(IDS_CAP_UMS_WEBCOLLAB);
		break;

	case _pcC_UMS_TASK_TYPE_VIDEO:
		lv_sCaption.LoadString(IDS_CAP_UMS_VIDEO);
		break;

	default:
		lv_sCaption.LoadString(IDS_CAP_UMS_UNKNOWN);
		break;
	}

	return lv_sCaption;
}

CString GetSubSystemNameString(UC f_nSysID, UC f_nIndex, UC f_nType)
{
	CString lv_sName = _T("");

	switch( f_nSysID )
	{
	case USER_CTI:					// CTI Server
		lv_sName.Format("CTI Server:%d", f_nIndex);
		break;
	case USER_PBX:					// PBX
		lv_sName = _T("PBX Link");
		break;
	case USER_ACD:					// 排队
		lv_sName = _T("Route Manager");
		break;
	case USER_OBS:					// 外拨
		lv_sName = _T("OB Service");
		break;
	case USER_DATA:					// Data Recorder
		lv_sName = _T("Data Recorder");
		break;

	case USER_MSG:					// Messager SERVER
		lv_sName = _T("Power Messager");
		break;
	case USER_ADMIN:				// 参数更新
		lv_sName = _T("Agent Manager");
		break;
	case USER_RECORDER:				// MaxiSense Server
		lv_sName.Format("PowerLogger Server:%d", f_nIndex);
		break;
	case USER_IVR:					// PowerVoice Server
		lv_sName.Format("Power Voice:%d", f_nIndex);
		break;

	// Sun addded 2006-05-20
	case USER_UMS:					// 统一消息接入模块
		lv_sName.Format("UMS:%d (%s)", f_nIndex, 
			GetUMSModuleName(f_nType));
		break;
	}

	return lv_sName;
}

CPCSClientSock::CPCSClientSock()
: CFastMessageQ(256, PCS_MAX_MESSAGE_SIZE),
  m_nServerPort(0),
  m_bConnected(FALSE),
  m_stPCS(PCS_PS_Disconnected),
  m_bytPCSIndex(0),
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
	m_sndhdr.Receiver = USER_CTI;
	m_DataQueue.SetMaxLength(BUFFER_OVERFLOW);

	SetMQName(_T("CTI"));

	// Sun added 2010-07-07
	ResetKeepLiveTickCount();

	// Sun added 2007-10-05, [A]
	/// max send time per message = 20 * 50 ms = 1s
	CFastMessageQ::SetMaxRepeatTimes(20);

	// Create sockect
	Create();
}

CPCSClientSock::~CPCSClientSock()
{
	Close();
}

BOOL CPCSClientSock::WaitForStartupToComplete(LPCSTR strServerIP, UINT nServerPort, HWND hWnd)
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

void CPCSClientSock::WaitForShutdownToComplete()
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

int CPCSClientSock::Run()
{
	SMsgHeader lv_sndhdr;
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
						// Sun moved 1 statement 2007-10-05 [A] from [Ln111]
						Sleep(50);

						/// Remove Message
						if( GetRepeatTimes() <= GetMaxRepeatTimes() )
							break;		// Don't remove message
						else
						{
							// Sun ommitted 2007-10-05 [A], [Ln111]
							//Sleep(20);

							//----------------------------------------------
							// Sun added 2006-10-01
							if( lv_len >= sizeof(SMsgHeader) )
								memcpy(&lv_sndhdr, &lv_msg, sizeof(SMsgHeader));
							else
								memset(&lv_sndhdr, 0x00, sizeof(SMsgHeader));
							WriteTrace(TraceWarn, "Warning - CPCSClientSock() failed to send messsage len:%d [packlen:%d, cmd:%d]!!!", 
								lv_len, lv_sndhdr.PackageLen, lv_sndhdr.Command);
							//----------------------------------------------
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
					InitSock();
					lv_tickStart = lv_tickNow;
				}
			}

			Sleep(50);
		}
	}
	catch(...)
	{
		WriteTrace(TraceError, _T("CPCSClientSock::Run() - Unexpected exception"));
	}
	
	return 0;
}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CPCSClientSock, CAsyncSocket)
	//{{AFX_MSG_MAP(CPCSClientSock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CPCSClientSock member functions
void CPCSClientSock::OnConnect(int nErrorCode) 
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

void CPCSClientSock::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CloseConnection();

	CAsyncSocket::OnClose(nErrorCode);
}

void CPCSClientSock::OnReceive(int nErrorCode) 
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
				// Sun added 2010-07-07
				ResetKeepLiveTickCount();

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

BOOL CPCSClientSock::InitSock()
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
void CPCSClientSock::CloseConnection()
{
	// Clear Buffer
	m_DataQueue.Remove();

	Close();

	SetConnected(FALSE);

	// Create Socket again
	Create();
}

BOOL CPCSClientSock::SendControlData(const BYTE *pData, size_t dataLength)
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

BOOL CPCSClientSock::GetConnected()
{
	return m_bConnected;
}

void CPCSClientSock::SetConnected(BOOL f_sw)
{
	if( m_bConnected != f_sw )
	{
		// Sun added 2010-07-07
		ResetKeepLiveTickCount();

		m_bConnected = f_sw;

		// Sun added 2010-05-18 [V7.1.8.3]
		if( f_sw )
			SetPCSStatus(PCS_PS_Init);
		else
			SetPCSStatus(PCS_PS_Disconnected);

		// Post message to the main thread
		if( m_hWndOwner && !pMainWindow->m_bShutdown )
		{
			S_WM_WPARAM *lv_pWParam = new S_WM_WPARAM;
			if( lv_pWParam )
			{
				memset(lv_pWParam, 0x00, sizeof(S_WM_WPARAM));
				lv_pWParam->UserType = USER_CTI;
				sprintf(lv_pWParam->PeerName, "%s : %d", m_strServerIP, m_nServerPort);
				lv_pWParam->pThread = 0;

				// Sun added 2010-05-18 [V7.1.8.3]
				lv_pWParam->dwTag = m_bytPCSIndex;

				/// Post message to the main thread
				::PostMessage(m_hWndOwner, (m_bConnected ? WM_CLIENTONLINE : WM_CLIENTOFFLINE), 
					(WPARAM)lv_pWParam, (LPARAM)GetThreadID());
			}
		}
	}
}

// Sun added 2010-05-18 [V7.1.8.3]
PCS_PCSStatus CPCSClientSock::GetPCSStatus()
{
	return m_stPCS;
}

// Sun added 2010-05-18 [V7.1.8.3]
void CPCSClientSock::SetPCSStatus(PCS_PCSStatus f_sw)
{
    if( m_stPCS != f_sw )
	{
		CString lv_sTrace;
		lv_sTrace.Format("Connect to server id(%d) at %s:%d switched from %s to %s", 
			m_bytPCSIndex, m_strServerIP, m_nServerPort, GetPCSStatusName(m_stPCS), GetPCSStatusName(f_sw));

		m_stPCS = f_sw;
		WriteTrace(TraceWarn, "%s", lv_sTrace);

		if( f_sw == PCS_PS_Active || f_sw == PCS_PS_Standby )
			pMainWindow->WriteStatusBar(STATUS_BAR_COMM, ICON_Ready);
		else
			pMainWindow->WriteStatusBar(STATUS_BAR_COMM, ICON_Stop);

		// Sun added 2013-01-14
		if( f_sw == PCS_PS_Active )
			pMainWindow->SetCTIOpened(TRUE, TRUE);
	}

	/// 发送PCS选择改变消息
	S_TCP_ALL2PCS_PCSSELECTCHANGED_EVT lv_sndbdy;
	lv_sndbdy.PCS0Status = pMainWindow->m_sckPCServer.GetPCSStatus();
	lv_sndbdy.PCS1Status = pMainWindow->m_sckPCServer2.GetPCSStatus();
	if( lv_sndbdy.PCS0Status == PCS_PS_Active )
		lv_sndbdy.ActivePCSID = pMainWindow->m_sckPCServer.GetPCSIndex();
	else if( lv_sndbdy.PCS1Status == PCS_PS_Active )
		lv_sndbdy.ActivePCSID = pMainWindow->m_sckPCServer2.GetPCSIndex();
	else
		lv_sndbdy.ActivePCSID = -1;
	SendProtocolData(TCP_ALL2PCS_PCSSELECTCHANGED_EVT, sizeof(lv_sndbdy), (UC *)&lv_sndbdy);
}

// Sun added 2010-05-18 [V7.1.8.3]
UC CPCSClientSock::GetPCSIndex()
{
	return m_bytPCSIndex;
}

// Sun added 2010-05-18 [V7.1.8.3]
void CPCSClientSock::SetPCSIndex(BYTE bytIndex)
{
	if( m_bytPCSIndex != bytIndex )
		m_bytPCSIndex = bytIndex;
}

// Sun added 2010-05-18 [V7.1.8.3]
BOOL CPCSClientSock::CompareThreadID(DWORD dwThreadID)
{
	if( GetThreadID() == dwThreadID )
		return TRUE;

	return FALSE;
}

BOOL CPCSClientSock::SendProtocolData(US f_nCmd, US f_nLen, UC *f_pData)
{
	BOOL lv_retval;
	SMsgHeader lv_sndhdr;
	BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];

	// Sun added 2010-05-18 [V7.1.8.3]
	memset(&lv_sndhdr, 0x00, sizeof(lv_sndhdr));

	lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
	lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
	lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
	lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
	lv_sndhdr.Sender = USER_IVR;
	lv_sndhdr.Receiver = USER_CTI;
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
int CPCSClientSock::AddMessage(const BYTE *f_data, size_t f_len, DWORD f_Tag)
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
size_t CPCSClientSock::GetMinimumMessageSize() const
{
   return sizeof(SMsgHeader);
}

size_t CPCSClientSock::GetMessageSize()
{
	size_t messageSize = 0;

	// Read message header
	memcpy(&m_rcvhdr, m_DataQueue.GetBuffer(), sizeof(SMsgHeader) );

	// Verify message
	/// Right Message Flag
	if( _mbsnbcmp((BYTE *)&m_rcvhdr, (BYTE *)&m_sndhdr, 4) == 0 )
	{
		/// Right Receiver & Right Sender
		if( (m_rcvhdr.Receiver == USER_IVR || m_rcvhdr.Receiver == USER_DUMMY || m_rcvhdr.Receiver == USER_TST_CTI ) 
			&& m_rcvhdr.Sender == USER_CTI )
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
void CPCSClientSock::GetRequests()
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
int CPCSClientSock::ProcessRequests()
{
	/// Process message directly
	CString lv_strMsg, lv_strTemp;
	CIVRChannel *lv_pAgent = NULL;
	int lv_nLen, lv_nLoop, lv_nPos;

	switch( m_rcvhdr.Command )
	{
	// Keep Live Message
	case TCP_KEEPLIVE_MSG:
		SendProtocolData(TCP_KEEPLIVE_ACK);
		break;

	// 注册响应消息
	case TCP_PCS2IVR_REGISTER_ACK:
		{
			S_TCP_PCS2IVR_REGISTER_ACK lv_rcvbdy;
			
			/// Message Analysis
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, m_DataQueue.GetBuffer()+sizeof(SMsgHeader), sizeof(lv_rcvbdy));
			
			switch( lv_rcvbdy.ErrorCode )
			{
			case PCS_DEF_RETCODE_BEUSED:
				lv_strMsg.Format("PCS%d-IVR Server failed to register, some others has already registered on CTI Server!", m_bytPCSIndex);
				break;
			case PCS_DEF_RETCODE_VERSION:
				lv_strMsg.Format("PCS%d-IVR Server failed to register, check the version of the appliction!", m_bytPCSIndex);
				break;
			case PCS_DEF_RETCODE_REGCODE:
				lv_strMsg.Format("PCS%d-IVR Server failed to register, check the register code!", m_bytPCSIndex);
				break;
			case PCS_DEF_RETCODE_STATUS:
				lv_strMsg.Format("PCS%d-IVR Server register OK, however CTI link is pending!", m_bytPCSIndex);
				break;
			case PCS_DEF_RETCODE_OTHERS:
				lv_strMsg.Format("PCS%d-IVR Server failed to register for an unknown error, call the software vendor!", m_bytPCSIndex);
				break;
			case PCS_DEF_RETCODE_SYSERROR:
				lv_strMsg.Format("PCS%d-IVR Server failed to register for a system error, call the software vendor!", m_bytPCSIndex);
				break;
			case PCS_DEF_RETCODE_SUCC:
				lv_strMsg.Format("PCS%d-IVR Server successfully registered on CTI server [%s : %d]", 
					m_bytPCSIndex, m_strServerIP, m_nServerPort);
				
				// Sun added 2012-10-14
				// Sun added if 2013-01-14, Single CTI does this
				if( pMainWindow->m_param_cti2port == 0 )
					pMainWindow->SetCTIOpened(TRUE, TRUE);

				pMainWindow->WriteStatusBar(STATUS_BAR_COMM, ICON_Ready);
				break;
			}

			WriteTrace(TraceInfo, lv_strMsg);
		}
		break;

	// 取消注册响应消息
	case TCP_PCS2IVR_UNREGISTER_ACK:
		lv_strMsg.Format("PCS%d-IVR Server successfully unregistered from CTI server.", m_bytPCSIndex);
		WriteTrace(TraceInfo, lv_strMsg);
		break;

	//================================================================
	// CTI link recovery
	case TCP_PCS2ALL_CTILinkStart:
		{
			S_TCP_PCS2ALL_CTILinkShift lv_rcvbdy;
			
			/// Message Analysis
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, m_DataQueue.GetBuffer()+sizeof(SMsgHeader), sizeof(lv_rcvbdy));

			WriteTrace(TraceInfo, "PCS%d-CTI monitor for device:%s is started.", m_bytPCSIndex, lv_rcvbdy.strDeviceID);

			if( strcmp(lv_rcvbdy.strDeviceID, "PBX") == 0 )
				pMainWindow->WriteStatusBar(STATUS_BAR_COMM, ICON_Ready);			
		}
		break;

	// CTI link is down
	case TCP_PCS2ALL_CTILinkStop:
		{
			S_TCP_PCS2ALL_CTILinkShift lv_rcvbdy;
			
			/// Message Analysis
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, m_DataQueue.GetBuffer()+sizeof(SMsgHeader), sizeof(lv_rcvbdy));

			WriteTrace(TraceInfo, "PCS%d-CTI monitor for device:%s is stoped.", m_bytPCSIndex, lv_rcvbdy.strDeviceID);

			if( strcmp(lv_rcvbdy.strDeviceID, "PBX") == 0 )
				pMainWindow->WriteStatusBar(STATUS_BAR_COMM, ICON_Stop);
		}
		break;

	// Display text message
	case TCP_PCS2ALL_TEXTMSG:
		WriteTrace(TraceInfo, "[PCS%d-Note]: %s", m_bytPCSIndex, (char *)(m_DataQueue.GetBuffer()+sizeof(SMsgHeader)));
		break;

	// Sun added 2010-05-18 [V7.1.8.3]
	/// 子系统状态报告消息
	case TCP_PCS2ALL_SUBSYS_STATUS_REPORT:
		{
			S_TCP_PCS2ALL_SUBSYS_STATUS_REPORT lv_rcvbdy;
			
			/// Message Analysis
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, m_DataQueue.GetBuffer()+sizeof(SMsgHeader), sizeof(lv_rcvbdy));

			WriteTrace(TraceWarn, "PCS%d-Sub-system [%s] - %s, Version:%d", m_bytPCSIndex,
				GetSubSystemNameString(lv_rcvbdy.SubSysID, lv_rcvbdy.SubSysIndex, lv_rcvbdy.SubSysType), 
				(lv_rcvbdy.SubSysStatus == PCS_DEF_CHOICE_YES ? _T("start") : _T("stop")),
				lv_rcvbdy.SubSysVersion);

			if( lv_rcvbdy.SubSysID == USER_CTI )
			{
				if( m_bytPCSIndex != lv_rcvbdy.SubSysIndex )
				{
					WriteTrace(TraceWarn, "Warn - PCS%d id is request to change to %d", m_bytPCSIndex, lv_rcvbdy.SubSysIndex);
					SetPCSIndex(lv_rcvbdy.SubSysIndex);
				}
				SetPCSStatus((PCS_PCSStatus)lv_rcvbdy.SubSysStatus);
			}
		}
		break;

	// 设备初始化响应消息
	case TCP_PCS2IVR_CH_OPEN_ACK:
		{
			S_TCP_PCS2IVR_CH_OPEN_ACK lv_rcvbdy;
			
			/// Message Analysis
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, m_DataQueue.GetBuffer()+sizeof(SMsgHeader), sizeof(lv_rcvbdy));

			/// Find IVR Channel by CTI AgentID
			if( lv_rcvbdy.ErrorCode == PCS_DEF_RETCODE_SUCC )
			{
				try
				{
					lv_pAgent = pMainWindow->m_CTIAgentList.FindAgentIndexWithID(lv_rcvbdy.AgentID);
					if( lv_pAgent )
						lv_pAgent->SetCTIOpened(TRUE);
				}
				catch(...)
				{
				}
				WriteTrace(TraceInfo, "PCS%d-Open IVR-CTI channel:%d succeed.", m_bytPCSIndex, lv_rcvbdy.AgentID);
			}
			else
				WriteTrace(TraceWarn, "PCS%d-Open IVR-CTI channel:%d failed! Error Code = %d", m_bytPCSIndex, lv_rcvbdy.AgentID, lv_rcvbdy.ErrorCode);
		}
		break;

	// 设备关闭响应消息
	case TCP_PCS2IVR_CH_CLOSE_ACK:
		{
			S_TCP_PCS2IVR_CH_CLOSE_ACK lv_rcvbdy;
			
			/// Message Analysis
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, m_DataQueue.GetBuffer()+sizeof(SMsgHeader), sizeof(lv_rcvbdy));

			// Sun added 2010-05-18 [V7.1.8.3], 仅处理Active Server的消息
			if( pMainWindow->GetActivePCSID() == m_bytPCSIndex )
			{
				/// Find IVR Channel by CTI AgentID
				try
				{
					lv_pAgent = pMainWindow->m_CTIAgentList.FindAgentIndexWithID(lv_rcvbdy.AgentID);
					if( lv_pAgent )
						lv_pAgent->SetCTIOpened(FALSE);
				}
				catch(...)
				{
				}
			}
			WriteTrace(TraceInfo, "PCS%d-Close IVR-CTI channel:%d return code = %d.", m_bytPCSIndex, lv_rcvbdy.AgentID, lv_rcvbdy.ErrorCode);
		}
		break;

	// Call In - Gother call info.
	case TCP_PCS2AGT_CALL_CALLIN:
		{
			S_TCP_PCS2AGT_CALL_CALLIN lv_rcvbdy;
			
			/// Message Analysis
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, m_DataQueue.GetBuffer()+sizeof(SMsgHeader), sizeof(lv_rcvbdy));

			WriteTrace(TraceDebug, "PCS%d-IVR-CTI channel:%d received callin msg callrfid = %d, ANI = %s, DNIS = %s, WaitTime = %d, AppData = %s, User Data = %s.",
				m_bytPCSIndex, 
				lv_rcvbdy.AgentID, lv_rcvbdy.CallRefID, lv_rcvbdy.strANI, lv_rcvbdy.strDNIS, lv_rcvbdy.WaitTime, 
				lv_rcvbdy.strAppData, lv_rcvbdy.strUserData);

			// Sun added 2010-05-18 [V7.1.8.3], 仅处理Active Server的消息
			if( pMainWindow->GetActivePCSID() == m_bytPCSIndex )
			{
				/// Find IVR Channel by CTI AgentID
				try
				{
					lv_pAgent = pMainWindow->m_CTIAgentList.FindAgentIndexWithID(lv_rcvbdy.AgentID);
					if( lv_pAgent )
					{
						if( lv_pAgent->GetStarted() )
						{
							if( lv_pAgent->GetStatus() < IVR_RUN )
#ifndef CISCO_CCM
								lv_pAgent->SetStatus(IVR_W_ANSWER);
#else
								WriteTrace(TraceWarn, "Warn - IVR Channel: %d received CTI callin msg but Channel status is %d", lv_pAgent->GetLineNO(), lv_pAgent->GetStatus());
#endif

							//------------------------------------------------------------------------
							// Sun added 2007-01-16
							CString lv_strData;
							CStringArray lv_saTestFileds;

							/// AppData测试标记
							BOOL lv_blnTestFlag = FALSE;
							lv_strData = lv_rcvbdy.strAppData;
							if( lv_strData.Left(strlen(DEF_IVR_TEST_FLAG)).CompareNoCase(DEF_IVR_TEST_FLAG) == 0 )
							{
								lv_pAgent->NF_SetUserData(lv_rcvbdy.strUserData, PCS_DEF_DATALEN_USER);
								lv_blnTestFlag = TRUE;
							}
							else
							{
								/// UserData测试标记
								lv_strData = lv_rcvbdy.strUserData;
								if( lv_strData.Left(strlen(DEF_IVR_TEST_FLAG)).CompareNoCase(DEF_IVR_TEST_FLAG) == 0 )
								{
									lv_pAgent->NF_SetAppData((UC *)lv_rcvbdy.strAppData);
									lv_blnTestFlag = TRUE;
								}
							}

							/// 设置测试参数
							lv_pAgent->SetIVRTestFlag(lv_blnTestFlag);
							if( lv_blnTestFlag )
							{
								/// 分析测试数据
								lv_saTestFileds.RemoveAll();

								Split(lv_strData, DEF_IVR_TEST_FIELD_DELIMIT, &lv_saTestFileds);
								lv_nLen = lv_saTestFileds.GetUpperBound();
								for( lv_nLoop = 1; lv_nLoop <= lv_nLen; lv_nLoop++ )
								{
									lv_strData = lv_saTestFileds[lv_nLoop];

									/// 设置测试时间
									lv_nPos = strlen(DEF_IVR_TEST_SET_TIME);
									if( lv_strData.Left(lv_nPos).CompareNoCase(DEF_IVR_TEST_SET_TIME) == 0 )
									{
										lv_strTemp = lv_strData.Mid(lv_nPos);
										lv_pAgent->SetIVRTestTime(lv_strTemp);
									}
									else
									{
										/// 设置测试主叫
										lv_nPos = strlen(DEF_IVR_TEST_SET_ANI);
										if( lv_strData.Left(lv_nPos).CompareNoCase(DEF_IVR_TEST_SET_ANI) == 0 )
										{
											lv_strTemp = lv_strData.Mid(lv_nPos);
											lv_nPos = lv_strTemp.GetLength();
											if( lv_nPos > 0 )
												lv_pAgent->NF_SetCallerID((UC *)lv_strTemp.GetBuffer(lv_nPos));
										}
										else
										{
											/// 设置测试被叫
											lv_nPos = strlen(DEF_IVR_TEST_SET_DNIS);
											if( lv_strData.Left(lv_nPos).CompareNoCase(DEF_IVR_TEST_SET_DNIS) == 0 )
											{
												lv_strTemp = lv_strData.Mid(lv_nPos);
												lv_nPos = lv_strTemp.GetLength();
												if( lv_nPos > 0 )
													lv_pAgent->NF_SetCalleeID((UC *)lv_strTemp.GetBuffer(lv_nPos));
											}
											else
											{
												/// 设置CDR标记
												lv_nPos = strlen(DEF_IVR_TEST_NOCDR);
												if( lv_strData.Left(lv_nPos).CompareNoCase(DEF_IVR_TEST_NOCDR) == 0 )
													lv_pAgent->SetIVRTestNOCDR(TRUE);
											}
										}
									}
								}
							}
							else
							//------------------------------------------------------------------------
							{
								lv_pAgent->NF_SetAppData((UC *)lv_rcvbdy.strAppData);
								lv_pAgent->NF_SetUserData(lv_rcvbdy.strUserData, PCS_DEF_DATALEN_USER);

								//------------------------------------------------------------------------
								// Sun updated 2012-05-15, remove condition compiler and add 'if == 0 '
								/// From
								// Sun updated 2008-10-27, [B], add condition compiler
/*
#ifndef CISCO_CCM
								if( lv_rcvbdy.strANI[0] != NULL )
									lv_pAgent->NF_SetCallerID((UC *)lv_rcvbdy.strANI);
								if( lv_rcvbdy.strDNIS[0] != NULL )
									lv_pAgent->NF_SetCalleeID((UC *)lv_rcvbdy.strDNIS);
#endif
*/
								/// To
								if( lv_rcvbdy.strANI[0] != NULL )
								{
#ifdef CISCO_CCM
									if( (lv_pAgent->NF_GetCallerID())[0] == 0 )
#endif
									lv_pAgent->NF_SetCallerID((UC *)lv_rcvbdy.strANI);
								}
								if( lv_rcvbdy.strDNIS[0] != NULL )
								{
#ifdef CISCO_CCM
									if( (lv_pAgent->NF_GetCalleeID())[0] == 0 )
#endif
									lv_pAgent->NF_SetCalleeID((UC *)lv_rcvbdy.strDNIS);
								}
								//------------------------------------------------------------------------
							}
							
							lv_pAgent->SetCallRefID(lv_rcvbdy.CallRefID, lv_rcvbdy.EventSource, lv_rcvbdy.CallSource, lv_rcvbdy.WaitTime, lv_rcvbdy.TimeStamp);
						}
					}
				}
				catch(...)
				{
					WriteTrace(TraceError, "Error IVR-CTI channel:%d callin msg callrfid = %d!", lv_rcvbdy.AgentID, lv_rcvbdy.CallRefID);
				}
			}
		}
		break;

	// Call Cleared - May stop IVR
	case TCP_PCS2AGT_CALL_CALLCLEARED:
		{
			S_TCP_PCS2AGT_CALL_CALLCLEARED lv_rcvbdy;
			
			/// Message Analysis
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, m_DataQueue.GetBuffer()+sizeof(SMsgHeader), sizeof(lv_rcvbdy));

			WriteTrace(TraceDebug, "PCS%d-IVR-CTI channel:%d received callclear msg callrfid = %d.", 
				m_bytPCSIndex, 
				lv_rcvbdy.AgentID, lv_rcvbdy.CallRefID);

			// Sun added 2010-05-18 [V7.1.8.3], 仅处理Active Server的消息
			if( pMainWindow->GetActivePCSID() == m_bytPCSIndex )
			{
				/// Find IVR Channel by CTI AgentID
				if( theApp.m_nCTIOnHookDelay > 0 )
				{
					S_WM_CTIONHOOKEVENT lv_CTIOnHook;
					try
					{
						lv_pAgent = pMainWindow->m_CTIAgentList.FindAgentIndexWithID(lv_rcvbdy.AgentID);
						if( lv_pAgent )
							if( lv_pAgent->GetStarted() && (lv_pAgent->GetCallRefID() == lv_rcvbdy.CallRefID || lv_pAgent->GetNewCallRefID() == lv_rcvbdy.CallRefID) )
							{
								lv_CTIOnHook.pChannel = (DWORD)lv_pAgent;
								lv_CTIOnHook.lngTickCount = GetTickCount();
								lv_CTIOnHook.lngCallRefID = lv_rcvbdy.CallRefID;
								g_MQCTIOnHookEvents.AddMessage((BYTE *)&lv_CTIOnHook, sizeof(S_WM_CTIONHOOKEVENT));
							}
					}
					catch(...)
					{
						WriteTrace(TraceError, "Sock process callclear msg goes error on channel:[%d], Call ID:[%d]", lv_rcvbdy.AgentID, lv_rcvbdy.CallRefID);
					}
				}
			}
		}
		break;

	// Sun added 2005-12-06
	case TCP_PCS2AGT_CALL_UNRINGING:
		{
			S_TCP_PCS2AGT_CALL_UNRINGING lv_rcvbdy;
			
			/// Message Analysis
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, m_DataQueue.GetBuffer()+sizeof(SMsgHeader), sizeof(lv_rcvbdy));

			WriteTrace(TraceDebug, "PCS%d-IVR-CTI channel:%d received unringing msg callrfid = %d.", 
				m_bytPCSIndex, 
				lv_rcvbdy.AgentID, lv_rcvbdy.CallRefID);

			// Sun added 2010-05-18 [V7.1.8.3], 仅处理Active Server的消息
			if( pMainWindow->GetActivePCSID() == m_bytPCSIndex )
			{
				/// Find IVR Channel by CTI AgentID
				if( theApp.m_nCTIOnHookDelay > 0 )
				{
					S_WM_CTIONHOOKEVENT lv_CTIOnHook;
					try
					{
						lv_pAgent = pMainWindow->m_CTIAgentList.FindAgentIndexWithID(lv_rcvbdy.AgentID);
						if( lv_pAgent )
							if( lv_pAgent->GetStarted() && (lv_pAgent->GetCallRefID() == lv_rcvbdy.CallRefID || lv_pAgent->GetNewCallRefID() == lv_rcvbdy.CallRefID) )
							{
								lv_CTIOnHook.pChannel = (DWORD)lv_pAgent;
								lv_CTIOnHook.lngTickCount = GetTickCount();
								lv_CTIOnHook.lngCallRefID = lv_rcvbdy.CallRefID;
								g_MQCTIOnHookEvents.AddMessage((BYTE *)&lv_CTIOnHook, sizeof(S_WM_CTIONHOOKEVENT));
							}
					}
					catch(...)
					{
						WriteTrace(TraceError, "Sock process unringing msg goes error on channel:[%d], Call ID:[%d]", lv_rcvbdy.AgentID, lv_rcvbdy.CallRefID);
					}
				}
			}
		}
		break;

	// Channel CTI Status Changed
	case TCP_PCS2AGT_POS_STATUS_CHANGE:
		{
			S_TCP_PCS2AGT_POS_STATUS_CHANGE lv_rcvbdy;
			
			/// Message Analysis
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, m_DataQueue.GetBuffer()+sizeof(SMsgHeader), sizeof(lv_rcvbdy));

			WriteTrace(TraceDebug, "PCS%d-IVR-CTI channel:%d changed status to: %d.", 
				m_bytPCSIndex, 
				lv_rcvbdy.AgentID, lv_rcvbdy.PositionStatus);
		}
		break;

	// 电话应答通知消息
	case TCP_PCS2AGT_CALL_CALLANSWERED:
		{
			S_TCP_PCS2AGT_CALL_CALLANSWERED lv_rcvbdy;
			
			/// Message Analysis
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, m_DataQueue.GetBuffer()+sizeof(SMsgHeader), sizeof(lv_rcvbdy));

			WriteTrace(TraceDebug, "PCS%d-IVR-CTI channel:%d received call answered msg callrfid = %d, operator:%d, OtherParty:%s.", 
				m_bytPCSIndex, 
				lv_rcvbdy.AgentID, lv_rcvbdy.CallRefID, lv_rcvbdy.Operator, lv_rcvbdy.strPeerParty);

			//------------------------------------------------------------
			// Sun updated 2008-12-13
			/// From
			/*
            // 外拨接通
			if( lv_rcvbdy.Operator == _pcC_CALLOPT_OTHERPARTY )
			{
				try
				{
					lv_pAgent = pMainWindow->m_CTIAgentList.FindAgentIndexWithID(lv_rcvbdy.AgentID);
					if( lv_pAgent )
						if( lv_pAgent->GetStarted() && lv_pAgent->m_OBEvent.m_nEventID == 1 )
						{
							lv_pAgent->m_OBEvent.m_nEventID = _pcC_OB_RESULT_SUCC;
							lv_pAgent->m_OBEvent.Set();
						}
				}
				catch(...)
				{
					WriteTrace(TraceError, "Sock process callanswer msg goes error on channel:[%d], Call ID:[%d]", lv_rcvbdy.AgentID, lv_rcvbdy.CallRefID);
				}
			}
			*/
			/// To
			// Sun added 2010-05-18 [V7.1.8.3], 仅处理Active Server的消息
			if( pMainWindow->GetActivePCSID() == m_bytPCSIndex )
			{
				try
				{
					lv_pAgent = pMainWindow->m_CTIAgentList.FindAgentIndexWithID(lv_rcvbdy.AgentID);
					if( lv_pAgent )
					{
						if( lv_pAgent->GetStarted() )
						{
							// 外拨接通
							if( lv_rcvbdy.Operator == _pcC_CALLOPT_OTHERPARTY )
							{
								if( lv_pAgent->m_OBEvent.m_nEventID == 1 )
								{
									// 设置CallRefID, [B]
									if( lv_pAgent->GetCallRefID() == 0 )
										lv_pAgent->SetCallRefID(lv_rcvbdy.CallRefID, lv_rcvbdy.EventSource, _pcC_CALLSRC_CALLCENTER, 0, lv_rcvbdy.TimeStamp);

									lv_pAgent->m_OBEvent.m_nEventID = _pcC_OB_RESULT_SUCC;
									lv_pAgent->m_OBEvent.Set();
								}
							}
							// 呼入接通
							else if( lv_rcvbdy.Operator == _pcC_CALLOPT_THISPARTY )
							{
								// 设置CallRefID, [A]
								if( lv_pAgent->GetCallRefID() == 0 )
									lv_pAgent->SetCallRefID(lv_rcvbdy.CallRefID, lv_rcvbdy.EventSource, _pcC_CALLSRC_INTERNAL, 0, lv_rcvbdy.TimeStamp);
								else if( lv_pAgent->GetCallRefID() != lv_rcvbdy.CallRefID )
									lv_pAgent->SetNewCallRefID(lv_rcvbdy.CallRefID);

#ifndef CISCO_CCM
								if( lv_rcvbdy.strPeerParty[0] != NULL )
									lv_pAgent->NF_SetCallerID((UC *)lv_rcvbdy.strPeerParty);
#endif
							}

							// Sun added 2013-01-14 'if...
							if( lv_pAgent->GetQueueWaitEventID() == 13 ||  lv_pAgent->GetQueueWaitEventID() == 14 )
							{
								// Sun added 2012-04-18 [B]
								lv_pAgent->LockFlagData(TRUE);
								try
								{
									if( lv_pAgent->GetQueueWaitEventID() == 13 ||  lv_pAgent->GetQueueWaitEventID() == 14 )
									{
										lv_pAgent->SetQueueWaitEventData((BYTE *)&lv_rcvbdy, sizeof(lv_rcvbdy));
										lv_pAgent->SetQueueWaitEvent(14);
									}
								}
								catch(...)
								{
								}
								lv_pAgent->LockFlagData(FALSE);
							}
						}
					}
				}
				catch(...)
				{
					WriteTrace(TraceError, "Sock process callanswer msg goes error on channel:[%d], Call ID:[%d]", lv_rcvbdy.AgentID, lv_rcvbdy.CallRefID);
				}
				//------------------------------------------------------------
			}
		}
		break;

	// Sun added 2012-04-18 [B]
	// 转接、会议初始化完成消息
	case TCP_PCS2AGT_CALL_CONSULTATION_INIT:
		{
			S_TCP_PCS2AGT_CALL_CONSULTATION_INIT lv_rcvbdy;
			
			/// Message Analysis
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, m_DataQueue.GetBuffer()+sizeof(SMsgHeader), sizeof(lv_rcvbdy));

			WriteTrace(TraceDebug, "PCS%d-IVR-CTI channel:%d received Call consultation init msg callrfid = %d, hold callrdif = %d, OtherParty:%s.", 
				m_bytPCSIndex, 
				lv_rcvbdy.AgentID, lv_rcvbdy.CallRefID, lv_rcvbdy.HoldRefID, lv_rcvbdy.strPeerParty);

			// Sun added 2010-05-18 [V7.1.8.3], 仅处理Active Server的消息
			if( pMainWindow->GetActivePCSID() == m_bytPCSIndex )
			{
				try
				{
					lv_pAgent = pMainWindow->m_CTIAgentList.FindAgentIndexWithID(lv_rcvbdy.AgentID);
					if( lv_pAgent )
					{
						if( lv_pAgent->GetStarted() )
						{
							// Sun added 2013-01-14 'if...
							if( lv_pAgent->GetQueueWaitEventID() == 13 ||  lv_pAgent->GetQueueWaitEventID() == 14 )
							{
								lv_pAgent->LockFlagData(TRUE);
								try
								{
									if( lv_pAgent->GetQueueWaitEventID() == 13 ||  lv_pAgent->GetQueueWaitEventID() == 14 )
									{
										lv_pAgent->SetQueueWaitEventData((BYTE *)&lv_rcvbdy, sizeof(lv_rcvbdy));
										lv_pAgent->SetQueueWaitEvent(13);
									}
								}
								catch(...)
								{
								}
								lv_pAgent->LockFlagData(FALSE);
							}
						}
					}
				}
				catch(...)
				{
					WriteTrace(TraceError, "Sock process consultation init msg goes error on channel:[%d], Call ID:[%d]", lv_rcvbdy.AgentID, lv_rcvbdy.CallRefID);
				}
			}
		}
		break;

	// Sun added 2012-04-18 [B]
	// 会议完成通知消息
	case TCP_PCS2AGT_CALL_CONFERENCED:
		{
			S_TCP_PCS2AGT_CALL_CONFERENCED lv_rcvbdy;
			
			/// Message Analysis
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, m_DataQueue.GetBuffer()+sizeof(SMsgHeader), sizeof(lv_rcvbdy));

			WriteTrace(TraceDebug, "PCS%d-IVR-CTI channel:%d received Conferenced msg callrfid = %d,OtherParty:%s, ThirdParty:%s.", 
				m_bytPCSIndex, 
				lv_rcvbdy.AgentID, lv_rcvbdy.CallRefID, lv_rcvbdy.strOtherParty, lv_rcvbdy.strThirdParty);

			// Sun added 2010-05-18 [V7.1.8.3], 仅处理Active Server的消息
			if( pMainWindow->GetActivePCSID() == m_bytPCSIndex )
			{
				try
				{
					lv_pAgent = pMainWindow->m_CTIAgentList.FindAgentIndexWithID(lv_rcvbdy.AgentID);
					if( lv_pAgent )
					{
						if( lv_pAgent->GetStarted() )
						{
							// Sun added 2013-01-14 'if...
							if( lv_pAgent->GetQueueWaitEventID() == 13 ||  lv_pAgent->GetQueueWaitEventID() == 14 )
							{
								lv_pAgent->LockFlagData(TRUE);
								try
								{
									if( lv_pAgent->GetQueueWaitEventID() == 13 ||  lv_pAgent->GetQueueWaitEventID() == 14 )
									{
										lv_pAgent->SetQueueWaitEventData((BYTE *)&lv_rcvbdy, sizeof(lv_rcvbdy));
										lv_pAgent->SetQueueWaitEvent(13);
									}
								}
								catch(...)
								{
								}
								lv_pAgent->LockFlagData(FALSE);
							}
						}
					}
				}
				catch(...)
				{
					WriteTrace(TraceError, "Sock process Conferenced msg goes error on channel:[%d], Call ID:[%d]", lv_rcvbdy.AgentID, lv_rcvbdy.CallRefID);
				}
			}
		}
		break;

	// 外拨通道建立通知消息
	case TCP_PCS2AGT_CALL_NETWORKREACHED:
		{
			S_TCP_PCS2AGT_CALL_NETWORKREACHED lv_rcvbdy;
			
			/// Message Analysis
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, m_DataQueue.GetBuffer()+sizeof(SMsgHeader), sizeof(lv_rcvbdy));

			// Sun added 2010-05-18 [V7.1.8.3], 仅处理Active Server的消息
			if( pMainWindow->GetActivePCSID() == m_bytPCSIndex )
			{
				try
				{
					lv_pAgent = pMainWindow->m_CTIAgentList.FindAgentIndexWithID(lv_rcvbdy.AgentID);
					if( lv_pAgent )
						if( lv_pAgent->GetCallRefID() == 0 )
							lv_pAgent->SetCallRefID(lv_rcvbdy.CallRefID, lv_rcvbdy.EventSource, _pcC_CALLSRC_CALLCENTER, 0, lv_rcvbdy.TimeStamp);
				}
				catch(...)
				{
					WriteTrace(TraceError, "Sock process NETWORKREACHED msg goes error on channel:[%d], Call ID:[%d]", lv_rcvbdy.AgentID, lv_rcvbdy.CallRefID);
				}
			}

			WriteTrace(TraceDebug, "PCS%d-IVR-CTI channel:%d received OB network reached msg callrfid = %d, OtherParty:%s.", 
				m_bytPCSIndex, 
				lv_rcvbdy.AgentID, lv_rcvbdy.CallRefID, lv_rcvbdy.strPeerParty);
		}
		break;

	// 外拨遇忙通知消息
	case TCP_PCS2AGT_CALL_DESTINATIONBUSY:
		{
			S_TCP_PCS2AGT_CALL_DESTINATIONBUSY lv_rcvbdy;
			
			/// Message Analysis
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, m_DataQueue.GetBuffer()+sizeof(SMsgHeader), sizeof(lv_rcvbdy));

			WriteTrace(TraceDebug, "PCS%d-IVR-CTI channel:%d received OB destination busy msg callrfid = %d, OtherParty:%s.", 
				m_bytPCSIndex, 
				lv_rcvbdy.AgentID, lv_rcvbdy.CallRefID, lv_rcvbdy.strPeerParty);

			// Sun added 2010-05-18 [V7.1.8.3], 仅处理Active Server的消息
			if( pMainWindow->GetActivePCSID() == m_bytPCSIndex )
			{
				try
				{
					lv_pAgent = pMainWindow->m_CTIAgentList.FindAgentIndexWithID(lv_rcvbdy.AgentID);
					if( lv_pAgent )
					{
						if( lv_pAgent->GetStarted() )
						{
							if( lv_pAgent->m_OBEvent.m_nEventID == 1 )
							{
								lv_pAgent->m_OBEvent.m_nEventID = _pcC_OB_RESULT_DEST_BUSY;
								lv_pAgent->m_OBEvent.Set();
							}

							// Sun added 2013-01-14 'if...
							if( lv_pAgent->GetQueueWaitEventID() == 13 ||  lv_pAgent->GetQueueWaitEventID() == 14 )
							{
								// Sun added 2012-04-18 [B]
								lv_pAgent->LockFlagData(TRUE);
								try
								{
									if( lv_pAgent->GetQueueWaitEventID() == 13 ||  lv_pAgent->GetQueueWaitEventID() == 14 )
									{
										lv_pAgent->SetQueueWaitEventData((BYTE *)&lv_rcvbdy, sizeof(lv_rcvbdy));
										lv_pAgent->SetQueueWaitEvent(17);
									}
								}
								catch(...)
								{
								}
                                lv_pAgent->LockFlagData(FALSE);
							}
						}
					}
				}
				catch(...)
				{
					WriteTrace(TraceError, "Sock process destination busy msg goes error on channel:[%d], Call ID:[%d]", lv_rcvbdy.AgentID, lv_rcvbdy.CallRefID);
				}
			}
		}
		break;

	case TCP_PCS2AGT_CALL_DESTINATIONINVALID:
		{
			S_TCP_PCS2AGT_CALL_DESTINATIONINVALID lv_rcvbdy;
			
			/// Message Analysis
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, m_DataQueue.GetBuffer()+sizeof(SMsgHeader), sizeof(lv_rcvbdy));

			WriteTrace(TraceDebug, "PCS%d-IVR-CTI channel:%d received OB destination invalid msg callrfid = %d, OtherParty:%s.", 
				m_bytPCSIndex, 
				lv_rcvbdy.AgentID, lv_rcvbdy.CallRefID, lv_rcvbdy.strPeerParty);

			// Sun added 2010-05-18 [V7.1.8.3], 仅处理Active Server的消息
			if( pMainWindow->GetActivePCSID() == m_bytPCSIndex )
			{
				try
				{
					lv_pAgent = pMainWindow->m_CTIAgentList.FindAgentIndexWithID(lv_rcvbdy.AgentID);
					if( lv_pAgent )
					{
						if( lv_pAgent->GetStarted() )
						{
							if( lv_pAgent->m_OBEvent.m_nEventID == 1 )
							{
								lv_pAgent->m_OBEvent.m_nEventID = _pcC_OB_RESULT_DEST_WRONG;
								lv_pAgent->m_OBEvent.Set();
							}

							// Sun added 2013-01-14 'if...
							if( lv_pAgent->GetQueueWaitEventID() == 13 ||  lv_pAgent->GetQueueWaitEventID() == 14 )
							{
								// Sun added 2012-04-18 [B]
								lv_pAgent->LockFlagData(TRUE);
								try
								{
									if( lv_pAgent->GetQueueWaitEventID() == 13 ||  lv_pAgent->GetQueueWaitEventID() == 14 )
									{
										lv_pAgent->SetQueueWaitEventData((BYTE *)&lv_rcvbdy, sizeof(lv_rcvbdy));
										lv_pAgent->SetQueueWaitEvent(16);
									}
								}
								catch(...)
								{
								}
								lv_pAgent->LockFlagData(FALSE);
							}
						}
					}
				}
				catch(...)
				{
					WriteTrace(TraceError, "Sock process destination invalid msg goes error on channel:[%d], Call ID:[%d]", lv_rcvbdy.AgentID, lv_rcvbdy.CallRefID);
				}
			}
		}
		break;

	// Sun added 2005-07-10
	case TCP_PCS2IVR_GET_RPTSTATUS_ACK:
		{
			S_TCP_PCS2IVR_GET_RPTSTATUS_ACK lv_rcvbdy;
			
			/// Message Analysis
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, m_DataQueue.GetBuffer()+sizeof(SMsgHeader), sizeof(lv_rcvbdy));

			WriteTrace(TraceDebug, "PCS%d-IVR-CTI channel:%d received route point query ack-msg Type = %d, RPTID = %d.", 
				m_bytPCSIndex, 
				lv_rcvbdy.ChannelID, lv_rcvbdy.nType, lv_rcvbdy.nGroupID);

			try
			{
				if( lv_rcvbdy.ChannelID >= 0 && lv_rcvbdy.ChannelID < theApp.m_nIVRChannels )
				{
					lv_pAgent = PIVRCH[lv_rcvbdy.ChannelID];
					if( lv_pAgent )
					{
						if( lv_pAgent->GetStarted() )
						{
							memcpy(lv_pAgent->m_QueryEvent.m_pData, &lv_rcvbdy, sizeof(lv_rcvbdy));
							lv_pAgent->m_QueryEvent.Set();
						}
					}
				}
			}
			catch(...)
			{
				WriteTrace(TraceError, "Sock process general external msg: %d goes error on channel:[%d]", 
					m_rcvhdr.Command, lv_rcvbdy.ChannelID);
			}
		}
		break;

	// Sun added 2005-08-15
	case TCP_PCS2IVR_GET_AGENTSTATUS_ACK:
		{
			S_TCP_PCS2IVR_GET_AGENTSTATUS_ACK lv_rcvbdy;
			
			/// Message Analysis
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, m_DataQueue.GetBuffer()+sizeof(SMsgHeader), sizeof(lv_rcvbdy));

			WriteTrace(TraceDebug, "PCS%d-IVR-CTI channel:%d received agent status query ack-msg Type = %d, AgentID = %d, UserID = %s, DNST = %d, PosST = %d.", 
				m_bytPCSIndex, 
				lv_rcvbdy.ChannelID, lv_rcvbdy.UserQuery, lv_rcvbdy.AgentID, lv_rcvbdy.strAliasName, lv_rcvbdy.DNStatus, lv_rcvbdy.PositionStatus);

			try
			{
				if( lv_rcvbdy.ChannelID >= 0 && lv_rcvbdy.ChannelID < theApp.m_nIVRChannels )
				{
					lv_pAgent = PIVRCH[lv_rcvbdy.ChannelID];
					if( lv_pAgent )
					{
						if( lv_pAgent->GetStarted() )
						{
							memcpy(lv_pAgent->m_QueryEvent.m_pData, &lv_rcvbdy, sizeof(lv_rcvbdy));
							lv_pAgent->m_QueryEvent.Set();
						}
					}
				}
			}
			catch(...)
			{
				WriteTrace(TraceError, "Sock process general external msg: %d goes error on channel:[%d]", 
					m_rcvhdr.Command, lv_rcvbdy.ChannelID);
			}
		}
		break;

	// Sun added 2007-04-15
	case TCP_PCS2IVR_GET_CALL_EWT_ACK:
		{
			S_TCP_PCS2IVR_GET_CALL_EWT_ACK lv_rcvbdy;
			
			/// Message Analysis
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, m_DataQueue.GetBuffer()+sizeof(SMsgHeader), sizeof(lv_rcvbdy));

			WriteTrace(TraceDebug, "PCS%d-IVR-CTI channel:%d received call queue position query ack-msg CallRefID = %d, RoutePoint = %d, ErrorCode = %d, Q.Position = %d, EWT = %d.", 
				m_bytPCSIndex, 
				lv_rcvbdy.ChannelID, lv_rcvbdy.CallRefID, lv_rcvbdy.nRoutePointID, lv_rcvbdy.ErrorCode, lv_rcvbdy.nQueuePosition, lv_rcvbdy.nEstimatedWaitTime);

			// Sun ToDo:
		}
		break;

	//---------------------------------------------------------
	// Sun added 2007-07-15
	// 入队响应消息
	case TCP_PCS2UMS_ADDTASK_ACK:
		{
			S_TCP_PCS2UMS_ADDTASK_ACK lv_rcvbdy;
			
			/// Message Analysis
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, m_DataQueue.GetBuffer()+sizeof(SMsgHeader), sizeof(lv_rcvbdy));

			WriteTrace(TraceDebug, "PCS%d-IVR-CTI Agent:%d received add ums task ack-msg TaskID = %s, RoutePoint = %d, ErrorCode = %d.", 
				m_bytPCSIndex, 
				lv_rcvbdy.TaskID, lv_rcvbdy.strUMSTaskID, lv_rcvbdy.RoutePointID, lv_rcvbdy.ErrorCode);

			// Sun added 2010-01-08 [B], added Confirm Flag
			BOOL lv_blnConfirmFlag = FALSE;

			try
			{
				lv_pAgent = pMainWindow->m_CTIAgentList.FindAgentIndexWithID(lv_rcvbdy.TaskID);
				if( lv_pAgent )
				{
					if( lv_pAgent->GetStarted() )
					{
						// Sun added 2013-01-14 'if...
						if( lv_pAgent->GetQueueWaitEventID() == 1 )
						{
							lv_pAgent->LockFlagData(TRUE);
							try
							{
								if( lv_pAgent->GetQueueWaitEventID() == 1 )
								{
									// Sun added 2010-01-08 [B]
									lv_blnConfirmFlag = TRUE;

									lv_pAgent->SetQueueWaitEventData((BYTE *)&lv_rcvbdy, sizeof(lv_rcvbdy));
									lv_pAgent->SetQueueWaitEvent(1);
								}
							}
							catch(...)
							{
							}
							lv_pAgent->LockFlagData(FALSE);
						}
					}
				}

				//----------------------------------------------------
				// Sun added 2010-01-08 [B]
				if( !lv_blnConfirmFlag )
				{
					WriteTrace(TraceWarn, "PCS%d-IVR-CTI Agent:%d can't process [add ums task ack-msg] TaskID = %s, RoutePoint = %d, ErrorCode = %d.", 
						m_bytPCSIndex, 
						lv_rcvbdy.TaskID, lv_rcvbdy.strUMSTaskID, lv_rcvbdy.RoutePointID, lv_rcvbdy.ErrorCode);

					// Send Remove USK Task Message
					S_TCP_UMS2PCS_REMOVETASK_REQ lv_sndbdy;

					memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));
					lv_sndbdy.InvokeID = GET_INVOKE_ID;
					lv_sndbdy.TaskID = lv_rcvbdy.TaskID;
					strncpy(lv_sndbdy.strUMSTaskID, lv_rcvbdy.strUMSTaskID, PCS_DEF_LEN_GCREF);
					lv_sndbdy.RoutePointID = lv_rcvbdy.RoutePointID;
					lv_sndbdy.ReasonCode = 9;			// 节点超时

					SendProtocolData(TCP_UMS2PCS_REMOVETASK_REQ, sizeof(lv_sndbdy), (UC *)&lv_sndbdy);
				}
				//----------------------------------------------------
			}
			catch(...)
			{
				WriteTrace(TraceError, "Sock process general external msg: %d goes error on agent:[%d]", 
					m_rcvhdr.Command, lv_rcvbdy.TaskID);
			}
		}
		break;

	// 任务分配请求消息
	case TCP_PCS2UMS_DISPATCHTASK_REQ:
		{
			S_TCP_PCS2UMS_DISPATCHTASK_REQ lv_rcvbdy;
			int lv_loop;
			
			/// Message Analysis
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, m_DataQueue.GetBuffer()+sizeof(SMsgHeader), sizeof(lv_rcvbdy));

			WriteTrace(TraceDebug, "PCS%d-IVR-CTI Agent:%d received dispatch ums task msg TaskID = %s, RoutePoint = %d, GroupID = %d, TeamID = %d, UserID = %s, ExtNo = %s, WorkNo = %d.", 
				m_bytPCSIndex, 
				lv_rcvbdy.TaskID, lv_rcvbdy.strUMSTaskID, lv_rcvbdy.RoutePointID, lv_rcvbdy.GroupID, lv_rcvbdy.TeamID, lv_rcvbdy.strUserID, lv_rcvbdy.strExtNO, lv_rcvbdy.LoginID);

			try
			{
				lv_pAgent = pMainWindow->m_CTIAgentList.FindAgentIndexWithID(lv_rcvbdy.TaskID);
				if( lv_pAgent )
				{
					if( lv_pAgent->GetStarted() )
					{
						lv_pAgent->LockFlagData(TRUE);

						try
						{
							// Sun added 2008-10-27
							lv_loop = 0;
							while( lv_pAgent->GetQueueWaitEventID() < 2 )
							{
								if( ++lv_loop > 100 )
								{
									WriteTrace(TraceError, "PCS%d-IVR-CTI Agent:%d wait CTI dispatch msg reached without waiting request.", 
										m_bytPCSIndex, lv_rcvbdy.TaskID);
									break;
								}
								Sleep(10);
							}

							if( lv_pAgent->GetQueueWaitEventID() == 2 )
							{
								// Sun added 'if' 2011-04-22
								if( lv_pAgent->m_strTaskID.CompareNoCase(lv_rcvbdy.strUMSTaskID) == 0 )
								{
									lv_pAgent->SetQueueWaitEventData((BYTE *)&lv_rcvbdy, sizeof(lv_rcvbdy));
									lv_pAgent->SetQueueWaitEvent(2);
								}
								else
								{
									WriteTrace(TraceWarn, "PCS%d-IVR-CTI Agent:%d IVR Channel: %d got CTI dispatch msg with wrong TaskID = %s from %s.", 
										m_bytPCSIndex, lv_rcvbdy.TaskID, lv_pAgent->GetLineNO(), lv_rcvbdy.strUMSTaskID, lv_pAgent->m_strTaskID);
								}
							}
						}
						catch(...)
						{
						}

						lv_pAgent->LockFlagData(FALSE);
					}
				}
			}
			catch(...)
			{
				WriteTrace(TraceError, "Sock process general external msg: %d goes error on agent:[%d]", 
					m_rcvhdr.Command, lv_rcvbdy.TaskID);
			}
		}
		break;

	// 任务分配处理消息
	case TCP_PCS2UMS_TASKST_DISPACTH:
		{
			S_TCP_PCS2UMS_TASKST_DISPACTH lv_rcvbdy;
			int lv_loop;
			
			/// Message Analysis
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, m_DataQueue.GetBuffer()+sizeof(SMsgHeader), sizeof(lv_rcvbdy));

			WriteTrace(TraceDebug, "PCS%d-IVR-CTI channel:%d received ums task accept msg TaskID = %s, RoutePoint = %d, UserID = %s, Result = '%c'.", 
				m_bytPCSIndex, 
				lv_rcvbdy.TaskID, lv_rcvbdy.strUMSTaskID, lv_rcvbdy.RoutePointID, lv_rcvbdy.strUserID, (lv_rcvbdy.DispacthResult >= '0' ? (char)lv_rcvbdy.DispacthResult : ' '));

			try
			{
				lv_pAgent = pMainWindow->m_CTIAgentList.FindAgentIndexWithID(lv_rcvbdy.TaskID);
				if( lv_pAgent )
				{
					if( lv_pAgent->GetStarted() )
					{
						lv_loop = 0;
						lv_pAgent->LockFlagData(TRUE);
						
						try
						{
							while( lv_pAgent->GetQueueWaitEventID() == 3 )
							{
								if( ++lv_loop > 100 )
								{
									WriteTrace(TraceError, "PCS%d-IVR-CTI Agent:%d wait consultation finish timeout when accpet msg has reached.", 
										m_bytPCSIndex,
										lv_rcvbdy.TaskID);
									break;
								}

								Sleep(10);
							}

							if( lv_pAgent->GetQueueWaitEventID() == 4 )
							{
								lv_pAgent->SetQueueWaitEventData((BYTE *)&lv_rcvbdy, sizeof(lv_rcvbdy));
								if( lv_rcvbdy.DispacthResult == _pcC_OB_RESULT_SUCC )
									lv_pAgent->SetQueueWaitEvent(4);
								else
									lv_pAgent->SetQueueWaitEvent(7);
							}
						}
						catch(...)
						{
						}

						lv_pAgent->LockFlagData(FALSE);
					}
				}
			}
			catch(...)
			{
				WriteTrace(TraceError, "Sock process general external msg: %d goes error on agent:[%d]", 
					m_rcvhdr.Command, lv_rcvbdy.TaskID);
			}

		}
		break;
	//---------------------------------------------------------

	// Sun added 2008-05-16
	// 任务暂停响应消息
	case TCP_PCS2UMS_PAUSETASK_ACK:
		{
			S_TCP_PCS2UMS_PAUSETASK_ACK lv_rcvbdy;
			
			/// Message Analysis
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, m_DataQueue.GetBuffer()+sizeof(SMsgHeader), sizeof(lv_rcvbdy));

			WriteTrace(TraceDebug, "PCS%d-IVR-CTI channel:%d received ums task pause-ack msg TaskID = %s, RoutePoint = %d, ErrorCode = %d.", 
				m_bytPCSIndex, 
				lv_rcvbdy.TaskID, lv_rcvbdy.strUMSTaskID, lv_rcvbdy.RoutePointID, lv_rcvbdy.ErrorCode);
		}
		break;

	// Sun added 2011-06-25
	case TCP_PCS2UMS_REMOVETASK_ACK:
		{
			S_TCP_PCS2UMS_REMOVETASK_ACK lv_rcvbdy;
			
			/// Message Analysis
			if( m_rcvhdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, m_DataQueue.GetBuffer()+sizeof(SMsgHeader), sizeof(lv_rcvbdy));

			WriteTrace(TraceDebug, "PCS%d-IVR-CTI channel:%d received ums task remove-ack msg TaskID = %s, RoutePoint = %d, ErrorCode = %d.", 
				m_bytPCSIndex, 
				lv_rcvbdy.TaskID, lv_rcvbdy.strUMSTaskID, lv_rcvbdy.RoutePointID, lv_rcvbdy.ErrorCode);
		}
		break;
	}

	return 1;
}

//----------------------------------------------------------------
// Sun added 2010-07-07
BOOL CPCSClientSock::CheckPBXKeepLive()
{
	DWORD lv_tickNow = GetTickCount();
	DWORD lv_KeepLiveTick = m_nKeepLiveTickCount;

	try
	{
		if( GetConnected() )
		{
			if( lv_tickNow < lv_KeepLiveTick )
				m_nKeepLiveTickCount = lv_tickNow;
			else if( (lv_tickNow - lv_KeepLiveTick) / 3000 > DEF_SCK_KEEPLIVE_INTERVAL )
			{
				CloseConnection();
				WriteTrace(TraceError, "SysErr - CTI Link: id(%d) lost connection to PBX for a period time!!!", m_bytPCSIndex);

				return FALSE;
			}
		}
	}
	catch(...)
	{
		WriteTrace(TraceError, _T("CheckPBXKeepLive() - CloseConnection Unexpected exception"));
	}

	return TRUE;
}

void CPCSClientSock::ResetKeepLiveTickCount()
{
	m_nKeepLiveTickCount = GetTickCount();
}
//----------------------------------------------------------------
