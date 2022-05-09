// CJfScriptEngineMgr.h: interface for the CJfScriptEngineMgr class.
//
//////////////////////////////////////////////////////////////////////
// author: chen lingpeng (陈凌鹏)
// 2021/5/1
// ver:1.8
// 代码免费使用，如果用于商务，请联系作者(905309882@qq.com)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CJfScriptECJfScriptEngineMgrngineMgr_H__0169D899_1CD0_11D3_A57C_00A0C9750ADE__INCLUDED_)
#define AFX_CJfScriptEngineMgr_H__0169D899_1CD0_11D3_A57C_00A0C9750ADE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "activscp.h"

#define		ATSVAR_TYPE_STRING		(2)
#define		ATSVAR_TYPE_INT			(3)

#define		JFSCRIPT_ENGINE_NONE	(0)
#define		JFSCRIPT_ENGINE_VBS		(1)
#define		JFSCRIPT_ENGINE_JS		(2)

class CJfScriptParams
{
public:
	CJfScriptParams();
	virtual ~CJfScriptParams();

	void AddItem( long nData);
	void AddItem( CString strData);
	void AddItem( BYTE cType , long nData);
	void AddItem( char  p[2] , long nData );
	void AddItem( BYTE cType , CString strData);
	void AddItem( char  p[2] , CString strData );
	BYTE	*NewParamTypes();
	DWORD	*NewParamDatas();
	void	AddParams(CString strParam );

	CObList		m_listParam;
};

class CJfScriptParam : public CObject  
{
public:
	CJfScriptParam();

	BYTE	m_cType;
	DWORD	m_nData;
	CString	m_strData;
};

///////////////////////////////////
// 用于存放 Script 中的用户对象
///////////////////////////////////
class CJfExposedObject  
{
public:
	CJfExposedObject();
	virtual ~CJfExposedObject();

	CString GetObjectName();
	void SetObjectName(CString newName);
	IUnknown* GetObjectInterface();
	void SetExposedObject(IUnknown* newCJfExposedObject);

private:
	IUnknown*	m_pExposedObject;
	CString		m_ObjectName;
};

///////////////////////////////////
// CJfActiveScriptSite
///////////////////////////////////
class CJfActiveScriptSite : public IActiveScriptSite,
						 public IActiveScriptSiteWindow  
{
public:
	// ctor / dtor
	CJfActiveScriptSite();
	virtual ~CJfActiveScriptSite();

public:
	// IUnknown Methods
	virtual HRESULT _stdcall QueryInterface(REFIID riid, void** ppvObject);
	virtual ULONG _stdcall AddRef();
	virtual ULONG _stdcall Release();

public:
	// ICJfActiveScriptSite methods
	virtual HRESULT _stdcall GetLCID(LCID* plcid);
	virtual HRESULT _stdcall GetItemInfo(LPCOLESTR pstrName, DWORD dwReturnMask, IUnknown** ppunkItem, ITypeInfo** ppti);
	virtual HRESULT _stdcall GetDocVersionString(BSTR* pbstrVersion);
	virtual HRESULT _stdcall OnScriptTerminate(const VARIANT* pvarResult, const EXCEPINFO* pexcepInfo);
	virtual HRESULT _stdcall OnStateChange(SCRIPTSTATE ssScriptState);
	virtual HRESULT _stdcall OnScriptError(IActiveScriptError* pScriptError);
	virtual HRESULT _stdcall OnEnterScript(void);
	virtual HRESULT _stdcall OnLeaveScript(void);

	// ICJfActiveScriptSiteWindow methods
	virtual HRESULT _stdcall GetWindow(HWND* phWnd);
	virtual HRESULT _stdcall EnableModeless(BOOL fEnable);

public:
	void DetachData();
	CArray<CJfExposedObject*, CJfExposedObject*>	m_aExposedObject;
	CStringArray									m_asErrorMessage;
	BOOL											m_bError;
	HWND											m_hWnd;

private:
	ULONG	m_dwRef;
};

///////////////////////////////////
// Script 管理类
///////////////////////////////////
class  CJfScriptEngineMgr : public COleDispatchDriver
{
public:
	CJfScriptEngineMgr();

public:
	void SetEngine( int nEngineType );
	virtual ~CJfScriptEngineMgr();

	// Singleton instantiator
	static CJfScriptEngineMgr* Instance();

	// Scripting members
//	bool GetMethodNames( CStringArray &asData);
	BOOL CallSimpleFun(CString strFunName , CJfScriptParams *pParam , CString &strReturn );
	void AddExposedObjectPtr(CString newObjectName, IUnknown* newExposedObject);
	void InitializeScriptEngine(int nEngineType = JFSCRIPT_ENGINE_NONE);
	void ClearErrorMessages();
	BOOL RunScript(CString newScript);
	BOOL LoadScript(CString newScript);
	BOOL UnloadScript();
	BOOL GetIDOfFunction(CString strFunctionName, DISPID* pID);
	void GetErrorMessages(CStringArray &asErrorInfo);
	BOOL IsError();

	void AFX_CDECL InvokeHelperEx(DISPID dwDispID, WORD wFlags,
		VARTYPE vtRet, void* pvRet, const BYTE* pbParamInfo, ...);
private:
	BOOL HRVerify(HRESULT hr, CString msg);

private:
	// Other members
	CJfActiveScriptSite	m_ScriptSite;
	IActiveScript*		m_iActiveScript;
	IActiveScriptParse*	m_iActiveScriptParse;
	IDispatch*			m_pScriptDispatch;

	BOOL				m_bInitData;
	int					m_nEngineType;
};





#endif // !defined(AFX_CJfScriptEngineMgr_H__0169D899_1CD0_11D3_A57C_00A0C9750ADE__INCLUDED_)
