#include "stdafx.h"
#include <iostream>

#include "XYTrace.h"

// private helper class
class XYTraceHelper
{
	// friend functions of this class
	friend void SetTraceFilePrefix(LPCTSTR strFilePrefix);
	friend void SetTraceLevel(const int nLevel);
	friend void WriteTrace(const int nLevel, LPCTSTR strFormat, ...);
	// internal data members
	HANDLE	m_hFile;
	int m_nLevel;
	long m_nThreadId;
	TCHAR* m_pTraceFilePrefix;
	SYSTEMTIME m_timeStart;
	// close the current trace file
	void CloseTraceFile()
	{
		if(m_hFile) ::CloseHandle(m_hFile);
		m_hFile = NULL;
	}
	// open a new trace file
	HANDLE OpenTraceFile()
	{
		// construct the new trace file path
		TCHAR strFilePath[1001];
		SYSTEMTIME sysTime;
		::GetLocalTime(&sysTime);
		_stprintf
		(
			strFilePath,
			_T("%s_%04d%02d%02d_%02d%02d%02d_%X.txt"),
			m_pTraceFilePrefix?m_pTraceFilePrefix:_T("Trace"),
			sysTime.wYear,
			sysTime.wMonth,
			sysTime.wDay,
			sysTime.wHour,
			sysTime.wMinute,
			sysTime.wSecond,
			::GetCurrentProcessId()
		);
		// create the new trace file
		m_hFile = CreateFile
		(
			strFilePath,
			GENERIC_WRITE,
			FILE_SHARE_READ,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);
		// if successful, save the start time variable
		if(m_hFile) m_timeStart = sysTime;
		// return the file handle
		return m_hFile;
	}
	// set lock to gain exclusive access to trace
	// functions
	void Lock()
	{
		long nThreadId = ::GetCurrentThreadId();
		while(m_nThreadId!=nThreadId)
		{
			// keep trying until successfully completed the operation
// VC6是1200   
// VC7是1300   
// VC7.1是1310
#if _MSC_VER >= 1300
			::InterlockedCompareExchange((volatile LONG*)&m_nThreadId, (LONG)nThreadId, 0);
#else
			::InterlockedCompareExchange((void **)&m_nThreadId, (void *)nThreadId, 0);
#endif
			if(m_nThreadId==nThreadId) break;
			::Sleep(25);
		}
	}
	// release lock so that other threads can access 
	// trace functions
	void Unlock()
	{
		// only the thread that set the lock can release it
// VC6是1200   
// VC7是1300   
// VC7.1是1310
#if _MSC_VER >= 1300
		::InterlockedCompareExchange((volatile LONG*)&m_nThreadId, 0, (LONG)::GetCurrentThreadId());
#else
		::InterlockedCompareExchange((void **)&m_nThreadId, 0, (void *)::GetCurrentThreadId());
#endif
	}
	// set the current trace level
	void SetTraceLevel(const int nLevel) { m_nLevel = nLevel>0?nLevel:0; }
	// set the trace file name prefix
	void SetTraceFilePrefix(LPCTSTR strFilePrefix)
	{
		// close existing trace file first
		CloseTraceFile();
		// copy the file name prefix
		int nSize = strFilePrefix?_tcslen(strFilePrefix):0;
		delete []m_pTraceFilePrefix;
		m_pTraceFilePrefix = new TCHAR[nSize+1];
		_tcscpy(m_pTraceFilePrefix, nSize>0?strFilePrefix:_T(""));
	}
public:
	// constructor and destructor
	XYTraceHelper()
	{
		m_hFile = NULL;
		m_nLevel = 0;
		m_nThreadId = 0;
		m_pTraceFilePrefix = NULL;
	}
	~XYTraceHelper()
	{
		CloseTraceFile();
		delete []m_pTraceFilePrefix;
	}
};

// the one and only instance of XYTraceHelper
XYTraceHelper theHelper;

void SetTraceFilePrefix(LPCTSTR strFilePrefix)
{
	// set lock
	theHelper.Lock();
	// set trace file name prefix
	theHelper.SetTraceFilePrefix(strFilePrefix);
	// release lock
	theHelper.Unlock();
}

void SetTraceLevel(const int nLevel)
{
	// set lock
	theHelper.Lock();
	// set trace level
	theHelper.SetTraceLevel(nLevel);
	// release lock
	theHelper.Unlock();
}

void WriteTrace(const int nLevel, LPCTSTR strFormat, ...)
{
	// if the specified trace level is greater than
	// the current trace level, return immediately
	if(theHelper.m_nLevel==0||nLevel>theHelper.m_nLevel) return;
	// set lock
	theHelper.Lock();
	try
	{
		// get local time
		SYSTEMTIME sysTime;
		::GetLocalTime(&sysTime);
		// get trace file handle	
		HANDLE hFile = theHelper.m_hFile;
		// open the trace file if not already open
		if(hFile==NULL) hFile = theHelper.OpenTraceFile();
		// if it is already a new day, close the old
		// trace file and open a new one
		else if
		(
			sysTime.wYear!=theHelper.m_timeStart.wYear||
			sysTime.wMonth!=theHelper.m_timeStart.wMonth||
			sysTime.wDay!=theHelper.m_timeStart.wDay)
		{
			theHelper.CloseTraceFile();
			theHelper.OpenTraceFile();
		}
		// write the trace message
		if(hFile)
		{
			// declare buffer (default max buffer size = 32k)
			const int nMaxSize = 32*1024;
			TCHAR pBuffer[nMaxSize+51];
			// print time stamp and thread id to buffer
			int nPos = _stprintf
			(
				pBuffer,
				_T("%02d:%02d:%02d_%03d_%X [%d] "), 
				sysTime.wHour,
				sysTime.wMinute,
				sysTime.wSecond,
				sysTime.wMilliseconds,
				theHelper.m_nThreadId,
				nLevel
			);
			// print the trace message to buffer
			va_list args;
			va_start(args, strFormat);
			nPos += _vsntprintf(pBuffer+nPos,nMaxSize,strFormat,args);
			va_end(args);
			// print the end of the line to buffer
			_stprintf(pBuffer+nPos,_T("\r\n"));
			// write the buffer to the trace file
			DWORD dwBytes;
			::WriteFile(hFile,pBuffer,_tcslen(pBuffer),&dwBytes,NULL);
			
#ifdef _DEBUG
			OutputDebugString(pBuffer);
#endif //_DEBUG

		}
	}
	catch(...)
	{
		// add code to handle exception (if needed)
	}
	// release lock
	theHelper.Unlock();
}

