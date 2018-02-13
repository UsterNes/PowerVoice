// ProductLicense.h: interface for the CProductLicense class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRODUCTLICENSE_H__47AB8FCD_50F1_4F3F_A0BC_227273810210__INCLUDED_)
#define AFX_PRODUCTLICENSE_H__47AB8FCD_50F1_4F3F_A0BC_227273810210__INCLUDED_

#include "stdafx.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Licsense �ļ���
#define PCS_LICENSE_FILENAME		"License.pcs"

// �����Ϣ�����������
struct SPCSLicenseData
{
	char	strProdutcID[20];				// ���ID
	char	strSpaceKeeper2;				// ��������ַ�2
	UC		ucLicenseType;					// ������ͣ�1 - ��ʽ��2 - ����
	char	strSpaceKeeper3;				// ��������ַ�3
	US		usLicense1;						// �������1
	char	strSpaceKeeper4;				// ��������ַ�4
	US		usLicense2;						// �������2
	char	strSpaceKeeper5;				// ��������ַ�5
	US		usLicense3;						// �������3
	char	strSpaceKeeper6;				// ��������ַ�6
	US		usLicense4;						// �������4
	char	strSpaceKeeper7;				// ��������ַ�7
	US		usLicense5;						// �������5
	char	strSpaceKeeper8;				// ��������ַ�8
	US		usLicense6;						// �������6
	char	strSpaceKeeper9;				// ��������ַ�9
	US		usLicense7;						// �������7
	char	strSpaceKeeper10;				// ��������ַ�10
	US		usLicense8;						// �������8
	char	strSpaceKeeper11;				// ��������ַ�11
	US		usLicense9;						// �������9
	char	strSpaceKeeper12;				// ��������ַ�12
	US		usLicense10;					// �������10
	char	strSpaceKeeper13;				// ��������ַ�13
	UL		ulMaxRunSeconds;				// ��������ʱ�䣺�룬0��ʾ������
	char	strSpaceKeeper14;				// ��������ַ�14
	char	strUserIdentification[100];		// �û���ʾ�����ƻ���
	char	strSpaceKeeper15;				// ��������ַ�15
	UC		ucServerMICIndex;				// �������������
	char	strSpaceKeeper16;				// ��������ַ�16
	char	strServerMacAddress[60];		// ������MAC��ַ
	char	strSpaceKeeper17;				// ��������ַ�16
	char	strServerHDSerialNumber[20];	// ������Ӳ�����к�
	char	strSpaceKeeper18;				// ��������ַ�17
	char	strLicenseDate[10];				// ע���ļ���������
	char	strSpaceKeeper19;				// ��������ַ�18
	char	strLicenseTime[8];				// ע���ļ�����ʱ��
	char	strSpaceKeeper20;				// ��������ַ�19
};

// Licsense �ļ���ʽ
struct SPCSLicenseFile
{
	char	strDeclare[200];				// ��������
	char	strSpaceKeeper0;				// ��������ַ�0
	char	strProdutcName[50];				// �������
	char	strSpaceKeeper1;				// ��������ַ�1
	SPCSLicenseData stLicData;				// �����Ϣ�����������
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

	CString m_strProductName;									// �������
	CString m_strProductID;										// ���ID
	CString m_strLicenseUID;									// �û���ʾ�����ƻ���
	BYTE m_bytLicenseType;										// ������ͣ�1 - ��ʽ��2 - ����
	UL m_ulMaxRunSeconds;										// ��������ʱ�䣺�룬0��ʾ������
	BYTE m_ucServerNICIndex;									// �������������
	CString m_strServerMacAddress;								// ����ɷ�����MAC��ַ
	CString	m_strServerHDSerialNumber;							// ����ɷ�����Ӳ�����к�
	CString	m_strLicenseDate;									// ע���ļ���������
	CString	m_strLicenseTime;									// ע���ļ�����ʱ��

private:
	UL m_ulStartTickCount;										// ������ʼ��ʱ
	int	m_nLicenseClient[PCS_LICENSE_END];						// �ͻ����������
	CMutexValueManger m_nLicenseUsedClient[PCS_LICENSE_END];	// �Ѿ�ʹ�ÿͻ����������

	BOOL DateDiscription(BYTE *f_pSource, BYTE *f_pTarget, int f_Len);
};

#endif // !defined(AFX_PRODUCTLICENSE_H__47AB8FCD_50F1_4F3F_A0BC_227273810210__INCLUDED_)
