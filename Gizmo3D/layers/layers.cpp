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
// File			: layers.cpp
// Module		: 
// Description	: Test app for image rendering to a texture
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
// AMO  051212  Created file    
//
// ******************************************************************************

#include "gzGizmo3DLibrary.h"


/***********************************************************************

This node action is used to clear the depth before each render pass of a certain node.
As the nodes are sorted in each pass we need to create seprate passes and this is done by using
the gzRecursive node and set the renderOnRecursion to TRUE

************************************************************************/

class DepthClearAction : public gzNodeAction
{
public:
	DepthClearAction(const gzString & name=GZ_EMPTY_STRING):gzNodeAction(name)
	{
	}

	virtual ~DepthClearAction()
	{
	}

	virtual gzVoid onAction(gzNodeActionEvent action , gzContext *context , gzNodeActionProvider *trigger ,gzTraverseAction *actionclass ,gzVoid *userdata)
	{
		switch(action)
		{
			case GZ_NODE_ACTION_BEFORE_PRE_TRAVERSE:
				{
					gzClearDepth(1.0);
					gzClear(GZ_DEPTH_BUFFER_BIT);
				}
				break;
		}
	}
};

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
// AMO	051212	Created 
//									
//******************************************************************************
gzScene *createScene()
{
	// Load a node with suggested animation
	gzNode *node=gzDbManager::loadDB("../dbviewer/gizmo.3ds",GZ_EVALUATE_EXTENSION,GZ_DB_FLAGS_USE_ANIMATION|GZ_DB_FLAGS_USE_MIPMAPS|GZ_DB_FLAGS_USE_PHONG_LIGHT);

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
// AMO	051212	Created 
//									
//******************************************************************************
class MyWindow : public gzWindow
{
public:
	MyWindow():gzWindow("Layer Rendering")
	{

		// Create a scene with a group of three recursive nodes that have separate transforms,
		// but with a shared child tree.
		
		gzScene *scene=new gzScene;

		gzEnvironment *env=new gzEnvironment;

		scene->addNode(env);

		gzLight *light=new gzLight;
		light->setPosition(0,0,100);
		env->addLight(light);

		// Set up a simple animation control

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

		// Set up the three recursive nodes

		gzRecursive *rec1=new gzRecursive;
		gzRecursive *rec2=new gzRecursive;
		gzRecursive *rec3=new gzRecursive;

		// and enable them to render on separate passes

		rec1->renderOnRecursion(TRUE);
		rec2->renderOnRecursion(TRUE);
		rec3->renderOnRecursion(TRUE);

		// and let them transfer light recursively

		rec1->transferLightStates(TRUE);
		rec2->transferLightStates(TRUE);
		rec3->transferLightStates(TRUE);

		env->addNode(rec1);
		env->addNode(rec2);
		env->addNode(rec3);


		// Create a shared tree

		gzNode *sub_scene=createScene();

		// and some states wto control render order of the three top nodes.

		gzState *sceneorder=new gzState;
		sceneorder->setActionStage(GZ_ACTION_STAGE_SCENE_GRAPH_ORDER);
		sceneorder->setMode(GZ_STATE_ACTION_STAGE,GZ_STATE_ON);

		gzState *defaultorder=new gzState;
		defaultorder->setMode(GZ_STATE_ACTION_STAGE,GZ_STATE_OFF);
		sub_scene->setState(defaultorder);

		// add the action and the transforms

		DepthClearAction *action=new DepthClearAction;

		gzTransform *tr1=new gzTransform("Tr1");
		tr1->addNode(sub_scene);
		action->attach(tr1);
		tr1->setState(sceneorder);
		
		gzTransform *tr2=new gzTransform("Tr2");
		tr2->addNode(sub_scene);
		action->attach(tr2);
		tr2->setState(sceneorder);

		gzTransform *tr3=new gzTransform("Tr3");
		tr3->addNode(sub_scene);
		action->attach(tr3);
		tr3->setState(sceneorder);

		rec1->addNode(tr1);
		rec2->addNode(tr2);
		rec3->addNode(tr3);

		tr1->setScale(1.0f);
		tr2->setScale(0.8f);
		tr3->setScale(0.5f);

		// Normal camera stuff...

		gzCamera *cam=getCamera();

		cam->setScene(scene);

		cam->setPosition(0,0,150);

		cam->lookAt(0,0,0);

		setSwapInterval(1);

		setBackground(0.5f,0.5f,0.5f);
	};


private:

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
// AMO	051212	Created 
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



