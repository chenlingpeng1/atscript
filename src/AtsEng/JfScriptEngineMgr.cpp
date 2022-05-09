//////////////////////////////////////////////////////////////////////
// 例：调用一个函数 MyFunction MyFunction1
// 
//  CJfScriptEngineMgr m_ScriptFactory;
//
//  m_ScriptFactory.InitializeScriptEngine();
//	
//	m_ScriptFactory.AddCJfExposedObjectPtr( "capp", pDoc->GetIDispatch(TRUE) );
//	m_ScriptFactory.LoadScript(m_strScript);
//
//	DISPID	dispid;
//	long	result;
//	if( m_ScriptFactory.GetIDOfFunction("MyFunction", &dispid)) 
//		m_ScriptFactory.InvokeHelper(dispid, DISPATCH_METHOD, VT_EMPTY, (void*) &result, NULL);
//
//  if( m_ScriptFactory.GetIDOfFunction("MyFunction1", &dispid)) 
//	{
//		static BYTE parms[] = VTS_I4 VTS_I4;
//		m_ScriptFactory.InvokeHelper( dispid, DISPATCH_METHOD, VT_EMPTY, (void*) &result, parms , 9 , 8);
//	}
//
//  m_ScriptFactory.UnloadScript();
// 
//////////////////////////////////////////////////////////////////////
//  运行一个 Function 返回 CString
//////////////////////////////////////////////////////////////////////
//  CJfScriptEngineMgr	ScriptFactory;
//	ScriptFactory.InitializeScriptEngine(nEngineType);
//	ScriptFactory.LoadScript(m_strScript);
//
//	DISPID dispid;
//	if( ScriptFactory.GetIDOfFunction("xx", &dispid)) 
//	{
//		CString	result;
//
//		static BYTE parms[] = VTS_I4 VTS_I4;
//	
//		ScriptFactory.InvokeHelper(dispid, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms , 9 ,8 );
//	}
//////////////////////////////////////////////////////////////////////
//  运行一个动态参数的 Function
//////////////////////////////////////////////////////////////////////
//  CJfScriptEngineMgr	ScriptFactory;
//	ScriptFactory.InitializeScriptEngine(nEngineType);
//	ScriptFactory.LoadScript(m_strScript);
//
//	CJfScriptParams	Params;
//	CString			strReturn;
//
//	Params.AddItem( "mf" );
//	Params.AddItem( "clp" );
//	ScriptFactory.CallSimpleFun( "xx" , &Params , strReturn );
//////////////////////////////////////////////////////////////////////
//
//  CJfScriptEngineMgr	ScriptMgr;
//
//  m_pCommand->QueryInterface( IID_IUnknown, (void **)&pUnk );
//	if( !pUnk )
//		return _T("");
//	
//	ScriptMgr.InitializeScriptEngine();
//	ScriptMgr.AddExposedObjectPtr( "CAPP" , pUnk );
//	ScriptMgr.RunScript( strScript );
//
//	pUnk->Release();
//
//	if( ScriptMgr.IsError() )
//	{
//		CStringArray	asErrorInfo;
//		int				nNum,nCount;
//
//		ScriptMgr.GetErrorMessages( asErrorInfo );
//		nCount = asErrorInfo.GetSize();
//		for( nNum = 0 ; nNum < nCount ; nNum++ )
//			AfxMessageBox( asErrorInfo.GetAt( nNum ));
//	}
// 
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JfScriptEngineMgr.h"
#include <afxconv.h>
#include "ComDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const CLSID CLSID_JScript =
	{ 0xf414c260, 0x6ac0, 0x11cf, { 0xb6, 0xd1, 0x00, 0xaa, 0x00, 0xbb, 0xbb, 0x58 } };

const CLSID CLSID_VBScript =
	{ 0xb54f3741, 0x5b07, 0x11cf, { 0xa4, 0xb0, 0x00, 0xaa, 0x00, 0x4a, 0x55, 0xe8 } };

CJfScriptParams::CJfScriptParams()
{
}

CJfScriptParams::~CJfScriptParams()
{
	POSITION			pos;
	CJfScriptParam		*pItem = NULL;

	for( pos = m_listParam.GetHeadPosition() ; pos ; )
	{
		pItem = (CJfScriptParam *)m_listParam.GetNext( pos );
		delete pItem;
	}
	m_listParam.RemoveAll();
}

void CJfScriptParams::AddItem( long nData)
{
	AddItem( VTS_I4 , nData );
}

