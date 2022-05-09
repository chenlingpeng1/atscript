// TestAts.h : main header file for the TESTATS application
//

#if !defined(AFX_TESTATS_H__52552A95_72BE_4730_B8B9_F65252594209__INCLUDED_)
#define AFX_TESTATS_H__52552A95_72BE_4730_B8B9_F65252594209__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestAtsApp:
// See TestAts.cpp for the implementation of this class
//

class CTestAtsApp : public CWinApp
{
public:
	CTestAtsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestAtsApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestAtsApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTATS_H__52552A95_72BE_4730_B8B9_F65252594209__INCLUDED_)
