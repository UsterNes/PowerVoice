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

	// 返回传真资源总数
	int GetFaxResourceCount();

	// 添加传真资源
	CFaxResource *AddFaxResource(Cchannelcontrol *f_pASR, long f_nFaxResID, long f_nFaxType);
	// 删除传真资源
	BOOL RemoveFaxResource(long f_nFaxResID);
	// 删除全部传真资源
	void RemoveAllFaxResource();

	// 分配传真资源
	CFaxResource *AllocateFaxResource(int f_intLineNo);
	// 释放传真资源
	BOOL ReleaseFaxResource(long f_nFaxResID);

private:
	CManulSync		m_bLock;
};

#endif // !defined(AFX_FAXMANAGER_H__9C826197_657F_4F35_B719_63E00D4F2B09__INCLUDED_)