void CJfScriptParams::AddItem( CString strData)
{
	AddItem( VTS_BSTR , strData );
}

void CJfScriptParams::AddItem( char p[2]  , long nData)
{
	AddItem( *p , nData );
}

void CJfScriptParams::AddItem( char p[2]  , CString strData)
{
	AddItem( *p , strData );
}

// VTS_BSTR VTS_I4
void CJfScriptParams::AddItem( BYTE cType, long nData )
{
	CJfScriptParam *pItem = new CJfScriptParam;
	pItem->m_cType = cType;
	pItem->m_nData = nData;

	m_listParam.AddTail( pItem );
}

// VTS_BSTR VTS_I4
void CJfScriptParams::AddItem( BYTE cType, CString strData )
{
	CJfScriptParam *pItem = new CJfScriptParam;
	pItem->m_cType = cType;
	pItem->m_strData = strData;

	m_listParam.AddTail( pItem );
}

void CJfScriptParams::AddParams(CString strParam )
{

}

BYTE * CJfScriptParams::NewParamTypes()
{
	POSITION	pos;
	BYTE		*parms = new BYTE[m_listParam.GetCount()+1];
	int			nNum = 0;
	CJfScriptParam *pItem = NULL;

	for( pos = m_listParam.GetHeadPosition() ; pos ; )
	{
		pItem = (CJfScriptParam *)m_listParam.GetNext( pos );
		parms[nNum] = pItem->m_cType;
		nNum++;
	}
	parms[m_listParam.GetCount()] = '\0';
	return parms;
}

//===========================
//argData[0]=(DWORD)tszHello;
//argData[1]=(DWORD)tszHello
//argData[0]=(DWORD)8;
//argData[1]=(DWORD)9;
//===========================
DWORD * CJfScriptParams::NewParamDatas()
{
	DWORD *pData = new DWORD[m_listParam.GetCount()];

	int				nNum = 0;
	CJfScriptParam *pItem = NULL;
	POSITION		pos;

	for( pos = m_listParam.GetHeadPosition() ; pos ; )
	{
		pItem = (CJfScriptParam *)m_listParam.GetNext( pos );
		//char  p[2] = VTS_I4;
		if( pItem->m_cType == *(char *)VTS_I4 )
			pData[nNum] = pItem->m_nData;
		else if( pItem->m_cType == *(char *)VTS_BSTR )
		{
			pData[nNum] = (DWORD)pItem->m_strData.GetBuffer( pItem->m_strData.GetLength() );
			pItem->m_strData.ReleaseBuffer();
		}

		nNum++;
	}
	return pData;
}

CJfScriptParam::CJfScriptParam()
{
}

//////////////////////////////////////////////////////////////////////
// 
// CJfScriptEngineMgr
//
//////////////////////////////////////////////////////////////////////
CJfScriptEngineMgr::CJfScriptEngineMgr()
{
	m_nEngineType			= JFSCRIPT_ENGINE_VBS;
	m_bInitData				= false;
	m_iActiveScript         = NULL;
	m_iActiveScriptParse    = NULL;
	m_pScriptDispatch       = NULL;
}

CJfScriptEngineMgr::~CJfScriptEngineMgr()
{
}

//=======================================================
// 与 COleDispatchDriver::InvokeHelper 
// 的区另是：对错误进行了 Catch
//=======================================================
void AFX_CDECL CJfScriptEngineMgr::InvokeHelperEx(DISPID dwDispID, WORD wFlags,
	VARTYPE vtRet, void* pvRet, const BYTE* pbParamInfo, ...)
{
	va_list argList;
	va_start(argList, pbParamInfo);

	TRY
	{
		InvokeHelperV(dwDispID, wFlags, vtRet, pvRet, pbParamInfo, argList);		
	}
	CATCH_ALL(e)
	{
		AfxMessageBox( "脚本执行出错");
		//m_ScriptSite.m_asErrorMessage.Add("脚本执行出错");
		//DELETE_EXCEPTION(e);
	}
	END_CATCH_ALL

	va_end(argList);
}

BOOL CJfScriptEngineMgr::HRVerify(HRESULT hr, CString msg)
{
	// If we have a failure, add the error string including
	// hr value to the errormessages array of the scriptsite
	if (FAILED(hr)) {
		CString		str;

		str.Format("COM Error: 0x%08lx\n", hr);
		str += msg;
		m_ScriptSite.m_asErrorMessage.Add(msg);

		return FALSE;
	}

	return TRUE;
}

