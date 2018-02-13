////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		TTSInterface.cpp
// ��;������	TTS�ӿ���ʵ���ļ�
// ���ߣ�		SUN
// ����ʱ�䣺	2004/11/20
// ����޸�ʱ�䣺2007/10/08
// ��Ȩ��Ϣ��	Copyright (C) 2003 - 2007 PicaSoft (�������)
// ��ע��
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
#include "stdafx.h"
#include "TTSInterface.h"

#ifdef USE_SINOVOICE_TTS
	#include "SinoVoice\Inc\jTTS_ML.h"
#else
	#include "InterPhonic\Inc\iFly_TTS.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifdef USE_SINOVOICE_TTS

// �ϳɹ����еĻص�����
BOOL TTSAPI AsyncDataCallbackProc(DWORD dwSessionID, WORD wParam, LONG lParam, 
									   unsigned char * pVoiceData, DWORD dwLen, 
									   INSERTINFO* pInsertInfo, int nInsertInfo, 
									   DWORD dwUserData)
{
	try
	{
		CTTSInterface::CTTSConnection* lv_pCn = (CTTSInterface::CTTSConnection *)dwUserData;

		// ֹͣ�ϳ�
		if( lv_pCn->GetCancelFlag() )
		{
			WriteTrace(TraceDetail, "TTS Connection: %d received cancel_flag", lv_pCn->GetIndex());
			return FALSE;
		}

		if( wParam == NOTIFY_BEGIN )
		{
			// �ϳɿ�ʼ
			WriteTrace(TraceDebug, "TTS Connection: %d received data_start_flag, total %d letters will be read.", lv_pCn->GetIndex(), lParam);
		}
		else if( wParam == NOTIFY_END )
		{
			// �ϳ����
			WriteTrace(TraceDebug, "TTS Connection: %d received data_end_flag, return code = %d.", lv_pCn->GetIndex(), lParam);

			// ֪ͨ�����ںϳ����
			lv_pCn->SetUsed(0);
		}
		else if( wParam == NOTIFY_SENTBEGIN )
		{
			// �Ķ��Ľ��� - ��ʼ
			WriteTrace(TraceDetail, "TTS Connection: %d received data_sentbegin_flag, total %d letters will be sended.", lv_pCn->GetIndex(), lParam);
		}
		else if( wParam == NOTIFY_SENTEND )
		{
			// �Ķ��Ľ��� - ����
			WriteTrace(TraceDetail, "TTS Connection: %d received data_sentend_flag, total %d letters are sended.", lv_pCn->GetIndex(), lParam);
		}
		else if( wParam == NOTIFY_CHANGECONFIG )
		{
			// ��������
			WriteTrace(TraceDebug, "TTS Connection: %d received data_changeconfig_flag, parameter = %d.", lv_pCn->GetIndex(), lParam);
		}
		else if( wParam == NOTIFY_DATA )
		{
			// Haha: ����
			WriteTrace(TraceDetail, "TTS Connection: %d received %d bytes data", lv_pCn->GetIndex(), dwLen);
			if( dwLen > 0 )
			{
				// Copy and clear TTS buffer
				lv_pCn->ReceiveTTSData((const BYTE *)pVoiceData, dwLen);
			}
		}
	}
	catch(...)
	{
		WriteTrace(TraceWarn, "TTS Warn - jTTS error in call back function, please check the TTS engine!!!");
		return FALSE;
	}
	
	return TRUE;
}

#else

// �ϳɹ����еĻص�����
TTSRETVAL SynthProcessProc(HTTSINSTANCE m_hTTSInstance, PTTSData pTTSData, TTSINT32 lParam, PTTSVOID pUserData)
{
	try
	{
		CTTSInterface::CTTSConnection* lv_pCn = (CTTSInterface::CTTSConnection *)pUserData;

		// ֹͣ�ϳ�
		if( lv_pCn->GetCancelFlag() )
		{
			WriteTrace(TraceDetail, "TTS Connection: %d received cancel_flag", lv_pCn->GetIndex());
			return TTSERR_CANCELED;
		}

		// Copy and clear TTS buffer
		if( pTTSData->dwOutBufSize > 0 )
		{
			WriteTrace(TraceDetail, "TTS Connection: %d received %d bytes data", lv_pCn->GetIndex(), pTTSData->dwOutBufSize);
			lv_pCn->ReceiveTTSData((const BYTE *)pTTSData->pOutBuf, pTTSData->dwOutBufSize);
		}

		if( pTTSData->dwOutFlags == TTS_FLAG_DATA_END )
		{
			WriteTrace(TraceDetail, "TTS Connection: %d received data_end_flag", lv_pCn->GetIndex());

			// ֪ͨ�����ںϳ����
			lv_pCn->SetUsed(0);
		}
	}
	catch(...)
	{
		WriteTrace(TraceWarn, "TTS Warn - iFlyTTS error in call back function, please check the TTS engine!!!");
	}
	
	return pTTSData->dwErrorCode;
}

// �ϳɹ��̵��¼��ص�����
TTSRETVAL SynthEventProc(HTTSINSTANCE hTTSInst, PTTSData pTTSData, TTSINT16 nNotify, TTSINT32 lParam, PTTSVOID pUserData)
{
	return TTSERR_OK;
}

#endif

//////////////////////////////////////////////////////////////////////
// CTTSServerInfo Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTTSServerInfo::CTTSServerInfo()
 :	m_strServerIP(_T("")),
	m_nServerPort(0),
	m_strCompanyName(_T("PicaSoft")),
	m_strUserName(_T("PowerVoice")),
	m_strProductName(_T("Power Voice Server")),
	m_strSerialNO(_T("")),
	m_nCodePage(0),
	m_nVID(0),
	m_nSpeed(0),
	m_nVolume(0),
	m_nPitch(0),
	m_nTextType(0),
	m_nBackAudioIndex(0),
	m_nBackAudioVolume(0),
	m_nBackAudioFlag(0)

// Dialogicʹ��VOX
#ifdef INTEL_DIALOGIC

#ifdef USE_SINOVOICE_TTS
	,m_nAudioFmt(FORMAT_VOX_8K),
	m_nAudioHeadFmt(PLAYTOFILE_NOHEAD)
#else
	,m_nAudioFmt(TTS_ADF_VOX8K1C),
	m_nAudioHeadFmt(TTS_AHF_NONE)
#endif

#endif

// CISCO CCMʹ��8K, 16Bit, 1Channel WAVE
#ifdef CISCO_CCM

#ifdef USE_SINOVOICE_TTS
	,m_nAudioFmt(FORMAT_WAV_8K16B),
	m_nAudioHeadFmt(PLAYTOFILE_NOHEAD)
#else
	,m_nAudioFmt(TTS_ADF_PCM8K16B1C),
	m_nAudioHeadFmt(TTS_AHF_NONE)
#endif

#endif
{
#ifdef USE_SINOVOICE_TTS
// jTTS Ĭ�Ϻϳɲ���
	m_nCodePage = CODEPAGE_GB;
	m_nBackAudioIndex = BACKAUDIO_NONE;
	m_nBackAudioVolume = 50;
	m_nBackAudioFlag = BACKAUDIOFLAG_REPEAT;
#else
// iFly TTSĬ�Ϻϳɲ���
#endif
}

CTTSServerInfo::~CTTSServerInfo()
{
}

const CTTSServerInfo& CTTSServerInfo::operator = (const CTTSServerInfo& ServerInfoScr)
{
	m_strServerIP = ServerInfoScr.m_strServerIP;
	m_strCompanyName = ServerInfoScr.m_strCompanyName;
	m_strUserName = ServerInfoScr.m_strUserName;
	m_strProductName = ServerInfoScr.m_strProductName;
	m_strSerialNO = ServerInfoScr.m_strSerialNO;
	m_nServerPort = ServerInfoScr.m_nServerPort;
	m_nCodePage = ServerInfoScr.m_nCodePage;
	m_nVID = ServerInfoScr.m_nVID;
	m_nAudioFmt = ServerInfoScr.m_nAudioFmt;
	m_nSpeed = ServerInfoScr.m_nSpeed;
	m_nVolume = ServerInfoScr.m_nVolume;
	m_nPitch = ServerInfoScr.m_nPitch;
	m_nAudioHeadFmt = ServerInfoScr.m_nAudioHeadFmt;
	m_nTextType = ServerInfoScr.m_nTextType;

	// Sun added 2007-10-20
	/// ��������
	m_nBackAudioIndex = ServerInfoScr.m_nBackAudioIndex;
	m_nBackAudioVolume = ServerInfoScr.m_nBackAudioVolume;
	m_nBackAudioFlag = ServerInfoScr.m_nBackAudioFlag;

	return *this;
}

