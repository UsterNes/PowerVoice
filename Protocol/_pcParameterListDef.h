////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		_pcParameterListDef.h
// ��;������   PowerCall�������ƶ���
// ���ߣ�		SUN
// ����ʱ�䣺	2006/01/08
// ����޸�ʱ�䣺2006/01/08
// ��Ȩ��Ϣ��	Copyright (C) 1998 - 2006 PicaSoft
// ��ע��
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __PCPARAMETERLISTDEF_H__
#define __PCPARAMETERLISTDEF_H__
//------------------------------------------------------------------------

// ����������������ϯ��������¼��ϯ�����ڴ�ֵ��Ͳ�������������ϯ�ǳ�
// ע�⣺�˲���ͨ������7 * 24Сʱ������������߹���Ա��Ҫ�ֹ���������ֵ�����°�ʱ��������ϯ�޷�ȫ���ǳ���
// ȱʡֵ��0 - ������
#ifndef PCS_SYSPARAM_NAME_MIN_OL_AGENTS
#define PCS_SYSPARAM_NAME_MIN_OL_AGENTS				"MinLoginAgents"
#endif

// ���������Ҵ���
// ȱʡֵ��'86' - �й�
#ifndef PCS_SYSPARAM_NAME_COUNTRYCODE
#define PCS_SYSPARAM_NAME_COUNTRYCODE				"Country_Code"
#endif

// ���������ص绰������
// ȱʡֵ��'21' - �Ϻ�
#ifndef PCS_SYSPARAM_NAME_LOCALCITYCODE
#define PCS_SYSPARAM_NAME_LOCALCITYCODE				"LocalCity_Code"
#endif

// ���������ص绰���볤��
// ȱʡֵ��'8' - 8λ
#ifndef PCS_SYSPARAM_NAME_LOCALNUMLEN
#define PCS_SYSPARAM_NAME_LOCALNUMLEN				"LocalNum_Length"
#endif

// �������л�����ǰ׺
// ȱʡֵ��'9'
#ifndef PCS_SYSPARAM_NAME_OBPREFIX_LOCAL
#define PCS_SYSPARAM_NAME_OBPREFIX_LOCAL			"LOCAL"
#endif

// ���������ڳ�;����ǰ׺
// ȱʡֵ��'9'
#ifndef PCS_SYSPARAM_NAME_OBPREFIX_NATIONAL
#define PCS_SYSPARAM_NAME_OBPREFIX_NATIONAL			"NATIONAL"
#endif

// ���������ʳ�;����ǰ׺
// ȱʡֵ��'9'
#ifndef PCS_SYSPARAM_NAME_OBPREFIX_INTER
#define PCS_SYSPARAM_NAME_OBPREFIX_INTER			"INTERNATIONAL"
#endif

// ����������Աͳ�����ݸ�λʱ�䣬ÿ�����ʱ�̶�ͳ�����ݽ��и�λ
// ȱʡֵ��'00:00:30'
#ifndef PCS_SYSPARAM_NAME_WORKDATA_RESET_TIME
#define PCS_SYSPARAM_NAME_WORKDATA_RESET_TIME		"WorkDataResetTime"
#endif

// Sun added 2012-08-10
// ������ժ���Զ����ź���
// ȱʡֵ����
#define PCS_SYSPARAM_NAME_AGT_OffHookAutoDial		"OffHookAutoDial"

//------------------------------------------------------------------------
#endif  //End #ifndef #define __PCPARAMETERLISTDEF_H__
//------------------------------------------------------------------------
