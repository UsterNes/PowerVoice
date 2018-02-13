// FileVoiceOut.cpp: implementation of the CFileVoiceOut class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "FileVoiceOut.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifdef CISCO_CCM

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileVoiceOut::CFileVoiceOut()
{
	m_sFilePath = _T("");
	m_bFileOpen = FALSE;
	m_bFilePlaying = FALSE;
	m_lngRecordBytes = 0;
	m_lngPlayedBytes = 0;
	m_nBytesPerSecond = 0;
	m_nBytesPerFrame = 0;
	m_nFramesPerGroup = 0;

	// Sun added 2013-09-16
	m_nFileReadLength = 0;

	// Sun added 2008-04-02
	m_blnLoopPlay = FALSE;
}

CFileVoiceOut::~CFileVoiceOut()
{
	StopPlay();
	Sleep(10);
	if( m_bFileOpen )
		CloseFile();
}

void CFileVoiceOut::BufferAdd()
{
	CWaveOutDevice::BufferAdd();
}

void CFileVoiceOut::BufferSub()
{
	// Play next group of frames
	m_lngPlayedBytes += m_nBytesPerFrame;

	if( IsPlaying() )
	{
		// Sun updated 2008-04-02
		if( m_lngPlayedBytes < m_lngRecordBytes || m_blnLoopPlay )
		{
			if( GetBufferNum() < PLAY_DELAY - m_nFramesPerGroup )
				if( !PlayNextGroup() )
					m_bFilePlaying = FALSE;
		}
		else
			m_bFilePlaying = FALSE;
	}

	CWaveOutDevice::BufferSub();

	//TRACE("Played %d of %d\n", m_lngPlayedBytes, m_lngRecordBytes);
}

CString CFileVoiceOut::GetFilePath()
{
	return m_sFilePath;
}

void CFileVoiceOut::SetFilePath(CString sFilePath)
{
	m_sFilePath = sFilePath;
}

BOOL CFileVoiceOut::IsFileOpen()
{
	return m_bFileOpen;
}

BOOL CFileVoiceOut::OpenFile()
{
	if( m_sFilePath == _T("") )
		return FALSE;

	// Close Old File
	if( m_bFileOpen )
	{
		CloseFile();

		// Sun added 2011-05-13, 保证设备关闭时间
		Sleep(50);
	}

	// Open File
	CFileException ex;
	try
	{
		m_lngPlayedBytes = 0;
		m_lngRecordBytes = 0;

		// Sun added 2013-09-16
		m_nFileReadLength = 0;

		if( m_recFile.Open(m_sFilePath, CFile::modeRead | CFile::shareDenyWrite, &ex) )
		{
			m_bFileOpen = TRUE;

			// Sun updated 2008-06-19, 如果打开文件头失败，则关闭文件
			if( !SetFormatByFile() )
				CloseFile();
		}
		else
		{
			TCHAR szError[1024];
			ex.GetErrorMessage(szError, 1024);
			WriteTrace(TraceError, "CFileVoiceOut::OpenFile() - Open file: %s error:%s", m_sFilePath, szError);
		}
		
	}
	catch(...)
	{
		WriteTrace(TraceError, "SysError in CFileVoiceOut::OpenFile() - Open file: %s error:%s", m_sFilePath);
	}

	return( m_bFileOpen );
}

BOOL CFileVoiceOut::OpenFile(CString sFilePath)
{
	SetFilePath(sFilePath);
	return OpenFile();
}

BOOL CFileVoiceOut::CloseFile()
{
	StopPlay();

	try
	{
		if( m_bFileOpen )
			m_recFile.Close();
	}
	catch(...)
	{
		return FALSE;
	}

	m_bFileOpen = FALSE;
	return TRUE;
}

