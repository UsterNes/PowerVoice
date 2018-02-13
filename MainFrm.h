// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__6F0AA496_D167_478D_B35A_14F0BFD48E2E__INCLUDED_)
#define AFX_MAINFRM_H__6F0AA496_D167_478D_B35A_14F0BFD48E2E__INCLUDED_

#include <afxcview.h>

#include "ListenSocket.h"
#include "MyThreadList.h"
#include "MyDBReader.h"
#include "PCSClientSock.h"
#include "PCSExtClientSock.h"
#include "IVRGroup.h"
#include "IVRChannel.h"
#include "IVRClient_Admin.h"
#include "..\CommonClass\TrayIcon.h"	// Added by ClassView
#include "..\CommonClass\DataRecorderConnect.h"
#include "TTSPackage\TTSInterface.h"
#include "ASRPackage\ASRInterface.h"	// Sun added 2008-03-10
#include "MediaDriverService.h"			// Sun added 2013-03-06

//-----------------------------------------------------------------
// Sun added 2010-03-08 for Cisco CME
/// Cisco CME Interface Include File
#ifdef CISCO_CME
#include "CMEIVR\CMEInterface.h"
#endif
//-----------------------------------------------------------------

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIVRGroupView : public CListView
{
protected: // create from serialization only
	CIVRGroupView();
	DECLARE_DYNCREATE(CIVRGroupView);

// Attributes
public:

protected:
	CImageList *m_pimagelist;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIVRGroupView)
	public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIVRGroupView();

	void Screen_Initialize_ListItmes();
	void Screen_ClearAllItems();
	BOOL Screen_InsertGroup(CIVRGroup *f_Group);
	BOOL Screen_DeleteGroup(WPARAM f_GroupNo);
	BOOL Screen_ChangeListItemIcon(WPARAM f_GroupNo);
	BOOL Screen_ChangeListItemProgram(WPARAM f_GroupNo);
	BOOL Screen_ChangeListItemResource(WPARAM f_GroupNo);
	BOOL Screen_ChangeListItemNeedCTI(WPARAM f_GroupNo);
	BOOL Screen_ChangeListItemCallCount(WPARAM f_GroupNo);

	// Read & Write Current Group ID
	int GetShowGroupNo() { return m_iShowGroupNo; }
	void SetShowGroupNo(int nGroupNo = 0);

protected:  // control bar embedded members

// Generated message map functions
protected:
	//{{AFX_MSG(CIVRGroupView)
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int				m_iShowGroupNo;

	void InitMainForm();
	BOOL FindListCtrlItem(CListCtrl *f_list, CString f_name, int *f_index);
};

class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	virtual ~CMainFrame();

	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	bool	m_bShowAllChannel;
	bool	m_bCompactChannelView;
	bool	m_bGCStarted;
	bool	m_bSystemAutoShutDown;									// 系统自动关闭
	CMyDBReader				m_DBReader;								// 读数据库
	CDataRecorderConnect	m_sckDataRecorder;						// 数据记录
		
	// TCP message functions
	BOOL			MQ_SendMsg_Register(DWORD dwThreadID);
	BOOL			MQ_SendMsg_Unregister();
	BOOL			MQ_SendExtMsg_Register();
	BOOL			MQ_SendExtMsg_Unregister();

	BOOL			GetCTIOpened();
	void			SetCTIOpened(BOOL blnValue, BOOL blnForce = FALSE);

	BOOL					m_bRunning;
	BOOL					m_bShutdown;
	CIVRCH_Collection		m_CTIAgentList;

    // list of thread pointers
    CMyThreadList m_ThreadList;

	// Sun added 2006-03-05
	BYTE m_nVerAdmin;					// 管理员版本号
	BYTE m_nVerClient;					// 用户版本号
	CIVRClient_Admin		m_IVRAdmin;

	// Sun added 2004-12-30
	/// TTS Interface
	CTTSInterface			m_TTSInterface;
	CTTSServerInfo			m_TTSServerInfo;
	int						m_nTTSBufferCount;
	int						m_nTTSBufferSize;
	UINT					m_nTTSResourceTO;

	// Sun added 2008-03-10
	/// ASR Interface
	CASRInterface			m_ASRInterface;

//-----------------------------------------------------------------
// Sun added 2010-03-08 for Cisco CME
/// Cisco CME Interface
#ifdef CISCO_CME
	CCMEInterface			m_CMEInterface;
	CCMEIVRServerInfo		m_CMEServerInfo;

	CIVRChannel *SelectChannelForRoutePoint(int nRoutePointID);
#endif
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Sun added 2012-02-08 for Cisco Media Driver
#ifdef CUCM_MEDIA
	CMediaDriverService		m_MediaDriver;
#endif
//-----------------------------------------------------------------

// Operations
public:
	LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	virtual void RecalcLayout(BOOL bNotify = TRUE);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ProgressStep();
	BOOL SB_PositionControl(CWnd * pWnd, int paneID);
	void SetMainMenuStatus(int nStatus = 0);
	void WriteStatusBar(int nPanel, int nStatus = 0);
	void WriteStatusBar(char *sMsg);
	void RefreshMainMenuOnGroup(CIVRGroup *f_pGroup);
	BOOL Sys_ConnectEXTServer();
	BOOL Sys_ConnectCTIServer();
	BOOL Sys_ConnectSecondCTIServer();			// Sun added 2012-12-29
	BOOL Sys_ConnectDataRecord();
	void MsgPrintf(UINT nTraceLevel, char *fmt, ...);

	// Sun added 2011-04-16
	/// 系统自动关闭
	void Sys_AutoCloseMe(UL lngErrorCode, UL lngReasonCode);

