// AtsSentence.h: interface for the CAtsSentence class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (陈凌鹏)
// 2021/5/1
// ver:1.8
// 代码免费使用，如果用于商务，请联系作者(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATSSENTENCE_H__70B73087_E62E_462C_9703_E7D132B5AD56__INCLUDED_)
#define AFX_ATSSENTENCE_H__70B73087_E62E_462C_9703_E7D132B5AD56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	ATS_SENTENCE_TYPE_NONE		(0)
#define	ATS_SENTENCE_TYPE_IF		(1)
#define	ATS_SENTENCE_TYPE_WHILE		(2)		//While...Wend 语句
#define	ATS_SENTENCE_TYPE_DO		(3)		//Do...loop 语句
#define	ATS_SENTENCE_TYPE_SWITCH	(4)		//switch 语句
#define	ATS_SENTENCE_TYPE_FOR		(5)		//for 语句

#include <afx.h>
#include "AtsString.h"

class CAtsSegIns;
////////////////////////////////////////////////////
// 语句 对象
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
	CAtsString		m_strInitCmd;		// 如果是for 语句,放的初始化的命令
	CAtsString		m_strNextCmd;	    // 如果是for 语句,放的是下次执行时的命令

public:
	BOOL			IsNoBracket();
	BOOL			IsEndSent(int nLinePos);
};


#endif // !defined(AFX_ATSSENTENCE_H__70B73087_E62E_462C_9703_E7D132B5AD56__INCLUDED_)
