// LibraryInsObj.cpp: implementation of the CDllModuleObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DllModuleObj.h"
#include "AtsEngine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDllModuleObj::CDllModuleObj()
{
	m_strClassName = "CDllModule";
}

CDllModuleObj::~CDllModuleObj()
{

}

CAtsObject *CDllModuleObj::NewObject(LPCTSTR strName)
{
	CDllModuleObj	*pNewObj = new  CDllModuleObj;
	pNewObj->m_strName = strName;
	return pNewObj;
}

//===================================================
// 执行 Install 的对象 Function
//===================================================
BOOL CDllModuleObj::DoFun( LPCTSTR sFunName , CAtsParam *pParam , CAtsSegIns *pSegIns)
{
	CAtsString		strFunName  = sFunName;

	if( strFunName.CompareNoCase("LoadLibrary") == 0 )
	{
		CString	 strFile =	pParam->GetItemData(1);
		if( strFile.Find(":\\") != 1 )
		{
			// 表示没有设置全路径
			strFile = pSegIns->m_pEngine->m_strScriptPath + "\\" + strFile;
		}

		m_LibraryIns.LoadLibrary( strFile );
		pParam->SetReturnData( 1 );
		return true;
	}
	else if( strFunName.CompareNoCase("AddProcDefine") == 0 )
	{
		m_LibraryIns.AddProcDefine( pParam->GetItemData(1) );
		pParam->SetReturnData( 1 );
		return true;
	}
	else if( strFunName.CompareNoCase("CallProc") == 0 )
	{
		// 调用VBScript 的 function
		// CallVBSFunction( "MyFun" , "abc" , 9 );

		CAtsString		strFunName = pParam->GetItemData(1);
		CStringArray	asParamData;
		
		CString			strReturn;
	
		for( int nNum = 2 ; nNum <= pParam->m_listParam.GetCount() ; nNum++ )
			asParamData.Add( pParam->GetItemData(nNum) );		

		if( !m_LibraryIns.CallProc( strFunName , asParamData , strReturn ) )
			return false;
		
		pParam->SetReturnDataEx( (LPCTSTR)strReturn , ATSVAR_TYPE_STRING );
		return true;
	}

	return false;
}