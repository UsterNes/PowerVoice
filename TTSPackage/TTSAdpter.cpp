////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		TTSAdapter.h
// ��;������	TTS�ӿ���ͷ�ļ�
// ���ߣ�		SUN
// ����ʱ�䣺	2004/11/20
// ����޸�ʱ�䣺2004/12/30
// ��Ȩ��Ϣ��	Copyright (C) 2003 - 2004 PicaSoft (�������)
// ��ע��
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
#include "stdafx.h"
#include "TTSAdapter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
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
	m_nAudioFmt(TTS_ADF_VOX8K1C),
	m_nAudioHeadFmt(TTS_AHF_NONE)
{
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

	return *this;
}

//////////////////////////////////////////////////////////////////////
// CTTSAdapter::CTTSConnection::CTTSBuffer Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTTSAdapter::CTTSConnection::CTTSBuffer::CTTSBuffer(int f_nIndex, UINT f_nBufferSize)
 :	m_nIndex(f_nIndex),
	m_Tag(0),
	m_lngLockTick(0),
	m_length(0),
	m_maxlen(0),
	m_pBuffer(NULL)
{
	CreateDataBuffer(f_nBufferSize * 1024);
}

CTTSAdapter::CTTSConnection::CTTSBuffer::~CTTSBuffer()
{
	ClearBuffer();
}

int CTTSAdapter::CTTSConnection::CTTSBuffer::GetBufferIndex()
{
	return m_nIndex;
}

void CTTSAdapter::CTTSConnection::CTTSBuffer::CreateDataBuffer(long f_nlen)
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

void CTTSAdapter::CTTSConnection::CTTSBuffer::ClearBuffer()
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

int CTTSAdapter::CTTSConnection::CTTSBuffer::GetTag()
{
	return m_Tag;
}

void CTTSAdapter::CTTSConnection::CTTSBuffer::ResetTag()
{
	m_sync.Enter ();
	m_Tag = 0;
	m_sync.Leave ();
}

int CTTSAdapter::CTTSConnection::CTTSBuffer::GetBufferData(BYTE *f_Data)
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

BOOL CTTSAdapter::CTTSConnection::CTTSBuffer::SetBufferData(const BYTE *f_Data, int f_Len, int f_Tag)
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

	m_lngLockTick = GetTickCount();
	return TRUE;
}

long CTTSAdapter::CTTSConnection::CTTSBuffer::GetLockTick()
{
	if( m_Tag <= 0 )
		return 0;

	return m_lngLockTick;
}

//////////////////////////////////////////////////////////////////////
// CTTSAdapter::CTTSConnection Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTTSAdapter::CTTSConnection::CTTSConnection(UINT f_nIndex, UINT f_nBuffers, UINT f_nBufferSize)
 :	m_nIndex(f_nIndex),
	m_nOwner(0),
	m_nWriteBufferPtr(0),
	m_nBufferCount(f_nBuffers),
	m_nBufferSize(f_nBufferSize),
	m_hTTSInstance(NULL)
{
	// ����������
	UINT lv_loop;

	m_pTtsData = new TTSData;
	memset(m_pTtsData, 0x00, sizeof(TTSData));

	for( lv_loop = 0; lv_loop < f_nBuffers; lv_loop++ )
		m_BufferList[lv_loop] = new CTTSBuffer((int)lv_loop, f_nBufferSize);
}

CTTSAdapter::CTTSConnection::~CTTSConnection()
{
	// �Ͽ�����
	DisconnectServer();

	// ���ٻ�����
	RemoveAllBuffers();
}

// �������л�����
void CTTSAdapter::CTTSConnection::RemoveAllBuffers()
{
	try
	{
		delete m_pTtsData;

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
		WriteTrace(TraceError, "CTTSAdapter::CTTSConnection::RemoveAllBuffers() runs error!!!");
	}
}

