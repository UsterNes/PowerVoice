////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		FaxResource.cpp
// 用途描述：	传真资源类程序文件
// 作者：		SUN
// 创建时间：	2006/09/26
// 最后修改时间：2006/10/01
// 版权信息：	Copyright (C) 2003 - 2006 PicaSoft (香港智联)
// 备注：
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
#include "stdafx.h"
#include "FaxResource.h"

#include "_Global.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFaxResource::CFaxResource(Cchannelcontrol *f_pASR, long f_nID, long f_nType)
 : m_nID(f_nID),
   m_nType(f_nType)
{
	m_intLineNo = -1;
	m_bytStatus = IVR_IDLE;
	m_dwTotalBusy = 0;
	m_dwBusyTick = GetTickCount();
	m_bytSendResult = 0;
	m_pARS = f_pASR;

	WriteTrace(TraceDebug, "Debug Info - Fax Resouce id: %d is created.", f_nID);
}

CFaxResource::~CFaxResource()
{
	m_pARS = NULL;
	Channel_Stop();

	WriteTrace(TraceDebug, "Debug Info - Fax Resouce id: %d is destroyed.", m_nID);
}

long CFaxResource::GetFaxResID()
{
	return m_nID;
}

long CFaxResource::GetFaxResType()
{
	return m_nType;
}

BYTE CFaxResource::GetStatus(int *f_pintLineNo)
{
	BYTE lv_bytStatus;

	m_bLock.Enter();
	lv_bytStatus = m_bytStatus;
	if( f_pintLineNo )
		*f_pintLineNo = m_intLineNo;
	m_bLock.Leave();

	return lv_bytStatus;
}

// 设置传真资源状态、计时
/// 目前只需要两个状态：空闲(IVR_IDLE)、占用(IVR_W_FAX)
void CFaxResource::SetStatus(BYTE f_bytStatus, int f_intLineNo)
{
	DWORD lv_nBusyTime;

	m_bLock.Enter();
	if( f_bytStatus != m_bytStatus )
	{
		if( f_bytStatus == IVR_IDLE )
		{
			lv_nBusyTime = (GetTickCount() - m_dwBusyTick) / 1000;

			WriteTrace(TraceDetail, "Detail Info - Fax Resource id: %d is deassigned by IVR Channel: %d, using duration: %d (s).", 
				m_nID, m_intLineNo, lv_nBusyTime);
			m_dwTotalBusy += lv_nBusyTime;
		}
		else if( f_bytStatus == IVR_W_FAX )
		{
			m_dwBusyTick = GetTickCount();
			WriteTrace(TraceDetail, "Detail Info - Fax Resource id: %d is assigned by IVR Channel: %d.", m_nID, f_intLineNo);
		}

		m_bytStatus = f_bytStatus;
		m_intLineNo = f_intLineNo;
	}
	m_bLock.Leave();
}

Cchannelcontrol* CFaxResource::GetASRObj()
{
	return m_pARS;
}

DWORD CFaxResource::GetTotalBusyTime()
{
	return m_dwTotalBusy;
}

BOOL CFaxResource::Channel_Stop()
{
#ifdef INTEL_DIALOGIC
	// Stop FAX
	if( m_nID > 0 )
		if( fx_stopch(m_nID, EV_SYNC) == -1 )
			return FALSE;
#endif

	return TRUE;
}

// 返回传真发送结果
BYTE CFaxResource::GetSendResult()
{
	return m_bytSendResult;
}

// 发送传真文件
BOOL CFaxResource::SendFaxFile(const char *f_pFilePath, const char *f_strFromNo, const char *f_strToNo, const char *f_strHeader, int nRetryTimes)
{
	BOOL lv_retval = FALSE;
	int lv_nRC;
	
	if( m_pARS == NULL )
	{
		m_bytSendResult = DEF_FAX_EC_SYSTEM;
		return FALSE;
	}

	m_bytSendResult = DEF_FAX_EC_LCOFF;
	lv_nRC = m_pARS->fax_channel_send(f_pFilePath, f_strToNo, f_strHeader, f_strFromNo, nRetryTimes);
	switch( lv_nRC )
	{
	case -1:					// No file
	case -2:					// File open error
		m_bytSendResult = DEF_FAX_EC_FILE;
		break;
	case -3:					// Disconnected
		m_bytSendResult = DEF_FAX_EC_LCOFF;
		break;
	case -4:					// Init Error
		m_bytSendResult = DEF_FAX_EC_INITFAILED;
		break;
	case -5:					// Fax Send Error
		m_bytSendResult = DEF_FAX_EC_NOFAXTONE;
		break;
	case 0:						// Successful
		lv_retval = TRUE;
		break;
	}

	return lv_retval;
}

// 接收传真文件
BOOL CFaxResource::ReceiveFaxFile(const char *f_pFilePath, char *f_strFromNo, const char *f_strToNo, int nRetryTimes)
{
	BOOL lv_retval = FALSE;
	int lv_nRC;
	
	m_bytSendResult = DEF_FAX_EC_LCOFF;
	lv_nRC = m_pARS->fax_channel_receive(f_pFilePath, f_strFromNo, f_strToNo, nRetryTimes);
	switch( lv_nRC )
	{
	case -2:					// initstat failed
		m_bytSendResult = DEF_FAX_EC_INITFAILED;
		break;
	case -3:					// Fax Receive Error
		m_bytSendResult = DEF_FAX_EC_NOFAXTONE;
		break;
	case 0:						// Successful
		lv_retval = TRUE;
		break;
	}

	return lv_retval;
}

// Sun added 2012-07-13
// 停止传真操作
BOOL CFaxResource::StopFax()
{
	BOOL lv_retval = FALSE;
	int lv_nRC;
	
	lv_nRC = m_pARS->fax_channel_stopch();
	switch( lv_nRC )
	{
	case 0:						// Successful
		lv_retval = TRUE;
		break;
	}

	return lv_retval;
}