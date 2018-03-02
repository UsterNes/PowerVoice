#if !defined(AFX_DATARECORDERCONNECT_H__5C9D1837_BA28_47B8_A818_90A659AA1F22__INCLUDED_)
#define AFX_DATARECORDERCONNECT_H__5C9D1837_BA28_47B8_A818_90A659AA1F22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataRecorderConnect.h : header file
//

#include "..\CommonClass\_DataQueue.h"
#include "..\CommonClass\MessageQ.h"
#include "..\CommonClass\MyThread.h"
#include "..\CommonClass\MySQLLog.h"

/////////////////////////////////////////////////////////////////////////////
// CDataRecorderConnect command target

//------------------------------------------------------------------------
// DataRecorder Class �汾����
//------------------------------------------------------------------------
#define DEF_DRC_VERSION_MAIN					2
#define DEF_DRC_VERSION_SUB						1
#define DEF_DRC_VERSION							(DEF_DRC_VERSION_MAIN * 16 + DEF_DRC_VERSION_SUB)

// DataRecorder ����������
#define DEF_DRC_SCK_MAXKEEPLIVE_INTERVAL		15

class CDataRecorderConnect
 : public CAsyncSocket,
   public CMySQLLog,				// Sun added 2004-11-05
   protected CMyThread,
   protected CFastMessageQ
{
// Attributes
public:
	BOOL SendProtocolData(US f_nCmd, US f_nLen = 0, UC *f_pData = NULL);

// Operations
public:
	CDataRecorderConnect();
	virtual ~CDataRecorderConnect();

// Overrides
public:
	void SetSenderParty(UC bytSender);
	void ChangeQueueSize(int nNewSize);

	// ����Socket����
	BOOL WaitForStartupToComplete(LPCSTR strServerIP, UINT nServerPort, HWND hWnd = NULL);

	// �ر�Socket����
	void WaitForShutdownToComplete();

	// ͨ����Ϣ���з���Ϣ
	int AddMessage(CString f_strSQL);

	// �Ƿ�����
	BOOL GetConnected();

	// �Ͽ�����
	void CloseConnection();

	// Sun added 2007-05-22, [A]
	BOOL IsSockTimeOut();

	size_t GetMinimumMessageSize() const;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataRecorderConnect)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CDataRecorderConnect)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
	// ͬ������Ϣ
	BOOL SendControlData(const BYTE *pData, size_t dataLength);
	CManualResetEvent m_shutdown;

	BOOL InitSock();
	void SetConnected(BOOL f_sw);
	virtual int Run();
	void GetRequests();
	int ProcessRequests();
	size_t GetMessageSize();

	// Sun added 2004-12-09
	void RemoveAllQueuedSQLStatements();

private:
	// Sun added 2007-05-22, [A]
	CTime					m_tmRecTime;			// ������ʱ�䣬�����ж��Ƿ�����������

	CDataQueue				m_DataQueue;
	CManualResetEvent		m_hNeedWait;
	SMsgHeader				m_rcvhdr;
	SMsgHeader				m_sndhdr;
	char					m_strServerIP[20];
	UINT					m_nServerPort;
	BOOL					m_bConnected;
	HWND					m_hWndOwner;
	CManulSync				m_bLock;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATARECORDERCONNECT_H__5C9D1837_BA28_47B8_A818_90A659AA1F22__INCLUDED_)
