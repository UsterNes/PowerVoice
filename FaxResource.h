// FaxResource.h: interface for the CFaxResource class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FAXRESOURCE_H__BAC1CB45_4389_4378_AA98_EA764145E77E__INCLUDED_)
#define AFX_FAXRESOURCE_H__BAC1CB45_4389_4378_AA98_EA764145E77E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "channelcontrol.h"

// FAX ���
#define	DEF_FAX_EC_SUCC			0			// ����ɹ�
#define	DEF_FAX_EC_INITFAILED	1			// �����豸��ʼ��ʧ��
#define	DEF_FAX_EC_FILE			2			// �����ļ���
#define	DEF_FAX_EC_NOFAXTONE	3			// �޴�����
#define	DEF_FAX_EC_LCOFF		4			// ����
#define	DEF_FAX_EC_USERSTOP		9			// �û��ж�
#define	DEF_FAX_EC_TS_LINK		10			// ʱ϶���Ӵ���
#define	DEF_FAX_EC_SYSTEM		255			// ϵͳ����

class CFaxResource  
{
public:
	CFaxResource(Cchannelcontrol *f_pASR, long f_nID, long f_nType);
	virtual ~CFaxResource();

	long GetFaxResID();
	long GetFaxResType();

	BYTE GetStatus(int *f_pintLineNo = NULL);
	void SetStatus(BYTE f_bytStatus, int f_intLineNo = -1);
	DWORD GetTotalBusyTime();

	BOOL Channel_Stop();
	
	BOOL SendFaxFile(const char *f_pFilePath, const char *f_strFromNo = NULL, const char *f_strToNo = NULL, const char *f_strHeader = NULL, int nRetryTimes = 0);
	BOOL ReceiveFaxFile(const char *f_pFilePath, char *f_strFromNo = NULL, const char *f_strToNo = NULL, int nRetryTimes = 0);
	BOOL StopFax();
	BYTE GetSendResult();
	Cchannelcontrol* GetASRObj();

private:
	long			m_nID;
	long			m_nType;

	BYTE			m_bytStatus;
	DWORD			m_dwBusyTick;
	DWORD			m_dwTotalBusy;
	int				m_intLineNo;				// ���IVRͨ����

	BYTE			m_bytSendResult;			// ���淢�ͽ��

	Cchannelcontrol	*m_pARS;

	CManulSync		m_bLock;
};

#endif // !defined(AFX_FAXRESOURCE_H__BAC1CB45_4389_4378_AA98_EA764145E77E__INCLUDED_)