void CJfScriptEngineMgr::AddExposedObjectPtr(CString newObjectName, IUnknown* newExposedObject)
{
	USES_CONVERSION;

	HRESULT				hr;
	CString				str;
	CJfExposedObject	*pObj = NULL;

	// Add to namespace
	hr = m_iActiveScript->AddNamedItem(
		T2OLE(newObjectName), 
		SCRIPTITEM_ISVISIBLE | SCRIPTITEM_ISSOURCE);
	str.Format("Could not add Exposed-Script-Object [%s]", newObjectName);
	HRVerify(hr, str);

	// Save IUnknown ptr ...
	pObj = new CJfExposedObject;
	pObj->SetExposedObject(newExposedObject);
	pObj->SetObjectName(newObjectName);

	m_ScriptSite.m_aExposedObject.Add(pObj);
}

void CJfScriptEngineMgr::InitializeScriptEngine(int nEngineType /* = JFSCRIPT_ENGINE_NONE */)
{
	HRESULT		hr;
	CLSID		clsid;
	CString		intMessage, extMessage;

	// 清除错误信息
	ClearErrorMessages();

	if( nEngineType == JFSCRIPT_ENGINE_NONE )
		nEngineType = m_nEngineType;

	if( nEngineType == JFSCRIPT_ENGINE_JS )
	{
		// Find CLSID
		hr = CLSIDFromProgID(L"JavaScript", &clsid);
		if (HRVerify(hr, "Scripting Engine (VBScript.dll) not registered") == FALSE)
			return;
	}
	else
	{
		// Find CLSID
		hr = CLSIDFromProgID(L"VBScript", &clsid);
		if (HRVerify(hr, "Scripting Engine (VBScript.dll) not registered") == FALSE)
			return;
	}



	// Start VBScript.dll (inproc server)
	hr = CoCreateInstance(
		clsid, NULL, CLSCTX_INPROC_SERVER, 
		IID_IActiveScript, (void**) &m_iActiveScript);
	if (HRVerify(hr, "Scripting Engine (VBScript.dll) not started") == FALSE)
		return;

	// QueryInterface for IActiveScriptParse
	hr = m_iActiveScript->QueryInterface(
		IID_IActiveScriptParse, (void**) &m_iActiveScriptParse);
	if (HRVerify(hr, "Scripting Engine Parser not available") == FALSE)
	{
		m_iActiveScript->Release();

		m_iActiveScriptParse = NULL;
		m_iActiveScript      = NULL;
		m_pScriptDispatch    = NULL;

		return;
	}

	// Setup our ScriptSite
	hr = m_iActiveScript->SetScriptSite(&m_ScriptSite);
	if (HRVerify(hr, "Could not host scripting site") == FALSE)
	{
		m_iActiveScript->Release();
		m_iActiveScriptParse->Release();

		m_iActiveScriptParse = NULL;
		m_iActiveScript      = NULL;
		m_pScriptDispatch    = NULL;

		return;
	}

	hr = m_iActiveScriptParse->InitNew();
	if (HRVerify(hr, "Could not initialize scripting parser") == FALSE)
	{
		m_iActiveScript->Release();
		m_iActiveScriptParse->Release();

		m_iActiveScriptParse = NULL;
		m_iActiveScript      = NULL;
		m_pScriptDispatch    = NULL;

		return;
	}

	// Initialize m_pScriptDispatch to NULL
	m_pScriptDispatch = NULL;

	m_ScriptSite.DetachData();

	// Set the application main window as ScriptWindow parent
	if( ::AfxGetMainWnd() )
		m_ScriptSite.m_hWnd = ::AfxGetMainWnd()->m_hWnd;

	m_bInitData = true;
}

BOOL CJfScriptEngineMgr::RunScript(CString newScript)
{
	HRESULT		hr;
	EXCEPINFO	ei;
	BSTR		pParseText = newScript.AllocSysString();

	if( !m_bInitData )
		InitializeScriptEngine( m_nEngineType );

	// Add script code to the engine
	hr = m_iActiveScriptParse->ParseScriptText(
		pParseText, NULL, NULL, NULL, 0, 0, 0L, NULL, &ei);
	if (HRVerify(hr, "Error parsing script text") == FALSE) {
		hr = m_iActiveScriptParse->Release();
		hr = m_iActiveScript->Release();

		m_iActiveScriptParse = NULL;
		m_iActiveScript      = NULL;
		m_pScriptDispatch    = NULL;

		return FALSE;
	}

	// This will actually run the script
	hr = m_iActiveScript->SetScriptState(SCRIPTSTATE_CONNECTED);
	if (HRVerify(hr, "Error running the script") == FALSE) {
		hr = m_iActiveScriptParse->Release();
		hr = m_iActiveScript->Release();

		m_iActiveScriptParse = NULL;
		m_iActiveScript      = NULL;
		m_pScriptDispatch    = NULL;

		return FALSE;
	}

	// This will disconnect the script
	hr = m_iActiveScriptParse->Release();
	hr = m_iActiveScript->Release();

	m_iActiveScriptParse = NULL;
	m_iActiveScript      = NULL;
	m_pScriptDispatch    = NULL;
	
	return TRUE;
}

