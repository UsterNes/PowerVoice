///////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		MessageQ.cpp
// 用途描述：	消息队列类实现
// 作者：		SUN
// 创建时间：	2001/09/06
// 最后修改时间：2003/03/10
// 版权信息：	Copyright (C) 1999 - 2003 PCSG
// 备注：Store & Resend
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
#include "stdafx.h"
#include "MessageQ.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
// Fast Message Queue
////////////////////////////////////////////////////////////////
CFastMessageQ::CFastMessageQ(int f_iMaxLen, int f_iNodeSize)
	: m_strMQName(_T("")),
	  m_iQLength(0),
	  m_iRepeatTimes(0),
	  m_iMaxRepeatTimes(3),
	  m_pQHead(NULL),
	  m_pQTail(NULL)
{
	// Get maxium length
	if( f_iMaxLen <= 0 )
		m_iMaxQLength = 256;
	else
		m_iMaxQLength = f_iMaxLen;

	// Get node size
	if( f_iNodeSize <= 0 )
		m_iNodeSize = 1024;
	else
		m_iNodeSize = f_iNodeSize;

	// Create queue
	CFastMessageNode *lv_new, *lv_old = NULL;
	for( int lv_loop = 0; lv_loop < m_iMaxQLength; lv_loop++ )
	{
		lv_new = new CFastMessageNode(f_iNodeSize);
		if( lv_new == NULL )
		{
			m_iMaxQLength = lv_loop;
			break;
		}

		/// Link Nodes
		if( lv_old != NULL )
		{
			lv_old->m_pNext = lv_new;
			lv_new->m_pPrev = lv_old;
		}
		else
			m_pQHead = m_pQTail = lv_new;
		lv_old = lv_new;
	}

	if( m_pQHead )
	{
		m_pQHead->m_pPrev = lv_old;
		lv_old->m_pNext = m_pQHead;
	}
}

CFastMessageQ::~CFastMessageQ()
{
	CFastMessageNode *lv_pNode;
	for( int lv_loop = 0; lv_loop < m_iMaxQLength; lv_loop++ )
	{
		lv_pNode = m_pQHead;
		m_pQHead = m_pQHead->m_pNext;
		delete(lv_pNode);
	}
}

void CFastMessageQ::SetMQName(LPCSTR strName)
{
	if( m_strMQName != strName )
		m_strMQName = strName;
}

int CFastMessageQ::ReclaimQueueLength(int f_iMaxLen, int f_iNodeSize)
{
	int lv_nOldLength, lv_loop;
	CFastMessageNode *lv_pNode;
	CFastMessageNode *lv_new, *lv_old = NULL;

	// Lock Queue
	m_bLock.Enter();

	// Get maxium length
	if( m_iMaxQLength != f_iMaxLen )
	{
		if( f_iMaxLen > 0 )
		{
			try
			{
				lv_nOldLength = m_iMaxQLength;
				m_iMaxQLength = f_iMaxLen;

				// Deallocate Queue Buffer
				for( lv_loop = 0; lv_loop < lv_nOldLength; lv_loop++ )
				{
					lv_pNode = m_pQHead;
					m_pQHead = m_pQHead->m_pNext;
					delete(lv_pNode);
				}

				// Create queue
				for( lv_loop = 0; lv_loop < m_iMaxQLength; lv_loop++ )
				{
					lv_new = new CFastMessageNode(f_iNodeSize);
					if( lv_new == NULL )
					{
						m_iMaxQLength = lv_loop;
						break;
					}

					/// Link Nodes
					if( lv_old != NULL )
					{
						lv_old->m_pNext = lv_new;
						lv_new->m_pPrev = lv_old;
					}
					else
						m_pQHead = m_pQTail = lv_new;
					lv_old = lv_new;
				}

				if( m_pQHead )
				{
					m_pQHead->m_pPrev = lv_old;
					lv_old->m_pNext = m_pQHead;
				}

				m_iQLength = 0;
				WriteTrace(TraceInfo, "Message Queue [%s] has been reclaimed length from [%d] to [%d]", 
					m_strMQName, lv_nOldLength, m_iMaxQLength);
			}
			catch(...)
			{
				WriteTrace(TraceError, "Error: CFastMessageQ::ReclaimQueueLength(%d), Queue Name: %s!!!", 
					f_iMaxLen, m_strMQName);
			}
		}
	}

	// Unlock
	m_bLock.Leave();

	return 1;
}

CFastMessageQ::CFastMessageNode::CFastMessageNode(int f_iSize)
{
	m_pData = new BYTE[f_iSize];
	if( m_pData )
	{
		m_nSize = f_iSize;
		memset(m_pData, 0x00, m_nSize);
	}
	else
		m_nSize = 0;

	m_nLen = 0;
	m_pPrev = NULL;
	m_pNext = NULL;
	m_Tag = 0;
}

CFastMessageQ::CFastMessageNode::~CFastMessageNode()
{
	if( m_pData )
		delete[] m_pData;
}

