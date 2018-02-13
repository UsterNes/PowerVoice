// ThreadProcessGCEvents.h: interface for the CThreadProcessGCEvents class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADPROCESSGCEVENTS_H__14C2092A_7E09_44C9_B094_B40618723320__INCLUDED_)
#define AFX_THREADPROCESSGCEVENTS_H__14C2092A_7E09_44C9_B094_B40618723320__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\COMMONCLASS\MyThread.h"

#ifdef INTEL_DIALOGIC

class CThreadProcessGCEvents : protected CMyThread  
{
public:
	CThreadProcessGCEvents();
	virtual ~CThreadProcessGCEvents();

	BOOL WaitForStartupToComplete();
	void WaitForShutdownToComplete();

protected:
	virtual int Run();
	CManualResetEvent m_shutdown;
};

#endif

#endif // !defined(AFX_THREADPROCESSGCEVENTS_H__14C2092A_7E09_44C9_B094_B40618723320__INCLUDED_)
