// channelcontrol.cpp: implementation of the Cchannelcontrol class.
//
//////////////////////////////////////////////////////////////////////
//
// Last modified 2007-06-12

#include "stdafx.h"
#include "_Global.h"
#include "_CommonFunc.h"

#include "channelcontrol.h"
#include "PowerVoice.h"
#include "PowerVoiceView.h"
#include <fcntl.h>
#include <math.h>

#ifdef CISCO_CME
#include "MainFrm.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// this class is dialogic control
//////////////////////////////////////////////////////////////////////

Cchannelcontrol::Cchannelcontrol()
{
	m_index = -1;
	m_pASRInt = NULL;

#ifdef INTEL_DIALOGIC
	m_line = -1;						// Physical Device Index No
	m_dtidev = -1;						// DTI Device Index No
	m_faxdev = -1;						// Physical FAX Device Index No
	m_faxtype = -1;
	m_hDevTimeSlot = -1;				// device time slot handle
	m_iBaudRate = 9600;
	memset(m_szRemoteID, 0x00, sizeof(m_szRemoteID));

	m_hAsynFileHandle = -1;

	//memset(&m_makecall_blk, 0x00, sizeof(GC_MAKECALL_BLK));
	//memset(&m_gclib_makecall, 0x00, sizeof(GCLIB_MAKECALL_BLK));
	m_parmblkp = NULL;
    m_pgclib_makecall = NULL;
	m_makecall_blk.cclib = NULL;
	m_makecall_blk.gclib = NULL;

#endif

#ifdef CISCO_CME
	for( int lv_nEventType = 0; lv_nEventType < MAX_CMESyncEvents; lv_nEventType++ )
		m_CMESyncEvent[lv_nEventType].Create();
#endif

	m_bGotBauRte = true;						// 是否已经显示协商速率, sun added 2012-06-13

	m_nTag = 0;
	m_nDevType = DEV_NONE;
	m_blnBindFaxResource = TRUE;
	memset(m_sChannelName, 0x00, sizeof(m_sChannelName));
	memset(m_sDTIName, 0x00, sizeof(m_sDTIName));
	memset(m_sFAXName, 0x00, sizeof(m_sFAXName));
}

Cchannelcontrol::~Cchannelcontrol()
{
}

void Cchannelcontrol::CreateChObj(int f_Index)
{
	m_index = f_Index;

#ifdef CISCO_CCM
	// Sun added 2011-04-16
	CWaveOutDevice::SetIVRChannelID(f_Index);
	CWaveInDevice::SetIVRChannelID(f_Index);
#endif
}

#ifdef INTEL_DIALOGIC
// Sun added 2002-03-01
int Cchannelcontrol::ivr_channel_addtone()
{
	int reco, index;

	if( dx_deltones(m_line) == -1 ) 
		return(-1);
	
	// initialize tones for PerfectCall
	for (index = 0; index < 9; index++) 
	{
		if( gb_Tone[index].freq1.freq != 0 )
		{
			reco = dx_chgfreq(
				gb_Tone[index].tid,
				gb_Tone[index].freq1.freq,
				gb_Tone[index].freq1.deviation,
				gb_Tone[index].freq2.freq,
				gb_Tone[index].freq2.deviation
				);
			reco = dx_chgdur(
				gb_Tone[index].tid,
				gb_Tone[index].on.time,
				gb_Tone[index].on.deviation,
				gb_Tone[index].off.time,
				gb_Tone[index].off.deviation
				);
			reco = dx_chgrepcnt(
				gb_Tone[index].tid,
				gb_Tone[index].repcnt
				);
		}
	}
	if (dx_initcallp(m_line) == -1) 
		return(-1);

	// initialize dt_tones for disconnet
	index = 9;
	reco = dx_blddtcad(
		gb_Tone[index].tid,
		gb_Tone[index].freq1.freq,
		gb_Tone[index].freq1.deviation,
		gb_Tone[index].freq2.freq,
		gb_Tone[index].freq2.deviation,
		gb_Tone[index].on.time,
		gb_Tone[index].on.deviation,
		gb_Tone[index].off.time,
		gb_Tone[index].off.deviation,
		gb_Tone[index].repcnt
		);
	if(reco == -1)
		return(-1);
	if(dx_addtone(m_line, NULL, 0) == -1) 
		return(0);

	// initialize st_tones for disconnet
	index = 10;
	reco = dx_bldstcad(
			gb_Tone[index].tid,
			gb_Tone[index].freq1.freq,		
			gb_Tone[index].freq1.deviation,	
			gb_Tone[index].freq2.freq,		
			gb_Tone[index].freq2.deviation,	
			gb_Tone[index].on.time,			
			gb_Tone[index].on.deviation,	
			gb_Tone[index].off.time
			);

	if(reco == -1)
		return(-1);
	if( (reco = dx_addtone(m_line, NULL, 0)) == -1 ) 
		return(-1);

	// Sun added 2006-02-20
	/// 第二组挂机参数
	if( gb_Enable2DiscDTone )
	{
		//// 第二双音频挂断音
		index = 11;
		reco = dx_blddtcad(
			gb_Tone[index].tid,
			gb_Tone[index].freq1.freq,
			gb_Tone[index].freq1.deviation,
			gb_Tone[index].freq2.freq,
			gb_Tone[index].freq2.deviation,
			gb_Tone[index].on.time,
			gb_Tone[index].on.deviation,
			gb_Tone[index].off.time,
			gb_Tone[index].off.deviation,
			gb_Tone[index].repcnt
			);
		if(reco == -1)
			return(-1);
		if(dx_addtone(m_line, NULL, 0) == -1) 
			return(0);
	}
	if( gb_Enable2DiscSTone )
	{
		//// 第二单音频挂断音
		index = 12;
		reco = dx_bldstcad(
				gb_Tone[index].tid,
				gb_Tone[index].freq1.freq,		
				gb_Tone[index].freq1.deviation,	
				gb_Tone[index].freq2.freq,		
				gb_Tone[index].freq2.deviation,	
				gb_Tone[index].on.time,			
				gb_Tone[index].on.deviation,	
				gb_Tone[index].off.time
				);

		if(reco == -1)
			return(-1);
		if( (reco = dx_addtone(m_line, NULL, 0)) == -1 ) 
			return(-1);
	}

	return 1;
}

BOOL Cchannelcontrol::dti_init_isdn_channel()
{
	int rc;
	char lv_sGCName[50];

	// Open Voice Resource Device
	sr_hold();
	if( ( m_line = dx_open(m_sChannelName, NULL)) == -1 )
		return FALSE;
	sr_release();

	//-----------------------------------------------------------------------
	// Sun added 2012-06-26
	gc_util_insert_parm_val(&m_parmblkp, GCSET_CHAN_CAPABILITY, GCPARM_CAPABILITY, 
		sizeof(int), GCCAP_AUDIO_g711Alaw64k);
	
	m_pgclib_makecall = (GCLIB_MAKECALL_BLKP) malloc(sizeof(GCLIB_MAKECALL_BLK) + m_parmblkp->parm_data_size);
	if( m_pgclib_makecall == NULL)
	{
		WriteTrace(TraceError, "IVR Channel: %d - Could not allocate memory for GCLIB_MAKECALL_BLKP", m_index);
		return FALSE;
	}

	m_makecall_blk.cclib = NULL;
	m_makecall_blk.gclib = m_pgclib_makecall;

	// Set the extra data in the makecall block
	m_pgclib_makecall->ext_datap = m_parmblkp;
	//-----------------------------------------------------------------------

	// Open ISDN DTI Device
	sprintf(lv_sGCName, ":N_%s:P_isdn", m_sDTIName);
	WriteTrace(TraceDebug, "IVR Channel: %d - Global Call ISDN Devname = %s", m_index, lv_sGCName);

	if( gc_OpenEx(&m_dtidev, lv_sGCName, EV_SYNC, (void *)m_nTag) != EGC_NOERR )
		return FALSE;

	m_nDevType = DEV_ISDN;
	rc = gc_ResetLineDev(m_dtidev, EV_ASYNC);
	WriteTrace(TraceDebug, "IVR Channel: %d - Global Call Func: ret(%d), gc_ResetLineDev(%d)", m_index, rc, m_dtidev);
	if( rc != EGC_NOERR )
		return FALSE;

	// Get the timeslot device handle attached to this linedevice
	rc = gc_GetNetworkH(m_dtidev, &m_hDevTimeSlot);
	WriteTrace(TraceDebug, "IVR Channel: %d - Global Call Func: ret(%d), gc_GetNetworkH(%d)", m_index, rc, m_dtidev);
	if( rc != EGC_NOERR )
		return FALSE;

	rc = gc_SetAlarmNotifyAll(m_dtidev, ALARM_SOURCE_ID_NETWORK_ID, ALARM_NOTIFY);
	WriteTrace(TraceDebug, "IVR Channel: %d - Global Call Func: ret(%d), gc_SetAlarmNotifyAll(%d)", m_index, rc, m_dtidev);

	// Do the SCBus Routing if necessary
	if( nr_scroute(m_line, SC_VOX, m_hDevTimeSlot, SC_DTI, SC_FULLDUP) == -1 )
	{
		WriteTrace(TraceWarn, "Warn - IVR Channel: %d nr_scroute(%s, %s) failed:%s, %s\n", 
			m_index,
		    ATDV_NAMEP(m_hDevTimeSlot),
		    ATDV_NAMEP(m_line),
		    ATDV_ERRMSGP(m_hDevTimeSlot),
		    ATDV_ERRMSGP(m_line));
		return FALSE;
	}

	return TRUE;
}

BOOL Cchannelcontrol::dti_init_anapi_channel()
{
	int rc;
	char lv_sGCName[50];

	// Open Analog Device
	sprintf(lv_sGCName, ":P_%s:V_%s", "na_an_io", m_sChannelName);
	TRACE("Global Call ANAPI Devname = %s\n", lv_sGCName);

	if( gc_OpenEx(&m_dtidev, lv_sGCName, EV_SYNC, (void *)m_nTag) != EGC_NOERR )
		return FALSE;

	m_nDevType = DEV_ANALOG;

	// Get the voice device handle attached to this linedevice
	rc = gc_GetVoiceH(m_dtidev, &m_line);
	TRACE("Global Call Func: ret(%d), gc_GetVoiceH(%d)", rc, m_dtidev);
	if( rc != EGC_NOERR )
		return FALSE;

	// Do the SCBus Routing if necessary
	if( nr_scroute(m_line, SC_VOX, m_line, SC_LSI, SC_FULLDUP) == -1 )
	{
		TRACE("nr_scroute(%s, %s) failed:%s, %s\n", 
		    ATDV_NAMEP(m_hDevTimeSlot),
		    ATDV_NAMEP(m_line),
		    ATDV_ERRMSGP(m_hDevTimeSlot),
		    ATDV_ERRMSGP(m_line));
		return FALSE;
	}

	return TRUE;
}

BOOL Cchannelcontrol::dti_init_icapi_channel()
{
	int rc;
	char lv_sGCName[50];

	// Open ICAPI Device
	sprintf(lv_sGCName, ":N_%s:P_%s", m_sDTIName, gb_NT_Protocol);
	WriteTrace(TraceInfo, "IVR Channel: %d - Global Call E1 Devname = %s", m_index, lv_sGCName);

	if( gc_OpenEx(&m_dtidev, lv_sGCName, EV_SYNC, (void *)m_nTag) != EGC_NOERR )
		return FALSE;

	m_nDevType = DEV_ICAPI;

	// Open Voice Resource Device
	if( ( m_line = dx_open(m_sChannelName, NULL)) == -1 )
	{
		WriteTrace(TraceError, "IVR Channel: %d - Error: dx_open(%s) failed!!", m_index, m_sChannelName);
		return FALSE;
	}

	if( gc_AttachResource(m_dtidev, m_line, NULL, NULL, GC_VOICEDEVICE, EV_SYNC) == -1 )
	{
		WriteTrace(TraceError, "IVR Channel: %d - Global Call Func Error: gc_AttachResource(%d), %s", 
			m_index, m_dtidev, ATDV_ERRMSGP(m_line));
		return FALSE;
	}
	
	// Get the timeslot device handle attached to this linedevice
	rc = gc_GetNetworkH(m_dtidev, &m_hDevTimeSlot);
	WriteTrace(TraceInfo, "IVR Channel: %d - Global Call Func: ret(%d), gc_GetNetworkH(%d)", m_index, rc, m_dtidev);
	if( rc != EGC_NOERR )
		return FALSE;

	// Do the SCBus Routing if necessary
	if( nr_scroute(m_line, SC_VOX, m_hDevTimeSlot, SC_DTI, SC_FULLDUP) == -1 )
	{
		WriteTrace(TraceError, "IVR Channel: %d - nr_scroute(%s, %s) failed:%s, %s", 
			m_index, 
		    ATDV_NAMEP(m_hDevTimeSlot),
		    ATDV_NAMEP(m_line),
		    ATDV_ERRMSGP(m_hDevTimeSlot),
		    ATDV_ERRMSGP(m_line));
		return FALSE;
	}

	// A/B-Signal
	if( gb_NT_CaptureABSignal )
	{
		if( sr_enbhdlr( m_hDevTimeSlot, DTEV_SIG, sig_hdlr ) == -1 ) 
		{
			WriteTrace(TraceError, "IVR Channel: %d - Cannot set handler: for (%s, %s) failed:%s, %s", 
				m_index, 
				ATDV_NAMEP(m_hDevTimeSlot),
				ATDV_NAMEP(m_line),
				ATDV_ERRMSGP(m_hDevTimeSlot),
				ATDV_ERRMSGP(m_line));
		}
		else if( dt_setsigmod( m_hDevTimeSlot, DTM_SIGINS ) == -1 )
		{
			WriteTrace(TraceError, "IVR Channel: %d - Cannot set SIGINS for (%s, %s) failed:%s, %s", 
				m_index, 
				ATDV_NAMEP(m_hDevTimeSlot),
				ATDV_NAMEP(m_line),
				ATDV_ERRMSGP(m_hDevTimeSlot),
				ATDV_ERRMSGP(m_line));
		}
		else if ( dt_setevtmsk( m_hDevTimeSlot, DTG_SIGEVT,
                     DTMM_AOFF | DTMM_AON | DTMM_BOFF | DTMM_BON, DTA_SETMSK) == -1 ) 
		{
			WriteTrace(TraceError, "IVR Channel: %d - Unable to set DTI signaling event mask for (%s, %s) failed:%s, %s", 
				m_index, 
				ATDV_NAMEP(m_hDevTimeSlot),
				ATDV_NAMEP(m_line),
				ATDV_ERRMSGP(m_hDevTimeSlot),
				ATDV_ERRMSGP(m_line));
		}
	}

	/*
	rc = gc_SetEvtMsk(m_dtidev, GCMSK_DETECTED, GCACT_ADDMSK);
	if( rc != EGC_NOERR )
	{
		WriteTrace(TraceError, "IVR Channel: %d - Global Call Func: ret(%d), gc_SetEvtMsk(%d)", m_index, rc, m_dtidev);
	}
	*/

	return TRUE;
}

void Cchannelcontrol::dti_releaseLineDev()
{
	int rc = gc_ResetLineDev(m_dtidev, EV_ASYNC);
	WriteTrace(TraceDebug, "IVR Channel: %d - Cchannelcontrol::cti_releaseLineDev - Return:%d, gc_ResetLineDev(%d)", m_index, rc, m_dtidev);
}

BOOL Cchannelcontrol::ivr_channel_route()
{
	if( m_bSCBus )
	{
		dx_unlisten(m_line);

		// Scbus function
		if( m_dtidev > 0 )
		{
			dt_unlisten(m_dtidev);
			if( nr_scroute(m_line, SC_VOX, m_hDevTimeSlot, SC_DTI, SC_FULLDUP) == -1)
				return(FALSE);
		}
		else
		{
			if( nr_scroute(m_line, SC_VOX, m_line, SC_LSI, SC_FULLDUP) == -1)
				return(FALSE);
		}
	}

	return TRUE;
}

#endif


BOOL Cchannelcontrol::ivr_channel_open(char *sChannelName, char *sDTIName, char *sFaxName)
{
	// Get Channel Name
	if( sChannelName == NULL )
		return FALSE;
	strcpy(m_sChannelName, sChannelName);

	// Get DTI Device Name
	if( sDTIName != NULL )
		if( sDTIName[0] != NULL )
			strcpy(m_sDTIName, sDTIName);

	// Get FAX Device Name
	if( sFaxName != NULL )
		if( sFaxName[0] != NULL )
			strcpy(m_sFAXName, sFaxName);

#ifdef INTEL_DIALOGIC
	int lv_tryTimes = 0;

	// Open Fax Channel
	if( sFaxName != NULL )
	{
		if( strlen(sFaxName) > 0 ) 
		{
			while( lv_tryTimes++ < 3 )
			{
				if( (m_faxdev = fx_open(sFaxName, NULL)) != -1 )
				{
					m_faxtype = ATFX_CHTYPE(m_faxdev);

					// Sun added 2006-10-01
					if( !m_blnBindFaxResource )
						gb_FaxManager.AddFaxResource(this, m_faxdev, m_faxtype);

					WriteTrace(TraceDebug, "IVR Channel: %d - FAX Line:%s opens FAX resource: %d", m_index, sFaxName, m_faxdev);
					break;
				}
				else
				{
					// Sun added 2012-08-02
					WriteTrace(TraceError, "IVR Channel: %d - FAX Line:%s failed to opens FAX resource! System Error No. - [%d]!!!", 
						m_index, sFaxName, dx_fileerrno());
					Sleep(1000);
					continue;
				}
			}

			// Sun added 2012-08-02
			if( m_faxdev == -1 )
				return FALSE;
		}
	}

	if( gb_NT_UserGlobalCall && 
		!(gb_NT_IncludeD41 && m_sDTIName[0] == NULL) )
	{
		if( m_sDTIName[0] == NULL || 
			gb_NT_Protocol.Find(_T("_an_")) >= 0 ||
			gb_NT_Protocol.Find(_T("_AN_")) >= 0 )
		{
			// Analog Interface
			if( !dti_init_anapi_channel() )
				return FALSE;

			// Add tone pattern
			if(	ivr_channel_addtone() == -1 )
				WriteTrace(TraceWarn, "IVR Channel: %d - Error when adding tones for linedev :%d (%s)", m_index, m_line, sChannelName);

			// Enable the CST events
			if ( dx_setevtmsk(m_line, DM_RINGS|DM_LCOFF) == -1 ) 
				return(FALSE);		

			// Set System Volume
			if(gb_VoiceVolume == 0)
				dx_adjsv(m_line, SV_VOLUMETBL, SV_ABSPOS, SV_NORMAL );
			else
				dx_adjsv(m_line, SV_VOLUMETBL, SV_ABSPOS, gb_VoiceVolume);

			// Set Ring times previous off hook
			dx_setparm(m_line, DXCH_RINGCNT, &gb_RingsPreOffhook);

			// Set the Channel to ON-HOOK state
			ivr_channel_hook(DX_ONHOOK);
		}
		else if( gb_NT_Protocol.CompareNoCase(_T("isdn")) == 0 )
		{
			// ISDN Interface
			if( !dti_init_isdn_channel() )
				return FALSE;
		}
		else
		{
			// ICAPI(E1) Interface
			if( !dti_init_icapi_channel() )
				return FALSE;
		}

		// Sun added 2002-12-02
		pChannelView->m_oIVRCHMap.SetAt(m_line, (CIVRChannel*)m_nTag);
		// End of Add
	}
	else
	{
		SC_TSINFO   sc_tsinfo;
		CRN			crn;
		long		scts;
		int			nchtype;

		if( ( m_line = dx_open(sChannelName, NULL)) == -1)
			return FALSE;

		if( (nchtype = ATDX_BDTYPE(m_line)) != AT_FAILURE )
			if( nchtype == DI_D41CH	 )
			  m_bSCBus = true;

		// Set pll
		// Sun added 2002-12-02
		m_nDevType = DEV_ANALOG;
		pChannelView->m_oIVRCHMap.SetAt(m_line, (CIVRChannel*)m_nTag);
		// End of Add

		// Open DTI Device
		if( sDTIName != NULL )
		{
			if( sDTIName[0] != NULL )
			{
				if( cc_Open( &m_dtidev, sDTIName, 0 ) < 0 )
					return FALSE;

				// Set pll
				// Sun added 2002-12-02
				m_nDevType = DEV_DIGIT;
				pChannelView->m_oIVRCHMap.SetAt(m_dtidev, (CIVRChannel*)m_nTag);
				// End of Add

				if( cc_WaitCall(m_dtidev, &crn, NULL, -1, EV_ASYNC) < 0 )
					return FALSE;		
			}
		}

		if( m_bSCBus )
		{
			sc_tsinfo.sc_numts = 1;
			sc_tsinfo.sc_tsarrayp = &scts;

			// get voice timeslot of current channel 
			if( dx_getxmitslot(m_line, &sc_tsinfo) == -1 )
			{
				m_bSCBus = false;
			}
			else
			{
				// get analog device timeslot of current channel 
				if( m_dtidev > 0 )
				{
					if( dt_getxmitslot(m_dtidev, &sc_tsinfo) == -1 )
						return FALSE;
					m_hDevTimeSlot = scts;
				}
				else
				{
					if( ag_getxmitslot(m_line, &sc_tsinfo) == -1 )
						return FALSE;
					m_hDevTimeSlot = scts;
				}
			}
		}

		// Add tone pattern
		if(	ivr_channel_addtone() == -1 )
			WriteTrace(TraceWarn, "IVR Channel: %d - Error when adding tones for linedev :%d (%s)", m_index, m_line, sChannelName);

		// Enable the CST events
		if ( dx_setevtmsk(m_line, DM_RINGS|DM_LCOFF) == -1 ) 
			return(FALSE);		

		// Set System Volume
		if(gb_VoiceVolume == 0)
			dx_adjsv(m_line, SV_VOLUMETBL, SV_ABSPOS, SV_NORMAL );
		else
			dx_adjsv(m_line, SV_VOLUMETBL, SV_ABSPOS, gb_VoiceVolume);

		// Set Ring times previous off hook
		dx_setparm(m_line, DXCH_RINGCNT, &gb_RingsPreOffhook);

		// Set the Channel to ON-HOOK state
		ivr_channel_hook(DX_ONHOOK);
		
		if( !ivr_channel_route() )
			return FALSE;
	}
#endif

#ifdef CISCO_CCM
	// Sun added 2013-03-06
	ivr_channel_waitring();

	// Open TAPI line device
	if( CTapiLine::Open() )
		return FALSE;

	// Adjust channel volume with global parameter 'gb_VoiceVolume'
	// Notes: Cisco TAPI 2.1 don't support volume adjustment
	//...

	// Add channel into device list
	m_nDevType = DEV_IP;
	pChannelView->m_oIVRCHMap.SetAt(CTapiLine::GetLineHandle(), (CIVRChannel*)m_nTag);
	WriteTrace(TraceInfo, "IVR Channel: %d bind with TapiLine: 0x%x.", m_index, CTapiLine::GetLineHandle());

#endif

#ifdef CISCO_CME
	m_nDevType = DEV_IP;
	for( int lv_nEventType = 0; lv_nEventType < MAX_CMESyncEvents; lv_nEventType++ )
		m_CMESyncEvent[lv_nEventType].m_nTag = m_index;
#endif

	return TRUE;
}

#ifdef INTEL_DIALOGIC

BOOL Cchannelcontrol::ivr_channel_dx_stop()
{
#ifndef DONGJIN

	int reco = 0;
	long lv_st;

	// Sun added 2012-07-13
	if( m_blnBindFaxResource )
		fax_channel_stopch();

	lv_st = ATDX_STATE(m_line);
	WriteTrace(TraceDebug, "IVR Channel: %d trys to Stop Channel in state %d...", m_index, lv_st);

	if( lv_st == CS_IDLE )
		return TRUE;

	// Stop Voice, sun updated 2012-06-01, EV_SYNC -> EV_ASYNC
	dx_stopch(m_line, EV_ASYNC);
	while(ATDX_STATE(m_line) != CS_IDLE)
	{
		if(reco > 10)
		{
			WriteTrace(TraceWarn, "IVR Channel: %d can't stop operation in ivr_channel_dx_stop(), state = %d!", m_index, ATDX_STATE(m_line));
			return FALSE;
		}
	    dx_stopch(m_line, EV_ASYNC);
		Sleep(50);
		reco ++;
	}
#endif

	return TRUE;
}
#endif

