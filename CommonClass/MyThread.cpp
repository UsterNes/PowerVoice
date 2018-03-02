#include "stdafx.h"
#include "MyThread.h"
#include <process.h>

///////////////////////////////////////////////////////////////////////////////
// CMyThread
///////////////////////////////////////////////////////////////////////////////

CMyThread::CMyThread()
   :  m_bAutoDeleteMe(false),
	  m_hThread(0),
	  m_pThread(NULL)
{
}
      
CMyThread::~CMyThread()
{
	CloseThreadHandle();
}

HANDLE CMyThread::GetHandle() const
{
	return m_hThread;
}

// Sun added 2011-06-24
void CMyThread::CloseThreadHandle()
{
	if( m_hThread != 0 )
	{
		::CloseHandle(m_hThread);
		m_hThread = 0;
	}
}

DWORD CMyThread::GetThreadID() const
{
	if( m_pThread )
		return m_pThread->m_nThreadID;

	return 0;
}

void CMyThread::Start(DWORD f_nStackSize, bool f_bAutoDelete)
{
	if( m_hThread == 0 )
	{
		m_bAutoDeleteMe = f_bAutoDelete;
		m_pThread = AfxBeginThread(
						(AFX_THREADPROC)ThreadFunction, 
						(LPVOID)this,
						THREAD_PRIORITY_NORMAL,
						f_nStackSize);
		if( m_pThread != NULL )
			m_hThread = m_pThread->m_hThread;
		else
			WriteTrace(TraceError, "Error : can't create mythread - CMyThread::Start() - AfxBeginThread()");
	}
}

void CMyThread::Wait() const
{
	if( !Wait(INFINITE) )
		WriteTrace(TraceWarn, "Warning : Unexpected timeout on infinite wait - CMyThread::Wait()");
}

bool CMyThread::Wait(DWORD timeoutMillis) const
{
	bool ok;

	if( m_hThread == 0 )
		return true;

	DWORD result = ::WaitForSingleObject(m_hThread, timeoutMillis);

	if( result == WAIT_TIMEOUT )
	{
		ok = false;
	}
	else if ( result == WAIT_OBJECT_0 )
	{
		ok = true;
	}
	else
	{
		WriteTrace(TraceWarn, "Warning : WaitForSingleObject get error %d - CMyThread::Wait()", ::GetLastError());
	}

	return ok;
}

int CMyThread::ToRunThread()
{
	int result;

	result = Run();

	m_hThread = 0;

	if( m_bAutoDeleteMe )
		delete this;

	return result;
}

unsigned int __stdcall CMyThread::ThreadFunction(void *pV)
{
	int result = 0;

	CMyThread* pThis = (CMyThread*)pV;

	if( pThis )
	{
		try
		{
			result = pThis->ToRunThread();
		}
		catch(...)
		{
		}
	}

	return result;
}
