// VbsObj.h: interface for the CWSModuleObj class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (������)
// 2021/5/1
// ver:1.8
// �������ʹ�ã����������������ϵ����(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VBSOBJ_H__69F095C4_FEBC_4648_A75D_750C45102E05__INCLUDED_)
#define AFX_VBSOBJ_H__69F095C4_FEBC_4648_A75D_750C45102E05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtsObject.h"
#include "WSModule.h"

class CWSModuleObj : public CAtsObject 
{
public:
	CWSModuleObj();
	virtual ~CWSModuleObj();

	virtual BOOL DoFun( LPCTSTR strFunName , CAtsParam *pParam , CAtsSegIns *pSegIns);
	virtual CAtsObject * NewObject(LPCTSTR strName = _T("") );

	CWSModule		m_VbsIns;

	BOOL CallMethod( CString strScript );
};

#endif // !defined(AFX_VBSOBJ_H__69F095C4_FEBC_4648_A75D_750C45102E05__INCLUDED_)
