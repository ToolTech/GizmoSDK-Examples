// MFCView.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MFCView.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "MFCViewDoc.h"
#include "MFCMainView.h"

#include "GizmoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCViewApp

BEGIN_MESSAGE_MAP(CMFCViewApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CMFCViewApp::OnAppAbout)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CMFCViewApp construction

CMFCViewApp::CMFCViewApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CMFCViewApp object

CMFCViewApp theApp;


// CMFCViewApp initialization

BOOL CMFCViewApp::InitInstance()
{
	CWinApp::InitInstance();

	CGizmoControl::initialize();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_MFCVIEWTYPE,
		RUNTIME_CLASS(CMFCViewDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CMFCMainView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

    // Don't display a new MDI child window during startup
    if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
        cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CMFCViewApp::ExitInstance()
{
	CGizmoControl::uninitialize();

	return CWinApp::ExitInstance();
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CMFCViewApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CMFCViewApp message handlers

