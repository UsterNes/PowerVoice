/*=========================================================================
 *         FILE:	iFly_TTS.h
 *  DESCRIPTION:	Contains the TTS API declarations of USTC iFly InfoTek.
 *		VERSION:	1.00
 *
 * Copyright (C)	1999 - 2000 by iFly InfoTek. Co.,LTD.
 *					All rights reserved.
 *=========================================================================
 *=========================================================================
 *	History:
 *	Index   Date			Author		Notes
 *  0		2000/10/21		jdyu		Create this file
 *  1		2001/02/16      Yan Jun     Add more comments, modify VID macros
 *  2		2002/05/10      jdyu	    Modify TTSLoadUserLib function
 *  3		2002/08/10		Gao Yi		Add more macros to read number
 *	4		2002/08/30		Gao Yi		Add macros to read english
 *	5		2003/11/13		wbli		remove unicode char definition(No support).
 *========================================================================*/

#ifndef IFLY_TTS_H
#define IFLY_TTS_H

#include "TTSErrcode.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef IFLYTTS

#ifdef WIN32
#define TTSLIBAPI __declspec(dllexport)
#else
#define TTSLIBAPI
#endif

/*
 *	Basic Data Types
 */

/* Integer types */
typedef long int			TTSINT32;
typedef short				TTSINT16;

/* Handles */
typedef void*				HTTSINSTANCE;
typedef TTSINT32			HTTSUSERLIB;

/* Char type */
typedef char				TTSCHAR;
typedef char*				PTTSCHAR;

/* Cardinal types */
#define TTSVOID				void
#define PTTSVOID			void*

typedef unsigned char       TTSUNS8;
typedef unsigned long int	TTSUNS32;
typedef unsigned short		TTSU16;
typedef float				TTSFLOAT;
typedef unsigned long       TTSDWORD;
typedef unsigned short      TTSWORD;

/* Boolean */
typedef unsigned int		TTSBOOL;

#ifndef TRUE
#define TRUE				1
#endif
#ifndef FALSE
#define FALSE				0
#endif

/* TTS Return value type */
typedef TTSINT32					TTSRETVAL;

#endif /* #ifndef IFLYTTS*/

/* iFly-TTS SDK Version */
#define IFLYTTS_SDK_VER				0x0101
/* Reserved Length */
#define TTS_RESERVED_LEN			0x000E

/* 
 *	TTS common defines
 */
#define TTS_CONNECT_STRUCT_VERSION		0x0100
#define TTS_SERVICE_UID_MAX				16
#define TTS_USER_NAME_MAX				20
#define TTS_COMPANY_NAME_MAX			64
#define TTS_SERIAL_NO_MAX				128
#define TTS_PRODUCT_NAME_MAX			64
#define TTS_IP_MAXLEN					32

/* Use TTS default configurations */

/* Synthesizing process flags */
#define TTS_FLAG_STILL_HAVE_DATA		1   
#define TTS_FLAG_DATA_END				2
#define TTS_FLAG_CMD_CANCELED			3

/* Chinese code page type */
#define TTS_CP_AUTO						0
#define	TTS_CP_GB2312					1   
#define	TTS_CP_GBK						2   
#define	TTS_CP_BIG5						3
#define TTS_CP_UNICODE					4
#define TTS_CP_GB18030					5
#define TTS_CP_UTF8						6

/* Pause and transition in melody */
#define TTS_SSL_NORMAL					0		/* Normal */
#define TTS_SSL_STALL					1		/* A little stall */
#define TTS_SSL_SNATCHY					2		/* Evident snatchy */
#define TTS_SSL_UNCEASING				3		/* Unceasing */
#define TTS_SSL_VERBOSE					4		/* Vobose */

