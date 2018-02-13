////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		TTSInterface.cpp
// 用途描述：	TTS接口类实现文件
// 作者：		SUN
// 创建时间：	2008/03/10
// 最后修改时间：2008/04/20
// 版权信息：	Copyright (C) 2003 - 2008 PicaSoft (香港智联)
// 备注：
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
#include "stdafx.h"
#include ".\asrinterface.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BOOL CASRInterface::m_blnASRInit = FALSE;

CASRInterface::CASRInterface(void)
 : m_blnASREnabled(FALSE),
   m_strActiveGrammer(_T(""))
{
}

CASRInterface::~CASRInterface(void)
{
	UninitializeASR();
}

// 初始化ASR
BOOL CASRInterface::InitializeASR(LPCSTR strInitString, UINT f_nThreadNum)
{
	long lv_lngRet;

	// 初始化
	if( !m_blnASRInit )
	{
		lv_lngRet = jASR_Init(NULL);
		if( lv_lngRet != 0 )
			WriteTrace(TraceError, "jASR Error - Failed to InitializeASR error code = %d", lv_lngRet);
		else
			m_blnASRInit = TRUE;
	}

	return(m_blnASRInit);
}

// 反初始化ASR
BOOL CASRInterface::UninitializeASR()
{
	// jASR_CreateInstance()对应的quit
	QuitASRInstance();

	// jASR_ Init ()对应的quit
	if( m_blnASRInit )
	{
		m_blnASRInit = FALSE;
		jASR_Quit(2);
	}

	return TRUE;
}

// 创建ASR实例
BOOL CASRInterface::CreateASRInstance()
{
	long lv_lngRet;

	// 创建运行实例
	if( m_blnASRInit && (!m_blnASREnabled) )
	{
		lv_lngRet = jASR_CreateInstance();
		if( lv_lngRet != 0 )
		{
			jASR_Quit(1);
			WriteTrace(TraceError, "jASR Error - Failed to CreateInstance error code = %d", lv_lngRet);
		}
		// 激活使用的引擎
		else if( (lv_lngRet = jASR_ActiveEngine(DEF_ChEngEngine)) != 0 )
		{
			jASR_Quit(1);
			WriteTrace(TraceError, "jASR Error - Failed to ActiveEngine error code = %d", lv_lngRet);
		}
		else
		{
			m_blnASREnabled = TRUE;
			WriteTrace(TraceInfo, "jASR Info - ASR is initialized.");
		}
	}

	return(m_blnASREnabled);
}

// 退出ASR实例
BOOL CASRInterface::QuitASRInstance()
{
	// jASR_CreateInstance()对应的quit
	if( m_blnASREnabled )
	{
		m_blnASREnabled = FALSE;
		jASR_Quit(1);
	}

	return TRUE;
}

// ASR是否可用
BOOL CASRInterface::IsASREnabled()
{
	return m_blnASREnabled;
}

// 设置合成参数
BOOL CASRInterface::SetASRParameters(short* pParams)
{
	if( !IsASREnabled() )
	{
		WriteTrace(TraceWarn, "jASR Warn - Please initialize ASR Engine firstly.");
		return FALSE;
	}

	long lv_lngRet;

	lv_lngRet = jASR_SetParams(pParams);
	if( lv_lngRet != 0 )
	{
		WriteTrace(TraceError, "jASR Error - Failed to Set ASR Parameters error code = %d", lv_lngRet);
		return FALSE;
	}

	return TRUE;
}

