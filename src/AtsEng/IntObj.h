// IntObj.h: interface for the CIntObj class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (陈凌鹏)
// 2021/5/1
// ver:1.8
// 代码免费使用，如果用于商务，请联系作者(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTOBJ_H__0D342FD4_6755_4B66_81F1_95B2E59DF60B__INCLUDED_)
#define AFX_INTOBJ_H__0D342FD4_6755_4B66_81F1_95B2E59DF60B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtsObject.h"

class CIntObj : public CAtsObject  
{
public:
	CIntObj();
	virtual ~CIntObj();

	virtual BOOL DoFun( LPCTSTR strFunName , CAtsParam *pParam , CAtsSegIns *pSegIns);
	virtual CAtsObject * NewObject(LPCTSTR strName = _T("") );
	virtual CString ToString();
	virtual int		ToInt();
	virtual double	ToDouble();
	virtual BOOL	CmpData( CAtsObject *pData );

	int		m_nData;
	void	AddData( int nData );
	void	SetData( int nData );
	void	MultiplyData( double dData );
	void	DivideData( double dData );
	void	ModeData( int nData );
};

#endif // !defined(AFX_INTOBJ_H__0D342FD4_6755_4B66_81F1_95B2E59DF60B__INCLUDED_)
