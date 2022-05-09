// AtsSegIns.cpp: implementation of the CAtsSegIns class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtsSegIns.h"
#include "AtsSentence.h"
#include "AtsFile.h"
#include "AtsCommDef.h"
#include "SentenceSite.h"
#include "AtsVarItem.h"
#include "AtsBaseFun.h"
#include "AtsEngine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAtsSegIns::CAtsSegIns()
{
	m_nLineErrorPos = -1;
	m_pSegmentItem	= NULL;
	m_pEngine		= NULL;
	m_nNextPos		= 0;
}

CAtsSegIns::~CAtsSegIns()
{
	DetachList();
}

CAtsVarItem * CAtsSegIns::NewVarItem(CAtsString strVarName , CString strType )
{
	CAtsVarItem	*pVarItem = m_pEngine->NewVarItem( strVarName , strType );
	if( !pVarItem )
		return NULL;

	pVarItem->AddRef();
	m_listUserVar.AddTail( pVarItem );
	return pVarItem;
}

void CAtsSegIns::DetachList(CAtsString strListName)
{
	POSITION			pos;
	CAtsVarItem			*pVarItem = NULL;
	CAtsSentence		*pItem = NULL;
	CAtsSentIns			*pSentIns = NULL;

	if( strListName.IsEmpty() || strListName == "m_listSentence" )
	{
		for( pos = m_listSentence.GetHeadPosition() ; pos ; )
		{
			pItem = ( CAtsSentence *)m_listSentence.GetNext( pos );
			delete pItem;
		}
		m_listSentence.RemoveAll();
	}

	if( strListName.IsEmpty() || strListName == "m_listUserVar" )
	{
		for( pos = m_listUserVar.GetHeadPosition() ; pos ; )
		{
			pVarItem = (CAtsVarItem *)m_listUserVar.GetNext(pos); 
			pVarItem->Release();
		}
		m_listUserVar.RemoveAll();

		if( !strListName.IsEmpty() )
			return;
	}
	if( strListName.IsEmpty() || strListName == "m_listSentenceHeap" )
	{
		for( pos = m_listSentenceHeap.GetHeadPosition() ; pos ; )
		{
			pSentIns = (CAtsSentIns *)m_listSentenceHeap.GetNext(pos); 
			delete pSentIns;
		}
		m_listSentenceHeap.RemoveAll();

		if( !strListName.IsEmpty() )
			return;
	}
}

BOOL CAtsSegIns::GetNextCmdLine(CAtsString &strCmdLine , BOOL bAddPos )
{
	if( !GetCmdLine( m_nNextPos , strCmdLine ) )
		return false;
	
	if( bAddPos )
		m_nNextPos++;

	return true;
}

BOOL CAtsSegIns::GetCmdLine( int nPos , CAtsString &strCmdLine )
{
	strCmdLine = "";
	if( nPos < 0 || nPos >= m_pSegmentItem->m_listData.size() )
		return false;

	m_nCurLinePos	= nPos;
	m_pSegmentItem->GetLineString( nPos , strCmdLine );

	return true;
}