BOOL Cchannelcontrol::ivr_channel_stop_play()
{
	WriteTrace(TraceDebug, "IVR Channel: %d - will stop Channel Playing in ivr_channel_stop_play()...", m_index);

#ifdef INTEL_DIALOGIC
	// Stop Playing
	if( ATDX_STATE(m_line) == CS_PLAY )
		dx_stopch(m_line, EV_SYNC);
#endif

#ifdef CISCO_CCM
	// Stop Playing
	if( CTapiLine::IsPlaying() )
		CTapiLine::StopPlay();

	// Stop Recording
	if( CTapiLine::IsRecording() )
		CTapiLine::StopRecord();
#endif

#ifdef CISCO_CME
	// Stop Current Operation
	if( pMainWindow->m_CMEInterface.GetIntefaceStarted() )
	{
		pMainWindow->m_CMEInterface.m_piCommands->IVRStartStop(m_index, ECET_Calling, TRUE);
		pMainWindow->m_CMEInterface.m_piCommands->FreeThreadMemory();
	}

	// 停止所有正在进行的CME操作
	ResetAllSyncEvents();

#endif

	return TRUE;
}

BOOL Cchannelcontrol::ivr_channel_stop()
{
	WriteTrace(TraceDebug, "IVR Channel: %d trys to call ivr_channel_stop!", m_index);
	
#ifdef INTEL_DIALOGIC
	// Sun removed 2006-10-01
	// Stop FAX
	//if( m_faxdev > 0 )
	//	fx_stopch(m_faxdev, EV_SYNC);
	
	// Stop Voice
	ivr_channel_dx_stop();

	// Stop DTI Device
	if( m_dtidev > 0 )
	{
		if( gb_NT_UserGlobalCall )
		{	
			// Sun updated 2012-05-30
			//if( m_nDevType == DEV_ANALOG || m_nDevType == DEV_ISDN )
			if( m_nDevType == DEV_ANALOG )
				gc_ResetLineDev(m_dtidev, EV_SYNC);
		}
		else
		{
			cc_Restart(m_dtidev, EV_SYNC);
		}
	}
#endif

#ifdef CISCO_CCM
	// Stop Playing
	CTapiLine::SetTerminateCode(VOICE_TM_USRSTOP);
	ivr_channel_stop_play();
	CTapiLine::SetTerminateCode(VOICE_TM_USRSTOP);

	// Restart line device
	// Should testing whether necissary
	//CTapiLine::Restart();
#endif

#ifdef CISCO_CME
	// Stop Current Operation
	ivr_channel_stop_play();
#endif

	return TRUE;
}

BOOL Cchannelcontrol::ivr_channel_close()
{
#ifdef INTEL_DIALOGIC
	int		encoding = DF_MH;
	int		gc_error;	/* GlobalCall error code */ 
	int		cclibid;	/* Call Control Library ID */ 
	long	cc_error;	/* Call Control Library error code */ 
	char	*msg;		/* points to the error message string */ 

	WriteTrace(TraceDetail, "IVR Channel: %d - ARS Detail - ivr_channel_close(%d) will stop channel.", m_index, m_line);

	// Stop Channel
	ivr_channel_stop();

	// Set Hook On
	if( m_nDevType == DEV_ANALOG )
	{
		if( ATDX_HOOKST(m_line) == DX_OFFHOOK )
		{
			if( dx_sethook(m_line, DX_ONHOOK, EV_SYNC) == -1 )
			{
				WriteTrace(TraceWarn, "IVR Channel: %d - ARS Warn - ivr_channel_close(%d) failed to On-Hook.", m_index, m_line);
				return FALSE;
			}
		}
	}

	// Close Fax
	if( m_faxdev > 0 )
	{
		// Sun added 2006-10-01
		if( !m_blnBindFaxResource )
			gb_FaxManager.RemoveFaxResource(m_faxdev);

		if (m_faxtype == DFS_FAX40E || m_faxtype == DFS_FAX40EPLUS)
			fx_setparm(m_faxdev, FC_RXCODING, (void *)&encoding);
		fx_close(m_faxdev);
		WriteTrace(TraceDebug, "IVR Channel: %d - FAX Line:%d close FAX resource: %d", m_index, m_line, m_faxdev);
	}

	// Close DTI
	if( m_dtidev > 0 )
	{
		if( gb_NT_UserGlobalCall )
		{
			nr_scunroute(m_line, SC_VOX, m_hDevTimeSlot, SC_DTI, SC_FULLDUP);

			// Sun added 2005-6-15
			if( gb_NT_CaptureABSignal )
				sr_dishdlr( m_hDevTimeSlot, DTEV_SIG, sig_hdlr );

			if( gc_Close(m_dtidev) != GC_SUCCESS )
			{
				gc_ErrorValue( &gc_error, &cclibid, &cc_error); 
				gc_ResultMsg( LIBID_GC, (long) gc_error, &msg); 
				WriteTrace(TraceError, "IVR Channel: %d - Error closing linedev : gc_Close(%d), msg: %s ", 
					m_index, m_dtidev, msg);
				return FALSE;
			}
		}
		else
		{
			nr_scunroute(m_line, SC_VOX, m_hDevTimeSlot, SC_DTI, SC_FULLDUP);
			cc_Close(m_dtidev);
			// Sun added 2002-12-02
			pChannelView->m_oIVRCHMap.RemoveKey(m_dtidev);
			// End of Add
		}

		//-----------------------------------------------------------------------
		// Sun added 2012-06-26
		// 回收内存
		if( m_makecall_blk.gclib )
		{
			// free the parm block if used using the utility functions
			// CAUTION: this must be done before freeing the GC makecall block
			if( m_makecall_blk.gclib->ext_datap )
			{
				gc_util_delete_parm_blk(m_makecall_blk.gclib->ext_datap);
			}

			// now free the GC makecall block
			free(m_makecall_blk.gclib);
			m_makecall_blk.gclib = NULL;
		}
		//-----------------------------------------------------------------------
	}

	// Sun added 2002-12-02
	m_nDevType = DEV_NONE;
	pChannelView->m_oIVRCHMap.RemoveKey(m_line);
	// End of Add

	// close device
	dx_close(m_line);

	WriteTrace(TraceDetail, "IVR Channel: %d - ARS Detail - ivr_channel_close(%d) channel closed.", m_index, m_line);

	m_line = -1;
	m_dtidev = -1;
	m_faxdev = -1;
#endif

#ifdef CISCO_CCM
	// Stop Playing
	ivr_channel_stop_play();

	// Drop Call
	//....

	// Remove line from seeking list
	m_nDevType = DEV_NONE;
	pChannelView->m_oIVRCHMap.RemoveKey(CTapiLine::GetLineHandle());
	WriteTrace(TraceInfo, "IVR Channel: %d unbind with TapiLine: 0x%x.", m_index, CTapiLine::GetLineHandle());

	// Close line device
	CTapiLine::ShutDown();

	WriteTrace(TraceDetail, "IVR Channel: %d - ARS Detail - ivr_channel_close() channel closed.", m_index);
#endif

#ifdef CISCO_CME
	// Stop Current Operation
	ivr_channel_stop_play();

	// Remove line from seeking list
	m_nDevType = DEV_NONE;

	WriteTrace(TraceDetail, "IVR Channel: %d - ARS Detail - ivr_channel_close() channel closed.", m_index);
#endif

	return TRUE;
}

#ifdef CISCO_CCM
// Restart TAPI line
BOOL Cchannelcontrol::ivr_channel_restart()
{
	// Sun omitted 2010-07-20 [B]
	// Remove line from seeking list
	//pChannelView->m_oIVRCHMap.RemoveKey(CTapiLine::GetLineHandle());
	DWORD lv_dwNewHandle = NULL;
	DWORD lv_dwOldHandle = CTapiLine::GetLineHandle();
	WriteTrace(TraceInfo, "IVR Channel: %d should unbind with TapiLine: 0x%x.", m_index, lv_dwOldHandle);
	pChannelView->m_oIVRCHMap.RemoveKey(lv_dwOldHandle);

	// Sun updated 2008-05-19
	long lv_ret;
	lv_ret = CTapiLine::Restart();
	if( lv_ret == 5 )
	{
		// 需要重新初始化
		return FALSE;
	}

	// Add channel into device list
	lv_dwNewHandle = CTapiLine::GetLineHandle();
	if( lv_dwNewHandle > 0 )
	{
		if( lv_dwNewHandle != lv_dwOldHandle )
		{
			pChannelView->m_oIVRCHMap.SetAt(lv_dwNewHandle, (CIVRChannel*)m_nTag);
			WriteTrace(TraceInfo, "IVR Channel: %d bind with TapiLine: 0x%x old Handle: 0x%x.", m_index, lv_dwNewHandle, lv_dwOldHandle);
		}
	}
	else
		WriteTrace(TraceWarn, "IVR Channel: %d restarted TapiLine but can't get new Handle, old Handle: 0x%x.", m_index, lv_dwOldHandle);

	return TRUE;
}

#endif

BOOL Cchannelcontrol::ivr_channel_waitring()
{
#ifdef INTEL_DIALOGIC
	if( m_nDevType == DEV_ANALOG )
	{
		if( ATDX_HOOKST(m_line) == DX_OFFHOOK )
		{
			if (dx_sethook(m_line, DX_ONHOOK, EV_SYNC) == -1 ) 
				return FALSE;
		}
	}
	
	if (dx_wtring(m_line, gb_RingsPreOffhook, DX_OFFHOOK, -1) == -1)
		return FALSE;
#endif

#ifdef CISCO_CCM
	CTapiLine::SetRingCount(gb_RingsPreOffhook);
#endif

#ifdef CISCO_CME
	//TODOCME: Not supported bu now
#endif

	return TRUE;
}

BOOL Cchannelcontrol::ivr_channel_hook(int nhook, long callrefno, bool blnForce)
{
#ifdef INTEL_DIALOGIC
	int rc, call_state;
	GC_INFO t_Info;

	if( m_nDevType == DEV_ANALOG || blnForce )
	{
		if( ATDX_HOOKST(m_line) != nhook )
		{
			if( dx_sethook(m_line, nhook, EV_SYNC) == -1 ) 
			{
				WriteTrace(TraceWarn, "IVR Channel: %d - Error: ivr_channel_hook(%d) set hook on", m_index, m_line);
				return FALSE;
			}
		}
	}
		
	if( nhook == DX_ONHOOK )
	{
		// Stop DTI Device
		if( m_dtidev > 0 )
		{
			if( gb_NT_UserGlobalCall )
			{
				if( callrefno > 0 )
				{
					rc = gc_DropCall(callrefno, GC_NORMAL_CLEARING, EV_SYNC);
					WriteTrace(TraceDebug, "IVR Channel: %d called gc_DropCall(0x%x) and Ret: %d.", 
						m_index, callrefno, rc);
					if( rc < 0 )
					{
						rc = gc_ErrorInfo(&t_Info);
						if( rc == GC_SUCCESS )
						{
							call_state = 0;
							gc_GetCallState(callrefno, &call_state);
							WriteTrace(TraceDebug, "IVR Channel: %d - GC get gc_DropCall error info, gcValue = 0x%X, gcCallStatus = %d, gcMsg = %s, AdditionalInfo: %s", 
								m_index, t_Info.gcValue, call_state, t_Info.gcMsg, t_Info.additionalInfo);
						}
					}

					rc = gc_ReleaseCall(callrefno);
					WriteTrace(TraceDebug, "IVR Channel: %d called gc_ReleaseCall(0x%x) and Ret: %d.", 
						m_index, callrefno, rc);

					// Sun added 2012-05-30
					if( rc < 0 )
						dti_releaseLineDev();
				}
			}
			else
			{
				if( callrefno > 0 )
				{
					cc_DropCall(callrefno, NORMAL_CLEARING, EV_SYNC);
					cc_ReleaseCall(callrefno);
				}
			}
		}
		if( m_nDevType == DEV_ANALOG )
			dx_setevtmsk(m_line, DM_RINGS);			// wait ring...
	}
#endif

#ifdef CISCO_CCM
	// Stop Playing
	ivr_channel_stop_play();

	if( nhook == DX_ONHOOK )
	{
		// Drop Call
		CTapiLine::DropCall();
		CTapiLine::TestTPTCondition();
	}
	else
	{
		// Answer Call
		CTapiLine::AnswerCall();
	}

	WriteTrace(TraceDetail, "IVR Channel: %d - ARS Detail - ivr_channel_hook() set hook state to %d.", m_index, nhook);
#endif

#ifdef CISCO_CME
	// Stop Playing
	ivr_channel_stop_play();

	if( nhook == DX_ONHOOK )
	{
		// Drop Call
		SetTerminateCode(CMESyncEvent_Common, VOICE_TM_Dummy);
		if( pMainWindow->m_CMEInterface.GetIntefaceStarted() )
		{
			pMainWindow->m_CMEInterface.m_piCommands->IVRStartHangup(m_index, FALSE);
			pMainWindow->m_CMEInterface.m_piCommands->FreeThreadMemory();
		}
	}
	else
	{
		// TODOCME: don't supported by now
		// Answer Call
		//SetTerminateCode(CMESyncEvent_Common, VOICE_TM_Dummy);
		//if( pMainWindow->m_CMEInterface.GetIntefaceStarted() )
		//{
		//	pMainWindow->m_CMEInterface.m_piCommands->IVRAnswerCall(m_index);
		//	pMainWindow->m_CMEInterface.m_piCommands->FreeThreadMemory();
		//}
	}

	WriteTrace(TraceDetail, "IVR Channel: %d - ARS Detail - ivr_channel_hook() set hook state to %d.", m_index, nhook);
#endif

	return TRUE;
}

BOOL Cchannelcontrol::ivr_channel_terminate()
{
	try
	{
		CIVRChannel* lv_pCH = (CIVRChannel*)m_nTag;
		if( lv_pCH == NULL )
			return TRUE;

		if( !lv_pCH->GetIsInCall() )
			return TRUE;
	}
	catch(...)
	{
		return TRUE;
	}

	return FALSE;
}

/***************************************************************************
*        NAME: int getdigmask(char mask[])
* DESCRIPTION: return a maskkey for DV_TPT
*       INPUT: char mask[]
*      OUTPUT: 
*     RETURNS: maskkey
*    CAUTIONS: None
***************************************************************************/
int Cchannelcontrol::getdigmask(char mask[])
{
    size_t i,len;
    int digMask = 0;

	len = strlen(mask);
    for(i = 0; i < len; i++)
	{
	    if (mask[i] == '\0')
		    break;

		digMask |= GetDigitMask(mask[i]);

    }
    return digMask;
}

#ifdef INTEL_DIALOGIC
void Cchannelcontrol::ivr_channel_setcapp(DX_CAP *capp)
{
   capp->ca_nbrdna=7;           /* # of rings before no answer.             */
   capp->ca_stdely=25;          /* Delay after dialing before analysis.     */
   capp->ca_cnosig=4000;        /* Duration of no signal time out delay.    */
   capp->ca_lcdly=400;          /* Delay after dial before lc drop connect  */
   capp->ca_lcdly1=10;          /* Delay after lc drop con. before msg.     */
   capp->ca_hedge=1;            /* Edge of answer to send connect message.  */
   capp->ca_cnosil=650;         /* Initial continuous noise timeout delay.  */
   capp->ca_lo1tola=8;          /* % acceptable pos. dev of short low sig.  */
   capp->ca_lo1tolb=8;          /* % acceptable neg. dev of short low sig.  */
   capp->ca_lo2tola=10;         /* % acceptable pos. dev of long low sig.   */
   capp->ca_lo2tolb=8;          /* % acceptable neg. dev of long low sig.   */
   capp->ca_hi1tola=8;          /* % acceptable pos. dev of high signal.    */
   capp->ca_hi1tolb=47;         /* % acceptable neg. dev of high signal.    */
   capp->ca_lo1bmax=57;         /* Maximum interval for shrt low for busy.  */
   capp->ca_lo2bmax=57;         /* Maximum interval for long low for busy.  */
   capp->ca_hi1bmax=59;         /* Maximum interval for 1st high for busy   */
   capp->ca_nsbusy=0;           /* Num. of highs after nbrdna busy check.   */
   capp->ca_logltch=6;          /* Silence deglitch duration.               */
   capp->ca_higltch=6;          /* Non-silence deglitch duration.           */
   capp->ca_lo1rmax=1;          /* Max. short low  dur. of double ring.     */
   capp->ca_lo2rmin=188;        /* Min. long low  dur. of double ring.      */
   capp->ca_intflg=DX_PAMDOPTEN;/* Operator intercept mode.                 */
   capp->ca_intfltr=10;         /* Minimum signal to qualify freq. detect.  */
   capp->ca_hisiz=90;           /* Used to determine which lowmax to use.   */
   capp->ca_alowmax=209;        /* Max. low before con. if high >hisize.    */
   capp->ca_blowmax=209;        /* Max. low before con. if high <hisize.    */
   capp->ca_nbrbeg=2;           /* Number of rings before analysis begins.  */
   capp->ca_hi1ceil=78;         /* Maximum 2nd high dur. for a retrain.     */
   capp->ca_lo1ceil=58;         /* Maximum 1st low dur. for a retrain.      */
   capp->ca_lowerfrq=900;       /* Lower allowable frequency in hz.         */
   capp->ca_upperfrq=1000;      /* Upper allowable frequency in hz.         */
   capp->ca_timefrq=5;          /* Total duration of good signal required.  */
   capp->ca_rejctfrq=20;        /* Allowable % of bad signal.               */
   capp->ca_maxansr=1000;       /* Maximum duration of answer.              */
   capp->ca_ansrdgl=3;          /* Silence deglitching value for answer.    */
   //capp->ca_mxtimefrq;        /*max time for 1st freq to remain in bounds */
   //capp->ca_lower2frq;        /*lower bound for second frequency          */
   //capp->ca_upper2frq;        /*upper bound for second frequency          */
   //capp->ca_time2frq;         /*min time for 2nd freq to remains in bounds*/
   //capp->ca_mxtime2frq;       /*max time for 2nd freq to remain in bounds */
   //capp->ca_lower3frq;        /*lower bound for third frequency           */
   //capp->ca_upper3frq;        /*upper bound for third frequency           */
   //capp->ca_time3frq;         /*min time for 3rd freq to remains in bounds*/
   //capp->ca_mxtime3frq;       /*max time for 3rd freq to remain in bounds */
   capp->ca_dtn_pres=1;         /* Length of a valid dial tone (def=1sec)   */
   capp->ca_dtn_npres=5;        /* Max time to wait for dial tone (def=3sec)*/
   capp->ca_dtn_deboff=100;     /* The dialtone off debouncer (def=100ms)   */
   capp->ca_pamd_failtime=2;    /* Wait for AMD/PVD after cadence break(def=4sec)*/
   capp->ca_pamd_minring=190;   /* min allowable ring duration (def=1.9sec)*/
   capp->ca_pamd_spdval=PAMD_QUICK; /*Set to 2 selects quick decision (def=1) */
   capp->ca_pamd_qtemp=PAMD_QUAL1TMP;/*The Qualification template to use for PAMD */
   //capp->ca_noanswer;         /*time before no answer after first ring (def=30sec) */
   //capp-> ca_maxintering;     /*   Max inter ring delay before connect (8 sec) */
}
#endif

int	Cchannelcontrol::ivr_channel_sendDTMF(const char *szdial, bool blnAsync)
{
	WriteTrace(TraceDebug, "IVR Channel: %d will call ivr_channel_sendDTMF(%s)", m_index, szdial);

#ifdef INTEL_DIALOGIC
	
	DX_CAP cap;
	dx_clrcap(&cap);

	if( szdial != NULL && *szdial != '\0')
	{
		if( m_nDevType == DEV_ANALOG )
		{
			if( ATDX_HOOKST(m_line) != DX_OFFHOOK )
				return -1;
		}

		cap.ca_nbrdna = 1;						// defual 4:无人应答等待震铃次数
		cap.ca_stdely = 20;						// d:25拨号完开始分析之间的延时
		cap.ca_dtn_pres = 100;
		cap.ca_dtn_npres = 300;					// 等待拨号音时间
		cap.ca_dtn_deboff = 10;
		cap.ca_noanswer = 1000;					// nTimeout * 100;
		cap.ca_hedge = 2;						// 1上升缘采样2下降缘采样 
		//cap.ca_intflg = DX_PVDOPTNOCON;
		cap.ca_intflg = DX_PVDOPTEN;
		//cap.ca_intflg = DX_OPTDIS;
		//cap.ca_intflg = DX_OPTNOCON;

		if( blnAsync )
		{
			if( dx_dial(m_line, szdial, (DX_CAP *)&cap, EV_ASYNC) == -1 )
			{
				WriteTrace(TraceError, "IVR Channel: %d - Fail ivr_channel_sendDTMF(%s, ASYNC)...channel:%u", m_index, szdial, m_line);
				return 0;
			}
		}
		else
		{
			if( dx_dial(m_line, szdial, (DX_CAP *)&cap, EV_SYNC) == -1 )
			{
				WriteTrace(TraceError, "IVR Channel: %d - Fail ivr_channel_sendDTMF(%s)...channel:%u", m_index, szdial, m_line);
				return 0;
			}
		}
	}
	else
	{
		WriteTrace(TraceWarn, "IVR Channel: %d - No ivr_channel_sendDTMF number...channel:%u", m_index, m_line);
		return 0;
	}

	WriteTrace(TraceDebug, "IVR Channel: %d - Success ivr_channel_sendDTMF(%s)...channel:%u", m_index, szdial, m_line);

#endif

#ifdef CISCO_CCM

	long ret;
	ret = CTapiLine::SendDTFM(szdial);
	if( ret != 0 )
		WriteTrace(TraceError, "IVR Channel: %d - Fail ivr_channel_sendDTMF(%s)...channel:%u, error code = 0x%x", m_index, szdial, m_index, ret);
	else
		WriteTrace(TraceDebug, "IVR Channel: %d - Success ivr_channel_sendDTMF(%s)...channel:%u", m_index, szdial, m_index);

#endif

#ifdef CISCO_CME

	IPCSParam *retparam = NULL;
	
	SetTerminateCode(CMESyncEvent_Common, VOICE_TM_Dummy);
	if( pMainWindow->m_CMEInterface.GetIntefaceStarted() )
	{
		retparam = pMainWindow->m_CMEInterface.m_piCommands->IVRStartPlayDTMF(m_index, ECET_Calling, szdial, TRUE, FALSE);
		if( m_CMESyncEvent[CMESyncEvent_Common].Wait(INFINITE) )
		{
			retparam = &(m_CMEEventParam[CMESyncEvent_Common]);
			if( retparam->GetString("result") != "success" )
			{
				WriteTrace(TraceError, "IVR Channel: %d - Fail ivr_channel_sendDTMF(%s)...channel:%u, error = %d (%s)", 
				m_index, szdial, m_index, retparam->GetInt("reason"), retparam->GetInt("errordescription"));
			}
		}
		pMainWindow->m_CMEInterface.m_piCommands->FreeThreadMemory();
	}

#endif

	return 1;
}

#ifdef INTEL_DIALOGIC
void build_makecall_blk( MAKECALL_BLK *makecall_blk )
{
      memset(makecall_blk,0xff,sizeof(MAKECALL_BLK));
      makecall_blk->isdn.BC_xfer_cap=BEAR_CAP_SPEECH;
      makecall_blk->isdn.BC_xfer_mode=ISDN_ITM_CIRCUIT;
      makecall_blk->isdn.BC_xfer_rate=BEAR_RATE_64KBPS;

      makecall_blk->isdn.facility_coding_value = 0x03;
      makecall_blk->isdn.destination_number_type = NAT_NUMBER;//national num
      makecall_blk->isdn.destination_number_plan = ISDN_NUMB_PLAN;
      makecall_blk->isdn.origination_number_type = ISDN_NOTUSED;
      makecall_blk->isdn.origination_number_plan = ISDN_NOTUSED;
      makecall_blk->isdn.origination_phone_number[0] = '\0';
	  makecall_blk->isdn.facility_feature_service = ISDN_SERVICE;
      makecall_blk->isdn.usrinfo_layer1_protocol = ISDN_NOTUSED;

      makecall_blk->isdn.usr_rate = ISDN_NOTUSED;//user rate - Default
      makecall_blk->isdn.usrinfo_bufp = NULL; 
      makecall_blk->isdn.nsfc_bufp = NULL;
      makecall_blk->isdn.destination_sub_number_type = OSI_SUB_ADDR;//NSAP(X.213/ISO 8348 AD2)
      makecall_blk->isdn.destination_sub_phone_number[0] = '\0';
}

