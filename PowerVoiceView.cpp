// PowerVoiceView.cpp : implementation of the CPowerVoiceView class
//

#include "stdafx.h"
#include "PowerVoice.h"

#include "MainFrm.h"
#include "PowerVoiceDoc.h"
#include "PowerVoiceView.h"
#include ".\powervoiceview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Channel View Pointer
CPowerVoiceView *pChannelView;

/////////////////////////////////////////////////////////////////////////////
// CPowerVoiceView
IMPLEMENT_DYNCREATE(CPowerVoiceView, CListView)

BEGIN_MESSAGE_MAP(CPowerVoiceView, CListView)
	//{{AFX_MSG_MAP(CPowerVoiceView)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_RCLICK, OnNMRclick)
	ON_COMMAND(ID_MenuCh_Reset, OnMenuchReset)
	ON_COMMAND(ID_MenuCh_Restart, OnMenuchRestart)
	ON_COMMAND(ID_MenuCh_OBTes, OnMenuchObtes)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPowerVoiceView construction/destruction

CPowerVoiceView::CPowerVoiceView()
{
	// TODO: add construction code here
	m_pimagelist = NULL;
	pChannelView = this;
	m_nCurrentItem = -1;

#ifndef CISCO_CME

	// Sun added 2002-12-02
	m_oIVRCHMap.RemoveAll();
	m_oIVRCHMap.InitHashTable(MAX_IVR_CHANNEL + MAX_IVR_CHANNEL / 4);
	// End of Add

#endif
}

CPowerVoiceView::~CPowerVoiceView()
{
	if( m_pimagelist )
		delete m_pimagelist;
}

/////////////////////////////////////////////////////////////////////////////
// CPowerVoiceView diagnostics

#ifdef _DEBUG
void CPowerVoiceView::AssertValid() const
{
	CListView::AssertValid();
}

void CPowerVoiceView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CPowerVoiceDoc* CPowerVoiceView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPowerVoiceDoc)));
	return (CPowerVoiceDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPowerVoiceView message handlers
void CPowerVoiceView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	// Init Form
	InitMainForm();

	pMainWindow->WriteStatusBar(STATUS_BAR_SYS, ICON_Stop);
	pMainWindow->WriteStatusBar(STATUS_BAR_DB, ICON_Stop);
	pMainWindow->WriteStatusBar(STATUS_BAR_COMM, ICON_Stop);
	pMainWindow->WriteStatusBar(STATUS_BAR_HARDWARE, ICON_Stop);

	// Get System H/W Parameters
	/// Sun updated 2013-09-23 from "%0X" to "%08X"
	theApp.m_strThisHDSerialNumber.Format("%08X", theApp.m_SysInfo.GetHDVolumeInformation("c:\\"));

	// Start System
	if( !pMainWindow->Sys_StartSystem() )
		pMainWindow->SetMainMenuStatus(MAIN_FRM_STATUS_STOP);
	
	// Add Items into Group List
	pGroupView->Screen_Initialize_ListItmes();

	if( PIVRGroup[0] )
		Screen_ShowIVRChannelInfo(PIVRGroup[0]);

	pMainWindow->m_bShowAllChannel = !pMainWindow->m_bShowAllChannel;
	pMainWindow->PostMessage(WM_COMMAND, ID_VIEW_ALL_CH);

	pMainWindow->m_bCompactChannelView = !pMainWindow->m_bCompactChannelView;
	pMainWindow->PostMessage(WM_COMMAND, ID_VIEW_REPORT);
}


#ifdef INTEL_DIALOGIC

