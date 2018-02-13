// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "PowerVoice.h"
#include "Splash.h"  // e.g. splash.h

#include "MainFrm.h"
#include "BrowsePath.h"
#include "PowerVoiceView.h"
#include "ConfigBox.h"
#include "DLGSELERESOURCE.h"
#include "DlgSeleCallFlowTable.h"
#include "DlgGetPassword.h"
#include "DlgViewLicense.h"
#include "DlgAutoLoadResource.h"
#include "DlgAutoLoadCallFlow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MYWM_NOTIFYICON				WM_USER + 100
#define MYTIP						"PowerVoice IVR Monitor"

#define HOTKEY_BASE					100
#define HOTKEY_SHOW_WINDOW			HOTKEY_BASE + 0 
#define HOTKEY_LAST					HOTKEY_SHOW_WINDOW

#define DEF_SYSTEM_ROUTINE_CHECK_INTERVAL			20

CString GetPCSStatusName(PCS_PCSStatus f_Status);

// Main Windows Pointer
CMainFrame *pMainWindow;

// Group View Pointer
CIVRGroupView *pGroupView;

/////////////////////////////////////////////////////////////////////////////
// CIVRGroupView
IMPLEMENT_DYNCREATE(CIVRGroupView, CListView)

BEGIN_MESSAGE_MAP(CIVRGroupView, CListView)
	//{{AFX_MSG_MAP(CListView)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CIVRGroupView construction/destruction

CIVRGroupView::CIVRGroupView()
{
	// TODO: add member initialization code here
	m_pimagelist = NULL;
	pGroupView = this;
	m_iShowGroupNo = 0;
}

CIVRGroupView::~CIVRGroupView()
{
	if( m_pimagelist )
		delete m_pimagelist;
}

void CIVRGroupView::OnDraw(CDC* pDC)
{
	//CSSTIVRDoc* pDoc = GetDocument();
	//ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CIVRGroupView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	// Init Form
	InitMainForm();
}

void CIVRGroupView::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int lv_index = pNMListView->iItem;
	
	//TRACE("List Item Changed with attribute: %u\n", pNMListView->uChanged);
	if( pNMListView->uChanged == LVIF_STATE && m_iShowGroupNo != lv_index )
	{
		SetShowGroupNo(lv_index);
		if( !pMainWindow->m_bShowAllChannel )
			pChannelView->Screen_ShowIVRChannelInfo(PIVRGroup[lv_index]);
	}

	*pResult = 0;
}

LRESULT CIVRGroupView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message)
	{
	case MYWM_CHANGE_GROUP:		// Modify ivr group info
		if( pMainWindow->m_bRunning )
		{
			if( wParam < theApp.m_nIVRGroups )
			{
				switch( lParam )
				{
				case SCR_GROUP_ITEM_GROUPID:
					// Only ICNO is changed
					Screen_ChangeListItemIcon(wParam);
					break;
				case SCR_GROUP_ITEM_PROGRAM:
					// Only IVR Program is changed
					Screen_ChangeListItemProgram(wParam);			
					break;
				case SCR_GROUP_ITEM_RESOURCE:
					// Only IVR Resource is changed
					Screen_ChangeListItemResource(wParam);
					break;
				case SCR_GROUP_ITEM_CTI:
					// Only IVR Resource is changed
					Screen_ChangeListItemNeedCTI(wParam);
					break;

				// Sun added 2006-02-26
				case SCR_GROUP_ITEM_CALLCOUNT:
					// Only Call Count Status is changed
					Screen_ChangeListItemCallCount(wParam);
					break;

				default:
					Screen_InsertGroup(PIVRGroup[wParam]);
					break;
				}
			}
			else
			{
				Screen_DeleteGroup(wParam);
			}
		}
		break;
	}
	return CListView::WindowProc(message, wParam, lParam);
}

// Initialize Main Form
void CIVRGroupView::InitMainForm()
{
	CString lv_sCaption;

	CListCtrl& control = this->GetListCtrl();
	m_pimagelist = new CImageList();

	control.ModifyStyle(LVS_ICON,LVS_REPORT|LVS_SORTASCENDING,0);
	control.DeleteAllItems();
	control.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_pimagelist->Create(16,16,TRUE,2,0);
	m_pimagelist->Add(theApp.LoadIcon(IDI_Error));
	m_pimagelist->Add(theApp.LoadIcon(IDI_Stop));
	m_pimagelist->Add(theApp.LoadIcon(IDI_Ready));
	m_pimagelist->Add(theApp.LoadIcon(IDI_Start));

	control.SetImageList(m_pimagelist, LVSIL_SMALL);

	// Create the GroupList columns
	lv_sCaption.LoadString(IDS_CAP_IVRGR_GROUPNO);
	control.InsertColumn(SCR_GROUP_ITEM_GROUPID, lv_sCaption, LVCFMT_LEFT, 50, SCR_GROUP_ITEM_GROUPID);

	lv_sCaption.LoadString(IDS_CAP_IVRGR_CAPTION);
	control.InsertColumn(SCR_GROUP_ITEM_CAPTION, lv_sCaption, LVCFMT_LEFT, 140, SCR_GROUP_ITEM_CAPTION);

	lv_sCaption.LoadString(IDS_CAP_IVRGR_CURIVR);
	control.InsertColumn(SCR_GROUP_ITEM_PROGRAM, lv_sCaption, LVCFMT_LEFT, 100, SCR_GROUP_ITEM_PROGRAM);

	lv_sCaption.LoadString(IDS_CAP_IVRGR_RESID);
	control.InsertColumn(SCR_GROUP_ITEM_RESOURCE, lv_sCaption, LVCFMT_LEFT, 40, SCR_GROUP_ITEM_RESOURCE);

	lv_sCaption.LoadString(IDS_CAP_IVRGR_STARTCH);
	control.InsertColumn(SCR_GROUP_ITEM_STARTCH, lv_sCaption, LVCFMT_LEFT, 60, SCR_GROUP_ITEM_STARTCH);

	lv_sCaption.LoadString(IDS_CAP_IVRGR_ENDCH);
	control.InsertColumn(SCR_GROUP_ITEM_ENDCH, lv_sCaption, LVCFMT_LEFT, 60, SCR_GROUP_ITEM_ENDCH);

	lv_sCaption.LoadString(IDS_CAP_IVRGR_CTIST);
	control.InsertColumn(SCR_GROUP_ITEM_CTI, lv_sCaption, LVCFMT_LEFT, 60, SCR_GROUP_ITEM_CTI);

	//----------------------------------------------------------
	// Sun added 2006-02-26
	lv_sCaption.LoadString(IDS_CAP_IVRGR_CALLCOUNT);
	control.InsertColumn(SCR_GROUP_ITEM_CALLCOUNT, lv_sCaption, LVCFMT_LEFT, 80, SCR_GROUP_ITEM_CALLCOUNT);

	lv_sCaption.LoadString(IDS_CAP_IVRGR_LASTLOADTIME);
	control.InsertColumn(SCR_GROUP_ITEM_LASTLOADTIME, lv_sCaption, LVCFMT_LEFT, 130, SCR_GROUP_ITEM_LASTLOADTIME);
	//----------------------------------------------------------
}

// Find item from list
BOOL CIVRGroupView::FindListCtrlItem(CListCtrl *f_list, CString f_name, int *f_index)
{
	int lv_Start, lv_End, lv_Mid;

	// 按照事件表处理(二分法查找)
	lv_Start = 0;
	lv_End = f_list->GetItemCount()-1;

	while(lv_Start <= lv_End)
	{
		lv_Mid = (int)((lv_Start + lv_End) / 2);
		if(f_list->GetItemText(lv_Mid, 0) == f_name) 
		{
			*f_index = lv_Mid;
			return TRUE;
		}
		if(f_list->GetItemText(lv_Mid, 0) < f_name) 
		{
			lv_Start = lv_Mid + 1;
		}
		else
		{
			lv_End = lv_Mid - 1;
		}
	}

	return FALSE;
}

void CIVRGroupView::SetShowGroupNo(int nGroupNo)
{	
	CIVRGroup *lv_pGroup;

	if( nGroupNo >= 0 && nGroupNo < theApp.m_nIVRGroups )
	{
		if( (lv_pGroup = PIVRGroup[nGroupNo]) != NULL )
		{
			m_iShowGroupNo = nGroupNo;
			pMainWindow->RefreshMainMenuOnGroup(lv_pGroup);
			return;
		}
	}

	// Set Main Menu to No Current Group State
	pMainWindow->SetMainMenuStatus(MAIN_FRM_STATUS_NOCUR);
}

void CIVRGroupView::Screen_ClearAllItems()
{
	CListCtrl& lv_wndGroup = GetListCtrl();
	if (lv_wndGroup.GetItemCount() > 0)
		lv_wndGroup.DeleteAllItems();
}

void CIVRGroupView::Screen_Initialize_ListItmes()
{
	int lv_loop;

	// Clear Screen
	Screen_ClearAllItems();

	// Add Items to Channel List
	for(lv_loop = 0; lv_loop < theApp.m_nIVRGroups; lv_loop++)
	{
		if( PIVRGroup[lv_loop] != NULL )
			Screen_InsertGroup(PIVRGroup[lv_loop]);
	}
}

BOOL CIVRGroupView::Screen_ChangeListItemProgram(WPARAM f_GroupNo)
{
    int item;
	CString GroupNo;
	CListCtrl& lv_wndGroup = GetListCtrl();
	CIVRGroup *lv_pGroup = PIVRGroup[f_GroupNo];
	CString ProgramID;

	GroupNo.Format("%3u", f_GroupNo);
	
	// Find Delete location
	if( FindListCtrlItem(&lv_wndGroup, GroupNo, &item) )
	{
		switch( lv_pGroup->GetIVRProgramID() )
		{
		case DEF_LD_GRP_DEFAULT_ET:
			ProgramID.LoadString(IDS_CAP_IVRGP_DEFAULT_CF);
			break;
		case DEF_LD_GRP_ET_FILE:
			ProgramID = lv_pGroup->GetIVRProgramCaption();
			break;
		default:
			ProgramID.Format("%u %s", lv_pGroup->GetIVRProgramID(), (const char *)lv_pGroup->GetIVRProgramCaption());
			break;
		}
		lv_wndGroup.SetItemText(item,SCR_GROUP_ITEM_PROGRAM,ProgramID);

		// Sun added 2006-02-26
		CTime lv_tCurrTime;
		lv_tCurrTime = CTime::GetCurrentTime();
		lv_wndGroup.SetItemText(item,SCR_GROUP_ITEM_LASTLOADTIME,lv_tCurrTime.Format("%Y-%m-%d %H:%M:%S"));

		lv_wndGroup.Update(TRUE);
		
		return TRUE;
	}

	return FALSE;
}

BOOL CIVRGroupView::Screen_ChangeListItemResource(WPARAM f_GroupNo)
{
    int item;
	CString GroupNo;
	CListCtrl& lv_wndGroup = GetListCtrl();
	CIVRGroup *lv_pGroup = PIVRGroup[f_GroupNo];
	CString ResourceID;

	GroupNo.Format("%3u", f_GroupNo);
	
	// Find Modify location
	if( FindListCtrlItem(&lv_wndGroup, GroupNo, &item) )
	{
		ResourceID.Format("#%u", lv_pGroup->GetResourcePrjID());
		lv_wndGroup.SetItemText(item,SCR_GROUP_ITEM_RESOURCE,ResourceID);
		lv_wndGroup.Update(TRUE);
		
		return TRUE;
	}

	return FALSE;
}

// Sun added 2006-02-26
BOOL CIVRGroupView::Screen_ChangeListItemCallCount(WPARAM f_GroupNo)
{
    int item;
	CString GroupNo;
	CListCtrl& lv_wndGroup = GetListCtrl();
	CIVRGroup *lv_pGroup = PIVRGroup[f_GroupNo];

	GroupNo.Format("%3u", f_GroupNo);
	
	// Find Modify location
	if( FindListCtrlItem(&lv_wndGroup, GroupNo, &item) )
	{
		CString lv_sCallCount;
		lv_sCallCount.Format("%u/%u", lv_pGroup->m_DailyCallCount.GetValue(), lv_pGroup->m_YTDCallCount.GetValue());
		lv_wndGroup.SetItemText(item,SCR_GROUP_ITEM_CALLCOUNT,lv_sCallCount);
		lv_wndGroup.Update(TRUE);
		
		return TRUE;
	}

	return FALSE;
}

BOOL CIVRGroupView::Screen_ChangeListItemNeedCTI(WPARAM f_GroupNo)
{
    int item;
	CString GroupNo;
	CListCtrl& lv_wndGroup = GetListCtrl();
	CIVRGroup *lv_pGroup = PIVRGroup[f_GroupNo];
	CString szCTIStatus;

	GroupNo.Format("%3u", f_GroupNo);
	
	// Find Modify location
	if( FindListCtrlItem(&lv_wndGroup, GroupNo, &item) )
	{
		if( lv_pGroup->GetNeedCTI() )
			szCTIStatus.LoadString(IDS_CAP_IVRGP_CTI_CONNECT);
		else
			szCTIStatus.LoadString(IDS_CAP_IVRGP_CTI_NOUSE);

		lv_wndGroup.SetItemText(item,SCR_GROUP_ITEM_CTI,szCTIStatus);
		lv_wndGroup.Update(TRUE);
		
		return TRUE;
	}

	return FALSE;
}

BOOL CIVRGroupView::Screen_ChangeListItemIcon(WPARAM f_GroupNo)
{
    int item;
	CString GroupNo;
	CListCtrl& lv_wndGroup = GetListCtrl();
	CIVRGroup *lv_pGroup = PIVRGroup[f_GroupNo];

	GroupNo.Format("%3u", f_GroupNo);
	
	// Find Delete location
	if( FindListCtrlItem(&lv_wndGroup, GroupNo, &item) )
	{
		if(lv_pGroup->GetEnabled())
			if(lv_pGroup->GetStarted())
				lv_wndGroup.SetItem(item, SCR_GROUP_ITEM_GROUPID, LVIF_IMAGE, GroupNo, ICON_Ready, 0, 0, 0);
			else
				lv_wndGroup.SetItem(item, SCR_GROUP_ITEM_GROUPID, LVIF_IMAGE, GroupNo, ICON_Error, 0, 0, 0);
		else
			lv_wndGroup.SetItem(item, SCR_GROUP_ITEM_GROUPID, LVIF_IMAGE, GroupNo, ICON_Stop, 0, 0, 0);

		lv_wndGroup.SetItemText(item, SCR_GROUP_ITEM_GROUPID, GroupNo);
		lv_wndGroup.Update(TRUE);
		
		return TRUE;
	}

	return FALSE;
}

BOOL CIVRGroupView::Screen_DeleteGroup(WPARAM f_GroupNo)
{
    int item;
	CString GroupNo;
	CListCtrl& lv_wndGroup = GetListCtrl();

	GroupNo.Format("%3u", f_GroupNo);
	
	// Find Delete location
	if( FindListCtrlItem(&lv_wndGroup, GroupNo, &item) )
	{
		lv_wndGroup.DeleteItem(item);
		lv_wndGroup.Update(TRUE);
		return TRUE;
	}

	return FALSE;
}

BOOL CIVRGroupView::Screen_InsertGroup(CIVRGroup *f_Group)
{
    int item;
	int image;
	CString GroupNo;
	CListCtrl& lv_wndGroup = GetListCtrl();

	if(f_Group->GetEnabled())
		if(f_Group->GetStarted())
			image=ICON_Ready;
		else
			image=ICON_Error;
	else
		image=ICON_Stop;

	 GroupNo.Format("%3u", f_Group->GetGroupNO());
	
	// Find Insert location
	// if occur an existed item, then replace it,
	// otherwise insert new one
	if( FindListCtrlItem(&lv_wndGroup, GroupNo, &item) )
	{
		// Modify old item
		lv_wndGroup.SetItem(item, SCR_GROUP_ITEM_GROUPID, LVIF_IMAGE, GroupNo, image, 0, 0, 0);
	}
	else
	{
		// Insert new item
		item=lv_wndGroup.InsertItem(f_Group->GetGroupNO(), GroupNo, image);
		if( item == -1 )
			return FALSE;
	}

    CString Caption;
	Caption=f_Group->GetCaption();
	lv_wndGroup.SetItemText(item,SCR_GROUP_ITEM_CAPTION,Caption);

	CString ProgramID;
	switch( f_Group->GetIVRProgramID() )
	{
	case DEF_LD_GRP_DEFAULT_ET:
		ProgramID.LoadString(IDS_CAP_IVRGP_DEFAULT_CF);
		break;
	case DEF_LD_GRP_ET_FILE:
		ProgramID = f_Group->GetIVRProgramCaption();
		break;
	default:
		ProgramID.Format("%u %s", f_Group->GetIVRProgramID(), (const char *)f_Group->GetIVRProgramCaption());
		break;
	}
	lv_wndGroup.SetItemText(item,SCR_GROUP_ITEM_PROGRAM,ProgramID);

	CString ResourceID;
	ResourceID.Format("#%u", f_Group->GetResourcePrjID());
	lv_wndGroup.SetItemText(item,SCR_GROUP_ITEM_RESOURCE,ResourceID);

	US StartCH, EndCH;
	CString start, end;
	StartCH=f_Group->GetStartCH();
	start.Format("CH%5d", StartCH);
	EndCH=f_Group->GetEndCH();
	end.Format("CH%5d", EndCH);
	lv_wndGroup.SetItemText(item,SCR_GROUP_ITEM_STARTCH,start);
	lv_wndGroup.SetItemText(item,SCR_GROUP_ITEM_ENDCH,end);

	// Sun added 2002-03-18
	CString szCTIStatus;
	if( f_Group->GetNeedCTI() )
		szCTIStatus.LoadString(IDS_CAP_IVRGP_CTI_CONNECT);
	else
		szCTIStatus.LoadString(IDS_CAP_IVRGP_CTI_NOUSE);
	lv_wndGroup.SetItemText(item,SCR_GROUP_ITEM_CTI,szCTIStatus);

	//----------------------------------------------------------
	// Sun added 2006-02-26
	CString lv_sCallCount;
	lv_sCallCount.Format("%u/%u", f_Group->m_DailyCallCount.GetValue(), f_Group->m_YTDCallCount.GetValue());
	lv_wndGroup.SetItemText(item,SCR_GROUP_ITEM_CALLCOUNT,lv_sCallCount);

	// Sun added 2006-02-26
	CTime lv_tCurrTime;
	lv_tCurrTime = CTime::GetCurrentTime();
	lv_wndGroup.SetItemText(item,SCR_GROUP_ITEM_LASTLOADTIME,lv_tCurrTime.Format("%Y-%m-%d %H:%M:%S"));
	//----------------------------------------------------------

	lv_wndGroup.EnsureVisible(item,true);
	lv_wndGroup.Update(TRUE);
		
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(IDM_RESTORE, OnRestore)
	ON_COMMAND(IDM_EXIT, OnExit)
	ON_UPDATE_COMMAND_UI(IDM_RESTORE, OnUpdateRestore)
	ON_WM_CLOSE()
	ON_COMMAND(ID_VIEW_ALL_CH, OnViewAllCh)
	ON_COMMAND(ID_EXE_START, OnExeStart)
	ON_COMMAND(ID_EXE_RESTART, OnExeRestart)
	ON_COMMAND(ID_EXE_RESTART_ALL, OnExeRestartAll)
	ON_COMMAND(ID_EXE_START_ALL, OnExeStartAll)
	ON_COMMAND(ID_EXE_STOP, OnExeStop)
	ON_COMMAND(ID_EXE_STOP_ALL, OnExeStopAll)
	ON_COMMAND(ID_FILE_CONFIG, OnFileConfig)
	ON_COMMAND(ID_FILE_LOAD_DB, OnFileLoadDb)
	ON_COMMAND(ID_FILE_LOAD_FILE, OnFileLoadFile)
	ON_COMMAND(ID_FILE_MRU_FILE1, OnFileMruFile1)
	ON_UPDATE_COMMAND_UI(ID_EXE_RESTART, OnUpdateExeRestart)
	ON_UPDATE_COMMAND_UI(ID_EXE_RESTART_ALL, OnUpdateExeRestartAll)
	ON_UPDATE_COMMAND_UI(ID_EXE_START, OnUpdateExeStart)
	ON_UPDATE_COMMAND_UI(ID_EXE_START_ALL, OnUpdateExeStartAll)
	ON_UPDATE_COMMAND_UI(ID_EXE_STOP, OnUpdateExeStop)
	ON_UPDATE_COMMAND_UI(ID_EXE_STOP_ALL, OnUpdateExeStopAll)
	ON_UPDATE_COMMAND_UI(ID_FILE_CONFIG, OnUpdateFileConfig)
	ON_UPDATE_COMMAND_UI(ID_FILE_LOAD_DB, OnUpdateFileLoadDb)
	ON_UPDATE_COMMAND_UI(ID_FILE_LOAD_FILE, OnUpdateFileLoadFile)
	ON_UPDATE_COMMAND_UI(ID_FILE_LOAD_RESOURCE, OnUpdateFileLoadResource)
	ON_COMMAND(ID_FILE_LOAD_RESOURCE, OnFileLoadResource)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ALL_CH, OnUpdateViewAllCh)
	ON_WM_TIMER()
	ON_COMMAND(IDM_VIEW_LIC_INFO, OnViewLicInfo)
	ON_COMMAND(IDM_TRUNK_SQLLOG, OnTrunkSqllog)
	ON_COMMAND(IDM_FILE_TIMER_LD_CF, OnFileTimerLdCf)
	ON_COMMAND(IDM_FILE_TIMER_LD_RES, OnFileTimerLdRes)
	ON_UPDATE_COMMAND_UI(IDM_FILE_TIMER_LD_CF, OnUpdateFileTimerLdCf)
	ON_UPDATE_COMMAND_UI(IDM_FILE_TIMER_LD_RES, OnUpdateFileTimerLdRes)
	ON_MESSAGE(WM_HOTKEY, OnHotKey)
	ON_COMMAND(ID_VIEW_REPORT, OnViewReport)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SYS_AUTO_SHUTDOWN, OnSysAutoShutDown)
	ON_MESSAGE(WM_SYS_REINIT_DEVICE, OnSysReInitDevices)
	ON_MESSAGE(WM_CLIENTONLINE, OnClientOnline)
	ON_MESSAGE(WM_CLIENTOFFLINE, OnClientOffline)
	ON_MESSAGE(MYWM_NOTIFYICON, OnTrayNotification)
	ON_MESSAGE(WM_MOVEPROGRESSBAR, OnMoveProgressBar)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
	IDS_INDICATOR_SYS,
	IDS_INDICATOR_DB,
	IDS_INDICATOR_COMM,
	IDS_INDICATOR_HW,
	IDS_INDICATOR_PROGRESS,
};

