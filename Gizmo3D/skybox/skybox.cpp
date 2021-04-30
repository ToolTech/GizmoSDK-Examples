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
// File			: SkyBox.cpp
// Module		: 
// Description	: Test app shows skybox feature
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

//#define GZ_MEM_DEBUG	//to enable mem debug


// Enable this line to get a user defined skybox
//#define USE_USER_SKYBOX

//#define USE_USER_SKYBOX_ALT

#include "gzGizmo3DLibrary.h"



// And here comes the Java look alike app. It is a matter of taste.

class MyWindow : public gzWindow
{
public:
	MyWindow(gzGraphicsFormat *format):gzWindow(gzString("Gizmo3D : ")+gzTime::now(TRUE).asString(),NULL,format)
	{
			// Create a scene to render -------------------------------------
			// We must have a scene that the camera can "look at"
			m_scene=new gzScene("Database Loader Example Scene");
			
			// Create a backface culling state
			// We don't want to render backfaces. this way we can "force" the geoms to be rendered without 
			// backfaces
			m_state=new gzState;

			m_state->setMode(GZ_STATE_POLYGON_MODE,GZ_STATE_ON);
			m_state->setBackPolygonMode(GZ_POLYGON_MODE_CULL);
			m_state->setFrontPolygonMode(GZ_POLYGON_MODE_FILL);

			m_state->disableAllNonEnabledModes();	// We need this for the top global state

			gzState::setGlobalState(getContext(),m_state);
			
			gzEnvironment *env=new gzEnvironment;
			
			//env->useFog(TRUE);
			env->setFogStart(0);
			env->setFogEnd(5000);
			env->setFogMode(GZ_FOG_MODE_LINEAR);
			env->setFogDensity(0.7f);
			env->setFogColor(gzRGBA(0.5f,0.5f,0.5f,1.0f));
				
			m_scene->addNode(env);


			gzSkyBox *box=new gzSkyBox;
			
			box->setHorizonAngle(1);
			

#if defined USE_USER_SKYBOX

			box->setImageSide(GZ_SKYBOX_EAST,gzImageManager::loadImage("east.bmp"));
			box->setImageSide(GZ_SKYBOX_SOUTH,gzImageManager::loadImage("south.bmp"));
			box->setImageSide(GZ_SKYBOX_WEST,gzImageManager::loadImage("west.bmp"));
			box->setImageSide(GZ_SKYBOX_NORTH,gzImageManager::loadImage("north.bmp"));
			box->setImageSide(GZ_SKYBOX_SKY,gzImageManager::loadImage("sky.bmp"));
			box->setImageSide(GZ_SKYBOX_GROUND,gzImageManager::loadImage("ground.bmp"));

			box->setHorizonAngle(20);

#endif


#if defined USE_USER_SKYBOX_ALT

			box->setImageSide(GZ_SKYBOX_SKY,gzImageManager::loadImage("sky.png"));
			box->setImageSide(GZ_SKYBOX_NORTH,gzImageManager::loadImage("north.png"));
			box->setImageSide(GZ_SKYBOX_EAST,gzImageManager::loadImage("east.png"));
			box->setImageSide(GZ_SKYBOX_WEST,gzImageManager::loadImage("west.png"));
			box->setImageSide(GZ_SKYBOX_SOUTH,gzImageManager::loadImage("south.png"));
			
			box->setHorizonAngle(10);

#endif

			box->setStandardHeight(2000);

			box->setGroundPoint(0,-500,0);
			
			box->useFixedHeight(TRUE);

			env->addNode(box);


			// Use this code to add moving sky layer

			

			gzSkyLayer *layer;

			layer=new gzSkyLayer;

			gzImage *clouds=gzImageManager::loadImage("cloud2.png");

			layer->setLayerImage(clouds,100000,100000,2000,600,300);
			
			layer->setHorizonAngle(10);

			env->addNode(layer);

			/*
			
			layer=new gzSkyLayer;

			layer->setHorizonAngle(10);
			
			layer->setLayerImage(clouds,70000,50000,3000,0,500);

			env->addNode(layer);

			*/


			// Now we want to look at the scene. Grab the default perspective camera from the window and set the scen as the
			// active one.
			getCamera()->setScene(m_scene);
			getCamera()->setNearClipPlane(1);
			getCamera()->setFarClipPlane(5000);

			// Lets add some movement to the scene

			m_input=new gzSimpleMouseViewControl(getCamera());

			addInputInterface(m_input);


			// Hmm. trivial
			setBackground(0.5f,0.5f,0.5f,1.0f);

			// and show us ...
			show();
	};


