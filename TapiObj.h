#pragma once

#ifdef CISCO_CCM

#include "..\CommonClass\PCSCommunicationEvent.h"
#include ".\FileVoiceOut.h"
#include ".\CtrlWaveIn.h"

#define DEF_TAPI_MAX_CALLLIST				12
#define DEF_TAPI_MAX_TPTCNT					10
#define DEF_TAPI_MAX_KEYBUFFER				512

class CTapiObj
{
public:
	CTapiObj(void);
	virtual ~CTapiObj(void);

	// Exception class
	class TEx {
	public:
		TEx(int id, CString res, CString dt, CString tm);
		int		code;
		CString result;
		CString date;
		CString time;
	};

public:
	long Init(UINT nID);
	long Restart();

	virtual long Open();
	virtual long Close();
	virtual void ShutDown();
	virtual void ResetTapiCallData();
	virtual void DropAllCalls() = 0;
	virtual void DeallocateAllCalls() = 0;

	long ServiceSelfCheck();
	long SearchMeOnLost();

	BOOL IsInitialized();
	BOOL IsOpened();

	UINT GetLineID();
	DWORD GetLineHandle();
	DWORD GetCallHandle();
	void SetCallHandle(HCALL hNewCall);

	CString GetDeviceName();
	void SetDeviceName(LPCSTR lpName);

	DWORD GetDeviceID();
	void SetDeviceID(DWORD dwID);

	STATUS GetLineStatus();
	void SetLineStatus(STATUS f_stST, LPARAM f_Refer = 0);

protected:
	
	void ThrowErr(int id, CString res);

	CString m_appName;
	DWORD	m_version;
	DWORD	m_mediaMode;

	CString m_strDeviceName;
	DWORD	m_dwDeviceID;

protected:
	BOOL				m_bInitialized;
	BOOL				m_bOpened;

	UINT				m_ID;
	HLINE				m_hLine;
	HCALL				m_hCall;
	LINEEXTENSIONID		m_extID;

	LINEDEVCAPS*		m_pLineDevCaps;
	DWORD				m_dwApiVersion;

	// Line Status
	STATUS				m_stCurrentStatus;

	// Call Data
	CString				m_sANI;
	CString				m_sDNIS;
	CString				m_sCallerName;
	CString				m_sCalledName;

private:
	long GetDevCaps();
};

class CTapiRoutePoint : public CTapiObj
{
public:
	CTapiRoutePoint(void);
	virtual ~CTapiRoutePoint(void);

	virtual void ResetTapiCallData();
	virtual void DropAllCalls();
	virtual void DeallocateAllCalls();

public:
	long DropCall(DWORD dwCallHandle = 0);
	long DeallocateCall(DWORD dwCallHandle = 0);
	long RedirectCall(DWORD dwCallHandle, LPCSTR lpszDestAddress);

	int FindCallListMember(DWORD dwCallHandle);
	int SetCallListMember(DWORD dwCallHandle);
	int ClearCallListMember(DWORD dwCallHandle);

protected:
	DWORD m_hCallList[DEF_TAPI_MAX_CALLLIST];
	int m_nConcurrentCalls;				// Sun added 2010-07-31
};

class CTapiLine 
  : public CTapiObj, 
	public CFileVoiceOut,
	public CCtrlWaveIn
{
public:
	CTapiLine(void);
	virtual ~CTapiLine(void);

	long Init(UINT nID, DWORD dwDeviceID);

	virtual long Open();
	virtual long Close();		// Sun added 2013-04-12
	virtual void ShutDown();
	virtual void ResetTapiCallData();
	virtual void DropAllCalls();
	virtual void DeallocateAllCalls();
	
	BOOL EndPointOpen();	// Sun added 2013-04-12

	void SetRingCount(int nRingCnt);

	DWORD GetConsultCallHandle();
	void SetConsultCallHandle(HCALL hNewCall);
	DWORD GetConfCallHandle();
	void SetConfCallHandle(HCALL hNewCall);
	DWORD GetLineWavOutDevID();
	DWORD GetLineWavInDevID();

public:
	virtual void BufferSub();

public:
	long AcceptCall();
	long AnswerCall();
	long DropCall(DWORD dwCallHandle = 0);
	long DeallocateCall(DWORD dwCallHandle = 0);
	long MonitorDigits(DWORD dwCallHandle = 0);
	long MakeCall(LPCSTR lpszDigits, int nNoAnsTimeOut = 0);
	long SingleStepTransfer(LPCSTR lpszDigits);
	long ConsultTransfer(LPCSTR lpszDigits, int nNoAnsTimeOut = 0);
	long ConsultConference(LPCSTR lpszDigits, int nNoAnsTimeOut = 0);
	long StopConsultaion();
	long CompleteTransfer(BOOL blnTransfer = TRUE);
	long SendDTFM(LPCSTR lpszDigits = NULL);
	long SendBeep();
	DWORD PrintLineStatus(DWORD *f_pdwFeature = NULL);

	long EnumCallsOnLine();

	UL GetTerminateCode();
	void SetTerminateCode(UL lngTermCode);
    void ClearKeyBuffer();
	int PushKey(char chKey);
	int PopKey(char *chKey, int nLength = 1);
	char GetLastKey();
	char GetFirstKey();
	char GetKeyIndex(int nIndex);
	int GetKeyBufferLength();
	void SetTPTParam(DV_TPT *pTPT, int nCount);
	void ClearTPTParam();
	BOOL TestTPTCondition(char chLastKey = 0);

	DWORD GetCurrentRequestID();
	void AddCurrentRequest(DWORD dwEventID);
	bool StartWaitRequest(DWORD dwTimeout = 3000);
	void StopWaitCurrentRequest(DWORD dwResult);
	DWORD GetDistributeRequest() { return m_dwDistributeReuestID; }
	void SetDistributeRequest(DWORD dwRequestID);

	long PlayFile(LPCSTR lpszFilePath, UINT nMaxLength, BOOL blnAsync = FALSE, BOOL blnLoopPlay = FALSE);
	long PlayBuffer(BYTE *pData, UINT nSize, UINT nMaxLength, BOOL blnAsync = FALSE);
	int GetDigit(char *pszDigit, int nDigitCnt, UINT nMaxLength, BOOL blnAsync = FALSE);
	long RecordFile(LPCSTR lpszFilePath, UINT nMaxLength, BOOL blnAsync = FALSE);

	BOOL StopPlay();
	BOOL StopRecord();

protected:
	HCALL				m_hConsultCall;
	HCALL				m_hConferenceCall;
	int					m_intRingCnt;

	// Voice Process Data
	DV_TPT				m_tpt[DEF_TAPI_MAX_TPTCNT];
	char				m_sKeyBuffer[DEF_TAPI_MAX_KEYBUFFER];
	int					m_nKeyPtr;
	int					m_nKeyReadPtr;
	CManulSync			m_keyLock;
	DWORD				m_dwLastKeyTick;		// 上一个按键计时
	DWORD				m_dwTotalKeyTick;		// 按键/录音总计时
	DWORD				m_dwIntervalTick;		// 放音/录音间隔计时

	CPCSCommunicationEvent m_PlaySyncEvent;
	CPCSCommunicationEvent m_CallSyncEvent;
	DWORD m_dwDistributeReuestID;

private:
	long GetWaveID();
};

#endif