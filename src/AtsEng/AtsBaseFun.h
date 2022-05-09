//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (陈凌鹏)
// 2021/5/1
// ver:1.8
// 代码免费使用，如果用于商务，请联系作者(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATSBASEFUN_H__FCD32011_F3F7_49D8_8439_1BFE5E7AB508__INCLUDED_)
#define AFX_ATSBASEFUN_H__FCD32011_F3F7_49D8_8439_1BFE5E7AB508__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtsDef.h"
#include "AtsVarItem.h"
#include "AtsParam.h"
#include "ValueSet.h"

#define		RETURN_FALSE		{ASSSERT(false);return false;}

class CAtsBaseFun  
{
public:
	CAtsBaseFun();
	virtual ~CAtsBaseFun();

	static CValueSet *  GetValueSetParam( CAtsParam *pParam , int nOrder );
	static int			CharToInt(char ch);
	static char			GetEscapeChar(char c);
	static char			GetEscapeChar_x(char c1, char c2);
	static void			LaunchApp(const char* temp_exe, BOOL bSync);
	static BOOL			IsValidVarName(CAtsString strName);
	static BOOL			GetElementItem(CAtsString &strStuff, CAtsString &strItem,CAtsString strApart);
	static CAtsString	GenerateGUIDString();
	static CAtsString	GetPathName( CAtsString strFileName );
	static int			GetOPLevel(int nOpType , CAtsString strData = _T("") );
};

#endif // !defined(AFX_ATSBASEFUN_H__FCD32011_F3F7_49D8_8439_1BFE5E7AB508__INCLUDED_)