// Sun added 2011-04-16
LRESULT CMainFrame::OnSysAutoShutDown(WPARAM wParam, LPARAM lParam)
{
	Sys_AutoCloseMe((UL)wParam, (UL)lParam);

	return 0;
}

// Sun added 2008-05-19
LRESULT CMainFrame::OnSysReInitDevices(WPARAM wParam, LPARAM lParam)
{
	return Sys_ReInitDevices();
}

/********************************************************************/
/*																	*/
/* Function name : OnClientOnline									*/
/* Description   : Client has registered							*/
/*																	*/
/********************************************************************/
LRESULT CMainFrame::OnClientOnline(WPARAM wParam, LPARAM lParam)
{
	// Get Parameter
	S_WM_WPARAM *lv_pWParam = (S_WM_WPARAM *)wParam;
	
	// LOG Info
	WriteTrace(TraceInfo, "Client(type: %d id:%s at: %s) binds with working thread(id: %X) tag: %d", 
		lv_pWParam->UserType, lv_pWParam->ClientName, lv_pWParam->PeerName, lParam, lv_pWParam->dwTag);

	// Insert New Item
	switch( lv_pWParam->UserType )
	{
	case USER_CTI:					// CTI Server
		MQ_SendMsg_Register((DWORD)lParam);
		break;

	case USER_DATA:					// Data Recorder
		WriteStatusBar(STATUS_BAR_DB, ICON_Ready);
		break;

	case USER_EXT_IVR:				// IVR External Interface
		MQ_SendExtMsg_Register();
	}

	// Recycle Parameter Buffer
	delete lv_pWParam;

	return 0;
}

/********************************************************************/
/*																	*/
/* Function name : OnClientOffline									*/
/* Description   : Client has unbind								*/
/*																	*/
/********************************************************************/
LRESULT CMainFrame::OnClientOffline(WPARAM wParam, LPARAM lParam)
{
	// Get Parameter
	S_WM_WPARAM *lv_pWParam = (S_WM_WPARAM *)wParam;

	// LOG Info
	WriteTrace(TraceDebug, "Client(type: %d id:%s at: %s) unbinds with working thread(id: %X)", 
		lv_pWParam->UserType, lv_pWParam->ClientName, lv_pWParam->PeerName, lParam);

	// Remove Item
	switch( lv_pWParam->UserType )
	{
	case USER_CTI:					// CTI Server
		WriteStatusBar(STATUS_BAR_COMM, ICON_Stop);
		break;

	case USER_DATA:					// Data Recorder
		WriteStatusBar(STATUS_BAR_DB, ICON_Stop);
		break;
	}

	// Recycle Parameter Buffer
	delete lv_pWParam;

	return 0;
}

// Progress Bar Move a Step
LRESULT CMainFrame::OnMoveProgressBar(WPARAM wParam, LPARAM lParam)
{
	ProgressStep();

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction
CMainFrame::CMainFrame() : m_TrayIcon(IDR_POPUP_MENU)
{
	// TODO: add member initialization code here
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	pMainWindow = this;

	m_hIcon = theApp.LoadIcon(IDR_MAINFRAME);
	m_bShowMe = false;
	m_bShowAllChannel = false;
	m_bCompactChannelView = false;
	m_bSystemAutoShutDown = false;

	m_bME_FILE_LOAD_FILE = false;
	m_bME_FILE_LOAD_DB = false;
	m_bME_FILE_LOAD_RESOURCE = false;
	m_bME_FILE_CONFIG = false;
	m_bME_FILE_MRU_FILE1 = false;
	m_bME_FILE_MRU_FILE2 = false;
	m_bME_FILE_MRU_FILE3 = false;
	m_bME_FILE_MRU_FILE4 = false;
	m_bME_EXE_START = false;
	m_bME_EXE_STOP = false;
	m_bME_EXE_RESTART = false;
	m_bME_EXE_START_ALL = false;
	m_bME_EXE_STOP_ALL = false;
	m_bME_EXE_RESTART_ALL = false;
	m_bME_VIEW_ALL_CH = false;
	m_bGCStarted = false;

	m_param_ctiip = _T("");
	m_param_ctiport = 0;
	m_param_drip = _T("");
	m_param_drport = 0;

	// Sun added 2010-05-18 [V7.1.8.3]
	m_param_cti2ip = _T("");
	m_param_cti2port = 0;

	// Sun added 2006-03-05
	m_param_localport = 0;
	m_bLocalSocketOpened = FALSE;
	m_nVerClient = 0;
	m_nVerAdmin = 0;

	// Sun added 2005-03-17
	m_blnEnableEXT = FALSE;
	m_blnEnableTTS = FALSE;
	m_blnEnableASR = FALSE;
	m_param_extip = _T("");
	m_param_extport = 0;
	m_param_asrip = _T("");
	m_param_asrport = 0;

	m_bRunning = FALSE;
	m_bShutdown = FALSE;
	m_bCTIOpened = FALSE;

	m_nTTSBufferCount = 4;
	m_nTTSBufferSize = 64;
	m_nTTSResourceTO = 0;

	m_sckDataRecorder.SetSenderParty(USER_IVR);
	glngInvokeID = 0;

	m_main_timer = 2;
	m_nMainTimerID = 0;

	// Sun added 2013-09-09
	gb_nMW_CreateOnHookThread = 0;
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::Screen_GetWindowPosition(int *x, int *y, int *cx, int *cy)
{
	*x = theApp.GetProfileInt("Desktop", "MainLeft", 0);
	*y = theApp.GetProfileInt("Desktop", "MainTop", 0);
	*cx = theApp.GetProfileInt("Desktop", "MainWidth", 740);
	*cy = theApp.GetProfileInt("Desktop", "MainHeight", 580);
	m_bShowAllChannel = (theApp.GetProfileInt("Desktop", "ShowAllChannel", 0) > 0);
	m_bCompactChannelView = (theApp.GetProfileInt("Desktop", "CompactChannelView", 0) > 0);
}

void CMainFrame::Screen_SetWindowPosition(int x, int y, int cx, int cy)
{
	theApp.WriteProfileInt("Desktop", "MainLeft", x);
	theApp.WriteProfileInt("Desktop", "MainTop", y);
	theApp.WriteProfileInt("Desktop", "MainWidth", cx);
	theApp.WriteProfileInt("Desktop", "MainHeight", cy);
	theApp.WriteProfileInt("Desktop", "ShowAllChannel", m_bShowAllChannel);
	theApp.WriteProfileInt("Desktop", "CompactChannelView", m_bCompactChannelView);
}

BOOL CMainFrame::SB_PositionControl(CWnd * pWnd, int paneID)
{
	try
	{
		if( pWnd == NULL )
			return FALSE;

		CStatusBarCtrl& pMySTC = m_wndStatusBar.GetStatusBarCtrl();
		CRect rect;

		pMySTC.GetRect(paneID, rect);
		if( (m_wndStatusBar.GetPaneStyle(paneID) & SBPS_NOBORDERS) == SBPS_NOBORDERS)
		{
			pWnd->MoveWindow(rect);
		}
		else
		{
			int h,v,s;
			pMySTC.GetBorders( h, v, s ) ;
			rect.top+=v;
			rect.bottom-=v;
			rect.left+=h+s;
			rect.right-=(h+s);
			pWnd->MoveWindow(rect);
		}
	}
	catch(...)
	{
		return FALSE;
	}

	return TRUE;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.CreateEx(this, SBT_TOOLTIPS, SBT_TOOLTIPS | WS_CHILD | WS_VISIBLE | CBRS_BOTTOM) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	else
	{
		// Create Progress Bar
		if(!m_wndProgressBar.Create(WS_VISIBLE, CRect(0, 0, 400, 20), &m_wndStatusBar, IDC_PROGRESSBAR))
		{
			TRACE0("Failed to create progressbar\n");
			return -1;      // fail to create
		}
		m_wndProgressBar.SetRange(0, 80);
		m_wndProgressBar.SetStep(5);
		
		// Add Panel & Tooltip to CJ Status Bar
		CString lv_sCaption;
		CStatusBarCtrl& pMySTC = m_wndStatusBar.GetStatusBarCtrl();
		m_wndStatusBar.SetPaneInfo(STATUS_BAR_Operation, ID_SEPARATOR, SBPS_POPOUT, 280);

		m_wndStatusBar.SetPaneInfo(STATUS_BAR_SYS, IDS_INDICATOR_SYS, SBPS_NORMAL, 12);
		lv_sCaption.LoadString(IDS_INDICATOR_SYS);
		lv_sCaption.TrimLeft();
		pMySTC.SetTipText(STATUS_BAR_SYS, lv_sCaption);

		m_wndStatusBar.SetPaneInfo(STATUS_BAR_DB, IDS_INDICATOR_DB, SBPS_NORMAL, 12);
		lv_sCaption.LoadString(IDS_INDICATOR_DB);
		lv_sCaption.TrimLeft();
		pMySTC.SetTipText(STATUS_BAR_DB, lv_sCaption);

		m_wndStatusBar.SetPaneInfo(STATUS_BAR_COMM, IDS_INDICATOR_COMM, SBPS_NORMAL, 12);
		lv_sCaption.LoadString(IDS_INDICATOR_COMM);
		lv_sCaption.TrimLeft();
		pMySTC.SetTipText(STATUS_BAR_COMM, lv_sCaption);

		m_wndStatusBar.SetPaneInfo(STATUS_BAR_HARDWARE, IDS_INDICATOR_HW, SBPS_NORMAL, 12);
		lv_sCaption.LoadString(IDS_INDICATOR_HW);
		lv_sCaption.TrimLeft();
		pMySTC.SetTipText(STATUS_BAR_HARDWARE, lv_sCaption);

		m_wndStatusBar.SetPaneInfo(STATUS_BAR_PROGRESS, IDS_INDICATOR_PROGRESS, SBPS_STRETCH, 400);
		lv_sCaption.LoadString(IDS_INDICATOR_PROGRESS);
		lv_sCaption.TrimLeft();
		pMySTC.SetTipText(STATUS_BAR_PROGRESS, lv_sCaption);

		if( m_wndStSYS.Create(NULL, SS_ICON | WS_VISIBLE, CRect(0, 0, 20, 20), &m_wndStatusBar, IDC_STATICSYS))
		{
			if(!SB_PositionControl(&m_wndStSYS, STATUS_BAR_SYS))
			{
				TRACE0("Failed to add static 0x01\n");
			}
		}

		if( m_wndStDB.Create(NULL, SS_ICON | WS_VISIBLE, CRect(0, 0, 20, 20), &m_wndStatusBar, IDC_STATICDB))
		{
			if(!SB_PositionControl(&m_wndStDB, STATUS_BAR_DB))
			{
				TRACE0("Failed to add static 0x02\n");
			}
		}

		if( m_wndStCOM.Create(NULL, SS_ICON | WS_VISIBLE, CRect(0, 0, 20, 20), &m_wndStatusBar, IDC_STATICCOM))
		{
			if(!SB_PositionControl(&m_wndStCOM, STATUS_BAR_COMM))
			{
				TRACE0("Failed to add static 0x03\n");
			}
		}

		if( m_wndStHW.Create(NULL, SS_ICON | WS_VISIBLE, CRect(0, 0, 20, 20), &m_wndStatusBar, IDC_STATICHW))
		{
			if(!SB_PositionControl(&m_wndStHW, STATUS_BAR_HARDWARE))
			{
				TRACE0("Failed to add static 0x04\n");
			}
		}

		if(!SB_PositionControl(&m_wndProgressBar, STATUS_BAR_PROGRESS))
		{
			TRACE0("Failed to add progress bar\n");
		}
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	// Add Tray Icon
	m_TrayIcon.SetNotificationWnd(this, MYWM_NOTIFYICON);
	m_TrayIcon.SetIcon(m_hIcon, MYTIP);
	
	// CG: The following line was added by the Splash Screen component.
	CSplashWnd::ShowSplashScreen(this);

	SB_PositionControl(&m_wndStSYS, STATUS_BAR_SYS);
	SB_PositionControl(&m_wndStDB, STATUS_BAR_DB);
	SB_PositionControl(&m_wndStCOM, STATUS_BAR_COMM);
	SB_PositionControl(&m_wndStHW, STATUS_BAR_HARDWARE);
	SB_PositionControl(&m_wndProgressBar, STATUS_BAR_PROGRESS);

	// Set Main Menu to Init State
	SetMainMenuStatus();

	// Window's position
	/// Sun modified 2002-07-16
	int lv_nLeft, lv_nTop, lv_nWidth, lv_nHeight;
	Screen_GetWindowPosition(&lv_nLeft, &lv_nTop, &lv_nWidth, &lv_nHeight);
	if( lv_nLeft < 0 )
		lv_nLeft = 0;
	if( lv_nTop < 0 )
		lv_nTop = 0;
	SetWindowPos (&wndTop, lv_nLeft, lv_nTop, lv_nWidth, lv_nHeight, SWP_SHOWWINDOW);
	ModifyStyle (0, WS_MAXIMIZEBOX, 0);

	// Install Hot Key
	/// Hot Keys
	CString lv_sTemp;
	lv_sTemp = theApp.GetProfileString("HotKey", "ShowWindow", _T("V"));
	if( lv_sTemp.GetLength() == 0 )
		lv_sTemp = _T("V");
	else
		lv_sTemp.MakeUpper();
	theApp.WriteProfileString("HotKey", "ShowWindow", lv_sTemp);
	HotKey_Install(HOTKEY_SHOW_WINDOW, lv_sTemp.GetAt(0));

	// Sun added 2010-05-18 [V7.1.8.3]
	m_sckPCServer.SetPCSIndex(0);
	m_sckPCServer2.SetPCSIndex(1);

	return 0;
}

LRESULT CMainFrame::OnTrayNotification(WPARAM wParam, LPARAM lParam)
{
 //   return m_TrayIcon.OnTrayNotification(wParam, lParam);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	// create splitter window
	if (!m_wndSplitter.CreateStatic(this, 2, 1))
		return FALSE;

	int lv_GroupViewHeight = theApp.GetProfileInt("Desktop", "GroupViewHeight", 100);

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CIVRGroupView), CSize(100, lv_GroupViewHeight), pContext) ||
		!m_wndSplitter.CreateView(1, 0, pContext->m_pNewViewClass, CSize(100, 100), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}
	
	return TRUE;
}

// Refresh Main Menu Status according to Current Group
void CMainFrame::RefreshMainMenuOnGroup(CIVRGroup *f_pGroup)
{
	if( f_pGroup != NULL )
	{
		if( f_pGroup->GetEnabled() )
		{
			if( f_pGroup->GetStarted() )
				SetMainMenuStatus(MAIN_FRM_STATUS_START);
			else
				SetMainMenuStatus(MAIN_FRM_STATUS_STOP);
			return;
		}
	}
	pMainWindow->SetMainMenuStatus(MAIN_FRM_STATUS_NOCUR);
}

BOOL CMainFrame::GetCTIOpened()
{
	return m_bCTIOpened;
}

void CMainFrame::SetCTIOpened(BOOL blnValue, BOOL blnForce)
{
	WriteTrace(TraceInfo, "SysInfo - CTI Connection status changed from %d to %d, Force set = %d", m_bCTIOpened, blnValue, blnForce);

	if( m_bCTIOpened != blnValue || blnForce )
	{
		m_bCTIOpened = blnValue;
		if( m_bCTIOpened )
		{
			// Initialize CTI devices group by group
			for( int lv_loop = 0; lv_loop < theApp.m_nIVRGroups; lv_loop++ )
			{
				if( PIVRGroup[lv_loop] != NULL )
					PIVRGroup[lv_loop]->InitializeChannels();
			}
		}
	}
}

// Modify Status Bar
void CMainFrame::WriteStatusBar(int nPanel, int nStatus)
{
	HICON pStatus;
	BOOL lv_blnCTIOK = FALSE;

	try
	{
		switch( nStatus )
		{
		case ICON_Error:
			pStatus = theApp.LoadIcon(IDI_SYSERROR);
			break;
		case ICON_Stop:
			pStatus = theApp.LoadIcon(IDI_SYSSTOP);
			break;
		case ICON_Ready:
			pStatus = theApp.LoadIcon(IDI_SYSREADY);
			break;
		default:
			return;
		}

		switch( nPanel )
		{
		case STATUS_BAR_SYS:
			m_wndStSYS.SetIcon(pStatus);
			SB_PositionControl(&m_wndStSYS, STATUS_BAR_SYS);
			if( nStatus != ICON_Ready )
				SetMainMenuStatus(MAIN_FRM_STATUS_INIT);
			break;
		case STATUS_BAR_DB:
			m_wndStDB.SetIcon(pStatus);
			SB_PositionControl(&m_wndStDB, STATUS_BAR_DB);
			break;
		case STATUS_BAR_COMM:

			// Sun added 2010-05-18 [V7.1.8.3]
			lv_blnCTIOK = (nStatus == ICON_Ready);
			if( m_param_cti2ip.GetLength() > 0 && m_param_cti2port > 0 )
			{
				if( nStatus == ICON_Ready )
				{
					if( m_sckPCServer.GetPCSStatus() == PCS_PS_Active )
					{
						if( m_sckPCServer2.GetPCSStatus() == PCS_PS_Standby )
							pStatus = theApp.LoadIcon(IDI_SYSBOTHREADY1);
						else
							pStatus = theApp.LoadIcon(IDI_SYSHALFREADY1);
					}
					else if( m_sckPCServer.GetPCSStatus() == PCS_PS_Standby )
					{
						if( m_sckPCServer2.GetPCSStatus() == PCS_PS_Active )
							pStatus = theApp.LoadIcon(IDI_SYSBOTHREADY2);
						else
							pStatus = theApp.LoadIcon(IDI_SYSHALFREADY3);
					}
					else
					{
						if( m_sckPCServer2.GetPCSStatus() == PCS_PS_Active )
							pStatus = theApp.LoadIcon(IDI_SYSHALFREADY2);
						else if( m_sckPCServer2.GetPCSStatus() == PCS_PS_Standby )
							pStatus = theApp.LoadIcon(IDI_SYSHALFREADY4);
					}
				}
				else if( nStatus == ICON_Stop )
				{
					if( m_sckPCServer.GetPCSStatus() == PCS_PS_Active )
					{
						lv_blnCTIOK = TRUE;
						pStatus = theApp.LoadIcon(IDI_SYSHALFREADY1);
					}
					else if( m_sckPCServer.GetPCSStatus() == PCS_PS_Standby )
					{
						lv_blnCTIOK = TRUE;
						pStatus = theApp.LoadIcon(IDI_SYSHALFREADY3);
					}
					else if( m_sckPCServer2.GetPCSStatus() == PCS_PS_Active )
					{
						lv_blnCTIOK = TRUE;
						pStatus = theApp.LoadIcon(IDI_SYSHALFREADY2);
					}
					else if( m_sckPCServer2.GetPCSStatus() == PCS_PS_Standby )
					{
						lv_blnCTIOK = TRUE;
						pStatus = theApp.LoadIcon(IDI_SYSHALFREADY4);
					}
				}

				CStatusBarCtrl& pMySTC = m_wndStatusBar.GetStatusBarCtrl();
				CString lv_sCaption;
				lv_sCaption.Format("PCS0:%s PCS1:%s", GetPCSStatusName(m_sckPCServer.GetPCSStatus()), GetPCSStatusName(m_sckPCServer2.GetPCSStatus()));
				pMySTC.SetTipText(STATUS_BAR_COMM, lv_sCaption);

				// Sun added 2012-10-14
				WriteTrace(TraceWarn, "CTI Connection status report - %s", lv_sCaption);

				// Sun added 2013-11-19
				/// 仅关闭标记，打开标记在其他函数中完成
				if( !lv_blnCTIOK )
					SetCTIOpened(lv_blnCTIOK);
			}
			// Sun added 2013-11-19
			else
			{
				// Sun added 2013-11-19
				SetCTIOpened(lv_blnCTIOK);
			}

			m_wndStCOM.SetIcon(pStatus);
			SB_PositionControl(&m_wndStCOM, STATUS_BAR_COMM);

			// Sun modified 2013-01-14
			/// From
			//SetCTIOpened(lv_blnCTIOK);
			/// To - 仅关闭标记，打开标记在其他函数中完成
			// Sun removed  2013-11-19
			//if( !lv_blnCTIOK )
			//	SetCTIOpened(lv_blnCTIOK);

			break;
		case STATUS_BAR_HARDWARE:
			m_wndStHW.SetIcon(pStatus);
			SB_PositionControl(&m_wndStHW, STATUS_BAR_HARDWARE);
			break;
		}
		m_wndStatusBar.RedrawWindow();
	}
	catch(...)
	{
	}
}

// Modify Status Bar
void CMainFrame::WriteStatusBar(char *sMsg)
{
	m_wndStatusBar.SetPaneText(STATUS_BAR_Operation, sMsg);
}

// Progress Bar Move a Step
void CMainFrame::ProgressStep()
{
	m_wndProgressBar.StepIt();
}

// Change Enable Status of Main Menu:
void CMainFrame::SetMainMenuStatus(int nStatus)
{
	switch( nStatus )
	{
	case MAIN_FRM_STATUS_NOCUR:
		m_bME_FILE_LOAD_FILE = false;
		m_bME_FILE_LOAD_DB = false;
		m_bME_FILE_LOAD_RESOURCE = false;
		m_bME_FILE_CONFIG = true;
		m_bME_FILE_MRU_FILE1 = false;
		m_bME_FILE_MRU_FILE2 = false;
		m_bME_FILE_MRU_FILE3 = false;
		m_bME_FILE_MRU_FILE4 = false;
		m_bME_EXE_START = false;
		m_bME_EXE_STOP = false;
		m_bME_EXE_RESTART = false;
		m_bME_EXE_START_ALL = true;
		m_bME_EXE_STOP_ALL = true;
		m_bME_EXE_RESTART_ALL = true;
		m_bME_VIEW_ALL_CH = true;
		break;

	case MAIN_FRM_STATUS_INIT:
	case MAIN_FRM_STATUS_LOAD:
	case MAIN_FRM_STATUS_RUN:
		m_bME_FILE_LOAD_FILE = false;
		m_bME_FILE_LOAD_DB = false;
		m_bME_FILE_LOAD_RESOURCE = false;
		m_bME_FILE_CONFIG = false;
		m_bME_FILE_MRU_FILE1 = false;
		m_bME_FILE_MRU_FILE2 = false;
		m_bME_FILE_MRU_FILE3 = false;
		m_bME_FILE_MRU_FILE4 = false;
		m_bME_EXE_START = false;
		m_bME_EXE_STOP = false;
		m_bME_EXE_RESTART = false;
		m_bME_EXE_START_ALL = false;
		m_bME_EXE_STOP_ALL = false;
		m_bME_EXE_RESTART_ALL = false;
		m_bME_VIEW_ALL_CH = true;
		break;

	case MAIN_FRM_STATUS_STOP:
		m_bME_FILE_LOAD_FILE = true;
		m_bME_FILE_LOAD_DB = true;
		m_bME_FILE_LOAD_RESOURCE = true;
		m_bME_FILE_CONFIG = true;
		m_bME_FILE_MRU_FILE1 = false;
		m_bME_FILE_MRU_FILE2 = false;
		m_bME_FILE_MRU_FILE3 = false;
		m_bME_FILE_MRU_FILE4 = false;
		m_bME_EXE_START = true;
		m_bME_EXE_STOP = false;
		m_bME_EXE_RESTART = false;
		m_bME_EXE_START_ALL = true;
		m_bME_EXE_STOP_ALL = true;
		m_bME_EXE_RESTART_ALL = true;
		m_bME_VIEW_ALL_CH = true;
		break;

	case MAIN_FRM_STATUS_START:
		m_bME_FILE_LOAD_FILE = true;
		m_bME_FILE_LOAD_DB = true;
		m_bME_FILE_LOAD_RESOURCE = true;
		m_bME_FILE_CONFIG = true;
		m_bME_FILE_MRU_FILE1 = false;
		m_bME_FILE_MRU_FILE2 = false;
		m_bME_FILE_MRU_FILE3 = false;
		m_bME_FILE_MRU_FILE4 = false;
		m_bME_EXE_START = false;
		m_bME_EXE_STOP = true;
		m_bME_EXE_RESTART = true;
		m_bME_EXE_START_ALL = true;
		m_bME_EXE_STOP_ALL = true;
		m_bME_EXE_RESTART_ALL = true;
		m_bME_VIEW_ALL_CH = true;
		break;
	}
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(nType == 1)
	{
		ShowWindow(SW_HIDE);
		m_bShowMe = true;
	}
}

BOOL CMainFrame::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	Sleep(1000);
	return CFrameWnd::DestroyWindow();
}

