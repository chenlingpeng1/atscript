// ValueSet.h: interface for the CValueSet class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (陈凌鹏)
// 2021/5/1
// ver:1.8
// 代码免费使用，如果用于商务，请联系作者(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALUESET_H__C8254E9E_4450_4A5F_99BA_2D6F8133315B__INCLUDED_)
#define AFX_VALUESET_H__C8254E9E_4450_4A5F_99BA_2D6F8133315B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtsObject.h"

class CValueSet : public CAtsObject  
{
public:
	CValueSet();
	virtual ~CValueSet();

	CPtrList		m_listUserVar;

	CString GetValueToStr( CString strVarName , CString strDefault = "");
	int		GetValueToInt( CString strVarName , int nDefault = 0);
	CAtsVarItem * GetVarItem( CString strVarName );
	void SetValue( CString strVarName , CString strData );
	void SetValue( CString strVarName , int nData );
	void ClearData();

	virtual BOOL DoFun( LPCTSTR strFunName , CAtsParam *pParam , CAtsSegIns *pSegIns);
	virtual CAtsObject * NewObject(LPCTSTR strName = _T("") );
};

#endif // !defined(AFX_VALUESET_H__C8254E9E_4450_4A5F_99BA_2D6F8133315B__INCLUDED_)
