// UnInstallAtsObj.h: interface for the CMyAtsObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNINSTALLATSOBJ_H__8F08D6DF_4A27_4F6E_BDB1_B62AA40D5A89__INCLUDED_)
#define AFX_UNINSTALLATSOBJ_H__8F08D6DF_4A27_4F6E_BDB1_B62AA40D5A89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtsObject.h"
#include "ValueSet.h"

class CMyAtsObj  : public CAtsObject 
{
public:
	CMyAtsObj();
	virtual ~CMyAtsObj();

	virtual BOOL DoFun( LPCTSTR strFunName , CAtsParam *pParam , CAtsSegIns *pSegIns);
	virtual CAtsVarItem * GetVarItem(LPCTSTR sName );
	virtual BOOL SetVar(LPCTSTR strName , LPCTSTR strData);
	virtual BOOL IsVar( LPCTSTR strName );
	virtual BOOL IsFunction( LPCTSTR strName );
};

#endif // !defined(AFX_UNINSTALLATSOBJ_H__8F08D6DF_4A27_4F6E_BDB1_B62AA40D5A89__INCLUDED_)
