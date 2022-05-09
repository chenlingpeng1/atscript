// WinApiIns.h: interface for the CDllModule class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (陈凌鹏)
// 2021/5/1
// ver:1.8
// 代码免费使用，如果用于商务，请联系作者(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINAPIINS_H__3F1B95C8_370B_4CF7_8A21_B395E9B045E7__INCLUDED_)
#define AFX_WINAPIINS_H__3F1B95C8_370B_4CF7_8A21_B395E9B045E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDllModule  
{
	class CProcDef : public CObject
	{
	public:
		CProcDef();
		virtual ~CProcDef();

		CString			m_strProcName;
		CStringArray	m_asParamName;
		CStringArray	m_asParamType;
		CUIntArray		m_anParamIsOut;
		CUIntArray		m_anParamSize;
		CString			m_strReturnType;

		BOOL LoadData( CString strDefine );	
		static CProcDef * GetProcDef( CObList &listDef, CString strName );
		static void DetachList( CObList &listDef );
		static void DeleteDef( CObList &listDef , CString strProName );
		static BOOL GetElementItem(CString &strStuff, CString &strItem,CString strApart);
		static void GetFistWord( CString strLine, CString &strFistWord, CString &strOtherData );
	};

public:
	CDllModule();
	virtual ~CDllModule();

	HINSTANCE		m_hLibIns;
	CObList			m_listProcDef;

	BOOL AddProcDefine(CString strDefine );
	BOOL LoadLibrary( CString strFile );
	BOOL CallProc( CString strProcName , CStringArray &asParam , CString &strReturn);
};

#endif // !defined(AFX_WINAPIINS_H__3F1B95C8_370B_4CF7_8A21_B395E9B045E7__INCLUDED_)
