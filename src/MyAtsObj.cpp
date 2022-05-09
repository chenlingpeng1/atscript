// UnInstallAtsObj.cpp: implementation of the CMyAtsObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyAtsObj.h"
#include "SubObj.h"
#include "AtsEngine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyAtsObj::CMyAtsObj()
{

}

CMyAtsObj::~CMyAtsObj()
{

}

BOOL CMyAtsObj::DoFun( LPCTSTR sFunName , CAtsParam *pParam , CAtsSegIns *pSegIns)
{
	CString		strFunName  = sFunName;
	if( strFunName.CompareNoCase("Test") == 0 )
	{
		if( atoi( pParam->GetItemData(1) ) < 3 )
			pParam->SetReturnDataEx( "" , ATSVAR_TYPE_STRING );
		else
			pParam->SetReturnDataEx( "abc" , ATSVAR_TYPE_STRING );

		return true;
	}
	else if( strFunName.CompareNoCase("GetName") == 0 )
	{
		pParam->SetReturnData( _T("MyObj") );
		return true;
	}
	else if( strFunName.CompareNoCase("CreateSubObj") == 0 )
	{
		CSubObj		*pObj = new CSubObj;

		pObj->m_strMyName = "abc123";
		pParam->SetReturnData_Obj( (CAtsObject *)pObj );
		return true;
	}

	return false;
}

BOOL CMyAtsObj::SetVar(LPCTSTR strName , LPCTSTR strData)
{
	return false;
}

CAtsVarItem * CMyAtsObj::GetVarItem(LPCTSTR sName )
{
	CString			strName;
	CAtsVarItem		*pItem = NULL;

	strName = sName;
	if( strName.CompareNoCase("TheName") == 0  )
	{
		pItem = new CAtsVarItem;
		pItem->m_vData.m_nType		= ATSVAR_TYPE_STRING;
		pItem->m_strName			= sName;
		pItem->m_vData.m_strSysData	= "the myobj name";
		return pItem;
	}
	else if( strName.CompareNoCase("@TheName") == 0  )
	{
		pItem = new CAtsVarItem;
		pItem->m_vData.m_nType		= ATSVAR_TYPE_STRING;
		pItem->m_strName			= sName;
		pItem->m_vData.m_strSysData	= "@ the myobj name";
		return pItem;
	}

	return NULL;
}

BOOL CMyAtsObj::IsVar( LPCTSTR strName )
{
	CString		strTheName;

	strTheName = strName;
	if( strTheName.CompareNoCase("TheName")		== 0	||
		strTheName.CompareNoCase("@TheName")	== 0	)
		return true;
	else
		return false;
}

BOOL CMyAtsObj::IsFunction( LPCTSTR strName )
{
	return false;
}