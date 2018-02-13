// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__DC938324_AA56_4C55_8944_75D1025185D8__INCLUDED_)
#define AFX_STDAFX_H__DC938324_AA56_4C55_8944_75D1025185D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _DEBUG
//{,,msvcr71d.dll}_crtBreakAlloc
#define CRTDBG_MAP_ALLOC
#endif

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC socket extensions

#include <comdef.h>
#import "c:\program files\common files\system\ado\msado15.dll" \
	no_namespace \
	rename ("EOF", "adoEOF")

// VC++7 Library
#ifndef VC7_LIB
#define VC7_LIB
#endif

// 互斥预编译头
#ifdef INTEL_DIALOGIC
#undef CISCO_CCM
#undef CISCO_CME
#endif

// Sun added 2010-03-18
#include ".\PicaCommon\PicaSoftHead.h"
using namespace PicaSoft;

// Sun added 2008-05-19
#define WM_SYS_REINIT_DEVICE	WM_USER+101

// Sun added 2011-04-16
#define WM_SYS_AUTO_SHUTDOWN	WM_USER+911

#define WM_THREADSTART			WM_USER+200
#define WM_THREADCLOSE			WM_USER+201
#define WM_CLIENTONLINE			WM_USER+202
#define WM_CLIENTOFFLINE		WM_USER+203
#define WM_MOVEPROGRESSBAR		WM_USER+300

#define SOCKET_WAIT_TIMEOUT				2000
#define SOCKET_CONNECT_RETRY_INTERVAL	5000
#define SOCKET_DEFAULT_MQ_LENGTH		64

#define PACKET_SIZE						1024
#define BUFFER_OVERFLOW					4096

DWORD AfmWaitWithMessageLoop(DWORD nCount,const HANDLE *pHandles,BOOL bWaitAll, DWORD dwTimeout);
void DoEvents();
CString BrowseForFolder(HWND hWnd, LPCSTR lpszTitle, UINT nFlags);
BOOL PcsMakeSureDirectoryPathExists(LPCTSTR lpszDirPath);
BOOL MakeSureFilePathExists(LPCTSTR lpszDirPath);
BOOL RemoveFilesInDirectory(LPCTSTR lpszDirPath);
BOOL FileExists(LPCTSTR lpszFileName, BOOL bIsDirCheck);
BOOL IsSubDirectory(LPCTSTR lpszDirectory, LPCTSTR lpszSubDirectory);
CString GetFileDate(CFileFind &find);
bool GetFileLen(LPCTSTR strFileName, __int64 *iLen);
CString ToString(int nValue);
void AutoSizeColumns(CListCtrl *pListCtrl);
int GetItemIndexWithPoint(CListCtrl *f_wndList, CPoint f_ptPoint);
void CopyToCString(CString& string, LPCSTR pStart, LPCSTR pEnd);
CString AddTailBackslashToPath(CString f_sPath);
BOOL WriteBufferToTextFile(LPCSTR lpszPath, LPCSTR lpszData, BOOL bOverWrite = TRUE);
CString SeperateTelNo(LPCSTR lpszTelNo);
BOOL ConvertStringToCTime(LPCSTR f_sYYYYmmDDHHMMSS, CTime &f_tmTime);
BOOL ConvertStringLongToCTime(LPCSTR f_sLongDate, CTime &f_tmTime);
BOOL IsValidDate(int iYear, int iMonth, int iDay);
int GetLastDayOfMonth(int iYear, int iMonth);
void Split(LPCTSTR strInput, LPCTSTR strDelimit, CStringArray *arrReturn);
CString TimeDurationFormat(long lngDuration, LPCSTR strFormat);

// 自定义数据类型
#ifndef UDL
	#define UDL unsigned __int64
#endif
#ifndef UL
	#define UL unsigned long
#endif
#ifndef US
	#define US unsigned short int
#endif
#ifndef UC
	#define UC unsigned char
#endif

// Application Definition
#define PCS_APP_IDENTIFICATION					0x0504

