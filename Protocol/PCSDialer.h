#include "Stdafx.h"
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __PCSDIALER_H__
#define __PCSDIALER_H__


#define MAX_BUF_SIZE 120
#define MAX_CHANS 8


#define dial_handler 3


#define DXCALL 0
#define DXOFF  1
#define DXUSED 2
#define DXERR  3
#define PCS_MSG_FLAG_OBDIALER		0x68			// OB Dialerͨ�ű�־��'D'

//���͡������߶��壺
#define	USER_OBSERVICE				0x23			// �Ⲧ����
#define	USER_OBDIALER				0x24			// �Զ�������
//ע���Ⲧ������Ϊ���������Զ�����������ͨ�š�


//��������״̬���壺
#define	OBD_DIAL_STATUS_STOP		 0x0 				// - ֹͣ
#define	OBD_DIAL_STATUS_READY		 0x1 				// - ����
#define	OBD_DIAL_STATUS_DIAL		 0x2 				// - ����
#define	OBD_DIAL_STATUS_SUCC		 0x3 				// - ���
#define	OBD_DIAL_STATUS_FAIL		 0x4	 			// - ʧ��
#define	OBD_DIAL_STATUS_RETRY		 0x5 				// - ����

//�������������壺
#define	OBD_DIAL_RESULT_SUCC		 0x48 			// - �ɹ�
#define	OBD_DIAL_RESULT_NOANS		 0x49 			// - ��Ӧ��
#define	OBD_DIAL_RESULT_BUSY		 0x50 			// - �Է�æ
#define	OBD_DIAL_RESULT_ABAND		 0x51 			// - ����
#define	OBD_DIAL_RESULT_FAXTONE		 0x52			// - ����
#define	OBD_DIAL_RESULT_AATONE		 0x53			// - �Զ�����
#define	OBD_DIAL_RESULT_WRONGNO		 0x54			// - �����
#define	OBD_DIAL_RESULT_POWEROFF	 0x55			// - �ػ�
#define	OBD_DIAL_RESULT_OUTSERVICE   0x56			// - ���ڷ���Χ
#define	OBD_DIAL_RESULT_STOPSERVICE  0x57			// - Ƿ��ͣ��
#define	OBD_DIAL_RESULT_OTHERS		 0x79			// - ����
//��Ϣ����
#define UDP_OBS2OBD_REGISTER		  0x1				//�Բ��������п���ע��	[0..19]������IP��[20..21]�����������˿�
#define UDP_OBS2OBD_UNREGISTER		  0x2				//ȡ���Բ������Ŀ���ע��	��
			
#define UDP_OBS2OBD_TASK_ADD		  0x11			//���Ӳ�������	[0..1]����Campaign ID��[2..3]��������ID��[4..23]�绰���룻[24..31]��ͨ��ת�Ӻ���
#define UDP_OBS2OBD_TASK_REMOVE		  0x12			//ɾ����������	[0..1]����Campaign ID��[2..3]��������ID
#define UDP_OBS2OBD_TASK_REMOVE_ALL	  0x13			//ɾ����������	��
#define UDP_OBS2OBD_TASK_QUERY		  0x14			//��ѯ��������״̬	[0..1]����Campaign ID��[2..3]��������ID
#define UDP_OBS2OBD_TASK_LIST_QUERY	  0x15			//��ѯ���������б�	��
			
#define UDP_OBS2OBD_ATTACH_DATA		  0x21			//��������	
			
			
#define UDP_OBD2OBS_REGISTER_ACK	  0x101			//����ע��Ӧ��	[0]�ɹ�0/����ʧ��

#define UDP_OBD2OBS_UNREGISTER_ACK	  0x102			//ȡ������ע��Ӧ��	��
			
#define UDP_OBD2OBS_TAST_ADD_ACK	  0x111			//���Ӳ�������Ӧ��	[0..1]����Campaign ID��[2..3]��������ID��[4]����0/�ܾ�1
#define UDP_OBD2OBS_TAST_REMOVE_ACK	  0x112			//ɾ����������Ӧ��	[0..1]����Campaign ID��[2..3]��������ID��[4]����0/�ܾ�1
#define UDP_OBD2OBS_TAST_REMOVE_ALL_ACK	0x113			//ɾ����������Ӧ��	[0..1]ɾ����������
#define UDP_OBD2OBS_TASK_QUERY_ACK	   0x114			//��ѯ��������״̬	[0..1]����Campaign ID��[2..3]��������ID��[4]����״̬��[5..6]��ǰ״̬����ʱ��
#define UDP_OBD2OBS_TASK_LIST_QUERY_AC	0x115			//��ѯ���������б�	[0..1]��ǰ�����������[2..3]�����ۼ��������[4..5]�����ۼƳɹ�����[6]����ƽ��ͨ��ռ��ʱ��

#define UDP_OBD2OBS_TASK_RESULT			0x120			//��������ִ�н��	[0..1]����Campaign ID��[2..3]��������ID��[4]����ִ�н����[5..6]����ID


#define PCS_MSGHDR_FLAG0		0x55		// ��Ϣͷ��־0
#define PCS_MSGHDR_FLAG1		0xAA		// ��Ϣͷ��־1
#define PCS_MSGHDR_FLAG2		0x7F		// ��Ϣͷ��־2
#define PCS_MSGHDR_FLAG3		0xFE		// ��Ϣͷ��־3



#endif