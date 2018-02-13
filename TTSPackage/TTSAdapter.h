// TTSAdapter.h: interface for the CTTSAdapter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TTSAdapter_H__E8C44467_06F6_4C21_9000_49E686740F2F__INCLUDED_)
#define AFX_TTSAdapter_H__E8C44467_06F6_4C21_9000_49E686740F2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "..\..\CommonClass\MyThread.h"

#include "InterPhonic\Inc\iFly_TTS.h"

// ��������
#define DEF_TTS_CST_BUFFER_LOCKTIMEOUT			15
#define DEF_TTS_CST_MAX_CONNECTION				256
#define DEF_TTS_CST_MAX_BUFFERS					1024

class CTTSServerInfo
{
public:
	CTTSServerInfo();
	virtual ~CTTSServerInfo();

	const CTTSServerInfo& CTTSServerInfo::operator = (const CTTSServerInfo& ServerInfoScr);

public:
	CString m_strCompanyName;							// ��˾����
	CString m_strUserName;								// �û���
	CString m_strProductName;							// ��Ʒ����
	CString m_strSerialNO;								// ��Ʒ���к�
	CString m_strServerIP;								// ������IP
	UINT m_nServerPort;									// �������˿�
	SHORT m_nCodePage;									// Default chinese codepage type
	SHORT m_nVID;										// Default TTS Voice Lib
	SHORT m_nAudioFmt;									// Default audio data format
	SHORT m_nSpeed;										// Default speed
	SHORT m_nVolume;									// Default volume
	SHORT m_nPitch;										// Default pitch
	SHORT m_nAudioHeadFmt;								// Default audio head fmt
	SHORT m_nTextType;									// Default text type
};

class CTTSAdapter  
{
public:
	CTTSAdapter();
	virtual ~CTTSAdapter();

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

	class CTTSConnection : public CMyThread
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
		
		BOOL CheckDeadLock();
		
	protected:
		CManualResetEvent m_shutdown;

		void WaitForStartupToComplete();
		void WaitForShutdownToComplete();

		virtual int Run();

		class CTTSBuffer
		{
		public:
			CTTSBuffer(int f_nIndex, UINT f_nBufferSize);
			virtual ~CTTSBuffer();
			int GetBufferIndex();

			int GetBufferData(BYTE *f_Data);
			BOOL SetBufferData(const BYTE *f_Data, int f_Len, int f_Tag = 0);
			long GetLockTick();
			int GetTag();
			void ResetTag();

			void ClearBuffer();

		protected:
			void		CreateDataBuffer(long f_nlen);

		private:
			int			m_nIndex;
			int			m_Tag;
			long		m_lngLockTick;
			BYTE		*m_pBuffer;
			long		m_length;
			long		m_maxlen;
			CManulSync	m_sync;
		};

		BOOL SetSynthParameters();

	private:
		int OutputSetParamError(LPCSTR f_szParaName, UINT f_nParaValue, UINT f_nErrorCode);
		void RemoveAllBuffers();
		int GetSynthFlag();

		int m_nOwner;
		UINT m_nWriteBufferPtr;
		TTSData *m_pTtsData;
		CTTSBuffer *m_BufferList[DEF_TTS_CST_MAX_BUFFERS];
		UINT m_nIndex;
		UINT m_nBufferCount;
		UINT m_nBufferSize;
		HANDLE m_hTTSInstance;
		CTTSServerInfo m_ServerInfo;
		CManulSync		m_bLock;
	};

	CTTSConnection *GetAvailableTTSConnection(int f_nOwner = 0, UINT f_nTimeOut = 0);

protected:
	void RemoveAllTTSConnections();
	CTTSConnection *FindTTSConnection(UINT f_nIndex);

private:
	BOOL			m_blnTTSEnabled;					// TTS�ӿ��Ƿ��ʼ������
	BOOL			m_blnTTSStarted;					// TTS�ӿ��Ƿ�����
	UINT			m_nMaxBuffers;						// ÿ���������ö��ٸ�����ת������
	UINT			m_nBufferSize;						// ÿ������ת������Ĵ�С(KB)
	UINT			m_nConnCount;
	CTTSConnection	*m_ConnList[DEF_TTS_CST_MAX_CONNECTION];
	CManulSync		m_bLock;
};

#endif // !defined(AFX_TTSAdapter_H__E8C44467_06F6_4C21_9000_49E686740F2F__INCLUDED_)
