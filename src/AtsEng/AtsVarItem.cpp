// AtVarItem.cpp: implementation of the CAtVarItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtsVarItem.h"
#include "AtsObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAtsVarItem::CAtsVarItem()
{
	m_nFlag		= 0;
	m_bIsConst	= false;
}

CAtsVarItem::~CAtsVarItem()
{
}

CAtsVarItem & CAtsVarItem::operator = ( CAtsVarItem &Item )
{
	m_nFlag		= Item.m_nFlag;
	m_strName	= Item.m_strName;
	m_vData		= Item.m_vData;

	return *this;
}

void CAtsVarItem::SetValueName( LPCTSTR strNam , int nType )
{
	m_strName		= strNam;
	m_vData.m_nType	= nType;
}

void CAtsVarItem::GetValueData( CAtsValue *pValue )
{
	*pValue = m_vData;
}

CAtsObject * CAtsVarItem::GetValueObjData()
{
	if( IsObject() )
		return m_vData.m_pObject;
	return NULL;
}

CAtsString CAtsVarItem::GetDataToStr()
{
	return m_vData.GetStrData();
}

int CAtsVarItem::GetVarType()
{
	return m_vData.m_nType;
}

BOOL CAtsVarItem::IsObject()
{
//	if( m_vData.m_nType == ATSVAR_TYPE_ATSOBJECT ||
//		m_vData.m_nType == ATSVAR_TYPE_VALUESET )
	if( m_vData.m_nType == ATSVAR_TYPE_ATSOBJECT )
		return true;
	else
		return false;
}

//================================================
// 默认类型为变体
//================================================
int CAtsVarItem::GetVarType( CAtsString strType )
{
	int		nVarType = ATSVAR_TYPE_VARIANT;

	if( strType.CompareNoCase("string") == 0 )
		nVarType = ATSVAR_TYPE_STRING;
	else if( strType.CompareNoCase("int") == 0 )
		nVarType = ATSVAR_TYPE_INT;
	else if( strType.CompareNoCase("bool") == 0 )
		nVarType = ATSVAR_TYPE_BOOL;
	else if( strType.CompareNoCase("double") == 0 )
		nVarType = ATSVAR_TYPE_DOUBLE;
	else if( strType.CompareNoCase("IAtsObject") == 0 )
		nVarType = ATSVAR_TYPE_ATSOBJECT;
	else if( strType.CompareNoCase("void") == 0 )
		nVarType = ATSVAR_TYPE_VOID;
	else
		nVarType = ATSVAR_TYPE_UNKNOW;

	return nVarType;
}

//================================================
// 默认类型为变体
//================================================
CAtsString CAtsVarItem::GetVarTypeName( int nVarType )
{
	if( nVarType == ATSVAR_TYPE_STRING )
		return "string";
	else if( nVarType == ATSVAR_TYPE_INT )
		return "int";
	else if( nVarType == ATSVAR_TYPE_BOOL )
		return "bool";
	else if( nVarType == ATSVAR_TYPE_DOUBLE )
		return "double";
	else if( nVarType == ATSVAR_TYPE_ATSOBJECT )
		return "IAtsObject";
	else if( nVarType == ATSVAR_TYPE_VOID )
		return "void";
//	else if( nVarType == ATSVAR_TYPE_VALUESET )
//		return "CValueSet";

	return _T("");
}

void CAtsVarItem::SetData( CAtsVarItem *pSItem )
{
	SetData( &pSItem->m_vData );
}

void CAtsVarItem::SetData( CAtsValue *pData )
{
	m_vData.SetData( pData );
}