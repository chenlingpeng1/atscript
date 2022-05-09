// BoolObj.cpp: implementation of the CBoolObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BoolObj.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBoolObj::CBoolObj()
{
	m_nData = 0;
	m_strClassName = "bool";
}

CBoolObj::~CBoolObj()
{
}

CAtsObject *CBoolObj::NewObject(LPCTSTR strName)
{
	CBoolObj *pObj = new CBoolObj;
	pObj->m_pEngine = m_pEngine;
	return pObj;
}

CString CBoolObj::ToString()
{
	if( m_nData == 0 )
		return "0";
	else
		return "1";
}

int CBoolObj::ToInt()
{
	return (int)m_nData;
}

double CBoolObj::ToDouble()
{
	return (double)m_nData;
}

BOOL CBoolObj::CmpData( CAtsObject *pData )
{
	/*if( !pData ) return false;
	if( m_strData == pData->ToString() )
		return true;
	else*/
		return false;
}

BOOL CBoolObj::DoFun( LPCTSTR sFunName , CAtsParam *pParam , CAtsSegIns *pSegIns)
{
	CString		strFunName  = sFunName;
	CAtsVarItem *pVarItem	= NULL;
	CString		strVarName;

	if( strFunName.CompareNoCase("ToString") == 0 )
	{
		pParam->SetReturnDataEx( ToString() , ATSVAR_TYPE_STRING );
		return true;
	}

	return false;
}

void CBoolObj::SetData( int nData )
{
	m_nData = nData;
}