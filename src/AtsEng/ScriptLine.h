// ScriptLine.h: interface for the CScriptLine class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (陈凌鹏)
// 2021/5/1
// ver:1.8
// 代码免费使用，如果用于商务，请联系作者(905309882@qq.com)
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
	int				m_nSource_Line;		// 在原码中的行号
	int				m_nSource_LinePos;	// 在原码中的行的开始位置
};

#endif // !defined(AFX_SCRIPTLINE_H__B67C3388_377D_4B84_A6E7_04EC319D7CB0__INCLUDED_)
