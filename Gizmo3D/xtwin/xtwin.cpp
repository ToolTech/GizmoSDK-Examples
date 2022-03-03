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
// File			: xtwin.cpp
// Module		: 
// Description	: Test app to show how to interface xt
// Author		: Anders Modén		
// Product		: Gizmo3D 2.11.70
//		
//
//			
// NOTE:	The GIZMO package defines a general purpose API for large model
//			visualisation and advanced graphic behaviours. The package is similar
//			to Cosmo3D, Inventor, Performer etc. but adds automated behaviour
//			patterns to the graphics. 
//
//
// Revision History...							
//									
// Who	Date	Description						
//									
// AMO	990611	Created file 	
//
// ******************************************************************************

#include "gzGizmo3DLibrary.h"  // include all Gizmo3D functionality

#include "X11/X.h"				// whatever you use in your xt app
#include "X11/Xlib.h"
#include <X11/StringDefs.h>
#include "X11/Intrinsic.h"
#include <X11/Shell.h>
#include "X11/Xaw/Form.h"


int main(int argc , char *argv[] )
{
	try
	{
		// Just a simple example to show xt init and some xt windows

		// XtAppContext app; It is possible to use normal xt app context

		gzApplication app;

		Widget toplevel = XtVaAppCreateShell("xtwin","GIZMO test",applicationShellWidgetClass,app.getAppContext().display,
										XtNwidth,300,
										XtNheight,300,
										NULL);

		Widget form = XtVaCreateManagedWidget(	"form",             // No fancy stuff. Just facts !!!!
												formWidgetClass, 
												toplevel, 
												XtNwidth, 604, 
                                        		XtNheight, 413,
												(XtPointer) NULL);
		
		
	    Widget mother = XtVaCreateManagedWidget ( "mother",
                                          		formWidgetClass,
                                          		form, 
												XtNwidth, 400, 
                                        		XtNheight, 200,
												XtNvertDistance,100,
												XtNhorizDistance,100,
												XtNborderWidth,0,	
                                          		(XtPointer) NULL ); 
		

		gzGraphicsEngine::useEngine(GZ_ENGINE_OPENGL);					
		gzInitializeDbManagers();


		// Create a scene to render
		gzScene *scene=new gzScene("3ds Scene");

		// Create some light environment group
		gzEnvironment *group=new gzEnvironment;
			
		gzKeyDatabase::setUserKey("GIZMO_PATH","../dbviewer/");

		gzNode *node=gzDbManager::loadDB("gizmo.3ds",GZ_EVALUATE_EXTENSION);

		gzTransform *scale=new gzTransform;

		scale->addNode(node);
		scale->unitScale();
		scale->scale(30,30,30);
		
		gzFrame *frame = new gzFrame;
		frame->setFrameDuration(1/40.0);
		frame->setNumberOfFrames(200);
		frame->selectChildPerFrame(TRUE);
	

		for(int i=0;i<200;i++)
		{
			gzTransform *transform=new gzTransform;
			transform->setHPR(360.0/200.0*i,0,0);
			transform->addNode(scale);
			frame->addNode(transform);
		}

		group->addNode(frame);
		scene->addNode(group);

		frame->startFrameCounter();

		// Add some light to environment
		gzLight *light=new gzLight;
		group->addLight(light);

		light->setSpecularColor(0,0,0);	// No shiny white
		light->setPosition(20,20,20);

		gzWindow *win=new gzWindow("XtWin",mother);		
		win->setBackground(0.0,0.6,0.6,1.0);
		win->getCamera()->setScene(scene);
		win->getCamera()->setPosition(100,100,100);
		win->getCamera()->lookAt(0,0,0);
		win->setRefreshRate(50);

		// The window created is a fully featured GIZMO window with scene graph capabilities etc.
		// Perhaps a bit boring because nothing is drawn, but it show how to connect GIZMO into your
		// Xt or Motif widget application


		XtRealizeWidget(toplevel);

		//We want to make a nice WM_QUIT
		XSetWMProtocols(app.getAppContext().display,XtWindow(toplevel),app.getWMProtocol(),app.getWMProtocolLength());

	    app.run();
	}
	catch(gzBaseError &error)	// In case of exceptions thrown we want to print the message
	{
		error.reportError();
	}

	return 0;
}
