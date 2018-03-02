// PCSCommunicationEvent.cpp: implementation of the CPCSCommunicationEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PCSCommunicationEvent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPCSCommunicationEvent::CPCSCommunicationEvent()
 : m_nEventID(0),
   m_nTag(0)
{
	memset(m_pData, 0x00, sizeof(m_pData));
}

CPCSCommunicationEvent::~CPCSCommunicationEvent()
{
}

bool CPCSCommunicationEvent::Reset()
{
	memset(m_pData, 0x00, sizeof(m_pData));
	return CManualResetEvent::Reset();
}

bool CPCSCommunicationEvent::Pulse()
{
	memset(m_pData, 0x00, sizeof(m_pData));
	return CManualResetEvent::Pulse();
}