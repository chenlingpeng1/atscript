// VbsIns.h: interface for the CWSModule class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (陈凌鹏)
// 2021/5/1
// ver:1.8
// 代码免费使用，如果用于商务，请联系作者(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VBSINS_H__A37A04FD_6A75_4510_B572_2D8B0276F70D__INCLUDED_)
#define AFX_VBSINS_H__A37A04FD_6A75_4510_B572_2D8B0276F70D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "JfScriptEngineMgr.h"

class CWSModule  
{
public:
	CWSModule();
	virtual ~CWSModule();

	CJfScriptEngineMgr	m_ScriptFactory;
	CStringArray		m_asErrorInfo;
	int					m_nScriptType;

	BOOL ReadFromFile( CString strFileName , CString &strData  );
	BOOL LoadScript(CString strScript);
	BOOL LoadFile( CString strFile );
	BOOL CallMethod( CString strProcName , CStringArray &asParam , CString &strReturn, CStringArray *pasParamType = NULL );
};

#endif // !defined(AFX_VBSINS_H__A37A04FD_6A75_4510_B572_2D8B0276F70D__INCLUDED_)