// ����TTS���ӱ��
UINT CTTSAdapter::CTTSConnection::GetIndex()
{
	return m_nIndex;
}

// ���������߳�
void CTTSAdapter::CTTSConnection::WaitForStartupToComplete()
{
	m_shutdown.Create();
	CMyThread::Start();
}

// ֹͣ�����߳�
void CTTSAdapter::CTTSConnection::WaitForShutdownToComplete()
{
	// if we havent already started a shut down, do so...
	if( m_shutdown.GetHANDLE() != INVALID_HANDLE_VALUE )
	{
		m_shutdown.Set();
		Wait();
	}
}

// �߳�ִ��
int CTTSAdapter::CTTSConnection::Run()
{
	try
	{
		BOOL lv_break = FALSE;
		int nRet;
		int lv_nFlag = 0;

		while( !m_shutdown.Wait(0) )
		{
			// Synthesize
			lv_nFlag = GetSynthFlag();
			while( lv_nFlag == TTS_FLAG_STILL_HAVE_DATA )
			{
				if( WaitForSingleObject(m_shutdown.GetHANDLE(), 0) == WAIT_OBJECT_0 )
				{
					lv_break = TRUE;
					break;
				}

				nRet = TTSFetchNext(m_hTTSInstance, m_pTtsData);
				if( nRet )
				{
					WriteTrace(TraceError, "TTSFetchNext runs error code = 0x%x", nRet);
					break;
				}
				else
				{
					if( m_pTtsData->dwOutBufSize > 0 )
					{
						if( !ReceiveTTSData((const BYTE *)m_pTtsData->pOutBuf, m_pTtsData->dwOutBufSize) )
							break;
					}
					lv_nFlag = GetSynthFlag();
				}
			}

			// Release TTS connect
			SetUsed(0);

			if( lv_break )
				break;

			Sleep(50);
		}
	}
	catch(...)
	{
		WriteTrace(TraceError, _T("CTTSConnection::Run() - Unexpected exception"));
	}

	return 0;
}

// TTS�����Ƿ����
int CTTSAdapter::CTTSConnection::IsTaken()
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
void CTTSAdapter::CTTSConnection::SetUsed(int f_nOwner)
{
	m_bLock.Enter();
	if( m_nOwner != f_nOwner )
	{
		m_nOwner = f_nOwner;
		memset(m_pTtsData, 0x00, sizeof(TTSData));
		TTSClean(m_hTTSInstance);
	}
	m_bLock.Leave();
}

// ���غϳɱ��
int CTTSAdapter::CTTSConnection::GetSynthFlag()
{
	int lv_retval;

	m_bLock.Enter();
	lv_retval = m_pTtsData->dwOutFlags;
	m_bLock.Leave();

	return lv_retval;
}

// ��黺�����Ƿ�����
BOOL CTTSAdapter::CTTSConnection::CheckDeadLock()
{
	try
	{
		CTTSBuffer *lv_pBuff;
		long lv_tickStart, lv_tickNow;
		
		lv_tickNow = GetTickCount();

		for( UINT lv_loop = 0; lv_loop < m_nBufferCount; lv_loop++ )
		{
			lv_pBuff = m_BufferList[lv_loop];
			if( lv_pBuff )
			{
				lv_tickStart = lv_pBuff->GetLockTick();
				if( lv_tickStart > 0 )
				{
					if( lv_tickNow - lv_tickStart > DEF_TTS_CST_BUFFER_LOCKTIMEOUT * 1000 )
					{
						WriteTrace(TraceWarn, "TTS buffer of channel: %d is locked for too long time!!!", lv_pBuff->GetTag());
						lv_pBuff->ResetTag();
					}
				}
			}
		}
	}
	catch(...)
	{
		WriteTrace(TraceError, "CTTSAdapter::CTTSConnection::CheckDeadLock() runs error!!!");
	}

	return TRUE;
}

