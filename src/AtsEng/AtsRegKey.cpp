// key.cpp : implementation file
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1997 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "AtsRegkey.h"
#include <winreg.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAtsRegKey

void CAtsRegKey::Close()
{
	if (m_hKey != NULL)
	{
		LONG lRes = RegCloseKey(m_hKey);
		ASSERT(lRes == ERROR_SUCCESS);
		m_hKey = NULL;
	}
}

BOOL CAtsRegKey::Create(HKEY hKey, LPCTSTR lpszKeyName)
{
	ASSERT(hKey != NULL);
	return (RegCreateKey(hKey, lpszKeyName, &m_hKey) == ERROR_SUCCESS);
}

//===================================================================
// 判断是否存在当前 Key
// 如: CAtsRegKey::ExistKey( HKEY_CLASSES_ROOT , "software\\asoft" )
//===================================================================
BOOL CAtsRegKey::ExistKey (HKEY hKeyRoot, LPCTSTR pszPath)
{
	ASSERT (hKeyRoot);
	ASSERT (pszPath);
	HKEY	hKey;

	LONG ReturnValue = RegOpenKeyEx(hKeyRoot, pszPath, 0L,
		KEY_READ, &hKey);
	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

//===================================================================
// 判断是否存在当前 Key
// 如: CAtsRegKey::ExistKey( HKEY_CLASSES_ROOT , "software\\asoft" )
//===================================================================
BOOL CAtsRegKey::ExistKey(LPCTSTR pszPath)
{
	ASSERT (m_hKey);
	HKEY	hKey;

	LONG ReturnValue = RegOpenKeyEx (m_hKey, pszPath, 0L,
		KEY_READ, &hKey);
	
	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CAtsRegKey::ExistValue(LPCTSTR pszValue)
{
	ASSERT(m_hKey);
	LONG lReturn = RegQueryValueEx(m_hKey, pszValue, NULL,
		NULL, NULL, NULL);

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

BOOL CAtsRegKey::Open(HKEY hKey, LPCTSTR lpszKeyName , REGSAM keyOption /* = KEY_READ|KEY_WRITE */)
{
	ASSERT(hKey != NULL);

	//return (RegOpenKey(hKey, lpszKeyName, &m_hKey ) == ERROR_SUCCESS);
	return (RegOpenKeyEx(hKey, lpszKeyName, 0 , keyOption , &m_hKey ) == ERROR_SUCCESS);
}

BOOL CAtsRegKey::SetStringValue(LPCTSTR pszKey , CAtsString strValue)
{
	LPCTSTR		lpszVarName;

	lpszVarName = strValue;

	ASSERT(m_hKey != NULL);
	return (RegSetValueEx(m_hKey, pszKey, NULL, REG_SZ, 
		(BYTE * const)lpszVarName, (lstrlen(lpszVarName )+1)*sizeof(TCHAR)) == ERROR_SUCCESS);
}

BOOL CAtsRegKey::GetStringValue(LPCTSTR pszKey , CAtsString& strValue)
{
	DWORD	dw		= 0;
	DWORD	dwType	= 0;
	LONG	lRes	= 0;

	ASSERT(m_hKey != NULL);
	
	lRes = RegQueryValueEx(	m_hKey, (LPTSTR)pszKey, NULL, &dwType, 
							NULL, &dw);

	if (lRes == ERROR_SUCCESS)
	{
		if(dwType != REG_SZ && dwType != REG_EXPAND_SZ )
			return false;

		LPTSTR lpsz = strValue.GetBufferSetLength(dw);
		lRes = RegQueryValueEx(m_hKey, (LPTSTR)pszKey, NULL, &dwType, (BYTE*)lpsz, &dw);
		ASSERT(lRes == ERROR_SUCCESS);
		strValue.ReleaseBuffer();
		return TRUE;
	}
	return FALSE;
}


BOOL CAtsRegKey::SetIntValue( LPCTSTR pszKey , int nValue )
{
	DWORD dwValue;

	ASSERT(m_hKey);
	ASSERT(pszKey);
	
	dwValue = (DWORD)nValue;
	LONG ReturnValue = RegSetValueEx (m_hKey, pszKey, 0L, REG_DWORD,
		(CONST BYTE*) &dwValue, sizeof(DWORD));

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

BOOL CAtsRegKey::SetDWordValue (LPCTSTR pszKey, DWORD dwVal)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);
	return RegSetValueEx (m_hKey, pszKey, 0L, REG_DWORD,
		(CONST BYTE*) &dwVal, sizeof(DWORD));
}

BOOL CAtsRegKey::GetIntValue(LPCTSTR pszKey, int& nValue)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);

	DWORD dwType;
	DWORD dwSize = sizeof (DWORD);
	DWORD dwDest;

	LONG lReturn = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL,
		&dwType, (BYTE *) &dwDest, &dwSize);

	if(lReturn == ERROR_SUCCESS)
	{
		nValue = (int)dwDest;
		return TRUE;
	}

	return FALSE;
}

