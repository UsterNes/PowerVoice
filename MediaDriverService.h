// Sun added 2012-02-08 for Cisco Media Driver
#pragma once

class CMediaDriverService
{
public:
	CMediaDriverService(void);
	virtual ~CMediaDriverService(void);

	UINT	m_nPortRangeStart;					// UDP起始端口
	UINT	m_nNumPorts;						// UDP端口数量（注意：一个IVR通道需要占用4个UDP端口）
	int		m_nTraceLevel;						// Media Driver的系统日志级别（0 - Error; 1 - Alarm; 2 - Warning; 3 - Info; 4 - Debug）

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