/* Output audio data formats */
#define	TTS_ADF_DEFAULT					0
#define	TTS_ADF_PCM8K8B1C				1
#define	TTS_ADF_PCM16K8B1C				2
#define	TTS_ADF_PCM8K16B1C				3
#define	TTS_ADF_PCM16K16B1C				4
#define	TTS_ADF_PCM11K8B1C				5
#define	TTS_ADF_PCM11K16B1C				6
#define TTS_ADF_PCM6K8B1C               7
#define TTS_ADF_PCM6K16B1C              8
#define	TTS_ADF_ALAW16K1C				9
#define	TTS_ADF_ULAW16K1C				10
#define	TTS_ADF_ALAW8K1C				11
#define	TTS_ADF_ULAW8K1C				12
#define	TTS_ADF_ALAW11K1C				13
#define	TTS_ADF_ULAW11K1C				14
#define TTS_ADF_ALAW6K1C                15
#define TTS_ADF_ULAW6K1C                16
#define	TTS_ADF_ADPCMG7218K4B1C			17
#define	TTS_ADF_ADPCMG7216K4B1C			18
#define	TTS_ADF_ADPCMG7233B1C			19
#define	TTS_ADF_ADPCMG7235B1C			20
#define	TTS_ADF_VOX6K1C					21
#define	TTS_ADF_VOX8K1C					22
#define TTS_ADF_AMR6K1C					23
#define TTS_ADF_AMR8K1C					24
#define TTS_ADF_AMR11K1C				25
#define TTS_ADF_AMR16K1C				26
#define TTS_ADF_MP36K1C					27
#define TTS_ADF_MP38K1C					28
#define TTS_ADF_MP311K1C				29
#define TTS_ADF_MP316K1C				30

/* Audio data head type */
#define TTS_AHF_DEFAULT                 0   /* Audio data head has 44 byte */
#define TTS_AHF_NONE                    1   /* No audio data head */
#define TTS_AHF_STAND                   2   /* Standard audio data head */

/* About type */
#define TTS_ABOUT_PRODUCTINFO			0	/* Information about TTS kernel product */

/* These macros are used when setting/getting TTS_PARAM_ENTERTREAT */
#define TTS_ET_AUTO						0	/* Automatically treat <enter> char(s) */
#define TTS_ET_SPLITSEN					1	/* When meeting <enter> char(s), split sentence */
#define TTS_ET_NOTHING					2	/* Treat <enter> char(s) as noting */
#define TTS_ET_SPACE					3	/* Treat <enter> char(s) as space char */

/* These macros are used when setting/getting TTS_PARM_AVAILABLEVID */
#define TTS_AVID_MAXAVAVIDCOUNT			30	/* Max available VID count */

/* These macros are used when setting/getting TTS_PARM_READNUMBER */
#define	TTS_RN_AUTO_VALUE				0	/* Auto, read as value if not sure */
#define	TTS_RN_VALUE					1	/* Read as value */
#define	TTS_RN_DIGIT					2	/* Read as string */
#define	TTS_RN_AUTO_DIGIT				3	/* Auto, read as string if not sure */

/* These macros are used when setting/getting TTS_PARM_READENGLISH */
#define TTS_RE_AUTO_WORD				0	/* Auto, read as word if not sure */
#define TTS_RE_LETTER					1	/* Read as letter */
#define TTS_RE_AUTO_LETTER				2	/* Auto, read as letter if not sure */

/* These macros are used when setting/getting TTS_PARAM_TEXTTYPE */
#define TTS_TT_AUTO						0	/* auto, read as plain text if not sure */
#define TTS_TT_PLAINTEXT				1	/* read as plain text */
#define TTS_TT_CSSMLTEXT				2	/* read as CSSML text */

/* audio data and its head byte-order when TTS_PARAM_BYTEORDER */
#define	TTS_BO_LITTLEENDIAN				0	/* intel x86 */
#define	TTS_BO_BIGENDIAN				1	/* Sun/Macintosh */

/* These macros are used when setting/getting TTS_PARAM_VPTTREAT*/
#define TTS_VPT_DISABLE					0	/* Disable replacing matching sentence with prompt voice*/
#define TTS_VPT_ENABLE					1	/* Replace matching sentence with prompt voice*/

/* TTS parameters used by Set/Get TTS parameter */
#define TTS_PARAM_LOCAL_BASE			0x0000

