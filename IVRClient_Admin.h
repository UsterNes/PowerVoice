// IVRClient_Admin.h: interface for the CIVRClient_Admin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IVRCLIENT_ADMIN_H__2B41CF18_6747_48D9_9532_3F8F1F623D46__INCLUDED_)
#define AFX_IVRCLIENT_ADMIN_H__2B41CF18_6747_48D9_9532_3F8F1F623D46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIVRClient_Admin  
{
public:
	CIVRClient_Admin();
	virtual ~CIVRClient_Admin();

	BYTE GetVersion();
	void SetVersion(BYTE f_nValue);

	PVOID GetConnected();
	void SetConnected(PVOID f_cn);
	void SetDisconnected();

	int ProcessTCPMessage(SMsgHeader f_hdr, const BYTE *f_bdy);

protected:
	PVOID			m_pSocket;							// 连接指针 

private:
	BYTE			m_nVersionNo;						// 版本号
	CManulSync		m_bLock;
};

#endif // !defined(AFX_IVRCLIENT_ADMIN_H__2B41CF18_6747_48D9_9532_3F8F1F623D46__INCLUDED_)
