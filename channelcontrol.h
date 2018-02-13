// channelcontrol.h: interface for the Cchannelcontrol class.
//
//////////////////////////////////////////////////////////////////////
//
// Last modified 2001-12-06

#if !defined(AFX_CHANNELCONTROL_H__60D695F6_6747_48F7_A0D3_8A326FEC1419__INCLUDED_)
#define AFX_CHANNELCONTROL_H__60D695F6_6747_48F7_A0D3_8A326FEC1419__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 获取线路状态常数
#define	LINESTATUS_DEVST		0			// Device State
#define	LINESTATUS_HOOKST		1			// Hook State
#define	LINESTATUS_LINEST		2			// Line State

// 线路状态常数
#define	VOICE_LS_ERROR			-1			// Fail to get line status

/// 摘挂机状态
#define	VOICE_LS_HOOKST_ONHOOK  0			// Hook State: On-Hook
#define	VOICE_LS_HOOKST_OFFHOOK 1			// Hook State: Off-Hook

/// 当前线路状态位
#define VOICE_LS_LINEST_SILENCE 0x80		// Sil Bit in Raw Line Status
#define VOICE_LS_LINEST_DTMF	0x40		// DTMF Signal Bit in Raw Line Status
#define VOICE_LS_LINEST_LCSENSE 0x20		// Loop Current Sense Bit in Raw Line Status
#define VOICE_LS_LINEST_RING	0x10		// Ring Detect Bit in Raw Line Status
#define VOICE_LS_LINEST_HOOK	0x08		// Hook Switch Status Bit in Raw Line Status
#define VOICE_LS_LINEST_RINGBK	0x04		// Audible Ringback Detect Bit in Raw Line Status

/// 当前设备状态
#define VOICE_LS_DEVST_IDLE		1			// Channel is idle
#define VOICE_LS_DEVST_PLAY		2			// Channel is playing back
#define VOICE_LS_DEVST_RECD		3			// Channel is recording
#define VOICE_LS_DEVST_DIAL		4			// Channel is dialing
#define VOICE_LS_DEVST_GTDIG	5			// Channel is getting digits
#define VOICE_LS_DEVST_TONE		6			// Channel is generating a tone
#define VOICE_LS_DEVST_STOPD	7			// Operation has terminated
#define VOICE_LS_DEVST_SENDFAX	8			// Channel is sending a fax
#define VOICE_LS_DEVST_RECVFAX	9			// Channel is receiving a fax
#define VOICE_LS_DEVST_FAXIO	10			// Channel is between fax pages
#define VOICE_LS_DEVST_HOOK		11			// A change in hookstate is in progress
#define VOICE_LS_DEVST_WINK		12			// A wink operation is in progress
#define VOICE_LS_DEVST_CALL		13			// Channel is Call Progress Mode
#define VOICE_LS_DEVST_GETR2MF	14			// Channel is Getting R2MF
#define VOICE_LS_DEVST_RINGS	15			// Call status Rings state
#define VOICE_LS_DEVST_BLOCKED	16			// Channel is blocked

// 获取语音处理函数中断类型
#define	VOICE_TM_NORMTERM		0			// 正常终止
#define	VOICE_TM_MAXDTMF		1			// 最大按键终止
#define	VOICE_TM_MAXSIL			2			// 最大静默时间
#define	VOICE_TM_MAXNOSIL		3			// 最大非静默时间
#define	VOICE_TM_LCOFF			4			// 断线
#define	VOICE_TM_IDDTIME		5			// 最大按键间隔
#define	VOICE_TM_MAXTIME		6			// 最大时间终止
#define	VOICE_TM_DIGIT			7			// 中断按键终止
#define	VOICE_TM_PATTERN		8			// 模式匹配
#define	VOICE_TM_USRSTOP		9			// Function stopped by user
#define	VOICE_TM_EOD			10			// End of Data reached on playback
#define	VOICE_TM_TONE			11			// Tone-on/off event
#define	VOICE_TM_BARGEIN		12			// Play terminated due to Barge-in
#define	VOICE_TM_ERROR			13			// I/O Device Error
#define	VOICE_TM_Dummy			255			// Dummy

#define	VOICE_OBT_FAILED		-1			// 外拨失败
#define	VOICE_OBT_SUCC			0			// 成功
#define	VOICE_OBT_BUSY			1			// 忙线
#define	VOICE_OBT_BREAK			2			// 中断
#define	VOICE_OBT_STATUS		3			// 条件错
#define	VOICE_OBT_FAX			4			// 传真
#define	VOICE_OBT_NOANSWER		5			// 没应答
#define	VOICE_OBT_NOTONE		6			// 没有拨号音
#define	VOICE_OBT_NORINGBACK	7			// 没有回铃音
#define	VOICE_OBT_CAPERROR		8			// 分析错误

