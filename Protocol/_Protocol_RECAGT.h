////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		_Protocol_RECAGT.h
// ��;������	¼���ͻ���ͨ��Э��
// ���ߣ�		SUN
// ����ʱ�䣺	1999/12/01
// ����޸�ʱ�䣺2006/08/08
// ��Ȩ��Ϣ��	Copyright (C) 1999 - 2006 PCSG
// ��ע��
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_RECORDAGENT_H__
#define __PROTOCOL_RECORDAGENT_H__
//------------------------------------------------------------------------

#ifndef MAX_WAVE_COMPRESS_RATE
#define MAX_WAVE_COMPRESS_RATE		16
#endif

#ifndef SIZE_PER_AUDIO_PACKAGE
#define SIZE_PER_AUDIO_PACKAGE		160
#endif

#ifndef SIZE_AUDIO_FRAME
#define SIZE_AUDIO_FRAME			3200
#endif

#ifndef SIZE_AUDIO_PACKED
#define SIZE_AUDIO_PACKED			200
#endif

// Updated 2006-08-08, from 800 to 8192
#ifndef SIZE_FILE_PACKED
//#define SIZE_FILE_PACKED			800
#define SIZE_FILE_PACKED			800
#endif

// Add 2006-08-08
#ifndef SIZE_MAX_AUDIO_PACKAGE
#define SIZE_MAX_AUDIO_PACKAGE		800
#endif

//--------------------------
// ¼���ͻ���ע�� - Total 4
//--------------------------

//--------------------------
// ע�� - 2
//------------
#define TCP_AGT2REC_REGISTER				1			// (0x0001) ע����Ϣ
struct S_TCP_AGT2REC_REGISTER
{
	char	UserName[20];					// �û���
	char	strPassword[20];				// �û�����
	char	strAliasName[50];				// ��¼����
	UL		AgentID;						// PowerCall��ϯ��ţ�CTI����ʱ�����ṩ
	UC		Version;						// �汾��
	UC		RegCode[PCS_DEF_NUMLEN_APP];	// ע����
};
//------------

//------------
#define TCP_REC2AGT_REGISTER_ACK			1001		// (0x03E9) ע����Ӧ��Ϣ
struct S_TCP_REC2AGT_REGISTER_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_BEUSED
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_VERSION
											// PCS_DEF_RETCODE_REGCODE
											// PCS_DEF_RETCODE_PASSWORD
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_NOLICENSE
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UC		bCTIEnabled;					// �Ƿ�ʹ��CTI����
	UC		nMaxSilenceTime;				// ���Ĭʱ��
	UC		nCTIMaxSilenceTime;				// CTI���Ĭʱ��
	char	strDisplayName[50];				// �û���ʾ����
	char	strRights[50];					// Ȩ��
	char	strFieldName1[50];				// ���������ֶ���1
	char	strFieldName2[50];				// ���������ֶ���2
	char	strFieldName3[50];				// ���������ֶ���3
	char	strFieldName4[50];				// ���������ֶ���4
	char	strFieldName5[50];				// ���������ֶ���5
};
//------------
//--------------------------

//--------------------------
// ȡ��ע�� - 2
//------------
#define TCP_AGT2REC_UNREGISTER				2			// (0x0002) ȡ��ע����Ϣ
//------------

//------------
#define TCP_REC2AGT_UNREGISTER_ACK			1002		// (0x03EA) ȡ��ע����Ӧ��Ϣ
//------------
//--------------------------


//--------------------------
// ¼���ͻ������趨 - Total 30
//--------------------------
//------------
#define TCP_AGT2REC_CHG_PASSWORD			11			// (0x000B) �ͻ����޸Ŀ���
struct S_TCP_AGT2REC_CHG_PASSWORD
{
	char	strPassword[20];				// �¿���
};

//------------
#define TCP_REC2AGT_CHG_PASSWORD_ACK		1011		// (0x03F3) �޸Ŀ�����Ӧ��Ϣ
struct S_TCP_REC2AGT_CHG_PASSWORD_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_CHG_PARAMETER			12			// (0x000C) �ͻ����޸ĸ��˲���
struct S_TCP_AGT2REC_CHG_PARAMETER
{
	char	strParamName[50];				// ������
	char	strParamValue[100];				// ����ֵ
};
//------------

