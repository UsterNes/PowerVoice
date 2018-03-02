////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		_msRightsDef.h
// ��;������   MaxiSenseȨ��λ����
// ���ߣ�		SUN
// ����ʱ�䣺	2002/06/05
// ����޸�ʱ�䣺2006/12/06
// ��Ȩ��Ϣ��	Copyright (C) 1998 - 2004 PicaSoft
// ��ע��
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

//------------------------------------------------------------------------
#ifndef __MSRIGHTSDEF_H__
#define __MSRIGHTSDEF_H__
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// �û�Ȩ��
// �ַ�����50λ��ÿλ���壬����ͼ��ʾ
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// ÿһλ0����û�д�Ȩ�ޣ�1�����и�Ȩ��

#define PVR_USE_COMPEL_RECORD					1				// ǿ�ƿͻ���¼��
#define PVR_USE_LISTENING						2				// ����
#define PVR_USE_CHANGE_PASSWORD					3				// �޸Ŀ���
#define PVR_USE_CHANGE_USERPARAM				4				// �޸��û�����
#define PVR_USE_CHANGE_SYSPARAM					5				// �޸�ϵͳ����
#define PVR_USE_ADD_USER						6				// ����û�
#define PVR_USE_DEL_USER						7				// ɾ���û�
#define PVR_USE_MOD_USER						8				// �޸��û�
#define PVR_USE_RELOAD_USERLIST					9				// ���¼����û���
#define PVR_USE_RELOAD_SYSPARAM					10				// ���¼���ϵͳ������
#define PVR_USE_DOWNLOAD_FILE					11				// �ӷ���������¼���ļ�
#define PVR_USE_MANAGER_ALL_GROUP 				12				// �ܷ�������е���(Administrator,Supervisor,Agent)

typedef enum __pcsUserRights
{
	pcsUR_USE_MANAGER_ALL_GROUP = 0,						// �ܷ�������е���(Administrator,Supervisor,Agent)
	pcsUR_USE_COMPEL_RECORD = 1,							// ǿ�ƿͻ���¼��
	pcsUR_USE_LISTENING	= 2,								// ����
	pcsUR_USE_CHANGE_PASSWORD =	3,							// �޸Ŀ���-User
	pcsUR_USE_CHANGE_USERPARAM = 4,							// �޸��û�����
	pcsUR_USE_CHANGE_SYSPARAM =	5,							// �޸�ϵͳ����-Administrator
	pcsUR_USE_ADD_USER = 6,									// ����û�
	pcsUR_USE_DEL_USER = 7,									// ɾ���û�
	pcsUR_USE_MOD_USER = 8,									// �޸��û�
	pcsUR_USE_RELOAD_USERLIST =	9,							// ���¼����û���
	pcsUR_USE_RELOAD_SYSPARAM =	10,							// ���¼���ϵͳ������-Administrator
	pcsUR_USE_DOWNLOAD_FILE = 11,							// �ӷ���������¼���ļ�-User	
	pcsUR_USE_DO_QA	= 12,									// �ܷ�����
	pcsUR_USE_EXPORT_RECORDFILE = 13,						// ����¼��Ȩ��
	pcsUR_USE_MOD_MANA_APS = 14,							// �޸�Admin���Բ鿴��ЩAP
	pcsUR_USE_ADD_AP = 15,									// ���AP
	pcsUR_USE_MOD_AP = 16,									// ɾ��AP
	pcsUR_USE_DEL_AP = 17,									// �޸�AP
	pcsUR_USE_RELOAD_APLIST = 18,							// ���¼���AP��

} pcsUserRights;
//------------------------------------------------------------------------
#endif  //End #ifndef #define __MSRIGHTSDEF_H__
//------------------------------------------------------------------------
