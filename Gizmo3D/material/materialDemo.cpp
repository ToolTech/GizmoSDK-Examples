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
// File			: materialDemo.cpp
// Module		: 
// Description	: Interactive material/light editor example
// Author		: Lisa Johansson
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
// LJH  040505	Created file    
//
// ******************************************************************************

#include "gzGizmo3DLibrary.h"	// Use this to include all Gizmo3D functionality

// Let the class be a subclass to gzApplication
// Gets an application with window management etc
class myTestApp: public gzApplication, public gzInputInterface
{
public:
	//Routes operators new and delete to gzApplication class.

	myTestApp();
	virtual ~myTestApp();
	gzVoid Create();
	gzVoid onIdle();
	virtual gzBool onKey (gzKeyValue key, gzKeyState keystate, const gzLongXY &mousePos, const gzLongXY &centerMousePos);

private:
	// Uses a smart pointer which automatically makes delete when necessary
	gzRefPointer<gzWindow> myWin;
	gzRefPointer<gzMaterial> myMat;
	gzFloat shininess;

	gzRefPointer<gzText>	myText;
	gzRefPointer<gzState>	myMaterialState;
	gzRefPointer<gzLight>	myLight;

	gzString mode;
	gzVec4 matAmb;
	gzVec4 matDif;
	gzVec4 matSpe;
	gzVec4 matEmi;
	gzVec4 ligAmb;
	gzVec4 ligDif;
	gzVec4 ligSpe;
};

myTestApp::myTestApp()		// Setup the initial values
{
	shininess = 16.0f;
	mode = "Ambient material";

	matAmb.set(0.16f , 0.16f, 0.16f , 0.5f);
	matDif.set(0.8f, 0.1f, 0.1f, 0.5f);
	matSpe.set(1.0f, 1.0f, 1.0f,1.0f);
	matEmi.set(0 , 0, 0, 0.5);

	ligAmb.set(0.0f, 0.0f, 0.0f, 1.0f);
	ligDif.set(1.0f, 1.0f, 1.0f, 1.0f); 
	ligSpe.set(1.0f, 1.0f, 1.0f, 1.0f); 
}

myTestApp::~myTestApp()		// Since we use gzRefPointer we don't need to destroy the pointers
{

}

// Creates the application
gzVoid myTestApp::Create()
{
	// necessary to get the debug output
	gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG);

	// loads a model and saves is as a node
	gzNode *myModel = gzDbManager::loadDB("torus2.3DS");
	
	// cast the model to a transform
	gzTransform * myTrans = gzDynamic_Cast<gzTransform>(myModel);
	
	// if there is a transform it can be modified with scaling, rotation and translation
	if(myTrans)
	{
		myTrans -> unitScale();
		myTrans -> rotate(gzVec3(0, 0, 1), 75);
		myTrans -> rotate(gzVec3(0, 1, 0), 15);
		myTrans -> scale((gzReal)(10),(gzReal)(10),(gzReal)(10));
	}

	// creates a material. The effect is only visible when a light source is added
	myMat = new gzMaterial;
	myMat->setAmbient(matAmb);
	myMat->setSpecular(matSpe);
	myMat->setDiffuse(matDif);
	myMat->setEmission(matEmi);
	myMat->setShininess(shininess);

	// creates a state with material mode enabled
	myMaterialState = new gzState;
    myMaterialState -> setMode(GZ_STATE_MATERIAL, GZ_STATE_ON );
	myMaterialState -> setOverride(GZ_STATE_MATERIAL, GZ_STATE_ON); // Used to override model definitions of material
	myMaterialState -> setMaterial(myMat);

	// creates a transform with rotation and translation. Adds a child and sets a state
	myTrans -> setState(myMaterialState);

	// The scene needs some light
	myLight = new gzLight;
	myLight -> setPosition(10.0, 3.0, 50.0);

	// creates an environment which makes it possible to add lighting to the scene
	gzEnvironment *myEnv = new gzEnvironment;
	myEnv -> addLight(myLight);
	myEnv -> addNode(myTrans);

	gzScene *myScene = new gzScene;
	myScene -> addNode(myEnv);

	myText=new gzText("Screen Text");		// Handle to text used to write messages to window
	myText->setColor(gzVec4(1,1,0,0.5));
	myText->use2DPosition(TRUE);
	myText->set2DPosition(10, -10);

	myScene -> addNode(myText);

	// adding a camera with perspective projection. The position is set, the viewing direction is
	// against the lookAt-point. The camera is connected to a scene.
	gzCamera *myCam = new gzPerspCamera;
	myCam -> setPosition(0.0, 0.0, 100.0);
	myCam -> lookAt(0, 0, 0);
	myCam -> setScene(myScene);

	// a window is created which shows the image (from the camera) on the screen.
	myWin = new gzWindow ("Material Demo");
	myWin->setCamera(myCam);

	// add some basic user interaction to the scene
	gzSimpleMouseViewControl * input = new gzSimpleMouseViewControl( myCam );
	myWin -> addInputInterface( input );

	// add our own keyboard input manager
	myWin -> addInputInterface( this );

	// adds a logger which collects information
	gzLogger logger("material_debug.log");

	// print the scene information into a the "material_debug.log" file
    myScene -> debug();
}

