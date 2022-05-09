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

	AddClassTemplate( new CWndParam );		// CWindParam
	AddClassTemplate( new CValueSet );		// CValueSet
	AddClassTemplate( new CRegKeyObj );		// CRegKey
	AddClassTemplate( new CStringObj );		// string
	AddClassTemplate( new CIntObj );		// CRegKey
	AddClassTemplate( new CDoubleObj );		// string
	AddClassTemplate( new CBoolObj );		// CBoolObj
	AddClassTemplate( new CWSModuleObj );		
	AddClassTemplate( new CDllModuleObj );

	// 向引擎中加入常量
	SetConstVar( "true"  , "1" , ATSVAR_TYPE_BOOL);
	SetConstVar( "false" , "0" , ATSVAR_TYPE_BOOL);

	// 向引擎中加入 System 对象类
	//CSystemObj * pSystemObj = new CSystemObj;
	//m_AtsEng.AddObjIns( _T("System") , pSystemObj );
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

/*	if( strListName.IsEmpty() || strListName == "m_listObjClass" )
	{
		for( pos = m_listObjClass.GetHeadPosition() ; pos ; )
		{
			pObject = (CAtsObject *)m_listObjClass.GetNext(pos); 
			delete pObject;
		}
		m_listObjClass.RemoveAll();

		if( !strListName.IsEmpty() )
			return;
	}*/
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
	BOOL				bInSegment = false;	// 是否正在读取一个 Segment
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
			// 把空行也加到段中,主要是为了在出错时能定位出行号
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

//===================================================
// 运行一个 Segment
//===================================================
BOOL CAtsEngine::RunSegment( CAtsString strSegmentName , CAtsParam *pParam )
{
	return DoCustomSysFun( strSegmentName, pParam , NULL );
}

