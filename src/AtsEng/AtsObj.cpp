// AMObj.cpp: implementation of the CAtsObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtsObj.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAtsObj::CAtsObj()
{
	m_nRef = 0;
}

CAtsObj::~CAtsObj()
{
}

long CAtsObj::AddRef()
{
	return InterlockedIncrement(&m_nRef);
}

long CAtsObj::Release()
{
	if(m_nRef == 0)
	{
		// 兼容在 New 出来后没有进行 AddRef 的对象
		delete this;
		return 0;
	}

	if( ::InterlockedDecrement(&m_nRef) == 0)
	{
		delete this;
		return 0;
	}

	return m_nRef;
}