//==================================================
// 得到 if 的语句
// nBeginPos  : 开始的行
// strCmdLine : 开始行的内容
// if()
// {
// }
//==================================================
CAtsSentence * CAtsSegIns::GetSentence_If(int nBeginPos , CAtsString strCmdLine)
{
	CAtsString		strFistWord , strLastWord , strOther;
	CAtsSentence	*pSentence = NULL;
	CAtsString		strCondition;
	int				nPos  = nBeginPos;
	int				nFindPos,nEndPos;
	int				nState;
	BOOL			bIsEnd = false;
	int				nError = 0;

	pSentence = FinSentence( nBeginPos );
	if( pSentence )
		return pSentence;

	CAtsFile::GetFistWord( strCmdLine , strFistWord , strOther );
	if( strFistWord.CompareNoCase("if") != 0 )
		return NULL;
	
	strCmdLine = strOther;

	pSentence					= new CAtsSentence;
	pSentence->m_pSegIns		= this;
	pSentence->m_nType			= ATS_SENTENCE_TYPE_IF;
	pSentence->m_nBeginPos		= nBeginPos;
	
	CSentenceSite	*pItem = new CSentenceSite;
	nEndPos						= m_pSegmentItem->GetSectionEndLinePos(nBeginPos+1);
	pItem->m_nBeginPos			= nBeginPos;
	pItem->m_nEndPos			= nEndPos;	
	pItem->m_strCondition		= strOther;
	pSentence->m_listSite.AddTail( pItem );

	nPos = nEndPos+1;
	while( bIsEnd == false )
	{
		m_pSegmentItem->GetLineString( nPos , strCmdLine );
		CAtsFile::GetFistWord( strCmdLine , strFistWord , strOther );
		if( strFistWord.CompareNoCase("else") == 0 ) 
		{
			if( strOther.IsEmpty() )
			{
				CSentenceSite	*pItem = new CSentenceSite;
				nEndPos						= m_pSegmentItem->GetSectionEndLinePos(nPos+1);
				pItem->m_nBeginPos			= nPos;
				pItem->m_nEndPos			= nEndPos;	
				pSentence->m_listSite.AddTail( pItem );
				bIsEnd = true;

				nPos = nEndPos + 1;
			}
			else 
			{
				CAtsFile::GetFistWord( strOther , strFistWord , strOther );
				if( strFistWord.CompareNoCase("if") == 0 ) 
				{
					if( strOther.IsEmpty() )
						nError = ATS_STATE_ERROR;
					else
					{
						CSentenceSite	*pItem = new CSentenceSite;
						nEndPos						= m_pSegmentItem->GetSectionEndLinePos(nPos+1);
						pItem->m_nBeginPos			= nPos;
						pItem->m_nEndPos			= nEndPos;
						pItem->m_strCondition		= strOther;
						pSentence->m_listSite.AddTail( pItem );

						nPos = nEndPos + 1;
					}
				}
				else
					nError = ATS_STATE_ERROR;
			}
		}
		else
			bIsEnd = true;
	}
	if( nError > 0 )
	{
		delete pSentence;
		return NULL;
	}

	pSentence->m_nEndPos = nPos-1;
	m_listSentence.AddTail( pSentence );
	return pSentence;
}

//==================================================
// 得到 switch 的语句
// nBeginPos  : 开始的行
// strCmdLine : 开始行的内容
// if ... then
// end if
//==================================================
CAtsSentence * CAtsSegIns::GetSentence_Switch(int nBeginPos , CAtsString strCmdLine)
{
	CAtsString		strFistWord , strLastWord , strOther;
	CAtsSentence	*pSentence = NULL;

	pSentence = FinSentence( nBeginPos );
	if( pSentence )
		return pSentence;

	CAtsFile::GetFistWord( strCmdLine , strFistWord , strOther );
	if( strFistWord.CompareNoCase("Switch") != 0 )
		return NULL;

	pSentence					= new CAtsSentence;
	pSentence->m_pSegIns		= this;
	pSentence->m_nType			= ATS_SENTENCE_TYPE_SWITCH;
	pSentence->m_nBeginPos		= nBeginPos;
	pSentence->m_strCondition	= strOther;
	pSentence->m_nEndPos		= m_pSegmentItem->GetSectionEndLinePos(nBeginPos+1);

	if( pSentence->m_nEndPos <= 0 )
	{
		delete pSentence;
		return NULL;
	}

	CAtsString		strCondition;
	int				nPos  = nBeginPos;
	int				nFindPos;
	int				nState;
	int				nSource_Line , nSource_LinePos;

	nPos++;
	while( nPos < pSentence->m_nEndPos )
	{
		m_pSegmentItem->GetLineString(nPos , strCmdLine );
		CAtsFile::GetFistWord( strCmdLine , strFistWord , strOther );
		if( strFistWord.CompareNoCase("case") == 0 ) 
		{
			CSentenceSite	*pItem = new CSentenceSite;

			pItem->m_nBeginPos		= nPos;
			strOther.TrimRight(":");
			pItem->m_strCondition	= strOther;
			pSentence->m_listSite.AddTail( pItem );
		}
		else if( strFistWord.CompareNoCase("Default:") == 0 ) 
		{
			CSentenceSite	*pItem = new CSentenceSite;

			pItem->m_nBeginPos		= nPos;
			pSentence->m_listSite.AddTail( pItem );
		}
		else if( strFistWord.CompareNoCase("switch") == 0 ) 
		{
			// 发现了一个嵌套的子 if
			CAtsSentence * pSubSentence = GetSentence_Switch( nPos , strCmdLine );
			if( pSubSentence )
				nPos = pSubSentence->m_nEndPos;
			else
			{
				delete pSentence;
				return NULL;
			}
		}
		nPos++;
	}

	m_listSentence.AddTail( pSentence );
	return pSentence;
}

//==============================================
// 得到语句堆的最上一个
//==============================================
CAtsSentIns * CAtsSegIns::SentTop()
{
	if( m_listSentenceHeap.GetCount() <= 0 )
		return NULL;

	return (CAtsSentIns *)m_listSentenceHeap.GetHead();
}

