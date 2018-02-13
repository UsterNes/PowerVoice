// MyWaveIn.h: interface for the CCtrlWaveIn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYWAVEIN_H__D6D88534_ED10_4ADA_8340_03302FE5E649__INCLUDED_)
#define AFX_MYWAVEIN_H__D6D88534_ED10_4ADA_8340_03302FE5E649__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef CISCO_CCM

#include "..\CommonClass\_DataQueue.h"
#include ".\wavedevice.h"

#include <afxmt.h>

class CCtrlWaveIn : public CWaveInDevice
{
public:
	CCtrlWaveIn();
	virtual ~CCtrlWaveIn();

	BOOL GetIsSilence();
	void SetIsSilence(BOOL f_Value);
	DWORD GetSilenceLength();

	int GetActiveThreshold();
	void SetActiveThreshold(int f_Value);

	int GetAveFrameValue();
	void SetAveFrameValue(int f_Value);

	int GetMaxSilenceTime();
	void SetMaxSilenceTime(int f_Value);
	
	CString GetRecordFileName();
	void SetRecordFileName(CString strFileName);
	BOOL WriteRecordData(BOOL f_bClose);

	int GetMinDiskBuffer();
	void SetMinDiskBuffer(int f_Value);
	int GetMaxDiskBuffer();
	void SetMaxDiskBuffer(int f_Value);

	BOOL StartRecord();
	BOOL StopRecord();
	DWORD GetRecordLength();
	CTime GetRecordStartTime();
	CTime GetRecordEndTime();

	virtual void GetData(char *pBuffer, int iLen);

protected:
	BOOL IsSilenceFrame(const char *pBuffer, int iLen);
	BOOL WriteRecordFile(const char *f_pBuffer, long f_nLen, BOOL f_bClose);
	BOOL WriteBufferOnDisk(const char *pBuffer, long iLen, BOOL f_bClose);

	CDataQueue m_diskdata;
	int m_nMinDiskBuffer;
	int m_nMaxDiskBuffer;

	HANDLE m_hRecordFile;
	CString m_sRecordFileName;

	CTime m_tmRecStart;
	CTime m_tmRecEnd;

	BOOL m_bIsSilence;
	DWORD m_dwSilenceTick;
	int m_nActiveThreshold;
	int m_nMaxSilenceTime;
	int m_nAverageFrameValue;
	CManulSync m_bLock;

private:
	int m_dwFileSize;
	WaveFileHeader m_head;

};

#endif

#endif // !defined(AFX_MYWAVEIN_H__D6D88534_ED10_4ADA_8340_03302FE5E649__INCLUDED_)
