// AtsSentIns.h: interface for the CAtsSentIns class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (������)
// 2021/5/1
// ver:1.8
// �������ʹ�ã����������������ϵ����(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATSSENTINS_H__E231D802_FACF_4FC8_ACFD_E04E4A60978F__INCLUDED_)
#define AFX_ATSSENTINS_H__E231D802_FACF_4FC8_ACFD_E04E4A60978F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afx.h>
#include "AtsString.h"
#include "AtsSentence.h"

//==============================================
// CAtsSentence ��ʵ��������( �� if, while , ... )
//==============================================
class CAtsSentIns : public CObject  
{
public:
	CAtsSentIns();
	virtual ~CAtsSentIns();

	CAtsSentence		*m_pSentence;
	int					m_nRunCount;		// ���еĴ���
};

#endif // !defined(AFX_ATSSENTINS_H__E231D802_FACF_4FC8_ACFD_E04E4A60978F__INCLUDED_)
