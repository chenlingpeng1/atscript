// AtsSegIns.h: interface for the CAtsSegIns class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (������)
// 2021/5/1
// ver:1.8
// �������ʹ�ã����������������ϵ����(905309882@qq.com)
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
// Segment ��ʵ��������( function , sub )
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
	int					m_nLineErrorPos;		// �������
	CAtsSegmentItem	*	m_pSegmentItem;
	
	CAtsParam			m_InParam;				// ת��Ĳ���
	int					m_nNextPos;
	int					m_nCurLinePos;
	CPtrList			m_listUserVar;			// ��ǰ���е��û�����ı���,����Ϊ @VarName
	CObList				m_listSentence;
	CObList				m_listSentenceHeap;		// ���ѣ���Ϊ, if while ... ���ܻ�Ƕ��
												// ֻ��ָ��,����ɾ������
};

#endif // !defined(AFX_ATSSEGINS_H__ACBC07F3_6DF6_46D8_8CDC_7078CF1F55FC__INCLUDED_)
