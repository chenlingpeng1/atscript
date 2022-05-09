// AtsEng.h: interface for the CAtsEngine class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (������)
// 2021/5/1
// ver:1.8
// �������ʹ�ã����������������ϵ����(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATSENG_H__997C2252_D48D_4766_A6B6_5C93750CBCB0__INCLUDED_)
#define AFX_ATSENG_H__997C2252_D48D_4766_A6B6_5C93750CBCB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtsDef.h"
#include "AtsSegmentItem.h"
#include "AtsSegIns.h"
#include "AtsObject.h"
#include "AtsFile.h"
#include "AtsSentence.h"
#include "ScriptExpExt.h"

//==================================
// AtScript ����
//==================================
class ATS_API CAtsEngine  
{
public:
	CAtsEngine();
	virtual ~CAtsEngine();

	BOOL	RunSubRegmentIns(CAtsSegIns *pSegIns , CAtsParam *pParam = NULL, CAtsSegIns *pParentSegIns = NULL );
	void	Goto( CAtsSegIns *pCurSegIns , CAtsString strTag );
	CAtsVarItem* NewVarItem( CAtsString strName , CAtsString strVarType);
	void	RunCmdLine_simple(CAtsSegIns *pCurSegIns , CAtsString strCmdLine , BOOL &bExit);
	BOOL	IsVar( LPCTSTR sName , CAtsSegIns *pSegIns);
	BOOL	IsCoustomFun( CAtsString strFunName );
	CAtsVarItem * GetVarItem(CAtsSegIns *pSegIns ,LPCTSTR sName );
	BOOL	CheckCondition( CAtsString strCondition , CAtsSegIns *pSegIns);
	BOOL	SetDataEx( CAtsString strVarName , CAtsString strExp ,CAtsSegIns *pSegIns);
	BOOL	SetData( CAtsString strVarName , CAtsString strOther , CAtsSegIns *pSegIns );
	BOOL	RunFun( CAtsString strFunName , CAtsParam *pParam , CAtsSegIns *pSegIns);
	BOOL	RunFun( CAtsString strFunName , CAtsString strParam , CAtsSegIns *pSegIns);
	BOOL	DoAtsSysFun( CAtsSegIns *pSegIns , CAtsString strFunName, CAtsParam *pParam );
	BOOL	ExistSegment( CAtsString strSegmentName );
	BOOL	LoadScript(LPCTSTR strScript);
	BOOL	LoadScriptFile(LPCTSTR strScriptFile);
	BOOL	LoadScriptFile(CAtsFile *pScriptFile);
	CAtsString GetFactData( CAtsString strData , CAtsSegIns *pSegIns );
	CAtsString GetFactData( CAtsValue * pData , CAtsSegIns *pSegIns );
	BOOL	IsObjVar(CAtsString strName , CAtsString &strObjName , CAtsString &strVarName );
	void	DetachList(CAtsString strListName = _T("") );
	BOOL	RunSegment( CAtsString strSegmentName , CAtsParam *pParam = NULL );
	void	ClearData();
	void	RunCmdLine(CAtsSegIns * pCurSegIns , CAtsString strCmdLine, BOOL &bExit);
	CAtsSegmentItem * GetSegment(CAtsString strName);
	CAtsObject * GetObjIns( CAtsString strName ,CAtsSegIns *pSegIns);
	BOOL		DoCustomSysFun( CAtsString strFunName, CAtsParam *pParam , CAtsSegIns *pSegIns);
	BOOL		DoObjFun( CAtsString strFistWord , CAtsParam *pParam , CAtsSegIns *pSegIns);
	void		SetVar(LPCTSTR strName , LPCTSTR strData, CAtsSegIns *pSegIns);
	void		SetVar(LPCTSTR sName , int nData, CAtsSegIns *pSegIns);
	void		SetVar(LPCTSTR sName , CAtsValue *pData, CAtsSegIns *pSegIns);
	CAtsString	GetVar(LPCTSTR strName, BOOL &bExist, CAtsSegIns *pSegIns);
	void		AddObjIns( LPCTSTR sName , CAtsObject *pObjCalss );
	BOOL		Computer( CAtsString strExpress , CAtsValue *pValue , CAtsSegIns *pSegIns);
	void		SetError(int nErrorCode , LPCTSTR strErrorWord = _T("") , int nErrorLine = -1);
	CAtsObject *GetClassTemplate( CAtsString strType );
	void		AddClassTemplate( CAtsObject *pObjCalss , CString sName = _T("") );
	void		SetConstVar(LPCTSTR sName , CString strData, int nType );

	void		SetGlobalParam( CString strName , CString strData );
	CString		GetGlobalParam( CString strName );
	CString		GetSystemParam( CString strName );

	CScriptExpExt	m_ExpExt;
//??	CAtsSegIns	*	m_pCurSegIns;
	CObList			m_listSegment;		// ���� Section �� function
	CPtrList		m_listUserVar;		// �û�����ı���
	CPtrList		m_listExtClass;		// ��ģ��(�����û��������)
	CObList			m_listSegIns;		// Segment ��ʵ����ջ
	CString			m_strScriptPath;	// Script �� Source Path
	CString			m_strVBScript;
	CString			m_strAppName;

	CStringArray	m_asGlobalParamName;	// ϵͳ�����������нű�ʱ���룬�������ڽű���
	CStringArray	m_asGlobalParamData;	//    �� GetSysParam �õ� Param ��ֵ

	// ������Ϣ
	BOOL			m_bIsError;			// �Ƿ����
	int				m_nLastErrorCode;	// �������
	int				m_nErrorLine;		// �������
	int				m_nErrorLinePos;	
	CAtsString		m_strErrorWord;		// �������

public:
	virtual  CAtsObject * CreateAtsObject(LPCTSTR strName)	{return NULL;}
};

#endif // !defined(AFX_ATSENG_H__997C2252_D48D_4766_A6B6_5C93750CBCB0__INCLUDED_)
