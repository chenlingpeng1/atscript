// AtsEng.cpp: implementation of the CAtsEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AtsEngine.h"
#include "AtsFile.h"
#include "AtsSegmentItem.h"
#include "AtsVarItem.h"
#include "AtsSegIns.h"
#include "AtsParam.h"
#include "AtsBaseFun.h"
#include "AtsRegKey.h"
#include "AtsExpMgr.h"
#include "AtsCommDef.h"
#include "SentenceSite.h"
#include "RegKeyObj.h"
#include "JfScriptEngineMgr.h"
#include "ValueSet.h"
#include "StringObj.h"
#include "IntObj.h"
#include "DoubleObj.h"
#include "BoolObj.h"
#include "WndParam.h"
#include "WSModuleObj.h"
#include "DllModuleObj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define		SET_ERROR(nCodeID)			{ m_nLastErrorCode=nCodeID;m_bIsError = true; return; }
#define		CHECK_ERROR()				{ if(m_bIsError) return; }
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAtsEngine::CAtsEngine()
{
//??	m_pCurSegIns		= NULL;
	m_nErrorLine		= 0;	
	m_bIsError			= false;
	m_nLastErrorCode	= 0;
	m_ExpExt.m_pEgnine = this;

	AddClassTemplate( new CWndParam );
	AddClassTemplate( new CValueSet );	
	AddClassTemplate( new CRegKeyObj );
	AddClassTemplate( new CStringObj );
	AddClassTemplate( new CIntObj );
	AddClassTemplate( new CDoubleObj );
	AddClassTemplate( new CBoolObj );
	AddClassTemplate( new CWSModuleObj );		
	AddClassTemplate( new CDllModuleObj );

	// 向引擎中加入常量
	SetConstVar( "true"  , "1" , ATSVAR_TYPE_BOOL);
	SetConstVar( "false" , "0" , ATSVAR_TYPE_BOOL);
}

CAtsEngine::~CAtsEngine()
{
	DetachList();
}

