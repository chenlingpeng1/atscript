// SentenceSite.h: interface for the CSentenceSite class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (陈凌鹏)
// 2021/5/1
// ver:1.8
// 代码免费使用，如果用于商务，请联系作者(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SENTENCESITE_H__5716B0EF_F278_4E7E_9A0C_69F72DE32383__INCLUDED_)
#define AFX_SENTENCESITE_H__5716B0EF_F278_4E7E_9A0C_69F72DE32383__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afx.h>
#include "AtsString.h"

////////////////////////////////////////////////////
// 语句对象中的 段
////////////////////////////////////////////////////
class CSentenceSite : public CObject  
{
public:
	CSentenceSite();
	virtual ~CSentenceSite();

	int				m_nBeginPos;
	int				m_nEndPos;
	CAtsString		m_strCondition;
};

#endif // !defined(AFX_SENTENCESITE_H__5716B0EF_F278_4E7E_9A0C_69F72DE32383__INCLUDED_)
