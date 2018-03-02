// SharedMem.h: interface for the CSharedMem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHAREDMEM_H__D5CF78A2_E597_45F2_A39C_BF04600EA990__INCLUDED_)
#define AFX_SHAREDMEM_H__D5CF78A2_E597_45F2_A39C_BF04600EA990__INCLUDED_

//TSharedMem
class ATL_NO_VTABLE CSharedMem
{
private:
	TCHAR m_Name[_MAX_PATH];
	int m_nSize;
	bool m_bCreated;
	void *m_pFileView;

public:
	CSharedMem();
	virtual ~CSharedMem(void);

	HRESULT Create(LPOLESTR Name, int Size);

	TCHAR *GetName() { return m_Name; }
	int GetSize() { return m_nSize; }
	void* GetPointor() { return m_pFileView; }
	bool GetCreated() { return m_bCreated; }
};

#endif // !defined(AFX_SHAREDMEM_H__D5CF78A2_E597_45F2_A39C_BF04600EA990__INCLUDED_)
