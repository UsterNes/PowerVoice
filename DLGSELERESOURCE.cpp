// DLGSELERESOURCE.cpp : implementation file
//

#include "stdafx.h"
#include "PowerVoice.h"
#include "MainFrm.h"
#include "DLGSELERESOURCE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDLGSELERESOURCE dialog


CDLGSELERESOURCE::CDLGSELERESOURCE(CWnd* pParent /*=NULL*/)
	: CDialog(CDLGSELERESOURCE::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDLGSELERESOURCE)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nselectitem = -1;
	m_nselectitemdata = 0;
}


void CDLGSELERESOURCE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDLGSELERESOURCE)
	DDX_Control(pDX, IDC_CMB_RESOURCE, m_wndlistbox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDLGSELERESOURCE, CDialog)
	//{{AFX_MSG_MAP(CDLGSELERESOURCE)
	ON_CBN_DBLCLK(IDC_CMB_RESOURCE, OnDblclkCmbResource)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDLGSELERESOURCE message handlers

void CDLGSELERESOURCE::OnOK() 
{
	// TODO: Add extra validation here
	if(AfxMessageBox(IDS_MSG_Q_CHANGE_RES, MB_YESNO | MB_APPLMODAL | MB_ICONQUESTION | MB_DEFBUTTON1) == IDNO)
		return;

	UpdateData(TRUE);

	m_nselectitem = m_wndlistbox.GetCurSel();
	if( m_nselectitem < 0 )
		m_nselectitemdata = 0;
	else
		m_nselectitemdata = (US)m_wndlistbox.GetItemData(m_nselectitem);

	CDialog::OnOK();
}

BOOL CDLGSELERESOURCE::OnInitDialog() 
{
	// Load Waiting Cursor
	HCURSOR hOldCur = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDOK)->EnableWindow(LoadResourceProjectList());

	SetSeletedItemWithCurrentProject();

	// Resume Cursor
	::SetCursor(hOldCur);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDLGSELERESOURCE::SetSeletedItemWithCurrentProject()
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

BOOL CDLGSELERESOURCE::LoadResourceProjectList()
{
	_RecordsetPtr	lv_RS;
	CString lv_item, lv_pname, lv_pdecr, lv_pversoin;
	int lv_pid;
	long lv_loop, lv_RCN;

	// Clear List
	m_wndlistbox.ResetContent();

	// Load Resource Project List from DB

	// 通过 ADO 访问资源文件
	try
	{
		char lv_query[200];
		memset(lv_query, 0x00, sizeof(lv_query));
		sprintf(lv_query, 
			"select * from tbRefIVR where P_Type='R' order by P_ID");
		_bstr_t bstrSQL = lv_query;

		// Connection String
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
		WriteTrace(TraceError, "Read tbResource table fialed!");
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
	}

	lv_RS->Close();
	UpdateData(FALSE);

	return TRUE;
}

void CDLGSELERESOURCE::OnDblclkCmbResource() 
{
	// TODO: Add your control notification handler code here
	OnOK();
}
