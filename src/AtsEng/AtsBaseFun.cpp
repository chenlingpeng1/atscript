// AtsBaseFun.cpp: implementation of the CAtsBaseFun class.
//
//////////////////////////////////////////////////////////////////////

#include "AtsBaseFun.h"
#include "objbase.h"
#include "AtsCommDef.h"
#include "ValueSet.h"
#include "AtsEngine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAtsBaseFun::CAtsBaseFun()
{

}

CAtsBaseFun::~CAtsBaseFun()
{

}

CAtsString CAtsBaseFun::GetPathName( CAtsString strFileName )
{
	int			nPos;
	CAtsString		strPathName;

	if ( strFileName.IsEmpty() )
		return	_T( "" );

	nPos	= strFileName.ReverseFind( '\\' );
	if ( nPos < 0 )
		return	_T( "" );

	strPathName	= strFileName.Left( nPos );
	return	strPathName;
}

BOOL CAtsBaseFun::GetElementItem(CAtsString &strStuff, CAtsString &strItem,CAtsString strApart)
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


int CAtsBaseFun::CharToInt(char ch)
{
	switch( ch )
	{
	case 'F' :
	case 'f' :
		return 15;
	case 'E' :
	case 'e' :
		return 14;
	case 'D' :
	case 'd' :
		return 13;
	case 'C' :
	case 'c' :
		return 12;
	case 'B' :
	case 'b' :
		return 11;
	case 'A' :
	case 'a' :
		return 10;
	default  :
		return ch - '0';
	}

	return 0;
}


//===============================================
// 得到转义字符
//===============================================
char CAtsBaseFun::GetEscapeChar(char c)
{
	if( c == '\\' )
		return '\\';
	else if( c == 'n' )
		return '\n';
	else if( c == 't' )
		return '\t';
	else if( c == 'r' )
		return '\r';
	else if( c == '\'' )
		return '\'';
	else if( c == '"' )
		return '"';
	else
		return c;
}

//===============================================
// 得到转义字符 \xAB
//===============================================
char CAtsBaseFun::GetEscapeChar_x(char c1, char c2)
{
	char		c;

	c =	CharToInt( c1 ) * 16 + CharToInt( c2 );
	return c;
}

CAtsString CAtsBaseFun::GenerateGUIDString()
{
/*	CAtsString	str;
	GUID	guid = GUID_NULL;
	::CoCreateGuid(&guid);

	ASSERT(guid != GUID_NULL);// call ::CoInitialize() in CWinApp::OnInitIntance()
	                          // to avoid it
	CAtsString strFormat = "%08lX-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X";
	str.Format(strFormat,
		// first copy...
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7],
		// second copy...
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
	return str;
*/
	CAtsString strGUID;
	GUID	guid;

	if(CoCreateGuid( &guid ) == S_OK)
	{
		OLECHAR sessionID[MAX_PATH];
		StringFromGUID2(guid, sessionID, MAX_PATH);
		strGUID = sessionID;
	}

	return strGUID;
}

//======================================
// 得到 OP 的等级，1 为马上要执行的 OP
// 是否为要马上进行计划的符号
//======================================
int CAtsBaseFun::GetOPLevel(int nOpType , CAtsString strData)
{
	if( nOpType == ATSOP_TYPE_BINOP )
	{
		if( strData == "*"  || strData == "/"   || 
			strData == "*=" || strData == "/=" ||
			strData == "+=" || strData == "-=" ||
			strData == "%"  ||strData == "%=")
		{
			return 1;
		}
		else if( strData == "+"  || strData == "-" )
		{
			return 2;
		}
	}
	else if( nOpType == ATSOP_TYPE_SINGLEOP )
		return 1;
	else if( nOpType == ATSOP_TYPE_LOG )
	{
		// 逻辑运算符( & , | )
		return 1;
	}
	else if( nOpType == ATSOP_TYPE_CHECK )
	{
		// 判断运算符,( AND , OR , == , >= , <= , > , < )
		if( strData == "=="  || strData == ">="   || 
			strData == "<=" || strData == ">" ||
			strData == "<"  || strData == "!=" ||
			strData == "<>" )
		{
			return 3;
		}
		else if( strData == "AND"  || strData == "OR" ||
				 strData == "&&"  || strData == "||" )
		{
			return 4;
		}
		return 5;
	}
	else if( nOpType == ATSOP_TYPE_SEG )
		return 6;
	
	return 10000;
}

//=============================================
// 是有效的变量名
//=============================================
BOOL CAtsBaseFun::IsValidVarName(CAtsString strName)
{
	char c = strName.GetAt(0);
	// 第一个字符为数字,则出错
	if( c >= '0' &&  c <= '9' )
		return false;

	return true;
}

//=============================================
// 是有效的变量名
//=============================================
void CAtsBaseFun::LaunchApp(const char* temp_exe, BOOL bSync)
{
	HANDLE hProcess;
	HANDLE hThread;
	PROCESS_INFORMATION PI;
	STARTUPINFO SI;

	memset(&SI, 0, sizeof(SI));
	SI.cb = sizeof(SI);
	if( !CreateProcess(temp_exe, NULL, NULL, NULL, FALSE,NORMAL_PRIORITY_CLASS, NULL, NULL, &SI, &PI) )
	{
		CString		strError;
		strError.Format("CreateProcess is error: %s" , temp_exe );
		MessageBox( NULL , strError , "AtScript" , 0 );
		return;
	}

	hProcess	= PI.hProcess;       
	hThread		= PI.hThread;
	
	if( bSync )  //同步执行
	{
		// 方法1:
		//WaitForSingleObject(hProcess, INFINITE);

		// 方法2:
		MSG				msg; 
		HANDLE			hHandle[1];
		hHandle[0]		= hProcess;
		while( true )
		{
			DWORD result = ::MsgWaitForMultipleObjects( 1, hHandle, FALSE, 0, 0 );
			if ( result == (WAIT_OBJECT_0) )//成功
			{
				break;
			}

			//有消息到达
			if( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
			{   
				// Get Next message in queue
				 if (msg.message == WM_QUIT)
				 {
					 break;
				 } // end if
				 TranslateMessage(&msg);
				 DispatchMessage(&msg);
			}
		}
	}
}

//=========================================
// 得到 ValueSet 的参数
//=========================================
CValueSet * CAtsBaseFun::GetValueSetParam( CAtsParam *pParam , int nOrder )
{
	CAtsObject		*pObj = NULL;
	CAtsVarItem *	pVarItrem = pParam->GetItem(nOrder);
	if( pVarItrem && pVarItrem->GetVarType() == ATSVAR_TYPE_VAR )
	{
		CAtsVarItem * pObjVar = pParam->m_pEng->GetVarItem( pParam->m_pParentSegIns , pVarItrem->GetDataToStr() );
		if( pObjVar && pObjVar->GetVarType() == ATSVAR_TYPE_ATSOBJECT )
		{
			pObj = pObjVar->GetValueObjData();
			if( pObj->m_strClassName.CompareNoCase("CValueSet") == 0 )
				return (CValueSet *)pObj;
		}
	}
	return NULL;
}