// AtsSegmentItem.h: interface for the CAtsSegmentItem class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (陈凌鹏)
// 2021/5/1
// ver:1.8
// 代码免费使用，如果用于商务，请联系作者(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATSSEGMENTITEM_H__EA376B98_D781_4234_B4D0_12301DF236E9__INCLUDED_)
#define AFX_ATSSEGMENTITEM_H__EA376B98_D781_4234_B4D0_12301DF236E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define		SEGMENT_TYPE_SECTION	(1)
#define		SEGMENT_TYPE_FUNCTION	(2)

#include "AtsDef.h"
#include "AtsParam.h"
#include "ScriptLine.h"
#include <vector>
#include <list>

//======================================================
// 如 function , sub
//======================================================
class ATS_API CAtsSegmentItem : public CObject  
{
public:
	CAtsSegmentItem();
	virtual ~CAtsSegmentItem();

	BOOL GetSourneLine(int nLineIndex , int &nSourceLine, int &nSourceLinePos );
	int	 GetSectionEndLinePos(int nBeginLinePos, BOOL bAutoAddBracket = false);
	BOOL IsLiftBracket(int nLinePos);
	void DetachList();
	int  GetTagIndex( CAtsString strTagName );
	void SetReturnDataType(int nDatType);
	void AddLine( CAtsString strLine , int nSourceLinePos = -1, int nIndexOnLine = -1);
	BOOL GetLineString(int nLinePos, CAtsString &strLine );
	BOOL InsertLine( int nLine , CAtsString strLine , int nSourceLinePos = -1, int nIndexOnLine = -1);

	int						m_nBeginPos;
	int						m_nSegmentType;
	CAtsString				m_strName;
	list<CScriptLine>		m_listData;
	CAtsParam				m_Param;
};

#endif // !defined(AFX_ATSSEGMENTITEM_H__EA376B98_D781_4234_B4D0_12301DF236E9__INCLUDED_)