BOOL CFileVoiceOut::SetFilePostion(int nPos /* 0 */)
{
	try
	{
		if( nPos >= 0 )
		{
			m_lngPlayedBytes = nPos * m_nBytesPerSecond;
			m_recFile.Seek(m_lngPlayedBytes + sizeof(WaveFileHeader), CFile::begin);

			// Sun added 2013-09-16
			m_nFileReadLength = m_lngPlayedBytes;
		}
	}
	catch(...)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CFileVoiceOut::IsFilePlaying()
{
	return( m_bFilePlaying );
}

BOOL CFileVoiceOut::PlayFile(int nStart /*=-1*/, BOOL blnLoopPlay /*=FALSE*/)
{
	if( !IsFileOpen() )
	{
		WriteTrace(TraceInfo, "Can't CFileVoiceOut::PlayFile() because file: %s is not opend", m_sFilePath);
		return FALSE;
	}

	if( IsFilePlaying() )
	{
		WriteTrace(TraceInfo, "Can't CFileVoiceOut::PlayFile() because file: %s is playing", m_sFilePath);
		return FALSE;
	}

	if( !m_bDevOpen )
	{
		//WriteTrace(TraceDetail, "Tony Debug 2010-07-18: Play 1-1");

		// Sun added 2010-04-06
		int lv_nPlayRet;
		lv_nPlayRet = StartPlay();
		if( lv_nPlayRet < 0 )
		{
			WriteTrace(TraceWarn, "Warn - CFileVoiceOut::PlayFile() failed to startplay, error = %d!!!", 
				lv_nPlayRet);

			return FALSE;
		}

		//WriteTrace(TraceDetail, "Tony Debug 2010-07-18: Play 1-2");
	}
	else
		ContinuePlay();

	try
	{
		if( nStart >= 0 )
		{
			m_lngPlayedBytes = nStart * m_nBytesPerSecond;
			m_recFile.Seek(m_lngPlayedBytes + sizeof(WaveFileHeader), CFile::begin);

			// Sun added 2013-09-16
			m_nFileReadLength = m_lngPlayedBytes;
		}
	}
	catch(...)
	{
		WriteTrace(TraceError, "File Error - failed to move file: %s pointer: %d bytes", m_sFilePath, m_lngPlayedBytes);
		return FALSE;
	}

	// Sun added 2008-04-02
	m_blnLoopPlay = blnLoopPlay;

	m_bFilePlaying = TRUE;
	return PlayNextGroup();
}

BOOL CFileVoiceOut::PlayFile(CString sFilePath, BOOL blnLoopPlay)
{
	if( !OpenFile(sFilePath) )
		return FALSE;

	return PlayFile(-1, blnLoopPlay);
}

BOOL CFileVoiceOut::PlayBuffer(BYTE *pData, UINT nSize)
{
	if( nSize <= 0 || pData == NULL )
	{
		WriteTrace(TraceWarn, "CFileVoiceOut::PlayBuffer() - no data to play!");
		return FALSE;
	}

	if( m_bFilePlaying )
	{
		WriteTrace(TraceWarn, "Should stop file playing before calls CFileVoiceOut::PlayBuffer()!");
		return FALSE;
	}

	m_bFilePlaying = 0;
	m_lngRecordBytes = nSize;
	m_nBytesPerFrame = nSize;

	return CWaveOutDevice::Play((char *)pData, nSize);
}

BOOL CFileVoiceOut::PlayNextGroup()
{
	long lv_loop;
	char lv_pBuff[SIZE_AUDIO_FRAME];
	UINT lv_len;
	BOOL retval = TRUE;

	if( !IsPlaying() )
	{
		WriteTrace(TraceInfo, "Can't CFileVoiceOut::PlayNextGroup() because file: %s is not playing", m_sFilePath);
		return FALSE;
	}

	try
	{
		//------------------------------------
		// Sun updated 2008-04-02
		/// From
		/*
		for( lv_loop = 0; lv_loop < m_nFramesPerGroup; lv_loop++)
		{
			lv_len = m_recFile.Read(lv_pBuff, m_nBytesPerFrame);
			if( lv_len == 0 )
				break;
			retval = CWaveOutDevice::Play(lv_pBuff, lv_len);
			if( !retval || lv_len < m_nBytesPerFrame )
				break;
		}
		*/
		/// To
		for( lv_loop = 0; lv_loop < m_nFramesPerGroup; lv_loop++)
		{
			lv_len = m_recFile.Read(lv_pBuff, m_nBytesPerFrame);

			// Sun added 2013-09-16
			if( m_nFileReadLength + lv_len > m_lngRecordBytes )
				lv_len = m_lngRecordBytes - m_nFileReadLength;
			m_nFileReadLength+=lv_len;

			if( lv_len < m_nBytesPerFrame && m_blnLoopPlay )
			{
				// Sun added 2013-09-16
				m_nFileReadLength = m_nBytesPerFrame-lv_len;

				m_recFile.SeekToBegin();
				lv_len += m_recFile.Read(lv_pBuff+lv_len, m_nFileReadLength);
			}
			if( lv_len == 0 )
				break;

			retval = CWaveOutDevice::Play(lv_pBuff, lv_len);
			if( !retval || lv_len < m_nBytesPerFrame )
				break;
		}
		// End of Sun updated 2008-04-02
		//------------------------------------
	}
	catch(...)
	{
		WriteTrace(TraceError, "File Error - failed to read from file: %s!", m_sFilePath);
		return FALSE;
	}
	
	//TRACE("Play a group of record data:%d\n", lv_loop);
	return retval;
}

BOOL CFileVoiceOut::StopPlay()
{
	m_bFilePlaying = FALSE;

	// Sun updated 2010-07-20
	/// From
	/*
	if( m_bDevOpen )
		return CWaveOutDevice::StopPlay();
	*/
	/// To
	return CWaveOutDevice::StopPlay();

	return TRUE;
}

BOOL CFileVoiceOut::PausePlay()
{
	m_bFilePlaying = FALSE;
	return CWaveOutDevice::Pause();
}

BOOL CFileVoiceOut::ContinuePlay()
{
	m_bFilePlaying = TRUE;
	return CWaveOutDevice::Continue();
}

BOOL CFileVoiceOut::SetFormatByFile()
{
	struct WaveFileHeader head;

	m_recFile.SeekToBegin();
	if(m_recFile.Read((void *)&head, sizeof(head))==0)//add by steven 2003-7-16
	{
		WriteTrace(TraceError, "CFileVoiceOut::SetFormatByFile() - can't read file header!");
		return FALSE;
	}

	// Sun added 2013-04-12
	SetFormat(head.cFormat);
	SetBit(head.cBit);

	SetChannel(head.cChannel);
	SetSample(head.cSample);

	BuildFormat();

	m_nBytesPerFrame = SIZE_AUDIO_FRAME;
	m_nBytesPerSecond = m_pcmWaveFormat.nAvgBytesPerSec;

	// Sun undated 2013-09-16
	/// From
	//m_lngRecordBytes = head.iFileLen - 36;
	/// To
	m_lngRecordBytes = head.iLenData;
	
	// 0.25 s per group
	//m_nFramesPerGroup = m_pcmWaveFormat.nAvgBytesPerSec / SIZE_AUDIO_FRAME / 4;
	m_nFramesPerGroup = 4;

	// Sun added 2013-04-12
	/// 目前Media Driver只支持G.711 uLaw
#ifdef CUCM_MEDIA
	if( GetFormat() != WAVE_FORMAT_MULAW )	// = 7
	{
		WriteTrace(TraceError, "CFileVoiceOut::SetFormatByFile() - File format is %d, which is not supported by Media Driver so far! Please convert the voice file to G.711 uLaw (ID=7) format.", GetFormat());
		return FALSE;
	}
#endif

	return TRUE;
}

long CFileVoiceOut::GetFileBytes()
{
	if( !m_bFileOpen )
		return -1;

	return m_lngRecordBytes;
}

long CFileVoiceOut::GetFileLength()
{
	if( !m_bFileOpen )
		return -1;

	return( m_lngRecordBytes / m_nBytesPerSecond );
}

long CFileVoiceOut::GetPlayedBytes()
{
	if( !m_bFileOpen )
		return -1;

	return m_lngPlayedBytes;
}

long CFileVoiceOut::GetPlayedLength()
{
	if( !m_bFileOpen )
		return -1;

	return( m_lngPlayedBytes / m_nBytesPerSecond );
}

#endif