// 加载语法文件
BOOL CASRInterface::LoadGrammerFile(LPCSTR strGrammerFiles, const int nFileCount)
{
	if( !IsASREnabled() )
	{
		WriteTrace(TraceWarn, "jASR Warn - Please initialize ASR Engine firstly.");
		return FALSE;
	}

	long lv_lngRet;

	if( nFileCount > 4 )
	{
		WriteTrace(TraceWarn, "jASR Warn - Failed to Load Grammar Files, file count(%d) is too big.", nFileCount);
		return FALSE;
	}

	// 加载多语法文件
	lv_lngRet = jASR_LoadMultiGrammar(strGrammerFiles, nFileCount);
	if( lv_lngRet != 0 )
	{
		WriteTrace(TraceError, "jASR Error - Failed to Load Grammar Files error code = %d", lv_lngRet);
		return FALSE;
	}

	m_strActiveGrammer = _T("");
	return TRUE;
}

// 激活语法
BOOL CASRInterface::SetActiveGrammer(LPCSTR strGrammerFile)
{
	if( !IsASREnabled() )
	{
		WriteTrace(TraceWarn, "jASR Warn - Please initialize ASR Engine firstly.");
		return FALSE;
	}

	long lv_lngRet;

	SetDeActiveGrammer();
	lv_lngRet = jASR_SetActiveMultiGramma(strGrammerFile, 1, 0, 1);
	if( lv_lngRet != 0 )
	{
		WriteTrace(TraceError, "jASR Error - Failed to Active Grammar File: %s, error code = %d", 
			strGrammerFile, lv_lngRet);
		return FALSE;
	}

	m_strActiveGrammer = strGrammerFile;
	return TRUE;
}

// 释放语法
BOOL CASRInterface::SetDeActiveGrammer()
{
	if( !IsASREnabled() )
	{
		WriteTrace(TraceWarn, "jASR Warn - Please initialize ASR Engine firstly.");
		return FALSE;
	}

	long lv_lngRet;

	if( m_strActiveGrammer.GetLength() > 0 )
	{
		lv_lngRet = jASR_SetDeActiveMultiGramma((LPCSTR)m_strActiveGrammer, 1);
		if( lv_lngRet != 0 )
		{
			WriteTrace(TraceError, "jASR Error - Failed to DeActive Grammar File: %s, error code = %d", 
				m_strActiveGrammer, lv_lngRet);
			return FALSE;
		}

		m_strActiveGrammer = _T("");
	}

	return TRUE;
}

// 进行语音识别
/// 返回值:
//// -1 --- 发生错误，不能继续识别
////  0 --- 调用成功，继续识别
////  1 --- 调用成功，识别完毕
long CASRInterface::RunASR(char* pWaveData, long DataLenth)
{
	if( !IsASREnabled() )
	{
		WriteTrace(TraceWarn, "jASR Warn - Please initialize ASR Engine firstly.");
		return -1;
	}

	long lv_lngRet;

	// 送入结束标志
	if( pWaveData == NULL )
	{
		lv_lngRet = jASR_RunASR(NULL, -1);
		if( lv_lngRet )
		{
			WriteTrace(TraceError, "jASR Error - Failed to RunASR(NULL), error code = %d", 
				lv_lngRet);
			return -1;
		}
	}
	else
	{
		lv_lngRet = jASR_RunASR(pWaveData, DataLenth);
		if( lv_lngRet )
		{
			WriteTrace(TraceError, "jASR Error - Failed to RunASR(), error code = %d, DataLength = %d", 
				lv_lngRet, DataLenth);
			return -1;
		}
	}

	if( lv_lngRet == DEF_RunStatus_Result )
		return 1;

	return 0;
}

// 获取识别结果
BOOL CASRInterface::GetASRResult(char *pResult, int *pPRI)
{
	if( !IsASREnabled() )
	{
		WriteTrace(TraceWarn, "jASR Warn - Please initialize ASR Engine firstly.");
		return FALSE;
	}

	try
	{
		memset(pResult, 0, sizeof(pResult));
		// 获取识别结果和信心值
		if( jASR_GetResult(pResult, pPRI) )
			return TRUE;
	}
	catch(...)
	{
		WriteTrace(TraceError, "jASR Error - GetResult() catched error!");
	}

	return FALSE;
}