/* Get maximum buffer size (in bytes) for storing input text data */
#define TTS_PARAM_INBUFSIZE				(TTS_PARAM_LOCAL_BASE + 1)
/* Get/Set maximum buffer size (in bytes) for storing output audio data */
#define TTS_PARAM_OUTBUFSIZE			(TTS_PARAM_LOCAL_BASE + 2)
/* Get/Set current voice library format ID */
#define TTS_PARAM_VID					(TTS_PARAM_LOCAL_BASE + 3)
/* Get/Set current Chinese code page type */
#define TTS_PARAM_CODEPAGE				(TTS_PARAM_LOCAL_BASE + 4)
/* Get/Set current audio data format */
#define TTS_PARAM_AUDIODATAFMT			(TTS_PARAM_LOCAL_BASE + 5)
/* Get/Set current speed value */
#define TTS_PARAM_SPEED					(TTS_PARAM_LOCAL_BASE + 6)
/* Get/Set current audio data head type */
#define TTS_PARAM_AUDIOHEADFMT			(TTS_PARAM_LOCAL_BASE + 7)
/* Get/Set current volume value of audio data */
#define TTS_PARAM_VOLUME				(TTS_PARAM_LOCAL_BASE + 8)
/* Get/Set current pitch value of audio data */
#define TTS_PARAM_PITCH					(TTS_PARAM_LOCAL_BASE + 9)
/* Set/Get treatment of <enter> char(s) when split sentence */
#define TTS_PARAM_ENTERTREAT            (TTS_PARAM_LOCAL_BASE + 10)
/* Set/Get max length of the splitted sentence (default is 128 chars, cannot less than 10) */
#define TTS_PARAM_MAXSENLEN             (TTS_PARAM_LOCAL_BASE + 11)
/* Get current available voice library list */
#define TTS_PARAM_AVAILABLEVID          (TTS_PARAM_LOCAL_BASE + 12)
/* Get/Set whether to read all marks or not */
#define	TTS_PARAM_READALLMARKS			(TTS_PARAM_LOCAL_BASE + 13)
/* Get/Set pause and transition in melody */
#define	TTS_PARAM_STALL_STYLE			(TTS_PARAM_LOCAL_BASE + 14)
/* Get/Set How to pronounce number, value or digit */
#define	TTS_PARAM_READNUMBER			(TTS_PARAM_LOCAL_BASE + 15)
/* Get/Set How to pronounce english, letter or word */
#define	TTS_PARAM_READENGLISH			(TTS_PARAM_LOCAL_BASE + 16)
/* Get/Set default text type */
#define	TTS_PARAM_TEXTTYPE				(TTS_PARAM_LOCAL_BASE + 17)
/* Get/Set byte-order */
#define TTS_PARAM_BYTEORDER				(TTS_PARAM_LOCAL_BASE + 18)
/* Get/Set Prompt voice treat, only supported by InterPhonic CE 3.0 or later*/
#define TTS_PARAM_VPTTREAT				(TTS_PARAM_LOCAL_BASE + 19)
/* Get/Set background sound id, only supported by InterPhonic CE 3.0 or later*/
#define TTS_PARAM_BGSOUND				(TTS_PARAM_LOCAL_BASE + 20)

/* Parameters used in net only */
#define TTS_PARAM_SERVER_BASE			0x0100

/* Set/Get current network send timeout */
#define TTS_PARAM_SERVER_SNDTO			(TTS_PARAM_SERVER_BASE + 1)
/* Set/Get current network receive timeout */
#define TTS_PARAM_SERVER_RCVTO			(TTS_PARAM_SERVER_BASE + 2)
/* Set/Get current network idle timeout */
#define TTS_PARAM_SERVER_IDLETO			(TTS_PARAM_SERVER_BASE + 3)
/* Get current network active connection count */
#define TTS_PARAM_SERVER_ACTCNT			(TTS_PARAM_SERVER_BASE + 4)
/* Get current network maximum connection count */
#define TTS_PARAM_SERVER_MAXCNT			(TTS_PARAM_SERVER_BASE + 5)
/* Set/get net connect timeout */
#define TTS_PARAM_SERVER_CNTTO			(TTS_PARAM_SERVER_BASE + 6)

