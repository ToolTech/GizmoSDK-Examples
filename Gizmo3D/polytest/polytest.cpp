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
// File			: polytest.cpp
// Module		: 
// Description	: Test app for rendering many polys
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
// AMO  030420  Created file    
//
// ******************************************************************************

#include "gzGizmo3DLibrary.h"  // include all Gizmo3D functionality

#include <ctype.h>

// Definition of a cube application
// The application provides an initialisation and an onIdle loop manager
// to do the refresh of the window

class PolyApp : public gzApplication
{
public:
		
	PolyApp();
    ~PolyApp();
	void Create(gzBool bOrthoCamera, gzBool bFullScreen, gzBool bTwoSideLighting);
	void onIdle();

private:
    gzNode *MakePolys();

	gzWindow    *m_win;         // the window   
	gzReal      m_angle;
    gzTransform *m_transform;

	int m_frame;
};


int main(int argc, char *argv[])
{
	gzStartUpGizmo();
	
	gzBool bOrthoCamera = FALSE;
	gzBool bFullScreen = FALSE;
	gzBool bTwoSideLighting = FALSE;

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
					case 'O':
						bOrthoCamera = TRUE;
						break;
					case '2':
						bTwoSideLighting = TRUE;
						break;
				}
			}
			argv++;
			argc--;
		}
	}

	// Make the application
        PolyApp app;
	try
	{       
		// Create the scene and the window
		app.Create(bOrthoCamera, bFullScreen, bTwoSideLighting);
		
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

PolyApp::PolyApp()
	: m_win(NULL), m_frame(0), m_angle(0)
{
}

PolyApp::~PolyApp()
{
	if(m_win)
		delete m_win;
}


void PolyApp::Create(gzBool bOrthoCamera, gzBool bFullScreen, gzBool bTwoSideLighting)
{

	// Set the appropriate graphics engine API
	gzGraphicsEngine::useEngine(GZ_ENGINE_OPENGL);

	// Create a scene to render
	gzScene *scene=new gzScene();

#ifdef GZ_DEBUG // Add axis in debug mode
	scene->addNode(new gzGeometryAxis(10,1));
#endif

	// Add the shape
        gzNode *shape = MakePolys();

        m_transform=new gzTransform;
        m_transform->addNode(shape);

        // Add Transform to Scene
        scene->addNode(m_transform);


	gzGraphicsFormat *format=NULL;
	if (bFullScreen)
	{
		// Setup fulllscreen format
		format = new gzGraphicsFormat;
		format->useFullScreen(TRUE);
		format->setFullScreenWidth(800);
	}


	//
	// Create our window
	//
	m_win=new gzWindow("Cube",NULL,this, format);

	if (!bFullScreen)
		m_win->setSize(600, 600);

	gzCamera *cam;

	if (bOrthoCamera)
	{
		gzOrthoCamera *orthoCam = new gzOrthoCamera;
		orthoCam->setWidth(4.0f);
		orthoCam->setHeight(4.0f);
		cam = orthoCam;
	}
	else
	{
		gzPerspCamera *perspCam = new gzPerspCamera;
		cam = perspCam;
	}


        cam->setPosition(0,0,20);

	cam->setScene(scene);

	m_win->setCamera(cam);
	m_win->refreshWindow();
}

void PolyApp::onIdle()
{
	m_angle+=(gzReal)0.2;
	if (m_angle > 360)
		m_angle = 0;


	// Rotate around Y axis in degrees
        m_transform->setHPR(m_angle,0,0);

        // And refresh window. Can be automated with setRefreshRate();
	m_win->refreshWindow();
	
	if (m_frame++>700)
	{
		printf("Frame Rate:%d\n",(int)m_win->getFrameRate());
		m_frame=0;
	}
}

gzNode *PolyApp::MakePolys()
{

        gzUInt32 numberOfTris=10000;

	gzTriGeometry *shape = new gzTriGeometry();

	shape->setSize(numberOfTris);
        
	for (gzUInt32 i = 0; i < numberOfTris; i++)
	{
                shape->setTri(i,gzVec3(gzRandom()*6-3,gzRandom()*6-3,gzRandom()*6-3),gzVec3(gzRandom()*6-3,gzRandom()*6-3,gzRandom()*6-3),gzVec3(gzRandom()*6-3,gzRandom()*6-3,gzRandom()*6-3));
                shape->setColor(i,gzVec4(gzRandom(),gzRandom(),gzRandom(),gzRandom()),gzVec4(gzRandom(),gzRandom(),gzRandom(),gzRandom()),gzVec4(gzRandom(),gzRandom(),gzRandom(),gzRandom()));
	}

        gzGeometryOptimizer::optimize(shape);

	shape->updateBoundary();        // update boundary for geometry

	return shape;
}
