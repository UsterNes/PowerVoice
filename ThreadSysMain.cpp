// ThreadSysMain.cpp: implementation of the CThreadSysMain class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PowerVoice.h"
#include "MainFrm.h"
#include "ThreadSysMain.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CThreadSysMain::CThreadSysMain()
{
	// 2004-08-07
	m_threadProgress = 0;
}

CThreadSysMain::~CThreadSysMain()
{

}

//==========================================================
// Thread member functions
BOOL CThreadSysMain::WaitForStartupToComplete()
{
	m_shutdown.Create();
	m_shutdown.Reset();
	CMyThread::Start();

	return TRUE;
}

void CThreadSysMain::WaitForShutdownToComplete()
{
	// if we havent already started a shut down, do so...
	if( m_shutdown.GetHANDLE() != INVALID_HANDLE_VALUE )
	{
		m_shutdown.Set();
		if( !Wait(2000) )
		{
			WriteTrace(TraceWarn, "Problem while killing System Main Thread!");

			// Sun added 2010-08-05
			try
			{
				TerminateThread(CMyThread::GetHandle(), 0);
			}
			catch(...)
			{
			}
		}
	}
}

// 2004-08-07
DWORD CThreadSysMain::GetPendingTime()
{
	//--------------------------------------------
	// Sun added 2007-05-22, [B]
	DWORD lv_ticknow = GetTickCount();
	if( lv_ticknow < m_dwPendTick )
	{
		m_dwPendTick = lv_ticknow;
		return 0;
	}
	//--------------------------------------------
	
	return( (lv_ticknow - m_dwPendTick) / 1000 );
}

void CThreadSysMain::ResetPendingTime()
{
	m_dwPendTick = GetTickCount();
}

int CThreadSysMain::Run()
{
	DWORD ta, ta1;
	DWORD tb;
	static int lv_nIndex = 0;
	BYTE lv_msg[PCS_MAX_MESSAGE_SIZE];
	size_t lv_len;
	S_WM_CTIONHOOKEVENT *lv_pCTIOnHook;
	CIVRChannel *lv_pAgent;
	int lv_nProcessTimer;

	ta = ta1 = GetTickCount();
	g_MQCTIOnHookEvents.SetMQName(_T("CTI_OnHook"));
	lv_pCTIOnHook = (S_WM_CTIONHOOKEVENT *)lv_msg;

	while( !m_shutdown.Wait(0) )
	{
		// 2004-08-07
		m_threadProgress = 1000 + lv_nIndex;;
		m_dwPendTick = GetTickCount();

		if( WaitForSingleObject(m_shutdown.GetHANDLE(), 0) == WAIT_OBJECT_0 )
			break;

		try
		{
			if( pMainWindow->m_bRunning )
			{
				// 2004-08-07
				m_threadProgress = 2000 + lv_nIndex;;

				// Indicate runing
				tb = GetTickCount();
				//--------------------------------------------
				// Sun added 2007-05-22, [B]
				if( tb < ta )
					ta = tb;
				//--------------------------------------------
				else if( (tb - ta) > 800 )
				{
					//pMainWindow->ProgressStep();
					pMainWindow->PostMessage(WM_MOVEPROGRESSBAR);
					ta = tb;
				}

				// 2004-08-07
				m_threadProgress = 3000 + lv_nIndex;;

				// CTI On-Hook Event
				while( g_MQCTIOnHookEvents.GetMessage(lv_msg, &lv_len) )
				{
					if( lv_len == sizeof(S_WM_CTIONHOOKEVENT) )
					{
						if( (US)(tb - lv_pCTIOnHook->lngTickCount) < theApp.m_nCTIOnHookDelay )
							break;
						else 
						{
							// 2004-08-07
							m_threadProgress = 4000 + lv_nIndex;;

							lv_pAgent = (CIVRChannel *)lv_pCTIOnHook->pChannel;
							WriteTrace(TraceDebug, "Main thread process set onhook cti-msg on IVR Channel: %d, Call ID: 0x%x", lv_pAgent->GetLineNO(), lv_pCTIOnHook->lngCallRefID);
							if( lv_pAgent->GetCallRefID() == lv_pCTIOnHook->lngCallRefID || lv_pAgent->GetNewCallRefID() == lv_pCTIOnHook->lngCallRefID )
							{
								// 2004-08-07
								m_threadProgress = 5000 + lv_nIndex;;

								//-----------------------------------------
								/// Sun updated 2010-07-20 [D]
								/// From
								//lv_pAgent->SetHookOn();
								/// To
								lv_pAgent->AnsySetHookOn(theApp.m_bytCTIAnsyOnHook);
								//-----------------------------------------
							}
						}
					}
					g_MQCTIOnHookEvents.RemoveMessage();

					// 2004-08-07
					m_threadProgress = 6000 + lv_nIndex;;
				}

				// 2004-08-07
				m_threadProgress = 7000 + lv_nIndex;;

				// Channel Checking
				//--------------------------------------------
				// Sun added 2007-05-22, [B]
				if( tb < ta1 )
					ta1 = tb;
				//--------------------------------------------
				else if( (tb - ta1) > 2000 )
				{
					ta1 = tb;
					for( int lv_loop = 0; lv_loop < theApp.m_nIVRChannels; lv_loop++ )
					{
						if( PIVRCH[lv_nIndex] != NULL )
						{
							// Sun Remove condition 2004-09-14
							//if( PIVRCH[lv_nIndex]->m_oARS.m_nDevType >= DEV_DIGIT )
							//{
								m_threadProgress = 10000 + lv_nIndex;

								lv_nProcessTimer = PIVRCH[lv_nIndex]->ProcessTimer();
								if( lv_nProcessTimer < 0 )
								{
									pMainWindow->m_bRunning = FALSE;

									// Inform Main-Form to Reinitialize Channels
									pMainWindow->PostMessage(WM_SYS_REINIT_DEVICE);
									break;
								}
								else if( lv_nProcessTimer == 0 )
								{
									m_threadProgress = 8000 + lv_nIndex;
									PIVRCH[lv_nIndex]->CTI_ErrorHandler();
									m_threadProgress = 9000 + lv_nIndex;;
								}
							//}

							// Sun added 2005-04-01
							if( GetTickCount() - tb > 2000 )
							{
								lv_nIndex++;
								if( lv_nIndex >= theApp.m_nIVRChannels )
									lv_nIndex = 0;
								break;
							}
						}

						lv_nIndex++;
						if( lv_nIndex >= theApp.m_nIVRChannels )
							lv_nIndex = 0;
					}
					ta1 = GetTickCount();
				}
			}
		}
		catch(...)
		{
			WriteTrace(TraceError, _T("CThreadSysMain::Run() - Unexpected exception"));
		}

		Sleep(50);
	}

	WriteTrace(TraceWarn, "Notes: Main thread exited!!!");
	return 0;
}

