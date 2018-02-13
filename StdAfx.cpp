// stdafx.cpp : source file that includes just the standard includes
//	PowerVoice.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

CManulSync::CManulSync ()
{
	m_sync = CreateMutex (NULL, false, NULL);
}

CManulSync::~CManulSync()
{
	if (m_sync != NULL)
	{
		CloseHandle (m_sync);
		m_sync = NULL;
	}
}

void CManulSync::Enter () const
{
	WaitForSingleObject (m_sync, INFINITE);
}

void CManulSync::Leave () const
{
	ReleaseMutex (m_sync);
}

CManualResetEvent::CManualResetEvent()
   :  m_hEvent(INVALID_HANDLE_VALUE)
{
   m_strEventName = _T("");
}

CManualResetEvent::~CManualResetEvent()
{
   if (m_hEvent != INVALID_HANDLE_VALUE)
   {
      ::CloseHandle(m_hEvent);
   }
}

bool CManualResetEvent::Create(TCHAR *f_pName)
{
	if( m_hEvent == INVALID_HANDLE_VALUE )
		m_hEvent = ::CreateEvent(0, TRUE, FALSE, f_pName);

   return (0 != m_hEvent);
}

bool CManualResetEvent::Set()
{
   return (0 != ::SetEvent(m_hEvent));
}

bool CManualResetEvent::Reset()
{
   return (0 != ::ResetEvent(m_hEvent));
}

bool CManualResetEvent::Pulse()
{
   return (0 != ::PulseEvent(m_hEvent));
}

HANDLE CManualResetEvent::GetHANDLE() const
{
   return m_hEvent;
}

void CManualResetEvent::Wait() const
{
   if( !Wait(INFINITE) )
	  WriteTrace(TraceWarn, "Warning : Unexpected timeout on infinite wait - CManualResetEvent::Wait(), name is %s", m_strEventName);
}

bool CManualResetEvent::Wait(DWORD timeoutMillis) const
{
	bool ok;

	DWORD result = AfmWaitWithMessageLoop(1, &m_hEvent, FALSE, timeoutMillis);

	if (result == WAIT_TIMEOUT)
	{
		ok = false;
	}
	else if (result == WAIT_OBJECT_0)
	{
		ok = true;
	}
	else
	{
		ok = false;
		WriteTrace(TraceWarn, "Warning : WaitForSingleObject get error %d - CManualResetEvent::Wait()", ::GetLastError());
	}

	return ok;

	/*
   bool ok;

   DWORD result = ::WaitForSingleObject(m_hEvent, timeoutMillis);

   if (result == WAIT_TIMEOUT)
   {
      ok = false;
   }
   else if (result == WAIT_OBJECT_0)
   {
      ok = true;
   }
   else
   {
	  WriteTrace(TraceWarn, "Warning : WaitForSingleObject(%s) get error %d - CManualResetEvent::Wait()", m_strEventName, ::GetLastError());
   }
    
   return ok;*/
}

CMutexValueManger::CMutexValueManger()
 : m_lngValue(0)
{
}

CMutexValueManger::~CMutexValueManger()
{
}

long CMutexValueManger::GetValue()
{
	long lv_Value;

	Enter();
	lv_Value = m_lngValue;
	Leave();

	return lv_Value;
}

void CMutexValueManger::SetValue(long f_Value)
{
	Enter();
	m_lngValue = f_Value;
	Leave();
}

void CMutexValueManger::IncreaseValue()
{
	Enter();
	m_lngValue++;
	Leave();
}

void CMutexValueManger::DecreaseValue()
{
	Enter();
	m_lngValue--;
	Leave();
}


CMyTimeBomb::CMyTimeBomb()
 :	m_blnEnabled(FALSE),
	m_blnExploded(FALSE)
{
}

CMyTimeBomb::~CMyTimeBomb()
{
}

BOOL CMyTimeBomb::GetEnabled()
{
	return m_blnEnabled;
}

void CMyTimeBomb::SetEnabled(BOOL f_blnValue)
{
	if( m_blnEnabled != f_blnValue )
		m_blnEnabled = f_blnValue;
}

BOOL CMyTimeBomb::GetExploded()
{
	return m_blnExploded;
}

CTime CMyTimeBomb::GetExplodeTime()
{
	return m_tmTimer;
}

void CMyTimeBomb::SetExplodeTime(CTime f_tmTime)
{
	m_tmTimer = f_tmTime;
	m_blnExploded = FALSE;
}

