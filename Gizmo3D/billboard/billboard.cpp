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
// File			: billboard.cpp
// Module		: 
// Description	: Test app for billboards. Shows how to make alpha masked trees etc.
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
// LJH	041012	Added comments   
//
// ******************************************************************************


#include "gzGizmo3DLibrary.h"
#include <ctype.h>

// Used to convert radians to degrees
#define RAD2DEG(a) ((gzFloat)((a)*180.0/GZ_PI))

// Specify the number of trees that shall be created
const int numTrees = 10000;
// Specify the size of one quad in the ground
const int groundQuadSize = 20;
// Specify the number of quads used to create the ground
const int numGroundQuads = 30;
// Compute the total width for the ground
const int groundWidth = groundQuadSize*numGroundQuads;
// The radius of the circle which specify the camera position
const int cameraRadius = (int)(groundWidth/2.5);




//compute a random position which is used to place the trees on the ground
inline gzFloat RNDPos() {return (gzFloat)(groundWidth*gzRandom() - groundWidth/2.0f); }






// Definition of an application
// The application provides an initialisation and an onIdle loop manager
// to do the refresh of the window. 
class BillboardApp : public gzApplication
{
public:
	
	BillboardApp();
	~BillboardApp();
	void Create(gzBool bFullScreen);
    virtual void onTick();
	
private:
	// A smart pointer is used for the window. 
	// You don't have to delete this in the destructor, Gizmo takes care of all smart pointers.
	gzRefPointer<gzWindow>   m_win;         
    gzDouble        m_angle;        
};


int main(int argc, char *argv[])
{
	// Needed for some systems to install external graphics engines
	gzStartUpGizmo();	
	
	gzArgumentParser args(argc,argv);
	
	gzBool bFullScreen = args.hasOption("f") | args.hasOption("fullscreen");
	
	// Make the application
	BillboardApp *app=new BillboardApp;

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

	delete app;
	
	gzShutDownGizmo();
	
	return 0;
}

BillboardApp::BillboardApp()
: m_angle(0.0)
{
}

BillboardApp::~BillboardApp()
{
}


