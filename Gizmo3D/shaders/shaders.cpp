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
// File			: shaders.cpp
// Module		: 
// Description	: Test app to show shader usage
// Author		: Anders Modén          
// Product		: Gizmo3D 2.12.199
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
// AMO  030707  Created file    
//
// ******************************************************************************

// The general include
#include "gzGizmo3DLibrary.h"

gzNode *createModel()
{
	gzGeometry *geom=new gzGeometrySphere(10);

	gzArray<gzVec3>	&normals=geom->getNormalArray(FALSE);

	gzArray<gzVec3>	tangents(normals.getSize());

	for(gzUInt32 i=0;i<tangents.getSize();i++)
	{
		// create sample tangent
		tangents[i]=gzVec3(normals[i].v3,0,-normals[i].v1);
	}

	gzVertexAttributeListTemplate<gzVec3> *vaList=new gzVertexAttributeListTemplate<gzVec3>(tangents,10);

	geom->addVertexAttributeList(vaList);

	return geom;
}

// And here comes the Java look alike app. It is a matter of taste.

class MyWindow : public gzWindow
{
public:
	MyWindow(gzGraphicsFormat *format , gzString filename ):gzWindow(gzString("Gizmo3D : ")+gzTime::now(TRUE).asString(),NULL,format,FALSE)
	{
			// Create a scene to render -------------------------------------
			// We must have a scene that the camera can "look at"
			m_scene=new gzScene("Database Loader Example Scene");
			
			// Create some light environment group. This way we get lighting into the model
			// All stuff under this is lit with material s etc.
			gzEnvironment *group=new gzEnvironment;

			// Create a backface culling state
			// We don't want to render backfaces. this way we can "force" the geoms to be rendered without 
			// backfaces. It also enable the use of the shaders

			m_state=new gzState;

			m_state->setMode(GZ_STATE_POLYGON_MODE,GZ_STATE_ON);
			m_state->setBackPolygonMode(GZ_POLYGON_MODE_CULL);
			m_state->setFrontPolygonMode(GZ_POLYGON_MODE_FILL);
			m_state->disableAllNonEnabledModes();	// We need this for the top global state

			gzState::setGlobalState(getContext(),m_state);
			

			// ---------------------- DB loading ------------------------------

			gzString vertexProgram			="glsl_PFLight.vsh";
			gzString fragmentProgram		="glsl_PFLight.fsh";

			gzString alt_vertexProgram		="PFLight.vsh";
			gzString alt_fragmentProgram	="PFLight.fsh";



			// Create a object node from a generic db file
			
			//model=gzDbManager::loadDB(filename);

			model=createModel();

			if(model)	// We got a model
			{
			
					gzState *state;

					state=model->getState();

					if(!state)
						state=new gzState;


					// Set the vertex program

					gzGFXProgram *program_vp=new gzGFXProgram;

					program_vp->loadProgramScript(vertexProgram);

					program_vp->bindVertexAttribute("tangent",10);

					// Create fallback program chain

					gzGFXProgram *program_vp_alt=new gzGFXProgram;

					program_vp_alt->loadProgramScript(alt_vertexProgram);

					program_vp->setFallbackProgram(program_vp_alt);


					state->setGFXProgram(GZ_GFX_VERTEX_PROGRAM,program_vp);
					

					// Set the fragment program

					gzGFXProgram *program_fp=new gzGFXProgram;

					program_fp->loadProgramScript(fragmentProgram);

					// Create fallback program

					gzGFXProgram *program_fp_alt=new gzGFXProgram;

					program_fp_alt->loadProgramScript(alt_fragmentProgram);

					program_fp->setFallbackProgram(program_fp_alt);

					state->setGFXProgram(GZ_GFX_FRAGMENT_PROGRAM,program_fp);
					
					program_fp->setLocalParameter("color",gzVec4(1.0f, 0.5f, 0.3f, 1.0f));


					// Enable GFX programs

					state->setMode(GZ_STATE_GFX_PROGRAM,GZ_STATE_ON);

					model->setState(state);


			}

			gzNodeOptimizer opt;		// We let the optimizer take away redundant geometry etc.

			opt.setMaxRecursionDepth(0);

			model=opt.optimize(model,(gzNodeOptimizeLevel)(GZ_NODE_OPTIMIZE_DONT_COMBINE_NAME_ENCODED/*|GZ_NODE_OPTIMIZE_DONT_CONCAT_GEOMETRY*/));

			// -------------------- lights --------------------------------

			// Add some dynamic light
			m_spin=new gzLight;						// This is the lamp that we move around
			m_spin->setSpecularColor(0.2f,0.2f,0.2f);
			m_spin->setDiffuseColor(0.5f,0.5f,0.5f);
			m_spin->setAmbientColor(0.3f,0.3f,0.3f);
			m_spin->setPosition(26,10,90);

			group->addLight(m_spin);

			// ------------------------ some global settings --------------

			gzEnvironment::setTwoSideLighting(getContext(),TRUE);

			gzEnvironment::setLocalViewer(getContext(),TRUE);

			// Translate the model and scale it

			gzTransform *trans=new gzTransform;

			trans->addNode(model);
				
			trans->unitScale();

			trans->scale(20.0f,20.0f,20.0f);

			group->addNode(trans);
			
			// Add a geometry alike lamp to show position of light

			m_lamp=new gzTransform;

			gzGeometry *sphere=new gzGeometrySphere(1 , 20 ,gzVec4(1.0,1.0,1.0,1.0) );

			m_lamp->addNode( sphere );

			// Set mouse press state
			m_inMousePress=FALSE;



			// And add the group to the scene as well
			m_scene->addNode(group);

			// And add the lamp
			m_scene->addNode(m_lamp);

						
			// Now we want to look at the scene. Grab the default perspective camera from the window and set the scen as the
			// active one.
			getCamera()->setScene(m_scene);
			getCamera()->setNearClipPlane(1);
			getCamera()->setFarClipPlane(2000);

			// Lets add some movement to the scene

			m_input=new gzSimpleMouseViewControl(getCamera());

			addInputInterface(m_input);

			// Hmm. trivial
			setBackground(0.0f,0.0f,1.0f,1.0f);

			// and show us ...
			show();

			// hm. wonder why I put this one here ?
			angle=2.22f;
	};