//////////////////////////////////////////////////////////////////////
// CTTSInterface::CTTSConnection::CTTSBuffer Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTTSInterface::CTTSConnection::CTTSBuffer::CTTSBuffer(int f_nIndex, UINT f_nBufferSize)
 :	m_nIndex(f_nIndex),
	m_Tag(0),
	m_dwLockTick(0),
	m_length(0),
	m_maxlen(0),
	m_pBuffer(NULL)
{
	CreateDataBuffer(f_nBufferSize * 1024);
}

CTTSInterface::CTTSConnection::CTTSBuffer::~CTTSBuffer()
{
	ClearBuffer();
}

int CTTSInterface::CTTSConnection::CTTSBuffer::GetBufferIndex()
{
	return m_nIndex;
}

void CTTSInterface::CTTSConnection::CTTSBuffer::CreateDataBuffer(long f_nlen)
{
	if( f_nlen > 0 )
	{
		m_sync.Enter ();
		if( m_pBuffer )
			delete[] m_pBuffer;
		m_pBuffer = new BYTE[f_nlen];
		m_length = 0;
		m_maxlen = f_nlen;
		m_sync.Leave ();
	}
}

void CTTSInterface::CTTSConnection::CTTSBuffer::ClearBuffer()
{
	m_sync.Enter ();
	if( m_pBuffer )
	{
		delete[] m_pBuffer;
		m_pBuffer = NULL;
	}
	m_length = 0;
	m_maxlen = 0;
	m_sync.Leave ();
}

int CTTSInterface::CTTSConnection::CTTSBuffer::GetTag()
{
	return m_Tag;
}

void CTTSInterface::CTTSConnection::CTTSBuffer::ResetTag()
{
	m_sync.Enter ();
	m_Tag = 0;
	m_sync.Leave ();
}

int CTTSInterface::CTTSConnection::CTTSBuffer::GetBufferData(BYTE *f_Data)
{
	int lv_len = 0;

	m_sync.Enter ();
	try
	{
		if( f_Data )
		{
			memcpy(f_Data, m_pBuffer, m_length);
			lv_len = m_length;
		}
	}
	catch(...)
	{
	}
	m_sync.Leave ();

	return lv_len;
}

BOOL CTTSInterface::CTTSConnection::CTTSBuffer::SetBufferData(const BYTE *f_Data, int f_Len, int f_Tag)
{
	if( m_Tag > 0 )
		return FALSE;

	m_sync.Enter ();

	if( f_Len < m_maxlen )
		m_length = f_Len;
	else
		m_length = m_maxlen;

	memcpy(m_pBuffer, f_Data, m_length);
	m_Tag = f_Tag;

	m_sync.Leave ();

	m_dwLockTick = GetTickCount();
	return TRUE;
}

DWORD CTTSInterface::CTTSConnection::CTTSBuffer::GetLockTick()
{
	if( m_Tag <= 0 )
		return 0;

	return m_dwLockTick;
}

//////////////////////////////////////////////////////////////////////
// CTTSInterface::CTTSConnection Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTTSInterface::CTTSConnection::CTTSConnection(UINT f_nIndex, UINT f_nBuffers, UINT f_nBufferSize)
 :	m_nIndex(f_nIndex),
	m_nOwner(0),
	m_nWriteBufferPtr(0),
	m_blnCancel(FALSE),
	m_nBufferCount(f_nBuffers),
	m_nBufferSize(f_nBufferSize),
	m_hTTSInstance(NULL),
	m_nCurrentSessionID(0)
{
	// ����������
	UINT lv_loop;

	for( lv_loop = 0; lv_loop < f_nBuffers; lv_loop++ )
		m_BufferList[lv_loop] = new CTTSBuffer((int)lv_loop, f_nBufferSize);
}

CTTSInterface::CTTSConnection::~CTTSConnection()
{
	// �Ͽ�����
	DisconnectServer();

	// ���ٻ�����
	RemoveAllBuffers();
}

// �������л�����
void CTTSInterface::CTTSConnection::RemoveAllBuffers()
{
	try
	{
		for( UINT lv_loop = 0; lv_loop < m_nBufferCount; lv_loop++ )
		{
			if( m_BufferList[lv_loop] )
			{
				delete m_BufferList[lv_loop];
				m_BufferList[lv_loop] = NULL;
			}
		}
	}
	catch(...)
	{
		WriteTrace(TraceError, "CTTSInterface::CTTSConnection::RemoveAllBuffers() runs error!!!");
	}
}

// �ͷű�ռ�õĻ�����
void CTTSInterface::CTTSConnection::ReleaseLockedBuffers(int f_nChannelID)
{
	try
	{
		int lv_curid;
		for( UINT lv_loop = 0; lv_loop < m_nBufferCount; lv_loop++ )
		{
			if( m_BufferList[lv_loop] )
			{
				if( f_nChannelID > 0 )
				{
					lv_curid = m_BufferList[lv_loop]->GetTag();
					if( lv_curid == f_nChannelID )
						m_BufferList[lv_loop]->ResetTag();
				}
				else
					m_BufferList[lv_loop]->ResetTag();
			}
		}
	}
	catch(...)
	{
		WriteTrace(TraceError, "CTTSInterface::CTTSConnection::ReleaseLockedBuffers(%d) runs error!!!", f_nChannelID);
	}
}

// ����TTS���ӱ��
UINT CTTSInterface::CTTSConnection::GetIndex()
{
	return m_nIndex;
}

// TTS�����Ƿ����
int CTTSInterface::CTTSConnection::IsTaken()
{
	int lv_retval;

	if( !IsConnected() )
		return -1;

	m_bLock.Enter();
	lv_retval = m_nOwner;
	m_bLock.Leave();

	return lv_retval;
}

// ռ�ñ�����
void CTTSInterface::CTTSConnection::SetUsed(int f_nOwner)
{
	m_bLock.Enter();
	if( m_nOwner != f_nOwner )
	{
		if( f_nOwner == 0 )
			WriteTrace(TraceDebug, "TTS Connection: %d used by UserID: %d is released.", m_nIndex, m_nOwner);
		else
			WriteTrace(TraceDebug, "TTS Connection: %d is used by UserID: %d.", m_nIndex, f_nOwner);

		m_nOwner = f_nOwner;

#ifndef USE_SINOVOICE_TTS
		//TTSClean(m_hTTSInstance);
#endif
	}
	m_bLock.Leave();
}

// ��ȡ��ֹ�ϳɱ��
BOOL CTTSInterface::CTTSConnection::GetCancelFlag()
{
	return m_blnCancel;
}

// ������ֹ�ϳɱ��
void CTTSInterface::CTTSConnection::SetCancelFlag(BOOL f_blnFlag)
{
	if( m_blnCancel != f_blnFlag )
		m_blnCancel = f_blnFlag;

	// Sun added 2007-10-20
#ifdef USE_SINOVOICE_TTS
	try
	{
		if( f_blnFlag )
		{
			// ����л�Ự�����ȹر�֮
			if( m_nCurrentSessionID > 0 )
			{
				jTTS_SessionStop(m_nCurrentSessionID);
				m_nCurrentSessionID = 0;
			}
		}
	}
	catch(...)
	{
		WriteTrace(TraceWarn, "TTS Warn - jTTS failed to call jTTS_SessionStop() on Connection: %d, please check the TTS engine!!!", m_nIndex);
		m_nCurrentSessionID = 0;
	}
#endif
}