//====================================================
// 运行一个命令行
//====================================================
void CAtsEngine::RunCmdLine(CAtsSegIns *pCurSegIns , CAtsString strCmdLine , BOOL &bExit)
{
	CAtsString			strFistWord,strOther;
	CAtsString			strTagName;
	CAtsString			strReturn;
	BOOL				bRet = false;
	CAtsSentence		*pSentence = NULL;
	CAtsSentIns			*pSentIns  = NULL;
	
	bExit = false;

	// 最后一个字符为 ':' 表示为 标签
	if( strCmdLine.Right(1) == ":" )
		return;

	CAtsFile::GetFistWord( strCmdLine , strFistWord , strOther );
	if( strFistWord.CompareNoCase("if") == 0 )
	{
		// if()
		CAtsSentIns			*pSentIns  = NULL;
		CAtsSentence		*pSentence = NULL;
		CAtsString			strCondition;

		pCurSegIns->m_nNextPos--;
		// 得到 if 的语句对象
		pSentence = pCurSegIns->GetSentence_If( pCurSegIns->m_nNextPos , strCmdLine );
		if( !pSentence ) 
		{
			m_bIsError = true;
			return;
		}

		pSentIns = pCurSegIns->SentTop();
		if( pSentIns == NULL || pSentIns->m_pSentence != pSentence )
		{
			// 如果还没有加入堆栈,则加入
			pSentIns = new CAtsSentIns;
			pSentIns->m_pSentence = pSentence;
			pCurSegIns->m_listSentenceHeap.AddHead( pSentIns );
		}
		else
		{
			// 有可能格式为:
			//      if( ... )
			//           MessageBox(...);
			// 没有 {} ，则在RunCmdLine 后会判断是否到了当前语句段结束，如果是会到语句段的开始，则会执行以下代码
			// 如果已存在,则表示执行过了,则退出
			pCurSegIns->m_nNextPos = pSentence->m_nEndPos + 1;
			pCurSegIns->RemoveTopSentIns();
			return;
		}

		// 判断条件
		CSentenceSite	*pSiteItem = NULL;
		POSITION		SitePos;
		for( SitePos = pSentence->m_listSite.GetHeadPosition() ; SitePos ; )
		{
			pSiteItem = (CSentenceSite *)pSentence->m_listSite.GetNext( SitePos );
			if( pSiteItem->m_strCondition.IsEmpty() )
			{
				// 如果无条件,则条件成立, 如 if ... else ... 
				pCurSegIns->m_nNextPos = pSiteItem->m_nBeginPos+1;
				return;
			}
			bRet = CheckCondition( pSiteItem->m_strCondition , pCurSegIns );
			CHECK_ERROR()
			if( bRet )
			{
				// 条件成立
				pCurSegIns->m_nNextPos = pSiteItem->m_nBeginPos+1;
				return;
			}
		}

		// 条件都不满足
		// 取出自已
		pCurSegIns->RemoveTopSentIns();
		pCurSegIns->m_nNextPos = pSentence->m_nEndPos + 1;
	}
	else if( strFistWord.CompareNoCase("}") == 0  )
	{
		// 结束一个语句
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
		//While
		CAtsSentence		*pSentence = NULL;
		CAtsString			strCondition;

		pCurSegIns->m_nNextPos--;
		// 得到 if 的语句对象
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
			// 如果还没有加入堆栈,则加入
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
			// 条件成立
			pCurSegIns->m_nNextPos++;
		}
		else
		{
			// 如果已加入堆栈,则取出
			pCurSegIns->RemoveTopSentIns();
			
			// 结束了, 跳过 end if
			pCurSegIns->m_nNextPos = pSentence->m_nEndPos + 1;
		}
	}
	else if( strFistWord.CompareNoCase("for") == 0 )
	{
		//for...next 语句
		CAtsString			strCondition;

		pCurSegIns->m_nNextPos--;
		// 得到 for 的语句对象
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
			// 如果还没有加入堆栈,则加入
			pSentIns = new CAtsSentIns;
			pSentIns->m_nRunCount++;
			pSentIns->m_pSentence = pSentence;
			pCurSegIns->m_listSentenceHeap.AddHead( pSentIns );

			// 执行初始化命令
			RunCmdLine_simple( pCurSegIns , pSentence->m_strInitCmd , bExit);
		}
		else
		{
			// 执行Next命令
			pSentIns->m_nRunCount++;
			RunCmdLine_simple( pCurSegIns , pSentence->m_strNextCmd , bExit);
		}
		
		bRet = CheckCondition( pSentence->m_strCondition, pCurSegIns  );
		CHECK_ERROR()
		if( bRet )
		{
			// 条件成立
			pCurSegIns->m_nNextPos++;
		}
		else
		{
			// 如果已加入堆栈,则取出
			pCurSegIns->RemoveTopSentIns();
			
			// 结束了, 跳过 end if
			pCurSegIns->m_nNextPos = pSentence->m_nEndPos + 1;
		}
	}
	else if( strFistWord.CompareNoCase("switch") == 0 )
	{
		// switch 语句
		CAtsSentence		*pSentence = NULL;
		CAtsString			strCondition;

		pCurSegIns->m_nNextPos--;
		// 得到 if 的语句对象
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
			// 如果还没有加入堆栈,则加入
			pSentIns = new CAtsSentIns;
			pSentIns->m_nRunCount++;
			pSentIns->m_pSentence = pSentence;
			pCurSegIns->m_listSentenceHeap.AddHead( pSentIns );
		}
		else
			pSentIns->m_nRunCount++;
		
		// 判断case条件
		CSentenceSite	*pSiteItem = NULL;
		POSITION		SitePos;
		for( SitePos = pSentence->m_listSite.GetHeadPosition() ; SitePos ; )
		{
			pSiteItem = (CSentenceSite *)pSentence->m_listSite.GetNext( SitePos );
			if( pSiteItem->m_strCondition.IsEmpty() )
			{
				// case else : 表示条件成立
				pCurSegIns->m_nNextPos = pSiteItem->m_nBeginPos+1;
				return;
			}
			CAtsString		strExp;
			strExp.Format("(%s) == (%s)" , pSentence->m_strCondition , pSiteItem->m_strCondition );
			bRet = CheckCondition( strExp, pCurSegIns  );
			CHECK_ERROR()
			if( bRet )
			{
				// 条件成立
				pCurSegIns->m_nNextPos = pSiteItem->m_nBeginPos+1;
				return;
			}
		}

		// 如果已加入堆栈,则取出
		pCurSegIns->RemoveTopSentIns();
		// 结束了, 跳过 end if
		pCurSegIns->m_nNextPos = pSentence->m_nEndPos + 1;
		return;
	}
	else if( strFistWord.CompareNoCase("case") == 0  )
	{
		// 如果没有 switch 语句在运行,则出错
		CAtsSentIns *pItem = pCurSegIns->SentTop();
		if( !pItem ) SET_ERROR( ATS_ERRCODE_SWITCH_ERROR )
		if( pItem->m_pSentence->m_nType != ATS_SENTENCE_TYPE_SWITCH ) SET_ERROR( ATS_ERRCODE_SWITCH_ERROR )
		pCurSegIns->m_nNextPos = pItem->m_pSentence->m_nEndPos + 1;
		//pCurSegIns->RemoveTopSentIns();
	}
	else if( strFistWord.CompareNoCase("do") == 0 )
	{
		//Do...while 语句
		CAtsSentence		*pSentence = NULL;
		CAtsString			strCondition;

		pCurSegIns->m_nNextPos--;
		// 得到 if 的语句对象
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
			// 如果还没有加入堆栈,则加入
			pSentIns = new CAtsSentIns;
			pSentIns->m_nRunCount++;
			pSentIns->m_pSentence = pSentence;
			pCurSegIns->m_listSentenceHeap.AddHead( pSentIns );
		}
		else
		{
			// 执行Next命令
			pSentIns->m_nRunCount++;
		}

		// 条件成立
		pCurSegIns->m_nNextPos++;
	}
	else if( strFistWord.CompareNoCase("break") == 0 )
	{
		//1. for
		//2. while
		//3. swith
		CAtsSentIns *pItem = NULL;
		
		pCurSegIns->SentPop_Cant_Break();	// 先取出语句堆的不支持  break 指令的段,如 IF
		pItem = pCurSegIns->SentTop();
		if( !pItem ) SET_ERROR( ATS_ERRCODE_EXIT_ERROR )
		pCurSegIns->m_nNextPos = pItem->m_pSentence->m_nEndPos + 1;
		pCurSegIns->RemoveTopSentIns();
	}
	else if( strFistWord.CompareNoCase("return") == 0 )
	{
		CAtsValue		vReturn;

		strReturn = (LPCTSTR)strOther;
		// 从一个 Segment 中退出
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
		// 执行 Object 的 Function , 如: Install.DoDailog sdWellcom,@@Param
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
		// 如果每一个字是变量，则进行赋值操作
		if( !SetDataEx( strFistWord , strOther , pCurSegIns ) )
		{
			m_bIsError = true;
			m_nLastErrorCode = ATS_ERRCODE_SETDATA_ERROR;
		}
	}
	else if( strFistWord.CompareNoCase("Set") == 0 )
	{
		// 赋值
		CAtsFile::GetFistWord( strOther , strFistWord , strOther );
		if( !IsVar(strFistWord,  pCurSegIns ) )
			SetError( ATS_ERRCODE_SETDATA_ERROR );
		
		if( !SetDataEx( strFistWord , strOther , pCurSegIns ) )
			SetError( ATS_ERRCODE_SETDATA_ERROR );
	}
	else if( strFistWord.CompareNoCase("Goto") == 0 )
	{
		// 跳转
		Goto( pCurSegIns , strOther );
	}
	else
	{
		// 先判断是否为 function
		if( strOther.Left(1) == "=" )
		{
			// 如果 Other 的第一个字符是 = ,并 strFistWord 不是变量，则出错
			SetError( ATS_ERRCODE_NOTEXISTVAR , strFistWord );
		}
		else
		{
			if( !RunFun( (LPCTSTR)strFistWord , (LPCTSTR)strOther , pCurSegIns ) )
				m_bIsError = true;
		}
	}
}

