// WinApiIns.cpp: implementation of the CDllModule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DllModule.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDllModule::CProcDef::CProcDef()
{
}

CDllModule::CProcDef::~CProcDef()
{
}

BOOL CDllModule::CProcDef::GetElementItem(CString &strStuff, CString &strItem,CString strApart)
{
	if( strStuff.GetLength() == 0 )
		return FALSE;

	int pos = strStuff.Find(strApart);
	if( pos >= 0 )
	{
		strItem = strStuff.Left(pos);
		strStuff = strStuff.Right( strStuff.GetLength() - pos - strApart.GetLength() );
		strItem.TrimLeft(' ');
		strItem.TrimRight(' ');
		strStuff.TrimLeft(' ');
		strStuff.TrimRight(' ');
	}
	else
	{
		strItem		= strStuff;
		strStuff	= "";
		strItem.TrimLeft(' ');
		strItem.TrimRight(' ');
	}
	return TRUE;
}

//========================================================
// 可以分开三种符号: ' ' , '(' , '='
// 如: if ... then
//     @a=3
//     Myfun()
//========================================================
void CDllModule::CProcDef::GetFistWord( CString strLine, CString &strFistWord, CString &strOtherData )
{
	CString			strData;
	int				nPos = -1;
	char			cItem;
	int				nNum,nCount;

	strLine.TrimLeft(' ');
	nCount = strLine.GetLength();
	for( nNum = 0 ; nNum < nCount ; nNum++ )
	{
		cItem = strLine.GetAt(nNum);
		if( cItem == ' ' || cItem == '(' || cItem == '=' || 
			cItem == '+' || cItem == '-' || cItem == '*' ||
		    cItem == '/' || cItem == '%')
		{
			nPos = nNum;
			break;
		}
	}
	if( nPos < 0 )
	{
		strOtherData	= _T("");
		strFistWord		= strLine;
		return;
	}

	strFistWord  = strLine.Left(nPos);
	strOtherData = strLine.Mid(nPos);

	strFistWord.TrimLeft(' ');
	strFistWord.TrimRight(' ');

	strOtherData.TrimLeft(' ');
	strOtherData.TrimRight(' ');
}

//==============================================================================
// 增加 Proc 定义
// 如： AddProcDefine( "int DllApi(string , int , out char 1024 , int )" );
//==============================================================================
BOOL CDllModule::CProcDef::LoadData( CString strDefine )
{
	CString		strFistWord;
	CString		strItem;

	GetFistWord( strDefine, m_strReturnType, strDefine );
	GetFistWord( strDefine, m_strProcName, strDefine );
	if( m_strReturnType.IsEmpty() || m_strProcName.IsEmpty() )
		return false;

	strDefine.TrimLeft("(");
	strDefine.TrimRight(")");

	while( GetElementItem( strDefine , strItem, "," ) )
	{
		// 得到参数的类型
		GetFistWord( strItem, strFistWord, strItem );
		if( strFistWord.CompareNoCase("out") == 0 )
		{
			GetFistWord( strItem, strFistWord, strItem );
			m_anParamIsOut.Add(1);
		}
		else
		{
			m_anParamIsOut.Add(0);
		}
		m_asParamType.Add( strFistWord );

		// 得到参数的大小
		m_anParamSize.Add( atoi(strItem) );
	}

	return true;
}


CDllModule::CProcDef * CDllModule::CProcDef::GetProcDef( CObList &listDef, CString strName )
{
	POSITION		pos;
	CProcDef	*pItem;

	for( pos = listDef.GetHeadPosition() ; pos ; )
	{
		pItem = (CProcDef *)listDef.GetNext( pos );
		if( pItem->m_strProcName.CompareNoCase(strName) == 0 )
			return pItem;
	}
	return NULL;
}

void CDllModule::CProcDef::DetachList( CObList &listDef )
{
	POSITION		pos;
	CProcDef	*pItem;

	for( pos = listDef.GetHeadPosition() ; pos ; )
	{
		pItem = (CProcDef *)listDef.GetNext( pos );
		delete pItem;
	}
	listDef.RemoveAll();
}