//------------
#define TCP_REC2AGT_CHG_PARAMETER_ACK		1012		// (0x03F4) �ͻ����޸ĸ��˲�����Ӧ��Ϣ
struct S_TCP_REC2AGT_CHG_PARAMETER_ACK
{
	char	strParamName[50];				// ������
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_CHG_SYSPARAM			13			// (0x000D) ����Ա���޸�ϵͳ����(�����ǹ���Ա�ͻ���)
struct S_TCP_AGT2REC_CHG_SYSPARAM
{
	char	strParamName[50];				// ������
	char	strParamValue[100];				// ����ֵ
};
//------------

//------------
#define TCP_REC2AGT_CHG_SYSPARAM_ACK		1013		// (0x03F5) ����Ա���޸�ϵͳ������Ӧ��Ϣ
struct S_TCP_REC2AGT_CHG_SYSPARAM_ACK
{
	char	strParamName[50];				// ������
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_ADD_AGENT				14			// (0x000E) ������ϯ��Ϣ(�����ǹ���Ա�ͻ���)
struct S_TCP_AGT2REC_ADD_AGENT
{
	char	UserName[20];					// �û���
};
//------------

//------------
#define TCP_REC2AGT_ADD_AGENT_ACK			1014		// (0x03F6) ������ϯ��Ӧ��Ϣ
struct S_TCP_REC2AGT_ADD_AGENT_ACK
{
	char	UserName[20];					// �û���
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_BEUSED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_DEL_AGENT				15			// (0x000F) ɾ����ϯ��Ϣ(�����ǹ���Ա�ͻ���)
struct S_TCP_AGT2REC_DEL_AGENT
{
	char	UserName[20];					// �û���
};
//------------

//------------
#define TCP_REC2AGT_DEL_AGENT_ACK			1015		// (0x03F7) ɾ����ϯ��Ӧ��Ϣ
struct S_TCP_REC2AGT_DEL_AGENT_ACK
{
	char	UserName[20];					// �û���
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_MOD_AGENT				16			// (0x0010) �޸���ϯ��Ϣ(�����ǹ���Ա�ͻ���)
struct S_TCP_AGT2REC_MOD_AGENT
{
	char	UserName[20];					// �û���
};
//------------

//------------
#define TCP_REC2AGT_MOD_AGENT_ACK			1016		// (0x03F8) �޸���ϯ��Ӧ��Ϣ
struct S_TCP_REC2AGT_MOD_AGENT_ACK
{
	char	UserName[20];					// �û���
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_RELOAD_AGENTLIST		17			// (0x0011) ���¼�����ϯ��(�����ǹ���Ա�ͻ���)
//------------

//------------
#define TCP_REC2AGT_RELOAD_AGENTLIST_ACK	1017		// (0x03F9) ���¼�����ϯ����Ӧ��Ϣ
struct S_TCP_REC2AGT_RELOAD_AGENT_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_RELOAD_SYSPARAM			18			// (0x0018) ���¼���ϵͳ������(�����ǹ���Ա�ͻ���)
//------------

//------------
#define TCP_REC2AGT_RELOAD_SYSPARAM_ACK		1018		// (0x03FA) ���¼���ϵͳ��������Ӧ��Ϣ
struct S_TCP_REC2AGT_RELOAD_SYSPARAM_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_UPDATE_GROUPINFO		19			// (0x0019) ��������Ϣ(�����ǹ���Ա�ͻ���)
struct S_TCP_AGT2REC_UPDATE_GROUPINFO
{
	US		GroupNo;						// ����
};
//------------

//------------
#define TCP_REC2AGT_UPDATE_GROUPINFO_ACK	1019		// (0x03FB) ��������Ϣ��Ӧ��Ϣ
struct S_TCP_REC2AGT_UPDATE_GROUPINFO_ACK
{
	US		GroupNo;						// ����
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_ADD_ACCESSPOINT			20			// (0x0014) ���ӽ������Ϣ(�����ǹ���Ա�ͻ���)
struct S_TCP_AGT2REC_ADD_ACCESSPOINT
{
	UC		AccessPointID;					// �����ID, 0 Based
};
//------------

//------------
#define TCP_REC2AGT_ADD_ACCESSPOINT_ACK		1020		// (0x03FC) ���ӽ������Ӧ��Ϣ
struct S_TCP_REC2AGT_ADD_ACCESSPOINT_ACK
{
	UC		AccessPointID;					// �����ID, 0 Based
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_LACK_PARAM
											// PCS_DEF_RETCODE_REJECTED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_DEL_ACCESSPOINT			21			// (0x0015) ɾ���������Ϣ(�����ǹ���Ա�ͻ���)
struct S_TCP_AGT2REC_DEL_ACCESSPOINT
{
	UC		AccessPointID;					// �����ID, 0 Based
};
//------------

//------------
#define TCP_REC2AGT_DEL_ACCESSPOINT_ACK		1021		// (0x03FD) ɾ���������Ӧ��Ϣ
struct S_TCP_REC2AGT_DEL_ACCESSPOINT_ACK
{
	UC		AccessPointID;					// �����ID, 0 Based
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_LACK_PARAM
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_MOD_ACCESSPOINT			22			// (0x0016) �޸Ľ������Ϣ(�����ǹ���Ա�ͻ���)
struct S_TCP_AGT2REC_MOD_ACCESSPOINT
{
	UC		AccessPointID;					// �����ID, 0 Based
};
//------------

//------------
#define TCP_REC2AGT_MOD_ACCESSPOINT_ACK		1022		// (0x03FE) �޸Ľ������Ӧ��Ϣ
struct S_TCP_REC2AGT_MOD_ACCESSPOINT_ACK
{
	UC		AccessPointID;					// �����ID, 0 Based
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_LACK_PARAM
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_RELOAD_APTPARAMLIST		30			// (0x001E) ���¼��ؽ���������(�����ǹ���Ա�ͻ���)
struct S_TCP_AGT2REC_RELOAD_APTPARAMLIST
{
	UC		AccessPointID;					// �����ID, 0 Based
};
//------------

//------------
#define TCP_REC2AGT_RELOAD_APTPARAMLIST_ACK 1030		// (0x0406) ���¼��ؽ�����������Ӧ��Ϣ
struct S_TCP_REC2AGT_RELOAD_APTPARAMLIST_ACK
{
	UC		AccessPointID;					// �����ID, 0 Based
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_CHG_APTPARAM			31			// (0x001F) �޸Ľ�������(�����ǹ���Ա�ͻ���)
struct S_TCP_AGT2REC_CHG_APTPARAM
{
	UC		AccessPointID;					// �����ID, 0 Based
	char	strParamName[50];				// ������
	char	strParamValue[100];				// ����ֵ
};
//------------

//------------
#define TCP_REC2AGT_CHG_APTPARAM_ACK		1031		// (0x0407) �޸Ľ���������Ӧ��Ϣ
struct S_TCP_REC2AGT_CHG_APTPARAM_ACK
{
	UC		AccessPointID;					// �����ID, 0 Based
	char	strParamName[50];				// ������
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_LACK_PARAM
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_CHG_APTCHPARAM			32			// (0x0020) �޸Ľ����ͨ������(�����ǹ���Ա�ͻ���)
struct S_TCP_AGT2REC_CHG_APTCHPARAM
{
	UC		AccessPointID;					// �����ID, 0 Based
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	char	strParamName[50];				// ������
	char	strParamValue[100];				// ����ֵ
};
//------------

//------------
#define TCP_REC2AGT_CHG_APTCHPARAM_ACK		1032		// (0x0408) �޸Ľ����ͨ��������Ӧ��Ϣ
struct S_TCP_REC2AGT_CHG_APTCHPARAM_ACK
{
	UC		AccessPointID;					// �����ID, 0 Based
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	char	strParamName[50];				// ������
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_LACK_PARAM
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_RELOAD_APTCHPARAMLIST		33			// (0x0021) ���¼��ؽ����ͨ��������(�����ǹ���Ա�ͻ���)
struct S_TCP_AGT2REC_RELOAD_APTCHPARAMLIST
{
	UC		AccessPointID;					// �����ID, 0 Based
};
//------------

//------------
#define TCP_REC2AGT_RELOAD_APTCHPARAMLIST_ACK 1033		// (0x0409) ���¼��ؽ����ͨ����������Ӧ��Ϣ
struct S_TCP_REC2AGT_RELOAD_APTCHPARAMLIST_ACK
{
	UC		AccessPointID;					// �����ID, 0 Based
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_AGT2REC_RELOAD_APLIST		34			// (0x0022) ���¼��ؽ����(�����ǹ���Ա�ͻ���)
/*struct S_TCP_AGT2REC_RELOAD_APIST
{
};*/
//------------

//------------
#define TCP_REC2AGT_RELOAD_APLIST_ACK 1034		// (0x0410) ���¼��ؽ������Ӧ��Ϣ
struct S_TCP_REC2AGT_RELOAD_APLIST_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------


//--------------------------
// ¼��������Ϣ - Total 18
//--------------------------
//------------
#define TCP_AGT2REC_STARTREC				51			// (0x0033) �ͻ��˿�ʼ¼����Ϣ
struct S_TCP_AGT2REC_STARTREC
{
	UC		RecordStartType;				// ¼���������Ʒ�ʽ���μ���¼�����Ʒ�ʽ��
	UC		SamplesPerSec;					// ����Ƶ��(����������)
	UC		BitsPerSample;					// ��������(8 or 16)
	UC		Channels;						// ͨ����(1 or 2)
};
//------------

//------------
#define TCP_AGT2REC_STARTREC_MON			52			// (0x0034) �೤ǿ�ƿ�ʼ¼����Ϣ(�����ǹ���Ա�ͻ���)
struct S_TCP_AGT2REC_STARTREC_MON
{
	char	UserName[20];					// �û���
	char	UserData[200];					// �û�����
};
//------------

//------------
#define TCP_REC2AGT_STARTREC_ACK			1051		// (0x041B) �ͻ��˿�ʼ¼����Ӧ��Ϣ
struct S_TCP_REC2AGT_STARTREC_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_REC2AGT_STARTREC_CMD			1052		// (0x041C) ����������ͻ��˿�ʼ¼��(CTI���ơ��೤����)
struct S_TCP_REC2AGT_STARTREC_CMD
{
	UC		RecordStartType;				// ¼���������Ʒ�ʽ���μ���¼�����Ʒ�ʽ��
	UC		CallDirection;					// ͨ������CTI����ʱʹ�ã��μ����������͡�
	UL		AgentID;						// PowerCall��ϯ��ţ�CTI����ʱʹ��
	UL		CallRefID;						// ���б�ʾ��CTI����ʱʹ��
	char	TelNo[PCS_DEF_NUMLEN_APP];		// �绰���룬CTI����ʱʹ��
	char	CallInTime[15];					// CTI�ṩ�ĺ���ʱ�䣨TimeStamp����CTI����ʱʹ��
	char	MonitorName[20];				// �೤�����೤����ʱʹ��
	char	UserData[200];					// �û�����
};
//------------

//------------
#define TCP_REC2AGT_STARTREC_MON_ACK		1053			// (0x041D) �೤ǿ�ƿ�ʼ¼����Ӧ��Ϣ
struct S_TCP_REC2AGT_STARTREC_MON_ACK
{
	char	UserName[20];					// �û���
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------


//------------
#define TCP_AGT2REC_STOPREC					61			// (0x003D) �ͻ���ֹͣ¼����Ϣ
struct S_TCP_AGT2REC_STOPREC
{
	UC		RecordStopType;					// ¼��ֹͣ���Ʒ�ʽ���μ���¼�����Ʒ�ʽ��
	char	UserData[200];					// �û�����
};
//------------

//------------
#define TCP_AGT2REC_STOPREC_MON				62			// (0x003E) �೤ǿ��ֹͣ¼����Ϣ(�����ǹ���Ա�ͻ���)
struct S_TCP_AGT2REC_STOPREC_MON
{
	char	UserName[20];					// �û���
};
//------------

//------------
#define TCP_AGT2REC_APPEND_USERDATA			63			// (0x003F) ׷���û����ݵ����һ�γɹ�¼��
struct S_TCP_AGT2REC_APPEND_USERDATA
{
	char	UserData[200];					// �û�����
};
//------------

//------------
#define TCP_AGT2REC_ATTACH_RECORDDATA		64			// (0x0040) ¼������������Ϣ
struct S_TCP_AGT2REC_ATTACH_RECORDDATA
{
	char	AttachData1[16];				// ����������1
	char	AttachData2[16];				// ����������2
	char	AttachData3[32];				// ����������3
	char	AttachData4[32];				// ����������4
	char	AttachData5[256];				// ����������5
};
//------------

//------------
#define TCP_AGT2REC_SET_SPECIALMARK			65			// (0x0041) ¼���ر�����Ϣ
struct S_TCP_AGT2REC_SET_SPECIALMARK
{
	UC		SpecialMark;					// �ر���
};
//------------

//------------
#define TCP_AGT2REC_VOICEDEVICE_STATUS		66			// (0x0042) �����豸״̬������Ϣ
struct S_TCP_AGT2REC_VOICEDEVICE_STATUS
{
	UC		VoiceDeviceStatus;				// �����豸״̬���μ����س�������
};
//------------

//------------
#define TCP_REC2AGT_STOPREC_ACK				1061		// (0x0425) �ͻ���ֹͣ¼����Ӧ��Ϣ
struct S_TCP_REC2AGT_STOPREC_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		RecordDuration;					// ¼��ʱ��
	char	FilePath[128];					// ¼���ļ�ȫ·��
};
//------------

//------------
#define TCP_REC2AGT_STOPREC_CMD				1062		// (0x0426) ����������ͻ���ֹͣ¼��(CTI���ơ��೤����)
struct S_TCP_REC2AGT_STOPREC_CMD
{
	UC		RecordStopType;					// ¼��ֹͣ���Ʒ�ʽ���μ���¼�����Ʒ�ʽ��
	char	MonitorName[20];				// �೤�����೤����ʱʹ��
};
//------------

//------------
#define TCP_REC2AGT_STOPREC_MON_ACK			1063			// (0x0427) �೤ǿ��ֹͣ¼����Ӧ��Ϣ
struct S_TCP_REC2AGT_STOPREC_MON_ACK
{
	char	UserName[20];					// �û���
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_REC2AGT_USER_ATTACHDATA			1064			// (0x0428) �û�����¼��������Ϣ
struct S_TCP_REC2AGT_USER_ATTACHDATA
{
	char	UserName[20];					// �������û���
	char	AttachData1[16];				// ����������1
	char	AttachData2[16];				// ����������2
	char	AttachData3[32];				// ����������3
	char	AttachData4[32];				// ����������4
	char	AttachData5[256];				// ����������5
};
//------------

//------------
#define TCP_REC2AGT_SET_SPECIALMARK			1065			// (0x0428) �û������ر�����Ϣ
struct S_TCP_REC2AGT_SET_SPECIALMARK
{
	char	UserName[20];					// �������û���
	UC		SpecialMark;					// �ر���
};
//------------

//------------
#define TCP_REC2AGT_USER_CTICALLINFO		1066			// (0x0429) ������û�CTI����������Ϣ
struct S_TCP_REC2AGT_USER_CTICALLINFO
{
	char	UserName[20];					// �������û���
	UC		CallDirection;					// ͨ�����򣬲μ����������͡�
	UL		AgentID;						// PowerCall��ϯ���
	UL		CallRefID;						// ���б�ʾ
	char	TelNo[PCS_DEF_NUMLEN_APP];		// �绰����
};
//------------

//------------
#define TCP_REC2AGT_APTCH_CTICALLINFO		1067			// (0x042A) �����ͨ��CTI����������Ϣ
struct S_TCP_REC2AGT_APTCH_CTICALLINFO
{
	UC		AccessPointID;					// �����ID, 0 Based
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	UC		CallDirection;					// ͨ�����򣬲μ����������͡�
	UL		AgentID;						// PowerCall��ϯ���
	UL		CallRefID;						// ���б�ʾ
	char	CallInTime[15];					// �绰����/����ʱ��
	char	TelNo[PCS_DEF_NUMLEN_APP];		// �绰����
};
//------------
//--------------------------


//--------------------------
// ��ء�����������Ϣ - Total 10
//--------------------------
//------------
#define TCP_AGT2REC_ADDMONITOR				71			// (0x0047) ����Ա��Ӽ�ض���(�����ǹ���Ա�ͻ���)
struct S_TCP_AGT2REC_ADDMONITOR
{
	char	UserName[20];					// ������û���
};
//------------

//------------
#define TCP_AGT2REC_REMOVEMONITOR			72			// (0x0048) ����Ա���ټ�ض���(�����ǹ���Ա�ͻ���)
struct S_TCP_AGT2REC_REMOVEMONITOR
{
	char	UserName[20];					// �û���
};
//------------

//------------
#define TCP_AGT2REC_START_LSN				73			// (0x0049) ����Ա��ʼ����(�����ǹ���Ա�ͻ���)
struct S_TCP_AGT2REC_START_LSN
{
	char	UserName[20];					// �������û���
};
//------------

//------------
#define TCP_AGT2REC_STOP_LSN				74			// (0x004A) ����Աֹͣ����(�����ǹ���Ա�ͻ���)
struct S_TCP_AGT2REC_STOP_LSN
{
	char	UserName[20];					// �û���
};
//------------

//------------
#define TCP_REC2AGT_ADDMONITOR_ACK			1071		// (0x042F) ����Ա���Ӽ����Ӧ��Ϣ
struct S_TCP_REC2AGT_ADDMONITOR_ACK
{
	char	UserName[20];					// ������û���
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UC		LoginStatus;					// ������û���¼״̬
	UC		RecordStatus;					// ������û�¼��״̬
	UC		ListenStatus;					// ������û���ǰ�ܿ�״̬
	UC		MonitorStatus;					// ������û��İ೤״̬
	UC		VoiceDeviceStatus;				// ������û�¼���豸�Ƿ�����
};
//------------

//------------
#define TCP_REC2AGT_REMOVEMONITOR_ACK		1072		// (0x0430) ����Ա���ټ����Ӧ��Ϣ
struct S_TCP_REC2AGT_REMOVEMONITOR_ACK
{
	char	UserName[20];					// ������û���
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_REC2AGT_START_LSN_ACK			1073		// (0x0431) ��������Ӧ����Ϣ
struct S_TCP_REC2AGT_START_LSN_ACK
{
	char	UserName[20];					// �������û���
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_REC2AGT_STOP_LSN_ACK			1074		// (0x0432) ֹͣ����Ӧ����Ϣ
struct S_TCP_REC2AGT_STOP_LSN_ACK
{
	char	UserName[20];					// �û���
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_REC2AGT_LSN_STARTED				1075		// (0x0433) ������ʼ��Ϣ
struct S_TCP_REC2AGT_LSN_STARTED
{
	char	UserName[20];					// �������û���
	UC		RecordStartType;				// ¼���������Ʒ�ʽ���μ���¼�����Ʒ�ʽ��
	UC		SamplesPerSec;					// ����Ƶ��(����������)
	UC		BitsPerSample;					// ��������(8 or 16)
	UC		Channels;						// ͨ����(1 or 2)
};
//------------

//------------
#define TCP_REC2AGT_LSN_STOPED				1076		// (0x0434) ����ֹͣ��Ϣ
struct S_TCP_REC2AGT_LSN_STOPED
{
	char	UserName[20];					// �������û���
	UC		RecordStartType;				// ¼���������Ʒ�ʽ���μ���¼�����Ʒ�ʽ��
};
//------------
//--------------------------


//--------------------------
// �ļ���ѯ��Ϣ - Total 6
//--------------------------
//------------
#define TCP_AGT2REC_DOWNLOAD_FILE			101			// (0x0065) ����������´�¼���ļ���Ϣ
struct S_TCP_AGT2REC_DOWNLOAD_FILE
{
	char	FilePath[128];					// ¼���ļ�ȫ·��
};
//------------

//------------
#define TCP_AGT2REC_DOWNLOAD_CANCEL			102			// (0x0066) ȡ���´�¼���ļ���Ϣ
//------------

//------------
#define TCP_REC2AGT_DOWNLOAD_FILE_ACK		1101		// (0x044D) ����������´�¼���ļ���Ӧ��Ϣ
struct S_TCP_REC2AGT_DOWNLOAD_FILE_ACK
{
	char	FilePath[128];					// ¼���ļ�ȫ·��
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_REC2AGT_DOWNLOAD_CANCEL_ACK		1102			// (0x044E) ȡ���´�¼���ļ���Ӧ��Ϣ
struct S_TCP_REC2AGT_DOWNLOAD_CANCEL_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_REC2AGT_DOWNLOAD_STARTED		1103			// (0x044F) �ļ����俪ʼ��Ϣ
struct S_TCP_REC2AGT_DOWNLOAD_STARTED
{
	UDL		FileSize;						// ¼���ļ���С(�ֽ���)
	char	FilePath[128];					// ¼���ļ�ȫ·��
};
//------------

//------------
#define TCP_REC2AGT_DOWNLOAD_FINISHED		1104			// (0x0450) �ļ����������Ϣ
struct S_TCP_REC2AGT_DOWNLOAD_FINISHED
{
	char	FilePath[128];					// ¼���ļ�ȫ·��
	UC		HasMoreFiles;					// �Ƿ��к����ļ���Ҫ���䣬�μ������س������塱
};
//------------
//--------------------------


//--------------------------
// ���������Ϣ - Total 16
//--------------------------
//------------
#define TCP_AGT2REC_APT_ADDMONITOR			121			// (0x0079) ����Ա��ӽ�����ض���(�����ǹ���Ա�ͻ���)
struct S_TCP_AGT2REC_APT_ADDMONITOR
{
	UC		AccessPointID;					// �����ID, 0 Based
};
//------------

//------------
#define TCP_AGT2REC_APT_REMOVEMONITOR		122			// (0x007A) ����Ա���ٽ�����ض���(�����ǹ���Ա�ͻ���)
struct S_TCP_AGT2REC_APT_REMOVEMONITOR
{
	UC		AccessPointID;					// �����ID, 0 Based
};
//------------

//------------
#define TCP_AGT2REC_APTCH_STARTREC_MON		125			// (0x007D) �೤ǿ�ƽ����ͨ����ʼ¼����Ϣ(�����ǹ���Ա�ͻ���)
struct S_TCP_AGT2REC_APTCH_STARTREC_MON
{
	UC		AccessPointID;					// �����ID, 0 Based
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	char	UserData[200];					// �û�����
};
//------------

//------------
#define TCP_AGT2REC_APTCH_STOPREC_MON		126			// (0x007E) �೤ǿ�ƽ����ͨ��ֹͣ¼����Ϣ(�����ǹ���Ա�ͻ���)
struct S_TCP_AGT2REC_APTCH_STOPREC_MON
{
	UC		AccessPointID;					// �����ID, 0 Based
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
};
//------------

//------------
#define TCP_AGT2REC_APTCH_START_LSN			127			// (0x007F) ����Ա��ʼ���������ͨ��(�����ǹ���Ա�ͻ���)
struct S_TCP_AGT2REC_APTCH_START_LSN
{
	UC		AccessPointID;					// �����ID, 0 Based
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
};
//------------

//------------
#define TCP_AGT2REC_APTCH_STOP_LSN			128			// (0x0080) ����Աֹͣ���������ͨ��(�����ǹ���Ա�ͻ���)
struct S_TCP_AGT2REC_APTCH_STOP_LSN
{
	UC		AccessPointID;					// �����ID, 0 Based
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
};
//------------

//------------
#define TCP_REC2AGT_APT_ADDMONITOR_ACK		1121		// (0x0461) ����Ա���ӽ��������Ӧ��Ϣ
struct S_TCP_REC2AGT_APT_ADDMONITOR_ACK
{
	UC		AccessPointID;					// �����ID, 0 Based
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UC		LoginStatus;					// �����ͨ����¼״̬
	US		nChannelCount;					// ͨ������
};
//------------

//------------
#define TCP_REC2AGT_APT_REMOVEMONITOR_ACK	1122		// (0x0462) ����Ա���ٽ��������Ӧ��Ϣ
struct S_TCP_REC2AGT_APT_REMOVEMONITOR_ACK
{
	UC		AccessPointID;					// �����ID, 0 Based
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_REC2AGT_APTCH_MONITOR_ACK		1123		// (0x0463) �����ͨ��״̬��ʼ���棨����Ա���ӽ�����غ�����������ͨ��״̬��
struct S_TCP_REC2AGT_APTCH_MONITOR_ACK
{
	UC		AccessPointID;					// �����ID, 0 Based
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	char	UserName[20];					// �������û���
	UC		LoginStatus;					// �����ͨ����¼״̬
	UC		RecordStatus;					// �����ͨ��¼��״̬
	UC		ListenStatus;					// �����ͨ����ǰ�ܿ�״̬
	UC		MonitorStatus;					// ������û��İ೤״̬
	UC		VoiceDeviceStatus;				// �����ͨ���Ƿ�����
};
//------------

//------------
#define TCP_REC2AGT_APTCH_STARTREC_MON_ACK	1125		// (0x0465) �೤ǿ�ƽ����ͨ����ʼ¼����Ӧ��Ϣ
struct S_TCP_REC2AGT_APTCH_STARTREC_MON_ACK
{
	UC		AccessPointID;					// �����ID, 0 Based
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_REC2AGT_APTCH_STOPREC_MON_ACK	1126		// (0x0466) �೤ǿ�ƽ����ͨ��ֹͣ¼����Ӧ��Ϣ
struct S_TCP_REC2AGT_APTCH_STOPREC_MON_ACK
{
	UC		AccessPointID;					// �����ID, 0 Based
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_REC2AGT_APTCH_START_LSN_ACK		1127		// (0x0467) ���������ͨ������Ӧ����Ϣ
// Replaced 2006-08-08
/// From
/*
struct S_TCP_REC2AGT_APTCH_START_LSN_ACK
{
	UC		AccessPointID;					// �����ID, 0 Based
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
*/
/// To
struct S_TCP_REC2AGT_APTCH_START_LSN_ACK
{
	UC		AccessPointID;					// �����ID, 0 Based
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UC		FormatTag;						// �����ɼ���ʽID�����磬1��G.729��2��ADPCM��3�� MP3�ȵȣ�
	char	FilePath[128];					// ¼���ļ�ȫ·��
	UC		HaveMoreFormatDescription;		// �Ƿ������µĸ�ʽ������
	US		FormatBufferLen;				// ��ʽ��������
	BYTE	FormatDescription[PCS_DEF_FORMAT_DESC_LEN];
};
//------------

//------------
#define TCP_REC2AGT_APTCH_STOP_LSN_ACK		1128		// (0x0468) ֹͣ�����ͨ������Ӧ����Ϣ
struct S_TCP_REC2AGT_APTCH_STOP_LSN_ACK
{
	UC		AccessPointID;					// �����ID, 0 Based
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_REC2AGT_APTCH_LSN_STARTED		1129		// (0x0469) ���������ͨ����ʼ��Ϣ
struct S_TCP_REC2AGT_APTCH_LSN_STARTED
{
	UC		AccessPointID;					// �����ID, 0 Based
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	UC		RecordStartType;				// ¼���������Ʒ�ʽ���μ���¼�����Ʒ�ʽ��
	UC		SamplesPerSec;					// ����Ƶ��(����������)
	UC		BitsPerSample;					// ��������(8 or 16)
	UC		Channels;						// ͨ����(1 or 2)
};
//------------

//------------
#define TCP_REC2AGT_APTCH_LSN_STOPED		1130		// (0x046A) ���������ͨ��ֹͣ��Ϣ
struct S_TCP_REC2AGT_APTCH_LSN_STOPED
{
	UC		AccessPointID;					// �����ID, 0 Based
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	UC		RecordStartType;				// ¼���������Ʒ�ʽ���μ���¼�����Ʒ�ʽ��
};
//------------

//------------
// Added 2006-08-08
#define TCP_REC2AGT_FORMATDESC_CONT			1131		// (0x046B) ������ʽ������
struct S_TCP_REC2AGT_FORMATDESC_CONT
{
	UC		AccessPointID;					// �����ID, 0 Based
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	UC		Index;							// ��˳��1 Based
	UC		HaveMoreFormatDescription;		// �Ƿ������µĸ�ʽ������
	US		FormatBufferLen;				// ��ʽ��������
	BYTE	FormatDescription[PCS_DEF_FORMAT_DESC_LEN];
};
//------------
//--------------------------

//--------------------------
// ¼������������Ϣ - Total 4
//--------------------------
//------------
#define TCP_AGT2REC_REQ_RECORDINDEX_UPDATE				201		// (0x00C9) ����ͬ��������Ϣ
struct S_TCP_AGT2REC_REQ_RECORDINDEX_UPDATE
{
	UC		AccessPointID;			// �����ID, 0 Based.
};
//------------

//------------
#define TCP_REC2AGT_END_RECORDINDEX_UPDATE				202		// (0x00CA) ͬ�����������Ϣ
struct S_TCP_REC2AGT_END_RECORDINDEX_UPDATE
{
	UC		AccessPointID;				// �����ID, 0 Based.
	UC		ErrorCode;						// �����룬�μ��������붨�塱
																// PCS_DEF_RETCODE_SUCC
																// PCS_DEF_RETCODE_USERINTERRUPT
																// PCS_DEF_RETCODE_OTHERS
																// PCS_DEF_RETCODE_SYSERROR
	UL		RecordCount;					// ʵ��ͬ����������
};
//------------

//------------
#define TCP_REC2AGT_ON_RECORDINDEX_UPDATE				203		// (0x00CB) ����ͬ��������Ϣ
struct S_TCP_REC2AGT_ON_RECORDINDEX_UPDATE
{
	UC		AccessPointID;				// �����ID, 0 Based.
	UL		RecordCount;					// ��ͬ����������
};
//------------

//------------
#define TCP_REC2AGT_REQ_RECORDINDEX_UPDATE_ACK				1201		// (0x04B1) ����ͬ��������Ϣ������Ϣ
struct S_TCP_REC2AGT_REQ_RECORDINDEX_UPDATE_ACK
{
	UC		AccessPointID;				// �����ID, 0 Based.
	UC		ErrorCode;						// �����룬�μ��������붨�塱
													// PCS_DEF_RETCODE_SUCC
													// PCS_DEF_RETCODE_NOTEXIST
													// PCS_DEF_RETCODE_OTHERS
													// PCS_DEF_RETCODE_SYSERROR
	UL		RecordCount;					// ��ͬ����������
};
//------------
//--------------------------


//--------------------------
// ��������Ϣ - Total 3
//--------------------------
//------------
#define TCP_AGT2REC_REC_DATAFLOW			901			// (0x0385) ¼��������
// Replaced 2006-08-08
/// From
/*
struct S_TCP_AGT2REC_REC_DATAFLOW
{
	UL		PackageIndex;					// �����
	UC		RecordData[SIZE_AUDIO_PACKED];	// ����
};
*/
/// To
struct S_TCP_AGT2REC_REC_DATAFLOW
{
	UL		PackageIndex;					// �����
	US		DataLength;						// ���ݳ���
	UC		RecordData[SIZE_MAX_AUDIO_PACKAGE];	// ����
};
//------------

//------------
#define TCP_REC2AGT_LSN_DATAFLOW			1901		// (0x076D) ����������
// Replaced 2006-08-08
/// From
/*
struct S_TCP_REC2AGT_LSN_DATAFLOW
{
	UL		PackageIndex;					// �����
	US		DataLength;						// ���ݳ���
	UC		ListenData[SIZE_AUDIO_PACKED];	// ����
};
*/
/// To
struct S_TCP_REC2AGT_LSN_DATAFLOW
{
	UL		PackageIndex;					// �����
	US		DataLength;						// ���ݳ���
	UC		ListenData[SIZE_MAX_AUDIO_PACKAGE];	// ����
};
//------------

//------------
#define TCP_REC2AGT_DOWNLOAD_DATAFLOW		1902		// (0x076E) �ļ��´�������
struct S_TCP_REC2AGT_DOWNLOAD_DATAFLOW
{
	US		DataLength;						// ���ݳ���
	UC		FileData[SIZE_FILE_PACKED];		// ����
};
//------------
//--------------------------


//--------------------------
// ״̬�ı�֪ͨ��Ϣ - Total 4
//--------------------------
//------------
#define TCP_REC2AGT_ACCESSPOINT_STATUS		1996		// (0x07CC) �����״��������Ϣ
struct S_TCP_REC2AGT_ACCESSPOINT_STATUS
{
	UC		AccessPointID;					// �����ID, 0 Based
	UC		AccessPointStatus;				// �豸״̬���μ������س������塱
	UC		Version;						// �汾��
	US		nChannelCount;					// ͨ������
	char	strAccessPointName[50];			// ���������
};
//------------

//------------
#define TCP_REC2AGT_ACCESSPOINT_CH_STATUS	1997		// (0x07CD) �����ͨ��״��������Ϣ
struct S_TCP_REC2AGT_ACCESSPOINT_CH_STATUS
{
	UC		AccessPointID;					// �����ID, 0 Based
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	char	UserName[20];					// �������û���
	UC		StatusID;						// ״̬��ʶ
	UC		StatusValue;					// ״ֵ̬
};
//------------

//------------
#define TCP_REC2AGT_STORAGE_STATUS			1998		// (0x07CE) �������洢״��������Ϣ
struct S_TCP_REC2AGT_STORAGE_STATUS
{
	UC		StorageID;						// �豸��ʶ
	UC		StorageIndex;					// �豸��ţ������ڶ����籸���豸��
	UC		StorageStatus;					// �豸״̬
};

/// �豸��ʶ����
#define PCS_DEF_DEV_ID_LOG					0
#define PCS_DEF_DEV_ID_INDEX				1
#define PCS_DEF_DEV_ID_DATA					2
#define PCS_DEF_DEV_ID_LOCAL_BAK			3
#define PCS_DEF_DEV_ID_NET_BAK				4
#define PCS_DEF_DEV_ID_TAPE					5

/// �豸״̬����
#define PCS_DEF_DEV_ST_ERROR				0
#define PCS_DEF_DEV_ST_WARNING				1
#define PCS_DEF_DEV_ST_NOTICE				2
#define PCS_DEF_DEV_ST_FINE					3
#define PCS_DEF_DEV_ST_UNKNOWN				255
//------------

//------------
#define TCP_REC2AGT_USER_STATUS				1999		// (0x07CF) �û�״̬�ı�֪ͨ
struct S_TCP_REC2AGT_USER_STATUS
{
	char	UserName[20];					// �������û���
	UC		StatusID;						// ״̬��ʶ
	UC		StatusValue;					// ״ֵ̬
};

/// ״̬��ʶ����
#define PCS_REC_ST_ID_LOGIN					0
#define PCS_REC_ST_ID_RECORD				1
#define PCS_REC_ST_ID_LISTEN				2
#define PCS_REC_ST_ID_MONITOR				3
#define PCS_REC_ST_ID_VOICEDEVICE			4
#define PCS_REC_ST_ID_CHANNELOPEN			5
#define PCS_REC_ST_ID_CTIBINDING			6
//------------
//--------------------------

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_RECORDAGENT_H__
//------------------------------------------------------------------------
