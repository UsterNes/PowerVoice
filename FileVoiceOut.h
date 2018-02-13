// FileVoiceOut.h: interface for the CFileVoiceOut class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEVOICEOUT_H__92C794CF_2549_4F51_970C_59B47E8536A2__INCLUDED_)
#define AFX_FILEVOICEOUT_H__92C794CF_2549_4F51_970C_59B47E8536A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef CISCO_CCM

#include ".\wavedevice.h"

class CFileVoiceOut : public CWaveOutDevice
{
public:
	CFileVoiceOut();
	virtual ~CFileVoiceOut();

	virtual void BufferSub();
	virtual void BufferAdd();

	CString GetFilePath();
	void SetFilePath(CString sFilePath);

	BOOL OpenFile();
	BOOL OpenFile(CString sFilePath);
	BOOL CloseFile();
	BOOL IsFileOpen();
	BOOL SetFilePostion(int nPos = 0);

	BOOL PlayFile(int nStart = -1, BOOL blnLoopPlay = FALSE);
	BOOL PlayFile(CString sFilePath, BOOL blnLoopPlay = FALSE);
	BOOL IsFilePlaying();
	BOOL PlayBuffer(BYTE *pData, UINT nSize);
	BOOL StopPlay();
	BOOL PausePlay();
	BOOL ContinuePlay();

	long GetFileBytes();
	long GetFileLength();

	long GetPlayedBytes();
	long GetPlayedLength();
	
protected:
	void SetIsFilePlaying(BOOL f_Value);
	void SetIsFileOpen(BOOL f_Value);
	CString m_sFilePath;
	CFile m_recFile;
	BOOL m_bFileOpen;
	BOOL m_bFilePlaying;
	long m_lngRecordBytes;
	long m_lngPlayedBytes;
	UINT m_nBytesPerSecond;
	UINT m_nBytesPerFrame;
	int m_nFramesPerGroup;

	// Sun added 2013-09-16
	long m_nFileReadLength;

	BOOL SetFormatByFile();
	BOOL PlayNextGroup();

	BOOL m_blnLoopPlay;
};

#endif

#endif // !defined(AFX_FILEVOICEOUT_H__92C794CF_2549_4F51_970C_59B47E8536A2__INCLUDED_)