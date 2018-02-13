#pragma once
#include <string>
using namespace std;
namespace SmartacLicense
{
	interface ISmartacLicense
	{
		// ��ʼ����ɣ��ɹ����������������ʧ�ܷ���0
		virtual int InitializeProductLicense(LPCTSTR strAppPath, LPCTSTR strAppName)=0;
	
		// ȡ��ָ�����������
		virtual int GetLicenseClient(int f_nIndex)=0;
		// ȡ��ָ�������������
		virtual int GetLicenseUsedClient(int f_nIndex)=0;
		// ����ָ���������
		virtual BOOL IncreaseLicenseUsedClient(int f_nIndex)=0;
		// �ݼ�ָ���������
		virtual BOOL DecreaseLicenseUsedClient(int f_nIndex)=0;
		// ָ����������Ƿ�ʣ��
		virtual BOOL IsLicenseClientUsedUp(int f_nIndex)=0;
		// ���ý�����⣨�������з����棩
		virtual BOOL TrialVersionExpiredCheck()=0;
		// Simon added 
		virtual BOOL GetLicenseValue(LPCTSTR lpszName,int& val)=0;
		virtual BOOL GetLicenseValue(LPCTSTR lpszName,CString& val)=0;


		CString m_strProductName;									// �������
		CString m_strProductID;										// ���ID
		CString m_strLicenseUID;									// �û���ʾ�����ƻ���
		BYTE m_bytLicenseType;										// ������ͣ�1 - ��ʽ��2 - ����
		ULONG m_ulMaxRunSeconds;									// ��������ʱ�䣺�룬0��ʾ������
		BYTE m_ucServerNICIndex;									// �������������
		CString m_strServerMacAddress;								// ����ɷ�����MAC��ַ
		CString	m_strServerHDSerialNumber;							// ����ɷ�����Ӳ�����к�
		CString	m_strLicenseDate;									// ע���ļ���������
		CString	m_strLicenseTime;									// ע���ļ�����ʱ��
		// Simon added
		CString m_strPublishUser;									// ������
		CString m_strDeadlineDate;									// ����ʱ��
		CString m_strServerName;									// ��ǰ��������
	};
	// ȡ�����ʵ��
	ISmartacLicense* GetSmartacLicenseInstance();
	// ��ȡ������������������Loopback
	UINT GetLocalNumberOfInterfaces();
	// ��ȡָ�����������MAC��ַ
	string GetLocalNICMAC(BYTE f_nIndex);
	// ��ȡָ��Ӳ�̷������
	string GetHDVolumeInformation();
	// ȡ�÷�����������
	string GetFeatureCode();


}