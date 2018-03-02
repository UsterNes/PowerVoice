////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		_Protocol_Admin.h
// ��;������	�������̨ͨ��Э��
// ���ߣ�		SUN
// ����ʱ�䣺	1999/12/01
// ����޸�ʱ�䣺2003/09/20
// ��Ȩ��Ϣ��	Copyright (C) 1999 - 2003 PCSG
// ��ע��
////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PROTOCOL_ADMIN_H__
#define __PROTOCOL_ADMIN_H__
//------------------------------------------------------------------------

//--------------------------
// ע�� - 2
//------------
#define TCP_ADM2PCS_REGISTER				1			// (0x0001) ע����Ϣ
struct S_TCP_AMD2PCS_REGISTER
{
	UC		Version;						// �汾��
	UC		RegCode[PCS_DEF_NUMLEN_APP];	// ע����
};
//------------

//------------
#define TCP_PCS2ADM_REGISTER_ACK			1001		// (0x03E9) ע����Ӧ��Ϣ
struct S_TCP_PCS2ADM_REGISTER_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_BEUSED
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
#define TCP_ADM2PCS_UNREGISTER				2			// (0x0002) ȡ��ע����Ϣ
//------------

//------------
#define TCP_PCS2ADM_UNREGISTER_ACK			1002		// (0x03EA) ȡ��ע����Ӧ��Ϣ
//------------
//--------------------------


//--------------------------
// ��ϯ���� - 6
//------------
#define TCP_ADM2PCS_ADD_AGENT				901			// (0x0385) ������ϯ��Ϣ
struct S_TCP_ADM2PCS_ADD_AGENT
{
	UL		AgentID;						// ��ϯID
	UC		IsMonitor;						// �Ƿ��Ǽ����
};
//------------

//------------
#define TCP_ADM2PCS_DEL_AGENT				902			// (0x0386) ɾ����ϯ��Ϣ
struct S_TCP_ADM2PCS_DEL_AGENT
{
	UL		AgentID;						// ��ϯID
	UC		IsMonitor;						// �Ƿ��Ǽ����
};
//------------

//------------
#define TCP_ADM2PCS_MOD_AGENT				903			// (0x0387) �޸���ϯ��Ϣ
struct S_TCP_ADM2PCS_MOD_AGENT
{
	UL		AgentID;						// ��ϯID
	UC		IsMonitor;						// �Ƿ��Ǽ����
};
//------------

