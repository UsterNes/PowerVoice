// MySysInfo.cpp: implementation of the CMySysInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MySysInfo.h"
#include <Nb30.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma comment(lib, "Netapi32.lib ")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMySysInfo::CMySysInfo()
{

}

CMySysInfo::~CMySysInfo()
{

}

// 获取本地网卡数量，包括Loopback
UINT CMySysInfo::GetLocalNumberOfInterfaces()
{
//	DWORD lv_nInterfaces;
//	if( GetNumberOfInterfaces(&lv_nInterfaces) == NO_ERROR )
//		return lv_nInterfaces;

	// 取得网卡列表
	LANA_ENUM AdapterList;
	NCB Ncb;
	memset(&Ncb, 0, sizeof(NCB));
	Ncb.ncb_command = NCBENUM;
	Ncb.ncb_buffer = (unsigned char *)&AdapterList;
	Ncb.ncb_length = sizeof(AdapterList);
	Netbios(&Ncb);

	return AdapterList.length;
}

// 获取指定编号网卡的MAC地址
CString CMySysInfo::GetLocalNICMAC(BYTE f_nIndex)
{
	CString lv_sMac = _T("");

	// 取得网卡列表
	LANA_ENUM AdapterList;
	NCB Ncb;
	memset(&Ncb, 0, sizeof(NCB));
	Ncb.ncb_command = NCBENUM;
	Ncb.ncb_buffer = (unsigned char *)&AdapterList;
	Ncb.ncb_length = sizeof(AdapterList);
	Netbios(&Ncb);

	if( f_nIndex >= AdapterList.length )
		return lv_sMac;

	// 重置网卡，以便我们可以查询
	memset(&Ncb, 0, sizeof(Ncb));
	Ncb.ncb_command = NCBRESET;
	Ncb.ncb_lana_num = AdapterList.lana[f_nIndex];
	if( Netbios(&Ncb) != NRC_GOODRET )
		return lv_sMac;

	// 准备取得接口卡的状态块
	memset(&Ncb, 0, sizeof(Ncb));
	Ncb.ncb_command = NCBASTAT;
	Ncb.ncb_lana_num = AdapterList.lana[f_nIndex];
	strcpy((char *) Ncb.ncb_callname, "*");
	struct ASTAT
	{
		ADAPTER_STATUS adapt;
		NAME_BUFFER NameBuff[30];
	} Adapter;
	memset(&Adapter, 0, sizeof(Adapter));
	Ncb.ncb_buffer = (unsigned char *)&Adapter;
	Ncb.ncb_length = sizeof(Adapter);

	// 取得网卡的信息，并且如果网卡正常工作的话，返回标准的减号分隔格式。
	if( Netbios(&Ncb) == 0 )
	{
		lv_sMac.Format( "%02X-%02X-%02X-%02X-%02X-%02X",
			int (Adapter.adapt.adapter_address[0]),
			int (Adapter.adapt.adapter_address[1]),
			int (Adapter.adapt.adapter_address[2]),
			int (Adapter.adapt.adapter_address[3]),
			int (Adapter.adapt.adapter_address[4]),
			int (Adapter.adapt.adapter_address[5]) );
	}

	return lv_sMac;
}

