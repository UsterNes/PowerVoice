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

	// 初始化ASR
	BOOL InitializeASR(LPCSTR strInitString, UINT f_nThreadNum);
	// 反初始化ASR
	BOOL UninitializeASR();
	// 创建ASR实例
	BOOL CreateASRInstance();
	// 退出ASR实例
	BOOL QuitASRInstance();
	// ASR是否可用
	BOOL IsASREnabled();

	// 设置合成参数
	BOOL SetASRParameters(short* pParams);

	// 加载语法文件
	BOOL LoadGrammerFile(LPCSTR strGrammerFiles, const int nFileCount = 1);

	// 激活语法
	BOOL SetActiveGrammer(LPCSTR strGrammerFile);
	// 释放语法
	BOOL SetDeActiveGrammer();

	// 进行语音识别
	long RunASR(char* pWaveData, long DataLenth);
	// 获取识别结果
	BOOL GetASRResult(char *pResult, int *pPRI);

protected:
	static BOOL		m_blnASRInit;
	BOOL			m_blnASREnabled;
	CString			m_strActiveGrammer;
};

#endif // !defined(AFX_ASRINTERFACE_H__E8C44467_06F6_4C21_9000_49E33BBBF2F__INCLUDED_)