BOOL CMyTimeBomb::CheckTimer()
{
	if( m_blnExploded || !m_blnEnabled )
		return FALSE;

	if( m_tmTimer > CTime::GetCurrentTime() )
		return FALSE;

	m_blnExploded = TRUE;
	return TRUE;
}

long CMyTimeBomb::GetRemainSeconds()
{
	if( m_blnExploded || !m_blnEnabled )
		return -1;

	CTime lv_tmNow = CTime::GetCurrentTime();
	if( m_tmTimer <= lv_tmNow )
		return 0;

	CTimeSpan lv_spn = lv_tmNow - m_tmTimer;

	return (long)lv_spn.GetTotalSeconds();
}


/********************************************************************/
/*																	*/
/* Function name : AfmWaitWithMessageLoop							*/
/* Description   : Pump messages while waiting for event			*/
/*																	*/
/********************************************************************/
//等待消息循环和事件
DWORD AfmWaitWithMessageLoop(DWORD nCount,const HANDLE *pHandles,BOOL bWaitAll, DWORD dwTimeout)
{   
	DWORD dwRet;

    DWORD dwMaxTick = GetTickCount() + dwTimeout;

	while (1)
	{
		DWORD dwWaitTime=dwMaxTick - GetTickCount();
		if (GetTickCount()>dwMaxTick && dwTimeout!=INFINITE) return WAIT_TIMEOUT;
		dwRet = MsgWaitForMultipleObjects(nCount, pHandles, bWaitAll,dwTimeout==INFINITE?INFINITE:dwWaitTime, QS_ALLINPUT);
		if (dwRet == WAIT_OBJECT_0 + nCount)
		{
			// process window messages
			DoEvents();
		}  
		else
		{
			// timed out !
			return dwRet;
		}
	}
}


void DoEvents()
{
	MSG msg;

	// window message         
	while (PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))         
	{            
		TranslateMessage(&msg);            
		DispatchMessage(&msg);         
	}      
}

/********************************************************************/
/*																	*/
/* Function name : BrowseForFolder									*/		
/* Description   : Browse for folder using SHBrowseForFolder.		*/
/*																	*/
/********************************************************************/
CString BrowseForFolder(HWND hWnd, LPCSTR lpszTitle, UINT nFlags)
{
	// We're going to use the shell to display a 
	// "Choose Directory" dialog box for the user.
	CString strResult = "";
  
	LPMALLOC lpMalloc;
	
	if (::SHGetMalloc(&lpMalloc) != NOERROR)
	{
		// failed to get allocator  
		return strResult; 
	}

	char szBuffer[_MAX_PATH];
	char szDisplayName[_MAX_PATH];

	BROWSEINFO browseInfo;
	browseInfo.hwndOwner = hWnd;
	// set root at Desktop
	browseInfo.pidlRoot = NULL; 
	browseInfo.pszDisplayName = szDisplayName;
	browseInfo.lpszTitle = lpszTitle;
	browseInfo.ulFlags = nFlags;
	browseInfo.lpfn = NULL;
	browseInfo.lParam = 0;
	
	LPITEMIDLIST lpItemIDList;

	if ((lpItemIDList = ::SHBrowseForFolder(&browseInfo)) != NULL)
	{
		// Get the path of the selected folder from the item ID list.
		if (::SHGetPathFromIDList(lpItemIDList, szBuffer))
		{
			// At this point, szBuffer contains the path the user chose.
			if (szBuffer[0] == '\0')
			{
				// SHGetPathFromIDList failed, or SHBrowseForFolder failed.
				AfxMessageBox("Failed to get directory", MB_ICONSTOP|MB_OK);
				return strResult;
			}
     
			// We have a path in szBuffer!
			strResult = szBuffer;
			return strResult;
		}
		else
		{
			// The thing referred to by lpItemIDList 
			// might not have been a file system object.
			// For whatever reason, SHGetPathFromIDList didn't work!
			AfxMessageBox("Failed to get directory", MB_ICONSTOP|MB_OK);
			return strResult; // strResult is empty 
		}
		lpMalloc->Free(lpItemIDList);
		lpMalloc->Release();      
	}
	return strResult;
}


