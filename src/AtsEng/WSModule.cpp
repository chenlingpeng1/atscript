// VbsIns.cpp: implementation of the CWSModule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WSModule.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// JFSCRIPT_ENGINE_VBS
// 
CWSModule::CWSModule()
{
	m_nScriptType = JFSCRIPT_ENGINE_VBS;
}

CWSModule::~CWSModule()
{
}

BOOL CWSModule::LoadScript(CString strScript)
{
	m_ScriptFactory.InitializeScriptEngine(m_nScriptType);
	m_ScriptFactory.LoadScript(strScript);
	return true;
}

BOOL CWSModule::LoadFile( CString strFile )
{
	CString		strScript;

	if( !ReadFromFile( strFile , strScript ) )
		return false;

	return LoadScript( strScript );
}

BOOL CWSModule::ReadFromFile( CString strFileName , CString &strData  )
{
	CFile		file;
	char		ch[2];
	int			nRead;

	if( !file.Open( strFileName , CFile::modeRead ) )
		return false;

	while( true )
	{
		nRead = file.Read(ch,1);
		ch[1] = '\0';
		if( nRead == 1 )
			strData = strData + ch;
		else
			break;
	}
	file.Close();
	return true;
}

BOOL CWSModule::CallMethod( CString strProcName , CStringArray &asParam , CString &strReturn, CStringArray *pasParamType )
{
	CJfScriptParams		Params;
	int					nNum,nCount;
	CString				strType;

	if( !pasParamType )
		return false;

	nCount = asParam.GetSize();
	for( nNum = 0 ; nNum < nCount ; nNum++ )
	{
		strType = pasParamType->GetAt(nNum);
		if( strType.CompareNoCase("int") == 0 ||
			strType.CompareNoCase("long") == 0  )
		{
			Params.AddItem( (long)atol(asParam.GetAt(nNum)) );
		}
		else
			Params.AddItem( asParam.GetAt(nNum) );
	}

	try
	{
		m_ScriptFactory.CallSimpleFun( strProcName , &Params , strReturn );
	}
	catch(...)
	{
	}
	if( m_ScriptFactory.IsError() )
	{
		m_ScriptFactory.GetErrorMessages( m_asErrorInfo );
		return false;
	}
	else
		return true;
}
