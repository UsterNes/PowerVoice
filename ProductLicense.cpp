// ProductLicense.cpp: implementation of the CProductLicense class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProductLicense.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProductLicense::CProductLicense()
{
	int lv_loop;

	m_strProductName = _T("PowerVoice Server");
	m_strProductID = _T("");
	m_strLicenseUID = _T("");
	m_bytLicenseType = 0;
	m_ulMaxRunSeconds = 1800;
	m_ucServerNICIndex = 0;
	m_strServerMacAddress = _T("");
	m_strServerHDSerialNumber = _T("");
	m_strLicenseDate = _T("");
	m_strLicenseTime = _T("");

	for( lv_loop = 0; lv_loop < PCS_LICENSE_END; lv_loop++ )
		m_nLicenseClient[lv_loop] = 0;

	// 开始计时
	m_ulStartTickCount = GetTickCount();
}

CProductLicense::~CProductLicense()
{
}

BOOL CProductLicense::DateDiscription(BYTE *f_pSource, BYTE *f_pTarget, int f_Len)
{
	try
	{
		BYTE lv_Source_Char, lv_Target_Char;

		// 遍历密文
		for( int lv_Lpv = 0;  lv_Lpv < f_Len; lv_Lpv++ )
		{
			/// 反向取一个字符
			lv_Source_Char = f_pSource[f_Len - lv_Lpv - 1];
			/// 进行变换
			lv_Target_Char = (f_Len + lv_Source_Char - lv_Lpv + 17) % 256;
			/// 连接放入明文中
			f_pTarget[lv_Lpv] = lv_Target_Char;
		}
	}
	catch(...)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CProductLicense::TrialVersionExpiredCheck()
{
	if( m_bytLicenseType != 1 && m_ulMaxRunSeconds > 0 )
		if( (GetTickCount() - m_ulStartTickCount) / 1000 > m_ulMaxRunSeconds )
			return FALSE;

	return TRUE;
}

int CProductLicense::InitializeProductLicense(LPCSTR strAppPath, LPCSTR strAppName)
{
	int lv_count = 0;

	// Initialize Lic through Soft Dog or License.pcs
//#ifndef _DEBUG

	CFile lv_LicenseFile;
	CFileException ex;
	CString lv_sFileName;
	SPCSLicenseFile lv_LicFileData;
	SPCSLicenseData lv_LicData;

	// Get License File Path
	lv_sFileName.Format("%s%s", strAppPath, PCS_LICENSE_FILENAME);

	// Read License File
	if( !lv_LicenseFile.Open(lv_sFileName, CFile::modeRead | CFile::shareDenyWrite, &ex) )
	{
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		WriteTrace(TraceError, "Open file:%s error:%s", lv_sFileName, szError);
	}
	else
	{
		if( lv_LicenseFile.Read((void *)&lv_LicFileData, sizeof(lv_LicFileData)) != sizeof(SPCSLicenseFile) )
			WriteTrace(TraceError, "License file has been damaged!!!");
		else
		{
			m_strProductName = lv_LicFileData.strProdutcName;

			// 许可数据解密
			memset(&lv_LicData, 0x00, sizeof(lv_LicData));
			if( DateDiscription((BYTE *)&lv_LicFileData.stLicData, (BYTE *)&lv_LicData,  sizeof(SPCSLicenseData)) )
			{
				m_strProductID = lv_LicData.strProdutcID;
				m_strLicenseUID = lv_LicData.strUserIdentification;
				m_bytLicenseType = lv_LicData.ucLicenseType;
				m_ulMaxRunSeconds = lv_LicData.ulMaxRunSeconds;
				m_strServerMacAddress = lv_LicData.strServerMacAddress;
				m_strServerHDSerialNumber = lv_LicData.strServerHDSerialNumber;
				m_strLicenseDate = lv_LicData.strLicenseDate;
				m_strLicenseTime = lv_LicData.strLicenseTime;
			}

			// 产品识别
			if( m_strProductID.CompareNoCase(strAppName) == 0 )
			{
				m_nLicenseClient[PCS_LIC_IVR_GROUP] = lv_LicData.usLicense1;
				m_nLicenseClient[PCS_LIC_IVR_CHANNEL] = lv_LicData.usLicense2;
				m_nLicenseClient[PCS_LIC_TTS_CONNECT] = lv_LicData.usLicense3;
				m_nLicenseClient[PCS_LIC_ASR_CONNECT] = lv_LicData.usLicense4;
				m_nLicenseClient[PCS_LIC_IVR_ADMIN] = lv_LicData.usLicense5;
				m_nLicenseClient[PCS_LIC_IVR_MSI] = lv_LicData.usLicense6;

				lv_count = PCS_LICENSE_END;
			}
		}

		/// Close File
		lv_LicenseFile.Close();
	}

/*
#else

	m_nLicenseClient[PCS_LIC_IVR_GROUP] = 1;
	m_nLicenseClient[PCS_LIC_IVR_CHANNEL] = 4;
	m_nLicenseClient[PCS_LIC_TTS_CONNECT] = 4;
	m_nLicenseClient[PCS_LIC_ASR_CONNECT] = 4;
	lv_count = PCS_LICENSE_END;

#endif
*/

	return lv_count;
}

int CProductLicense::GetLicenseClient(int f_nIndex)
{
	int lv_Count;

	if( f_nIndex >= 0 && f_nIndex < PCS_LICENSE_END )
		lv_Count = m_nLicenseClient[f_nIndex];
	else
		lv_Count = -1;

	return lv_Count;
}

int CProductLicense::GetLicenseUsedClient(int f_nIndex)
{
	int lv_Count;

	if( f_nIndex >= 0 && f_nIndex < PCS_LICENSE_END )
		lv_Count = (int)m_nLicenseUsedClient[f_nIndex].GetValue();
	else
		lv_Count = -1;

	return lv_Count;
}

BOOL CProductLicense::IncreaseLicenseUsedClient(int f_nIndex)
{
	if( f_nIndex >= 0 && f_nIndex < PCS_LICENSE_END )
	{
		m_nLicenseUsedClient[f_nIndex].IncreaseValue();
		WriteTrace(TraceInfo, "One license type: %d is assigned, available number is %d", 
			f_nIndex, m_nLicenseClient[f_nIndex] - m_nLicenseUsedClient[f_nIndex].GetValue());
		return TRUE;
	}

	return FALSE;
}

BOOL CProductLicense::DecreaseLicenseUsedClient(int f_nIndex)
{
	if( f_nIndex >= 0 && f_nIndex < PCS_LICENSE_END )
	{
		m_nLicenseUsedClient[f_nIndex].DecreaseValue();
		WriteTrace(TraceInfo, "One license type: %d is released, available number is %d", 
			f_nIndex, m_nLicenseClient[f_nIndex] - m_nLicenseUsedClient[f_nIndex].GetValue());
		return TRUE;
	}

	return FALSE;
}

BOOL CProductLicense::IsLicenseClientUsedUp(int f_nIndex)
{
	if( f_nIndex >= 0 && f_nIndex < PCS_LICENSE_END )
	{
		if( m_nLicenseUsedClient[f_nIndex].GetValue() < m_nLicenseClient[f_nIndex] )
			return FALSE;
	}

	return TRUE;
}