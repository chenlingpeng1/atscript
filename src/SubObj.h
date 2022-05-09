// SubObj.h: interface for the CSubObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUBOBJ_H__542E999C_9D44_4136_91B7_4E507977C309__INCLUDED_)
#define AFX_SUBOBJ_H__542E999C_9D44_4136_91B7_4E507977C309__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtsObject.h"

class CSubObj  : public CAtsObject 
{
public:
	CSubObj();
	virtual ~CSubObj();

	virtual BOOL DoFun( LPCTSTR strFunName , CAtsParam *pParam , CAtsSegIns *pSegIns);
public:
	CString		m_strMyName;

};

#endif // !defined(AFX_SUBOBJ_H__542E999C_9D44_4136_91B7_4E507977C309__INCLUDED_)
