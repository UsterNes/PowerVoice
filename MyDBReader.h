// MyDBReader.h: interface for the CMyDBReader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYDBREADER_H__C1D4D90A_1F24_45B1_99C1_F4FB48730C01__INCLUDED_)
#define AFX_MYDBREADER_H__C1D4D90A_1F24_45B1_99C1_F4FB48730C01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MYDB_CONNTYPE_OLEDB				0
#define MYDB_CONNTYPE_ODBC				1
#define MYDB_CONNTYPE_UNKNOWN			2

class CMyDBReader  
{
public:
	CMyDBReader();
	virtual ~CMyDBReader();

	void SetDBConnectType(UINT f_type = MYDB_CONNTYPE_OLEDB);
	void SetDataSource(const CString f_str);
	void SetDBName(const CString f_str);
	void SetUserName(const CString f_str);
	void SetPassword(const CString f_str);

	UINT GetDBConnectType();
	CString GetDataSource();
	CString GetDBName();
	CString GetUserName();
	CString GetPassword();

	CString GetConnectionString();

	BOOL OpenADORecordset(_RecordsetPtr f_pRS, _bstr_t f_SQL);
	BOOL GotherIVRNodeDataSmall(_RecordsetPtr f_pRS, BYTE *f_pData);
	BOOL GotherIVRNodeDataBig(_RecordsetPtr f_pRS, BYTE *f_pData);

private:
	/// Connection Type
	UINT m_nDBConnectType;

	/// DB Server / DSN
	CString m_sDBServer;

	/// Database Name
	CString m_sDBName;
	
	/// User Name
	CString m_sDBUserName;

	/// Password
	CString m_sDBPassword;
};

#endif // !defined(AFX_MYDBREADER_H__C1D4D90A_1F24_45B1_99C1_F4FB48730C01__INCLUDED_)
