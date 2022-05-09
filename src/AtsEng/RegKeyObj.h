// RegKeyObj.h: interface for the CRegKeyObj class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (陈凌鹏)
// 2021/5/1
// ver:1.8
// 代码免费使用，如果用于商务，请联系作者(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGKEYOBJ_H__E02D676D_3250_4CDF_A27D_B04EF5E2F5FD__INCLUDED_)
#define AFX_REGKEYOBJ_H__E02D676D_3250_4CDF_A27D_B04EF5E2F5FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtsObject.h"

class CRegKeyObj : public CAtsObject
{
public:
	CRegKeyObj();
	virtual ~CRegKeyObj();

	virtual BOOL DoFun( LPCTSTR strFunName , CAtsParam *pParam , CAtsSegIns *pSegIns);
	virtual CAtsVarItem * GetVarItem(LPCTSTR sName );
	virtual BOOL SetVar(LPCTSTR strName , LPCTSTR strData);
	virtual CAtsObject * NewObject(LPCTSTR strName = _T("") );
};

#endif // !defined(AFX_REGKEYOBJ_H__E02D676D_3250_4CDF_A27D_B04EF5E2F5FD__INCLUDED_)
