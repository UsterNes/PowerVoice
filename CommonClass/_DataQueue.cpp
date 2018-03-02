#include "stdafx.h"
#include "_DataQueue.h"

CDataStack::CDataStack ()
{
	m_buffer = NULL;
	m_length = 0;
	m_mark = 0;
}

CDataStack::~CDataStack ()
{
	ClearBuffer();
}

void CDataStack::Append (const char* data, long len)
{
	if (data == NULL || len <= 0) return;

	m_sync.Enter ();
	if (m_length == 0)
	{
		m_buffer = new char[len];
		memcpy(m_buffer, data, len);
		m_length = len;
	}
	else
	{
		//	backup old buffer (temporarily)
		char* oldbuff = new char[m_length];
		memcpy(oldbuff, m_buffer, m_length);

		//	create a new buffer that holds the old one PLUS the new stuff
		delete[] m_buffer;
		m_buffer = new char[m_length + len];

		//	copy old buffer and new data into the new (bigger) buffer
		memcpy(m_buffer, oldbuff, m_length);
		memcpy(&m_buffer[m_length], data, len);

		//	store the new length of the data
		m_length += len;

		//	do clean-up
		delete[] oldbuff;
	}
	m_sync.Leave ();
}

long CDataStack::Remove (char* data, long len)
{
	memset(data, 0, len);

	m_sync.Enter ();
	//	if the buffer is empty, don't remove anything
	if (m_length == 0)
	{
		m_sync.Leave ();
		return 0;
	}

	//	determine how much we will actually copy
	long remlen = len;
	if (remlen > m_length)
		remlen = m_length;

	//	backup old buffer (temporarily)
	char* oldbuff = new char[m_length];
	memcpy(oldbuff, m_buffer, m_length);

	//	copy part of buffer (or all) to 'data'
	memcpy(data, oldbuff, remlen);

	//	create a new buffer that holds the old one MINUS the removed stuff
	delete[] m_buffer;
	if (m_length - remlen <= 0)
	{
		m_buffer = NULL;
		m_length = 0;
	}
	else
	{
		m_buffer = new char[m_length - remlen];

		//	remove the part that has been copied from the buffer
		memcpy(m_buffer, &oldbuff[remlen], m_length - remlen);

		//	store the new length of the data
		m_length -= remlen;
	}

	//	do clean-up
	delete[] oldbuff;

	m_sync.Leave ();
	return remlen;
}

long CDataStack::Length ()
{
	long length;

	m_sync.Enter ();
	length = m_length;
	m_sync.Leave ();

	return length;
}

void CDataStack::ClearBuffer()
{
	m_sync.Enter ();
	if( m_buffer )
	{
		delete[] m_buffer;
		m_buffer = NULL;
	}
	m_length = 0;
	m_sync.Leave ();
}

void CDataStack::SetMark()
{
	m_sync.Enter ();
	m_mark = m_length;
	m_sync.Leave ();
}

long CDataStack::GetMark()
{
	long lv_mark;

	m_sync.Enter ();
	lv_mark = m_mark;
	m_sync.Leave ();

	return lv_mark;
}

CDataQueue::CDataQueue (long f_maxlen)
{
	m_pBuffer = NULL;
	m_length = 0;
	m_maxlen = 0;
	CreateDataBuffer(f_maxlen);
}

CDataQueue::CDataQueue ()
{
	m_pBuffer = NULL;
	m_length = 0;
	m_maxlen = 0;
}

CDataQueue::~CDataQueue ()
{
	ClearBuffer();
}

void CDataQueue::CreateDataBuffer(long f_nlen)
{
	if( f_nlen > 0 )
	{
		m_sync.Enter ();
		if( m_pBuffer )
			delete[] m_pBuffer;
		m_pBuffer = new BYTE[f_nlen];
		m_length = 0;
		m_maxlen = f_nlen;
		m_sync.Leave ();
	}
}

long CDataQueue::GetMaxLength()
{
	long lv_Value;

	m_sync.Enter ();
	lv_Value = m_maxlen;
	m_sync.Leave ();

	return lv_Value;
}

void CDataQueue::SetMaxLength(long f_maxlen)
{
	CreateDataBuffer(f_maxlen);
}

long CDataQueue::Append (const BYTE* data, long len)
{
	if (data == NULL || len <= 0 ) return 0;

	long lv_retval;

	m_sync.Enter ();
	if( m_length + len > m_maxlen )
		lv_retval = -1;
	else
	{
		memcpy(m_pBuffer + m_length, data, len);
		m_length += len;
		lv_retval = len;
	}
	m_sync.Leave ();

	return lv_retval;
}

long CDataQueue::Remove (long f_first, BYTE* data)
{
	long length = 0, copylen;

	m_sync.Enter ();

	copylen = ( f_first > 0 ? f_first : m_length );
	if( copylen > 0 )
	{
		if( data != NULL )
		{
			try
			{
				memcpy(data, m_pBuffer, copylen);
				length = copylen;
			}
			catch(...)
			{
				length = -1;
			}
		}
		m_length -= copylen;
		if( m_length )
			memmove(m_pBuffer, m_pBuffer + copylen, m_length);
	}
	m_sync.Leave ();

	return length;
}

long CDataQueue::Length ()
{
	long length;

	m_sync.Enter ();
	length = m_length;
	m_sync.Leave ();

	return length;
}

void CDataQueue::ClearBuffer()
{
	m_sync.Enter ();
	if( m_pBuffer )
	{
		delete[] m_pBuffer;
		m_pBuffer = NULL;
	}
	m_length = 0;
	m_maxlen = 0;
	m_sync.Leave ();
}

BYTE *CDataQueue::GetBuffer()
{
	return m_pBuffer;
}

void CDataQueue::LockBuffer()
{
	m_sync.Enter ();
}

void CDataQueue::UnLockBuffer()
{
	m_sync.Leave ();
}