#ifdef CISCO_CCM
	LONG outputError(LONG nErrorID);
#endif

	int				Sys_StartSystem();
	int				Sys_StopSystem();
	int				Sys_ReInitDevices();

	CStatic			m_wndStSYS;
	CStatic			m_wndStDB;
	CStatic			m_wndStCOM;
	CStatic			m_wndStHW;
	CProgressCtrl	m_wndProgressBar;

	CString					m_param_ctiip;
	UINT					m_param_ctiport;
	CString					m_param_drip;
	UINT					m_param_drport;

	// Sun added 2010-05-18 [V7.1.8.3]
	BOOL MQ_HA_SendMsg(const BYTE *pData, size_t dataLength, BOOL blnTwoWay = FALSE);
	BYTE GetActivePCSID();
	CString					m_param_cti2ip;
	UINT					m_param_cti2port;
	CPCSClientSock			m_sckPCServer2;

	// Sun added 2006-03-05
	UINT					m_param_localport;

	// Sun added 2005-03-17
	BOOL					m_blnEnableEXT;
	BOOL					m_blnEnableTTS;
	BOOL					m_blnEnableASR;
	CString					m_param_extip;
	UINT					m_param_extport;
	CString					m_param_asrip;
	UINT					m_param_asrport;
	
	CPCSClientSock			m_sckPCServer;

	// Sun added 2005-03-17
	CPCSExtClientSock		m_sckExternalServer;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	HICON			m_hIcon;
	CStatusBar		m_wndStatusBar;
	CToolBar		m_wndToolBar;
	CSplitterWnd	m_wndSplitter;
	CTrayIcon		m_TrayIcon;
	UINT			m_main_timer;

	BOOL			m_bCTIOpened;
	BOOL			m_bLocalSocketOpened;

// Generated message map functions
protected:
	bool m_bShowMe;
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRestore();
	afx_msg void OnExit();
	afx_msg void OnUpdateRestore(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnViewAllCh();
	afx_msg void OnExeStart();
	afx_msg void OnExeRestart();
	afx_msg void OnExeRestartAll();
	afx_msg void OnExeStartAll();
	afx_msg void OnExeStop();
	afx_msg void OnExeStopAll();
	afx_msg void OnFileConfig();
	afx_msg void OnFileLoadDb();
	afx_msg void OnFileLoadFile();
	afx_msg void OnFileMruFile1();
	afx_msg void OnUpdateExeRestart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateExeRestartAll(CCmdUI* pCmdUI);
	afx_msg void OnUpdateExeStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateExeStartAll(CCmdUI* pCmdUI);
	afx_msg void OnUpdateExeStop(CCmdUI* pCmdUI);
	afx_msg void OnUpdateExeStopAll(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileConfig(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileLoadDb(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileLoadFile(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileLoadResource(CCmdUI* pCmdUI);
	afx_msg void OnFileLoadResource();
	afx_msg void OnUpdateViewAllCh(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnViewLicInfo();
	afx_msg void OnTrunkSqllog();
	afx_msg void OnFileTimerLdCf();
	afx_msg void OnFileTimerLdRes();
	afx_msg void OnUpdateFileTimerLdCf(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileTimerLdRes(CCmdUI* pCmdUI);
	afx_msg void OnViewReport();
	//}}AFX_MSG
	LRESULT OnSysAutoShutDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnSysReInitDevices(WPARAM wParam, LPARAM lParam);
	LRESULT OnClientOnline(WPARAM wParam, LPARAM lParam);
	LRESULT OnClientOffline(WPARAM wParam, LPARAM lParam);
	LRESULT OnMoveProgressBar(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	LRESULT OnHotKey(WPARAM wParam, LPARAM lParam);

private:
	void			Screen_GetWindowPosition(int *x, int *y, int *cx, int *cy);
	void			Screen_SetWindowPosition(int x, int y, int cx, int cy);
	bool			m_bME_FILE_LOAD_FILE;
	bool			m_bME_FILE_LOAD_DB;
	bool			m_bME_FILE_LOAD_RESOURCE;
	bool			m_bME_FILE_CONFIG;
	bool			m_bME_FILE_MRU_FILE1;
	bool			m_bME_FILE_MRU_FILE2;
	bool			m_bME_FILE_MRU_FILE3;
	bool			m_bME_FILE_MRU_FILE4;
	bool			m_bME_EXE_START;
	bool			m_bME_EXE_STOP;
	bool			m_bME_EXE_RESTART;
	bool			m_bME_EXE_START_ALL;
	bool			m_bME_EXE_STOP_ALL;
	bool			m_bME_EXE_RESTART_ALL;
	bool			m_bME_VIEW_ALL_CH;

	// Sun added 2006-02-26
	CTime			m_tmStatisticsDate;

	int				Sys_CreateWorkObjects(void);
	int				Sys_InitializePhysicalDev();
	BOOL			Sys_AutoDetectVoiceDev();

	void			Information_Init_SetSystemTones(CString f_Tone, int f_Index);
	int				Information_LoadIniSystemParamter();

	BOOL			HotKey_Install(UINT nIndex, TCHAR cKey);
	void			HotKey_Remove(UINT nIndex);

	// Sun added 2013-01-27 [A]
	void			CancelAllUMSTasks();

	CListenSocket	m_ListenSocket;

	// Sun added 2008-06-26
	UINT			m_nMainTimerID;
};

/////////////////////////////////////////////////////////////////////////////

// Main Windows Pointer
extern CMainFrame *pMainWindow;

// Views' Pointer
extern CIVRGroupView *pGroupView;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__6F0AA496_D167_478D_B35A_14F0BFD48E2E__INCLUDED_)