/********************************************************************/
/*																	*/
/* Function name : PcsMakeSureDirectoryPathExists					*/
/* Description   : This function creates all the directories in		*/
/*				   the specified DirPath, beginning with the root.	*/
/*				   This is a clone a Microsoft function with the	*/
/*			       same name.										*/
/*																	*/
/********************************************************************/
BOOL PcsMakeSureDirectoryPathExists(LPCTSTR lpszDirPath)
{
	CString strDirPath = lpszDirPath;
	
	int nPos = 0;
   
	while((nPos = strDirPath.Find('\\', nPos+1)) != -1) 
	{
		CreateDirectory(strDirPath.Left(nPos), NULL);
	}
	return CreateDirectory(strDirPath, NULL);
}

/********************************************************************/
/*																	*/
/* Function name : MakeSureFilePathExists  							*/
/* Description   : This function creates all the directories in		*/
/*				   the specified File Path, beginning with the root.*/
/*				   This is a clone a Microsoft function with the	*/
/*			       same name.										*/
/*																	*/
/********************************************************************/
BOOL MakeSureFilePathExists(LPCTSTR lpszDirPath)
{
	int lv_nPos;
	CString lv_sTemp, lv_sPath;

	lv_sTemp = lpszDirPath;
	lv_nPos = lv_sTemp.ReverseFind('\\');
	if( lv_nPos >= 0 )
	{
		lv_sPath = lv_sTemp.Left(lv_nPos);
		return PcsMakeSureDirectoryPathExists(lv_sPath);
	}

	return FALSE;
}

/********************************************************************/
/*																	*/
/* Function name : RemoveFilesInDirectory							*/
/* Description   : This function removes all files in				*/
/*				   the specified DirPath.							*/
/*																	*/
/********************************************************************/
BOOL RemoveFilesInDirectory(LPCTSTR lpszDirPath)
{
	CFileFind find;
	BOOL      bFind;
	CString filepath;
	CStringArray strFileArray;
	long lv_loop, lv_filecount;

	try
	{
		strFileArray.RemoveAll();
		filepath = lpszDirPath;
		bFind = find.FindFile( filepath );
		while ( bFind )
		{
			bFind = find.FindNextFile();
			if ( !find.IsDirectory() )
			{
				filepath = find.GetFilePath();
				strFileArray.Add( filepath );
			}
		}

		find.Close();

		lv_filecount = strFileArray.GetSize(); 
		for( lv_loop = 0; lv_loop < lv_filecount; lv_loop++ )
		{
			DeleteFile( strFileArray[lv_loop] );
		}
	}
	catch(...)
	{
		return FALSE;
	}
	
	return TRUE;
}

/********************************************************************/
/*																	*/
/* Function name : FileExists										*/
/* Description   : Check if file or directory exists				*/
/*																	*/
/********************************************************************/
BOOL FileExists(LPCTSTR lpszFileName, BOOL bIsDirCheck)
{
	// A quick'n'easy way to see if a file exists.
	DWORD dwAttributes = GetFileAttributes(lpszFileName);
    if (dwAttributes == 0xFFFFFFFF)
        return FALSE;

	if ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
	{
		if (bIsDirCheck)
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		if (!bIsDirCheck)
			return TRUE;
		else
			return FALSE;
	}
}

/********************************************************************/
/*																	*/
/* Function name : IsSubDirectory									*/
/* Description   : Check if lpszSubDirectory is sub of lpszDirectory*/
/*																	*/
/********************************************************************/
BOOL IsSubDirectory(LPCTSTR lpszDirectory, LPCTSTR lpszSubDirectory)
{
	CString strDir1 = lpszDirectory;
	CString strDir2 = lpszSubDirectory;

	strDir1.MakeLower();
	strDir2.MakeLower();

	int nLength = strDir1.GetLength();
	if (strDir2.Left(nLength) == strDir1)
		return TRUE;

	return FALSE;
}


/********************************************************************/
/*																	*/
/* Function name : GetFileDate										*/
/* Description   : return file date in unix style					*/
/*																	*/
/********************************************************************/
CString GetFileDate(CFileFind &find)
{
	CString strResult;

	CTime time = CTime::GetCurrentTime();

	find.GetLastWriteTime(time);

	CTimeSpan timeSpan = CTime::GetCurrentTime() - time;

	if (timeSpan.GetDays() > 356)
	{
		strResult = time.Format(" %b %d %Y ");
	}
	else
	{
		strResult.Format(" %s %02d:%02d ", time.Format("%b %d"), time.GetHour(), time.GetMinute());
	}
	return strResult;
}


