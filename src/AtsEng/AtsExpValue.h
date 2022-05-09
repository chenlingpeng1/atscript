//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (陈凌鹏)
// 2021/5/1
// ver:1.8
// 代码免费使用，如果用于商务，请联系作者(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FVALUE_H__8AE9B2FD_1F4D_490B_8D9F_F34034F18592__INCLUDED_)
#define AFX_FVALUE_H__8AE9B2FD_1F4D_490B_8D9F_F34034F18592__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Afx.h"
#include "AtsString.h"

class CAtsExpOpItem : public CObject  
{
public:
	CAtsExpOpItem();
	virtual ~CAtsExpOpItem();

	int				m_nType;
	CAtsString		m_strName;
	int				m_nPos;		// 在整个表达试中的开始位置

	int GetOPLevel();
	bool IsMastCal();
};

#endif // !defined(AFX_FVALUE_H__8AE9B2FD_1F4D_490B_8D9F_F34034F18592__INCLUDED_)
