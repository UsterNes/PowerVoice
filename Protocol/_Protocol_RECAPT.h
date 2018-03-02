////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		_Protocol_RECAPT.h
// ��;������	¼�������ͨ��Э��
// ���ߣ�		SUN
// ����޸��ˣ�Stephen Wang
// ����ʱ�䣺	2006/01/11
// ����޸�ʱ�䣺2006/12/06
// ��Ȩ��Ϣ��	Copyright (C) 1999 - 2006 PCSG
// ��ע��
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_RECORDACCESSPOINT_H__
#define __PROTOCOL_RECORDACCESSPOINT_H__
//------------------------------------------------------------------------

//--------------------------
// ¼�������ע�� - Total 4
//--------------------------

//--------------------------
// ע�� - 2
//------------
#define TCP_APT2REC_REGISTER				1			// (0x0001) ע����Ϣ
struct S_TCP_APT2REC_REGISTER
{
	UC		AccessPointID;					// �����ID, 0 Based
	UC		Version;						// �汾��
	UC		RegCode[PCS_DEF_NUMLEN_APP];	// ע����
};
//------------

//------------
#define TCP_REC2APT_REGISTER_ACK			1001		// (0x03E9) ע����Ӧ��Ϣ
struct S_TCP_REC2APT_REGISTER_ACK
{
	UC		AccessPointID;					// �����ID, 0 Based
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_BEUSED
											// PCS_DEF_RETCODE_VERSION
											// PCS_DEF_RETCODE_REGCODE
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UC		FirstRecordStartType;			// ��ѡ¼�����Ʒ�ʽ���μ���¼�����Ʒ�ʽ��
	UC		SecondRecordStartType;			// ����¼�����Ʒ�ʽ���μ���¼�����Ʒ�ʽ��
	US		nChannelCount;					// ͨ������
	char	strAccessPointName[50];			// ���������
};
//------------
//--------------------------

//--------------------------
// ȡ��ע�� - 2
//------------
#define TCP_APT2REC_UNREGISTER				2			// (0x0002) ȡ��ע����Ϣ
//------------

//------------
#define TCP_REC2APT_UNREGISTER_ACK			1002		// (0x03EA) ȡ��ע����Ӧ��Ϣ
//------------
//--------------------------

//--------------------------
// �ļ���ѯ��Ϣ - Total 6
//--------------------------
//------------
#define TCP_REC2APT_DOWNLOAD_FILE			101			// (0x0065) ����AP�´�¼���ļ���Ϣ
struct S_TCP_REC2APT_DOWNLOAD_FILE
{
	char	FilePath[128];					// ¼���ļ�ȫ·��
	char	RequirAdmin[20];				// ������
};
//------------

//------------
#define TCP_REC2APT_DOWNLOAD_CANCEL			102			// (0x0066) ȡ���´�¼���ļ���Ϣ
struct S_TCP_REC2APT_DOWNLOAD_CANCEL
{
	int		TransferID;						// �ļ�����ID
};

//------------

