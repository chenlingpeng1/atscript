// AtsSegmentItem.cpp: implementation of the CAtsSegmentItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtsSegmentItem.h"
#include "AtsCommdef.h"
#include "AtsVarItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAtsSegmentItem::CAtsSegmentItem()
{
	m_nBeginPos		= -1;
	m_nSegmentType	= 0;
}

CAtsSegmentItem::~CAtsSegmentItem()
{
	DetachList();
}

void CAtsSegmentItem::DetachList()
{
	m_listData.clear();
}

void CAtsSegmentItem::SetReturnDataType(int nDatType)
{
	m_Param.m_vReturn.m_nType = nDatType;
}

void CAtsSegmentItem::AddLine(CAtsString strLine , int nSourceLine , int nIndexOnLine)
{
	CScriptLine		Line; 

	Line.m_strLine = strLine;
	Line.m_nSource_Line		= nSourceLine;
	Line.m_nSource_LinePos	= nIndexOnLine;
	m_listData.push_back( Line );
}

BOOL CAtsSegmentItem::InsertLine( int nLine , CAtsString strLine , int nSourceLinePos , int nIndexOnLine)
{
	CScriptLine		Line; 

	list<CScriptLine>::iterator it;
	int					nIndex = 0;
	int					nSize = m_listData.size();
		
	if( nLine < 0 || nLine > nSize )
		return false;
	if( nLine == nSize )
	{
		AddLine( strLine , nSourceLinePos , nIndexOnLine);
		return true;
	}

	for( it = m_listData.begin(); it != m_listData.end(); it++)
	{
		if( nIndex == nLine )
			break;
		nIndex++;
	}

	Line.m_strLine			= strLine;
	Line.m_nSource_Line		= nSourceLinePos;
	Line.m_nSource_LinePos	= nIndexOnLine;
	m_listData.insert( it , Line );
	return true;
}
BOOL CAtsSegmentItem::GetSourneLine(int nLineIndex , int &nSourceLine, int &nSourceLinePos )
{
	list<CScriptLine>::iterator it;
	int							nIndex = 0;

	if( nLineIndex < 0 )
		return false;

	for( it = m_listData.begin(); it != m_listData.end(); it++)
	{
		if( nIndex == nLineIndex )
		{
			nSourceLine		= ((CScriptLine *)&*it)->m_nSource_Line;
			nSourceLinePos	= ((CScriptLine *)&*it)->m_nSource_LinePos;
			return true;
		}
		nIndex++;
	}
	nSourceLine		= -1;
	nSourceLinePos	= -1;
	return false;
}

BOOL CAtsSegmentItem::GetLineString(int nLinePos , CAtsString &strLine )
{
	list<CScriptLine>::iterator it;
	int							nIndex = 0;

	strLine = "";
	for( it = m_listData.begin(); it != m_listData.end(); it++)
	{
		if( nIndex == nLinePos )
		{
			strLine					= ((CScriptLine *)&*it)->m_strLine;
			return true;
		}

		nIndex++;
	}
	return false;
}
int CAtsSegmentItem::GetTagIndex( CAtsString strTagName )
{
	CAtsString			strData;
	int					nIndex = 0;

	list<CScriptLine>::iterator it = m_listData.begin();
	for ( ; it != m_listData.end(); it++ )
	{
		strData = ((CScriptLine *)&*it)->m_strLine;
		if( strData.Right(1) == ":" )
		{
			strData.TrimRight(':');
			if( strData.CompareNoCase(strTagName) == 0 )
				return nIndex;
		}
		nIndex++;
	}
	return -1;
}
BOOL CAtsSegmentItem::IsLiftBracket(int nLinePos)
{
	CAtsString		strLine;

	GetLineString(nLinePos , strLine );
	if( strLine == "{" )
		return true;
	else
		return false;
}
int CAtsSegmentItem::GetSectionEndLinePos(int nBeginLinePos, BOOL bAutoAddBracket)
{
	int				nLeftBracket = 0;
	CAtsString		strLine;
	int				nPos;
	
	if( !IsLiftBracket(nBeginLinePos) )
	{
		if( bAutoAddBracket )
		{
			InsertLine( nBeginLinePos,  "{" );
			InsertLine( nBeginLinePos+2,  "}" );
			return nBeginLinePos+2;
		}	
		else
			return nBeginLinePos;
	}
	else
		nLeftBracket++;

	nPos = nBeginLinePos+1;
	while( nPos < m_listData.size() )
	{
		GetLineString(nPos, strLine );
		if( strLine.CompareNoCase("{") == 0 ) nLeftBracket++;
		if( strLine.CompareNoCase("}") == 0 ) nLeftBracket--;
		if( nLeftBracket == 0 )
			return nPos;
		
		nPos++;
	}
	return -1;
}