void CMainFrame::OnRestore() 
{
	// TODO: Add your command handler code here
	ShowWindow(SW_SHOWNORMAL);
	SetForegroundWindow();
	m_bShowMe = false;
}

void CMainFrame::OnExit() 
{
	// TODO: Add your command handler code here
	OnClose();
}

// Start Current IVR Group
void CMainFrame::OnExeStart() 
{
	// TODO: Add your command handler code here
	CIVRGroup *lv_pGroup;
	int f_GroupNo = pGroupView->GetShowGroupNo();

	// Is No. right ?
	if( f_GroupNo >= 0 && f_GroupNo < theApp.m_nIVRGroups )
	{
		// Is object available ?
		if( (lv_pGroup = PIVRGroup[f_GroupNo]) != NULL )
		{
			// Load Waiting Cursor
			HCURSOR hOldCur = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));

			// Already Started ?
			if( !lv_pGroup->GetStarted() )
			{
				// Set Main Menu to Running State
				SetMainMenuStatus(MAIN_FRM_STATUS_RUN);

				// Start Group
				lv_pGroup->StartGroup();
				if( GetCTIOpened() )
					lv_pGroup->InitializeChannels();
				Sleep(1000);

				// Refresh Main Menu Status according to Current Group
				RefreshMainMenuOnGroup(lv_pGroup);
			}

			// Resume Cursor
			::SetCursor(hOldCur);

			// Exit Function
			return;
		}
	}

	// Set Main Menu to No Current Group State
	SetMainMenuStatus(MAIN_FRM_STATUS_NOCUR);
}

void CMainFrame::OnExeRestart() 
{
	// TODO: Add your command handler code here
	if(AfxMessageBox(IDS_MSG_Q_RESTARTGROUP, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION | MB_DEFBUTTON1) == IDNO)
	{
		return;
	}

	CIVRGroup *lv_pGroup;
	int f_GroupNo = pGroupView->GetShowGroupNo();

	// Is No. right ?
	if( f_GroupNo >= 0 && f_GroupNo < theApp.m_nIVRGroups )
	{
		// Is object available ?
		if( (lv_pGroup = PIVRGroup[f_GroupNo]) != NULL )
		{
			// Load Waiting Cursor
			HCURSOR hOldCur = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));

			// Set Main Menu to Running State
			SetMainMenuStatus(MAIN_FRM_STATUS_RUN);

			// Already Started ?
			if( lv_pGroup->GetStarted() )
			{
				// Stop Group
				lv_pGroup->StopGroup();
				Sleep(20);
			}
				
			// Start Group
			lv_pGroup->StartGroup();
			if( GetCTIOpened() )
				lv_pGroup->InitializeChannels();
			Sleep(1000);

			// Refresh Main Menu Status according to Current Group
			RefreshMainMenuOnGroup(lv_pGroup);

			// Resume Cursor
			::SetCursor(hOldCur);

			// Exit Function
			return;
		}
	}

	// Set Main Menu to No Current Group State
	SetMainMenuStatus(MAIN_FRM_STATUS_NOCUR);
}

void CMainFrame::OnExeRestartAll() 
{
	// TODO: Add your command handler code here
	if(AfxMessageBox(IDS_MSG_Q_RESTARTALL, MB_YESNO | MB_APPLMODAL | MB_ICONEXCLAMATION | MB_DEFBUTTON1) == IDNO)
	{
		return;
	}

	CIVRGroup *lv_pGroup;
	int f_GroupNo;

	// Load Waiting Cursor
	HCURSOR hOldCur = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));

	// Set Main Menu to Running State
	SetMainMenuStatus(MAIN_FRM_STATUS_RUN);

	// All Groups
	for( f_GroupNo = 0; f_GroupNo < theApp.m_nIVRGroups; f_GroupNo++ )
	{
		// Is object available ?
		if( (lv_pGroup = PIVRGroup[f_GroupNo]) != NULL )
		{
			// Already Started ?
			if( lv_pGroup->GetStarted() )
			{
				// Stop Group
				lv_pGroup->StopGroup();
				Sleep(20);
			}
				
			// Start Group
			lv_pGroup->StartGroup();
			if( GetCTIOpened() )
				lv_pGroup->InitializeChannels();
			Sleep(20);
		}
	}
	Sleep(1000);

	// Resume Cursor
	::SetCursor(hOldCur);

	// Refresh Main Menu Status according to Current Group
	f_GroupNo = pGroupView->GetShowGroupNo();
	// Is No. right ?
	if( f_GroupNo >= 0 && f_GroupNo < theApp.m_nIVRGroups )
	{
		// Is object available ?
		if( (lv_pGroup = PIVRGroup[f_GroupNo]) != NULL )
		{
			RefreshMainMenuOnGroup(lv_pGroup);
			return;
		}
	}
	
	// Set Main Menu to No Current Group State
	SetMainMenuStatus(MAIN_FRM_STATUS_NOCUR);
}

void CMainFrame::OnExeStartAll() 
{
	// TODO: Add your command handler code here
	if(AfxMessageBox(IDS_MSG_Q_STARTALL, MB_YESNO | MB_APPLMODAL | MB_ICONEXCLAMATION | MB_DEFBUTTON1) == IDNO)
	{
		return;
	}

	CIVRGroup *lv_pGroup;
	int f_GroupNo;

	// Load Waiting Cursor
	HCURSOR hOldCur = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));

	// Set Main Menu to Running State
	SetMainMenuStatus(MAIN_FRM_STATUS_RUN);

	// All Groups
	for( f_GroupNo = 0; f_GroupNo < theApp.m_nIVRGroups; f_GroupNo++ )
	{
		// Is object available ?
		if( (lv_pGroup = PIVRGroup[f_GroupNo]) != NULL )
		{
			// Already Started ?
			if( !lv_pGroup->GetStarted() )
			{
				// Start Group
				lv_pGroup->StartGroup();
				if( GetCTIOpened() )
					lv_pGroup->InitializeChannels();
				Sleep(20);
			}
		}
	}
	Sleep(1000);

	// Resume Cursor
	::SetCursor(hOldCur);

	// Refresh Main Menu Status according to Current Group
	f_GroupNo = pGroupView->GetShowGroupNo();
	// Is No. right ?
	if( f_GroupNo >= 0 && f_GroupNo < theApp.m_nIVRGroups )
	{
		// Is object available ?
		if( (lv_pGroup = PIVRGroup[f_GroupNo]) != NULL )
		{
			RefreshMainMenuOnGroup(lv_pGroup);
			return;
		}
	}
	
	// Set Main Menu to No Current Group State
	SetMainMenuStatus(MAIN_FRM_STATUS_NOCUR);
}

// Stop Current IVR Group
void CMainFrame::OnExeStop() 
{
	// TODO: Add your command handler code here
	if(AfxMessageBox(IDS_MSG_Q_STOPGROUP, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION | MB_DEFBUTTON1) == IDNO)
	{
		return;
	}

	CIVRGroup *lv_pGroup;
	int f_GroupNo = pGroupView->GetShowGroupNo();

	// Is No. right ?
	if( f_GroupNo >= 0 && f_GroupNo < theApp.m_nIVRGroups )
	{
		// Is object available ?
		if( (lv_pGroup = PIVRGroup[f_GroupNo]) != NULL )
		{
			// Load Waiting Cursor
			HCURSOR hOldCur = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));

			// Already Stoped ?
			if( lv_pGroup->GetStarted() )
			{
				// Set Main Menu to Running State
				SetMainMenuStatus(MAIN_FRM_STATUS_RUN);

				// Stop Group
				lv_pGroup->StopGroup();
				Sleep(1000);

				// Refresh Main Menu Status according to Current Group
				RefreshMainMenuOnGroup(lv_pGroup);
			}

			// Resume Cursor
			::SetCursor(hOldCur);

			// Exit Function
			return;
		}
	}

	// Set Main Menu to No Current Group State
	SetMainMenuStatus(MAIN_FRM_STATUS_NOCUR);
}

void CMainFrame::OnExeStopAll() 
{
	// TODO: Add your command handler code here
	if(AfxMessageBox(IDS_MSG_Q_STOPALL, MB_YESNO | MB_APPLMODAL | MB_ICONEXCLAMATION | MB_DEFBUTTON1) == IDNO)
	{
		return;
	}

	CIVRGroup *lv_pGroup;
	int f_GroupNo;

	// Load Waiting Cursor
	HCURSOR hOldCur = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));

	// Set Main Menu to Running State
	SetMainMenuStatus(MAIN_FRM_STATUS_RUN);

	// All Groups
	for( f_GroupNo = 0; f_GroupNo < theApp.m_nIVRGroups; f_GroupNo++ )
	{
		// Is object available ?
		if( (lv_pGroup = PIVRGroup[f_GroupNo]) != NULL )
		{
			// Already Stoped ?
			if( lv_pGroup->GetStarted() )
			{
				// Stop Group
				lv_pGroup->StopGroup();
				Sleep(20);
			}
		}
	}
	Sleep(1000);

	// Resume Cursor
	::SetCursor(hOldCur);

	// Refresh Main Menu Status according to Current Group
	f_GroupNo = pGroupView->GetShowGroupNo();
	// Is No. right ?
	if( f_GroupNo >= 0 && f_GroupNo < theApp.m_nIVRGroups )
	{
		// Is object available ?
		if( (lv_pGroup = PIVRGroup[f_GroupNo]) != NULL )
		{
			RefreshMainMenuOnGroup(lv_pGroup);
			return;
		}
	}
	
	// Set Main Menu to No Current Group State
	SetMainMenuStatus(MAIN_FRM_STATUS_NOCUR);
}

// Show Configuration Form
void CMainFrame::OnFileConfig() 
{
	// TODO: Add your command handler code here
//#ifndef _DEBUG
	CDlgGetPassword lv_dlg;
	if( lv_dlg.DoModal() != IDOK )
		return;
//#endif

	CConfigBox dlgConfig;
	dlgConfig.DoModal();

//	AfxMessageBox(IDS_RESTART_POWERVOICE, MB_ICONEXCLAMATION | MB_OK);
}

// Load call flow from DB table
void CMainFrame::OnFileLoadDb() 
{
	// TODO: Add your command handler code here
	BOOL lv_retval;
	int f_GroupNo;
	char	lv_buffer[10];
	char	lv_nm_prgid[50];
	CDlgSeleCallFlowTable lv_dlg;
	CIVRGroup *lv_pGroup;
	
	f_GroupNo = pGroupView->GetShowGroupNo();
	// Is No. right ?
	if( f_GroupNo >= 0 && f_GroupNo < theApp.m_nIVRGroups )
	{
		// Is object available ?
		if( (lv_pGroup = PIVRGroup[f_GroupNo]) != NULL )
		{
			lv_dlg.m_nselectitem = 0;
			lv_dlg.m_nselectitemdata = PGroupInfo[f_GroupNo].IVRProgramID;
			if( lv_dlg.DoModal() == IDOK )
			{
				if( lv_dlg.m_nselectitem >= 0 )
				{
					// Load Waiting Cursor
					HCURSOR hOldCur = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));

					lv_pGroup->SetIVRProgramID((UC)lv_dlg.m_nselectitemdata);
					
					// Reload Resource Project
					lv_retval = lv_pGroup->Init_SelectET();
					
					// Resume Cursor
					::SetCursor(hOldCur);

					// Succeed?
					if( lv_retval )
					{
						// Save Setting
						PGroupInfo[f_GroupNo].IVRProgramID = lv_pGroup->GetIVRProgramID();
						_itoa(f_GroupNo + 1, lv_buffer, 10);
						sprintf(lv_nm_prgid, "GROUP%s_IVRProgramID", lv_buffer);
						theApp.WriteProfileInt("GROUPINFO", lv_nm_prgid, PGroupInfo[f_GroupNo].IVRProgramID);
						
						if( lv_pGroup->GetIVRProgramID() != 0 )
						{
							AfxMessageBox(IDS_MSG_M_LOADCF_OK, MB_ICONINFORMATION | MB_OK);
						}
						else
						{
							// Default Flow
							AfxMessageBox(IDS_MSG_M_LOADCF_FAIL, MB_ICONEXCLAMATION | MB_OK);
						}
					}
					else
					{
						// Restore Data
						lv_pGroup->SetIVRProgramID(PGroupInfo[f_GroupNo].IVRProgramID);
						AfxMessageBox(IDS_MSG_M_LOADCF_ERROR, MB_ICONEXCLAMATION | MB_OK);
					}
				}
			}
		}
	}
}

// Sun updated 2005-06-08
/// 用自己的对话框代替Windows的文件对话框
void CMainFrame::OnFileLoadFile() 
{
	// TODO: Add your command handler code here
	BOOL lv_retval;
	CString lv_filename;
	int f_GroupNo;
	char	lv_buffer[10];
	char	lv_nm_prgid[50];
	char	lv_nm_prgfilename[50];
	CIVRGroup *lv_pGroup;

	/*
	CFileDialog dlgOpen (TRUE,  "*.pvc",  NULL, OFN_HIDEREADONLY, "PowerVoice Callflow Files (*.pvc)|*.pvc|");

	if ( IDOK == dlgOpen.DoModal() ) 
	{ 
		lv_filename = dlgOpen.GetPathName();

		f_GroupNo = pGroupView->GetShowGroupNo();
		// Is No. right ?
		if( f_GroupNo >= 0 && f_GroupNo < theApp.m_nIVRGroups )
		{
			// Is object available ?
			if( (lv_pGroup = PIVRGroup[f_GroupNo]) != NULL )
			{
				// Load Waiting Cursor
				HCURSOR hOldCur = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));

				// Load from File
				lv_pGroup->SetIVRProgramID(DEF_LD_GRP_ET_FILE);
				lv_pGroup->SetIVRFileName(lv_filename);
				
				// Reload Call Flow
				lv_retval = lv_pGroup->Init_SelectET();
				
				// Resume Cursor
				::SetCursor(hOldCur);

				// Succeed?
				if( lv_retval )
				{
					// Save Setting
					PGroupInfo[f_GroupNo].IVRProgramID = lv_pGroup->GetIVRProgramID();
					_itoa(f_GroupNo + 1, lv_buffer, 10);
					sprintf(lv_nm_prgid, "GROUP%s_IVRProgramID", lv_buffer);
					theApp.WriteProfileInt("GROUPINFO", lv_nm_prgid, PGroupInfo[f_GroupNo].IVRProgramID);
					
					if( PGroupInfo[f_GroupNo].IVRProgramID == DEF_LD_GRP_ET_FILE )
					{
						sprintf(lv_nm_prgfilename, "GROUP%s_IVRProgramFN", lv_buffer);
						theApp.WriteProfileString("GROUPINFO",lv_nm_prgfilename,lv_filename);
					}
					
					if( PGroupInfo[f_GroupNo].IVRProgramID != 0 )
					{
						AfxMessageBox(IDS_MSG_M_LOADCF_OK, MB_ICONINFORMATION | MB_OK);
					}
					else
					{
						// Default Flow
						AfxMessageBox(IDS_MSG_M_LOADCF_FAIL, MB_ICONEXCLAMATION | MB_OK);
					}
				}
				else
				{
					// Restore Data
					lv_pGroup->SetIVRProgramID(PGroupInfo[f_GroupNo].IVRProgramID);
					AfxMessageBox(IDS_MSG_M_LOADCF_ERROR, MB_ICONEXCLAMATION | MB_OK);
				}
			}
		}
	}*/

	CBrowsePath lv_dlg(NULL, FALSE);
	
	f_GroupNo = pGroupView->GetShowGroupNo();
	// Is No. right ?
	if( f_GroupNo >= 0 && f_GroupNo < theApp.m_nIVRGroups )
	{
		// Is object available ?
		if( (lv_pGroup = PIVRGroup[f_GroupNo]) != NULL )
		{
			// Get File Path
			lv_dlg.m_szExt = _T(".pvc");
			lv_dlg.m_szCurFile = lv_pGroup->GetIVRFileName();

			if( IDOK == lv_dlg.DoModal() ) 
			{ 
				lv_filename = lv_dlg.m_szCurFile;

				// Load Waiting Cursor
				HCURSOR hOldCur = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));

				// Load from File
				lv_pGroup->SetIVRProgramID(DEF_LD_GRP_ET_FILE);
				lv_pGroup->SetIVRFileName(lv_filename);
				
				// Reload Call Flow
				lv_retval = lv_pGroup->Init_SelectET();
				
				// Resume Cursor
				::SetCursor(hOldCur);

				// Succeed?
				if( lv_retval )
				{
					// Save Setting
					PGroupInfo[f_GroupNo].IVRProgramID = lv_pGroup->GetIVRProgramID();
					_itoa(f_GroupNo + 1, lv_buffer, 10);
					sprintf(lv_nm_prgid, "GROUP%s_IVRProgramID", lv_buffer);
					theApp.WriteProfileInt("GROUPINFO", lv_nm_prgid, PGroupInfo[f_GroupNo].IVRProgramID);
					
					if( PGroupInfo[f_GroupNo].IVRProgramID == DEF_LD_GRP_ET_FILE )
					{
						sprintf(lv_nm_prgfilename, "GROUP%s_IVRProgramFN", lv_buffer);
						theApp.WriteProfileString("GROUPINFO",lv_nm_prgfilename,lv_filename);
					}
					
					if( PGroupInfo[f_GroupNo].IVRProgramID != 0 )
					{
						AfxMessageBox(IDS_MSG_M_LOADCF_OK, MB_ICONINFORMATION | MB_OK);
					}
					else
					{
						// Default Flow
						AfxMessageBox(IDS_MSG_M_LOADCF_FAIL, MB_ICONEXCLAMATION | MB_OK);
					}
				}
				else
				{
					// Restore Data
					lv_pGroup->SetIVRProgramID(PGroupInfo[f_GroupNo].IVRProgramID);
					AfxMessageBox(IDS_MSG_M_LOADCF_ERROR, MB_ICONEXCLAMATION | MB_OK);
				}
			}
		}
	}
}

void CMainFrame::OnFileMruFile1() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnUpdateRestore(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bShowMe);	
}

LRESULT CMainFrame::OnHotKey(WPARAM wParam, LPARAM lParam)
{
	switch( wParam )
	{
	case HOTKEY_SHOW_WINDOW:
		OnRestore();
		break;
	}

	return 0;
}

BOOL CMainFrame::HotKey_Install(UINT nIndex, TCHAR cKey)
{
	ASSERT((nIndex >= HOTKEY_BASE) && (nIndex <= HOTKEY_LAST));

	if( (cKey >= 'A' && cKey <= 'Z') || (cKey >= '0' && cKey <= '9') )
		if( RegisterHotKey(this->m_hWnd, nIndex, MOD_ALT | MOD_CONTROL, cKey) )
			return TRUE;

	return FALSE;
}

void CMainFrame::HotKey_Remove(UINT nIndex)
{
	ASSERT((nIndex >= HOTKEY_BASE) && (nIndex <= HOTKEY_LAST));

	UnregisterHotKey(this->m_hWnd, nIndex);
}

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
//#ifndef _DEBUG
	if( !m_bSystemAutoShutDown )
	{
		if(AfxMessageBox(IDS_MSG_Q_EXITAPP, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION | MB_DEFBUTTON2) == IDNO)
		{
			return;
		}
	}