//------------
#define TCP_APT2REC_DOWNLOAD_FILE_ACK		1101		// (0x044D) ����AP�´�¼���ļ���Ӧ��Ϣ
struct S_TCP_APT2REC_DOWNLOAD_FILE_ACK
{
	char	FilePath[128];					// ¼���ļ�ȫ·��
	char	RequirAdmin[20];				// �������û���
	int		TransferID;						// ����ʱ���ļ�ID����AP����
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
#define TCP_APT2REC_DOWNLOAD_CANCEL_ACK		1102			// (0x044E) ȡ���´�¼���ļ���Ӧ��Ϣ
struct S_TCP_APT2REC_DOWNLOAD_CANCEL_ACK
{
	int		TransferID;						// �ļ�����ID
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_APT2REC_DOWNLOAD_STARTED		1103			// (0x044F) �ļ����俪ʼ��Ϣ
struct S_TCP_APT2REC_DOWNLOAD_STARTED
{
	int		TransferID;						// �ļ�����ID
	UDL		FileSize;						// ¼���ļ���С(�ֽ���)
};
//------------

//------------
#define TCP_APT2REC_DOWNLOAD_FINISHED		1104			// (0x0450) �ļ����������Ϣ
struct S_TCP_APT2REC_DOWNLOAD_FINISHED
{
	int		TransferID;						// �ļ�����ID
	UC		HasMoreFiles;					// �Ƿ��к����ļ���Ҫ���䣬�μ������س������塱
};
//------------
//--------------------------



//--------------------------
// ¼��ͨ������ - Total 5
//--------------------------
//------------
#define TCP_APT2REC_CH_OPEN					201			// (0x00C9) ¼��ͨ����ʼ��������Ϣ
struct S_TCP_APT2REC_CH_OPEN
{
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	UC		ChannelStatus;					// ¼��ͨ���豸״̬���μ����豸״̬���塱
};
//------------

//------------
#define TCP_APT2REC_CH_CLOSE				202			// (0x00CA) ¼��ͨ���ر�������Ϣ
struct S_TCP_APT2REC_CH_CLOSE
{
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
};
//------------

//------------
#define TCP_APT2REC_CH_STATUS				220		// (0x00DC) ͨ��״��������Ϣ
struct S_TCP_APT2REC_CH_STATUS
{
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	UC		StatusID;						// ״̬��ʶ
	UC		StatusValue;					// ״ֵ̬
};
//------------

//------------
#define TCP_REC2APT_CH_OPEN_ACK				1201		// (0x04B1) ¼��ͨ����ʼ����Ӧ��Ϣ
struct S_TCP_REC2APT_CH_OPEN_ACK
{
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		AgentID;						// PowerCall��ϯ��ţ�CTI����ʱ�����ṩ
	UC		bMustLogin;						// ֻ�е�¼���¼��������ȫ��¼��
	char	strChannelName[20];				// ¼��ͨ�����֣�ȫ��Ψһ��
	UC		FirstRecordStartType;			// ��ѡ¼�����Ʒ�ʽ���μ���¼�����Ʒ�ʽ��
	UC		SecondRecordStartType;			// ����¼�����Ʒ�ʽ���μ���¼�����Ʒ�ʽ��
};
//------------

//------------
#define TCP_REC2APT_CH_CLOSE_ACK			1202		// (0x04B2) ¼��ͨ���ر���Ӧ��Ϣ
struct S_TCP_REC2APT_CH_CLOSE_ACK
{
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------


//--------------------------
// ¼���������� - Total 6
//--------------------------
//------------
#define TCP_APT2REC_GET_SYSPARAM			221			// (0x00DD) ��ѯϵͳ����������Ϣ
struct S_TCP_APT2REC_GET_SYSPARAM
{
	char	strParamName[50];				// ������
	char	strTimeStamp[15];				// ʱ���
};
//------------

//------------
#define TCP_APT2REC_GET_CHPARAM				222			// (0x00DE) ��ѯͨ������������Ϣ
struct S_TCP_APT2REC_GET_CHPARAM
{
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	char	strParamName[50];				// ������
};
//------------

//------------
// Added 2006-12-06
#define TCP_APT2REC_UPDATE_SYSPARAM			223		// (0x00DF) ����ϵͳ������Ϣ
struct S_TCP_APT2REC_UPDATE_SYSPARAM
{
	char	strParamName[50];				// ������
	char	strParamValue[100];				// ����ֵ
	char	strTimeStamp[15];				// ʱ���
};
//------------

//------------
#define TCP_APT2REC_UPDATE_SYSPARAM_ACK		224		// (0x00E0) ����ϵͳ������Ϣ��Ӧ��Ϣ
struct S_TCP_APT2REC_UPDATE_SYSPARAM_ACK
{
	char	strParamName[50];			// ������
	UC		ErrorCode;					// �����룬�μ��������붨�塱��
										// PCS_DEF_RETCODE_SUCC
										// PCS_DEF_RETCODE_NOTEXIST
										// PCS_DEF_RETCODE_LACK_PARAM
										// PCS_DEF_RETCODE_OTHERS
										// PCS_DEF_RETCODE_SYSERROR
	char	strTimeStamp[15];			// ʱ���
};
//------------


//------------
#define TCP_REC2APT_GET_SYSPARAM_ACK		1221		// (0x04C5) ��ѯϵͳ������Ӧ��Ϣ
struct S_TCP_REC2APT_GET_SYSPARAM_ACK
{
	char	strParamName[50];				// ������
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	char	strParamValue[100];				// ����ֵ
};
//------------

//------------
#define TCP_REC2APT_GET_CHPARAM_ACK			1222		// (0x04C6) ��ѯͨ��������Ӧ��Ϣ
struct S_TCP_REC2APT_GET_CHPARAM_ACK
{
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	char	strParamName[50];				// ������
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_LACK_PARAM
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	char	strParamValue[100];				// ����ֵ
};
//------------

//------------
#define TCP_APT2REC_GET_CHPARAM_ACK			1223		// (0x04C7) ��ѯͨ��������Ӧ��Ϣ
struct S_TCP_APT2REC_GET_CHPARAM_ACK
{
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	char	strParamName[50];				// ������
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_LACK_PARAM
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	char	strParamValue[100];				// ����ֵ
};
//------------

// Added 2006-12-06
#define TCP_REC2APT_UPDATE_SYSPARAM_ACK		1224		// (0x04C8) ����ϵͳ������Ϣ��Ӧ��Ϣ
struct S_TCP_REC2APT_UPDATE_SYSPARAM_ACK
{
	char	strParamName[50];			// ������
	UC		ErrorCode;					// �����룬�μ��������붨�塱��
										// PCS_DEF_RETCODE_SUCC
										// PCS_DEF_RETCODE_NOTEXIST
										// PCS_DEF_RETCODE_LACK_PARAM
										// PCS_DEF_RETCODE_OTHERS
										// PCS_DEF_RETCODE_SYSERROR
	char	strTimeStamp[15];			// ʱ���
};
//------------

//------------
#define TCP_REC2APT_UPDATE_SYSPARAM			1225		// (0x04C9) ����ϵͳ������Ϣ
struct S_TCP_REC2APT_UPDATE_SYSPARAM
{
	char	strParamName[50];				// ������
	char	strParamValue[100];				// ����ֵ
	char	strTimeStamp[15];				// ʱ���
};
//------------

//--------------------------


//--------------------------
// ¼��������Ϣ - Total 9
//--------------------------
//------------
#define TCP_APT2REC_STARTREC				251			// (0x00FB) ͨ����ʼ¼��������Ϣ
// Replaced 2006-08-08
/// From
/*
struct S_TCP_APT2REC_STARTREC
{
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	UC		RecordStartType;				// ¼���������Ʒ�ʽ���μ���¼�����Ʒ�ʽ��
	UC		SamplesPerSec;					// ����Ƶ��(����������)
	UC		BitsPerSample;					// ��������(8 or 16)
	UC		Channels;						// ͨ����(1 or 2)
	char	strANI[PCS_DEF_NUMLEN_APP];		// �绰���룬�忨��ȡ
};
*/
/// To
struct S_TCP_APT2REC_STARTREC
{
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	UC		RecordStartType;				// ¼���������Ʒ�ʽ���μ�"¼�����Ʒ�ʽ"
	UC		FormatTag;						// �����ɼ���ʽID�����磬1��G.729��2��ADPCM��3�� MP3�ȵȣ�
	char	strANI[PCS_DEF_NUMLEN_APP];		// �绰���룬�忨��ȡ
	char	FilePath[128];					// ����¼���ļ�ȫ·��
	UC		HaveMoreFormatDescription;		// �Ƿ������µĸ�ʽ������
	US		FormatBufferLen;				// ��ʽ��������
	BYTE	FormatDescription[PCS_DEF_FORMAT_DESC_LEN];	
											// ����Ǹ�ʽ�����Ķ�������������˵FormatTag ΪADPCM��ʱ�򣬾���һ��WAVEFORMATEX�ṹ��ʵ�������ΪMP3 ʱ������һ��MP3��ʽ�����Ľṹ��ʵ����
};
//------------

//------------
// Added 2006-08-08
#define TCP_APT2REC_FORMATDESC_CONT			252			// (0x00FC) ������ʽ������
struct S_TCP_APT2REC_FORMATDESC_CONT
{
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	UC		Index;							// ��˳��1 Based
	UC		HaveMoreFormatDescription;		// �Ƿ������µĸ�ʽ������
	US		FormatBufferLen;				// ��ʽ��������
	BYTE	FormatDescription[PCS_DEF_FORMAT_DESC_LEN];
};
//------------

//------------
#define TCP_APT2REC_STOPREC					261			// (0x0105) ͨ��ֹͣ¼��������Ϣ
struct S_TCP_APT2REC_STOPREC
{
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	UC		RecordStopType;					// ¼��ֹͣ���Ʒ�ʽ���μ���¼�����Ʒ�ʽ��
	UL		RecordLength;					// ��¼������ added by stephen wang 2006-08-21
	char	KeyPressData[200];				// ��������
};
//------------

//------------
// Added 2006-08-08
#define TCP_APT2REC_GET_RECDATA_ACK			271			// (0x010F) ����AP�����������ݵ���Ӧ��Ϣ
struct S_TCP_APT2REC_GET_RECDATA_ACK
{
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_STATUS
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_REC2APT_STARTREC_ACK			1251		// (0x04E3) ͨ����ʼ¼����Ӧ��Ϣ
// Replaced 2006-08-08
/// From
/*
struct S_TCP_REC2APT_STARTREC_ACK
{
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
*/
/// To
struct S_TCP_REC2APT_STARTREC_ACK
{
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	UC		ErrorCode;						// �����룬�μ�"�����붨��"
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UC		IsLocalSave;					// �Ƿ񱾵ش洢
};
//------------

//------------
#define TCP_REC2APT_STARTREC_CMD			1252		// (0x04E4) ����������ͨ����ʼ¼��(CTI���ơ��೤���ơ��û��ֶ�����)
struct S_TCP_REC2APT_STARTREC_CMD
{
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
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
#define TCP_REC2APT_STOPREC_ACK				1261		// (0x04ED) ͨ��ֹͣ¼����Ӧ��Ϣ
struct S_TCP_REC2APT_STOPREC_ACK
{
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
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
#define TCP_REC2APT_STOPREC_CMD				1262		// (0x04EE) ����������ͨ��ֹͣ¼��(CTI���ơ��೤���ơ��û��ֶ�����)
struct S_TCP_REC2APT_STOPREC_CMD
{
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	UC		RecordStopType;					// ¼��ֹͣ���Ʒ�ʽ���μ���¼�����Ʒ�ʽ��
	char	MonitorName[20];				// �೤�����೤����ʱʹ��
};
//------------

//------------
#define TCP_REC2APT_CTI_CALLHELD				1263		// (0x04EF) �绰������Ϣ
struct S_TCP_REC2APT_CTI_CALLHELD
{
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
};
//------------
//--------------------------



//------------
// Added 2006-08-08
#define TCP_REC2APT_GET_RECDATA_REQ			1271		// (0x04F7) ����AP��������������Ϣ
struct S_TCP_REC2APT_GET_RECDATA_REQ
{
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
};
//------------


//------------
// Added 2006-08-22
#define TCP_REC2APT_CANCEL_RECDATA_TRANS	1272		// (0x04F8)	ȡ������AP��������������Ϣ
struct S_TCP_REC2APT_CANCEL_RECDATA_TRANS
{
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
};
//------------
//--------------------------


//--------------------------
// ¼���������ݹ�����Ϣ - Total 4
//--------------------------
//------------
#define TCP_APT2REC_REQ_RECORDINDEX_UPDATE_ACK	301		// (0x012D) Server��AP���ʹ���¼������������Ӧ��Ϣ
struct S_TCP_APT2REC_REQ_RECORDINDEX_UPDATE_ACK
{
	UC		AccessPointID;					// �����ID, 0 Based.
	UC		ErrorCode;						// �����룬�μ��������붨�塱
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		RecordCount;					// ��ͬ����������
};
//------------

//------------
#define TCP_APT2REC_END_RECORDINDEX_UPDATE	302			// (0x012E) �������ͽ�����Ϣ
struct S_TCP_APT2REC_END_RECORDINDEX_UPDATE
{
	UC		AccessPointID;					// �����ID, 0 Based.
	UC		ErrorCode;						// �����룬�μ��������붨�塱
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_USERINTERRUPT
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		RecordCount;					// ʵ��ͬ����������
};
//------------

//------------
#define TCP_APT2REC_RECORDINDEX_UPDATE		303			// (0x012F) ¼���������ݰ���Ϣ
struct S_TCP_APT2REC_RECORDINDEX_UPDATE
{
	UC		AccessPointID;					// �����ID, 0 Based.
	UL		PackageNo;						// ¼����������
	US		DataLength;						// ¼�����������ݳ��ȣ����һ�������ܲ�������󳤶ȣ�
	UC		HaveMoreData;					// ���к������ݷ�
	UC		RecordIndexBuffer[SIZE_FILE_PACKED];	// ¼������Buffer
};
//------------

//------------
#define TCP_REC2APT_REQ_RECORDINDEX_UPDATE	1301		// (0x0515) Server��AP���ʹ���¼������������Ϣ
struct S_TCP_REC2APT_REQ_RECORDINDEX_UPDATE
{
	UC		AccessPointID;					// �����ID, 0 Based.
};
//------------
//--------------------------



//--------------------------
// ��������Ϣ - Total 2
//--------------------------
//------------
#define TCP_APT2REC_REC_DATAFLOW			951			// (0x03B7) ͨ��¼��������
// Replaced 2006-08-08
/// From
/*
struct S_TCP_APT2REC_REC_DATAFLOW
{
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	UL		PackageIndex;					// �����
	US		DataLength;						// ���ݳ���
	UC		RecordData[SIZE_AUDIO_PACKED];	// ����
};
*/
/// To
struct S_TCP_APT2REC_REC_DATAFLOW
{
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	UL		PackageIndex;					// �����
	US		DataLength;						// ���ݳ���
	UC		RecordData[SIZE_MAX_AUDIO_PACKAGE];	// ����
};
//------------

//------------
// Added 2006-12-06
#define TCP_APT2REC_DOWNLOAD_DATAFLOW		952		// (0x03B8) �ļ��´�������
struct S_TCP_APT2REC_DOWNLOAD_DATAFLOW
{
	int		TransferID;						// �ļ�����ID
	UL		PackageNo;						// �ļ������ ��1-n��
	US		DataLength;						// ���ݳ���
	UC		FileData[SIZE_FILE_PACKED];		// ����
};
//------------
//--------------------------



//--------------------------
// ״̬֪ͨ��Ϣ - Total 1
//--------------------------
//------------
#define TCP_REC2APT_USER_STATUS				1999		// (0x07CF) ͨ���û�״̬�ı�֪ͨ
struct S_TCP_REC2APT_USER_STATUS
{
	US		ChannelIndex;					// ¼��ͨ����ţ�1 Based
	char	UserName[20];					// �������û���
	UC		StatusID;						// ״̬��ʶ
	UC		StatusValue;					// ״ֵ̬
};
//--------------------------


//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_RECORDACCESSPOINT_H__
//------------------------------------------------------------------------