void CPowerVoiceView::Sys_IVREventAnalysis(void)
{
	CIVRChannel *lv_pIVRCh = NULL;
	DX_CST	*cstp;
	void	*datap;
	char	tempbuf[30];
	int		ndevno, ntype;
	int		lv_lineno;
	
	if( !pMainWindow->m_bRunning )
		return;

	try
	{
		sr_waitevt(0);
		ndevno = sr_getevtdev();
		ntype = sr_getevttype();

		// Sun Added 2001-11-28
		if( !m_oIVRCHMap.Lookup(ndevno, lv_pIVRCh) )
			return;

		if( lv_pIVRCh == NULL )
			return;

		lv_lineno = lv_pIVRCh->GetLineNO();
		if( lv_pIVRCh->m_oARS.GetVoxHandle() == ndevno )
		{
			// DEV_ANALOG
			// ToDo: Here we process analog (eg. DTMF) event
			switch( ntype )
			{
			case TDX_CST:
				cstp = (DX_CST *)sr_getevtdatap();

				switch(cstp->cst_event)
				{
				case DE_RINGS:
					WriteTrace(TraceDebug, "Ring... Ring... channel [%d]", ndevno);
					lv_pIVRCh->NF_CallIn();
					break;

				case DE_TONEON:
				case DE_LCOF:
					WriteTrace(TraceDebug, "Hang Up Code [%d] on channel [%d]", cstp->cst_data, ndevno);
					lv_pIVRCh->SetHookOn();
					break;
				}
				break;

			case TDX_GETDIG:
				break;
			
			// 2002-02-01
			case TDX_CALLP:
				long cpret;
				if( (cpret = ATDX_CPTERM( ndevno )) != CR_CNCT )
				{
					WriteTrace(TraceDebug, "Dial failed on channel [%d] with error code [%d]", ndevno, cpret);

					// Send Clear Call Message to Agent
				}
				break;
			}
		}
		else
		{
			// DEV_DIGIT:
			// ToDo: Here we process digital (eg. ISDN) event
			switch( ntype )
			{
			case CCEV_DISCONNECTED:
				WriteTrace(TraceDebug, "Dev%d:CCEV_DISCONNECTED.", ndevno);
				
				// ToDo: On-Hook Process
				lv_pIVRCh->SetHookOn();
				break;

			case CCEV_OFFERED:
				datap = sr_getevtdatap();
				WriteTrace(TraceDebug, "Dev%d:CCEV_OFFERED -- Incoming call on line:%d.\n", ndevno, lv_lineno);

				// Get Call Ref. ID
				if( cc_GetCRN(&lv_pIVRCh->m_callrefno, datap) == -1 )
					lv_pIVRCh->CTI_ErrorHandler();

				// Get Caller ID
				if( cc_GetANI(lv_pIVRCh->m_callrefno, tempbuf) < 0 )
					WriteTrace(TraceError, "cc_GetANI, not available.");
				else
					WriteTrace(TraceDebug, "cc_GetANI:%s.", tempbuf);

				// Answer Call Automatically
				if( cc_AnswerCall(lv_pIVRCh->m_callrefno, 0, EV_ASYNC) == -1 )
					lv_pIVRCh->CTI_ErrorHandler();
				else
				{
					if( lv_pIVRCh->GetStarted() )
						if( !lv_pIVRCh->GetCTIOpened() )
							lv_pIVRCh->NF_SetCallerID((UC *)tempbuf);
				}

				break;

			case CCEV_ANSWERED:
				WriteTrace(TraceDebug, "Dev%d:CCEV_ANSWERED.", ndevno);
				
				// ToDo: Start a Call Process Thread
				lv_pIVRCh->NF_CallIn();

				break;

			case CCEV_ALERTING:
				WriteTrace(TraceDebug, "Dev%d:CCEV_ALERTING.", ndevno);
				break;

			case CCEV_PROCEEDING:
				WriteTrace(TraceDebug, "Dev%d:CCEV_PROCEEDING.", ndevno);
				break;

			case CCEV_DROPCALL:
				WriteTrace(TraceDebug, "Dev%d:CCEV_DROPCALL.", ndevno);
				break;

			case CCEV_RESTART:
				WriteTrace(TraceDebug, "Dev%d:CCEV_RESTART.", ndevno);
				break;

			case CCEV_CONNECTED	:				// Make Call Successfully
				// 连接两个通道
				WriteTrace(TraceDebug, "Dev%d:Connected.", ndevno);
				break;

			case CCEV_TASKFAIL:					// Make Call Failed
				WriteTrace(TraceError, "Dev%d:CCEV_TASKFAIL.", ndevno);
				break;

			case CCEV_MOREDIGITS:
				WriteTrace(TraceDebug, "Dev%d:CCEV_MOREDIGITS.", ndevno);
				break;
			}
		}
	}
	catch(...)
	{
		WriteTrace(TraceError, "Sys_IVREventAnalysis() Error!!!");
	}
}

