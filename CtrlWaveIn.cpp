// MyWaveIn.cpp: implementation of the CCtrlWaveIn class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CtrlWaveIn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifdef CISCO_CCM

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCtrlWaveIn::CCtrlWaveIn()
{
	m_bIsSilence = FALSE;
	m_dwSilenceTick = 0;
	m_nMaxSilenceTime = 5;
	m_nActiveThreshold = 0;
	m_nAverageFrameValue = 0;

	m_nMinDiskBuffer = 0;
	m_nMaxDiskBuffer = 2880;
	m_hRecordFile = INVALID_HANDLE_VALUE;
	m_sRecordFileName = _T("");
}

CCtrlWaveIn::~CCtrlWaveIn()
{
	if( IsRecording() )
	{
		StopRecord();
		Sleep(10);
	}
}

CString CCtrlWaveIn::GetRecordFileName()
{
	return m_sRecordFileName;
}

void CCtrlWaveIn::SetRecordFileName(CString strFileName)
{
	if( m_sRecordFileName != strFileName )
	{
		TRACE("File Name:%s\n", strFileName);
		m_sRecordFileName = strFileName;
	}
}

int CCtrlWaveIn::GetAveFrameValue()
{
	int lv_Value;

	m_bLock.Enter();
	lv_Value = m_nAverageFrameValue;
	m_bLock.Leave();

	return lv_Value;
}

void CCtrlWaveIn::SetAveFrameValue(int f_Value)
{
	m_bLock.Enter();
	m_nAverageFrameValue = f_Value;
	m_bLock.Leave();
}

BOOL CCtrlWaveIn::IsSilenceFrame(const char *pBuffer, int iLen)
{
	// 录音门限
	int lv_loop, lv_AveValue;
	long lv_sum = 0;

	if( GetBit() == 8 )
	{
		for( lv_loop = 0; lv_loop < iLen; lv_loop++ )
			lv_sum += abs( pBuffer[lv_loop] );
	}
	else
	{
		for( lv_loop = 0; lv_loop < iLen / 2; lv_loop++ )
			lv_sum += abs( ((SHORT*)pBuffer)[lv_loop] );
	}

	lv_AveValue = lv_sum / lv_loop;
	SetAveFrameValue(lv_AveValue);

	TRACE("FRAME SUM:%d\n", lv_AveValue);
	if( lv_AveValue < GetActiveThreshold() )
		return TRUE;

	return FALSE;
}

// 最短录音时间（秒）
int CCtrlWaveIn::GetMinDiskBuffer()
{
	return m_nMinDiskBuffer;
}

void CCtrlWaveIn::SetMinDiskBuffer(int f_Value)
{
	m_nMinDiskBuffer = f_Value;
}

// KB
int CCtrlWaveIn::GetMaxDiskBuffer()
{
	return m_nMaxDiskBuffer;
}

// KB
void CCtrlWaveIn::SetMaxDiskBuffer(int f_Value)
{
	if( f_Value < SIZE_AUDIO_FRAME / 10 )
		m_nMaxDiskBuffer = SIZE_AUDIO_FRAME / 10;
	else
		m_nMaxDiskBuffer = f_Value;
	m_diskdata.SetMaxLength(m_nMaxDiskBuffer * 1000);
}

BOOL CCtrlWaveIn::WriteRecordData(BOOL f_bClose)
{
	BOOL lv_retval = FALSE;

	long lv_buflen;

	if( (lv_buflen = m_diskdata.Length()) > 0 )
	{
		//TRACE("File Buffer Length:%d\n", lv_buflen);
		m_diskdata.LockBuffer();
		lv_retval = WriteBufferOnDisk((const char *)(m_diskdata.GetBuffer()), lv_buflen, f_bClose);
		m_diskdata.UnLockBuffer();
		m_diskdata.Remove();
	}

	return lv_retval;
}