void CDllModule::CProcDef::DeleteDef( CObList &listDef , CString strProName )
{
	POSITION		pos;
	CProcDef	*pItem;

	for( pos = listDef.GetHeadPosition() ; pos ; )
	{
		pItem = (CProcDef *)listDef.GetNext( pos );
		if( pItem->m_strProcName.CompareNoCase( strProName ) == 0 )
		{
			delete pItem;
			return;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDllModule::CDllModule()
{
	m_hLibIns = NULL;
}

CDllModule::~CDllModule()
{
	CDllModule::CProcDef::DetachList( m_listProcDef );

	if( m_hLibIns )
		FreeLibrary(m_hLibIns);
}

BOOL CDllModule::LoadLibrary( CString strFile )
{
	m_hLibIns = ::LoadLibraryEx( strFile , NULL , LOAD_WITH_ALTERED_SEARCH_PATH );
	if( !m_hLibIns )
		return false;

	return true;
}

//==============================================================================
// 增加 Proc 定义
// 如： AddProcDefine( "int DllApi(string , int , out char 1024 , int )" );
//==============================================================================
BOOL CDllModule::AddProcDefine(CString strDefine )
{
	CDllModule::CProcDef	*pProcDef = new CDllModule::CProcDef;

	if( !pProcDef->LoadData( strDefine ) )
	{
		delete pProcDef;
		return false;
	}

	CDllModule::CProcDef::DeleteDef( m_listProcDef , pProcDef->m_strProcName );
	m_listProcDef.AddTail( pProcDef );

	return true;
}

BOOL CDllModule::CallProc( CString strProcName , CStringArray &asParam , CString &strReturn)
{
	FARPROC					pFun;
	int						nNum,nCount;
	CDllModule::CProcDef	*pProcDef;
	int						nIndex;
	char					*pData;
	char					*pDataSet[10];
	int						nCharDataCount = 0;
	int						nParamSize;
	long					lReturn;
		
	pProcDef = CDllModule::CProcDef::GetProcDef(m_listProcDef, strProcName );
	if( !pProcDef )
		return false;

	pFun = ::GetProcAddress( m_hLibIns, strProcName );
	if( !pFun )
		return false;

	nCount = asParam.GetSize();
	nIndex = 0;
	for( nNum = nCount - 1 ; nNum >= 0 ; nNum-- )
	{
		CString	strParamType;
	
		strParamType = pProcDef->m_asParamType.GetAt( nNum );
		if( strParamType.CompareNoCase("int") == 0 )
		{
			int		nData = atoi( asParam.GetAt(nNum) );
			__asm 
			{ 
				PUSH nData
			}
		}
		else if( strParamType.CompareNoCase("long") == 0 )
		{
			long		lData = atol( asParam.GetAt(nNum) );
			__asm 
			{ 
				PUSH lData
			}
		}
		else if( strParamType.CompareNoCase("mstring") == 0 ||
				 strParamType.CompareNoCase("astring") == 0 ||
				 strParamType.CompareNoCase("char") == 0 )
		{
			CString		strData = asParam.GetAt(nNum);

			nParamSize = pProcDef->m_anParamSize.GetAt( nNum );
			if( nParamSize <= 0 )
				nParamSize = strData.GetLength() + 1;
			pData = new char[nParamSize];

			memset( pData , 0 , nParamSize);
			if( pProcDef->m_anParamIsOut.GetAt( nNum ) == 0 )
				strcpy( pData , strData );
			__asm 
			{
				MOV ECX, dword ptr pData ; 局部变量用
				PUSH ECX 
			}
			pDataSet[nCharDataCount] = pData;
			nCharDataCount++;
		}
		else if( strParamType.CompareNoCase("wstring") == 0 )
		{
		}

		nIndex++;
	}

	// 设置返回值的存放位置, 执行结果是放在 EAX 中的 
	/*if( pProcDef->m_strReturnType.CompareNoCase("long") == 0 )
	{
		__asm 
		{ 
			MOV EAX, lReturn
		}
	}*/
	
	__asm 
	{ 
		CALL pFun
	}

	for( nNum = 0 ; nNum < nCharDataCount ; nNum++ )
	{
		pData = pDataSet[nNum];
		delete []pData;
	}

	return true;
}