#endif


LRESULT CPowerVoiceView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message)
	{

#ifdef INTEL_DIALOGIC

	case MYWM_IVREVENT:					// Dialogic MSG
		Sys_IVREventAnalysis();
		break;

#endif

	case MYWM_CHANGE_CHANNEL:			// Modify ivr channel info
		if( pMainWindow->m_bRunning )
		{
			if( wParam < theApp.m_nIVRChannels )
			{
				if( (UC)(lParam / 256) == pGroupView->GetShowGroupNo() || pMainWindow->m_bShowAllChannel )
				{
					switch( lParam % 256 )
					{
					case SCR_CHANNEL_ITEM_CHID:
						// Only ICNO is changed
					case SCR_CHANNEL_ITEM_STATUS:
						// Only Channel Status is changed
						Screen_ChangeListItemStatus(wParam);
						break;

					case SCR_CHANNEL_ITEM_CALLERID:
						// Only Caller ID is changed
						Screen_ChangeListItemCallerID(wParam);
						break;

					case SCR_CHANNEL_ITEM_CALLLENGTH:
						// Only Time Length is changed
						Screen_ChangeListItemCallLength(wParam);
						break;

					case SCR_CHANNEL_ITEM_CTI:
						// Only CTI Monitoring Status is changed
						Screen_ChangeListItemCTIStatus(wParam);
						break;

					// Sun added 2006-02-26
					case SCR_CHANNEL_ITEM_CALLCOUNT:
						// Only Call Count Status is changed
						Screen_ChangeListItemCallCount(wParam);
						break;

					default:
						Screen_InsertChannel(PIVRCH[wParam]);
						break;
					}
				}
			}
		}
		break;
	}

	return CListView::WindowProc(message, wParam, lParam);
}

// Initialize Main Form
void CPowerVoiceView::InitMainForm()
{
	CString lv_sCaption;

	lv_sCaption.LoadString(IDS_CAP_IVRGR_STARTCH);

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

	//Create the ChannelList columns
	lv_sCaption.LoadString(IDS_CAP_IVRCH_CHANNELNO);
	control.InsertColumn(SCR_CHANNEL_ITEM_CHID, lv_sCaption, LVCFMT_LEFT, 80, SCR_CHANNEL_ITEM_CHID);
	
	lv_sCaption.LoadString(IDS_CAP_IVRCH_STATUS);
	control.InsertColumn(SCR_CHANNEL_ITEM_STATUS, lv_sCaption, LVCFMT_LEFT, 80, SCR_CHANNEL_ITEM_STATUS);

	lv_sCaption.LoadString(IDS_CAP_IVRCH_DURATION);
	control.InsertColumn(SCR_CHANNEL_ITEM_CALLLENGTH, lv_sCaption, LVCFMT_LEFT, 80, SCR_CHANNEL_ITEM_CALLLENGTH);

	lv_sCaption.LoadString(IDS_CAP_IVRCH_ANI);
	control.InsertColumn(SCR_CHANNEL_ITEM_CALLERID, lv_sCaption, LVCFMT_LEFT, 100, SCR_CHANNEL_ITEM_CALLERID);

	lv_sCaption.LoadString(IDS_CAP_IVRCH_DNIS);
	control.InsertColumn(SCR_CHANNEL_ITEM_CALLEEID, lv_sCaption, LVCFMT_LEFT, 80, SCR_CHANNEL_ITEM_CALLEEID);

	lv_sCaption.LoadString(IDS_CAP_IVRCH_CTI_ST);
	control.InsertColumn(SCR_CHANNEL_ITEM_CTI, lv_sCaption, LVCFMT_LEFT, 80, SCR_CHANNEL_ITEM_CTI);

	//----------------------------------------------------------
	// Sun added 2006-02-26
	lv_sCaption.LoadString(IDS_CAP_IVRGR_CALLCOUNT);
	control.InsertColumn(SCR_CHANNEL_ITEM_CALLCOUNT, lv_sCaption, LVCFMT_LEFT, 120, SCR_CHANNEL_ITEM_CALLCOUNT);
	//----------------------------------------------------------

	lv_sCaption.LoadString(IDS_CAP_IVRCH_EDID);
	control.InsertColumn(SCR_CHANNEL_ITEM_EDID, lv_sCaption, LVCFMT_LEFT, 100, SCR_CHANNEL_ITEM_EDID);
}