#include "..\Protocol\XYTrace.h"
#include "..\Protocol\_pcCommonDef.h"
#include "..\Protocol\_Protocol.h"
#include "..\Protocol\_Protocol_IVRAdmin.h"
#include "..\Protocol\_Protocol_IVRExt.h"

//--------------------------------------
// Sun added 2012-12-27
#include ".\PicaCommon\PicaSoftHead.h"
#include ".\PicaCommon\PicaSoftAssistant.h"
#include ".\PicaCommon\PicaSoftExceptionReport.h"
//--------------------------------------

struct S_WM_WPARAM
{
	BYTE UserType;
	char PeerName[30];
	char ClientName[50];
	DWORD pThread;
	DWORD dwTag;
};

struct S_WM_CTIONHOOKEVENT
{
	DWORD pChannel;
	UL lngCallRefID;
	UL lngTickCount;
};

//	little class to make code more readable
class CManulSync
{
	HANDLE		m_sync;

public:
	CManulSync ();
	~CManulSync ();

	CManulSync (CManulSync& s);
	CManulSync& operator= (CManulSync& s);

	void Enter () const;
	void Leave () const;
};

class CManualResetEvent 
{
   public :

      CManualResetEvent();
      ~CManualResetEvent();

      bool Create(TCHAR *f_pName = NULL);

      bool Set();
	  bool Reset();
	  bool Pulse();

      HANDLE GetHANDLE() const;

      void Wait() const;

      bool Wait(
         DWORD timeoutMillis) const;

	  CString m_strEventName;

   private :

      HANDLE m_hEvent;
};

class CMutexValueManger : private CManulSync
{
public:
	CMutexValueManger();
	~CMutexValueManger();

	long GetValue();
	void SetValue(long f_Value);
	void IncreaseValue();
	void DecreaseValue();

private:
	long m_lngValue;
};

class CMyCopyStringArray : public CStringArray
{
public:
	const CMyCopyStringArray& CMyCopyStringArray::operator = (const CMyCopyStringArray& SourceObj)
	{
		CStringArray::RemoveAll();
		for( int i = 0; i < SourceObj.GetSize(); i++ )
		{
			CStringArray::Add(SourceObj[i]);
		}
		return *this;
	}

	CMyCopyStringArray(const CMyCopyStringArray & SourceObj)
	{
		*this = SourceObj;
	}

	CMyCopyStringArray()
	{
		CStringArray();
	}
};

class CMyTimeBomb
{
public:
	CMyTimeBomb();
	~CMyTimeBomb();

	BOOL GetEnabled();
	void SetEnabled(BOOL f_blnValue);

	BOOL GetExploded();

	CTime GetExplodeTime();
	void SetExplodeTime(CTime f_tmTime);

	BOOL CheckTimer();

	long GetRemainSeconds();

protected:
	BOOL m_blnEnabled;
	BOOL m_blnExploded;
	CTime m_tmTimer;
};


//------------------------------------------------------------
// Dialogic Special Defination
//------------------------------------------------------------
#ifdef INTEL_DIALOGIC

// Dialogic head file
#include <srllib.h>				// SRL header file
#include <dxxxlib.h>			// Voice library header file
#include <dtilib.h>				// DTI library header file
#include <gclib.h>				// Global Call API header file
#include <gcerr.h>				// Global Call API errors file
#include <gcisdn.h>				// ISDN translation layer
#include <faxlib.h>				// FAX library header file
#include <sctools.h>			// SCBus library header file

#endif
//------------------------------------------------------------

//------------------------------------------------------------
// Cisco CallManager Special Defination
//------------------------------------------------------------
#ifdef CISCO_CCM

// TAPI & WAVE head file
#include <tapi.h>				// TAPI header file
#include <mmsystem.h>			// Wave API header file

//------------------------------------------------------------
// Sun added 2013-0-06
// Cisco Special Defination: CUCM 8.6 MediaDriver
//------------------------------------------------------------
#ifdef CUCM_MEDIA
#include <ciscortpapi.h>
#include <ciscortpep.h>
#include <CiscoLineDevSpecificMsg.h>
#endif

