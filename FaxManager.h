// FaxManager.h: interface for the CFaxManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FAXMANAGER_H__9C826197_657F_4F35_B719_63E00D4F2B09__INCLUDED_)
#define AFX_FAXMANAGER_H__9C826197_657F_4F35_B719_63E00D4F2B09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "FaxResource.h"

class CFaxManager : protected CMap<long, long, CFaxResource *, CFaxResource *> 
{
public:
	CFaxManager();
	virtual ~CFaxManager();

	// ���ش�����Դ����
	int GetFaxResourceCount();

	// ��Ӵ�����Դ
	CFaxResource *AddFaxResource(Cchannelcontrol *f_pASR, long f_nFaxResID, long f_nFaxType);
	// ɾ��������Դ
	BOOL RemoveFaxResource(long f_nFaxResID);
	// ɾ��ȫ��������Դ
	void RemoveAllFaxResource();

	// ���䴫����Դ
	CFaxResource *AllocateFaxResource(int f_intLineNo);
	// �ͷŴ�����Դ
	BOOL ReleaseFaxResource(long f_nFaxResID);

private:
	CManulSync		m_bLock;
};

#endif // !defined(AFX_FAXMANAGER_H__9C826197_657F_4F35_B719_63E00D4F2B09__INCLUDED_)
