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

char CAtsBaseFun::GetEscapeChar_x(char c1, char c2)
{
	char		c;
	c =	CharToInt( c1 ) * 16 + CharToInt( c2 );
	return c;
}

CAtsString CAtsBaseFun::GenerateGUIDString()
{
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
		return 1;
	}
	else if( nOpType == ATSOP_TYPE_CHECK )
	{
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

BOOL CAtsBaseFun::IsValidVarName(CAtsString strName)
{
	char c = strName.GetAt(0);
	if( c >= '0' &&  c <= '9' )
		return false;
	return true;
}