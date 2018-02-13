// ASRInterface.h: interface for the CASRInterface class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_ASRINTERFACE_H__E8C44467_06F6_4C21_9000_49E33BBBF2F__INCLUDED_)
#define AFX_ASRINTERFACE_H__E8C44467_06F6_4C21_9000_49E33BBBF2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include ".\jASR\Inc\dll_JASR_API.h"

class CASRInterface
{
public:
	CASRInterface(void);
	virtual ~CASRInterface(void);

	// ��ʼ��ASR
	BOOL InitializeASR(LPCSTR strInitString, UINT f_nThreadNum);
	// ����ʼ��ASR
	BOOL UninitializeASR();
	// ����ASRʵ��
	BOOL CreateASRInstance();
	// �˳�ASRʵ��
	BOOL QuitASRInstance();
	// ASR�Ƿ����
	BOOL IsASREnabled();

	// ���úϳɲ���
	BOOL SetASRParameters(short* pParams);

	// �����﷨�ļ�
	BOOL LoadGrammerFile(LPCSTR strGrammerFiles, const int nFileCount = 1);

	// �����﷨
	BOOL SetActiveGrammer(LPCSTR strGrammerFile);
	// �ͷ��﷨
	BOOL SetDeActiveGrammer();

	// ��������ʶ��
	long RunASR(char* pWaveData, long DataLenth);
	// ��ȡʶ����
	BOOL GetASRResult(char *pResult, int *pPRI);

protected:
	static BOOL		m_blnASRInit;
	BOOL			m_blnASREnabled;
	CString			m_strActiveGrammer;
};

#endif // !defined(AFX_ASRINTERFACE_H__E8C44467_06F6_4C21_9000_49E33BBBF2F__INCLUDED_)