#pragma once

#define WIOSA_POOR					8000
#define WIOSA_LOW					11025
#define WIOSA_NORMAL				22050
#define WIOSA_HIGH					44100

#define PLAY_DELAY					10
#define NUM_BUF						10

#define SIZE_AUDIO_FRAME			3200

#pragma pack(push, 1)
struct WaveFileHeader
{
	char cFlagFiff[4];
	unsigned __int32 iFileLen;
	char cFlagWave[4];
	char cFlagFmt[4];
	char cResove[4];
	unsigned __int16 cFormat;
	unsigned __int16 cChannel;
	unsigned __int32 cSample;
	unsigned __int32 cBytePerSec;
	unsigned __int16 cByteprocess;
	unsigned __int16 cBit;
	char cFlagDat[4];
	unsigned __int32 iLenData;
};

#pragma pack(pop)

#ifdef CISCO_CCM

#include <mmsystem.h>
#include <afxmt.h>

class CWaveDevice
{
public:
	CWaveDevice(void);
	virtual ~CWaveDevice(void);

	int GetBufferNum();
	virtual void BufferSub();
	virtual void BufferAdd();

	DWORD GetInstance();
	WORD GetFormat();
	WORD GetBit();
	DWORD GetSample();
	WORD GetChannel();

	void SetFormat(WORD wFormat);
	void SetBit(WORD wBit);
	void SetSample(DWORD dwSample);
	void SetChannel(WORD wChannel);

	// Sun added 2011-04-16
	long GetIVRChannelID();
	void SetIVRChannelID(long nIVRChannelID);

	// Sun added 2011-06-24
	void SetOpenDevWait();
	void ResetOpenDevWait();
	DWORD TestOpenDevWait();

	// Sun added 2011-06-24
	void SetCloseDevWait();
	void ResetCloseDevWait();
	DWORD TestCloseDevWait();

	inline MMRESULT GetLastMMError();
	CString GetLastErrorString();

protected:
	virtual BOOL OpenDev() = 0;
	virtual BOOL CloseDev() = 0;
	virtual int Run() = 0;

	BOOL StopThread();
	BOOL StartThread();

protected:
	WAVEFORMATEX m_pcmWaveFormat;
	void BuildFormat();

	CCriticalSection m_bLock;

protected:
	MMRESULT m_mmr;
	DWORD m_dwAudioDevId;		// 线程ID
	HANDLE m_hAudioDev;			// 线程Handle

	// Sun added 2013-04-12
#ifdef CUCM_MEDIA
	HANDLE m_hMediaStream;			// Media Stream Handle
#endif

	BOOL m_bAllocBuffer;
	int m_iBufferNum;
	CCriticalSection m_csLock;

	static DWORD s_dwInstance;

	BOOL m_bThreadStart;
	BOOL m_bDevOpen;

	// Sun added 2011-04-16
	long m_lngIVRChannelID;

	// Sun added 2011-06-24
	DWORD m_dwOpenTick;
	BOOL m_blnOpenWait;

	// Sun added 2011-06-24
	DWORD m_dwCloseTick;
	BOOL m_blnCloseWait;

private :
	int ToRunThread();
	
	static DWORD WINAPI AudioIOThreadProc(LPVOID lpParameter);
};

class CWaveOutDevice : public CWaveDevice
{
public:
	CWaveOutDevice(void);
	virtual ~CWaveOutDevice(void);

	virtual void BufferSub();
	virtual void BufferAdd();

	BOOL Play(char* buf, UINT uSize);

	int StartPlay();
	BOOL StopPlay();
	BOOL IsPlaying();

	// Sun added 2013-04-12
	void WaveoutDone(DWORD hHandle, PVOID pData);
	void RecycleWaveBuffer(DWORD hHandle);

protected:
	DWORD m_dwWaveOutID;
	HWAVEOUT m_hOut;
	BOOL m_bPlaying;

	// Sun added 2013-04-12
#ifdef CUCM_MEDIA
	HANDLE m_hMediaEpOut;			// Endpoint Handle
#endif

	BOOL Continue();
	BOOL Pause();
	BOOL Stop();

	virtual BOOL OpenDev();
	virtual BOOL CloseDev();
	virtual int Run();

	// Sun added 2013-04-12
	LPWAVEHDR m_pWavData[NUM_BUF+1];
	int m_nPrtHead;
};

class CWaveInDevice : public CWaveDevice
{
public:
	CWaveInDevice(void);
	virtual ~CWaveInDevice(void);

	virtual void GetData(char *pBuffer,int iLen);

	BOOL StartRecord();
	BOOL StopRecord();
	BOOL IsRecording();

	// Sun added 2013-04-12
	void WaveinData(DWORD hHandle, PVOID pData);

protected:
	DWORD m_dwWaveInID;
	HWAVEIN m_hIn;
	BOOL m_bRecording;

	// Sun added 2013-04-12
#ifdef CUCM_MEDIA
	HANDLE m_hMediaEpIn;			// Endpoint Handle
#endif

	WAVEHDR* m_pHdr;

	BOOL PerPareBuffer();
	BOOL FreeBuffer();

	BOOL OpenRecord();
	BOOL CloseRecord();

	virtual BOOL OpenDev();
	virtual BOOL CloseDev();
	virtual int Run();
};

#endif