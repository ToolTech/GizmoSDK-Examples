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
// File			: large.cpp
// Module		: 
// Description	: Test app for dynamic loading
// Author		: Anders Modén          
// Product		: Gizmo3D 2.11.70
//              
//
//                      
// NOTE:	The GIZMO package defines a general purpose API for large model
//			visualisation and advanced graphic behaviours. The package is similar
//			to Cosmo3D, Inventor, Performer etc. but adds automated behaviour
//			patterns to the graphics. 
//
//
// Revision History...                                                  
//                                                                      
// Who  Date    Description                                             
//                                                                      
// AMO  010730  Added plugin loading and new db loaders
//
// ******************************************************************************
//
//  The Application shows how to control the camera position with gzRoiPosition enabled. The camera is not actually 
//  moving but a double precision offset is used to emulate postion control from -1e308 to 1e308 values.
//
//  You can also see the dynamic loading feature. Objects that are within a certian distance are dynamically loaded by 
//  another thread and objects that are going out of scope are erased so very large databases can be used stored
//  on disk. Just add the gzRoi and add gzRoiNodes to that and set camera->useRoiPosition and you are up and running !
//
// ******************************************************************************


#include "gzGizmo3DLibrary.h"

class myApp : public gzApplication
{
public:
		
		myApp()   
		{

			// Create a scene to render
			gzScene *scene=new gzScene("3ds Scene");

			gzEnvironment *env=new gzEnvironment;

			scene->addNode(env);

			gzLight *light=new gzLight;

			light->setPosition(0,0,400);

			env->addLight(light);

			gzRoi *roi = new gzRoi;

			
			for(gzInt32 i=-200;i<200;i+=10)
			{
				for(gzInt32 j=-200;j<200;j+=10)
				{
					roi->addRoiNode(gzDoubleXYZ(i,0,j),70,200,"articulate.flt",TRUE,GZ_DB_FLAGS_OPTIMIZE_DB,1);
				}
			}
			
			
			
			// Add the roi management to the scene environment
			env->addNode(roi);

			
			// Initialise a window to display the scene

			m_win=new gzWindow("Infinity");

			cam=m_win->getCamera();
			
			//Using default persp camera

			// Enable this if huge positions are used. This way the camera doesn't move but the entire scene is transformed to the local position
			//cam->useRoiPosition(TRUE);
			
			cam->setScene(scene);
			cam->setHPR(45,-60,0);
			cam->setPosition(0,50,0);
			//cam->lookAt(0,0,0);

				
			// And add the scene to the camera
			
			// Additional settings for window
			m_win->setBackground(0.0f, 0.6f, 0.6f, 1.0f);


			// Set some vars for camera movement
			pos=0;
			dir=1;

			setTickInterval(0.03);

			time=gzTime::systemSeconds();

		};

		gzVoid onTick()
		{
			if(pos>150)
			{
				dir=dir * -1;
				pos=150;
			}
			else if(pos<-150)
			{
				dir=dir * -1;
				pos=-150;
			}

			gzDouble now=gzTime::systemSeconds();

			pos=pos+dir*(now-time);

			time=now;

			cam->setPosition(pos,50,pos);
						
			m_win->refreshWindow();
		}

		~myApp()                // destructor
		{
			setTickInterval(0);

			if(m_win)
				delete m_win;
		}

	
private:

	gzWindow		*m_win;

	gzCamera		*cam;

	gzDouble pos,dir,time;
};



int main(int argc , char *argv[] )
{
	gzStartUpGizmo();	// Needed for some systems to install external graphics engines

	gzDynamicLoaderManager::startManager();
	
	try
	{
		// Set the appropriate notification level

        gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG);

		// Start dynamic loading


		// Notify user of current license

		gzLicense::notifyDefaultLicense();

		// Initialize all db managers ( the image managers as well )

		gzInitializeDbManagers();

		// Set the appropriate graphics engine API

		gzGraphicsEngine::useEngine(GZ_ENGINE_OPENGL);

		// Make an application and run it
		myApp app;
		
		app.run();

	}
	catch(gzBaseError &error)       // In case of exceptions thrown we want to print the message
	{
		error.reportError();
	}

	gzShutDownGizmo();

	return 0;
}