//==============================================
// 取出语句堆的最上一个
//==============================================
void CAtsSegIns::RemoveTopSentIns()
{
	if( m_listSentenceHeap.GetCount() <= 0 )
		return;

	CAtsSentIns	*pItem = NULL;
	POSITION		pos;

	pos = m_listSentenceHeap.GetHeadPosition();
	pItem = (CAtsSentIns *)m_listSentenceHeap.GetHead();
	m_listSentenceHeap.RemoveAt( pos );
	delete pItem;
}

//==============================================
// 取出语句堆的不支持 break 指令的段
// 目前只有 if 不支持 break
//==============================================
void CAtsSegIns::SentPop_Cant_Break()
{
	if( m_listSentenceHeap.GetCount() <= 0 )
		return;

	CAtsSentIns		*pItem = NULL;
	POSITION		pos;

	while( true )
	{
		pos = m_listSentenceHeap.GetHeadPosition();
		if( pos )
		{
			pItem = (CAtsSentIns *)m_listSentenceHeap.GetHead();
			if( pItem->m_pSentence->m_nType == ATS_SENTENCE_TYPE_IF  )
			{
				m_listSentenceHeap.RemoveAt( pos );
			}
			else
				break;
		}
		else
			break;
	}
}

//==============================================
// 按 nBeginPos 找到一个语句对象
//==============================================
CAtsSentence * CAtsSegIns::FinSentence( int nBeginPos )
{
	POSITION		pos;
	CAtsSentence	*pItem = NULL;

	for( pos = m_listSentence.GetHeadPosition() ; pos ; )
	{
		pItem = ( CAtsSentence *)m_listSentence.GetNext( pos );
		if( pItem->m_nBeginPos == nBeginPos )
			return pItem;
	}
	return NULL;
}

//==================================================
// 得到 While  语句
// nBeginPos  : 开始的行
// strCmdLine : 开始行的内容
// while()
// {
// }
//==================================================
CAtsSentence * CAtsSegIns::GetSentence_While(int nBeginPos , CAtsString strCmdLine)
{
	CAtsString		strFistWord , strLastWord , strOther;
	CAtsSentence	*pSentence = NULL;

	pSentence = FinSentence( nBeginPos );
	if( pSentence )
		return pSentence;

	CAtsFile::GetFistWord( strCmdLine , strFistWord , strOther );
	if( strFistWord.CompareNoCase("while") != 0 )
		return NULL;

	pSentence					= new CAtsSentence;
	pSentence->m_pSegIns		= this;
	pSentence->m_nType			= ATS_SENTENCE_TYPE_WHILE;
	pSentence->m_nBeginPos		= nBeginPos;
	pSentence->m_strCondition	= strOther;
	pSentence->m_nEndPos		= m_pSegmentItem->GetSectionEndLinePos(nBeginPos+1);
	if( pSentence->m_nEndPos < 0 )
	{
		delete pSentence;
		return NULL;
	}

	m_listSentence.AddTail( pSentence );
	return pSentence;
}

//=======================================================
// 得到变量对象
//=======================================================
CAtsVarItem * CAtsSegIns::GetVarItem(LPCTSTR sName )
{
	POSITION		pos;
	CPtrList		*pList = NULL;
	CAtsVarItem		*pVarItem = NULL;
	CAtsString			strName = sName;

	pList = &m_listUserVar;

	for( pos = pList->GetHeadPosition() ; pos ; )
	{
		pVarItem = (CAtsVarItem *)pList->GetNext(pos); 
		if( pVarItem->m_strName.CompareNoCase( strName ) == 0 )
			return pVarItem;
	}

	return NULL;
}

//==================================================
// 得到 
// Do
// {
// }
// while() 语句
// nBeginPos  : 开始的行
// strCmdLine : 开始行的内容
//==================================================
CAtsSentence * CAtsSegIns::GetSentence_Do(int nBeginPos , CAtsString strCmdLine)
{
	CAtsString		strFistWord , strLastWord , strOther;
	CAtsSentence	*pSentence = NULL;
	CAtsString		strCondition;
	int				nEnd;
	CAtsString		strData;
	int				nSource_Line , nSource_LinePos;

	pSentence = FinSentence( nBeginPos );
	if( pSentence )
		return pSentence;

	CAtsFile::GetFistWord( strCmdLine , strFistWord , strOther );
	if( strFistWord.CompareNoCase("Do") != 0 )
		return NULL;

	nEnd	= m_pSegmentItem->GetSectionEndLinePos(nBeginPos+1);
	m_pSegmentItem->GetLineString(nEnd+1 , strData );
	CAtsFile::GetFistWord( strData , strFistWord , strOther );
	if( strFistWord.CompareNoCase("While") != 0 )
		return NULL;

	pSentence					= new CAtsSentence;
	pSentence->m_pSegIns		= this;
	pSentence->m_nType			= ATS_SENTENCE_TYPE_DO;
	pSentence->m_nBeginPos		= nBeginPos;
	pSentence->m_nEndPos		= nEnd+1;
	pSentence->m_strCondition	= strOther;

	m_listSentence.AddTail( pSentence );
	return pSentence;
}