BOOL CAtsRegKey::GetDWordValue(LPCTSTR pszKey, DWORD& dwVal)
{
	ASSERT(m_hKey);
	ASSERT(pszKey);

	DWORD dwType;
	DWORD dwSize = sizeof (DWORD);
	DWORD dwDest;

	LONG lReturn = RegQueryValueEx (m_hKey, (LPSTR) pszKey, NULL, 
		&dwType, (BYTE *) &dwDest, &dwSize);

	if(lReturn == ERROR_SUCCESS)
	{
		dwVal = dwDest;
		return TRUE;
	}

	return FALSE;
}

/********************************************************************/
/*																	*/
/* Function name : DeleteSubKey										*/
/* Description   : Deletes the specified key.						*/
/*																	*/
/********************************************************************/
BOOL CAtsRegKey::DeleteSubKey(LPCTSTR lpszSubKey)
{
	ASSERT(m_hKey != NULL);
	LONG lReturn = RegDeleteKey(m_hKey, lpszSubKey);
	if(lReturn == ERROR_SUCCESS)
		return true;
	else
		return false;
}

BOOL CAtsRegKey::DeleteValue(LPCTSTR lpszValue)
{
	ASSERT(m_hKey != NULL);
	LONG lReturn =  RegDeleteValue(m_hKey, (LPTSTR)lpszValue);
	if(lReturn == ERROR_SUCCESS)
		return true;
	else
		return false;
}

/********************************************************************/
/*																	*/
/* Function name : RecurseDeleteKey									*/
/* Description   : Deletes the specified key and explicitly deletes */
/*				   all subkeys.										*/
/*																	*/
/********************************************************************/
BOOL CAtsRegKey::RecurseDeleteKey(LPCTSTR lpszKey)
{
	CAtsRegKey myKey;

	if( !myKey.Open( m_hKey, lpszKey ) )
		return false;

	FILETIME time;
	DWORD dwSize = 256;
	TCHAR szBuffer[256];
	while (RegEnumKeyEx( myKey.m_hKey, 0, szBuffer, &dwSize, NULL, NULL, NULL,
		&time)==ERROR_SUCCESS)
	{
		if( !myKey.RecurseDeleteKey(szBuffer) )
			return false;

		dwSize = 256;
	}
	myKey.Close();
	return DeleteSubKey(lpszKey);
}

BOOL CAtsRegKey::RemoveRegItem(CAtsString strRegInfo)
{
	CAtsString		strType		= _T("");
	CAtsString		strRootKey	= _T("");
	CAtsString		strPath		= _T("");
	CAtsString		strValue	= _T("");
	CAtsString		strItem;
	CAtsRegKey		myKey;
	HKEY			hRoot;

	if( !GetElementItem(strRegInfo, strType , "," ) )
		return false;

	if( !GetElementItem(strRegInfo, strRootKey , "," ) )
		return false;

	if( !GetElementItem(strRegInfo, strPath , "," ) )
		return false;

	if( !GetElementItem(strRegInfo, strItem , "," ) )
		return false;

	hRoot = GetRootkey( strRootKey );
	if( !myKey.Open( hRoot, strPath ) )
		return	false;

	if( strType.CompareNoCase( "Key" ) == 0 )
		myKey.RecurseDeleteKey( strItem );
	else
		myKey.DeleteValue( strItem );

	return true;
}

BOOL CAtsRegKey::GetElementItem(CAtsString &strStuff, CAtsString &strItem,CAtsString strApart)
{
	if( strStuff.GetLength() == 0 )
		return FALSE;

	int pos = strStuff.Find(strApart);
	if( pos >= 0 )
	{
		strItem = strStuff.Left(pos);
		strStuff = strStuff.Right( strStuff.GetLength() - pos - strApart.GetLength() );
	}
	else
	{
		strItem		= strStuff;
		strStuff	= "";
	}
	return TRUE;
}

HKEY CAtsRegKey::GetRootkey( CAtsString strKey)
{
	if( strKey == "HKEY_LOCAL_MACHINE" )
		return HKEY_LOCAL_MACHINE;
	else if( strKey == "HKEY_CLASSES_ROOT" )
		return HKEY_CLASSES_ROOT;
	else if( strKey == "HKEY_CURRENT_USER" )
		return HKEY_CURRENT_USER;
	else
		return NULL;
}