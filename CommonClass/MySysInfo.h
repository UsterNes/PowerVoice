// MySysInfo.h: interface for the CMySysInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSYSINFO_H__5C8CF31B_FE17_42BA_BC4B_93679BD1ED80__INCLUDED_)
#define AFX_MYSYSINFO_H__5C8CF31B_FE17_42BA_BC4B_93679BD1ED80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMySysInfo  
{
public:
	CMySysInfo();
	virtual ~CMySysInfo();

	BYTE FindLocalNICMAC(LPCSTR f_sMAC);
	CString GetLocalNICMAC(BYTE f_nIndex);
	UINT GetLocalNumberOfInterfaces();
	UINT GetHDVolumeInformation(LPCSTR f_sDriverName);
	UINT QueryDriveType(CString f_sPath);
	BOOL QueryDriveSpace(CString f_sPath, __int64 *f_nAvailable, __int64 *f_nTotal, __int64 *f_nFree);
};

#endif // !defined(AFX_MYSYSINFO_H__5C8CF31B_FE17_42BA_BC4B_93679BD1ED80__INCLUDED_)
