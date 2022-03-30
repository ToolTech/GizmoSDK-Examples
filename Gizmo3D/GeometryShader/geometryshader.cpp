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
// File			: geometryshader.cpp
// Module		: 
// Description	: Test app to show geometry shader usage
// Author		: Anders ModÈn          
// Product		: Gizmo3D 2.11.76
//              
// Copyright © 2003- Saab Training Systems AB, Sweden	
//                      
// NOTE:	Gizmo3D is a high performance 3D Scene Graph and effect visualisation 
//			C++ toolkit for Linux, Mac OS X, Windows (Win32) and IRIXÆ for  
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
	gzGeometry *geom=new gzGeometry;
	
	gzArray<gzVec3> coords;
	
	coords+=gzVec3(0,0,0);
	
	geom->setGeoPrimType(GZ_PRIM_POINTS);
	geom->setCoordinateArray(coords);
	
	gzArray<gzVec4> colors;
	colors+=gzVec4(1,1,1,1);
	
	geom->setColorArray(colors);
	geom->setColorBind(GZ_BIND_OVERALL);
	
	geom->useRadius(FALSE);
	
	return geom;
}

//------------------------------------- Vertex Program -----------------------------------------

const gzString vertexProgram=

"	varying vec4 p1,p2,p3,p4;												\n"
"	uniform float gz_RenderTime;											\n"
"																			\n"
"	void main()																\n"
"	{																		\n"
"																			\n"
"		p1=gl_ModelViewProjectionMatrix*(gl_Vertex+vec4(-1,-1,0,0));		\n"
"		p2=gl_ModelViewProjectionMatrix*(gl_Vertex+vec4(1,-1,0,0));			\n"
"		p3=gl_ModelViewProjectionMatrix*(gl_Vertex+vec4(1,1,0,0));			\n"
"		p4=gl_ModelViewProjectionMatrix*(gl_Vertex+vec4(-1,1,0,0));			\n"
"																			\n"
"		gl_Position=ftransform();											\n"
"																			\n"
"		float val=abs(fract(gz_RenderTime)*2.0-1.0);						\n"
"		gl_FrontColor=vec4(val);											\n"
"	}																		\n"
;

//------------------------------------- Geometry Program ------------------------------------------

const gzString geometryProgram=

"	#version 120													\n"
"	#extension GL_EXT_geometry_shader4 : enable						\n"
"																	\n"
"	varying in vec4 p1[],p2[];										\n"
"	varying in vec4 p3[],p4[];										\n"
"																	\n"
"	void main()														\n"
"	{																\n"
"		gl_FrontColor = gl_FrontColorIn[0];							\n"
"		gl_Position = p1[0];										\n"
"		gl_TexCoord[0] = vec4(0,0,1,1);								\n"
"		EmitVertex();												\n"
"																	\n"
"		gl_FrontColor = gl_FrontColorIn[0];							\n"
"		gl_Position = p2[0];										\n"
"		gl_TexCoord[0] = vec4(1,0,1,1);								\n"
"		EmitVertex();												\n"
"																	\n"
"		gl_FrontColor = gl_FrontColorIn[0];							\n"
"		gl_Position = p4[0];										\n"
"		gl_TexCoord[0] = vec4(0,1,1,1);								\n"
"		EmitVertex();												\n"
"																	\n"
"		gl_FrontColor = gl_FrontColorIn[0];							\n"
"		gl_Position = p3[0];										\n"
"		gl_TexCoord[0] = vec4(1,1,1,1);								\n"
"		EmitVertex();												\n"
"	}																\n"
;

//------------------------------------- Fragment Program ----------------------------------------

const gzString fragmentProgram=

"	void main()														\n"
"	{																\n"
"		gl_FragColor=gl_Color;										\n"
"	}																\n"
;



// And here comes the Java look alike app. It is a matter of taste.

class MyWindow : public gzWindow
{
public:
	MyWindow(gzGraphicsFormat *format):gzWindow(gzString("Gizmo3D : ")+gzTime::now(TRUE).asString(),NULL,format,FALSE)
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
			

			model=createModel();

			if(model)	// We got a model
			{
			
					gzState *state;

					state=model->getState();

					if(!state)
						state=new gzState;


					// Set the vertex program

					gzGFXProgram *program_v=new gzGFXProgram;

					program_v->setProgramScript(vertexProgram);

					state->setGFXProgram(GZ_GFX_VERTEX_PROGRAM,program_v);

					
					// Set the geometry program

					gzGFXProgram *program_g=new gzGFXProgram;

					program_g->setProgramScript(geometryProgram);
					
					program_g->setAttribute(GZ_GFX_PROGRAM_SETTINGS,GZ_GFX_GEOMETRY_PROGRAM_INPUT_TYPE, GZ_POINTS);
					
					program_g->setAttribute(GZ_GFX_PROGRAM_SETTINGS,GZ_GFX_GEOMETRY_PROGRAM_OUTPUT_TYPE, GZ_TRIANGLE_STRIP);
					
					program_g->setAttribute(GZ_GFX_PROGRAM_SETTINGS,GZ_GFX_GEOMETRY_PROGRAM_VERTICES_OUT, 4);

					state->setGFXProgram(GZ_GFX_GEOMETRY_PROGRAM,program_g);


					// Set the fragment program

					gzGFXProgram *program_f=new gzGFXProgram;

					program_f->setProgramScript(fragmentProgram);

					state->setGFXProgram(GZ_GFX_FRAGMENT_PROGRAM,program_f);
			
			
					// Enable GFX programs

					state->setMode(GZ_STATE_GFX_PROGRAM,GZ_STATE_ON);

					model->setState(state);


			}

			// Translate the model and scale it

			gzTransform *trans=new gzTransform;

			trans->addNode(model);
				
			trans->unitScale();

			trans->scale(10.0f,10.0f,10.0f);

			group->addNode(trans);
			
			// And add the group to the scene as well
			m_scene->addNode(group);

						
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

	};


	virtual ~MyWindow()
	{
		if(m_input)
			delete m_input;

	}

	gzRefPointer<gzState>		m_state;

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

    void Create()
	{
		m_format = new gzGraphicsFormat;

		m_format->useStencil(TRUE);

		m_win = new MyWindow(m_format);

		gzPerspCamera *camera=gzDynamic_Cast<gzPerspCamera>(m_win->getCamera());

		camera->setPosition(30,30,150);
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
};


int main(int argc, char *argv[])
{

	gzStartUpGizmo();	// Needed for some systems to install external graphics engines

	gzMessage::setMessageLevel(GZ_MESSAGE_MEM_DEBUG);

	gzCheckLibraryVersion();


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




