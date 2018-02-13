#include "stdafx.h"
#include ".\wavedevice.h"

// Sun added 2011-04-16
#include "_Global.h"
#include "PowerVoice.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifdef CISCO_CCM
//********************************************************************
//* Construction: Wave in/Out Parent class
//********************************************************************

DWORD CWaveDevice::s_dwInstance = 0;

#ifdef CUCM_MEDIA

// Stream 回调函数, Sun added 2013-04-12
// Application can set endpoint callback to receive an indication about operation completions, data
/// transfers, errors, etc. Endpoint callback function type is declared as follows:
/* Parameters：
	hEp - Endpoint handle
	hStream - Rtp stream handle
	dwError - If not 0 (zero), indicates an error
	pData - Endpoint handle
	dwDataSize - Number of bytes received / transfered.
	pUserData - Application data associated with an operation.
	bIsSilence - If set to true, indicates that silence has been detected.
	streamDir - Stream direction. Can be one of the following:
			ToApp = 0x01,         //In,
			ToNwk = 0x02,         //Out
			Both  = 0x03          //In/Out
*/
void WINAPI MediaStreamCallback(HANDLE hEp, HANDLE hStream, DWORD dwError, PUCHAR pData, DWORD dwDataSize, LPVOID pUserData, bool bIsSilence, StreamDirection streamDir)
{
	LPWAVEHDR lv_pWH = (LPWAVEHDR)pUserData;
	CIVRChannel *lv_pIVRCH = NULL;
	long lv_nCH = -1;

	if( lv_pWH )
	{
		lv_nCH = (long)lv_pWH->dwUser;
		if( lv_nCH >= 0 && lv_nCH < theApp.m_nIVRChannels )
			lv_pIVRCH = PIVRCH[lv_nCH];
		else
			lv_nCH = -1;
	}

	if( dwError != 0 )
	{
		// Error
		WriteTrace(TraceError, "MediaStreamCallback Error: 0x%x on IVR Channel: %d, Endpoint handle = 0x%x", dwError, lv_nCH, hEp);
	}
	else
	{
		// Detail
		WriteTrace(TraceDetail, "MediaStreamCallback Detail on IVR Channel: %d, Endpoint handle = 0x%x, stream = 0x%x, Data Size = %d, StreamDir = %d, IsSilence = %d", 
			lv_nCH, hEp, hStream, dwDataSize, streamDir, bIsSilence);
	}

	if( streamDir == ToNwk )
	{
		// Play Done
		if( lv_pIVRCH )
		{
			lv_pWH->dwBufferLength = dwDataSize;
			lv_pIVRCH->m_oARS.WaveoutDone((DWORD)lv_nCH, (PVOID)lv_pWH);
		}
	}
	else
	{
		// Record Done
		if( lv_pIVRCH )
		{
			lv_pWH->dwBytesRecorded = dwDataSize;
			lv_pIVRCH->m_oARS.WaveinData((DWORD)lv_nCH, (PVOID)lv_pWH);
		}
	}
}

#endif

CWaveDevice::CWaveDevice(void)
{
	ZeroMemory((void*)&m_pcmWaveFormat, sizeof(m_pcmWaveFormat));
	m_pcmWaveFormat.wFormatTag = 1;
	m_pcmWaveFormat.nChannels = 1;
	m_pcmWaveFormat.wBitsPerSample = 16;
	m_pcmWaveFormat.nSamplesPerSec = WIOSA_POOR;

	m_bThreadStart = FALSE;
	m_bDevOpen = FALSE;

	m_bAllocBuffer = FALSE;
	m_iBufferNum = 0;

	m_dwAudioDevId = 0;
	m_hAudioDev = NULL;
	s_dwInstance ++;

	// Sun added 2011-04-16
	m_lngIVRChannelID = -1;

	// Sun added 2013-04-12
#ifdef CUCM_MEDIA
	m_hMediaStream = NULL;
#endif

	// Sun added 2011-06-24
	m_blnOpenWait = FALSE;
	m_dwOpenTick = GetTickCount();

	// Sun added 2011-06-24
	m_blnCloseWait = FALSE;
	m_dwCloseTick = GetTickCount();
}

CWaveDevice::~CWaveDevice(void)
{
	// Sun added 2011-06-24
	m_blnOpenWait = FALSE;
}

//---------------------------------------------
// Sun added 2011-06-24
void CWaveDevice::SetOpenDevWait()
{
	m_dwOpenTick = GetTickCount();
	m_blnOpenWait = TRUE;
}

void CWaveDevice::ResetOpenDevWait()
{
	m_blnOpenWait = FALSE;
}

// 尝试打开设备后的等待毫秒数
DWORD CWaveDevice::TestOpenDevWait()
{
	DWORD lv_nDur = 0;

	if( m_blnOpenWait )
	{
		lv_nDur = GetTickCount() - m_dwOpenTick;
	}

	return lv_nDur;
}
void CWaveDevice::SetCloseDevWait()
{
	m_dwCloseTick = GetTickCount();
	m_blnCloseWait = TRUE;
}

void CWaveDevice::ResetCloseDevWait()
{
	m_blnCloseWait = FALSE;
}

// 尝试关闭设备后的等待毫秒数
DWORD CWaveDevice::TestCloseDevWait()
{
	DWORD lv_nDur = 0;

	if( m_blnCloseWait )
	{
		lv_nDur = GetTickCount() - m_dwCloseTick;
	}

	return lv_nDur;
}
//---------------------------------------------

//---------------------------------------------
// Sun added 2011-04-16
long CWaveDevice::GetIVRChannelID()
{
	return m_lngIVRChannelID;
}

void CWaveDevice::SetIVRChannelID(long nIVRChannelID)
{
	m_lngIVRChannelID = nIVRChannelID;
}
//---------------------------------------------

BOOL CWaveDevice::StartThread()
{
	if( m_bThreadStart )
	{
		WriteTrace(TraceWarn, "IVR Channel: %d - CWaveDevice::StartThread: Wave I/O thread has run.", m_lngIVRChannelID);
		return FALSE;
	}

#ifdef CUCM_MEDIA

	m_bThreadStart = TRUE;

#else

	//WriteTrace(TraceDetail, "IVR Channel: %d - Tony Debug 2010-07-18: Play 1-2-1", m_lngIVRChannelID);

	m_hAudioDev = CreateThread(0, 0, AudioIOThreadProc, this, 0, &m_dwAudioDevId);
	if( !m_hAudioDev )
	{
        WriteTrace(TraceError, "IVR Channel: %d - CWaveDevice::StartThread: Start wave I/O thread fail Error Code = %d.",
			m_lngIVRChannelID, GetLastError());
		return FALSE;
	}

	//--------------------------------------------------
	// Sun updated 2011-06-24
	/// From
	//m_bThreadStart = TRUE;
	/// To
	/// 确认线程启动
	int t=50;
	while(t)
	{
		if( m_bThreadStart )
		{
			break;
		}
		else
			Sleep(10);
		t--;
	}
	if( !m_bThreadStart )
	{
        WriteTrace(TraceError, "IVR Channel: %d - CWaveDevice::StartThread: Start wave I/O thread timeout.",
			m_lngIVRChannelID);
		return FALSE;
	}
	//--------------------------------------------------

#endif

	WriteTrace(TraceDetail, "IVR Channel: %d - CWaveDevice::StartThread: Wave I/O thread started.", 
		m_lngIVRChannelID);

	return TRUE;
}

