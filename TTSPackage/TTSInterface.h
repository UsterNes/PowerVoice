// TTSInterface.h: interface for the CTTSInterface class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TTSINTERFACE_H__E8C44467_06F6_4C21_9000_49E686740F2F__INCLUDED_)
#define AFX_TTSINTERFACE_H__E8C44467_06F6_4C21_9000_49E686740F2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

// 条件编译：使用iFly TTS（科大讯飞） 还是 SinoVoice jTTS（捷通华声）
/// 如果使用iFly TTS（科大讯飞），则将下面一行注释掉。
#define USE_SINOVOICE_TTS

// 常量定义
#define DEF_TTS_CST_MAX_CONNECTION				256
#define DEF_TTS_CST_MAX_BUFFERS					1024
#define DEF_TTS_CST_MAX_INBUF_SIZE				4096

#ifdef CISCO_CCM
// CISCO CCM使用8K, 16Bit, 1Channel WAVE，所以单个Buffer的超时时间短
#define DEF_TTS_CST_BUFFER_LOCKTIMEOUT			4
#else
#define DEF_TTS_CST_BUFFER_LOCKTIMEOUT			8
#endif

// 返回值定义
#define DEF_TTS_EC_MOREDATA				1
#define DEF_TTS_EC_SUCC					0
#define DEF_TTS_EC_COMPLETE				0
#define DEF_TTS_EC_SYSERROR				-1
#define DEF_TTS_EC_NORESOURCE			-2
#define DEF_TTS_EC_NOTSTARTED			-3
#define DEF_TTS_EC_INVALIDHANDLE		-4
#define DEF_TTS_EC_NOTEXIST				-5

class CTTSServerInfo
{
public:
	CTTSServerInfo();
	virtual ~CTTSServerInfo();

	const CTTSServerInfo& CTTSServerInfo::operator = (const CTTSServerInfo& ServerInfoScr);

public:
	CString m_strCompanyName;							// 公司名称
	CString m_strUserName;								// 用户名
	CString m_strProductName;							// 产品名称
	CString m_strSerialNO;								// 产品序列号
	CString m_strServerIP;								// 服务器IP
	UINT m_nServerPort;									// 服务器端口
	SHORT m_nCodePage;									// Default chinese codepage type
	SHORT m_nVID;										// Default TTS Voice Lib
	SHORT m_nAudioFmt;									// Default audio data format
	SHORT m_nSpeed;										// Default speed
	SHORT m_nVolume;									// Default volume
	SHORT m_nPitch;										// Default pitch
	SHORT m_nAudioHeadFmt;								// Default audio head fmt
	SHORT m_nTextType;									// Default text type

	// Sun added 2007-10-20
	/// 背景音乐
	SHORT m_nBackAudioIndex;							// 背景音乐编号，0表示没有
	SHORT m_nBackAudioVolume;							// 背景音乐音量
	SHORT m_nBackAudioFlag;								// 背景音乐属性：是否允许重复播放
};

class CTTSInterface  
{
public:
	CTTSInterface();
	virtual ~CTTSInterface();

	BOOL InitializeTTS(UINT f_nConnections, UINT f_nBuffers = 2, UINT f_nBufferSize = 64);
	BOOL UninitializeTTS();
	BOOL IsTTSEnabled();

	BOOL StartupTTS(CTTSServerInfo f_Server);
	BOOL StopTTS();
	BOOL IsTTSStarted();
	
	UINT GetMaxTTSConnections();
	UINT GetMaxTTSBuffers();
	UINT GetTTSBufferSize();

	void CheckTTSConnections();

	int TTSSynthesizeText(int *f_nStartBuffer, int f_nChannelID, LPSTR f_pTextData, int f_nDataLen, UINT f_nTimeOut = 5);
	int TTSIsPlayBufferOK(int f_nChannelID, int f_nConnectionIndex, int f_nBufferID, BYTE *f_pVoiceData, int *f_nDataLen);
	int TTSStopSynthesize(int f_nChannelID, int f_nConnectionIndex);

	class CTTSConnection
	{
	public:
		CTTSConnection(UINT f_nIndex, UINT f_nBuffers, UINT f_nBufferSize);
		virtual ~CTTSConnection();
		UINT GetIndex();

		BOOL ConnectServer(CTTSServerInfo f_Server);
		BOOL DisconnectServer();
		BOOL IsConnected();
		int IsTaken();
		void SetUsed(int f_nOwner = 0);

		int ExecuteSynthesize(LPSTR f_pTextData, int f_nDataLen);
		BOOL ReceiveTTSData(const BYTE *f_pVoiceData, UINT f_nDataLen);
		BOOL FetchVoiceData(int f_nChannelID, int f_nBufferID, BYTE *f_pVoiceData, int *f_nDataLen);
		
		BOOL GetCancelFlag();
		void SetCancelFlag(BOOL f_blnFlag);

		void ReleaseLockedBuffers(int f_nChannelID = 0);

		BOOL CheckDeadLock();
		BOOL CheckMaxIdleDuration();
		
	protected:
		class CTTSBuffer
		{
		public:
			CTTSBuffer(int f_nIndex, UINT f_nBufferSize);
			virtual ~CTTSBuffer();
			int GetBufferIndex();

			int GetBufferData(BYTE *f_Data);
			BOOL SetBufferData(const BYTE *f_Data, int f_Len, int f_Tag);
			DWORD GetLockTick();
			int GetTag();
			void ResetTag();

			void ClearBuffer();

		protected:
			void		CreateDataBuffer(long f_nlen);

		private:
			int			m_nIndex;			// 0 Based
			int			m_Tag;
			DWORD		m_dwLockTick;
			BYTE		*m_pBuffer;
			long		m_length;
			long		m_maxlen;
			CManulSync	m_sync;
		};

		BOOL SetSynthParameters();

	private:
		int OutputSetParamError(LPCSTR f_szParaName, UINT f_nParaValue, UINT f_nErrorCode);
		void RemoveAllBuffers();

		int m_nOwner;
		UINT m_nWriteBufferPtr;
		CTTSBuffer *m_BufferList[DEF_TTS_CST_MAX_BUFFERS];
		BOOL m_blnCancel;
		UINT m_nIndex;
		UINT m_nBufferCount;
		UINT m_nBufferSize;
		HANDLE m_hTTSInstance;
		CTTSServerInfo m_ServerInfo;
		CManulSync		m_bLock;

		DWORD		m_dwIdleTick;		// Sun added 2008-11-16

		// Sun added 2007-10-20, for closing jTTS Session
		DWORD m_nCurrentSessionID;
	};

protected:
	void RemoveAllTTSConnections();
	CTTSConnection *FindTTSConnection(UINT f_nIndex);
	CTTSConnection *GetAvailableTTSConnection(int f_nOwner = 0);

private:
	CTTSServerInfo	m_TTSServerInfo;					// Sun added 2007-10-28

	BOOL			m_blnTTSEnabled;					// TTS接口是否初始化正常
	BOOL			m_blnTTSStarted;					// TTS接口是否启动
	UINT			m_nMaxBuffers;						// 每个连接设置多少个语音转换缓冲
	UINT			m_nBufferSize;						// 每个语音转换缓冲的大小(KB)
	UINT			m_nConnCount;
	CTTSConnection	*m_ConnList[DEF_TTS_CST_MAX_CONNECTION];
	CManulSync		m_bLock;
};

#endif // !defined(AFX_TTSINTERFACE_H__E8C44467_06F6_4C21_9000_49E686740F2F__INCLUDED_)