BOOL CCtrlWaveIn::StartRecord()
{
	BOOL lv_retval;

	m_dwSilenceTick = GetTickCount();
	m_bIsSilence = FALSE;
	lv_retval = CWaveInDevice::StartRecord();
	
	if( lv_retval )
		m_tmRecStart = CTime::GetCurrentTime();

	return lv_retval;
}

BOOL CCtrlWaveIn::StopRecord()
{
	m_tmRecEnd = CTime::GetCurrentTime();
	BOOL lv_retval = CWaveInDevice::StopRecord();

	WriteRecordData(TRUE);

	return lv_retval;
}

CTime CCtrlWaveIn::GetRecordStartTime()
{
	CTime lv_Value;

	lv_Value = m_tmRecStart;

	return lv_Value;
}

CTime CCtrlWaveIn::GetRecordEndTime()
{
	CTime lv_Value;

	lv_Value = m_tmRecEnd;

	return lv_Value;
}

DWORD CCtrlWaveIn::GetRecordLength()
{
	CTimeSpan lv_Dur;
	DWORD lv_Value;

	if( IsRecording() )
	{
		lv_Dur = CTime::GetCurrentTime() - m_tmRecStart;
		lv_Value = lv_Dur.GetTotalSeconds();
	}
	else
	{
		lv_Dur = m_tmRecEnd - m_tmRecStart;
		lv_Value = lv_Dur.GetTotalSeconds();
	}

	return lv_Value;
}

void CCtrlWaveIn::GetData(char *pBuffer, int iLen)
{
	long lv_applen;

	// Silence ?
	BOOL lv_bSilence = IsSilenceFrame(pBuffer, iLen);

	// Set Silence
	SetIsSilence(lv_bSilence);

	// Store Data
	lv_applen = m_diskdata.Append((const BYTE *)pBuffer, iLen);
	if( lv_applen < 0 )
	{
		// Buffer is full
		/// Notice to write down a record
		WriteRecordData(FALSE);

		m_diskdata.Append((const BYTE *)pBuffer, iLen);
	}

	CWaveInDevice::GetData(pBuffer,iLen);	
}

BOOL CCtrlWaveIn::GetIsSilence()
{
	BOOL lv_Value;

	m_bLock.Enter();
	lv_Value = m_bIsSilence;
	m_bLock.Leave();

	return lv_Value;
}

void CCtrlWaveIn::SetIsSilence(BOOL f_Value)
{
	m_bLock.Enter();
	if( m_bIsSilence != f_Value )
	{
		m_bIsSilence = f_Value;
		if( f_Value )
			m_dwSilenceTick = GetTickCount();

		TRACE("Debug Info - WaveInDev:%d input status changed to %d\n", m_dwWaveInID, !f_Value);
	}
	m_bLock.Leave();
}

DWORD CCtrlWaveIn::GetSilenceLength()
{
	DWORD lv_Value = 0;

	m_bLock.Enter();
	if( m_bIsSilence )
		lv_Value = GetTickCount() - m_dwSilenceTick;
	m_bLock.Leave();

	return lv_Value;
}

int CCtrlWaveIn::GetActiveThreshold()
{
	int lv_Value;

	m_bLock.Enter();
	lv_Value = m_nActiveThreshold;
	m_bLock.Leave();

	return lv_Value;
}

void CCtrlWaveIn::SetActiveThreshold(int f_Value)
{
	m_bLock.Enter();
	m_nActiveThreshold = f_Value;
	m_bLock.Leave();
}

int CCtrlWaveIn::GetMaxSilenceTime()
{
	int lv_Value;

	m_bLock.Enter();
	lv_Value = m_nMaxSilenceTime;
	m_bLock.Leave();

	return lv_Value;
}

void CCtrlWaveIn::SetMaxSilenceTime(int f_Value)
{
	m_bLock.Enter();
	m_nMaxSilenceTime = f_Value;
	m_bLock.Leave();
}