BOOL CWaveDevice::StopThread()
{
	if( !m_bThreadStart )
	{
		WriteTrace(TraceWarn, "IVR Channel: %d - CWaveDevice::StopThread: Wave I/O thread hasn't run.", m_lngIVRChannelID);
		return FALSE;
	}

#ifdef CUCM_MEDIA

	// Sun added 2013-04-12
	// Do nothing here

#else

	if( m_hAudioDev )
	{
		int t=50;
		DWORD ExitCode;
		BOOL bEnd=FALSE;
		PostThreadMessage(m_dwAudioDevId, WM_QUIT, 0, 0);
		while(t)
		{
			GetExitCodeThread(m_hAudioDev, &ExitCode);

			// Sun added '|| m_bThreadStart == FALSE '  2011-06-24
			if( ExitCode != STILL_ACTIVE || m_bThreadStart == FALSE )
			{
				bEnd = TRUE;
				break;
			}
			else
				Sleep(20);			// Sun updated 2011-06-24, from 10 ms -> 20 ms
			t--;
		}
		if( !bEnd )
		{
			TerminateThread(m_hAudioDev, 0);
			WriteTrace(TraceWarn, "IVR Channel: %d - CWaveDevice::StopThread: TerminateThread wave I/O thread.", m_lngIVRChannelID);

			// Sun added 2011-06-24
			m_csLock.Unlock();
		}
		// Don't forget to close the thread handle
		CloseHandle(m_hAudioDev);
		m_hAudioDev = 0;
	}

#endif

	m_bThreadStart = FALSE;
	WriteTrace(TraceDetail, "IVR Channel: %d - CWaveDevice::StopThread: Wave I/O thread stoped.", m_lngIVRChannelID);

	return TRUE;
}

void CWaveDevice::BuildFormat()
{
	m_pcmWaveFormat.nAvgBytesPerSec = m_pcmWaveFormat.nSamplesPerSec * m_pcmWaveFormat.nChannels * m_pcmWaveFormat.wBitsPerSample / 8;
	m_pcmWaveFormat.nBlockAlign = m_pcmWaveFormat.nChannels * m_pcmWaveFormat.wBitsPerSample / 8;
}	

MMRESULT CWaveDevice::GetLastMMError()
{
	return m_mmr;
}

// Notes: Cisco TAPI 2.1 don't support 'waveOutGetErrorText' routine
CString CWaveDevice::GetLastErrorString()
{
	char buffer[256];
	memset(buffer,0,256);
	waveOutGetErrorText(m_mmr,buffer,256);
	return buffer;
}

WORD CWaveDevice::GetFormat()
{
	return m_pcmWaveFormat.wFormatTag;
}

void CWaveDevice::SetFormat(WORD wFormat)
{
	m_pcmWaveFormat.wFormatTag = wFormat;
}

void CWaveDevice::SetChannel(WORD wChannel)
{
	m_pcmWaveFormat.nChannels = wChannel;
}

void CWaveDevice::SetSample(DWORD dwSample)
{
	m_pcmWaveFormat.nSamplesPerSec = dwSample;
}

void CWaveDevice::SetBit(WORD wBit)
{
	m_pcmWaveFormat.wBitsPerSample = (wBit == 8) ? 8:16;
}

DWORD CWaveDevice::GetInstance()
{
	return s_dwInstance;
}

WORD CWaveDevice::GetBit()
{
	return m_pcmWaveFormat.wBitsPerSample;
}

DWORD CWaveDevice::GetSample()
{
	return m_pcmWaveFormat.nSamplesPerSec;
}

WORD CWaveDevice::GetChannel()
{
	return m_pcmWaveFormat.nChannels;
}

void CWaveDevice::BufferAdd()
{
	m_csLock.Lock ();
	m_iBufferNum ++;
	m_csLock.Unlock ();
}

void CWaveDevice::BufferSub()
{
	m_csLock.Lock ();
	m_iBufferNum --;
	m_csLock.Unlock ();
}

int CWaveDevice::GetBufferNum()
{
	int iTemp;
	m_csLock.Lock ();
	iTemp = m_iBufferNum;
	m_csLock.Unlock ();
	return iTemp;

}

int CWaveDevice::ToRunThread()
{
	int result;

	// Sun added 2011-06-24
	m_bThreadStart = TRUE;

	result = Run();

	// Sun added 2011-06-24
	m_bThreadStart = FALSE;

	return result;
}

DWORD WINAPI CWaveDevice::AudioIOThreadProc(LPVOID lpParameter)
{
	int result = 0;

	TRACE("AudioIOThreadProc start.\n");
	CWaveDevice* pThis = (CWaveDevice*)lpParameter;

	if( pThis )
	{
		// Sun added 2011-06-24
		WriteTrace(TraceDetail, "IVR Channel: %d - CWaveDevice::AudioIOThreadProc start.", pThis->GetIVRChannelID());

		try
		{
			result = pThis->ToRunThread();
		}
		catch(...)
		{
		}

		// Sun added 2011-06-24
		WriteTrace(TraceDetail, "IVR Channel: %d - CWaveDevice::AudioIOThreadProc exit.", pThis->GetIVRChannelID());
	}

	TRACE("AudioIOThreadProc exit.\n");
	return result;
}

//********************************************************************
//* Construction: Wave Out Device class
//********************************************************************
CWaveOutDevice::CWaveOutDevice(void)
{
	m_dwWaveOutID = 0;
	m_hOut = 0;
	m_bPlaying = FALSE;

	// Sun added 2013-04-12
	for( int lv_loop = 0; lv_loop < NUM_BUF+1; lv_loop++ )
		m_pWavData[lv_loop] = NULL;
	m_nPrtHead = 0;

	// Sun added 2013-04-12
#ifdef CUCM_MEDIA
	m_hMediaEpOut = NULL;
#endif

}

CWaveOutDevice::~CWaveOutDevice(void)
{
	// Stop undergoing process
	if( IsPlaying() )
		Stop();

	// Close Device
	if( m_bDevOpen )
		CloseDev();

	// Stop Thread
	if( m_bThreadStart )
		StopThread();
}

void CWaveOutDevice::BufferAdd()
{
	CWaveDevice::BufferAdd();
}

void CWaveOutDevice::BufferSub()
{
	CWaveDevice::BufferSub();
}

