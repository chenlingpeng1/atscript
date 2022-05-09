// StringObj.h: interface for the CStringObj class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (������)
// 2021/5/1
// ver:1.8
// �������ʹ�ã����������������ϵ����(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRINGOBJ_H__ED7F033A_D31C_485F_8E8F_0C8046466976__INCLUDED_)
#define AFX_STRINGOBJ_H__ED7F033A_D31C_485F_8E8F_0C8046466976__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtsObject.h"
class CStringObj  : public CAtsObject  
{
public:
	CStringObj();
	virtual ~CStringObj();

	virtual BOOL DoFun( LPCTSTR strFunName , CAtsParam *pParam , CAtsSegIns *pSegIns);
	virtual CAtsObject * NewObject(LPCTSTR strName = _T("") );
	virtual CString ToString();
	virtual int		ToInt();
	virtual double	ToDouble();
	virtual BOOL	CmpData( CAtsObject *pData );

	CString		m_strData;

	void	AddData( CString strData );
	void	SetData( LPCTSTR strData );
};

#endif // !defined(AFX_STRINGOBJ_H__ED7F033A_D31C_485F_8E8F_0C8046466976__INCLUDED_)
