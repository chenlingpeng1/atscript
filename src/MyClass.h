// MyClass.h: interface for the CMyClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYCLASS_H__D975D0C6_2CE2_46D9_87E2_A285BBAC97C3__INCLUDED_)
#define AFX_MYCLASS_H__D975D0C6_2CE2_46D9_87E2_A285BBAC97C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtsObject.h"

class CMyClass  : public CAtsObject 
{
public:
	CMyClass();
	virtual ~CMyClass();

	CString		m_strData;

	virtual CAtsObject * NewObject(LPCTSTR strName = _T("") );
	virtual BOOL DoFun( LPCTSTR strFunName , CAtsParam *pParam , CAtsSegIns *pSegIns);
};

#endif // !defined(AFX_MYCLASS_H__D975D0C6_2CE2_46D9_87E2_A285BBAC97C3__INCLUDED_)
