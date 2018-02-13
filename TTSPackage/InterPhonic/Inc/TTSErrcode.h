/*=========================================================================
 *         FILE:	TTSErrcode.h
 *  DESCRIPTION:	Contains the error code definition for iFly TTS system
 *      VERSION:	1.00
 *
 * Copyright (C)	1999 - 2000 by iFly InfoTek. Co.,LTD.
 *					All rights reserved.
 *=========================================================================
 *	History:
 *	Index   Date        Author  Notes
 *  0       2000/10/21  Alex    Create this file
 *========================================================================*/

#ifndef _TTSERRCODE_H_
#define _TTSERRCODE_H_

#ifdef _WINDOWS
#include "WinError.h"
#endif

/*=========================================================================
 *   Macro Name:    TTS_ERRCHECK_SEVERE
 *   If defined:    Severely check TTS error code (Treat warning as error)
 *  Not defined:    Not severely check TTS error code (Only report real error)
 *========================================================================*/
#define TTS_ERRCHECK_SEVERE     1

/*
 *  TTSERRVALs are 32 bit values layed out as follows:
 *
 *   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
 *   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 *  +-+-+-+-+-+---------------------+-------------------------------+
 *  |S|R|C|N|r|    Facility         |               Code            |
 *  +-+-+-+-+-+---------------------+-------------------------------+
 *
 *  where
 *
 *      S - Severity - indicates success/fail
 *
 *          0 - Success
 *          1 - Fail (COERROR)
 *
 *      R - reserved portion of the facility code, corresponds to NT's
 *              second severity bit.
 *
 *      C - reserved portion of the facility code, corresponds to NT's
 *              C field.
 *
 *      N - reserved portion of the facility code. Used to indicate a
 *              mapped NT status value.
 *
 *      r - reserved portion of the facility code. Reserved for internal
 *              use. Used to indicate HRESULT values that are not status
 *              values, but are instead message ids for display strings.
 *
 *      Facility - is the facility code
 *
 *      Code - is the facility's status code
 */

/*
 * Error Macros
 * Using macro TTSAPIERROR and TTSAPIWARNING to generate a TTS error (or warning) code.
 * For example, 
 *		nRet = TTSAPIERROR(TTSERR_READFILE);
 *		nRet = TTSAPIWARNING(TTSERR_INVALIDPTR);
 */
#define FACILITY_TTSAPI   (0x66)
#define TTSAPIERROR(x)    MAKE_SCODE(SEVERITY_ERROR,   FACILITY_TTSAPI, (x))
#define TTSAPIWARNING(x)  MAKE_SCODE(SEVERITY_SUCCESS, FACILITY_TTSAPI, (x))

/*
 * Using macro TTSERRCHECK to check the return code of TTS function.
 * For example, 
 *		if(!TTSERRCHECK(nRet))
 *		{
 *			printf("Error! Function return %d!\n", nRet);      
 *		}
 * If define macro TTS_ERRCHECK_SEVERE, warning will be treated as error.
 */
#ifdef TTS_ERRCHECK_SEVERE
#define TTSERRCHECK(x) (x == TTSERR_OK)
#else
#define TTSERRCHECK(x) SUCCEEDED(x)
#endif

#define TTSGETERRCODE(x) ((x) & 0x0000FFFF)

#define TTSERR_OK		0x0L
#define TTSERR_FALSE	0x1L
#define TTSERR_FAIL		0x80000000L

/* Base code of general error */
#define TTSERR_GENERROR     0x0
#define TTSERR_GENBASE(x)   ((x) + TTSERR_GENERROR)
/* Base code of system error */
#define TTSERR_SYSERROR     0x100
#define TTSERR_SYSBASE(x)   ((x) + TTSERR_SYSERROR)
/* Base code of memory error */
#define TTSERR_MEMERROR     0x200
#define TTSERR_MEMBASE(x)   ((x) + TTSERR_MEMERROR)

/* Base code of file error */
#define TTSERR_FILEERROR    0x300
#define TTSERR_FILEBASE(x)  ((x) + TTSERR_FILEERROR)
/* Base code of network error */
#define TTSERR_NETERROR     0x400
#define TTSERR_NETBASE(x)   ((x) + TTSERR_NETERROR)

/* Base code of resource error */
#define TTSERR_RESERROR     0x500
#define TTSERR_RESBASE(x)   ((x) + TTSERR_RESERROR)

/* Base code of lexicon error */
#define TTSERR_LEXERROR     0x800
#define TTSERR_LEXBASE(x)   ((x) + TTSERR_LEXERROR)
/* Base code of synth error */
#define TTSERR_SYNTHERROR   0x900
#define TTSERR_SYNTHBASE(x) ((x) + TTSERR_SYNTHERROR)

