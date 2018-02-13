#if !defined(AFX_PCSCLIENTSOCK_H__5B6F0206_16D6_4218_91AE_A80766DF748D__INCLUDED_)
#define AFX_PCSCLIENTSOCK_H__5B6F0206_16D6_4218_91AE_A80766DF748D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PCSClientSock.h : header file
//

#include "..\CommonClass\_DataQueue.h"
#include "..\CommonClass\MessageQ.h"
#include "..\CommonClass\MyThread.h"

/////////////////////////////////////////////////////////////////////////////
// CPCSClientSock command target

class CPCSClientSock
 : public CAsyncSocket,
   protected CMyThread,
   protected CFastMessageQ
{
// Attributes
public:
	BOOL SendProtocolData(US f_nCmd, US f_nLen = 0, UC *f_pData = NULL);

// Operations
public:
	CPCSClientSock();
	virtual ~CPCSClientSock();

// Overrides
public:
	// ����Socket����
	BOOL WaitForStartupToComplete(LPCSTR strServerIP, UINT nServerPort, HWND hWnd = NULL);

	// �ر�Socket����
	void WaitForShutdownToComplete();

	// ͬ������Ϣ
	BOOL SendControlData(const BYTE *pData, size_t dataLength);

	// ͨ����Ϣ���з���Ϣ
	int AddMessage(const BYTE *f_data, size_t f_len, DWORD f_Tag = 0);

	// �Ƿ�����
	BOOL GetConnected();

	// Sun added 2010-05-18 [V7.1.8.3]
	PCS_PCSStatus GetPCSStatus();
	void SetPCSStatus(PCS_PCSStatus f_sw);
	UC GetPCSIndex();
	void SetPCSIndex(BYTE bytIndex);
	BOOL CompareThreadID(DWORD dwThreadID);

	// �Ͽ�����
	void CloseConnection();

	// ���������Ϣ
	BOOL CheckPBXKeepLive();

	size_t GetMinimumMessageSize() const;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPCSClientSock)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CPCSClientSock)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	CManualResetEvent m_shutdown;

	// Sun added 2010-05-18 [V7.1.8.3]
	PCS_PCSStatus			m_stPCS;			// ��ǰPCS��״̬
	UC						m_bytPCSIndex;		// ��ǰPCS��ID�ţ�0 based

	BOOL InitSock();
	void ResetKeepLiveTickCount();
	void SetConnected(BOOL f_sw);
	virtual int Run();
	void GetRequests();
	int ProcessRequests();
	size_t GetMessageSize();

private:
	CDataQueue				m_DataQueue;
	CManualResetEvent		m_hNeedWait;
	SMsgHeader				m_rcvhdr;
	SMsgHeader				m_sndhdr;
	char					m_strServerIP[20];
	UINT					m_nServerPort;
	BOOL					m_bConnected;
	HWND					m_hWndOwner;
	CManulSync				m_bLock;

	DWORD					m_nKeepLiveTickCount;				// Sun added 2006-02-26
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PCSCLIENTSOCK_H__5B6F0206_16D6_4218_91AE_A80766DF748D__INCLUDED_)