// function that updates for every frame
gzVoid myTestApp::onIdle()
{
	gzString outputString;

	outputString = mode + gzString("\n\n");
	
	outputString += gzString("Material properties \n");
	outputString += gzString("Shininess: ") + gzString(shininess) + gzString("\n");
	outputString += gzString("Ambient reflection: ") + gzString(matAmb.v1, "%.2f") + gzString(", ") + gzString(matAmb.v2, "%.2f") + gzString(", ") + gzString(matAmb.v3, "%.2f") + gzString(", ") + gzString(matAmb.v4, "%.2f") + gzString("\n");
	outputString += gzString("Diffuse reflection: ") + gzString(matDif.v1, "%.2f") + gzString(", ") + gzString(matDif.v2, "%.2f") + gzString(", ") + gzString(matDif.v3, "%.2f") + gzString(", ") + gzString(matDif.v4, "%.2f") + gzString("\n");
	outputString += gzString("Specular reflection: ") + gzString(matSpe.v1, "%.2f") + gzString(", ") + gzString(matSpe.v2, "%.2f") + gzString(", ") + gzString(matSpe.v3, "%.2f") + gzString(", ") + gzString(matSpe.v4, "%.2f") + gzString("\n");
	outputString += gzString("Emission: ") + gzString(matEmi.v1, "%.2f") + gzString(", ") + gzString(matEmi.v2, "%.2f") + gzString(", ") + gzString(matEmi.v3, "%.2f") + gzString(", ") + gzString(matEmi.v4, "%.2f") + gzString("\n\n");
		
	outputString += gzString("Light properties \n");
	outputString += gzString("Ambient color: ") + gzString(ligAmb.v1, "%.2f") + gzString(", ") + gzString(ligAmb.v2, "%.2f") + gzString(", ") + gzString(ligAmb.v3, "%.2f") + gzString(", ") + gzString(ligAmb.v4, "%.2f") + gzString("\n");
	outputString += gzString("Diffuse color: ")+ gzString(ligDif.v1, "%.2f") + gzString(", ") + gzString(ligDif.v2, "%.2f") + gzString(", ") + gzString(ligDif.v3, "%.2f") + gzString(", ") + gzString(ligDif.v4, "%.2f") + gzString("\n");
	outputString += gzString("Specular color: ") + gzString(ligSpe.v1, "%.2f") + gzString(", ") + gzString(ligSpe.v2, "%.2f") + gzString(", ") + gzString(ligSpe.v3, "%.2f") + gzString(", ") + gzString(ligSpe.v4, "%.2f") + gzString("\n");
	
	myText -> setText(outputString);

	// Checks if any button is pressed
	myWin -> triggerKeyEvaluation();

	// updates the window
	myWin -> refreshWindow();
}