BOOL CJfScriptEngineMgr::IsError()
{
	return m_ScriptSite.m_bError;
}

//=============================================
// 清除错误信息
//=============================================
void CJfScriptEngineMgr::ClearErrorMessages()
{
	m_ScriptSite.m_bError = false;
	m_ScriptSite.m_asErrorMessage.RemoveAll();
}

//=============================================
// 得到错误信息
//=============================================
void CJfScriptEngineMgr::GetErrorMessages(CStringArray &asErrorInfo)
{
	asErrorInfo.Copy( m_ScriptSite.m_asErrorMessage );
}

BOOL CJfScriptEngineMgr::LoadScript(CString newScript)
{
	HRESULT		hr;
	EXCEPINFO	ei;
	BSTR		pParseText = newScript.AllocSysString();

	if( !m_bInitData )
		InitializeScriptEngine( m_nEngineType );

	// This loads the script and keeps it around for the user
	// to allow to call specific methods in the script
	// Add script code to the engine
	hr = m_iActiveScriptParse->ParseScriptText(
		pParseText, NULL, NULL, NULL, 0, 0, 0L, NULL, &ei);
	if (HRVerify(hr, "Error parsing script text") == FALSE) {
		hr = m_iActiveScriptParse->Release();
		hr = m_iActiveScript->Release();

		m_iActiveScriptParse = NULL;
		m_iActiveScript      = NULL;
		m_pScriptDispatch    = NULL;

		return FALSE;
	}

	// This will actually run the script
	hr = m_iActiveScript->SetScriptState(SCRIPTSTATE_CONNECTED);
	if (HRVerify(hr, "Error running the script") == FALSE) {
		hr = m_iActiveScriptParse->Release();
		hr = m_iActiveScript->Release();

		m_iActiveScriptParse = NULL;
		m_iActiveScript      = NULL;
		m_pScriptDispatch    = NULL;

		return FALSE;
	}

	// We need the IDispatch for the script itself
	// to allow to use InvokeHelper
	hr = m_iActiveScript->GetScriptDispatch(0, &m_pScriptDispatch);
	if (HRVerify(hr, "Error getting script dispatch") == FALSE) {
		m_pScriptDispatch = NULL;
		return FALSE;
	}

	// Attach for easy access to InvokeHelper
	// CJfScriptEngineMgr is actually also derived from COleDispatchDriver,
	// So we can call AttachDispatch to use it's InvokeHelper method
	AttachDispatch(m_pScriptDispatch);

	return TRUE;
}

BOOL CJfScriptEngineMgr::UnloadScript()
{
	m_ScriptSite.DetachData(); 

	// Release script dispatch pointer from this
	DetachDispatch();

	// Release all interfaces
	if (m_iActiveScriptParse)	m_iActiveScriptParse->Release();
	if (m_iActiveScript)		m_iActiveScript->Release();
	if (m_pScriptDispatch)		m_pScriptDispatch->Release();

	m_iActiveScriptParse = NULL;
	m_iActiveScript      = NULL;
	m_pScriptDispatch    = NULL;

	return TRUE;
}

BOOL CJfScriptEngineMgr::GetIDOfFunction(CString strFunctionName, DISPID* pID)
{
	// This method retrieves the DISPID to use when
	// calling a method in the loaded script
	// One needs to know how many parameters are used and
	// if there is a return value
	// FALSE is returned when either there is no script loaded,
	// or the method is not there.
	if (m_pScriptDispatch == NULL)
		return FALSE;

	BSTR pFunctionName = strFunctionName.AllocSysString();
	BOOL bStatus = FALSE;

	bStatus = m_pScriptDispatch->GetIDsOfNames(
		IID_NULL, 
		&pFunctionName, 
		1, 
		LOCALE_SYSTEM_DEFAULT, 
		pID) == S_OK;

	return bStatus;
}

