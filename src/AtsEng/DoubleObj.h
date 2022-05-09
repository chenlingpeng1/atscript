// DoubleObj.h: interface for the CDoubleObj class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (陈凌鹏)
// 2021/5/1
// ver:1.8
// 代码免费使用，如果用于商务，请联系作者(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOUBLEOBJ_H__62604E27_E06B_4108_AF9E_358F963F33DD__INCLUDED_)
#define AFX_DOUBLEOBJ_H__62604E27_E06B_4108_AF9E_358F963F33DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtsObject.h"

class CDoubleObj : public CAtsObject 
{
public:
	CDoubleObj();
	virtual ~CDoubleObj();

	virtual BOOL DoFun( LPCTSTR strFunName , CAtsParam *pParam , CAtsSegIns *pSegIns);
	virtual CAtsObject * NewObject(LPCTSTR strName = _T("") );
	virtual CString ToString();
	virtual int		ToInt();
	virtual double	ToDouble();
	virtual BOOL	CmpData( CAtsObject *pData );

	double	m_nData;
	void	AddData( double nData );
	void	SetData( double nData );
	void	MultiplyData( double dData );
	void	DivideData( double dData );
	void	ModeData( int dData );
};

#endif // !defined(AFX_DOUBLEOBJ_H__62604E27_E06B_4108_AF9E_358F963F33DD__INCLUDED_)
