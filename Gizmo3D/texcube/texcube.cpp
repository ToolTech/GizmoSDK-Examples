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
// File			: texcube.cpp
// Module		: 
// Description	: Test app for textured cubes showing state management
// Author		: Anders Modén          
// Product		: Gizmo3D 2.11.48
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

// Definition of a cube application
// The application provides an initialisation and an onIdle loop manager
// to do the refresh of the window

class CubeApp : public gzApplication
{
public:
		
	CubeApp();
	~CubeApp();
	void Create(bool bFullScreen, bool bTwoSideLighting);
	void onIdle();

private:
	gzNode *MakeTexCube();

	gzWindow    *m_win;         // the window   
	gzReal      m_angle;
	gzTransform *m_cubeTransform;
};


int main(int argc, char *argv[])
{
	gzStartUpGizmo();	// Needed for some systems to install external graphics engines
	
	bool bFullScreen = false;
	bool bTwoSideLighting = false;

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
						bFullScreen = true;
						break;
					case '2':
						bTwoSideLighting = true;
						break;
				}
			}
			argv++;
			argc--;
		}
	}

	// Make the application
	CubeApp app;
	try
	{
		// Create the scene and the window
		app.Create(bFullScreen, bTwoSideLighting);

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

CubeApp::CubeApp()
	: m_win(NULL), m_angle(0)
{
}

CubeApp::~CubeApp()
{
	if(m_win)
		delete m_win;
}


void CubeApp::Create(bool bFullScreen, bool bTwoSideLighting)
{
	// Set the appropriate notification level
#ifdef GZ_DEBUG // Add axis in debug mode
        gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG);
#endif

	// Initialize all db managers ( the image managers as well )
	gzInitializeDbManagers();

	// Set the appropriate graphics engine API
	gzGraphicsEngine::useEngine(GZ_ENGINE_OPENGL);

	// Create a scene to render
	gzScene *scene=new gzScene();

#ifdef GZ_DEBUG // Add axis in debug mode
	scene->addNode(new gzGeometryAxis(10,1));
#endif

	// Add a point light to the scene
	gzEnvironment * env = new gzEnvironment();
	gzLight *pointlight = new gzLight();
	pointlight->setPosition(-10, 10, 10);
	env->addLight(pointlight);
    
	scene->addNode(env);


	// 
	// Floor
	//
	{
		gzTriGeometry *shape = new gzTriGeometry();
		shape->setSize(2);
		const gzReal dim = 3;
		const gzReal numtex = 3;
		shape->setTri(0, gzVec3(-dim,0,dim), gzVec3(dim,0,dim), gzVec3(dim,0,-dim));
		shape->setTexCord(0 , gzVec2(0,0) , gzVec2(numtex,0) , gzVec2(numtex,numtex));
		shape->setTri(1, gzVec3(dim,0,-dim), gzVec3(-dim,0,-dim), gzVec3(-dim,0,dim));
		shape->setTexCord(1 , gzVec2(numtex,numtex) , gzVec2(0,numtex) , gzVec2(0,0));
		
		// Load floor texture
		gzImage	*image = gzImageManager::loadImage("tex2.bmp", GZ_EVALUATE_EXTENSION);
		if (!image)
		{
			printf("Failed to load texture\n");
			exit(0);
		}

		printf("Load image %s, %d*%d\n", (const char *)image->getName(), 
			image->getWidth(), image->getHeight());
		
		gzTexture* tex = new gzTexture();
		tex->setImage(image);
		tex->setWrapS(GZ_TEXTURE_WRAP_REPEAT);
		tex->setWrapT(GZ_TEXTURE_WRAP_REPEAT);


		// Create texture environment
		gzTexEnv *texenv = new gzTexEnv;
		texenv->setMode(GZ_TEXENV_MODE_DECAL);


		gzState* state=new gzState;
		state->setMode(GZ_STATE_TEXTURE,GZ_STATE_ON);	// Enable texture
		state->setMode(GZ_STATE_TEXENV,GZ_STATE_ON);	// Enable texture environment
		state->setTexture(tex);							// Set texture
		state->setTexEnv(texenv);

		shape->setState(state);		// Set state on floor shape

		env->addNode(shape);
	}



	//
	// Texture Cube
	// 
	{
		// Create transparent red material
		gzMaterial *mat = new gzMaterial();
		mat->setAmbient(0.3f , 0.3f, 0.3f , 0.5f);
		mat->setSpecular(1.0f, 1.0f, 1.0f,1.0f);
		mat->setDiffuse(0.8f, 0.1f, 0.1f, 0.5f);
		mat->setEmission(0 , 0, 0, 0.5f);
		mat->setShininess(16);

		// Load cube texture
		gzImage	*image = gzImageManager::loadImage("tex.bmp", GZ_EVALUATE_EXTENSION);
		if (!image)
		{
			printf("Failed to load texture\n");
			exit(0);
		}

		printf("Load image %s, %d*%d\n", (const char *)image->getName(), 
			image->getWidth(), image->getHeight());
		
		gzTexture* tex = new gzTexture();
		tex->setImage(image);
		tex->setWrapS(GZ_TEXTURE_WRAP_CLAMP);
		tex->setWrapT(GZ_TEXTURE_WRAP_CLAMP);


		gzTexEnv *texenv = new gzTexEnv;
		texenv->setMode(GZ_TEXENV_MODE_MODULATE);


		gzState* state=new gzState;
		state->setMaterial(mat);
		state->setMode(GZ_STATE_MATERIAL,GZ_STATE_ON);	// Enable material
		state->setBlendFunction( GZ_BLEND_SRC_ALPHA, GZ_BLEND_ONE_MINUS_SRC_ALPHA);
		state->setMode(GZ_STATE_BLENDING,GZ_STATE_ON);	// Enable blending
		state->setMode(GZ_STATE_TEXTURE,GZ_STATE_ON);	// Enable texture
		state->setMode(GZ_STATE_TEXENV,GZ_STATE_ON);	// Enable texture environment
		state->setTexture(tex);		// Set texture
		state->setTexEnv(texenv);	// Set texture environment
                state->setSeparateFrontAndBackRender(TRUE);     // Do not let fron cull back side

		// Add shape
		gzNode *shape = MakeTexCube();

		m_cubeTransform=new gzTransform;
		m_cubeTransform->setState(state);
		m_cubeTransform->setTranslation(0,1,0);
		m_cubeTransform->addNode(shape);

		// Add transform to group
		env->addNode(m_cubeTransform);
	}

	//
	// Window
	//

	gzGraphicsFormat *format=NULL;
	if (bFullScreen)
	{
		// Setup fulllscreen format
		format = new gzGraphicsFormat;
		format->useFullScreen(true);
		format->setFullScreenWidth(800);
	}


	//
	// Create our window
	//
	m_win=new gzWindow("Cube",NULL,format);
	m_win->setBackground(0.0,0.5,0.5,1.0);

	if (bTwoSideLighting)
		gzEnvironment::setTwoSideLighting(m_win->getContext(),TRUE);

	gzEnvironment::setGlobalAmbientColor(m_win->getContext(),0.8f, 0.8f, 0.8f);

	gzCamera *cam = m_win->getCamera(); // use default perspective camera

	cam->setPosition(0,8,25);
	cam->lookAt(0,1,0);
	cam->setScene(scene);

	m_win->refreshWindow();
}