BOOL CCtrlWaveIn::WriteRecordFile(const char *f_pBuffer, long f_nLen, BOOL f_bClose)
{
	CString f_sName = GetRecordFileName();

	TRY
	{
		// Open File
		if( m_hRecordFile == INVALID_HANDLE_VALUE )
		{
			m_hRecordFile = CreateFile(f_sName,
					 GENERIC_WRITE, FILE_SHARE_READ,
					 NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if( m_hRecordFile == INVALID_HANDLE_VALUE )
				return FALSE;
			m_dwFileSize = 0;
		}
		CFile f(m_hRecordFile);

		// Write File Header
		if( m_dwFileSize == 0 )
		{
			strncpy(m_head.cFlagFiff, "RIFF", 4);

			m_dwFileSize = f_nLen + 36;
			m_head.iFileLen = m_dwFileSize;
			strncpy(m_head.cFlagWave, "WAVE", 4);
			strncpy(m_head.cFlagFmt, "fmt ", 4);
			m_head.cResove[0] = 0x10;
			m_head.cResove[1] = 0x0;
			m_head.cResove[2] = 0x0;
			m_head.cResove[3] = 0x0;
			m_head.cFormat = m_pcmWaveFormat.wFormatTag;
			m_head.cChannel = m_pcmWaveFormat.nChannels;
			m_head.cSample = m_pcmWaveFormat.nSamplesPerSec;
			m_head.cBytePerSec = m_pcmWaveFormat.nAvgBytesPerSec;
			m_head.cByteprocess = m_pcmWaveFormat.nBlockAlign;
			m_head.cBit = m_pcmWaveFormat.wBitsPerSample;
			strncpy(m_head.cFlagDat, "data", 4);
			m_head.iLenData = f_nLen;
			f.Write((void *)&m_head, sizeof(m_head));
		}
		else
			m_dwFileSize += f_nLen;

		// Write Data
		if( f_nLen > 0 )
			f.Write(f_pBuffer, f_nLen);

		// Close File
		if( f_bClose )
		{
			long lv_samples = m_pcmWaveFormat.nAvgBytesPerSec;
			if( m_dwFileSize / lv_samples < GetMinDiskBuffer() )
			{
				f.Abort();
				TRACE("Discard a invalid length:%d record.\n", m_dwFileSize);
				m_hRecordFile = INVALID_HANDLE_VALUE;
				m_dwFileSize = 0;
				f.Remove(f_sName);
				return FALSE;
			}
			
			/// Rewrite Length
			if( m_dwFileSize != f_nLen + 36 )
			{
				m_head.iFileLen = m_dwFileSize;
				m_head.iLenData = m_dwFileSize - 36;
				f.SeekToBegin();
				f.Write((void *)&m_head, sizeof(m_head));
			}

			// Close File
			f.Close();
			TRACE("Record a file length:%d.\n", m_dwFileSize);
			m_hRecordFile = INVALID_HANDLE_VALUE;
			m_dwFileSize = 0;
		}
	}
	CATCH( CFileException, e )
	{
		if( m_hRecordFile != INVALID_HANDLE_VALUE )
		{
			CloseHandle(m_hRecordFile);
			m_hRecordFile = INVALID_HANDLE_VALUE;
			TRACE("Write file:%s error!!!\n", f_sName);
		}
		THROW_LAST();
		return FALSE;
	}
	END_CATCH

	return TRUE;
}

BOOL CCtrlWaveIn::WriteBufferOnDisk(const char *pBuffer, long iLen, BOOL f_bClose)
{
	// File path
	long lv_samples;
	long lv_reclen;

	// Get Sample Rate
	lv_samples = m_pcmWaveFormat.nAvgBytesPerSec;
	
	// Get Record Length
	lv_reclen = iLen;
	if( m_hRecordFile == INVALID_HANDLE_VALUE )
	{
		if( f_bClose && ( lv_reclen / lv_samples < GetMinDiskBuffer() ) )
		{
			TRACE("Discard a record header length:%d record.\n", iLen);
			return FALSE;
		}
	}

	// Write File
	return( WriteRecordFile(pBuffer, lv_reclen, f_bClose) );
}

#endif