//#endif

	// Sun added 2011-06-27
	WriteTrace(TraceWarn, "SysWarn - PowerVoice got OnClose Message, AutoShutDown = %d", m_bSystemAutoShutDown);

	// Stop System
	m_bShutdown = TRUE;
	m_sckDataRecorder.SetSenderParty(0);
	if( !Sys_StopSystem() )
	{
		m_bShutdown = FALSE;
		m_sckDataRecorder.SetSenderParty(USER_IVR);
		return;
	}

	// Sun added 2002-07-16
	CRect lv_Rect;
	GetWindowRect(lv_Rect);
	Screen_SetWindowPosition(lv_Rect.left, lv_Rect.top, lv_Rect.Width(), lv_Rect.Height());
	pGroupView->GetWindowRect(lv_Rect);
	theApp.WriteProfileInt("Desktop", "GroupViewHeight", lv_Rect.Height());

	// Remove HotKey
	HotKey_Remove(HOTKEY_SHOW_WINDOW);

	CFrameWnd::OnClose();
}

//////////////////////////////////////////////////////////////////////
// Information Process
//////////////////////////////////////////////////////////////////////
//
void CMainFrame::Information_Init_SetSystemTones(CString f_Tone, int f_Index)
{
	TCHAR	seps[]   = " {},\t\n";
	TCHAR*	lpTone;
	TCHAR*	token;

	lpTone = f_Tone.GetBuffer(f_Tone.GetLength());

	token = strtok(lpTone, seps);
	if(token != NULL)
		gb_Tone[f_Index].freq1.freq = atoi(token);
	else
		return;

	token = strtok( NULL, seps );
	if(token != NULL)
		gb_Tone[f_Index].freq1.deviation = atoi(token);	
	else
		return;

	token = strtok( NULL, seps );
	if(token != NULL)
		gb_Tone[f_Index].freq2.freq = atoi(token);
	else
		return;

	token = strtok( NULL, seps );
	if(token != NULL)
		gb_Tone[f_Index].freq2.deviation = atoi(token);
	else
		return;

	token = strtok( NULL, seps );
	if(token != NULL)
		gb_Tone[f_Index].on.time = atoi(token);
	else
		return;

	token = strtok( NULL, seps );
	if(token != NULL)
		gb_Tone[f_Index].on.deviation = atoi(token);			
	else
		return;

	token = strtok( NULL, seps );
	if(token != NULL)
		gb_Tone[f_Index].off.time = atoi(token);
	else
		return;

	token = strtok( NULL, seps );
	if(token != NULL)
		gb_Tone[f_Index].off.deviation = atoi(token);
	else
		return;

	token = strtok( NULL, seps );
	if(token != NULL)
		gb_Tone[f_Index].repcnt = atoi(token);
	else
		return;

	while(token != NULL){
		token = strtok( NULL, seps );
	}
}

int CMainFrame::Information_LoadIniSystemParamter()
{
	CString Tones[MAX_CHANNEL_TONES];
	CString lv_sTemp;
	int		lv_loop;
	char	lv_buffer[10];
	char	lv_nm_caption[50];
	char	lv_nm_enabled[50];
	char	lv_nm_needcti[50];
	char	lv_nm_startch[50];
	char	lv_nm_endch[50];
	char	lv_nm_prgid[50];
	char	lv_nm_prgfilename[50];
	char	lv_nm_channdn[50];
	char	lv_nm_obonly[50];
	char	lv_nm_BindFaxRes[50];
	char	lv_nm_rptid[50];
	char	lv_nm_custid[50];		// Sun added 2012-09-08 

	// Sun added 2001-05-07
	// Get Vox & Record path
	gb_FN_VoxPath=AddTailBackslashToPath(theApp.GetProfileString("SYSDIR","FN_VoxPath",FN_VOXPATH));
	theApp.WriteProfileString("SYSDIR","FN_VoxPath",gb_FN_VoxPath);
	
	gb_FN_RecPath=AddTailBackslashToPath(theApp.GetProfileString("SYSDIR","FN_RecPath",FN_RECPATH));
	theApp.WriteProfileString("SYSDIR","FN_RecPath",gb_FN_RecPath);

	gb_FN_TempPath=AddTailBackslashToPath(theApp.GetProfileString("SYSDIR","FN_TempPath",FN_TEMPPATH));
	theApp.WriteProfileString("SYSDIR","FN_TempPath",gb_FN_TempPath);
	PcsMakeSureDirectoryPathExists(gb_FN_TempPath);

	// Sys ID
	theApp.m_nSysNo = (UC)theApp.GetProfileInt("SYSEN", "SystemID", 0);
	theApp.WriteProfileInt("SYSEN", "SystemID", theApp.m_nSysNo);

	// Sun added 2006-02-20
	/// Main Timer Interval (in seconds)
	m_main_timer = theApp.GetProfileInt("SYSEN", "MainTimer", 2);
	if( m_main_timer <= 0 || m_main_timer > 30 )
		m_main_timer = 2;
	theApp.WriteProfileInt("SYSEN", "MainTimer", m_main_timer);

	// Rich Refresh
	/// Log Information
	theApp.m_blnChannelRichRefresh = (theApp.GetProfileInt("SYSEN", "RichRefresh", 1) > 0);
	theApp.WriteProfileInt("SYSEN", "RichRefresh", theApp.m_blnChannelRichRefresh);

	// LOG Level
	theApp.m_nLOG = theApp.GetProfileInt("SYSEN", "LOGLEVEL", TraceNone);
	theApp.WriteProfileInt("SYSEN", "LOGLEVEL", theApp.m_nLOG);

	//// Create LOG Directory
	// Set trace information
	/// set trace path
	lv_sTemp = theApp.m_sAppPath;
	lv_sTemp += _T("\\LOG");
	//// Create LOG Directory
	PcsMakeSureDirectoryPathExists(lv_sTemp);
	lv_sTemp += _T("\\pvc");
	SetTraceFilePrefix( lv_sTemp );
	/// update trace level
	SetTraceLevel(theApp.m_nLOG);

	//===================================================
	// Sun added 2004-11-20
	/// DataRecord MQ Length
	theApp.m_nDRMQLength = theApp.GetProfileInt("SYSEN", "MQL_DataRecorder", 1024);
	m_sckDataRecorder.ChangeQueueSize(theApp.m_nDRMQLength);
	theApp.WriteProfileInt("SYSEN", "MQL_DataRecorder", theApp.m_nDRMQLength);

	/// SQL Log
	lv_sTemp = theApp.GetProfileString("SYSEN", "SQLREM", _T("--"));
	theApp.WriteProfileString("SYSEN", "SQLREM", lv_sTemp);
	m_sckDataRecorder.SetRemoFlag(lv_sTemp);
	// Set SQL Log path
	lv_sTemp = theApp.m_sAppPath;
	lv_sTemp += _T("\\SQL");
	//// Create SQL Data Directory
	PcsMakeSureDirectoryPathExists(lv_sTemp);
	lv_sTemp += _T("\\PVC");
	m_sckDataRecorder.SetFilePrefix( lv_sTemp );
	//===================================================

	// DB Access Parameters
	//////////////////////////////////////////////
	// Database
	/// Connection Type
	m_DBReader.SetDBConnectType( theApp.GetProfileInt("SYSEN", "SYSDBACCESSTYPE", MYDB_CONNTYPE_OLEDB) );
	theApp.WriteProfileInt("SYSEN", "SYSDBACCESSTYPE", m_DBReader.GetDBConnectType());

	/// DB Server / DSN
	lv_sTemp = theApp.GetProfileString("SYSEN", "SYSDBSERVER");
	if( lv_sTemp.GetLength() == 0 )
	{
		m_DBReader.SetDataSource("127.0.0.1");
		theApp.WriteProfileString("SYSEN", "SYSDBSERVER", m_DBReader.GetDataSource());
	}
	else
		m_DBReader.SetDataSource(lv_sTemp);

	/// Database Name
	lv_sTemp = theApp.GetProfileString("SYSEN", "SYSDBNAME");
	if( lv_sTemp.GetLength() == 0 )
		theApp.WriteProfileString("SYSEN", "SYSDBNAME", "");
	m_DBReader.SetDBName(lv_sTemp);

	/// NULL / User Name
	lv_sTemp = theApp.GetProfileString("SYSEN", "SYSDBUserName");
	if( lv_sTemp.GetLength() == 0 )
	{
		m_DBReader.SetUserName("sa");
		theApp.WriteProfileString("SYSEN", "SYSDBUserName", m_DBReader.GetUserName());
	}
	else
		m_DBReader.SetUserName(lv_sTemp);

	/// Null / Password
	lv_sTemp = theApp.GetProfileString("SYSEN", "SYSDBPASSWORD");
	if( lv_sTemp.GetLength() == 0 )
		theApp.WriteProfileString("SYSEN", "SYSDBPASSWORD", "");
	m_DBReader.SetPassword(lv_sTemp);

	// Sun added 2003-04-28
	/// 是否使用Global Call
	if( theApp.GetProfileInt("SYSEN","NT_GLOBALCALL",0) == 1 )
		gb_NT_UserGlobalCall = TRUE;
	else
		gb_NT_UserGlobalCall = FALSE;

	/// 信令方式
	if( gb_NT_UserGlobalCall )
	{
		/// Sun added 2005-03-23
		if( theApp.GetProfileInt("SYSEN", "NT_INCLUDE_D41", 0) )
			gb_NT_IncludeD41 = TRUE;
		else
			gb_NT_IncludeD41 = FALSE;
		theApp.WriteProfileInt("SYSEN", "NT_INCLUDE_D41", gb_NT_IncludeD41);

		gb_NT_Protocol=theApp.GetProfileString("SYSEN", "NT_PROTOCOL", "");
		if( gb_NT_Protocol.GetLength() == 0 )
		{
			lv_sTemp.LoadString(IDS_MSG_E_NEED_GC_PROTOCOL);
			WriteTrace(TraceError, lv_sTemp);
			AfxMessageBox(IDS_MSG_E_NEED_GC_PROTOCOL);
			return 0;
		}

		// Sun added 2005-06-15
#ifdef INTEL_DIALOGIC
		gb_NT_CaptureABSignal = (theApp.GetProfileInt("SYSEN", "NT_ABSIGNAL", 0) == 1);
		gb_NT_ABS_Disconnect = theApp.GetProfileInt("SYSEN", "NT_ABS_DISC", DTB_AON);
		theApp.WriteProfileInt("SYSEN", "NT_ABSIGNAL", gb_NT_CaptureABSignal);
		theApp.WriteProfileInt("SYSEN", "NT_ABS_DISC", gb_NT_ABS_Disconnect);
#endif
	}
	else
	{
		gb_NT_IncludeD41 = FALSE;
		gb_NT_CaptureABSignal = FALSE;
		gb_NT_ABS_Disconnect = 0;
	}
	// End of Sun added 2003-04-28

	// Get CTI Server Info
	// CTI Listening Port#
	m_param_ctiip = theApp.GetProfileString("SOCKETINFO","CTIIP","");
	m_param_ctiport = theApp.GetProfileInt("SOCKETINFO", "CTIPORT", 0);

	// Sun added 2006-03-05
	m_param_localport = theApp.GetProfileInt("SOCKETINFO", "LOCALPORT", 10020);
	theApp.WriteProfileInt("SOCKETINFO", "LOCALPORT", m_param_localport);

	//-------------------------------------------------------
	// Sun added 2010-05-18 [V7.1.8.3]
	/// Connection Parameters for PowerCallServer2
	m_param_cti2ip = theApp.GetProfileString("SOCKETINFO", "CTIIP2", "");
	if( m_param_cti2ip.GetLength() < 7 )
	{
		m_param_cti2ip = _T("");
		theApp.WriteProfileString("SOCKETINFO", "CTIIP2", m_param_cti2ip);
	}

	m_param_cti2port = theApp.GetProfileInt("SOCKETINFO", "CTIPORT2", 0);
	theApp.WriteProfileInt("SOCKETINFO", "CTIPORT2", m_param_cti2port);
	//-------------------------------------------------------

	//-----------------------------------------------------------------
	// Sun added 2005-03-17
	// Get Ext Server Info
	m_blnEnableEXT = (theApp.GetProfileInt("SOCKETINFO", "SupportEXT", 0) > 0);
	m_param_extport = theApp.GetProfileInt("SOCKETINFO", "EXTPORT", 0);
	m_param_extip = theApp.GetProfileString("SOCKETINFO", "EXTIP", "");

	// Get ASR Server Info
	m_blnEnableASR = (theApp.GetProfileInt("ASR", "SupportASR", 0) > 0);
	m_param_asrport = theApp.GetProfileInt("ASR", "ServerPort", 0);
	m_param_asrip = theApp.GetProfileString("ASR", "ServerIP", "");
	//-----------------------------------------------------------------

	// Data Recorder Host
	m_param_drport = theApp.GetProfileInt("SOCKETINFO", "SYSDBRecorderPort", 7815);
	m_param_drip = theApp.GetProfileString("SOCKETINFO", "SYSDBRecorderIP", "");

	// 是否自动退出登录
	theApp.m_blnAutoChLogout = (theApp.GetProfileInt("WORKINFO", "AutoChannelLogout", 0) > 0 );
	theApp.WriteProfileInt("WORKINFO", "AutoChannelLogout", theApp.m_blnAutoChLogout);

	// Get Device Configuration
	theApp.m_nIVRBoardStart = theApp.GetProfileInt("WORKINFO", "IVRBoardStart", 1);
	if( theApp.m_nIVRBoardStart < 1 )
		theApp.m_nIVRBoardStart = 1;
	theApp.WriteProfileInt("WORKINFO", "IVRBoardStart", theApp.m_nIVRBoardStart);
	theApp.m_nIVRGroups = theApp.GetProfileInt("WORKINFO", "IVRGROUPS", 0);
	if( theApp.m_nIVRGroups > MAX_IVR_GROUP )
		theApp.m_nIVRGroups = (UC)MAX_IVR_GROUP;
	theApp.m_nIVRChannels = theApp.GetProfileInt("WORKINFO", "IVRCHANNELS", 0);
	if( theApp.m_nIVRChannels > MAX_IVR_CHANNEL )
		theApp.m_nIVRChannels = MAX_IVR_CHANNEL;

	// Start # of DTI Resource Board，Sun added 2012-05-24
	theApp.m_nDTIBoardStart = theApp.GetProfileInt("WORKINFO", "DTIBoardStart", 1);
	if( theApp.m_nDTIBoardStart < 0 )
		theApp.m_nDTIBoardStart = 1;
	theApp.WriteProfileInt("WORKINFO", "DTIBoardStart", theApp.m_nDTIBoardStart);

	// Start # of Fax Resource Board，Sun added 2012-05-24
	theApp.m_nFaxBoardStart = theApp.GetProfileInt("WORKINFO", "FaxBoardStart", 0);
	if( theApp.m_nFaxBoardStart < 0 )
		theApp.m_nFaxBoardStart = 0;
	theApp.WriteProfileInt("WORKINFO", "FaxBoardStart", theApp.m_nFaxBoardStart);

	//-------------------------------------------------------------
	// Sun added 2012-06-19
	// Continuous # of IVR Channels
	theApp.m_nIVRChannelSPAN = theApp.GetProfileInt("WORKINFO", "IVRChannelSPAN", 0);
	if( theApp.m_nIVRChannelSPAN < 0 )
		theApp.m_nIVRChannelSPAN = 0;
	theApp.WriteProfileInt("WORKINFO", "IVRChannelSPAN", theApp.m_nIVRChannelSPAN);

	// Whether IVR & FAX Spans apear alternantly
	theApp.m_blnIVRFaxAlternant = theApp.GetProfileInt("WORKINFO", "IVRFaxAlternant", 0);
	theApp.WriteProfileInt("WORKINFO", "IVRFaxAlternant", theApp.m_blnIVRFaxAlternant);
	//-------------------------------------------------------------

	// 通道声音大小  -10 -- +10
	gb_VoiceVolume = theApp.GetProfileInt("WORKINFO", "VOICEVOLUME", 0);
	if(gb_VoiceVolume > 10)
		gb_VoiceVolume = 10;
	if(gb_VoiceVolume < -10)
		gb_VoiceVolume = -10;

	// 摘机前振铃次数
	gb_RingsPreOffhook = theApp.GetProfileInt("WORKINFO", "RINGSPREOFFHOOK", 0);
	if(gb_RingsPreOffhook > 3)
		gb_RingsPreOffhook = 3;
	if(gb_RingsPreOffhook < 0)
		gb_RingsPreOffhook = 0;

	// Sun added 2004-11-20
	// 模拟线转接Flash时长设置
	theApp.m_nAnalogFlashTM = theApp.GetProfileInt("WORKINFO", "FlashTime", 50);
	if(theApp.m_nAnalogFlashTM > 100)
		theApp.m_nAnalogFlashTM = 50;
	if(theApp.m_nAnalogFlashTM < 1)
		theApp.m_nAnalogFlashTM = 10;
	theApp.WriteProfileInt("WORKINFO", "FlashTime", theApp.m_nAnalogFlashTM);

	// Sun added 2009-06-27
	// 是否进行异步转接
	g_blnAnalogAsynDivert = (theApp.GetProfileInt("WORKINFO", "AnalogAsynDivert", 0) > 0 );
	theApp.WriteProfileInt("WORKINFO", "AnalogAsynDivert", g_blnAnalogAsynDivert);

	// Sun added 2004-08-24
	// 最长通话时间(s)
	theApp.m_nMaxCallLength = theApp.GetProfileInt("WORKINFO", "MaxCallLength", 1800);
	if(theApp.m_nMaxCallLength < 10)
		theApp.m_nMaxCallLength = 0;
	theApp.WriteProfileInt("WORKINFO", "MaxCallLength", theApp.m_nMaxCallLength);

	// 最长放音时间(s)
	theApp.m_nMaxSoundTime = theApp.GetProfileInt("WORKINFO", "MaxSoundTime", 0);
	if(theApp.m_nMaxSoundTime < 0)
		theApp.m_nMaxSoundTime = 0;

	// 最大队列等待时长(s)
	theApp.m_nMaxWaitTime = theApp.GetProfileInt("WORKINFO", "MaxWaitTime", 0);
	if(theApp.m_nMaxWaitTime < 0)
		theApp.m_nMaxWaitTime = 0;

	// Sun added 2006-10-01
	// 最长传真时间(s)
	theApp.m_nMaxFaxTime = theApp.GetProfileInt("WORKINFO", "MaxFaxTime", 0);
	if(theApp.m_nMaxFaxTime < 0)
		theApp.m_nMaxFaxTime = 0;
	if(theApp.m_nMaxFaxTime > 0 && theApp.m_nMaxFaxTime < 60)
		theApp.m_nMaxFaxTime = 600;
	theApp.WriteProfileInt("WORKINFO", "MaxFaxTime", theApp.m_nMaxFaxTime);
	
	// 通道应答延时
	theApp.m_nCallAnswerDelay = theApp.GetProfileInt("WORKINFO", "CallAnswerDelay", 1000);
	if(theApp.m_nCallAnswerDelay < 100)
		theApp.m_nCallAnswerDelay = 0;

	// CTI转接延时
	theApp.m_nCTITransferDelay = theApp.GetProfileInt("WORKINFO","CTITransferDelay", 200);
	if(theApp.m_nCTITransferDelay < 10)
		theApp.m_nCTITransferDelay = 0;
	else if(theApp.m_nCTITransferDelay > 2000)
		theApp.m_nCTITransferDelay = 2000;
	theApp.WriteProfileInt("WORKINFO", "CTITransferDelay", theApp.m_nCTITransferDelay);

	// CTI挂机延时
	theApp.m_nCTIOnHookDelay = theApp.GetProfileInt("WORKINFO", "CTIOnHookDelay", 200);
	if(theApp.m_nCTIOnHookDelay < 10)
		theApp.m_nCTIOnHookDelay = 0;
	else if(theApp.m_nCTIOnHookDelay > 2000)
		theApp.m_nCTIOnHookDelay = 2000;
	theApp.WriteProfileInt("WORKINFO", "CTIOnHookDelay", theApp.m_nCTIOnHookDelay);

	// Sun added 2013-03-18, CTI挂机异步挂机[1, 0..1]
	theApp.m_bytCTIAnsyOnHook = theApp.GetProfileInt("WORKINFO", "CTIAnsyOnHook", 1);
	if(theApp.m_bytCTIAnsyOnHook > 1)
		theApp.m_bytCTIAnsyOnHook = 1;
	else if(theApp.m_bytCTIAnsyOnHook < 0)
		theApp.m_bytCTIAnsyOnHook = 0;
	theApp.WriteProfileInt("WORKINFO", "CTIAnsyOnHook", theApp.m_bytCTIAnsyOnHook);
	
	// FAX远端挂断延时（秒）
	theApp.m_nFaxDiscDelay = theApp.GetProfileInt("WORKINFO", "FaxRemoteDiscDelay", 5);
	theApp.WriteProfileInt("WORKINFO", "FaxRemoteDiscDelay", theApp.m_nFaxDiscDelay);

	// Sun added 2012-05-07
	theApp.m_nMaxBeforeHookOnSteps = theApp.GetProfileInt("WORKINFO", "MaxBeforeHookOnStep", 3);
	if(theApp.m_nMaxBeforeHookOnSteps < 0)
		theApp.m_nMaxBeforeHookOnSteps = 0;
	else if(theApp.m_nMaxBeforeHookOnSteps > 20)
		theApp.m_nMaxBeforeHookOnSteps = 3;
	theApp.WriteProfileInt("WORKINFO", "MaxBeforeHookOnStep", theApp.m_nMaxBeforeHookOnSteps);
	
	// Sun added 2008-05-29, RoutePoint设备空闲自检周期（秒）
	gb_lngRoutePointCheckTimer = theApp.GetProfileInt("WORKINFO", "RoutePointAutoCheckTimer", 15);
	theApp.WriteProfileInt("WORKINFO", "RoutePointAutoCheckTimer", gb_lngRoutePointCheckTimer);

#ifdef CISCO_CCM
	// Sun added 2010-07-31, CTI路由点最大并发呼叫数
	gb_nRoutePointMaxConcurrentCall = theApp.GetProfileInt("WORKINFO", "RoutePointMaxConcurrentCall", 4);
	if( gb_nRoutePointMaxConcurrentCall > DEF_TAPI_MAX_CALLLIST )
		gb_nRoutePointMaxConcurrentCall = DEF_TAPI_MAX_CALLLIST;
	else if( gb_nRoutePointMaxConcurrentCall < 2 )
		gb_nRoutePointMaxConcurrentCall = 2;
	theApp.WriteProfileInt("WORKINFO", "RoutePointMaxConcurrentCall", gb_nRoutePointMaxConcurrentCall);
#endif

	// 通道复位时间，分钟
	theApp.m_nChannelRestartTime = theApp.GetProfileInt("WORKINFO", "ChannelRestartTime", 15);
	if(theApp.m_nChannelRestartTime < 1)
		theApp.m_nChannelRestartTime = 0;

	// Sun added 2007-11-04
	/// 是否自动计算空闲时的通道复位时间
	theApp.m_blnAutoCalcCHRT = (theApp.GetProfileInt("WORKINFO", "IsAutoCalcChannelRestartTime", 1) > 0);
	theApp.WriteProfileInt("WORKINFO", "IsAutoCalcChannelRestartTime", theApp.m_blnAutoCalcCHRT);

	// Sun added 2011-04-16
	/// 自动重启开关（位操作）
	gb_lngAutoCloseFlag = theApp.GetProfileInt("SYSEN", "AutoCloseFlag", 0xFF);
	theApp.WriteProfileInt("SYSEN", "AutoCloseFlag", gb_lngAutoCloseFlag);

	//----------------------------------------------------------------------------
	// Sun added 2011-04-25
	//----------------------------------------------------------------------------
	// 统计转接失败率参数
	/// 1、统计转接失败率的最近请求转接电话个数，默认20，取值范围[5..200]
	gb_nMW_TransFail_Calls = theApp.GetProfileInt("SYSEN", "MW_TransFail_Calls", 20);
	if(gb_nMW_TransFail_Calls < 5 || gb_nMW_TransFail_Calls > 200)
		gb_nMW_TransFail_Calls = 20;
	theApp.WriteProfileInt("SYSEN", "MW_TransFail_Calls", gb_nMW_TransFail_Calls);

	/// 2、允许的转接失败率最大值（百分比），默认30，取值范围[1..100]
	gb_nTH_MaxTransFailRate = theApp.GetProfileInt("SYSEN", "TH_MaxTransFailRate", 30);
	if(gb_nTH_MaxTransFailRate < 1 || gb_nTH_MaxTransFailRate > 100)
		gb_nTH_MaxTransFailRate = 30;
	theApp.WriteProfileInt("SYSEN", "TH_MaxTransFailRate", gb_nTH_MaxTransFailRate);
	//----------------------------------------------------------------------------

	// Sun added 2013-09-09
	gb_nMW_CreateOnHookThread = 0;

	// 最大尝试次数
	theApp.m_nMaxTryTimes = theApp.GetProfileInt("WORKINFO","MaxTryTimes",MAX_TRYTIMES);
	if(theApp.m_nMaxTryTimes < 1)
		theApp.m_nMaxTryTimes = MAX_TRYTIMES;
	theApp.WriteProfileInt("WORKINFO", "MaxTryTimes", theApp.m_nMaxTryTimes);

	// TAPI会议磋商失败最大尝试次数，Sun added 2010-08-02 [B]
	theApp.m_nCallTransferMaxTryTimes = theApp.GetProfileInt("WORKINFO","CallTransferMaxTryTimes",MAX_TRYTIMES);
	if(theApp.m_nCallTransferMaxTryTimes < 1)
		theApp.m_nCallTransferMaxTryTimes = MAX_TRYTIMES;
	theApp.WriteProfileInt("WORKINFO", "CallTransferMaxTryTimes", theApp.m_nCallTransferMaxTryTimes);

#ifdef CISCO_CCM
	// TAPI会议磋商失败处理方式参数，Sun added 2010-08-10
	/// -1 - 挂断；1 - 继续排队；2 - 做无应答处理；3 - 尝试单步转接
	gb_nSetupConferenceFailedMethod = theApp.GetProfileInt("SYSEN","ConferenceFailedMethod",-1);
	if( gb_nSetupConferenceFailedMethod < -1)
		gb_nSetupConferenceFailedMethod = -1;
	else if( gb_nSetupConferenceFailedMethod > 10 )
		gb_nSetupConferenceFailedMethod = 2;
	else if( gb_nSetupConferenceFailedMethod == 0 )
		gb_nSetupConferenceFailedMethod = 1;
	theApp.WriteProfileInt("SYSEN", "ConferenceFailedMethod", gb_nSetupConferenceFailedMethod);
#endif

//-----------------------------------------------------------------
// Sun added 2010-03-08 for Cisco CME
#ifdef CISCO_CME

	// CME IVR服务参数
	m_CMEServerInfo.m_strServerIP = theApp.GetProfileString("CMEIVRServer", "ServerIP", "");
	theApp.WriteProfileString("CMEIVRServer", "ServerIP", m_CMEServerInfo.m_strServerIP);
	
	m_CMEServerInfo.m_nServerPort = theApp.GetProfileInt("CMEIVRServer", "ListenPort", 8000);
	theApp.WriteProfileInt("CMEIVRServer", "ListenPort", m_CMEServerInfo.m_nServerPort);
	
	m_CMEServerInfo.m_strDefaultStardPage = theApp.GetProfileString("CMEIVRServer", "StardPage", "");
	theApp.WriteProfileString("CMEIVRServer", "StardPage", m_CMEServerInfo.m_strDefaultStardPage);
	
	m_CMEServerInfo.m_strLocalDownloadPath = theApp.GetProfileString("CMEIVRServer", "DownloadPath", "");
	theApp.WriteProfileString("CMEIVRServer", "DownloadPath", m_CMEServerInfo.m_strLocalDownloadPath);
	
	m_CMEServerInfo.m_strLocalUploadPath = theApp.GetProfileString("CMEIVRServer", "UploadPath", "");
	theApp.WriteProfileString("CMEIVRServer", "UploadPath", m_CMEServerInfo.m_strLocalUploadPath);

	m_CMEServerInfo.m_strHTTPPath = theApp.GetProfileString("CMEIVRServer", "HTTPPath", "");
	theApp.WriteProfileString("CMEIVRServer", "HTTPPath", m_CMEServerInfo.m_strHTTPPath);

	m_CMEServerInfo.m_blnUseVirtualFilename = theApp.GetProfileInt("CMEIVRServer", "UseVirtualFilename", 0);
	theApp.WriteProfileInt("CMEIVRServer", "UseVirtualFilename", m_CMEServerInfo.m_blnUseVirtualFilename);

#endif
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Sun added 2012-02-08 for Cisco Media Driver
#ifdef CUCM_MEDIA

	UINT lv_uintTemp;

	lv_uintTemp = theApp.GetProfileInt("CiscoMediaDriver", "portRangeStart", 0);
	if( lv_uintTemp > 0 )
		m_MediaDriver.m_nPortRangeStart = lv_uintTemp;
	theApp.WriteProfileInt("CiscoMediaDriver", "portRangeStart", m_MediaDriver.m_nPortRangeStart);

	lv_uintTemp = theApp.GetProfileInt("CiscoMediaDriver", "numPorts", 0);
	if( lv_uintTemp > 0 )
		m_MediaDriver.m_nNumPorts = lv_uintTemp;
	theApp.WriteProfileInt("CiscoMediaDriver", "numPorts", m_MediaDriver.m_nNumPorts);

	lv_uintTemp = theApp.GetProfileInt("CiscoMediaDriver", "epAPITraceLevel", 2);
	if( lv_uintTemp >= 0 && lv_uintTemp <= 4 )
		m_MediaDriver.m_nTraceLevel = lv_uintTemp;
	theApp.WriteProfileInt("CiscoMediaDriver", "epAPITraceLevel", m_MediaDriver.m_nTraceLevel);

#endif
//-----------------------------------------------------------------

	// Get Tone info
	gb_Enable2DiscDTone = (theApp.GetProfileInt("SYSTONE", "Enable2DiscDTone", 0));
	theApp.WriteProfileInt("SYSTONE", "Enable2DiscDTone", gb_Enable2DiscDTone);
	gb_Enable2DiscSTone = (theApp.GetProfileInt("SYSTONE", "Enable2DiscSTone", 0));
	theApp.WriteProfileInt("SYSTONE", "Enable2DiscSTone", gb_Enable2DiscSTone);
	for( lv_loop = 0; lv_loop < MAX_CHANNEL_TONES; lv_loop++ )
	{
		Tones[lv_loop]=theApp.GetProfileString("SYSTONE",gb_Tone[lv_loop].str,"");
		if( Tones[lv_loop] == "" )
			continue;
		Information_Init_SetSystemTones(Tones[lv_loop], lv_loop);
		Tones[lv_loop].ReleaseBuffer();
	}

	// Necessary Parameters Verify
	if(theApp.m_nIVRGroups == 0)
		return 0;
	if(theApp.m_nIVRChannels == 0)
		return 0;

	// Read IVR Group Info
	for( lv_loop=0; lv_loop < theApp.m_nIVRGroups; lv_loop++ )
	{
		_itoa(lv_loop + 1, lv_buffer, 10);
		sprintf(lv_nm_caption, "GROUP%s_Caption", lv_buffer);
		sprintf(lv_nm_enabled, "GROUP%s_Enabled", lv_buffer);
		sprintf(lv_nm_needcti, "GROUP%s_NeedCTI", lv_buffer);
		sprintf(lv_nm_startch, "GROUP%s_StartCH", lv_buffer);
		sprintf(lv_nm_endch, "GROUP%s_EndCH", lv_buffer);
		sprintf(lv_nm_prgid, "GROUP%s_IVRProgramID", lv_buffer);
		sprintf(lv_nm_prgfilename, "GROUP%s_IVRProgramFN", lv_buffer);
		sprintf(lv_nm_obonly, "GROUP%s_OBOnly", lv_buffer);
		sprintf(lv_nm_BindFaxRes, "GROUP%s_BindFaxResource", lv_buffer);
		sprintf(lv_nm_rptid, "GROUP%s_RoutePoint", lv_buffer);

		// Sun added 2012-09-08
		sprintf(lv_nm_custid, "GROUP%s_CustID", lv_buffer);
								
		PGroupInfo[lv_loop].Caption = theApp.GetProfileString("GROUPINFO",lv_nm_caption,"UNTITLED");
		PGroupInfo[lv_loop].Enabled = (theApp.GetProfileInt("GROUPINFO",lv_nm_enabled,0) == 1);
		PGroupInfo[lv_loop].NeedCTI = (theApp.GetProfileInt("GROUPINFO",lv_nm_needcti,0) == 1);
		PGroupInfo[lv_loop].StartCH = theApp.GetProfileInt("GROUPINFO",lv_nm_startch,0);
		PGroupInfo[lv_loop].EndCH = theApp.GetProfileInt("GROUPINFO",lv_nm_endch,0);
		PGroupInfo[lv_loop].IVRProgramID = theApp.GetProfileInt("GROUPINFO",lv_nm_prgid,0);

		// Sun added 2005-07-10
		PGroupInfo[lv_loop].blnOBOnly = (theApp.GetProfileInt("GROUPINFO",lv_nm_obonly,0) == 1);

		if( PGroupInfo[lv_loop].IVRProgramID == DEF_LD_GRP_ET_FILE )
		{
			PGroupInfo[lv_loop].IVRFileName = theApp.GetProfileString("GROUPINFO",lv_nm_prgfilename,"");
		}
		else
		{
			PGroupInfo[lv_loop].IVRFileName = _T("");
		}

		// Sun added 2006-11-30
		PGroupInfo[lv_loop].blnBindFaxResource = (theApp.GetProfileInt("GROUPINFO",lv_nm_BindFaxRes,1) == 1);
		theApp.WriteProfileInt("GROUPINFO", lv_nm_BindFaxRes, PGroupInfo[lv_loop].blnBindFaxResource);

#ifndef INTEL_DIALOGIC
		// Route Point ID for the Group
		PGroupInfo[lv_loop].dwRoutePointID = theApp.GetProfileInt("GROUPINFO",lv_nm_rptid,0);
		theApp.WriteProfileInt("GROUPINFO", lv_nm_rptid, PGroupInfo[lv_loop].dwRoutePointID);
#endif

		// CustID for the Group, Sun added 2012-09-08
		PGroupInfo[lv_loop].bytCustID = theApp.GetProfileInt("GROUPINFO",lv_nm_custid,0);
		theApp.WriteProfileInt("GROUPINFO", lv_nm_custid, PGroupInfo[lv_loop].bytCustID);

		if( PGroupInfo[lv_loop].StartCH >= theApp.m_nIVRChannels )
			return 0;
		if( PGroupInfo[lv_loop].EndCH >= theApp.m_nIVRChannels )
			return 0;
		if( PGroupInfo[lv_loop].StartCH > PGroupInfo[lv_loop].EndCH )
			PGroupInfo[lv_loop].EndCH = PGroupInfo[lv_loop].StartCH;
	}

	// Read IVR Channel Info
	for( lv_loop=0; lv_loop < theApp.m_nIVRChannels; lv_loop++ )
	{
		_itoa(lv_loop + 1, lv_buffer, 10);
		sprintf(lv_nm_channdn, "CH%s_DN", lv_buffer);
		PChannelDN[lv_loop] = theApp.GetProfileString("CHANNELINFO",lv_nm_channdn,"");
	}

	//===================================================
	// Sun added 2004-12-30
	/// TTS section
	m_blnEnableTTS = (theApp.GetProfileInt("TTS", "SupportTTS", 0) > 0);
	theApp.WriteProfileInt("TTS", "SupportTTS", m_blnEnableTTS);
	m_nTTSBufferCount = theApp.GetProfileInt("TTS", "BufferCount", 4);
	theApp.WriteProfileInt("TTS", "BufferCount", m_nTTSBufferCount);
	m_nTTSBufferSize = theApp.GetProfileInt("TTS", "BufferSize", 64);
	theApp.WriteProfileInt("TTS", "BufferSize", m_nTTSBufferSize);
	m_nTTSResourceTO = theApp.GetProfileInt("TTS", "ResourceTimeOut", 5);
	theApp.WriteProfileInt("TTS", "ResourceTimeOut", m_nTTSResourceTO);
	m_TTSServerInfo.m_strCompanyName = _T("PicaSoft");
	m_TTSServerInfo.m_strUserName = _T("PowerVoice");
	m_TTSServerInfo.m_strProductName = THEAPP_NAME;
	m_TTSServerInfo.m_strSerialNO = theApp.GetProfileString("TTS", "SerialNO", "");
	theApp.WriteProfileString("TTS", "SerialNO", m_TTSServerInfo.m_strSerialNO);
	m_TTSServerInfo.m_strServerIP = theApp.GetProfileString("TTS", "ServerIP", "127.0.0.1");
	theApp.WriteProfileString("TTS", "ServerIP", m_TTSServerInfo.m_strServerIP);
	m_TTSServerInfo.m_nServerPort = theApp.GetProfileInt("TTS", "ServerPort", 0);
	theApp.WriteProfileInt("TTS", "ServerPort", m_TTSServerInfo.m_nServerPort);
	m_TTSServerInfo.m_nVID = theApp.GetProfileInt("TTS", "VID", 0);
	theApp.WriteProfileInt("TTS", "VID", m_TTSServerInfo.m_nVID);
	m_TTSServerInfo.m_nSpeed = theApp.GetProfileInt("TTS", "Speed", 0);
	theApp.WriteProfileInt("TTS", "Speed", m_TTSServerInfo.m_nSpeed);
	m_TTSServerInfo.m_nVolume = theApp.GetProfileInt("TTS", "Volume", 0);
	theApp.WriteProfileInt("TTS", "Volume", m_TTSServerInfo.m_nVolume);
	m_TTSServerInfo.m_nPitch = theApp.GetProfileInt("TTS", "Pitch", 0);
	theApp.WriteProfileInt("TTS", "Pitch", m_TTSServerInfo.m_nPitch);
	//===================================================

	//===================================================
	// Sun added 2007-10-20， TTS属性扩展
	/// 背景音乐
	//// 背景音乐编号，0表示没有
	m_TTSServerInfo.m_nBackAudioIndex = theApp.GetProfileInt("TTS", "BackAudioIndex", 0);
	theApp.WriteProfileInt("TTS", "BackAudioIndex", m_TTSServerInfo.m_nBackAudioIndex);

	//// 背景音乐音量，0-100，缺省50
	m_TTSServerInfo.m_nBackAudioVolume = theApp.GetProfileInt("TTS", "BackAudioVolume", 50);
	theApp.WriteProfileInt("TTS", "BackAudioVolume", m_TTSServerInfo.m_nBackAudioVolume);

	//// 背景音乐属性：是否允许重复播放
	m_TTSServerInfo.m_nBackAudioFlag = theApp.GetProfileInt("TTS", "BackAudioFlag", 0);
	theApp.WriteProfileInt("TTS", "BackAudioFlag", m_TTSServerInfo.m_nBackAudioFlag);
	//===================================================

	//////////////////////////////////////////////
	// Version Info.
	m_nVerAdmin = theApp.GetProfileInt("Version", "IVRAdmin", 0);
	theApp.WriteProfileInt("Version", "IVRAdmin", m_nVerAdmin);

	m_nVerClient = theApp.GetProfileInt("Version", "IVRClient", 0);
	theApp.WriteProfileInt("Version", "IVRClient", m_nVerClient);
	//////////////////////////////////////////////

	return 1;
}

