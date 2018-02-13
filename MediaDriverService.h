// Sun added 2012-02-08 for Cisco Media Driver
#pragma once

class CMediaDriverService
{
public:
	CMediaDriverService(void);
	virtual ~CMediaDriverService(void);

	UINT	m_nPortRangeStart;					// UDP��ʼ�˿�
	UINT	m_nNumPorts;						// UDP�˿�������ע�⣺һ��IVRͨ����Ҫռ��4��UDP�˿ڣ�
	int		m_nTraceLevel;						// Media Driver��ϵͳ��־����0 - Error; 1 - Alarm; 2 - Warning; 3 - Info; 4 - Debug��

protected:
	bool	m_blnInitialized;

#ifdef CUCM_MEDIA
public:
	bool InitAPI(UINT portStart = 0, UINT numPorts = 0);
	bool CloseAPI();
	bool IsAPIInitialized();

	PRTPADDR m_pRTPAddr;

#endif

};
