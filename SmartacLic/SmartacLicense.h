#pragma once
#include <string>
using namespace std;
namespace SmartacLicense
{
	interface ISmartacLicense
	{
		// 初始化许可，成功返回许可种类数，失败返回0
		virtual int InitializeProductLicense(LPCTSTR strAppPath, LPCTSTR strAppName)=0;
	
		// 取得指定类型许可数
		virtual int GetLicenseClient(int f_nIndex)=0;
		// 取得指定类型许可用量
		virtual int GetLicenseUsedClient(int f_nIndex)=0;
		// 递增指定类型许可
		virtual BOOL IncreaseLicenseUsedClient(int f_nIndex)=0;
		// 递减指定类型许可
		virtual BOOL DecreaseLicenseUsedClient(int f_nIndex)=0;
		// 指定类型许可是否剩余
		virtual BOOL IsLicenseClientUsedUp(int f_nIndex)=0;
		// 试用结束检测（继续运行返回真）
		virtual BOOL TrialVersionExpiredCheck()=0;
		// Simon added 
		virtual BOOL GetLicenseValue(LPCTSTR lpszName,int& val)=0;
		virtual BOOL GetLicenseValue(LPCTSTR lpszName,CString& val)=0;


		CString m_strProductName;									// 软件名称
		CString m_strProductID;										// 软件ID
		CString m_strLicenseUID;									// 用户标示：名称或编号
		BYTE m_bytLicenseType;										// 许可类型：1 - 正式；2 - 试用
		ULONG m_ulMaxRunSeconds;									// 连续运行时间：秒，0表示无限制
		BYTE m_ucServerNICIndex;									// 服务器网卡序号
		CString m_strServerMacAddress;								// 获许可服务器MAC地址
		CString	m_strServerHDSerialNumber;							// 获许可服务器硬盘序列号
		CString	m_strLicenseDate;									// 注册文件生成日期
		CString	m_strLicenseTime;									// 注册文件生成时间
		// Simon added
		CString m_strPublishUser;									// 发布者
		CString m_strDeadlineDate;									// 到期时间
		CString m_strServerName;									// 当前服务器名
	};
	// 取得许可实例
	ISmartacLicense* GetSmartacLicenseInstance();
	// 获取本地网卡数量，包括Loopback
	UINT GetLocalNumberOfInterfaces();
	// 获取指定编号网卡的MAC地址
	string GetLocalNICMAC(BYTE f_nIndex);
	// 获取指定硬盘分区卷号
	string GetHDVolumeInformation();
	// 取得服务器特征码
	string GetFeatureCode();


}