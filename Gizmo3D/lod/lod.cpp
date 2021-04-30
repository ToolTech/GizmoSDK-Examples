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
// File			: lod.cpp
// Module		: 
// Description	: Test app for a sample lod
// Author		: Lisa Johansson          
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
// LJH  040322  Created file    
//
// ******************************************************************************

#include "gzGizmo3DLibrary.h"

// *******************************************************************************
// Defines the window class

class MyWin : public gzWindow
{
public:
		
	MyWin();

	virtual ~MyWin(){};

	void onIdle();

	gzRefPointer<gzText>		screenText;

	gzRefPointer<gzPerspCamera>	cam;

	gzRefPointer<gzLod>			lod;
};


MyWin::MyWin():gzWindow("Lod Test")
{

	// Create a scene to render
	gzScene * scene=new gzScene();

	scene -> addNode(new gzGeometryAxis(10,1));

	gzEnvironment * env = new gzEnvironment();

	gzState * state=new gzState;
	
	// the state removes the backfaces in the rendering
	state -> setMode(GZ_STATE_POLYGON_MODE,GZ_STATE_ON);
	
	state -> setBackPolygonMode(GZ_POLYGON_MODE_CULL);
	
	state -> setFrontPolygonMode(GZ_POLYGON_MODE_FILL);

	env -> setState(state);

	gzNode * gizmo1 = gzDbManager::loadDB("lodmodel1.3DS",GZ_EVALUATE_EXTENSION,GZ_DB_FLAGS_USE_MIPMAPS);

	gzTransform * gt1 = gzDynamic_Cast<gzTransform>(gizmo1);

	if(gt1)
	{
		gt1 -> unitScale();

		gt1 -> scale((gzFloat)(2),(gzFloat)(2),(gzFloat)(2));

	}

	gzNode * gizmo2 = gzDbManager::loadDB("lodmodel2.3DS",GZ_EVALUATE_EXTENSION,GZ_DB_FLAGS_USE_MIPMAPS);

	gzTransform * gt2 = gzDynamic_Cast<gzTransform>(gizmo2);

	if(gt2)
	{
		gt2 -> unitScale();

		gt2 -> scale((gzFloat)(2),(gzFloat)(2),(gzFloat)(2));

	}

	gzNode * gizmo3 = gzDbManager::loadDB("lodmodel3.3DS",GZ_EVALUATE_EXTENSION,GZ_DB_FLAGS_USE_MIPMAPS);

	gzTransform * gt3 = gzDynamic_Cast<gzTransform>(gizmo3);

	if(gt3)
	{
		gt3 -> unitScale();

		gt3 -> scale((gzFloat)(2),(gzFloat)(2),(gzFloat)(2));

	}

	lod = new gzLod;

	lod -> addNode(gizmo1);

	lod -> addNode(gizmo2);

	lod -> addNode(gizmo3);

	//lod->setLOD(gzUInt32 index , gzBool enable , gzFloat min = 0 , gzFloat max = 1, gzFloat fade = 0);

	lod->setLOD(0, TRUE , 0, 20, 0,30);

	lod->setLOD(1, TRUE, 20, 40, 10,40);

	lod->setLOD(2, TRUE, 40, 100, 20,0);

	env -> addNode(lod);
	
	scene -> addNode (env);

	//
	// Add some text to the scene
	//

	screenText = new gzText("Screen Text");

	screenText -> setColor(gzVec4(1,1,0,0.5));
	
	screenText -> use2DPosition(TRUE);
	
	screenText -> set2DPosition(10, -10);
			
	scene -> addNode(screenText);

	cam = new gzPerspCamera;

	cam -> setPosition(0,0,20);

	cam -> setScene(scene);

	cam -> setNearClipPlane(1);

	cam -> setFarClipPlane(100000);

	setCamera(cam);

	gzSimpleMouseViewControl * myInput = new gzSimpleMouseViewControl( cam );
	
	addInputInterface( myInput );

	refreshWindow();
}


void MyWin::onIdle()
{
	triggerKeyEvaluation();

	refreshWindow();
	
};


// ******************************************************************************
// Definition of a lod application
// The application provides an initialisation and an onIdle loop manager
// to do the refresh of the window


class LodApp : public gzApplication
{
public:
		
	LodApp();

	~LodApp();

private:

	friend class MyWin;

	gzRefPointer<MyWin>	m_win;         // the window   
	
	void onIdle();

};

LodApp::LodApp()
{
		m_win = new MyWin();
}

LodApp::~LodApp()
{
	// When the lod application is deleted the smart pointer for the window is
	// automatically removed
}

void LodApp::onIdle()
{
	if(m_win)
	{
		m_win -> onIdle();

		m_win -> triggerKeyEvaluation();

		gzString outputString;

		outputString = gzString("FrameRate:")+gzString(m_win->getFrameRate());

		outputString = outputString + gzString("\n\nCamera position: ") + gzString(m_win->cam->getPosition().v1) + gzString(", ") + gzString(m_win->cam->getPosition().v2) + gzString(", ") + gzString(m_win->cam->getPosition().v3);
	
		outputString = outputString + gzString("\n\nHeading: ") + gzString(m_win->cam->getHPR().v1) + gzString("\nPitch: ") + gzString(m_win->cam->getHPR().v2) + gzString("\nRoll: ") + gzString(m_win->cam->getHPR().v3);
	
		gzVec3 lodCenter = m_win->lod->getCenter();

		gzDouble lodDistance = sqrt((m_win->cam->getPosition().v1 - lodCenter.v1)*(m_win->cam->getPosition().v1 - lodCenter.v1) + (m_win->cam->getPosition().v2 - lodCenter.v2)*(m_win->cam->getPosition().v2 - lodCenter.v2) + (m_win->cam->getPosition().v3 - lodCenter.v3)*(m_win->cam->getPosition().v3 - lodCenter.v3));
		
		outputString = outputString + gzString("\n\nLodDistance: ") + gzString(lodDistance);

		m_win->screenText->setText(outputString);

	}

}


// ******************************************************************************
// The main method

int main(int argc, char *argv[])
{
	gzStartUpGizmo();	// Needed for some systems to install external graphics engines
	
	try
	{       
		// Set the appropriate graphics engine API
		gzGraphicsEngine::useEngine(GZ_ENGINE_OPENGL);

		gzInitializeDbManagers();

		// Make the application
		LodApp app;

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
