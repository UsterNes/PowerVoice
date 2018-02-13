// IVRClient_MSI.cpp: implementation of the CIVRClient_MSI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PowerVoice.h"
#include "IVRClient_MSI.h"
#include "ClientThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIVRClient_MSI::CIVRClient_MSI(UL lngAgentID)
:	m_lngAgentID(lngAgentID),
	m_pSocket(NULL),
	m_nVersionNo(0)
{

}

CIVRClient_MSI::~CIVRClient_MSI()
{

}

UL CIVRClient_MSI::GetAgentID()
{
	return m_lngAgentID;
}

BYTE CIVRClient_MSI::GetVersion()
{
	return m_nVersionNo;
}

void CIVRClient_MSI::SetVersion(BYTE f_nValue)
{
	if( m_nVersionNo != f_nValue )
		m_nVersionNo = f_nValue;
}

PVOID CIVRClient_MSI::GetConnected()
{
	PVOID lv_cn;

	m_bLock.Enter();
	lv_cn = m_pSocket;
	m_bLock.Leave();

	return lv_cn;
}

void CIVRClient_MSI::SetConnected(PVOID f_cn)
{
	m_bLock.Enter();

	if( m_pSocket == NULL )
		m_pSocket = f_cn;

	m_bLock.Leave();
}

void CIVRClient_MSI::SetDisconnected()
{
	m_bLock.Enter();
	if( m_pSocket != NULL )
		m_pSocket = NULL;
	m_bLock.Leave();
}

int CIVRClient_MSI::ProcessTCPMessage(SMsgHeader f_hdr, const BYTE *f_bdy)
{
	///////////////////////////////////////////
	// Set Header
	SMsgHeader lv_sndhdr;
	memset(&lv_sndhdr, 0x00, sizeof(lv_sndhdr));
	lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
	lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
	lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
	lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
	lv_sndhdr.Sender = USER_IVR;
	lv_sndhdr.Receiver = USER_AGT_MSI;
	///////////////////////////////////////////

	//BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];
	CClientThread *lv_pThread = (CClientThread *)GetConnected();

	if( lv_pThread == NULL )
		return 0;

	/*
	switch( f_hdr.Command )
	{
		break;
	}*/

	return 1;
}