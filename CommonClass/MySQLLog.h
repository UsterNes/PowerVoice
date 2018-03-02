// MySQLLog.h: interface for the CMySQLLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSQLLOG_H__4DD3529E_AF8F_4A27_B88D_9FE75BE21D73__INCLUDED_)
#define AFX_MYSQLLOG_H__4DD3529E_AF8F_4A27_B88D_9FE75BE21D73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMySQLLog  
{
public:
	CMySQLLog();
	virtual ~CMySQLLog();

	// File Operations
	HANDLE OpenSQLFile();
	void CloseSQLFile();
	void SetFilePrefix(LPCTSTR strFilePrefix);
	CString GetRemoFlag();
	void SetRemoFlag(LPCTSTR strFlagString);
	void WriteToFile(LPCTSTR strSQL, int nLen);

	// Sun added 2006-12-06, [C]
	UINT GetMaxFileRows();
	void SetMaxFileRows(UINT nValue);

private:
	CManulSync				m_bLock;

	SYSTEMTIME				m_timeStart;
	CString					m_strSQLFilePrefix;
	CString					m_strRemoFlag;
	HANDLE					m_hSQLFile;

	// Sun added 2006-12-06, [C]
	/// 日志文件的最大行数，0表示无限制
	UINT					m_nMaxFileRows;
	/// 日志文件当前行数
	UINT					m_nRowCount;
};

#endif // !defined(AFX_MYSQLLOG_H__4DD3529E_AF8F_4A27_B88D_9FE75BE21D73__INCLUDED_)
