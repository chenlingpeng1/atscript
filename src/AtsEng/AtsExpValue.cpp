// FValue.cpp: implementation of the CAtsExpValue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtsExpValue.h"
#include "AtsCommdef.h"
#include "AtsBaseFun.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAtsExpOpItem::CAtsExpOpItem()
{
	m_nPos	= -1;
	m_nType = ATSOP_TYPE_BINOP;
}

CAtsExpOpItem::~CAtsExpOpItem()
{
}
bool CAtsExpOpItem::IsMastCal()
{
	if( GetOPLevel() == 1 )
		return true;
	else
		return false;
}
int CAtsExpOpItem::GetOPLevel()
{
	return CAtsBaseFun::GetOPLevel( m_nType , m_strName );
}