//==========================================
// 赋值, strExp 是一个表达式
// strExp 中要包括 '='
//==========================================
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
	// 长度最小为2,如: "=3"
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
	
	// 先判断是不是一些特殊的赋值Function
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
		// 是创建 Ats 对象
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

//==========================================
// 赋值
//==========================================
BOOL CAtsEngine::SetData( CAtsString strVarName , CAtsString strOther , CAtsSegIns *pSegIns)
{
	CAtsString		strItem;

	strOther.TrimLeft(' ');
	CAtsFile::GetElementItem( strOther,  strItem, " " );
	if( strItem != "=" || strOther.IsEmpty() )
		return false;
	if( strOther.Right(1) == ")" )
	{
		//path: 表示为一个 Fun
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

//===================================================
// 执行一个 function
//===================================================
BOOL CAtsEngine::RunFun( CAtsString strFunName , CAtsString strParam , CAtsSegIns *pSegIns)
{
	CAtsExpMgr		Jfe;
		
	Jfe.m_pFEExt = &m_ExpExt;
	// 整行进行计算,则所有的 function 都必须有括号
	//Jfe.Computer( (LPCTSTR)strCmdLine , strReturn );
	//if( Jfe.m_bIsError  )
	//	m_bIsError = true;
	
	// 只对后面进行计算,则第一个function 可以没括号
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

//===================================================
// 执行一个 function
//===================================================
BOOL CAtsEngine::RunFun( CAtsString strFunName , CAtsParam *pParam , CAtsSegIns *pSegIns)
{
	if( strFunName.Find('.') > 0 )
	{
		// 执行 Object 的 Function , 如: Install.DoDailog sdWellcom,@@Param
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

	//-------------------------------------------
	// 处理 out 的变量数据
	//-------------------------------------------
	POSITION		pos;
	CAtsVarItem		*pVarItem = NULL;
	for( pos = pParam->m_listParam.GetHeadPosition() ; pos ; )
	{
		pVarItem = (CAtsVarItem *)pParam->m_listParam.GetNext( pos );
		if( pVarItem->m_nFlag & ATSVAR_FLAG_OUT )
		{
			// 如果当前变量的数据是out , 则要对外部的变量进行赋值
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

//===========================================================
// 执行 AtScript 的系统 Function
//===========================================================
BOOL CAtsEngine::DoAtsSysFun( CAtsSegIns *pSegIns , CAtsString strFunName, CAtsParam *pParam )
{
	CAtsString		strData;

	if( strFunName.CompareNoCase("DataCpy") == 0 )
	{
		// 对一个变量进行赋值
		strData = GetFactData( pParam->GetItemData(2) , pSegIns );
		pParam->SetOutData( 1, strData );
		return true;
	}
	else if( strFunName.CompareNoCase("SetGlobalParam") == 0 )
	{
		// 得到全局的 Param 值
		CString		strName;

		strName = GetFactData( pParam->GetItemData(1) , pSegIns );
		strData = GetFactData( pParam->GetItemData(2) , pSegIns );
		SetGlobalParam( strName , strData);
		return true;
	}
	else if( strFunName.CompareNoCase("GetGlobalParam") == 0 )
	{
		// 得到全局的 Param 值
		CString		strName;

		strName = GetFactData( pParam->GetItemData(1) , pSegIns );
		strData = GetGlobalParam( strName );
		pParam->SetReturnDataEx( strData , ATSVAR_TYPE_STRING );
		return true;
	}
	else if( strFunName.CompareNoCase("GetSystemParam") == 0 )
	{
		// 得到全局的 Param 值
		CString		strName;

		strName = GetFactData( pParam->GetItemData(1) , pSegIns );
		strData = GetSystemParam( strName );
		pParam->SetReturnDataEx( strData , ATSVAR_TYPE_STRING );
		return true;
	}
	/*else if( strFunName.CompareNoCase("CreateNewGUID") == 0 )
	{
		// 得到一个新的 GUID
		CAtsString		strData;
	
		strData = CAtsBaseFun::GenerateGUIDString();
		pParam->SetOutData( 1, strData );
		return true;
	}*/
	else if( strFunName.CompareNoCase("MessageBox") == 0 )
	{
		//MessageBox( LPCTSTR lpszText, LPCTSTR lpszCaption = NULL, UINT nType = MB_OK );
		//MessageBox(prompt[, buttons][, title][, helpfile, context])
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
		// 如果比较数据结果相同则跳转
		// 不支持对参数进行运算, 不是一个真正的 function
		if( pParam->DataCmp( 1 , 2 ) == 0 )
		{
			CAtsString	strTagName = pParam->GetItemData( 3,false);
			Goto( pSegIns , strTagName );
		}
		return true;
	}
	//CallVBSFunctionTof
	//CallVBSFunctionToi
	else if( strFunName.CompareNoCase("CallVBSFunction") == 0 )
	{
		// 调用VBScript 的 function
		// CallVBSFunction( "MyFun" , "abc" , 9 );
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

//====================================================
// 得到一个存在的对象
//====================================================
CAtsObject * CAtsEngine::GetObjIns( CAtsString strName ,CAtsSegIns *pSegIns)
{
	CAtsVarItem		*pVarItem = NULL;

	pVarItem =	GetVarItem( pSegIns , strName );
	if( !pVarItem || !pVarItem->m_vData.m_pObject )// !pVarItem->IsObject() )
		return NULL;
	return  (CAtsObject *)pVarItem->m_vData.m_pObject;
}

//====================================================
// 新建一个对象Class 模板
//====================================================
void CAtsEngine::AddClassTemplate( CAtsObject *pObjCalss , CString sName)
{
	if( !sName.IsEmpty() )
		pObjCalss->m_strClassName	= sName;

	pObjCalss->m_pEngine = this;
	m_listExtClass.AddTail( pObjCalss );
}

//====================================================
// 新建一个对象Class
//====================================================
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

//=======================================================
// 执行一个 Obj 的 function
//=======================================================
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

//==========================================================
// 是否为对象的变量
// 支持 @ 提前
// 支持 @ObjName.VarName 和 ObjName.@VarName
//==========================================================
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

//=======================================================
// 是否为变量
//=======================================================
BOOL CAtsEngine::IsVar( LPCTSTR sName ,  CAtsSegIns *pSegIns )
{
	if( GetVarItem( pSegIns , sName ) )
		return true;
	else
		return false;
}

//=======================================================
// 得到变量对象
//=======================================================
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

	// 判断是不是段中的变量
	if( pSegIns )
	{
		pVarItem = pSegIns->GetVarItem( strName );
		if( pVarItem )
			return pVarItem;
	}

	// 判断是不是全局的变量
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

//??	if( !pSegIns )
//??		pSegIns = m_pCurSegIns;

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

//=====================================================
// 得到真实的数据，如把一个变量名用其值取代
//=====================================================
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
		// 去除字串器引号
		if( strData.Left(1) == "\"" && strData.Right(1) == "\"" )
			strData = strData.Mid(1, strData.GetLength() - 2 );
	}

	return strData;
}

//==================================================
// 判断条件是否成立
//==================================================
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


//==================================================
// 进行表达式计算
//==================================================
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

//==================================================
// 判断是否为Sript 自定放的 Function
//==================================================
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
	// 这样可以保最小的出错信息
	if( m_bIsError )
		return;

	m_bIsError			= true;
	m_nLastErrorCode	= nErrorCode;
	m_nErrorLine		= nErrorLine;
	m_strErrorWord		= strErrorWord;
}

//====================================================
// 运行一个子Seg实例
//====================================================
BOOL CAtsEngine::RunSubRegmentIns( CAtsSegIns *pSubSegIns , CAtsParam *pParam , CAtsSegIns *pParentSegIns )
{
	CAtsString		strCmdLine;
	BOOL			bExit;

	// 先Load 数据事,再设置 m_pCurSegIns , 因为在 LoadParamData
	// 是可能要用到上一层的 Ins
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

//??m_pEngine->m_pCurSegIns = pSegIns;
	while( true )
	{
		if( !pSubSegIns->GetNextCmdLine(strCmdLine) )
			break;

		if( strCmdLine.IsEmpty() )
			continue;
		if( strCmdLine == "{" )
			continue;

		// 首先判断是不是变量
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

		// 判断是不是一个语句段结束了
		CAtsSentIns * pTopSentIns = NULL;
		pTopSentIns = pSubSegIns->SentTop();
 		if( pTopSentIns && pTopSentIns->m_pSentence->IsEndSent( pSubSegIns->m_nNextPos-1 ) )
		{
			// 如果一个语句段结束了(已执行过最后一行),则回到语句段的开始
			pSubSegIns->m_nNextPos = pTopSentIns->m_pSentence->m_nBeginPos;
		}
	}
	return true;
}

//===========================================================
// 执行 自定放的 Function
//===========================================================
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

	// 入载
	m_listSegIns.AddHead( pSubSegIns );

	// 运行
	if( !RunSubRegmentIns( pSubSegIns , pParam, pSegIns ) )
		return false;

	// 出载
	POSITION		pos;
	CAtsVarItem		*pVarItem	= NULL;
	CAtsVarItem		*pTempItem	= NULL;

//??	m_pCurSegIns = NULL;

	// 把最上面的取出
	m_listSegIns.RemoveHead();
	
	// 如果堆中还有 Ins , 地取出
	if( m_listSegIns.GetCount() > 0 )
	{
		// 把最上一个设置为当前 Ins
//??		m_pCurSegIns = (CAtsSegIns *)m_listSegIns.GetHead();

		// 把 pCurSegIns 中 Out 的数据取回
		for( pos = pSubSegIns->m_InParam.m_listParam.GetHeadPosition() ; pos ; )
		{
			pVarItem = ( CAtsVarItem * )pSubSegIns->m_InParam.m_listParam.GetNext( pos );
			if( pVarItem->m_nFlag & ATSVAR_FLAG_OUT )
			{
				// 当前 pVarItem 存入的是不是一个外部变量名
				if( IsVar( (LPCTSTR)pVarItem->m_vData.GetStrData() , pSegIns  ) )
				{
					pTempItem = pSubSegIns->GetVarItem( pVarItem->m_strName );
					if( pTempItem )
						SetVar( pVarItem->m_vData.GetStrData() , (CAtsValue *)&pTempItem->m_vData , pSegIns );
				}
			}
		}
	}

	// 把 Return 的值也取回来
	if( pParam )
		pParam->m_vReturn = pSubSegIns->m_InParam.m_vReturn;

	delete pSubSegIns;
	pSubSegIns   = NULL;
	return true;
}

//==================================================
// 是否是扩展类
//==================================================
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

//========================================================
// 跳转
// 跳转是一个危险的指令，可能会破坏语句段的链表
// 所以有一个规定，语标(Tag)不能入在语句段中(但可以在 sub function 中)
//========================================================
void CAtsEngine::Goto( CAtsSegIns *pCurSegIns , CAtsString strTag )
{
	int				nPos;
	POSITION		OldPos,pos;
	CAtsSentIns	*pItem;

	nPos = pCurSegIns->m_pSegmentItem->GetTagIndex( strTag );
	pCurSegIns->m_nNextPos = nPos;

	//如果 goto 指令是在一个 语句段 中发出的，则要退出这些语句段
	
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

//======================================
// 设置常量
//======================================
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
	if( strName.CompareNoCase("ScriptPath") == 0 )
	{
	}
	else if( strName.CompareNoCase("ScriptDir") == 0 )
	{
	}

	return _T("");
}