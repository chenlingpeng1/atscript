//////////////////////////////////////////////////////////////////////
//   SetRegStrValue <ROOT> , <PATH> , <NAME> , <DATA>
//   strData = GetRegStrValue( <ROOT> , <PATH> , <NAME> , <DefaultDATA> )
//   GetRegStrValueEx( <ROOT> , <PATH> , <NAME>  ) as AtsInt
//
//   ExistRegValue  <ROOT> , <PATH> , <NAME> , <DATA>
//
//   SetRegDWordValue <ROOT> , <PATH> , <NAME> , <DATA>
//   dData = GetRegDWordValue( <ROOT> , <PATH> , <NAME> , <DefaultDATA> )
//   GetRegDWordValueEx( <ROOT> , <PATH> , <NAME> , <DefaultDATA> ) as AtsInt
//////////////////////////////////////////////////////////////////////

#include "RegKeyObj.h"
#include "AtsRegKey.h"
#include "AtsEngine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegKeyObj::CRegKeyObj()
{
	m_strClassName = "CRegKey";
}

CRegKeyObj::~CRegKeyObj()
{

}

CAtsObject *CRegKeyObj::NewObject(LPCTSTR strName)
{
	CAtsObject	*pNewObj = new  CRegKeyObj;
	pNewObj->m_strName = strName;
	return pNewObj;
}