void BillboardApp::Create(gzBool bFullScreen)
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
	{
		// Quad plan
		gzTriGeometry *shape = new gzTriGeometry();
		//specify the number of triangles in the gzTriGeometry
		shape->setSize(2);
		//add a triangle to the geometry. The index (0) and the three points for the triangle is specified.
		shape->setTri(0, gzVec3(-1,0,0), gzVec3(1,0,0), gzVec3(1,2,0));
		//the texture coordinates for the triangle is specified.
		shape->setTexCord(0 , gzVec2(0,0) , gzVec2(1,0) , gzVec2(1,1));
		//add a second triangle to the geometry. This triangle has index 1.
		shape->setTri(1, gzVec3(1,2,0), gzVec3(-1,2,0), gzVec3(-1,0,0));
		//the texture coordinates for triangle with index 1 is added.
		shape->setTexCord(1 , gzVec2(1,1) , gzVec2(0,1) , gzVec2(0,0));
		//the color for the geometry is added
		shape->setColor(gzVec4(1,1,1,1));
		
		// Create the billboard
		gzBillboard *billBoard = new gzBillboard;
		// specify the billboard type. In this case a cylindrical billboard is used which rotates around an axis
		billBoard->setType(GZ_BILLBOARD_AXIS);
		// specify axis to rotate around. In this case the billboard rotates around the Y-axis
		billBoard->setAxis(gzVec3(0,1,0));
		// add the geometry "shape" to the billboard. This geometry will rotate around the y-axis
		billBoard->addNode(shape);
		billBoard->setCopyMode(GZ_CLONE_SHARE_ALL);
		
		
		// Load the image tree.bmp
		gzImage	*image = gzImageManager::loadImage("tree.bmp", GZ_EVALUATE_EXTENSION);

		if (!image)
		{
			//if the image not is loaded a message is printed. 
			GZMESSAGE(GZ_MESSAGE_FATAL,"Failed to load texture");
			//exit the application
			exit(0);
		}
		
		// when the image is loaded the message "Load image tree.bmp, 128*128" is printed in the console
		GZMESSAGE(GZ_MESSAGE_NOTICE,"Load image %s, %d*%d", (const char *)image->getName(), 
			image->getWidth(), image->getHeight());
		
		// Convert RGB image to RGBA where A is 0 (transparent) for black color
		gzImage *alpha=image->create(gzImage::createImage(GZ_IMAGE_TYPE_RGBA_8));
		//create a mask with black color as default
		gzRGBA mask;
		//use the mask on the alpha image. 
		alpha->alphaMask(mask);
		

		//create a texture
		gzTexture* tex = new gzTexture();
		// set the alpha image as texture
		tex->setImage(alpha);	
		//Specify the method used to apply the texture to the model. 
		//If the texture coordinates have values outside the range [0,1]
		//the value over 1 is clamped to 1 and the value below 0 is clamped to 0.
		tex->setWrapS(GZ_TEXTURE_WRAP_CLAMP);
		tex->setWrapT(GZ_TEXTURE_WRAP_CLAMP);
		// Specify active components in texture. The Red, Green, Blue and alpha component is active.
		tex->setComponents(4);	
		//Activate the use of mipmaps. When the camera zooms in a more detailed image is applied to the geometry
		//and if the camera zooms out a less detailed image is applied to the geometry. 
		tex->useMipMaps(TRUE);
		// a minification filter is used
		tex->setMinFilter(GZ_TEXTURE_MIN_LINEAR_MIPMAP_LINEAR);
		
		// Create texture environment
		gzTexEnv *texenv = new gzTexEnv;
		texenv->setMode(GZ_TEXENV_MODE_MODULATE);	// C=CtCf, A=AtAf
		
		//create a state which is used to apply properties to the geometry.
		gzState* state=new gzState;
		
		//alpha func mode is activated. A fragment is drawn or culled depending on the alpha value
		state->setMode(GZ_STATE_ALPHA_FUNC,GZ_STATE_ON);
		//If the fragment alpha value is greates than 0.5 the fragment is drawn
		state->setAlphaFunc(GZ_ALPHA_FUNC_GREATER,0.5f);
		
		//Enable texture mode
		state->setMode(GZ_STATE_TEXTURE,GZ_STATE_ON);
		//Enable texture environment mode
		state->setMode(GZ_STATE_TEXENV,GZ_STATE_ON);
		//set the texture that shall be used
		state->setTexture(tex);					
		//set the texture environment that shall be used
		state->setTexEnv(texenv);
		
		// Create a group for all trees. All trees is children to this group
		gzGroup *trees = new gzGroup;
		// Apply the state to the group. 
		trees->setState(state);
	
		for( int i=0; i<numTrees; i++ )
		{
			//create a new billboard which is a clone of the billboard created above. 
			gzBillboard *trans = (gzBillboard *)billBoard->clone();
			// Translates the billboard in the x and z directions. 
			// This gives a random position for each tree on the ground
			// A billboard has the same properties as a transform, but rotations cannot be specified
			trans->setTranslation(RNDPos(), 0, RNDPos());
			//add the new billboard to the group.
			trees->addNode(trans);
		}
		// Add the group with all trees to the scene
		scene->addNode(trees);
	}
	
	
	//------------------------------------------------------------------------------
	// Ground
	//------------------------------------------------------------------------------
	{
		// Creates a group node which contains the ground
		gzGroup *ground = new gzGroup;

		// For each quad the corners have to be computed.
		// There are two x values and two z values that build each quad.
		//
		//	(x1, z1)			(x2, z1)
		//			-------------
		//			|			|
		//			|			|
		//			|			|
		//			|			|
		//			|			|
		//			-------------
		//	(x1, z2)			(x2, z2)
		//
		
		for (int ix=0; ix < numGroundQuads ; ix++)
		{
			// Create a triGeometry which is a geometry built of triangles
			gzTriGeometry *shape = new gzTriGeometry();
			// Define the number of triangles in the ground
			shape->setSize(numGroundQuads*2);
			
			//compute the two positions in x direction for each quad
			gzFloat x1 = ix*groundQuadSize - groundWidth/2.0f;
			gzFloat x2 = (ix+1)*groundQuadSize - groundWidth/2.0f;
			
			//define two green colors
			gzVec4 color1(0.0f,0.3f,0.0f,1);
			gzVec4 color2(0.0f,0.4f,0.0f,1);
			
			int triIndex = 0;
			
			for (int iz=0; iz < numGroundQuads; iz++)
			{
				// Compute the two positions in z direction for each quad
				gzFloat z1 = iz*groundQuadSize - groundWidth/2.0f;
				gzFloat z2 = (iz+1)*groundQuadSize - groundWidth/2.0f;
				// add a triangle to the triGeometry. 
				shape->setTri(triIndex, gzVec3(x1,0,z1), gzVec3(x2,0,z1), gzVec3(x2,0,z2));
				// define the color for each triangle. A triangle index specifies the triangle
				// on which the color shall be applied. The green colors are applied in a chess pattern
				shape->setColor(triIndex, ((iz+ix)%2==0) ? color1 : color2);
				// increase the variable triIndex with 1.
				triIndex++;
				// Add another triangle to the triGeometry
				shape->setTri(triIndex, gzVec3(x2,0,z2), gzVec3(x1,0,z2), gzVec3(x1,0,z1));
				// Apply the same color to this triangle as to the previous.
				shape->setColor(triIndex, ((iz+ix)%2==0) ? color1 : color2);
				triIndex++;
			}
			// An optimization of the triGeometry is made.
			gzGeometryOptimizer::optimize(shape);
			
			// The trigeometry is added to the ground group node
			ground->addNode(shape);
		}
		// The ground is added to the scene.
		scene->addNode(ground);
	}
	
	//------------------------------------------------------------------------------
	// Skybox
	//------------------------------------------------------------------------------
	{
		// Create a skyBox. If no images is specified for the skybox a default skybox
		// with blue sky and green ground is created
		gzSkyBox *skyBox=new gzSkyBox;
		// Add the skyBox to the scene. 
		scene->addNode(skyBox);
	}
	
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
		format->setFullScreenWidth(1600);

	}
	
	
	//------------------------------------------------------------------------------
	// Create our window
	//------------------------------------------------------------------------------
	//
	m_win=new gzWindow("Billboards",NULL, format);
	// The background color to the window is specified
	m_win->setBackground(0.3f, 0.3f, 1.0f, 1.0f);
	
	// The window has a default perspective camera which is used. The scene is connected with this camera.
	m_win->getCamera()->setScene(scene);
	// Specify the tick interval. When the interval time has elapsed the methoc onTick() is called.
    setTickInterval(0.01);
}


void BillboardApp::onTick()
{
	// Move the camera in a large circle through the trees
	// The angle is changed each time the onTick() method is called
    m_angle=fmod(m_angle+0.002,2*GZ_PI);
	
	// Compute a position for the camera. The position in x and z direction is changed but the 
	// vertical camera placement is constant.
    gzVec3 from((gzFloat)(cameraRadius*cos(m_angle)), 3.0f, (gzFloat)(-cameraRadius*sin(m_angle)));
	// Set the position for the camera
	m_win->getCamera()->setPosition(from.v1, from.v2, from.v3);
	// Set the heading, pitch and roll for the camera. 
	// The heading specify in which direction the camera is looking. 
	// The pitch specify if the camera is looking up or down. In this case the camera is looking slightly down
    m_win->getCamera()->setHPR(RAD2DEG(m_angle)+20, -5, 0);
	
	// When the new camera position and viewing direction is changed 
	// the image in the window is refreshed. This is the information that is visible to the camera
	// which is connected to the window. This can be automated with setRefreshRate();
	m_win->refreshWindow();
	
}
