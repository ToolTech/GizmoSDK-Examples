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
// File			: mirror.cpp
// Module		: 
// Description	: Test app for a mirror sample
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
// AMO  020901  Created file    
//
// ******************************************************************************

#include "gzGizmo3DLibrary.h"

#include <ctype.h>
#include <iostream>


class MyWindow : public gzWindow
{
public:
	MyWindow(gzString filename , gzGraphicsFormat *format):gzWindow("MyWindow",NULL,format)
	{
		// Create top node as an environment with lights
        gzEnvironment *group=new gzEnvironment;

        // Create a light and add it to environment
		rotlight=new gzLight;
		group->addLight(rotlight);

        // Create a node with a 3D model that has a size of 6x6x6 units at 10,0,10
		gzTransform *unit=new gzTransform;
		unit->addNode(gzDbManager::loadDB(filename,GZ_EVALUATE_EXTENSION));

		unit->unitScale();
		unit->scale(6,6,6);
		unit->translate(10,0,10);

		group->addNode(unit);

        // Now lets create a scene with two mirrors

		gzScene *scene=new gzScene;

		gzMirror *mirror=new gzMirror();
		gzMirror *mirror2=new gzMirror();

        // A mask to create mirror effect
		gzTriGeometry *tri=new gzTriGeometry;

		tri->setSize(1);
		tri->setTri(0,gzVec3(-5,-5,1),gzVec3(5,-5,1),gzVec3(0,5,1));
		tri->setColor(0,gzVec4(0,1,1,1));

        // A frame around the mirror
		gzTriGeometry *border=new gzTriGeometry;

		border->setSize(6);
        border->setColor(gzVec4(0.8f,0.7f,0.9f,1.0f));
		border->setTri(0,gzVec3(-6,-6,1),gzVec3(5,-5,1),gzVec3(-5,-5,1));
		border->setTri(1,gzVec3(-6,-6,1),gzVec3(6,-6,1),gzVec3(5,-5,1));
		border->setTri(2,gzVec3(6,-6,1),gzVec3(0,6,1),gzVec3(5,-5,1));
		border->setTri(3,gzVec3(5,-5,1),gzVec3(0,6,1),gzVec3(0,5,1));
		border->setTri(4,gzVec3(0,5,1),gzVec3(0,6,1),gzVec3(-5,-5,1));
		border->setTri(5,gzVec3(0,6,1),gzVec3(-6,-6,1),gzVec3(-5,-5,1));

        // A frame for mirror2 with another color
		gzTriGeometry *border2=(gzTriGeometry *)border->clone();
        border2->setColor(gzVec4(0.2f,0.2f,0.9f,1.0f));


        // We want the mirrors to clip objects behind the mirrors
        // So we do not get fake mirror images

		gzEnvironment *clipper=new gzEnvironment;
		gzEnvironment *clipper2=new gzEnvironment;

		gzClip *clip=new gzClip;
        clip->setClipPlane(gzVec3(0,0,-1),gzVec3(0,0,1.1f));

        // And lets se some mirror params
		mirror->setMaskGeometry(tri);
		mirror->setNormalBase(gzVec3(0,0,1),gzVec3(0,0,1));
		mirror->setMaxDepth(5);
		mirror->addNode(group);

		mirror2->setMaskGeometry(tri);
		mirror2->setNormalBase(gzVec3(0,0,1),gzVec3(0,0,1));
		mirror2->setMaxDepth(5);
		mirror2->addNode(group);

		clipper->addClipPlane(clip);
		clipper->addNode(mirror);
		clipper->addNode(border);
		clipper->addNode(new gzGeometryAxis(10,1));

		clipper2->addClipPlane(clip);
		clipper2->addNode(mirror2);
		clipper2->addNode(border2);


        // Locate the first mirror with a transform
		trans=new gzTransform();
		trans->addNode(clipper);

        // Locate the second mirror
        gzTransform *trans2=new gzTransform;

        trans2->setTranslation(20,0,0);
        trans2->setHPR(-50,0,0);
        trans2->addNode(clipper2);

        // Add mirrors and objects to scene
		scene->addNode(trans);
		scene->addNode(group);
        scene->addNode(trans2);

        // Add mirrors to each other
        mirror->addNode(trans2);
		mirror2->addNode(trans);

        // Lets print out the scene
		scene->debug(GZ_DEBUG_RECURSIVE);
				
		angle=0;

		gzCamera *cam=getCamera();

		cam->setScene(scene);

		cam->setPosition(5,0,50);
		cam->setHPR(0,0,0);
		cam->setFarClipPlane(5000);

		addInputInterface(new gzSimpleMouseViewControl(getCamera()));

	};


