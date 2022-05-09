// WindParam.cpp: implementation of the CWndParam class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WndParam.h"
#include "AtsEngine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWndParam::CWndParam()
{
	m_strClassName = "CWndParam";
}

CWndParam::~CWndParam()
{
	ClearData();
}

void CWndParam::ClearData()
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

CString CWndParam::GetValueToStr( CString strVarName , CString strDefault)
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

CAtsVarItem * CWndParam::GetVarItem( CString strVarName )
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

int CWndParam::GetValueToInt( CString strVarName , int nDefault )
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

BOOL CWndParam::DoFun( LPCTSTR sFunName , CAtsParam *pParam , CAtsSegIns *pSegIns)
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
	else if( strFunName.CompareNoCase("SetWndProc") == 0 )
	{
		SetValue( "WndProc" , (LPCTSTR)pParam->GetItemData(1) );
		return true;
	}
	return false;
}

void CWndParam::SetValue( CString strVarName , int nData )
{
	CAtsVarItem * pItem = CWndParam::GetVarItem( strVarName );
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

void CWndParam::SetValue( CString strVarName , CString strData )
{
	CAtsVarItem * pItem = CWndParam::GetVarItem( strVarName );
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

CAtsObject *CWndParam::NewObject(LPCTSTR strName)
{
	CAtsObject	*pNewObj = new  CWndParam;
	pNewObj->m_strName = strName;
	return pNewObj;
}