BOOL CPowerVoiceView::Screen_ChangeViewMode(bool f_bListMode)
{
	CListCtrl& control = this->GetListCtrl();

	if( f_bListMode )
		control.ModifyStyle(LVS_ICON|LVS_REPORT,LVS_LIST|LVS_SORTASCENDING,0);
	else
		control.ModifyStyle(LVS_ICON|LVS_LIST,LVS_REPORT|LVS_SORTASCENDING,0);


	return TRUE;
}

// Show information of IVR channels under a group
void CPowerVoiceView::Screen_ShowIVRChannelInfo( CIVRGroup *f_Group )
{
	CListCtrl& lv_wndChannel = GetListCtrl();
	int lv_from, lv_to;
	int lv_loop;
	
	if( pMainWindow->m_bShowAllChannel )
	{
		lv_from = 0;
		lv_to = theApp.m_nIVRChannels;
	}
	else
	{
		if( f_Group == NULL )
			return;
		lv_from = f_Group->GetStartCH();
		lv_to = f_Group->GetEndCH() + 1;
	}

	lv_wndChannel.EnableWindow(FALSE);
	lv_wndChannel.DeleteAllItems();
	for(lv_loop = lv_from; lv_loop < lv_to; lv_loop++)
	{
		if( PIVRCH[lv_loop] != NULL )
			Screen_InsertChannel(PIVRCH[lv_loop]);
	}
	lv_wndChannel.EnableWindow(TRUE);
}

BOOL CPowerVoiceView::Screen_ChangeListItemCallLength(WPARAM f_ChannelNo)
{
    int item;
	CString sChannelNo;
	long duration;
	CString time;
	CListCtrl& lv_wndChannel = GetListCtrl();
	CIVRChannel *lv_pChannel = PIVRCH[f_ChannelNo];

	sChannelNo.Format(" CH%5d", f_ChannelNo);
	
	// Find Delete location
	if( FindListCtrlItem(&lv_wndChannel, sChannelNo, &item) )
	{
		duration=lv_pChannel->GetDuration();
		time.Format("%u s", duration);
		lv_wndChannel.SetItemText(item,SCR_CHANNEL_ITEM_CALLLENGTH,time);
//		lv_wndChannel.Update(TRUE);

		return TRUE;
	}

	return FALSE;
}

// Sun added 2006-02-26
BOOL CPowerVoiceView::Screen_ChangeListItemCallCount(WPARAM f_ChannelNo)
{
    int item;
	CString sChannelNo;
	CListCtrl& lv_wndChannel = GetListCtrl();
	CIVRChannel *lv_pChannel = PIVRCH[f_ChannelNo];

	sChannelNo.Format(" CH%5d", f_ChannelNo);
	
	// Find Delete location
	if( FindListCtrlItem(&lv_wndChannel, sChannelNo, &item) )
	{
		CString lv_sCallCount;
		lv_sCallCount.Format("%u/%u", lv_pChannel->GetDailyCallCount(), lv_pChannel->GetYTDCallCount());
		lv_wndChannel.SetItemText(item,SCR_CHANNEL_ITEM_CALLCOUNT,lv_sCallCount);
//		lv_wndChannel.Update(TRUE);

		return TRUE;
	}

	return FALSE;
}

