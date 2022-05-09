// SentenceSite.h: interface for the CSentenceSite class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (������)
// 2021/5/1
// ver:1.8
// �������ʹ�ã����������������ϵ����(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SENTENCESITE_H__5716B0EF_F278_4E7E_9A0C_69F72DE32383__INCLUDED_)
#define AFX_SENTENCESITE_H__5716B0EF_F278_4E7E_9A0C_69F72DE32383__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afx.h>
#include "AtsString.h"

////////////////////////////////////////////////////
// �������е� ��
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