BOOL CWaveOutDevice::OpenDev()
{
#ifdef CUCM_MEDIA

	// Sun added 2013-04-12
	BuildFormat();

	// Ep已经在打开TapiLine的同时打开
	if( m_hMediaEpOut )
	{
		m_hMediaStream = EpGetStreamHandle(m_hMediaEpOut, STREAM_TYPE_AUDIO, ToNwk);
		if( m_hMediaStream == NULL )
		{
			WriteTrace(TraceError, "IVR Channel: %d - CWaveOutDevice::OpenDev0: EpGetStreamHandle error 0x%x.", m_lngIVRChannelID, EpApiGetLastError());
			return FALSE;
		}

		WAVEFORMATEX lv_WaveF;
		if( !EpStreamCodecOutGet(m_hMediaStream, &lv_WaveF) )
		{
			WriteTrace(TraceError, "IVR Channel: %d - CWaveOutDevice::OpenDev: EpStreamCodecOutGet error 0x%x.", m_lngIVRChannelID, EpApiGetLastError());
		}

		// 比较文件格式和目前支持的格式
		if( lv_WaveF.wFormatTag !=  GetFormat() )
		{
			WriteTrace(TraceWarn, "IVR Channel: %d - CWaveOutDevice::OpenDev: stream requires format %d, which is defferent to the voice file's format %d.", lv_WaveF.wFormatTag, GetFormat());
		}

		// Sets stream outbound codec format
		if( !EpStreamCodecOutSet(m_hMediaStream, &m_pcmWaveFormat, 0) )
		//if( !EpStreamCodecOutSet(m_hMediaStream, &lv_WaveF, 0) )
		{
			WriteTrace(TraceError, "IVR Channel: %d - CWaveOutDevice::OpenDev0: EpStreamCodecOutSet error 0x%x.", m_lngIVRChannelID, EpApiGetLastError());
		}

		// Sun added 2013-04-12
		PRTPENDPOINTCALLBACK lv_pCallback = MediaStreamCallback;
		if( !EpStreamStart(m_hMediaStream, lv_pCallback) )
		{
			WriteTrace(TraceError, "IVR Channel: %d - CWaveOutDevice::OpenDev: Start wave I/O thread fail error 0x%x.", m_lngIVRChannelID, EpApiGetLastError());
			return FALSE;
		}
	}
	else
		return FALSE;

#else

	if( m_bDevOpen )
	{
		WriteTrace(TraceWarn, "IVR Channel: %d - CWaveOutDevice::OpenDev: Device has open.", m_lngIVRChannelID);
		return FALSE;
	}

	// Sun added 2011-06-24
	SetOpenDevWait();

	BuildFormat();

	if( m_dwWaveOutID == 0 )
	{
		m_mmr = waveOutOpen(0, WAVE_MAPPER, &m_pcmWaveFormat, 0, 0, WAVE_FORMAT_QUERY);
		if( m_mmr )
		{
			// Sun added 2011-06-24
			ResetOpenDevWait();

			WriteTrace(TraceError, "IVR Channel: %d - CWaveOutDevice::OpenDev0: waveOutOpen error 0x%x.", m_lngIVRChannelID, m_mmr);
			return FALSE;
		}
		m_mmr = waveOutOpen(&m_hOut, WAVE_MAPPER, &m_pcmWaveFormat, m_dwAudioDevId, s_dwInstance, CALLBACK_THREAD);
	}
	else
	{
		m_mmr = waveOutOpen(&m_hOut, m_dwWaveOutID, &m_pcmWaveFormat, m_dwAudioDevId, s_dwInstance, CALLBACK_THREAD | WAVE_MAPPED);
	}
	
	if( m_mmr )
	{
		// Sun added 2011-06-24
		ResetOpenDevWait();

		WriteTrace(TraceError, "IVR Channel: %d - CWaveOutDevice::OpenDev: waveOutOpen error 0x%x.", m_lngIVRChannelID, m_mmr);
		return FALSE;
	}

	WriteTrace(TraceDetail, "IVR Channel: %d - CWaveOutDevice(%d)::OpenDev: waveOutOpen Finished.", m_lngIVRChannelID, m_dwWaveOutID);

#endif

	m_bDevOpen = TRUE;
	m_iBufferNum = 0;

	return TRUE;
}

BOOL CWaveOutDevice::CloseDev()
{
#ifdef CUCM_MEDIA
	// Sun added 2013-04-12
	if( m_hMediaStream )
	{
		if( !EpStreamStop(m_hMediaStream) )
		{
			WriteTrace(TraceError, "IVR Channel: %d - CWaveOutDevice::StopThread: Stop wave I/O thread failed error 0x%x.", m_lngIVRChannelID, EpApiGetLastError());
		}
	}

#else

	// Sun added 2011-06-27
	if( pMainWindow->m_bSystemAutoShutDown )
	{
		m_bDevOpen = FALSE;
		return TRUE;
	}

	//----------------------------
	// Sun moved from 2011-07-10 [A]
	// Lock Me
	CSingleLock lv_SingleLock(&m_bLock, TRUE);
	//----------------------------

	// Sun added 2011-07-10
	if( m_blnCloseWait )
	{
		// Do Nothing
		WriteTrace(TraceInfo, "IVR Channel: %d - CWaveOutDevice(%d)::CloseDev: is closing.", m_lngIVRChannelID, m_dwWaveOutID);
		return TRUE;
	}

	if( !m_bDevOpen )
	{
		WriteTrace(TraceWarn, "IVR Channel: %d - CWaveOutDevice(%d)::CloseDev: Device hasn't opened.", m_lngIVRChannelID, m_dwWaveOutID);
		return FALSE;
	}

	if( !m_hOut )
	{
		WriteTrace(TraceWarn, "IVR Channel: %d - CWaveOutDevice(%d)::CloseDev: Device hasn't opened.", m_lngIVRChannelID, m_dwWaveOutID);
		return FALSE;
	}
	
	// Sun added 2011-06-24
	SetCloseDevWait();

	// Sun moved ahead 2011-07-10 [A]
	// Lock Me
	//CSingleLock lv_SingleLock(&m_bLock, TRUE);

	WriteTrace(TraceDetail, "IVR Channel: %d - CWaveOutDevice(%d)::CloseDev: waveOutClose.", m_lngIVRChannelID, m_dwWaveOutID);
	m_mmr = waveOutClose(m_hOut);
	if( m_mmr )
	{
		WriteTrace(TraceError, "IVR Channel: %d - CWaveOutDevice(%d)::CloseDev: waveOutClose error 0x%x.", m_lngIVRChannelID, m_dwWaveOutID, m_mmr);

		if( m_mmr == WAVERR_STILLPLAYING )
		{
			m_mmr = waveOutReset(m_hOut);
			m_mmr = waveOutClose(m_hOut);
		}
		// Sun added 2010-08-02
		else if( m_mmr == MMSYSERR_INVALHANDLE )
		{
			// 忽略此错误：已经关闭
			m_mmr = MMSYSERR_NOERROR;

			// Sun added 2011-07-10
			ResetCloseDevWait();
		}

		if( m_mmr )
		{
			// Sun added 2011-06-24
			ResetCloseDevWait();

			WriteTrace(TraceError, "IVR Channel: %d - CWaveOutDevice::CloseDev: waveOutClose error 0x%x.", m_lngIVRChannelID, m_mmr);
			return FALSE;
		}
	}
	WriteTrace(TraceDetail, "IVR Channel: %d - CWaveOutDevice(%d)::CloseDev: waveOutClose Finished.", m_lngIVRChannelID, m_dwWaveOutID);

#endif

	m_hOut=0;
	m_bDevOpen = FALSE;

	// Sun added 2013-04-12
	if( GetBufferNum() > 0 )
		RecycleWaveBuffer((DWORD)m_hOut);

	return TRUE;
}

