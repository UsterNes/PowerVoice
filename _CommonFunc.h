////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		_CommonFunc.h
// 用途描述：	公共函数声明
// 作者：		SUN
// 创建时间：	2001/07/20
// 最后修改时间：2001/07/20
// 版权信息：	Copyright (C) 1999 - 2000 Changjiang Xincheng
// 备注：
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//------------------------------------------------------------------------
#ifndef __COMMONFUNC_H_
#define __COMMONFUNC_H_
//------------------------------------------------------------------------

#include "stdafx.h" 

// 自定义数据类型
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