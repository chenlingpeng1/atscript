// FEExt.cpp: implementation of the CAtsExpExt class.
//
//////////////////////////////////////////////////////////////////////

#include "AtsExpExt.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAtsExpExt::CAtsExpExt()
{
	m_pEgnine = NULL;
	m_pSegIns = NULL;
}

CAtsExpExt::~CAtsExpExt()
{

}

//============================================
// ���ݱ������õ�������ֵ
//============================================
BOOL CAtsExpExt::GetVarValue( CAtsString strVarName , CAtsValue *pValue )
{
	return false;
}

//============================================
// ���ݱ������õ�������ֵ
//============================================
BOOL CAtsExpExt::RunFun( CAtsString strFunName , CAtsParam *pParam )
{
	return false;
}

//============================================
// �ж��Ƿ�Ϊ������
//============================================
BOOL CAtsExpExt::IsVar( CAtsString strName )
{
	return false;
}

//============================================
// �ж��Ƿ�ΪFunctionName
//============================================
BOOL CAtsExpExt::IsFunction( CAtsString strName )
{
	return false;
}

