#ifndef __MYTHREAD_H
#define __MYTHREAD_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyThread 
{
public :
	CMyThread();

	virtual ~CMyThread();

	HANDLE GetHandle() const;
	DWORD GetThreadID() const;

	void Wait() const;

	bool Wait(DWORD timeoutMillis) const;

	void Start(DWORD f_nStackSize = 0, bool f_bAutoDelete = false);

	// Sun added 2011-06-24
	/// ǿ�ƻ����߳̾��������ǿ����ֹ�߳�ʱ�����ֳ����Ա��߳��ܹ���������
	void CloseThreadHandle();

private :
	int ToRunThread();
	virtual int Run() = 0;

	static unsigned int __stdcall ThreadFunction(void *pV);

	bool			m_bAutoDeleteMe;
	HANDLE			m_hThread;
	CWinThread		*m_pThread;
	DWORD			m_nStackSize;
};

#endif // __MYTHREAD_H
