// ManulCallCom.h: interface for the CManulCallCom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MANULCALLCOM_H__35064A72_9746_4AFE_B549_D9A89D051369__INCLUDED_)
#define AFX_MANULCALLCOM_H__35064A72_9746_4AFE_B549_D9A89D051369__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CManulCallCom  
{
public:
	CManulCallCom();
	virtual ~CManulCallCom();

	HRESULT CreateObjectEx(LPOLESTR pszProgID);
	HRESULT Invoke(WORD wFlags, LPVARIANT pvRet, EXCEPINFO FAR* pexcepinfo, UINT FAR* pnArgErr, LPOLESTR pszName, LPCTSTR pszFmt, ...);
	BOOL IsCreateObject();
	void ClearDispatchObject();

private:
	HRESULT CountArgsInFormat(LPCTSTR pszFmt, UINT FAR *pn);
	LPCTSTR GetNextVarType(LPCTSTR pszFmt, VARTYPE FAR* pvt);

	CManulSync		m_sync;
	LPDISPATCH		m_pDispatch;					// 用户COM调用分派对象
};

#endif // !defined(AFX_MANULCALLCOM_H__35064A72_9746_4AFE_B549_D9A89D051369__INCLUDED_)