//------------
#define TCP_PCS2ADM_ADD_AGENT_ACK			1901		// (0x076D) ������ϯ��Ӧ��Ϣ
struct S_TCP_PCS2ADM_ADD_AGENT_ACK
{
	UL		AgentID;						// ��ϯID
	UC		IsMonitor;						// �Ƿ��Ǽ����
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_BEUSED
											// PCS_DEF_RETCODE_NOLICENSE
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2ADM_DEL_AGENT_ACK			1902		// (0x076E) ɾ����ϯ��Ӧ��Ϣ
struct S_TCP_PCS2ADM_DEL_AGENT_ACK
{
	UL		AgentID;						// ��ϯID
	UC		IsMonitor;						// �Ƿ��Ǽ����
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2ADM_MOD_AGENT_ACK			1903		// (0x076F) �޸���ϯ��Ӧ��Ϣ
struct S_TCP_PCS2ADM_MOD_AGENT_ACK
{
	UL		AgentID;						// ��ϯID
	UC		IsMonitor;						// �Ƿ��Ǽ����
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------


//--------------------------
// �û����� - 6
//------------
#define TCP_ADM2PCS_ADD_USER				906			// (0x038A) �����û���Ϣ
struct S_TCP_ADM2PCS_ADD_USER
{
	char	strAliasName[50];				// ��¼����
};
//------------

//------------
#define TCP_ADM2PCS_DEL_USER				907			// (0x038B) ɾ���û���Ϣ
struct S_TCP_ADM2PCS_DEL_USER
{
	char	strAliasName[50];				// ��¼����
};
//------------

//------------
#define TCP_ADM2PCS_MOD_USER				908			// (0x038C) �޸��û���Ϣ
struct S_TCP_ADM2PCS_MOD_USER
{
	char	strAliasName[50];				// ��¼����
};
//------------

//------------
#define TCP_PCS2ADM_ADD_USER_ACK			1906		// (0x0772) �����û���Ӧ��Ϣ
struct S_TCP_PCS2ADM_ADD_USER_ACK
{
	char	strAliasName[50];				// ��¼����
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_BEUSED
											// PCS_DEF_RETCODE_NOLICENSE
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2ADM_DEL_USER_ACK			1907		// (0x0773) ɾ���û���Ӧ��Ϣ
struct S_TCP_PCS2ADM_DEL_USER_ACK
{
	char	strAliasName[50];				// ��¼����
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_BEUSED
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2ADM_MOD_USER_ACK			1908		// (0x0774) �޸��û���Ӧ��Ϣ
struct S_TCP_PCS2ADM_MOD_USER_ACK
{
	char	strAliasName[50];				// ��¼����
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------


//--------------------------
// Զ�̿��� - 16
//------------
//------------
#define TCP_ADM2PCS_RELOAD_AGENTLIST		911			// (0x038F) ���¼�����ϯ�б���Ϣ
//------------

//------------
#define TCP_ADM2PCS_RELOAD_MONITORLIST		912			// (0x0390) ���¼��ؼ���б���Ϣ
//------------

//------------
#define TCP_ADM2PCS_RELOAD_ACD_SCENARIO		913			// (0x0391) ���¼����Ŷӹ�����Ϣ
//------------

//------------
#define TCP_ADM2PCS_RELOAD_SYSPARAM			914			// (0x0392) ���¼���ϵͳ������
//------------

//------------
#define TCP_ADM2PCS_RELOAD_USERLIST			915			// (0x0393) ���¼����û��б���Ϣ
//------------

//------------
#define TCP_ADM2PCS_RELOAD_PBXACDLIST		916			// (0x0394) ���¼���ACD����б���Ϣ
//------------

//------------
#define TCP_ADM2PCS_RELOAD_USERPARAM		917			// (0x0395) ���¼����û�������Ϣ
struct S_TCP_ADM2PCS_RELOAD_USERPARAM
{
	char	strAliasName[50];				// ��¼����
};
//------------

//------------
#define TCP_ADM2PCS_RELOAD_CODELIST			918			// (0x0396) ���´������Ϣ
struct S_TCP_ADM2PCS_RELOAD_CODELIST
{
	UC		bytCodeListID;					// �����ID���μ������ID����
};

/// �����ID����
#define PCS_DEF_CODELIST_COUNTRYCODE		1			// ���Ҵ����
#define PCS_DEF_CODELIST_CITYCODE			2			// ���ű�
#define PCS_DEF_CODELIST_SPECIALTELNO		3			// ����绰�����
#define PCS_DEF_CODELIST_IPPREFIX			4			// IPǰ׺��
#define PCS_DEF_CODELIST_MOBILEAREA			5			// �ֻ������ر�
//------------

//------------
#define TCP_PCS2ADM_RELOAD_AGENTLIST_ACK	1911		// (0x0777) ���¼�����ϯ�б���Ӧ��Ϣ
struct S_TCP_PCS2ADM_RELOAD_AGENTLIST_ACK
{
	US		LoadAgents;						// ������ϯ����
	US		ReclaimAgents;					// ������ϯ����
};
//------------

//------------
#define TCP_PCS2ADM_RELOAD_MONITORLIST_ACK	1912		// (0x0778) ���¼��ؼ���б���Ӧ��Ϣ
struct S_TCP_PCS2ADM_RELOAD_MONITORLIST_ACK
{
	US		LoadMonitors;					// ���ؼ������
	US		ReclaimMonitors;				// ���ռ������
};
//------------

//------------
#define TCP_PCS2ADM_RELOAD_ACD_SCENARIO_ACK	1913		// (0x0779) ���¼����Ŷӹ�����Ӧ��Ϣ
struct S_TCP_PCS2ADM_RELOAD_ACD_SCENARIO_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2ADM_RELOAD_SYSPARAM_ACK		1914		// (0x077A) ���¼���ϵͳ��������Ӧ��Ϣ
struct S_TCP_PCS2ADM_RELOAD_SYSPARAM_ACK
{
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2ADM_RELOAD_USERLIST_ACK		1915		// (0x077B) ���¼����û��б���Ӧ��Ϣ
struct S_TCP_PCS2ADM_RELOAD_USERLIST_ACK
{
	US		LoadUsers;						// �����û�����
	US		ReclaimUsers;					// �����û�����
};
//------------

//------------
#define TCP_PCS2ADM_RELOAD_PBXACDLIST_ACK	1916		// (0x077C) ���¼���ACD����б���Ӧ��Ϣ
struct S_TCP_PCS2ADM_RELOAD_PBXACDLIST_ACK
{
	US		LoadACDDNs;						// ����ACD DN����
	US		ReclaimACDDNs;					// ����ACD DN����
};
//------------

//------------
#define TCP_PCS2ADM_RELOAD_USERPARAM_ACK	1917		// (0x077B) ���¼����û���������Ӧ��Ϣ
struct S_TCP_PCS2ADM_RELOAD_USERPARAM_ACK
{
	char	strAliasName[50];				// ��¼����
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------

//------------
#define TCP_PCS2ADM_RELOAD_CODELIST_ACK		1918		// (0x077C) ���´������Ӧ��Ϣ
struct S_TCP_PCS2ADM_RELOAD_CODELIST_ACK
{
	UC		bytCodeListID;					// �����ID���μ������ID����
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------


//--------------------------
// �������� - 8
//------------
//------------
#define TCP_ADM2PCS_GET_SYSPARAM			921			// (0x0399) ��ѯϵͳ����
struct S_TCP_ADM2PCS_GET_SYSPARAM
{
	char	strParamName[50];				// ������
};
//------------

//------------
#define TCP_ADM2PCS_CHG_SYSPARAM			922			// (0x039A) �޸�ϵͳ����
struct S_TCP_ADM2PCS_CHG_SYSPARAM
{
	char	strParamName[50];				// ������
	char	strParamValue[100];				// ����ֵ
};
//------------

//------------
#define TCP_ADM2PCS_GET_USERPARAM			923			// (0x039B) ��ѯ�û�����
struct S_TCP_ADM2PCS_GET_USERPARAM
{
	char	strAliasName[50];				// ��¼����
	char	strParamName[50];				// ������
};
//------------

//------------
#define TCP_ADM2PCS_CHG_USERPARAM			924			// (0x039C) �޸��û�����
struct S_TCP_ADM2PCS_CHG_USERPARAM
{
	char	strAliasName[50];				// ��¼����
	char	strParamName[50];				// ������
	char	strParamValue[100];				// ����ֵ
};
//------------

//------------
#define TCP_PCS2ADM_GET_SYSPARAM_ACK		1921		// (0x0781) ��ѯϵͳ������Ӧ��Ϣ
struct S_TCP_PCS2ADM_GET_SYSPARAM_ACK
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
#define TCP_PCS2ADM_CHG_SYSPARAM_ACK		1922		// (0x0782) �޸�ϵͳ������Ӧ��Ϣ
struct S_TCP_PCS2ADM_CHG_SYSPARAM_ACK
{
	char	strParamName[50];				// ������
	char	strParamValue[100];				// ����ֵ
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
#define TCP_PCS2ADM_GET_USERPARAM_ACK		1923		// (0x0783) ��ѯ�û�������Ӧ��Ϣ
struct S_TCP_PCS2ADM_GET_USERPARAM_ACK
{
	char	strAliasName[50];				// ��¼����
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
#define TCP_PCS2ADM_CHG_USERPARAM_ACK		1924		// (0x0784) �޸��û�������Ӧ��Ϣ
struct S_TCP_PCS2ADM_CHG_USERPARAM_ACK
{
	char	strAliasName[50];				// ��¼����
	char	strParamName[50];				// ������
	char	strParamValue[100];				// ����ֵ
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_NORIGHTS
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
};
//------------
//--------------------------

//--------------------------
// ϵͳά�� - 2
//------------
#define TCP_ADM2PCS_CONNECTION_RESET		981			// (0x03D5) ���Ӹ�λ��Ϣ
struct S_TCP_ADM2PCS_CONNECTION_RESET
{
	UC		SysType;						// ϵͳ���
	UC		SysNo;							// ϵͳ����
};
//------------

//------------
#define TCP_PCS2ADM_CONNECTION_RESET_ACK	1981		// (0x07BD) ���Ӹ�λ��Ӧ��Ϣ
struct S_TCP_PCS2ADM_CONNECTION_RESETE_ACK
{
	UC		SysType;						// ϵͳ���
	UC		SysNo;							// ϵͳ����
	UC		ErrorCode;						// �����룬�μ��������붨�塱��
											// PCS_DEF_RETCODE_SUCC
											// PCS_DEF_RETCODE_NOTEXIST
											// PCS_DEF_RETCODE_NOTNEED
											// PCS_DEF_RETCODE_OTHERS
											// PCS_DEF_RETCODE_SYSERROR
	UL		ThreadID;						// �߳�ID
};
//------------
//--------------------------

//------------------------------------------------------------------------
#endif  //End #ifndef __PROTOCOL_ADMIN_H__
//------------------------------------------------------------------------