//////////////////////////////////////////////////////////////////////
// SYSTEM Process
//////////////////////////////////////////////////////////////////////
//
// Start System 
// SUCC:Return 1;ERROR:Return 0
int CMainFrame::Sys_StartSystem()
{
	BOOL lv_bLicensed = TRUE;

	// Load Parameters from Ini file
	if(!Information_LoadIniSystemParamter())
	{
		MsgPrintf(TraceError, "Load system parameters Failed!");
		return 0;
	}

	// Sun added 2009-01-09
	/// Log Version Information
	// Version Info
	CString lv_strVersionInfo;
	lv_strVersionInfo.Format("%s - FileVersion(%s): %s", THEAPP_NAME, theApp.m_sFileVersion, theApp.m_sVersionData);
	WriteTrace(TraceInfo, "Application Version Infomation - %s", lv_strVersionInfo);

	// Sun added 2004-11-20
	// Load Product License
	// Sun added 2013-04-25
#ifndef SMARTAC_LICENSE
	if( theApp.m_License.InitializeProductLicense(theApp.m_sAppPath, THEAPP_NAME) <= 0 )
#else
	if( SMARTAC_LICENSE->InitializeProductLicense(theApp.m_sAppPath, THEAPP_NAME) <= 0 )
#endif
	{
		MsgPrintf(TraceWarn, "Can't initialize product license file!");
		lv_bLicensed = FALSE;
	}

// Sun added 2013-04-25
#ifndef SMARTAC_LICENSE
	theApp.m_strThisMacAddress = theApp.m_SysInfo.GetLocalNICMAC(theApp.m_License.m_ucServerNICIndex);
#else
	theApp.m_strThisMacAddress = theApp.m_SysInfo.GetLocalNICMAC(SMARTAC_LICENSE->m_ucServerNICIndex);
#endif

//#ifndef _DEBUG

	// Verify Mac Address
	if( lv_bLicensed )
	{
// Sun added 2013-04-25
#ifndef SMARTAC_LICENSE
		if( theApp.m_License.m_strServerMacAddress != _T("") )
#else
		if( SMARTAC_LICENSE->m_strServerMacAddress != _T("") )
#endif
		{
// Sun added 2013-04-25
#ifndef SMARTAC_LICENSE
			if( theApp.m_SysInfo.FindLocalNICMAC( theApp.m_License.m_strServerMacAddress ) == 255 )
#else
			if( theApp.m_SysInfo.FindLocalNICMAC( SMARTAC_LICENSE->m_strServerMacAddress ) == 255 )
#endif
			{
				MsgPrintf(TraceWarn, "NIC of current server don't match the license!");
				lv_bLicensed = FALSE;
			}
			else
// Sun added 2013-04-25
#ifndef SMARTAC_LICENSE
				theApp.m_strThisMacAddress = theApp.m_License.m_strServerMacAddress;
#else
				theApp.m_strThisMacAddress = SMARTAC_LICENSE->m_strServerMacAddress;
#endif
		}
	}

	// Verify HDSN
	if( lv_bLicensed )
	{
// Sun added 2013-04-25
#ifndef SMARTAC_LICENSE
		if( theApp.m_License.m_strServerHDSerialNumber != _T("") )
#else
		if( SMARTAC_LICENSE->m_strServerHDSerialNumber != _T("") )
#endif
		{
// Sun added 2013-04-25
#ifndef SMARTAC_LICENSE
			if( theApp.m_strThisHDSerialNumber.CompareNoCase(theApp.m_License.m_strServerHDSerialNumber) != 0 )
#else
			if( theApp.m_strThisHDSerialNumber.CompareNoCase(SMARTAC_LICENSE->m_strServerHDSerialNumber) != 0 )
#endif
			{
				MsgPrintf(TraceWarn, "HD Volume Serial Number of current server don't match the license!");
				lv_bLicensed = FALSE;
			}
		}
	}

//#endif

	// Sun updated 2004-12-30
// Sun added 2013-04-25
#ifndef SMARTAC_LICENSE
	MsgPrintf( TraceInfo, "License information - IVR Group [%d], IVR Channel [%d], TTS [%d], ASR [%d].", 
		theApp.m_License.GetLicenseClient(PCS_LIC_IVR_GROUP), 
		theApp.m_License.GetLicenseClient(PCS_LIC_IVR_CHANNEL), 
		theApp.m_License.GetLicenseClient(PCS_LIC_TTS_CONNECT),
		theApp.m_License.GetLicenseClient(PCS_LIC_ASR_CONNECT) );
#else
	MsgPrintf( TraceInfo, "License information - IVR Group [%d], IVR Channel [%d], TTS [%d], ASR [%d].", 
		SMARTAC_LICENSE->GetLicenseClient(PCS_LIC_IVR_GROUP), 
		SMARTAC_LICENSE->GetLicenseClient(PCS_LIC_IVR_CHANNEL), 
		SMARTAC_LICENSE->GetLicenseClient(PCS_LIC_TTS_CONNECT),
		SMARTAC_LICENSE->GetLicenseClient(PCS_LIC_ASR_CONNECT) );
#endif

	if( !lv_bLicensed )
		return 0;
	
	// Sun added 2004-11-20
// Sun added 2013-04-25
#ifndef SMARTAC_LICENSE
	if( theApp.m_nIVRGroups > theApp.m_License.GetLicenseClient(PCS_LIC_IVR_GROUP) )
		theApp.m_nIVRGroups = theApp.m_License.GetLicenseClient(PCS_LIC_IVR_GROUP);
	if( theApp.m_nIVRChannels > theApp.m_License.GetLicenseClient(PCS_LIC_IVR_CHANNEL) )
		theApp.m_nIVRChannels = theApp.m_License.GetLicenseClient(PCS_LIC_IVR_CHANNEL);
#else
	if( theApp.m_nIVRGroups > SMARTAC_LICENSE->GetLicenseClient(PCS_LIC_IVR_GROUP) )
		theApp.m_nIVRGroups = SMARTAC_LICENSE->GetLicenseClient(PCS_LIC_IVR_GROUP);
	if( theApp.m_nIVRChannels > SMARTAC_LICENSE->GetLicenseClient(PCS_LIC_IVR_CHANNEL) )
		theApp.m_nIVRChannels = SMARTAC_LICENSE->GetLicenseClient(PCS_LIC_IVR_CHANNEL);
#endif

//-----------------------------------------------------------------
// Sun added 2010-03-08 for Cisco CME
#ifdef CISCO_CME
	// 打开监听
	if( !m_CMEInterface.GetIntefaceStarted() )
		m_CMEInterface.StartupCMEIVR(m_CMEServerInfo);	
#endif
//-----------------------------------------------------------------

	// Create IVR Groups' and Channels' Objects
	if( !Sys_CreateWorkObjects() )
	{
		MsgPrintf(TraceError, "Create System Working Objects Failed!");
		return(0);
	}

	Sys_ConnectDataRecord();

	// Sun added 2004-12-30
	if( m_blnEnableTTS )
	{
		/// TTS initialize
// Sun added 2013-04-25
#ifndef SMARTAC_LICENSE
		m_TTSInterface.InitializeTTS(theApp.m_License.GetLicenseClient(PCS_LIC_TTS_CONNECT), m_nTTSBufferCount, m_nTTSBufferSize);
#else
		m_TTSInterface.InitializeTTS(SMARTAC_LICENSE->GetLicenseClient(PCS_LIC_TTS_CONNECT), m_nTTSBufferCount, m_nTTSBufferSize);
#endif
		/// TTS Startup
		m_TTSInterface.StartupTTS(m_TTSServerInfo);
	}

	// Sun added 2008-03-10
	if( m_blnEnableASR )
	{
		/// ASR initialize
// Sun added 2013-04-25
#ifndef SMARTAC_LICENSE
		m_ASRInterface.InitializeASR(NULL, theApp.m_License.GetLicenseClient(PCS_LIC_ASR_CONNECT));
#else
		m_ASRInterface.InitializeASR(NULL, SMARTAC_LICENSE->GetLicenseClient(PCS_LIC_ASR_CONNECT));
#endif
	}

	// Start Threads
	/// Start System Main thread
	g_thSystemMain.WaitForStartupToComplete();

#ifdef INTEL_DIALOGIC
	/// Start GC events processing thread
	if( m_bGCStarted )
		g_thProcessGCEvents.WaitForStartupToComplete();
#endif

#ifdef CISCO_CCM
	g_thProcessTAPIEvents.WakeUp();
#endif

	// created the listen socket
	if( m_param_localport > 1000 )
	{
		if( m_ListenSocket.Create(m_param_localport) )
		{
			// start listening
			if( m_ListenSocket.Listen() )
			{
				m_bLocalSocketOpened = TRUE;
				m_ListenSocket.m_pWndDlg = this;
				WriteTrace(TraceInfo, "Power Voice Server started on port %d.", m_param_localport);
			}
		}
	}

//-----------------------------------------------------------------
// Sun added 2010-03-08 for Cisco CME
#ifdef CISCO_CME

	// 启动CME IVR服务接口
	if( m_CMEInterface.StartupCMEIVR(m_CMEServerInfo) )
	{
		WriteTrace(TraceInfo, "Power Voice Server started CME IVR interface on port %d.", m_CMEServerInfo.m_nServerPort);
	}

#endif
//-----------------------------------------------------------------

#ifdef INTEL_DIALOGIC
	// Sun moved from ahead 2005-01-21, V6.2.0.4, and add sleep 50 per channel
	/// Sun updated 2012-12-29, changed 50ms to 20ms
	Sleep(theApp.m_nIVRChannels * 20);
#endif

	Sys_ConnectCTIServer();

	// Sun added 2005-03-17
	Sys_ConnectEXTServer();

	// Sun added 2012-12-29
	if( m_param_cti2ip.GetLength() > 0 && m_param_cti2port > 0 )
	{
		Sleep(50);
		Sys_ConnectSecondCTIServer();
	}

	MsgPrintf(TraceInfo, "Power Voice started successfully!");
	
	m_bRunning = TRUE;
	WriteStatusBar(STATUS_BAR_SYS, ICON_Ready);
	SB_PositionControl(&m_wndProgressBar, STATUS_BAR_PROGRESS);

	// Set Main Menu on Group 0
	pGroupView->SetShowGroupNo(0);

	// Sun added 2006-02-26
	m_tmStatisticsDate = CTime::GetCurrentTime();

	// 2004-08-07
	if( m_nMainTimerID == 0 )
		m_nMainTimerID = SetTimer(1, m_main_timer * 1000, NULL);
	
	return(1);
}