/*
 * TTS Data Structures
 */

#pragma pack(2)

/* This structure used by client in TTSConnect function */
typedef struct
{
	TTSDWORD	dwSDKVersion;							/* [in]  The client's TTS SDK version */
	TTSCHAR		szUserName[TTS_USER_NAME_MAX];			/* [in]  User name */
	TTSCHAR		szCompanyName[TTS_COMPANY_NAME_MAX];	/* [in]  Company name */
	TTSCHAR		szSerialNumber[TTS_SERIAL_NO_MAX];		/* [in]  Serial Number has form as "xxxxxx-xxxxxx-xxxxxx" */
	TTSCHAR		szServiceUID[TTS_SERVICE_UID_MAX];		/* [in]  TTS Sevice sign */
	TTSCHAR		szProductName[TTS_PRODUCT_NAME_MAX];	/* [in]  Client program name */
	TTSCHAR		szTTSServerIP[TTS_IP_MAXLEN];			/* [in]  The IP address of TTS Server */
	TTSDWORD	dwServiceID;							/* [out] TTS Service ID return by Service Module */
	TTSDWORD	dwErrorCode;							/* [out] The error code return by Service Module */
	TTSBOOL		bSetParams;								/* [in]	 Determine whether the following setting is valid or not */
	TTSINT16	nCodePage;								/* [in]  Default chinese codepage type */
	TTSINT16	nVID;									/* [in]  Default TTS Voice Lib */
	TTSINT16	nAudioFmt;								/* [in]  Default audio data format */
	TTSINT16	nSpeed;									/* [in]  Default speed */
	TTSINT16	nVolume;								/* [in]  Default volume */
	TTSINT16	nPitch;									/* [in]	 Default pitch */
	TTSINT16	nAudioHeadFmt;							/* [in]  Default audio head fmt */
	TTSINT16	nTextType;								/* [in]	 Default text type */
	TTSDWORD	dwReserved[TTS_RESERVED_LEN];			/* [in/out] Reserved field, set it to zero */
} TTSConnectStruct, *PTTSConnectStruct;

/* The structure used in synthesizing process */
typedef struct 
{
	TTSDWORD	dwServiceID;	/* [in]  Connect's service ID */
	TTSDWORD	dwInBufSize;	/* [in]  Text buffer size */
    TTSDWORD	dwOutBufSize;	/* [out] Output audio data length */
    TTSDWORD	dwInFlags;		/* [in]  Input data flag */
	TTSDWORD	dwOutFlags;		/* [out] Output data flag */
	TTSWORD		wAudioHeadLen;	/* [out] Output audio data head length */
	TTSDWORD	dwCurStart;		/* [out] The start position of current synthesizing text in szInBuf */
	TTSDWORD	dwCurEnd;		/* [out] The end position of current synthesizing text in szInBuf */
	TTSDWORD	dwErrorCode ;	/* [out] The error code returned by service module */
	
	TTSCHAR		*szInBuf;		/* [in]  Input text buffer pointer, malloced by caller */
	PTTSVOID	pOutBuf;		/* [out] Output audio data pointer, malloced by TTS service module */
	PTTSCHAR	pszPYBuf;		/* [out] Output PinYin string buf, malloced by TTS service mudule */

	TTSDWORD	dwReserved;		/* [in/out]Reserved field */
} TTSData, *PTTSData;

/*
 * The prototype of callback funtions used in synthesizing process
 */

/* Synthesize data process callback */
typedef TTSRETVAL (*TTSPROCESSCB)(HTTSINSTANCE hTTSInst, PTTSData pTTSData, TTSINT32 lParam, PTTSVOID pUserData);

