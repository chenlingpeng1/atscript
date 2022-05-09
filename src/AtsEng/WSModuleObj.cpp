// VbsObj.cpp: implementation of the CWSModuleObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WSModuleObj.h"
#include "AtsEngine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWSModuleObj::CWSModuleObj()
{
	m_strClassName = "CWSModule";
}

CWSModuleObj::~CWSModuleObj()
{

}

CAtsObject *CWSModuleObj::NewObject(LPCTSTR strName)
{
	CWSModuleObj	*pNewObj = new  CWSModuleObj;
	pNewObj->m_strName = strName;
	return pNewObj;
}

//===================================================
// 执行 Install 的对象 Function
//===================================================
BOOL CWSModuleObj::DoFun( LPCTSTR sFunName , CAtsParam *pParam , CAtsSegIns *pSegIns)
{
	CAtsString		strFunName  = sFunName;
	long			nRet;

	if( strFunName.CompareNoCase("LoadFile") == 0 )
	{
		CString	 strFile =	pParam->GetItemData(1);
		if( strFile.Find(":\\") != 1 )
		{
			// 表示没有设置全路径
			strFile = pSegIns->m_pEngine->m_strScriptPath + "\\" + strFile;
		}
		m_VbsIns.LoadFile( strFile );
		pParam->SetReturnData( 1 );
		return true;
	}
	else if( strFunName.CompareNoCase("LoadScript") == 0 )
	{
		m_VbsIns.LoadScript( pParam->GetItemData(1) );
		pParam->SetReturnData( 1 );
		return true;
	}
	else if( strFunName.CompareNoCase("CallMethod") == 0 )
	{
		// 调用VBScript 的 function
		// CallVBSFunction( "MyFun" , "abc" , 9 );

		CAtsString		strFunName = pParam->GetItemData(1);
		CStringArray	asParamData;
		CStringArray	asParamType;
		
		CString			strReturn;
		CAtsVarItem		*pItem = NULL;

		for( int nNum = 2 ; nNum <= pParam->m_listParam.GetCount() ; nNum++ )
		{
			pItem = pParam->GetItem( nNum );
			if( pItem->GetVarType() == ATSVAR_TYPE_VAR )
			{
				pItem = pParam->m_pEng->GetVarItem( pParam->m_pParentSegIns , pItem->m_vData.GetStrData() );
				if( !pItem ) return false;
			} 

			asParamData.Add( (LPCTSTR)pItem->GetDataToStr() );
			if( pItem->GetVarType() == ATSVAR_TYPE_STRING )
				asParamType.Add( "string" );
			else
				asParamType.Add( "long" );
		}
	
		if( !m_VbsIns.CallMethod( strFunName , asParamData , strReturn , &asParamType ) )
		{
			int				nNum,nCount;

			nCount = m_VbsIns.m_asErrorInfo.GetSize();
			for( nNum = 0 ; nNum < nCount ; nNum++ )
				MessageBox( NULL , m_VbsIns.m_asErrorInfo.GetAt( nNum ) , "AtScript" , 0 );

			return false;
		}
		pParam->SetReturnDataEx( (LPCTSTR)strReturn , ATSVAR_TYPE_STRING );
		return true;
	}

	return false;
}