void CubeApp::onIdle()
{
	m_angle+=(gzReal)0.2;
	if (m_angle > 360)
		m_angle = 0;

	// Rotate cube transformations in degrees
	m_cubeTransform->setRotation(gzVec3(1.0f, 1.0f, 0.0f), m_angle);

	// And refresh window. Can be automated with setRefreshRate();
	m_win->refreshWindow();
}

gzNode *CubeApp::MakeTexCube()
{
	static gzVec3 cubeCoords[] = 
	{
		gzVec3(-1,-1,-1),gzVec3(1,-1,-1),gzVec3(1,1,-1),gzVec3(-1,1,-1),
		gzVec3(-1,-1,1),gzVec3(1,-1,1),gzVec3(1,1,1),gzVec3(-1,1,1)
	};

	static gzVec2 cubeTCoords[] = 
	{
		gzVec2(0.0f, 0.0f), gzVec2(1.0f, 0.0f), gzVec2(1.0f, 1.0f), gzVec2(0.0f, 1.0f)
	};

	static gzUInt32 indexList[] =
	{
		4,5,6,  // Z+
		6,7,4,  // Z+
		1,0,3,  // Z-
		3,2,1,  // Z-
		5,1,2,  // X+
		2,6,5,  // X+
		0,4,7,  // X-
		7,3,0,  // X-
		7,6,2,  // Y+
		2,3,7,  // Y+
		0,1,5,  // Y-
		5,4,0   // Y-
	};

	static gzUInt32 indexTList[] =
	{
		0,1,2,  // Z+
		2,3,0,  // Z+
		0,1,2,  // Z-
		2,3,0,  // Z-
		0,1,2,  // X+
		2,3,0,  // X+
		0,1,2,  // X-
		2,3,0,  // X-
		0,1,2,  // Y+
		2,3,0,  // Y+
		0,1,2,  // Y-
		2,3,0   // Y-
	};


	static gzUInt32 numberOfTris=sizeof(indexList)/3/sizeof(gzUInt32);

	gzTriGeometry *shape = new gzTriGeometry();
	shape->setSize(numberOfTris);

	// Create the cube from triangles. Normals are automatically
	// calculated. Each point has a corresponding normal.
	// This cube can be made in other manners but this is easy enough
	// Because of the large number of normals, it is not as fast as it
	// can be made.

	for (gzUInt32 i = 0; i < numberOfTris; i++)
	{
		shape->setTri(i,cubeCoords[indexList[i*3]],cubeCoords[indexList[i*3+1]],cubeCoords[indexList[i*3+2]]);
		shape->setTexCord(i , cubeTCoords[indexTList[i*3]] , cubeTCoords[indexTList[i*3+1]] , cubeTCoords[indexTList[i*3+2]]);
	}

	return shape;
}
