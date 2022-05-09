// DoubleObj.cpp: implementation of the CDoubleObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DoubleObj.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDoubleObj::CDoubleObj()
{
	m_nData = 0;
	m_strClassName = "double";
}

CDoubleObj::~CDoubleObj()
{

}

void CDoubleObj::DivideData( double dData )
{
	m_nData = m_nData / dData;
}

void CDoubleObj::MultiplyData( double dData )
{
	m_nData = m_nData * dData;
}

void CDoubleObj::ModeData( int dData )
{
	m_nData = (int)m_nData % dData;
}

CAtsObject *CDoubleObj::NewObject(LPCTSTR strName)
{
	CDoubleObj *pObj = new CDoubleObj;
	pObj->m_pEngine = m_pEngine;
	return pObj;
}

CString CDoubleObj::ToString()
{
	CString		strData;
	strData.Format("%f",m_nData);
	return strData;
}

int CDoubleObj::ToInt()
{
	return (int)m_nData;
}

double CDoubleObj::ToDouble()
{
	return (double)m_nData;
}

BOOL CDoubleObj::CmpData( CAtsObject *pData )
{
	/*if( !pData ) return false;
	if( m_strData == pData->ToString() )
		return true;
	else*/
		return false;
}

BOOL CDoubleObj::DoFun( LPCTSTR sFunName , CAtsParam *pParam , CAtsSegIns *pSegIns)
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

void CDoubleObj::AddData( double nData )
{
	m_nData = m_nData + nData;
}

void CDoubleObj::SetData( double nData )
{
	m_nData = nData;
}