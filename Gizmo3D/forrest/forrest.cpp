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
// File			: forrest.cpp
// Module		: 
// Description	: Test app for crossboards. Shows how to make alpha masked trees etc.
// Author		: Anders Modén          
// Product		: Gizmo3D 2.10.9
//              
//
//                      
// NOTE:	Gizmo3D is a high performance 3D Scene Graph and effect visualisation 
//			C++ toolkit for Linux, Mac OS X, Windows (Win32) and IRIX® for  
//			usage in Game or VisSim development.
//
//
// Revision History...                                                  
//                                                                      
// Who  Date    Description                                             
//                                                                      
// AMO  080505  Created file 
//
// ******************************************************************************


#include "gzGizmo3DLibrary.h"

// Definition of an application
// The application provides an initialisation and an onIdle loop manager
// to do the refresh of the window. 
class ForrestApp : public gzApplication
{
public:
	
	ForrestApp();
	~ForrestApp();
	void Create(gzBool bFullScreen);
    virtual void onTick();
	
private:
	// A smart pointer is used for the window. 
	// You don't have to delete this in the destructor, Gizmo takes care of all smart pointers.
	gzRefPointer<gzWindow>   m_win;         
    gzDouble        m_angle;        
};

GZ_DECLARE_REFPTR(ForrestApp);


int main(int argc, char *argv[])
{
	// Needed for some systems to install external graphics engines
	gzStartUpGizmo();	
	
	gzArgumentParser args(argc,argv);
	
	gzBool bFullScreen = args.hasOption("f") | args.hasOption("fullscreen");
	
	// Make the application
	ForrestAppPtr app=new ForrestApp;

	try
	{
		// Create the scene and the window
		app->Create(bFullScreen);
		
		// run the application
		app->run();
		
	}
	catch(gzBaseError &error)       // In case of exceptions thrown we want to print the message
	{
		error.reportError();
	}
	
	gzShutDownGizmo();
	
	return 0;
}

ForrestApp::ForrestApp()
{
}

ForrestApp::~ForrestApp()
{
}


void ForrestApp::Create(gzBool bFullScreen)
{
	// Set the appropriate notification level
#ifdef GZ_DEBUG 
	// Set massage level debug in debug mode. All messages with debug mode or higher will be shown.
	gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG);
#endif
	
	// Initialize all db and image managers. This makes it possible to load models and images with the supported formats.
	gzInitializeDbManagers();
	
	// Set the appropriate graphics engine API
	gzGraphicsEngine::useEngine(GZ_ENGINE_OPENGL);
	
	// Create a scene to render
	gzScene *scene=new gzScene();
	
	//------------------------------------------------------------------------------
	// Create trees
	//------------------------------------------------------------------------------
	
	
	
	//------------------------------------------------------------------------------
	// Graphics Format
	//------------------------------------------------------------------------------
	// The graphics format is used if you need so
	gzGraphicsFormat *format=NULL;
	if (bFullScreen)
	{
		// Setup fulllscreen format
		format = new gzGraphicsFormat;
		format->useFullScreen(TRUE);
	}
	
	
	//------------------------------------------------------------------------------
	// Create our window
	//------------------------------------------------------------------------------
	//
	m_win=new gzWindow("Forrest",NULL, format);
	// The background color to the window is specified
	m_win->setBackground(0.3f, 0.3f, 1.0f, 1.0f);
	
	// The window has a default perspective camera which is used. The scene is connected with this camera.
	m_win->getCamera()->setScene(scene);
	// Specify the tick interval. When the interval time has elapsed the methoc onTick() is called.
    setTickInterval(0.01);
}


void ForrestApp::onTick()
{
	m_win->refreshWindow();
}