void CAtsEngine::DetachList(CAtsString strListName)
{
	POSITION			pos;
	CAtsSegmentItem		*pSegmentItem = NULL;
	CAtsVarItem			*pVarItem = NULL;
	CAtsSegIns			*pSegIns = NULL;
	CAtsObject			*pObject = NULL;

	if( strListName.IsEmpty() || strListName == "m_listSegment" )
	{
		for( pos = m_listSegment.GetHeadPosition() ; pos ; )
		{
			pSegmentItem = (CAtsSegmentItem *)m_listSegment.GetNext(pos); 
			delete pSegmentItem;
		}
		m_listSegment.RemoveAll();

		if( !strListName.IsEmpty() )
			return;
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
	if( strListName.IsEmpty() || strListName == "m_listExtClass" )
	{
		for( pos = m_listExtClass.GetHeadPosition() ; pos ; )
		{
			pObject = (CAtsObject *)m_listExtClass.GetNext(pos); 
			delete pObject;
		}
		m_listExtClass.RemoveAll();

		if( !strListName.IsEmpty() )
			return;
	}
	if( strListName.IsEmpty() || strListName == "m_listSegIns" )
	{
		for( pos = m_listSegIns.GetHeadPosition() ; pos ; )
		{
			pSegIns = (CAtsSegIns *)m_listSegIns.GetNext(pos); 
			delete pSegIns;
		}
		m_listSegIns.RemoveAll();

		if( !strListName.IsEmpty() )
			return;
	}
}

CAtsSegmentItem * CAtsEngine::GetSegment(CAtsString strName)
{
	POSITION			pos;
	CAtsSegmentItem		*pSegmentItem = NULL;
	for( pos = m_listSegment.GetHeadPosition() ; pos ; )
	{
		pSegmentItem = (CAtsSegmentItem *)m_listSegment.GetNext(pos); 
		if( pSegmentItem->m_strName.CompareNoCase( strName ) == 0 )
			return pSegmentItem;
	}
	return NULL;
}

BOOL CAtsEngine::LoadScript(LPCTSTR strScript)
{
	CAtsFile		ScriptFile;
	ScriptFile.m_pEngine = this;
	ScriptFile.LoadScript(strScript);
	return LoadScriptFile( &ScriptFile );
}

BOOL CAtsEngine::LoadScriptFile(LPCTSTR strScriptFile)
{
	CAtsFile		ScriptFile;

	ScriptFile.m_pEngine = this;
	if( !ScriptFile.OpenFile(strScriptFile) )
		return false;
	return LoadScriptFile( &ScriptFile );
}

BOOL CAtsEngine::LoadScriptFile(CAtsFile *pScriptFile)
{
	CAtsString			strSegmentData;
	int					nSegmentType;
	BOOL				bInSegment = false;
	CAtsSegmentItem		*pCurSegment = NULL;
	CAtsString			strLine;
	CAtsString			strVarName;
	CAtsString			strTemp,strItem;
	int					nSouneLine = 0;
	int					nLeftBracket = 0;
	CAtsString			strCode;
	CAtsString			strVarType;

	while( !pScriptFile->IsEndFile() )
	{
		if( !pScriptFile->GetDataLine( strLine , true ) )
			break;
		if( bInSegment )
		{
			if( strLine.CompareNoCase("{") == 0 ) nLeftBracket++;
			if( strLine.CompareNoCase("}") == 0 ) nLeftBracket--;
			if( nLeftBracket == 0 )
			{
				bInSegment	= false;
				pCurSegment = NULL;
				continue;
			}
			if( strLine.Right(1) == ";" )
				strLine.TrimRight(";");
			
			nSouneLine = pScriptFile->m_nCurLine;
			pCurSegment->AddLine( strLine , nSouneLine , -1 );
		}
		else
		{
			if( strLine.IsEmpty() )
				continue;
			else if( pScriptFile->IsPreCompiled( strLine,strCode ) )
			{
				if( strCode.CompareNoCase("#include") == 0 )
				{
					if( !m_strScriptPath.IsEmpty() )
					{
						CAtsString	strFile,strType;
						CAtsString	strFileData;
						strTemp = strLine;
						pScriptFile->GetElementItem(strTemp, strItem," ");
						pScriptFile->GetElementItem(strTemp, strFile," ");
						pScriptFile->GetElementItem(strTemp, strType," ");
						if( strType.CompareNoCase("VbScript") == 0 )
						{
							strFile.TrimLeft("\"");
							strFile.TrimRight("\"");
							strTemp = m_strScriptPath + "\\";
							strTemp = strTemp + strFile;
							CAtsFile::ReadFromFile( strTemp, strFileData );
							m_strVBScript += "\r\n";
							m_strVBScript += strFileData;
						}
					}
				}
			}
			else if( pScriptFile->IsVar( strLine , strVarName , strVarType ) )
			{
				CAtsVarItem	*pVarItem = NewVarItem( strVarName , strVarType );
				if( !pVarItem )
				{
					SetError( ATS_ERRCODE_VARTYTPE_ERROR , strVarType , nSouneLine );
					return false;
				}

				pVarItem->AddRef();
				pVarItem->SetValueName( strVarName , pVarItem->GetVarType() );
				m_listUserVar.AddTail( pVarItem );
			}
			else if( pScriptFile->IsSegment( strLine , strSegmentData , nSegmentType ) )
			{
				bInSegment	 = true;
				nLeftBracket = 0;
				pCurSegment  = pScriptFile->GetSegment( strLine );

				if( pCurSegment )
				{
					pCurSegment->m_nBeginPos	= pScriptFile->m_nCurLine;
					m_listSegment.AddTail( pCurSegment );
				}
				else
				{
					nSouneLine = pScriptFile->m_nCurLine;
					SetError( ATS_ERRCODE_SEGMENT_ERROR , strSegmentData , nSouneLine );
					return false;
				}
			}
		}
	}
	return true;
}

BOOL CAtsEngine::ExistSegment( CAtsString strSegmentName )
{
	CAtsSegmentItem * pItem = NULL;
	CAtsString			strError;

	pItem = GetSegment(strSegmentName);
	if( pItem )
		return true;
	else
		return false;
}
BOOL CAtsEngine::RunSegment( CAtsString strSegmentName , CAtsParam *pParam )
{
	return DoCustomSysFun( strSegmentName, pParam , NULL );
}
void CAtsEngine::RunCmdLine(CAtsSegIns *pCurSegIns , CAtsString strCmdLine , BOOL &bExit)
{
	CAtsString			strFistWord,strOther;
	CAtsString			strTagName;
	CAtsString			strReturn;
	BOOL				bRet = false;
	CAtsSentence		*pSentence = NULL;
	CAtsSentIns			*pSentIns  = NULL;
	bExit = false;
	if( strCmdLine.Right(1) == ":" )
		return;
	CAtsFile::GetFistWord( strCmdLine , strFistWord , strOther );
	if( strFistWord.CompareNoCase("if") == 0 )
	{
		CAtsSentIns			*pSentIns  = NULL;
		CAtsSentence		*pSentence = NULL;
		CAtsString			strCondition;

		pCurSegIns->m_nNextPos--;
		pSentence = pCurSegIns->GetSentence_If( pCurSegIns->m_nNextPos , strCmdLine );
		if( !pSentence ) 
		{
			m_bIsError = true;
			return;
		}
		pSentIns = pCurSegIns->SentTop();
		if( pSentIns == NULL || pSentIns->m_pSentence != pSentence )
		{
			pSentIns = new CAtsSentIns;
			pSentIns->m_pSentence = pSentence;
			pCurSegIns->m_listSentenceHeap.AddHead( pSentIns );
		}
		else
		{
			pCurSegIns->m_nNextPos = pSentence->m_nEndPos + 1;
			pCurSegIns->RemoveTopSentIns();
			return;
		}
		CSentenceSite	*pSiteItem = NULL;
		POSITION		SitePos;
		for( SitePos = pSentence->m_listSite.GetHeadPosition() ; SitePos ; )
		{
			pSiteItem = (CSentenceSite *)pSentence->m_listSite.GetNext( SitePos );
			if( pSiteItem->m_strCondition.IsEmpty() )
			{
				pCurSegIns->m_nNextPos = pSiteItem->m_nBeginPos+1;
				return;
			}
			bRet = CheckCondition( pSiteItem->m_strCondition , pCurSegIns );
			CHECK_ERROR()
			if( bRet )
			{
				pCurSegIns->m_nNextPos = pSiteItem->m_nBeginPos+1;
				return;
			}
		}
		pCurSegIns->RemoveTopSentIns();
		pCurSegIns->m_nNextPos = pSentence->m_nEndPos + 1;
	}
	else if( strFistWord.CompareNoCase("}") == 0  )
	{
		CAtsSentIns *pItem = pCurSegIns->SentTop();
		if( !pItem ) SET_ERROR( ATS_ERRCODE_SENTENCE_ERROR )

		if( pItem->m_pSentence->m_nType == ATS_SENTENCE_TYPE_WHILE ||
			pItem->m_pSentence->m_nType == ATS_SENTENCE_TYPE_FOR)
		{
			pCurSegIns->m_nNextPos = pItem->m_pSentence->m_nBeginPos;
		}
		else if( pItem->m_pSentence->m_nType == ATS_SENTENCE_TYPE_DO )
		{
			bRet = CheckCondition( pItem->m_pSentence->m_strCondition, pCurSegIns );
			CHECK_ERROR()
			if( bRet )
				pCurSegIns->m_nNextPos = pItem->m_pSentence->m_nBeginPos;
			else
			{
				pCurSegIns->m_nNextPos = pItem->m_pSentence->m_nEndPos + 1;
				pCurSegIns->RemoveTopSentIns();
			}
		}
		else
		{
			pCurSegIns->m_nNextPos = pItem->m_pSentence->m_nEndPos + 1;
			pCurSegIns->RemoveTopSentIns();
		}
	}
	else if( strFistWord.CompareNoCase("while") == 0 )
	{
		CAtsSentence		*pSentence = NULL;
		CAtsString			strCondition;

		pCurSegIns->m_nNextPos--;
		pSentence = pCurSegIns->GetSentence_While( pCurSegIns->m_nNextPos , strCmdLine );
		if( !pSentence ) 
		{
			m_bIsError = true;
			return;
		}
		strCondition = pSentence->m_strCondition;
		if( strCondition.IsEmpty() )
			SET_ERROR(ATS_ERRCODE_IF_ERROR)

		pSentIns = pCurSegIns->SentTop();
		if( pSentIns == NULL || pSentIns->m_pSentence != pSentence )
		{
			pSentIns = new CAtsSentIns;
			pSentIns->m_nRunCount++;
			pSentIns->m_pSentence = pSentence;
			pCurSegIns->m_listSentenceHeap.AddHead( pSentIns );
		}
		else
			pSentIns->m_nRunCount++;
	
		bRet = CheckCondition( strCondition, pCurSegIns  );
		CHECK_ERROR()
		if( bRet )
		{
			pCurSegIns->m_nNextPos++;
		}
		else
		{
			pCurSegIns->RemoveTopSentIns();
			pCurSegIns->m_nNextPos = pSentence->m_nEndPos + 1;
		}
	}
	else if( strFistWord.CompareNoCase("for") == 0 )
	{
		CAtsString			strCondition;

		pCurSegIns->m_nNextPos--;
		pSentence = pCurSegIns->GetSentence_for( pCurSegIns->m_nNextPos , strCmdLine );
		if( !pSentence ) 
		{
			m_bIsError = true;
			m_nLastErrorCode = ATS_ERRCODE_FOR_ERROR;
			return;
		}
	
		pSentIns = pCurSegIns->SentTop();
		if( pSentIns == NULL || pSentIns->m_pSentence != pSentence )
		{
			pSentIns = new CAtsSentIns;
			pSentIns->m_nRunCount++;
			pSentIns->m_pSentence = pSentence;
			pCurSegIns->m_listSentenceHeap.AddHead( pSentIns );
			RunCmdLine_simple( pCurSegIns , pSentence->m_strInitCmd , bExit);
		}
		else
		{
			pSentIns->m_nRunCount++;
			RunCmdLine_simple( pCurSegIns , pSentence->m_strNextCmd , bExit);
		}
		
		bRet = CheckCondition( pSentence->m_strCondition, pCurSegIns  );
		CHECK_ERROR()
		if( bRet )
		{
			pCurSegIns->m_nNextPos++;
		}
		else
		{
			pCurSegIns->RemoveTopSentIns();
			pCurSegIns->m_nNextPos = pSentence->m_nEndPos + 1;
		}
	}
	else if( strFistWord.CompareNoCase("switch") == 0 )
	{
		CAtsSentence		*pSentence = NULL;
		CAtsString			strCondition;

		pCurSegIns->m_nNextPos--;
		pSentence = pCurSegIns->GetSentence_Switch( pCurSegIns->m_nNextPos , strCmdLine );
		if( !pSentence ) 
		{
			m_bIsError = true;
			return;
		}
		if( pSentence->m_strCondition.IsEmpty() )
			SET_ERROR(ATS_ERRCODE_SWITCH_ERROR)

		pSentIns = pCurSegIns->SentTop();
		if( pSentIns == NULL || pSentIns->m_pSentence != pSentence )
		{
			pSentIns = new CAtsSentIns;
			pSentIns->m_nRunCount++;
			pSentIns->m_pSentence = pSentence;
			pCurSegIns->m_listSentenceHeap.AddHead( pSentIns );
		}
		else
			pSentIns->m_nRunCount++;
		CSentenceSite	*pSiteItem = NULL;
		POSITION		SitePos;
		for( SitePos = pSentence->m_listSite.GetHeadPosition() ; SitePos ; )
		{
			pSiteItem = (CSentenceSite *)pSentence->m_listSite.GetNext( SitePos );
			if( pSiteItem->m_strCondition.IsEmpty() )
			{
				pCurSegIns->m_nNextPos = pSiteItem->m_nBeginPos+1;
				return;
			}
			CAtsString		strExp;
			strExp.Format("(%s) == (%s)" , pSentence->m_strCondition , pSiteItem->m_strCondition );
			bRet = CheckCondition( strExp, pCurSegIns  );
			CHECK_ERROR()
			if( bRet )
			{
				pCurSegIns->m_nNextPos = pSiteItem->m_nBeginPos+1;
				return;
			}
		}
		pCurSegIns->RemoveTopSentIns();
		pCurSegIns->m_nNextPos = pSentence->m_nEndPos + 1;
		return;
	}
	else if( strFistWord.CompareNoCase("case") == 0  )
	{
		CAtsSentIns *pItem = pCurSegIns->SentTop();
		if( !pItem ) SET_ERROR( ATS_ERRCODE_SWITCH_ERROR )
		if( pItem->m_pSentence->m_nType != ATS_SENTENCE_TYPE_SWITCH ) SET_ERROR( ATS_ERRCODE_SWITCH_ERROR )
		pCurSegIns->m_nNextPos = pItem->m_pSentence->m_nEndPos + 1;
	}
	else if( strFistWord.CompareNoCase("do") == 0 )
	{
		CAtsSentence		*pSentence = NULL;
		CAtsString			strCondition;
		pCurSegIns->m_nNextPos--;
		pSentence = pCurSegIns->GetSentence_Do( pCurSegIns->m_nNextPos , strCmdLine );
		if( !pSentence ) 
		{
			m_bIsError = true;
			return;
		}
		strCondition = pSentence->m_strCondition;
		if( strCondition.IsEmpty() )
			SET_ERROR(ATS_ERRCODE_DO_ERROR)

		pSentIns = pCurSegIns->SentTop();
		if( pSentIns == NULL || pSentIns->m_pSentence != pSentence )
		{
			pSentIns = new CAtsSentIns;
			pSentIns->m_nRunCount++;
			pSentIns->m_pSentence = pSentence;
			pCurSegIns->m_listSentenceHeap.AddHead( pSentIns );
		}
		else
			pSentIns->m_nRunCount++;
		pCurSegIns->m_nNextPos++;
	}
	else if( strFistWord.CompareNoCase("break") == 0 )
	{
		CAtsSentIns *pItem = NULL;
		
		pCurSegIns->SentPop_Cant_Break();
		pItem = pCurSegIns->SentTop();
		if( !pItem ) SET_ERROR( ATS_ERRCODE_EXIT_ERROR )
		pCurSegIns->m_nNextPos = pItem->m_pSentence->m_nEndPos + 1;
		pCurSegIns->RemoveTopSentIns();
	}
	else if( strFistWord.CompareNoCase("return") == 0 )
	{
		CAtsValue		vReturn;

		strReturn = (LPCTSTR)strOther;
		if( !strOther.IsEmpty() )
		{
			CAtsExpMgr		Jfe;
			Jfe.m_pFEExt = &m_ExpExt;
			m_ExpExt.m_pSegIns = pCurSegIns;
			if( !Jfe.Computer( (LPCTSTR)strOther , &vReturn ) )
			{
				SetError( ATS_ERRCODE_COMPUTER_ERROR , strOther );
				return;
			}
		}

		pCurSegIns->m_InParam.m_vReturn = vReturn;
		bExit = true;
	}
	else
		RunCmdLine_simple( pCurSegIns , strCmdLine , bExit );
}

void CAtsEngine::RunCmdLine_simple(CAtsSegIns *pCurSegIns , CAtsString strCmdLine , BOOL &bExit)
{
	CAtsString		strFistWord,strOther;
	CAtsString		strReturn;
	BOOL			bRet = false;

	bExit = false;

	CAtsFile::GetFistWord( strCmdLine , strFistWord , strOther );

	if( strFistWord.Find('.') > 0 )
	{
		if( !RunFun( (LPCTSTR)strFistWord , (LPCTSTR)strOther, pCurSegIns ) )
			m_bIsError = true;
		return;
	}
	else if( strFistWord.CompareNoCase("var") == 0 )
	{
		CAtsString		strVarType,strVarName,strItem;
		CAtsString		strSeting;
		CAtsFile::GetFistWord( strOther , strVarType , strOther );
		if( strVarType.IsEmpty() || strOther.IsEmpty() )
		{
			SetError( ATS_ERRCODE_DEFINE_VAR );
			return;
		}
		while( CAtsFile::GetElementItem(strOther,  strItem,"," ) )
		{
			CAtsFile::GetFistWord( strItem , strVarName , strSeting );
			if( !pCurSegIns->NewVarItem( strVarName ,strVarType ) )
			{
				m_bIsError		= true;
				pCurSegIns->m_pSegmentItem->GetSourneLine( pCurSegIns->m_nNextPos , m_nErrorLine , m_nErrorLinePos );
				TRACE("Script Error:%s,%d" , __FILE__ , __LINE__ );
				return;
			}
			if( !strSeting.IsEmpty() )
			{
				if( !SetDataEx( strVarName , strSeting , pCurSegIns ) )
				{
					SetError( ATS_ERRCODE_SETDATA_ERROR );
					return;
				}
			}
		}
	}
	else if( IsVar( strFistWord , pCurSegIns ) )
	{
		if( !SetDataEx( strFistWord , strOther , pCurSegIns ) )
		{
			m_bIsError = true;
			m_nLastErrorCode = ATS_ERRCODE_SETDATA_ERROR;
		}
	}
	else if( strFistWord.CompareNoCase("Set") == 0 )
	{
		CAtsFile::GetFistWord( strOther , strFistWord , strOther );
		if( !IsVar(strFistWord,  pCurSegIns ) )
			SetError( ATS_ERRCODE_SETDATA_ERROR );
		
		if( !SetDataEx( strFistWord , strOther , pCurSegIns ) )
			SetError( ATS_ERRCODE_SETDATA_ERROR );
	}
	else if( strFistWord.CompareNoCase("Goto") == 0 )
	{
		Goto( pCurSegIns , strOther );
	}
	else
	{
		if( strOther.Left(1) == "=" )
			SetError( ATS_ERRCODE_NOTEXISTVAR , strFistWord );
		else
		{
			if( !RunFun( (LPCTSTR)strFistWord , (LPCTSTR)strOther , pCurSegIns ) )
				m_bIsError = true;
		}
	}
}
BOOL CAtsEngine::SetDataEx( CAtsString strVarName , CAtsString strExp , CAtsSegIns *pSegIns )
{
	CAtsExpMgr		Jfe;
	BOOL			bRet = false;
	CAtsString		strReturn;
	CAtsString		strFistWord,strOther;
	CAtsVarItem		*pVarItem = NULL;
	CAtsString		strOP;
	strExp.Replace('\t', ' ');
	strExp.TrimLeft(' ');
	if( strExp.GetLength() < 2 )
		return false;

	strOP = strExp.Left(2);
	if( strOP == "++" )
	{
		pVarItem = GetVarItem( pSegIns , strVarName );
		pVarItem->m_vData.AddData(1);
		SetVar( strVarName , &pVarItem->m_vData , pSegIns );
		return true;
	}
	else if( strOP == "--" )
	{
		pVarItem = GetVarItem( pSegIns , strVarName );
		pVarItem->m_vData.AddData(-1);
		SetVar( strVarName , &pVarItem->m_vData , pSegIns );
		return true;
	}
	else if( strOP == "+=" )
	{
		strExp = strExp.Mid(2);
		CAtsValue		vReturn;
		Jfe.m_pFEExt = &m_ExpExt;
		m_ExpExt.m_pSegIns = pSegIns;
		bRet = Jfe.Computer( (LPCTSTR)strExp , &vReturn );
		if( Jfe.m_bIsError  )
		{
			m_bIsError = Jfe.m_bIsError;
			return false;
		}

		pVarItem = GetVarItem( pSegIns , strVarName );
		pVarItem->m_vData.AddData( &vReturn );
		SetVar( strVarName , &pVarItem->m_vData , pSegIns );
		return true;
	}
	else if( strOP == "-=" )
	{
		strExp = strExp.Mid(2);
		CAtsValue		vReturn;
		Jfe.m_pFEExt = &m_ExpExt;
		m_ExpExt.m_pSegIns = pSegIns;
		bRet = Jfe.Computer( (LPCTSTR)strExp , &vReturn );
		if( Jfe.m_bIsError  )
		{
			m_bIsError = Jfe.m_bIsError;
			return false;
		}

		pVarItem = GetVarItem( pSegIns , strVarName );
		pVarItem->m_vData.AddData( 0 - vReturn.GetdData() ); 
		SetVar( strVarName , &pVarItem->m_vData , pSegIns );
		return true;
	}
	else if( strOP == "*=" )
	{
		strExp = strExp.Mid(2);
		CAtsValue		vReturn;
		Jfe.m_pFEExt = &m_ExpExt;
		m_ExpExt.m_pSegIns = pSegIns;
		bRet = Jfe.Computer( (LPCTSTR)strExp , &vReturn );
		if( Jfe.m_bIsError  )
		{
			m_bIsError = Jfe.m_bIsError;
			return false;
		}

		pVarItem = GetVarItem( pSegIns , strVarName );
		pVarItem->m_vData.MultiplyData( vReturn.GetdData() );
		SetVar( strVarName , &pVarItem->m_vData , pSegIns );
		return true;
	}
	else if( strOP == "%=" )
	{
		strExp = strExp.Mid(2);
		CAtsValue		vReturn;
		Jfe.m_pFEExt = &m_ExpExt;
		m_ExpExt.m_pSegIns = pSegIns;
		bRet = Jfe.Computer( (LPCTSTR)strExp , &vReturn );
		if( Jfe.m_bIsError  )
		{
			m_bIsError = Jfe.m_bIsError;
			return false;
		}

		pVarItem = GetVarItem( pSegIns , strVarName );
		pVarItem->m_vData.ModeData( vReturn.GetdData() );
		SetVar( strVarName , &pVarItem->m_vData , pSegIns );
		return true;
	}
	else if( strOP == "/=" )
	{
		strExp = strExp.Mid(2);
		CAtsValue		vReturn;
		Jfe.m_pFEExt = &m_ExpExt;
		m_ExpExt.m_pSegIns = pSegIns;
		bRet = Jfe.Computer( (LPCTSTR)strExp , &vReturn );
		if( Jfe.m_bIsError  )
		{
			m_bIsError = Jfe.m_bIsError;
			return false;
		}

		pVarItem = GetVarItem( pSegIns , strVarName );
		pVarItem->m_vData.DivideData( vReturn.GetdData() ); 
		SetVar( strVarName , &pVarItem->m_vData , pSegIns );
		return true;
	}

	strOP = strExp.Left(1);
	if( strOP != "=" )
		return false;
	strExp = strExp.Mid(1);
	CAtsFile::GetFistWord( strExp , strFistWord , strOther );
	if( strFistWord.CompareNoCase("Nothing") == 0 )
	{
		CAtsVarItem		*pVarItem = NULL;
		pVarItem =	GetVarItem( pSegIns , strVarName );
		if( !pVarItem || pVarItem->IsObject() )
		{
			SetError( ATS_ERRCODE_NOTEXISTATSOBJ ,strVarName );
			return false;
		}
		pVarItem->m_vData.m_pObject = NULL;
		return true;
	}
	else if( strFistWord.CompareNoCase("new") == 0 )
	{
		CAtsVarItem	*pVarItem	= NULL;
		CAtsObject	*pClass		= NULL;
		pVarItem =	GetVarItem( pSegIns , strVarName );

		CAtsFile::GetFistWord( strOther , strFistWord , strOther );
		pClass = GetClassTemplate( strFistWord );
		pVarItem->m_vData.m_pObject = pClass->NewObject();
		return true;
	}
	else if( strFistWord.CompareNoCase("CreateAtsObject") == 0 )
	{
		CAtsParam		Param;
		CAtsObject		*pObj = NULL;

		Param.m_pEng			= this;
		Param.m_pParentSegIns	= pSegIns;
		Param.LoadParamData( strOther );
		pObj = CreateAtsObject( Param.GetItemData(1) );
		if( pObj )
		{
			CAtsVarItem		*pVarItem = NULL;
			pVarItem =	GetVarItem( pSegIns , strVarName );
			if( pVarItem )
			{
				if( pVarItem->GetVarType() == ATSVAR_TYPE_ATSOBJECT )
				{
					pVarItem->m_vData.m_pObject = pObj;
					return true;
				}
				else
					SetError( ATS_ERRCODE_VARTYTPE_ERROR , strVarName );
			}
			else
				SetError( ATS_ERRCODE_NOTEXISTVAR , strVarName );
		}
		else
			SetError( ATS_ERRCODE_NOTEXISTATSOBJ , Param.GetItemData(1) );

		return false;
	}
	else
	{
		CAtsValue		vReturn;
		Jfe.m_pFEExt = &m_ExpExt;
		m_ExpExt.m_pSegIns = pSegIns;
		bRet = Jfe.Computer( (LPCTSTR)strExp , &vReturn );
		if( Jfe.m_bIsError  )
		{
			m_strErrorWord	= Jfe.m_strErrWord;
			m_bIsError		= Jfe.m_bIsError;
			return false;
		}
		SetVar( strVarName , &vReturn , pSegIns);

		return bRet;
	}
}
BOOL CAtsEngine::SetData( CAtsString strVarName , CAtsString strOther , CAtsSegIns *pSegIns)
{
	CAtsString		strItem;
	strOther.TrimLeft(' ');
	CAtsFile::GetElementItem( strOther,  strItem, " " );
	if( strItem != "=" || strOther.IsEmpty() )
		return false;
	if( strOther.Right(1) == ")" )
	{
		CAtsString		strFistWord;
		CAtsParam		param;
	
		CAtsFile::GetFistWord( strOther , strFistWord , strOther );

		param.LoadParamData( strOther );
		param.m_pEng			= this;
		param.m_pParentSegIns	= pSegIns;
		if( DoAtsSysFun( pSegIns,  strFistWord , &param ) )
		{
			SetVar( strVarName , param.m_vReturn.GetStrData() , pSegIns );
			return true;
		}
	}
	else
	{
		SetVar( strVarName , GetFactData( strOther , pSegIns ) , pSegIns );
		return true;
	}
	return false;
}
BOOL CAtsEngine::RunFun( CAtsString strFunName , CAtsString strParam , CAtsSegIns *pSegIns)
{
	CAtsExpMgr		Jfe;
		
	Jfe.m_pFEExt = &m_ExpExt;
	CAtsParam		param;
	
	param.m_pEng			= this;
	param.m_pParentSegIns	= pSegIns;
	m_ExpExt.m_pSegIns = pSegIns;
	if( Jfe.CmputerParam( (LPCTSTR)strParam , &param ) )
		return RunFun( (LPCTSTR)strFunName , &param , pSegIns );
	else
	{
		m_strErrorWord = Jfe.m_strErrWord;
		return false;
	}
}
BOOL CAtsEngine::RunFun( CAtsString strFunName , CAtsParam *pParam , CAtsSegIns *pSegIns)
{
	if( strFunName.Find('.') > 0 )
	{
		if( !DoObjFun( (LPCTSTR)strFunName , pParam , pSegIns ) )
		{
			m_bIsError		= true;
			m_strErrorWord	= strFunName;
			m_nLastErrorCode = ATS_ERRCODE_OBJFUN_ERROR;
			return false;
		}
	}
	else if( IsCoustomFun(strFunName) )
	{
		if( !DoCustomSysFun( (LPCTSTR)strFunName , pParam , pSegIns ) )
			return false;
	}
	else
	{
		if( !DoAtsSysFun( pSegIns , (LPCTSTR)strFunName , pParam ) )
		{
			m_strErrorWord	= strFunName;
			return false;
		}
	}
	POSITION		pos;
	CAtsVarItem		*pVarItem = NULL;
	for( pos = pParam->m_listParam.GetHeadPosition() ; pos ; )
	{
		pVarItem = (CAtsVarItem *)pParam->m_listParam.GetNext( pos );
		if( pVarItem->m_nFlag & ATSVAR_FLAG_OUT )
		{
			if( !pVarItem->m_strName.IsEmpty() )
				SetVar( pVarItem->m_strName , &pVarItem->m_vData , pSegIns );
		}
	}
	return true;
}

void CAtsEngine::SetGlobalParam( CString strName , CString strData )
{
	int		nNum,nCount;

	nCount = m_asGlobalParamName.GetSize();
	for( nNum = 0 ; nNum < nCount ; nNum++ )
	{
		if( m_asGlobalParamName.GetAt( nNum ).CompareNoCase( strName ) == 0 )
		{
			m_asGlobalParamData.SetAt( nNum , strData );
			return;
		}
	}
	m_asGlobalParamName.Add( strName );
	m_asGlobalParamData.Add( strData );
}

CString CAtsEngine::GetGlobalParam( CString strName )
{
	int		nNum,nCount;
	nCount = m_asGlobalParamName.GetSize();
	for( nNum = 0 ; nNum < nCount ; nNum++ )
	{
		if( m_asGlobalParamName.GetAt( nNum ).CompareNoCase( strName ) == 0 )
			return m_asGlobalParamData.GetAt( nNum );

	}
	return _T("");
}
BOOL CAtsEngine::DoAtsSysFun( CAtsSegIns *pSegIns , CAtsString strFunName, CAtsParam *pParam )
{
	CAtsString		strData;

	if( strFunName.CompareNoCase("DataCpy") == 0 )
	{
		strData = GetFactData( pParam->GetItemData(2) , pSegIns );
		pParam->SetOutData( 1, strData );
		return true;
	}
	else if( strFunName.CompareNoCase("SetGlobalParam") == 0 )
	{
		CString		strName;
		strName = GetFactData( pParam->GetItemData(1) , pSegIns );
		strData = GetFactData( pParam->GetItemData(2) , pSegIns );
		SetGlobalParam( strName , strData);
		return true;
	}
	else if( strFunName.CompareNoCase("GetGlobalParam") == 0 )
	{
		CString		strName;

		strName = GetFactData( pParam->GetItemData(1) , pSegIns );
		strData = GetGlobalParam( strName );
		pParam->SetReturnDataEx( strData , ATSVAR_TYPE_STRING );
		return true;
	}
	else if( strFunName.CompareNoCase("GetSystemParam") == 0 )
	{
		CString		strName;

		strName = GetFactData( pParam->GetItemData(1) , pSegIns );
		strData = GetSystemParam( strName );
		pParam->SetReturnDataEx( strData , ATSVAR_TYPE_STRING );
		return true;
	}
	else if( strFunName.CompareNoCase("MessageBox") == 0 )
	{
		CAtsString		strData;
		CAtsVarItem		*pVarItrem	= NULL;
		CAtsVarItem		*pTitle		= NULL;
		CAtsVarItem		*pButtons	= NULL;
		int				nButtons	= 0;
		int				nRet;
		CString			strTitle	= m_strAppName;

		if( strTitle.IsEmpty() )
			strTitle = "Prompt";

		pVarItrem = pParam->GetItem(1);
		if( pVarItrem )
			strData	= GetFactData( (CAtsValue *)&pVarItrem->m_vData , pSegIns );
		pTitle = pParam->GetItem(2);
		if( pTitle )
			strTitle	= GetFactData( (CAtsValue  *)&pTitle->m_vData , pSegIns );
		pButtons = pParam->GetItem(3);
		if( pButtons )
			nButtons = pButtons->m_vData.GetnData();
		
		nRet = MessageBox( NULL , strData , strTitle , nButtons );
		pParam->SetReturnDataEx( nRet , ATSVAR_TYPE_INT );
		return true;
	}
	else if( strFunName.CompareNoCase("DataCmpGoto") == 0 )
	{
		if( pParam->DataCmp( 1 , 2 ) == 0 )
		{
			CAtsString	strTagName = pParam->GetItemData( 3,false);
			Goto( pSegIns , strTagName );
		}
		return true;
	}
	else if( strFunName.CompareNoCase("CallVBSFunction") == 0 )
	{
		if( m_strVBScript.IsEmpty() )
		{
			pParam->SetReturnDataEx( "" , ATSVAR_TYPE_STRING );
			return false;
		}
		CAtsString		strFunName = GetFactData( pParam->GetItemData(1), pSegIns );
		CJfScriptEngineMgr	ScriptFactory;
		ScriptFactory.InitializeScriptEngine(JFSCRIPT_ENGINE_VBS);
		ScriptFactory.LoadScript(m_strVBScript);
		CJfScriptParams	Params;
		CString			strReturn;
		CAtsVarItem		*pItem = NULL;
		for( int nNum = 2 ; nNum <= pParam->m_listParam.GetCount() ; nNum++ )
		{
			pItem = pParam->GetItem( nNum );
			if( pItem->GetVarType() == ATSVAR_TYPE_VAR )
			{
				pItem = GetVarItem( pSegIns , pItem->m_vData.GetStrData() );
				if( !pItem ) return false;
			} 
			if( pItem->GetVarType() == ATSVAR_TYPE_STRING )
				Params.AddItem( (LPCTSTR)pItem->GetDataToStr()  );
			else
				Params.AddItem( pItem->m_vData.GetnData() );
		}

		try
		{
			ScriptFactory.CallSimpleFun( (LPCTSTR)strFunName , &Params , strReturn );
			pParam->SetReturnDataEx( (LPCTSTR)strReturn , ATSVAR_TYPE_STRING );
		}
		catch(...)
		{
		}
		if( ScriptFactory.IsError() )
		{
			CStringArray	asErrorInfo;
			int				nNum,nCount;
			ScriptFactory.GetErrorMessages( asErrorInfo );
			nCount = asErrorInfo.GetSize();
			for( nNum = 0 ; nNum < nCount ; nNum++ )
				MessageBox( NULL , asErrorInfo.GetAt( nNum ) , "AtScript" , 0 );
			return false;
		}
		return true;
	}
	return false;
}
CAtsObject * CAtsEngine::GetObjIns( CAtsString strName ,CAtsSegIns *pSegIns)
{
	CAtsVarItem		*pVarItem = NULL;

	pVarItem =	GetVarItem( pSegIns , strName );
	if( !pVarItem || !pVarItem->m_vData.m_pObject )
		return NULL;
	return  (CAtsObject *)pVarItem->m_vData.m_pObject;
}
void CAtsEngine::AddClassTemplate( CAtsObject *pObjCalss , CString sName)
{
	if( !sName.IsEmpty() )
		pObjCalss->m_strClassName	= sName;

	pObjCalss->m_pEngine = this;
	m_listExtClass.AddTail( pObjCalss );
}
void CAtsEngine::AddObjIns( LPCTSTR sName , CAtsObject *pObjCalss )
{
	pObjCalss->m_pEngine = this;

	CAtsVarItem		*pNewVar = new CAtsVarItem;
	pNewVar->m_strName			= sName;
	pNewVar->m_vData.m_nType	= ATSVAR_TYPE_ATSOBJECT;
	pNewVar->m_vData.m_pObject	= pObjCalss;
	pNewVar->AddRef();
	m_listUserVar.AddTail( pNewVar );
}
BOOL CAtsEngine::DoObjFun( CAtsString strFistWord , CAtsParam *pParam , CAtsSegIns *pSegIns)
{
	int				nPos;
	CAtsString		strObjName;
	CAtsString		strFunName;
	CAtsObject		*pObjIns = NULL;
	nPos		= strFistWord.Find('.');
	strObjName	= strFistWord.Left(nPos);
	strFunName	= strFistWord.Mid(nPos+1);
	pObjIns = GetObjIns( strObjName , pSegIns );
	if( !pObjIns )
		return false;
	pParam->m_pEng = this;
	return pObjIns->DoFun( strFunName , pParam , pSegIns);
}
void CAtsEngine::SetVar(LPCTSTR sName , int nData , CAtsSegIns *pSegIns)
{
	CAtsString		strData;
	strData.Format("%d" , nData );
	SetVar( sName , (LPCTSTR)strData , pSegIns );
}
void CAtsEngine::SetVar(LPCTSTR sName , LPCTSTR sData , CAtsSegIns *pSegIns)
{
	CPtrList			*pList = NULL;
	CAtsVarItem			*pVarItem = NULL;
	BOOL				bFind = false;
	CAtsString			strName = sName;
	CAtsString			strData = sData;
	pVarItem =	GetVarItem( pSegIns , sName );
	if( pVarItem )
	{
		if( !pVarItem->m_bIsConst )
			pVarItem->m_vData.SetData( strData );
		return;
	}
	pVarItem = new CAtsVarItem;
	pVarItem->AddRef();
	pVarItem->m_strName = strName;
	pVarItem->m_vData.SetData( strData );
	m_listUserVar.AddTail( pVarItem );
}

void CAtsEngine::SetVar(LPCTSTR sName , CAtsValue *pData, CAtsSegIns *pSegIns)
{
	CPtrList			*pList = NULL;
	CAtsVarItem			*pVarItem = NULL;
	BOOL				bFind = false;
	CAtsString			strName = sName;

	pVarItem =	GetVarItem( pSegIns , sName );
	if( pVarItem )
	{
		if( !pVarItem->m_bIsConst )
			pVarItem->SetData( pData );
		return;
	}

	pVarItem = new CAtsVarItem;
	pVarItem->AddRef();
	pVarItem->SetData( pData );
	m_listUserVar.AddTail( pVarItem );
}

BOOL CAtsEngine::IsObjVar(CAtsString strName , CAtsString &strObjName , CAtsString &strVarName )
{
	BOOL	bIsSysVar = false;
	int		nBegin = 0;
	int		nPos = 0;

	nPos = strName.Find(".");
	if( nPos <= 0 )
		return false;

	if( strName.Left(2) == "@@" )
		nBegin = 2;
	else if( strName.Left(1) == "@" )
		nBegin = 1;

	strObjName = strName.Left(nPos);
	strObjName = strObjName.Mid( nBegin );
	strVarName = strName.Mid( nPos + 1 );
	if( nBegin == 2 )
		strVarName = "@@" + strVarName;
	else if( nBegin == 1 )
		strVarName = "@" + strVarName;
	return true;
}

CAtsString CAtsEngine::GetVar(LPCTSTR sName , BOOL &bExit , CAtsSegIns *pSegIns)
{
	CAtsObjPtr<CAtsVarItem>		pVarItem = NULL;

	bExit			= false;
	pVarItem = GetVarItem( pSegIns , sName );
	if( pVarItem )
	{
		bExit = true;
		return pVarItem->m_vData.GetStrData();
	}
	return _T("");
}
BOOL CAtsEngine::IsVar( LPCTSTR sName ,  CAtsSegIns *pSegIns )
{
	if( GetVarItem( pSegIns , sName ) )
		return true;
	else
		return false;
}
CAtsVarItem * CAtsEngine::GetVarItem(CAtsSegIns *pSegIns , LPCTSTR sName )
{
	POSITION		pos;
	CAtsVarItem		*pVarItem = NULL;
	BOOL			bFind = false;
	CAtsString		strObjName,strVarName;
	CAtsString		strName = sName;
	CAtsString		strData;

	if( IsObjVar( strName , strObjName , strVarName ) )
	{
		CAtsObject * pObj = NULL;
		pObj = GetObjIns( strObjName , pSegIns );
		if( pObj )
			return pObj->GetVarItem( strVarName );
		return NULL;
	}
	if( pSegIns )
	{
		pVarItem = pSegIns->GetVarItem( strName );
		if( pVarItem )
			return pVarItem;
	}
	for( pos = m_listUserVar.GetHeadPosition() ; pos ; )
	{
		pVarItem = (CAtsVarItem *)m_listUserVar.GetNext(pos); 
		if( pVarItem->m_strName.CompareNoCase( strName ) == 0 )
			return pVarItem;
	}
	return NULL;
}

CAtsString CAtsEngine::GetFactData( CAtsValue * pData , CAtsSegIns *pSegIns)
{
	BOOL		bExist = false;
	if( pData->m_nType == ATSVAR_TYPE_VAR )
	{
		CAtsString		strFaceData;
		BOOL			bExist;

		strFaceData = GetVar( pData->GetStrData() , bExist, pSegIns );
		return strFaceData;
	}
	else
		return pData->GetStrData();
}
CAtsString CAtsEngine::GetFactData( CAtsString strData , CAtsSegIns *pSegIns)
{
	if( strData.IsEmpty() )
		return "";

	if( IsVar( strData , pSegIns ) )
	{
		CAtsString		strFaceData;
		BOOL			bExist;

		strFaceData = GetVar( strData , bExist, pSegIns );
		return strFaceData;
	}
	else if( strData.GetLength() >= 2 )
	{
		if( strData.Left(1) == "\"" && strData.Right(1) == "\"" )
			strData = strData.Mid(1, strData.GetLength() - 2 );
	}

	return strData;
}
BOOL CAtsEngine::CheckCondition( CAtsString strCondition , CAtsSegIns *pSegIns)
{
	CAtsExpMgr		Jfe;
	BOOL			bRet = false;

	Jfe.m_pFEExt		= &m_ExpExt;
	m_ExpExt.m_pSegIns	= pSegIns;
	bRet = Jfe.CheckCondition( (LPCTSTR)strCondition );
	if( Jfe.m_bIsError  )
	{
		m_strErrorWord 	= Jfe.m_strErrWord;
		m_bIsError		= Jfe.m_bIsError;
		return false;
	}
	return bRet;
}
BOOL CAtsEngine::Computer( CAtsString strExpress , CAtsValue *pValue , CAtsSegIns *pSegIns)
{
	CAtsExpMgr		Jfe;
	BOOL			bRet = false;

	Jfe.m_pFEExt = &m_ExpExt;
	m_ExpExt.m_pSegIns = pSegIns;
	bRet = Jfe.Computer( strExpress , pValue );
	if( Jfe.m_bIsError  )
	{
		m_bIsError			= Jfe.m_bIsError;
		m_nLastErrorCode	= ATS_ERRCODE_COMPUTER_ERROR;
		m_strErrorWord		= strExpress;
		return false;
	}
	return true;
}
BOOL CAtsEngine::IsCoustomFun( CAtsString strFunName )
{
	POSITION			pos;
	CAtsSegmentItem		*pSegmentItem = NULL;

	for( pos = m_listSegment.GetHeadPosition() ; pos ; )
	{
		pSegmentItem = (CAtsSegmentItem *)m_listSegment.GetNext(pos); 
		if( pSegmentItem->m_strName.CompareNoCase( strFunName ) == 0 )
			return true;
	}
	return false;
}

void CAtsEngine::ClearData()
{
	m_bIsError = false;
	DetachList();
}

void CAtsEngine::SetError(int nErrorCode , LPCTSTR strErrorWord , int nErrorLine)
{
	ASSERT( false );
	if( m_bIsError )
		return;

	m_bIsError			= true;
	m_nLastErrorCode	= nErrorCode;
	m_nErrorLine		= nErrorLine;
	m_strErrorWord		= strErrorWord;
}
BOOL CAtsEngine::RunSubRegmentIns( CAtsSegIns *pSubSegIns , CAtsParam *pParam , CAtsSegIns *pParentSegIns )
{
	CAtsString		strCmdLine;
	BOOL			bExit;
	if( !pSubSegIns->LoadParamData(pParam) )
	{
		m_bIsError		= true;
		if( pParentSegIns )
			pParentSegIns->m_pSegmentItem->GetSourneLine( pParentSegIns->m_nNextPos , m_nErrorLine , m_nErrorLinePos );
		else
			m_nErrorLine = pSubSegIns->m_pSegmentItem->m_nBeginPos;

		TRACE("Script Error:%s,%d" , __FILE__ , __LINE__ );
		return false;
	}
	while( true )
	{
		if( !pSubSegIns->GetNextCmdLine(strCmdLine) )
			break;

		if( strCmdLine.IsEmpty() )
			continue;
		if( strCmdLine == "{" )
			continue;
		CAtsString		strVarName,strVarType;
		RunCmdLine( pSubSegIns , strCmdLine , bExit );
		if( m_bIsError )
		{
			if( pSubSegIns->m_nLineErrorPos >= 0 )
				pSubSegIns->m_pSegmentItem->GetSourneLine( pSubSegIns->m_nLineErrorPos , m_nErrorLine , m_nErrorLinePos );
			else
				pSubSegIns->m_pSegmentItem->GetSourneLine( pSubSegIns->m_nNextPos - 1 , m_nErrorLine , m_nErrorLinePos );

			return false;
		}
		if( bExit )
			break;
		CAtsSentIns * pTopSentIns = NULL;
		pTopSentIns = pSubSegIns->SentTop();
 		if( pTopSentIns && pTopSentIns->m_pSentence->IsEndSent( pSubSegIns->m_nNextPos-1 ) )
			pSubSegIns->m_nNextPos = pTopSentIns->m_pSentence->m_nBeginPos;
	}
	return true;
}
BOOL CAtsEngine::DoCustomSysFun( CAtsString strFunName, CAtsParam *pParam , CAtsSegIns *pSegIns)
{
	CAtsSegmentItem *	pItem = NULL;
	CAtsString			strError;

	pItem = GetSegment(strFunName);
	if( !pItem )
	{
		SetError( ATS_ERRCODE_NOSEGMENT , (LPCTSTR)strFunName );
		return false;
	}

	CAtsSegIns * pSubSegIns = new CAtsSegIns;
	
	pSubSegIns->m_pEngine		= this;
	pSubSegIns->m_pSegmentItem	= pItem;

	m_listSegIns.AddHead( pSubSegIns );
	if( !RunSubRegmentIns( pSubSegIns , pParam, pSegIns ) )
		return false;
	POSITION		pos;
	CAtsVarItem		*pVarItem	= NULL;
	CAtsVarItem		*pTempItem	= NULL;
	m_listSegIns.RemoveHead();
	if( m_listSegIns.GetCount() > 0 )
	{
		for( pos = pSubSegIns->m_InParam.m_listParam.GetHeadPosition() ; pos ; )
		{
			pVarItem = ( CAtsVarItem * )pSubSegIns->m_InParam.m_listParam.GetNext( pos );
			if( pVarItem->m_nFlag & ATSVAR_FLAG_OUT )
			{
				if( IsVar( (LPCTSTR)pVarItem->m_vData.GetStrData() , pSegIns  ) )
				{
					pTempItem = pSubSegIns->GetVarItem( pVarItem->m_strName );
					if( pTempItem )
						SetVar( pVarItem->m_vData.GetStrData() , (CAtsValue *)&pTempItem->m_vData , pSegIns );
				}
			}
		}
	}
	if( pParam )
		pParam->m_vReturn = pSubSegIns->m_InParam.m_vReturn;

	delete pSubSegIns;
	pSubSegIns   = NULL;
	return true;
}
CAtsObject * CAtsEngine::GetClassTemplate( CAtsString strType )
{
	POSITION		pos;
	CAtsObject		*pItem = NULL;

	for( pos = m_listExtClass.GetHeadPosition() ; pos ; )
	{
		pItem = (CAtsObject *)m_listExtClass.GetNext( pos );
		if( pItem->m_strClassName.CompareNoCase( strType ) == 0 )
			return pItem;
	}
	return NULL;
}

CAtsVarItem * CAtsEngine::NewVarItem( CAtsString strName , CAtsString strVarType )
{
	int				nVarType;
	CAtsObject		*pExtClass = NULL;

	nVarType = CAtsVarItem::GetVarType( strVarType );
	if( nVarType != ATSVAR_TYPE_ATSOBJECT )
	{
		pExtClass = GetClassTemplate( strVarType );
		if( !pExtClass )
			return NULL;
		if( nVarType == ATSVAR_TYPE_UNKNOW )
			nVarType = ATSVAR_TYPE_ATSOBJECT;
	}
	
	CAtsVarItem *pNewVar = new CAtsVarItem;

	pNewVar->m_strName			= strName;
	pNewVar->m_vData.m_nType	= nVarType;

	if( pExtClass )
	{
		CAtsObject	*pObj = pExtClass->NewObject();
		pObj->m_pEngine				= this;
		pNewVar->m_vData.m_pObject	= pObj;
	}

	return pNewVar;
}
void CAtsEngine::Goto( CAtsSegIns *pCurSegIns , CAtsString strTag )
{
	int				nPos;
	POSITION		OldPos,pos;
	CAtsSentIns	*pItem;

	nPos = pCurSegIns->m_pSegmentItem->GetTagIndex( strTag );
	pCurSegIns->m_nNextPos = nPos;
	for( pos = pCurSegIns->m_listSentenceHeap.GetHeadPosition() ; pos ;  )
	{
		OldPos	= pos;
		pItem	= (CAtsSentIns *)pCurSegIns->m_listSentenceHeap.GetNext( pos );

		if( pItem->m_pSentence->m_nBeginPos > nPos || pItem->m_pSentence->m_nEndPos < nPos )
		{
			pCurSegIns->m_listSentenceHeap.RemoveAt( OldPos );
			delete pItem;
		}
	}
}
void CAtsEngine::SetConstVar(LPCTSTR sName , CString strData , int nType )
{
	CAtsVarItem *pVarItem	= new CAtsVarItem;
	pVarItem->m_bIsConst	= true;
	pVarItem->AddRef();
	pVarItem->m_strName = sName;
	pVarItem->m_vData.SetDataEx( strData , nType );
	m_listUserVar.AddTail( pVarItem );
}
CString	CAtsEngine::GetSystemParam( CString strName )
{
	return _T("");
}