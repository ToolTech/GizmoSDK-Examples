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
// File			: multipass.cpp
// Module		: 
// Description	: Test app for multipass rendering
// Author		: Anders Modén          
// Product		: Gizmo3D 2.11.1
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
// AMO  030420  Created file    
//
// ******************************************************************************

#include "gzGizmo3DLibrary.h"
#include <ctype.h>


class MyWindow : public gzWindow
{
public:
	MyWindow():gzWindow("Multipass Texturing")
	{
		gzScene *scene=new gzScene;

		gzTriGeometry *geom=new gzTriGeometry;

		geom->setSize(2);

		geom->setTextureUnits(2);

		geom->setTri(0,gzVec3(-5,-5,1),gzVec3(5,-5,1),gzVec3(5,5,1));
		geom->setTri(1,gzVec3(-5,-5,1),gzVec3(5,5,1),gzVec3(-5,5,1));

		geom->setColor(gzVec4(0,1,1,1));

		geom->setTexCord(0,gzVec2(0,0),gzVec2(1,0),gzVec2(1,1),0);
		geom->setTexCord(1,gzVec2(0,0),gzVec2(1,1),gzVec2(0,1),0);

		geom->setTexCord(0,gzVec2(0,0),gzVec2(1,0),gzVec2(1,1),1);
		geom->setTexCord(1,gzVec2(0,0),gzVec2(1,1),gzVec2(0,1),1);


		gzState *multistate=new gzState;

		gzTexture *texA=new gzTexture;
		texA->setImage(gzImageManager::loadImage("gizmo.bmp",GZ_EVALUATE_EXTENSION));

		gzTexEnv *texenvA=new gzTexEnv;
		texenvA->setMode(GZ_TEXENV_MODE_DECAL);

		gzTexture *texB=new gzTexture;
		texB->setImage(gzImageManager::loadImage("light.bmp",GZ_EVALUATE_EXTENSION));

		gzTexEnv *texenvB=new gzTexEnv;
		texenvB->setMode(GZ_TEXENV_MODE_MODULATE);


		multistate->setNumberOfTextures(2);
		multistate->setNumberOfTexEnv(2);

		multistate->setTexture(texA);
		multistate->setMode(GZ_STATE_TEXTURE,GZ_STATE_ON);

		multistate->setTexEnv(texenvA);
		multistate->setMode(GZ_STATE_TEXENV,GZ_STATE_ON);

		multistate->setTexture(texB,1);
		multistate->setMode(GZ_STATE_TEXTURE,GZ_STATE_ON);

		multistate->setTexEnv(texenvB,1);
		multistate->setMode(GZ_STATE_TEXENV,GZ_STATE_ON);

		geom->setState(multistate);

		scene->addNode(geom);
				
		gzCamera *cam=getCamera();

		cam->setScene(scene);

		cam->setPosition(0,0,30);
		cam->lookAt(0,0,0);
	};

};

// Definition of a sample application
// The application provides an initialisation and an onIdle loop manager
// to do the refresh of the window

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
	
	MyWindow			*m_win;
};



int main(int argc, char *argv[])
{
    gzStartUpGizmo();	// Needed for some systems to install external graphics engines

	gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG);

	gzGraphicsEngine::useEngine(GZ_ENGINE_OPENGL);

	gzInitializeDbManagers();

	// Make the application
	WindowApp app;

	try
	{
		// Create the scene and the window
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



