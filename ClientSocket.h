// ClientSocket.h : header file

#if !defined(AFX_CLIENTSOCKET_H__B7C54BD3_A555_11D0_8996_00AA00B92B2E__INCLUDED_)
#define AFX_CLIENTSOCKET_H__B7C54BD3_A555_11D0_8996_00AA00B92B2E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "..\CommonClass\_DataQueue.h"
#include "..\CommonClass\MessageQ.h"

#define DEF_SCK_MAX_REGISTER_DELAY			10
#define DEF_SCK_KEEPLIVE_INTERVAL			5

class CClientThread;

class CClientSocket : 
	public CSocket,
	public CFastMessageQ

{
	friend CClientThread;

// Operations
public:
	CClientSocket();
	virtual ~CClientSocket();

	CClientThread* m_pThread;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientSocket)
	public:
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CClientSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	void CloseConnection();
	BOOL SendControlData(const BYTE *pData, size_t dataLength);
	BOOL SendTextMessage(LPCTSTR lpszMessage, BOOL blnClose = FALSE);
	BOOL SendServerTimeMessage();
	
	BOOL KeepLiveChecking();

	void *GetWorkObject(const BYTE f_Type);
	void *GetWorkObject(BYTE *f_Type);
	BYTE GetWorkObjectType();
	void ClearWorkObject();

	size_t GetMinimumMessageSize() const;
	DWORD GetConnectionLength();
	long GetBindingLength();
	void SetWorkObject(void *f_prt = NULL, BYTE f_Type = USER_NULL);
	int AddMessage(const BYTE *f_data, size_t f_len, DWORD f_Tag = 0);
	
// Implementation
protected:
	CDataQueue	m_DataQueue;
	CManulSync	m_sync;

	void GetRequests();
	int ProcessRequests();

	size_t GetMessageSize();

	int ProcessMsg_Admin();
	int ProcessMsg_Client();

private:
	// Keep Live Count
	CMutexValueManger m_bKeepLiveCount;
	SMsgHeader	m_keeplivemsg;

	SMsgHeader m_rcvhdr;
	SMsgHeader m_sndhdr;

	void *m_pWorkObjPtr;
	BYTE m_bytWorkObjType;

	CTime m_tmConnected;					// 连接开始时间
	CTime m_tmBinding;						// 绑定开始时间
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCKET_H__B7C54BD3_A555_11D0_8996_00AA00B92B2E__INCLUDED_)