// ��黺�����Ƿ�����
BOOL CTTSInterface::CTTSConnection::CheckDeadLock()
{
	try
	{
		CTTSBuffer *lv_pBuff;
		DWORD lv_tickStart, lv_tickNow;
		
		lv_tickNow = GetTickCount();

		for( UINT lv_loop = 0; lv_loop < m_nBufferCount; lv_loop++ )
		{
			lv_pBuff = m_BufferList[lv_loop];
			if( lv_pBuff )
			{
				lv_tickStart = lv_pBuff->GetLockTick();
				if( lv_tickStart > 0 )
				{
					// Sun added '* m_nBufferCount' 2008-01-21
					if( lv_tickNow - lv_tickStart > DEF_TTS_CST_BUFFER_LOCKTIMEOUT * 1000 * m_nBufferCount )
					{
						WriteTrace(TraceWarn, "TTS buffer: %d of channel: %d is locked for too long time!!!", lv_loop, lv_pBuff->GetTag());
						lv_pBuff->ResetTag();
					}
				}
			}
		}
	}
	catch(...)
	{
		WriteTrace(TraceError, "CTTSInterface::CTTSConnection::CheckDeadLock() runs error!!!");
	}

	return TRUE;
}

// Sun added 2008-11-16
/// ������ӿ���ʱ���Ƿ���ã��������iFlyTTS��Ҫ����һ��
BOOL CTTSInterface::CTTSConnection::CheckMaxIdleDuration()
{
#ifndef USE_SINOVOICE_TTS
// iFly TTS TTS Connetion Idle Check
	if( m_hTTSInstance )
	{
		TTSINT32 dwErr;
		TTSINT32 lv_nVID;		// Temp TTS Voice Lib

		if( GetTickCount() - m_dwIdleTick > (DWORD)1800000 )	// Half an hour
		{
			m_dwIdleTick = GetTickCount();
			WriteTrace(TraceDebug, "TTS Debug - TTS Connection: %d idle check invoked.", m_nIndex);

			/// ͨ����ѯ�ϳɲ���������TTS����
			dwErr = TTSGetSynthParam(m_hTTSInstance, TTS_PARAM_VID, &lv_nVID);
			if( dwErr != TTSERR_OK )
				WriteTrace(TraceWarn, "TTS Connection: %d TTSGetSynthParam return error code = 0x%00x.", m_nIndex, TTSGETERRCODE(dwErr));

			return FALSE;
		}
	}
#endif

	return TRUE;
}

// ����TTS Server
BOOL CTTSInterface::CTTSConnection::ConnectServer(CTTSServerInfo f_Server)
{
	if( m_hTTSInstance == NULL )
	{
		// Sun added 2008-11-16
		m_dwIdleTick = GetTickCount();

#ifdef USE_SINOVOICE_TTS
// SinoVoice jTTS opens Connetion
		// ���غϳɲ���Ҫ���ӷ�������
		/// �������Ӿ��ֱ������ΪIndex+1����Ϊm_nIndex��0��ʼ������
		m_hTTSInstance = (HANDLE)(m_nIndex + 1);

#else
// iFly TTS opens Connetion

		// ��TTSϵͳ��������
		TTSConnectStruct TtsConnect;
		memset(&TtsConnect, 0, sizeof(TTSConnectStruct));
		TtsConnect.dwSDKVersion = IFLYTTS_SDK_VER;
		strcpy(TtsConnect.szCompanyName, (LPCSTR)f_Server.m_strCompanyName);
		strcpy(TtsConnect.szUserName, (LPCSTR)f_Server.m_strUserName);
		strcpy(TtsConnect.szProductName, (LPCSTR)f_Server.m_strProductName);
		strcpy(TtsConnect.szSerialNumber, (LPCSTR)f_Server.m_strSerialNO);
		// Comment: Need network runtime library's support
		strcpy(TtsConnect.szTTSServerIP, (LPCSTR)f_Server.m_strServerIP);
		
		m_hTTSInstance = TTSConnect(&TtsConnect);
		if( m_hTTSInstance == NULL )
		{
			if( TTSGETERRCODE(TtsConnect.dwErrorCode) == TTSERR_INVALIDSN )
				WriteTrace(TraceError, "Open TTS connection error: invalid serial number!!!");
			else
				WriteTrace(TraceError, "Open TTS connection error code = 0x%00x!!!", TtsConnect.dwErrorCode);
			return FALSE;
		}
#endif

		// Sun added 2007-10-20
		m_nCurrentSessionID = 0;

		// Record server info.
		m_ServerInfo = f_Server;

		// Set parameters
		SetSynthParameters();

		WriteTrace(TraceDebug, "TTS Connection: %d is opened.", m_nIndex);
	}

	return TRUE;
}

// ����������ô�����Ϣ
int CTTSInterface::CTTSConnection::OutputSetParamError(LPCSTR f_szParaName, UINT f_nParaValue, UINT f_nErrorCode)
{
#ifdef USE_SINOVOICE_TTS
	
	switch( f_nErrorCode )
	{
	case ERR_NONE:
		return 1;
		break;

	case ERR_SECURITY:
		WriteTrace(TraceError, "jTTS called %s return error %d. Faied to initialize SinoVoice jTTS engine! SN is wrong or corrupted!!!", f_szParaName, f_nErrorCode);
		return -1;
		break;

	case ERR_NOTINIT:
		WriteTrace(TraceError, "jTTS called %s return error %d. TTS engine was not initialized!!!", f_szParaName, f_nErrorCode);
		return -1;
		break;

	case ERR_MEMORY:
		WriteTrace(TraceError, "jTTS called %s return error %d. TTS engine encounted memory error!!!", f_szParaName, f_nErrorCode);
		return -1;
		break;

	case ERR_INVALIDHWND:
		WriteTrace(TraceError, "jTTS called %s return error %d. Invalid handle(0x%x)!!!", f_szParaName, f_nErrorCode, f_nParaValue);
		return -1;
		break;

	case ERR_INVALIDFUNC:
		WriteTrace(TraceError, "jTTS called %s return error %d. Invalid function(0x%x)!!!", f_szParaName, f_nErrorCode, f_nParaValue);
		return -1;
		break;

	case ERR_INVALIDTEXT:
		WriteTrace(TraceError, "jTTS called %s return error %d. Invalid text!!!", f_szParaName, f_nErrorCode);
		return -1;
		break;

	case ERR_NOTSUPPORT:
		WriteTrace(TraceError, "jTTS called %s return error %d. Parameter type(%d) is not supported!!!", f_szParaName, f_nErrorCode, f_nParaValue);
		return -1;
		break;

	case ERR_TOOMANYSESSION:			// ��ǰ�����߳����Ѿ�������Ȩ�߳���
		WriteTrace(TraceError, "jTTS called %s return error %d. TTS licenses are used up!!!", f_szParaName, f_nErrorCode);
		return 0;
		break;

	case ERR_NEEDRESTART:				// ����ϵͳ��������ʱ���Ѿ�������Ȩʱ�䣬��Ҫ��������
		WriteTrace(TraceError, "jTTS called %s return error %d. TTS server needs to restart!!!", f_szParaName, f_nErrorCode);
		return -1;
		break;

	case ERR_EXPIRED:					// ��ǰʱ���Ѿ�������Ȩ����ʱ��
		WriteTrace(TraceError, "jTTS called %s return error %d. TTS service is expired!!!", f_szParaName, f_nErrorCode);
		return -1;
		break;

	case ERR_CONFIG:					// ֵ����Ч��Χ֮�� 
		WriteTrace(TraceError, "jTTS called %s return error %d. Parameter value(%d) is out of range!!!", f_szParaName, f_nErrorCode, f_nParaValue);
		break;

	case ERR_PARAM:						// �������Ϸ�
		WriteTrace(TraceError, "jTTS called %s return error %d. Parameter is not supported!!!", f_szParaName, f_nErrorCode);
		return 2;			// �������ò��˳����ϳ��˳�
		break;

	default:
		WriteTrace(TraceError, "jTTS SetSynthParameters(%s, %d) runs error code = 0x%00x!", f_szParaName, f_nParaValue, f_nErrorCode);
	}

#else

	UINT lv_errcode = TTSGETERRCODE(f_nErrorCode);
	switch( lv_errcode )
	{
	case TTSERR_OK:
		return 1;
		break;

	case TTSERR_INVALIDHANDLE:
		WriteTrace(TraceError, "SetSynthParameters(%s, %d) runs error: invalid connection handle!!!", f_szParaName, f_nParaValue);
		return -1;
		break;

	case TTSERR_NOTSUPP:
		WriteTrace(TraceError, "SetSynthParameters(%s, %d) runs error: parameter type is not supported!!!", f_szParaName, f_nParaValue);
		break;

	case TTSERR_INVALIDPARA:
		WriteTrace(TraceError, "SetSynthParameters(%s, %d) runs error: invalid parameter value!!!", f_szParaName, f_nParaValue);
		break;

	default:
		WriteTrace(TraceError, "SetSynthParameters(%s, %d) runs error code = 0x%00x!", f_szParaName, f_nParaValue, lv_errcode);
	}

#endif

	return 0;
}

