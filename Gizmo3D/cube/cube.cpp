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
// File			: cube.cpp
// Module		: 
// Description	: Test app for a sample cube
// Author		: Anders Modén          
// Product		: Gizmo3D 2.11.76
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

// Definition of a cube application
// The application provides an initialisation and an onIdle loop manager
// to do the refresh of the window

class CubeApp : public gzApplication
{
public:
		
	CubeApp();
	~CubeApp();
	void Create(gzBool bOrthoCamera, gzBool bFullScreen, gzBool bTwoSideLighting);
	void onIdle();

private:
	gzNode *MakeCube();
	
	gzWindow    *m_win;         // the window   
	gzFloat      m_angle;
	gzTransform *m_redTransform;
	gzTransform *m_greenTransform;
	int m_frame;
};

GZ_DECLARE_REFPTR(CubeApp);


int main(int argc, char *argv[])
{
	gzStartUpGizmo();	// Needed for some systems to install external graphics engines
	
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
	CubeAppPtr app=new CubeApp;

	try
	{       
		// Create the scene and the window
		app->Create(bOrthoCamera, bFullScreen, bTwoSideLighting);
		
		// run the application
		app->run();

	}
	catch(gzBaseError &error)       // In case of exceptions thrown we want to print the message
	{
                error.reportError();
	}

	app=NULL;
	
	gzShutDownGizmo();
	
	return 0;
}

CubeApp::CubeApp()
	: m_win(NULL), m_angle(0), m_frame(0)
{
}

CubeApp::~CubeApp()
{
	if(m_win)
		delete m_win;
}


void CubeApp::Create(gzBool bOrthoCamera, gzBool bFullScreen, gzBool bTwoSideLighting)
{

	// Set the appropriate graphics engine API
	gzGraphicsEngine::useEngine(GZ_ENGINE_OPENGL);

	// Create a scene to render
	gzScene *scene=new gzScene();

#ifdef GZ_DEBUG // Add axis in debug mode
	scene->addNode(new gzGeometryAxis(10,1));
#endif

	// Create an environment. The environment makes it possible to add lights and fog to the scene.
	gzEnvironment * env = new gzEnvironment();
	// Create a point light. 
	gzLight *pointlight = new gzLight();
	// Set the position for the light
	pointlight->setPosition(0, 0, 0);
	// Add the light to the environment. The light will lit every object which is a child to the scene
	env->addLight(pointlight);

	// Add the environment to the scene
	scene->addNode(env);

	// Create a state                      
	gzState *state=new gzState;
	// Enable polygon mode. This makes it possible to change settings for the polygons
	state->setMode(GZ_STATE_POLYGON_MODE,GZ_STATE_ON);
	// Sets the mode for the back of each polygon. The polygons are filled on the back
	state->setBackPolygonMode(GZ_POLYGON_MODE_FILL);
	// Set the state to the environment. This state will be applied to env and to all nodes which 
	// are children to env.  
	env->setState(state);

	//-----------------------------------------------------------------------------
	// RED CUBE
	//----------------------------------------------------------------------------- 

	// Create a red material with a bit of transparency
	gzMaterial *redMaterial = new gzMaterial();
	// Set the ambient reflection property for the material. 
	redMaterial->setAmbient(0.16f , 0.16f, 0.16f , 0.5f);
	// Set the specular reflection property
	redMaterial->setSpecular(1.0f, 1.0f, 1.0f,1.0f);
	// Specifies how the diffuse light is reflected
	redMaterial->setDiffuse(0.8f, 0.1f, 0.1f, 0.5f);
	// Sets the color of the light which is emitted from the material
	redMaterial->setEmission(0 , 0, 0, 0.5f);
	// Set the shininess, which specifies the size and brightness for the specular reflection.
	redMaterial->setShininess(16);

	// create a state which uses the red material
	gzState* redState=new gzState;
	// set the material the state shall use
	redState->setMaterial(redMaterial);
	// enable material mode for the state. If the material shall be visible a light has to exist in the scene.
	redState->setMode(GZ_STATE_MATERIAL,GZ_STATE_ON);
	// specify the blending function. 
	redState->setBlendFunction( GZ_BLEND_SRC_ALPHA, GZ_BLEND_ONE_MINUS_SRC_ALPHA);
	// enable blending mode
	redState->setMode(GZ_STATE_BLENDING,GZ_STATE_ON);
	redState->setSeparateFrontAndBackRender(TRUE);

	// Create the cube.
	gzNode *cubeShape = MakeCube();

	// Create a transform 
	m_redTransform=new gzTransform;
	// translate -5 in the z direction
	m_redTransform->setTranslation(0.0f, 0.0f, -5.0f);
	// set the red state to the transform. The state will be applied to all children to the transform
	m_redTransform->setState(redState);
	// Add the cube as child to the transform
	m_redTransform->addNode(cubeShape);
	// Add transform to the environment. 
	env->addNode(m_redTransform);

	//----------------------------------------------------------------------------- 
	// GREEN CUBE
	//----------------------------------------------------------------------------- 

	// Create a green material
	gzMaterial *greenMaterial = new gzMaterial();
	// set ambient reflection
	greenMaterial->setAmbient(0.16f , 0.16f, 0.16f , 0.5f);
	// set diffuse reflection
	greenMaterial->setDiffuse(0.1f, 0.8f, 0.1f, 0.5f);
	// set color to light emitted from the material. 
	greenMaterial->setEmission(0 , 0, 0, 0.5f);
	// Set the shininess, which specifies the size and brightness for the specular reflection.
	greenMaterial->setShininess(16);

	// Create a new state 
	gzState* greenState=new gzState;
	// The state uses the green material
	greenState->setMaterial(greenMaterial);
	// enable material mode for the state
	greenState->setMode(GZ_STATE_MATERIAL,GZ_STATE_ON);

	// create a transform
	m_greenTransform=new gzTransform;
	// the transform uses the green state
	m_greenTransform->setState(greenState);
	// Translate 
	m_greenTransform->setTranslation(0.5f, 0.1f, -6.0f);
	// add the same node as child to this transform. 
	// We now have one geometry which is child to two different transforms
	m_greenTransform->addNode(cubeShape);
	// Add the transform as a child to the environment
	env->addNode(m_greenTransform);

	gzGraphicsFormat *format=NULL;
	if (bFullScreen)
	{
		// Setup fulllscreen format
		format = new gzGraphicsFormat;
		format->useFullScreen(TRUE);
		format->setFullScreenWidth(800);
	}


	//----------------------------------------------------------------------------- 
	// Create our window
	//----------------------------------------------------------------------------- 
	m_win=new gzWindow("Cube",NULL, format);

	// Default size
	if (!bFullScreen)
		m_win->setSize(600, 600);

	if (bTwoSideLighting)
       gzEnvironment::setTwoSideLighting(m_win->getContext(),TRUE);

	// create a camera
	gzCamera *cam;

	if (bOrthoCamera)
	{
		// A orthogonal camera is created
		gzOrthoCamera *orthoCam = new gzOrthoCamera;
		// Width and height is set to 4
		orthoCam->setWidth(4.0f);
		orthoCam->setHeight(4.0f);
		cam = orthoCam;
	}
	else
	{
		// Default a perspective camera is created
		gzPerspCamera *perspCam = new gzPerspCamera;
		cam = perspCam;
	}

	// Set position for the camera. the default viewing direction is along the negativ z-axis
	cam->setPosition(0,0,8);
	// Set the scene to the camera. The camera will observe the scene
	cam->setScene(scene);
	// Connect the camera to the window. The part of the scene visible to the camera will be drawn in the window
	m_win->setCamera(cam);
	// Refresh the window. An image is captured with the camera and then drawn in the window
	m_win->refreshWindow();
}