int CMainFrame::Sys_StopSystem()
{
#if defined(NDEBUG) || defined(INTEL_DIALOGIC)

	if( !m_bSystemAutoShutDown )
	{
		CDlgGetPassword lv_dlg;
		if( lv_dlg.DoModal() != IDOK )
			return 0;
	}

#endif

	// 2004-08-07
	if( m_nMainTimerID > 0 )
	{
		KillTimer(m_nMainTimerID);
		m_nMainTimerID = 0;
	}

//-----------------------------------------------------------------
// Sun added 2010-03-08 for Cisco CME
#ifdef CISCO_CME

	// 停止CME IVR服务接口
	if( m_CMEInterface.GetIntefaceStarted() )
		m_CMEInterface.StopCMEIVR();

#endif
//-----------------------------------------------------------------

	// Sun added 2004-12-30
	/// Stop TTS
	m_TTSInterface.StopTTS();

	// Sun added 2008-03-10
	/// Stop ASR
	m_ASRInterface.UninitializeASR();

	int lv_loop;

	m_bRunning = FALSE;	
	WriteStatusBar(STATUS_BAR_SYS, ICON_Stop);

	// Sun added 2006-03-05
	if( m_bLocalSocketOpened )
	{
		m_bLocalSocketOpened = FALSE;
		m_ListenSocket.Close();
		m_ThreadList.RemoveAllThreads();
	}

	/// Stop System Main thread
	g_thSystemMain.WaitForShutdownToComplete();

#ifdef INTEL_DIALOGIC
	// Sun added 2004-11-20
	/// Close IVR boards
	for(lv_loop = theApp.m_nIVRBoardStart; lv_loop <= theApp.m_nIVRBoardCount; lv_loop++)
	{
		if( theApp.m_hBoardHandles[lv_loop] != -1 )
			dx_close(theApp.m_hBoardHandles[lv_loop]);
	}
#endif

	// 删除对象
	for(lv_loop = 0; lv_loop < theApp.m_nIVRGroups; lv_loop++)
	{
		if( PIVRGroup[lv_loop] != NULL )
		{
			delete(PIVRGroup[lv_loop]);
			PIVRGroup[lv_loop] = NULL;
		}
	}
	
	m_CTIAgentList.RemoveAllAgents();
	for(lv_loop = 0; lv_loop < theApp.m_nIVRChannels; lv_loop++)
	{
		if( PIVRCH[lv_loop] != NULL )
		{
			delete(PIVRCH[lv_loop]);
			PIVRCH[lv_loop] = NULL;
		}
	}

	// Sun added 2005-03-17
	/// Disconnect from External Server
	m_sckExternalServer.WaitForShutdownToComplete();

	// Disconnect from Data Recorder
	m_sckDataRecorder.WaitForShutdownToComplete();

	// Disonnect from Power Call Server
	m_sckPCServer.WaitForShutdownToComplete();

#ifdef INTEL_DIALOGIC

	// Stop the GC Libraries
	///Issue gc_Stop() before exiting the process, but only when gc_Start has already been called
	if( m_bGCStarted )
	{
		gc_Stop();
		TRACE("gc_Stop!!!\n");
	}

#endif

	// Stop Threads
#ifdef INTEL_DIALOGIC
	/// Stop GC events processing thread
	g_thProcessGCEvents.WaitForShutdownToComplete();
#endif

#ifdef CISCO_CCM

#ifdef CUCM_MEDIA
		
		// Sun added 2013-03-06
		m_MediaDriver.CloseAPI();

#endif

	g_thProcessTAPIEvents.SleepAll();
	if( gb_hLineApp ) 
		lineShutdown(gb_hLineApp);
#endif

	MsgPrintf(TraceInfo, "Power Voice stoped successfully!");
		
	return(1);
}

// Sun added 2008-05-19
int	CMainFrame::Sys_ReInitDevices()
{
	MsgPrintf(TraceWarn, "Power Voice will reinitializing devices!");

#ifdef CISCO_CCM
	
	g_thProcessTAPIEvents.SleepAll();
	if( gb_hLineApp ) 
		lineShutdown(gb_hLineApp);

	int		lv_loop;

	// Stop All Groups
	for( lv_loop = 0; lv_loop < theApp.m_nIVRGroups; lv_loop++ )
	{
		if( PIVRGroup[lv_loop] != NULL )
			PIVRGroup[lv_loop]->StopGroup();
	}

	Sleep(1000);

	// Sun added 2008-06-26 [C]
	theApp.m_nIVRChannels = theApp.GetProfileInt("WORKINFO", "IVRCHANNELS", 0);
	if( theApp.m_nIVRChannels > MAX_IVR_CHANNEL )
		theApp.m_nIVRChannels = MAX_IVR_CHANNEL;

// Sun added 2013-04-25
#ifndef SMARTAC_LICENSE
	if( theApp.m_nIVRChannels > theApp.m_License.GetLicenseClient(PCS_LIC_IVR_CHANNEL) )
		theApp.m_nIVRChannels = theApp.m_License.GetLicenseClient(PCS_LIC_IVR_CHANNEL);
#else
	if( theApp.m_nIVRChannels > SMARTAC_LICENSE->GetLicenseClient(PCS_LIC_IVR_CHANNEL) )
		theApp.m_nIVRChannels = SMARTAC_LICENSE->GetLicenseClient(PCS_LIC_IVR_CHANNEL);
#endif
	// End of 2008-06-26 [C]

	// Detect Hardware
	if( Sys_InitializePhysicalDev() )
	{
		// Open Channel
		for( lv_loop = 0; lv_loop < theApp.m_nIVRChannels; lv_loop++ )
		{
			if( !PIVRCH[lv_loop]->OpenCh() )
				MsgPrintf(TraceError, "Error Opening channel => channel name : %s", PIVRCH[lv_loop]->m_szChannelName);
			else
				MsgPrintf(TraceInfo, "Channel opened => channel name : %s", PIVRCH[lv_loop]->m_szChannelName);
		}
	}
	else
		return 0;

	// Sun added 2008-06-26 [B]
	if( m_nMainTimerID > 0 )
	{
		KillTimer(m_nMainTimerID);
		m_nMainTimerID = 0;
	}

	g_thProcessTAPIEvents.WakeUp();
	m_bRunning = TRUE;		// Must set before start group

	// Start All Groups and Channels
	for( lv_loop = 0; lv_loop < theApp.m_nIVRGroups; lv_loop++ )
	{
		if( PIVRGroup[lv_loop] != NULL )
		{
			if( !PIVRGroup[lv_loop]->StartGroup(TRUE) )
				MsgPrintf(TraceError, "Error Starting Group #%d named %s", lv_loop, PIVRGroup[lv_loop]->GetCaption());
			// Sun added 2008-06-26 [B], 重新初始化后，确保IVR通道注册在CTI上
			else if( GetCTIOpened() )
				PIVRGroup[lv_loop]->InitializeChannels();
			// End of 2008-06-26 [B]
		}
	}

	// Sun added 2008-06-26 [B]
	if( m_nMainTimerID == 0 )
		m_nMainTimerID = SetTimer(1, m_main_timer * 1000, NULL);

#endif

	return 1;
}

// Sun added 2005-03-17
BOOL CMainFrame::Sys_ConnectEXTServer()
{
	try
	{
		// Open External Server connection
		if( m_blnEnableEXT && m_param_extport > 0 )
			if( m_sckExternalServer.WaitForStartupToComplete(m_param_extip, m_param_extport, this->m_hWnd) )
				return TRUE;
	}
	catch(...)
	{
	}

	return FALSE;
}

// 连接主PCS Server
BOOL CMainFrame::Sys_ConnectCTIServer()
{
	BOOL lv_retval = FALSE;

	try
	{
		// Open CTI Server connection
		if( m_param_ctiport > 0 )
		{
			if( m_sckPCServer.WaitForStartupToComplete(m_param_ctiip, m_param_ctiport, this->m_hWnd) )
				lv_retval = TRUE;
			else
				WriteTrace(TraceError, "Can't connect to server at %s:%d, please check it!", m_param_ctiip, m_param_ctiport);
		}
				
		// Sun removed 2012-12-29
		/*
		// Sun added 2010-05-18 [V7.1.8.3]
		/// 连接第二PCS Server
		if( m_param_cti2ip.GetLength() > 0 && m_param_cti2port > 0 )
		{
			if( m_sckPCServer2.WaitForStartupToComplete(m_param_cti2ip.GetBuffer(m_param_cti2ip.GetLength()), m_param_cti2port, this->m_hWnd) )
				lv_retval = TRUE;
			else
				WriteTrace(TraceError, "Can't connect to server2 at %s:%d, please check it!", m_param_cti2ip, m_param_cti2port);
		}
		*/

		return lv_retval;
	}
	catch(...)
	{
	}

	WriteStatusBar(STATUS_BAR_COMM, ICON_Error);
	return FALSE;
}

// Sun added 2012-12-29
/// 连接第二PCS Server
BOOL CMainFrame::Sys_ConnectSecondCTIServer()
{
	BOOL lv_retval = FALSE;

	try
	{
		// Sun added 2010-05-18 [V7.1.8.3]
		/// 连接第二PCS Server
		if( m_param_cti2ip.GetLength() > 0 && m_param_cti2port > 0 )
		{
			if( m_sckPCServer2.WaitForStartupToComplete(m_param_cti2ip.GetBuffer(m_param_cti2ip.GetLength()), m_param_cti2port, this->m_hWnd) )
				lv_retval = TRUE;
			else
				WriteTrace(TraceError, "Can't connect to server2 at %s:%d, please check it!", m_param_cti2ip, m_param_cti2port);
		}

		return lv_retval;
	}
	catch(...)
	{
	}

	WriteStatusBar(STATUS_BAR_COMM, ICON_Error);
	return FALSE;
}

