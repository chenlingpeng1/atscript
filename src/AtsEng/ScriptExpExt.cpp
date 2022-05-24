// ScriptExpExt.cpp: implementation of the CScriptExpExt class.
//
//////////////////////////////////////////////////////////////////////

#include "ScriptExpExt.h"
#include "AtsEngine.h"
#include "AtsSegIns.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScriptExpExt::CScriptExpExt()
{
}

CScriptExpExt::~CScriptExpExt()
{
}
BOOL CScriptExpExt::GetVarValue( CAtsString strVarName , CAtsValue *pValue )
{
	CAtsVarItem		*pVarItem = NULL;

	if( !m_pEgnine )
		return false;

	pVarItem = m_pEgnine->GetVarItem( m_pSegIns , (LPCTSTR)strVarName );
	if( !pVarItem )
		return false;

	pVarItem->GetValueData( pValue );
	return true;
}
BOOL CScriptExpExt::RunFun( CAtsString strFunName , CAtsParam *pParam )
{
	return m_pEgnine->RunFun( strFunName , pParam , m_pSegIns );
}
BOOL CScriptExpExt::IsVar( CAtsString strName )
{
	if( m_pEgnine->IsVar( strName , m_pSegIns ) )
		return true;

	return false;
}
BOOL CScriptExpExt::IsFunction( CAtsString strName )
{
	return false;
}