// Sun added 2013-04-12
void CWaveOutDevice::WaveoutDone(DWORD hHandle, PVOID pData)
{
	// Lock Me
	CSingleLock lv_SingleLock(&m_bLock, TRUE);

	try
	{
		if( pData )
		{
			WAVEHDR* pwh = (WAVEHDR*)pData;

#ifdef CUCM_MEDIA
#else
			waveOutUnprepareHeader((HWAVEOUT)hHandle, pwh, sizeof(WAVEHDR));
#endif

			// Sun added 2013-04-12
			WriteTrace(TraceDetail, "IVR Channel: %d - CWaveOutDevice(%d)::Play: recycle data memory dataindex = %d, item count = %d.", m_lngIVRChannelID, m_dwWaveOutID, pwh->reserved, m_iBufferNum - 1);
			m_pWavData[pwh->reserved] = NULL;

			// 删除Play调用时分配的内存
			delete []pwh->lpData;
			delete pwh;
		}
	}
	catch(...)
	{
	}

	// Sun added 2011-06-24, 
	/// BufferSub有锁处理，不清楚以前为什么能正常工作
	lv_SingleLock.Unlock();

	BufferSub();
}

// Sun added 2013-04-12
void CWaveOutDevice::RecycleWaveBuffer(DWORD hHandle)
{
	int lv_loop;

	// Lock Me
	CSingleLock lv_SingleLock(&m_bLock, TRUE);

	try
	{
		for( lv_loop = 0; lv_loop < NUM_BUF+1; lv_loop++ )
		{
			if( m_pWavData[lv_loop] )
			{
				WAVEHDR* pwh = m_pWavData[lv_loop];

#ifdef CUCM_MEDIA
#else
				waveOutUnprepareHeader((HWAVEOUT)hHandle, pwh, sizeof(WAVEHDR));
#endif

				// 删除Play调用时分配的内存
				delete []pwh->lpData;
				delete pwh;

				// Sun added 2013-04-12
				m_pWavData[lv_loop] = NULL;
				WriteTrace(TraceDetail, "IVR Channel: %d - CWaveOutDevice(%d)::Play: recycle data memory dataindex = %d, item count = %d.", m_lngIVRChannelID, m_dwWaveOutID, lv_loop, --m_iBufferNum);
			}
		}
	}
	catch(...)
	{
	}

	m_nPrtHead = 0;
	m_iBufferNum = 0;
}

int CWaveOutDevice::Run()
{
	MSG msg;
	while( GetMessage(&msg, 0, 0, 0) )
	{
		switch( msg.message )
		{
		case WOM_OPEN:
			// Sun added 2011-06-24
			ResetOpenDevWait();
			ResetCloseDevWait();

			WriteTrace(TraceDetail, "IVR Channel: %d - CWaveOutDevice(%d)::WOM_OPEN.", m_lngIVRChannelID, m_dwWaveOutID);
			break;

		case WOM_CLOSE:
			// Sun added 2011-06-24
			ResetOpenDevWait();
			ResetCloseDevWait();

			// Sun added 2013-04-12
			if( GetBufferNum() > 0 )
				RecycleWaveBuffer((DWORD)msg.wParam);

			WriteTrace(TraceDetail, "IVR Channel: %d -CWaveOutDevice(%d)::WOM_CLOSE.", m_lngIVRChannelID, m_dwWaveOutID);
			break;

		case WOM_DONE:
			// Sun added 2013-04-12
			WaveoutDone((DWORD)msg.wParam, (PVOID)msg.lParam);
			break;
		}
	}

	return msg.wParam;
}

int CWaveOutDevice::StartPlay()
{
	int nRet = 0;

	if( !StartThread() )
	{
		// Failed to start thread
		nRet = -1;
		goto Exit;
	};

	if( !OpenDev() )
	{
		// Failed to open device
		nRet = -2;
		goto Exit1;
	};

	m_bPlaying = TRUE;
	goto Exit;
Exit1:
	StopThread();
Exit:
	return nRet;
}

BOOL CWaveOutDevice::StopPlay()
{
	// Sun added 2011-06-27
	if( pMainWindow->m_bSystemAutoShutDown )
		return TRUE;

	// Sun updated 2010-07-20
	/// From
	//Stop();
	//CloseDev();
	//StopThread();
	/// To
	if( m_bPlaying )
		Stop();
	if( m_bDevOpen )
		CloseDev();
	if( m_bThreadStart )
		StopThread();

	return TRUE;
}

// 是否正在放音
BOOL CWaveOutDevice::IsPlaying()
{
	// Sun updated 2010-07-20
	/// From
	//return (m_bThreadStart && m_bDevOpen && m_bPlaying );
	/// To
	return (m_bThreadStart || m_bPlaying || m_bDevOpen);
}