BOOL CMainFrame::Sys_ConnectDataRecord()
{
	try
	{
		// Open data recorder connection
		if( m_param_drport > 0 )
			if( m_sckDataRecorder.WaitForStartupToComplete(m_param_drip, m_param_drport, this->m_hWnd) )
				return TRUE;
	}
	catch(...)
	{
	}

	WriteStatusBar(STATUS_BAR_DB, ICON_Error);
	return FALSE;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
BOOL CMainFrame::Sys_AutoDetectVoiceDev()
{
#ifdef INTEL_DIALOGIC

	int		i, j;

	try
	{
		long	nBoardhWnd=0;
		int		nBoardCnt=0, nDTIBoardCnt=0;
		int		nChannelCnt=0;
		char	szBoardName[12];
		
		if( !gb_NT_UserGlobalCall )
		{
			// Set Call Back Function
			sr_NotifyEvent(pChannelView->m_hWnd, MYWM_IVREVENT, SR_NOTIFY_ON);
		}

		// get voice board count
		sr_getboardcnt(DEV_CLASS_VOICE, &nBoardCnt);
		theApp.m_nIVRBoardCount = nBoardCnt;
		if( nBoardCnt <= 0 )
		{
			MsgPrintf(TraceError, "No voice boards detected...");
			//AfxMessageBox("没有检测到语音设备，请检查硬件设置", MB_ICONEXCLAMATION | MB_OK);
			WriteStatusBar(STATUS_BAR_HARDWARE, ICON_Error);
			return FALSE;
		}
		else
			MsgPrintf(TraceInfo, "Voice board(s) is detected, total span(s): %d.", nBoardCnt);

		// get dti board count
		sr_getboardcnt(DEV_CLASS_DTI, &nDTIBoardCnt);
		if( nDTIBoardCnt > 0 )
			MsgPrintf(TraceInfo, "DTI board(s) is detected, total span(s): %d.", nDTIBoardCnt);
		
		// get channel name
		int lv_nIVRSpan = 1;		// Sun added 2012-06-19
		int lv_nIVRSpanCh = 0;		// Sun added 2012-06-19
		for( j=theApp.m_nIVRBoardStart; j<=nBoardCnt; j++ )
		{
			// Sun added 2012-06-19
			/// dxxx可能是语音，也可能是传真，而且不是连续的，所以要跳过一些
			if( theApp.m_blnIVRFaxAlternant )
			{
				if( theApp.m_nIVRChannelSPAN > 0 )
				{
					if( lv_nIVRSpanCh >= theApp.m_nIVRChannelSPAN )
					{
						lv_nIVRSpan++;
						MsgPrintf(TraceInfo, "Jump to the next IVR Resource span: %d, this span contains %d channels.", lv_nIVRSpan, lv_nIVRSpanCh);
						lv_nIVRSpanCh = 0;
						j = j + (int)((theApp.m_nIVRChannelSPAN-1) / 4 + 1);
						if( j > nBoardCnt )
							break;
					}
				}
			}

			sprintf(szBoardName, "dxxxB%d", j);
			if( (nBoardhWnd = dx_open(szBoardName, 0)) == -1 )
				return FALSE;
			theApp.m_hBoardHandles[j] = nBoardhWnd;

			/// Sun added 2004-11-20
			if( dx_setparm(nBoardhWnd, DXBD_FLASHTM, &theApp.m_nAnalogFlashTM) == -1 )
				MsgPrintf(TraceError, "Error: dx_setparm(DXBD_FLASHTM, %d) for board %s!!!", theApp.m_nAnalogFlashTM, szBoardName);

			char **szChannName = ATDX_CHNAMES(nBoardhWnd);

			for( i=0; i<ATDV_SUBDEVS(nBoardhWnd); i++ )
			{
				if( nChannelCnt >= theApp.m_nIVRChannels )
					break;

				if( PIVRCH[nChannelCnt]!= NULL )
				{
					memset (PIVRCH[nChannelCnt]->m_szChannelName, 0x00, sizeof(PIVRCH[nChannelCnt]->m_szChannelName));
					strcpy (PIVRCH[nChannelCnt]->m_szChannelName, *(szChannName+i));
					lv_nIVRSpanCh++;		// Sun added 2012-06-19
				}
				else
					break;		// Exit For Loop

				// Sun added 2012-05-24
				/// Name Fax Channel
				memset (PIVRCH[nChannelCnt]->m_szFaxName, 0x00, sizeof(PIVRCH[nChannelCnt]->m_szFaxName));
				if( theApp.m_nFaxBoardStart > 0 )
				{
					if( theApp.m_nFaxBoardStart == theApp.m_nIVRBoardStart )
					{
						strcpy (PIVRCH[nChannelCnt]->m_szFaxName, PIVRCH[nChannelCnt]->m_szChannelName);
					}
					else if( theApp.m_nFaxBoardStart + j - theApp.m_nIVRBoardStart > 0 )
					{
						sprintf (PIVRCH[nChannelCnt]->m_szFaxName, "dxxxB%dC%d", theApp.m_nFaxBoardStart + j - theApp.m_nIVRBoardStart, i+1);
					}
				}

				// Sun fixed 2012-05-24
				// Sun added to support DTI Cards
				/// Note: If DTI and Voice Cards exist in a same system,
				//// DTI card(s) must be placed ahead of Voice Card(s).
				if( nDTIBoardCnt > 0 && theApp.m_nDTIBoardStart > 0 )
				{
					if( nChannelCnt + (theApp.m_nDTIBoardStart - 1) * 30 < nDTIBoardCnt * 30 )
					{
						memset (PIVRCH[nChannelCnt]->m_szDTIName, 0x00, sizeof(PIVRCH[nChannelCnt]->m_szDTIName));
						sprintf(PIVRCH[nChannelCnt]->m_szDTIName, "dtiB%dT%d", 
							nChannelCnt / 30 + theApp.m_nDTIBoardStart, nChannelCnt % 30 + 1);
					}
				}

				nChannelCnt++;
			}

			if( nChannelCnt >= theApp.m_nIVRChannels )
				break;
		}

		if( nChannelCnt < theApp.m_nIVRChannels )
		{
			// Sun added 2002-03-01
			for( i = nChannelCnt; i < theApp.m_nIVRChannels; i++ )
			{
				if( PIVRCH[i] != NULL )
				{
					delete(PIVRCH[i]);
					PIVRCH[i] = NULL;
				}
			}
			theApp.m_nIVRChannels = nChannelCnt;
		}
	}
	catch(...)
	{
		WriteStatusBar(STATUS_BAR_HARDWARE, ICON_Error);
		return FALSE;
	}

#endif
	
#ifdef CISCO_CCM

	try
	{
		int lv_loop;
		long nRet=0;
		DWORD	dwChannelCnt=0;						// 通道数量
		DWORD	dwSize;
		DWORD	dwRoutePointID;
		CString sLineName, sUpperLineName, sTemp;
		CList<CString,CString&> lv_lstLines;
		POSITION lv_pos, lv_posTemp;
		int		lv_nActualLineCnt, lv_nPos, lv_nPos2;
		UL		lv_nLineID;

		// To be able to control loops. It will keep track of the numbers
		// a loop continued, so after a maximum, we are in trouble=> break
		int		patience = 0;
		// Working with memory in TAPI is a hell!
		BOOL	noMem;

		// Initialize the line, (~ 200 lines)
		LINEINITIALIZEEXPARAMS   *lineParams = 
		(LINEINITIALIZEEXPARAMS *) calloc(1,
									sizeof(LINEINITIALIZEEXPARAMS));
		memset(lineParams, NULL, sizeof(LINEINITIALIZEEXPARAMS));

		lineParams->dwTotalSize =   sizeof(LINEINITIALIZEEXPARAMS);
		lineParams->dwOptions   =   LINEINITIALIZEEXOPTION_USEEVENT;

		gb_hLineApp = 0;
		gb_dwgTAPIVersion = TAPI_CURRENT_VERSION;
		gb_lMngrThread = NULL;
		gb_hLineEvent = NULL;

		// Try to initilize line
		// We need a loop untill needed memory is alocated
		do 
		{
			// We have not memory shortage yet
			noMem = FALSE;

			// First TAPI call in the sequence
			nRet = lineInitializeEx(
				&gb_hLineApp,
				AfxGetInstanceHandle(),
				NULL,
				THEAPP_NAME,
				&dwChannelCnt,
				&gb_dwgTAPIVersion,
				lineParams );

			// Didn't Itell you about memory hell? Here it starts
			// If more memory is needed, Realocate
			// Re alocate memory if more is needed
			if( lineParams->dwNeededSize > lineParams->dwTotalSize )
			{
				int needed = lineParams->dwNeededSize;
				noMem = TRUE;
				free(lineParams);
				lineParams = NULL;

				lineParams = (LINEINITIALIZEEXPARAMS *) calloc(1, needed);
				if( !lineParams )
				{
					WriteTrace(TraceError, _T("TAPI Error - Can't alocate memory for lineInitializeEx"));
					WriteStatusBar(STATUS_BAR_HARDWARE, ICON_Error);
					return FALSE;
				}

				memset(lineParams, NULL, needed);
				lineParams->dwOptions = LINEINITIALIZEEXOPTION_USEEVENT;
				lineParams->dwTotalSize = needed;
			}

			// If this loop ran 1000 times!!, there must be a problem
			if( ++patience > 1000 )
			{
				if( lineParams )
					free(lineParams);

				WriteTrace(TraceError, _T("TAPI Error - Recovered from unlimited loop in lineInitializeEx"));
				WriteStatusBar(STATUS_BAR_HARDWARE, ICON_Error);
				return FALSE;
			};

			// Sleep 5 mili seconds and re run the loop again
			Sleep(5);
			// Repeat untill everythings go right
		} while( (nRet == LINEERR_REINIT) || (noMem == TRUE) );

		gb_hLineEvent = (lineParams->Handles.hEvent);
		// Free unused memory
		if (lineParams) free(lineParams);
		MsgPrintf(TraceInfo, "Initialize TAPI successfully, TAPI version: 0x%08X", gb_dwgTAPIVersion);

		// 枚举CISCO TSP线路
		lv_lstLines.RemoveAll();
		for( DWORD i = 0;i < dwChannelCnt; i++ )
		{
			LINEEXTENSIONID lineExten;
			ZeroMemory(&lineExten, sizeof(lineExten));
			DWORD dwApiVer=0;
			nRet = lineNegotiateAPIVersion(gb_hLineApp, i, 0x00010003, 0x00030000, &dwApiVer, &lineExten);
			outputError(nRet);
			if( nRet == 0 )
			{
				LPLINEDEVCAPS pDev = new LINEDEVCAPS;
				pDev->dwTotalSize = sizeof(LINEDEVCAPS);
				nRet = lineGetDevCaps(gb_hLineApp, i, dwApiVer, 0, pDev);
				outputError(nRet);
				if( pDev->dwNeededSize > pDev->dwTotalSize )
				{
					dwSize = pDev->dwNeededSize;
					delete pDev;
					pDev = (LPLINEDEVCAPS)new BYTE[dwSize];
					pDev->dwTotalSize = dwSize;				
					nRet = lineGetDevCaps(gb_hLineApp, i, dwApiVer, 0, pDev);
				}
				if( nRet == 0 )
				{
					sLineName = (LPCTSTR)((LPBYTE)pDev + pDev->dwLineNameOffset);
					WriteTrace(TraceDebug, "Debug - Enum get TAPI line: %d - %s, media mode: 0x%08X", i, sLineName, pDev->dwMediaModes);

					// 只采用Cisco CTI Port设备
					/// Line Device
					if( (pDev->dwMediaModes & (LINEMEDIAMODE_AUTOMATEDVOICE | LINEMEDIAMODE_INTERACTIVEVOICE)) == (LINEMEDIAMODE_AUTOMATEDVOICE | LINEMEDIAMODE_INTERACTIVEVOICE) )
					{
						sUpperLineName = sLineName;
						sUpperLineName.MakeUpper();
						if( sUpperLineName.Find("CISCO LINE") >= 0 )
						{
							// Sun added 2010-06-25, Cisco Line: [IVR001] (1001)
							/// Sort by DN
							lv_nPos = sUpperLineName.Find("(");
							lv_nPos2 = sUpperLineName.Find(")");
							if( lv_nPos > 0 && lv_nPos2 > lv_nPos )
							{
								sTemp = sUpperLineName.Mid(lv_nPos+1, lv_nPos2-lv_nPos-1);
								sUpperLineName.Format("DN%s-%s:%d", sTemp, sLineName, i);
							}
							else
								sUpperLineName.Format("%s:%d", sLineName, i);
							
							// Sort the List
							lv_pos = lv_lstLines.GetHeadPosition();
							while( lv_pos != NULL )
							{
								lv_posTemp = lv_pos;
								sTemp = lv_lstLines.GetNext(lv_pos);
								if( sTemp > sUpperLineName )
								{
									lv_lstLines.InsertBefore(lv_posTemp, sUpperLineName);
									lv_pos = lv_posTemp;
									break;
								}
								//-----------------------------------
								// Sun added 2009-04-13
								/// 避免重复设备名多次监控
								else if( sTemp == sUpperLineName )
								{
									lv_pos = lv_posTemp;
									break;
								}
								//-----------------------------------
							}
							if( lv_pos == NULL )
								lv_lstLines.AddTail(sUpperLineName);
						}
					}
					/// Route Point Device
					else if( pDev->dwMediaModes == LINEMEDIAMODE_INTERACTIVEVOICE )
					{
						sUpperLineName = sLineName;
						sUpperLineName.MakeUpper();
						if( sUpperLineName.Find("CISCO LINE") >= 0 )
						{
							/// 获取Route Point ID
							lv_nPos = sUpperLineName.Find("(");
							lv_nPos2 = sUpperLineName.Find(")");
							if( lv_nPos > 0 && lv_nPos2 > lv_nPos )
							{
								dwRoutePointID = (DWORD)atol(sUpperLineName.Mid(lv_nPos+1, lv_nPos2-lv_nPos-1));

								for( lv_loop = 0; lv_loop < theApp.m_nIVRGroups; lv_loop++ )
								{
									if( PGroupInfo[lv_loop].dwRoutePointID == dwRoutePointID )
									{
										PIVRGroup[lv_loop]->SetCTIRoutePointID(dwRoutePointID);
										PIVRGroup[lv_loop]->Init(i);

										// Sun added 2008-05-23
										PIVRGroup[lv_loop]->SetDeviceName(sLineName);

										continue;
									}
								}
							}
						}
					}
				}
				delete pDev;
			}
		}

		lv_nActualLineCnt = lv_lstLines.GetCount();
		lv_pos = lv_lstLines.GetHeadPosition();
		dwChannelCnt = 0;
		while( lv_pos != NULL && dwChannelCnt < theApp.m_nIVRChannels )
		{
			// 分解线路ID和线路名
			sUpperLineName = lv_lstLines.GetNext(lv_pos);
			// Sun added 2010-06-25, Cisco Line: [IVR001] (1001)
			sTemp = sUpperLineName;
			lv_nPos = sTemp.Find("CISCO LINE");
			if( lv_nPos > 0 )
			{
				sUpperLineName = sTemp.Mid(lv_nPos);
			}

			lv_nPos = sUpperLineName.ReverseFind(':');
			if( lv_nPos > 0 )
			{
				sLineName = sUpperLineName.Left(lv_nPos);
				lv_nLineID = (UL)atol(sUpperLineName.Mid(lv_nPos + 1));

				PIVRCH[dwChannelCnt]->m_oARS.Init(lv_nLineID, dwChannelCnt);
				memset(PIVRCH[dwChannelCnt]->m_szChannelName, 0x00, sizeof(PIVRCH[dwChannelCnt]->m_szChannelName));
				strncpy(PIVRCH[dwChannelCnt]->m_szChannelName, (LPCSTR)sLineName, 50);

				// Sun added 2008-05-23
				PIVRCH[dwChannelCnt]->m_oARS.SetDeviceName(sLineName);

				/// 获取分机号
				lv_nPos = sLineName.Find("(");
				lv_nPos2 = sLineName.Find(")");
				if( lv_nPos > 0 && lv_nPos2 > lv_nPos )
				{
					sTemp = sUpperLineName.Mid(lv_nPos+1, lv_nPos2-lv_nPos-1);
					PIVRCH[dwChannelCnt]->SetExtensionNo(sTemp);
					WriteTrace(TraceInfo, "IVR Channel: %d DeviceNo: %s ExtensionNo: %s", dwChannelCnt, PIVRCH[dwChannelCnt]->GetDeviceNo(), sTemp);
				}

				dwChannelCnt++;
			}
		}

		if( lv_nActualLineCnt < theApp.m_nIVRChannels )
		{
			for( i = lv_nActualLineCnt; i < theApp.m_nIVRChannels; i++ )
			{
				if( PIVRCH[i] != NULL )
				{
					delete(PIVRCH[i]);
					PIVRCH[i] = NULL;
				}
			}
			theApp.m_nIVRChannels = lv_nActualLineCnt;
		}

#ifdef CUCM_MEDIA
		
		// Sun added 2013-03-06
		//bool lv_epRet = EpApiInitDefault(NULL);
		m_MediaDriver.InitAPI();
			
#endif

		MsgPrintf(TraceInfo, "Successfully enum IVR lines total %d and %d on working", lv_nActualLineCnt, theApp.m_nIVRChannels);
	}
	catch(...)
	{
		WriteStatusBar(STATUS_BAR_HARDWARE, ICON_Error);
		return FALSE;
	}

#endif

	WriteStatusBar(STATUS_BAR_HARDWARE, ICON_Ready);
	return TRUE;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
int CMainFrame::Sys_InitializePhysicalDev()
{

#ifdef INTEL_DIALOGIC
	
	int lv_mode;

	// Set the device to polled mode
	lv_mode = SR_POLLMODE | SR_STASYNC;
	if (sr_setparm(SRL_DEVICE, SR_MODEID, &lv_mode) == -1) 
		return(0);

#endif

	// Initialize analog devices [HWDP]
	if(	!Sys_AutoDetectVoiceDev() )
		return(0);

#ifdef INTEL_DIALOGIC

	// Start the GC Libraries
	if( gb_NT_UserGlobalCall )
	{
		if( gc_Start(NULL) == -1 )
		{
			// Failed to start the libraries
			MsgPrintf(TraceError, "Error Starting Global Call Libraies(gc_Start())!");
			return(0);
		}

		// Sun added 2012-05-24
		GC_PARM_BLK			  *parmblkp = NULL;
		gc_util_insert_parm_val(&parmblkp, GCSET_CHAN_CAPABILITY, GCPARM_CAPABILITY, 
			sizeof(int), GCCAP_AUDIO_g711Alaw64k);

		if( gc_SetAlarmNotifyAll(0, ALARM_SOURCE_ID_SPRINGWARE_E1, ALARM_NOTIFY) == -1 )
		//if( gc_SetAlarmNotifyAll(0, ALARM_SOURCE_ID_NETWORK_ID, ALARM_NOTIFY) == -1 )
		{
			// Failed to start the libraries
			MsgPrintf(TraceError, "Fail to enable Global Call Notification(gc_SetAlarmNotifyAll())!");
			return(0);
		}

		m_bGCStarted = true;
	}

#endif

	return(1);
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Create IVR Objects as well as IVR Group Objects
// Create threads for IVR Channels
int CMainFrame::Sys_CreateWorkObjects(void)
{
	int lv_loop;					// 循环变量
	
	try
	{
		// Create IVR Group Objects
		for(lv_loop = 0; lv_loop < theApp.m_nIVRGroups; lv_loop++)
		{
			PIVRGroup[lv_loop] = new CIVRGroup;
			PIVRGroup[lv_loop]->Create(lv_loop);

//-----------------------------------------------------------------
// Sun added 2010-03-08 for Cisco CME
/// Cisco CME Interface
#ifdef CISCO_CME
			PIVRGroup[lv_loop]->SetCTIRoutePointID(PGroupInfo[lv_loop].dwRoutePointID);
#endif
// End of Sun added 2010-03-08
//-----------------------------------------------------------------

		}
		
		// Create IVR Channel Objects
		for(lv_loop = 0; lv_loop < theApp.m_nIVRChannels; lv_loop++)
		{
			PIVRCH[lv_loop] = new CIVRChannel;
			PIVRCH[lv_loop]->Create(lv_loop);
			PIVRCH[lv_loop]->SetMaxPlayTime(theApp.m_nMaxSoundTime);
			PIVRCH[lv_loop]->SetMaxQueueTime(theApp.m_nMaxWaitTime);
			PIVRCH[lv_loop]->SetMaxTryTimes(theApp.m_nMaxTryTimes);

			// Sun added 2006-10-01
			PIVRCH[lv_loop]->SetMaxFaxTime(theApp.m_nMaxFaxTime);

//-----------------------------------------------------------------
// Sun added 2010-03-08 for Cisco CME
/// Cisco CME Interface
#ifdef CISCO_CME
			sprintf(PIVRCH[lv_loop]->m_szChannelName, "CME_S%03d_C%04d", theApp.m_nSysNo, lv_loop+1);
#endif
// End of Sun added 2010-03-08
//-----------------------------------------------------------------

			// Sun added 2002-03-15
			PIVRCH[lv_loop]->SetDeviceNo(PChannelDN[lv_loop]);
			m_CTIAgentList.AddAgent(atol(PChannelDN[lv_loop]), PIVRCH[lv_loop]);
		}

		// Detect Hardware
		if( Sys_InitializePhysicalDev() )
		{
			// Open Channel
			for( lv_loop = 0; lv_loop < theApp.m_nIVRChannels; lv_loop++ )
			{
				if( !PIVRCH[lv_loop]->OpenCh() )
					MsgPrintf(TraceError, "IVR Channel: %d - Error Opening channel => channel name : %s", lv_loop, PIVRCH[lv_loop]->m_szChannelName);
				else
					MsgPrintf(TraceInfo, "IVR Channel: %d - Channel opened => channel name : %s", lv_loop, PIVRCH[lv_loop]->m_szChannelName);
				PIVRCH[lv_loop]->NF_SetChannelRestartTime(theApp.m_nChannelRestartTime);
				PIVRCH[lv_loop]->NF_SetCallAnswerDelay(theApp.m_nCallAnswerDelay);
			}
		}
		else
			return 0;

		// Start All Groups and Channels
		for( lv_loop = 0; lv_loop < theApp.m_nIVRGroups; lv_loop++ )
		{
			if( PIVRGroup[lv_loop] != NULL )
				if( !PIVRGroup[lv_loop]->StartGroup() )
					MsgPrintf(TraceError, "Error Starting Group #%d named %s", lv_loop, PIVRGroup[lv_loop]->GetCaption());
		}
	}
	catch(...)
	{
		MsgPrintf(TraceError, "Fatal Error - When Create System Objects!!!");
		return 0;
	}

	return(1);
}
//----------------------------------------------------------------------------------------------

void CMainFrame::MsgPrintf(UINT nTraceLevel, char *fmt, ...)
{
	va_list		ap;
	char		Msgbuff[2048];

	memset (Msgbuff, 0x00, sizeof(Msgbuff));

    va_start(ap, fmt);
    vsprintf(Msgbuff, fmt, ap);
    va_end(ap);

	WriteTrace(nTraceLevel, Msgbuff);
}

#ifdef CISCO_CCM
LONG CMainFrame::outputError(LONG nErrorID)
{
	if( nErrorID >= 0 )
		return nErrorID;

	char pBuf[1024]="";
	HMODULE hDll= GetModuleHandle("TAPIUI.DLL");

	if( !hDll )
	{
		WriteTrace(TraceError, "TSP System Error: 0x%08X", GetLastError());
		return nErrorID;
	}

	FormatMessage(FORMAT_MESSAGE_FROM_HMODULE,
		(LPCVOID)hDll,
		TAPIERROR_FORMATMESSAGE(nErrorID),
		0,
		(LPTSTR)pBuf,
		1024,
		NULL);

	WriteTrace(TraceError, "TSP Error: [0x%08] %s", nErrorID, pBuf);

	return nErrorID;
}
#endif

void CMainFrame::OnViewAllCh() 
{
	// TODO: Add your command handler code here
	CMenu *lv_MainMenu = GetMenu();
	m_bShowAllChannel = !m_bShowAllChannel;
	if( m_bShowAllChannel )
	{
		lv_MainMenu->CheckMenuItem(ID_VIEW_ALL_CH, MF_CHECKED);
	}
	else
	{
		lv_MainMenu->CheckMenuItem(ID_VIEW_ALL_CH, MF_UNCHECKED);
	}
	CToolBarCtrl& lv_ToolBarCtrl = m_wndToolBar.GetToolBarCtrl();
	lv_ToolBarCtrl.PressButton(ID_VIEW_ALL_CH, m_bShowAllChannel);
	pChannelView->Screen_ShowIVRChannelInfo(PIVRGroup[pGroupView->GetShowGroupNo()]);
}

void CMainFrame::OnViewReport() 
{
	// TODO: Add your command handler code here
	CMenu *lv_MainMenu = GetMenu();
	m_bCompactChannelView = !m_bCompactChannelView;
	if( m_bCompactChannelView )
	{
		lv_MainMenu->CheckMenuItem(ID_VIEW_REPORT, MF_CHECKED);
	}
	else
	{
		lv_MainMenu->CheckMenuItem(ID_VIEW_REPORT, MF_UNCHECKED);
	}
	pChannelView->Screen_ChangeViewMode(m_bCompactChannelView);
}

void CMainFrame::OnUpdateExeRestart(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bME_EXE_RESTART && m_bRunning );
}

void CMainFrame::OnUpdateExeRestartAll(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bME_EXE_RESTART_ALL && m_bRunning );
}

void CMainFrame::OnUpdateExeStart(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bME_EXE_START && m_bRunning );
}

void CMainFrame::OnUpdateExeStartAll(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bME_EXE_START_ALL && m_bRunning );
}

void CMainFrame::OnUpdateExeStop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bME_EXE_STOP && m_bRunning );
}

void CMainFrame::OnUpdateExeStopAll(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bME_EXE_STOP_ALL && m_bRunning );
}

void CMainFrame::OnUpdateFileConfig(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bME_FILE_CONFIG);
}

void CMainFrame::OnUpdateFileLoadDb(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bME_FILE_LOAD_DB && m_bRunning );
}

void CMainFrame::OnUpdateFileLoadFile(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bME_FILE_LOAD_FILE && m_bRunning );
}

void CMainFrame::OnUpdateFileLoadResource(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bME_FILE_LOAD_RESOURCE && m_bRunning );
}

// Select and reload resource table
void CMainFrame::OnFileLoadResource() 
{
	// TODO: Add your command handler code here
	BOOL lv_retval;
	int f_GroupNo;
	CDLGSELERESOURCE lv_dlg;
	CIVRGroup *lv_pGroup;
	US lv_oldResID;
	
	f_GroupNo = pGroupView->GetShowGroupNo();
	// Is No. right ?
	if( f_GroupNo >= 0 && f_GroupNo < theApp.m_nIVRGroups )
	{
		// Is object available ?
		if( (lv_pGroup = PIVRGroup[f_GroupNo]) != NULL )
		{
			lv_dlg.m_nselectitem = 0;
			lv_oldResID = lv_pGroup->GetResourcePrjID();
			lv_dlg.m_nselectitemdata = lv_oldResID;
			if( lv_dlg.DoModal() == IDOK )
			{
				if( lv_dlg.m_nselectitem >= 0 )
				{
					// Load Waiting Cursor
					HCURSOR hOldCur = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));

					// Reload Resource Project
					lv_retval = lv_pGroup->LoadResourceMap(lv_dlg.m_nselectitemdata);
					
					// Resume Cursor
					::SetCursor(hOldCur);

					// Succeed?
					if( lv_retval )
					{
						// Update System Node Data
						lv_pGroup->SetResourcePrjID(lv_dlg.m_nselectitemdata);
						AfxMessageBox(IDS_MSG_M_LOADRES_OK, MB_ICONINFORMATION | MB_OK);
					}
					else
						AfxMessageBox(IDS_MSG_M_LOADRES_FAIL, MB_ICONEXCLAMATION | MB_OK);
				}
			}
		}
	}
}

void CMainFrame::OnUpdateViewAllCh(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bME_VIEW_ALL_CH);
}

void CMainFrame::RecalcLayout(BOOL bNotify) 
{
	// TODO: Add your specialized code here and/or call the base class
	CFrameWnd::RecalcLayout(bNotify);

	if( m_bRunning )
		SB_PositionControl(&m_wndProgressBar, STATUS_BAR_PROGRESS);	
}