	/*
		The following code snippet is a virtual function to catch the window messages
		for pressed, repeated and released keys
	*/

	gzBool onKey(gzKeyValue key , gzKeyState keystate , gzInt32 mouse_x , gzInt32 mouse_y)
	{
		switch(key)
		{
			case ' ':	// Space pressed

				if(keystate == GZ_KEY_STATE_PRESSED)
				{
					gzState *state=new gzState;

					state->setFrontPolygonMode(GZ_POLYGON_MODE_LINE);
					state->setBackPolygonMode(GZ_POLYGON_MODE_CULL);
					state->setMode(GZ_STATE_POLYGON_MODE,GZ_STATE_ON);
					state->setOverride(GZ_STATE_POLYGON_MODE,GZ_STATE_ON);
					state->setOverride(GZ_STATE_TEXTURE,GZ_STATE_ON);

					state->setMode(GZ_STATE_GENERATE_DEBUG_INFO,GZ_STATE_ON);
					state->setDebugMode((gzStateDebugMode)(GZ_STATE_DEBUG_COLLECT_STATS));
			

					state->disableAllNonEnabledModes();

					gzState::setGlobalState(getContext(),state);
				}
				if(keystate == GZ_KEY_STATE_RELEASED)
				{
					gzState::setGlobalState(getContext(),m_state);
				}
				break;

			case GZ_KEY_LBUTTON :

				if((keystate == GZ_KEY_STATE_PRESSED) && !m_inMousePress)
				{
					setCaptureMouse(TRUE);
					setHideMouse(TRUE);
					m_inMousePress=TRUE;
				}
				else if((keystate == GZ_KEY_STATE_RELEASED) && m_inMousePress)
				{
					setHideMouse(FALSE);
					setCaptureMouse(FALSE);
					m_inMousePress=FALSE;
				}
				break;

			case 'd' :
				if(keystate == GZ_KEY_STATE_PRESSED)
				{
					getCamera()->getScene()->debug(GZ_DEBUG_SHOW_ALL);
				}
				break;


		}

		return gzWindow::onKey((gzKeyValue)key,(gzKeyState)keystate,mouse_x,mouse_y);
	}


	virtual ~MyWindow()
	{
		if(m_input)
			delete m_input;
	}

	gzRefPointer<gzState> m_state;

	gzBool				m_inMousePress;

	gzScene				*m_scene;

	gzSimpleMouseViewControl	*m_input;

};

// Definition of a sample application
// The application provides an initialisation and an onIdle loop manager
// to do the refresh of the window

class WindowApp : public gzApplication
{
public:
		
	WindowApp():m_win(NULL),m_format(NULL),m_heading(0),m_pitch(0),m_position(30,0,200)
	{
	};

    ~WindowApp()
	{
		if(m_win)
		{
			delete m_win;
		}

	}

    void Create()
	{
		gzGraphicsFormat *format=new gzGraphicsFormat;

		m_win = new MyWindow(format);

		gzPerspCamera *camera=gzDynamic_Cast<gzPerspCamera>(m_win->getCamera());

		camera->setHPR(m_heading,m_pitch,0);
		camera->setPosition(m_position.v1,m_position.v2,m_position.v3);
		camera->lookAt(0,0,0);
		camera->useInfiniteFarPlane(TRUE);

	}

	void onIdle()
	{
		if(m_win)
		{
			m_win->triggerKeyEvaluation();

			if(!m_win->refreshWindow())	// Yield some time if no rendering
				gzSleep(30);

		}

	}

private:

	friend class MyWindow;
	
	MyWindow                *m_win;
	gzGraphicsFormat        *m_format;

	gzFloat             m_heading;
	gzFloat             m_pitch;
	gzVec3              m_position;

};



//---------------------------------------------------------------


int main(int argc, char *argv[])
{
	gzStartUpGizmo();	// Needed for some systems to install external graphics engines
	
	gzMessage::setMessageLevel(GZ_MESSAGE_MEM_DEBUG);

	try
	{ 
		gzLicense::notifyDefaultLicense();
	
		gzGraphicsEngine::useEngine(GZ_ENGINE_OPENGL);
	
		gzInitializeDbManagers();


		// Make the application
		WindowApp app;
	      
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




