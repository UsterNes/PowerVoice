////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����			_Protocol_MSGAGT.h
// ��;������		��Ϣ�ͻ���ͨ��Э��
// ���ߣ�			Jerry, Tony
// ����ʱ�䣺		2004/11/23
// ����޸�ʱ�䣺	2010/02/08
// ��Ȩ��Ϣ��		Copyright (C) 1999 - 2010 PCSG & PicaSoft
// ��ע��
//   2010-02-08 [Tony]: ���ӡ��������ɿ���Э�顱
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_MSGAGENT_H__
#define __PROTOCOL_MSGAGENT_H__
//------------------------------------------------------------------------

//--------------------------
// �������� - 2
//------------
#ifndef MAXHEADLEN
	#define MAXHEADLEN			50			// ��Ϣ������󳤶�
#endif

#ifndef MAXMSGLEN
	#define MAXMSGLEN			200			// ��Ϣ������󳤶�
#endif

//------------------------------------------
// �������� - 4  - MAX
//-----------------------------------------
#define	MSG_MAX_USER					16384			// ����û���
#define	MSG_MAX_TEAM					512				// ���С����
#define	MSG_MAX_GROUP					256				// ��������
#define	MSG_MAX_TEAMGROUP				4096			// ����С�������
//------------
//--------------------------

//------------------------------------------
// Ȩ����ʼλ�� -- Ȩ����λ��  -- 2
//-----------------------------------------
#define	RIGHTSTART			20			// 
#define	RIGHTCOUNT			6			// 

// Agent ���(16 - 30)
#define MSG_USE_RECEIVECHATMSG					1				// ����������Ϣ
#define MSG_USE_SENDTOLED						2				// ������Ϣ��LED
#define MSG_USE_SENDALLMESSAGE					3				// ������Ϣ��ȫ����ϯ
#define MSG_USE_SENDGROUPMESSAGE				4				// ������Ϣ����ϯ��
#define MSG_USE_SENDTEAMMESSAGE					5				// ������Ϣ����ϯС��
#define MSG_USE_SENDSINGLEMESSAGE				6				// ������Ϣ��������ϯ
//------------
//--------------------------

//------------------------------------------
// ��Ϣ����/�������� - 4  - TargetType
//-----------------------------------------
#define	MSG_TARGET_ALL					10			// ȫ����ϯ
#define	MSG_TARGET_GROUP				3			// ��ϯ����
#define	MSG_TARGET_TEAM					2			// ��ϯС��
#define	MSG_TARGET_USER					1			// ������ϯ
#define	MSG_TARGET_LED					4			// LED
//------------
//--------------------------

//------------------------------------------
// ��Ϣ���� - 5 -- MessageLevel
//-----------------------------------------
#define	MSG_LEVEL_1			1			// �������
#define	MSG_LEVEL_2			2			// 
#define	MSG_LEVEL_3			3			// 
#define	MSG_LEVEL_4			4			// 
#define	MSG_LEVEL_5			5			// �������
//�ο���Ϣ��һ��֪ͨ����Ҫ֪ͨ������֪ͨ
//��Ҫ��-�ߡ��ϸߡ�һ�㡢�ϵ͡���
//------------
//--------------------------

//------------------------------------------
// ��Ϣ���� - 3 -- MessageType
//-----------------------------------------
#define	MSG_TYPE_TEXT			1			// ������Ϣ
#define	MSG_TYPE_ACD			2			// ACD������Ϣ
#define	MSG_TYPE_SQL			3			// SQL���
//------------
//--------------------------

//--------------------------
// ��Ϣ�ͻ���ע�� - Total 4
//--------------------------

//--------------------------
// ע�� - 2
//------------
#define TCP_AGT2MSG_REGISTER				1			// (0x0001) ע����Ϣ
struct S_TCP_AGT2MSG_REGISTER
{
	char	strUserName[20];				// �û���
	char	strPassword[20];				// �û�����
	UC		Version;						// �汾��
	UC		RegCode[PCS_DEF_NUMLEN_APP];	// ע����
};
//------------

//------------
#define TCP_MSG2AGT_REGISTER_ACK			1001		// (0x03E9) ע����Ӧ��Ϣ
struct S_TCP_MSG2AGT_REGISTER_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
	char	strDisplayName[50];				// �û���ʾ����
	char	strRights[RIGHTCOUNT];			// Ȩ��
	US		TeamID;							// ���û�������С��
	UC		InGroupCount;					// ���û����ڶ��ٸ���
	US		GroupList[64];					// ���б�
};
//------------
//--------------------------

//--------------------------
// ȡ��ע�� - 2
//------------
#define TCP_AGT2MSG_UNREGISTER				2			// (0x0002) ȡ��ע����Ϣ
//------------