// ����TTS Server
BOOL CTTSAdapter::CTTSConnection::ConnectServer(CTTSServerInfo f_Server)
{
	if( m_hTTSInstance == NULL )
	{
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

		// Record server info.
		m_ServerInfo = f_Server;

		// Set parameters
		SetSynthParameters();

		WaitForStartupToComplete();
		WriteTrace(TraceDebug, "TTS Connection: %d is opened.", m_nIndex);
	}

	return TRUE;
}

// ����������ô�����Ϣ
int CTTSAdapter::CTTSConnection::OutputSetParamError(LPCSTR f_szParaName, UINT f_nParaValue, UINT f_nErrorCode)
{
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

	return 0;
}

// ���úϳɲ���
BOOL CTTSAdapter::CTTSConnection::SetSynthParameters()
{
	TTSINT32 dwErr;

	/// �����������С
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

	return TRUE;
}

// �Ͽ�TTS Server����
BOOL CTTSAdapter::CTTSConnection::DisconnectServer()
{
	if( m_hTTSInstance != NULL )
	{
		WaitForShutdownToComplete();
		TTSDisconnect(m_hTTSInstance);
		m_hTTSInstance = NULL;
		WriteTrace(TraceDebug, "TTS Connection: %d is closed.", m_nIndex);
	}

	return TRUE;
}

// �Ƿ��Ѿ���TTS Server����
BOOL CTTSAdapter::CTTSConnection::IsConnected()
{
	return( m_hTTSInstance != NULL );
}

// ִ�������ϳ�
int CTTSAdapter::CTTSConnection::ExecuteSynthesize(LPSTR f_pTextData, int f_nDataLen)
{
	int lv_retval = -1;
	int nRet, nRet1, lv_bufferid;
	UINT lv_errcode;

	lv_bufferid = m_nWriteBufferPtr;

	memset(m_pTtsData, 0, sizeof(TTSData));
	m_pTtsData->szInBuf = f_pTextData;
	m_pTtsData->dwInBufSize = f_nDataLen;
	nRet = TTSSynthText( m_hTTSInstance, m_pTtsData );
	if( nRet != TTSERR_OK )
	{
		lv_errcode = TTSGETERRCODE(nRet);
		if( lv_errcode == TTSERR_SYNC )
			WriteTrace(TraceWarn, "TTS Connection: %d error: last synthesizition not complete.", m_nIndex);
		else if( lv_errcode == TTSERR_INVALIDHANDLE )
		{
			WriteTrace(TraceError, "TTS Connection: %d error: connection handle is invalid.", m_nIndex);
			// Try to resume connection and attempt again
			DisconnectServer();
			if( ConnectServer(m_ServerInfo) )
			{
				/// Synthesize again
				nRet1 = TTSSynthText( m_hTTSInstance, m_pTtsData );
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

	if( lv_retval >= 0 && m_pTtsData->dwOutBufSize > 0 )
	{
		if( !ReceiveTTSData((const BYTE *)m_pTtsData->pOutBuf, m_pTtsData->dwOutBufSize) )
			lv_retval = -2;
	}

	return lv_retval;
}

// ��¼��������
BOOL CTTSAdapter::CTTSConnection::ReceiveTTSData(const BYTE *f_pVoiceData, UINT f_nDataLen)
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
			WriteTrace(TraceError, "TTS buffer receive data write timeout!!!");
			return FALSE;
		}

		Sleep(20);
	}

	// Use buffer
	if( !lv_pBuf->SetBufferData(f_pVoiceData, f_nDataLen, m_nOwner) )
	{
		WriteTrace(TraceError, "TTS buffer receive data error!!!");
		return FALSE;
	}

	// Move to Next Buffer
	m_nWriteBufferPtr++;
	if( m_nWriteBufferPtr >= m_nBufferCount )
		m_nWriteBufferPtr = 0;

	return TRUE;
}

