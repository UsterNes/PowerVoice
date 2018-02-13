// MyThreadList.cpp: implementation of the CMyThreadList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyThreadList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyThreadList::CMyThreadList()
{

}

CMyThreadList::~CMyThreadList()
{
	RemoveAllThreads();
}

BOOL CMyThreadList::AddThread(CClientThread *f_pThread)
{
	m_bLock.Enter();
	m_ThreadList.AddTail(f_pThread);
	m_bLock.Leave();

	return TRUE;
}

BOOL CMyThreadList::RemoveThread(CClientThread *f_pThread)
{
	m_bLock.Enter();

	POSITION pos = m_ThreadList.Find(f_pThread);
	if(pos != NULL)
		m_ThreadList.RemoveAt(pos);

	m_bLock.Leave();

	return TRUE;
}

void CMyThreadList::CheckAllThreads()
{
	CClientThread *pThread = NULL;

	m_bLock.Enter();

	try
	{
		POSITION pos = m_ThreadList.GetHeadPosition();
		while( pos != NULL )
		{
			pThread = (CClientThread *)m_ThreadList.GetNext(pos);
			if( pThread )
				pThread->PostThreadMessage(WM_NULL,0,0);
		}
	}
	catch(...)
	{
	}

	m_bLock.Leave();
}

void CMyThreadList::RemoveAllThreads()
{
	CClientThread *pThread = NULL;

	// close all running threads
	do
	{
		m_bLock.Enter();

		POSITION pos = m_ThreadList.GetHeadPosition();
		if( pos != NULL )
		{
			pThread = (CClientThread *)m_ThreadList.GetAt(pos);
		
			m_bLock.Leave();

			// save thread members
			int nThreadID = pThread->m_nThreadID;
			HANDLE hThread = pThread->m_hThread;

			WriteTrace(TraceDebug, "Shutting down thread (id: %X)...", nThreadID);

			// tell thread to stop
			pThread->m_ClientSocket.SetWorkObject();
			pThread->SetThreadPriority(THREAD_PRIORITY_HIGHEST);
			pThread->PostThreadMessage(WM_QUIT,0,0);

			// wait for thread to end, while keeping the messages pumping (max 5 seconds)
			if( AfmWaitWithMessageLoop(1, &hThread, TRUE, 5000) == WAIT_TIMEOUT )
			{
				// thread doesn't want to stopped
				WriteTrace(TraceWarn, "Problem while killing thread.");
				// don't try again, so remove
				m_bLock.Enter();
				POSITION rmPos = m_ThreadList.Find(pThread);
				if (rmPos != NULL)
					m_ThreadList.RemoveAt(rmPos);
				m_bLock.Leave();
			}
			else
			{
				WriteTrace(TraceDebug, "Thread successfully stopped.");
			}
		}
		else
		{
			m_bLock.Leave();
			pThread = NULL;
		}
	}
	while (pThread != NULL);
}
