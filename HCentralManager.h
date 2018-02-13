// HCentralManager.h: interface for the CHCentralManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HCENTRALMANAGER_H__1BF30FE3_12D8_4821_8E79_386A711088FD__INCLUDED_)
#define AFX_HCENTRALMANAGER_H__1BF30FE3_12D8_4821_8E79_386A711088FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef CISCO_CCM

#include "TapiObj.h"

class CHCentralManager : protected CMap<DWORD, DWORD, CTapiLine *, CTapiLine *>   
{
private:

	// All objects under control of this manager
	CWinThread		*m_pThread,
					*m_pEventThread,
					*m_pDTMFThread;

	static BOOL		s_threadTerminate;

public:
	void SleepAll();
	BOOL WakeUp();
	CHCentralManager();
	virtual ~CHCentralManager();
	CIVRObject *FindIVRObjectWithHandle(DWORD dwHandle);
	CIVRObject *FindIVRObjectWithCall(DWORD dwHandle, BYTE bytType = 1);
	DWORD GetCallInfo(DWORD dwCallInfoFlag, HCALL hCall);

	// Sun added 2011-06-16 [A]
	CIVRChannel *RematchIVRObjectWithHandle(DWORD dwHandle);

	CMutex				m_ReplyLock;			// Sun added 2010-08-04

	int GetRequesCount();
	BOOL AddRequest(DWORD dwReqID, CTapiLine *f_pLine);
	void RemoveRequest(DWORD dwReqID);

protected:
	void ProcessMessage(MSG msg);
	static UINT MessageThread(LPVOID pParam);
	static UINT TapiEventThread(LPVOID pParam);
	static UINT DTMFThread(LPVOID pParam);

	CTapiLine *FindTapiLineWithRequestID(DWORD dwReqID);
	void RemoveAllRequests();

private:
	CManulSync		m_bLock;
};

#endif

#endif // !defined(AFX_HCENTRALMANAGER_H__1BF30FE3_12D8_4821_8E79_386A711088FD__INCLUDED_)