BOOL CPowerVoiceView::Screen_ChangeListItemCTIStatus(WPARAM f_ChannelNo)
{
    int item;
	CString sChannelNo;
	CString lv_sCaption;
	CListCtrl& lv_wndChannel = GetListCtrl();
	CIVRChannel *lv_pChannel = PIVRCH[f_ChannelNo];

	sChannelNo.Format(" CH%5d", f_ChannelNo);
	
	// Find Delete location
	if( FindListCtrlItem(&lv_wndChannel, sChannelNo, &item) )
	{
		if( lv_pChannel->GetNeedCTI() )
		{
			if( lv_pChannel->GetCTIAvailble() )
				lv_sCaption.LoadString(IDS_CAP_IVRCH_CTI_MONITOR_OK);
			else
				lv_sCaption.LoadString(IDS_CAP_IVRCH_CTI_MONITOR_ERROR);
		}
		else
			lv_sCaption.LoadString(IDS_CAP_IVRCH_CTI_DISABLE);

		lv_wndChannel.SetItemText(item,SCR_CHANNEL_ITEM_CTI,lv_sCaption);
//		lv_wndChannel.Update(TRUE);

		return TRUE;
	}

	return FALSE;
}

BOOL CPowerVoiceView::Screen_ChangeListItemCallerID(WPARAM f_ChannelNo)
{
    int item;
	CString sChannelNo;
	CString lv_callerid, lv_calleeid;
	CListCtrl& lv_wndChannel = GetListCtrl();
	CIVRChannel *lv_pChannel = PIVRCH[f_ChannelNo];
	UC *lv_TelNo;

	sChannelNo.Format(" CH%5d", f_ChannelNo);
	
	// Find Delete location
	if( FindListCtrlItem(&lv_wndChannel, sChannelNo, &item) )
	{
		lv_TelNo = lv_pChannel->NF_GetCallerID();
		if( lv_TelNo[0] == NULL )
			lv_callerid = _T("");
		else
			lv_callerid.Format("%s", (const char *)lv_TelNo);
		lv_wndChannel.SetItemText(item,SCR_CHANNEL_ITEM_CALLERID,lv_callerid);

		lv_TelNo = lv_pChannel->NF_GetCalleeID();
		if( lv_TelNo[0] == NULL )
			lv_calleeid = _T("");
		else
			lv_calleeid.Format("%s", (const char *)lv_TelNo);
		lv_wndChannel.SetItemText(item,SCR_CHANNEL_ITEM_CALLEEID,lv_calleeid);

//		lv_wndChannel.Update(TRUE);

		return TRUE;
	}

	return FALSE;
}

