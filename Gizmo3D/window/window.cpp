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
// File			: window.cpp
// Module		: 
// Description	: Test app for IRMA algorithm.
// Author		: Anders Modén          
// Product		: Gizmo3D 2.11.70
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
// The example shows how to derive the gzWindow class to catch the events
// in the virtual methods and how to use the APARS geometry with LODs.
// The example gives a simple implementation of a keyboard controlled camera
// that can be moved around with the arrow keys and the keys 'a' and 'z'
// Press spacebar to see wireframe mode.
// Press 's' to increase LOD and 'x' to decrease it
//
// The example understands the following command switches
// "window -f" runs in full screen mode
// "window -s xx" gives a grid of xx*xx with 32*32*2 tris in each patch
//
// ******************************************************************************



#include "gzGizmo3DLibrary.h"  // Include alla Gizmo files

#include <ctype.h>

long g_gridsize=10;
					

// A sample subclass of the parametric geometry base class

// It uses some of the protected member variables that gzParametricGeometry defines

class MyParametricGeometry : public gzParametricGeometry
{
public:

	GZ_DECLARE_TYPE_INTERFACE;	// typed interface

	gzVec3 getCoordinate(gzDouble s , gzDouble t)	// Provide this for your coordinates
	{
		return gzVec3((gzReal)s,(gzReal)(50*cos(s/400)*cos(t/500)+4*cos(s/20)*cos(t/15)+5*cos(s/100)*cos(t/150))+9,(gzReal)(-t));
	}

	gzVec2 getTexCoordinate(gzDouble s , gzDouble t , gzUInt32 unit)
	{
		return gzVec2((gzReal)((s-m_start_s)/(m_stop_s-m_start_s)),(gzReal)((t-m_start_t)/(m_stop_t-m_start_t)));
	}

	gzVec4 getColor(gzDouble s , gzDouble t)		// Provide this for your colors if you use useColors(TRUE)
	{
		gzDouble lum=0.5*cos(s/20)*cos(t/15)+0.5;
		return gzVec4((gzReal)lum,(gzReal)lum/2,(gzReal)(0.4*cos(s/70)+0.2),(gzReal)1.0);
	}

	// Just so we can clone the geometry !!

	gzReference* clone() const
	{
		return (gzReference *) new MyParametricGeometry(*this);
	}

};

// Declare the type info 

GZ_DECLARE_TYPE_CHILD(gzParametricGeometry,MyParametricGeometry,"MyParametricGeometry");

class MyWindow : public gzWindow
{
public:
	MyWindow( gzGraphicsFormat *format):gzWindow("MyWindow",NULL,format)
	{

        gzImage *image=gzImageManager::loadImage("dust.bmp",GZ_EVALUATE_EXTENSION);

		gzTexture *texture=new gzTexture;
		texture->setImage(image);
		texture->useMipMaps(TRUE);
		texture->setMinFilter(GZ_TEXTURE_MIN_LINEAR_MIPMAP_LINEAR);


        gzImage *image2=gzImageManager::loadImage("light.bmp",GZ_EVALUATE_EXTENSION);

        gzTexture *texture2=new gzTexture;
        texture2->setImage(image2);
        texture2->useMipMaps(TRUE);
        texture2->setMinFilter(GZ_TEXTURE_MIN_LINEAR_MIPMAP_LINEAR);


		gzTexEnv *texenvA=new gzTexEnv;
		texenvA->setMode(GZ_TEXENV_MODE_DECAL);


		gzTexEnv *texenvB=new gzTexEnv;
		texenvB->setMode(GZ_TEXENV_MODE_MODULATE);

		m_state=new gzState;
        m_state->setNumberOfTextures(2);
		m_state->setMode(GZ_STATE_TEXTURE,GZ_STATE_ON);
		m_state->setTexture(texture);
        m_state->setTexture(texture2,1);

		m_state->setNumberOfTexEnv(2);

		m_state->setTexEnv(texenvA);
		m_state->setTexEnv(texenvB,1);
		m_state->setMode(GZ_STATE_TEXENV,GZ_STATE_ON);



		m_state->setFrontPolygonMode(GZ_POLYGON_MODE_FILL);
		m_state->setBackPolygonMode(GZ_POLYGON_MODE_CULL);
		m_state->setMode(GZ_STATE_POLYGON_MODE,GZ_STATE_ON);

		m_state->ref();

        gzState::setGlobalState(getContext(),m_state);

		gzGroup *group=new gzGroup;

                m_level=5.0;

		/*	
			The following code snippet creates a CATS (Continious Adaptive Terrain Set).
			The base class is called gzMapGeometry which only stores the Z=f(x,y) (Altitude)
			To use your own terrian you must derive this class and provide your own
			virtual functions to retreive coordinates etc.
		*/

		int i,j;

		for(i=0;i<=g_gridsize;i++)
			for(j=0;j<=g_gridsize;j++)
			{
				m_geom[i][j] = new MyParametricGeometry();
				m_geom[i][j]->useColor(TRUE);
				m_geom[i][j]->setTextureUnits( 2);

				m_geom[i][j]->useTexture(TRUE,0);
				m_geom[i][j]->useTexture(TRUE,1);
                m_geom[i][j]->setStartParams(i*64,j*64);
                m_geom[i][j]->setStopParams(i*64+64,j*64+64);
				m_geom[i][j]->setStepParams(1,1);

				m_geom[i][j]->createPatch();

				m_geom[i][j]->setLODFactor(m_level);
				m_geom[i][j]->createDynamicLOD();

				group->addNode(m_geom[i][j]);
			}

		// Connecting the patches

		for(i=0;i<=g_gridsize;i++)
			for(j=0;j<=g_gridsize;j++)
			{
				if(j!=g_gridsize)
					m_geom[i][j]->unifyDynamicLOD(m_geom[i][j+1]);

				if(i!=g_gridsize)
					m_geom[i][j]->unifyDynamicLOD(m_geom[i+1][j]);
			}
		
			
		
		// General scene stuff

		gzScene *scene=new gzScene;

		scene->addNode(group);

		gzCamera *cam=getCamera();

		cam->setScene(scene);

		cam->setPosition(0,30,0);
		cam->lookAt(0,0,0);
                cam->setFarClipPlane(10000);


		m_key_a_is_down=FALSE;			// Some states for keys to be used
		m_key_z_is_down=FALSE;
		m_key_up_is_down=FALSE;
		m_key_down_is_down=FALSE;
		m_key_left_is_down=FALSE;
		m_key_right_is_down=FALSE;


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
			case GZ_KEY_LEFT        : m_key_left_is_down = key_is_down;             break;
			case GZ_KEY_RIGHT       : m_key_right_is_down = key_is_down;			break;
			case GZ_KEY_UP          : m_key_up_is_down = key_is_down;               break;
			case GZ_KEY_DOWN        : m_key_down_is_down = key_is_down;             break;
			case 'a'                : m_key_a_is_down = key_is_down;                break;
			case 'z'                : m_key_z_is_down = key_is_down;                break;

			case ' ':

				if(keystate == GZ_KEY_STATE_PRESSED)
				{
					gzState *state=new gzState;

					state->setFrontPolygonMode(GZ_POLYGON_MODE_LINE);
					state->setBackPolygonMode(GZ_POLYGON_MODE_CULL);
					state->setMode(GZ_STATE_POLYGON_MODE,GZ_STATE_ON);
					state->setOverride(GZ_STATE_POLYGON_MODE,GZ_STATE_ON);

                    gzState::setGlobalState(getContext(),state);
				}
				if(keystate == GZ_KEY_STATE_RELEASED)
				{
                     gzState::setGlobalState(getContext(),m_state);
				}
				break;

			case 's':
				if(keystate == GZ_KEY_STATE_PRESSED)
				{
					m_level*=2;
					for(int i=0;i<=g_gridsize;i++)
						for(int j=0;j<=g_gridsize;j++)
							m_geom[i][j]->setLODFactor(m_level);
				}

				break;

			case 'x':
				if(keystate == GZ_KEY_STATE_PRESSED)
				{
					m_level/=2;
					for(int i=0;i<=g_gridsize;i++)
						for(int j=0;j<=g_gridsize;j++)
							m_geom[i][j]->setLODFactor(m_level);
				}
				break;


		}

