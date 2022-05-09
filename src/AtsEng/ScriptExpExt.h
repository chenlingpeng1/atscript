// ScriptExpExt.h: interface for the CScriptExpExt class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (陈凌鹏)
// 2021/5/1
// ver:1.8
// 代码免费使用，如果用于商务，请联系作者(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCRIPTEXPEXT_H__F2098A67_5832_4C57_A961_CB0D07C2EA36__INCLUDED_)
#define AFX_SCRIPTEXPEXT_H__F2098A67_5832_4C57_A961_CB0D07C2EA36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtsExpExt.h"
class CAtsEngine;
class CAtsSegIns;

class CScriptExpExt  : public CAtsExpExt
{
public:
	CScriptExpExt();
	virtual ~CScriptExpExt();
	
	virtual BOOL GetVarValue( CAtsString strVarName , CAtsValue *pValue );
	virtual BOOL RunFun( CAtsString strFunName , CAtsParam *pParam );
	virtual BOOL IsVar( CAtsString strName );
	virtual BOOL IsFunction( CAtsString strName );
};

#endif // !defined(AFX_SCRIPTEXPEXT_H__F2098A67_5832_4C57_A961_CB0D07C2EA36__INCLUDED_)