//////////////////////////////////////////////////////////////////////
//
// CJfExposedObject
//
//////////////////////////////////////////////////////////////////////
CJfExposedObject::CJfExposedObject()
{
	m_pExposedObject = NULL;
	m_ObjectName     = "";
}

CJfExposedObject::~CJfExposedObject()
{

}

void CJfExposedObject::SetExposedObject(IUnknown *newExposedObject)
{
	ASSERT(newExposedObject);
	m_pExposedObject = newExposedObject;
}

IUnknown* CJfExposedObject::GetObjectInterface()
{
	return m_pExposedObject;
}

void CJfExposedObject::SetObjectName(CString newName)
{
	m_ObjectName = newName;
}

CString CJfExposedObject::GetObjectName()
{
	return m_ObjectName;
}

//////////////////////////////////////////////////////////////////////
//
// CJfActiveScriptSite
//
//////////////////////////////////////////////////////////////////////
CJfActiveScriptSite::CJfActiveScriptSite()
{
	m_bError	= false;
	m_dwRef		= 1;
	m_hWnd		= NULL;
}

CJfActiveScriptSite::~CJfActiveScriptSite()
{
	DetachData();
}

HRESULT CJfActiveScriptSite::QueryInterface(REFIID riid, void** ppvObject)
{
	HRESULT		hResult = S_OK;

	if (riid == IID_IActiveScriptSiteWindow)
	{
		*ppvObject = (IActiveScriptSiteWindow *) this;
	} 
	else 
	{
		*ppvObject = NULL;
		hResult = E_NOTIMPL;
	}

	return hResult;
}

ULONG CJfActiveScriptSite::AddRef()
{
	return ++m_dwRef;
}

ULONG CJfActiveScriptSite::Release()
{
	if (m_dwRef == 0) return 0;
	return --m_dwRef;
}

HRESULT CJfActiveScriptSite::GetLCID(LCID* plcid)
{
	return S_OK;
}

HRESULT CJfActiveScriptSite::GetItemInfo(LPCOLESTR pstrName, DWORD dwReturnMask, IUnknown** ppunkItem, ITypeInfo** ppti)
{
	USES_CONVERSION;
	BOOL	bFound = FALSE;

	// Is it expecting an ITypeInfo?
	if (ppti)
	{
		// Default to NULL
		*ppti = NULL;

		// Return if asking about ITypeInfo
		if (dwReturnMask & SCRIPTINFO_ITYPEINFO) {
			return TYPE_E_ELEMENTNOTFOUND;
		}
	}
	int i;
	// Is the engine passing a IUnknown buffer?
	if (ppunkItem) {
		// Default to NULL
		*ppunkItem = NULL;

		// Is the script looking for an IUnknown from our registered list?
		if (dwReturnMask & SCRIPTINFO_IUNKNOWN) {
			for (i = 0; i < m_aExposedObject.GetSize(); i ++) {
				if (!m_aExposedObject[i]->GetObjectName().CompareNoCase(OLE2T(pstrName))) {
					*ppunkItem = m_aExposedObject[i]->GetObjectInterface();
					(*ppunkItem)->AddRef();
					bFound = TRUE;
					break;
				}
			}
		}
	}

	if (! bFound) {
		return TYPE_E_ELEMENTNOTFOUND;
	}

	return S_OK;
}

HRESULT CJfActiveScriptSite::GetDocVersionString(BSTR* pbstrVersion)
{
	return S_OK;
}

HRESULT CJfActiveScriptSite::OnScriptTerminate(const VARIANT* pvarResult, const EXCEPINFO* pexcepInfo)
{
	return S_OK;
}

HRESULT CJfActiveScriptSite::OnStateChange(SCRIPTSTATE ssScriptState)
{
	return S_OK;
}

