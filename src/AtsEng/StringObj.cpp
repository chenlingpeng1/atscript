// StringObj.cpp: implementation of the CStringObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StringObj.h"
#include "AtsEngine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStringObj::CStringObj()
{
	m_strClassName = "string";
}

CStringObj::~CStringObj()
{
}

CAtsObject *CStringObj::NewObject(LPCTSTR strName)
{
	CAtsObject *pObj = new CStringObj;
	pObj->m_pEngine = m_pEngine;
	return pObj;
}

CString CStringObj::ToString()
{
	return m_strData;
}

int CStringObj::ToInt()
{
	return atoi( m_strData );
}

double CStringObj::ToDouble()
{
	return atof( m_strData );
}

BOOL CStringObj::CmpData( CAtsObject *pData )
{
	if( !pData ) return false;
	if( m_strData == pData->ToString() )
		return true;
	else
		return false;
}

void CStringObj::AddData( CString strData )
{
	m_strData = m_strData + strData;
}

void CStringObj::SetData( LPCTSTR strData )
{
	m_strData = strData;
}


BOOL CStringObj::DoFun( LPCTSTR sFunName , CAtsParam *pParam , CAtsSegIns *pSegIns)
{
	CString		strFunName  = sFunName;
	CAtsVarItem *pVarItem	= NULL;
	CString		strVarName;

	if( strFunName.CompareNoCase("Left") == 0 )
	{
		int	nLen = atoi( m_pEngine->GetFactData(pParam->GetItemData(1) , pSegIns) );
		
		pParam->SetReturnDataEx( m_strData.Left(nLen) , ATSVAR_TYPE_STRING );
		return true;
	}
	else if( strFunName.CompareNoCase("Right") == 0 )
	{
		int	nLen = atoi( m_pEngine->GetFactData(pParam->GetItemData(1), pSegIns ) );
		
		pParam->SetReturnDataEx( m_strData.Right(nLen) , ATSVAR_TYPE_STRING);
		return true;
	}
	else if( strFunName.CompareNoCase("Mid") == 0 )
	{
		int nPos = atoi( m_pEngine->GetFactData(pParam->GetItemData(1), pSegIns) );
		int nLen = atoi( m_pEngine->GetFactData(pParam->GetItemData(2), pSegIns) );
		
		if( nLen > 0 )
			pParam->SetReturnDataEx( m_strData.Mid(nPos,nLen) , ATSVAR_TYPE_STRING);
		else
			pParam->SetReturnDataEx( m_strData.Mid(nPos) , ATSVAR_TYPE_STRING);
		return true;
	}
	else if( strFunName.CompareNoCase("ToUpper") == 0 )
	{
		CString		strTemp = m_strData;
		strTemp.MakeUpper();
		pParam->SetReturnDataEx( strTemp , ATSVAR_TYPE_STRING );
		return true;
	}
	else if( strFunName.CompareNoCase("ToLower") == 0 )
	{
		CString		strTemp = m_strData;
		strTemp.MakeLower();
		pParam->SetReturnDataEx( strTemp , ATSVAR_TYPE_STRING);
		return true;
	}
	else if( strFunName.CompareNoCase("Replace") == 0 )
	{
		CAtsString		strData1	= m_pEngine->GetFactData( pParam->GetItemData(1), pSegIns  );
		CAtsString		strData2	= m_pEngine->GetFactData( pParam->GetItemData(2), pSegIns  );
		
		m_strData.Replace( strData1 , strData2 );
		return true;
	}
	else if( strFunName.CompareNoCase("ToInt") == 0 )
	{
		pParam->SetReturnDataEx( ToInt() , ATSVAR_TYPE_INT );
		return true;
	}
	else if( strFunName.CompareNoCase("Find") == 0 )
	{
		CAtsString		strData = m_pEngine->GetFactData( pParam->GetItemData(1), pSegIns );
		int				nPos;

		if( strData.IsEmpty() )
			pParam->SetReturnDataEx( -1 , ATSVAR_TYPE_INT );
		else
		{
			nPos = m_strData.Find(strData.GetAt(0));
			pParam->SetReturnDataEx( nPos , ATSVAR_TYPE_INT );
		}
		return true;
	}
	else if( strFunName.CompareNoCase("ReverseFind") == 0 )
	{
		CAtsString		strData = m_pEngine->GetFactData( pParam->GetItemData(1), pSegIns );
		int				nPos;
		
		if( strData.IsEmpty() )
			pParam->SetReturnDataEx( -1 , ATSVAR_TYPE_INT );
		else
		{
			nPos = m_strData.ReverseFind(strData.GetAt(0));
			pParam->SetReturnDataEx( nPos , ATSVAR_TYPE_INT );
		}
		return true;
	}
	else if( strFunName.CompareNoCase("CompareNoCase") == 0 )
	{
		CAtsString		strData = m_pEngine->GetFactData( pParam->GetItemData(1), pSegIns );
		int				nData;
		
		nData = m_strData.CompareNoCase( strData );
		pParam->SetReturnDataEx( nData , ATSVAR_TYPE_INT );
		
		return true;
	}

	return false;
}