////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		_Protocol_IVRExt.h
// ��;������	IVR���ⲿ�ӿ�ģ��ͨ��Э��
// ���ߣ�		STEVEN
// ����ʱ�䣺	2005/03/19
// ����޸�ʱ�䣺2005/03/21
// ��Ȩ��Ϣ��	Copyright (C) 1999 - 2005 PCSG
// ��ע��
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_IVREXT_H__
#define __PROTOCOL_IVREXT_H__
//------------------------------------------------------------------------

//--------------------------
// ע�� - 2
//------------
#define TCP_IVR2EXT_REGISTER				1			// (0x0001) ע����Ϣ
struct S_TCP_IVR2EXT_REGISTER
{
	US		ChannelCount;								// ͨ������
};
//------------

//------------
#define TCP_EXT2IVR_REGISTER_ACK			1001		// (0x03E9) ע����Ӧ��Ϣ
struct S_TCP_EXT2IVR_REGISTER_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_BEUSED
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_VERSION
											// PCS_DEF_RETCODE_REGCODE
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------

//--------------------------
// ȡ��ע�� - 2
//------------
#define TCP_IVR2EXT_UNREGISTER				2			// (0x0002) ȡ��ע����Ϣ
//------------

//------------
#define TCP_EXT2IVR_UNREGISTER_ACK			1002		// (0x03EA) ȡ��ע����Ӧ��Ϣ
struct S_TCP_EXT2IVR_UNREGISTER_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_BEUSED
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_VERSION
											// PCS_DEF_RETCODE_REGCODE
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------

//--------------------------
// �Ⲧ������Ϣ - 11
//------------
#define TCP_EXT2IVR_TASK_ADD				0x11		// ���Ӳ�������
struct S_TCP_EXT2IVR_TASK_ADD
{
	UL		PacketID;						// ��ϢID
	US		GroupNO;						// IVR���
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ��������ID
	UC		TelNoLength;					// �绰���볤��
	UC		ServiceCode;					// ������룬'0'-'9', 'a'-'z', 'A'-'Z'��Ч����������¼
	US		NotesLength;					// ��ע����
	US		StartNode;						// ����IVR���̽ڵ�ID�����С��256��Ӹ��ڵ㿪ʼ
};
//------------

//------------
#define TCP_EXT2IVR_TASK_REMOVE				0x12		// ɾ����������
struct S_TCP_EXT2IVR_TASK_REMOVE
{
	UL		PacketID;						// ��ϢID
	US		GroupNO;						// IVR���
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ��������ID
};
//------------

//------------
#define TCP_EXT2IVR_TASK_REMOVE_ALL			0x13		// ɾ����������
struct S_TCP_EXT2IVR_TASK_REMOVE_ALL
{
	UL		PacketID;						// ��ϢID
	US		GroupNO;						// IVR���
};
//------------

//------------
#define TCP_EXT2IVR_TASK_QUERY				0x14		// ��ѯ��������״̬
struct S_TCP_EXT2IVR_TASK_QUERY
{
	UL		PacketID;						// ��ϢID
	US		GroupNO;						// IVR���
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ��������ID
};
//------------

//------------
#define TCP_EXT2IVR_TASK_LIST_QUERY			0x15		// ��ѯ���������б�
struct S_TCP_EXT2IVR_TASK_LIST_QUERY
{
	UL		PacketID;						// ��ϢID
	US		GroupNO;						// IVR���
};
//------------			

//------------
#define TCP_IVR2EXT_TAST_ADD_ACK			0x111		// ���Ӳ�������Ӧ��
struct S_TCP_IVR2EXT_TAST_ADD_ACK
{
	UL		PacketID;						// ��ϢID
	US		GroupNO;						// IVR���
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ��������ID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NOTSUPPORT
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	US		TaskPos;						// �����λ��
};
//------------

//------------
#define TCP_IVR2EXT_TAST_REMOVE_ACK			0x112		// ɾ����������Ӧ��
struct S_TCP_IVR2EXT_TAST_REMOVE_ACK
{
	UL		PacketID;						// ��ϢID
	US		GroupNO;						// IVR���
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ��������ID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NOTSUPPORT
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	US		TaskPos;						// �����λ��
};
//------------

//------------
#define TCP_IVR2EXT_TAST_REMOVE_ALL_ACK		0x113		// ɾ����������Ӧ��
struct S_TCP_IVR2EXT_TAST_REMOVE_ALL_ACK
{
	UL		PacketID;						// ��ϢID
	US		GroupNO;						// IVR���
	US		TaskCount;						// ɾ����������
};
//------------

//------------
#define TCP_IVR2EXT_TASK_QUERY_ACK			0x114		// ��ѯ��������״̬
struct S_TCP_IVR2EXT_TASK_QUERY_ACK
{
	UL		PacketID;						// ��ϢID
	US		GroupNO;						// IVR���
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ��������ID
	UC		TaskStatus;						// ����״̬���μ�_pcC_OutBoundTaskFlag
	US		Duration;						// ��ǰ״̬����ʱ��
};
//------------

//------------
#define TCP_IVR2EXT_TASK_LIST_QUERY_ACK		0x115		// ��ѯ���������б�
struct S_TCP_IVR2EXT_TASK_LIST_QUERY_ACK
{
	UL		PacketID;						// ��ϢID
	US		GroupNO;						// IVR���
	US		TaskCount;						// ��ǰ���������
	UL		FinishTask;						// �����ۼ��������
	UL		SuccTask;						// �����ۼƳɹ�����
	US		AveLine;						// ����ƽ��ͨ��ռ��ʱ��
};
//------------

//------------
#define TCP_IVR2EXT_TASK_RESULT				0x120		// ��������ִ�н��
struct S_TCP_IVR2EXT_TASK_RESULT
{
	UL		PacketID;						// ��ϢID
	US		GroupNO;						// IVR���
	US		CampaignID;						// ��ĿID
	UL		TaskID;							// ��������ID
	UC		TaskResult;						// ����ִ�н�����μ�_pcC_OutBoundCallResult
	UL		CallRefID;						// ���б�ʾ
};
//------------
//--------------------------

//--------------------------
// ͨ�����ݰ��ṹ
/// IVR -> Ext ������ [2000 - 2999]
/// Ext -> IVR ������ [3000 - 3999]
struct S_TCP_IVREXT_PACKGET
{
	UL PacketID;							// ��ϢID
	US ChannelNO;							// ͨ����
	UC Seperator;							// �ָ���
	US VariableLength;						// ��������
	US NotesLength;							// ��ע����
};
//--------------------------

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_IVREXT_H__
//------------------------------------------------------------------------
