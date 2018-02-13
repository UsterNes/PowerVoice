// DlgAutoLoadCallFlow.cpp : implementation file
//

#include "stdafx.h"
#include "PowerVoice.h"
#include "DlgAutoLoadCallFlow.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoLoadCallFlow dialog


CDlgAutoLoadCallFlow::CDlgAutoLoadCallFlow(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAutoLoadCallFlow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAutoLoadCallFlow)
	m_tmDate = 0;
	m_tmTime = 0;
	m_strFileName = _T("");
	m_optSource = -1;
	//}}AFX_DATA_INIT
	m_nselectitem = -1;
	m_nselectitemdata = 0;
	m_blnEnabled = FALSE;
	strFileNameArray.RemoveAll();
}


void CDlgAutoLoadCallFlow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAutoLoadCallFlow)
	DDX_Control(pDX, IDC_CMB_CALLFLOW, m_wndlistbox);
	DDX_Control(pDX, IDC_GRP_ENABLED, m_cgbEnabled);
	DDX_DateTimeCtrl(pDX, IDC_LOG_DAY_START, m_tmDate);
	DDX_DateTimeCtrl(pDX, IDC_LOG_TIME_START, m_tmTime);
	DDX_Text(pDX, IDC_TXT_FILE, m_strFileName);
	DDX_Radio(pDX, IDC_OPT_SOURCE, m_optSource);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAutoLoadCallFlow, CDialog)
	//{{AFX_MSG_MAP(CDlgAutoLoadCallFlow)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_BROWSE, OnBtnBrowse)
	ON_BN_CLICKED(IDC_OPT_SOURCE, OnOptSource)
	ON_BN_CLICKED(IDC_OPT_SOURCE2, OnOptSource)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAutoLoadCallFlow message handlers
