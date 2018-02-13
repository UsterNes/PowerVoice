// ThreadProcessGCEvents.cpp: implementation of the CThreadProcessGCEvents class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PowerVoice.h"
#include "PowerVoiceView.h"
#include "MainFrm.h"
#include "ThreadProcessGCEvents.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define SIGEVTCHK       0x1010  /* Check for type of signalling event   */
#define SIGBITCHK       0x1111  /* Check for signalling bit change      */

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef INTEL_DIALOGIC

CThreadProcessGCEvents::CThreadProcessGCEvents()
{

}

CThreadProcessGCEvents::~CThreadProcessGCEvents()
{

}

//==========================================================
// Thread member functions
BOOL CThreadProcessGCEvents::WaitForStartupToComplete()
{
	m_shutdown.Create();
	m_shutdown.Reset();
	CMyThread::Start();

	return TRUE;
}

void CThreadProcessGCEvents::WaitForShutdownToComplete()
{
	// if we havent already started a shut down, do so...
	if( m_shutdown.GetHANDLE() != INVALID_HANDLE_VALUE )
	{
		m_shutdown.Set();
		if( !Wait(2000) )
			WriteTrace(TraceWarn, "Problem while killing GC Message Pool Thread!");
	}
}

int CThreadProcessGCEvents::Run()
{
	int		ndevno, ntype, nlen, rc;
	int		hDTIHandle, call_state;
	void	*evtdatap;
	void	*lv_ptr;
	DX_CST	*cstp;
	CIVRChannel *lv_pIVRCh = NULL;
	METAEVENT metaevent;
	GC_INFO t_Info;
	long lv_lngCRN;
	char  szBuff[50];

	while( !m_shutdown.Wait(0) )
	{
		if( WaitForSingleObject(m_shutdown.GetHANDLE(), 0) == WAIT_OBJECT_0 )
			break;

		try
		{
			if( sr_waitevt(2000) != -1 )
			{
				ndevno = sr_getevtdev();
				ntype = sr_getevttype();
				evtdatap = sr_getevtdatap();

				// Retrieve the Metaevent block
				rc = gc_GetMetaEvent(&metaevent);
				if( rc != EGC_NOERR )
				{
					// Process the error here
					WriteTrace(TraceError, "Global Call Func: ret(%d), gc_GetMetaEvent()", rc);
					continue;
				}

				if( metaevent.flags & GCME_GC_EVENT )
				{
					ndevno = (int)metaevent.evtdev;
					ntype = (int)metaevent.evttype;
					nlen = (int)metaevent.evtlen;
					evtdatap = (void *)metaevent.evtdatap;

					if( (metaevent.evtdatap != evtdatap) ||
						 (metaevent.evtlen != nlen) ||
						 (metaevent.evtdev != ndevno) ||
						 (metaevent.evttype != ntype) )
					{
						WriteTrace(TraceError, "ERROR: gc_MetaEvent() mismatch\n\tmetaevent.evtdatap=0x%lx, "
							"evtdatap=0x%lx\n\tmetaevent.evtlen=%d, len=%d\n\t"
							"metaevent.evtdev=0x%lx, devh=0x%lx\n\t"
							"metaevent.evttype=0x%lx, code=0x%lx\n", 
							metaevent.evtdatap, evtdatap, 
							metaevent.evtlen, nlen, 
							metaevent.evtdev, ndevno, 
							metaevent.evttype, ntype);
						continue;
					}

					// GCAPI event - get channel ptr corresponding to this line dev
					rc = gc_GetUsrAttr(metaevent.linedev, &lv_ptr);
					if( rc != EGC_NOERR )
					{
						WriteTrace(TraceError, "Global Call Func: ret(%d), gc_GetUsrAttr()", rc);
						continue;
					}

					// Event Trace
					call_state = 0;
					if( metaevent.crn != 0 )
						gc_GetCallState(metaevent.crn, &call_state);
					WriteTrace(TraceDetail, "Received event 0x%X on device %d, gcCallStatus(0x%x) = %d", ntype, ndevno, metaevent.crn, call_state);

					// Get Active Object
					if( (lv_pIVRCh = (CIVRChannel *)lv_ptr) == NULL )
					{
						WriteTrace(TraceError, "Event Occurred On Unknown Object!");
						continue;
					}
					else
					{
						hDTIHandle = lv_pIVRCh->m_oARS.GetDTIHandle();
						if( hDTIHandle != ndevno )
							WriteTrace(TraceError, "IVR Channel: %d DTI handle error: %d - dev: %d", 
							lv_pIVRCh->GetLineNO(), hDTIHandle, ndevno);
					}

					// Global Call Event Analysis
					switch( ntype )
					{
					case GCEV_RESETLINEDEV:
						// Sun updated 2005-07-10
						if( !lv_pIVRCh->m_objGroup->GetIsOBOnly() )
						{
							// Issue gc_WaitCall()
							rc = gc_WaitCall(hDTIHandle, NULL, NULL, 0, EV_ASYNC);
							WriteTrace(TraceDebug, "IVR Channel: %d - Ret:%d, gc_WaitCall(%d)", lv_pIVRCh->GetLineNO(), rc, hDTIHandle);
							if( rc != EGC_NOERR )
							{
								WriteTrace(TraceError, "IVR Channel: %d - Warning: gc_WaitCall(%d) return %d, on GCEV_RESETLINEDEV event!", 
									lv_pIVRCh->GetLineNO(), ndevno, rc);
								continue;
							}
							else
							{
								// Sun added 2005-01-20
								/// Resume from error state
								lv_pIVRCh->CTI_UnsetChannelError();
							}
						}
						break;

					case GCEV_DISCONNECTED:
						// ToDo: On-Hook Process
						WriteTrace(TraceDebug, "IVR Channel: %d - Dev%d:GCEV_DISCONNECTED.", lv_pIVRCh->GetLineNO(), hDTIHandle);

						// Sun replaced 2013-02-18
						/// ±ÜÃâ×èÈûGCÏß³Ì
						/// From
						//lv_pIVRCh->SetHookOn();
						// To
						lv_pIVRCh->AnsySetHookOn();

						break;

					case GCEV_ALERTING:
						WriteTrace(TraceDebug, "IVR Channel: %d - Dev%d:GCEV_ALERTING.", lv_pIVRCh->GetLineNO(), hDTIHandle);
						break;

					case GCEV_DROPCALL:
						WriteTrace(TraceDebug, "IVR Channel: %d - Dev%d:GCEV_DROPCALL.", lv_pIVRCh->GetLineNO(), hDTIHandle);
						break;

					case GCEV_BLOCKED:
						WriteTrace(TraceDebug, "IVR Channel: %d - Dev%d:GCEV_BLOCKED.", lv_pIVRCh->GetLineNO(), hDTIHandle);
						break;

					case GCEV_UNBLOCKED:
						// Call control activity can begin on this line
						switch( lv_pIVRCh->m_oARS.m_nDevType )
						{
						case DEV_ICAPI:
						case DEV_ANALOG:
							if( !lv_pIVRCh->m_objGroup->GetIsOBOnly() )
							{
								rc = gc_WaitCall(hDTIHandle, NULL, NULL, 0, EV_ASYNC);
								WriteTrace(TraceDebug, "IVR Channel: %d - Ret:%d, gc_WaitCall(%d), on GCEV_UNBLOCKED event!", lv_pIVRCh->GetLineNO(), rc, ndevno);
								if( rc != EGC_NOERR )
								{
									WriteTrace(TraceError, "Warning: IVR Channel: %d - gc_WaitCall(%d) return %d, on GCEV_UNBLOCKED event!", lv_pIVRCh->GetLineNO(), ndevno, rc);
									lv_pIVRCh->CTI_ErrorHandler();
									continue;
								}
								else
								{
									// Sun added 2005-01-20
									/// Resume from error state
									lv_pIVRCh->CTI_UnsetChannelError();
								}
							}
						}
						break;

					case GCEV_TASKFAIL:
						rc = gc_ErrorInfo(&t_Info);
						if( rc == GC_SUCCESS )
						{
							WriteTrace(TraceDebug, "IVR Channel: %d - GC get GCEV_TASKFAIL error info on (%d), gcValue = 0x%X, gcMsg = %s, AdditionalInfo: %s", 
								lv_pIVRCh->GetLineNO(), hDTIHandle, 
								t_Info.gcValue, t_Info.gcMsg, t_Info.additionalInfo);

							// Sun replaced 1 line 2005-01-20
							//if( t_Info.gcValue != 0x41 )
							if( t_Info.gcValue > 0 && t_Info.gcValue != EGC_INVSTATE 
								&& t_Info.gcValue != EGC_FATALERROR_ACTIVE && t_Info.gcValue != EGC_FATALERROR_OCCURRED )
							{
								rc = gc_ResetLineDev(hDTIHandle, EV_ASYNC);
								WriteTrace(TraceDebug, "IVR Channel: %d - Ret:%d, gc_ResetLineDev(%d)", lv_pIVRCh->GetLineNO(), rc, hDTIHandle);
							}
						}
						if( rc != EGC_NOERR )
						{
							lv_pIVRCh->CTI_ErrorHandler();
							continue;
						}
						break;

					case GCEV_FATALERROR:
						WriteTrace(TraceError, "IVR Channel: %d - Dev%d:GCEV_FATALERROR.", lv_pIVRCh->GetLineNO(), hDTIHandle);

						// Sun added 2005-01-20
						rc = gc_ResultInfo(&metaevent, &t_Info);
						if( rc == GC_SUCCESS )
						{
							WriteTrace(TraceDebug, "IVR Channel: %d - GC get GCEV_FATALERROR result info on (%d), gcValue = 0x%X, gcMsg = %s, AdditionalInfo: %s", 
								lv_pIVRCh->GetLineNO(), hDTIHandle, 
								t_Info.gcValue, t_Info.gcMsg, t_Info.additionalInfo);

							if( t_Info.gcValue == GCRV_NONRECOVERABLE_FATALERROR )
							{
								// The application must then shut down. The firmware should then be reloaded, and the application restarted
								WriteTrace(TraceError, "IVR Channel: %d - NONRECOVERABLE_FATALERROR: The application must then shut down. The firmware should then be reloaded, and the application restarted!!!",
									lv_pIVRCh->GetLineNO());

								// Sun replaced 2011-06-27
								/// From
								//pMainWindow->m_bSystemAutoShutDown = true;
								//pMainWindow->PostMessage(WM_CLOSE);
								/// To
								if( (gb_lngAutoCloseFlag & PCS_PVC_AUTOCLOSE_DRIVER_FATALERROR) == PCS_PVC_AUTOCLOSE_DRIVER_FATALERROR )
								{
									pMainWindow->PostMessage(WM_SYS_AUTO_SHUTDOWN, PCS_PVC_AUTOCLOSE_DRIVER_FATALERROR, lv_pIVRCh->GetLineNO());
								}

								return -1;
							}
							else if( t_Info.gcValue == GCRV_RECOVERABLE_FATALERROR )
							{
								// Indicating that the application needs to issue a gc_Close( ) followed by a gc_OpenEx( )
								lv_pIVRCh->CTI_ErrorHandler();
							}
							else
							{
								// Assumes GCRV_RESETABLE_FATALERROR and gc_ResetLineDev( ) has been done
								lv_pIVRCh->CTI_SetChannelError();
							}
						}
						break;

					case GCEV_OFFERED:
						WriteTrace(TraceDebug, "Dev%d:GCEV_OFFERED -- Incoming call on IVR Channel: %d.", 
							hDTIHandle, lv_pIVRCh->GetLineNO());

						// get CRN corresponding to this event
						rc = gc_GetCRN(&lv_lngCRN, &metaevent);
						if( rc != EGC_NOERR )
						{
							lv_pIVRCh->CTI_ErrorHandler();
							continue;
						}
						if( lv_pIVRCh->CTI_SetGCCRN(lv_lngCRN) > 0 )
						{
							WriteTrace(TraceWarn, "Dev%d:GCEV_OFFERED -- can't set call ref id: %d, old CRN is %d, IVR Channel: %d.", 
								hDTIHandle, lv_lngCRN, lv_pIVRCh->m_callrefno, lv_pIVRCh->GetLineNO());
							continue;
						}

						rc = gc_AcceptCall(lv_lngCRN, 0, EV_ASYNC);

						// Sun added 2003-05-09
						if( lv_lngCRN <= 0 )
						{
							WriteTrace(TraceError, "Warning: IVR Channel: %d - gc_AcceptCall(%d) has no callrefid, on GCEV_OFFERED event!", lv_pIVRCh->GetLineNO(), ndevno);
							lv_pIVRCh->CTI_ErrorHandler();
						}
						else
							lv_pIVRCh->SetStatus(IVR_W_ANSWER);
						// End of Sun added 2003-05-09

						WriteTrace(TraceDebug, "gc_AcceptCall ret:%d, Dev%d", rc, hDTIHandle);
						if( rc != EGC_NOERR )
						{
							WriteTrace(TraceError, "Warning: IVR Channel: %d - gc_AcceptCall(%d) return %d, on GCEV_OFFERED event!", lv_pIVRCh->GetLineNO(), ndevno, rc);
							lv_pIVRCh->CTI_ErrorHandler();
							continue;
						}
						break;

					case GCEV_ACCEPT:
						// Sun added 2007-06-14
						lv_lngCRN = lv_pIVRCh->CTI_GetGCCRN();

						rc = gc_AnswerCall(lv_lngCRN, 0, EV_ASYNC);
						WriteTrace(TraceDebug, "IVR Channel: %d - gc_AnswerCall ret:%d, Dev%d", lv_pIVRCh->GetLineNO(), rc, hDTIHandle);
						if( rc != EGC_NOERR )
						{
							WriteTrace(TraceError, "Warning: IVR Channel: %d - gc_AnswerCall(%d) return %d, on GCEV_ACCEPT event!", lv_pIVRCh->GetLineNO(), ndevno, rc);
							lv_pIVRCh->CTI_ErrorHandler();
							continue;
						}
						break;

					case GCEV_CONNECTED:
					case GCEV_ANSWERED:
						WriteTrace(TraceDebug, "IVR Channel: %d - Dev%d:GCEV_ANSWERED.", lv_pIVRCh->GetLineNO(), hDTIHandle);
						
						// ToDo: Start a Call Process Thread
						lv_pIVRCh->NF_CallIn();
						break;

					// Sun added 2007-06-14
					case GCEV_BLINDTRANSFER:
						lv_lngCRN = lv_pIVRCh->CTI_GetGCCRN();
						WriteTrace(TraceDebug, "IVR Channel: %d - Dev%d:GCEV_BLINDTRANSFER(%d).", lv_pIVRCh->GetLineNO(), hDTIHandle, lv_lngCRN);

						rc = gc_ReleaseCallEx(lv_lngCRN, EV_ASYNC);
						if( rc != EGC_NOERR )
							WriteTrace(TraceError, "Warning: IVR Channel: %d - gc_ReleaseCallEx(%d) return %d, on GCEV_OFFERED event!", lv_pIVRCh->GetLineNO(), ndevno, rc);
						break;

					case GCEV_PROCEEDING:
						WriteTrace(TraceDebug, "IVR Channel: %d - Dev%d:GCEV_PROCEEDING.", lv_pIVRCh->GetLineNO(), hDTIHandle);
						break;

					case GCEV_PROGRESSING:
						WriteTrace(TraceDebug, "IVR Channel: %d - Dev%d:GCEV_PROGRESSING.", lv_pIVRCh->GetLineNO(), hDTIHandle);
						break;
					}
				}
				else if( (ntype & DT_DTI) == DT_DTI )
				{
					// unsolicited DTI event!
					WriteTrace(TraceError, "Unkown DTI event 0x%X on device 0x%X(%s) - Ignored", 
							ntype, ndevno, ATDV_NAMEP(ndevno));
					continue;
				}
				else
				{
					// Now that it must be a D40 event, look for corresponding chan
					/// since this is not a GCAPI event,we can NOT use same mechanism
					/// to search channel as in DT_GC
					// ToDo: Here we process analog (eg. DTMF) event
					if( !pChannelView->m_oIVRCHMap.Lookup(ndevno, lv_pIVRCh) )
						continue;

					if( lv_pIVRCh == NULL )
						continue;

					switch( ntype )
					{
					//-------------------------------
					// Sun added 2012-11-23
					case TDX_PLAY:
						WriteTrace(TraceDebug, "IVR Channel: %d received play end msg channel [%d], AsynPlaying flag = %d, Term reason = %d", 
							lv_pIVRCh->GetLineNO(), ndevno, lv_pIVRCh->GetIsAsynPlaying(), ATDX_TERMMSK(ndevno));   // TM_LCOFF

						if( lv_pIVRCh->GetIsAsynPlaying() )
							lv_pIVRCh->SetIsAsynPlaying(FALSE);
						break;

					case TDX_RECORD:
						WriteTrace(TraceDebug, "IVR Channel: %d received record end msg channel [%d], AsynPlaying flag = %d, Term reason = %d", 
							lv_pIVRCh->GetLineNO(), ndevno, lv_pIVRCh->GetIsAsynPlaying(), ATDX_TERMMSK(ndevno));
						break;

					case TDX_GETDIG:
						WriteTrace(TraceDebug, "IVR Channel: %d received getdig end msg channel [%d], AsynPlaying flag = %d, Term reason = %d", 
							lv_pIVRCh->GetLineNO(), ndevno, lv_pIVRCh->GetIsAsynPlaying(), ATDX_TERMMSK(ndevno));
						break;

					case TDX_SETHOOK:
						WriteTrace(TraceDebug, "IVR Channel: %d received sethook end msg channel [%d], AsynPlaying flag = %d, Term reason = %d", 
							lv_pIVRCh->GetLineNO(), ndevno, lv_pIVRCh->GetIsAsynPlaying(), ATDX_TERMMSK(ndevno));
						break;
					//-------------------------------

					case TDX_CST:
						cstp = (DX_CST *)evtdatap;

						switch( cstp->cst_event )
						{
						case DE_RINGS:
							WriteTrace(TraceDebug, "IVR Channel: %d received Ring... Ring... channel [%d]", lv_pIVRCh->GetLineNO(), ndevno);
							lv_pIVRCh->NF_CallIn();
							break;

						case DE_TONEON:
						case DE_LCOF:
							WriteTrace(TraceDebug, "IVR Channel: %d received Hang Up Code [%d] on channel [%d]", lv_pIVRCh->GetLineNO(), cstp->cst_data, ndevno);
							lv_pIVRCh->SetHookOn();
							break;
						}
						break;

					// 2002-02-01
					case TDX_DIAL:
					case TDX_CALLP:
						long cpret;

						cpret = ATDX_CPTERM( ndevno );
						switch( cpret )
						{
						case CR_BUSY:
							WriteTrace(TraceInfo, "IVR Channel: %d received Dial failed on channel [%d] with error code CR_BUSY[%d]", 
								lv_pIVRCh->GetLineNO(), ndevno, CR_BUSY);
							break;
						case CR_CEPT:
							WriteTrace(TraceDebug, "IVR Channel: %d received Dial failed on channel [%d] with error code CR_CEPT[%d]", 
								lv_pIVRCh->GetLineNO(), ndevno, CR_CEPT);
							break;
						case CR_CNCT:
							WriteTrace(TraceDebug, "IVR Channel: %d received Dial failed on channel [%d] with error code CR_CNCT[%d]", 
								lv_pIVRCh->GetLineNO(), ndevno, CR_CNCT);
							break;
						case CR_FAXTONE:
							WriteTrace(TraceInfo, "IVR Channel: %d received Dial failed on channel [%d] with error code CR_FAXTONE[%d]", 
								lv_pIVRCh->GetLineNO(), ndevno, CR_FAXTONE);
							break;
						case CR_NOANS:
							WriteTrace(TraceInfo, "IVR Channel: %d received Dial failed on channel [%d] with error code CR_NOANS[%d]", 
								lv_pIVRCh->GetLineNO(), ndevno, CR_NOANS);
							break;
						case CR_NODIALTONE:
							WriteTrace(TraceInfo, "IVR Channel: %d received Dial failed on channel [%d] with error code CR_NODIALTONE[%d]", 
								lv_pIVRCh->GetLineNO(), ndevno, CR_NODIALTONE);
							break;
						case CR_NORB:
							WriteTrace(TraceInfo, "IVR Channel: %d received Dial failed on channel [%d] with error code CR_NORB[%d]", 
								lv_pIVRCh->GetLineNO(), ndevno, CR_NORB);
							break;
						case CR_STOPD:
							WriteTrace(TraceInfo, "IVR Channel: %d received Dial failed on channel [%d] with error code CR_STOPD[%d]", 
								lv_pIVRCh->GetLineNO(), ndevno, CR_STOPD);
							break;
						case CR_ERROR:
							WriteTrace(TraceInfo, "IVR Channel: %d received Dial failed on channel [%d] with error code CR_ERROR[%d]", 
								lv_pIVRCh->GetLineNO(), ndevno, CR_ERROR);
							break;
						default:
							WriteTrace(TraceInfo, "IVR Channel: %d received Dial failed on channel [%d] with error code [%d]", 
								lv_pIVRCh->GetLineNO(), ndevno, cpret);
							break;
						}

						if( cpret != CR_CNCT )
						{
							// Send Clear Call Message to Agent
						}
						if( cpret != CR_FAXTONE )
						{
						}
						break;

					//-----------------------------------------------------------------------
					// Sun added 2012-06-01
					case TFX_PHASEB:
						WriteTrace(TraceDebug, "IVR Channel: %d received TFX_PHASEB event 0x%x, Speed = %ld", 
							lv_pIVRCh->GetLineNO(), ntype, ATFX_SPEED(lv_pIVRCh->m_oARS.m_faxdev));
						if( (ATFX_BSTAT(lv_pIVRCh->m_oARS.m_faxdev) & DFS_REMOTEID) == DFS_REMOTEID )
						{
							if( fx_getparm(lv_pIVRCh->m_oARS.m_faxdev, FC_REMOTEID, szBuff) != 0 )
							{
								WriteTrace(TraceWarn, "FAX IVR Channel: %d Getting FC_REMOTEID() error.", lv_pIVRCh->GetLineNO());
							}
							else
							{
								// remove any leading/trailing spaces from remote id b4 storing it
								strcpy(lv_pIVRCh->m_oARS.m_szRemoteID, szBuff);
								WriteTrace(TraceDebug, "FAX IVR Channel: %d got FC_REMOTEID %s", 
									lv_pIVRCh->GetLineNO(), lv_pIVRCh->m_oARS.m_szRemoteID);
							}
						}
						break;

					case TFX_PHASED:
						WriteTrace(TraceDebug, "IVR Channel: %d received TFX_PHASED event 0x%x", lv_pIVRCh->GetLineNO(), ntype);
						break;

					case TFX_FAXERROR:
						WriteTrace(TraceInfo, "IVR Channel: %d received TFX_FAXERROR event 0x%x", lv_pIVRCh->GetLineNO(), ntype);
						if( (rc = ATFX_ESTAT(lv_pIVRCh->m_oARS.m_faxdev)) != 0 )
						{
							WriteTrace(TraceWarn, "IVR Channel: %d received TFX_FAXERROR event 0x%x, %s Err Status %d", 
								lv_pIVRCh->GetLineNO(), ATDV_LASTERR(lv_pIVRCh->m_oARS.m_faxdev), ATDV_ERRMSGP(lv_pIVRCh->m_oARS.m_faxdev), rc);
						}
						break;

					case TFX_FAXSEND:
						WriteTrace(TraceDebug, "IVR Channel: %d received TFX_FAXSEND event 0x%x, %d page%s to %s", 
							lv_pIVRCh->GetLineNO(), ntype, 
							ATFX_PGXFER(lv_pIVRCh->m_oARS.m_faxdev), ATFX_PGXFER(lv_pIVRCh->m_oARS.m_faxdev) > 1 ? "s" : "",
							lv_pIVRCh->m_oARS.m_szRemoteID);
						break;

					case TFX_FAXRECV:
						WriteTrace(TraceDebug, "IVR Channel: %d received TFX_FAXRECV event 0x%x, %d page%s from %s", 
							lv_pIVRCh->GetLineNO(), ntype,
							ATFX_PGXFER(lv_pIVRCh->m_oARS.m_faxdev), ATFX_PGXFER(lv_pIVRCh->m_oARS.m_faxdev) > 1 ? "s" : "",
							lv_pIVRCh->m_oARS.m_szRemoteID);
						break;

					default:
						WriteTrace(TraceInfo, "IVR Channel: %d Unhandled event 0x%x", lv_pIVRCh->GetLineNO(), ntype);
						break;   
					//-----------------------------------------------------------------------
					}
				}
			}
		}
		catch(...)
		{
			WriteTrace(TraceError, _T("CThreadProcessGCEvents::Run() - Unexpected exception"));
		}
	}

	return 0;
}