// ���úϳɲ���
BOOL CTTSInterface::CTTSConnection::SetSynthParameters()
{
#ifdef USE_SINOVOICE_TTS
// SinoVoice jTTS set Synthesize Parameters

	ERRCODE lv_errCode;

	/// ȡĿǰ��������
	JTTS_VOICEATTRIBUTE lv_Attribute;

	while( true )
	{
		lv_errCode = jTTS_GetVoiceAttribute(m_ServerInfo.m_nVID, &lv_Attribute);
		if( lv_errCode != ERR_NONE )
		{
			OutputSetParamError(_T("jTTS_GetVoiceAttribute"), m_ServerInfo.m_nVID, (UINT)lv_errCode);
			/// �Զ��л���Ĭ�������⣬�ٴγ���
			if( m_ServerInfo.m_nVID != 0 )
			{
				WriteTrace(TraceWarn, "Warn - TTS automatically switches voice library from %d to default(0)!", m_ServerInfo.m_nVID);
				m_ServerInfo.m_nVID = 0;
				continue;
			}
		}
		break;
	}
	/// ���ѡ��������ʧ�ܣ�����ָ����Ĭ�ϣ����򷵻ش���
	if( lv_errCode != ERR_NONE )
		return FALSE;

	/// �����������С��KBΪ��λ��16K-8M��Ĭ��128K��
	lv_errCode = jTTS_SetParam(PARAM_VOICEBUFSIZE, m_nBufferSize);
	if( OutputSetParamError(_T("jTTS_SetParam(PARAM_VOICEBUFSIZE)"), m_nBufferSize, (UINT)lv_errCode) < 0 )
		return FALSE;

	/// ����ҳ
	lv_errCode = jTTS_SetParam(PARAM_CODEPAGE, m_ServerInfo.m_nCodePage);
	if( OutputSetParamError(_T("jTTS_SetParam(PARAM_CODEPAGE)"), m_ServerInfo.m_nCodePage, (UINT)lv_errCode) < 0 )
		return FALSE;

	/// ���������ͣ�����ֵ��String���ͣ�ֵ�������ID
    lv_errCode = jTTS_SetParam(PARAM_VOICEID, (DWORD)(&lv_Attribute.szVoiceID[0]));
	if( OutputSetParamError(_T("jTTS_SetParam(PARAM_VOICEID)"), m_ServerInfo.m_nVID, (UINT)lv_errCode) < 0 )
		return FALSE;

	/// �ϳ����٣�����0~9��Ϊ10����ȱʡֵΪ5������Խ������Խ�졣 
	lv_errCode = jTTS_SetParam(PARAM_SPEED, m_ServerInfo.m_nSpeed);
	if( OutputSetParamError(_T("jTTS_SetParam(PARAM_SPEED)"), m_ServerInfo.m_nSpeed, (UINT)lv_errCode) < 0 )
		return FALSE;

	/// �������������0~9��Ϊ10����ȱʡֵΪ5������Խ������Խ�� 
	lv_errCode = jTTS_SetParam(PARAM_VOLUME, m_ServerInfo.m_nVolume);
	if( OutputSetParamError(_T("jTTS_SetParam(PARAM_VOLUME)"), m_ServerInfo.m_nVolume, (UINT)lv_errCode) < 0 )
		return FALSE;

	/// ������ߣ�����0~9��Ϊ10����ȱʡֵΪ5������Խ���ƵԽ�ߡ�
	lv_errCode = jTTS_SetParam(PARAM_PITCH, m_ServerInfo.m_nPitch);
	if( OutputSetParamError(_T("jTTS_SetParam(PARAM_PITCH)"), m_ServerInfo.m_nPitch, (UINT)lv_errCode) < 0 )
		return FALSE;

	/// �ı����ͣ���ע�Ĵ���ʽ��ȱʡΪTAG_AUTO
	//// TAG_AUTO���Զ��ж�TAG��Ҳ��ͬʱ��ѯJTTS Tag��SSMLTag
	//// TAG_JTTS����������jTTS 3.0֧�ֵ�TAG������\read=a1\
	//// TAG_SSML��������SSML TAG������ <voice gender="female" />
	//// TAG_NONE���ı������κ�TAG������TAG���ᱻ����
	lv_errCode = jTTS_SetParam(PARAM_TAGMODE, m_ServerInfo.m_nTextType);
	if( OutputSetParamError(_T("jTTS_SetParam(PARAM_TAGMODE)"), m_ServerInfo.m_nTextType, (UINT)lv_errCode) < 0 )
		return FALSE;

	/// �������ֱ�ţ�0��ʾû��
	lv_errCode = jTTS_SetParam(PARAM_BACKAUDIO, m_ServerInfo.m_nBackAudioIndex);
	if( OutputSetParamError(_T("jTTS_SetParam(PARAM_BACKAUDIO)"), m_ServerInfo.m_nBackAudioIndex, (UINT)lv_errCode) < 0 )
		return FALSE;

	/// ����������������������������0~100��ȱʡΪ50��
	lv_errCode = jTTS_SetParam(PARAM_BACKAUDIOVOLUME, m_ServerInfo.m_nBackAudioVolume);
	if( OutputSetParamError(_T("jTTS_SetParam(PARAM_BACKAUDIOVOLUME)"), m_ServerInfo.m_nBackAudioVolume, (UINT)lv_errCode) < 0 )
		return FALSE;

	/// �����������ԣ������������ԣ�Ŀǰ�����ֵΪ��
	//// 0: ���ظ�����
	//// BACKAUDIOFLAG_REPEAT���ظ�����
	lv_errCode = jTTS_SetParam(PARAM_BACKAUDIOFLAG, m_ServerInfo.m_nBackAudioFlag);
	if( OutputSetParamError(_T("jTTS_SetParam(PARAM_BACKAUDIOFLAG)"), m_ServerInfo.m_nBackAudioFlag, (UINT)lv_errCode) < 0 )
		return FALSE;

#else
// iFly TTS set Synthesize Parameters
	TTSINT32 dwErr;

	/// �����������С���ֽ�Ϊ��λ
	dwErr = TTSSetSynthParam(m_hTTSInstance, TTS_PARAM_OUTBUFSIZE, m_nBufferSize * 1024);
	if( dwErr != TTSERR_OK )
		if( OutputSetParamError(_T("TTS_PARAM_OUTBUFSIZE"), m_nBufferSize * 1024, dwErr) < 0 )
			return FALSE;

	/// ����ҳ
	dwErr = TTSSetSynthParam(m_hTTSInstance, TTS_PARAM_CODEPAGE, m_ServerInfo.m_nCodePage);
	if( dwErr != TTSERR_OK )
		if( OutputSetParamError(_T("TTS_PARAM_CODEPAGE"), m_ServerInfo.m_nCodePage, dwErr) < 0 )
			return FALSE;

	/// ����������
	dwErr = TTSSetSynthParam(m_hTTSInstance, TTS_PARAM_VID, m_ServerInfo.m_nVID);
	if( dwErr != TTSERR_OK )
		if( OutputSetParamError(_T("TTS_PARAM_VID"), m_ServerInfo.m_nVID, dwErr) < 0 )
			return FALSE;

	/// ������ʽ
	dwErr = TTSSetSynthParam(m_hTTSInstance, TTS_PARAM_AUDIODATAFMT, m_ServerInfo.m_nAudioFmt);
	if( dwErr != TTSERR_OK )
		if( OutputSetParamError(_T("TTS_PARAM_AUDIODATAFMT"), m_ServerInfo.m_nAudioFmt, dwErr) < 0 )
			return FALSE;

	/// �ϳ�����
	dwErr = TTSSetSynthParam(m_hTTSInstance, TTS_PARAM_SPEED, m_ServerInfo.m_nSpeed);
	if( dwErr != TTSERR_OK )
		if( OutputSetParamError(_T("TTS_PARAM_SPEED"), m_ServerInfo.m_nSpeed, dwErr) < 0 )
			return FALSE;

	/// �������
	dwErr = TTSSetSynthParam(m_hTTSInstance, TTS_PARAM_VOLUME, m_ServerInfo.m_nVolume);
	if( dwErr != TTSERR_OK )
		if( OutputSetParamError(_T("TTS_PARAM_VOLUME"), m_ServerInfo.m_nVolume, dwErr) < 0 )
			return FALSE;

	/// �������
	dwErr = TTSSetSynthParam(m_hTTSInstance, TTS_PARAM_PITCH, m_ServerInfo.m_nPitch);
	if( dwErr != TTSERR_OK )
		if( OutputSetParamError(_T("TTS_PARAM_PITCH"), m_ServerInfo.m_nPitch, dwErr) < 0 )
			return FALSE;

	/// �����Ƶ����ͷ��ʽ
	dwErr = TTSSetSynthParam(m_hTTSInstance, TTS_PARAM_AUDIOHEADFMT, m_ServerInfo.m_nAudioHeadFmt);
	if( dwErr != TTSERR_OK )
		if( OutputSetParamError(_T("TTS_PARAM_AUDIOHEADFMT"), m_ServerInfo.m_nAudioHeadFmt, dwErr) < 0 )
			return FALSE;
			
	/// �ı�����
	dwErr = TTSSetSynthParam(m_hTTSInstance, TTS_PARAM_TEXTTYPE, m_ServerInfo.m_nTextType);
	if( dwErr != TTSERR_OK )
		if( OutputSetParamError(_T("TTS_PARAM_TEXTTYPE"), m_ServerInfo.m_nTextType, dwErr) < 0 )
			return FALSE;
#endif

	return TRUE;
}

