////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		_pcRightsDef.h
// ��;������   PowerCallȨ��λ����
// ���ߣ�		SUN
// ����ʱ�䣺	2002/06/05
// ����޸�ʱ�䣺2003/09/16
// ��Ȩ��Ϣ��	Copyright (C) 1998 - 2003 Proactive Cyber Space
// ��ע��
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PCRIGHTSDEF_H__
#define __PCRIGHTSDEF_H__
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Ȩ��
// �ַ�����50λ��ÿλ���壬����ͼ��ʾ
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// ÿһλ0����û�д�Ȩ�ޣ�1�����и�Ȩ��

// ��ϯȨ��
#define PCS_AGENT_CONFIG_POWERBAR				2				// ���ÿͻ��˲���
#define PCS_AGENT_CHANGEPASSWORD				26				// �޸Ŀ���
#define PCS_AGENT_CALLOUT						27				// �Ⲧ
#define PCS_AGENT_TRANSFER						28				// �绰ת��
#define PCS_AGENT_CONFERENCE					29				// ����
#define PCS_AGENT_SINGLESTEPTRANSFER			30				// ����ת��
#define PCS_AGENT_PARKING						31				// �绰פ��
#define PCS_AGENT_FETCHING						32				// פ���绰ȡ��
#define PCS_AGENT_MONITOR_BREAK					44				// �೤ǿ��
#define PCS_AGENT_MONITOR_HOLDUP				45				// �೤����
#define PCS_AGENT_MONITOR_INSERT				46				// �೤ǿ��
#define PCS_AGENT_MONITOR_LOGOUT				47				// �೤ǿ��ע��
#define PCS_AGENT_PRESET_MESSAGE				48				// ������Ϣ
#define PCS_AGENT_AUTOSEND_MESSAGE				49				// ��ʱ��Ϣ�趨

// ����ԱȨ��
// ��ϵͳ(1 - 19)
#define PCS_USE_POWERBAR						1				// ʹ�ÿͻ���
#define PCS_USE_CONFIG_POWERBAR					2				// ���ÿͻ��˲���
#define PCS_USE_AGENTMANAGER					3				// ʹ��AgentManager
#define PCS_USE_POWERVOICE						4				// ʹ��PowerVoice
#define PCS_USE_CONFIG_POWERVOICE				5				// ����PowerVoice
#define PCS_USE_ROUTEMANAGE						6				// ʹ��RouteManager
#define PCS_USE_CONFIG_ROUTEMANAGE				7				// ����RouteManager
#define PCS_USE_POWERANALYSIS					8				// ʹ�ñ�����
#define PCS_USE_CONFIG_POWERANALYSIS			9				// ���ñ�����
#define PCS_USE_POWERMONITOR					10				// ʹ�ü�ع���
#define PCS_USE_CONFIG_POWERMONITOR				11				// ���ü�ع���
#define PCS_USE_OBSERVICE						12				// ʹ��OB Service
#define PCS_USE_CONFIG_OBSERVICE				13				// ����OB Service
#define PCS_USE_RECORDCLIENT					14				// ʹ��¼���ͻ���
#define PCS_USE_CONFIG_RECORDCLIENT				15				// ����¼���ͻ���
#define PCS_USE_RECORDADMIN						16				// ʹ��¼������Ա
#define PCS_USE_CONFIG_RECORDADMIN				17				// ����¼������Ա
#define PCS_USE_CHECK_VOICEMAIL					18				// �����������
#define PCS_USE_DELETE_VOICEMAIL				19				// ������������

// Agent ���(20 - 30)
#define PCS_USE_RECEIVECHATMSG					20				// ����������Ϣ
#define PCS_USE_SENDTOLED						21				// ������Ϣ��LED
#define PCS_USE_SENDALLMESSAGE					22				// ������Ϣ��ȫ����ϯ
#define PCS_USE_SENDGROUPMESSAGE				23				// ������Ϣ����ϯ��
#define PCS_USE_SENDTEAMMESSAGE					24				// ������Ϣ����ϯС��
#define PCS_USE_SENDSINGLEMESSAGE				25				// ������Ϣ��������ϯ
#define PCS_USE_CHANGEPASSWORD					26				// �޸Ŀ���
#define PCS_USE_CALLOUT							27				// �Ⲧ
#define PCS_USE_TRANSFER						28				// �绰ת��
#define PCS_USE_CONFERENCE						29				// ����
#define PCS_USE_SINGLESTEPTRANSFER				30				// ����ת��

// OB ���( 31 - 40)
#define PCS_USE_VIEWALL_TASK					31				// �鿴��������
#define PCS_USE_VIEWGROUP_TASK					32				// �鿴��������
#define PCS_USE_VIEWTEAM_TASK					33				// �鿴��С������
#define PCS_USE_VIEWSELF_TASK					34				// �鿴��������

// Monitor ���(41 - 60)
#define PCS_USE_MONITOR_ALL						41				// ���������ϯ
#define PCS_USE_MONITOR_GROUP					42				// ��ر�����ϯ
#define PCS_USE_MONITOR_TEAM					43				// ��ر�С����ϯ
#define PCS_USE_MONITOR_BREAK					44				// �೤ǿ��
#define PCS_USE_MONITOR_HOLDUP					45				// �೤����
#define PCS_USE_MONITOR_INSERT					46				// �೤ǿ��
#define PCS_USE_MONITOR_LOGOUT					47				// �೤ǿ��ע��
#define PCS_USE_PRESET_MESSAGE					48				// ������Ϣ
#define PCS_USE_AUTOSEND_MESSAGE				49				// ��ʱ��Ϣ�趨
#define PCS_USE_MODIFY_FLOOR_PLAN				50				// �޸Ĳ���
#define PCS_USE_CONFIG_LOCAL_ALERT				51				// �޸ı��ر���
#define PCS_USE_SET_CELL_FORMAT					52				// ���Ƶ�Ԫ���ʽ
#define PCS_USE_SET_BAR_FORMAT					53				// ����״̬��
#define PCS_USE_CONFIG_VIEWS					54				// ������ͼ
#define PCS_USE_SELECT_VIEWS					55				// ѡ����ͼ
#define PCS_USE_SET_LEAVE_THRESHOLD				56				// ��Ϣ��������
#define PCS_USE_SET_BULLETIN_MESSAGE			57				// ���ù�����Ϣ
#define PCS_USE_MONITOR_CHG_AGTST				58				// �೤ǿ���޸���ϯ״̬

//------------------------------------------------------------------------
#endif  //End #ifndef #define __PCRIGHTSDEF_H__
//------------------------------------------------------------------------