int Cchannelcontrol::set_calling_num(char *szANI)
{
#ifdef INTEL_DIALOGIC

	if( szANI )
	{
		int cclibid;		// cclib id for gc_ErrorValue
		int gc_error;		// GlobalCall error code
		long cc_error;		// Call Control Library error code
		char *msg;			// points to the error message string

		if( m_nDevType == DEV_ICAPI )
		{
			if( gc_SetCallingNum(m_dtidev, szANI) != GC_SUCCESS )
			{ 
				// process error return as shown
				gc_ErrorValue( &gc_error, &cclibid, &cc_error );
				gc_ResultMsg( LIBID_GC, (long) gc_error, &msg ); 
				WriteTrace(TraceError, "IVR Channel: %d - Error when calling gc_SetCallingNum on Device handle: 0x%lx, ErrorValue: %d - %s", 
					m_index, m_dtidev, gc_error, msg); 
			}
			else
			{
				WriteTrace(TraceDebug, "IVR Channel: %d - set calling number (ANI): %s Success", m_index, szANI); 
			}
		}
		else if( m_nDevType == DEV_ISDN )
		{
			/*******
			-- Set the called number plan to unknown 
			-- Modified, as this causes issues on some switches
			-- - UNKNOWN works on 99% of E1 switches
			--	gclib_makecallp->destination.address_plan = GCADDRPLAN_ISDN;
			*******/
			m_pgclib_makecall->destination.address_plan = GCADDRPLAN_UNKNOWN;

			/*******
			-- Set the address type to transparent
			-- Modified, as this causes issues on some switches 
			-- - TRANSPARENT works on 99% of E1 switches
			--	gclib_makecallp->destination.address_type = GCADDRTYPE_NAT;
			*******/
			m_pgclib_makecall->destination.address_type = GCADDRTYPE_TRANSPARENT;

			/*******
			-- Removed sub-addresses, as this causes issues on some switches
			--	strcpy(gclib_makecallp->destination.sub_address, "456");
			--	gclib_makecallp->destination.sub_address_plan = 0;
			--	gclib_makecallp->destination.sub_address_type = GCSUBADDR_OSI;
			*******/
			m_pgclib_makecall->destination.sub_address[0] = '\x0';
			m_pgclib_makecall->destination.sub_address_plan = 0;
			m_pgclib_makecall->destination.sub_address_type = 0;

			/*******
			-- Note to customers: make sure that the Calling Address is set correctly, as some switches
			-- could reject the call if the calling address is not correct.
			*******/
			memset(m_pgclib_makecall->origination.address, 0x00, sizeof(m_pgclib_makecall->origination.address));
			strcpy(m_pgclib_makecall->origination.address, szANI);

			/*******
			-- Set the calling number plan to ISDN type
			-- Modified, as this causes issues on some switches
			-- gclib_makecallp->origination.address_plan = GCADDRPLAN_ISDN;
			*******/
			m_pgclib_makecall->origination.address_plan = GCADDRPLAN_UNKNOWN;

			/*******
			-- Set the address type
			-- Modified, as this causes issues on some switches
			-- gclib_makecallp->origination.address_type = GCADDRTYPE_NAT;
			*******/
			m_pgclib_makecall->origination.address_type = GCADDRTYPE_TRANSPARENT;

			/*******
			-- Removed sub-addresses, as this causes issues on some switches
			--	strcpy(gclib_makecallp->origination.sub_address, "456");
			-- gclib_makecallp->origination.sub_address_plan = 0;
			-- gclib_makecallp->origination.sub_address_type = GCSUBADDR_OSI;
			*******/
			m_pgclib_makecall->origination.sub_address[0] = '\x0';
			m_pgclib_makecall->origination.sub_address_plan = 0;
			m_pgclib_makecall->origination.sub_address_type = 0;

			m_pgclib_makecall->call_info.address_info = GCADDRINFO_ENBLOC;

			WriteTrace(TraceDebug, "IVR Channel: %d - set ISDN calling number (ANI): %s Success", m_index, szANI); 

			return 1;
		}
	}

#endif

	return 0;
}

void build_makecall_blk( GC_MAKECALL_BLK *makecall_blk )
{
      memset(makecall_blk,0xff,sizeof(MAKECALL_BLK));
}
#endif

int Cchannelcontrol::ivr_channel_dailout(const char *szdial, char *szANI, int nTimeout, bool blnAsync, long *callrefno)
{
	if( szdial == NULL || *szdial == '\0')
		return -1;

	int nResult = VOICE_OBT_FAILED;

#ifdef INTEL_DIALOGIC

	int ndxRet;
	char strTemp[50];

	DX_CAP cap;

	if( m_nDevType == DEV_ANALOG )
	{
		// Off-hook if phone is idle
		if( ATDX_HOOKST(m_line) != DX_OFFHOOK )
			ivr_channel_hook(DX_OFFHOOK);
		TRACE("CALL: %s\n", szdial);

		//Sleep(100);
		dx_clrcap(&cap);

		/*
		cap.ca_nbrdna = (int)(nTimeout / 6);	// defual 4:无人应答等待震铃次数
		//cap.ca_stdely = 5;					// d:25拨号完开始分析之间的延时
		cap.ca_dtn_pres = 200;
		cap.ca_dtn_npres = 500;					// 等待拨号音时间
		cap.ca_dtn_deboff = 10;
		cap.ca_noanswer = 500;					// nTimeout * 100;
		cap.ca_hedge = 2;						// 1上升缘采样2下降缘采样 
		cap.ca_intflg = DX_PAMDOPTEN;
		*/
		cap.ca_nbrdna = 4;						// defual 4:无人应答等待震铃次数
		cap.ca_stdely = 20;						// d:25拨号完开始分析之间的延时
		cap.ca_dtn_pres = 100;
		cap.ca_dtn_npres = 300;					// 等待拨号音时间
		cap.ca_dtn_deboff = 10;
		cap.ca_noanswer = 400;					// nTimeout * 100;
		cap.ca_hedge = 2;						// 1上升缘采样2下降缘采样 
		cap.ca_intflg = DX_PAMDOPTEN;
		//cap.ca_pamd_minring = 1500;
		//cap.ca_maxintering = 600;

		if( szdial[0] == '&' )
		{
			cap.ca_nbrdna = 1;
			cap.ca_noanswer = 100;
			//cap.ca_intflg = DX_PVDOPTNOCON;
			//cap.ca_intflg = DX_PVDENABLE;
			cap.ca_intflg = DX_PVDOPTEN;			
			//cap.ca_intflg = DX_OPTNOCON;		// DX_OPTNOCON, DX_OPTDIS
		}

		if( blnAsync )
		{
			ndxRet = dx_dial(m_line, szdial, (DX_CAP *)&cap, DX_CALLP|EV_ASYNC);
			if( ndxRet == -1 )
			{
				WriteTrace(TraceError, "IVR Channel: %d - Fail ivr_channel_dailout(%s, async)...channel:%u", m_index, szdial, m_line);
				return -1;
			}
			nResult = VOICE_OBT_SUCC;
			WriteTrace(TraceDebug, "IVR Channel: %d - Finish ivr_channel_dailout(%s, async) finished...channel:%u\n", m_index, szdial, m_line);
		}
		else
		{
			ndxRet = dx_dial(m_line, szdial, (DX_CAP *)&cap, DX_CALLP|EV_SYNC);
			if( ndxRet == -1 )
			{
				WriteTrace(TraceError, "IVR Channel: %d - Fail ivr_channel_dailout(%s)...channel:%u", m_index, szdial, m_line);
				return -1;
			}

			switch( ndxRet )
			{
			case CR_BUSY:
				sprintf(strTemp, "Line Busy");
				nResult = VOICE_OBT_BUSY;
				break;
			case CR_CEPT:
				sprintf(strTemp, "Operator Intercept");
				nResult = VOICE_OBT_BREAK;
				break;
			case CR_CNCT:
				sprintf(strTemp, "Connection Succes");
				nResult = VOICE_OBT_SUCC;
				break;
			case CR_ERROR:
				sprintf(strTemp, "Error condition");
				nResult = VOICE_OBT_STATUS;
				break;
			case CR_FAXTONE:
				sprintf(strTemp, "FAX tone detected");
				nResult = VOICE_OBT_FAX;
				break;
			case CR_NOANS:
				sprintf(strTemp, "No answer");
				nResult = VOICE_OBT_NOANSWER;
				break;
			case CR_NODIALTONE:
				sprintf(strTemp, "No dialtone detected");
				nResult = VOICE_OBT_NOTONE;
				break;
			case CR_NORB:
				sprintf(strTemp, "No ringback detected");
				nResult = VOICE_OBT_NORINGBACK;
				break;
			case CR_STOPD:
				sprintf(strTemp, "Call analysis stopped");
				nResult = VOICE_OBT_CAPERROR;
				break;
			}
			WriteTrace(TraceDebug, "IVR Channel: %d - Finish ivr_channel_dailout(%s) with result:%s...channel:%u", m_index, szdial, strTemp, m_line);
		}
	}
	else if( m_dtidev > 0 )
	{
		if( gb_NT_UserGlobalCall )
		{
			// gc_MakeCall
			GC_MAKECALL_BLK *lv_pMakeCall = NULL;
			int rc;

			//build_makecall_blk(&makecall_blk);

			// Sun added 2012-06-26
			if( szANI != NULL )
			{
				if( set_calling_num(szANI) ==  1 )
					lv_pMakeCall = &m_makecall_blk;
			}

			if( blnAsync )
			{
				if( ( rc = gc_MakeCall(m_dtidev, callrefno, 
					(char *)szdial, lv_pMakeCall, 0, EV_ASYNC) ) != EGC_NOERR )
				{
					WriteTrace(TraceError, "IVR Channel: %d - Fail gc_MakeCall(%s, async)... Channel:%d (reason = %x), MakeCall Addr: 0x%x", 
						m_index, szdial, m_line, rc, lv_pMakeCall);
					return -1;
				}
				nResult = VOICE_OBT_SUCC;
			}
			else
			{
				if( ( rc = gc_MakeCall(m_dtidev, callrefno, 
					(char *)szdial, lv_pMakeCall, nTimeout, EV_SYNC) ) < 0 )
				{
					int cclibid;		// cclib id for gc_ErrorValue()
					int gc_error;		// GlobalCall error code 
					long cc_error;		// Call Control Library error code
					char *gc_msg;		// points to the error message string

					gc_ErrorValue( &gc_error, &cclibid, &cc_error); 
					gc_ResultMsg( LIBID_GC, (long) gc_error, &gc_msg); 
					WriteTrace(TraceError, "IVR Channel: %d - Fail gc_MakeCall(%s, sync)... Channel:%d (reason = %x) ErrorValue: %d - %s, MakeCall Addr: 0x%x", 
						m_index, szdial, m_line, rc, gc_error, gc_msg, lv_pMakeCall);

					// Sun added 2012-07-04
					if( *callrefno != 0 )
					{
						WriteTrace(TraceWarn, "IVR Channel: %d - trys to clear call because of gc_MakeCall() error but get GC callrefid 0x%x", 
							m_index, *callrefno);
						ivr_channel_hook(DX_ONHOOK, *callrefno);
						*callrefno = 0;
					}

					return -1;
				}
				nResult = VOICE_OBT_SUCC;
				
				// Sun added 2012-07-04
				int call_state = 0;
				if( *callrefno > 0 )
					gc_GetCallState(*callrefno, &call_state);
				WriteTrace(TraceDebug, "IVR Channel: %d - Finish gc_MakeCall(%s, %d) with rc:%d...channel:%u, gcCallStatus(0x%x) = %d", 
					m_index, szdial, nTimeout, rc, m_line, *callrefno, call_state);
			}
		}
		else
		{
			// cc_MakeCall
			MAKECALL_BLK makecall_blk;
			int reason;
			char *msg;

			build_makecall_blk(&makecall_blk);

			if( blnAsync )
			{
				if( cc_MakeCall(m_dtidev, callrefno, 
					(char *)szdial, &makecall_blk, 0, EV_ASYNC) < 0 )
				{
					reason = cc_CauseValue(m_dtidev);
					cc_ResultMsg(m_dtidev, reason, &msg);
					WriteTrace(TraceError, "IVR Channel: %d - Fail ivr_channel_dailout(%s, async)... Channel:%d (reason = %x - %s)", 
						m_index, szdial, m_line, reason, msg);
					return -1;
				}
				nResult = VOICE_OBT_SUCC;
			}
			else
			{
				if( (ndxRet = cc_MakeCall(m_dtidev, callrefno, 
					(char *)szdial, &makecall_blk, 0, EV_SYNC)) < 0 )
				{
					reason = cc_CauseValue(m_dtidev);
					cc_ResultMsg(m_dtidev, reason, &msg);
					WriteTrace(TraceError, "IVR Channel: %d - Fail ivr_channel_dailout(%s, sync)... Channel:%d (reason = %x - %s)", 
						m_index, szdial, m_line, reason, msg);
					return -1;
				}

				switch( ndxRet )
				{
				case CR_BUSY:
					sprintf(strTemp, "Line Busy");
					nResult = VOICE_OBT_BUSY;
					break;
				case CR_CEPT:
					sprintf(strTemp, "Operator Intercept");
					nResult = VOICE_OBT_BREAK;
					break;
				case CR_CNCT:
					sprintf(strTemp, "Connection Succes");
					nResult = VOICE_OBT_SUCC;
					break;
				case CR_ERROR:
					sprintf(strTemp, "Error condition");
					nResult = VOICE_OBT_STATUS;
					break;
				case CR_FAXTONE:
					sprintf(strTemp, "FAX tone detected");
					nResult = VOICE_OBT_FAX;
					break;
				case CR_NOANS:
					sprintf(strTemp, "No answer");
					nResult = VOICE_OBT_NOANSWER;
					break;
				case CR_NODIALTONE:
					sprintf(strTemp, "No dialtone detected");
					nResult = VOICE_OBT_NOTONE;
					break;
				case CR_NORB:
					sprintf(strTemp, "No ringback detected");
					nResult = VOICE_OBT_NORINGBACK;
					break;
				case CR_STOPD:
					sprintf(strTemp, "Call analysis stopped");
					nResult = VOICE_OBT_CAPERROR;
					break;
				}
				WriteTrace(TraceDebug, "IVR Channel: %d - Finish ivr_channel_dailout(%s) with result:%s...channel:%u\n", 
					m_index, szdial, strTemp, m_line);
			}
		}
	}
#endif

#ifdef CISCO_CCM

	long ret;
	ret = CTapiLine::MakeCall(szdial);
	if( ret != 0 )
		WriteTrace(TraceError, "IVR Channel: %d - Fail ivr_channel_dailout(%s)...channel:%u, error code = 0x%x", m_index, szdial, m_index, ret);
	else
	{
		nResult = VOICE_OBT_SUCC;
		WriteTrace(TraceDebug, "IVR Channel: %d - Finish ivr_channel_dailout(%s)...channel:%u", m_index, szdial, m_index);
	}

#endif

#ifdef CISCO_CME

	// TODOCME: don't supported by now
	/*
	long ret = NULL;
	//SetTerminateCode(VOICE_TM_Dummy);
	if( pMainWindow->m_CMEInterface.GetIntefaceStarted() )
	{
		ret = pMainWindow->m_CMEInterface.m_piCommands->IVRMakeCall(m_index, TRUE);
		if( ret != 0 )
			WriteTrace(TraceError, "IVR Channel: %d - Fail ivr_channel_sendDTMF(%s)...channel:%u, error code = 0x%x", m_index, szdial, m_index, ret);
		else
		{
			nResult = VOICE_OBT_SUCC;
			WriteTrace(TraceDebug, "IVR Channel: %d - Finish ivr_channel_dailout(%s)...channel:%u", m_index, szdial, m_index);
		}
		pMainWindow->m_CMEInterface.m_piCommands->FreeThreadMemory();
	}
	*/

#endif

	return nResult;
}

