// AtsSentence.h: interface for the CAtsSentence class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (������)
// 2021/5/1
// ver:1.8
// �������ʹ�ã����������������ϵ����(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATSSENTENCE_H__70B73087_E62E_462C_9703_E7D132B5AD56__INCLUDED_)
#define AFX_ATSSENTENCE_H__70B73087_E62E_462C_9703_E7D132B5AD56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	ATS_SENTENCE_TYPE_NONE		(0)
#define	ATS_SENTENCE_TYPE_IF		(1)
#define	ATS_SENTENCE_TYPE_WHILE		(2)		//While...Wend ���
#define	ATS_SENTENCE_TYPE_DO		(3)		//Do...loop ���
#define	ATS_SENTENCE_TYPE_SWITCH	(4)		//switch ���
#define	ATS_SENTENCE_TYPE_FOR		(5)		//for ���

#include <afx.h>
#include "AtsString.h"

class CAtsSegIns;
////////////////////////////////////////////////////
// ��� ����
////////////////////////////////////////////////////
class CAtsSentence : public CObject  
{
public:
	CAtsSentence();
	virtual ~CAtsSentence();

	int				m_nType;
	int				m_nBeginPos;
	int				m_nEndPos;
	CAtsString		m_strCondition;
	CAtsSegIns		*m_pSegIns;
	CObList			m_listSite;
	CAtsString		m_strInitCmd;		// �����for ���,�ŵĳ�ʼ��������
	CAtsString		m_strNextCmd;	    // �����for ���,�ŵ����´�ִ��ʱ������

public:
	BOOL			IsNoBracket();
	BOOL			IsEndSent(int nLinePos);
};


#endif // !defined(AFX_ATSSENTENCE_H__70B73087_E62E_462C_9703_E7D132B5AD56__INCLUDED_)
