// MyThreadList.h: interface for the CMyThreadList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYTHREADLIST_H__272361F2_2B76_4971_85CD_3D4064394896__INCLUDED_)
#define AFX_MYTHREADLIST_H__272361F2_2B76_4971_85CD_3D4064394896__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "ClientThread.h"

class CManulSync;

class CMyThreadList
{
public:
	CMyThreadList();
	virtual ~CMyThreadList();

	BOOL AddThread(CClientThread *f_pThread);
	BOOL RemoveThread(CClientThread *f_pThread);
	void RemoveAllThreads();
	void CheckAllThreads();

private:
	CManulSync m_bLock;
	CTypedPtrList<CObList, CClientThread*>  m_ThreadList;
};

#endif // !defined(AFX_MYTHREADLIST_H__272361F2_2B76_4971_85CD_3D4064394896__INCLUDED_)
