// MathObj.h: interface for the CMathObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATHOBJ_H__D29D3AF5_AD51_4707_95EA_C3F267F28ED8__INCLUDED_)
#define AFX_MATHOBJ_H__D29D3AF5_AD51_4707_95EA_C3F267F28ED8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtsObject.h"

class CMathObj  : public CAtsObject 
{
public:
	CMathObj();
	virtual ~CMathObj();

	double ToMod(double x);
	double ToRad(double x);
	
	int				modus;

	virtual BOOL DoFun( LPCTSTR strFunName , CAtsParam *pParam , CAtsSegIns *pSegIns);
};

#endif // !defined(AFX_MATHOBJ_H__D29D3AF5_AD51_4707_95EA_C3F267F28ED8__INCLUDED_)