// Play Buffer
BOOL CWaveOutDevice::Play(char* buf, UINT uSize)
{
	if( !m_bPlaying )
		return TRUE;

	if( !m_bDevOpen )
	{
		WriteTrace(TraceError, "IVR Channel: %d - CWaveOutDevice(%d)::Play: Device hasn't been open.", m_lngIVRChannelID, m_dwWaveOutID);
		return TRUE;
	}

	if( GetBufferNum () > PLAY_DELAY )
	{
		WriteTrace(TraceError, "IVR Channel: %d - CWaveOutDevice(%d)::Play: Delay too big pass over.", m_lngIVRChannelID, m_dwWaveOutID);
		return TRUE;
	}

	// Lock Me
	CSingleLock lv_SingleLock(&m_bLock, TRUE);

	char* p;
	LPWAVEHDR pwh = new WAVEHDR;
	if(!pwh)
	{
		WriteTrace(TraceError, "IVR Channel: %d - CWaveOutDevice(%d)::Play: alloc WAVEHDR memory error.", m_lngIVRChannelID, m_dwWaveOutID);
		return FALSE;
	}
	
	p = new char[uSize];
	if(!p)
	{
		WriteTrace(TraceError, "IVR Channel: %d - CWaveOutDevice(%d)::Play: alloc data memory error.", m_lngIVRChannelID, m_dwWaveOutID);
		delete pwh;		// Sun added 2013-04-12
		return FALSE;
	}

	// Sun added 2013-04-12
	m_pWavData[m_nPrtHead] = pwh;
	WriteTrace(TraceDetail, "IVR Channel: %d - CWaveOutDevice(%d)::Play: alloc data memory dataindex = %d, item count = %d.", m_lngIVRChannelID, m_dwWaveOutID, m_nPrtHead, m_iBufferNum+1);

	CopyMemory(p, buf, uSize);
	ZeroMemory(pwh, sizeof(WAVEHDR));
	pwh->dwBufferLength = uSize;
	pwh->lpData = p;
	pwh->reserved = m_nPrtHead;	// Sun added 2013-04-12
	
#ifdef CUCM_MEDIA

	// Sun added 2013-04-12
	pwh->dwUser = m_lngIVRChannelID;

	PRTPENDPOINTCALLBACK lv_pCallback = MediaStreamCallback;	
	//if( !EpStreamWrite(m_hMediaStream, (PUCHAR)buf, uSize, (PVOID)m_lngIVRChannelID, lv_pCallback) )
	if( !EpStreamWrite(m_hMediaStream, (PUCHAR)p, uSize, (PVOID)pwh, lv_pCallback) )
	{
		// 防止内存泄露
		try
		{
			delete []pwh->lpData;
			delete pwh;

			// Sun added 2013-04-12
			m_pWavData[m_iBufferNum] = NULL;
			WriteTrace(TraceDetail, "IVR Channel: %d - CWaveOutDevice(%d)::Play: recycle data memory dataindex = %d, item count = %d.", m_lngIVRChannelID, m_dwWaveOutID, m_nPrtHead, m_iBufferNum);
		}
		catch(...)
		{
		}

        WriteTrace(TraceError, "IVR Channel: %d - CWaveOutDevice::Play: EpStreamWrite error 0x%x.", m_lngIVRChannelID, EpApiGetLastError());
		return FALSE;
	}

#else

	m_mmr = waveOutPrepareHeader(m_hOut, pwh, sizeof(WAVEHDR));
	
	// Sun added 2011-07-22
	/// 遇到MMSYSERR_NOMEM时，再试一次，还不行才终止程序
	if(m_mmr)
	{
		if( m_mmr == MMSYSERR_NOMEM )		// 0x07
		{
			WriteTrace(TraceError, "IVR Channel: %d - CWaveOutDevice(%d)::Play: waveOutPrepareHeader error: MMSYSERR_NOMEM(0x07).", 
				m_lngIVRChannelID, m_dwWaveOutID);

			Sleep(20);
			m_mmr = waveOutPrepareHeader(m_hOut, pwh, sizeof(WAVEHDR));
		}
	}

  	if(m_mmr)
	{
		// 防止内存泄露
		try
		{
			delete []pwh->lpData;
			delete pwh;

			// Sun added 2013-04-12
			m_pWavData[m_iBufferNum] = NULL;
			WriteTrace(TraceDetail, "IVR Channel: %d - CWaveOutDevice(%d)::Play: recycle data memory dataindex = %d, item count = %d.", m_lngIVRChannelID, m_dwWaveOutID, m_nPrtHead, m_iBufferNum);
		}
		catch(...)
		{
		}

		WriteTrace(TraceError, "IVR Channel: %d - CWaveOutDevice(%d)::Play: waveOutPrepareHeader error: 0x%x.", 
			m_lngIVRChannelID, m_dwWaveOutID, m_mmr);

		// Sun added 2011-04-16
		if( m_mmr == MMSYSERR_NOMEM )		// 0x07
		{
			if( (gb_lngAutoCloseFlag & PCS_PVC_AUTOCLOSE_WAVEOUT_NOMEM) == PCS_PVC_AUTOCLOSE_WAVEOUT_NOMEM )
			{
				pMainWindow->PostMessage(WM_SYS_AUTO_SHUTDOWN, PCS_PVC_AUTOCLOSE_WAVEOUT_NOMEM, m_lngIVRChannelID);
			}
		}

		if( !m_bPlaying )
			return TRUE;

		return FALSE;
	}

	m_mmr = waveOutWrite(m_hOut, pwh, sizeof(WAVEHDR));
  	if (m_mmr)
	{
		// 防止内存泄露
		try
		{
			waveOutUnprepareHeader(m_hOut, pwh, sizeof(WAVEHDR));
			delete []pwh->lpData;
			delete pwh;

			// Sun added 2013-04-12
			m_pWavData[m_iBufferNum] = NULL;
			WriteTrace(TraceDetail, "IVR Channel: %d - CWaveOutDevice(%d)::Play: recycle data memory dataindex = %d, item count = %d.", m_lngIVRChannelID, m_dwWaveOutID, m_nPrtHead, m_iBufferNum);
		}
		catch(...)
		{
		}

		WriteTrace(TraceError, "IVR Channel: %d - CWaveOutDevice(%d)::Play: waveOutWrite error: 0x%x.", m_lngIVRChannelID, m_dwWaveOutID, m_mmr);
		if( !m_bPlaying )
			return TRUE;

		return FALSE;
	}

#endif

	m_iBufferNum++;
	
	// Sun added 2013-04-12
	m_nPrtHead++;
	if( m_nPrtHead >= NUM_BUF )
		m_nPrtHead = 0;

	return TRUE;
}

// Notes: Cisco TAPI 2.1 don't support 'waveOutRestart' routine
BOOL CWaveOutDevice::Continue()
{
	if( !m_bDevOpen )
	{
		WriteTrace(TraceError, "IVR Channel: %d - CWaveOutDevice(%d)::Continue: Device hasn't been open.", m_lngIVRChannelID, m_dwWaveOutID);
		return FALSE;
	}

	// Lock Me
	CSingleLock lv_SingleLock(&m_bLock, TRUE);

#ifdef CUCM_MEDIA
#else

	if( m_hOut )
	{
		m_mmr = waveOutRestart(m_hOut);
  		if( m_mmr )
		{
			WriteTrace(TraceError, "IVR Channel: %d - CWaveOutDevice(%d)::Continue: waveOutRestart error.", m_lngIVRChannelID, m_dwWaveOutID);
			return FALSE;
		}
	}

#endif

	return TRUE;
}

// Notes: Cisco TAPI 2.1 don't support 'waveOutPause' routine
BOOL CWaveOutDevice::Pause()
{
	// Lock Me
	CSingleLock lv_SingleLock(&m_bLock, TRUE);

	if( !m_bDevOpen )
	{
		WriteTrace(TraceWarn, "IVR Channel: %d - CWaveOutDevice(%d)::Pause: Device hasn't been open.", m_lngIVRChannelID, m_dwWaveOutID);
		return FALSE;
	}

#ifdef CUCM_MEDIA
#else

	if( m_hOut )
	{
		m_mmr = waveOutPause(m_hOut);
  		if( m_mmr )
		{
			WriteTrace(TraceError, "IVR Channel: %d - CWaveOutDevice(%d)::Pause: waveOutPause error.", m_lngIVRChannelID, m_dwWaveOutID);
			return FALSE;
		}
	}

#endif

	return TRUE;
}

BOOL CWaveOutDevice::Stop()
{
	// Sun added 2011-06-27
	if( pMainWindow->m_bSystemAutoShutDown )
	{
		m_bPlaying = FALSE;
		return TRUE;
	}

	if( !m_bDevOpen )
	{
		m_bPlaying = FALSE;
		WriteTrace(TraceWarn, "IVR Channel: %d - CWaveOutDevice(%d)::Stop: Device hasn't been open.", m_lngIVRChannelID, m_dwWaveOutID);
		return FALSE;
	}

	// Lock Me
	CSingleLock lv_SingleLock(&m_bLock, TRUE);

#ifdef CUCM_MEDIA

	// Sun added 2013-04-12
	if( m_hMediaStream && m_bPlaying )
	{
		WriteTrace(TraceDetail, "IVR Channel: %d - CWaveOutDevice(%d)::Stop: EpStreamStop.", m_lngIVRChannelID, m_dwWaveOutID);

		// Prevent to play more buffer
		m_bPlaying = FALSE;

		if( !EpStreamStop(m_hMediaStream) )
		{
			WriteTrace(TraceError, "IVR Channel: %d - CWaveOutDevice::Stop: EpStreamStop error 0x%x.", m_lngIVRChannelID, EpApiGetLastError());
		}
	}
	WriteTrace(TraceDetail, "IVR Channel: %d - CWaveOutDevice(%d)::Stop: EpStreamStop Finished.", m_lngIVRChannelID, m_dwWaveOutID);

#else

	if( m_hOut && m_bPlaying )
	{
		WriteTrace(TraceDetail, "IVR Channel: %d - CWaveOutDevice(%d)::Stop: waveOutReset.", m_lngIVRChannelID, m_dwWaveOutID);

		// Prevent to play more buffer
		m_bPlaying = FALSE;

		m_mmr = waveOutReset(m_hOut);
  		if( m_mmr )
		{
			WriteTrace(TraceError, "IVR Channel: %d - CWaveOutDevice(%d)::Stop: waveOutReset error.", m_lngIVRChannelID, m_dwWaveOutID);
			return FALSE;
		}
		WriteTrace(TraceDetail, "IVR Channel: %d - CWaveOutDevice(%d)::Stop: waveOutReset Finished.", m_lngIVRChannelID, m_dwWaveOutID);
	}

#endif

	m_bPlaying = FALSE;
	return TRUE;
}