BOOL CPowerVoiceView::Screen_ChangeListItemStatus(WPARAM f_ChannelNo)
{
    int item;
	int image;
	CString sChannelNo;
	CString lv_state;
	CListCtrl& lv_wndChannel = GetListCtrl();
	CIVRChannel *lv_pChannel = PIVRCH[f_ChannelNo];
	UC lv_chstate;

	sChannelNo.Format(" CH%5d", f_ChannelNo);
	
	lv_chstate = lv_pChannel->GetStatus();
	// Find Delete location
	if( FindListCtrlItem(&lv_wndChannel, sChannelNo, &item) )
	{
		// ICON
		if(!lv_pChannel->GetEnabled())
		{
			image=ICON_Error;			// red
		}
		else if(!lv_pChannel->GetStarted())
		{
			image=ICON_Stop;			// yellow
		}
		else
		{
			switch(lv_chstate)
			{
			case IVR_IDLE:
				image=ICON_Ready;		// green
				break;
			case IVR_BLOCK:
				image=ICON_Stop;		// yellow
				break;
			case IVR_ERROR:
				image=ICON_Error;		// red
				break;
			case IVR_RUN:
			case IVR_W_ACK:
			case IVR_W_ANSWER:
			case IVR_W_PLAY:
			case IVR_W_KEY:
			case IVR_W_RECORD:
			case IVR_W_FAX:
			case IVR_W_SWITCH:
				image=ICON_Start;		// blue
				break;
			default:
				image=ICON_Error;		// red
				break;
			}
		}

		// Status Caption
		switch(lv_chstate)
		{
		case IVR_IDLE:					// 信道空闲
			lv_state = _T("Idle");
			break;

		case IVR_BLOCK:					// 信道阻塞
			lv_state = _T("Block");
			break;

		case IVR_RUN:					// 信道正常运行
			lv_state = _T("Run");
			break;

		case IVR_W_ACK:					// 等待消息应答
		case IVR_W_ANSWER:
			lv_state = _T("Wait");
			break;

		case IVR_W_PLAY:				// 等待放音结束
			lv_state = _T("Play");
			break;

		case IVR_W_KEY:					// 等待按键结束
			lv_state = _T("Keypress");
			break;

		case IVR_W_RECORD:				// 等待录音结束
			lv_state = _T("Record");
			break;

		case IVR_W_FAX:					// 等待传真结束
			lv_state = _T("FAX");
			break;
			
		case IVR_W_SWITCH:				// 等待转接
			lv_state = _T("Switch");
			break;

		case IVR_ERROR:
			lv_state = _T("Error");
			break;

		default:
			lv_state = _T("");
		}

		lv_wndChannel.SetItem(item, SCR_CHANNEL_ITEM_CHID, LVIF_IMAGE, sChannelNo, image, 0, 0, 0);
		lv_wndChannel.SetItemText(item, SCR_CHANNEL_ITEM_CHID, sChannelNo);
		lv_wndChannel.SetItemText(item, SCR_CHANNEL_ITEM_STATUS, lv_state);

//		lv_wndChannel.Update(TRUE);
		
		return TRUE;
	}

	return FALSE;
}

