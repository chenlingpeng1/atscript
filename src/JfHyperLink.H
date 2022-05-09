// HyperLink.h : header file
////////////////////////////////////////////////////////////////////
//
// DDX_Control(pDX, IDC_HYPERLINK5, m_hyperlink5);	
//
// m_hyperlink5.SetURL("mailto:giancarlo@saria.com");
// m_hyperlink5.ModifyLinkStyle(0, CHyperLink::StyleDownClick);	
//
//////////////////////////////////////////////////////////////////////
//
// HyperLink static control.
//
// Copyright Giancarlo Iovino, 1997 (giancarlo@saria.com)
// This code is based on CJfHyperLink by Chris Maunder.
// Feel free to use and distribute. May not be sold for profit. 

#if !defined(AFX_JFHYPERLINK_H_04ET323B01_023500_0204251998_ENm_INCLUDED_)
#define AFX_JFHYPERLINK_H_04ET323B01_023500_0204251998_ENm_INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define		MSG_JFHYPERLINK_CLICK		(WM_USER + 742)
/////////////////////////////////////////////////////////////////////////////
// CJfHyperLink window

class CJfHyperLink : public CStatic
{
	DECLARE_DYNAMIC(CJfHyperLink)

public:
// Link styles
	static const DWORD StyleUnderline;
	static const DWORD StyleUseHover;
	static const DWORD StyleAutoSize;
	static const DWORD StyleDownClick;
	static const DWORD StyleGetFocusOnClick;
	static const DWORD StyleNoHandCursor;
	static const DWORD StyleNoActiveColor;

// Construction/destruction
	CJfHyperLink();
	virtual ~CJfHyperLink();

// Attributes
public:

// Operations
public:	
	HCURSOR			GetLinkCursor();
	void			SetLinkCursor(HCURSOR hCursor);
    
	void			SetLinkColor(COLORREF crLinkColor);
	void			SetActiveColor(COLORREF crActiveColor);
	void			SetVisitedColor(COLORREF crVisitedColor);
	void			SetHoverColor(COLORREF crHoverColor);
    void			SetColors(COLORREF crLinkColor, COLORREF crActiveColor, 
					COLORREF crVisitedColor, COLORREF crHoverColor = -1);

	void			SetURL(CString strURL);
    CString			GetURL() const;

	DWORD			GetLinkStyle() const;
	BOOL			ModifyLinkStyle(DWORD dwRemove, DWORD dwAdd, BOOL bApply=TRUE);
	BOOL			SetLinkStyle(DWORD dwStyle,BOOL bApply  =TRUE );
    
	void			SetWindowText(LPCTSTR lpszText);
	void			SetFont(CFont *pFont);
	
	BOOL			IsVisited() const;
	void			SetVisited(BOOL bVisited = TRUE);
	
	// Use this if you want to subclass and also set different URL
	BOOL			SubclassDlgItem(UINT nID, CWnd* pParent, LPCTSTR lpszURL=NULL)
	{
		m_strURL = lpszURL;
		return CStatic::SubclassDlgItem(nID, pParent);
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJfHyperLink)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);	
	protected:
	virtual void PreSubclassWindow();	
	//}}AFX_VIRTUAL

// Implementation
protected:
	void		SetDefaultCursor();
	LONG		GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata);
	void		ReportError(int nError);	
	HINSTANCE	GotoURL(LPCTSTR url, int showcmd);

	void		AdjustWindow();	
	inline void SwitchUnderline();
	
public:
	void		FollowLink();

// Protected attributes
protected:
	COLORREF m_crLinkColor;		// Link normal color
	COLORREF m_crActiveColor;	// Link active color
	COLORREF m_crVisitedColor;	// Link visited color
	COLORREF m_crHoverColor;		// Hover color
	HCURSOR  m_hLinkCursor;		// Hyperlink mouse cursor

	BOOL	 m_bLinkActive;				// Is the link active?
	BOOL     m_bOverControl;			// Is cursor over control?
	BOOL	 m_bVisited;				// Has link been visited?
	DWORD	 m_dwStyle;					// Link styles
	CString  m_strURL;					// Hyperlink URL string
	CFont    m_Font;					// Underlined font (if required)	
	CToolTipCtrl m_ToolTip;				// The link tooltip	
public:
	CWnd	*m_pOwner;
	// Generated message map functions
protected:
	//{{AFX_MSG(CJfHyperLink)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line

#endif // !defined(AFX_JFHYPERLINK_H_04ET323B01_023500_0204251998_ENm_INCLUDED_)
