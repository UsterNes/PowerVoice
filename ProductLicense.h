// ProductLicense.h: interface for the CProductLicense class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRODUCTLICENSE_H__47AB8FCD_50F1_4F3F_A0BC_227273810210__INCLUDED_)
#define AFX_PRODUCTLICENSE_H__47AB8FCD_50F1_4F3F_A0BC_227273810210__INCLUDED_

#include "stdafx.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Licsense 文件名
#define PCS_LICENSE_FILENAME		"License.pcs"

// 许可信息，内容须加密
struct SPCSLicenseData
{
	char	strProdutcID[20];				// 软件ID
	char	strSpaceKeeper2;				// 辅助间隔字符2
	UC		ucLicenseType;					// 许可类型：1 - 正式；2 - 试用
	char	strSpaceKeeper3;				// 辅助间隔字符3
	US		usLicense1;						// 许可数量1
	char	strSpaceKeeper4;				// 辅助间隔字符4
	US		usLicense2;						// 许可数量2
	char	strSpaceKeeper5;				// 辅助间隔字符5
	US		usLicense3;						// 许可数量3
	char	strSpaceKeeper6;				// 辅助间隔字符6
	US		usLicense4;						// 许可数量4
	char	strSpaceKeeper7;				// 辅助间隔字符7
	US		usLicense5;						// 许可数量5
	char	strSpaceKeeper8;				// 辅助间隔字符8
	US		usLicense6;						// 许可数量6
	char	strSpaceKeeper9;				// 辅助间隔字符9
	US		usLicense7;						// 许可数量7
	char	strSpaceKeeper10;				// 辅助间隔字符10
	US		usLicense8;						// 许可数量8
	char	strSpaceKeeper11;				// 辅助间隔字符11
	US		usLicense9;						// 许可数量9
	char	strSpaceKeeper12;				// 辅助间隔字符12
	US		usLicense10;					// 许可数量10
	char	strSpaceKeeper13;				// 辅助间隔字符13
	UL		ulMaxRunSeconds;				// 连续运行时间：秒，0表示无限制
	char	strSpaceKeeper14;				// 辅助间隔字符14
	char	strUserIdentification[100];		// 用户标示：名称或编号
	char	strSpaceKeeper15;				// 辅助间隔字符15
	UC		ucServerMICIndex;				// 服务器网卡序号
	char	strSpaceKeeper16;				// 辅助间隔字符16
	char	strServerMacAddress[60];		// 服务器MAC地址
	char	strSpaceKeeper17;				// 辅助间隔字符16
	char	strServerHDSerialNumber[20];	// 服务器硬盘序列号
	char	strSpaceKeeper18;				// 辅助间隔字符17
	char	strLicenseDate[10];				// 注册文件生成日期
	char	strSpaceKeeper19;				// 辅助间隔字符18
	char	strLicenseTime[8];				// 注册文件生成时间
	char	strSpaceKeeper20;				// 辅助间隔字符19
};

// Licsense 文件格式
struct SPCSLicenseFile
{
	char	strDeclare[200];				// 声明文字
	char	strSpaceKeeper0;				// 辅助间隔字符0
	char	strProdutcName[50];				// 软件名称
	char	strSpaceKeeper1;				// 辅助间隔字符1
	SPCSLicenseData stLicData;				// 许可信息，内容须加密
};


#define PCS_LIC_IVR_GROUP			0
#define PCS_LIC_IVR_CHANNEL			1
#define PCS_LIC_TTS_CONNECT			2
#define PCS_LIC_ASR_CONNECT			3
#define PCS_LIC_IVR_ADMIN			4
#define PCS_LIC_IVR_MSI				5
#define PCS_LICENSE_END				PCS_LIC_IVR_MSI + 1

class CProductLicense  
{
public:
	CProductLicense();
	virtual ~CProductLicense();

	int InitializeProductLicense(LPCSTR strAppPath, LPCSTR strAppName);
	
	int GetLicenseClient(int f_nIndex);
	int GetLicenseUsedClient(int f_nIndex);
	BOOL IncreaseLicenseUsedClient(int f_nIndex);
	BOOL DecreaseLicenseUsedClient(int f_nIndex);
	BOOL IsLicenseClientUsedUp(int f_nIndex);

	BOOL TrialVersionExpiredCheck();

	CString m_strProductName;									// 软件名称
	CString m_strProductID;										// 软件ID
	CString m_strLicenseUID;									// 用户标示：名称或编号
	BYTE m_bytLicenseType;										// 许可类型：1 - 正式；2 - 试用
	UL m_ulMaxRunSeconds;										// 连续运行时间：秒，0表示无限制
	BYTE m_ucServerNICIndex;									// 服务器网卡序号
	CString m_strServerMacAddress;								// 获许可服务器MAC地址
	CString	m_strServerHDSerialNumber;							// 获许可服务器硬盘序列号
	CString	m_strLicenseDate;									// 注册文件生成日期
	CString	m_strLicenseTime;									// 注册文件生成时间

private:
	UL m_ulStartTickCount;										// 启动开始计时
	int	m_nLicenseClient[PCS_LICENSE_END];						// 客户端许可数量
	CMutexValueManger m_nLicenseUsedClient[PCS_LICENSE_END];	// 已经使用客户端许可数量

	BOOL DateDiscription(BYTE *f_pSource, BYTE *f_pTarget, int f_Len);
};

#endif // !defined(AFX_PRODUCTLICENSE_H__47AB8FCD_50F1_4F3F_A0BC_227273810210__INCLUDED_)