	/*
		The following code snippet is a virtual function to catch the window messages
		for pressed, repeated and released keys
	*/

	gzBool onKey(gzKeyValue key , gzKeyState keystate , gzInt32 mouse_x , gzInt32 mouse_y)
	{
		gzBool key_is_down;
		
		if( (keystate == GZ_KEY_STATE_PRESSED )|| (keystate ==GZ_KEY_STATE_REPEATED))
			key_is_down=TRUE;
		else
			key_is_down=FALSE;

		switch(key)
		{
			case 'f':

				if(keystate == GZ_KEY_STATE_PRESSED)
					std::cout<<"Framerate: "<<getFrameRate()<<std::endl;
				break;

			case 't':
				angle=(gzFloat)fmod(angle+1,360);
				trans->setHPR(angle,0,0);
				break;

			case 'r':
				angle=(gzFloat)fmod(angle-1,360);
				trans->setHPR(angle,0,0);
				break;


		}

		return gzWindow::onKey(key,keystate,mouse_x,mouse_y);
	}
	

	gzTransform *trans;
	gzFloat angle;
	gzLight	*rotlight;
};

// Definition of a sample application
// The application provides an initialisation and an onIdle loop manager
// to do the refresh of the window

class WindowApp : public gzApplication
{
public:
		
	WindowApp():m_win(NULL),m_format(NULL)
	{
		m_angle=0;
	};

    ~WindowApp()
	{
		if(m_win)
			delete m_win;
	}

        void Create(gzBool bFullScreen,gzString filename)
	{
		if(bFullScreen)
		{
			m_format = new gzGraphicsFormat;

			m_format->setFullScreenWidth(800);
			m_format->useFullScreen(TRUE);
            m_format->useStencil(TRUE);
		}
        else
		{
			m_format = new gzGraphicsFormat;
			m_format->useStencil(TRUE);
		}


		m_win = new MyWindow(filename ,m_format);
                m_win->setBackground(0,0.8f,0.5f,0);
	}

	void onIdle()
	{
		
		//	The following code snippet is used to control the movements of the camera

		if(m_win)
		{
			m_win->triggerKeyEvaluation();
			m_win->refreshWindow();

			m_angle=fmod(m_angle+2,360);
            m_win->rotlight->setPosition((gzFloat)(40*cos(m_angle/180.0*GZ_PI)),0,(gzFloat)(30*sin(m_angle/180.0*GZ_PI)));

		}
	}

private:
	
	MyWindow            *m_win;
    gzGraphicsFormat    *m_format;

	gzDouble			m_angle;
};

GZ_DECLARE_REFPTR(WindowApp);



int main(int argc, char *argv[])
{
	gzStartUpGizmo();	// Needed for some systems to install external graphics engines

	gzBool bFullScreen = FALSE;
	
    gzString filename="model.3ds";

    gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG);
	
	gzGraphicsEngine::useEngine(GZ_ENGINE_OPENGL);

	gzInitializeDbManagers();

	if(argc > 1)
	{
		argv++;
		argc--;
		while(argc > 0)
		{
			if(argv[0][0] == '-')
			{
				switch(toupper(argv[0][1]))
				{
					case 'F':
						bFullScreen = TRUE;
						break;

					case 'M':
						argv++;
						argc--;
						filename = argv[0];
						break;


				}

			}
			argv++;
			argc--;
		}
	}

	try
	{
		// Make the application
		WindowApp *app=new WindowApp;

		// Create the scene and the window
		app->Create(bFullScreen,filename);
		
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

