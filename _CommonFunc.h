////////////////////////////////////////////////////////////////////////////////////////////////////////
// �ļ�����		_CommonFunc.h
// ��;������	������������
// ���ߣ�		SUN
// ����ʱ�䣺	2001/07/20
// ����޸�ʱ�䣺2001/07/20
// ��Ȩ��Ϣ��	Copyright (C) 1999 - 2000 Changjiang Xincheng
// ��ע��
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//------------------------------------------------------------------------
#ifndef __COMMONFUNC_H_
#define __COMMONFUNC_H_
//------------------------------------------------------------------------

#include "stdafx.h" 

// �Զ�����������
#ifndef US
	#define US unsigned short int
#endif
#ifndef UC
	#define UC unsigned char
#endif

//---------------------------------------------------------------------
void SetOneBit( UC *f_byte, UC f_bit,  bool f_val );
bool GetOneBit( UC f_byte, UC f_bit );
char *strRTrim(char *strDest);
char *strDos2C_Path(const char *strSource, char *strDest);
char *strC2Dos_Path(const char *strSource, char *strDest);
char *strBSTRCpy(char * strDest, BSTR bstrSource, int length);
char *StringToSQL(LPCSTR lpszString, LPSTR strResult, int nMaxLen = 0);

int GetDigitMask(char chDigit);

//------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------