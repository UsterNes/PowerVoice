// IVRClient_MSI.h: interface for the CIVRClient_MSI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IVRCLIENT_MSI_H__0C71E94E_FFEF_4CC5_BC68_9603BE157078__INCLUDED_)
#define AFX_IVRCLIENT_MSI_H__0C71E94E_FFEF_4CC5_BC68_9603BE157078__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIVRClient_MSI  
{
public:
	CIVRClient_MSI(UL lngAgentID);
	virtual ~CIVRClient_MSI();

	UL GetAgentID();

	BYTE GetVersion();
	void SetVersion(BYTE f_nValue);

	PVOID GetConnected();
	void SetConnected(PVOID f_cn);
	void SetDisconnected();

	int ProcessTCPMessage(SMsgHeader f_hdr, const BYTE *f_bdy);

protected:
	UL				m_lngAgentID;						// 座席ID - Number Type
	PVOID			m_pSocket;							// 连接指针 

private:
	BYTE			m_nVersionNo;						// 版本号
	CManulSync		m_bLock;
};

#endif // !defined(AFX_IVRCLIENT_MSI_H__0C71E94E_FFEF_4CC5_BC68_9603BE157078__INCLUDED_)
