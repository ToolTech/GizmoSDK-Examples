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
// File			: osxwin.cpp
// Module		: 
// Description	: Test app for OSX window
// Author		: Anders Modén          
// Product		: Gizmo3D 2.11.70
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
// AMO  040510  Created file    
//
// ******************************************************************************

#include "gzGizmo3DLibrary.h"

class MyMessageReceiver : public gzMessageReceiverInterface
{
public:

	MyMessageReceiver(ControlRef control)
	{
		text=HITextViewGetTXNObject( (HIViewRef) control);
		
		gzMessage::addMessageReceiver(this);
	}
	
	virtual ~MyMessageReceiver()
	{
		gzMessage::removeMessageReceiver(this);
	}

	virtual gzVoid onMessage(const gzString& sender , gzMessageLevel level , const char *message)
	{
		TXNOffset oStartOffset,oEndOffset;
		
		TXNGetSelection (text,&oStartOffset,&oEndOffset);
		
		gzString string(message);
		
		string=string+'\n';
		
		TXNSetData (text,kTXNTextData,string.getString(),string.length(),oStartOffset,oEndOffset);
	}
	
	TXNObject text;

};

class MyNodeClass : public gzNode
{
public:

	GZ_DECLARE_TYPE_INTERFACE;		// provide RTTI typed interface

	MyNodeClass(const gzString & name="MyNodeClass"):gzNode(name)
	{

	}

	virtual ~MyNodeClass()
	{
	}

	// ---------- Clone interface ---------------------------------------

	virtual gzReference* clone() const	// Provide a clone method for factories
	{
		return new MyNodeClass(*this);
	}

	// ---------- Action Interface --------------------------------------

	virtual gzVoid preTraverseAction( gzTraverseAction *actionclass , gzContext *context)
	{
		if(actionclass->isExactType(gzRenderAction::getClassType()))	// Exact a graphic action
		{
			gzBegin(GZ_QUADS);
			gzColor4d(1,1,1,1);
			gzVertex3d(0,0,0);
			gzColor4d(1,0,1,1);
			gzVertex3d(0,1,0);
			gzColor4d(1,1,0,1);
			gzVertex3d(1,1,0);
			gzColor4d(0,1,1,1);
			gzVertex3d(1,0,0);
			gzEnd();
		}
	}

	virtual gzVoid postTraverseAction( gzTraverseAction *actionclass , gzContext *context)
	{
		// Here we could add some cleanup if we were a group etc.
	}

	virtual gzActionStage useActionStage( gzTraverseAction *actionclass , gzContext *context) 
	{ 
		return GZ_ACTION_STAGE_SORTED_TEXTURE; 
	}

	virtual gzVoid updateBoundary()		// Note new code since Gizmo3D 1.1 Beta 24
	{
		gzVec3 center(0.5,0.5,0);		// Her is your own code to calculate the boundary sphere

		gzReal radius=2;

		resetBoundary();				// And set it
		includeBoundary(center,radius);	

		adjustBoundary();				// We must apply to this rule
	}
};

// Add some type info inheritance
GZ_DECLARE_TYPE_CHILD(gzNode,MyNodeClass,"MyNodeClassName");



class MyApplication : public gzApplication
{
public:
};

class MyWindow : public gzWindow
{
public:

	MyWindow(const gzString & name , gzWindowHandle parent ,gzApplication *app , gzGraphicsFormat *format, gzBool activateWindow):gzWindow(name,parent,app,format,activateWindow)
	{
	}
	
	gzVoid setUpQuad(gzVec3 viewPosition)
	{
		gzScene *scene=new gzScene;
	
		scene->addNode(new MyNodeClass("A test quad"));

        // Lets print out the scene
		scene->debug(GZ_DEBUG_RECURSIVE);
				
		gzCamera *cam=getCamera();

		cam->setScene(scene);

		cam->setPosition(viewPosition);
		cam->lookAt(0.5,0.5,0);
		
		setRefreshRate(10);

	}
	
	virtual ~MyWindow()
	{
	}

};

gzWindow * GZ_CALLBACK MyWindowCreator(const gzString & name , gzWindowHandle parent ,gzApplication *app , gzGraphicsFormat *format, gzBool activateWindow)
{
	return new MyWindow(name,parent,app,format,activateWindow);
}

int main(int argc, char* argv[])
{
	gzStartUpGizmo();
	
	OSStatus		err=noErr;
	
	try
	{
		MyApplication   app;
    
		gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG);
		
		gzGraphicsEngine::useEngine(GZ_ENGINE_OPENGL);
		
		gzWindow::setDefaultWindowCreator(MyWindowCreator);

		// ---------------------------------------------------------------------
		// OSX Standard Carbon App setup
		
		IBNibRef 		nibRef;
		WindowRef 		window;
		
		CFURLRef    bundleURL;
		CFBundleRef widgetBundle;


		bundleURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,CFSTR("."),kCFURLPOSIXPathStyle,TRUE);
		
		widgetBundle = CFBundleCreate (kCFAllocatorDefault, bundleURL);


		
		// Create a Nib reference passing the name of the nib file (without the .nib extension)
		// CreateNibReference only searches into the application bundle.
		err = CreateNibReferenceWithCFBundle(widgetBundle,CFSTR("main"), &nibRef);
		require_noerr( err, CantGetNibRef );
    
		// Once the nib reference is created, set the menu bar. "MainMenu" is the name of the menu bar
		// object. This name is set in InterfaceBuilder when the nib is created.
		err = SetMenuBarFromNib(nibRef, CFSTR("MenuBar"));
		require_noerr( err, CantSetMenuBar );
    
		// Then create a window. "MainWindow" is the name of the window object. This name is set in 
		// InterfaceBuilder when the nib is created.
		err = CreateWindowFromNib(nibRef, CFSTR("MainWindow"), &window);
		require_noerr( err, CantCreateWindow );

		// We don't need the nib reference anymore.
		DisposeNibReference(nibRef);
		
		CFRelease (bundleURL);
		CFRelease (widgetBundle);

    
		// The window was created hidden so show it.
		ShowWindow( window );
		
		// ----------------------------------------------------------------------
		
		ControlID id;
		ControlRef control;
		
		id.id=0;
		
		id.signature='text';
		
		GetControlByID(window,&id,&control);
		
		MyMessageReceiver listView(control);
		
		// ---- gzWindow management ---------------------------------------------
		
		MyWindow *mainWindow=(MyWindow *)gzWindow::getWindow("mainWindow");

		MyWindow *listWindow=(MyWindow *)gzWindow::getWindow("listWindow");
		
		if(mainWindow && listWindow)
		{
			mainWindow->setBackground(0.9,0.8,0.7);
			
			mainWindow->setUpQuad(gzVec3(0.5,0.5,10));
			
			listWindow->setUpQuad(gzVec3(10,10,20));
		}
		
				    
		// Call the event loop
		app.run();
	
	}
	catch (gzBaseError &error)
	{
		error.reportError();
	}

CantCreateWindow:
CantSetMenuBar:
CantGetNibRef:

	gzShutDownGizmo();
	
	return err;
}

