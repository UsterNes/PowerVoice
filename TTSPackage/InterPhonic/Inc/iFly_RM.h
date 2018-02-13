/*=========================================================================
 *         FILE:	iFly_RM.h
 *  DESCRIPTION:	Contains the RM API declarations of USTC iFly InfoTek.
 *		VERSION:	1.00
 *
 * Copyright (C)	1999 - 2000 by iFly InfoTek. Co.,LTD.
 *					All rights reserved.
 *=========================================================================
 *=========================================================================
 *	History:
 *	Index   Date			Author		Notes
 *  0      2000/10/21		jdyu		Create this file
 *=========================================================================*/

#ifndef IFLY_RM_H
#define IFLY_RM_H

#include "iFly_TTS.h"

#ifdef __cplusplus
extern "C" {
#endif

/*==========================================================================
 *
 * The functions related to Load Balance system and TTS Server Control
 *
 *=========================================================================*/

/* TTS Server state */
#define TTS_SERVER_STATUS_STARTED	 			1
#define TTS_SERVER_STATUS_STOPPED 				2
#define TTS_SERVER_STATUS_LOAD					4
#define TTS_SERVER_STATUS_BUSY	 				8

/* Callback level */
#define	TTS_CALLBACK_LEVEL_WARNING				1
#define	TTS_CALLBACK_LEVEL_ERROR				2
#define	TTS_MAX_ERROR_LEVEL						4
#define	TTS_CALLBACK_LEVEL_ALL					7

/* Operations to TTS Server */
#define TTS_SERVER_OPERATION_START				1
#define TTS_SERVER_OPERATION_STOP				2
#define	TTS_SERVER_OPERATION_SHUTDOWN			4

#pragma pack(2)
/* TTS Server information */
typedef struct tagTTSServerInfo
{
	TTSDWORD 	dwIP;				/* server IP */
	TTSCHAR 	szName[64];			/* server name */
	TTSINT32	nStatus;			/* server state */
	TTSINT32	nActiveThreads;		/* active connection number of specified server */
	TTSINT32	nReserved[2];		/* reserved */
}TTSServerInfo, *PTTSServerInfo;
#pragma pack()

/* Callback funtion prototype */
typedef TTSVOID (*TTSServerStatusCB)(PTTSVOID pUserData, PTTSServerInfo pServer, TTSINT32 nLevel);

/**************************************************************************
 * Funtion Name		: RMTTSGetBestServer
 * Parameters		:
 *					: TTSDWORD* pdwServerIP --- [out] TTS Server IP that has minimum load
 * Descrption		: Get the TTS Server IP that has minimum load
 * Return Value		: Return 0 in success, otherwise return error code
 * Error Codes		: N
 * Exception Handle	: N
***************************************************************************/
TTSRETVAL TTSLIBAPI RMTTSGetBestServer(TTSDWORD* pdwServerIP);
typedef TTSRETVAL (*Proc_RMTTSGetBestServer)(TTSDWORD* pdwServerIP);

/**************************************************************************
 * Funtion Name		: RMTTSOperateServer
 * Parameters		:
 *					: TTSCHAR* pszServerIP --- [in] Destination TTS server IP
 *					: TTSINT16 nOperation [in]
						== TTS_SERVER_OPERATION_START --- Start TTS service
						== TTS_SERVER_OPERATION_STOP --- Stop TTS service
						== TTS_SERVER_OPERATION_SHUTDOWN --- Shut down the TTS server program
 * Descrption		: Operate the TTS service on TTS Server
 * Return Value		: Return 0 in success, otherwise return error code
 * Error Codes		: N
 * Exception Handle	: N
***************************************************************************/
TTSRETVAL TTSLIBAPI RMTTSOperateServer(TTSCHAR* pszServerIP, TTSINT16 nOperation);
typedef TTSRETVAL (*Proc_RMTTSOperateServer)(TTSCHAR* pszServerIP, TTSINT16 nOperation);

/**************************************************************************
 * Funtion Name		: RMTTSEnumServerInfo
 * Parameters		:
 *					: TTSDWORD *pdwIP --- [out] The buffer to return server IPs
 *					: UINT nSize --- [in] Maximum number of IPs that can load to pdwIP
 *					: UINT *pnServers --- [out] The return number of server IPs at pdwIP acturally
 * Descrption		: Enumerate TTS servers' IP that can offer TTS service
 * Return Value		: Return 0 in success, otherwise return error code
*Error Codes		: N
*Exception Handle	: N
***************************************************************************/
TTSRETVAL TTSLIBAPI RMTTSEnumServerInfo(TTSDWORD *pdwIP, TTSINT32 nSize, TTSINT32 *pnServers);
typedef TTSRETVAL (*Proc_RMTTSEnumServerInfo)(TTSDWORD *pdwIP, TTSINT32 nSize, TTSINT32 *pnServers);

/**************************************************************************
 * Funtion Name		: RMTTSGetServerInfo
 * Parameters		:
 *					: PTTSServerInfo pInfo --- [out] The buffer address to recieve TTS Server information
 *					: TTSDWORD dwIP --- [in] The server IP need to get information
 * Descrption		: Get information of the specified TTS Server
 * Return Value		: Return 0 in success, otherwise return error code
 * Error Codes		: N
 * Exception Handle	: N
***************************************************************************/
TTSRETVAL TTSLIBAPI RMTTSGetServerInfo(PTTSServerInfo pInfo, TTSDWORD dwIP);
typedef TTSRETVAL (*Proc_RMTTSGetServerInfo)(PTTSServerInfo pInfo, TTSDWORD dwIP);

/**************************************************************************
 * Funtion Name		: RMTTSSetServerMonitorCallback
 * Parameters		:
 *					: TTSServerStatusCB procCallback --- [in] callback function address
 *					: PTTSVOID pUserData --- [in] the user data transfer to callback function
 *					: TTSINT32 nLevel --- [in] the level to activate the callback function
						== TTS_CALLBACK_LEVEL_ALL	--- All error and warning information will recieved by callback function
						== TTS_CALLBACK_LEVEL_WARNING --- Only warning information will activate callback function
						== TTS_CALLBACK_LEVEL_ERROR	--- Only error information will activate callback function
 *					: TTSINT32 *pnCallbackID --- Pointer to return callback function ID value
 * Descrption		: Set the callback function when occuring error on TTS server
 * Return Value		: Return 0 in success, otherwise return error code
 * Error Codes		: N
 * Exception Handle	: N
***************************************************************************/
TTSRETVAL TTSLIBAPI RMTTSSetServerMonitorCallback(TTSServerStatusCB procCallback, PTTSVOID pUserData, TTSINT32 nLevel, TTSINT32 *pnCallbackID);
typedef TTSRETVAL (*Proc_RMTTSSetServerMonitorCallback)(TTSServerStatusCB procCallback, PTTSVOID pUserData, TTSINT32 nLevel, TTSINT32 *pnCallbackID);

/**************************************************************************
 * Funtion Name		: RMTTSRemoveServerMonitorCallback
 * Parameters		:
 *					: TTSDWORD dwCallbackID --- [in] The callback ID need to remove
 * Descrption		: Remove a callback funtion that has set by RMTTSSetServerMonitorCallback
 * Return Value		: Return 0 in success, otherwise return error code
 * Error Codes		: N
 * Exception Handle	: N
***************************************************************************/
TTSRETVAL TTSLIBAPI RMTTSRemoveServerMonitorCallback(TTSDWORD dwCallbackID);
typedef TTSRETVAL (*Proc_RMTTSRemoveServerMonitorCallback)(TTSDWORD dwCallbackID);

#ifdef __cplusplus
}
#endif

#endif /* IFLY_RM_H */
