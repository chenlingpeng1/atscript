// SubObj.cpp: implementation of the CSubObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestAts.h"
#include "SubObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSubObj::CSubObj()
{

}

CSubObj::~CSubObj()
{

}

BOOL CSubObj::DoFun( LPCTSTR sFunName , CAtsParam *pParam , CAtsSegIns *pSegIns)
{
	CString		strFunName  = sFunName;
	if( strFunName.CompareNoCase("GetMyName") == 0 )
	{
		pParam->SetReturnData( m_strMyName );
		return true;
	}

	return false;
}