//===================================================
// 执行 Install 的对象 Function
//===================================================
BOOL CRegKeyObj::DoFun( LPCTSTR sFunName , CAtsParam *pParam , CAtsSegIns *pSegIns)
{
	CAtsString		strFunName  = sFunName;
	long			nRet;

	if( strFunName.CompareNoCase("SetRegStrValue") == 0 )
	{
		// 设置注册表中的一个字符串值
		CAtsRegKey		RegKey;
		CAtsString		strRoot,strPath,strVarName,strData;

		strRoot		= pParam->GetItemData(1);
		strPath		= pParam->GetItemData(2);
		strVarName	= pParam->GetItemData(3);
		strData		= pParam->GetItemData(4);
		if( RegKey.Open( CAtsRegKey::GetRootkey( strRoot ), strPath , KEY_READ|KEY_WRITE ) )
		{
			RegKey.SetStringValue( strVarName , strData );	
		}
		return true;
	}
	else if( strFunName.CompareNoCase("GetRegStrValue") == 0 )
	{
		// 得到注册表中的一个字符串值
		CAtsRegKey		RegKey;
		CAtsString		strRoot,strPath,strVarName,strData,strDefaultData;

		strRoot			= pParam->GetItemData(1);
		strPath			= pParam->GetItemData(2);
		strVarName		= pParam->GetItemData(3);
		strDefaultData	= pParam->GetItemData(4);
	
		strData = strDefaultData;
		if( RegKey.Open( CAtsRegKey::GetRootkey( strRoot ), strPath , KEY_READ ) )
			RegKey.GetStringValue( strVarName , strData );	
		
		pParam->SetReturnDataEx( strData , ATSVAR_TYPE_STRING );
		return true;
	}
	else if( strFunName.CompareNoCase("GetRegStrValueEx") == 0 )
	{
		// 得到注册表中的一个字符串值
		CAtsRegKey		RegKey;
		CAtsString		strRoot,strPath,strVarName,strData;

		strRoot			= m_pEngine->GetFactData( pParam->GetItemData(1), pSegIns );
		strPath			= m_pEngine->GetFactData( pParam->GetItemData(2), pSegIns );
		strVarName		= m_pEngine->GetFactData( pParam->GetItemData(3), pSegIns );
	
		nRet    = 1;
		if( RegKey.Open( CAtsRegKey::GetRootkey( strRoot ), strPath , KEY_READ ) )
		{
			if( !RegKey.GetStringValue( strVarName , strData ) )
				nRet = 0;
		}
		else
			nRet = 0;
		
		pParam->SetOutData( 4, strData );
		pParam->SetReturnData( nRet );
		return true;
	}
	else if( strFunName.CompareNoCase("SetRegDWordValue") == 0 )
	{
		// 设置注册表中的一个DWORD值
		CAtsRegKey		RegKey;
		int				nData;
		CAtsString		strRoot,strPath,strVarName,strData;

		strRoot		= m_pEngine->GetFactData( pParam->GetItemData(1), pSegIns );
		strPath		= m_pEngine->GetFactData( pParam->GetItemData(2), pSegIns );
		strVarName	= m_pEngine->GetFactData( pParam->GetItemData(3), pSegIns );
		nData		= atoi(m_pEngine->GetFactData( pParam->GetItemData(4), pSegIns ));
		if( RegKey.Open( CAtsRegKey::GetRootkey( strRoot ), strPath , KEY_READ|KEY_WRITE ) )
		{
			RegKey.SetIntValue( strVarName , nData );	
		}
		return true;
	}
	else if( strFunName.CompareNoCase("GetRegDWordValue") == 0 )
	{
		// 得到注册表中的一个字符串值
		CAtsRegKey		RegKey;
		int				nData,nDefautlData;
		CAtsString		strRoot,strPath,strVarName,strDefaultData;

		strRoot			= m_pEngine->GetFactData( pParam->GetItemData(1), pSegIns );
		strPath			= m_pEngine->GetFactData( pParam->GetItemData(2), pSegIns );
		strVarName		= m_pEngine->GetFactData( pParam->GetItemData(3), pSegIns );
		nDefautlData	= atoi( m_pEngine->GetFactData( pParam->GetItemData(4), pSegIns ) );
	
		nData = nDefautlData;
		if( RegKey.Open( CAtsRegKey::GetRootkey( strRoot ), strPath , KEY_READ ) )
			RegKey.GetIntValue( strVarName , nData );	
		
		pParam->SetReturnData( nData );
		return true;
	}
	else if( strFunName.CompareNoCase("GetRegDWordValueEx") == 0 )
	{
		// 得到注册表中的一个字符串值
		CAtsRegKey		RegKey;
		int				nData;
		CAtsString		strRoot,strPath,strVarName;

		strRoot			= m_pEngine->GetFactData( pParam->GetItemData(1), pSegIns );
		strPath			= m_pEngine->GetFactData( pParam->GetItemData(2), pSegIns );
		strVarName		= m_pEngine->GetFactData( pParam->GetItemData(3), pSegIns );
	
		nRet    = 1;
		if( RegKey.Open( CAtsRegKey::GetRootkey( strRoot ), strPath , KEY_READ ) )
		{
			if( !RegKey.GetIntValue( strVarName , nData ) )
				nRet = 0;
		}
		else
			nRet = 0;
		
		pParam->SetOutData( 4, nData );
		pParam->SetReturnData( nRet );
		return true;
	}
	else if( strFunName.CompareNoCase("ExistValue") == 0 )
	{
		// 得到注册表中的一个字符串值
		CAtsRegKey		RegKey;
		CAtsString		strRoot,strPath,strVarName;

		strRoot			= m_pEngine->GetFactData( pParam->GetItemData(1), pSegIns );
		strPath			= m_pEngine->GetFactData( pParam->GetItemData(2), pSegIns );
		strVarName		= m_pEngine->GetFactData( pParam->GetItemData(3), pSegIns );
	
		nRet    = 0;
		if( RegKey.Open( CAtsRegKey::GetRootkey( strRoot ), strPath , KEY_READ ) )
		{
			if( RegKey.ExistValue( strVarName ) )
				nRet = 1;
		}
		pParam->SetReturnData( nRet );
		return true;
	}
	else if( strFunName.CompareNoCase("DelSubKey") == 0 )
	{
		// 得到注册表中的一个字符串值
		CAtsRegKey		RegKey;
		CAtsString		strRoot,strPath,strKey;
		
		strRoot			= m_pEngine->GetFactData( pParam->GetItemData(1), pSegIns );
		strPath			= m_pEngine->GetFactData( pParam->GetItemData(2), pSegIns );
		strKey			= m_pEngine->GetFactData( pParam->GetItemData(3), pSegIns );
		
		nRet    = 0;
		if( RegKey.Open( CAtsRegKey::GetRootkey( strRoot ), strPath  ) )
		{
			if( RegKey.RecurseDeleteKey(strKey) )
				nRet = 1;
		}
		pParam->SetReturnData( nRet );
		return true;
	}
	//else if( strFunName.CompareNoCase("Open") == 0 )
	//else if( strFunName.CompareNoCase("ExistKey") == 0 )
	//else if( strFunName.CompareNoCase("GetSubKeyCount") == 0 )
	//GetSubKeyCount() return nCount;
	//GetSubKeyName( int nIndex )

	return false;
}

BOOL CRegKeyObj::SetVar(LPCTSTR strName , LPCTSTR strData)
{
	return false;
}

//=======================================================
// 得到变量对象
//=======================================================
CAtsVarItem * CRegKeyObj::GetVarItem(LPCTSTR sName )
{
	return NULL;
}
