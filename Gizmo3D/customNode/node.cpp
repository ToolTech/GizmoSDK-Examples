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
// File			: node.cpp
// Module		: 
// Description	: Test app for custom nodes
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
// AMO  020901  Created file    
//
// ******************************************************************************

#include "gzGizmo3DLibrary.h"

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

	// Gets called on node data updates
	virtual gzVoid updateNode()			// Note new code since Gizmo3D 1.1 Beta 24
	{
		gzVec3 center(0.5,0.5,0);		// Here is your own code to calculate the boundary sphere

		gzFloat radius=2;

		resetBoundary();				// And set it
		includeBoundary(center,radius);	

		adjustBoundary();				// We must apply to this rule
	}
};

// Add some type info inheritance
GZ_DECLARE_TYPE_CHILD(gzNode,MyNodeClass,"MyNodeClassName");


class MyWindow : public gzWindow
{
public:
	MyWindow():gzWindow("MyWindow")
	{

		gzScene *scene=new gzScene;

		scene->addNode(new MyNodeClass("A test quad"));

        // Lets print out the scene
		scene->debug(GZ_DEBUG_RECURSIVE);
				
		gzCamera *cam=getCamera();

		cam->setScene(scene);

		cam->setPosition(0.5,0.5,10);
		cam->lookAt(0.5,0.5,0);
	}


};

// Definition of a sample application

class WindowApp : public gzApplication
{
public:
		
	WindowApp():m_win(NULL)
	{
	};

    ~WindowApp()
	{
		if(m_win)
			delete m_win;
	}

	void Create()
	{
		m_win = new MyWindow();
        m_win->setBackground(0,0.8f,0.5f,0);
		m_win->refreshWindow();
	}


private:
	
	MyWindow            *m_win;
};



int main(int argc, char *argv[])
{
   	gzStartUpGizmo();	// Needed for some systems to install external graphics engines
	
	gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG);

	gzGraphicsEngine::useEngine(GZ_ENGINE_OPENGL);


	
	try
	{
		WindowApp app;

		// Create the app + window
		app.Create();

		// run the application
		app.run();

	}
	catch(gzBaseError &error)       // In case of exceptions thrown we want to print the message
	{
                error.reportError();
	}

	gzShutDownGizmo();

	return 0;
}

