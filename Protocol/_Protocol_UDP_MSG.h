////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		_Protocol_UDP_MSG.h
// ��;������	��Ϣͨ��UDPЭ��
// ���ߣ�		SUN
// ����ʱ�䣺	2002/06/05
// ����޸�ʱ�䣺2004/02/28
// ��Ȩ��Ϣ��	Copyright (C) 1998 - 2004 PicaSoft
// ��ע��
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif
//------------------------------------------------------------------------
#ifndef __PROTOCOL_UDP_MSG_H__
#define __PROTOCOL_UDP_MSG_H__
//------------------------------------------------------------------------

//--------------------------
// �������� - 2
//------------
#ifndef MAXHEADLEN
	#define MAXHEADLEN			50			// ������Ϣ��ʾ����(<MAX_PACKET_LEN - 16)
#endif

#ifndef MAXMSGLEN
	#define MAXMSGLEN			200			// ������Ϣ��ʾ����(<MAX_PACKET_LEN - 16)
#endif

//------------------------------------------
// ��Ϣ���� - 4
//-----------------------------------------
#define	PKGTYP_AllNOTE			10			// ȫ��
#define	PKGTYP_AGENTNOTE		1			// ��ϯ
#define	PKGTYP_TEAMNOTE			2			// С��
#define	PKGTYP_GROUPNOTE		3			// ��
//------------
//--------------------------

//--------------------------
// �ṹ�� - 2
//------------
struct SMemberListItem
{
	UL		AgentID;				// Agent ID
	char	UserID[20];				// User ID
	UC		AgentStatus;			// ��ϯ״̬
};

struct SMemberTreeItem
{
	US		GroupID;				// Group ID
	char	GroupName[50];			// Group Name
	US		TeamID;					// Team	ID
	char	TeamName[50];			// Team Name
	char	UserID[20];				// User ID
	char	UserName[50];			// User Name
};
//------------
//--------------------------

//------------------------------------------------------------------------
// ͨ��Э�鶨�� - 2
//------------------------------------------------------------------------
// ��Ϣͷ
struct SUDPMsgHeader
{
	UC Sender;					// ������
	UC Receiver;				// ������
	US PackageLen;				// ����
	US Command;		    		// ������
};

//------------------------------------------------------------------------
#define UDP_ICQ_MSG							100			//����������Ϣ

#define UDP_ACD_INFROMATION_MSG				200			//·�ɵ�״̬���ݱ�����Ϣ
//------------------------------------------------------------------------
// ͨ����Ϣ���ṹ����
struct SUDPPackage
{
    SUDPMsgHeader	MsgHeader;					// ��Ϣ��ͷ
	UL				ResourceID;					// ����Դ���
	char			ResourceName[50];			// ����Դ
	UL				TargetID;					// ����Դ���
	char			TargetName[50];				// ����Դ����
	UC				MessageType;		    	// ��Ϣ����
    char			Title[MAXHEADLEN];		// ����
	char			Content[MAXMSGLEN];		// ����
};

//------------------------------------------------------------------------
// 
struct SACDPackage
{
	SUDPMsgHeader	MsgHeader;					// ��Ϣ��ͷ
	S_TCP_PCS2ALL_ACD_STATUS_REPORT MsgBody;	// ��Ϣ��
	/*
	US		nGroupID;				// ·�ɵ���ID��0��ʾ��������
	/// Call Data
	US		nCallsWait;				// �Ŷӵȴ�����
	US		nCallsAveWaitTime;		// �Ŷ�ƽ���ȴ�ʱ��
	US		nCallsMaxWaitTime;		// �Ŷ���ȴ�ʱ��
	UL		nCallsTotal;			// ������������(�����ۼ�)
	UL		nCallsTransfer;			// ����ת����(�����ۼ�)
	UL		nCallsSucc;				// ����ת�ӳɹ���(�����ۼ�)
	UL		nCallsSuccInTime;		// ���м�ʱת����(�����ۼ�)
	/// Agent Data
	US		nAgentSum;				// ��ϯ����
	US		nAgentIdle;				// ������ϯ��
	US		nAgentBusy;				// ��æ��ϯ��
	US		nAgentNotReady;			// δ������ϯ��
	US		nAgentAfterCall;		// ������ϯ��
	US		nAgentLogout;			// δ��¼��ϯ��
	*/
};

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_MSGR_H__
//------------------------------------------------------------------------