// �Ͽ�TTS Server����
BOOL CTTSInterface::CTTSConnection::DisconnectServer()
{
	if( m_hTTSInstance != NULL )
	{
#ifdef USE_SINOVOICE_TTS
// SinoVoice jTTS close Connetion
		/// Do nothing here: ���غϳ�û�з���������
#else
// iFly TTS close Connetion
		TTSDisconnect(m_hTTSInstance);
#endif
		m_nCurrentSessionID = 0;
		m_hTTSInstance = NULL;
		WriteTrace(TraceDebug, "TTS Connection: %d is closed.", m_nIndex);
	}

	return TRUE;
}

// �Ƿ��Ѿ���TTS Server����
BOOL CTTSInterface::CTTSConnection::IsConnected()
{
	return( m_hTTSInstance != NULL );
}

// ִ�������ϳ�
int CTTSInterface::CTTSConnection::ExecuteSynthesize(LPSTR f_pTextData, int f_nDataLen)
{
	int lv_retval = -1;
	int nRet, nRet1 = 0, lv_bufferid;
	UINT lv_errcode = 0;

	// Reset Cancel Flag
	SetCancelFlag(FALSE);
	lv_bufferid = m_nWriteBufferPtr;

	// Sun added 2008-11-16
	m_dwIdleTick = GetTickCount();

#ifdef USE_SINOVOICE_TTS
// SinoVoice jTTS execute Synthesize
	
	ERRCODE lv_errCode;

	DWORD dwSessionID = 0;
	int lv_iBitsPerSample;
	int lv_nSamplesPerSec;

	// ����л�Ự�����ȹر�֮
	if( m_nCurrentSessionID > 0 )
	{
		jTTS_SessionStop(m_nCurrentSessionID);
		m_nCurrentSessionID = 0;
	}

	// �����첽�ص��ϳ�
	lv_errCode = jTTS_SessionStart(f_pTextData, &dwSessionID, 
								m_ServerInfo.m_nAudioFmt, NULL,
								PLAYCONTENT_TEXT | PLAYMODE_ASYNC,
								&lv_iBitsPerSample, &lv_nSamplesPerSec);
	nRet = OutputSetParamError(_T("jTTS_SessionStart"), m_nIndex, (UINT)lv_errCode);
	if( nRet < 0 || nRet == 2 )
	{
		// ʧ��
		/// Maybe need Stop & Start TTS again:
		/// Call "CTTSInterface::StopTTS()", "CTTSInterface::StartupTTS()"
	}
	else if( nRet == 0 )
	{
		// �ȴ���Դ
		/// ʵ��TTS��Դ����IVR��ɵ���Դ������Ҫȷ��IVR�����Լ������TTS��Դ����
	}
	else if( nRet == 1 )
	{
		// �ϳ������ɹ������ûص�
		m_nCurrentSessionID = dwSessionID;
		lv_errCode = jTTS_SessionExec(dwSessionID, AsyncDataCallbackProc,(DWORD)this);
		if( lv_errCode != ERR_NONE )
		{
			WriteTrace(TraceWarn, "jTTS Connection: %d error: %d.", m_nIndex, lv_errCode);
			m_nCurrentSessionID = 0;
			jTTS_SessionStop(dwSessionID);
		}
		else
		{
			// ���ûص��ɹ����ȴ����ݣ�
			/// ע����ɺ�һ��Ҫ���á�jTTS_SessionStop����Ŀǰ��TTSStopSynthesize/SetCancelFlag(TRUE)�����е���
			lv_retval = lv_bufferid;
		}
	}

#else
// iFly TTS execute Synthesize

	TTSData tTtsData;
	TTSCallBacks tTtsCB;

	memset(&tTtsData, 0, sizeof(tTtsData));
	tTtsData.szInBuf = f_pTextData;
	tTtsData.dwInBufSize = f_nDataLen;
	// �ص���������
	tTtsCB.nNumCallbacks = 2;
	// ���̴���ص�����
	tTtsCB.pfnTTSProcessCB = SynthProcessProc;
	// �¼��ص�
	tTtsCB.pfnTTSEventCB = SynthEventProc;
	// �����첽�ϳɺ�������thisָ����Ϊ�û����ݴ��ݽ�ȥ
	TTSClean(m_hTTSInstance);
	nRet = TTSSynthTextEx( m_hTTSInstance, &tTtsData, &tTtsCB, TRUE, this );
	if( nRet != TTSERR_OK )
	{
		lv_errcode = TTSGETERRCODE(nRet);
		if( lv_errcode == TTSERR_SYNC )
			WriteTrace(TraceWarn, "TTS Connection: %d error: last synthesizition not complete.", m_nIndex);
		else if( lv_errcode == TTSERR_INVALIDHANDLE || lv_errcode == TTSERR_CREATEHANDLE )
		{
			WriteTrace(TraceError, "TTS Connection: %d error: connection handle is invalid.", m_nIndex);
			// Try to resume connection and attempt again
			DisconnectServer();
			if( ConnectServer(m_ServerInfo) )
			{
				/// Synthesize again
				nRet1 = TTSSynthTextEx( m_hTTSInstance, &tTtsData, &tTtsCB, TRUE, this );
				if( nRet1 != TTSERR_OK )
				{
					lv_errcode = TTSGETERRCODE(nRet1);
					WriteTrace(TraceError, "TTS Connection: %d error2: can't execute synthesizition, error code = 0x%00x.", m_nIndex, lv_errcode);
				}
				else
					lv_retval = lv_bufferid;
			}
		}
		else
			WriteTrace(TraceError, "TTS Connection: %d error: can't execute synthesizition, error code = 0x%00x.", m_nIndex, lv_errcode);
	}
	else
		lv_retval = lv_bufferid;

#endif

	return lv_retval;
}

