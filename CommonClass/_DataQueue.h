#ifndef PCS_DATAQUEUE_INCLUDED_
#define PCS_DATAQUEUE_INCLUDED_

class CDataStack
{
	char*		m_buffer;
	long		m_length;
	long		m_mark;
	CManulSync	m_sync;

public:
	CDataStack ();
	~CDataStack ();

	void Append (const char* data, long len);
	long Remove (char* data, long len);
	long Length ();
	void ClearBuffer();
	void SetMark();
	long GetMark();
};

class CDataQueue
{
	BYTE		*m_pBuffer;
	CManulSync	m_sync;
	long		m_length;
	long		m_maxlen;

	void		CreateDataBuffer(long f_nlen);

public:
	CDataQueue (long f_maxlen);
	CDataQueue ();
	~CDataQueue ();

	long GetMaxLength();
	void SetMaxLength(long f_maxlen);
	long Append (const BYTE* data, long len);
	long Remove (long f_first = 0, BYTE* data = NULL);
	long Length ();
	void ClearBuffer();
	BYTE *GetBuffer();
	void LockBuffer();
	void UnLockBuffer();
};

#endif // PCS_DATAQUEUE_INCLUDED_

///////////////////////////////////////////////////////////////////////////////
// End of file
///////////////////////////////////////////////////////////////////////////////
