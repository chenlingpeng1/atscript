// TestAtsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestAts.h"
#include "TestAtsDlg.h"
#include "MyAtsObj.h"
#include "MyClass.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestAtsDlg dialog

CTestAtsDlg::CTestAtsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestAtsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestAtsDlg)
	m_strScript = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestAtsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestAtsDlg)
	DDX_Control(pDX, IDC_LS_FILE, m_ctlFile);
	DDX_Control(pDX, IDC_WEB, m_ctlWeb);
	DDX_Control(pDX, IDC_MAIL, m_ctlMail);
	DDX_Text(pDX, IDC_ED_SCRIPT, m_strScript);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestAtsDlg, CDialog)
	//{{AFX_MSG_MAP(CTestAtsDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BN_RUN, OnBnRun)
	ON_BN_CLICKED(IDC_BN_OPEN, OnBnOpen)
	ON_LBN_SELCHANGE(IDC_LS_FILE, OnSelchangeLsFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestAtsDlg message handlers

BOOL CTestAtsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	m_ctlMail.SetURL("mailto:Nextinstaller@gmail.com");

	// TODO: Add extra initialization here
	LoadAllScriptFile();
	m_strScript = "function void Main() \r\n{\r\n MessageBox( \"hello\" ); \r\n}";
	UpdateData(false);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestAtsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestAtsDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestAtsDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

#include "AtsEngine.h"
#include "MathObj.h"
void CTestAtsDlg::OnBnRun() 
{
	CAtsEngine		Eng;

	CString			strPrompt;
	
	UpdateData();
	Eng.m_strScriptPath = "E:\\VCPROGRAM\\InsAide\\TestAts\\bin";
	Eng.LoadScript( m_strScript );

	// Add Customize object to engine
	CMathObj * pMathObj = new CMathObj;
	Eng.AddObjIns( _T("Math") , pMathObj );

	// Add Customize object to engine
	CMyAtsObj * pNewObj = new CMyAtsObj;
	pNewObj->m_pEngine = &Eng;
	Eng.AddObjIns( "MyObj" , pNewObj );

	// Add Customize class to engine
	Eng.AddClassTemplate( new CMyClass );

	// Add Const Var
	Eng.SetConstVar("SS_INSTALL" , "1", ATSVAR_TYPE_INT );

	if( !Eng.RunSegment( "Main") )	
	{
		strPrompt.Format("Script is error, ErrorCode=%d,Line = %d,ErrorWord=%s" , Eng.m_nLastErrorCode , Eng.m_nErrorLine , Eng.m_strErrorWord );
		AfxMessageBox( strPrompt );
	}
}



void CTestAtsDlg::OnBnOpen() 
{
	CFileDialog		fileopen( TRUE );
	CString			strWorkDir;

	strWorkDir = GetModulePath();

	fileopen.m_ofn.lpstrDefExt		= "*.ats";
	fileopen.m_ofn.lpstrFilter		= "AtScript File(*.ats)\0*.ats";
	fileopen.m_ofn.Flags			=  OFN_ENABLEHOOK | OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST;
	fileopen.m_ofn.lpstrInitialDir  = strWorkDir;

	if ( fileopen.DoModal() != IDOK )
		return;
	
	m_strScript.Empty();
	ReadFromFile( fileopen.GetPathName() , m_strScript  );
	UpdateData( false );
	
}

CString CTestAtsDlg::GetModulePath()
{
	CString		strFileName;

	GetModuleFileName( NULL, strFileName.GetBuffer( MAX_PATH ), MAX_PATH );
	strFileName.ReleaseBuffer();

	return	GetPathName( strFileName );
}

CString CTestAtsDlg::GetPathName( CString strFileName )
{
	int			nPos;
	CString		strPathName;

	if ( strFileName.IsEmpty() )
		return	_T( "" );

	nPos	= strFileName.ReverseFind( '\\' );
	if ( nPos < 0 )
		return	_T( "" );

	strPathName	= strFileName.Left( nPos );
	return	strPathName;
}

void CTestAtsDlg::LoadAllScriptFile()
{
	CFileFind		filefind;
	CString			strTemp;
	BOOL			bReturn;
	CString			strFind;
	CString			strFileName;
	CString			strPath;
	int				nIndex = 0;

	strFind.Format( "%s\\*.ats" ,GetModulePath() ); 

	bReturn	= filefind.FindFile( strFind );
	while( bReturn  )
	{
		bReturn = filefind.FindNextFile();	
		if( filefind.IsDots() )
			continue;

		if( !filefind.IsDirectory() )
		{
			strTemp = filefind.GetFileName();
			m_ctlFile.AddString( strTemp );
		}
	}
	filefind.Close();
}

BOOL CTestAtsDlg::ReadFromFile( CString strFileName , CString &strData  )
{
	CFile		file;
	char		ch[2];
	int			nRead;

	if( !file.Open( strFileName , CFile::modeRead ) )
		return false;

	while( true )
	{
		nRead = file.Read(ch,1);
		ch[1] = '\0';
		if( nRead == 1 )
			strData = strData + ch;
		else
			break;
	}
	file.Close();
	return true;
}

void CTestAtsDlg::OnSelchangeLsFile() 
{
	CString		strFile;
	CString		strFileName;
	int			nSel;

	nSel = m_ctlFile.GetCurSel();
	m_ctlFile.GetText( nSel , strFileName );
	strFile.Format("%s\\%s" , GetModulePath(), strFileName );

	m_strScript.Empty();
	ReadFromFile( strFile , m_strScript  );
	UpdateData( false );
}
