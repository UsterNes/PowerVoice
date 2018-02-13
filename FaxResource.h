// FaxResource.h: interface for the CFaxResource class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FAXRESOURCE_H__BAC1CB45_4389_4378_AA98_EA764145E77E__INCLUDED_)
#define AFX_FAXRESOURCE_H__BAC1CB45_4389_4378_AA98_EA764145E77E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "channelcontrol.h"

// FAX 结果
#define	DEF_FAX_EC_SUCC			0			// 传真成功
#define	DEF_FAX_EC_INITFAILED	1			// 传真设备初始化失败
#define	DEF_FAX_EC_FILE			2			// 传真文件错
#define	DEF_FAX_EC_NOFAXTONE	3			// 无传真音
#define	DEF_FAX_EC_LCOFF		4			// 断线
#define	DEF_FAX_EC_USERSTOP		9			// 用户中断
#define	DEF_FAX_EC_TS_LINK		10			// 时隙连接错误
#define	DEF_FAX_EC_SYSTEM		255			// 系统错误

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
	int				m_intLineNo;				// 相关IVR通道号

	BYTE			m_bytSendResult;			// 传真发送结果

	Cchannelcontrol	*m_pARS;

	CManulSync		m_bLock;
};

#endif // !defined(AFX_FAXRESOURCE_H__BAC1CB45_4389_4378_AA98_EA764145E77E__INCLUDED_)
