// PCSCommunicationEvent.h: interface for the CPCSCommunicationEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCSCommunicationEvent_H__5A43A1D5_09CE_4447_8C26_CBDA24DEBE3B__INCLUDED_)
#define AFX_PCSCommunicationEvent_H__5A43A1D5_09CE_4447_8C26_CBDA24DEBE3B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"

class CPCSCommunicationEvent : public CManualResetEvent  
{
public:
	CPCSCommunicationEvent();
	virtual ~CPCSCommunicationEvent();

	bool Reset();
	bool Pulse();

	BYTE m_pData[PCS_MAX_MESSAGE_SIZE];
};

#endif // !defined(AFX_PCSCommunicationEvent_H__5A43A1D5_09CE_4447_8C26_CBDA24DEBE3B__INCLUDED_)
