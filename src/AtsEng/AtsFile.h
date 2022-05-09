// AtsFile.h: interface for the CAtsFile class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (陈凌鹏)
// 2021/5/1
// ver:1.8
// 代码免费使用，如果用于商务，请联系作者(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATSFILE_H__F37AEEC4_7814_449E_8233_3837229F002F__INCLUDED_)
#define AFX_ATSFILE_H__F37AEEC4_7814_449E_8233_3837229F002F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AtsDef.h"
#include "MyMemFile.h"
#include "AtsSegmentItem.h"

class CAtsEngine;
class ATS_API CAtsFile  
{
public:
	CAtsFile();
	virtual ~CAtsFile();
	
	BOOL IsPreCompiled( CAtsString strLine , CAtsString &strCode );
	BOOL ReadLineFromFile( CAtsString &strData , BOOL &bIsEend );
	BOOL OpenFile( CAtsString strFileName );
	void LoadScript( CAtsString strScript );
	void CloseFile();
	BOOL IsEndFile();
	CAtsSegmentItem * GetSegment( CAtsString strLine );
	BOOL IsSegment( CAtsString strLine , CAtsString &strSegmentData , int &nSegmentType );
	BOOL IsSegmentEnd( CAtsString strLine , int &nSegmentType );
	BOOL GetDataLine(CAtsString &strLine , bool bCancelEmpLine  );
	
	static BOOL IsRem(CAtsString strLine);
	static BOOL ReadFromFile( CAtsString strFileName , CAtsString &strData  );
	static BOOL IsVar( CAtsString strLine , CAtsString &strVarName , CString &strVarType );
	static void GetLastWord( CAtsString strLine, CAtsString &strLastWord, CAtsString &strOtherData );
	static void GetFistWord( CAtsString strLine, CAtsString &strFistWord, CAtsString &strOtherData );
	static BOOL GetElementItem(CAtsString &strStuff, CAtsString &strItem,CAtsString strApart);

	CMyMemFile		m_MemFile;
	CFile			m_IOFile;
	BOOL			m_bUserMemFile;
	BOOL			m_bEndFile;
	CAtsString		m_strCurSegment;
	int				m_nCurLine;
	CAtsEngine		*m_pEngine;
};

#endif // !defined(AFX_ATSFILE_H__F37AEEC4_7814_449E_8233_3837229F002F__INCLUDED_)
