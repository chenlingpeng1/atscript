// AtsObject.h: interface for the CAtsObject class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (陈凌鹏)
// 2021/5/1
// ver:1.8
// 代码免费使用，如果用于商务，请联系作者(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATSOBJECT_H__1BB1EDE0_A891_41DF_9730_76DD2BA5BB0D__INCLUDED_)
#define AFX_ATSOBJECT_H__1BB1EDE0_A891_41DF_9730_76DD2BA5BB0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtsDef.h"
#include "AtsParam.h"
#include "AtsVarItem.h"

class CAtsEngine;

//==================================
// AtString 对象基类
//==================================
class ATS_API CAtsObject : public CAtsObj  
{
public:
	CAtsObject();
	virtual ~CAtsObject();
	virtual CAtsObject * NewObject(LPCTSTR strName = _T("") );

	virtual void DetachList(CAtsString strListName);
	virtual BOOL DoFun( LPCTSTR strFunName , CAtsParam *pParam , CAtsSegIns *pSegIns);
	virtual BOOL SetVar(LPCTSTR strName , LPCTSTR strData);
	virtual CAtsVarItem * GetVarItem(LPCTSTR sName );
	virtual BOOL IsVar( LPCTSTR strName )		{return false;}
	virtual BOOL IsFunction( LPCTSTR strName )	{return false;}
	virtual CString ToString()					{return _T("");}
	virtual int		ToInt()						{return 0;}
	virtual double	ToDouble()					{return 0;}
	virtual BOOL	CmpData( CAtsObject *pData ){return false;}

	CAtsString		m_strName;
	CAtsString		m_strClassName;
	CPtrList		m_listUserVar;
	CAtsEngine		*m_pEngine;
};	

#endif // !defined(AFX_ATSOBJECT_H__1BB1EDE0_A891_41DF_9730_76DD2BA5BB0D__INCLUDED_)
