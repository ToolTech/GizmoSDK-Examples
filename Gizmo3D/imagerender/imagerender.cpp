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
// File			: imagerender.cpp
// Module		: 
// Description	: Test app for image rendering to a texture
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
// AMO  040601  Created file    
//
// ******************************************************************************

#include "gzGizmo3DLibrary.h"


//******************************************************************************
// Function	: gzScene *createScene()
//									
// Purpose  : Create a scene with a animated model
//									
// Notes	: -	
//									
// Revision History...							
//									
// Who	Date	Description						
//									
// AMO	040603	Created 
//									
//******************************************************************************
gzScene *createScene()
{
	// Load a node with suggested animation
	gzNode *node=gzDbManager::loadDB("../dbviewer/anim.3ds",GZ_EVALUATE_EXTENSION,GZ_DB_FLAGS_USE_ANIMATION|GZ_DB_FLAGS_USE_MIPMAPS);

	// Check to see if it is a frame
	gzFrame *frame=gzDynamic_Cast<gzFrame>(node);

	if(frame)
	{
		frame->setFrameDuration(0.05);
		frame->startFrameCounter();
	}

	// add a top unit scaling transform * 60

	gzTransform *trans=new gzTransform;

	trans->addNode(node);

	trans->unitScale();
	trans->scale(60);

	gzScene *scene=new gzScene;

	scene->addNode(trans);

	// And return a scene with the animated node

	return scene;
}

//******************************************************************************
// Class	: MyWindow
//									
// Purpose  : Derive a gzWindow and update a image based texture
//									
// Notes	: -	
//									
// Revision History...							
//									
// Who	Date	Description						
//									
// AMO	040603	Created 
//									
//******************************************************************************
class MyWindow : public gzWindow
{
public:
	MyWindow():gzWindow("Image Rendering")
	{
		// Set up actual image render

		m_imageRender=new gzImageRender(this,512,512);

		gzPerspCamera *imagecam=new gzPerspCamera;

		imagecam->setPosition(0,0,50);

		imagecam->lookAt(0,0,0);

		imagecam->setScene(createScene());

		m_imageRender->setCamera(imagecam);


		// Create a scene with a rotating textured quad
		
		gzScene *scene=new gzScene;

		
		gzGeometry *sphere=new gzGeometrySphere(5,40,gzVec4(1,1,1,1));


		gzState *state=new gzState;

		gzTexture *tex=new gzTexture;

		//m_image=new gzImage_RGB_8;
		m_image=gzImage::createImage(GZ_IMAGE_TYPE_RGB_8);

		m_image->setSize(512,512);

		m_image->createArray(TRUE);

		m_imageRender->setImage(m_image);

		tex->setImage(m_image);

		state->setTexture(tex);
		state->setMode(GZ_STATE_TEXTURE,GZ_STATE_ON);

		sphere->setState(state);


		// Create nodes

		gzFrameTransform *ftrans=new gzFrameTransform;

		gzFrame *frame=new gzFrame;


		// Set up node animation

		ftrans->setFrameRotation(0,gzVec3(0,1,0),0);
		ftrans->setFrameRotation(1,gzVec3(0,1,0),90);
		ftrans->setFrameRotation(2,gzVec3(0,1,0),180);
		ftrans->setFrameRotation(3,gzVec3(0,1,0),270);
		ftrans->setFrameRotation(4,gzVec3(0,1,0),360);
		ftrans->calculateRotations();


		// Set up frame control

		frame->setNumberOfFrames(4);
		frame->setFrameDuration(2);
		frame->startFrameCounter();

		// Connect scene nodes

		ftrans->addNode(sphere);

		frame->addNode(ftrans);

		scene->addNode(frame);


		// Control the camera
				
		gzCamera *cam=getCamera();

		cam->setScene(scene);

		cam->setPosition(0,0,50);

		cam->lookAt(0,0,0);

		setSwapInterval(0);

		setBackground(0.5f,0.5f,0.5f);
	};

	virtual gzBool	refreshWindow()
	{
		// Refresh the image before window refresh
		m_imageRender->refreshImage();
		return gzWindow::refreshWindow();
	}

	virtual gzBool onKey(gzKeyValue key , gzKeyState keystate , gzInt32 mouse_x , gzInt32 mouse_y)
	{
		printf("Refresh Rate:%f swaps/second\n",getFrameRate());

		return gzWindow::onKey(key,keystate,mouse_x,mouse_y);
	}


private:

	gzImage *m_image;

	gzImageRender *m_imageRender;

};

//******************************************************************************
// Class	: WindowApp
//									
// Purpose  : Derive a gzApplication to control our window
//									
// Notes	: -	
//									
// Revision History...							
//									
// Who	Date	Description						
//									
// AMO	040603	Created 
//									
//******************************************************************************
class WindowApp : public gzApplication
{
public:
		
	WindowApp():m_win(NULL)
	{
	};

    virtual ~WindowApp()
	{
	}

    void Create()
	{
		m_win = new MyWindow;
	}

	virtual gzVoid onIdle()
	{
		if(!m_win->refreshWindow())
			gzSleep(10);
	}

private:
	
	// Note that we need a ref pointer to our window as it can be referenced by Gizmo3D as well.
	gzRefPointer<MyWindow>		m_win;
};

GZ_DECLARE_REFPTR(WindowApp);



//******************************************************************************
// Function	: main()
//									
// Purpose  : Setup our WindowApp
//									
// Notes	: -	
//									
// Revision History...							
//									
// Who	Date	Description						
//									
// AMO	040603	Created 
//									
//******************************************************************************
int main(int argc, char *argv[])
{
    gzStartUpGizmo();	// Needed for some systems to install external graphics engines

	gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG);

	gzGraphicsEngine::useEngine(GZ_ENGINE_OPENGL);

	gzInitializeDbManagers();

	try
	{
		// Make the application
		WindowAppPtr app=new WindowApp;

		// Create the scene and the window
		app->Create();

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



