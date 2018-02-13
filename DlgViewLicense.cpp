// DlgViewLicense.cpp : implementation file
//

#include "stdafx.h"
#include "PowerVoice.h"
#include "DlgViewLicense.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgViewLicense dialog


CDlgViewLicense::CDlgViewLicense(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgViewLicense::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgViewLicense)
	m_strLicense1 = _T("");
	m_strLicense2 = _T("");
	m_strLicense3 = _T("");
	m_strLicense4 = _T("");
	m_strLicDateTime = _T("");
	m_strProductName = _T("");
	m_strLicServerHDSN = _T("");
	m_strLicServerMAC = _T("");
	m_strLicUID = _T("");
	m_strThisServerHDSN = _T("");
	m_strThisServerMAC = _T("");
	//}}AFX_DATA_INIT
}


void CDlgViewLicense::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgViewLicense)
	DDX_Text(pDX, IDC_ST_LICENSE1, m_strLicense1);
	DDX_Text(pDX, IDC_ST_LICENSE2, m_strLicense2);
	DDX_Text(pDX, IDC_ST_LICENSEDATE, m_strLicDateTime);
	DDX_Text(pDX, IDC_ST_ProdutcName, m_strProductName);
	DDX_Text(pDX, IDC_ST_SERVERHDSN, m_strLicServerHDSN);
	DDX_Text(pDX, IDC_ST_SERVERMAC, m_strLicServerMAC);
	DDX_Text(pDX, IDC_ST_UserIdentification, m_strLicUID);
	DDX_Text(pDX, IDC_TXT_SERVERHDSN, m_strThisServerHDSN);
	DDX_Text(pDX, IDC_TXT_SERVERMAC, m_strThisServerMAC);
	DDX_Text(pDX, IDC_ST_LICENSE3, m_strLicense3);
	DDX_Text(pDX, IDC_ST_LICENSE4, m_strLicense4);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgViewLicense, CDialog)
	//{{AFX_MSG_MAP(CDlgViewLicense)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgViewLicense message handlers

BOOL CDlgViewLicense::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
// Sun added 2013-04-25
#ifndef SMARTAC_LICENSE
	theApp.m_License.InitializeProductLicense(theApp.m_sAppPath, THEAPP_NAME);
#else
	SMARTAC_LICENSE->InitializeProductLicense(theApp.m_sAppPath, THEAPP_NAME);
#endif

// Sun added 2013-04-25
#ifndef SMARTAC_LICENSE
	m_strProductName.Format( "%s%s", theApp.m_License.m_strProductName, 
		(theApp.m_License.m_bytLicenseType != 1 ? _T(" Trial Copy") : _T("")) );
	m_strLicUID = theApp.m_License.m_strLicenseUID;
	m_strLicDateTime.Format("%s %s", theApp.m_License.m_strLicenseDate, 
									theApp.m_License.m_strLicenseTime);

	m_strLicServerMAC = theApp.m_License.m_strServerMacAddress;
	m_strLicServerHDSN = theApp.m_License.m_strServerHDSerialNumber;

	m_strLicense1.Format("%d", theApp.m_License.GetLicenseClient(PCS_LIC_IVR_GROUP));
	m_strLicense2.Format("%d", theApp.m_License.GetLicenseClient(PCS_LIC_IVR_CHANNEL));
	m_strLicense3.Format("%d", theApp.m_License.GetLicenseClient(PCS_LIC_TTS_CONNECT));
	m_strLicense4.Format("%d", theApp.m_License.GetLicenseClient(PCS_LIC_ASR_CONNECT));
#else
	m_strProductName.Format( "%s%s", SMARTAC_LICENSE->m_strProductName, 
		(SMARTAC_LICENSE->m_bytLicenseType != 1 ? _T(" Trial Copy") : _T("")) );
	m_strLicUID = SMARTAC_LICENSE->m_strLicenseUID;
	m_strLicDateTime.Format("%s %s", SMARTAC_LICENSE->m_strLicenseDate, 
									SMARTAC_LICENSE->m_strLicenseTime);

	m_strLicServerMAC = SMARTAC_LICENSE->m_strServerMacAddress;
	m_strLicServerHDSN = SMARTAC_LICENSE->m_strServerHDSerialNumber;

	m_strLicense1.Format("%d", SMARTAC_LICENSE->GetLicenseClient(PCS_LIC_IVR_GROUP));
	m_strLicense2.Format("%d", SMARTAC_LICENSE->GetLicenseClient(PCS_LIC_IVR_CHANNEL));
	m_strLicense3.Format("%d", SMARTAC_LICENSE->GetLicenseClient(PCS_LIC_TTS_CONNECT));
	m_strLicense4.Format("%d", SMARTAC_LICENSE->GetLicenseClient(PCS_LIC_ASR_CONNECT));
#endif

	m_strThisServerMAC = theApp.m_strThisMacAddress;
	m_strThisServerHDSN = theApp.m_strThisHDSerialNumber;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
