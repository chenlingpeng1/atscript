// ValueSet.cpp: implementation of the CValueSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValueSet.h"
#include "AtsEngine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CValueSet::CValueSet()
{
	m_strClassName = "CValueSet";
}

CValueSet::~CValueSet()
{
	ClearData();
}

void CValueSet::ClearData()
{
	POSITION		pos;
	CAtsVarItem		*pVarItem = NULL;
 
	for( pos = m_listUserVar.GetHeadPosition() ; pos ; )
	{
		pVarItem = (CAtsVarItem *)m_listUserVar.GetNext(pos); 
		delete pVarItem;
	}
	m_listUserVar.RemoveAll();
}

CString CValueSet::GetValueToStr( CString strVarName , CString strDefault)
{
	POSITION		pos;
	CAtsVarItem		*pVarItem = NULL;

	for( pos = m_listUserVar.GetHeadPosition() ; pos ; )
	{
		pVarItem = (CAtsVarItem *)m_listUserVar.GetNext(pos); 
		if( pVarItem->m_strName.CompareNoCase( strVarName ) == 0 )
			return pVarItem->GetDataToStr();
	}
	return strDefault;
}

CAtsVarItem * CValueSet::GetVarItem( CString strVarName )
{
	POSITION		pos;
	CAtsVarItem		*pVarItem = NULL;

	for( pos = m_listUserVar.GetHeadPosition() ; pos ; )
	{
		pVarItem = (CAtsVarItem *)m_listUserVar.GetNext(pos); 
		if( pVarItem->m_strName.CompareNoCase( strVarName ) == 0 )
			return pVarItem;
	}
	return NULL;
}

int CValueSet::GetValueToInt( CString strVarName , int nDefault )
{
	POSITION		pos;
	CAtsVarItem		*pVarItem = NULL;

	for( pos = m_listUserVar.GetHeadPosition() ; pos ; )
	{
		pVarItem = (CAtsVarItem *)m_listUserVar.GetNext(pos); 
		if( pVarItem->m_strName.CompareNoCase( strVarName ) == 0 )
			return atoi( pVarItem->GetDataToStr() );
	}
	return nDefault;
}

BOOL CValueSet::DoFun( LPCTSTR sFunName , CAtsParam *pParam , CAtsSegIns *pSegIns)
{
	CString		strFunName  = sFunName;
	CAtsVarItem *pVarItem	= NULL;
	CString		strVarName;

	if( strFunName.CompareNoCase("SetValue") == 0 )
	{
		SetValue( (LPCTSTR)pParam->GetItemData(1) , (LPCTSTR)pParam->GetItemData(2) );
		return true;
	}
	else if( strFunName.CompareNoCase("GetValue") == 0 )
	{
		strVarName = pParam->GetItemData(1);
		pParam->SetReturnDataEx( GetValueToStr(strVarName) , ATSVAR_TYPE_STRING );
		
		return true;
	}
	else if( strFunName.CompareNoCase("ClearData") == 0 )
	{
		ClearData();
		return true;
	}
	return false;
}

void CValueSet::SetValue( CString strVarName , int nData )
{
	CAtsVarItem * pItem = CValueSet::GetVarItem( strVarName );
	if( pItem )
		pItem->m_vData.SetData( nData );
	else
	{ 
		CAtsVarItem *pVarItem = new CAtsVarItem;
		pVarItem->m_strName = strVarName;
		pVarItem->m_vData.SetData( nData , ATSVAR_TYPE_INT );
		pVarItem->AddRef();
		m_listUserVar.AddTail( pVarItem );
	}
}

void CValueSet::SetValue( CString strVarName , CString strData )
{
	CAtsVarItem * pItem = CValueSet::GetVarItem( strVarName );
	if( pItem )
		pItem->m_vData.SetData( strData );
	else
	{
		CAtsVarItem *pVarItem = new CAtsVarItem;
		pVarItem->m_strName = strVarName;
		pVarItem->m_vData.SetData( strData );
		pVarItem->AddRef();
		m_listUserVar.AddTail( pVarItem );
	}
}

CAtsObject *CValueSet::NewObject(LPCTSTR strName)
{
	CAtsObject	*pNewObj = new  CValueSet;
	pNewObj->m_strName = strName;
	return pNewObj;
}