/* Book mark event id */
#define TTS_EVENT_MARK			1
/* Synthesize events callback */
typedef TTSRETVAL (*TTSEVENTCB)(HTTSINSTANCE hTTSInst, PTTSData pTTSData, TTSINT16 nNotify, TTSINT32 lParam, PTTSVOID pUserData);

/* Callback data structure */
typedef struct
{
    TTSINT32		nNumCallbacks;      /* [in] Number of callback functions */
    TTSPROCESSCB	pfnTTSProcessCB;	/* [in] Synthesizing process callback function */
    TTSEVENTCB		pfnTTSEventCB;		/* [in] Event notifies in synthesizing process */
} TTSCallBacks, *PTTSCallBacks;

typedef struct
{
    /* [out] Count of available VIDs */
    TTSINT32        nCount;
    /* [out] List of available VID Indexs */
    TTSDWORD        dwVIDIndexList[TTS_AVID_MAXAVAVIDCOUNT];     
    /* [out] List of available VID ResourceIDs */
    TTSDWORD        dwVIDResList[TTS_AVID_MAXAVAVIDCOUNT];     
} TTSAvailableVIDs;

#pragma pack()

/*==========================================================================
	Declaration of Interface Funtions
 ==========================================================================*/

/***************************************************************************
 * Funtion Name		: TTSInitialize
 * Parameters		: None
 * Descrption		: Global initialize TTS, it is the first function to be called
 * Return Value		: Return 0 in success, otherwise return error code
 * Error Codes		: N
 * Exception Handle	: N
***************************************************************************/
TTSRETVAL TTSLIBAPI TTSInitialize();
typedef TTSRETVAL (*Proc_TTSInitialize)();

/**************************************************************************
 * Funtion Name		: TTSUninitialize
 * Parameters		: None
 * Descrption		: Global uninitialize TTS, it is the last function to be called
 * Return Value		: Return 0 in success, otherwise return error code
 * Error Codes		: N
 * Exception Handle	: N
***************************************************************************/
TTSRETVAL TTSLIBAPI TTSUninitialize();
typedef TTSRETVAL (*Proc_TTSUninitialize)();

/**************************************************************************
 * Funtion Name		: TTSConnect
 * Parameters		: PTTSConnectStruct pConnect --- [in/out] The connect structuture
 * Descrption		: TTSConnect create a instance to TTS Core service
 * Return Value		: Return a handle to the TTS Instance, otherwise return NULL.
 * Error Codes		: N
 * Exception Handle	: N
***************************************************************************/
HTTSINSTANCE TTSLIBAPI TTSConnect(PTTSConnectStruct pConnect);
typedef HTTSINSTANCE (*Proc_TTSConnect)(PTTSConnectStruct pConnect);

/**************************************************************************
 * Funtion Name		: TTSDisconnect
 * Parameters		: HTTSINSTANCE hTTSInst --- [in] The TTS handle returned by TTSConnect funtion
 * Descrption		: TTSDiconnect destroy the connect to TTS Service Module
 * Return Value		: Return 0 in success, otherwise return error code
 * Error Codes		: N
 * Exception Handle	: N
***************************************************************************/
TTSRETVAL TTSLIBAPI TTSDisconnect(HTTSINSTANCE hTTSInst);
typedef TTSRETVAL (*Proc_TTSDisconnect)(HTTSINSTANCE hTTSInst);

/**************************************************************************
 * Funtion Name		: TTSSynthText
 * Parameters		: HTTSINSTANCE hTTSInst --- [in] The TTS handle returned by TTSConnect funtion
 *					: PTTSData --- [in/out] Pointer to TTSData structure
 * Descrption		: Synthesize some text to memory
 * Return Value		: Return 0 in success, otherwise return error code
 * Error Codes		: N
 * Exception Handle	: N
***************************************************************************/
TTSRETVAL TTSLIBAPI TTSSynthText(HTTSINSTANCE hTTSInst, PTTSData pTTSData);
typedef TTSRETVAL (*Proc_TTSSynthText)(HTTSINSTANCE hTTSInst, PTTSData pTTSData);

