// AtsSentence.cpp: implementation of the CAtsSentence class.
//
//////////////////////////////////////////////////////////////////////

#include "AtsSentence.h"
#include "AtsSegIns.h"
#include "AtsFile.h"
#include "AtsCommDef.h"
#include "SentenceSite.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAtsSentence::CAtsSentence()
{
	m_nType			= ATS_SENTENCE_TYPE_NONE;
	m_nBeginPos		= -1;
	m_nEndPos		= -1;
	m_pSegIns		= NULL;
}

CAtsSentence::~CAtsSentence()
{
	POSITION		pos;
	CSentenceSite	*pItem = NULL;

	for( pos = m_listSite.GetHeadPosition() ; pos ; )
	{
		pItem = ( CSentenceSite *)m_listSite.GetNext( pos );
		delete pItem;
	}
	m_listSite.RemoveAll();
}

//===============================
// ц╩сп {}
//===============================
BOOL CAtsSentence::IsNoBracket()
{
	if( m_nBeginPos == m_nEndPos )
		return true;
	else
		return false;
}

BOOL CAtsSentence::IsEndSent(int nLinePos)
{
	if( m_nEndPos == nLinePos )
		return true;

	POSITION		pos;
	CSentenceSite	*pItem = NULL;

	for( pos = m_listSite.GetHeadPosition() ; pos ; )
	{
		pItem = ( CSentenceSite *)m_listSite.GetNext( pos );
		if( pItem->m_nEndPos == nLinePos )
			return true;
	}

	return false;
}