gzBool myTestApp::onKey(gzKeyValue key, gzKeyState keystate, const gzLongXY &mousePos, const gzLongXY &centerMousePos)
{
	switch(key)
	{
		case GZ_KEY_ADD :
		{
			if (shininess < 128)
			{
				shininess += 0.5f;
				myMat->setShininess(shininess);
			}
		}
		break;
		
		case GZ_KEY_SUBTRACT :
		{
			if (shininess > 0)
			{
				shininess-=0.5f;
				myMat->setShininess(shininess);
			}
		}
		break;
		case ' ':	// Space pressed (Show graphics in wireframe mode)
		{
			if(keystate == GZ_KEY_STATE_PRESSED)
			{
				gzState *state=new gzState;

				state->setFrontPolygonMode(GZ_POLYGON_MODE_LINE);
				state->setBackPolygonMode(GZ_POLYGON_MODE_LINE);
				state->setMode(GZ_STATE_POLYGON_MODE,GZ_STATE_ON);
				state->setOverride(GZ_STATE_POLYGON_MODE,GZ_STATE_ON);
				state->setOverride(GZ_STATE_TEXTURE,GZ_STATE_OFF);
				state->setMode(GZ_STATE_GENERATE_DEBUG_INFO,GZ_STATE_ON);
				state->setDebugMode((gzStateDebugMode)(GZ_STATE_DEBUG_COLLECT_STATS));
					
				state->disableAllNonEnabledModes();

				gzState::setGlobalState(myWin->getContext(),state);
			}
			if(keystate == GZ_KEY_STATE_RELEASED)
			{
				gzState::setGlobalState(myWin->getContext(),myMaterialState);
			}
			break;
		}
		case 'c':	// c key pressed (toggle between modes)
		{
			if(keystate == GZ_KEY_STATE_PRESSED)
			{
				if (mode=="Ambient material")
					mode = "Diffuse material";
				else if (mode=="Diffuse material")
					mode = "Specular material";
				else if (mode=="Specular material")
					mode = "Material emission";
				else if (mode=="Material emission")
					mode = "Ambient light";
				else if (mode=="Ambient light")
					mode = "Diffuse light";
				else if (mode=="Diffuse light")
					mode = "Specular light";
				else if (mode=="Specular light")
					mode = "Ambient material";
       			}
			break;
		}
		case 'e':	// e key pressed. The red component is increased
		{
			if (mode=="Ambient material" && matAmb.v1 < 1)
			{
				matAmb.v1+=0.01f;

				myMat->setAmbient(matAmb.v1, matAmb.v2, matAmb.v3, matAmb.v4);
			}
			else if (mode=="Diffuse material" && matDif.v1 < 1)
			{
				matDif.v1+=0.01f;

				myMat->setDiffuse(matDif.v1, matDif.v2, matDif.v3, matDif.v4);
			}
			else if (mode=="Specular material" && matSpe.v1 < 1)
			{
				matSpe.v1+=0.01f;

				myMat->setSpecular(matSpe.v1, matSpe.v2, matSpe.v3, matSpe.v4);
			}
			else if (mode=="Material emission" && matEmi.v1 < 1)
			{
				matEmi.v1+=0.01f;

				myMat->setEmission(matEmi.v1, matEmi.v2, matEmi.v3, matEmi.v4);
			}
			else if (mode=="Ambient light" && ligAmb.v1 < 1)
			{
				ligAmb.v1+=0.01f;

				myLight->setAmbientColor(ligAmb.v1, ligAmb.v2, ligAmb.v3, ligAmb.v4);
			}
			else if (mode=="Diffuse light" && ligDif.v1 < 1)
			{
				ligDif.v1+=0.01f;

				myLight->setDiffuseColor(ligDif.v1, ligDif.v2, ligDif.v3, ligDif.v4);
			}
			else if (mode=="Specular light" && ligSpe.v1 < 1)
			{
				ligSpe.v1+=0.01f;

				myLight->setSpecularColor(ligSpe.v1, ligSpe.v2, ligSpe.v3, ligSpe.v4);
			}		
			break;
		}
		case 'd':	// d key pressed. The red component is decreased
		{
			if (mode=="Ambient material" && matAmb.v1 > 0)
			{
				matAmb.v1-=0.01f;

				myMat->setAmbient(matAmb.v1, matAmb.v2, matAmb.v3, matAmb.v4);
			}
			else if (mode=="Diffuse material" && matDif.v1 > 0)
			{
				matDif.v1-=0.01f;

				myMat->setDiffuse(matDif.v1, matDif.v2, matDif.v3, matDif.v4);
			}
			else if (mode=="Specular material" && matSpe.v1 > 0)
			{
				matSpe.v1-=0.01f;

				myMat->setSpecular(matSpe.v1, matSpe.v2, matSpe.v3, matSpe.v4);
			}
			else if (mode=="Material emission" && matEmi.v1 > 0)
			{
				matEmi.v1-=0.01f;

				myMat->setEmission(matEmi.v1, matEmi.v2, matEmi.v3, matEmi.v4);
			}
			else if (mode=="Ambient light" && ligAmb.v1 > 0)
			{
				ligAmb.v1-=0.01f;

				myLight->setAmbientColor(ligAmb.v1, ligAmb.v2, ligAmb.v3, ligAmb.v4);
			}
			else if (mode=="Diffuse light" && ligDif.v1 > 0)
			{
				ligDif.v1-=0.01f;

				myLight->setDiffuseColor(ligDif.v1, ligDif.v2, ligDif.v3, ligDif.v4);
			}
			else if (mode=="Specular light" && ligSpe.v1 > 0)
			{
				ligSpe.v1-=0.01f;

				myLight->setSpecularColor(ligSpe.v1, ligSpe.v2, ligSpe.v3, ligSpe.v4);
			}
			
			break;
		}
		case 'r':	// increases the green component
		{
			if (mode=="Ambient material" && matAmb.v2 < 1)
			{
				matAmb.v2+=0.01f;

				myMat->setAmbient(matAmb.v1, matAmb.v2, matAmb.v3, matAmb.v4);
			}
			else if (mode=="Diffuse material" && matDif.v2 < 1)
			{
				matDif.v2+=0.01f;

				myMat->setDiffuse(matDif.v1, matDif.v2, matDif.v3, matDif.v4);
			}
			else if (mode=="Specular material" && matSpe.v2 < 1)
			{
				matSpe.v2+=0.01f;

				myMat->setSpecular(matSpe.v1, matSpe.v2, matSpe.v3, matSpe.v4);
			}
			else if (mode=="Material emission" && matEmi.v2 < 1)
			{
				matEmi.v2+=0.01f;

				myMat->setEmission(matEmi.v1, matEmi.v2, matEmi.v3, matEmi.v4);
			}
			else if (mode=="Ambient light" && ligAmb.v2 < 1)
			{
				ligAmb.v2+=0.01f;

				myLight->setAmbientColor(ligAmb.v1, ligAmb.v2, ligAmb.v3, ligAmb.v4);
			}
			else if (mode=="Diffuse light" && ligDif.v2 < 1)
			{
				ligDif.v2+=0.01f;

				myLight->setDiffuseColor(ligDif.v1, ligDif.v2, ligDif.v3, ligDif.v4);
			}
			else if (mode=="Specular light" && ligSpe.v2 < 1)
			{
				ligSpe.v2+=0.01f;

				myLight->setSpecularColor(ligSpe.v1, ligSpe.v2, ligSpe.v3, ligSpe.v4);
			}		
			break;
		}

		case 'f':	// decreases the green component
		{
			if (mode=="Ambient material" && matAmb.v2 > 0)
			{
				matAmb.v2-=0.01f;

				myMat->setAmbient(matAmb.v1, matAmb.v2, matAmb.v3, matAmb.v4);
			}
			else if (mode=="Diffuse material" && matDif.v2 > 0)
			{
				matDif.v2-=0.01f;

				myMat->setDiffuse(matDif.v1, matDif.v2, matDif.v3, matDif.v4);
			}
			else if (mode=="Specular material" && matSpe.v2 > 0)
			{
				matSpe.v2-=0.01f;

				myMat->setSpecular(matSpe.v1, matSpe.v2, matSpe.v3, matSpe.v4);
			}
			else if (mode=="Material emission" && matEmi.v2 > 0)
			{
				matEmi.v2-=0.01f;

				myMat->setEmission(matEmi.v1, matEmi.v2, matEmi.v3, matEmi.v4);
			}
			else if (mode=="Ambient light" && ligAmb.v2 > 0)
			{
				ligAmb.v2-=0.01f;

				myLight->setAmbientColor(ligAmb.v1, ligAmb.v2, ligAmb.v3, ligAmb.v4);
			}
			else if (mode=="Diffuse light" && ligDif.v2 > 0)
			{
				ligDif.v2-=0.01f;

				myLight->setDiffuseColor(ligDif.v1, ligDif.v2, ligDif.v3, ligDif.v4);
			}
			else if (mode=="Specular light" && ligSpe.v2 > 0)
			{
				ligSpe.v2-=0.01f;

				myLight->setSpecularColor(ligSpe.v1, ligSpe.v2, ligSpe.v3, ligSpe.v4);
			}
			
			break;
		}
		case 't':	// Increases the blue component
		{
			if (mode=="Ambient material" && matAmb.v3 < 1)
			{
				matAmb.v3+=0.01f;

				myMat->setAmbient(matAmb.v1, matAmb.v2, matAmb.v3, matAmb.v4);
			}
			else if (mode=="Diffuse material" && matDif.v3 < 1)
			{
				matDif.v3+=0.01f;

				myMat->setDiffuse(matDif.v1, matDif.v2, matDif.v3, matDif.v4);
			}
			else if (mode=="Specular material" && matSpe.v3 < 1)
			{
				matSpe.v3+=0.01f;

				myMat->setSpecular(matSpe.v1, matSpe.v2, matSpe.v3, matSpe.v4);
			}
			else if (mode=="Material emission" && matEmi.v3 < 1)
			{
				matEmi.v3+=0.01f;

				myMat->setEmission(matEmi.v1, matEmi.v2, matEmi.v3, matEmi.v4);
			}
			else if (mode=="Ambient light" && ligAmb.v3 < 1)
			{
				ligAmb.v3+=0.01f;

				myLight->setAmbientColor(ligAmb.v1, ligAmb.v2, ligAmb.v3, ligAmb.v4);
			}
			else if (mode=="Diffuse light" && ligDif.v3 < 1)
			{
				ligDif.v3+=0.01f;

				myLight->setDiffuseColor(ligDif.v1, ligDif.v2, ligDif.v3, ligDif.v4);
			}
			else if (mode=="Specular light" && ligSpe.v3 < 1)
			{
				ligSpe.v3+=0.01f;

				myLight->setSpecularColor(ligSpe.v1, ligSpe.v2, ligSpe.v3, ligSpe.v4);
			}		
			break;
		}
		case 'g':	// decreases the blue component
		{
			if (mode=="Ambient material" && matAmb.v3 > 0)
			{
				matAmb.v3-=0.01f;

				myMat->setAmbient(matAmb.v1, matAmb.v2, matAmb.v3, matAmb.v4);
			}
			else if (mode=="Diffuse material" && matDif.v3 > 0)
			{
				matDif.v3-=0.01f;

				myMat->setDiffuse(matDif.v1, matDif.v2, matDif.v3, matDif.v4);
			}
			else if (mode=="Specular material" && matSpe.v3 > 0)
			{
				matSpe.v3-=0.01f;

				myMat->setSpecular(matSpe.v1, matSpe.v2, matSpe.v3, matSpe.v4);
			}
			else if (mode=="Material emission" && matEmi.v3 > 0)
			{
				matEmi.v3-=0.01f;

				myMat->setEmission(matEmi.v1, matEmi.v2, matEmi.v3, matEmi.v4);
			}
			else if (mode=="Ambient light" && ligAmb.v3 > 0)
			{
				ligAmb.v3-=0.01f;

				myLight->setAmbientColor(ligAmb.v1, ligAmb.v2, ligAmb.v3, ligAmb.v4);
			}
			else if (mode=="Diffuse light" && ligDif.v3 > 0)
			{
				ligDif.v3-=0.01f;

				myLight->setDiffuseColor(ligDif.v1, ligDif.v2, ligDif.v3, ligDif.v4);
			}
			else if (mode=="Specular light" && ligSpe.v3 > 0)
			{
				ligSpe.v3-=0.01f;

				myLight->setSpecularColor(ligSpe.v1, ligSpe.v2, ligSpe.v3, ligSpe.v4);
			}
			
			break;
		}
		case 'y':	// increases the alpha component
		{
			if (mode=="Ambient material" && matAmb.v4 < 1)
			{
				matAmb.v4+=0.01f;

				myMat->setAmbient(matAmb.v1, matAmb.v2, matAmb.v3, matAmb.v4);
			}
			else if (mode=="Diffuse material" && matDif.v4 < 1)
			{
				matDif.v4+=0.01f;

				myMat->setDiffuse(matDif.v1, matDif.v2, matDif.v3, matDif.v4);
			}
			else if (mode=="Specular material" && matSpe.v4 < 1)
			{
				matSpe.v4+=0.01f;

				myMat->setSpecular(matSpe.v1, matSpe.v2, matSpe.v3, matSpe.v4);
			}
			else if (mode=="Material emission" && matEmi.v4 < 1)
			{
				matEmi.v4+=0.01f;

				myMat->setEmission(matEmi.v1, matEmi.v2, matEmi.v3, matEmi.v4);
			}
			else if (mode=="Ambient light" && ligAmb.v4 < 1)
			{
				ligAmb.v4+=0.01f;

				myLight->setAmbientColor(ligAmb.v1, ligAmb.v2, ligAmb.v3, ligAmb.v4);
			}
			else if (mode=="Diffuse light" && ligDif.v4 < 1)
			{
				ligDif.v4+=0.01f;

				myLight->setDiffuseColor(ligDif.v1, ligDif.v2, ligDif.v3, ligDif.v4);
			}
			else if (mode=="Specular light" && ligSpe.v4 < 1)
			{
				ligSpe.v4+=0.01f;

				myLight->setSpecularColor(ligSpe.v1, ligSpe.v2, ligSpe.v3, ligSpe.v4);
			}		
			break;
		}
		case 'h':	// decreases the alpha component
		{
			if (mode=="Ambient material" && matAmb.v3 > 0)
			{
				matAmb.v3-=0.01f;

				myMat->setAmbient(matAmb.v1, matAmb.v2, matAmb.v3, matAmb.v4);
			}
			else if (mode=="Diffuse material" && matDif.v4 > 0)
			{
				matDif.v4-=0.01f;

				myMat->setDiffuse(matDif.v1, matDif.v2, matDif.v3, matDif.v4);
			}
			else if (mode=="Specular material" && matSpe.v4 > 0)
			{
				matSpe.v4-=0.01f;

				myMat->setSpecular(matSpe.v1, matSpe.v2, matSpe.v3, matSpe.v4);
			}
			else if (mode=="Material emission" && matEmi.v4 > 0)
			{
				matEmi.v4-=0.01f;

				myMat->setEmission(matEmi.v1, matEmi.v2, matEmi.v3, matEmi.v4);
			}
			else if (mode=="Ambient light" && ligAmb.v4 > 0)
			{
				ligAmb.v4-=0.01f;

				myLight->setAmbientColor(ligAmb.v1, ligAmb.v2, ligAmb.v3, ligAmb.v4);
			}
			else if (mode=="Diffuse light" && ligDif.v4 > 0)
			{
				ligDif.v4-=0.01f;

				myLight->setDiffuseColor(ligDif.v1, ligDif.v2, ligDif.v3, ligDif.v4);
			}
			else if (mode=="Specular light" && ligSpe.v4 > 0)
			{
				ligSpe.v4-=0.01f;

				myLight->setSpecularColor(ligSpe.v1, ligSpe.v2, ligSpe.v3, ligSpe.v4);
			}
			
			break;	
		}
	
	}
	return TRUE;
}

int main(int argc, char *argv[])
{
	gzStartUpGizmo();	// Needed for some systems to install external graphics engines

	// the try statement runs the program. If an exception is thrown this is caught
	// and reported by the catch statement. 
	try
	{
		// specifies graphics engine
		gzGraphicsEngine::useEngine(GZ_ENGINE_OPENGL);

		// makes it possible to load images with common formats
		gzInitializeDbManagers();
	
		myTestApp app;

		app.Create();

		app.run();
	}

	catch(gzBaseError &error)		// In case we get some exceptions
	{
		error.reportError();
	}

	gzShutDownGizmo();

	return 0;
}

