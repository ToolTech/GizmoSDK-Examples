// *****************************************************************************
//
// Copyright (C) SAAB AB
//
// All rights, including the copyright, to the computer program(s)
// herein belong to Saab AB. The program(s) may be used and/or
// copied only with the written permission of Saab AB, or in
// accordance with the terms and conditions stipulated in the
// agreement/contract under which the program(s) have been
// supplied.
//
//
// Information Class:	COMPANY UNCLASSIFIED
// Defence Secrecy:		NOT CLASSIFIED
// Export Control:		NOT EXPORT CONTROLLED
//
//
// File         : showWindow.cpp
// Module       : 
// Description  : Test program for creating an application with a gzWindow
// Author       : Lisa Johansson
// Product      : Gizmo3D 1.3 beta 7
//              
//
//                      
// NOTE:    Gizmo3D is a high performance 3D Scene Graph and effect visualisation 
//          C++ toolkit for Linux, Mac OS X, Windows (Win32) and IRIX® for  
//          usage in Game or VisSim development.
//
//
// Revision History...                                                  
//                                                                      
// Who  Date    Description                                             
//                                                                      
// LJH  041019  Created file    
//
// ******************************************************************************

#include "gzGizmo3DLibrary.h"


class ShowGzWindow : public gzApplication
{
public:

	// constructor
    ShowGzWindow();

	// destructor
    virtual ~ShowGzWindow();

	// onIdle method. This method is called when the application is running.
	void onIdle(); 

private:
	gzRefPointer<gzWindow> win;

};


ShowGzWindow::ShowGzWindow()
{
	// instantiating a gzWindow.
	win = new gzWindow("Window Test",(gzWindowHandle)NULL,(gzGraphicsFormat *)NULL,FALSE);

	// sets the size for the window.
	win->setSize(300, 300);

	// sets the position for the window on the screen.
	win->setPosition(100, 100);

	// Let the window be visible.
	win->show();
}

ShowGzWindow::~ShowGzWindow()
{
}

// this method is called when the application is running. 
// When the cpu don't have anything to do the onIdle is called.
gzVoid ShowGzWindow::onIdle()
{
	// refresh the contents in the window.
	win->refreshWindow();
}

int main(int argc, char *argv[])
{
	// Needed for some systems to install external graphics engines
    gzStartUpGizmo();   
    
    // Run your gizmo code. If anything goes wrong an exception will be thrown.
    try
    {     
		// Make the application
		ShowGzWindow win;

		// Start the application
		win.run();
			
    }
    catch(gzBaseError &error)       
    {
		// In case of exceptions thrown we want to print the message
        error.reportError();
    }
    
    gzShutDownGizmo();
    
    return 0;
}