// ��ȡ��������
BOOL CTTSAdapter::CTTSConnection::FetchVoiceData(int f_nChannelID, int f_nBufferID, BYTE *f_pVoiceData, int *f_nDataLen)
{
	CTTSBuffer *lv_pBuf;
	int lv_nBufferIndex = f_nBufferID % m_nBufferCount;

	// Find Buffer
	lv_pBuf = m_BufferList[lv_nBufferIndex];
	if( !lv_pBuf )
		return FALSE;

	// Judge buffer id
	int lv_curid = lv_pBuf->GetTag();
	if( lv_curid != f_nChannelID )
	{
		if( lv_curid == 0 )
		{
			*f_nDataLen = -1;
			if( IsTaken() != f_nChannelID )
				return FALSE;
			else
				return TRUE;
		}
		else
			return FALSE;
	}

	// Get Data & Clean Buffer
	*f_nDataLen = lv_pBuf->GetBufferData(f_pVoiceData);
	lv_pBuf->ResetTag();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CTTSAdapter Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTTSAdapter::CTTSAdapter()
 :  m_blnTTSEnabled(FALSE),
	m_blnTTSStarted(FALSE),
	m_nConnCount(0),
	m_nMaxBuffers(2),
	m_nBufferSize(64)
{
	for( int lv_loop = 0; lv_loop < DEF_TTS_CST_MAX_CONNECTION; lv_loop++ )
		m_ConnList[lv_loop] = NULL;
}

CTTSAdapter::~CTTSAdapter()
{	
	UninitializeTTS();
}

// ��ʼ��TTS���棬����TTS����
BOOL CTTSAdapter::InitializeTTS(UINT f_nConnections, UINT f_nBuffers, UINT f_nBufferSize)
{
	UINT lv_loop;

	if( !m_blnTTSEnabled )
	{
		TTSINT32 dwErr;

		// ��ʼ��TTSϵͳ
		dwErr = TTSInitialize();
		if( dwErr != TTSERR_OK )
		{
			if( TTSGETERRCODE(dwErr) == TTSERR_NOLICENCE )
				WriteTrace(TraceError, "Faied to initialize TTS engine! Has no proper license!");
			else
				WriteTrace(TraceError, "Faied to initialize TTS engine! Error code = 0x%00x", dwErr);

			return FALSE;
		}

		WriteTrace(TraceInfo, "Initializing TTS engine...OK!");
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
		WriteTrace(TraceError, "CTTSAdapter::InitializeTTS() runs error!!!");
	}
	m_bLock.Leave();

	return TRUE;
}

// ����TTS���ӣ�����ʼ��TTS����
BOOL CTTSAdapter::UninitializeTTS()
{
	if( m_blnTTSEnabled )
	{
		TTSINT32 dwErr;

		// ֹͣ����TTS����
		StopTTS();

		// ɾ������TTS����
		RemoveAllTTSConnections();

		// ���ʼ��TTSϵͳ
		dwErr = TTSUninitialize();
		if( dwErr != TTSERR_OK )
		{
			if( TTSGETERRCODE(dwErr) == TTSERR_NOTINIT )
				WriteTrace(TraceError, "No TTS engine needs to be uninitialized!");
			else
				WriteTrace(TraceError, "Faied to uninitialize TTS engine! Error code = 0x%00x", dwErr);

			return FALSE;
		}

		WriteTrace(TraceInfo, "Uninitializing TTS engine...OK!");
		m_blnTTSEnabled = FALSE;
	}

	return TRUE;
}

// TTS �����Ƿ����
BOOL CTTSAdapter::IsTTSEnabled()
{
	return m_blnTTSEnabled;
}

// ��������TTS����
void CTTSAdapter::RemoveAllTTSConnections()
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
		WriteTrace(TraceError, "CTTSAdapter::RemoveAllTTSConnections() runs error!!!");
	}

	m_bLock.Leave();
}