/* General errors */
#define TTSERR_UNKNOWN          TTSERR_GENBASE(0x1)     /* Unkown error */
#define TTSERR_EXCEPTION        TTSERR_GENBASE(0x2)     /* Exception */
#define TTSERR_NOTSUPP          TTSERR_GENBASE(0x3)     /* Not supported */
#define TTSERR_NOTIMPL          TTSERR_GENBASE(0x4)     /* Not implemented */
#define TTSERR_UNKNOWNCMD       TTSERR_GENBASE(0x5)     /* Unkown command */
#define TTSERR_INVALIDPARA      TTSERR_GENBASE(0x6)     /* Invalid parameter */
#define TTSERR_DATASIZE         TTSERR_GENBASE(0x7)     /* Data size */
#define TTSERR_ALREADYEXIST     TTSERR_GENBASE(0x8)     /* Object already exist */
#define TTSERR_OVERFLOW         TTSERR_GENBASE(0x9)     /* Over flow */
#define TTSERR_NOTRESPONSE      TTSERR_GENBASE(0xA)     /* Not response */
#define TTSERR_STOPPED          TTSERR_GENBASE(0xB)     /* Stopped */
#define TTSERR_CANCELED         TTSERR_GENBASE(0xC)     /* Canceled */
#define TTSERR_TOOMANYREQ       TTSERR_GENBASE(0xD)     /* Too many request */
#define TTSERR_TIMEOUT          TTSERR_GENBASE(0xE)     /* Time out */
#define TTSERR_NOTCONNECT       TTSERR_GENBASE(0xF)     /* Not connect */
#define TTSERR_INITFAIL         TTSERR_GENBASE(0x10)    /* Init fail */
#define TTSERR_NOTINIT          TTSERR_GENBASE(0x11)    /* Not Init */
#define TTSERR_CLOSED           TTSERR_GENBASE(0x12)    /* Closed */
#define TTSERR_NOMOREDATA       TTSERR_GENBASE(0x13)    /* No more data */
#define TTSERR_VERSIONCHECK     TTSERR_GENBASE(0x14)    /* Not pass version check */
#define TTSERR_PRECONDITION     TTSERR_GENBASE(0x15)    /* Not meet preconditon */
#define TTSERR_NOTREGISTERED    TTSERR_GENBASE(0x16)    /* Not registered */
#define TTSERR_INVALIDCONFIG	TTSERR_GENBASE(0x17)	/* Invalid configuration */

/* System errors */
#define TTSERR_CREATEHANDLE     TTSERR_SYSBASE(0x1)     /* Create system handle */
#define TTSERR_NULLHANDLE       TTSERR_SYSBASE(0x2)     /* Handle is NULL */
#define TTSERR_INVALIDHANDLE    TTSERR_SYSBASE(0x3)     /* Handle is invalid */
#define TTSERR_OPENDEV          TTSERR_SYSBASE(0x4)     /* Open device */
#define TTSERR_SETHOOK          TTSERR_SYSBASE(0x5)     /* Set hook */
#define TTSERR_REMOVEHOOK       TTSERR_SYSBASE(0x6)     /* Remove hook */
#define TTSERR_LOADDLL          TTSERR_SYSBASE(0x7)     /* Load dll */
#define TTSERR_GETPROCADDR      TTSERR_SYSBASE(0x8)     /* Get procedure address */
#define TTSERR_SYNC             TTSERR_SYSBASE(0x9)     /* Synchronize */
#define TTSERR_CREATEOBJECT     TTSERR_SYSBASE(0xA)     /* Create system object */
#define TTSERR_OBJECTEXIST      TTSERR_SYSBASE(0xB)     /* System object already exist */
#define TTSERR_WAITTIMEOUT      TTSERR_SYSBASE(0xC)     /* Wait system object time out */
#define TTSERR_OBJECTABANDON    TTSERR_SYSBASE(0xD)     /* System object abandoned */
#define TTSERR_INVALIDOBJECT    TTSERR_SYSBASE(0xE)     /* Invalid system object */