BOOL CDlgAutoLoadCallFlow::OnInitDialog() 
{
	// Load Waiting Cursor
	HCURSOR hOldCur = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if( m_nselectitemdata == DEF_LD_GRP_ET_FILE )
	{
		m_optSource = 0;
		GetDlgItem(IDC_TXT_FILE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_BROWSE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMB_CALLFLOW)->EnableWindow(FALSE);
	}
	else
	{
		m_optSource = 1;
		GetDlgItem(IDC_TXT_FILE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_BROWSE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMB_CALLFLOW)->EnableWindow(TRUE);
	}

	m_cgbEnabled.InitCheckBox();
	m_cgbEnabled.SetCheck(m_blnEnabled);
	GetDlgItem(IDOK)->EnableWindow(LoadCallFlowProjectList());

	SetSeletedItemWithCurrentProject();

	// Resume Cursor
	::SetCursor(hOldCur);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAutoLoadCallFlow::OnOK() 
{
	// TODO: Add extra validation here
	if(AfxMessageBox(IDS_MSG_Q_UPDATE_SCH_CF, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION | MB_DEFBUTTON1) == IDNO)
		return;

	UpdateData(TRUE);

	if( m_optSource == 0 )
	{
		m_nselectitem = -1;
		m_nselectitemdata = DEF_LD_GRP_ET_FILE;
	}
	else
	{
		m_nselectitem = m_wndlistbox.GetCurSel();
		if( m_nselectitem < 0 )
			m_nselectitemdata = 0;
		else
			m_nselectitemdata = (US)m_wndlistbox.GetItemData(m_nselectitem);
	}
	m_blnEnabled = m_cgbEnabled.GetCheck();
	
	CDialog::OnOK();
}

void CDlgAutoLoadCallFlow::SetSeletedItemWithCurrentProject()
{
	int lv_loop;

	if( m_nselectitem >= 0 )
	{
		for( lv_loop = 0; lv_loop < m_wndlistbox.GetCount(); lv_loop++ )
		{
			if( m_wndlistbox.GetItemData(lv_loop) == m_nselectitemdata )
			{
				m_wndlistbox.SetCurSel(lv_loop);
				break;
			}
		}
	}
}

BOOL CDlgAutoLoadCallFlow::LoadCallFlowProjectList()
{
	_RecordsetPtr	lv_RS;
	CString lv_item, lv_pname, lv_pdecr, lv_pversoin;
	int lv_pid;
	long lv_loop, lv_RCN;

	// Clear List
	m_wndlistbox.ResetContent();
	strFileNameArray.RemoveAll();

	// Load Call Flow Project List from DB

	try
	{
		char lv_query[200];
		memset(lv_query, 0x00, sizeof(lv_query));
		sprintf(lv_query, 
			"select * from tbRefIVR where P_Type='P' order by P_ID");
		_bstr_t bstrSQL = lv_query;

		// Connection String 2004-1-17 by steven
		_bstr_t bstrLocalDSN = pMainWindow->m_DBReader.GetConnectionString();
		if( bstrLocalDSN.length() == 0 )
			return FALSE;

		lv_RS.CreateInstance(__uuidof(Recordset));
		lv_RS->CursorType = adOpenStatic;

		// 返回结果集
		lv_RS->CursorLocation = adUseClient;
		lv_RS->Open(bstrSQL, bstrLocalDSN, adOpenKeyset, adLockReadOnly, adCmdText);
	}
	catch(...)
	{
		//outmsg(MSG_SHOW3, "Read tbResource table fialed!");
		return FALSE;
	}
	
	lv_RCN = lv_RS->RecordCount;
	for( lv_loop = 0; lv_loop < lv_RCN; lv_loop++ )
	{
		// 读取保存
		lv_pid = lv_RS->Fields->Item["P_ID"]->Value.iVal;
		try
		{
			lv_pname.Empty();
			lv_pdecr.Empty();
			lv_pversoin.Empty();
			if( lv_RS->Fields->Item["P_Name"]->ActualSize > 0 )
			{
				lv_pname = lv_RS->Fields->Item["P_Name"]->Value.bstrVal;
				//lv_pname.TrimRight(lv_pname);
				lv_pname = lv_pname.Left(lv_pname.Find(' '));
			}
			if( lv_RS->Fields->Item["P_Description"]->ActualSize > 0 )
			{
				lv_pdecr = lv_RS->Fields->Item["P_Description"]->Value.bstrVal;
				//lv_pdecr.TrimRight(lv_pdecr);
			}
			if( lv_RS->Fields->Item["P_Version"]->ActualSize > 0 )
			{
				lv_pversoin = lv_RS->Fields->Item["P_Version"]->Value.bstrVal;
				//lv_pversoin.TrimRight(lv_pversoin);
				lv_pversoin = lv_pversoin.Left(lv_pversoin.Find(' '));
			}
			lv_RS->MoveNext();
		}
		catch(...)
		{
		}
		lv_item.Format("ID:%3d  -  %s    %s    %s", lv_pid, lv_pname, lv_pversoin, lv_pdecr);

		m_wndlistbox.InsertString(lv_loop, lv_item);
		m_wndlistbox.SetItemData(lv_loop, lv_pid);
		strFileNameArray.Add(lv_pname);
	}

	lv_RS->Close();
	UpdateData(FALSE);

	return TRUE;
}

void CDlgAutoLoadCallFlow::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	strFileNameArray.RemoveAll();
}

void CDlgAutoLoadCallFlow::OnBtnBrowse() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlgOpen (TRUE,  "*.pvc",  NULL, OFN_HIDEREADONLY, "PowerVoice Callflow Files (*.pvc)|*.pvc|");
	if( IDOK == dlgOpen.DoModal() ) 
	{
		m_strFileName = dlgOpen.GetPathName();
		UpdateData(FALSE);
	}
}

void CDlgAutoLoadCallFlow::OnOptSource() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if( m_optSource == 0 )
	{
		GetDlgItem(IDC_TXT_FILE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_BROWSE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CMB_CALLFLOW)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_TXT_FILE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_BROWSE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CMB_CALLFLOW)->EnableWindow(TRUE);
	}
}