//------------
#define TCP_MSG2AGT_UNREGISTER_ACK			1002		// (0x03EA) ȡ��ע����Ӧ��Ϣ
//------------
//--------------------------

//--------------------------
// ״̬�ı���Ϣ - 1
//------------
#define TCP_MSG2AGT_STATUS_CHANGE		1018		// (0x03FA) λ��״̬�ı���Ϣ
struct S_TCP_MSG2AGT_STATUS_CHANGE
{
	char						UserID[20];					// ��ϯID
	_pcC_stMsgClientStatus		Status;					// λ��״̬������_pcC_stPOS
};
//------------

//--------------------------
// ������Ϣ - 3
//------------
#define TCP_AGT2MSG_SENDMESSAGE				3001		// (0x0BB9) ����������Ϣ
// ͨ����Ϣ���ṹ����
struct S_TCP_AGT2MSG_SENDMESSAGE
{
	UC				MessageType;		    	// ��Ϣ����
	UC				MessageLevel;				// ��Ϣ����
	UL				ResourceID;					// ����Դ���
	char			ResourceName[50];			// ����Դ
	UC				TargetType;					// ���շ�Χ
	UL				TargetID;					// ����Դ���
	char			TargetName[50];				// ����Դ����
	char			SendTime[15];				// ��Ϣ����ʱ��
    char			Title[MAXHEADLEN];			// ����
	char			Content[MAXMSGLEN];			// ����
};

#define TCP_MSG2AGT_SENDMESSAGE				3002		// (0x0BBA) ����������Ϣ

#define TCP_MSG2AGT_SENDACD					3003		// (0x0BBB) ����ACD��Ϣ
//------------
//--------------------------

//--------------------------
// ״̬��ѯ - 2
//------------
#define TCP_AGT2MSG_STATUS_QUERY			11			// (0x000B) ��ѯ״̬��Ϣ
//------------

//------------
#define TCP_MSG2AGT_STATUS_QUERY_ACK		1011		// (0x03F3) ��ѯ״̬��Ӧ��Ϣ
struct S_TCP_MSG2AGT_STATUS_QUERY_ACK
{
	UC		ErrorCode;					// �����룬�μ��������붨�塱��
	UC		Status;							// ״̬ Ready Or Not Ready
	UC		LeaveReason;					// ��ϯԭ���Զ���
};
//------------
//--------------------------

//--------------------------
// �޸�״̬ - 2
//------------
#define TCP_AGT2MSG_STATUS_CHANGE			14			// (0x000E) �޸�״̬��Ϣ
struct S_TCP_AGT2MSG_STATUS_CHANGE
{
	UC		Status;							// ״̬ Ready Or Not Ready
	UC		LeaveReason;					// ��ϯԭ���Զ���
};
//------------

//------------
#define TCP_MSG2AGT_STATUS_CHANGE_ACK		1014		// (0x03F6) �޸�״̬��Ӧ��Ϣ
struct S_TCP_MSG2AGT_STATUS_CHANGE_ACK
{
	UC		ErrorCode;					// �����룬�μ��������붨�塱��
	UC		Status;							// ״̬ Ready Or Not Ready
	UC		LeaveReason;					// ��ϯԭ���Զ���
};
//------------
//--------------------------

//--------------------------
// ��ȡ��Ա�� - 6
//------------
#define TCP_AGT2MSG_TREEVIEW_QUERY			500				// (0x01F6) ��ѯ����Ա��Ϣ�б���Ӧ��������ϢΪ��Ա��
//------------
#define TCP_MSG2AGT_TREEVIEW_DATA			1500			// (0x0466) ��Ա����Ϣ
struct S_TCP_MSG2AGT_TREEVIEW_DATA
{
	US		TotalItemCount;									// ��Ա��Ŀ
	US		ItemIndex;										// ��Ա���
	UC		ItemCount;										// ������Ϣ�����ĳ�Ա����
	UC		*pSubTree;										// ָ���Ա����
};
//------------
#define TCP_MSG2AGT_TREEVIEW_GROUP_DATA			1501		// (0x0466) ��Ա��-GROUP��Ϣ

#define TCP_MSG2AGT_TREEVIEW_TEAMGROUP_DATA		1502		// (0x0466) ��Ա��-TEAMGROUp��Ϣ

#define TCP_MSG2AGT_TREEVIEW_TEAM_DATA			1503		// (0x0466) ��Ա��-TEAM��Ϣ

#define TCP_MSG2AGT_TREEVIEW_USER_DATA			1504		// (0x0466) ��Ա��-USER��Ϣ
//------------
//--------------------------