//********************************************************************
//* Construction: Wave In Device class
//********************************************************************
CWaveInDevice::CWaveInDevice(void)
{
	m_dwWaveInID = 0;
	m_hIn = 0;
	m_pHdr = NULL;
	m_bRecording = FALSE;

	// Sun added 2013-04-12
#ifdef CUCM_MEDIA
	m_hMediaEpIn = NULL;
#endif

}

CWaveInDevice::~CWaveInDevice(void)
{
	// Stop undergoing process
	if( IsRecording() )
		CloseRecord();

	// Close Device
	if( m_bDevOpen )
		CloseDev();

	// Stop Thread
	if( m_bThreadStart )
		StopThread();
}

BOOL CWaveInDevice::StartRecord()
{
	BOOL bRet = FALSE;

	if( !StartThread() )
	{
		goto Exit;
	};

	if( !OpenDev() )
	{
		goto Exit1;
	};

	if( !PerPareBuffer() )
	{
		goto Exit2;
	}

	if( !OpenRecord() )
	{
		goto Exit3;
	}
	bRet = TRUE;
	m_bRecording = TRUE;
	goto Exit;
Exit3:
	FreeBuffer();
Exit2:
	CloseDev();
Exit1:
	StopThread();
Exit:
	return bRet;
}

BOOL CWaveInDevice::StopRecord()
{
	// Sun added 2011-06-27
	if( pMainWindow->m_bSystemAutoShutDown )
		return TRUE;

	//------------------------------
	// Sun updated 2010-07-20
	/// From
	/*
	CloseRecord();	
	Sleep(1500);

	// notice delete
	FreeBuffer();
	if( CloseDev() )
	{
		StopThread();
	}
	*/
	/// To
	if( m_bRecording )
	{
		CloseRecord();	
		Sleep(1500);
	}

	// notice delete
	FreeBuffer();
	if( m_bDevOpen )
		CloseDev();
	if( m_bThreadStart )
		StopThread();
	//------------------------------	

	return TRUE;
}

BOOL CWaveInDevice::IsRecording()
{
	// Sun updated 2010-07-20
	/// From
	// Sun replaced back 2010-11-16 from 2010-07-20
	return (m_bThreadStart && m_bDevOpen && m_bRecording);
	/// To
	//return (m_bThreadStart || m_bRecording || m_bDevOpen);
}

BOOL CWaveInDevice::PerPareBuffer()
{
	// Lock Me
	CSingleLock lv_SingleLock(&m_bLock, TRUE);

	if( m_bAllocBuffer )
	{
		WriteTrace(TraceWarn, "IVR Channel: %d - CWaveInDevice(%d)::PerPareBuffer: Buffer has been alloc.", m_lngIVRChannelID, m_dwWaveInID);
		return FALSE;
	}

#ifdef CUCM_MEDIA

	// Sun added 2013-04-12
	char* p;
	LPWAVEHDR pwh = new WAVEHDR;
	if(!pwh)
	{
		WriteTrace(TraceError, "IVR Channel: %d - CWaveInDevice(%d)::PerPareBuffer: alloc WAVEHDR memory error.", m_lngIVRChannelID, m_dwWaveInID);
		return FALSE;
	}
	
	p = new char[SIZE_AUDIO_FRAME];
	if(!p)
	{
		WriteTrace(TraceError, "IVR Channel: %d - CWaveInDevice(%d)::PerPareBuffer: alloc data memory error.", m_lngIVRChannelID, m_dwWaveInID);
		delete pwh;		// Sun added 2013-04-12
		return FALSE;
	}

	// Sun added 2013-04-12
	m_bAllocBuffer = TRUE;
	WriteTrace(TraceDetail, "IVR Channel: %d - CWaveInDevice(%d)::PerPareBuffer: alloc data memory dataindex = %d.", m_lngIVRChannelID, m_dwWaveInID, m_iBufferNum);

	ZeroMemory(p, SIZE_AUDIO_FRAME);
	ZeroMemory(pwh, sizeof(WAVEHDR));
	pwh->dwBufferLength = SIZE_AUDIO_FRAME;
	pwh->lpData = p;
	pwh->dwUser = m_lngIVRChannelID;
	m_pHdr = pwh;

#else

	m_mmr = waveInReset(m_hIn);
	if( m_mmr )
	{
		WriteTrace(TraceError, "IVR Channel: %d - CWaveInDevice(%d)::PerPareBuffer: waveInReset error.", m_lngIVRChannelID, m_dwWaveInID);
		return FALSE;
	}

	UINT i;
	m_pHdr = new WAVEHDR[NUM_BUF];
	if( m_pHdr )
	{
		m_bAllocBuffer = TRUE;
			
		for( i = 0; i < NUM_BUF; i++ )
		{
			ZeroMemory(&m_pHdr[i], sizeof(WAVEHDR));
			m_pHdr[i].lpData = new char[SIZE_AUDIO_FRAME];
			m_pHdr[i].dwBufferLength = SIZE_AUDIO_FRAME;
			m_mmr = waveInPrepareHeader(m_hIn, &m_pHdr[i], sizeof(WAVEHDR));
			if( m_mmr )
			{
				WriteTrace(TraceError, "IVR Channel: %d - CWaveInDevice(%d)::PerPareBuffer: waveInPrepareHeader error.", m_lngIVRChannelID, m_dwWaveInID);
				FreeBuffer();
				return FALSE;
			}

			m_mmr = waveInAddBuffer(m_hIn, &m_pHdr[i], sizeof(WAVEHDR));
			if( m_mmr )
			{
				WriteTrace(TraceError, "IVR Channel: %d - CWaveInDevice(%d)::PerPareBuffer: waveInAddBuffer error.", m_lngIVRChannelID, m_dwWaveInID);
				FreeBuffer();
				return FALSE;
			}
		}
	}

#endif

	return TRUE;
}

