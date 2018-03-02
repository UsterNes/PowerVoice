// MySQLLog.cpp: implementation of the CMySQLLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MySQLLog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMySQLLog::CMySQLLog()
{
	// Sun added 2006-12-06, [C]
	m_nMaxFileRows = 0;
	m_nRowCount = 0;

	m_hSQLFile = NULL;
	m_strSQLFilePrefix = _T("");
	m_strRemoFlag = _T("--");
}

CMySQLLog::~CMySQLLog()
{
	CloseSQLFile();
}

// Sun added 2006-12-06, [C]
UINT CMySQLLog::GetMaxFileRows()
{
	return m_nMaxFileRows;
}

// Sun added 2006-12-06, [C]
void CMySQLLog::SetMaxFileRows(UINT nValue)
{
	if( m_nMaxFileRows != nValue )
		m_nMaxFileRows = nValue;
}

void CMySQLLog::SetFilePrefix(LPCTSTR strFilePrefix)
{
	// close existing sql file first
	CloseSQLFile();

	// copy the file name prefix
	m_strSQLFilePrefix = strFilePrefix;
	if( m_strSQLFilePrefix.GetLength() == 0 )
		m_strSQLFilePrefix = _T("Lost");
}

CString CMySQLLog::GetRemoFlag()
{
	return m_strRemoFlag;
}

void CMySQLLog::SetRemoFlag(LPCTSTR strFlagString)
{
	m_strRemoFlag = strFlagString;
	if( m_strRemoFlag.GetLength() == 0 )
		m_strRemoFlag = _T("--");
}

// close the current sql file
void CMySQLLog::CloseSQLFile()
{
	if( m_hSQLFile )
	{
		::CloseHandle(m_hSQLFile);
		m_hSQLFile = NULL;
	}
}

// open a new sql file
HANDLE CMySQLLog::OpenSQLFile()
{
	// construct the new sql file path
	TCHAR strFilePath[1001];
	SYSTEMTIME sysTime;
	::GetLocalTime(&sysTime);
	_stprintf
	(
		strFilePath,
		_T("%s_%04d%02d%02d_%02d%02d%02d_%X.sql"),
		m_strSQLFilePrefix,
		sysTime.wYear,
		sysTime.wMonth,
		sysTime.wDay,
		sysTime.wHour,
		sysTime.wMinute,
		sysTime.wSecond,
		::GetCurrentProcessId()
	);
	
	// create the new sql file
	m_hSQLFile = CreateFile
	(
		strFilePath,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	// if successful, save the start time variable
	if( m_hSQLFile )
		m_timeStart = sysTime;

	// Sun added 2006-12-06
	m_nRowCount = 0;

	// return the file handle
	return m_hSQLFile;
}

void CMySQLLog::WriteToFile(LPCTSTR strSQL, int nLen)
{
	m_bLock.Enter();

	try
	{
		bool lv_blnDateSeperator = false;

		// open the sql file if not already open
		if( m_hSQLFile == NULL )
		{
			m_hSQLFile = OpenSQLFile();
			lv_blnDateSeperator = true;
		}
		else
		{
			// get local time
			SYSTEMTIME sysTime;
			::GetLocalTime(&sysTime);

			if( sysTime.wYear != m_timeStart.wYear || sysTime.wMonth != m_timeStart.wMonth || sysTime.wDay != m_timeStart.wDay )
			{
				m_timeStart = sysTime;
				lv_blnDateSeperator = true;
			}
		}

		// write the sql message
		if( m_hSQLFile )
		{
			// declare buffer
			TCHAR pBuffer[2048];
			DWORD dwBytes;

			// Insert Date Seperator
			if( lv_blnDateSeperator )
			{
				memset(pBuffer, 0x00, sizeof(pBuffer));
				_stprintf(pBuffer, _T("%s Date[%04d-%02d-%02d] %s\r\n"), 
							m_strRemoFlag,
							m_timeStart.wYear,
							m_timeStart.wMonth,
							m_timeStart.wDay,
							m_strRemoFlag);
				::WriteFile(m_hSQLFile, pBuffer, _tcslen(pBuffer),&dwBytes,NULL);
			}

			// write the buffer to file
			_sntprintf(pBuffer, nLen, _T("%s"), strSQL);
			pBuffer[nLen] = '\r';
			pBuffer[nLen+1] = '\n';
			::WriteFile(m_hSQLFile,pBuffer,nLen + 2,&dwBytes,NULL);

			//---------------------------------------
			// Sun added 2006-12-06
			/// 是否达到最大行数限制
			m_nRowCount++;
			if( m_nMaxFileRows > 0 && m_nRowCount >= m_nMaxFileRows )
				CloseSQLFile();
			//----------------------------------------
		}
	}
	catch(...)
	{
		// add code to handle exception (if needed)
	}

	m_bLock.Leave();
}

//========================================================