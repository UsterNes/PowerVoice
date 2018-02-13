// MyDBReader.cpp: implementation of the CMyDBReader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainFrm.h"
#include "MyDBReader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define SQLConnectionString(ServerName, DBName) \
	    "Provider=SQLOLEDB;Integrated Security=SSPI;Data Source=" + ServerName + ";Initial Catalog=" + DBName

#define ODBCConnectionString(pDSNName, DBName, pUserID, pPassword) \
	    "DSN=" + pDSNName + ";User ID=" + pUserID + ";Password=" + pPassword + ";Initial Catalog=" + DBName

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyDBReader::CMyDBReader()
 :	m_nDBConnectType(0),
	m_sDBServer(_T("")),
	m_sDBName(_T("")),
	m_sDBUserName(_T("")),
	m_sDBPassword(_T(""))
{
}

CMyDBReader::~CMyDBReader()
{
}

CString CMyDBReader::GetConnectionString()
{
	CString lv_strLocalDSN;

	if( m_nDBConnectType == MYDB_CONNTYPE_OLEDB )
		lv_strLocalDSN = SQLConnectionString(m_sDBServer, m_sDBName);
	else  if( m_nDBConnectType == MYDB_CONNTYPE_ODBC )
		lv_strLocalDSN = ODBCConnectionString(m_sDBServer, m_sDBName, m_sDBUserName, m_sDBPassword);
	else
		lv_strLocalDSN = _T("");

	return lv_strLocalDSN;
}

BOOL CMyDBReader::OpenADORecordset(_RecordsetPtr f_pRS, _bstr_t f_SQL)
{
	_bstr_t			bstrLocalDSN;
	bstrLocalDSN = GetConnectionString();

	if( bstrLocalDSN.length() == 0 )
		return FALSE;

	f_pRS->Open(f_SQL, bstrLocalDSN, adOpenKeyset, adLockReadOnly, adCmdText);

	return TRUE;
}

void CMyDBReader::SetDBConnectType(UINT f_type)
{
	m_nDBConnectType = f_type;
}

void CMyDBReader::SetDataSource(const CString f_str)
{
	m_sDBServer = f_str;
}

void CMyDBReader::SetDBName(const CString f_str)
{
	m_sDBName = f_str;
}

void CMyDBReader::SetUserName(const CString f_str)
{
	m_sDBUserName = f_str;
}

void CMyDBReader::SetPassword(const CString f_str)
{
	m_sDBPassword = f_str;
}

UINT CMyDBReader::GetDBConnectType()
{
	return m_nDBConnectType;
}

CString CMyDBReader::GetDataSource()
{
	return m_sDBServer;
}

CString CMyDBReader::GetDBName()
{
	return m_sDBName;
}

CString CMyDBReader::GetUserName()
{
	return m_sDBUserName;
}

CString CMyDBReader::GetPassword()
{
	return m_sDBPassword;
}

BOOL CMyDBReader::GotherIVRNodeDataSmall(_RecordsetPtr f_pRS, BYTE *f_pData)
{
	int lv_loop;
	char lv_fieldname[20];

	memset(f_pData, 0x00, DEF_NODE_DATA_SMALL_LEN);

	for( lv_loop = 0; lv_loop < DEF_NODE_DATA_SMALL_LEN; lv_loop++ )
	{
		memset(lv_fieldname, 0x00, sizeof(lv_fieldname));
		sprintf(lv_fieldname, "N_D1_%u", lv_loop + 1);
		if( f_pRS->Fields->Item[lv_fieldname]->ActualSize > 0 )
			f_pData[lv_loop] = (BYTE)f_pRS->Fields->Item[lv_fieldname]->Value.iVal;
	}

	return TRUE;
}

BOOL CMyDBReader::GotherIVRNodeDataBig(_RecordsetPtr f_pRS, BYTE *f_pData)
{
	int lv_loop;
	char lv_fieldname[20];

	memset(f_pData, 0x00, DEF_NODE_DATA_BIG_LEN);

	for( lv_loop = 0; lv_loop < DEF_NODE_DATA_BIG_LEN; lv_loop++ )
	{
		memset(lv_fieldname, 0x00, sizeof(lv_fieldname));
		sprintf(lv_fieldname, "N_D2_%u", lv_loop + 1);
		if( f_pRS->Fields->Item[lv_fieldname]->ActualSize > 0 )
			f_pData[lv_loop] = (BYTE)f_pRS->Fields->Item[lv_fieldname]->Value.iVal;
	}

	return TRUE;
}