//==================================================
// 得到 for 的语句
//==================================================
CAtsSentence * CAtsSegIns::GetSentence_for(int nBeginPos , CAtsString strCmdLine)
{
	CAtsString			strFistWord , strLastWord , strOther;
	CAtsSentence		*pSentence = NULL;
	CAtsString			strTemp;
	int					nPos = 0;
	CAtsString			strStart;
	CAtsString			strExp;
	CAtsString			strInitCmd,strNextCmd,strCondition;

	pSentence = FinSentence( nBeginPos );
	if( pSentence )
		return pSentence;

	CAtsFile::GetFistWord( strCmdLine , strFistWord , strOther );
	if( strFistWord.CompareNoCase("for") != 0 )
		return NULL;

	strOther.TrimLeft(' ');
	strOther.TrimRight(' ');
	strOther.TrimLeft('(');
	strOther.TrimRight(')');

	CAtsBaseFun::GetElementItem( strOther ,  strInitCmd , ";" );
	CAtsBaseFun::GetElementItem( strOther ,  strCondition , ";" );
	CAtsBaseFun::GetElementItem( strOther ,  strNextCmd , ";" );


	//--------------------------------
	// 新增一个 段
	//--------------------------------
	pSentence					= new CAtsSentence;
	pSentence->m_pSegIns		= this;
	pSentence->m_nType			= ATS_SENTENCE_TYPE_FOR;
	pSentence->m_nBeginPos		= nBeginPos;
	pSentence->m_strCondition	= strCondition;
	pSentence->m_strInitCmd		= strInitCmd;
	pSentence->m_strNextCmd		= strNextCmd;
	pSentence->m_nEndPos		= m_pSegmentItem->GetSectionEndLinePos(nBeginPos+1);
	
	if( pSentence->m_nEndPos > 0 )
	{
		m_listSentence.AddTail( pSentence );
		return pSentence;
	}
	else
	{
		delete pSentence;
		return NULL;
	}

	return NULL;
}

//====================================
// 装入参数数据
//====================================
BOOL CAtsSegIns::LoadParamData( CAtsParam *pParam )
{
	if( !pParam )
		return true;

	if( m_pSegmentItem->m_Param.IsEmpty() )
		return true;

	int				nIndex		= 0;
	CAtsVarItem		*pItem		= NULL;
	CAtsVarItem		*pInItem	= NULL;
	CAtsVarItem		*pInsVar	= NULL;
	POSITION		pos,ThePos;

	// 复制一份参数信息
	m_InParam					= m_pSegmentItem->m_Param;
	m_InParam.m_pEng			= m_pEngine;
	m_InParam.m_pParentSegIns	= pParam->m_pParentSegIns;
	for( pos = pParam->m_listParam.GetHeadPosition() ; pos ; )
	{
		pItem		= (CAtsVarItem *)pParam->m_listParam.GetNext( pos );
		ThePos		= m_InParam.m_listParam.FindIndex( nIndex );
		if( !ThePos )
			return false;

		pInItem	= (CAtsVarItem *)m_InParam.m_listParam.GetAt( ThePos );
		
		// 转入的数据可能是个变量,则把输入的变量名存入在数据中
		pInItem->SetData( &pItem->m_vData );

		// 以参数Item的变量名为名称,在 Segment 内部生成一个变量
		pInsVar = NewVarItem( pInItem->m_strName , CAtsVarItem::GetVarTypeName( pInItem->GetVarType() ) );
		if( !pInsVar )
			return false;

		if( pItem->m_vData.m_nType == ATSVAR_TYPE_VAR )
		{
			// 如果传入的是一个变量，则从 Sengine 中得到变量，并把值传进去
			CAtsVarItem * pVarItem = m_pEngine->GetVarItem( pParam->m_pParentSegIns , pItem->GetDataToStr() );
			if( pVarItem )
				pInsVar->m_vData.SetData( &pVarItem->m_vData );
			else
				return false;
		}
		else
			pInsVar->m_vData.SetData( pItem->GetDataToStr() );

		nIndex++;
	}
	return true;
}
