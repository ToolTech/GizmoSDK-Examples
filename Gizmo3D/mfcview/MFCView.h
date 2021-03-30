// MFCView.h : main header file for the MFCView application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMFCViewApp:
// See MFCView.cpp for the implementation of this class
//

class CMFCViewApp : public CWinApp
{
public:
	CMFCViewApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFCViewApp theApp;
