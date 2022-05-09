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
// ���ݱ������õ�������ֵ
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
// ִ��һ�� function
//===================================================
BOOL CScriptExpExt::RunFun( CAtsString strFunName , CAtsParam *pParam )
{
	return m_pEgnine->RunFun( strFunName , pParam , m_pSegIns );
}

//============================================
// �ж��Ƿ�Ϊ������
//============================================
BOOL CScriptExpExt::IsVar( CAtsString strName )
{
	if( m_pEgnine->IsVar( strName , m_pSegIns ) )
		return true;

	return false;
}

//============================================
// �ж��Ƿ�ΪFunctionName
//============================================
BOOL CScriptExpExt::IsFunction( CAtsString strName )
{
	return false;
}
