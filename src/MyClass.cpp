// MyClass.cpp: implementation of the CMyClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TestAts.h"
#include "MyClass.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyClass::CMyClass()
{
	m_strClassName = "CMyClass";
}

CMyClass::~CMyClass()
{

}

CAtsObject *CMyClass::NewObject(LPCTSTR strName)
{
	CAtsObject	*pNewObj = new  CMyClass;
	pNewObj->m_strName = strName;
	return pNewObj;
}

BOOL CMyClass::DoFun( LPCTSTR sFunName , CAtsParam *pParam , CAtsSegIns *pSegIns)
{
	CString		strFunName  = sFunName;
	if( strFunName.CompareNoCase("SetData") == 0 )
	{
		m_strData = pParam->GetItemData(1);
		return true;
	}
	else if( strFunName.CompareNoCase("GetData") == 0 )
	{
		pParam->SetReturnDataEx( m_strData , ATSVAR_TYPE_STRING );
		return true;
	}

	return false;
}
