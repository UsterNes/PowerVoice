///////////////////////////////////////////////////////////////////////////////////////////////////////
// 文件名：		_CommonFunc.cpp
// 用途描述：	公共函数定义
// 作者：		SUN
// 创建时间：	2001/07/20
// 最后修改时间：2001/07/20
// 版权信息：	Copyright (C) 1999 - 2000 Changjiang Xincheng
// 备注：
///////////////////////////////////////////////////////////////////////////////////////////////////////
//
#include "stdafx.h"
#include "_CommonFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Bit operation
// Set one bit
void SetOneBit( UC *f_byte, UC f_bit,  bool f_val )
{
	UC lv_byte = 0x01;

	if(f_bit >=8)
		return;

	lv_byte = lv_byte << f_bit;
	if(f_val)
		*f_byte |= lv_byte;
	else
		*f_byte &= (~lv_byte);
}

/// Get one bit
bool GetOneBit( UC f_byte, UC f_bit )
{
	if(f_bit >=8)
		return(false);

	return((f_byte >> f_bit) & 0x01);
}

// Get rid of illegal characters
char *strRTrim(char *strDest)
{
	int lv_loop;
	int lv_len;

	lv_len = strlen(strDest);
	for(lv_loop=0;lv_loop<lv_len;lv_loop++)
	{
		if(strDest[lv_loop] <= 32 || strDest[lv_loop] > 127)
			break;
	}
	strDest[lv_loop] = '\0';
	return strDest;
}

// Convert all '\' in string to '\\'
char *strDos2C_Path(const char *strSource, char *strDest)
{
	int lv_loop;
	int lv_ptr=0;
	int lv_len;
	bool lv_pro1 = false;
	bool lv_pro2 = false;

	lv_len = strlen(strSource);
	for(lv_loop=0;lv_loop<lv_len;lv_loop++)
	{
		if(strSource[lv_loop] != '\\')
		{
			if(!lv_pro1 && lv_pro2)
			{
				strDest[lv_ptr++] = '\\';
							
				if(lv_ptr >= 100)
					break;
			}
			lv_pro1 = lv_pro2;
			lv_pro2 = false;
		}
		else 
		{ 
			lv_pro1 = lv_pro2;
			lv_pro2 = true;
		}
		strDest[lv_ptr++] = strSource[lv_loop];
		
		if(lv_ptr >= 100)
			break;
	}

	return strDest;
}

// Convert all '\\' in string to '\'
char *strC2Dos_Path(const char *strSource, char *strDest)
{
	int lv_loop;
	int lv_ptr=0;
	int lv_len;
	bool lv_pro1 = false;

	lv_len = strlen(strSource);
	for(lv_loop=0;lv_loop<lv_len;lv_loop++)
	{
		if(strSource[lv_loop] == '\\')
		{
			if(!lv_pro1)
			{
				lv_pro1 = true;
				continue;
			}
			lv_pro1 = true;
		}
		else 
			lv_pro1 = false;
		
		strDest[lv_ptr++] = strSource[lv_loop];
		
		if(lv_ptr >= 100)
			break;
	}

	return strDest;
}

// Copy a BSTR to string
char *strBSTRCpy(char * strDest, BSTR bstrSource, int length)
{
	int lv_loop;
	_bstr_t lv_bstr = bstrSource;
	
	try
	{
		for(lv_loop = 0; lv_loop < length; lv_loop++)
		{
			strDest[lv_loop] = (char)bstrSource[lv_loop];
			if( strDest[lv_loop] == NULL )
				break;
		}
	}
	catch(...)
	{
	}

	return strDest;
}

// Change to IP format ((UC *)xxx.xxx.xxx.xxx)
// 1. (char */ UC *)xxx.xxx.xxx.xxx
// 2. (char *)x x x . x x x . x x x . x x x 
void strIPCpy(UC strTarget[], const UC strSource[])
{
	int lv_loop;
	int lv_subloop = 0;
	
	for(lv_loop = 0; lv_loop < 30; lv_loop++)
	{
		if( strSource[lv_loop] != 0 )
		{
			strTarget[lv_subloop++] = strSource[lv_loop];
			if(lv_subloop >= 15)
				break;
		}
		else
		{
			strTarget[lv_subloop] = 0;
			return;
		}
	}

	strTarget[15] = 0;
}

// 处理SQL中含单引号的字符串，将一个单引号用两个单引号替换
char *StringToSQL(LPCSTR lpszString, LPSTR strResult, int nMaxLen)
{
	int lv_nLen, lv_loop, lv_nTotal;
	
	// 将一个单引号用两个单引号替换
	lv_nLen = 0;
	lv_nTotal = strlen(lpszString);
	for( lv_loop = 0; lv_loop < lv_nTotal; lv_loop++ )
	{
		if( lpszString[lv_loop] == 39 )
		{
			if( lv_nLen >= nMaxLen - 1 )
				break;
			strResult[lv_nLen++] = 39;
		}
		if( lv_nLen >= nMaxLen )
			break;
		strResult[lv_nLen++] = lpszString[lv_loop];
	}
	strResult[lv_nLen] = 0;

	return(strResult);
}

// 返回按键的屏蔽码
int GetDigitMask(char chDigit)
{
	int digMask = 0;

	if( chDigit == '\0' )
		return 0;

	switch( chDigit )
	{
	case 'd':
	case 'D':
		digMask |= DM_D;   // 0000.0000.0000.0001
		break;
	case '1':
		digMask |= DM_1;   // 0000.0000.0000.0010
		break;
	case '2':
		digMask |= DM_2;   // 0000.0000.0000.0100
		break;
	case '3':
		digMask |= DM_3;   // 0000.0000.0000.1000
		break;
	case '4':
		digMask |= DM_4;   // 0000.0000.0001.0000
		break;
	case '5':
		digMask |= DM_5;   // 0000.0000.0010.0000
		break;
	case '6':
		digMask |= DM_6;   // 0000.0000.0100.0000
		break;
	case '7':
		digMask |= DM_7;   // 0000.0000.1000.0000
		break;
	case '8':
		digMask |= DM_8;   // 0000.0001.0000.0000
		break;
	case '9':
		digMask |= DM_9;   // 0000.0010.0000.0000
		break;
	case '0':
		digMask |= 0x0400; // 0000.0100.0000.0000
		break;
	case '*':
		digMask |= DM_S;   // 0000.1000.0000.0000
		break;
	case '#':
		digMask |= DM_P;   // 0001.0000.0000.0000
		break;
	case 'a':
	case 'A':
		digMask |= DM_A;   // 0010.0000.0000.0000
		break;
	case 'b':
	case 'B':
		digMask |= DM_B;   // 0100.0000.0000.0000
		break;
	case 'c':
	case 'C':
		digMask |= DM_C;   // 1000.0000.0000.0000
		break;
	}

	return digMask;
}
//----------------------------------------------------------------------------------------------
// End of _CommonFunc.cpp
//----------------------------------------------------------------------------------------------