void CFastMessageQ::CFastMessageNode::WriteMessage(const BYTE *f_data, size_t f_len, DWORD f_Tag)
{
	try
	{
		int lv_len = __min(f_len, m_nSize);
		if( lv_len > 0 )
			memcpy( m_pData, f_data, lv_len );
		m_nLen = lv_len;
		m_Tag = f_Tag;
	}
	catch(...)
	{
		WriteTrace(TraceError, "Error: CFastMessageNode::WriteMessage() - Source message buffer length can not beyond %d bytes!!!", f_len);
	}
}

size_t CFastMessageQ::CFastMessageNode::ReadMessage(BYTE *f_data, DWORD *f_Tag)
{
	try
	{
		if( f_Tag )
			*f_Tag = m_Tag;
		if( m_nLen > 0 )
			memcpy(f_data, m_pData, m_nLen);
	}
	catch(...)
	{
		m_nLen = 0;
		WriteTrace(TraceError, "Error: CFastMessageNode::ReadMessage() - Target message buffer length must beyond %d bytes!!!", m_nLen);
	}

	return m_nLen;
}

int CFastMessageQ::GetMQLength()
{
	int lv_value;

	// Lock Queue
	m_bLock.Enter();

	lv_value = m_iQLength;

	// Unlock
	m_bLock.Leave();

	return lv_value;
}

int CFastMessageQ::GetRepeatTimes()
{
	int lv_value;

	// Lock Queue
	m_bLock.Enter();

	lv_value = m_iRepeatTimes;

	// Unlock
	m_bLock.Leave();

	return lv_value;
}

void CFastMessageQ::SetRepeatTimes( int f_times )
{
	// Lock Queue
	m_bLock.Enter();

	m_iRepeatTimes = f_times;

	// Unlock
	m_bLock.Leave();
}

int CFastMessageQ::GetMaxRepeatTimes()
{
	int lv_value;

	// Lock Queue
	m_bLock.Enter();

	lv_value = m_iMaxRepeatTimes;

	// Unlock
	m_bLock.Leave();

	return lv_value;
}

void CFastMessageQ::SetMaxRepeatTimes( int f_times )
{
	// Lock Queue
	m_bLock.Enter();

	m_iMaxRepeatTimes = f_times;

	// Unlock
	m_bLock.Leave();
}

int CFastMessageQ::GetMQMaxLength()
{
	int lv_value;

	// Lock Queue
	m_bLock.Enter();

	lv_value = m_iMaxQLength;

	// Unlock
	m_bLock.Leave();

	return lv_value;
}

// 消息入队
int CFastMessageQ::AddMessage(const BYTE *f_data, size_t f_len, DWORD f_Tag)
{
	int lv_retVal;

	// Lock Queue
	m_bLock.Enter();

	if( m_iQLength < m_iMaxQLength )
	{
		// Set Data
		m_pQTail->WriteMessage(f_data, f_len, f_Tag);
		m_pQTail = m_pQTail->m_pNext;
		m_iQLength++;
		lv_retVal = m_iQLength;
	}
	else
	{
		// Messgae Queue is full
		WriteTrace(TraceWarn, "Fast Message Queue [%s] is full!!!", m_strMQName);
		lv_retVal = 0;
	}

	// Unlock
	m_bLock.Leave();

	return lv_retVal;
}

// 读出一条消息
int CFastMessageQ::GetMessage(BYTE *f_data, size_t *f_len, DWORD *f_Tag)
{
	int lv_retVal;

	// Lock Queue
	m_bLock.Enter();

	try
	{
		lv_retVal = m_iQLength;
		if( lv_retVal > 0 )
		{
			// Get message body
			*f_len = m_pQHead->ReadMessage(f_data, f_Tag);
			m_iRepeatTimes++;
		}
	}
	catch(...)
	{
	}

	// Unlock
	m_bLock.Leave();
	
	return lv_retVal;
}

// 消息出队
int CFastMessageQ::RemoveMessage()
{
	int lv_retVal;

	// Lock Queue
	m_bLock.Enter();

	try
	{
		lv_retVal = m_iQLength;
		if( lv_retVal > 0 )
		{
			// Get message body
			m_pQHead = m_pQHead->m_pNext;
			m_iQLength--;
			m_iRepeatTimes = 0;
		}
	}
	catch(...)
	{
	}

	// Unlock
	m_bLock.Leave();
	
	return lv_retVal;
}

// 删除队列中的所有消息
int CFastMessageQ::RemoveAllMessage()
{
	// Lock Queue
	m_bLock.Enter();

	m_iQLength = 0;
	m_iRepeatTimes = 0;
	m_pQHead = m_pQTail;

	// Unlock
	m_bLock.Leave();

	return 1;
}

//jackgu added 2007-01-11
//lock queue
void CFastMessageQ::LockQueue()
{
	m_bLock.Enter();	
}

//jackgu added 2007-01-11
//unlock
void CFastMessageQ::UnlockQueue()
{
	m_bLock.Leave();
}