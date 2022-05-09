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
		// ������ New ������û�н��� AddRef �Ķ���
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