// 查找MAC是否属于本机
BYTE CMySysInfo::FindLocalNICMAC(LPCSTR f_sMAC)
{
	CString lv_sMac;

	// 取得网卡列表
	LANA_ENUM AdapterList;
	NCB Ncb;
	memset(&Ncb, 0, sizeof(NCB));
	Ncb.ncb_command = NCBENUM;
	Ncb.ncb_buffer = (unsigned char *)&AdapterList;
	Ncb.ncb_length = sizeof(AdapterList);
	Netbios(&Ncb);

	// 遍历所有网卡
	for( BYTE lv_nIndex = 0; lv_nIndex < AdapterList.length; lv_nIndex++ )
	{
		// 重置网卡，以便我们可以查询
		memset(&Ncb, 0, sizeof(Ncb));
		Ncb.ncb_command = NCBRESET;
		Ncb.ncb_lana_num = AdapterList.lana[lv_nIndex];
		if( Netbios(&Ncb) != NRC_GOODRET )
			return 255;

		// 准备取得接口卡的状态块
		memset(&Ncb, 0, sizeof(Ncb));
		Ncb.ncb_command = NCBASTAT;
		Ncb.ncb_lana_num = AdapterList.lana[lv_nIndex];
		strcpy((char *) Ncb.ncb_callname, "*");
		struct ASTAT
		{
			ADAPTER_STATUS adapt;
			NAME_BUFFER NameBuff[30];
		} Adapter;
		memset(&Adapter, 0, sizeof(Adapter));
		Ncb.ncb_buffer = (unsigned char *)&Adapter;
		Ncb.ncb_length = sizeof(Adapter);

		// 取得网卡的信息，并且如果网卡正常工作的话，返回标准的减号分隔格式。
		if( Netbios(&Ncb) == 0 )
		{
			lv_sMac.Format( "%02X-%02X-%02X-%02X-%02X-%02X",
				int (Adapter.adapt.adapter_address[0]),
				int (Adapter.adapt.adapter_address[1]),
				int (Adapter.adapt.adapter_address[2]),
				int (Adapter.adapt.adapter_address[3]),
				int (Adapter.adapt.adapter_address[4]),
				int (Adapter.adapt.adapter_address[5]) );
			if( lv_sMac.CollateNoCase(f_sMAC) == 0 )
				return lv_nIndex;
		}
	}

	return 255;
}

// 获取指定硬盘分区卷号
UINT CMySysInfo::GetHDVolumeInformation(LPCSTR f_sDriverName)
{
	DWORD VolumeSerialNumber;

	GetVolumeInformation(f_sDriverName,NULL,12,&VolumeSerialNumber,NULL,NULL,NULL,10);

	return VolumeSerialNumber;
}

// 获取指定驱动器类型
UINT CMySysInfo::QueryDriveType(CString f_sPath)
{
	return GetDriveType(AddTailBackslashToPath(f_sPath));
}

// 获取指定驱动器介质空间
BOOL CMySysInfo::QueryDriveSpace(CString f_sPath, __int64 *f_nAvailable, __int64 *f_nTotal, __int64 *f_nFree)
{
	ULARGE_INTEGER AvailableToCaller, Disk, Free;

	if( GetDiskFreeSpaceEx(AddTailBackslashToPath(f_sPath), &AvailableToCaller,	&Disk, &Free))
	{
		*f_nAvailable = AvailableToCaller.QuadPart;
		*f_nTotal = Disk.QuadPart;
		*f_nFree = Free.QuadPart;
		return TRUE;
	}

	return FALSE;
}


/*
CString CMySysInfo::QueryDriveSpace()
{
	ULARGE_INTEGER AvailableToCaller, Disk, Free;
	CString Result, Temp;

	for (int iCounter=0;iCounter<MAX_OF_HARD_DISKS;iCounter++)
	{
		if (GetDriveType(HardDiskLetters[iCounter])==DRIVE_FIXED)
		{
			if (GetDiskFreeSpaceEx(HardDiskLetters[iCounter],&AvailableToCaller,
									&Disk, &Free))
			{
				Temp.Format("*** Hard Disk: (%s) ***\r\n",HardDiskLetters[iCounter]);
				Result+=Temp;

				Temp.Format("Total size: %I64d (MB)\r\n",Disk.QuadPart/1024/1024);
				Result+=Temp;

				ULONGLONG Used=Disk.QuadPart-Free.QuadPart;
				Temp.Format("Used: %I64d (MB)\r\n",Used/1024/1024);
				Result+=Temp;

				Temp.Format("Free: %I64d (MB)\r\n",Free.QuadPart/1024/1024);
				Result+=Temp;
			}

			char Label[256]="";
			char FS[256]="";
			DWORD SerialNumber=0, MaxFileLen=0, FileSysFlag=0;
			
			if (GetVolumeInformation(HardDiskLetters[iCounter], Label, 256, &SerialNumber,
				&MaxFileLen, &FileSysFlag, FS, 256))
			{
				Temp.Format("%X", SerialNumber);
				Temp=CString("Serial Number: ") + Temp.Left(4) + 
					CString("-") + Temp.Right(4) + CString("\r\n");
				Result+=Temp;
			
				Temp.Format("Max of File Length Support: %d\r\n", MaxFileLen);
				Result+=Temp;

				Temp.Format("File System: %s\r\n\r\n", FS);
				Result+=Temp;
			}
		}
	}

	return Result;
}
*/