// Sun updated 2007-06-14
int Cchannelcontrol::ivr_channel_divert(const char *szdial, long callrefno)
{
	int nResult = VOICE_OBT_FAILED;

#ifdef INTEL_DIALOGIC
	DX_CAP	capp;
	dx_clrcap(&capp);
	
if( m_nDevType == DEV_ANALOG )
{

	ivr_channel_setcapp(&capp);
	if (szdial != NULL && *szdial != '\0') {
		if( ATDX_HOOKST(m_line) != DX_OFFHOOK )
			ivr_channel_hook(DX_OFFHOOK);
		TRACE("Divert: %s\n", szdial);

#ifndef DONGJIN
		dx_dial(m_line, "&, ", (DX_CAP *)NULL, EV_SYNC);
#endif

// Sun replaced 2009-06-27
/// From
/*
#ifdef PCS_ASYN_DIVERT
		if (dx_dial(m_line, szdial, (DX_CAP *)NULL, DX_CALLP|EV_ASYNC) == -1) {
#else

#ifndef DONGJIN
		if (dx_dial(m_line, szdial, (DX_CAP *)NULL, EV_SYNC) == -1) {
#else
		char lv_szDialNum[24];
		memset(lv_szDialNum, 0x00, sizeof(lv_szDialNum));
		sprintf(lv_szDialNum, "&,%s", szdial);
		if (dx_dial(m_line, lv_szDialNum, (DX_CAP *)NULL, EV_SYNC) == -1) {
#endif

#endif
			dx_dial(m_line, "&", (DX_CAP *)NULL, EV_SYNC);
			WriteTrace(TraceError, "IVR Channel: %d - Fail ivr_channel_divert dial...channel:%u", m_index, m_line);
			return 0;
		}
	}
	else {
		WriteTrace(TraceWarn, "IVR Channel: %d - No ivr_channel_divert number...channel:%u", m_index, m_line);
		return 0;
	}

#ifdef PCS_ASYN_DIVERT
	Sleep(5000);
#else
	ivr_channel_hook(DX_ONHOOK);
#endif
*/
/// To
	if( g_blnAnalogAsynDivert )
	{
		if( dx_dial(m_line, szdial, (DX_CAP *)NULL, DX_CALLP|EV_ASYNC) == -1 )
		{
			dx_dial(m_line, "&", (DX_CAP *)NULL, EV_SYNC);
			WriteTrace(TraceError, "IVR Channel: %d - Fail ivr_channel_divert async dial: %s...channel:%u", m_index, szdial, m_line);
			return 0;
		}
	}
	else
	{
#ifndef DONGJIN
		if( dx_dial(m_line, szdial, (DX_CAP *)NULL, EV_SYNC) == -1 )
		{
#else
		char lv_szDialNum[24];
		memset(lv_szDialNum, 0x00, sizeof(lv_szDialNum));
		sprintf(lv_szDialNum, "&,%s", szdial);
		if( dx_dial(m_line, lv_szDialNum, (DX_CAP *)NULL, EV_SYNC) == -1 )
		{
#endif

			dx_dial(m_line, "&", (DX_CAP *)NULL, EV_SYNC);
			WriteTrace(TraceError, "IVR Channel: %d - Fail ivr_channel_divert: %s dial...channel:%u", m_index, szdial, m_line);
			return 0;
		}
	}
	}
	else
	{
		WriteTrace(TraceWarn, "IVR Channel: %d - No ivr_channel_divert number...channel:%u", m_index, m_line);
		return 0;
	}

	if( g_blnAnalogAsynDivert )
	{
		Sleep(5000);
	}
}
else if( m_dtidev > 0 )
{
	// Global Call or Call Control
	if( gb_NT_UserGlobalCall )
	{
		// gc_MakeCall
		//GC_MAKECALL_BLK makecall_blk;
		int rc;

		//build_makecall_blk(&makecall_blk);

		int cclibid;		// cclib id for gc_ErrorValue()
		int gc_error;		// GlobalCall error code 
		long cc_error;		// Call Control Library error code
		char *msg;			// points to the error message string

#ifdef PCS_ASYN_DIVERT
		if( ( rc = gc_BlindTransfer(callrefno, (char *)szdial, NULL, 0, EV_ASYNC) ) != EGC_NOERR )
		{
			gc_ErrorValue(&gc_error, &cclibid, &cc_error); 
			gc_ResultMsg(LIBID_GC, (long) gc_error, &msg); 
			WriteTrace(TraceError, "IVR Channel: %d - Fail ivr_channel_divert(%s, async)... Channel:%d (reason = %x), error:%d - %s", 
				m_index, szdial, m_line, rc, gc_error, msg);
			return -1;
		}
		nResult = VOICE_OBT_SUCC;
#else
		if( ( rc = gc_BlindTransfer(callrefno, (char *)szdial, NULL, 0, EV_SYNC) ) < 0 )
		{
			gc_ErrorValue(&gc_error, &cclibid, &cc_error); 
			gc_ResultMsg(LIBID_GC, (long) gc_error, &msg); 
			WriteTrace(TraceError, "IVR Channel: %d - Fail ivr_channel_divert(%s, sync)... Channel:%d (reason = %x), error:%d - %s", 
				m_index, szdial, m_line, rc, gc_error, msg);
			return -1;
		}
		else
		{
			// Must return the channel to the Null call state. 
			gc_ReleaseCallEx(callrefno, EV_SYNC);
		}
		nResult = VOICE_OBT_SUCC;
#endif
	
	}
	else
	{
		// Call Control do not support SingleStep Transfer
		return 0;
	}
}

	WriteTrace(TraceDebug, "IVR Channel: %d - Success ivr_channel_divert dial(%s)...channel:%u", 
		m_index, szdial, m_line);

#endif


#ifdef CISCO_CCM

	long ret;

	ret = CTapiLine::SingleStepTransfer(szdial);
	if( ret < 0 )
		WriteTrace(TraceError, "IVR Channel: %d - Fail ivr_channel_divert(%s)...channel:%u, error code = 0x%x", m_index, szdial, m_index, ret);
	else
	{
		nResult = VOICE_OBT_SUCC;
		WriteTrace(TraceDebug, "IVR Channel: %d - Success ivr_channel_divert dial(%s)...channel:%u", m_index, szdial, m_index);
	}

	// Testing code
	/*
	ret = CTapiLine::ConsultTransfer(szdial);
	if( ret != 0 )
		WriteTrace(TraceError, "IVR Channel: %d - Fail ivr_channel_divert(%s)...channel:%u, error code = 0x%x", m_index, szdial, m_index, ret);
	else
	{
		nResult = VOICE_OBT_SUCC;
		WriteTrace(TraceDebug, "IVR Channel: %d - Success ivr_channel_divert dial(%s)...channel:%u", m_index, szdial, m_index);
	}
	*/

#endif

#ifdef CISCO_CME

	int retcode = 0;
	IPCSParam *retparam = NULL;
	SetTerminateCode(CMESyncEvent_Divert, VOICE_TM_Dummy);
	if( pMainWindow->m_CMEInterface.GetIntefaceStarted() )
	{
		retparam = pMainWindow->m_CMEInterface.m_piCommands->IVRStartBlindTransfer(m_index, szdial, FALSE);
		if( m_CMESyncEvent[CMESyncEvent_Divert].Wait(INFINITE) )
		{
			retparam = &(m_CMEEventParam[CMESyncEvent_Divert]);
			if( retparam->GetString("result") != "success" )
			{
				WriteTrace(TraceError, "IVR Channel: %d - Fail ivr_channel_divert(%s)...channel:%u, error code = 0x%x, Error = %d (%s)", 
					m_index, szdial, m_index, retcode, retparam->GetInt("reason"), retparam->GetInt("errordescription"));
			}
			else
			{
				nResult = VOICE_OBT_SUCC;
				retcode = 1;
				WriteTrace(TraceDebug, "IVR Channel: %d - Success ivr_channel_divert dial(%s)...channel:%u, , Reason = %d", m_index, szdial, m_index, GetTerminateCode(CMESyncEvent_Divert));
			}
		}
		pMainWindow->m_CMEInterface.m_piCommands->FreeThreadMemory();
	}
	else
		retcode = -1;
		
	return retcode;

#endif

	return 1;
}

//--------------------------------------------------
// Sun added 2010-03-08
#ifdef CISCO_CME
// To Exit all pending operations
void Cchannelcontrol::ResetAllSyncEvents()
{
	for( int lv_nEventType = 0; lv_nEventType < MAX_CMESyncEvents; lv_nEventType++ )
		SetTerminateCode(lv_nEventType, VOICE_TM_USRSTOP);
}

UL Cchannelcontrol::GetTerminateCode(int nEventType)
{
	if( nEventType >= 0 && nEventType < MAX_CMESyncEvents )
	{
		if( m_CMESyncEvent[nEventType].Wait(0) )
			return m_CMESyncEvent[nEventType].m_nEventID;
	}

	return VOICE_TM_Dummy;
}

void Cchannelcontrol::SetTerminateCode(int nEventType, UL lngTermCode, LPCSTR strData, IPCSParam *pParam)
{
	if( nEventType >= 0 && nEventType < MAX_CMESyncEvents )
	{
		if( strData )
		{
			memset(m_CMESyncEvent[nEventType].m_pData, 0x00, sizeof(m_CMESyncEvent[nEventType].m_pData));
			memcpy(m_CMESyncEvent[nEventType].m_pData, strData, sizeof(strData));
		}

		// 转移事件参数
		if( pParam )
		{
			CStringArray arrParamnames;
			pParam->GetNamelist(arrParamnames);
			for( int nParamIndex=0;nParamIndex<arrParamnames.GetCount(); nParamIndex++ )
			{
				m_CMEEventParam[nEventType].SetString(arrParamnames[nParamIndex], pParam->GetString(arrParamnames[nParamIndex]));
			}
		}

		m_CMESyncEvent[nEventType].m_nEventID = lngTermCode;
		if( lngTermCode == VOICE_TM_Dummy )
			m_CMESyncEvent[nEventType].Reset();
		else
			m_CMESyncEvent[nEventType].Set();

		WriteTrace(TraceDetail, "IVR Channel: %d CME Event Type: %d terminate code = %d, strdata = %s", 
			m_index, nEventType, lngTermCode, strData);
	}
}

int Cchannelcontrol::ConsultConference(LPCSTR lpszDigits, int nNoAnsTimeOut)
{
	int reco = -1;

	WriteTrace(TraceDetail, "IVR Channel: %d - will CallConsultTransfer(%s)", m_index, lpszDigits);

	IPCSParam *retparam = NULL;

	// Get Digit
	SetTerminateCode(CMESyncEvent_Divert, VOICE_TM_Dummy);
	if( pMainWindow->m_CMEInterface.GetIntefaceStarted() )
	{
		retparam = pMainWindow->m_CMEInterface.m_piCommands->IVRStartConsultTransfer(m_index, lpszDigits, nNoAnsTimeOut, FALSE);
		if( m_CMESyncEvent[CMESyncEvent_Divert].Wait(INFINITE) )
		{
			retparam = &(m_CMEEventParam[CMESyncEvent_Divert]);
			if( retparam->GetString("result") != "success" )
			{
				WriteTrace(TraceError, "IVR Channel: %d - Fail ConsultConference(%s)...channel:%u, error = %d (%s)", 
					m_index, lpszDigits, m_index, retparam->GetInt("reason"), retparam->GetInt("errordescription"));
			}
			else
				reco = 1;
		}
		pMainWindow->m_CMEInterface.m_piCommands->FreeThreadMemory();
	}

	if (ivr_channel_terminate())			// confirm terminate
		return -1;

	return reco;
}

int Cchannelcontrol::CompleteTransfer(BOOL blnTransfer)
{
	int reco = -1;

	IPCSParam *retparam = NULL;

	// Get Digit
	SetTerminateCode(CMESyncEvent_Divert, VOICE_TM_Dummy);
	if( pMainWindow->m_CMEInterface.GetIntefaceStarted() )
	{
		retparam = pMainWindow->m_CMEInterface.m_piCommands->IVRStartConsultTransferComplete(m_index, FALSE);
		if( m_CMESyncEvent[CMESyncEvent_Divert].Wait(INFINITE) )
		{
			retparam = &(m_CMEEventParam[CMESyncEvent_Divert]);
			if( retparam->GetString("result") != "success" )
			{
				WriteTrace(TraceError, "IVR Channel: %d - Fail CompleteTransfer()...channel:%u, error = %d (%s)", 
					m_index, m_index, retparam->GetInt("reason"), retparam->GetInt("errordescription"));
			}
			else
				reco = 1;
		}
		pMainWindow->m_CMEInterface.m_piCommands->FreeThreadMemory();
	}

	if (ivr_channel_terminate())			// confirm terminate
		return -1;

	return reco;
}

int Cchannelcontrol::StopConsultaion()
{
	int reco = -1;

	IPCSParam *retparam = NULL;

	// Get Digit
	SetTerminateCode(CMESyncEvent_Divert, VOICE_TM_Dummy);
	if( pMainWindow->m_CMEInterface.GetIntefaceStarted() )
	{
		retparam = pMainWindow->m_CMEInterface.m_piCommands->IVRStartConsultTransferCancel(m_index, FALSE);
		if( m_CMESyncEvent[CMESyncEvent_Divert].Wait(INFINITE) )
		{
			retparam = &(m_CMEEventParam[CMESyncEvent_Divert]);
			if( retparam->GetString("result") != "success" )
			{
				WriteTrace(TraceError, "IVR Channel: %d - Fail StopConsultaion()...channel:%u, error = %d (%s)", 
					m_index, m_index, retparam->GetInt("reason"), retparam->GetInt("errordescription"));
			}
			else
				reco = 1;
		}
		pMainWindow->m_CMEInterface.m_piCommands->FreeThreadMemory();
	}

	if (ivr_channel_terminate())			// confirm terminate
		return -1;

	return reco;
}
#endif
//--------------------------------------------------

int Cchannelcontrol::ivr_channel_gettermmsk(int nEventType)
{
	int reco;
	long termcode;

	if (ivr_channel_terminate())			// confirm terminate
		return -1;

#ifdef INTEL_DIALOGIC
	// Examine bitmap to determine if digits caused termination
	if( (termcode = ATDX_TERMMSK(m_line)) == AT_FAILURE) 
		return -1;
	
	// 终止类型
	if(termcode & TM_EOD)
	{	// 播放完毕
		reco = VOICE_TM_EOD;
	}
	else if(termcode & TM_MAXDTMF)
	{	// 最大按键
		reco = VOICE_TM_MAXDTMF;
	}
	else if(termcode & TM_DIGIT)
	{	// 中断按键
		reco = VOICE_TM_DIGIT;
	}
	else if(termcode & TM_MAXTIME)
	{	// 超时
		reco = VOICE_TM_MAXTIME;
	}
	else if(termcode & TM_IDDTIME)
	{	// 最大按键间隔
		reco = VOICE_TM_IDDTIME;
	}
	else if(termcode & TM_USRSTOP)
	{
		// Function stopped by user
		reco = VOICE_TM_USRSTOP;
	}
	else if(termcode & TM_LCOFF)
	{	// 断线
		reco = VOICE_TM_LCOFF;
	}
	else if(termcode & TM_BARGEIN)
	{	// Barge in
		reco = VOICE_TM_BARGEIN;
	}
	else
	{	// 其他
		reco = VOICE_TM_ERROR;
	}
#endif

#ifdef CISCO_CCM
	termcode = CTapiLine::GetTerminateCode();
	reco = termcode;
#endif

#ifdef CISCO_CME

	termcode = GetTerminateCode(nEventType);
	reco = termcode;

#endif

	WriteTrace(TraceDebug, "IVR Channel: %d operation stoped with terminal code: %d, ret code: %d", m_index, termcode, reco);
	return reco;
}

int Cchannelcontrol::ivr_channel_play(const char *pMent, char *termchar, bool clrdigitbuf, int nTimeout)
{
	long ret = -1;
	DV_TPT	tpt[6];
	char	szMent[256];
	char	*strp;
	int lv_nIndex = 0;

	if (ivr_channel_terminate())			// confirm terminate
		return -1;

	// File Name
	memset (szMent, 0x00, sizeof(szMent));
	
	// Sun added 2010-11-22 [A]
	BOOL lv_blnAbsolutePath = FALSE;
	lv_blnAbsolutePath = (strrchr(pMent, ':') != NULL);

	// Sun added 2002-05-24
	/// Add Ext if absent
	strp = strrchr(pMent, '.');
	if (strp == NULL)
	{
#ifdef INTEL_DIALOGIC
		sprintf(szMent, "%s%s.vox", (lv_blnAbsolutePath ? "" : gb_FN_VoxPath), pMent);
#else
		sprintf(szMent, "%s%s.wav", (lv_blnAbsolutePath ? "" : gb_FN_VoxPath), pMent);
#endif
	}
	else
	{
		sprintf(szMent, "%s%s", (lv_blnAbsolutePath ? "" : gb_FN_VoxPath), pMent);
	}

	WriteTrace(TraceDetail, "IVR Channel: %d - Play Ment ==> %s", m_index, szMent);

#ifdef INTEL_DIALOGIC

	// Sun updated 2012-07-04
	/// From
	//ivr_channel_dx_stop()
	/// To
	if( !ivr_channel_dx_stop() )
		return -1;

	if( clrdigitbuf )
		dx_clrdigbuf(m_line);				// clear previously entered digits

	memset(tpt, 0x00, sizeof( DV_TPT ) * 6);
	dx_clrtpt(tpt, 6);
	
	// Sun added 2008-01-24
	if( nTimeout > 0 )
	{
		tpt[lv_nIndex].tp_type   = IO_CONT;
		tpt[lv_nIndex].tp_termno = DX_MAXTIME;
		tpt[lv_nIndex].tp_length = nTimeout * 10;
		tpt[lv_nIndex].tp_flags  = TF_MAXTIME;
		lv_nIndex++;
	}

	if( termchar != NULL )
	{
		if (termchar[0] == '\0')		// 不中断语音播放
		{		
			tpt[lv_nIndex].tp_type   = IO_CONT;
			tpt[lv_nIndex].tp_termno = DX_LCOFF;
			tpt[lv_nIndex].tp_length = 1;
			tpt[lv_nIndex].tp_flags  = TF_LCOFF;
			lv_nIndex++;
		}
		else if(termchar[0] == '@')		// 接收到任意DTMF键中断语音播放
		{	
			tpt[lv_nIndex].tp_type   = IO_CONT;
			tpt[lv_nIndex].tp_termno = DX_MAXDTMF;
			tpt[lv_nIndex].tp_length = 1;
			tpt[lv_nIndex].tp_flags  = TF_MAXDTMF | TF_CLRBEG;
			lv_nIndex++;
		}
		else							// 接收到指定DTMF键中断语音播放
		{						
			tpt[lv_nIndex].tp_type   = IO_CONT;
			tpt[lv_nIndex].tp_termno = DX_DIGMASK;
			tpt[lv_nIndex].tp_length = getdigmask(termchar);
			tpt[lv_nIndex].tp_flags  = TF_DIGMASK;
			lv_nIndex++;
		}
	}
	else
	{
		tpt[lv_nIndex].tp_type   = IO_CONT;
		tpt[lv_nIndex].tp_termno = DX_LCOFF;
		tpt[lv_nIndex].tp_length = 1;
		tpt[lv_nIndex].tp_flags  = TF_LCOFF;
		lv_nIndex++;
	}

	if( m_nDevType == DEV_ANALOG )
	{
		tpt[lv_nIndex].tp_type   = IO_CONT;
		tpt[lv_nIndex].tp_termno = DX_TONE;
		tpt[lv_nIndex].tp_length = TID_DISCON_DT;
		tpt[lv_nIndex].tp_flags  = TF_TONE;
		tpt[lv_nIndex].tp_data   = DX_TONEON;
		lv_nIndex++;

		if( gb_Enable2DiscDTone || gb_Enable2DiscSTone )
			tpt[lv_nIndex].tp_type = IO_CONT;
		else
			tpt[lv_nIndex].tp_type = IO_EOT;
		tpt[lv_nIndex].tp_termno = DX_TONE;
		tpt[lv_nIndex].tp_length = TID_DISCON_ST;
		tpt[lv_nIndex].tp_flags  = TF_TONE;
		tpt[lv_nIndex].tp_data   = DX_TONEON;
		lv_nIndex++;

		// Sun added 2006-02-20
		if( gb_Enable2DiscDTone )
		{
			if( gb_Enable2DiscSTone )
				tpt[lv_nIndex].tp_type = IO_CONT;
			else
				tpt[lv_nIndex].tp_type = IO_EOT;
			tpt[lv_nIndex].tp_termno = DX_TONE;
			tpt[lv_nIndex].tp_length = TID_DISCON_DT2;
			tpt[lv_nIndex].tp_flags  = TF_TONE;
			tpt[lv_nIndex].tp_data   = DX_TONEON;
			lv_nIndex++;
		}
		if( gb_Enable2DiscSTone )
		{
			tpt[lv_nIndex].tp_type = IO_EOT;
			tpt[lv_nIndex].tp_termno = DX_TONE;
			tpt[lv_nIndex].tp_length = TID_DISCON_ST2;
			tpt[lv_nIndex].tp_flags  = TF_TONE;
			tpt[lv_nIndex].tp_data   = DX_TONEON;
			lv_nIndex++;
		}
	}
	else
	{
		tpt[lv_nIndex].tp_type   = IO_EOT;
		tpt[lv_nIndex].tp_termno = DX_LCOFF;
		tpt[lv_nIndex].tp_length = 1;
		tpt[lv_nIndex].tp_flags  = TF_LCOFF | TF_USE;
	}

	//-------------------------------------------
	// Sun added 2007-11-27
	bool	lv_blnPlayWave = false;
	strp = strrchr(szMent, '.');	
	if(strp != NULL)
		if( _strnicmp(strp, ".wav", 4) == 0 )
			lv_blnPlayWave = true;

	if( lv_blnPlayWave )
	{
		if (dx_playwav(m_line, szMent, tpt, EV_SYNC) == -1) 
			WriteTrace(TraceWarn, "IVR Channel: %d - ARS error playing wave file: %s - [%s]", m_index, szMent, ATDV_ERRMSGP(m_line));
	}
	else
	//-------------------------------------------
	{
		if (dx_playf(m_line, szMent, tpt, EV_SYNC | PM_SR8) == -1) 
		{
			//TRACE ("Error playing file - [%s]\n", ATDV_ERRMSGP(m_line));
			WriteTrace (TraceError, "IVR Channel: %d - Error playing file: %s - [%s]\n", m_index, szMent, ATDV_ERRMSGP(m_line));
			if( ATDV_LASTERR( m_line ) == EDX_SYSTEM )
			{
				int lv_syserr = dx_fileerrno(); // EACCES
				WriteTrace(TraceError, "IVR Channel: %d System Error No. - [%d]!!!", m_index, lv_syserr);
				switch( lv_syserr )
				{
				case 2:			// ENOENT
					break;
				default:
					return -1;
					break;
				}
			}
		}
	}

	// Sun Added 2001-12-03
	long		term;			// 退出事件
	if((term = ATDX_TERMMSK(m_line)) == AT_FAILURE)
	{
		return -1;
	}
	if(term & TM_USRSTOP)
	{
		// Function stopped by user
		ret = 0;
	}
	else
		ret = 1;

#else

#ifdef CISCO_CCM
	// clear previously entered digits
	if( clrdigitbuf )
		CTapiLine::ClearKeyBuffer();
#endif

	memset(tpt, 0x00, sizeof( DV_TPT ) * 6);

	// Sun added 2008-01-24
	if( nTimeout > 0 )
	{
		tpt[lv_nIndex].tp_type   = IO_CONT;
		tpt[lv_nIndex].tp_termno = DX_MAXTIME;
		tpt[lv_nIndex].tp_length = nTimeout * 10;
		tpt[lv_nIndex].tp_flags  = TF_MAXTIME;
		lv_nIndex++;
	}

	if( termchar != NULL )
	{
		if (termchar[0] == '\0')		// 不中断语音播放
		{		
			tpt[lv_nIndex].tp_type   = IO_CONT;
			tpt[lv_nIndex].tp_termno = DX_LCOFF;
			tpt[lv_nIndex].tp_length = 1;
			tpt[lv_nIndex].tp_flags  = TF_LCOFF;
			lv_nIndex++;
		}
		else if(termchar[0] == '@')		// 接收到任意DTMF键中断语音播放
		{	
			tpt[lv_nIndex].tp_type   = IO_CONT;
			tpt[lv_nIndex].tp_termno = DX_MAXDTMF;
			tpt[lv_nIndex].tp_length = 1;
			tpt[lv_nIndex].tp_flags  = TF_MAXDTMF | TF_CLRBEG;
			lv_nIndex++;
		}
		else							// 接收到指定DTMF键中断语音播放
		{						
			tpt[lv_nIndex].tp_type   = IO_CONT;
			tpt[lv_nIndex].tp_termno = DX_DIGMASK;
			tpt[lv_nIndex].tp_length = getdigmask(termchar);
			tpt[lv_nIndex].tp_flags  = TF_DIGMASK;
			lv_nIndex++;
		}

		tpt[lv_nIndex].tp_type   = IO_EOT;
		tpt[lv_nIndex].tp_termno = DX_LCOFF;
		tpt[lv_nIndex].tp_length = 1;
		tpt[lv_nIndex].tp_flags  = TF_LCOFF | TF_USE;
		lv_nIndex++;
	}
	else
	{
		tpt[lv_nIndex].tp_type   = IO_EOT;
		tpt[lv_nIndex].tp_termno = DX_LCOFF;
		tpt[lv_nIndex].tp_length = 1;
		tpt[lv_nIndex].tp_flags  = TF_LCOFF | TF_USE;
		lv_nIndex++;
	}

#ifdef CISCO_CCM
	CTapiLine::SetTPTParam(tpt, lv_nIndex);

	// Play File
	if( !CTapiLine::TestTPTCondition(CTapiLine::GetLastKey()) )
	{
		// Sun added 2010-08-02 [C]
		int lv_nWait;
		if( nTimeout > 0 )
			lv_nWait = nTimeout + 10;
		else
			lv_nWait = theApp.m_nMaxSoundTime;

		ret = CTapiLine::PlayFile(szMent, lv_nWait);
		if( ret == -1 )
			return -1;
	}
	else
		ret = 0;		// Already terminated.
#endif

#ifdef CISCO_CME

	IPCSParam *retparam = NULL;
	SetTerminateCode(CMESyncEvent_Play, VOICE_TM_Dummy);
	if( pMainWindow->m_CMEInterface.GetIntefaceStarted() )
	{
		retparam = pMainWindow->m_CMEInterface.m_piCommands->IVRStartPlayMedia(m_index, ECET_Calling, szMent, tpt, clrdigitbuf, FALSE);
		if( m_CMESyncEvent[CMESyncEvent_Play].Wait(INFINITE) )
		{
			retparam = &(m_CMEEventParam[CMESyncEvent_Play]);
			if( retparam->GetString("result") != "success" )
			{
				WriteTrace(TraceError, "IVR Channel: %d - Fail ivr_channel_play(%s)...channel:%u, error = %d (%s)", 
					m_index, szMent, m_index, retparam->GetInt("reason"), retparam->GetInt("errordescription"));
				ret = 0;
			}
			else
				ret = 1;
		}
		else
			ret = -1;
		pMainWindow->m_CMEInterface.m_piCommands->FreeThreadMemory();
	}

#endif

#endif

	if (ivr_channel_terminate())			// confirm terminate
		return -1;

	return ret;
}

int Cchannelcontrol::ivr_channel_playbuffer(const BYTE *pData, int nLen, char *termchar, bool clrdigitbuf)
{
	long ret = -1;

#ifdef INTEL_DIALOGIC
	DV_TPT	tpt[5];
	DX_IOTT iott;
	int lv_tptindex;

	if (ivr_channel_terminate())			// confirm terminate
		return -1;

	ivr_channel_dx_stop();
	if( clrdigitbuf )
		dx_clrdigbuf(m_line);				// clear previously entered digits

	// Set buffer data
	memset(&iott, 0x00, sizeof(DX_IOTT));
	iott.io_type = IO_MEM | IO_EOT;
	iott.io_bufp = (LPSTR)pData;
	iott.io_length = nLen;

	memset(tpt, 0x00, sizeof( DV_TPT ) * 5);
	dx_clrtpt(tpt, 5);
	
	if( termchar != NULL )
	{
		if (termchar[0] == '\0')		// 不中断语音播放
		{		
			tpt[0].tp_type   = IO_CONT;
			tpt[0].tp_termno = DX_LCOFF;
			tpt[0].tp_length = 1;
			tpt[0].tp_flags  = TF_LCOFF;
		}
		else if(termchar[0] == '@')		// 接收到任意DTMF键中断语音播放
		{	
			tpt[0].tp_type   = IO_CONT;
			tpt[0].tp_termno = DX_MAXDTMF;
			tpt[0].tp_length = 1;
			tpt[0].tp_flags  = TF_MAXDTMF | TF_CLRBEG;
		}
		else							// 接收到指定DTMF键中断语音播放
		{						
			tpt[0].tp_type   = IO_CONT;
			tpt[0].tp_termno = DX_DIGMASK;
			tpt[0].tp_length = getdigmask(termchar);
			tpt[0].tp_flags  = TF_DIGMASK;
		}
	}
	else
	{
		tpt[0].tp_type   = IO_CONT;
		tpt[0].tp_termno = DX_LCOFF;
		tpt[0].tp_length = 1;
		tpt[0].tp_flags  = TF_LCOFF;
	}

	if( m_nDevType == DEV_ANALOG )
	{
		tpt[1].tp_type   = IO_CONT;
		tpt[1].tp_termno = DX_TONE;
		tpt[1].tp_length = TID_DISCON_DT;
		tpt[1].tp_flags  = TF_TONE;
		tpt[1].tp_data   = DX_TONEON;

		if( gb_Enable2DiscDTone || gb_Enable2DiscSTone )
			tpt[2].tp_type = IO_CONT;
		else
			tpt[2].tp_type = IO_EOT;
		tpt[2].tp_termno = DX_TONE;
		tpt[2].tp_length = TID_DISCON_ST;
		tpt[2].tp_flags  = TF_TONE;
		tpt[2].tp_data   = DX_TONEON;

		// Sun added 2006-02-20
		lv_tptindex = 3;
		if( gb_Enable2DiscDTone )
		{
			if( gb_Enable2DiscSTone )
				tpt[lv_tptindex].tp_type = IO_CONT;
			else
				tpt[lv_tptindex].tp_type = IO_EOT;
			tpt[lv_tptindex].tp_termno = DX_TONE;
			tpt[lv_tptindex].tp_length = TID_DISCON_DT2;
			tpt[lv_tptindex].tp_flags  = TF_TONE;
			tpt[lv_tptindex].tp_data   = DX_TONEON;
			lv_tptindex++;
		}
		if( gb_Enable2DiscSTone )
		{
			tpt[lv_tptindex].tp_type = IO_EOT;
			tpt[lv_tptindex].tp_termno = DX_TONE;
			tpt[lv_tptindex].tp_length = TID_DISCON_ST2;
			tpt[lv_tptindex].tp_flags  = TF_TONE;
			tpt[lv_tptindex].tp_data   = DX_TONEON;
		}
	}
	else
	{
		tpt[1].tp_type   = IO_EOT;
		tpt[1].tp_termno = DX_LCOFF;
		tpt[1].tp_length = 1;
		tpt[1].tp_flags  = TF_LCOFF | TF_USE;
	}

	if (dx_play(m_line, &iott, tpt, EV_SYNC | PM_SR8) == -1) 
	{
		WriteTrace (TraceError, "IVR Channel: %d - Error playing buffer: [%s]", m_index, ATDV_ERRMSGP(m_line));
	}

	// Sun Added 2001-12-03
	long		term;			// 退出事件
	if((term = ATDX_TERMMSK(m_line)) == AT_FAILURE)
	{
		return -1;
	}
	if(term & TM_USRSTOP)
	{
		// Function stopped by user
		return( 0 );
	}
	else
		ret = 1;

#endif


#ifdef CISCO_CCM

	DV_TPT	tpt[5];

	if (ivr_channel_terminate())			// confirm terminate
		return -1;

	// clear previously entered digits
	if( clrdigitbuf )
		CTapiLine::ClearKeyBuffer();

	memset(tpt, 0x00, sizeof( DV_TPT ) * 5);
	if( termchar != NULL )
	{
		if (termchar[0] == '\0')		// 不中断语音播放
		{		
			tpt[0].tp_type   = IO_CONT;
			tpt[0].tp_termno = DX_LCOFF;
			tpt[0].tp_length = 1;
			tpt[0].tp_flags  = TF_LCOFF;
		}
		else if(termchar[0] == '@')		// 接收到任意DTMF键中断语音播放
		{	
			tpt[0].tp_type   = IO_CONT;
			tpt[0].tp_termno = DX_MAXDTMF;
			tpt[0].tp_length = 1;
			tpt[0].tp_flags  = TF_MAXDTMF | TF_CLRBEG;
		}
		else							// 接收到指定DTMF键中断语音播放
		{						
			tpt[0].tp_type   = IO_CONT;
			tpt[0].tp_termno = DX_DIGMASK;
			tpt[0].tp_length = getdigmask(termchar);
			tpt[0].tp_flags  = TF_DIGMASK;
		}

		tpt[1].tp_type   = IO_EOT;
		tpt[1].tp_termno = DX_LCOFF;
		tpt[1].tp_length = 1;
		tpt[1].tp_flags  = TF_LCOFF | TF_USE;
	}
	else
	{
		tpt[0].tp_type   = IO_EOT;
		tpt[0].tp_termno = DX_LCOFF;
		tpt[0].tp_length = 1;
		tpt[0].tp_flags  = TF_LCOFF | TF_USE;
	}
	CTapiLine::SetTPTParam(tpt, 2);

	// Play Buffer
	if( !CTapiLine::TestTPTCondition(CTapiLine::GetLastKey()) )
	{
		ret = CTapiLine::PlayBuffer((BYTE *)pData, nLen, theApp.m_nMaxSoundTime);
		if( ret == -1 )
			return -1;
	}
	else
		ret = 0;		// Already terminated.

#endif

#ifdef CISCO_CME
	// TODOCME: Don't supported by now
	//SetTerminateCode(CMESyncEvent_Play, VOICE_TM_Dummy);
	//if( pMainWindow->m_CMEInterface.GetIntefaceStarted() )
	//{
	//	pMainWindow->m_CMEInterface.m_piCommands->PlayBuffer(m_index, (BYTE *)pData, nLen, TRUE);
	//	pMainWindow->m_CMEInterface.m_piCommands->FreeThreadMemory();
	//}
#endif

	if (ivr_channel_terminate())			// confirm terminate
		return -1;

	return ret;
}

int	Cchannelcontrol::ivr_channel_closefile()
{
#ifdef INTEL_DIALOGIC

	// Sun added 2012-11-23
	if( m_hAsynFileHandle > 0 )
	{
		WriteTrace(TraceDebug, "IVR Channel: %d - will close an asyn handle 0x%x", m_index, m_hAsynFileHandle);
		dx_fileclose(m_hAsynFileHandle);
		m_hAsynFileHandle = -1;
	}
#endif

	return 1;
}

int	Cchannelcontrol::ivr_channel_async_play(const char *pMent, char *termchar, bool clrdigitbuf, bool loop)
{
	char	szMent[256];
	char	*strp;
	DV_TPT	tpt[5];
	int retval = 1;

	if (ivr_channel_terminate())			// confirm terminate
		return -1;

	// File Name
	memset(szMent, 0x00, sizeof(szMent));

	/// Add Ext if absent
	strp = strrchr(pMent, '.');
	if( strp == NULL )
	{
		sprintf(szMent, "%s%s.vox", gb_FN_VoxPath, pMent);
	}
	else
	{
		sprintf(szMent, "%s%s", gb_FN_VoxPath, pMent);
	}

	WriteTrace(TraceDebug, "IVR Channel: %d - Async Play Ment ==> %s", m_index, szMent);

#ifdef INTEL_DIALOGIC
	int		lv_hFD;							// file descriptor for file to be played
	int lv_tptindex;

	// Open vox file
	if( (lv_hFD = dx_fileopen(szMent, O_RDONLY|O_BINARY) ) == -1 )
	{ 
		WriteTrace(TraceError, "IVR Channel: %d - SP Error - File: %s open error %d!", m_index, szMent, dx_fileerrno()); 
		return 0; 
	} 
	// Sun added 2012-11-23
	ivr_channel_closefile();

	ivr_channel_dx_stop();
	if( clrdigitbuf )
		dx_clrdigbuf(m_line);				// clear previously entered digits

	// Set play data
	DX_IOTT iott;
	memset(&iott, 0x00, sizeof(DX_IOTT));
	iott.io_fhandle = lv_hFD;
	iott.io_length = -1;				// End of Data
	if( loop )
	{
		iott.io_type = IO_DEV | IO_LINK;
		iott.io_nextp = &iott;
	}
	else
		iott.io_type = IO_DEV | IO_EOT;

	memset(tpt, 0x00, sizeof( DV_TPT ) * 5);
	dx_clrtpt(tpt, 5);
	
	if( termchar != NULL )
	{
		if (termchar[0] == '\0')		// 不中断语音播放
		{		
			tpt[0].tp_type   = IO_CONT;
			tpt[0].tp_termno = DX_LCOFF;
			tpt[0].tp_length = 1;
			tpt[0].tp_flags  = TF_LCOFF;
		}
		else if(termchar[0] == '@')		// 接收到任意DTMF键中断语音播放
		{	
			tpt[0].tp_type   = IO_CONT;
			tpt[0].tp_termno = DX_MAXDTMF;
			tpt[0].tp_length = 1;
			tpt[0].tp_flags  = TF_MAXDTMF | TF_CLRBEG;
		}
		else							// 接收到指定DTMF键中断语音播放
		{						
			tpt[0].tp_type   = IO_CONT;
			tpt[0].tp_termno = DX_DIGMASK;
			tpt[0].tp_length = getdigmask(termchar);
			tpt[0].tp_flags  = TF_DIGMASK;
		}
	}
	else
	{
		tpt[0].tp_type   = IO_CONT;
		tpt[0].tp_termno = DX_LCOFF;
		tpt[0].tp_length = 1;
		tpt[0].tp_flags  = TF_LCOFF;
	}

	if( m_nDevType == DEV_ANALOG )
	{
		tpt[1].tp_type   = IO_CONT;
		tpt[1].tp_termno = DX_TONE;
		tpt[1].tp_length = TID_DISCON_DT;
		tpt[1].tp_flags  = TF_TONE;
		tpt[1].tp_data   = DX_TONEON;

		if( gb_Enable2DiscDTone || gb_Enable2DiscSTone )
			tpt[2].tp_type = IO_CONT;
		else
			tpt[2].tp_type = IO_EOT;
		tpt[2].tp_termno = DX_TONE;
		tpt[2].tp_length = TID_DISCON_ST;
		tpt[2].tp_flags  = TF_TONE;
		tpt[2].tp_data   = DX_TONEON;

		// Sun added 2006-02-20
		lv_tptindex = 3;
		if( gb_Enable2DiscDTone )
		{
			if( gb_Enable2DiscSTone )
				tpt[lv_tptindex].tp_type = IO_CONT;
			else
				tpt[lv_tptindex].tp_type = IO_EOT;
			tpt[lv_tptindex].tp_termno = DX_TONE;
			tpt[lv_tptindex].tp_length = TID_DISCON_DT2;
			tpt[lv_tptindex].tp_flags  = TF_TONE;
			tpt[lv_tptindex].tp_data   = DX_TONEON;
			lv_tptindex++;
		}
		if( gb_Enable2DiscSTone )
		{
			tpt[lv_tptindex].tp_type = IO_EOT;
			tpt[lv_tptindex].tp_termno = DX_TONE;
			tpt[lv_tptindex].tp_length = TID_DISCON_ST2;
			tpt[lv_tptindex].tp_flags  = TF_TONE;
			tpt[lv_tptindex].tp_data   = DX_TONEON;
		}
	}
	else
	{
		tpt[1].tp_type   = IO_EOT;
		tpt[1].tp_termno = DX_LCOFF;
		tpt[1].tp_length = 1;
		tpt[1].tp_flags  = TF_LCOFF | TF_USE;
	}

	if( dx_play(m_line, &iott, tpt, EV_ASYNC | PM_SR8) == -1 ) 
	{
		WriteTrace (TraceError, "IVR Channel: %d - Error async playing file: %s, error: [%s]\n", m_index, pMent, ATDV_ERRMSGP(m_line));

		if( ivr_channel_terminate() )
			retval = -1;
		else
			retval = 0;
	}

	// Sun added 2012-04-18, Sun removed 2012-11-23 如果关闭就不能完成全部放音
	//dx_fileclose(lv_hFD);
	m_hAsynFileHandle = lv_hFD;
	WriteTrace(TraceDebug, "IVR Channel: %d - get an asyn file handle 0x%x", m_index, lv_hFD);

	return retval;

#else

#ifdef CISCO_CCM
	// clear previously entered digits
	if( clrdigitbuf )
		CTapiLine::ClearKeyBuffer();
#endif

	memset(tpt, 0x00, sizeof( DV_TPT ) * 5);
	if( termchar != NULL )
	{
		if (termchar[0] == '\0')		// 不中断语音播放
		{		
			tpt[0].tp_type   = IO_CONT;
			tpt[0].tp_termno = DX_LCOFF;
			tpt[0].tp_length = 1;
			tpt[0].tp_flags  = TF_LCOFF;
		}
		else if(termchar[0] == '@')		// 接收到任意DTMF键中断语音播放
		{	
			tpt[0].tp_type   = IO_CONT;
			tpt[0].tp_termno = DX_MAXDTMF;
			tpt[0].tp_length = 1;
			tpt[0].tp_flags  = TF_MAXDTMF | TF_CLRBEG;
		}
		else							// 接收到指定DTMF键中断语音播放
		{						
			tpt[0].tp_type   = IO_CONT;
			tpt[0].tp_termno = DX_DIGMASK;
			tpt[0].tp_length = getdigmask(termchar);
			tpt[0].tp_flags  = TF_DIGMASK;
		}

		tpt[1].tp_type   = IO_EOT;
		tpt[1].tp_termno = DX_LCOFF;
		tpt[1].tp_length = 1;
		tpt[1].tp_flags  = TF_LCOFF | TF_USE;
	}
	else
	{
		tpt[0].tp_type   = IO_EOT;
		tpt[0].tp_termno = DX_LCOFF;
		tpt[0].tp_length = 1;
		tpt[0].tp_flags  = TF_LCOFF | TF_USE;
	}

#ifdef CISCO_CCM
	CTapiLine::SetTPTParam(tpt, 2);

	// Play File
	if( !CTapiLine::TestTPTCondition(CTapiLine::GetLastKey()) )
	{
		return CTapiLine::PlayFile(szMent, TRUE, loop);
	}
	else
		return 0;			// Already terminated.
#endif

#ifdef CISCO_CME
	IPCSParam *retparam = NULL;
	SetTerminateCode(CMESyncEvent_Play, VOICE_TM_Dummy);
	if( pMainWindow->m_CMEInterface.GetIntefaceStarted() )
	{
		retparam = pMainWindow->m_CMEInterface.m_piCommands->IVRStartPlayMedia(m_index, ECET_Calling, szMent, tpt, clrdigitbuf, FALSE);
		pMainWindow->m_CMEInterface.m_piCommands->FreeThreadMemory();
	}
	if( retparam )
		return 0;
	else
		return -1;
#endif

#endif
}

int Cchannelcontrol::ivr_channel_playrecord(const char *pMent)
{
	long ret = -1;
	DV_TPT	tpt[5];
	char	szMent[256];
	
	if (ivr_channel_terminate())			// confirm terminate
		return -1;

	// Sun added 2010-11-22 [A]
	BOOL lv_blnAbsolutePath = FALSE;
	lv_blnAbsolutePath = (strrchr(pMent, ':') != NULL);

	// File Name
	memset (szMent, 0x00, sizeof(szMent));
	sprintf(szMent, "%s%s", (lv_blnAbsolutePath ? "" : gb_FN_RecPath), pMent);
	WriteTrace(TraceDetail, "IVR Channel: %d - Play Record ==> %s", m_index, szMent);

#ifdef INTEL_DIALOGIC

	int		lv_tptindex;

	ivr_channel_dx_stop();
	//Sleep(500);
	dx_clrdigbuf(m_line);					// clear previously entered digits

	memset(tpt, 0x00, sizeof( DV_TPT ) * 5);
	dx_clrtpt(tpt, 5);
	
	// 接收到任意DTMF键中断语音播放
	tpt[0].tp_type   = IO_CONT;
	tpt[0].tp_termno = DX_MAXDTMF;
	tpt[0].tp_length = 1;
	tpt[0].tp_flags  = TF_MAXDTMF | TF_CLRBEG;
	if( m_nDevType == DEV_ANALOG )
	{
		tpt[1].tp_type   = IO_CONT;
		tpt[1].tp_termno = DX_TONE;
		tpt[1].tp_length = TID_DISCON_DT;
		tpt[1].tp_flags  = TF_TONE;
		tpt[1].tp_data   = DX_TONEON;

		if( gb_Enable2DiscDTone || gb_Enable2DiscSTone )
			tpt[2].tp_type = IO_CONT;
		else
			tpt[2].tp_type = IO_EOT;
		tpt[2].tp_termno = DX_TONE;
		tpt[2].tp_length = TID_DISCON_ST;
		tpt[2].tp_flags  = TF_TONE;
		tpt[2].tp_data   = DX_TONEON;

		// Sun added 2006-02-20
		lv_tptindex = 3;
		if( gb_Enable2DiscDTone )
		{
			if( gb_Enable2DiscSTone )
				tpt[lv_tptindex].tp_type = IO_CONT;
			else
				tpt[lv_tptindex].tp_type = IO_EOT;
			tpt[lv_tptindex].tp_termno = DX_TONE;
			tpt[lv_tptindex].tp_length = TID_DISCON_DT2;
			tpt[lv_tptindex].tp_flags  = TF_TONE;
			tpt[lv_tptindex].tp_data   = DX_TONEON;
			lv_tptindex++;
		}
		if( gb_Enable2DiscSTone )
		{
			tpt[lv_tptindex].tp_type = IO_EOT;
			tpt[lv_tptindex].tp_termno = DX_TONE;
			tpt[lv_tptindex].tp_length = TID_DISCON_ST2;
			tpt[lv_tptindex].tp_flags  = TF_TONE;
			tpt[lv_tptindex].tp_data   = DX_TONEON;
		}
	}
	else
	{
		tpt[1].tp_type   = IO_EOT;
		tpt[1].tp_termno = DX_LCOFF;
		tpt[1].tp_length = 1;
		tpt[1].tp_flags  = TF_LCOFF | TF_USE;
	}

	//-------------------------------------------
	// Sun added 2007-11-27
	char	*strp;
	bool	lv_blnPlayWave = false;
	strp = strrchr(szMent, '.');	
	if(strp != NULL)
		if( _strnicmp(strp, ".wav", 4) == 0 )
			lv_blnPlayWave = true;

	if( lv_blnPlayWave )
	{
		if (dx_playwav(m_line, szMent, tpt, EV_SYNC) == -1) 
			WriteTrace(TraceWarn, "IVR Channel: %d - ARS error playing wave file: %s - [%s]", m_index, szMent, ATDV_ERRMSGP(m_line));
	}
	else
	//-------------------------------------------
	{
		if (dx_playf(m_line, szMent, tpt, EV_SYNC | PM_SR8) == -1) 
			TRACE ("Error playing file - [%s]\n", ATDV_ERRMSGP(m_line));
	}

	// Sun Added 2001-12-03
	long		term;			// 退出事件
	if((term = ATDX_TERMMSK(m_line)) == AT_FAILURE)
	{
		return -1;
	}
	if(term & TM_USRSTOP)
	{
		// Function stopped by user
		ret = 0;
	}

	ret = 1;

#else

#ifdef CISCO_CCM
	// clear previously entered digits
	CTapiLine::ClearKeyBuffer();
#endif

	memset(tpt, 0x00, sizeof( DV_TPT ) * 5);
	// 接收到任意DTMF键中断语音播放
	tpt[0].tp_type   = IO_CONT;
	tpt[0].tp_termno = DX_MAXDTMF;
	tpt[0].tp_length = 1;
	tpt[0].tp_flags  = TF_MAXDTMF | TF_CLRBEG;
	tpt[1].tp_type   = IO_EOT;
	tpt[1].tp_termno = DX_LCOFF;
	tpt[1].tp_length = 1;
	tpt[1].tp_flags  = TF_LCOFF | TF_USE;

#ifdef CISCO_CCM
	CTapiLine::SetTPTParam(tpt, 2);

	// Play File
	ret = CTapiLine::PlayFile(szMent, theApp.m_nMaxSoundTime);
	if( ret == -1 )
		return -1;
	else
		ret = 1;
#endif

#ifdef CISCO_CME
	IPCSParam *retparam = NULL;
	SetTerminateCode(CMESyncEvent_Play, VOICE_TM_Dummy);
	if( pMainWindow->m_CMEInterface.GetIntefaceStarted() )
	{
		retparam = pMainWindow->m_CMEInterface.m_piCommands->IVRStartPlayMedia(m_index, ECET_Calling, szMent, tpt, TRUE, FALSE);
		if( m_CMESyncEvent[CMESyncEvent_Play].Wait(INFINITE) )
		{
			retparam = &(m_CMEEventParam[CMESyncEvent_Play]);
			if( retparam->GetString("result") != "success" )
			{
				WriteTrace(TraceError, "IVR Channel: %d - Fail ivr_channel_playrecord(%s)...channel:%u, error = %d (%s)", 
					m_index, szMent, m_index, retparam->GetInt("reason"), retparam->GetInt("errordescription"));
				
				ret = 0;
			}
			else
				ret = 1;
		}
		else
			ret = -1;
		pMainWindow->m_CMEInterface.m_piCommands->FreeThreadMemory();
	}
#endif

#endif

	if (ivr_channel_terminate())			// confirm terminate
		return -1;

	return ret;
}

int Cchannelcontrol::ivr_channel_getdigit(const char *pMent, int ndigitcnt, char *pGetDigit, int ntime, int ninterval, char *termchar, bool clrdigitbuf)
{
#ifdef INTEL_DIALOGIC
	DV_TPT		tpt_digit[9], tpt_play[5];
	DV_DIGIT	digp;
	int			numdigs;
	char		szGetDigit[256];
	char		szMent[256];
	int			lv_tptindex;
	
	if (ivr_channel_terminate())			// confirm terminate
		return -1;

	if (ntime == 1)
		ntime = 3;
	else
		ntime *= 10;
	ninterval *= 10;

	// File Name
	memset (szMent, 0x00, sizeof(szMent));
	sprintf(szMent, "%s%s", gb_FN_VoxPath, pMent);
	WriteTrace(TraceDetail, "IVR Channel: %d - Play Ment ===> %s", m_index, szMent);

	ivr_channel_dx_stop();
	if( clrdigitbuf )
		dx_clrdigbuf(m_line);					// clear previously entered digits	tpt_play[0].tp_type		= IO_CONT;				// only entry in the table

	memset(tpt_digit, 0x00, sizeof( DV_TPT ) * 9);
	dx_clrtpt(tpt_digit, 9);

	// Terminate GetDigits on Receiving MAXDTMF Digits 
	tpt_digit[ 0 ].tp_type   = IO_CONT;
	tpt_digit[ 0 ].tp_termno = DX_MAXDTMF;
	tpt_digit[ 0 ].tp_length = ndigitcnt;
	tpt_digit[ 0 ].tp_flags  = TF_MAXDTMF;
	
	// Terminate GetDigits after X Seconds 
	tpt_digit[ 1 ].tp_type   = IO_CONT;
	tpt_digit[ 1 ].tp_termno = DX_MAXTIME;
	tpt_digit[ 1 ].tp_length = ntime;
	tpt_digit[ 1 ].tp_flags  = TF_MAXTIME;
	
	// Terminate GetDigits after X Seconds 
	tpt_digit[ 2 ].tp_type   = IO_CONT;
	tpt_digit[ 2 ].tp_termno = DX_IDDTIME;
	tpt_digit[ 2 ].tp_length = ninterval;
	tpt_digit[ 2 ].tp_flags  = TF_IDDTIME;
	
	// Terminate When Receive X
	tpt_digit[ 3 ].tp_type   = IO_CONT;
	tpt_digit[ 3 ].tp_termno = DX_DIGMASK;
	tpt_digit[ 3 ].tp_length = getdigmask(termchar);
	tpt_digit[ 3 ].tp_flags  = TF_DIGMASK;

	// Terminate When Get Disconnect Tone or LCOFF
	if( m_nDevType == DEV_ANALOG )
	{
		tpt_digit[ 4 ].tp_type	 = IO_CONT;
		tpt_digit[ 4 ].tp_termno = DX_TONE;
		tpt_digit[ 4 ].tp_length = TID_DISCON_DT;
		tpt_digit[ 4 ].tp_flags	 = TF_TONE;
		tpt_digit[ 4 ].tp_data	 = DX_TONEON;
		tpt_digit[ 5 ].tp_type	 = IO_CONT;
		tpt_digit[ 5 ].tp_termno = DX_TONE;
		tpt_digit[ 5 ].tp_length = TID_DISCON_ST;
		tpt_digit[ 5 ].tp_flags	 = TF_TONE;
		tpt_digit[ 5 ].tp_data	 = DX_TONEON;

		// Sun added 2006-02-20
		lv_tptindex = 6;
		if( gb_Enable2DiscDTone )
		{
			tpt_digit[lv_tptindex].tp_type = IO_CONT;
			tpt_digit[lv_tptindex].tp_termno = DX_TONE;
			tpt_digit[lv_tptindex].tp_length = TID_DISCON_DT2;
			tpt_digit[lv_tptindex].tp_flags  = TF_TONE;
			tpt_digit[lv_tptindex].tp_data   = DX_TONEON;
			lv_tptindex++;
		}
		if( gb_Enable2DiscSTone )
		{
			tpt_digit[lv_tptindex].tp_type = IO_CONT;
			tpt_digit[lv_tptindex].tp_termno = DX_TONE;
			tpt_digit[lv_tptindex].tp_length = TID_DISCON_ST2;
			tpt_digit[lv_tptindex].tp_flags  = TF_TONE;
			tpt_digit[lv_tptindex].tp_data   = DX_TONEON;
			lv_tptindex++;
		}

		tpt_digit[ lv_tptindex ].tp_type	 = IO_EOT;
		tpt_digit[ lv_tptindex ].tp_termno = DX_LCOFF;
		tpt_digit[ lv_tptindex ].tp_length = ntime;
		tpt_digit[ lv_tptindex ].tp_flags	 = TF_LCOFF | TF_10MS;
	}
	else
	{
		tpt_digit[ 4 ].tp_type   = IO_EOT;
		tpt_digit[ 4 ].tp_termno = DX_LCOFF;
		tpt_digit[ 4 ].tp_length = 1;
		tpt_digit[ 4 ].tp_flags  = TF_LCOFF | TF_USE;
	}

	memset(tpt_play, 0x00, sizeof( DV_TPT ) * 5);
	dx_clrtpt(tpt_play, 5);

	// Terminate Play
	tpt_play[0].tp_type		= IO_CONT;				// only entry in the table
	tpt_play[0].tp_termno	= DX_MAXDTMF;			// Maximum digits
	tpt_play[0].tp_length	= 1;					// terminate on the first digit
	tpt_play[0].tp_flags	= TF_MAXDTMF;			// Use the default flags

	if( m_nDevType == DEV_ANALOG )
	{
		tpt_play[1].tp_type		= IO_CONT;
		tpt_play[1].tp_termno	= DX_TONE;
		tpt_play[1].tp_length	= TID_DISCON_DT;
		tpt_play[1].tp_flags	= TF_TONE;
		tpt_play[1].tp_data		= DX_TONEON;

		if( gb_Enable2DiscDTone || gb_Enable2DiscSTone )
			tpt_play[2].tp_type = IO_CONT;
		else
			tpt_play[2].tp_type = IO_EOT;
		tpt_play[2].tp_termno	= DX_TONE;
		tpt_play[2].tp_length	= TID_DISCON_ST;
		tpt_play[2].tp_flags	= TF_TONE;
		tpt_play[2].tp_data		= DX_TONEON;

		// Sun added 2006-02-20
		lv_tptindex = 3;
		if( gb_Enable2DiscDTone )
		{
			if( gb_Enable2DiscSTone )
				tpt_play[lv_tptindex].tp_type = IO_CONT;
			else
				tpt_play[lv_tptindex].tp_type = IO_EOT;
			tpt_play[lv_tptindex].tp_termno = DX_TONE;
			tpt_play[lv_tptindex].tp_length = TID_DISCON_DT2;
			tpt_play[lv_tptindex].tp_flags  = TF_TONE;
			tpt_play[lv_tptindex].tp_data   = DX_TONEON;
			lv_tptindex++;
		}
		if( gb_Enable2DiscSTone )
		{
			tpt_play[lv_tptindex].tp_type = IO_EOT;
			tpt_play[lv_tptindex].tp_termno = DX_TONE;
			tpt_play[lv_tptindex].tp_length = TID_DISCON_ST2;
			tpt_play[lv_tptindex].tp_flags  = TF_TONE;
			tpt_play[lv_tptindex].tp_data   = DX_TONEON;
		}
	}
	else
	{
		tpt_play[1].tp_type		= IO_EOT;
		tpt_play[1].tp_termno	= DX_LCOFF;
		tpt_play[1].tp_length	= 1;
		tpt_play[1].tp_flags	= TF_LCOFF | TF_USE;
	}

	if( pMent[0] != NULL )
	{
		//-------------------------------------------
		// Sun added 2007-11-27
		char	*strp;
		bool	lv_blnPlayWave = false;
		strp = strrchr(szMent, '.');	
		if(strp != NULL)
			if( _strnicmp(strp, ".wav", 4) == 0 )
				lv_blnPlayWave = true;

		if( lv_blnPlayWave )
		{
			if (dx_playwav(m_line, szMent, tpt_play, EV_SYNC) == -1) 
				WriteTrace(TraceWarn, "IVR Channel: %d - ARS error playing wave file: %s - [%s]", m_index, szMent, ATDV_ERRMSGP(m_line));
		}
		else
		//-------------------------------------------
		{
			if (dx_playf(m_line, szMent, tpt_play, EV_SYNC|PM_SR8) == -1) 
			{
				WriteTrace(TraceWarn, "IVR Channel: %d playing file Error [%s]", m_index, ATDV_ERRMSGP(m_line));
				if( ATDV_LASTERR( m_line ) == EDX_SYSTEM )
				{
					int lv_syserr = dx_fileerrno();
					WriteTrace(TraceError, "IVR Channel: %d System Error No. - [%d]!!!", m_index, lv_syserr);
					switch( lv_syserr )
					{
					case 2:
						break;
					default:
						return -1;
						break;
					}
				}
			}
		}

		if (ivr_channel_terminate())			// confirm terminate
			return -1;
	}

	if ((numdigs = dx_getdig(m_line, tpt_digit, &digp, EV_SYNC)) == -1)
		WriteTrace(TraceWarn, "IVR Channel: %d getting digits on %s, Error [%s]", m_index, ATDV_NAMEP(m_line), ATDV_ERRMSGP(ATDV_LASTERR(m_line)));
	
	memset (szGetDigit, 0x00, sizeof(szGetDigit));
	strcpy (szGetDigit, digp.dg_value);
	WriteTrace(TraceDetail, "IVR Channel: %d - ARS Get Digit received : %d - %s", m_index, numdigs, szGetDigit);

	if (ivr_channel_terminate())			// confirm terminate
		return -1;

	strcpy (pGetDigit, szGetDigit);

#else

	int			ret = -1;
	DV_TPT		tpt_play[5], tpt_digit[5];
	char		szMent[256];
	
	if (ivr_channel_terminate())			// confirm terminate
		return -1;

	ntime *= 10;
	ninterval *= 10;

	// File Name
	memset (szMent, 0x00, sizeof(szMent));
	sprintf(szMent, "%s%s", gb_FN_VoxPath, pMent);
	TRACE ("Play Ment ==> %s \n", szMent);

#ifdef CISCO_CCM
	if( clrdigitbuf )
		CTapiLine::ClearKeyBuffer();
#endif

	// Terminate Play
	memset(tpt_play, 0x00, sizeof( DV_TPT ) * 5);
	// 接收到任意DTMF键中断语音播放
	tpt_play[0].tp_type		= IO_CONT;				// only entry in the table
	tpt_play[0].tp_termno	= DX_MAXDTMF;			// Maximum digits
	tpt_play[0].tp_length	= 1;					// terminate on the first digit
	tpt_play[0].tp_flags	= TF_MAXDTMF;			// Use the default flags

	tpt_play[1].tp_type		= IO_EOT;
	tpt_play[1].tp_termno	= DX_LCOFF;
	tpt_play[1].tp_length	= 1;
	tpt_play[1].tp_flags	= TF_LCOFF | TF_USE;

	// Terminate Get Digit
	memset(tpt_digit, 0x00, sizeof( DV_TPT ) * 5);
	// Terminate GetDigits on Receiving MAXDTMF Digits 
	tpt_digit[ 0 ].tp_type   = IO_CONT;
	tpt_digit[ 0 ].tp_termno = DX_MAXDTMF;
	tpt_digit[ 0 ].tp_length = ndigitcnt;
	tpt_digit[ 0 ].tp_flags  = TF_MAXDTMF;
	
	// Terminate GetDigits after X Seconds 
	tpt_digit[ 1 ].tp_type   = IO_CONT;
	tpt_digit[ 1 ].tp_termno = DX_MAXTIME;
	tpt_digit[ 1 ].tp_length = ntime;
	tpt_digit[ 1 ].tp_flags  = TF_MAXTIME;
	
	// Terminate GetDigits after key interval X Seconds 
	tpt_digit[ 2 ].tp_type   = IO_CONT;
	tpt_digit[ 2 ].tp_termno = DX_IDDTIME;
	tpt_digit[ 2 ].tp_length = ninterval;
	tpt_digit[ 2 ].tp_flags  = TF_IDDTIME;
	
	// Terminate When Receive X
	tpt_digit[ 3 ].tp_type   = IO_CONT;
	tpt_digit[ 3 ].tp_termno = DX_DIGMASK;
	tpt_digit[ 3 ].tp_length = getdigmask(termchar);
	tpt_digit[ 3 ].tp_flags  = TF_DIGMASK;

	tpt_digit[ 4 ].tp_type   = IO_EOT;
	tpt_digit[ 4 ].tp_termno = DX_LCOFF;
	tpt_digit[ 4 ].tp_length = 1;
	tpt_digit[ 4 ].tp_flags  = TF_LCOFF | TF_USE;

#ifdef CISCO_CCM

	if( pMent[0] != NULL )
	{
		// Sun added 2013-04-06
		WriteTrace(TraceDetail, "IVR Channel: %d - Play Ment ===> %s", m_index, szMent);

		CTapiLine::SetTPTParam(tpt_play, 2);

		// Play File
		if( !CTapiLine::TestTPTCondition(CTapiLine::GetLastKey()) )
		{
			ret = CTapiLine::PlayFile(szMent, theApp.m_nMaxSoundTime);
			if( ret == -1 )
				return -1;

			if (ivr_channel_terminate())			// confirm terminate
				return -1;
		}
	}

	CTapiLine::SetTPTParam(tpt_digit, 5);
	CTapiLine::TestTPTCondition(CTapiLine::GetLastKey());

	// Sun added 2010-08-02 [C]
	int lv_nWait;
	if( ntime > 0 )
		lv_nWait = ntime + 10;
	else
		lv_nWait = theApp.m_nMaxSoundTime;
	ret = CTapiLine::GetDigit(pGetDigit, ndigitcnt, lv_nWait);

#endif

#ifdef CISCO_CME
	
	IPCSParam *retparam = NULL;

	// Get Digit
	SetTerminateCode(CMESyncEvent_GetDigit, VOICE_TM_Dummy);
	if( pMainWindow->m_CMEInterface.GetIntefaceStarted() )
	{
		retparam = pMainWindow->m_CMEInterface.m_piCommands->IVRStartCollectDigits(m_index, ECET_Calling, szMent, tpt_digit, clrdigitbuf, FALSE);
		if( m_CMESyncEvent[CMESyncEvent_GetDigit].Wait(INFINITE) )
		{
			retparam = &(m_CMEEventParam[CMESyncEvent_GetDigit]);
			if( retparam->GetString("result") == "success" )
			{
				strncpy(pGetDigit, (LPCSTR)(retparam->GetString("digits")), ndigitcnt);
				ret = strlen(pGetDigit);
			}
			else
			{
				WriteTrace(TraceError, "IVR Channel: %d - Fail ivr_channel_getdigit(%s)...channel:%u, error = %d (%s)", 
					m_index, szMent, m_index, retparam->GetInt("reason"), retparam->GetInt("errordescription"));

				ret = 0;
			}
		}
		else
			ret = -1;
		pMainWindow->m_CMEInterface.m_piCommands->FreeThreadMemory();
	}

#endif

	WriteTrace(TraceDetail, "IVR Channel: %d - ARS Get Digit received : %d - %s", m_index, ret, pGetDigit);

	if (ivr_channel_terminate())			// confirm terminate
		return -1;

	return ret;

#endif

	return 1;
}

int Cchannelcontrol::ivr_channel_record(const char *pMent, int rectime, int siltime, int minRecTime, bool toneon, int nNotifyIntvl, char *termchar)
{
	// Sun added 2009-7-24
	int lv_nNotifyIntvl;
	if( nNotifyIntvl <= 1 || nNotifyIntvl >= rectime )
		lv_nNotifyIntvl = 0;
	else
		lv_nNotifyIntvl = nNotifyIntvl;

#ifdef INTEL_DIALOGIC
	DV_TPT	tpt[8];
	char	szMent[256];
	int		lv_tptindex;

	if (ivr_channel_terminate())			// confirm terminate
		return -1;

	DX_XPB xpbVox = {FILE_FORMAT_VOX,
		DATA_FORMAT_DIALOGIC_ADPCM,  DRT_8KHZ,  4};  // Default vox format

	// File Name
	memset (szMent, 0x00, sizeof(szMent));
	sprintf(szMent, "%s%s", gb_FN_RecPath, pMent);
	TRACE ("Record File ==> %s \n", szMent);

	ivr_channel_dx_stop();

	memset(tpt, 0, sizeof(DV_TPT) * 8);
	dx_clrtpt(tpt, 8);
	
	dx_clrdigbuf(m_line);					// clear previously entered digits

	// Terminate Record on Receiving any DTMF tone
	// Sun replaced 2009-07-24, [E]
	/// From
	//tpt[ 0 ].tp_type   = IO_CONT;
	//tpt[ 0 ].tp_termno = DX_MAXDTMF;
	//tpt[ 0 ].tp_length = 1;
	//tpt[ 0 ].tp_flags  = TF_MAXDTMF;
	/// To
	if( termchar[0] == '\0' )		// 不中断语音播放
	{		
		tpt[0].tp_type   = IO_CONT;
		tpt[0].tp_termno = DX_LCOFF;
		tpt[0].tp_length = 1;
		tpt[0].tp_flags  = TF_LCOFF;
	}
	else if(termchar[0] == '@')		// 接收到任意DTMF键中断语音播放
	{	
		tpt[0].tp_type   = IO_CONT;
		tpt[0].tp_termno = DX_MAXDTMF;
		tpt[0].tp_length = 1;
		tpt[0].tp_flags  = TF_MAXDTMF | TF_CLRBEG;
	}
	else							// 接收到指定DTMF键中断语音播放
	{						
		tpt[0].tp_type   = IO_CONT;
		tpt[0].tp_termno = DX_DIGMASK;
		tpt[0].tp_length = getdigmask(termchar);
		tpt[0].tp_flags  = TF_DIGMASK;
	}
	
	// Terminate Record on xx Seconds of Silence 
	tpt[ 1 ].tp_type   = IO_CONT;
	tpt[ 1 ].tp_termno = DX_MAXSIL;
	tpt[ 1 ].tp_length = siltime * 10;
	tpt[ 1 ].tp_flags  = TF_MAXSIL;
	
	// Terminate Record After xx Seconds of Recording
	tpt[ 2 ].tp_type   = IO_CONT;
	tpt[ 2 ].tp_termno = DX_MAXTIME;
	tpt[ 2 ].tp_length = rectime * 10;
	tpt[ 2 ].tp_flags  = TF_MAXTIME;

	if( m_nDevType == DEV_ANALOG )
	{
		tpt[ 3 ].tp_type   = IO_CONT;
		tpt[ 3 ].tp_termno = DX_TONE;
		tpt[ 3 ].tp_length = TID_DISCON_DT;
		tpt[ 3 ].tp_flags  = TF_TONE;
		tpt[ 3 ].tp_data   = DX_TONEON;
		
		tpt[ 4 ].tp_type   = IO_CONT;
		tpt[ 4 ].tp_termno = DX_TONE;
		tpt[ 4 ].tp_length = TID_DISCON_ST;
		tpt[ 4 ].tp_flags  = TF_TONE;
		tpt[ 4 ].tp_data   = DX_TONEON;

		// Sun added 2006-02-20
		lv_tptindex = 5;
		if( gb_Enable2DiscDTone )
		{
			tpt[lv_tptindex].tp_type = IO_CONT;
			tpt[lv_tptindex].tp_termno = DX_TONE;
			tpt[lv_tptindex].tp_length = TID_DISCON_DT2;
			tpt[lv_tptindex].tp_flags  = TF_TONE;
			tpt[lv_tptindex].tp_data   = DX_TONEON;
			lv_tptindex++;
		}
		if( gb_Enable2DiscSTone )
		{
			tpt[lv_tptindex].tp_type = IO_CONT;
			tpt[lv_tptindex].tp_termno = DX_TONE;
			tpt[lv_tptindex].tp_length = TID_DISCON_ST2;
			tpt[lv_tptindex].tp_flags  = TF_TONE;
			tpt[lv_tptindex].tp_data   = DX_TONEON;
			lv_tptindex++;
		}

		tpt[ lv_tptindex ].tp_type   = IO_EOT;
		tpt[ lv_tptindex ].tp_termno = DX_LCOFF;
		tpt[ lv_tptindex ].tp_length = 1;
		tpt[ lv_tptindex ].tp_flags  = TF_LCOFF | TF_10MS;
	}
	else
	{
		tpt[ 3 ].tp_type   = IO_EOT;
		tpt[ 3 ].tp_termno = DX_LCOFF;
		tpt[ 3 ].tp_length = 1;
		tpt[ 3 ].tp_flags  = TF_LCOFF | TF_10MS;
	}

	// Sun added 2009-07-24, [D]
	DWORD lv_dwMode;
	if( toneon )
		lv_dwMode = EV_SYNC | PM_TONE;
	else
		lv_dwMode = EV_SYNC;

	//-------------------------------------------
	// Sun added 2007-11-27
	char	*strp;
	bool	lv_blnRecordWave = false;
	strp = strrchr(szMent, '.');	
	if(strp != NULL)
		if( _strnicmp(strp, ".wav", 4) == 0 )
			lv_blnRecordWave = true;

	if( lv_blnRecordWave )
	{
		DX_XPB xpbWav = {FILE_FORMAT_WAVE,
			DATA_FORMAT_PCM,  DRT_8KHZ,  16};  // Default vox format

		if (dx_recwav(m_line, szMent, tpt, &xpbWav, lv_dwMode) == -1) 
			WriteTrace(TraceWarn, "IVR Channel: %d - ARS error recording wave file: %s - [%s]", m_index, szMent, ATDV_ERRMSGP(m_line));
	}
	else
	//-------------------------------------------
	{
		if (dx_recvox(m_line, szMent, tpt, &xpbVox, lv_dwMode) == -1) 
			TRACE ("Error recording file - [%s]\n", ATDV_ERRMSGP(m_line));
	}

	if (ivr_channel_terminate())			// confirm terminate
		return -1;

	return 1;

#else		// CISCO_CCM or CISCO_CME
	
	long	ret = -1;
	DV_TPT	tpt[5];
	char	szMent[256];

	if (ivr_channel_terminate())			// confirm terminate
		return -1;

#ifdef CISCO_CCM
	// clear previously entered digits
	CTapiLine::ClearKeyBuffer();

	// Generate a beep tone
	if( toneon )
	{
		ret = CTapiLine::SendBeep();
		if( ret != 0 )
			WriteTrace(TraceWarn, "IVR Channel: %d - Fail to execute SendBeep(), error = 0x%x", m_index, ret);
	}
#endif

	// File Name
	memset (szMent, 0x00, sizeof(szMent));
	sprintf(szMent, "%s%s", gb_FN_RecPath, pMent);
	TRACE ("Record File ==> %s \n", szMent);

	memset(tpt, 0x00, sizeof( DV_TPT ) * 5);
	// Terminate Record on Receiving any DTMF tone
	// Sun replaced 2009-07-24, [E]
	/// From
	//tpt[ 0 ].tp_type   = IO_CONT;
	//tpt[ 0 ].tp_termno = DX_MAXDTMF;
	//tpt[ 0 ].tp_length = 1;
	//tpt[ 0 ].tp_flags  = TF_MAXDTMF;
	/// To
	if( termchar[0] == '\0' )		// 不中断语音播放
	{		
		tpt[0].tp_type   = IO_CONT;
		tpt[0].tp_termno = DX_LCOFF;
		tpt[0].tp_length = 1;
		tpt[0].tp_flags  = TF_LCOFF;
	}
	else if(termchar[0] == '@')		// 接收到任意DTMF键中断语音播放
	{	
		tpt[0].tp_type   = IO_CONT;
		tpt[0].tp_termno = DX_MAXDTMF;
		tpt[0].tp_length = 1;
		tpt[0].tp_flags  = TF_MAXDTMF | TF_CLRBEG;
	}
	else							// 接收到指定DTMF键中断语音播放
	{						
		tpt[0].tp_type   = IO_CONT;
		tpt[0].tp_termno = DX_DIGMASK;
		tpt[0].tp_length = getdigmask(termchar);
		tpt[0].tp_flags  = TF_DIGMASK;
	}
	
	// Terminate Record on xx Seconds of Silence 
	tpt[ 1 ].tp_type   = IO_CONT;
	tpt[ 1 ].tp_termno = DX_MAXSIL;
	tpt[ 1 ].tp_length = siltime * 10;
	tpt[ 1 ].tp_flags  = TF_MAXSIL;
	
	// Terminate Record After xx Seconds of Recording
	tpt[ 2 ].tp_type   = IO_CONT;
	tpt[ 2 ].tp_termno = DX_MAXTIME;
	tpt[ 2 ].tp_length = rectime * 10;
	tpt[ 2 ].tp_flags  = TF_MAXTIME;

	// Sun added 2009-07-24, [C]
	// Beep Interval
	tpt[ 3 ].tp_type   = IO_CONT;
	tpt[ 3 ].tp_termno = DX_MAXNOSIL;
	tpt[ 3 ].tp_length = lv_nNotifyIntvl * 10;
	tpt[ 3 ].tp_flags  = TF_USE;

	// Loop current off
	tpt[ 4 ].tp_type   = IO_EOT;
	tpt[ 4 ].tp_termno = DX_LCOFF;
	tpt[ 4 ].tp_length = 1;
	tpt[ 4 ].tp_flags  = TF_LCOFF | TF_USE;

#ifdef CISCO_CCM
	CTapiLine::SetTPTParam(tpt, 5);
	
	CTapiLine::SetMinDiskBuffer(minRecTime);
	CTapiLine::SetMaxSilenceTime(siltime);

	// Sun added 2010-08-02 [C]
	int lv_nWait;
	if( rectime > 0 )
		lv_nWait = rectime + 10;
	else
		lv_nWait = theApp.m_nMaxSoundTime;
	ret = CTapiLine::RecordFile(szMent, lv_nWait);
	if( ret == -1 )
		return -1;

	ret = CTapiLine::GetRecordLength();
#endif

#ifdef CISCO_CME
	IPCSParam *retparam = NULL;
	SetTerminateCode(CMESyncEvent_Record, VOICE_TM_Dummy);
	if( pMainWindow->m_CMEInterface.GetIntefaceStarted() )
	{
		retparam = pMainWindow->m_CMEInterface.m_piCommands->IVRStartRecord(m_index, ECET_Calling, NULL, szMent, toneon, tpt, TRUE, FALSE);
		if( m_CMESyncEvent[CMESyncEvent_Record].Wait(INFINITE) )
		{
			retparam = &(m_CMEEventParam[CMESyncEvent_Record]);
			if( retparam->GetString("result") != "success" )
			{
				WriteTrace(TraceError, "IVR Channel: %d - Fail ivr_channel_record()...channel:%u, error = %d (%s)", 
					m_index, m_index, retparam->GetInt("reason"), retparam->GetInt("errordescription"));
			}
			else
			{
				ret = retparam->GetInt("recordduration");
			}
		}
		else
			ret = -1;
		pMainWindow->m_CMEInterface.m_piCommands->FreeThreadMemory();
	}
#endif

	if (ivr_channel_terminate())			// confirm terminate
		return -1;

	return ret;

#endif	
}

// Sun added 2008-03-10
#ifdef CISCO_CCM
void Cchannelcontrol::GetData(char *pBuffer, int iLen)
{
	CCtrlWaveIn::GetData(pBuffer,iLen);

	// 进行语音识别
	if( m_pASRInt )
	{
		int nRes;
		nRes = m_pASRInt->RunASR(pBuffer, iLen);
		if( nRes < 0 )
		{
			// 出错
			CTapiLine::SetTerminateCode(VOICE_TM_USRSTOP);
		}
		else if( nRes == DEF_RunStatus_Result )
		{
			// 识别完成
			CTapiLine::SetTerminateCode(VOICE_TM_PATTERN);
		}
	}
}
#endif

// Sun added 2008-05-25
int Cchannelcontrol::ivr_asr_recording(CASRInterface *pASRInt, const char *pMent, bool blnBeep, int rectime, int siltime)
{
#ifdef INTEL_DIALOGIC
	// Dialogic目前不支持
	return -1;
#endif

#ifdef CISCO_CCM
	
	long	ret = -1;
	long	termcode;
	DV_TPT	tpt[5];

	if( pASRInt == NULL )
		return -1;

	if (ivr_channel_terminate())			// confirm terminate
		return -1;

	m_pASRInt = pASRInt;

	// clear previously entered digits
	CTapiLine::ClearKeyBuffer();

	// Generate a beep tone
	if( blnBeep )
		CTapiLine::SendBeep();

	// File Name
	TRACE ("ASR Record File ==> %s \n", pMent);

	memset(tpt, 0x00, sizeof( DV_TPT ) * 5);
	// Terminate Record on Receiving any DTMF tone
	tpt[ 0 ].tp_type   = IO_CONT;
	tpt[ 0 ].tp_termno = DX_MAXDTMF;
	tpt[ 0 ].tp_length = 1;
	tpt[ 0 ].tp_flags  = TF_MAXDTMF;
	
	// Terminate Record on xx Seconds of Silence 
	tpt[ 1 ].tp_type   = IO_CONT;
	tpt[ 1 ].tp_termno = DX_MAXSIL;
	tpt[ 1 ].tp_length = siltime * 10;
	tpt[ 1 ].tp_flags  = TF_MAXSIL;
	
	// Terminate Record After xx Seconds of Recording
	tpt[ 2 ].tp_type   = IO_CONT;
	tpt[ 2 ].tp_termno = DX_MAXTIME;
	tpt[ 2 ].tp_length = rectime * 10;
	tpt[ 2 ].tp_flags  = TF_MAXTIME;

	// Loop current off
	tpt[ 3 ].tp_type   = IO_EOT;
	tpt[ 3 ].tp_termno = DX_LCOFF;
	tpt[ 3 ].tp_length = 1;
	tpt[ 3 ].tp_flags  = TF_LCOFF | TF_USE;

	CTapiLine::SetTPTParam(tpt, 4);
	
	CTapiLine::SetMinDiskBuffer(0);
	CTapiLine::SetMaxSilenceTime(siltime);

	// Sun added 2010-08-02 [C]
	int lv_nWait;
	if( rectime > 0 )
		lv_nWait = rectime + 10;
	else
		lv_nWait = theApp.m_nMaxSoundTime;
	ret = CTapiLine::RecordFile(pMent, lv_nWait);
	if( ret != -1 )
	{
		termcode = CTapiLine::GetTerminateCode();
		if( termcode == VOICE_TM_PATTERN )
			ret = 1;
		else if( termcode == VOICE_TM_USRSTOP )
			ret = 0;
		else
		{
			if( pASRInt->RunASR(NULL, -1) < 0 )
				ret = 0;
			else
				ret = 1;
		}
	}

	m_pASRInt = NULL;

	if (ivr_channel_terminate())			// confirm terminate
		return -1;

	return ret;

#endif	

	return -1;
}

// Sun enhanced 2004-12-30
/// Read letters
// Sun enhanced 2003-05-09
/// Delay inter-digit
/// '-' to delay 500ms
/// ',' to delay 200ms
int Cchannelcontrol::ivr_channel_numberplay(const char *pNumber, BOOL f_blnReadLetter, BYTE f_LangID)
{
	WriteTrace(TraceDebug, "IVR Channel: %d - ARS =>  %s number play", m_index, pNumber);
	
	char szNum[50];
	BOOL lv_bLetter;
	for( int i=0; i<(int)strlen(pNumber); i++ )
	{
		if (ivr_channel_terminate())			// confirm terminate
			return -1;

		if( i > 50 )							// Max Length
			break;

		memset(szNum, 0x00, sizeof(szNum));
		lv_bLetter = FALSE;
		if( f_blnReadLetter )
			if( (pNumber[i] >= 'a' && pNumber[i] <= 'z') || (pNumber[i] >= 'A' && pNumber[i] <= 'Z') )
				lv_bLetter = TRUE;
		
		if( lv_bLetter )
		{
			sprintf(szNum, "SYS_0_%c", pNumber[i]);
			if( ivr_channel_play(szNum) == -1 )
				return -1;
		}
		else if( pNumber[i] >= '0' && pNumber[i] <= '9' )
		{
			sprintf(szNum, "SYS_%d_%c", f_LangID, pNumber[i]);
			if( ivr_channel_play(szNum) == -1 )
				return -1;
		}
		else if( pNumber[i] == DEF_IVR_PLAY_LONGPAUSE )
			Sleep(500);
		else if( pNumber[i] == DEF_IVR_PLAY_SHORTPAUSE )
			Sleep(200);
		else
			break;
	}

	return 1;
}

void Cchannelcontrol::ivr_channel_clear()
{
	// ToDo: Reset IVR Channel Data
}

// Sun added 2012-08-02
BOOL Cchannelcontrol::fax_channel_reopen()
{
#ifdef INTEL_DIALOGIC

	int lv_tryTimes = 0;
	int		encoding = DF_MH;

	WriteTrace(TraceDebug, "IVR Channel: %d - FAX Line:%d trys to reopen FAX resource: %d - %s", m_index, m_line, m_faxdev, m_sFAXName);

	// Close Fax Channel
	if( m_faxdev > 0 )
	{
		// Sun added 2006-10-01
		if( !m_blnBindFaxResource )
			gb_FaxManager.RemoveFaxResource(m_faxdev);

		if (m_faxtype == DFS_FAX40E || m_faxtype == DFS_FAX40EPLUS)
			fx_setparm(m_faxdev, FC_RXCODING, (void *)&encoding);
		fx_close(m_faxdev);
		WriteTrace(TraceDebug, "IVR Channel: %d - FAX Line:%d close FAX resource: %d", m_index, m_line, m_faxdev);

		Sleep(1000);
	}

	// Open Fax Channel
	if( m_sFAXName != NULL )
	{
		while( lv_tryTimes++ < 3 )
		{
			if( (m_faxdev = fx_open(m_sFAXName, NULL)) != -1 )
			{
				m_faxtype = ATFX_CHTYPE(m_faxdev);

				// Sun added 2006-10-01
				if( !m_blnBindFaxResource )
					gb_FaxManager.AddFaxResource(this, m_faxdev, m_faxtype);

				WriteTrace(TraceDebug, "IVR Channel: %d - FAX Line:%s opens FAX resource: %d", m_index, m_sFAXName, m_faxdev);
				break;
			}
			else
			{
				// Sun added 2012-08-02
				WriteTrace(TraceError, "IVR Channel: %d - FAX Line:%s failed to opens FAX resource! System Error No. - [%d]!!!", 
					m_index, m_sFAXName, dx_fileerrno());
				Sleep(1000);
				continue;
			}
		}
	}

	if( m_faxdev == -1 )
		return FALSE;

#endif

	return TRUE;
}

BOOL Cchannelcontrol::fax_channel_filefind(const char *sFileName)
{
	HANDLE			hFile;
	WIN32_FIND_DATA	FindFileData;

	if( sFileName != NULL )
	{
		if( sFileName[0] != NULL )
		{
			TRACE ("Find FAX File[%d] : %s \n", sFileName);
			hFile = FindFirstFile(sFileName, &FindFileData);
			if( hFile == INVALID_HANDLE_VALUE )
			{
				if( GetLastError() == ERROR_FILE_NOT_FOUND )
					FindClose(hFile);
				return FALSE;
			}
			FindClose(hFile);
		}
	}

	return TRUE;
}

void Cchannelcontrol::fax_channel_tolower_string(char *ptr)
{
	// convert the extension to lowercase
	char c1, c2;
	int i;

	if (ptr == NULL)
		return;

	for (i=0; i<3; i++)	{
		c1 = *(ptr+1+i);
		if ( c1 != '\0' && isupper( c1 ) ) {
			c2 = tolower( c1 );
			*(ptr+1+i) = c2;
		}
	}
}

BOOL Cchannelcontrol::fax_channel_fileopen(const char *sFileName)
{
#ifdef INTEL_DIALOGIC

	char	*strp;
	int		rc;
	
	if( sFileName != NULL )
	{
		if( sFileName[0] != NULL )
		{
			TRACE ("Open FAX File[%d] : %s \n", sFileName);

			if( (rc = dx_fileopen(sFileName, O_RDONLY|O_BINARY, 0)) == -1 )
			{
				TRACE("Error: Unable to open channel:%d filename:%s", m_index, sFileName);
				return FALSE;
			}

			strp = strrchr(sFileName, '.');
			fax_channel_tolower_string(strp);

			if( strp != NULL && !strnicmp(strp+1, "raw", 3) ) 		// raw file format
				fx_setiott(&m_iott, rc, DF_RAW, DFC_AUTO);
			else if( strp != NULL && !strnicmp(strp+1, "tif", 3) ) 	// TIFF/F file format
			{
				fx_setiott(&m_iott, rc, DF_TIFF, DFC_AUTO);		// Sun updated 2012-06-01 from DFC_AUTO to DFC_EOP
				m_iott.io_pgcount = -1;
			}
			else 
			{													// no extension or other than TIF or RAW
				fx_setiott(&m_iott, rc, DF_ASCII, DFC_AUTO);

				// Initialize Ascii information
				m_asciidata.units=DF_UNITS_IN10;	// values in 10th of an inch
				m_asciidata.font=DF_FONT_0;			// normal font
				m_asciidata.pagepad=DF_PAD;			// pad short pages to pagelength
				m_asciidata.pagelength=110;			// normal letter size paper
				m_asciidata.topmargin=10;			// set all margins to 1 inch
				m_asciidata.botmargin=10;
				m_asciidata.leftmargin=10;
				m_asciidata.rightmargin=10;
				m_asciidata.linespace=DF_SINGLESPACE;
				m_asciidata.tabstops=10;
		
				m_iott.io_datap = (void *)&m_asciidata;	// set IOTT to use it
			}
		}
	}

	m_iott.io_type|=IO_EOT;						// mark last IOTT as the last one
	m_iott.io_phdcont=DFC_EOP;					// and set continuation to EOP

#endif

	return TRUE;
}

void Cchannelcontrol::fax_channel_fileclose()
{
#ifdef INTEL_DIALOGIC
	if( m_iott.io_fhandle > 0 )
		dx_fileclose(m_iott.io_fhandle);
#endif
}

int Cchannelcontrol::fax_channel_stopch()
{
#ifdef INTEL_DIALOGIC

	int reco = 0;
	long lv_st;

	if( m_faxdev > 0 )
	{
		lv_st = ATFX_STATE(m_faxdev);
		if( lv_st != CS_IDLE && lv_st != CS_STOPD )
		{
			WriteTrace(TraceDebug, "IVR Channel: %d trys to Stop Fax Dev: %d in state %d...", m_index, m_faxdev, lv_st);
			
			fx_stopch(m_faxdev, EV_ASYNC);
			//fx_stopch(m_faxdev, EV_SYNC);

			lv_st = ATFX_STATE(m_faxdev);
			while(lv_st != CS_IDLE && lv_st != CS_STOPD)
			{
				if(reco > 10)
				{
					WriteTrace(TraceWarn, "IVR Channel: %d can't stop operation in fax_channel_stopch(), state = %d!", m_index, ATFX_STATE(m_faxdev));
					return -1;
				}
				fx_stopch(m_faxdev, EV_ASYNC);
				Sleep(50);
				reco ++;

				lv_st = ATFX_STATE(m_faxdev);
			}
		}

		// Sun added 2012-08-02
		if( lv_st == CS_STOPD )
		{
			// Reopen FAX resource
			// ToDo:...
			WriteTrace(TraceInfo, "IVR Channel: %d need to reopen fax resources in fax_channel_stopch()", m_index);
		}
	}

#endif

	return 0;
}

void Cchannelcontrol::fax_channel_send_init(const char *sRemoteID, const char *sHeader, const char *sFromNo, int nRetryTimes)
{
#ifdef INTEL_DIALOGIC
	int		txcoding[]={DF_MH, DF_MR, DF_MMR}, encoding;
	int		phdcont=DFC_AUTO, nretry=DF_RETRY1|DF_RETRYDCN;
	int		baudrate[]={DF_MAXBAUD,DF_14400BAUD,DF_9600BAUD,DF_4800BAUD,DF_2400BAUD};
	
	if( sFromNo != NULL )
		if( sFromNo[0] != NULL )
			if( fx_setparm(m_faxdev, FC_LOCALID, (void *)sFromNo) != 0 )
				WriteTrace(TraceWarn, "FAX IVR Channel: %d - Setting local id %s  error.", m_index, sFromNo);

	if( sRemoteID != NULL )
		if( sRemoteID[0] != NULL )
			if( fx_setparm(m_faxdev, FC_REMOTEID, (void *)sRemoteID) != 0 )
				WriteTrace(TraceWarn, "FAX IVR Channel: %d - Setting remote id %s  error.", m_index, sRemoteID);
	
	// Fax header
	if( m_faxtype >= DFS_FAX40 )
	{
		if( fx_setparm(m_faxdev, FC_HDRUSER, (void *)sHeader) != 0 )
			WriteTrace(TraceWarn, "FAX IVR Channel: %d Setting FC_HDRUSER(%s) error.", m_index, sHeader);
	}

	// transmit/receive coding and baud rate
	m_iBaudRate = DF_MAXBAUD;
	if( m_faxtype >= DFS_FAX40E )
	{
		/*
		if( m_faxtype == DFS_FAX40E || m_faxtype == DFS_FAX40EPLUS )
		{
			//encoding = (0)? (txcoding[2]|DF_ECM):(txcoding[2]);
			encoding = 2;
			if( fx_setparm(m_faxdev, FC_TXCODING, (void *)&encoding) != 0 )
				TRACE ("Setting FC_TXCODING channel:%d\n", m_index);
		}*/
		if( m_faxtype == DFS_FAX40E || m_faxtype == DFS_FAX40EPLUS )
			encoding = DF_MMR;
		else
			encoding = DF_MH;
		if( fx_setparm(m_faxdev, FC_TXCODING, (void *)&encoding) != 0 )
			WriteTrace(TraceWarn, "FAX IVR Channel: %d Setting FC_TXCODING error.", m_index);
	}
	else
	{
		// Max Baud Rate is 9600
		//if( m_iBaudRate > DF_9600BAUD )
		//	m_iBaudRate = DF_9600BAUD;
		//m_iBaudRate = DF_4800BAUD;
	}

	// Set Baud Rate
	///FAX: m_iBaudRate: 14400 -> 12000 -> 9600 -> 7200 -> 4800 -> 2400
	if( nRetryTimes > 0 )
	{
		// 9600 试1次
		switch( nRetryTimes )
		{
		case 1:
			if( m_iBaudRate > DF_9600BAUD )
				m_iBaudRate = DF_9600BAUD;
			break;
		case 2:
			if( m_iBaudRate > DF_9600BAUD )
				m_iBaudRate = DF_4800BAUD;
			else
				m_iBaudRate = DF_2400BAUD;
			break;
		default:
			if( m_iBaudRate > DF_9600BAUD )
				m_iBaudRate = DF_4800BAUD;
			else
				m_iBaudRate = DF_2400BAUD;
			break;
		}
	}
	if( fx_setparm(m_faxdev, FC_TXBAUDRATE, &m_iBaudRate) != 0 )
		WriteTrace(TraceWarn, "FAX IVR Channel: %d Setting FC_TXBAUDRATE(%d) error.", m_index, m_iBaudRate);
	else
		WriteTrace(TraceDebug, "FAX IVR Channel: %d set FC_TXBAUDRATE(%d).", m_index, m_iBaudRate);

	// let fax library determine Phase D continuation values, set to DFC_AUTO
	if( fx_setparm(m_faxdev, FC_SENDCONT, (void *)&phdcont) != 0 )
	{
		WriteTrace(TraceWarn, "FAX IVR Channel: %d Setting FC_SENDCONT(DFC_AUTO) Error - %s (error code %d).", 
			m_index, ATDV_ERRMSGP(m_faxdev), ATDV_LASTERR(m_faxdev));
		if( ATDV_LASTERR(m_line) == EDX_SYSTEM )
			WriteTrace(TraceWarn, "FAX IVR Channel: %d EDX_SYSTEM errno = %d", m_index, errno);
	}

	if( fx_setparm(m_faxdev, FC_RETRYCNT, (void *)&nretry) != 0 )
		WriteTrace(TraceWarn, "FAX IVR Channel: %d Setting retry count() error", m_index);

	m_bGotBauRte = false;			// Sun added 2012-06-13

#endif
}

int Cchannelcontrol::fax_channel_send(const char *pMent, const char *sRemoteID, const char *sHeader, const char *sFromNo, int nRetryTimes)
{
	int lv_ret = 0;

#ifdef INTEL_DIALOGIC
	int rc;

	char	szMent[MAX_PATH];
	char	*strp;

	memset(&m_iott, 0x00, sizeof(DF_IOTT));
	memset(&m_asciidata, 0x00, sizeof(DF_ASCIIDATA));

	// File Name
	memset (szMent, 0x00, sizeof(szMent));
	
	fax_channel_send_init(sRemoteID, sHeader, sFromNo, nRetryTimes);

	// Sun added 2002-05-24
	/// Add Ext if absent
	strp = strrchr(pMent, '.');
	if (strp == NULL)
		sprintf(szMent, "%s%s.tif", gb_FN_VoxPath, pMent);
	else
		sprintf(szMent, "%s%s", gb_FN_VoxPath, pMent);

	if( ! fax_channel_filefind(szMent) )
		return -1;

	if( ! fax_channel_fileopen(szMent) )
		return -2;

	if( fx_initstat(m_faxdev, DF_TX) == -1 )
	{
		WriteTrace(TraceWarn, "IVR Channel: %d - Fax Init Failed - %s (error code %d) on linedev :%d (%s)", 
			m_index,
			ATDV_ERRMSGP(m_faxdev), ATDV_LASTERR(m_faxdev), 
			m_line, m_sChannelName);
		if (ATDV_LASTERR(m_line) == EDX_SYSTEM)
			WriteTrace(TraceWarn, "IVR Channel: %d - EDX_SYSTEM errno = %d", m_index, errno);

		lv_ret = -4;
	}

	if( lv_ret >= 0 )
	{
		if( fx_sendfax(m_faxdev, &m_iott, EV_SYNC|DF_PHASEB|DF_PHASED) == -1 )		// DF_PHASED
		{
			rc = ATDV_LASTERR(m_faxdev);
			WriteTrace(TraceWarn, "IVR Channel: %d - Fax Send Error code: 0x%X Error message: %s on linedev :%d (%s) for file:%s", 
				m_index,
				rc, ATDV_ERRMSGP(m_faxdev),
				m_line, m_sChannelName, szMent);
			if( ATDV_LASTERR(m_line) == EDX_SYSTEM )
		  		WriteTrace(TraceWarn, "IVR Channel: %d - EDX_SYSTEM errno = 0x%x", m_index, errno);
			WriteTrace(TraceWarn, "IVR Channel: %d - Send FAX Phase.. status: %ld", m_index, ATFX_ESTAT(m_faxdev));
			// EFX_ABORTCMD: 199

			if( rc == EFX_DISCONNECT )
			{
				// FAX远端挂断延时（秒）
				if( theApp.m_nFaxDiscDelay > 0 )
					Sleep(theApp.m_nFaxDiscDelay * 1000);
				lv_ret = -3;
			}
			// Sun addded for debug 2012-08-02 for errorcode 107, in log 107 (0x6B) is remote disc but not in constant definition
			/// This may be a bug of dialogic
			else if( rc == 107 )
				lv_ret = -3;
			// unknown error, Sun addded 2012-08-02
			else if( rc == EDX_BUSY )
			{
				// May need repoen fax channel
				fax_channel_reopen();
				lv_ret = -3;
			}
			else
				lv_ret = -5;
		}
	}

	// Sun added 2012-06-13
	m_bGotBauRte = true;

#endif	

	fax_channel_fileclose();
	return lv_ret;
}

void Cchannelcontrol::fax_channel_receive_init(const char *sToNo, int nRetryTimes)
{
#ifdef INTEL_DIALOGIC
	int temp;

	int	rxcoding[]={DF_MH, DF_MMR}, encoding;
	
	if( sToNo != NULL )
		if( sToNo[0] != NULL )
			if( fx_setparm(m_faxdev, FC_LOCALID, (void *)sToNo) != 0 )
				WriteTrace(TraceWarn, "FAX IVR Channel: %d - Setting local id %s  error.", m_index, sToNo);
	
	// transmit/receive coding and baud rate
	m_iBaudRate = DF_MAXBAUD;
	if( m_faxtype >= DFS_FAX40E )
	{
		encoding = 1;
		if( fx_setparm(m_faxdev, FC_RXCODING, (void *)&encoding) != 0 )
			WriteTrace(TraceWarn, "FAX IVR Channel: %d Setting FC_RXCODING error.", m_index);
	}
	else
	{
		//encoding = 0;
		//if( fx_setparm(m_faxdev, FC_RXCODING, (void *)&encoding) != 0 )
		//	WriteTrace(TraceWarn, "FAX IVR Channel: %d Setting FC_RXCODING error.", m_index);

		// Max Baud Rate is 9600
		//if( m_iBaudRate > DF_9600BAUD )
		//	m_iBaudRate = DF_9600BAUD;
	}

	// Set Baud Rate
	///FAX: m_iBaudRate: 14400 -> 12000 -> 9600 -> 7200 -> 4800 -> 2400
	if( nRetryTimes > 0 )
	{
		// 9600 试2次
		switch( nRetryTimes )
		{
		case 1:
			if( m_iBaudRate > DF_9600BAUD )
				m_iBaudRate = DF_9600BAUD;
			break;
		case 2:
			if( m_iBaudRate > DF_9600BAUD )
				m_iBaudRate = DF_9600BAUD;
			else
				m_iBaudRate = DF_4800BAUD;
			break;
		default:
			if( m_iBaudRate > DF_9600BAUD )
				m_iBaudRate = DF_4800BAUD;
			else
				m_iBaudRate = DF_2400BAUD;
			break;
		}
	}
	if( fx_setparm(m_faxdev, FC_RXBAUDRATE, &m_iBaudRate) != 0 )
		WriteTrace(TraceWarn, "FAX IVR Channel: %d Setting FC_RXBAUDRATE(%d) error.", m_index, m_iBaudRate);
	else
		WriteTrace(TraceDebug, "FAX IVR Channel: %d set FC_RXBAUDRATE(%d).", m_index, m_iBaudRate);

	// let fax libarary determine TIFF/F page numbering used
	temp = TF_AUTOPG;
	if( fx_setparm(m_faxdev, FC_TFPGBASE, &temp) != 0 )
		WriteTrace(TraceWarn, "FAX IVR Channel: %d Setting FC_TFPGBASE(TF_AUTOPG) error.", m_index);

	m_bGotBauRte = false;			// Sun added 2012-06-13

#endif
}

int Cchannelcontrol::fax_channel_receive(const char *pFile, char *sFromNo, const char *sToNo, int nRetryTimes)
{
#ifdef INTEL_DIALOGIC
	char    buff[MAX_PATH+1], rxfile[MAX_PATH+1];
	char	*strp, *rxfilep;
	int		rc;
	long    phdcmd,phdrpy;
	int		flags=EV_SYNC;					// default receive flags
	int     rxcount=1;						// number of files
	int		lv_trys = 0;

	memset(&m_iott, 0x00, sizeof(DF_IOTT));
	memset(&m_asciidata, 0x00, sizeof(DF_ASCIIDATA));
	
	fax_channel_receive_init(sToNo);

	strcpy(rxfile, pFile);					// copy receive file to local buffer
	rxfilep=rxfile;

	// check what extension type it has
	strp=strrchr(rxfile,'.');				// find file extension

	// convert the extension to lowercase
	fax_channel_tolower_string(strp);

	if( strp!=NULL && !strnicmp(strp+1,"tif",3) )
	{
   		flags |= DF_TIFF;    
	} else {
	// For all others, receive as Raw
		flags |= DF_RAW;     
	}

	// set inital FAX state for channel
	if( fx_initstat(m_faxdev, DF_RX) != 0 )
	{
		WriteTrace(TraceWarn, "IVR Channel: %d - FAX fx_initstat failed!", m_index);
		return(-2);
	}
	
	// Phase B & D handler
	//if (Chdev.handler)
	flags |= DF_PHASEB|DF_PHASED;
	
	//
	// Receive fax loop.  If we are storing the fax in Raw format,
	// each page requires a separate call to fx_rcvfax().  Terminate
	// the loop on EOP or error.
	//

	while( lv_trys <= nRetryTimes ) 
	{
		// Sun added 2012-06-01
		if( lv_trys > 0 )
			fax_channel_receive_init(sToNo, lv_trys);
		lv_trys++;

		// receive the fax
		WriteTrace(TraceDebug, "IVR Channel: %d - FAX Dev: %d Receiving %s...try times = %d", m_index, m_faxdev, rxfilep, lv_trys);
		if( (rc = fx_rcvfax(m_faxdev, rxfilep, flags)) == -1 )
		{
			WriteTrace(TraceWarn, "IVR Channel: %d - FAX Receive failed: Phase E Status: %ld, ec=0x%x!", 
				m_index, ATFX_ESTAT(m_faxdev), ATDV_LASTERR(m_faxdev));
		}
    
		// Sun added 2012-06-13
		m_bGotBauRte = true;

		// break from while loop if fx_rcvfax returned an error
		if( rc != 0 )
		{
			WriteTrace(TraceWarn, "IVR Channel: %d - FAX Receive failed: rc = 0x%x, ec=0x%x!", 
				m_index, rc, ATDV_LASTERR(m_faxdev));

			// Sun added 2013-03-12
			// May need to delete file with 0 size
			try
			{
				if( rxfilep )
				{
					// Delete file
					if( FileExists(rxfilep, FALSE) )
					{
						__int64 lv_lngFIleSize = 0;

						if( GetFileLen(rxfilep, &lv_lngFIleSize) )
						{
							if( lv_lngFIleSize == 0 )
							{
								DeleteFile(rxfilep);
								WriteTrace(TraceDebug, "IVR Channel: %d - FAX received zero size file was deleted %s", m_index, rxfilep);
							}
						}
					}
				}
			}
			catch(...)
			{
				WriteTrace(TraceError, "IVR Channel: %d - FAX Receive failed to delete file %s", 
					m_index, rxfilep);
			}

			return(-3);
		}
      
		if( flags & DF_RAW )
		{
			// Raw receive, check if we need to continue
			phdcmd = ATFX_PHDCMD(m_faxdev);
			phdrpy = ATFX_PHDRPY(m_faxdev);
	 
			//
			// If we received EOP from transmitter, check to make sure
			// we didn't respond with an RTN or PIN.  If we did, the
			// transmitter may retransmit the page, so we need to issue
			// another fx_rcvfax() command.
			//
	 
			if( phdcmd == DFS_EOP && phdrpy != DFS_RTN && phdrpy != DFS_PIN )
			{
				// Get RemoteID
				if( fx_getparm(m_faxdev, FC_REMOTEID, (void *)sFromNo) != 0 )
				{
					WriteTrace(TraceWarn, "IVR Channel: %d - Fax Receive get remote id failed, ec=0x%x!", 
						m_index, ATDV_LASTERR(m_faxdev));
				}
				else
					strcpy(m_szRemoteID, sFromNo);
				break;  // completed, break from while loop
			}
	 
			// change file extension to "02.raw","03.raw","04.raw", etc.
			strcpy(buff, rxfile);
			if( (strp = strrchr(buff,'.')) != NULL )
			{
				sprintf(strp, "_%02d.raw", ++rxcount);
				rxfilep = buff;
			}
		}
		else
		{
			// Get RemoteID
			if( fx_getparm(m_faxdev, FC_REMOTEID, (void *)sFromNo) != 0 )
			{
				WriteTrace(TraceWarn, "IVR Channel: %d - Fax Receive get remote id failed, ec=0x%x!", 
					m_index, ATDV_LASTERR(m_faxdev));
			}
			else
				strcpy(m_szRemoteID, sFromNo);

			// TIFF/F receive, we are done
			break; // completed, break from while loop
		}
	}   // end of while
#endif

	return 0;   
}

// Sun added 2012-06-13
void Cchannelcontrol::fax_log_NegotiatedSpeed()
{
#ifdef INTEL_DIALOGIC
	if( m_faxdev )
	{
		if( !m_bGotBauRte )
		{
			long lv_ret = ATFX_SPEED(m_faxdev);

			if( lv_ret != AT_FAILURE )
			{
				m_bGotBauRte = true;
				WriteTrace(TraceDetail, "FAX IVR Channel: %d - negotiated fax speed = %ld", m_index, lv_ret);
			}
			else
			{
				WriteTrace(TraceInfo, "FAX IVR Channel: %d - failed to get negotiated fax speed", m_index);
			}
		}
	}
#endif
}

int Cchannelcontrol::ivr_channel_getstate(int nClass)
{
	int nState = VOICE_LS_ERROR;

#ifdef INTEL_DIALOGIC
	long nrtncode;

	switch( nClass )
	{
	case LINESTATUS_HOOKST:				// Hook State
		if( (nrtncode = ATDX_HOOKST(m_line)) != AT_FAILURE )
		{
			switch( nrtncode )
			{
			case DX_ONHOOK:
				nState = VOICE_LS_HOOKST_ONHOOK;
				break;
			case DX_OFFHOOK:
				nState = VOICE_LS_HOOKST_OFFHOOK;
				break;
			}
		}
		break;
	
	case LINESTATUS_LINEST:				// Line State
		if( (nrtncode = ATDX_LINEST(m_line)) != AT_FAILURE )
		{
			nState = 0;
			if( nrtncode & RLS_SILENCE )
				nState += VOICE_LS_LINEST_SILENCE;
			if( nrtncode & RLS_DTMF )
				nState += VOICE_LS_LINEST_DTMF;
			if( nrtncode & RLS_RING )
				nState += VOICE_LS_LINEST_RING;
			if( nrtncode & RLS_HOOK )
				nState += VOICE_LS_LINEST_HOOK;
			if( nrtncode & RLS_RINGBK )
				nState += VOICE_LS_LINEST_RINGBK;
		}
		break;

	default:			// Device State
		if( (nrtncode = ATDX_STATE(m_line)) != AT_FAILURE )
		{
			switch( nrtncode )
			{
			case CS_IDLE:
				nState = VOICE_LS_DEVST_IDLE;
				break;
			case CS_PLAY: 
				nState = VOICE_LS_DEVST_PLAY;
				break;
			case CS_RECD: 
				nState = VOICE_LS_DEVST_RECD;
				break;
			case CS_DIAL: 
				nState = VOICE_LS_DEVST_DIAL;
				break;
			case CS_GTDIG: 
				nState = VOICE_LS_DEVST_GTDIG;
				break;
			case CS_TONE: 
				nState = VOICE_LS_DEVST_TONE;
				break;
			case CS_STOPD: 
				nState = VOICE_LS_DEVST_STOPD;
				break;
			case CS_SENDFAX: 
				nState = VOICE_LS_DEVST_SENDFAX;
				break;
			case CS_RECVFAX: 
				nState = VOICE_LS_DEVST_RECVFAX;
				break;
			case CS_FAXIO: 
				nState = VOICE_LS_DEVST_FAXIO;
				break;
			case CS_HOOK: 
				nState = VOICE_LS_DEVST_HOOK;
				break;
			case CS_WINK: 
				nState = VOICE_LS_DEVST_WINK;
				break;
			case CS_CALL: 
				nState = VOICE_LS_DEVST_CALL;
				break;
			case CS_GETR2MF: 
				nState = VOICE_LS_DEVST_GETR2MF;
				break;
			case CS_RINGS: 
				nState = VOICE_LS_DEVST_RINGS;
				break;
			case CS_BLOCKED: 
				nState = VOICE_LS_DEVST_BLOCKED;
				break;
			}
		}
	}
#endif

	return nState;
}

// 播放数值
int	Cchannelcontrol::ivr_channel_digitplay(double f_Digit, int f_AfterDot, BYTE f_LangID)
{
	__int64 n1;
	int n2,d,temp,tempyi;
	char digitbuffer[10];
	char szNum[50];					// Sun enlarged 2007-10-30
	bool yi0=true,wan0=true,qian0=true,bai0=true,shi0=true;
	double initial = 0;
	long lv_mi = 0;

	if( f_AfterDot > 8 )
		f_AfterDot = 8;

	WriteTrace(TraceDetail, "IVR Channel: %d - ARS =>  %f play digital", m_index, f_Digit);

	if( f_Digit >=0 && f_Digit <= 0.0001 )
	{
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_0", f_LangID);
		if(ivr_channel_play(szNum) == -1)
			return -1;
	}
	// Sun added 2007-12-25, [A]
	else if( f_Digit < 0 )
	{
		// Play "负" or "Negative"
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_Negative", f_LangID);
		if(ivr_channel_play(szNum) == -1)
			return -1;

		initial = -f_Digit;
	}
	else
		initial = f_Digit;

    n1=(__int64)initial;						// 整数部分
	if( f_AfterDot > 0 )
	{
		lv_mi = pow(10, f_AfterDot);
		n2=((int)((initial-n1)*lv_mi))%lv_mi;		// 小数部分
	}
	else
		n2 = 0;

	temp=n1/10000;							// x万
	tempyi = temp/10000;					// 亿
	temp %= 10000;
	if( tempyi >= 10000 )					// 超过万亿，Too big to read
		return 0;

	// 读“亿位”
	if( tempyi >= 1 )
	{
		d=1;
		if( ivr_channel_digitplay(tempyi, 0, f_LangID) == -1 )
			return -1;
	}
	else
	{
		d=0;
		yi0=false;
	}
	if (d)
	{
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_YI", f_LangID);
		if(ivr_channel_play(szNum) == -1)
			return -1;
	}

	if( temp >= 1 )
	{
		d=1;
		if( ivr_channel_digitplay(temp, 0, f_LangID) == -1 )
			return -1;
	}
	else
	{
		d=0;
		wan0=false;
	}

	if (d)
	{
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_WAN", f_LangID);
		if(ivr_channel_play(szNum) == -1)
			return -1;
	}

	n1=n1%10000;
	temp=n1/1000;							// x千
		
	d=1;
	memset(digitbuffer, 0x00, sizeof(digitbuffer));
	digitbuffer[0] = temp+'0';
	if( digitbuffer[0] >= '1' && digitbuffer[0] <= '9' )
	{
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_%c", f_LangID, digitbuffer[0]);
		if(ivr_channel_play(szNum) == -1)
			return -1;
	}
	else
	{
		d=0;
		qian0=false;
	}
	if (d)
	{
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_QIAN", f_LangID);
		if(ivr_channel_play(szNum) == -1)
			return -1;
	}

	n1=n1%1000;
	temp=n1/100;							// 百
	d=1;
	memset(digitbuffer, 0x00, sizeof(digitbuffer));
	digitbuffer[0] = temp+'0';
	if( digitbuffer[0] >= '1' && digitbuffer[0] <= '9' )
	{
		if( (yi0 || wan0) && !qian0 )
		{
			memset(szNum, 0x00, sizeof(szNum));
			sprintf(szNum, "SYS_%d_0", f_LangID);
			if(ivr_channel_play(szNum) == -1)
				return -1;
		}
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_%c", f_LangID, digitbuffer[0]);
		if(ivr_channel_play(szNum) == -1)
			return -1;
	}
	else
	{
		d=0;
		bai0=false;
	}
	if (d)
	{
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_BAI", f_LangID);
		if(ivr_channel_play(szNum) == -1)
			return -1;
	}

	n1=n1%100;
	temp=n1/10;								// 十
	d=1;
	memset(digitbuffer, 0x00, sizeof(digitbuffer));
	digitbuffer[0] = temp+'0';
	if( digitbuffer[0] >= '1' && digitbuffer[0] <= '9' )
	{
		if( (yi0 || wan0 || qian0 ) && !bai0 )
		{
			memset(szNum, 0x00, sizeof(szNum));
			sprintf(szNum, "SYS_%d_0", f_LangID);
			if(ivr_channel_play(szNum) == -1)
				return -1;
		}
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_%c", f_LangID, digitbuffer[0]);
		if(ivr_channel_play(szNum) == -1)
			return -1;
	}
	else
	{
		d=0;
		shi0=false;
	}
	if (d)
	{
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_10", f_LangID);
		if(ivr_channel_play(szNum) == -1)
			return -1;
	}

	n1=n1%10;
	temp=n1;								// 个位
	d=1;
	memset(digitbuffer, 0x00, sizeof(digitbuffer));
	digitbuffer[0] = temp+'0';
	if( digitbuffer[0] >= '1' && digitbuffer[0] <= '9' )
	{
		if( (yi0 || wan0 || qian0 || bai0) && !shi0 )
		{
			memset(szNum, 0x00, sizeof(szNum));
			sprintf(szNum, "SYS_%d_0", f_LangID);
			if(ivr_channel_play(szNum) == -1)
				return -1;
		}
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_%c", f_LangID, digitbuffer[0]);
		if(ivr_channel_play(szNum) == -1)
			return -1;
	}
	else
		d=0;
	
	// 小数点
	if( n2 > 0 )
	{
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_DIAN", f_LangID);
		if(ivr_channel_play(szNum) == -1)
			return -1;

		/// 小数部分
		CString lv_sTemp;

		lv_sTemp.Format("0000000000%u", n2);
		memset(digitbuffer, 0x00, sizeof(digitbuffer));
		strncpy(digitbuffer, (LPCSTR)(lv_sTemp.Right(f_AfterDot)), f_AfterDot);

		if(ivr_channel_numberplay(digitbuffer, FALSE, f_LangID) == -1)
			return -1;
	}

	return 1;
}

// 播放金额
int Cchannelcontrol::ivr_channel_moneyplay(double f_FEE, BYTE f_LangID)
{
    __int64 n1;
    int n2,njiao,nfen,d,temp,tempyi;
	char digitbuffer[MAX_PATH];
	char szNum[MAX_PATH];
	bool blnYuan=false;
	bool yi0=true,wan0=true,qian0=true,bai0=true,shi0=true,jiao0=true;
	double initial;

	WriteTrace(TraceDebug, "IVR Channel: %d - ARS =>  %f play monkey", m_index, f_FEE);

	// 最多读到分
	/// Sun updated from abs() to fabs(), 2008-04-23
	if( fabs(f_FEE) < 0.01 )
	{
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_0", f_LangID);
		if(ivr_channel_play(szNum) == -1)
			return -1;
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_YUAN", f_LangID);
		if(ivr_channel_play(szNum) == -1)
			return -1;
		return 1;
	}
	// Sun added 2007-12-25, [A]
	else if( f_FEE < 0 )
	{
		// Play "负" or "Negative"
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_Negative", f_LangID);
		if(ivr_channel_play(szNum) == -1)
			return -1;

		initial = -f_FEE;
	}
	else
		initial=f_FEE;
	
    n1=(__int64)initial;					// 整数部分
	n2=((int)((initial-n1)*100))%100;		// 小数部分，Sun updated 2008-04-23
	njiao=n2/10;							// x角
	nfen=n2%10;								// x分

	temp=n1/10000;							// x万
	tempyi = temp/10000;					// 亿
	temp %= 10000;
	if( tempyi >= 10000 )					// 超过万亿，Too big to read
		return 0;
	
	// 读“亿位”
	if( tempyi >= 1 )
	{
		d=1;
		if( ivr_channel_digitplay(tempyi, 0, f_LangID) == -1 )
			return -1;
	}
	else
	{
		d=0;
		yi0=false;
	}
	if (d)
	{
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_YI", f_LangID);
		if(ivr_channel_play(szNum) == -1)
			return -1;
		blnYuan=true;
	}

	// 读“万位”
	if( temp >= 1 )
	{
		d=1;
		if( ivr_channel_digitplay(temp, 0, f_LangID) == -1 )
			return -1;
	}
	else
	{
		d=0;
		wan0=false;
	}

	if (d)
	{
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_WAN", f_LangID);
		if(ivr_channel_play(szNum) == -1)
			return -1;
		blnYuan=true;
	}

	n1=n1%10000;
	temp=n1/1000;							// x千
		
	d=1;
	memset(digitbuffer, 0x00, sizeof(digitbuffer));
	digitbuffer[0] = temp+'0';
	if( digitbuffer[0] >= '1' && digitbuffer[0] <= '9' )
	{
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_%c", f_LangID, digitbuffer[0]);
		if(ivr_channel_play(szNum) == -1)
			return -1;
	}
	else
	{
		d=0;
		qian0=false;
	}
	if (d)
	{
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_QIAN", f_LangID);
		if(ivr_channel_play(szNum) == -1)
			return -1;
		blnYuan=true;
	}

	n1=n1%1000;
	temp=n1/100;							// 百
	d=1;
	memset(digitbuffer, 0x00, sizeof(digitbuffer));
	digitbuffer[0] = temp+'0';
	if( digitbuffer[0] >= '1' && digitbuffer[0] <= '9' )
	{
		if( (yi0 || wan0) && !qian0 )
		{
			memset(szNum, 0x00, sizeof(szNum));
			sprintf(szNum, "SYS_%d_0", f_LangID);
			if(ivr_channel_play(szNum) == -1)
				return -1;
		}
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_%c", f_LangID, digitbuffer[0]);
		if(ivr_channel_play(szNum) == -1)
			return -1;
	}
	else
	{
		d=0;
		bai0=false;
	}
	if (d)
	{
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_BAI", f_LangID);
		if(ivr_channel_play(szNum) == -1)
			return -1;
		blnYuan=true;
	}

	n1=n1%100;
	temp=n1/10;								// 十
	d=1;
	memset(digitbuffer, 0x00, sizeof(digitbuffer));
	digitbuffer[0] = temp+'0';
	if( digitbuffer[0] >= '1' && digitbuffer[0] <= '9' )
	{
		if( (yi0 || wan0 || qian0 ) && !bai0 )
		{
			memset(szNum, 0x00, sizeof(szNum));
			sprintf(szNum, "SYS_%d_0", f_LangID);
			if(ivr_channel_play(szNum) == -1)
				return -1;
		}
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_%c", f_LangID, digitbuffer[0]);
		if(ivr_channel_play(szNum) == -1)
			return -1;
	}
	else
	{
		d=0;
		shi0=false;
	}
	if (d)
	{
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_10", f_LangID);
		if(ivr_channel_play(szNum) == -1)
			return -1;
		blnYuan=true;
	}

	n1=n1%10;
	temp=n1;								// 元
	d=1;
	memset(digitbuffer, 0x00, sizeof(digitbuffer));
	digitbuffer[0] = temp+'0';
	if( digitbuffer[0] >= '1' && digitbuffer[0] <= '9' )
	{
		if( (yi0 || wan0 || qian0 || bai0) && !shi0 )
		{
			memset(szNum, 0x00, sizeof(szNum));
			sprintf(szNum, "SYS_%d_0", f_LangID);
			if(ivr_channel_play(szNum) == -1)
				return -1;
		}
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_%c", f_LangID, digitbuffer[0]);
		if(ivr_channel_play(szNum) == -1)
			return -1;
	}
	else
		d=0;
	if(d)
		blnYuan=true;
	
	if(blnYuan)
	{
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_YUAN", f_LangID);
		if(ivr_channel_play(szNum) == -1)
			return -1;
	}
			
	// 角
	d=1;
	memset(digitbuffer, 0x00, sizeof(digitbuffer));
	digitbuffer[0] = njiao+'0';
	if( digitbuffer[0] >= '1' && digitbuffer[0] <= '9' )
	{
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_%c", f_LangID, digitbuffer[0]);
		if(ivr_channel_play(szNum) == -1)
			return -1;
	}
	else
	{
		d=0;
		jiao0 = false;
	}
	if (d)
	{
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_JIAO", f_LangID);
		if(ivr_channel_play(szNum) == -1)
			return -1;
	}

	// 分
	d=1;
	memset(digitbuffer, 0x00, sizeof(digitbuffer));
	digitbuffer[0] = nfen+'0';
	if( digitbuffer[0] >= '1' && digitbuffer[0] <= '9' )
	{
		if( blnYuan && !jiao0 )
		{
			memset(szNum, 0x00, sizeof(szNum));
			sprintf(szNum, "SYS_%d_0", f_LangID);
			if(ivr_channel_play(szNum) == -1)
				return -1;
		}
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_%c", f_LangID, digitbuffer[0]);
		if(ivr_channel_play(szNum) == -1)
			return -1;
	}
	else
		d=0;
	if (d)
	{
		memset(szNum, 0x00, sizeof(szNum));
		sprintf(szNum, "SYS_%d_FEN", f_LangID);
		if(ivr_channel_play(szNum) == -1)
			return -1;
	}

	return 1;
}

// 播放时间
/// 参数：
/// f_Seconds - 以秒计算的时间长度
/// f_Type - 播报方式：
///					0 - 不播报；
///					1 -- 时分秒；
///					2 -- 分秒；
///					3 -- 分（四舍五入）；
///					4 -- 秒
///					8 - 播报：n点n分n秒；
int	Cchannelcontrol::ivr_channel_timeplay(DWORD f_Seconds, BYTE f_Type, BYTE f_LangID)
{
	char filename[MAX_PATH];
	int lv_nHour, lv_nMin, lv_nSec;

	lv_nHour = (int)(f_Seconds / 3600);
	lv_nMin = (int)((f_Seconds % 3600) / 60);
	lv_nSec = f_Seconds % 60;
	if( f_Type > 0 && f_Seconds > 0 )
	{
		/// 播放时间
		//// Hours
		if( (f_Type == 1 || f_Type == 8 ) && lv_nHour > 0 )		// HH:MM:SS
		{
			if( ivr_channel_digitplay(lv_nHour, 0, f_LangID) == -1 )
				return -1;
			memset(filename, 0x00, sizeof(filename));
			if( f_Type == 8 && f_LangID == 0 )
				sprintf(filename, "SYS_%d_DIAN", f_LangID);
			else
				sprintf(filename, "SYS_%d_HOUR", f_LangID);
			if( ivr_channel_play(filename) == -1 )
				return -1;
			lv_nHour = 0;
		}
		//// Minutes
		if( f_Type > 1 )
		{
			lv_nMin += (lv_nHour * 60);
			if( f_Type == 3 && lv_nSec >= 30 )
				lv_nMin++;
		}
		if( (f_Type != 4) && (lv_nMin > 0 || f_Type == 3) )
		{
			if( ivr_channel_digitplay(lv_nMin, 0, f_LangID) == -1 )
				return -1;
			memset(filename, 0x00, sizeof(filename));
			sprintf(filename, "SYS_%d_MINUTE", f_LangID);
			if( ivr_channel_play(filename) == -1 )
				return -1;
			lv_nMin = 0;
		}
		//// Seconds
		if( f_Type != 3 && lv_nSec > 0 )
		{
			lv_nSec += (lv_nMin * 60);
			if( ivr_channel_digitplay(lv_nSec, 0, f_LangID) == -1 )
				return -1;
			memset(filename, 0x00, sizeof(filename));
			sprintf(filename, "SYS_%d_SECOND", f_LangID);
			if( ivr_channel_play(filename) == -1 )
				return -1;
		}
	}

	return 1;
}

// Sun added 2005-05-27
int	Cchannelcontrol::sc_channel_link(Cchannelcontrol* f_nLinkTo)
{
#ifdef INTEL_DIALOGIC
	if( f_nLinkTo == this )
		return 0;

	// Sun updated 2010-10-20 to support Analog Link
	/// From
	/*
	//chg by steven 2005-06-20
	return( nr_scroute(m_hDevTimeSlot, SC_DTI, f_nLinkTo->m_hDevTimeSlot, SC_DTI, SC_FULLDUP) == 0 );
	*/
	/// To
	int lv_nLineType1, lv_nLineType2, lv_ret;
	int lv_nDev1, lv_nDev2;
	if( this->m_nDevType == DEV_ANALOG )
	{
		lv_nLineType1 = SC_LSI;
		lv_nDev1 = m_line;
	}
	else
	{
		lv_nLineType1 = SC_DTI;
		lv_nDev1 = m_hDevTimeSlot;
	}
	if( f_nLinkTo->m_nDevType == DEV_ANALOG )
	{
		lv_nLineType2 = SC_LSI;
		lv_nDev2 = f_nLinkTo->m_line;
	}
	else
	{
		lv_nLineType2 = SC_DTI;
		lv_nDev2 = f_nLinkTo->m_hDevTimeSlot;
	}
	lv_ret = nr_scroute(lv_nDev1, lv_nLineType1, lv_nDev2, lv_nLineType2, SC_FULLDUP);
	if( lv_ret != 0 )
	{
		WriteTrace(TraceWarn, "IVR Channel: %d - Error when link channel linedev :%d (%s - Type:%d TS:%d) to linedev :%d (%s - Type:%d TS:%d) ERROR: %s",
			m_index, m_line, m_sChannelName, m_nDevType, m_hDevTimeSlot, 
			f_nLinkTo->m_line, f_nLinkTo->m_sChannelName, f_nLinkTo->m_nDevType, f_nLinkTo->m_hDevTimeSlot,
			ATDV_ERRMSGP(lv_nDev1));
	}

	return( lv_ret == 0 );

#else
	return 1;
#endif
}

// Sun added 2005-05-27
int	Cchannelcontrol::sc_channel_unlink(Cchannelcontrol* f_nLinkTo)
{
#ifdef INTEL_DIALOGIC
	if( f_nLinkTo == this )
		return 0;

	// Sun updated 2010-10-20 to support Analog Link
	/// From
	/*
	//chg by steven 2005-06-20
	nr_scunroute(m_hDevTimeSlot, SC_DTI, f_nLinkTo->m_hDevTimeSlot, SC_DTI, SC_FULLDUP);
	nr_scroute(f_nLinkTo->GetVoxHandle(), SC_VOX, f_nLinkTo->m_hDevTimeSlot, SC_DTI, SC_FULLDUP);
	return( nr_scroute(m_line, SC_VOX, m_hDevTimeSlot, SC_DTI, SC_FULLDUP) == 0 );
	*/
	/// To
	int lv_nLineType1, lv_nLineType2, lv_ret;
	int lv_nDev1, lv_nDev2;
	if( this->m_nDevType == DEV_ANALOG )
	{
		lv_nLineType1 = SC_LSI;
		lv_nDev1 = m_line;
	}
	else
	{
		lv_nLineType1 = SC_DTI;
		lv_nDev1 = m_hDevTimeSlot;
	}
	if( f_nLinkTo->m_nDevType == DEV_ANALOG )
	{
		lv_nLineType2 = SC_LSI;
		lv_nDev2 = f_nLinkTo->m_line;
	}
	else
	{
		lv_nLineType2 = SC_DTI;
		lv_nDev2 = f_nLinkTo->m_hDevTimeSlot;
	}
	nr_scunroute(lv_nDev1, lv_nLineType1, lv_nDev2, lv_nLineType2, SC_FULLDUP);
	nr_scroute(f_nLinkTo->GetVoxHandle(), SC_VOX, lv_nDev2, lv_nLineType2, SC_FULLDUP);
	lv_ret = nr_scroute(m_line, SC_VOX, lv_nDev1, lv_nLineType1, SC_FULLDUP);
	return( lv_ret == 0 );

#else
	return 1;
#endif
}

// Sun added 2006-10-01
int	Cchannelcontrol::sc_fax_link(Cchannelcontrol* f_nLinkTo)
{
	int lv_retval = 0;

	if( f_nLinkTo == NULL )
		return 0;

#ifdef INTEL_DIALOGIC

	// Break the full-duplex connection between the Voice 
	/// channel device and the Network interface device.
	dx_unlisten(m_line);
	fx_unlisten(f_nLinkTo->m_faxdev);
	if( m_dtidev > 0 )
		dt_unlisten(m_dtidev);

	// Set full-duplex connection between the FAX
	/// channel device and the Network interface device
	if( m_dtidev > 0 )
	{
		lv_retval = nr_scroute(m_hDevTimeSlot, SC_DTI, f_nLinkTo->m_faxdev, SC_FAX, SC_FULLDUP);
		if( lv_retval != 0 )
		{
			WriteTrace(TraceWarn, "IVR Channel: %d - Error when link fax channel linedev :%d (%s - Type:%d TS:%d) to linedev :%d (%s - Type:%d TS:%d) Fax Resource id: %d, ERROR: %s",
				m_index, m_line, m_sChannelName, m_nDevType, m_hDevTimeSlot, 
				f_nLinkTo->m_line, f_nLinkTo->m_sChannelName, f_nLinkTo->m_nDevType, f_nLinkTo->m_hDevTimeSlot,
				f_nLinkTo->m_faxdev, ATDV_ERRMSGP(m_hDevTimeSlot));
		}
	}
	else
	{
		lv_retval = nr_scroute(m_line, SC_LSI, f_nLinkTo->m_faxdev, SC_FAX, SC_FULLDUP);
		if( lv_retval != 0 )
		{
			WriteTrace(TraceWarn, "IVR Channel: %d - Error when link fax channel linedev :%d (%s - Type:%d TS:%d) to linedev :%d (%s - Type:%d TS:%d) Fax Resource id: %d, ERROR: %s",
				m_index, m_line, m_sChannelName, m_nDevType, m_hDevTimeSlot, 
				f_nLinkTo->m_line, f_nLinkTo->m_sChannelName, f_nLinkTo->m_nDevType, f_nLinkTo->m_hDevTimeSlot,
				f_nLinkTo->m_faxdev, ATDV_ERRMSGP(m_line));
		}
	}

#endif

	return( lv_retval == 0 );
}

// Sun added 2006-10-01
int	Cchannelcontrol::sc_fax_unlink(long f_lngFaxDev)
{
#ifdef INTEL_DIALOGIC
	fx_unlisten(f_lngFaxDev);

	// Recover the full-duplex connection between the Voice
	/// channel device and the Network interface device.
	ivr_channel_route();
#endif

	return 1;
}
