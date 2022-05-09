// IntObj.cpp: implementation of the CIntObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IntObj.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIntObj::CIntObj()
{
	m_nData = 0;
	m_strClassName = "int";
}

CIntObj::~CIntObj()
{

}

CAtsObject *CIntObj::NewObject(LPCTSTR strName)
{
	CIntObj *pObj = new CIntObj;
	pObj->m_pEngine = m_pEngine;
	return pObj;
}

CString CIntObj::ToString()
{
	CString		strData;
	strData.Format("%d",m_nData);
	return strData;
}

int CIntObj::ToInt()
{
	return m_nData;
}

double CIntObj::ToDouble()
{
	return (double)m_nData;
}

BOOL CIntObj::CmpData( CAtsObject *pData )
{
	/*if( !pData ) return false;
	if( m_strData == pData->ToString() )
		return true;
	else*/
		return false;
}

BOOL CIntObj::DoFun( LPCTSTR sFunName , CAtsParam *pParam , CAtsSegIns *pSegIns)
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

void CIntObj::AddData( int nData )
{
	m_nData = m_nData + nData;
}

void CIntObj::SetData( int nData )
{
	m_nData = nData;
}

void CIntObj::DivideData( double dData )
{
	m_nData = m_nData / dData;
}

void CIntObj::MultiplyData( double dData )
{
	m_nData = m_nData * dData;
}

void CIntObj::ModeData( int dData )
{
	m_nData = m_nData % dData;
}