BOOL CPowerVoiceView::Screen_DeleteChannel(WPARAM f_ChannelNo)
{
    int item;
	CString LineNo;
	CListCtrl& lv_wndChannel = GetListCtrl();

	LineNo.Format(" CH%5d", f_ChannelNo);
	
	// Find Delete location
	if( FindListCtrlItem(&lv_wndChannel, LineNo, &item) )
	{
		lv_wndChannel.DeleteItem(item);
//		lv_wndChannel.Update(TRUE);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CPowerVoiceView::Screen_InsertChannel(CIVRChannel *f_Channel)
{
	int item;
    int image;
	CString LineNo;
	CListCtrl& lv_wndChannel = GetListCtrl();

	UC lv_chstate = f_Channel->GetStatus();

	if(!f_Channel->GetEnabled())
	{
		image=ICON_Error;			// red
	}
	else if(!f_Channel->GetStarted())
	{
		image=ICON_Stop;			// yellow
	}
	else
	{
		switch(lv_chstate)
		{
		case IVR_IDLE:
			image=ICON_Ready;		// green
			break;
		case IVR_BLOCK:
			image=ICON_Stop;		// yellow
			break;
		case IVR_ERROR:
			image=ICON_Error;		// red
			break;
		case IVR_RUN:
		case IVR_W_ACK:
		case IVR_W_ANSWER:
		case IVR_W_PLAY:
		case IVR_W_KEY:
		case IVR_W_RECORD:
		case IVR_W_FAX:
		case IVR_W_SWITCH:
			image=ICON_Start;		// blue
			break;
		default:
			image=ICON_Error;		// red
			break;
		}
	}

    LineNo.Format(" CH%5d", f_Channel->GetLineNO());
	
	// Find Insert location
	// if occur an existed item, then replace it,
	// otherwise insert new one
	if( FindListCtrlItem(&lv_wndChannel, LineNo, &item) )
	{
		// Modify old item
		lv_wndChannel.SetItem(item, SCR_CHANNEL_ITEM_CHID, LVIF_IMAGE, LineNo, image, 0, 0, 0);
		lv_wndChannel.SetItemText(item, SCR_CHANNEL_ITEM_CHID, LineNo);
	}
	else
	{
		// Insert new item
		item=lv_wndChannel.InsertItem(MAX_IVR_CHANNEL, LineNo, image);
		if( item == -1 )
			return FALSE;

		// Sun added 2006-05-26
		lv_wndChannel.SetItemData(item, f_Channel->GetLineNO());
	}
	
	CString lv_state;

	switch(lv_chstate)
	{
	case IVR_IDLE:					// 信道空闲
		lv_state = _T("Idle");
		break;

	case IVR_BLOCK:					// 信道阻塞
		lv_state = _T("Block");
		break;

	case IVR_RUN:					// 信道正常运行
		lv_state = _T("Run");
		break;

	case IVR_W_ACK:					// 等待消息应答
	case IVR_W_ANSWER:
		lv_state = _T("Wait");
		break;

	case IVR_W_PLAY:				// 等待放音结束
		lv_state = _T("Play");
		break;

	case IVR_W_KEY:					// 等待按键结束
		lv_state = _T("Keypress");
		break;

	case IVR_W_RECORD:				// 等待录音结束
		lv_state = _T("Record");
		break;

	case IVR_W_FAX:					// 等待传真结束
		lv_state = _T("FAX");
		break;
		
	case IVR_W_SWITCH:				// 等待转接
		lv_state = _T("Switch");
		break;

	case IVR_ERROR:
		lv_state = _T("Error");
		break;

	default:
		lv_state = _T("");
	}
	lv_wndChannel.SetItemText(item,SCR_CHANNEL_ITEM_STATUS,lv_state);

	long duration;
	CString time;
    duration=f_Channel->GetDuration();
    time.Format("%u s", duration);
	lv_wndChannel.SetItemText(item,SCR_CHANNEL_ITEM_CALLLENGTH,time);

	lv_wndChannel.SetItemText(item,SCR_CHANNEL_ITEM_CALLERID, (char *)f_Channel->NF_GetCallerID());
	lv_wndChannel.SetItemText(item,SCR_CHANNEL_ITEM_CALLEEID, (char *)f_Channel->NF_GetCalleeID());

	if( f_Channel->GetNeedCTI() )
	{
		if( f_Channel->GetCTIAvailble() )
			lv_state.LoadString(IDS_CAP_IVRCH_CTI_MONITOR_OK);
		else
			lv_state.LoadString(IDS_CAP_IVRCH_CTI_MONITOR_ERROR);
	}
	else
		lv_state.LoadString(IDS_CAP_IVRCH_CTI_DISABLE);
	lv_wndChannel.SetItemText(item,SCR_CHANNEL_ITEM_CTI,lv_state);

	CString CurEditID;
	CurEditID.Format("%u", f_Channel->GetCurETID());
	lv_wndChannel.SetItemText(item,SCR_CHANNEL_ITEM_EDID, CurEditID);

	//----------------------------------------------------------
	// Sun added 2006-02-26
	CString lv_sCallCount;
	lv_sCallCount.Format("%u/%u", f_Channel->GetDailyCallCount(), f_Channel->GetYTDCallCount());
	lv_wndChannel.SetItemText(item,SCR_CHANNEL_ITEM_CALLCOUNT, lv_sCallCount);
	//----------------------------------------------------------

//	lv_wndChannel.EnsureVisible(item,true);
//	lv_wndChannel.Update(TRUE);

    return TRUE;
}

// Find item from list
BOOL CPowerVoiceView::FindListCtrlItem(CListCtrl *f_list, CString f_name, int *f_index)
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

// Sun added 2006-05-26
void CPowerVoiceView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CListCtrl& lv_wndChannel = GetListCtrl();
	
	int lv_nChannelNo, lv_index = pNMListView->iItem;
	if( lv_index >= 0 )
	{
		lv_nChannelNo = lv_wndChannel.GetItemData(lv_index);
		if( lv_nChannelNo >= 0 )
		{
			CIVRChannel *lv_pChannel = PIVRCH[lv_nChannelNo];
			if( lv_pChannel )
			{
				if( lv_pChannel->GetStarted() && lv_pChannel->GetStatus() < IVR_RUN )
					lv_pChannel->CTI_UnsetChannelError();
			}
		}
	}

	*pResult = 0;
}

void CPowerVoiceView::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	//draw each item.set txt color,bkcolor....
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	
	// Take the default processing unless we set this to something else below.
	*pResult = CDRF_DODEFAULT;
	
	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.
	
	if (pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		// This is the notification message for an item.  We'll request
		// notifications before each subitem's prepaint stage.
		
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM))
	{
		// This is the prepaint stage for a subitem. Here's where we set the
		// item's text and background colors. Our return value will tell
		// Windows to draw the subitem itself, but it will use the new colors
		// we set here.
		int nItem = static_cast<int> (pLVCD->nmcd.dwItemSpec);
		
		// store the colors back in the NMLVCUSTOMDRAW struct
		if( nItem % 2 == 1 )
			pLVCD->clrTextBk = RGB(225, 225, 225);
		else
			pLVCD->clrTextBk = RGB(255, 255, 255);

		//CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
		//CRect rect;
		//control.GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
		//pDC->SetBkColor(crBkgnd);

		//*pResult = CDRF_SKIPDEFAULT;	// We've painted everything.
	}
}

