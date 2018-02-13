// ThreadSysMain.h: interface for the CThreadSysMain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADSYSMAIN_H__9E77B6E5_5322_4BF8_905F_D0793F014690__INCLUDED_)
#define AFX_THREADSYSMAIN_H__9E77B6E5_5322_4BF8_905F_D0793F014690__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\COMMONCLASS\MyThread.h"

class CThreadSysMain : protected CMyThread  
{
public:
	CThreadSysMain();
	virtual ~CThreadSysMain();

	BOOL WaitForStartupToComplete();
	void WaitForShutdownToComplete();

	// 2004-08-07
	DWORD GetPendingTime();
	void ResetPendingTime();
	int m_threadProgress;
	DWORD m_dwPendTick;

protected:
	virtual int Run();
	CManualResetEvent m_shutdown;
};

#endif // !defined(AFX_THREADSYSMAIN_H__9E77B6E5_5322_4BF8_905F_D0793F014690__INCLUDED_)
