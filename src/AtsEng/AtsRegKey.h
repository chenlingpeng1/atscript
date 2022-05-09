#if !defined(AFX_ATSREGKEY_H__15796546_EB23_11D2_8E55_00C00C02B5CD__INCLUDED_)
#define AFX_ATSREGKEY_H__15796546_EB23_11D2_8E55_00C00C02B5CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (陈凌鹏)
// 2021/5/1
// ver:1.8
// 代码免费使用，如果用于商务，请联系作者(905309882@qq.com)
//////////////////////////////////////////////////////////////////////
#include "AtsDef.h"

class CAtsRegKey
{
public:
	CAtsRegKey() {m_hKey = NULL;}
	~CAtsRegKey() {Close();}

// Attributes
public:
	HKEY	m_hKey;

public:
	BOOL	DeleteSubKey(LPCTSTR lpszSubKey);
	BOOL	RecurseDeleteKey(LPCTSTR lpszKey);
	BOOL	SetStringValue(LPCTSTR pszKey , CAtsString strValue );
	BOOL	GetStringValue(LPCTSTR pszKey , CAtsString& strValue );
	BOOL	SetIntValue( LPCTSTR pszKey , int nValue );
	BOOL	GetIntValue( LPCTSTR pszKey , int &nValue );
	BOOL	SetDWordValue (LPCTSTR pszKey, DWORD dwVal);
	BOOL	GetDWordValue(LPCTSTR pszKey, DWORD& dwVal);
	BOOL	DeleteValue(LPCTSTR lpszValue);

	BOOL	ExistKey(LPCTSTR pszPath);
	BOOL	ExistValue (LPCTSTR pszValue);

	static	BOOL ExistKey( HKEY hKeyRoot, LPCTSTR pszPath );
	static	BOOL RemoveRegItem(CAtsString strRegInfo);
	static	BOOL GetElementItem(CAtsString &strStuff, CAtsString &strItem,CAtsString strApart);
	static	HKEY GetRootkey( CAtsString strKey);
// Operations
public:
	BOOL	Create(HKEY hKey, LPCTSTR lpszKeyName);
	BOOL	Open(HKEY hKey, LPCTSTR lpszKeyName , REGSAM keyOption = KEY_READ|KEY_WRITE );

	void	Close();

// Overrides

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_ATSREGKEY_H__15796546_EB23_11D2_8E55_00C00C02B5CD__INCLUDED_)