//----------------------------------------------------------------------------------------------
// Send TCP message functions
//----------------------------------------------------------------------------------------------
// Sun updated 2010-05-18 [V7.1.8.3]
BOOL CMainFrame::MQ_HA_SendMsg(const BYTE *pData, size_t dataLength, BOOL blnTwoWay)
{
	BOOL lv_retval = FALSE;

	// 兼容老版本
	if( m_param_cti2ip.GetLength() == 0 || m_param_cti2port == 0 )
	{
		return m_sckPCServer.SendControlData(pData, dataLength);
	}

	/// 确保先发给Active PCS
	if( m_sckPCServer.GetPCSStatus() == PCS_PS_Active )
	{
		lv_retval = m_sckPCServer.SendControlData(pData, dataLength);
		if( blnTwoWay )
			if( m_sckPCServer2.GetPCSStatus() <= PCS_PS_Standby )
				m_sckPCServer2.SendControlData(pData, dataLength);
	}
	else if( m_sckPCServer2.GetPCSStatus() == PCS_PS_Active )
	{
		lv_retval = m_sckPCServer2.SendControlData(pData, dataLength);
		if( blnTwoWay )
			if( m_sckPCServer.GetPCSStatus() <= PCS_PS_Standby )
				m_sckPCServer.SendControlData(pData, dataLength);
	}
	else
	{
		// 没有Active PCS
		if( blnTwoWay )
		{
			// 有链接就发送
			if( m_sckPCServer.GetPCSStatus() <= PCS_PS_Standby )
				lv_retval = m_sckPCServer.SendControlData(pData, dataLength);
			if( m_sckPCServer2.GetPCSStatus() <= PCS_PS_Standby )
				lv_retval = (m_sckPCServer2.SendControlData(pData, dataLength) || lv_retval);
		}
		else
		{
			// 只发给第一个Standby PCS
			if( m_sckPCServer.GetPCSStatus() == PCS_PS_Standby )
			{
				lv_retval = m_sckPCServer.SendControlData(pData, dataLength);
			}
			else if( m_sckPCServer2.GetPCSStatus() == PCS_PS_Standby )
			{
				lv_retval = m_sckPCServer2.SendControlData(pData, dataLength);
			}
		}
	}

	return lv_retval;
}

// Sun added 2010-05-18 [V7.1.8.3]
/// 判定Active PCS
BYTE CMainFrame::GetActivePCSID()
{
	BYTE lv_ID = -1;
	PCS_PCSStatus lv_st1, lv_st2;

	lv_st1 = m_sckPCServer.GetPCSStatus();
	lv_st2 = m_sckPCServer2.GetPCSStatus();

	if( lv_st1 == PCS_PS_Active )
		lv_ID = m_sckPCServer.GetPCSIndex();
	else if( lv_st2 == PCS_PS_Active )
		lv_ID = m_sckPCServer2.GetPCSIndex();
	else if( lv_st1 <= PCS_PS_Standby )
		lv_ID = m_sckPCServer.GetPCSIndex();
	else if( lv_st2 <= PCS_PS_Standby )
		lv_ID = m_sckPCServer2.GetPCSIndex();

	return lv_ID;
}

BOOL CMainFrame::MQ_SendMsg_Register(DWORD dwThreadID)
{
	BOOL lv_retval;

	//-------------------------------
	BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];
	SMsgHeader lv_sndhdr;

	// Sun added 2010-05-18 [V7.1.8.3]
	memset(&lv_sndhdr, 0x00, sizeof(lv_sndhdr));

	lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
	lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
	lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
	lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
	lv_sndhdr.Sender = USER_IVR;
	lv_sndhdr.Receiver = USER_CTI;

	//-------------------------------
	S_TCP_IVR2PCS_REGISTER lv_sndbdy;
	
	lv_sndhdr.Command = TCP_IVR2PCS_REGISTER;
	lv_sndhdr.PackageLen = sizeof(SMsgHeader) + sizeof(lv_sndbdy);

	lv_sndbdy.SysNo = theApp.m_nSysNo;
	lv_sndbdy.GroupCount = theApp.m_nIVRGroups;
	lv_sndbdy.ChannelCount = theApp.m_nIVRChannels;
	lv_sndbdy.AutoLogout = (UC)theApp.m_blnAutoChLogout;

	//-------------------------------
	TCHAR	seps[]   = " .,ces";
	TCHAR*	lpName;
	TCHAR*	token;

	CString lv_sVersion = theApp.m_sVersionData;
	lv_sVersion.TrimLeft();
	lpName = lv_sVersion.GetBuffer(lv_sVersion.GetLength());
	token = strtok(lpName, seps);
	if(token != NULL )
	{
		lv_sndbdy.Version = (UC)atoi(token);
		lv_sndbdy.Version *= 16;
		token = strtok( NULL, seps );
		if(token != NULL )
			lv_sndbdy.Version += (UC)atoi(token);
	}

	///--lv_sndbdy.RegCode;

	memcpy(lv_sndmsg, &lv_sndhdr, sizeof(SMsgHeader));
	memcpy(lv_sndmsg+sizeof(SMsgHeader), &lv_sndbdy, sizeof(lv_sndbdy));

	// Sun updated 2010-05-1 [V7.1.8.3]
	if( m_sckPCServer.CompareThreadID(dwThreadID) )
		lv_retval = m_sckPCServer.SendControlData(lv_sndmsg, lv_sndhdr.PackageLen);
	else
		lv_retval = m_sckPCServer2.SendControlData(lv_sndmsg, lv_sndhdr.PackageLen);

	//return MQ_HA_SendMsg(lv_sndmsg, lv_sndhdr.PackageLen, TRUE);
	return lv_retval;
}

BOOL CMainFrame::MQ_SendMsg_Unregister()
{
	//-------------------------------
	BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];
	SMsgHeader lv_sndhdr;

	// Sun added 2010-05-18 [V7.1.8.3]
	memset(&lv_sndhdr, 0x00, sizeof(lv_sndhdr));

	lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
	lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
	lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
	lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
	lv_sndhdr.Sender = USER_IVR;
	lv_sndhdr.Receiver = USER_CTI;

	//-------------------------------

	lv_sndhdr.Command = TCP_IVR2PCS_UNREGISTER;
	lv_sndhdr.PackageLen = sizeof(SMsgHeader);

	// Send Message
	memcpy(lv_sndmsg, &lv_sndhdr, lv_sndhdr.PackageLen);

	// Sun updated 2010-05-18 [V7.1.8.3]
	return MQ_HA_SendMsg(lv_sndmsg, lv_sndhdr.PackageLen, TRUE);
}

// Sun added 2005-03-22
/// External Register / Unregister Message
BOOL CMainFrame::MQ_SendExtMsg_Register()
{
	//-------------------------------
	BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];
	SMsgHeader lv_sndhdr;

	// Sun added 2010-05-18 [V7.1.8.3]
	memset(&lv_sndhdr, 0x00, sizeof(lv_sndhdr));

	lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
	lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
	lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
	lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
	lv_sndhdr.Sender = USER_IVR;
	lv_sndhdr.Receiver = USER_EXT_IVR;

	//-------------------------------
	S_TCP_IVR2EXT_REGISTER lv_sndbdy;
	
	lv_sndhdr.Command = TCP_IVR2EXT_REGISTER;
	lv_sndhdr.PackageLen = sizeof(SMsgHeader) + sizeof(lv_sndbdy);

	lv_sndbdy.ChannelCount = theApp.m_nIVRChannels;

	memcpy(lv_sndmsg, &lv_sndhdr, sizeof(SMsgHeader));
	memcpy(lv_sndmsg+sizeof(SMsgHeader), &lv_sndbdy, sizeof(lv_sndbdy));

	return m_sckExternalServer.SendControlData(lv_sndmsg, lv_sndhdr.PackageLen);
}

BOOL CMainFrame::MQ_SendExtMsg_Unregister()
{
	//-------------------------------
	BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];
	SMsgHeader lv_sndhdr;

	// Sun added 2010-05-18 [V7.1.8.3]
	memset(&lv_sndhdr, 0x00, sizeof(lv_sndhdr));

	lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
	lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
	lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
	lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
	lv_sndhdr.Sender = USER_IVR;
	lv_sndhdr.Receiver = USER_EXT_IVR;

	//-------------------------------

	lv_sndhdr.Command = TCP_IVR2EXT_UNREGISTER;
	lv_sndhdr.PackageLen = sizeof(SMsgHeader);

	// Send Message
	memcpy(lv_sndmsg, &lv_sndhdr, lv_sndhdr.PackageLen);

	return m_sckExternalServer.SendControlData(lv_sndmsg, lv_sndhdr.PackageLen);
}
//----------------------------------------------------------------------------------------------

// 2004-08-07
void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	static int lv_nSteps = 0;
	static UINT lv_nDailyChk = 0;
	int lv_loop;
	CTime lv_tCurrTime;
	CString lv_sTemp;
	BOOL lv_blnResetDailyCallData;

	try
	{
		if( nIDEvent == 1 && m_bRunning )
		{
			// Sun added 2006-03-05
			// Check Connections
			lv_nSteps+=m_main_timer;
			if( lv_nSteps >= DEF_SCK_KEEPLIVE_INTERVAL )
			{
				lv_nSteps = 0;
				m_ThreadList.CheckAllThreads();
			}

			if( g_thSystemMain.GetPendingTime() > 5 )
			{
				WriteTrace(TraceError, "Error - Main Thread is pending on step: %d", g_thSystemMain.m_threadProgress);
				g_thSystemMain.ResetPendingTime();
				
				/// Stop System Main thread
				//g_thSystemMain.WaitForShutdownToComplete();

				/// Start System Main thread
				//g_thSystemMain.WaitForStartupToComplete();

#ifdef CISCO_CCM
				// Sun added 2010-08-05
				// Sun updated 2011-04-16, add gb_lngAutoCloseFlag
				/// 重启系统
				if( ( (int)(g_thSystemMain.m_threadProgress / 1000) == 10 || (int)(g_thSystemMain.m_threadProgress / 1000) == 8 )
					&& ((gb_lngAutoCloseFlag & PCS_PVC_AUTOCLOSE_MAINTHREAD_ERROR) == PCS_PVC_AUTOCLOSE_MAINTHREAD_ERROR) )
				{
					// Sun replaced 2011-04-16
					Sys_AutoCloseMe(PCS_PVC_AUTOCLOSE_MAINTHREAD_ERROR, g_thSystemMain.m_threadProgress);

					return;
				}
#endif
			}

			// Check TTS
			if( m_blnEnableTTS )
				m_TTSInterface.CheckTTSConnections();

			//-----------------------------------------------------------------
			// Sun added 2006-02-26
			/// Daily Routine Processing
			lv_nDailyChk+=m_main_timer;
			lv_blnResetDailyCallData = FALSE;
			if( lv_nDailyChk >= DEF_SYSTEM_ROUTINE_CHECK_INTERVAL )
			{
				lv_nDailyChk = 0;

				// Get data reset time
				lv_sTemp = theApp.GetProfileString("SYSEN", "WorkDataResetTime", _T("00:00:30"));
				/// Must be "00:00:00"
				if( lv_sTemp.GetLength() != 8 )
					lv_sTemp = _T("00:00:30");

				lv_tCurrTime = CTime::GetCurrentTime();
				if( lv_tCurrTime.GetDay() != m_tmStatisticsDate.GetDay() )
				{
					if( lv_sTemp <= lv_tCurrTime.Format("%H:%M:%S") )
					{
						m_tmStatisticsDate = lv_tCurrTime;
						lv_blnResetDailyCallData = TRUE;
					}
				}
			}
			//-----------------------------------------------------------------

			// Sun added 2004-12-30
			// Check Groups
			for( lv_loop = 0; lv_loop < theApp.m_nIVRGroups; lv_loop++ )
			{
				if( PIVRGroup[lv_loop] != NULL )
				{
					PIVRGroup[lv_loop]->CheckAutomaticTimer(lv_blnResetDailyCallData);

					// Sun added 2007-11-04
					/// 计算＂组通道通话最小间隔＂
					if( lv_nDailyChk == m_main_timer )
						PIVRGroup[lv_loop]->CalculateGCTIT();
				}
			}

			//--------------------------------------------------------
			// Sun added 2010-07-07
			// Check DataRecorder Connection
			m_sckDataRecorder.IsSockTimeOut();

			if( m_param_ctiport > 0 )
				m_sckPCServer.CheckPBXKeepLive();
			if( m_param_cti2ip.GetLength() > 0 && m_param_cti2port > 0 )
				m_sckPCServer2.CheckPBXKeepLive();
			//--------------------------------------------------------
		}
	}
	catch(...)
	{
		WriteTrace(TraceError, "CMainFrame::OnTimer() catch one error!!!");
	}
	
	//CFrameWnd::OnTimer(nIDEvent);
}

// Sun added 2004-11-20
void CMainFrame::OnViewLicInfo() 
{
	// TODO: Add your command handler code here
	CDlgViewLicense lv_dlg;
	
	lv_dlg.DoModal();
}

// Sun added 2004-11-20
void CMainFrame::OnTrunkSqllog() 
{
	// TODO: Add your command handler code here
	if( AfxMessageBox(IDS_MSG_Q_TRUNCATE_LOG, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION | MB_DEFBUTTON1) == IDYES )
	{
		m_sckDataRecorder.CloseSQLFile();
		WriteTrace(TraceInfo, "Truncate sql log OK!");
	}
}

//------------------------------------------------------
// Sun added 2004-12-30
void CMainFrame::OnFileTimerLdCf() 
{
	// TODO: Add your command handler code here
	CDlgAutoLoadCallFlow lv_dlg;
	int f_GroupNo;
	CIVRGroup *lv_pGroup;
	CTime lv_tmTime;
	UC lv_nPID;
	CString lv_sFileName;

	f_GroupNo = pGroupView->GetShowGroupNo();
	// Is No. right ?
	if( f_GroupNo >= 0 && f_GroupNo < theApp.m_nIVRGroups )
	{
		// Is object available ?
		if( (lv_pGroup = PIVRGroup[f_GroupNo]) != NULL )
		{
			lv_dlg.m_blnEnabled = lv_pGroup->GetAutoLoadCallFlow(lv_tmTime, &lv_nPID, lv_sFileName);
			lv_dlg.m_tmDate = lv_tmTime;
			lv_dlg.m_tmTime = lv_tmTime;
			lv_dlg.m_strFileName = lv_sFileName;
			lv_dlg.m_nselectitem = 0;
			lv_dlg.m_nselectitemdata = lv_nPID;

			if( lv_dlg.DoModal() == IDOK )
			{
				lv_tmTime = CTime::CTime(lv_dlg.m_tmDate.GetYear(), lv_dlg.m_tmDate.GetMonth(), lv_dlg.m_tmDate.GetDay(), lv_dlg.m_tmTime.GetHour(), lv_dlg.m_tmTime.GetMinute(), lv_dlg.m_tmTime.GetSecond());
				lv_pGroup->SetAutoLoadCallFlow(lv_dlg.m_blnEnabled, lv_tmTime, (UC)lv_dlg.m_nselectitemdata, lv_dlg.m_strFileName);
				WriteTrace(TraceInfo, "Info - IVR Group: %d set auto-load call flow: %d or %s at %s", 
					f_GroupNo, lv_dlg.m_nselectitemdata, lv_dlg.m_strFileName, lv_tmTime.Format("%Y-%m-%d %H:%M:%S"));
			}
		}
	}
}

void CMainFrame::OnUpdateFileTimerLdCf(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//pCmdUI->Enable(m_bME_FILE_LOAD_DB && m_bRunning );	
}

void CMainFrame::OnFileTimerLdRes() 
{
	// TODO: Add your command handler code here
	CDlgAutoLoadResource lv_dlg;
	int f_GroupNo;
	CIVRGroup *lv_pGroup;
	CTime lv_tmTime;
	US lv_nPID;

	f_GroupNo = pGroupView->GetShowGroupNo();
	// Is No. right ?
	if( f_GroupNo >= 0 && f_GroupNo < theApp.m_nIVRGroups )
	{
		// Is object available ?
		if( (lv_pGroup = PIVRGroup[f_GroupNo]) != NULL )
		{
			lv_dlg.m_blnEnabled = lv_pGroup->GetAutoLoadResource(lv_tmTime, &lv_nPID);
			lv_dlg.m_tmDate = lv_tmTime;
			lv_dlg.m_tmTime = lv_tmTime;
			lv_dlg.m_nselectitem = 0;
			lv_dlg.m_nselectitemdata = lv_nPID;

			if( lv_dlg.DoModal() == IDOK )
			{
				if( lv_dlg.m_nselectitem >= 0 )
				{
					lv_tmTime = CTime::CTime(lv_dlg.m_tmDate.GetYear(), lv_dlg.m_tmDate.GetMonth(), lv_dlg.m_tmDate.GetDay(), lv_dlg.m_tmTime.GetHour(), lv_dlg.m_tmTime.GetMinute(), lv_dlg.m_tmTime.GetSecond());
					lv_pGroup->SetAutoLoadResource(lv_dlg.m_blnEnabled, lv_tmTime, lv_dlg.m_nselectitemdata);
					WriteTrace(TraceInfo, "Info - IVR Group: %d set auto-load resource: %d at %s", 
						f_GroupNo, lv_dlg.m_nselectitemdata, lv_tmTime.Format("%Y-%m-%d %H:%M:%S"));
				}
			}
		}
	}
}

void CMainFrame::OnUpdateFileTimerLdRes(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bME_FILE_LOAD_RESOURCE && m_bRunning );
}
// End of Sun added 2004-12-30
//------------------------------------------------------

//-----------------------------------------------------------------
// Sun added 2010-03-08 for Cisco CME
/// Cisco CME Interface
#ifdef CISCO_CME

// 根据路由点选择IVR组及通道
CIVRChannel *CMainFrame::SelectChannelForRoutePoint(int nRoutePointID)
{
	int lv_loop;
	int lv_nGroupIndex = -1;
	CIVRChannel *lv_pCH = NULL;

	// 查找IVR组
	if( nRoutePointID >= 0 )
	{
		for( lv_loop = 0; lv_loop < theApp.m_nIVRGroups; lv_loop++ )
		{
			if( PIVRGroup[lv_loop] != NULL )
			{
				if( PIVRGroup[lv_loop]->GetCTIRoutePointID() == nRoutePointID )
				{
					lv_nGroupIndex = lv_loop;
					break;
				}
			}
		}
	}

	// 获取空闲IVR通道
	if( lv_nGroupIndex >= 0 )
		lv_pCH = PIVRGroup[lv_loop]->MoveToNextFlagCH();
	else
		WriteTrace(TraceWarn, "Warn - There is no IVR Group whose route point matches %d!", nRoutePointID);

	return lv_pCH;
}

#endif
// End of Sun added 2010-03-08
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Sun added 2011-07-10 [C]
static UINT ProcessSuicideThread(LPVOID pParam)
{
	// Sun added 2011-07-22
	WriteTrace(TraceError, "SysError - will execute ExitProcess() after 10s!");
	Sleep(10000);

	// Sun added 2011-07-10 [C]
	WriteTrace(TraceError, "SysError - will execute ExitProcess() now!");
	ExitProcess(0);

	// TerminateProcess((HANDLE)pParam, 0);

	return 0;
}
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Sun added 2013-01-27 [A]
/// 发送任务离队消息
void CMainFrame::CancelAllUMSTasks()
{
	WriteTrace(TraceInfo, "SysInfo - IVR is going to cancel all UMS tasks in queue...");

	// Initialize CTI devices group by group
	for( int lv_loop = 0; lv_loop < theApp.m_nIVRGroups; lv_loop++ )
	{
		if( PIVRGroup[lv_loop] != NULL )
			PIVRGroup[lv_loop]->CancelUMSTasks();
	}
}
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Sun added 2011-04-16
/// 系统自动关闭
void CMainFrame::Sys_AutoCloseMe(UL lngErrorCode, UL lngReasonCode)
{
	HANDLE lv_hdMain;
	DWORD lv_dwPID;

	WriteTrace(TraceError, "Error - System will shutdown automatically ErrorCode = 0x%X, ReasonCode = %d!", lngErrorCode, lngReasonCode);

	// 不接受新呼叫
	m_bRunning = FALSE;

	m_bSystemAutoShutDown = true;

	// Sun added 2013-01-27 [A]
	/// 自杀前，每个有排队任务的通道，发送任务离队消息
	CancelAllUMSTasks();

	/// 向CTI发送IVR注销消息
	MQ_SendMsg_Unregister();

	lv_hdMain = GetCurrentProcess();
	lv_dwPID = GetCurrentProcessId();

	// Sun moved backward 2011-10-11 To [A1]
	//this->PostMessage(WM_CLOSE);

	//--------------------------------------
	// Sun added 2011-07-10 [C]
	// Start Suicide Thread
	CWinThread  *lv_Thread;
	lv_Thread = AfxBeginThread((AFX_THREADPROC)ProcessSuicideThread, lv_hdMain, THREAD_PRIORITY_HIGHEST, 0);
	if( lv_Thread == NULL )
	{
		WriteTrace(TraceError, "Error : can't create ProcessSuicideThread - AfxBeginThread() error 0x%x.", GetLastError());

		// Sun replaced 2011-05-10
		/// 确保退出
		WriteTrace(TraceError, "SysError - will execute TerminateProcess()!");
		ReleaseMutex(theApp.m_hPROID);
		TerminateProcess(lv_hdMain, 0);

		//--------------------------------------
		// Sun added 2011-07-10 [C]
		char lv_cmd[50];
		memset(lv_cmd, 0x00, sizeof(lv_cmd));
		sprintf(lv_cmd, "ntsd -c q -p %d", lv_dwPID);
		system(lv_cmd);
		//--------------------------------------

		return;
	}
	// Sun removed 2011-07-22
	/*
	else
	{
		// Wait Process Recycle Resource
		Sleep(10000);
	}
	*/
	//--------------------------------------

	// Sun moved backward 2011-10-11 From [A1]
	//this->PostMessage(WM_CLOSE);

	return;
}
//-----------------------------------------------------------------