// Sun added 2012-06-01
/// 弹出通道操作菜单
void CPowerVoiceView::OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CListCtrl& lv_wndChannel = GetListCtrl();
	
	int lv_nChannelNo, lv_index = pNMListView->iItem;
	if( lv_index >= 0 )
	{
		m_nCurrentItem = lv_index;

		/// Popup Menu
		CMenu menu, *pSubMenu;
		POINT lv_scPos;
	
		if (!menu.LoadMenu(IDR_POPMENU_CH)) return;
		if (!(pSubMenu = menu.GetSubMenu(0))) return;

		/// Display and track the popup menu
		GetCursorPos(&lv_scPos);
		pSubMenu->TrackPopupMenu(0, lv_scPos.x, lv_scPos.y, this);
		menu.DestroyMenu();
	}
	else
		m_nCurrentItem = -1;

	*pResult = 0;
}

// Sun added 2012-06-01
void CPowerVoiceView::OnMenuchReset()
{
	// TODO: 在此添加命令处理程序代码
	if( m_nCurrentItem >= 0 )
	{
		CListCtrl& lv_wndChannel = GetListCtrl();
		int lv_nChannelNo = lv_wndChannel.GetItemData(m_nCurrentItem);

		if( lv_nChannelNo >= 0 )
		{
			CIVRChannel *lv_pChannel = PIVRCH[lv_nChannelNo];
			if( lv_pChannel )
			{
				if( lv_pChannel->GetStarted() && lv_pChannel->GetStatus() < IVR_RUN )
					lv_pChannel->CTI_UnsetChannelError();
			}
		}
	}
}

// Sun added 2012-06-01
void CPowerVoiceView::OnMenuchRestart()
{
	// TODO: 在此添加命令处理程序代码
	if( m_nCurrentItem >= 0 )
	{
		CListCtrl& lv_wndChannel = GetListCtrl();
		int lv_nChannelNo = lv_wndChannel.GetItemData(m_nCurrentItem);

		if( lv_nChannelNo >= 0 )
		{
			CIVRChannel *lv_pChannel = PIVRCH[lv_nChannelNo];
			if( lv_pChannel )
			{
				lv_pChannel->CTI_ErrorHandler();
			}
		}
	}
}

// Sun added 2012-06-01
/// 测试：走流程，通常是外拨流程
void CPowerVoiceView::OnMenuchObtes()
{
	// TODO: 在此添加命令处理程序代码
	if( m_nCurrentItem >= 0 )
	{
		CListCtrl& lv_wndChannel = GetListCtrl();
		int lv_nChannelNo = lv_wndChannel.GetItemData(m_nCurrentItem);

		if( lv_nChannelNo >= 0 )
		{
			CIVRChannel *lv_pChannel = PIVRCH[lv_nChannelNo];
			if( lv_pChannel )
			{
				if( lv_pChannel->GetStarted() && lv_pChannel->GetStatus() == IVR_IDLE )
				{
					WriteTrace(TraceInfo, "User OB Test on IVR Channel: %d", lv_nChannelNo);
					lv_pChannel->NF_CallIn();
				}
			}
		}
	}
}