enum STATUS {	ST_NONE=0, ST_INIT, ST_OPEN, ST_RESTART, ST_SHUTDN, ST_FAILED, ST_READY, ST_IBBUSY, 
				ST_OFFER, ST_RING, ST_OBBUSY, ST_DTMF, ST_CALLER_ID, ST_DIAL, ST_RINGBK};


// TAPI Process Event
#define WM_TAPI_STATUS_CHNGD	WM_USER + 350
#define WM_TAPI_CALLERID		WM_USER + 351
#define WM_TAPI_DTMF			WM_USER + 352
#define WM_TAPI_LINEEVENT		WM_USER + 353

#define IDS_LINEERR_REINIT		190
#define IDS_LINEERR_NOMEM		182

#endif
//------------------------------------------------------------

//------------------------------------------------------------
// Cisco Special Defination: CCM & CME
//------------------------------------------------------------
#ifndef INTEL_DIALOGIC

#ifndef __SRLTPT_H__
#include <srltpt.h>				// Use Dialogic Defination
#endif

/*
 * Masked DTMF termination/initiation equates
 */
#define  DM_D        0x0001    /* Mask for DTMF d. */
#define  DM_1        0x0002    /* Mask for DTMF 1. */
#define  DM_2        0x0004    /* Mask for DTMF 2. */
#define  DM_3        0x0008    /* Mask for DTMF 3. */
#define  DM_4        0x0010    /* Mask for DTMF 4. */
#define  DM_5        0x0020    /* Mask for DTMF 5. */
#define  DM_6        0x0040    /* Mask for DTMF 6. */
#define  DM_7        0x0080    /* Mask for DTMF 7. */
#define  DM_8        0x0100    /* Mask for DTMF 8. */
#define  DM_9        0x0200    /* Mask for DTMF 9. */
#define  DM_0        0x0400    /* Mask for DTMF 0. */
#define  DM_S        0x0800    /* Mask for DTMF *. */
#define  DM_P        0x1000    /* Mask for DTMF #. */
#define  DM_A        0x2000    /* Mask for DTMF a. */
#define  DM_B        0x4000    /* Mask for DTMF b. */
#define  DM_C        0x8000    /* Mask for DTMF c. */

/*
 * Values for setting hook state
 */
#define DL_ONHOOK             0
#define DL_OFFHOOK            1
#define DX_ONHOOK             0
#define DX_OFFHOOK            1

/*
 * Defines for the TPT
 */
#define DX_MAXDTMF            1     /* Maximum Number of Digits Received */
#define DX_MAXSIL             2     /* Maximum Silence */
#define DX_MAXNOSIL           3     /* Maximum Non-Silence */
#define DX_LCOFF              4     /* Loop Current Off */
#define DX_IDDTIME            5     /* Inter-Digit Delay */
#define DX_MAXTIME            6     /* Function Time */
#define DX_DIGMASK            7     /* Digit Mask Termination */
#define DX_PMOFF              8     /* Pattern Match Silence On */
#define DX_PMON               9     /* Pattern Match Silence Off */
#define DX_DIGTYPE            11    /* Digit Type Termination */
#define DX_TONE               12    /* Tone On/Off Termination */
#define DX_MAXDATA            13    /* Maximum bytes for ADSI data*/

/*
 * Defines for TPT Termination Flags
 */
#define TF_EDGE               0x00
#define TF_LEVEL              0x01
#define TF_CLREND             0x02
#define TF_CLRBEG             0x04
#define TF_USE                0x08
#define TF_SETINIT            0x10
#define TF_10MS               0x20
#define TF_FIRST              TF_CLREND