/******************************************************************************
 *        NAME: int sig_hdlr()
 * DESCRIPTION: Signal handler to catch DTEV_SIG events generated by the dti
 *              timeslots.
 *       INPUT: None.
 *      OUTPUT: None.
 *     RETURNS: 0 
 *    CAUTIONS: None.
 ******************************************************************************/
long sig_hdlr( unsigned long event_handle )
{
	CIVRChannel *lv_pIVRCh = NULL;
	int tsdev = sr_getevtdev( event_handle );
	int event = sr_getevttype( event_handle );
	unsigned short *ev_datap = ( unsigned short * )sr_getevtdatap( event_handle );
	unsigned short sig = ( unsigned short )( *ev_datap );

	if( !pChannelView->m_oIVRCHMap.Lookup(tsdev, lv_pIVRCh) )
		return 0;

	if( lv_pIVRCh == NULL )
		return 0;

   if( event != DTEV_SIG ) 
   {
      WriteTrace(TraceError, "IVR Channel: %d received Unknown Event 0x%lx Received on %s.  Data = 0x%hx",
               lv_pIVRCh->GetLineNO(), event, ATDV_NAMEP( tsdev ), sig );
      return 0;
   }
	
   for( int i = 0; i < 4; i++ ) 
   {
		/*
		 * Check if bit in change mask (upper nibble - lower byte) is set or
		 * if this is a WINK (upper nibble - upper byte) event
		 */
		if ( !( sig & ( SIGEVTCHK << i ) ) ) 
			continue;
 
    
		if( gb_NT_ABS_Disconnect > 0 )
		{
			if( (UINT)(sig & ( SIGBITCHK << i )) == gb_NT_ABS_Disconnect )
			{
				WriteTrace(TraceDebug, "Disconnet on receive AB signal for IVR Channel: %d.", lv_pIVRCh->GetLineNO());
				lv_pIVRCh->SetHookOn();
			}
		}
	}
	return 0;
}

#endif