//===============================================
// 出错处理，生成错误信息
//===============================================
HRESULT CJfActiveScriptSite::OnScriptError(IActiveScriptError* pScriptError)
{
	static BSTR		pwcErrorText;
	DWORD			contextCookie;
	ULONG			lineNumber;
	LONG			charPosition;
	CString			additional;
	CString			strMsg;

	pScriptError->GetSourceLineText(&pwcErrorText);
	pScriptError->GetSourcePosition(&contextCookie, &lineNumber, &charPosition);

	strMsg.Format("Script error in line %ld, position %ld", lineNumber+1, charPosition+1);

	// get the source line
	BSTR		bstrSourceLine;
	CString		strSourceLine;

	bstrSourceLine = (_bstr_t)strSourceLine;
	pScriptError->GetSourceLineText(&bstrSourceLine);
	// ASSERT(SUCCEEDED(hr));

	// get the exception info
	EXCEPINFO excepinfo;
	memset(&excepinfo, 0, sizeof(excepinfo));
	pScriptError->GetExceptionInfo(&excepinfo);
	
	if (bstrSourceLine != NULL) {
		strMsg += "\nSource line: ";
		strMsg += bstrSourceLine;
		::SysFreeString(bstrSourceLine);
	}

	if (excepinfo.bstrDescription != NULL) {
		strMsg += "\nDescription: ";
		strMsg += excepinfo.bstrDescription;
	}

	m_asErrorMessage.Add(strMsg);
	if (pwcErrorText)
	{
//		m_asErrorMessage.Add(CString(pwcErrorText));
		::SysFreeString(pwcErrorText);
	}

	m_bError = true;
	return S_OK;
}

HRESULT CJfActiveScriptSite::OnEnterScript(void)
{
	return S_OK;
}

HRESULT CJfActiveScriptSite::OnLeaveScript(void)
{
	return S_OK;
}

HRESULT CJfActiveScriptSite::GetWindow(HWND* phWnd)
{
	// First try to get our active window
	HWND activeWindow = NULL;
	CWnd* activeWnd = CWnd::GetActiveWindow();
	if (activeWnd) activeWindow = activeWnd->m_hWnd;

	// If active window results in nothing, try the application main window
	if (! activeWindow) {
		activeWnd = AfxGetMainWnd();
		if (activeWnd) {
			activeWindow = activeWnd->m_hWnd;
		}
	}

	// Store window handle
	m_hWnd = activeWindow;
	*phWnd = m_hWnd;
	return S_OK;
}

HRESULT CJfActiveScriptSite::EnableModeless(BOOL fEnable)
{
	return S_OK;
}

void CJfActiveScriptSite::DetachData()
{
	int		nIndex;

	// Empty exposed objects array
	for( nIndex = 0; nIndex < m_aExposedObject.GetSize(); nIndex++ )
		delete m_aExposedObject[nIndex];

	m_aExposedObject.RemoveAll();
}

//==============================================================
// 设置脚本引擎的类型，该函数只有在 InitializeScriptEngine 
// 之前执行才有效
//==============================================================
void CJfScriptEngineMgr::SetEngine(int nEngineType)
{
	m_nEngineType = nEngineType;
}

//==============================================================
// 调用一个简单的 Function , 参数都是字符串，返加值也是字符串
// 
// 例：
//	CJfScriptParams	Params;
//	CString			strReturn;
//
//	Params.AddItem( "mf" );
//	Params.AddItem( "clp" );
//	ScriptFactory.CallSimpleFun( "xx" , &Params , strReturn );
//==============================================================
BOOL CJfScriptEngineMgr::CallSimpleFun(CString strFunName , CJfScriptParams *pParam , CString &strReturn )
{
	DISPID			dispid;
	CString			result;

	if( GetIDOfFunction( strFunName , &dispid)) 
	{	
		BYTE  *parms  = pParam->NewParamTypes();
		DWORD *pDatas = pParam->NewParamDatas();

		va_list argList;
		argList = (char *)pDatas;
		InvokeHelperV(dispid, DISPATCH_METHOD, VT_BSTR, (void*)&result, (BYTE *)parms, argList);

		delete[] parms;
		delete[] pDatas;
	}
	strReturn = result;

	return true;
}

/*
bool CJfScriptEngineMgr::GetMethodNames( CStringArray &asData)
{
	bool bResult = false;
	if (m_pScript != NULL)
	{
		IScriptProcedureCollectionPtr pIProcedures = m_pScript->GetProcedures();

		// empty list...
		m_FunctionList.clear();

		try{
			long count = pIProcedures->GetCount();
			for(long index=1; index <= count; index++)
			{
				IScriptProcedurePtr pIProcPtr = pIProcedures->GetItem( _variant_t(index) );
				_bstr_t name = pIProcPtr->GetName();
				asData.AddString(LPCTSTR) name );
				pIProcPtr = NULL;
			}

			bResult = true;
		}
		catch(...)
		{
			// Just catch the exception, call GetErrorString()
			// to retreive last error
		}

		pIProcedures = NULL;
	}
	return bResult;
}*/