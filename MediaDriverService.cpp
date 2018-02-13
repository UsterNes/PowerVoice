// Sun added 2013-03-06 for Cisco Media Driver
#include "stdafx.h"
#include ".\mediadriverservice.h"

// Rtp Library日志回调函数
#ifdef CUCM_MEDIA

void WINAPI MediaDriverAppTracer(int level, const _TCHAR * pbuf)
{
	UINT lv_nMyTrace;

	switch( level )
	{
	case 0:			// Error
		lv_nMyTrace = TraceError;
		break;
	case 1:			// Alarm
		lv_nMyTrace = TraceWarn;
		break;
	case 2:			// Warning
		lv_nMyTrace = TraceInfo;
		break;
	case 3:			// Info
		lv_nMyTrace = TraceDebug;
		break;
	default:		// 4 - Debug
		lv_nMyTrace = TraceDetail;
		break;
	}

	WriteTrace(lv_nMyTrace, "MediaDriverLog - %s", pbuf);
}

#endif

CMediaDriverService::CMediaDriverService(void) :
 m_blnInitialized(false),
 m_nTraceLevel(2)
{
	m_nPortRangeStart = 21100;
	m_nNumPorts = 600;

#ifdef CUCM_MEDIA
	m_pRTPAddr = NULL;
#endif
}

CMediaDriverService::~CMediaDriverService(void)
{
#ifdef CUCM_MEDIA
	CloseAPI();
#endif
}

#ifdef CUCM_MEDIA

bool CMediaDriverService::IsAPIInitialized()
{
	return m_blnInitialized;
}

// 初始化Media Driver API
bool CMediaDriverService::InitAPI(UINT portStart, UINT numPorts)
{
	bool lv_retval = false;
	UINT lv_nPortStart = 0;
	UINT lv_nPortNum = 0;

	// 避免重复初始化
	if( m_blnInitialized )
	{
		WriteTrace(TraceWarn, "CMediaDriverService::InitAPI() already initialized! Don't try to init again.");
		return true;
	}

	// 判断参数是否合法，如果不合法，则使用最近一次的正确配置
	if( portStart > 0 )
		lv_nPortStart = portStart;
	else
		lv_nPortStart = m_nPortRangeStart;

	if( numPorts > 0 )
		lv_nPortNum = numPorts;
	else
		lv_nPortNum = m_nNumPorts;

	// 尝试初始化Media Driver库
	try
	{
		PRTPLIBTRACE lv_pCb = MediaDriverAppTracer;
		// IPAddressFamily: 
		// – AF_UNSPEC: both AF_INET and AF_INET6 can be used.
		// – AF_INET: AF_INET only can be used.
		// – AF_INET6: AF_INET6 only can be used.
		lv_retval = EpApiInit(lv_pCb, lv_nPortStart, lv_nPortNum, AF_UNSPEC, NULL);
		if( !lv_retval )
		{
			WriteTrace(TraceError, "Failed to initialize Cisco RTP Lib, error = %d, port start = %d, port num = %d", 
				EpApiGetLastError(), lv_nPortStart, lv_nPortNum);
		}
	}
	catch(...)
	{
		WriteTrace(TraceError, "CMediaDriverService::InitAPI() encountered system error when called EpApiInit()");
	}

	// 设置日志级别
	EpApiTraceLevelSet(m_nTraceLevel);

	// 更新最近一次的正确配置
	if( lv_retval )
	{
		m_nPortRangeStart = lv_nPortStart;
		m_nNumPorts = lv_nPortNum;
	}

	//----------------------------------------------------------
	// Sun added 2013-04-12
	/// Initialize RTP Address
	int lv_nCurSize, lv_nActualSize = 0, lv_nItems = 0, lv_nIPs = 0;

	lv_nIPs = EpLocalAddrGetAll(NULL, lv_nActualSize);
	if( lv_nIPs <= 0 )
	{
		WriteTrace(TraceError, "CMediaDriverServicel::InitAPI() can not determine the size of RTP Addresses, may larger than %d bytes and %d items. Error = 0x%x", lv_nActualSize, lv_nItems, EpApiGetLastError());
		return false;
	}
	else
		WriteTrace(TraceInfo, "CMediaDriverServicel::InitAPI() find total %d local IP addresses.", lv_nIPs);

	// 计算实际需要多少个结构体
	lv_nItems = (int)((lv_nActualSize - 1) / sizeof(RTPADDR)) + 1;
	lv_nCurSize = lv_nItems * sizeof(RTPADDR);
	m_pRTPAddr = new RTPADDR[lv_nItems];
	ZeroMemory(m_pRTPAddr, lv_nCurSize);
	lv_nIPs = EpLocalAddrGetAll(m_pRTPAddr, lv_nActualSize);
	if( lv_nIPs <= 0 )
	{
		WriteTrace(TraceError, "CMediaDriverServicel::InitAPI() can not determine the size of RTP Addresses, may larger than %d bytes and %d items. Error = 0x%x", lv_nActualSize, lv_nItems, EpApiGetLastError());
		return false;
	}
	//----------------------------------------------------------

	m_blnInitialized = lv_retval;
	return lv_retval;
}

// 关闭Media Driver API
bool CMediaDriverService::CloseAPI()
{
	if( !m_blnInitialized )
		return true;

	bool lv_epRet = EpApiClose();
	if( !lv_epRet )
		WriteTrace(TraceError, "Failed to close Cisco RTP Lib, error = %d", EpApiGetLastError());

	m_blnInitialized = false;

#ifdef CUCM_MEDIA
	if( m_pRTPAddr )
		delete []m_pRTPAddr;
#endif

	return lv_epRet;
}


#endif