BOOL CWaveInDevice::FreeBuffer()
{
	// Lock Me
	CSingleLock lv_SingleLock(&m_bLock, TRUE);

	if( !m_bAllocBuffer )
	{
		WriteTrace(TraceWarn, "IVR Channel: %d - CWaveInDevice(%d)::FreeBuffer: Buffer hasn't been alloc.", m_lngIVRChannelID, m_dwWaveInID);
		return FALSE;
	}
	
	if( !m_pHdr )
	{
		WriteTrace(TraceWarn, "IVR Channel: %d - CWaveInDevice(%d)::FreeBuffer: m_pHdr is NULL.", m_lngIVRChannelID, m_dwWaveInID);
		return FALSE;
	}

#ifdef CUCM_MEDIA

	// Sun added 2013-04-12
	// Do nothing here
	// 防止内存泄露
	try
	{
		delete []m_pHdr->lpData;
		delete m_pHdr;

		// Sun added 2013-04-12
		WriteTrace(TraceDetail, "IVR Channel: %d - CWaveInDevice(%d)::FreeBuffer: recycle data memory dataindex = %d.", m_lngIVRChannelID, m_dwWaveInID, m_iBufferNum);
	}
	catch(...)
	{
	}

#else

	UINT i;

	try
	{
		for( i = 0; i < NUM_BUF; i++ )
		{
			m_mmr = waveInUnprepareHeader(m_hIn, &m_pHdr[i], sizeof(WAVEHDR));
			if( m_mmr )
			{
				Sleep(100);
				WriteTrace(TraceError, "IVR Channel: %d - CWaveInDevice(%d)::FreeBuffer: waveInUnprepareHeader error: %d.", m_lngIVRChannelID, m_dwWaveInID, m_mmr);
				continue;
			}
			TRACE("CWaveInDevice::FreeBuffer: %d.\n",i);

			if( m_pHdr[i].lpData )
				delete []m_pHdr[i].lpData;
		
		}

		if( m_pHdr )
			delete []m_pHdr;
	}
	catch(...)
	{
	}

#endif

	m_pHdr = NULL;
	m_bAllocBuffer = FALSE;

	return TRUE;
}

BOOL CWaveInDevice::OpenRecord()
{
	// Lock Me
	CSingleLock lv_SingleLock(&m_bLock, TRUE);

	if( IsRecording() )
	{
		WriteTrace(TraceWarn, "IVR Channel: %d - CWaveInDevice(%d)::OpenRecord: You may be has begun recored.", m_lngIVRChannelID, m_dwWaveInID);
		return FALSE;
	}

#ifdef CUCM_MEDIA

	// Sun added 2013-04-12
	PRTPENDPOINTCALLBACK lv_pCallback = MediaStreamCallback;	
	if( !EpStreamRead(m_hMediaStream, (PUCHAR)m_pHdr->lpData, SIZE_AUDIO_FRAME, (PVOID)m_pHdr, lv_pCallback) )
	{
        WriteTrace(TraceError, "IVR Channel: %d - CWaveInDevice::OpenRecord: EpStreamRead error 0x%x.", m_lngIVRChannelID, EpApiGetLastError());
		return FALSE;
	}

#else

	if( !m_hIn )
	{
		WriteTrace(TraceWarn, "IVR Channel: %d - CWaveInDevice(%d)::CloseDev: Device hasn't opened.", m_lngIVRChannelID, m_dwWaveInID);
		return FALSE;
	}

	m_mmr = waveInStart(m_hIn);
	if( m_mmr )
	{
		WriteTrace(TraceError, "IVR Channel: %d - CWaveInDevice(%d)::OpenRecord: waveInStart error.", m_lngIVRChannelID, m_dwWaveInID);
		return FALSE;
	}

#endif

	return TRUE;
}

BOOL CWaveInDevice::CloseRecord()
{
	// Sun added 2011-06-27
	if( pMainWindow->m_bSystemAutoShutDown )
	{
		m_bRecording = FALSE;
		return TRUE;
	}

	// Lock Me
	CSingleLock lv_SingleLock(&m_bLock, TRUE);

	if( !IsRecording() )
	{
		m_bRecording = FALSE;
		WriteTrace(TraceWarn, "IVR Channel: %d - CWaveIn(%d)::CloseRecord: You may be hasn't begun recored.", m_lngIVRChannelID, m_dwWaveInID);
		return FALSE;
	}

#ifdef CUCM_MEDIA

	// Sun added 2013-04-12
	if( m_hMediaStream && m_bRecording )
	{
		WriteTrace(TraceDetail, "IVR Channel: %d - CWaveInDevice(%d)::CloseRecord: EpStreamStop.", m_lngIVRChannelID, m_dwWaveInID);

		// Prevent to play more buffer
		m_bRecording = FALSE;

		if( !EpStreamStop(m_hMediaStream) )
		{
			WriteTrace(TraceError, "IVR Channel: %d - CWaveInDevice::CloseRecord: EpStreamStop error 0x%x.", m_lngIVRChannelID, EpApiGetLastError());
		}
	}
	WriteTrace(TraceDetail, "IVR Channel: %d - CWaveInDevice(%d)::CloseRecord: EpStreamStop Finished.", m_lngIVRChannelID, m_dwWaveInID);

#else

	if( !m_hIn )
	{
		m_bRecording = FALSE;
		WriteTrace(TraceWarn, "IVR Channel: %d - CWaveIn(%d)::CloseRecord: Device hasn't opened.", m_lngIVRChannelID, m_dwWaveInID);
		return FALSE;
	}

/*	m_mmr=waveInStop(m_hIn);
	if(m_mmr)
	{
		TRACE("CWaveIn::CloseRecord: waveInStop error.\n");
		return FALSE;
	}
 */	
	m_mmr = waveInReset(m_hIn);
	if( m_mmr )
	{
		m_bRecording = FALSE;
		WriteTrace(TraceError, "IVR Channel: %d - CWaveIn(%d)::CloseRecord: waveInReset error.", m_lngIVRChannelID, m_dwWaveInID);
		return FALSE;
	}

#endif

	m_bRecording = FALSE;
	return TRUE;
}

BOOL CWaveInDevice::OpenDev()
{
#ifdef CUCM_MEDIA

	// Sun added 2013-04-12
	// Ep已经在打开TapiLine的同时打开
	if( m_hMediaEpIn )
	{
		m_hMediaStream = EpGetStreamHandle(m_hMediaEpIn, STREAM_TYPE_AUDIO, ToApp);
		if( m_hMediaStream == NULL )
		{
			WriteTrace(TraceError, "IVR Channel: %d - CWaveInDevice::OpenDev0: EpGetStreamHandle error 0x%x.", m_lngIVRChannelID, EpApiGetLastError());
			return FALSE;
		}

		WAVEFORMATEX lv_WaveF;
		if( !EpStreamCodecInGet(m_hMediaStream, &lv_WaveF) )
		{
			SetFormat(WAVE_FORMAT_MULAW);
			SetBit(8);
			SetChannel(1);
			SetSample(8000);
			WriteTrace(TraceError, "IVR Channel: %d - CWaveInDevice::OpenDev: EpStreamCodecInGet error 0x%x.", m_lngIVRChannelID, EpApiGetLastError());
		}
		else
		{
			SetFormat(lv_WaveF.wFormatTag);
			SetBit(lv_WaveF.wBitsPerSample);
			SetChannel(lv_WaveF.nChannels);
			SetSample(lv_WaveF.nSamplesPerSec);
		}
		BuildFormat();

		// Sets stream inbound codec format
		//if( !EpStreamCodecInSet(m_hMediaStream, &m_pcmWaveFormat, 0) )
		if( !EpStreamCodecInSet(m_hMediaStream, &lv_WaveF, 0) )
		{
			WriteTrace(TraceError, "IVR Channel: %d - CWaveInDevice::OpenDev: EpStreamCodecInSet error 0x%x.", m_lngIVRChannelID, EpApiGetLastError());
		}

		// Sun added 2013-04-12
		PRTPENDPOINTCALLBACK lv_pCallback = MediaStreamCallback;
		if( !EpStreamStart(m_hMediaStream, lv_pCallback) )
		{
			WriteTrace(TraceError, "IVR Channel: %d - CWaveInDevice::OpenDev: Start wave I/O thread fail error 0x%x.", m_lngIVRChannelID, EpApiGetLastError());
			return FALSE;
		}
	}
	else
		return FALSE;

#else

	if( m_bDevOpen )
	{
		WriteTrace(TraceWarn, "IVR Channel: %d - CWaveInDevice(%d)::OpenDev: Device has open.", m_lngIVRChannelID, m_dwWaveInID);
		return FALSE;
	}

	BuildFormat();

	if( m_dwWaveInID == 0 )
	{
		m_mmr = waveInOpen(0, WAVE_MAPPER, &m_pcmWaveFormat, 0, 0, WAVE_FORMAT_QUERY);
		if( m_mmr )
		{
			WriteTrace(TraceError, "IVR Channel: %d - CWaveInDevice(%d)::OpenDev0: waveInOpen error.", m_lngIVRChannelID, m_dwWaveInID);
			return FALSE;
		}
		m_mmr = waveInOpen(&m_hIn, WAVE_MAPPER, &m_pcmWaveFormat, m_dwAudioDevId, s_dwInstance, CALLBACK_THREAD);
	}
	else
	{
		m_mmr = waveInOpen(&m_hIn, m_dwWaveInID, &m_pcmWaveFormat, m_dwAudioDevId, s_dwInstance, CALLBACK_THREAD | WAVE_MAPPED);
	}

	if( m_mmr )
	{
		WriteTrace(TraceError, "IVR Channel: %d - CWaveInDevice(%d)::OpenDev: waveInOpen error.", m_lngIVRChannelID, m_dwWaveInID);
		return FALSE;
	}

#endif

	m_bDevOpen = TRUE;
	m_iBufferNum = 0;

	return TRUE;
}

