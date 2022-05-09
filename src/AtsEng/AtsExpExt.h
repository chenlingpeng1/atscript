// FEExt.h: interface for the CAtsExpExt class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (陈凌鹏)
// 2021/5/1
// ver:1.8
// 代码免费使用，如果用于商务，请联系作者(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FEEXT_H__6B967173_8580_4ECF_9AEF_2614DA792D04__INCLUDED_)
#define AFX_FEEXT_H__6B967173_8580_4ECF_9AEF_2614DA792D04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtsValue.h"
#include "AtsParam.h"

class CAtsExpExt  
{
public:
	CAtsExpExt();
	virtual ~CAtsExpExt();

	virtual BOOL GetVarValue( CAtsString strVarName , CAtsValue *pValue );
	virtual BOOL RunFun( CAtsString strFunName , CAtsParam *pParam );
	virtual BOOL IsVar( CAtsString strName );
	virtual BOOL IsFunction( CAtsString strName );

	CAtsEngine	*m_pEgnine;
	CAtsSegIns  *m_pSegIns;
};

#endif // !defined(AFX_FEEXT_H__6B967173_8580_4ECF_9AEF_2614DA792D04__INCLUDED_)
