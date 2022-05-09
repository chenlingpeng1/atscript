// AtsSegIns.h: interface for the CAtsSegIns class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (陈凌鹏)
// 2021/5/1
// ver:1.8
// 代码免费使用，如果用于商务，请联系作者(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATSSEGINS_H__ACBC07F3_6DF6_46D8_8CDC_7078CF1F55FC__INCLUDED_)
#define AFX_ATSSEGINS_H__ACBC07F3_6DF6_46D8_8CDC_7078CF1F55FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtsSegmentItem.h"
#include "AtsSentence.h"
#include "AtsVarItem.h"
#include "AtsSentIns.h"

class CAtsEngine;

//==============================================
// Segment 的实例对象类( function , sub )
//==============================================
class ATS_API CAtsSegIns : public CObject  
{
public:
	CAtsSegIns();
	virtual ~CAtsSegIns();

	CAtsVarItem *	NewVarItem(CAtsString strVarName , CString strType );
	void			SentPop_Cant_Break();
	BOOL			GetNextCmdLine(CAtsString &strCmdLine , BOOL bAddPos = true);
	BOOL			GetCmdLine( int nPos , CAtsString &strCmdLine );
	CAtsSentence *	GetSentence_While(int nBeginPos , CAtsString strCmdLine);
	CAtsSentence *	GetSentence_Switch(int nBeginPos , CAtsString strCmdLine);
	CAtsSentence *	GetSentence_Do(int nBeginPos , CAtsString strCmdLine);
	CAtsSentence *	GetSentence_for(int nBeginPos , CAtsString strCmdLine);
	CAtsSentence *	GetSentence_If(int nBeginPos , CAtsString strCmdLine);
	void			DetachList(CAtsString strListName = _T(""));
	BOOL			LoadParamData( CAtsParam *pParam );

	CAtsSentIns *	SentTop();
	void			RemoveTopSentIns();
	CAtsSentence *	FinSentence( int nBeginPos );
	CAtsVarItem *	GetVarItem(LPCTSTR sName );

	CAtsEngine		*	m_pEngine;
	int					m_nLineErrorPos;		// 出错的行
	CAtsSegmentItem	*	m_pSegmentItem;
	
	CAtsParam			m_InParam;				// 转入的参数
	int					m_nNextPos;
	int					m_nCurLinePos;
	CPtrList			m_listUserVar;			// 当前段中的用户定义的变量,规则为 @VarName
	CObList				m_listSentence;
	CObList				m_listSentenceHeap;		// 语句堆，因为, if while ... 可能会嵌套
												// 只存指针,不用删除数据
};

#endif // !defined(AFX_ATSSEGINS_H__ACBC07F3_6DF6_46D8_8CDC_7078CF1F55FC__INCLUDED_)
