// IVRClient_Admin.cpp: implementation of the CIVRClient_Admin class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PowerVoice.h"
#include "IVRClient_Admin.h"
#include "ClientThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIVRClient_Admin::CIVRClient_Admin()
:	m_pSocket(NULL),
	m_nVersionNo(0)
{

}

CIVRClient_Admin::~CIVRClient_Admin()
{

}

BYTE CIVRClient_Admin::GetVersion()
{
	return m_nVersionNo;
}

void CIVRClient_Admin::SetVersion(BYTE f_nValue)
{
	if( m_nVersionNo != f_nValue )
		m_nVersionNo = f_nValue;
}

PVOID CIVRClient_Admin::GetConnected()
{
	PVOID lv_cn;

	m_bLock.Enter();
	lv_cn = m_pSocket;
	m_bLock.Leave();

	return lv_cn;
}

void CIVRClient_Admin::SetConnected(PVOID f_cn)
{
	m_bLock.Enter();

	if( m_pSocket == NULL )
		m_pSocket = f_cn;

	m_bLock.Leave();
}

void CIVRClient_Admin::SetDisconnected()
{
	m_bLock.Enter();
	if( m_pSocket != NULL )
		m_pSocket = NULL;
	m_bLock.Leave();
}

int CIVRClient_Admin::ProcessTCPMessage(SMsgHeader f_hdr, const BYTE *f_bdy)
{
	///////////////////////////////////////////
	// Set Header
	SMsgHeader lv_sndhdr;
	memset(&lv_sndhdr, 0x00, sizeof(lv_sndhdr));
	lv_sndhdr.Flag0 = PCS_MSGHDR_FLAG0;
	lv_sndhdr.Flag1 = PCS_MSGHDR_FLAG1;
	lv_sndhdr.Flag2 = PCS_MSGHDR_FLAG2;
	lv_sndhdr.Flag3 = PCS_MSGHDR_FLAG3;
	lv_sndhdr.Sender = USER_IVR;
	lv_sndhdr.Receiver = USER_PROGRAM;
	///////////////////////////////////////////

	BYTE lv_sndmsg[PCS_MAX_MESSAGE_SIZE];
	CClientThread *lv_pThread = (CClientThread *)GetConnected();
	CIVRGroup *lv_pGroup;
	CIVRChannel *lv_pAgent = NULL;

	if( lv_pThread == NULL )
		return 0;

	switch( f_hdr.Command )
	{
	//-----------------------------------------------------------------------
	// Sun added 2006-03-05
	// IVR组监控启动消息
	case TCP_ADM2IVR_GROUP_MON:
	case TCP_ADM2IVR_GROUP_UNMON:
		{
			S_TCP_ADM2IVR_GROUP_MON lv_rcvbdy;
			UC lv_retval = PCS_DEF_RETCODE_NOTEXIST;
			BOOL lv_blnSW;
			
			/// Message Analysis
			if( f_hdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, f_bdy, sizeof(lv_rcvbdy));

			lv_blnSW = (f_hdr.Command == TCP_ADM2IVR_GROUP_MON);
			try
			{
				if( lv_rcvbdy.GroupID >= 0 && lv_rcvbdy.GroupID < theApp.m_nIVRGroups )
				{
					if( (lv_pGroup = PIVRGroup[lv_rcvbdy.GroupID]) != NULL )
					{
						if( lv_pGroup->GetIsMonitorred() == lv_blnSW )
							lv_retval = PCS_DEF_RETCODE_NOTNEED;
						else
						{
							lv_pGroup->SetIsMonitorred(lv_blnSW);
							lv_retval = PCS_DEF_RETCODE_SUCC;
						}
					}
				}
			}
			catch(...)
			{
				lv_retval = PCS_DEF_RETCODE_SYSERROR;
				WriteTrace(TraceError, "Sock process group monitor msg goes error on group:[%d]", 
					lv_rcvbdy.GroupID);
			}

			if( lv_retval != PCS_DEF_RETCODE_SUCC )
				lv_pGroup->MQ_SendMsg_GroupMonitorAck(lv_blnSW, lv_retval);
		}
		break;

	// IVR组加载流程消息
	case TCP_ADM2IVR_GROUP_LOAD_CALLFLOW:
		{
			S_TCP_ADM2IVR_GROUP_LOAD_CALLFLOW lv_rcvbdy;
						
			/// Message Analysis
			if( f_hdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, f_bdy, sizeof(lv_rcvbdy));

			try
			{
				if( lv_rcvbdy.GroupID >= 0 && lv_rcvbdy.GroupID < theApp.m_nIVRGroups )
				{
					if( (lv_pGroup = PIVRGroup[lv_rcvbdy.GroupID]) != NULL )
					{
						lv_pGroup->SetIVRProgramID(lv_rcvbdy.CallFlowID);
						if( lv_pGroup->Init_SelectET() )
						{
							// Save Setting
							CString lv_sCaption;
							PGroupInfo[lv_rcvbdy.GroupID].IVRProgramID = lv_pGroup->GetIVRProgramID();
							lv_sCaption.Format("GROUP%u_IVRProgramID", lv_rcvbdy.GroupID + 1);
							theApp.WriteProfileInt("GROUPINFO", lv_sCaption, PGroupInfo[lv_rcvbdy.GroupID].IVRProgramID);
						}
						else
						{
							// Restore Data
							lv_pGroup->SetIVRProgramID(PGroupInfo[lv_rcvbdy.GroupID].IVRProgramID);
						}
					}
				}
			}
			catch(...)
			{
				WriteTrace(TraceError, "Sock process group load call flow [%d] msg goes error on group:[%d]", 
					lv_rcvbdy.CallFlowID, lv_rcvbdy.GroupID);
			}
		}
		break;

	// IVR组加载资源消息
	case TCP_ADM2IVR_GROUP_LOAD_RESOURCE:
		{
			S_TCP_ADM2IVR_GROUP_LOAD_RESOURCE lv_rcvbdy;
			
			/// Message Analysis
			if( f_hdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, f_bdy, sizeof(lv_rcvbdy));

			try
			{
				if( lv_rcvbdy.GroupID >= 0 && lv_rcvbdy.GroupID < theApp.m_nIVRGroups )
				{
					if( (lv_pGroup = PIVRGroup[lv_rcvbdy.GroupID]) != NULL )
					{
						if( lv_pGroup->LoadResourceMap(lv_rcvbdy.ResourceID) )
							lv_pGroup->SetResourcePrjID(lv_rcvbdy.ResourceID);
					}
				}
			}
			catch(...)
			{
				WriteTrace(TraceError, "Sock process group load resource [%d] msg goes error on group:[%d]", 
					lv_rcvbdy.ResourceID, lv_rcvbdy.GroupID);
			}
		}
		break;

	// IVR组定时加载流程消息
	case TCP_ADM2IVR_GROUP_TIME_CALLFLOW:
		{
			S_TCP_ADM2IVR_GROUP_TIME_CALLFLOW lv_rcvbdy;
			S_TCP_IVR2ADM_GROUP_TIME_CALLFLOW_ACK lv_sndbdy;
			
			/// Message Analysis
			if( f_hdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, f_bdy, sizeof(lv_rcvbdy));

			/// Prepare Sending Message
			lv_sndhdr.Command = TCP_IVR2ADM_GROUP_TIME_CALLFLOW_ACK;
			lv_sndhdr.PackageLen = sizeof(SMsgHeader) + sizeof(lv_sndbdy);
			memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));

			lv_sndbdy.GroupID = lv_rcvbdy.GroupID;
			lv_sndbdy.CallFlowID = lv_rcvbdy.CallFlowID;
			strcpy(lv_sndbdy.LoadTime, lv_rcvbdy.LoadTime);
			lv_sndbdy.ErrorCode = PCS_DEF_RETCODE_NOTEXIST;

			try
			{
				if( lv_rcvbdy.GroupID >= 0 && lv_rcvbdy.GroupID < theApp.m_nIVRGroups )
				{
					if( (lv_pGroup = PIVRGroup[lv_rcvbdy.GroupID]) != NULL )
					{
						CTime lv_tmTime;
						if( ConvertStringToCTime(lv_rcvbdy.LoadTime, lv_tmTime) )
						{
							lv_pGroup->SetAutoLoadCallFlow(TRUE, lv_tmTime, lv_rcvbdy.CallFlowID);
							lv_sndbdy.ErrorCode = PCS_DEF_RETCODE_SUCC;
							WriteTrace(TraceInfo, "Info - IVR Group: %d received msg to set timely load call flow: %d at %s", 
								lv_rcvbdy.GroupID, lv_rcvbdy.CallFlowID, lv_tmTime.Format("%Y-%m-%d %H:%M:%S"));
						}
						else
						{
							lv_sndbdy.ErrorCode = PCS_DEF_RETCODE_OTHERS;
							WriteTrace(TraceWarn, "Warn - IVR Group: %d received msg to set timely load call flow: %d at wrong time %s", 
								lv_rcvbdy.GroupID, lv_rcvbdy.CallFlowID, lv_rcvbdy.LoadTime);
						}
					}
				}
			}
			catch(...)
			{
				lv_sndbdy.ErrorCode = PCS_DEF_RETCODE_SYSERROR;
				WriteTrace(TraceError, "Sock process group timely load call flow [%d] at [%s] goes error on group:[%d]", 
					lv_rcvbdy.CallFlowID, lv_rcvbdy.LoadTime, lv_rcvbdy.GroupID);
			}

			//// Send Message
			memcpy(lv_sndmsg, &lv_sndhdr, sizeof(SMsgHeader));
			memcpy(lv_sndmsg+sizeof(SMsgHeader), &lv_sndbdy, sizeof(lv_sndbdy));
			lv_pThread->m_ClientSocket.AddMessage(lv_sndmsg, lv_sndhdr.PackageLen);
		}
		break;

	// IVR组定时加载资源消息
	case TCP_ADM2IVR_GROUP_TIME_RESOURCE:
		{
			S_TCP_ADM2IVR_GROUP_TIME_RESOURCE lv_rcvbdy;
			S_TCP_IVR2ADM_GROUP_TIME_RESOURCE_ACK lv_sndbdy;
			
			/// Message Analysis
			if( f_hdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, f_bdy, sizeof(lv_rcvbdy));

			/// Prepare Sending Message
			lv_sndhdr.Command = TCP_IVR2ADM_GROUP_TIME_RESOURCE_ACK;
			lv_sndhdr.PackageLen = sizeof(SMsgHeader) + sizeof(lv_sndbdy);
			memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));

			lv_sndbdy.GroupID = lv_rcvbdy.GroupID;
			lv_sndbdy.ResourceID = lv_rcvbdy.ResourceID;
			strcpy(lv_sndbdy.LoadTime, lv_rcvbdy.LoadTime);
			lv_sndbdy.ErrorCode = PCS_DEF_RETCODE_NOTEXIST;

			try
			{
				if( lv_rcvbdy.GroupID >= 0 && lv_rcvbdy.GroupID < theApp.m_nIVRGroups )
				{
					if( (lv_pGroup = PIVRGroup[lv_rcvbdy.GroupID]) != NULL )
					{
						CTime lv_tmTime;
						if( ConvertStringToCTime(lv_rcvbdy.LoadTime, lv_tmTime) )
						{
							lv_pGroup->SetAutoLoadResource(TRUE, lv_tmTime, lv_rcvbdy.ResourceID);
							lv_sndbdy.ErrorCode = PCS_DEF_RETCODE_SUCC;
							WriteTrace(TraceInfo, "Info - IVR Group: %d received msg to set timely load resource: %d at %s", 
								lv_rcvbdy.GroupID, lv_rcvbdy.ResourceID, lv_tmTime.Format("%Y-%m-%d %H:%M:%S"));
						}
						else
						{
							lv_sndbdy.ErrorCode = PCS_DEF_RETCODE_OTHERS;
							WriteTrace(TraceWarn, "Warn - IVR Group: %d received msg to set timely load resource: %d at wrong time %s", 
								lv_rcvbdy.GroupID, lv_rcvbdy.ResourceID, lv_rcvbdy.LoadTime);
						}
					}
				}
			}
			catch(...)
			{
				lv_sndbdy.ErrorCode = PCS_DEF_RETCODE_SYSERROR;
				WriteTrace(TraceError, "Sock process group timely load resource [%d] at [%s] goes error on group:[%d]", 
					lv_rcvbdy.ResourceID, lv_rcvbdy.LoadTime, lv_rcvbdy.GroupID);
			}

			//// Send Message
			memcpy(lv_sndmsg, &lv_sndhdr, sizeof(SMsgHeader));
			memcpy(lv_sndmsg+sizeof(SMsgHeader), &lv_sndbdy, sizeof(lv_sndbdy));
			lv_pThread->m_ClientSocket.AddMessage(lv_sndmsg, lv_sndhdr.PackageLen);
		}
		break;

	// IVR通道监控启动消息
	case TCP_ADM2IVR_CH_MON:
		{
			S_TCP_ADM2IVR_CH_MON lv_rcvbdy;
			S_TCP_IVR2ADM_CH_MON_ACK lv_sndbdy;
			
			/// Message Analysis
			if( f_hdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, f_bdy, sizeof(lv_rcvbdy));

			/// Prepare Sending Message
			lv_sndhdr.Command = TCP_IVR2ADM_CH_MON_ACK;
			lv_sndhdr.PackageLen = sizeof(SMsgHeader) + sizeof(lv_sndbdy);
			memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));

			lv_sndbdy.GroupID = lv_rcvbdy.GroupID;
			lv_sndbdy.ChannelID = lv_rcvbdy.ChannelID;
			lv_sndbdy.ErrorCode = PCS_DEF_RETCODE_NOTEXIST;

			try
			{
				if( lv_rcvbdy.GroupID >= 0 && lv_rcvbdy.GroupID < theApp.m_nIVRGroups )
				{
					if( (lv_pGroup = PIVRGroup[lv_rcvbdy.GroupID]) != NULL )
						lv_sndbdy.ErrorCode = lv_pGroup->SetMonitorChannel(lv_rcvbdy.ChannelID);
				}
			}
			catch(...)
			{
				lv_sndbdy.ErrorCode = PCS_DEF_RETCODE_SYSERROR;
				WriteTrace(TraceError, "Sock process channel monitor msg [%d] goes error on group:[%d]", 
					lv_rcvbdy.ChannelID, lv_rcvbdy.GroupID);
			}

			//// Send Message
			memcpy(lv_sndmsg, &lv_sndhdr, sizeof(SMsgHeader));
			memcpy(lv_sndmsg+sizeof(SMsgHeader), &lv_sndbdy, sizeof(lv_sndbdy));
			lv_pThread->m_ClientSocket.AddMessage(lv_sndmsg, lv_sndhdr.PackageLen);
		}
		break;

	// IVR通道监控结束消息
	case TCP_ADM2IVR_CH_UNMON:
		{
			S_TCP_ADM2IVR_CH_UNMON lv_rcvbdy;
			S_TCP_IVR2ADM_CH_UNMON_ACK lv_sndbdy;
			
			/// Message Analysis
			if( f_hdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, f_bdy, sizeof(lv_rcvbdy));

			/// Prepare Sending Message
			lv_sndhdr.Command = TCP_IVR2ADM_CH_UNMON_ACK;
			lv_sndhdr.PackageLen = sizeof(SMsgHeader) + sizeof(lv_sndbdy);
			memset(&lv_sndbdy, 0x00, sizeof(lv_sndbdy));

			lv_sndbdy.GroupID = lv_rcvbdy.GroupID;
			lv_sndbdy.ChannelID = lv_rcvbdy.ChannelID;
			lv_sndbdy.ErrorCode = PCS_DEF_RETCODE_NOTEXIST;

			try
			{
				if( lv_rcvbdy.GroupID >= 0 && lv_rcvbdy.GroupID < theApp.m_nIVRGroups )
				{
					if( (lv_pGroup = PIVRGroup[lv_rcvbdy.GroupID]) != NULL )
						lv_sndbdy.ErrorCode = lv_pGroup->SetUnMonitorChannel(lv_rcvbdy.ChannelID);
				}
			}
			catch(...)
			{
				lv_sndbdy.ErrorCode = PCS_DEF_RETCODE_SYSERROR;
				WriteTrace(TraceError, "Sock process channel unmonitor msg [%d] goes error on group:[%d]", 
					lv_rcvbdy.ChannelID, lv_rcvbdy.GroupID);
			}

			//// Send Message
			memcpy(lv_sndmsg, &lv_sndhdr, sizeof(SMsgHeader));
			memcpy(lv_sndmsg+sizeof(SMsgHeader), &lv_sndbdy, sizeof(lv_sndbdy));
			lv_pThread->m_ClientSocket.AddMessage(lv_sndmsg, lv_sndhdr.PackageLen);
		}
		break;

	// IVR通道流程跳转控制消息
	case TCP_ADM2IVR_NODE_TRANSFER:
		{
			S_TCP_ADM2IVR_NODE_TRANSFER lv_rcvbdy;
			
			/// Message Analysis
			if( f_hdr.PackageLen < sizeof(SMsgHeader) + sizeof(lv_rcvbdy) )
				return 0;
			memcpy(&lv_rcvbdy, f_bdy, sizeof(lv_rcvbdy));

			try
			{
				if( lv_rcvbdy.GroupID >= 0 && lv_rcvbdy.GroupID < theApp.m_nIVRGroups )
				{
					if( (lv_pGroup = PIVRGroup[lv_rcvbdy.GroupID]) != NULL )
					{
						if( lv_rcvbdy.ChannelID >= lv_pGroup->GetStartCH() && lv_rcvbdy.ChannelID <= lv_pGroup->GetEndCH() )
						{
							lv_pAgent = PIVRCH[lv_rcvbdy.ChannelID];
							if( lv_pAgent )
								lv_pAgent->SetForceNextNode(lv_rcvbdy.NodeID);
						}
					}
				}
			}
			catch(...)
			{
				WriteTrace(TraceError, "Sock process channel flow control msg [%d] goes error on group:[%d]", 
					lv_rcvbdy.ChannelID, lv_rcvbdy.GroupID);
			}
		}
		break;
	//-----------------------------------------------------------------------
	}

	return 1;
}