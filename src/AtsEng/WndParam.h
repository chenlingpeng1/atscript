// WindParam.h: interface for the CWndParam class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (陈凌鹏)
// 2021/5/1
// ver:1.8
// 代码免费使用，如果用于商务，请联系作者(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDPARAM_H__627C67C7_08A2_4F65_BC70_AB1E3E94CEB6__INCLUDED_)
#define AFX_WINDPARAM_H__627C67C7_08A2_4F65_BC70_AB1E3E94CEB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtsObject.h"

class CWndParam  : public CAtsObject
{
public:
	CWndParam();
	virtual ~CWndParam();

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

#endif // !defined(AFX_WINDPARAM_H__627C67C7_08A2_4F65_BC70_AB1E3E94CEB6__INCLUDED_)