/**************************************************************************
 * Funtion Name		: TTSFetchNext
 * Parameters		: HTTSINSTANCE hTTSInst --- [in] The TTS handle returned by TTSConnect funtion
 * 					: PTTSData --- [in/out] Pointer to TTSData structure
 * Descrption		: Fetch the audio data remains until OutFlag is TTS_DATA_FLAG_END
 * Return Value		: Return 0 in success, otherwise return error code
 * Error Codes		: N
 * Exception Handle	: N
***************************************************************************/
TTSRETVAL TTSLIBAPI TTSFetchNext(HTTSINSTANCE hTTSInst, PTTSData pTTSData);
typedef TTSRETVAL (*Proc_TTSFetchNext)(HTTSINSTANCE hTTSInst, PTTSData pTTSData);

/**************************************************************************
 * Funtion Name		: TTSSynthTextEx
 * Parameters		: HTTSINSTANCE hTTSInst --- [in] The TTS handle returned by TTSConnect funtion
 *					: PTTSData --- [in/out] Pointer to TTSData structure
 *					: pTTSCallBacks --- [in] Pointer to TTSCallbacks structure
 * 					: bASynch --- [in] The synchesize mode is asynchronous or not
 *					: PTTSVOID pUserData --- [in] User defined data
 * Descrption		: Synthesize some text to memory
 *					:
 * Return Value		: Return 0 in success, otherwise return error code
 * Error Codes		: N
 * Exception Handle	: N
***************************************************************************/
TTSRETVAL TTSLIBAPI TTSSynthTextEx(HTTSINSTANCE hTTSInst, PTTSData pTTSData, PTTSCallBacks pTTSCallBacks, TTSBOOL bASynch, PTTSVOID pUserData);
typedef TTSRETVAL (*Proc_TTSSynthTextEx)(HTTSINSTANCE hTTSInst, PTTSData pTTSData, PTTSCallBacks pTTSCallBacks, TTSBOOL bASynch, PTTSVOID pUserData);

/**************************************************************************
 * Funtion Name		: TTSClean
 * Parameters		: HTTSINSTANCE hTTSInst --- [in] The TTS handle returned by TTSConnect funtion
 * Descrption		: Reset the TTS status to initial status, free memory malloced in synthesizing process
 * Return Value		: Return 0 in success, otherwise return error code
 * Error Codes		: N
 * Exception Handle	: N
***************************************************************************/
TTSRETVAL TTSLIBAPI TTSClean(HTTSINSTANCE hTTSInst);
typedef TTSRETVAL (*Proc_TTSClean)(HTTSINSTANCE hTTSInst);

/**************************************************************************
 * Funtion Name		: TTSGetSynthParam
 * Parameters		: HTTSINSTANCE hTTSInst --- [in] The TTS instance handle return by TTSConnect
					: TTSDWORD dwParamType --- [in] Parameter type
					: TTSDWORD *pdwParam --- [in/out] Paremeter value
 * Descrption		: Get the parameters of the TTS system
 * Return Value		: Return 0 in success, otherwise return error code
 * Error Codes		: N
 * Exception Handle	: N
***************************************************************************/
TTSRETVAL TTSLIBAPI TTSGetSynthParam(HTTSINSTANCE hTTSInst, TTSDWORD dwParamType, TTSINT32 *pnParam);
typedef TTSRETVAL (*Proc_TTSGetSynthParam)(HTTSINSTANCE hTTSInst, TTSDWORD dwParamType, TTSINT32 *pnParam);

/**************************************************************************
 * Funtion Name		: TTSSetSynthParam
 * Parameters		: HTTSINSTANCE hTTSInst --- [in] The TTS instance handle return by TTSConnect
					: TTSDWORD dwParamType --- [in] Parameter type
					: TTSDWORD dwParam --- [in] Paremeter value
 * Descrption		: Set the parameters of the TTS system
 * Return Value		: Return 0 in success, otherwise return error code
 * Error Codes		: N
 * Exception Handle	: N
***************************************************************************/
TTSRETVAL TTSLIBAPI TTSSetSynthParam(HTTSINSTANCE hTTSInst, TTSDWORD dwParamType, TTSINT32 nParam);
typedef TTSRETVAL (*Proc_TTSSetSynthParam)(HTTSINSTANCE hTTSInst, TTSDWORD dwParamType, TTSINT32 nParam);

