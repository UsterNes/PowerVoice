////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		FaxManager.cpp
// ��;������	������Դ����������ļ�
// ���ߣ�		SUN
// ����ʱ�䣺	2006/09/26
// ����޸�ʱ�䣺2006/10/01
// ��Ȩ��Ϣ��	Copyright (C) 2003 - 2006 PicaSoft (�������)
// ��ע��
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
#include "stdafx.h"
#include "FaxManager.h"

#include "_Global.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFaxManager::CFaxManager()
{
}

CFaxManager::~CFaxManager()
{
	RemoveAllFaxResource();
}

// ɾ��ȫ��������Դ
void CFaxManager::RemoveAllFaxResource()
{
	m_bLock.Enter();

	try
	{
		POSITION pos = GetStartPosition();
		long    nKey;
		CFaxResource *lv_pFaxRes;

		while( pos != NULL )
		{
			GetNextAssoc( pos, nKey, lv_pFaxRes );

			if( lv_pFaxRes )
			{
				delete lv_pFaxRes;
				lv_pFaxRes = NULL;
			}
		}
		
		RemoveAll();
	}
	catch(...)
	{
		WriteTrace(TraceError, "Error - CFaxManager::RemoveAllFaxResource(): Fax resource count = %d!", GetCount());
	}

	m_bLock.Leave();
}

// ���ش�����Դ����
int CFaxManager::GetFaxResourceCount()
{
	int lv_value;

	m_bLock.Enter();

	lv_value = GetCount();

	m_bLock.Leave();

	return lv_value;
}

// ��Ӵ�����Դ
CFaxResource *CFaxManager::AddFaxResource(Cchannelcontrol *f_pASR, long f_nFaxResID, long f_nFaxType)
{
	CFaxResource *lv_pFaxRes = NULL;

	m_bLock.Enter();

	try
	{
		if( !Lookup(f_nFaxResID, lv_pFaxRes) )
		{
			lv_pFaxRes = new CFaxResource(f_pASR, f_nFaxResID, f_nFaxType);
			if( lv_pFaxRes )
				SetAt(f_nFaxResID, lv_pFaxRes);
		}
	}
	catch(...)
	{
		lv_pFaxRes = NULL;
		WriteTrace(TraceError, "Error - CFaxManager::AddFaxResource(%d)!!!", f_nFaxResID);
	}

	m_bLock.Leave();

	return lv_pFaxRes;
}

// ɾ��������Դ
BOOL CFaxManager::RemoveFaxResource(long f_nFaxResID)
{
	BOOL lv_retval= FALSE;
	CFaxResource *lv_pFaxRes;

	m_bLock.Enter();

	try
	{
		if( Lookup(f_nFaxResID, lv_pFaxRes) )
		{
			if( RemoveKey(f_nFaxResID) )
			{
				delete(lv_pFaxRes);
				lv_retval = TRUE;
			}
		}
	}
	catch(...)
	{
		WriteTrace(TraceError, "Error - CFaxManager::RemoveFaxResource(%d)!!!", f_nFaxResID);
	}

	m_bLock.Leave();

	return lv_retval;
}

// ���䴫����Դ���ۼ�ʹ��ʱ����̵�
CFaxResource *CFaxManager::AllocateFaxResource(int f_intLineNo)
{
	CFaxResource *lv_pFoundFaxRes = NULL;

	m_bLock.Enter();

	try
	{
		CFaxResource *lv_pFaxRes;
		POSITION pos = GetStartPosition();
		long    nKey;
		
		while( pos != NULL )
		{
			GetNextAssoc( pos, nKey, lv_pFaxRes );

			if( lv_pFaxRes )
			{
				// �Ƿ����
				if( lv_pFaxRes->GetStatus() == IVR_IDLE )
				{
					if( lv_pFoundFaxRes == NULL )
						lv_pFoundFaxRes = lv_pFaxRes;
					else if( lv_pFoundFaxRes->GetTotalBusyTime() > lv_pFaxRes->GetTotalBusyTime() )
						lv_pFoundFaxRes = lv_pFaxRes;
				}
			}
		}

		// ռ�ô�����Դ
		if( lv_pFoundFaxRes )
			lv_pFoundFaxRes->SetStatus(IVR_W_FAX, f_intLineNo);
		else
			WriteTrace(TraceWarn, "Warn - IVR Channel: %d failed to allocate fax resource!!!", f_intLineNo);
	}
	catch(...)
	{
		WriteTrace(TraceError, "Error - CFaxManager::AllocateFaxResource(%d): Fax resource count = %d!", 
			f_intLineNo, GetCount());
	}

	m_bLock.Leave();

	return lv_pFoundFaxRes;
}

// �ͷŴ�����Դ
BOOL CFaxManager::ReleaseFaxResource(long f_nFaxResID)
{
	BOOL lv_retval= FALSE;
	CFaxResource *lv_pFaxRes;

	m_bLock.Enter();

	try
	{
		if( Lookup(f_nFaxResID, lv_pFaxRes) )
		{
			if( lv_pFaxRes )
				lv_pFaxRes->SetStatus(IVR_IDLE);
		}
	}
	catch(...)
	{
		WriteTrace(TraceError, "Error - CFaxManager::ReleaseFaxResource(%d)!!!", f_nFaxResID);
	}

	m_bLock.Leave();

	return lv_retval;
}

