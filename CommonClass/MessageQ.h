///////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		MessageQ.h
// 用途描述：	消息队列类声明
// 作者：		SUN
// 创建时间：	2001/09/06
// 最后修改时间：2003/03/10
// 版权信息：	Copyright (C) 1999 - 2003 PCSG
// 备注：Store & Resend
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
	int AddMessage(const BYTE *f_data, size_t f_len, DWORD f_Tag = 0);			// 消息入队
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
	class CFastMessageNode							// 消息节点
	{
	public:
		CFastMessageNode(int f_iSize);
		~CFastMessageNode();

		CFastMessageNode *m_pNext;					// 后一个节点
		CFastMessageNode *m_pPrev;					// 前一个节点

		void WriteMessage(const BYTE *f_data, size_t f_len, DWORD f_Tag = 0);
		size_t ReadMessage(BYTE *f_data, DWORD *f_Tag = NULL);

	private:	
		BYTE *m_pData;							// 消息数据
		size_t m_nLen;							// 消息长度
		size_t m_nSize;							// 节点容量
		DWORD m_Tag;
	};

	int m_iRepeatTimes;							// 单条消息失败重发次数
	int m_iMaxRepeatTimes;						// 单条消息失败最大重发次数
	CFastMessageNode *m_pQHead;					// 队首
	CFastMessageNode *m_pQTail;					// 队尾
	int m_iQLength;								// 消息队列长度
	int m_iMaxQLength;							// 消息队列最大长度
	int m_iNodeSize;							// 消息节点大小
	
private:
	CManulSync m_bLock;
	CString m_strMQName;
};

#endif // !defined(AFX_MESSAGEQ_H__220ADFAE_D4C1_4E82_A7FB_0617B8418596__INCLUDED_)
