////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		TTSInterface.cpp
// ��;������	TTS�ӿ���ʵ���ļ�
// ���ߣ�		SUN
// ����ʱ�䣺	2008/03/10
// ����޸�ʱ�䣺2008/04/20
// ��Ȩ��Ϣ��	Copyright (C) 2003 - 2008 PicaSoft (�������)
// ��ע��
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

// ��ʼ��ASR
BOOL CASRInterface::InitializeASR(LPCSTR strInitString, UINT f_nThreadNum)
{
	long lv_lngRet;

	// ��ʼ��
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

// ����ʼ��ASR
BOOL CASRInterface::UninitializeASR()
{
	// jASR_CreateInstance()��Ӧ��quit
	QuitASRInstance();

	// jASR_ Init ()��Ӧ��quit
	if( m_blnASRInit )
	{
		m_blnASRInit = FALSE;
		jASR_Quit(2);
	}

	return TRUE;
}

// ����ASRʵ��
BOOL CASRInterface::CreateASRInstance()
{
	long lv_lngRet;

	// ��������ʵ��
	if( m_blnASRInit && (!m_blnASREnabled) )
	{
		lv_lngRet = jASR_CreateInstance();
		if( lv_lngRet != 0 )
		{
			jASR_Quit(1);
			WriteTrace(TraceError, "jASR Error - Failed to CreateInstance error code = %d", lv_lngRet);
		}
		// ����ʹ�õ�����
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

// �˳�ASRʵ��
BOOL CASRInterface::QuitASRInstance()
{
	// jASR_CreateInstance()��Ӧ��quit
	if( m_blnASREnabled )
	{
		m_blnASREnabled = FALSE;
		jASR_Quit(1);
	}

	return TRUE;
}

// ASR�Ƿ����
BOOL CASRInterface::IsASREnabled()
{
	return m_blnASREnabled;
}

// ���úϳɲ���
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

// �����﷨�ļ�
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

	// ���ض��﷨�ļ�
	lv_lngRet = jASR_LoadMultiGrammar(strGrammerFiles, nFileCount);
	if( lv_lngRet != 0 )
	{
		WriteTrace(TraceError, "jASR Error - Failed to Load Grammar Files error code = %d", lv_lngRet);
		return FALSE;
	}

	m_strActiveGrammer = _T("");
	return TRUE;
}

// �����﷨
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

// �ͷ��﷨
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

// ��������ʶ��
/// ����ֵ:
//// -1 --- �������󣬲��ܼ���ʶ��
////  0 --- ���óɹ�������ʶ��
////  1 --- ���óɹ���ʶ�����
long CASRInterface::RunASR(char* pWaveData, long DataLenth)
{
	if( !IsASREnabled() )
	{
		WriteTrace(TraceWarn, "jASR Warn - Please initialize ASR Engine firstly.");
		return -1;
	}

	long lv_lngRet;

	// ���������־
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

// ��ȡʶ����
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
		// ��ȡʶ����������ֵ
		if( jASR_GetResult(pResult, pPRI) )
			return TRUE;
	}
	catch(...)
	{
		WriteTrace(TraceError, "jASR Error - GetResult() catched error!");
	}

	return FALSE;
}