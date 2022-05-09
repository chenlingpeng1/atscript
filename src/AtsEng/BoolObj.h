// BoolObj.h: interface for the CBoolObj class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (������)
// 2021/5/1
// ver:1.8
// �������ʹ�ã����������������ϵ����(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOOLOBJ_H__3473271D_4C91_4787_9FF4_22971F1C7749__INCLUDED_)
#define AFX_BOOLOBJ_H__3473271D_4C91_4787_9FF4_22971F1C7749__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtsObject.h"

class CBoolObj  : public CAtsObject 
{
public:
	CBoolObj();
	virtual ~CBoolObj();

	virtual BOOL DoFun( LPCTSTR strFunName , CAtsParam *pParam , CAtsSegIns *pSegIns);
	virtual CAtsObject * NewObject(LPCTSTR strName = _T("") );
	virtual CString ToString();
	virtual int		ToInt();
	virtual double	ToDouble();
	virtual BOOL	CmpData( CAtsObject *pData );

	int		m_nData;
	void	SetData( int nData );
};

#endif // !defined(AFX_BOOLOBJ_H__3473271D_4C91_4787_9FF4_22971F1C7749__INCLUDED_)