// 返回文件大小
bool GetFileLen(LPCTSTR strFileName,__int64 *iLen)
{
	HANDLE hFile;
	DWORD dwHigh,dwError,dwLow;
	bool blnReturn;
	__int64 iReturn;
	__int64 iHigh=1<<32;
	
	hFile = CreateFile(strFileName,
						GENERIC_READ,
						FILE_SHARE_READ, 
						NULL,
						OPEN_EXISTING, 
						FILE_FLAG_SEQUENTIAL_SCAN, 
						NULL);
	if (hFile==INVALID_HANDLE_VALUE)
	{
		iReturn=0;
		blnReturn=false;
	}
	else
	{
		dwLow=GetFileSize(hFile,&dwHigh);
		if (dwLow == INVALID_FILE_SIZE 
			&& 
			(dwError = GetLastError()) != NO_ERROR )
		{
			iReturn=0;
			blnReturn=false;
		}
		else
		{
			iReturn = dwHigh*iHigh;

			iReturn+=dwLow;
			blnReturn=true;
		}
		CloseHandle(hFile);
	}
	*iLen=iReturn;
	return blnReturn;
}

CString ToString(int nValue)
{
	CString strResult;
	strResult.Format("%d", nValue);
	return strResult;
}


/********************************************************************/
/*																	*/
/* Function name : GetItemIndexWithPoint							*/
/* Description   : 													*/
/*																	*/
/********************************************************************/
int GetItemIndexWithPoint(CListCtrl *f_wndList, CPoint f_ptPoint)
{
	CRect lv_rect;

	if( f_wndList != NULL )
	{
		for(int lv_Loop = 0; lv_Loop < f_wndList->GetItemCount(); lv_Loop++ )
		{
			f_wndList->GetItemRect(lv_Loop, lv_rect, LVIR_BOUNDS);
			f_wndList->ClientToScreen(lv_rect);
			if( lv_rect.PtInRect(f_ptPoint) )
			{
				return(lv_Loop);
			}
		}
	}

	return -1;
}

/********************************************************************/
/*																	*/
/* Function name : AutoSizeColumns									*/
/* Description   : 													*/
/*																	*/
/********************************************************************/
void AutoSizeColumns(CListCtrl *pListCtrl) 
{
	// Call this after your the control is filled
	pListCtrl->SetRedraw(FALSE);
	int mincol = 0;
    int maxcol = pListCtrl->GetHeaderCtrl()->GetItemCount()-1;
    for (int col = mincol; col <= maxcol; col++) 
	{
		pListCtrl->SetColumnWidth(col, LVSCW_AUTOSIZE);
        int wc1 = pListCtrl->GetColumnWidth(col);
        pListCtrl->SetColumnWidth(col, LVSCW_AUTOSIZE_USEHEADER);
        int wc2 = pListCtrl->GetColumnWidth(col);
        // 10 is minumim column width
		int wc = max(10, max(wc1,wc2));
        pListCtrl->SetColumnWidth(col,wc);
     }
     pListCtrl->SetRedraw(TRUE);
}

/************************************************************************/
/*																		*/
/* Function name : CopyToCString										*/
/* Description   : Copy a substring to a CString reference				*/ 
/*																		*/
/************************************************************************/
void CopyToCString(CString& string, LPCSTR pStart, LPCSTR pEnd)
{
	ASSERT( pStart != NULL );
	ASSERT( pEnd != NULL );

	int nLength = (int)(pEnd-pStart);

	if( nLength <= 0 )
	{
		string.Empty();
	}
	else
	{
		LPSTR pszBuffer = string.GetBuffer(nLength);
		memcpy(pszBuffer, pStart, nLength*sizeof(TCHAR));
		string.ReleaseBuffer(nLength);
	}
}

CString AddTailBackslashToPath(CString f_sPath)
{
	CString lv_sDir;

	lv_sDir = f_sPath;
	if( lv_sDir.GetLength() > 0 )
	{
		if( lv_sDir.GetAt(lv_sDir.GetLength() - 1) != '\\' )
			lv_sDir += _T("\\");
	}

	return lv_sDir;
}