// ��¼��������
BOOL CTTSInterface::CTTSConnection::ReceiveTTSData(const BYTE *f_pVoiceData, UINT f_nDataLen)
{
	CTTSBuffer *lv_pBuf;

	// Find Buffer
	lv_pBuf = m_BufferList[m_nWriteBufferPtr];
	if( !lv_pBuf )
		return FALSE;

	// Wait buffer free
	int lv_loop = 0;
	while( lv_pBuf->GetTag() > 0 )
	{
		if( lv_loop++ > DEF_TTS_CST_BUFFER_LOCKTIMEOUT * 50 )
		{
			WriteTrace(TraceError, "TTS Connection: %d buffer: %d receive data write timeout!!!", m_nIndex, m_nWriteBufferPtr);
			return FALSE;
		}

		Sleep(20);
	}

	if( m_nOwner <= 0 )
		return FALSE;

	// Use buffer
	if( !lv_pBuf->SetBufferData(f_pVoiceData, f_nDataLen, m_nOwner) )
	{
		WriteTrace(TraceError, "TTS Connection: %d buffer: %d receive data error!!!", m_nIndex, m_nWriteBufferPtr);
		return FALSE;
	}

	WriteTrace(TraceDetail, "TTS Connection: %d buffer: %d set data length: %d for owner: %d", m_nIndex, m_nWriteBufferPtr, f_nDataLen, m_nOwner);

	// Move to Next Buffer
	m_nWriteBufferPtr++;
	if( m_nWriteBufferPtr >= m_nBufferCount )
		m_nWriteBufferPtr = 0;

	return TRUE;
}