BOOL CWaveInDevice::CloseDev()
{
#ifdef CUCM_MEDIA
	// Sun added 2013-04-12
	if( m_hMediaStream )
	{
		if( !EpStreamStop(m_hMediaStream) )
		{
			WriteTrace(TraceError, "IVR Channel: %d - CWaveInDevice::StopThread: Stop wave I/O thread failed error 0x%x.", m_lngIVRChannelID, EpApiGetLastError());
		}
	}
#else

	// Sun added 2011-06-27
	if( pMainWindow->m_bSystemAutoShutDown )
	{
		m_bDevOpen = FALSE;
		return TRUE;
	}

	// Sun moved from 2011-07-10 [B]
	// Lock Me
	CSingleLock lv_SingleLock(&m_bLock, TRUE);

	if( !m_bDevOpen )
	{
		WriteTrace(TraceWarn, "IVR Channel: %d - CWaveInDevice(%d)::CloseDev: Device hasn't opened.", m_lngIVRChannelID, m_dwWaveInID);
		return FALSE;
	}

	if( !m_hIn )
	{
		WriteTrace(TraceWarn, "IVR Channel: %d - CWaveInDevice(%d)::CloseDev: Device hasn't opened.", m_lngIVRChannelID, m_dwWaveInID);
		return FALSE;
	}

	// Sun moved ahead 2011-07-10 [B]
	// Lock Me
	//CSingleLock lv_SingleLock(&m_bLock, TRUE);

	m_mmr = waveInClose(m_hIn);
	if( m_mmr )
	{
		WriteTrace(TraceError, "IVR Channel: %d - CWaveInDevice(%d)::CloseDev: waveInClose error: 0x%x.", m_lngIVRChannelID, m_dwWaveInID, m_mmr);

		if( m_mmr == WAVERR_STILLPLAYING )
		{
			m_mmr = waveInReset(m_hIn);
			m_mmr = waveInClose(m_hIn);
		}
		if( m_mmr )
			return FALSE;
	}

#endif

	m_hIn=0;
	m_bDevOpen = FALSE;

	return TRUE;
}

// Sun added 2013-04-12
void CWaveInDevice::WaveinData(DWORD hHandle, PVOID pData)
{
	char buffer[SIZE_AUDIO_FRAME];

	if( pData )
	{
		if( IsRecording() )
		{
			// Lock Me
			CSingleLock lv_SingleLock(&m_bLock, TRUE);

			try
			{
				WAVEHDR* pWH = (WAVEHDR*)pData;

#ifdef CUCM_MEDIA
				// 保存数据，继续录音
				memcpy(buffer, pWH->lpData, pWH->dwBytesRecorded);
				GetData(buffer, pWH->dwBytesRecorded);

				ZeroMemory(pWH->lpData, SIZE_AUDIO_FRAME);
				pWH->dwBytesRecorded = 0;
				//pWH->dwBufferLength = SIZE_AUDIO_FRAME;
				//pWH->dwUser = hHandle;

				PRTPENDPOINTCALLBACK lv_pCallback = MediaStreamCallback;	
				if( !EpStreamRead(m_hMediaStream, (PUCHAR)pWH->lpData, SIZE_AUDIO_FRAME, (PVOID)pWH, lv_pCallback) )
				{
					// 防止内存泄露
					try
					{
						delete []pWH->lpData;
						delete pWH;

						// Sun added 2013-04-12
						WriteTrace(TraceDetail, "IVR Channel: %d - CWaveInDevice(%d)::WaveinData: recycle data memory dataindex = %d.", hHandle, m_dwWaveInID, m_iBufferNum);
					}
					catch(...)
					{
					}

					WriteTrace(TraceError, "IVR Channel: %d - CWaveInDevice::WaveinData: EpStreamRead error 0x%x.", hHandle, EpApiGetLastError());
				}

#else
				waveInUnprepareHeader((HWAVEIN)hHandle, pWH, sizeof(WAVEHDR));
				
				if( pWH->dwBytesRecorded == SIZE_AUDIO_FRAME )
				{
					/*
					DWORD lv_dwSum = 0;
					for( int i = 0; i < pWH->dwBytesRecorded; i++ )
						lv_dwSum += pWH->lpData[i];
					WriteTrace(TraceDetail, "Get Data len = %d, sum=%u", pWH->dwBytesRecorded, lv_dwSum);
					*/

					memcpy(buffer, pWH->lpData, pWH->dwBytesRecorded);
					GetData(buffer, pWH->dwBytesRecorded);

					waveInPrepareHeader((HWAVEIN)hHandle, pWH, sizeof(WAVEHDR));
					waveInAddBuffer((HWAVEIN)hHandle, pWH, sizeof(WAVEHDR));
				}
#endif
			}
			catch(...)
			{
			}
		}
	}
}

int CWaveInDevice::Run()
{
	MSG msg;
	while( GetMessage(&msg, 0, 0, 0) )
	{
		switch( msg.message )
		{
		case MM_WIM_OPEN:
			WriteTrace(TraceDetail, "IVR Channel: %d - CWaveInDevice(%d)::MM_WIM_OPEN.", m_lngIVRChannelID, m_dwWaveInID);
			break;

		case MM_WIM_CLOSE:
			WriteTrace(TraceDetail, "IVR Channel: %d - CWaveInDevice(%d)::MM_WIM_CLOSE.", m_lngIVRChannelID, m_dwWaveInID);
			break;

		case MM_WIM_DATA:
			// Sun added 2013-04-12
			WaveinData((DWORD)msg.wParam, (PVOID)msg.lParam);
			break;
		}
	}

	return msg.wParam;
}

void CWaveInDevice::GetData(char *pBuffer,int iLen)
{

}

#endif