// Write string buffer to a named text file
BOOL WriteBufferToTextFile(LPCSTR lpszPath, LPCSTR lpszData, BOOL bOverWrite)
{	
	CFileException ex;
	CStdioFile lv_hFile;

	if( FileExists(lpszPath, FALSE) && !bOverWrite )
		return FALSE;

	try
	{
		if( !lv_hFile.Open(lpszPath, CFile::modeWrite | CFile::modeCreate, &ex) )
		{
			TCHAR szError[1024];
			ex.GetErrorMessage(szError, 1024);
			TRACE("Open file:%s error:%s\n", lpszPath, szError);
			return FALSE;
		}
		else
		{
			lv_hFile.WriteString(lpszPath);
			lv_hFile.Close();
		}
	}
	catch(...)
	{
		return FALSE;
	}

	return TRUE;
}

CString SeperateTelNo(LPCSTR lpszTelNo)
{
	int lv_loop, lv_subloop = 0;
	int lv_varlength = strlen(lpszTelNo);
	char lv_sTemp[50];
	char lv_char;

	memset(lv_sTemp, 0x00, sizeof(lv_sTemp));
	for( lv_loop = 0; lv_loop < lv_varlength; lv_loop++ )
	{
		lv_char = lpszTelNo[lv_loop];
		if( (lv_char > '9' || lv_char < '0') && lv_char != '-' )
			break;
		
		lv_sTemp[lv_subloop++] = lv_char;
	}

	return lv_sTemp;
}

BOOL ConvertStringToCTime(LPCSTR f_sYYYYmmDDHHMMSS, CTime &f_tmTime)
{
	BOOL lv_retval = FALSE;
	CString lv_sTemp = f_sYYYYmmDDHHMMSS;
	int lv_nYear, lv_nMonth, lv_nDay, lv_nHour, lv_nMin, lv_nSec;

	if( lv_sTemp.GetLength() >= 14 )
	{
		lv_nYear = atoi((const char *)lv_sTemp.Left(4));
		lv_nMonth = atoi((const char *)lv_sTemp.Mid(4, 2));
		lv_nDay = atoi((const char *)lv_sTemp.Mid(6, 2));
		lv_nHour = atoi((const char *)lv_sTemp.Mid(8, 2));
		lv_nMin = atoi((const char *)lv_sTemp.Mid(10, 2));
		lv_nSec = atoi((const char *)lv_sTemp.Mid(12, 2));

		if( lv_nYear > 1000 && lv_nYear < 9999 && 
			lv_nMonth >= 1 && lv_nMonth <= 12 && 
			lv_nDay >= 1 && lv_nDay <= 31 &&
			lv_nHour >= 0 && lv_nHour <= 23 &&
			lv_nMin >= 0 && lv_nMin <= 59 &&
			lv_nSec >= 0 && lv_nSec <= 59 )
		{
			f_tmTime = CTime::CTime(lv_nYear, lv_nMonth, lv_nDay, lv_nHour, lv_nMin, lv_nSec);
			lv_retval = TRUE;
		}
	}

	return lv_retval;
}

// YYYY-mm-DD HH:MM:SS
BOOL ConvertStringLongToCTime(LPCSTR f_sLongDate, CTime &f_tmTime)
{
	BOOL lv_retval = FALSE;
	CString lv_sTemp = f_sLongDate;
	CString lv_sDate, lv_sTime;
	int lv_nLen, lv_nPos, lv_nPosTwo;
	int lv_nYear = 0, lv_nMonth = 0, lv_nDay = 0;
	int lv_nHour = 0, lv_nMin = 0, lv_nSec = 0;

	lv_nPos = lv_sTemp.Find(' ');
	if( lv_nPos > 0 )
	{
		// 拆分日期和时间
		lv_sDate = lv_sTemp.Left(lv_nPos);
		lv_sTime = lv_sTemp.Mid(lv_nPos + 1);
		if( lv_sDate.GetLength() > 0 && lv_sTime.GetLength() > 0 )
		{
			// 日期分解
			lv_nPos = lv_sDate.Find('-');
			lv_nPosTwo = lv_sDate.ReverseFind('-');
			if( lv_nPos > 0 && lv_nPosTwo > lv_nPos )
			{
				lv_nLen = lv_sDate.GetLength();
				lv_nYear = atoi((const char *)lv_sDate.Left(lv_nPos));
				lv_nMonth = atoi((const char *)lv_sDate.Mid(lv_nPos + 1, lv_nPosTwo - lv_nPos - 1));
				lv_nDay = atoi((const char *)lv_sDate.Mid(lv_nPosTwo + 1));
			}

			// 时间分解
			lv_nPos = lv_sTime.Find(':');
			lv_nPosTwo = lv_sTime.ReverseFind(':');
			if( lv_nPos > 0 && lv_nPosTwo > lv_nPos )
			{

				lv_nHour = atoi((const char *)lv_sTime.Left(lv_nPos));
				lv_nMin = atoi((const char *)lv_sTime.Mid(lv_nPos + 1, lv_nPosTwo - lv_nPos - 1));
				lv_nSec = atoi((const char *)lv_sTime.Mid(lv_nPosTwo+ 1));
			}

			// 生成时间类型
			if( lv_nYear > 1000 && lv_nYear < 9999 && 
				lv_nMonth >= 1 && lv_nMonth <= 12 && 
				lv_nDay >= 1 && lv_nDay <= 31 &&
				lv_nHour >= 0 && lv_nHour <= 23 &&
				lv_nMin >= 0 && lv_nMin <= 59 &&
				lv_nSec >= 0 && lv_nSec <= 59 )
			{
				f_tmTime = CTime::CTime(lv_nYear, lv_nMonth, lv_nDay, lv_nHour, lv_nMin, lv_nSec);
				lv_retval = TRUE;
			}
		}
	}

	return lv_retval;
}

