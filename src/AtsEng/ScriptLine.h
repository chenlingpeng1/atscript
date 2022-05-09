// ScriptLine.h: interface for the CScriptLine class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (������)
// 2021/5/1
// ver:1.8
// �������ʹ�ã����������������ϵ����(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCRIPTLINE_H__B67C3388_377D_4B84_A6E7_04EC319D7CB0__INCLUDED_)
#define AFX_SCRIPTLINE_H__B67C3388_377D_4B84_A6E7_04EC319D7CB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtsBaseFun.h"
#include "objbase.h"

class CScriptLine  
{
public:
	CScriptLine();
	virtual ~CScriptLine();

	CAtsString		m_strLine;
	int				m_nSource_Line;		// ��ԭ���е��к�
	int				m_nSource_LinePos;	// ��ԭ���е��еĿ�ʼλ��
};

#endif // !defined(AFX_SCRIPTLINE_H__B67C3388_377D_4B84_A6E7_04EC319D7CB0__INCLUDED_)
