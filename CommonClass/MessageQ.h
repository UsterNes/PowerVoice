///////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		MessageQ.h
// ��;������	��Ϣ����������
// ���ߣ�		SUN
// ����ʱ�䣺	2001/09/06
// ����޸�ʱ�䣺2003/03/10
// ��Ȩ��Ϣ��	Copyright (C) 1999 - 2003 PCSG
// ��ע��Store & Resend
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if !defined(AFX_MESSAGEQ_H__220ADFAE_D4C1_4E82_A7FB_0617B8418596__INCLUDED_)
#define AFX_MESSAGEQ_H__220ADFAE_D4C1_4E82_A7FB_0617B8418596__INCLUDED_

class CFastMessageQ  
{
public:
	int RemoveAllMessage();
	int RemoveMessage();
	int GetMessage(BYTE *f_data, size_t *f_len, DWORD *f_Tag = NULL);
	int AddMessage(const BYTE *f_data, size_t f_len, DWORD f_Tag = 0);			// ��Ϣ���
	int GetMQLength();
	int GetRepeatTimes();
	void SetRepeatTimes( int f_times = 0 );
	int GetMaxRepeatTimes();
	void SetMaxRepeatTimes( int f_times = 3 );
	int GetMQMaxLength();
	int ReclaimQueueLength(int f_iMaxLen, int f_iNodeSize = 1024);

	CFastMessageQ(int f_iMaxLen = 256, int f_iNodeSize = 1024);
	virtual ~CFastMessageQ();
	void SetMQName(LPCSTR strName);
	
	//jackgu added 2007-01-11
	void LockQueue();
	void UnlockQueue();

protected:
	class CFastMessageNode							// ��Ϣ�ڵ�
	{
	public:
		CFastMessageNode(int f_iSize);
		~CFastMessageNode();

		CFastMessageNode *m_pNext;					// ��һ���ڵ�
		CFastMessageNode *m_pPrev;					// ǰһ���ڵ�

		void WriteMessage(const BYTE *f_data, size_t f_len, DWORD f_Tag = 0);
		size_t ReadMessage(BYTE *f_data, DWORD *f_Tag = NULL);

	private:	
		BYTE *m_pData;							// ��Ϣ����
		size_t m_nLen;							// ��Ϣ����
		size_t m_nSize;							// �ڵ�����
		DWORD m_Tag;
	};

	int m_iRepeatTimes;							// ������Ϣʧ���ط�����
	int m_iMaxRepeatTimes;						// ������Ϣʧ������ط�����
	CFastMessageNode *m_pQHead;					// ����
	CFastMessageNode *m_pQTail;					// ��β
	int m_iQLength;								// ��Ϣ���г���
	int m_iMaxQLength;							// ��Ϣ������󳤶�
	int m_iNodeSize;							// ��Ϣ�ڵ��С
	
private:
	CManulSync m_bLock;
	CString m_strMQName;
};

#endif // !defined(AFX_MESSAGEQ_H__220ADFAE_D4C1_4E82_A7FB_0617B8418596__INCLUDED_)