// 年月的最后一天
int GetLastDayOfMonth(int iYear, int iMonth)
{
	int lv_nMonth = iMonth % 13;
	int iMaxDay;
	int arrMonth[13];

	arrMonth[0] = 31;
	arrMonth[1] = 31;
	arrMonth[2] = 28;
	arrMonth[3] = 31;
	arrMonth[4] = 30;
	arrMonth[5] = 31;
	arrMonth[6] = 30;
	arrMonth[7] = 31;
	arrMonth[8] = 31;
	arrMonth[9] = 30;
	arrMonth[10] = 31;
	arrMonth[11] = 30;
	arrMonth[12] = 31;
	
	if( ( lv_nMonth == 2 ) && 
			( iYear % 400 == 0 || ( iYear % 4 == 0 && iYear % 100 != 0 ) ) )
		iMaxDay = 29;
	else
		iMaxDay = arrMonth[ lv_nMonth ];

	return iMaxDay;
}

BOOL IsValidDate(int iYear, int iMonth, int iDay)
{
	int iMaxDay;
	
	if( iYear < 0 || iYear > 2100 )
		return FALSE;

	if( iMonth < 1 || iMonth > 12 )
		return FALSE;

	iMaxDay = GetLastDayOfMonth(iYear, iMonth);

	if ( iDay < 1 || iDay > iMaxDay )
		return FALSE;

	return TRUE;
}

// 将一个字符串拆分为数组
void Split(LPCTSTR strInput, LPCTSTR strDelimit, CStringArray *arrReturn)
{
	CString strToken;
	CString strCopy;

	arrReturn->RemoveAll();

	strCopy=strInput;
	int iBegin=0,iEnd=0;
	iEnd=strCopy.Find(strDelimit,iBegin);
	while (iEnd>=0)
	{
		if (iEnd==iBegin)
			strToken="";
		else
			strToken=strCopy.Mid(iBegin,iEnd-iBegin);
		arrReturn->Add(strToken);
		iBegin=iEnd+1;
		iEnd=strCopy.Find(strDelimit,iBegin);
	}
	if (strCopy.GetLength()==iBegin)
		strToken="";
	else
		strToken=strCopy.Right(strCopy.GetLength()-iBegin);

	arrReturn->Add(strToken);
}

// 返回给定时间长度的字符串格式
/// "%S", "%M:%S", "%H:%M:%S"
CString TimeDurationFormat(long lngDuration, LPCSTR strFormat)
{
	CString lv_strCaption, lv_strTemp;
	long lv_lngHour, lv_lngMinute;

	lv_strTemp = strFormat;
	lv_strTemp.MakeUpper();

	if( strFormat == "%M:%S" )
		lv_strCaption.Format("%d:%d", long(lngDuration / 60), lngDuration % 60);
	else if( strFormat == "%H:%M:%S" )
	{
		lv_lngHour = long(lngDuration / 3600);
		lv_lngMinute = lngDuration % 3600; 
		lv_strCaption.Format("%d:%d:%d", lv_lngHour, long(lv_lngMinute / 60), lv_lngMinute % 60);
	}
	else
		lv_strCaption.Format("%d", lngDuration);

	return lv_strCaption;
}