// 座席状态退出码
#define	MSI_TM_NOMSI			100			// No MSI exist
#define	MSI_TM_MSIBUSY			101			// the MSI is busy now
#define	MSI_TM_GROUPBUSY		102			// the MSI Group is busy but others may available
#define	MSI_TM_ALLMSIBUSY		103			// All MSIs are busy
#define	MSI_TM_MSIOK			106			// Switch to the MSI OK
#define	MSI_TM_GROUPOK			107			// Switch to the MSI Group OK
#define	MSI_TM_ANYMSIOK			108			// Switch to an MSI OK
#define	MSI_TM_NOANSWER			109			// No people answer
#define	MSI_TM_ANSWER			110			// MSI answer
#define	MSI_TM_ERROR			113			// Any exception
#define	MSI_TM_Dummy			255			// Dummy

#define CMESyncEvent_Common		0
#define CMESyncEvent_Play		1
#define CMESyncEvent_GetDigit	2
#define CMESyncEvent_Record		3
#define CMESyncEvent_Divert		4
#define MAX_CMESyncEvents		5

#include ".\tapiobj.h"
#include "ASRPackage\ASRInterface.h"	// Sun added 2008-03-10

#ifdef CISCO_CME
#include "..\CommonClass\PCSCommunicationEvent.h"
#include ".\PicaCommon\pcsparam.h"
#endif

class Cchannelcontrol
#ifdef CISCO_CCM
	: public CTapiLine
