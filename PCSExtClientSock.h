// PCSExtClientSock.h: interface for the CPCSExtClientSock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCSEXTCLIENTSOCK_H__2B3F873D_20AC_45EC_8E0B_77A94EF398C5__INCLUDED_)
#define AFX_PCSEXTCLIENTSOCK_H__2B3F873D_20AC_45EC_8E0B_77A94EF398C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\CommonClass\_DataQueue.h"
#include "..\CommonClass\MessageQ.h"
#include "..\CommonClass\MyThread.h"

class CPCSExtClientSock  
 : public CAsyncSocket,
   protected CMyThread,
   protected CFastMessageQ
{
public:
	CPCSExtClientSock();
	virtual ~CPCSExtClientSock();

	BOOL SendProtocolData(US f_nCmd, US f_nLen = 0, UC *f_pData = NULL);

	// 启动Socket服务
	BOOL WaitForStartupToComplete(LPCSTR strServerIP, UINT nServerPort, HWND hWnd = NULL);

	// 关闭Socket服务
	void WaitForShutdownToComplete();

	// 同步发消息
	BOOL SendControlData(const BYTE *pData, size_t dataLength);

	// 通过消息队列发消息
	int AddMessage(const BYTE *f_data, size_t f_len, DWORD f_Tag = 0);

	// 是否连接
	BOOL GetConnected();

	// 断开连接
	void CloseConnection();

	size_t GetMinimumMessageSize() const;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPCSExtClientSock)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CPCSExtClientSock)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

protected:
	CManualResetEvent m_shutdown;

	BOOL InitSock();
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
};

#endif // !defined(AFX_PCSEXTCLIENTSOCK_H__2B3F873D_20AC_45EC_8E0B_77A94EF398C5__INCLUDED_)