/* Memory errors */
#define TTSERR_MALLOC           TTSERR_MEMBASE(0x1)     /* Malloc (or new) memory */
#define TTSERR_REMALLOC         TTSERR_MEMBASE(0x2)     /* Remalloc memory */
#define TTSERR_OVERFLOWBUF      TTSERR_MEMBASE(0x3)     /* Memory buffer overflow */
#define TTSERR_INVALIDPTR       TTSERR_MEMBASE(0x4)     /* Invalid memory pointer */
#define TTSERR_NULLPTR          TTSERR_MEMBASE(0x5)     /* Memory pointer is NULL */
#define TTSERR_READMEM          TTSERR_MEMBASE(0x6)     /* Read memory */
#define TTSERR_WRITEMEM         TTSERR_MEMBASE(0x7)     /* Write memory */
#define TTSERR_NOENOUGHMEM      TTSERR_MEMBASE(0x8)     /* No enough memory */
#define TTSERR_NOENOUGHBUF      TTSERR_MEMBASE(0x9)     /* Buffer size is not enough */

/* Resource errors */
#define TTSERR_RESLOAD	        TTSERR_RESBASE(0x1)		/* Load resource */
#define TTSERR_RESFREE          TTSERR_RESBASE(0x2)     /* Free resource */
#define TTSERR_RESMISSING       TTSERR_RESBASE(0x3)     /* Resource File Missing */
#define TTSERR_INVALID_RESNAME  TTSERR_RESBASE(0x4)     /* Invalid resource file name */
#define TTSERR_INVALID_RESID    TTSERR_RESBASE(0x5)     /* Invalid resource ID */
#define TTSERR_INVALID_RESIMG   TTSERR_RESBASE(0x6)     /* Invalid resource image pointer */
#define TTSERR_RESWRITE         TTSERR_RESBASE(0x7)     /* Write read-only resource */
#define TTSERR_RESLEAK          TTSERR_RESBASE(0x8)     /* Resource leak out */

/* File errors */
#define TTSERR_OPENFILE         TTSERR_FILEBASE(0x1)    /* Open file */
#define TTSERR_READFILE         TTSERR_FILEBASE(0x2)    /* Read file */
#define TTSERR_WRITEFILE        TTSERR_FILEBASE(0x3)    /* Write file */
#define TTSERR_RENAMEFILE       TTSERR_FILEBASE(0x4)    /* Rename file */
#define TTSERR_MOVEFILE         TTSERR_FILEBASE(0x5)    /* Move file */
#define TTSERR_EMPTYFILE        TTSERR_FILEBASE(0x6)    /* File is empty */

/* Network errors */
#define TTSERR_OPENSOCK         TTSERR_NETBASE(0x1)     /* Open socket */
#define TTSERR_CONNECTSOCK      TTSERR_NETBASE(0x2)     /* Connect socket */
#define TTSERR_ACCEPTSOCK       TTSERR_NETBASE(0x3)     /* Accept socket */
#define TTSERR_SENDSOCK         TTSERR_NETBASE(0x4)     /* Send socket data */
#define TTSERR_RECVSOCK         TTSERR_NETBASE(0x5)     /* Recv socket data */
#define TTSERR_INVALIDSOCK      TTSERR_NETBASE(0x6)     /* Invalid socket handle */
#define TTSERR_SERVICEID        TTSERR_NETBASE(0x7)     /* Invalid service ID */

/* Lexicon errors */
#define TTSERR_CONVERT          TTSERR_LEXBASE(0x1)     /* Convert */
#define TTSERR_MATCH            TTSERR_LEXBASE(0x2)     /* Match */
#define TTSERR_GETPOS           TTSERR_LEXBASE(0x3)     /* Get position */
#define TTSERR_LOADINDEX        TTSERR_LEXBASE(0x4)     /* Load Index */
#define TTSERR_INDEX            TTSERR_LEXBASE(0x5)     /* Index */
#define TTSERR_TEXTEND          TTSERR_LEXBASE(0x6)     /* Meet text end */
#define TTSERR_NOTFOUND         TTSERR_LEXBASE(0x7)     /* Not found */

/* Synthesize errors */
#define TTSERR_INVALIDSN        TTSERR_SYNTHBASE(0x1)   /* Invalid SN code */
#define TTSERR_GETSTATUS        TTSERR_SYNTHBASE(0x2)   /* Get status info */
#define TTSERR_GETPARAM         TTSERR_SYNTHBASE(0x3)   /* Get synth parameter */
#define TTSERR_SETPARAM         TTSERR_SYNTHBASE(0x4)   /* Set synth parameter */
#define TTSERR_NOLICENCE        TTSERR_SYNTHBASE(0x5)   /* Have no licence to run */
#define TTSERR_VLIBUNKNOWN      TTSERR_SYNTHBASE(0x6)   /* Unknown voclib */
#define TTSERR_NODELEVEL        TTSERR_SYNTHBASE(0x7)   /* Synth tree node level */

#endif /* _TTSERRCODE_H_ */
