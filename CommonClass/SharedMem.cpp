// SharedMem.cpp: implementation of the CSharedMem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SharedMem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/* This class simplifies the process of creating a region of shared memory.
  In Win32, this is accomplished by using the CreateFileMapping and
  MapViewOfFile functions. */
CSharedMem::CSharedMem()
   :  m_pFileView(NULL),
	  m_bCreated(false)
{
}

CSharedMem::~CSharedMem()
{
	if( m_pFileView != NULL )
		UnmapViewOfFile(m_pFileView);
}

HRESULT CSharedMem::Create(LPOLESTR Name, int Size)
{
    wsprintf(m_Name, _T("%ls"), Name);
    m_nSize = Size;

    /* CreateFileMapping, when called with $FFFFFFFF for the handle value,
      creates a region of shared memory. If an object with same name already
      exists, then a Handle to that object will be returned. */
    HANDLE FHandle = CreateFileMapping((HANDLE)0xFFFFFFFF,
                                NULL,
                                PAGE_READWRITE,
                                0,
                                m_nSize,
                                m_Name);
    if( FHandle == 0 ) 
		return -1;

    m_bCreated = (GetLastError == 0);

    /* We still need to map a pointer to the handle of the shared memory region */
    m_pFileView = MapViewOfFile(FHandle,
                              FILE_MAP_WRITE,
                              0, 0, m_nSize);
    if( m_pFileView == NULL )
		return -2;

	m_bCreated = true;
	return S_OK;
}