	/*
		The following code snippet is a virtual function to catch the window messages
		for pressed, repeated and released keys
	*/

	gzBool onKey(gzKeyValue key , gzKeyState keystate , gzInt32 mouse_x , gzInt32 mouse_y)
	{
		switch(key)
		{
			case ' ':	// Space pressed // default to non shader view

				if(keystate == GZ_KEY_STATE_PRESSED)
				{
					gzState *state=new gzState;

					state->setOverride(GZ_STATE_GFX_PROGRAM,GZ_STATE_ON);
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

		}

		return gzWindow::onKey((gzKeyValue)key,(gzKeyState)keystate,mouse_x,mouse_y);
	}


	gzVoid onIdle()
	{
		// Hmm. Now i remember. The spinning lamp..

		angle+=0.003f;

		if(angle>2*GZ_PI)
			angle-=(gzReal)(2*GZ_PI);

		m_spin->setPosition((gzReal)(50*cos(5*angle)),5.0f,(gzReal)(50*sin(angle/2)+20));

		m_lamp->setTranslation((gzReal)(50*cos(5*angle)),5.0f,(gzReal)(50*sin(angle/2)+20));

	}

	virtual ~MyWindow()
	{
		if(m_input)
			delete m_input;

	}

	gzRefPointer<gzState>		m_state;

	gzLight *					m_spin;

	gzReal						angle;

	gzTransform	*				m_lamp;
	
	gzBool						m_inMousePress;

	gzNode						*model;

	gzScene						*m_scene;

	gzSimpleMouseViewControl	*m_input;

};

// Definition of a sample application
// The application provides an initialisation and an onIdle loop manager
// to do the refresh of the window

class WindowApp : public gzApplication
{
public:
		
	WindowApp():m_win(NULL),m_format(NULL)
	{
	}

    ~WindowApp()
	{
		if(m_win)
		{
			delete m_win;
		}

	}

    void Create(gzString filename)
	{
		m_format = new gzGraphicsFormat;

		m_format->useStencil(TRUE);

		m_win = new MyWindow(m_format,filename);

		gzPerspCamera *camera=gzDynamic_Cast<gzPerspCamera>(m_win->getCamera());

		camera->setPosition(30,30,150);
		camera->lookAt(0,0,0);
		camera->useInfiniteFarPlane(TRUE);

	}

	void onIdle()
	{
		if(m_win)
		{
			m_win->onIdle();

			m_win->triggerKeyEvaluation();

			if(!m_win->refreshWindow())	// Yield some time if no rendering
				gzSleep(30);

		}

	}

private:

	friend class MyWindow;
	
	MyWindow                *m_win;
	gzGraphicsFormat        *m_format;
};


int main(int argc, char *argv[])
{

	gzStartUpGizmo();	// Needed for some systems to install external graphics engines

	gzMessage::setMessageLevel(GZ_MESSAGE_MEM_DEBUG);

	gzCheckLibraryVersion();


	try
	{ 
		gzString	filename="script.3ds";

		gzLicense::notifyDefaultLicense();
	
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

						case 'M' :
							filename=gzString(*(++argv));
							argc--;
							break;

					}

				}
				argv++;
				argc--;
			}
		}

		// Make the application
		WindowApp app;
	      
		// Create the scene and the window
		app.Create(	filename);
		
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