// ������Ų���TTS����
CTTSAdapter::CTTSConnection *CTTSAdapter::FindTTSConnection(UINT f_nIndex)
{
	if( f_nIndex < m_nConnCount )
		return m_ConnList[f_nIndex];

	return NULL;
}

//--------------------------------------------------------------------
// �ҵ�һ�����õ�TTS����
/// ������
////		f_nOwer - ����TTS������ͨ����ʶ�� > 0 ��
////		f_pTextData - ��Ҫ�ϳɵ��ı�
////		f_nDataLen - �ı����ݳ���
////		f_nTimeOut - ��ʱ����λ�룬0 ��ʾ������
/// ����ֵ��
////		����ռ������ָ��
//--------------------------------------------------------------------
CTTSAdapter::CTTSConnection *CTTSAdapter::GetAvailableTTSConnection(int f_nOwner, UINT f_nTimeOut)
{
	CTTSConnection *lv_pRetVal = NULL;

	m_bLock.Enter();

	try
	{
		static UINT pos = 0;
		UINT lv_nCount = 0, lv_loop;
		CTTSConnection *lv_pCn;

		while( true)
		{
			for( lv_loop = 0; lv_loop < m_nConnCount; lv_loop++ )
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

			if( lv_pRetVal )
				break;
		
			if( f_nTimeOut > 0 )
			{
				lv_nCount++;
				if( lv_nCount > f_nTimeOut * 10 )
				{
					WriteTrace(TraceWarn, "CTTSAdapter::GetAvailableTTSConnection(%d, %d) timeout, TTS resources are insufficient!", f_nOwner, f_nTimeOut);
					break;
				}
			}

			Sleep(100);
		}
	}
	catch(...)
	{
		WriteTrace(TraceError, "CTTSAdapter::GetAvailableTTSConnection(%d, %d) runs error!!!", f_nOwner, f_nTimeOut);
	}

	m_bLock.Leave();

	return lv_pRetVal;
}

// ������TTS���ӽ������м��
void CTTSAdapter::CheckTTSConnections()
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
				//...

				/// Check Buffer Dead-lock
				lv_pCn->CheckDeadLock();
			}
		}
	}
	catch(...)
	{
		WriteTrace(TraceError, "CTTSAdapter::CheckTTSConnections() runs error!!!");
	}

	m_bLock.Leave();
}

// TTS�ӿڰ������ٸ�����
UINT CTTSAdapter::GetMaxTTSConnections()
{
	return m_nConnCount;
}

// ÿ���������ö��ٸ�����ת������
UINT CTTSAdapter::GetMaxTTSBuffers()
{
	return m_nMaxBuffers;
}

// ÿ������ת������Ĵ�С(KB)
UINT CTTSAdapter::GetTTSBufferSize()
{
	return m_nBufferSize;
}

// ����TTS�ӿ�
BOOL CTTSAdapter::StartupTTS(CTTSServerInfo f_Server)
{
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
					if( lv_pCn->ConnectServer(f_Server) )
						lv_nCounter++;
			}
		}
		catch(...)
		{
			WriteTrace(TraceError, "CTTSAdapter::StopTTS() runs error!!!");
		}

		m_bLock.Leave();

		m_blnTTSStarted = TRUE;
		WriteTrace(TraceInfo, "TTS Interface is started up, total active connections: %d.", lv_nCounter);
	}

	return TRUE;
}

// ֹͣTTS�ӿ�
BOOL CTTSAdapter::StopTTS()
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
			WriteTrace(TraceError, "CTTSAdapter::StopTTS() runs error!!!");
		}

		m_bLock.Leave();
		
		m_blnTTSStarted = FALSE;
		WriteTrace(TraceInfo, "TTS Interface is stoped.");
	}

	return TRUE;
}

// TTS�ӿ��Ƿ�����
BOOL CTTSAdapter::IsTTSStarted()
{
	return m_blnTTSStarted;
}