#endif
{
#ifdef CISCO_CCM
public:
	virtual void GetData(char *pBuffer,int iLen);
#endif

public:
	Cchannelcontrol();
	virtual ~Cchannelcontrol();

	DWORD	m_nTag;								// Tag Data
	BYTE	m_nDevType;							// Physical Device Type
	long	m_faxdev;							// Physical FAX Device Index No

#ifdef INTEL_DIALOGIC
	int		m_hDevTimeSlot;						// device time slot handle
	char	m_szRemoteID[50];
#endif

private:
	int		m_index;							// Object Index
	bool	m_bGotBauRte;						// 是否已经显示协商速率, sun added 2012-06-13

#ifdef INTEL_DIALOGIC
	int		m_line;								// Physical Device Index No
	long	m_dtidev;							// DTI Device Index No
	long	m_faxtype;							// FAX Device Type
	bool	m_bSCBus;
	int		m_iBaudRate;
	int		m_hAsynFileHandle;					// 异步操作文件句柄，Sun added 2012-11-23
#endif

	char	m_sChannelName[50];					// Name of Channel
	char	m_sDTIName[50];						// Name of DTI Interface
	char	m_sFAXName[50];						// Name of FAX Interface

#ifdef INTEL_DIALOGIC
	DF_IOTT			m_iott;
	DF_ASCIIDATA    m_asciidata;
#endif

	int	getdigmask(char mask[]);

#ifdef INTEL_DIALOGIC
	int		ivr_channel_addtone();				// Added call analysis tones

	BOOL	dti_init_isdn_channel();
	BOOL	dti_init_anapi_channel();
	BOOL	dti_init_icapi_channel();
#endif

public:
	BOOL	m_blnBindFaxResource;

	void	CreateChObj(int f_Index);
	BOOL	ivr_channel_open(char *sChannelName = NULL, char *sDTIName = NULL, char *sFaxName = NULL);
	BOOL	ivr_channel_close();
	int		ivr_channel_closefile();

#ifdef CISCO_CCM
	BOOL	ivr_channel_restart();
#endif

#ifdef INTEL_DIALOGIC
	BOOL	ivr_channel_route();
	BOOL	ivr_channel_dx_stop();

	void	dti_releaseLineDev();
	void	ivr_channel_setcapp(DX_CAP *capp);
#endif

	int		sc_channel_link(Cchannelcontrol* f_nLinkTo);
	int		sc_channel_unlink(Cchannelcontrol* f_nLinkTo);

	int		sc_fax_link(Cchannelcontrol* f_nLinkTo);
	int		sc_fax_unlink(long f_lngFaxDev);

	void	ivr_channel_clear();
	BOOL	ivr_channel_stop();
	BOOL	ivr_channel_stop_play();
	
	int		ivr_channel_getstate(int nClass = 0);
	int		ivr_channel_gettermmsk(int nEventType = CMESyncEvent_Common);
	BOOL	ivr_channel_terminate();

	BOOL	ivr_channel_waitring();
	BOOL	ivr_channel_hook(int nhook, long callrefno = 0, bool blnForce = false);
	
	int		ivr_channel_play(const char *pMent, char *termchar = NULL, bool clrdigitbuf = true, int nTimeout = 0);
	int		ivr_channel_playbuffer(const BYTE *pData, int nLen, char *termchar = NULL, bool clrdigitbuf = true);
	int		ivr_channel_async_play(const char *pMent, char *termchar = NULL, bool clrdigitbuf = true, bool loop = false);
	int		ivr_channel_playrecord(const char *pMent);
	int		ivr_channel_getdigit(const char *pMent, int ndigitcnt, char *pGetDigit, int ntime=10, int ninterval=5, char *termchar=NULL, bool clrdigitbuf = true);
	int		ivr_channel_record(const char *pMent, int rectime = 120, int siltime = 5, int minRecTime = 5, bool toneon = true, int nNotifyIntvl = 0, char *termchar = NULL);

	// Sun added 2008-03-10
	int		ivr_asr_recording(CASRInterface *pASRInt, const char *pMent, bool blnBeep, int rectime, int siltime);

	int		set_calling_num(char *szANI);		// Sun added 2012-06-26
	int		ivr_channel_sendDTMF(const char *szdial, bool blnAsync = false);
	int		ivr_channel_dailout(const char *szdial, char *szANI, int nTimeout, bool blnAsync = false, long *callrefno = NULL);
	int		ivr_channel_divert(const char *szdial, long callrefno);

	void	fax_channel_send_init(const char *sRemoteID = NULL, const char *sHeader = NULL, const char *sFromNo = NULL, int nRetryTimes = 0);
	int		fax_channel_send(const char *pMent, const char *sRemoteID = NULL, const char *sHeader = NULL, const char *sFromNo = NULL, int nRetryTimes = 0);
	void	fax_channel_receive_init(const char *sToNo = NULL, int nRetryTimes = 0);
	int		fax_channel_receive(const char *pFile, char *sFromNo = NULL, const char *sToNo = NULL, int nRetryTimes = 0);
	BOOL	fax_channel_filefind(const char *sFileName = NULL);
	BOOL	fax_channel_fileopen(const char *sFileName = NULL);
	void	fax_channel_fileclose();
	void	fax_channel_tolower_string(char *ptr);
	void	fax_log_NegotiatedSpeed();			// Sun added 2012-06-13
	int		fax_channel_stopch();				// Sun added 2012-07-13
	BOOL	fax_channel_reopen();				// Sun added 2012-08-02


	int		ivr_channel_moneyplay(double f_FEE, BYTE f_LangID = 0);
	int		ivr_channel_numberplay(const char *pNumber, BOOL f_blnReadLetter = FALSE, BYTE f_LangID = 0);
	int		ivr_channel_digitplay(double f_Digit, int f_AfterDot = 2, BYTE f_LangID = 0);
	int		ivr_channel_timeplay(DWORD f_Seconds, BYTE f_Type, BYTE f_LangID = 0);

#ifdef INTEL_DIALOGIC
	int		GetVoxHandle() { return m_line; }
	int		GetDTIHandle() { return m_dtidev; }

	GC_MAKECALL_BLK m_makecall_blk;
	GCLIB_MAKECALL_BLK	*m_pgclib_makecall;

	// Set the extra data in the makecall block
	GC_PARM_BLKP m_parmblkp;

#endif

protected:
	CASRInterface *m_pASRInt;

//--------------------------------------------------
// Sun added 2010-03-08
#ifdef CISCO_CME
public:
	void ResetAllSyncEvents();
	UL GetTerminateCode(int nEventType);
	void SetTerminateCode(int nEventType, UL lngTermCode, LPCSTR strData = NULL, IPCSParam *pParam = NULL);

	int ConsultConference(LPCSTR lpszDigits, int nNoAnsTimeOut = 0);
	int CompleteTransfer(BOOL blnTransfer = TRUE);
	int StopConsultaion();

protected:
	CPCSCommunicationEvent m_CMESyncEvent[MAX_CMESyncEvents];
	CPCSParam m_CMEEventParam[MAX_CMESyncEvents];
#endif
//--------------------------------------------------
};

#endif // !defined(AFX_CHANNELCONTROL_H__60D695F6_6747_48F7_A0D3_8A326FEC1419__INCLUDED_)
