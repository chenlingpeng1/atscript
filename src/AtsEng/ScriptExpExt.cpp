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

//============================================
// 根据变量名得到变量的值
//============================================
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

//===================================================
// 执行一个 function
//===================================================
BOOL CScriptExpExt::RunFun( CAtsString strFunName , CAtsParam *pParam )
{
	return m_pEgnine->RunFun( strFunName , pParam , m_pSegIns );
}

//============================================
// 判断是否为变量名
//============================================
BOOL CScriptExpExt::IsVar( CAtsString strName )
{
	if( m_pEgnine->IsVar( strName , m_pSegIns ) )
		return true;

	return false;
}

//============================================
// 判断是否为FunctionName
//============================================
BOOL CScriptExpExt::IsFunction( CAtsString strName )
{
	return false;
}