/**************************************************************************
 * Funtion Name		: TTSLoadUserLib
 * Parameters		: HTTSINSTANCE hTTSInst --- [in] The TTS instance handle return by TTSConnect
 *					: HTTSUSERLIB* lphUserLib --- [out] Pointer to the user defined library handle
 *					: PTTSVOID lpUserLibPath --- [in] User library
 *					: TTSINT32 nSize --- [in] sizeof lpUserLibPath data
 * Descrption		: Load a user defined library to TTS system
 * Return Value		: Return 0 in success, otherwise return error code
 * Error Codes		: N
 * Exception Handle	: N
***************************************************************************/
TTSRETVAL TTSLIBAPI TTSLoadUserLib(HTTSINSTANCE hTTSInst, HTTSUSERLIB* lphUserLib, PTTSVOID pUserLib, TTSINT32 nSize);
typedef TTSRETVAL (*Proc_TTSLoadUserLib)(HTTSINSTANCE hTTSInst, HTTSUSERLIB* lphUserLib, PTTSVOID pUserLib, TTSINT32 nSize);


/**************************************************************************
 * Funtion Name		: TTSUnloadUserLib
 * Parameters		: HTTSINSTANCE hTTSInst --- [in] The TTS instance handle return by TTSConnect
 *					: HTTSUSERLIB hUserLib --- [in] The labrary handle returned by TTSLoadUserLib
 * Descrption		: Remove a user defined library to TTS system
 * Return Value		: Return 0 in success, otherwise return error code
 * Error Codes		: N
 * Exception Handle	: N
***************************************************************************/
TTSRETVAL TTSLIBAPI TTSUnloadUserLib(HTTSINSTANCE hTTSInst, HTTSUSERLIB hUserLib);
typedef TTSRETVAL (*Proc_TTSUnloadUserLib)(HTTSINSTANCE hTTSInst, HTTSUSERLIB hUserLib);

/*==========================================================================
	Accessorial Functions
 ===========================================================================*/

/**************************************************************************
 * Funtion Name		: TTSFormatMessage
 * Parameters		: TTSDWORD dwTTSErrCode --- [in] TTS error code
 *					: PTTSVOID pMsgBuf --- [out] Pointer to a buffer(malloced by user) 
 *					: TTSINT16* pnSize --- [in/out] message buffer length
 * Descrption		: Format a TTS error code to error description string
 * Return Value		: Return 0 in success, otherwise return error code
 * Error Codes		: N
 * Exception Handle	: N
**************************************************************************/
TTSRETVAL TTSLIBAPI TTSFormatMessage(TTSDWORD dwTTSErrCode, PTTSVOID pMsgBuf, TTSINT16* pnSize);
typedef TTSRETVAL (*Proc_TTSFormatMessage)(TTSDWORD dwTTSErrCode, PTTSVOID pMsgBuf, TTSINT16* pnSize);

/**************************************************************************
 * Funtion Name		: TTSAbout
 * Parameters		: TTSDWORD dwAboutType --- [in] TTS About type
 *					: PTTSVOID pAboutContent --- [out] TTS About content
 *					: TTSINT16* nSize --- [in/out] content size
 * Descrption		: Get TTS System information of current version
 * Return Value		: Return 0 in success, otherwise return error code
 * Error Codes		: N
 * Exception Handle	: N
***************************************************************************/
TTSRETVAL TTSLIBAPI	TTSAbout(TTSDWORD dwAboutType, PTTSVOID pAboutContent, TTSINT16* pnSize);
typedef TTSRETVAL (*Proc_TTSAbout)(TTSDWORD dwAboutType, PTTSVOID pAboutContent, TTSINT16* pnSize);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*#ifndef IFLY_TTS_H*/
