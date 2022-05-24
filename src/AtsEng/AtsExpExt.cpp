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
BOOL CAtsExpExt::GetVarValue( CAtsString strVarName , CAtsValue *pValue )
{
	return false;
}
BOOL CAtsExpExt::RunFun( CAtsString strFunName , CAtsParam *pParam )
{
	return false;
}
BOOL CAtsExpExt::IsVar( CAtsString strName )
{
	return false;
}
BOOL CAtsExpExt::IsFunction( CAtsString strName )
{
	return false;
}