#define TF_MAXDTMF            (TF_LEVEL|TF_USE)
#define TF_MAXSIL             (TF_EDGE|TF_USE)
#define TF_MAXNOSIL           (TF_EDGE|TF_USE)
#define TF_LCOFF              (TF_LEVEL|TF_USE|TF_CLREND)
#define TF_IDDTIME            (TF_EDGE)
#define TF_MAXTIME            (TF_EDGE)
#define TF_DIGMASK            (TF_LEVEL)
#define TF_PMON               (TF_EDGE)
#define TF_DIGTYPE            (TF_LEVEL)
#define TF_TONE               (TF_LEVEL|TF_USE|TF_CLREND)
#define TF_MAXDATA             0


/*
 * Defines for the DV_TPT data for DX_TONE terminator
 */
#define DX_TONEON             0x0001
#define DX_TONEOFF            0x0000


//--------------------------------------------------
/*
 * Tone ID types
 */
#define TID_FIRST             250
#define TID_DIAL_LCL          250
#define TID_DIAL_INTL         251
#define TID_DIAL_XTRA         252
#define TID_BUSY1             253
#define TID_RNGBK1            254
#define TID_BUSY2             255
#define TID_RNGBK2            256

#define TID_DISCONNECT	      257
#define TID_FAX1              258
#define TID_FAX2              259
#define TID_LAST              259  /* last in springware */

#define	TID_SIT_NC	      260
#define TID_SIT_NO_CIRCUIT (TID_SIT_NC)
#define	TID_SIT_IC	      261
#define TID_SIT_OPERATOR_INTERCEPT (TID_SIT_IC)
#define	TID_SIT_VC	      262
#define TID_SIT_VACANT_CIRCUIT (TID_SIT_VC)
#define	TID_SIT_RO	      263
#define TID_SIT_REORDER_TONE (TID_SIT_RO)
#define	TID_SIT_ANY	      264

#define	TID_SIT_NC_INTERLATA  265
#define TID_SIT_NO_CIRCUIT_INTERLATA (TID_SIT_NC_INTERLATA)
#define	TID_SIT_RO_INTERLATA  266
#define TID_SIT_REORDER_TONE_INTERLATA (TID_SIT_RO_INTERLATA)
#define	TID_SIT_IO	      267
#define TID_SIT_INEFFECTIVE_OTHER (TID_SIT_IO)
//--------------------------------------------------

#endif
//------------------------------------------------------------

#define MAXMSGLEN				128

// Use multi node data fields
#define PVC_USE_MULTI_NODE_FIELDS

//------------------------------------------------------------
// System upgraded 2011-12-01 [Tag: ANTIDECEIT]
/// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/// If want to disable Anti-deceit function, just remove this line
#define PICADEF_ENABLE_ANTIDECEIT
/// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#ifdef PICADEF_ENABLE_ANTIDECEIT

//--------------------------------------------------
// 激活码
#define PICADEF_ANTIDECEIT_ACTIVECODE		"8741"
// 休眠码
#define PICADEF_ANTIDECEIT_DEACTIVECODE		"8963"
// 最大计数
#define PICADEF_ANTIDECEIT_MAXCALLCOUNT		10

// 键值保存位置：Section
#define PICADEF_ANTIDECEIT_SECTION			"AntiDeceit"
// 键值保存位置：Section
#define PICADEF_ANTIDECEIT_ENTRY_CODE		"Code"
//--------------------------------------------------

#endif
//------------------------------------------------------------

// Sun added 2013-04-25
#define SMARTAC_LICENSE						SmartacLicense::GetSmartacLicenseInstance()
//#define SMARTAC_LICENSE						theApp.m_License

// Sun added 2013-04-25
#ifdef SMARTAC_LICENSE
#include "./SmartacLic/SmartacLicense.h"
#endif

// Sun added 2013-04-25
#ifdef _DEBUG
#pragma comment(lib,"./SmartacLic/SmartacLicenseLib71D.lib")
#pragma comment(lib,"./SmartacLic/zlib-static-mtd-vc71d.lib")
#else
#pragma comment(lib,"./SmartacLic/SmartacLicenseLib71.lib")
#pragma comment(lib,"./SmartacLic/zlib-static-mtd-vc71.lib")
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__DC938324_AA56_4C55_8944_75D1025185D8__INCLUDED_)
