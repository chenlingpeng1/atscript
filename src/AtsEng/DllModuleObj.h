// LibraryInsObj.h: interface for the CDllModuleObj class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (陈凌鹏)
// 2021/5/1
// ver:1.8
// 代码免费使用，如果用于商务，请联系作者(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIBRARYINSOBJ_H__9373AE7D_471C_4EF6_8046_7C1727BE13AA__INCLUDED_)
#define AFX_LIBRARYINSOBJ_H__9373AE7D_471C_4EF6_8046_7C1727BE13AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DllModule.h"
#include "AtsObject.h"

class CDllModuleObj  : public CAtsObject 
{
public:
	CDllModuleObj();
	virtual ~CDllModuleObj();

	CDllModule		m_LibraryIns;

	virtual BOOL DoFun( LPCTSTR strFunName , CAtsParam *pParam , CAtsSegIns *pSegIns);
	virtual CAtsObject * NewObject(LPCTSTR strName = _T("") );

};

#endif // !defined(AFX_LIBRARYINSOBJ_H__9373AE7D_471C_4EF6_8046_7C1727BE13AA__INCLUDED_)
