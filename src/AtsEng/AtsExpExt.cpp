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
// 根据变量名得到变量的值
//============================================
BOOL CAtsExpExt::GetVarValue( CAtsString strVarName , CAtsValue *pValue )
{
	return false;
}

//============================================
// 根据变量名得到变量的值
//============================================
BOOL CAtsExpExt::RunFun( CAtsString strFunName , CAtsParam *pParam )
{
	return false;
}

//============================================
// 判断是否为变量名
//============================================
BOOL CAtsExpExt::IsVar( CAtsString strName )
{
	return false;
}

//============================================
// 判断是否为FunctionName
//============================================
BOOL CAtsExpExt::IsFunction( CAtsString strName )
{
	return false;
}

