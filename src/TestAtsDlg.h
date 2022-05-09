// TestAtsDlg.h : header file
//

#if !defined(AFX_TESTATSDLG_H__2450A7EE_297E_4AEE_872B_2B33F7832FE3__INCLUDED_)
#define AFX_TESTATSDLG_H__2450A7EE_297E_4AEE_872B_2B33F7832FE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "JfHyperLink.h"
/////////////////////////////////////////////////////////////////////////////
// CTestAtsDlg dialog

class CTestAtsDlg : public CDialog
{
// Construction
public:
	CTestAtsDlg(CWnd* pParent = NULL);	// standard constructor

	CString GetModulePath();
	CString GetPathName( CString strFileName );
	void LoadAllScriptFile();
	BOOL ReadFromFile( CString strFileName , CString &strData  );
// Dialog Data
	//{{AFX_DATA(CTestAtsDlg)
	enum { IDD = IDD_TESTATS_DIALOG };
	CListBox	m_ctlFile;
	CJfHyperLink	m_ctlWeb;
	CJfHyperLink	m_ctlMail;
	CString			m_strScript;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestAtsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestAtsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnRun();
	afx_msg void OnBnOpen();
	afx_msg void OnSelchangeLsFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTATSDLG_H__2450A7EE_297E_4AEE_872B_2B33F7832FE3__INCLUDED_)