// When the application has started with run() the onIdle() method is called 
void CubeApp::onIdle()
{
	// Increase the angle value
	m_angle+=(gzFloat)0.2;
	// If the angle is larger than 360 it is reset to 0
	if (m_angle > 360)
		m_angle = 0;

	// Rotate around the specified axis in degrees. The m_angle decides the size of rotation
	m_redTransform->setRotation(gzVec3(1.0f, 1.0f, 0.0f), m_angle);
	m_greenTransform->setRotation(gzVec3(0.5f, 0.1f, 1.0f), m_angle);

	// And refresh window. Can be automated with setRefreshRate();
	m_win->refreshWindow();
	
	if (m_frame++>700)
	{
		// The framerate is printed in the console each 700th frame
		GZMESSAGE(GZ_MESSAGE_NOTICE,"Frame Rate:%d",(int)m_win->getFrameRate());
		m_frame=0;
	}
}

// The method that creates the cube
gzNode *CubeApp::MakeCube()
{
	// Create an array with gzVec3 which is the points in the cube
	static gzVec3 cubeCoords[] = 
	{
		gzVec3(-1,-1,-1),gzVec3(1,-1,-1),gzVec3(1,1,-1),gzVec3(-1,1,-1),
		gzVec3(-1,-1,1),gzVec3(1,-1,1),gzVec3(1,1,1),gzVec3(-1,1,1)
	};

	// Create a list with indexes. This indexes indicates the position in the array with cubeCoords
	// for the point that shall be used
	static gzUInt32 indexList[] =
	{
		1,0,3,  // Z-
		3,2,1,  // Z-
		4,5,6,  // Z+
		6,7,4,  // Z+
		5,1,2,  // X+
		2,6,5,  // X+
		0,4,7,  // X-
		7,3,0,  // X-
		7,6,2,  // Y+
		2,3,7,  // Y+
		0,1,5,  // Y-
		5,4,0   // Y-
	};

	// Compute the number of triangles in the cube
	static gzUInt32 numberOfTris=sizeof(indexList)/3/sizeof(gzUInt32);

	// Create a triangle geometry
	gzTriGeometry *shape = new gzTriGeometry();
	// Define the number of triangles used in the geometry
	shape->setSize(numberOfTris);

	// Create the cube from triangles. Normals are automatically
	// calculated. Each point has a corresponding normal.
	// This cube can be made in other manners but this is easy enough
	// Because of the large number of normals, it is not as fast as it
	// can be made.

	for (gzUInt32 i = 0; i < numberOfTris; i++)
	{
		// Add a triangle to the shape using the coordinate array and determine position with the indexlist.
		shape->setTri(i,cubeCoords[indexList[i*3]],cubeCoords[indexList[i*3+1]],cubeCoords[indexList[i*3+2]]);
	}
	// Optimize the shape
    gzGeometryOptimizer::optimize(shape);

	// return the gzTriGeometry as a gzNode. 
	return shape;
}
