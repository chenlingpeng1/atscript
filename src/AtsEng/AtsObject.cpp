// AtsObject.cpp: implementation of the CAtsObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtsObject.h"
#include "AtsVarItem.h"
#include "AtsEngine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAtsObject::CAtsObject()
{
	m_pEngine = NULL;
}

CAtsObject::~CAtsObject()
{
	
}

void CAtsObject::DetachList(CAtsString strListName)
{
	CAtsVarItem		*pVarItem = NULL;
	POSITION		pos;

	if( strListName.IsEmpty() || strListName == "m_listUserVar" )
	{
		for( pos = m_listUserVar.GetHeadPosition() ; pos ; )
		{
			pVarItem = (CAtsVarItem *)m_listUserVar.GetNext(pos); 
			pVarItem->Release();
		}
		m_listUserVar.RemoveAll();

		if( !strListName.IsEmpty() )
			return;
	}
}

CAtsObject *CAtsObject::NewObject(LPCTSTR strName)
{
	CAtsObject	*pNewObj = new  CAtsObject;
	pNewObj->m_strName = strName;
	return pNewObj;
}

BOOL CAtsObject::DoFun( LPCTSTR strFunName , CAtsParam *pParam , CAtsSegIns *pSegIns)
{
	return false;
}

BOOL CAtsObject::SetVar(LPCTSTR sName , LPCTSTR strData)
{
	POSITION		pos;
	CPtrList		*pList = NULL;
	CAtsVarItem		*pVarItem = NULL;
	BOOL			bFind	= false;
	CAtsString		strName = sName;
 
	for( pos = m_listUserVar.GetHeadPosition() ; pos ; )
	{
		pVarItem = (CAtsVarItem *)m_listUserVar.GetNext(pos); 
		if( pVarItem->m_strName.CompareNoCase( strName ) == 0 )
		{
			pVarItem->m_vData.SetData( strData );
			bFind = true;
			return true;
		}
	}
	if( !bFind )
	{
		pVarItem = new CAtsVarItem;
		pVarItem->m_strName = strName;
		pVarItem->m_vData.SetData( strData );
		pVarItem->AddRef();
		m_listUserVar.AddTail( pVarItem );
	}
	return true;
}

//=======================================================
// 得到变量对象
//=======================================================
CAtsVarItem * CAtsObject::GetVarItem(LPCTSTR sName )
{
	POSITION		pos;
	CPtrList		*pList		= NULL;
	CAtsVarItem		*pVarItem	= NULL;
	BOOL			bFind		= false;
	CAtsString		strName		= sName;
	CAtsString		strData;

	pList = &m_listUserVar;
	
	for( pos = pList->GetHeadPosition() ; pos ; )
	{
		pVarItem = (CAtsVarItem *)pList->GetNext(pos); 
		if( pVarItem->m_strName.CompareNoCase( strName ) == 0 )
			return pVarItem;
	}
	return NULL;
}