// ��ȡ��������
BOOL CTTSInterface::CTTSConnection::FetchVoiceData(int f_nChannelID, int f_nBufferID, BYTE *f_pVoiceData, int *f_nDataLen)
{
	CTTSBuffer *lv_pBuf;
	int lv_nBufferIndex = f_nBufferID % m_nBufferCount;

	// Find Buffer
	lv_pBuf = m_BufferList[lv_nBufferIndex];
	if( !lv_pBuf )
	{
		WriteTrace(TraceWarn, "TTS Connection: %d FetchVoiceData(%d, %d) can't get buffer!", m_nIndex, f_nChannelID, lv_nBufferIndex);
		return FALSE;
	}

	// Judge buffer id
	int lv_curid = lv_pBuf->GetTag();
	if( lv_curid != f_nChannelID )
	{
		/// Not my buffer
		if( lv_curid == 0 )
		{
			//// Idle buffer
			*f_nDataLen = -1;
			if( IsTaken() != f_nChannelID )
				return FALSE;				// Finished
			else
				return TRUE;				// Continue
		}
		else
		{
			//// Occupied buffer
			WriteTrace(TraceWarn, "TTS Connection: %d FetchVoiceData(%d, %d) meets confused buffer occupied by %d!", m_nIndex, f_nChannelID, lv_nBufferIndex, lv_curid);
			ReleaseLockedBuffers();
			return FALSE;
		}
	}

	// Get Data & Clean Buffer
	*f_nDataLen = lv_pBuf->GetBufferData(f_pVoiceData);
	lv_pBuf->ResetTag();
	
	WriteTrace(TraceDetail, "TTS Connection: %d FetchVoiceData(%d, %d) gets data length: %d!", m_nIndex, f_nChannelID, lv_nBufferIndex, *f_nDataLen);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CTTSInterface Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTTSInterface::CTTSInterface()
 :  m_blnTTSEnabled(FALSE),
	m_blnTTSStarted(FALSE),
	m_nConnCount(0),
	m_nMaxBuffers(2),
	m_nBufferSize(64)
{
	for( int lv_loop = 0; lv_loop < DEF_TTS_CST_MAX_CONNECTION; lv_loop++ )
		m_ConnList[lv_loop] = NULL;
}

CTTSInterface::~CTTSInterface()
{	
	UninitializeTTS();
}

// ��ʼ��TTS���棬����TTS����
BOOL CTTSInterface::InitializeTTS(UINT f_nConnections, UINT f_nBuffers, UINT f_nBufferSize)
{
	UINT lv_loop;

	if( !m_blnTTSEnabled )
	{
#ifdef USE_SINOVOICE_TTS
// SinoVoice jTTS goes to StartupTTS()
		// Do nothing here
#else
// iFly TTS
		WriteTrace(TraceInfo, "Info - Begin to Initialize iFly TTS engine interface.");

		TTSINT32 dwErr;

		// ��ʼ��TTSϵͳ
		dwErr = TTSInitialize();
		if( dwErr != TTSERR_OK )
		{
			if( TTSGETERRCODE(dwErr) == TTSERR_NOLICENCE )
				WriteTrace(TraceError, "Faied to initialize iFly TTS engine! Has no proper license!");
			else
				WriteTrace(TraceError, "Faied to initialize iFly TTS engine! Error code = 0x%00x", dwErr);

			return FALSE;
		}
		WriteTrace(TraceInfo, "Initializing iFly TTS engine...OK!");
#endif

		m_blnTTSEnabled = TRUE;
	}

	// ����TTS����
	UINT lv_nBuffers;
	if( f_nBuffers > DEF_TTS_CST_MAX_BUFFERS )
		lv_nBuffers = DEF_TTS_CST_MAX_BUFFERS;
	else
		lv_nBuffers = f_nBuffers;
	/// ����ԭ��TTS����
	if( m_nMaxBuffers != lv_nBuffers || m_nBufferSize != f_nBufferSize )
		RemoveAllTTSConnections();
	m_bLock.Enter();
	
	try
	{
		if( f_nConnections < m_nConnCount )
		{
			for( lv_loop = f_nConnections; lv_loop < m_nConnCount; lv_loop++ )
			{
				if( m_ConnList[lv_loop] )
				{
					delete m_ConnList[lv_loop];
					m_ConnList[lv_loop] = NULL;
				}
			}
			m_nConnCount = f_nConnections;
		}
		else if( f_nConnections > m_nConnCount )
		{
			/// ����������
			for( lv_loop = m_nConnCount; lv_loop < f_nConnections; lv_loop++ )
			{
				m_ConnList[lv_loop] = new CTTSConnection(lv_loop, lv_nBuffers, f_nBufferSize);
			}
			m_nConnCount = f_nConnections;
		}
		WriteTrace(TraceDebug, "Notes: %d TTS connections are created.", m_nConnCount);
	}
	catch(...)
	{
		WriteTrace(TraceError, "CTTSInterface::InitializeTTS() runs error!!!");
	}
	m_nMaxBuffers = lv_nBuffers;
	m_nBufferSize = f_nBufferSize;

	m_bLock.Leave();

	return TRUE;
}

// ����TTS���ӣ�����ʼ��TTS����
BOOL CTTSInterface::UninitializeTTS()
{
	if( m_blnTTSEnabled )
	{
		// ֹͣ����TTS����
		StopTTS();

		// ɾ������TTS����
		RemoveAllTTSConnections();

#ifdef USE_SINOVOICE_TTS
// SinoVoice jTTS
		ERRCODE lv_errCode;

		// ���ʼ��TTSϵͳ���ͷ�jTTS����ռ���ڴ�
		lv_errCode = jTTS_End();
		if( lv_errCode != ERR_NONE )
		{	
			if( lv_errCode == ERR_NOTINIT )
				WriteTrace(TraceError, "No TTS engine needs to be uninitialized!");
			else if( lv_errCode == ERR_PLAYING )
				WriteTrace(TraceError, "Faied to initialize SinoVoice jTTS engine! TTS engine is in working state!");
			else
				WriteTrace(TraceError, "Faied to uninitialize SinoVoice jTTS engine! Error code = %d", lv_errCode);

			return FALSE;
		}
#else
// iFly TTS
		TTSINT32 dwErr;

		// ���ʼ��TTSϵͳ
		dwErr = TTSUninitialize();
		if( dwErr != TTSERR_OK )
		{
			if( TTSGETERRCODE(dwErr) == TTSERR_NOTINIT )
				WriteTrace(TraceError, "No TTS engine needs to be uninitialized!");
			else
				WriteTrace(TraceError, "Faied to uninitialize iFly TTS engine! Error code = 0x%00x", dwErr);

			return FALSE;
		}
#endif

		WriteTrace(TraceInfo, "Uninitializing TTS engine...OK!");
		m_blnTTSEnabled = FALSE;
	}

	return TRUE;
}

// TTS �����Ƿ����
BOOL CTTSInterface::IsTTSEnabled()
{
	return m_blnTTSEnabled;
}

// ��������TTS����
void CTTSInterface::RemoveAllTTSConnections()
{
	m_bLock.Enter();

	try
	{
		for( UINT lv_loop = 0; lv_loop < m_nConnCount; lv_loop++ )
		{
			if( m_ConnList[lv_loop] )
			{
				delete m_ConnList[lv_loop];
				m_ConnList[lv_loop] = NULL;
			}
		}

		m_nConnCount = 0;
	}
	catch(...)
	{
		WriteTrace(TraceError, "CTTSInterface::RemoveAllTTSConnections() runs error!!!");
	}

	m_bLock.Leave();
}

// ������Ų���TTS����
CTTSInterface::CTTSConnection *CTTSInterface::FindTTSConnection(UINT f_nIndex)
{
	if( f_nIndex < m_nConnCount )
		return m_ConnList[f_nIndex];

	return NULL;
}

// �ҵ�һ�����õ�TTS����
CTTSInterface::CTTSConnection *CTTSInterface::GetAvailableTTSConnection(int f_nOwner)
{
	CTTSConnection *lv_pRetVal = NULL;

	m_bLock.Enter();

	try
	{
		static UINT pos = 0;
		CTTSConnection *lv_pCn;

		for( UINT lv_loop = 0; lv_loop < m_nConnCount; lv_loop++ )
		{
			if( pos >= m_nConnCount )
				pos = 0;

			lv_pCn = m_ConnList[pos++];
			if( lv_pCn )
			{
				if( lv_pCn->IsTaken() == 0 )
				{
					lv_pCn->SetUsed(f_nOwner);
					lv_pRetVal = lv_pCn;
					break;
				}
			}
		}
	}
	catch(...)
	{
		WriteTrace(TraceError, "CTTSInterface::GetAvailableTTSConnection(%d) runs error!!!", f_nOwner);
	}

	m_bLock.Leave();

	return lv_pRetVal;
}

// ������TTS���ӽ������м��
void CTTSInterface::CheckTTSConnections()
{
	m_bLock.Enter();

	try
	{
		CTTSConnection *lv_pCn;
		for( UINT lv_loop = 0; lv_loop < m_nConnCount; lv_loop++ )
		{
			lv_pCn = m_ConnList[lv_loop];
			if( lv_pCn )
			{
				/// Check Connection Status
				// Sun added 2008-11-16
				lv_pCn->CheckMaxIdleDuration();

				/// Check Buffer Dead-lock
				lv_pCn->CheckDeadLock();
			}
		}
	}
	catch(...)
	{
		WriteTrace(TraceError, "CTTSInterface::CheckTTSConnections() runs error!!!");
	}

	m_bLock.Leave();
}

// TTS�ӿڰ������ٸ�����
UINT CTTSInterface::GetMaxTTSConnections()
{
	return m_nConnCount;
}

// ÿ���������ö��ٸ�����ת������
UINT CTTSInterface::GetMaxTTSBuffers()
{
	return m_nMaxBuffers;
}

// ÿ������ת������Ĵ�С(KB)
UINT CTTSInterface::GetTTSBufferSize()
{
	return m_nBufferSize;
}

// ����TTS�ӿ�
BOOL CTTSInterface::StartupTTS(CTTSServerInfo f_Server)
{
	// Sun added 2007-10-28
	/// TTS������һ������
	m_TTSServerInfo = f_Server;
#ifdef USE_SINOVOICE_TTS

	/// �ϳ����٣�����0~9��Ϊ10����ȱʡֵΪ5������Խ������Խ�졣 
	m_TTSServerInfo.m_nSpeed = (f_Server.m_nSpeed + 10) / 2;   // 0-10
	if( m_TTSServerInfo.m_nSpeed >= 10 )
		m_TTSServerInfo.m_nSpeed = 9;
	if( m_TTSServerInfo.m_nSpeed < 0 )
		m_TTSServerInfo.m_nSpeed = 0;

	/// �������������0~9��Ϊ10����ȱʡֵΪ5������Խ������Խ�� 
	m_TTSServerInfo.m_nVolume = (f_Server.m_nVolume + 10) / 2;   // 0-10
	if( m_TTSServerInfo.m_nVolume >= 10 )
		m_TTSServerInfo.m_nVolume = 9;	
	if( m_TTSServerInfo.m_nVolume < 0 )
		m_TTSServerInfo.m_nVolume = 0;	

	/// ������ߣ�����0~9��Ϊ10����ȱʡֵΪ5������Խ���ƵԽ�ߡ�
	m_TTSServerInfo.m_nPitch = (f_Server.m_nPitch + 10) / 2;   // 0-10
	if( m_TTSServerInfo.m_nPitch >= 10 )
		m_TTSServerInfo.m_nPitch = 9;	
	if( m_TTSServerInfo.m_nPitch < 0 )
		m_TTSServerInfo.m_nPitch = 0;	

	/// ����������������������������0~100��ȱʡΪ50��
	m_TTSServerInfo.m_nBackAudioVolume = (f_Server.m_nPitch + 10) * 5;   // 0-100
	if( m_TTSServerInfo.m_nBackAudioVolume > 100 )
		m_TTSServerInfo.m_nBackAudioVolume = 100;	
	if( m_TTSServerInfo.m_nBackAudioVolume < 0 )
		m_TTSServerInfo.m_nBackAudioVolume = 0;	

#else

	/// �ϳ����٣�[-500..500]
	m_TTSServerInfo.m_nSpeed = f_Server.m_nSpeed * 50;
	if( m_TTSServerInfo.m_nSpeed > 500 )
		m_TTSServerInfo.m_nSpeed = 500;
	if( m_TTSServerInfo.m_nSpeed < -500 )
		m_TTSServerInfo.m_nSpeed = -500;

	/// ���������[-20..20]
	m_TTSServerInfo.m_nVolume = f_Server.m_nVolume * 2;
	if( m_TTSServerInfo.m_nVolume > 20 )
		m_TTSServerInfo.m_nVolume = 20;	
	if( m_TTSServerInfo.m_nVolume < -20 )
		m_TTSServerInfo.m_nVolume = -20;	

	/// ������ߣ�[-500..500]
	m_TTSServerInfo.m_nPitch = f_Server.m_nPitch * 50;
	if( m_TTSServerInfo.m_nPitch > 500 )
		m_TTSServerInfo.m_nPitch = 500;	
	if( m_TTSServerInfo.m_nPitch < -500 )
		m_TTSServerInfo.m_nPitch = -500;	

	/// ��������������[-20..20]
	m_TTSServerInfo.m_nBackAudioVolume = f_Server.m_nPitch * 2;
	if( m_TTSServerInfo.m_nBackAudioVolume > 20 )
		m_TTSServerInfo.m_nBackAudioVolume = 20;	
	if( m_TTSServerInfo.m_nBackAudioVolume < -20 )
		m_TTSServerInfo.m_nBackAudioVolume = -20;	

#endif

// ��SinoVoice jTTS֧�ֱ��غϳ�
#ifdef USE_SINOVOICE_TTS

	// Initialize jTTS engine
	WriteTrace(TraceInfo, "Info - Begin to Initialize SinoVoice jTTS engine interface.");

	ERRCODE lv_errCode;

	// ��һ������Ϊ��NULL��ʹ��ע���\\HKEY_LOCAL_MACHINE\SOFTWARE\SinoVoice\jTTS_Professional\LibPath4��
	/// Ŀǰ��ʹ�ñ��غϳ�
	// �ڶ�������ʹ��INI���������Ϊ�մ���ʹ��ע���\\HKEY_LOCAL_MACHINE\SOFTWARE\SinoVoice\jTTS_Professional\SerialNo���е����к�
	lv_errCode = jTTS_Init(NULL, (LPCSTR)m_TTSServerInfo.m_strSerialNO);
	if( lv_errCode != ERR_NONE )
	{	
		if( lv_errCode == ERR_ALREADYINIT )
		{
			WriteTrace(TraceError, "Faied to initialize SinoVoice jTTS engine! �Ѿ������˳�ʼ��!");

			// ����ڵ���jTTS_Initʱϵͳ����ERR_ALREADYINIT�������û���ε�����jTTS_Init��
			/// ����ʱ�û���ȻҪ������ͬ������jTTS_End��ϵͳ�Ż���ȫ�ͷ��ڴ�
			jTTS_End();
		}
		else
		{
			if( lv_errCode == ERR_SECURITY )
				WriteTrace(TraceError, "Faied to initialize SinoVoice jTTS engine! SN is wrong or corrupted!");
			else if( lv_errCode == ERR_NOTSUPPORT )
				WriteTrace(TraceError, "Faied to initialize SinoVoice jTTS engine! SN is not matched with jTTS engine!");
			else if( lv_errCode == ERR_CONNECTSOCK )
				WriteTrace(TraceError, "Faied to initialize SinoVoice jTTS engine! Socket Error!");
			else
				WriteTrace(TraceError, "Faied to initialize SinoVoice jTTS engine! Error code = %d", lv_errCode);

			return FALSE;
		}
	}

	WriteTrace(TraceInfo, "Initializing SinoVoice jTTS engine...OK!");

#else
	if( m_TTSServerInfo.m_strServerIP == _T("") )
		return FALSE;
#endif

	if( !m_blnTTSEnabled )
		return FALSE;

	if( !m_blnTTSStarted )
	{
		// Start all TTS connections
		int lv_nCounter = 0;

		m_bLock.Enter();

		try
		{
			CTTSConnection *lv_pCn;
			for( UINT lv_loop = 0; lv_loop < m_nConnCount; lv_loop++ )
			{
				lv_pCn = m_ConnList[lv_loop];
				if( lv_pCn )
					if( lv_pCn->ConnectServer(m_TTSServerInfo) )
						lv_nCounter++;
			}
		}
		catch(...)
		{
			WriteTrace(TraceError, "CTTSInterface::StartupTTS() runs error!!!");
		}

		m_bLock.Leave();

		m_blnTTSStarted = TRUE;
		WriteTrace(TraceInfo, "TTS Interface is started up, total active connections: %d.", lv_nCounter);
	}

	return TRUE;
}

// ֹͣTTS�ӿ�
BOOL CTTSInterface::StopTTS()
{
	if( m_blnTTSStarted )
	{
		// Stop all TTS connections
		m_bLock.Enter();

		try
		{
			CTTSConnection *lv_pCn;
			
			for( UINT lv_loop = 0; lv_loop < m_nConnCount; lv_loop++ )
			{
				lv_pCn = m_ConnList[lv_loop];
				if( lv_pCn )
					lv_pCn->DisconnectServer();
			}
		}
		catch(...)
		{
			WriteTrace(TraceError, "CTTSInterface::StopTTS() runs error!!!");
		}

		m_bLock.Leave();
		
		m_blnTTSStarted = FALSE;
		WriteTrace(TraceInfo, "TTS Interface is stoped.");
	}

	return TRUE;
}

// TTS�ӿ��Ƿ�����
BOOL CTTSInterface::IsTTSStarted()
{
	return m_blnTTSStarted;
}

//--------------------------------------------------------------------
// �ı�ת�����ϳ�
/// ������
////		*f_nStartBuffer - ������ʼ����������
////		f_nChannelID - ����TTS������ͨ����ʶ�� > 0 ��
////		f_pTextData - ��Ҫ�ϳɵ��ı�
////		f_nDataLen - �ı����ݳ���
////		f_nTimeOut - ��ʱ����λ�룬0 ��ʾ������
/// ����ֵ��
////		����ռ�����ӵ���ţ�0 based��
//--------------------------------------------------------------------
int CTTSInterface::TTSSynthesizeText(int *f_nStartBuffer, int f_nChannelID, LPSTR f_pTextData, int f_nDataLen, UINT f_nTimeOut)
{
	if( !IsTTSStarted() )
		return DEF_TTS_EC_NOTSTARTED;

	// Get a free connection
	CTTSConnection *lv_pCn;

	UINT lv_nCount = 0;
	while( true)
	{
		lv_pCn = GetAvailableTTSConnection(f_nChannelID);
		if( lv_pCn )
			break;
	
		if( f_nTimeOut > 0 )
		{
			lv_nCount++;
			if( lv_nCount > f_nTimeOut * 10 )
				break;
		}

		Sleep(100);
	}

	if( lv_pCn == NULL )
		return DEF_TTS_EC_NORESOURCE;

	// Start Synthesize
	try
	{
		if( (*f_nStartBuffer = lv_pCn->ExecuteSynthesize(f_pTextData, f_nDataLen)) >= 0 )
			return( (int)lv_pCn->GetIndex() ); 
	}
	catch(...)
	{
		WriteTrace(TraceError, "CTTSInterface::TTSSynthesizeText(%d) runs error!!!", f_nChannelID);
	}

	// Release TTS connection if error is catched
	lv_pCn->SetUsed(0);
		
	return DEF_TTS_EC_SYSERROR;
}

//--------------------------------------------------------------------
// ����ϳɽ��
/// ������
////		f_nChannelID - ����TTS������ͨ����ʶ�� > 0 ��
////		f_nConnectionIndex - ռ�õ�TTS�������
////		f_nBufferID - ��һ������������
////		f_pVoiceData - ��������
////		f_nDataLen - �������ݳ���
/// ����ֵ��
////		��ֵ��ʾʧ�ܣ���ֵ�ɹ�
//--------------------------------------------------------------------
int CTTSInterface::TTSIsPlayBufferOK(int f_nChannelID, int f_nConnectionIndex, int f_nBufferID, BYTE *f_pVoiceData, int *f_nDataLen)
{
	if( f_nConnectionIndex < 0 )
		return DEF_TTS_EC_INVALIDHANDLE;

	// Find Connection
	CTTSConnection *lv_pCn = FindTTSConnection((UINT)f_nConnectionIndex);
	if( lv_pCn == NULL )
		return DEF_TTS_EC_NOTEXIST;

	// Get Data Buffer
	if( lv_pCn->FetchVoiceData(f_nChannelID, f_nBufferID, f_pVoiceData, f_nDataLen) )
		return DEF_TTS_EC_MOREDATA;

	return DEF_TTS_EC_COMPLETE;
}

// ǿ����ֹ�ϳ�
int CTTSInterface::TTSStopSynthesize(int f_nChannelID, int f_nConnectionIndex)
{
	try
	{
		if( f_nConnectionIndex < 0 )
			return DEF_TTS_EC_INVALIDHANDLE;

		// Find Connection
		CTTSConnection *lv_pCn = FindTTSConnection((UINT)f_nConnectionIndex);
		if( lv_pCn == NULL )
			return DEF_TTS_EC_NOTEXIST;
		
		lv_pCn->SetCancelFlag(TRUE);
		lv_pCn->SetUsed(0);

		// Release all locked buffers
		lv_pCn->ReleaseLockedBuffers(f_nChannelID);
	}
	catch(...)
	{
		WriteTrace(TraceWarn, "TTS Warn - Fail to call TTSStopSynthesize(%d, %d)!!!", 
			f_nChannelID, f_nConnectionIndex);
	}

	return DEF_TTS_EC_SUCC;
}