//--------------------------
// ��ȡȫ��״̬ - 2
//------------
#define TCP_AGT2MSG_USERSTATUS_QUERY			1505		// (0x0466) ��ѯȫ����ϯ״̬��Ϣ
#define TCP_MSG2AGT_USERSTATUS_DATA				1506		// (0x0466) ��Ա״̬��Ϣ
struct S_TCP_MSG2AGT_USERSTATUS_DATA
{
	US		TotalItemCount;									// ��Ա��Ŀ
	US		ItemIndex;										// ��Ա���
	UC		ItemCount;										// ������Ϣ�����ĳ�Ա����
	UC		*pSubTree;										// ָ���Ա����
};
//------------
//--------------------------

//--------------------------
// �ṹ�� - 1 - For User Status
//------------
struct SUserStatusItem
{
	char					UserID[20];				// User ID
	_pcC_stMsgClientStatus	Status;					
};  
//------------
//--------------------------

//--------------------------
// �ṹ�� - 4 - For Tree Item
//------------
struct SGroupMemberTreeItem
{
	US		GroupID;						// Group ID
	char	GroupName[50];					// Group Name
};

struct STeamGroupMemberTreeItem
{
	US		GroupID;						// Group ID
	US		TeamID;							// Team ID
};

struct STeamMemberTreeItem
{
	US		TeamID;							// Team ID
	char	TeamName[50];					// Team Name	
};

struct SUserMemberTreeItem
{					
	char					UserID[20];		// User ID
	char					UserName[50];	// User Name
	US						UserType;				
	US						TeamID;		
	_pcC_stMsgClientStatus	Status;
};
//------------
//--------------------------

//--------------------------
// Sun added 2010-02-08
// �������ɿ���Э�� - 4
//------------
#define TCP_AGT2MSG_CHANGE_STATUS_REQ		3021		// (0x0BCD) �޸�״̬���������Ϣ
struct S_TCP_AGT2MSG_CHANGE_STATUS_REQ
{
	UL		InvokeID;						// Invoke ID
	UC		PositionStatus;					// λ��״̬������_pcC_stPOS
	UC		LeaveReason;					// ��ϯԭ���Զ���
	char	Token[MAXHEADLEN];				// Token
	UL		Reserved1;						// ����1
	UL		Reserved2;						// ����2
};

#define TCP_MSG2AGT_CHANGE_STATUS_ACK		3022		// (0x0BCE) �޸�״̬�����Ӧ��Ϣ
struct S_TCP_MSG2AGT_CHANGE_STATUS_ACK
{
	UL		InvokeID;						// Invoke ID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_LACK_PARAM
											// PCS_DEF_RETCODE_REJECTED
											// PCS_DEF_RETCODE_NOTSUPPORT
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

#define TCP_AGT2MSG_CHANGE_STATUS_ADMREQ	3023		// (0x0BCF) ��೤�����޸�״̬
struct S_TCP_AGT2MSG_CHANGE_STATUS_ADMREQ
{
	UL		InvokeID;						// Invoke ID
	UL		MonitorID;						// �����ID�������ָ�����򷢸��������߹���Ա
	UC		PositionStatus;					// λ��״̬������_pcC_stPOS
	UC		LeaveReason;					// ��ϯԭ���Զ���
	char	ShortMessage[MAXHEADLEN];		// ������������Ϣ������ԭ��
	UL		Reserved1;						// ����1
	UL		Reserved2;						// ����2
};

#define TCP_MSG2AGT_CHANGE_STATUS_ADMACK		3024		// (0x0BD0) �����޸�״̬�೤��Ӧ
struct S_TCP_MSG2AGT_CHANGE_STATUS_ADMACK
{
	UL		InvokeID;						// Invoke ID
	UL		MonitorID;						// �����ID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_LACK_PARAM
											// PCS_DEF_RETCODE_REJECTED
											// PCS_DEF_RETCODE_NOTSUPPORT
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	char	ShortMessage[MAXHEADLEN];		// ������������Ϣ�����˵����
	char	Token[MAXHEADLEN];				// Token
	UL		Reserved1;						// ����1
	UL		Reserved2;						// ����2
};
//------------
// Henan added 2010-02-25
// �������ɿ���Э�� - 1
#define TCP_AGT2MSG_CHANGE_CAUSA_ASTRICT	3025		// (0x0BD1) ��MSG֪ͨ��������ֵ�ı�
//------------
//--------------------------


//--------------------------
// Sun added 2010-10-22
// С��������Ϣ����
#define TCP_NRAM_REQUEST					1			// (0x01)��ϯС������
#define TCP_NRAM_RESPONSE_REFUSED			10			// (0x0A)�೤�ܾ�
#define TCP_NRAM_RESPONSE_APPROVED			11			// (0x0B)�೤����
#define TCP_NRAM_NTF_DISABLED				30			// (0x1E)С�����뿪�ر仯֪ͨ���ر�
#define TCP_NRAM_NTF_ENABLED				31			// (0x1F)С�����뿪�ر仯֪ͨ������

//--------------------------


//--------------------------
//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_RECORDAGENT_H__
//------------------------------------------------------------------------