		return gzWindow::onKey(key,keystate,mouse_x,mouse_y);
	}
	

	gzBool m_key_a_is_down;
	gzBool m_key_z_is_down;
	gzBool m_key_up_is_down;
	gzBool m_key_down_is_down;
	gzBool m_key_left_is_down;
	gzBool m_key_right_is_down;

	gzDynamicArray< gzDynamicArray< MyParametricGeometry *> > m_geom;

	gzState	*m_state;

	gzFloat m_level;

};

// Definition of a sample application
// The application provides an initialisation and an onIdle loop manager
// to do the refresh of the window

class WindowApp : public gzApplication
{
public:
		
        WindowApp():m_win(NULL),m_format(NULL),m_heading(-20),m_pitch(-20),m_position(0,80,0)
	{
	};

    ~WindowApp()
	{
		if(m_win)
			delete m_win;

		if(m_format)
			delete m_format;
	}

    void Create(gzBool bFullScreen)
	{
		if(bFullScreen)
		{
			m_format = new gzGraphicsFormat;

			m_format->setFullScreenWidth(800);
			//m_format->setFullScreenBits(16);
			m_format->useFullScreen(TRUE);
		}

		m_win = new MyWindow(m_format);
	}

	void onIdle()
	{

		//	The following code snippet is used to control the movements of the camera

		if(m_win)
		{
			if(m_win->m_key_left_is_down)
				m_heading=(gzFloat)fmod(m_heading+1,360);

			if(m_win->m_key_right_is_down)
				m_heading=(gzFloat)fmod(m_heading-1,360);

			if(m_win->m_key_up_is_down)
				m_pitch=(gzFloat)fmod(m_pitch-0.5,360);

			if(m_win->m_key_down_is_down)
				m_pitch=(gzFloat)fmod(m_pitch+0.5,360);

			if(m_win->m_key_z_is_down)
				m_position=m_position-m_win->getCamera()->getDirection();

			if(m_win->m_key_a_is_down)
				m_position=m_position+m_win->getCamera()->getDirection();

			m_win->getCamera()->setHPR(m_heading,m_pitch,0);
			m_win->getCamera()->setPosition(m_position.v1,m_position.v2,m_position.v3);
			m_win->refreshWindow();
		}
	}

private:
	
	MyWindow			*m_win;
	gzGraphicsFormat	*m_format;

	gzFloat             m_heading;
	gzFloat             m_pitch;
	gzVec3              m_position;


};



int main(int argc, char *argv[])
{
	gzStartUpGizmo();	// Needed for some systems to install external graphics engines
	
	gzBool bFullScreen = FALSE;

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

					case 'S':
						argv++;
						argc--;
						g_gridsize=atol(argv[0]);
						break;
				}

			}
			argv++;
			argc--;
		}
	}

	// Make the application
	WindowApp app;
	try
	{
		// Create the scene and the window
		app.Create(bFullScreen);

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

