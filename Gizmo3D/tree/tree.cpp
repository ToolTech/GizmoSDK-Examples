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
// File			: tree.cpp
// Module		: 
// Description	: Test app for recursive tree sample
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
// AMO  020901  Created file    
//
// ******************************************************************************

//#define GZ_MEM_DEBUG	//to enable mem debug

#define USE_STATS

//#define USE_COMPILED_TREE

//#define USE_IMPOSTOR

// The general includes
#include "gzGizmo3DLibrary.h"


gzFloatXY xyfunk1(gzFloat in)
{
	return gzFloatXY(in*(1-in)*sin(in*GZ_PI*3)*5,in*(1-in)*sin(3*in*GZ_PI)*5);
}

gzFloatXY xyfunk2(gzFloat in)
{
	return gzFloatXY(in*(1-in)*sin(2*in*GZ_PI)*5,in*(1-in)*sin(5*in*GZ_PI));
}


gzGeometry *leaf()
{
	gzGeometry *leaf=new gzGeometry;

	gzArray<gzVec3> &coordinates=leaf->getCoordinateArray();
	gzArray<gzVec2> &tex=leaf->getTexCoordinateArray();
	gzArray<gzVec3> &norm=leaf->getNormalArray();


	coordinates+=	gzVec3(-80,0,0);
	coordinates+=	gzVec3(80,0,0);
	coordinates+=	gzVec3(80,160,0);
	coordinates+=	gzVec3(-80,160,0);

	tex+=	gzVec2(0,0);
	tex+=	gzVec2(1,0);
	tex+=	gzVec2(1,1);
	tex+=	gzVec2(0,1);

	norm+=gzVec3(0,0,1);

	leaf->setTexBind(GZ_BIND_ON);
	leaf->setNormalBind(GZ_BIND_PER_PRIM);
	leaf->setGeoPrimType(GZ_PRIM_QUADS);


	// Fix state on leaf

	gzMaterial *material=new gzMaterial;
	gzState *state=new gzState;

	state->setMaterial(material);
	//state->setBackMaterial(material);
	state->setMode(GZ_STATE_MATERIAL,GZ_STATE_ON);

	gzTexture *texture=new gzTexture;

	texture->setImage(gzImageManager::loadImage("leaf.png"));
	texture->setComponents(4);
	texture->setWrapS(GZ_TEXTURE_WRAP_REPEAT);
	texture->setWrapT(GZ_TEXTURE_WRAP_REPEAT);
	texture->useMipMaps(TRUE);
	texture->setMagFilter(GZ_TEXTURE_MAG_LINEAR);
	texture->setMinFilter(GZ_TEXTURE_MIN_LINEAR_MIPMAP_LINEAR);

	state->setMode(GZ_STATE_ALPHA_FUNC,GZ_STATE_ON);
	state->setAlphaFunc(GZ_ALPHA_FUNC_GREATER,0.5f);



	state->setTexture(texture);
	state->setMode(GZ_STATE_TEXTURE,GZ_STATE_ON);

	state->setMode(GZ_STATE_POLYGON_MODE,GZ_STATE_ON);
	state->setBackPolygonMode(GZ_POLYGON_MODE_FILL);
	state->setFrontPolygonMode(GZ_POLYGON_MODE_FILL);

	leaf->setState(state);

	return leaf;
}
// A test sample of a geometry
gzNode *buildTree()
{

	
	gzGeometry *base_1=new gzGeometryTube(50,5,11,6,GZ_TUBE_CONE,xyfunk1,5,5);

	//base_1->useDisplayList(TRUE);

	gzGeometry *base_2=new gzGeometryTube(50,5,11,6,GZ_TUBE_CONE,xyfunk2,5,5);

	//gzGeometry *branch_base=new gzGeometryTube(20,0.2,10,3,0,xyfunk2,10,10);


	//base_2->useDisplayList(TRUE);

	// Base branch

	gzSeedSwitch *base=new gzSeedSwitch;

	base->addNode(base_1);
	base->addNode(base_2);


	gzUInt32 depth=13;

	// Build tree

	gzRecursive *tree=new gzRecursive;

	tree->setMaxDepth(depth);

	//tree->useDistanceDepth(TRUE);
	//tree->setDistanceDepthEquation(1000,1);

	// tree=(base+T1*R1*S1*tree+T2*R2*S2*tree+SW*(T3*R3*S3*tree.....))

	// tree=base
	tree->addNode(base);

	tree->setMaxDepthNode(leaf());

	gzFloat rotation=180;

	gzFloat balance=0.6f;

	gzFloat spread=50;

	gzFloat offset=0;

	gzFloat add_rotation=108;

	// TRS1 - top striving high
	gzSeedTransform *pSeedTransform_1=new gzSeedTransform();
	pSeedTransform_1->setTranslation(0,48.5,0);
	pSeedTransform_1->setScale(0.85f,balance*1.5f,0.85f);
	pSeedTransform_1->setHPRSeedValue(TRUE,gzVec3(rotation+add_rotation,(1-balance)*spread,0),gzVec3(rotation+add_rotation,(1-balance)*spread,0));
	
	pSeedTransform_1->setStriveDirection(gzVec3(1,0,0));
	pSeedTransform_1->setStriveFactors(0.1f,0.1f,2.0f);
	
	pSeedTransform_1->addNode(tree);
	pSeedTransform_1->setRecursiveDepthScaleFactor(1);

	//pSeedTransform_1->setCacheSize(1000);

	// tree=base+T1*R1*S1*tree
	tree->addNode(pSeedTransform_1);

	
	// TRS2 - side winder
	gzSeedTransform *pSeedTransform_2=new gzSeedTransform();
	pSeedTransform_2->setTranslation(0,48,0);
	pSeedTransform_2->setScale((1-balance)*1.7,(1-balance)*1.7,(1-balance)*1.7);
	pSeedTransform_2->setHPRSeedValue(TRUE,gzVec3(rotation+add_rotation,-(offset+balance*spread),0),gzVec3(rotation+add_rotation,-(offset+balance*spread),0));
	
	pSeedTransform_2->setStriveDirection(gzVec3(1,-1,0));
	pSeedTransform_2->setStriveFactors(0.5);
	
	pSeedTransform_2->addNode(tree);
	pSeedTransform_2->setRecursiveDepthScaleFactor(1.1f);

	//pSeedTransform_2->setCacheSize(1000);

	

	tree->addNode(pSeedTransform_2);


	// Fix state on overall tree

	gzMaterial *material=new gzMaterial;
	gzState *state=new gzState;

	state->setMaterial(material);
	state->setMode(GZ_STATE_MATERIAL,GZ_STATE_ON);

	gzTexture *tex=new gzTexture;

	tex->setImage(gzImageManager::loadImage("bark.png"));
	tex->setWrapS(GZ_TEXTURE_WRAP_REPEAT);
	tex->setWrapT(GZ_TEXTURE_WRAP_REPEAT);
	tex->useMipMaps(TRUE);
	tex->setMagFilter(GZ_TEXTURE_MAG_LINEAR);
	tex->setMinFilter(GZ_TEXTURE_MIN_LINEAR_MIPMAP_LINEAR);


	state->setTexture(tex);
	state->setMode(GZ_STATE_TEXTURE,GZ_STATE_ON);

	/*state->setMode(GZ_STATE_GENERATE_DEBUG_INFO,GZ_STATE_ON);
	state->setDebugMode(GZ_STATE_DEBUG_SHOW_BSPHERE);*/

	// Seed starter
	gzSeedControl *topNode=new gzSeedControl;
	//topNode->setSeedControlType(GZ_SEED_FROM_TIME);

	topNode->setState(state);

	// Add recursive tree 

	topNode->addNode(tree);

	return topNode;
}


// And here comes the Java look alike app. It is a matter of taste.

class MyWindow : public gzWindow
{
public:
	MyWindow():gzWindow(gzString("Gizmo3D : ")+gzTime::now(TRUE).asString())
	{
			gzGraphicsFormat *ctx=new gzGraphicsFormat;

			ctx->useAlpha(TRUE);
			ctx->setAlphaBits(8);

			gzRefPointer<gzImageRender> imgRnd= new gzImageRender(this,1024,1024,ctx);

			gzImageRenderInterface::installFactory(imgRnd);

			// Create a scene to render -------------------------------------
			// We must have a scene that the camera can "look at"
			m_scene=new gzScene("Database Loader Example Scene");
			
			// Create some light environment group. This way we get lighting into the model
			// All stuff under this is lit with material s etc.
			gzEnvironment *group=new gzEnvironment;

			// Create a backface culling state
			// We don't want to render backfaces. this way we can "force" the geoms to be rendered without 
			// backfaces
			m_state=new gzState;

			m_state->setMode(GZ_STATE_POLYGON_MODE,GZ_STATE_ON);
			m_state->setBackPolygonMode(GZ_POLYGON_MODE_CULL);
			m_state->setFrontPolygonMode(GZ_POLYGON_MODE_FILL);

#ifdef USE_STATS			
			m_state->setMode(GZ_STATE_GENERATE_DEBUG_INFO,GZ_STATE_ON);
			m_state->setDebugMode(GZ_STATE_DEBUG_COLLECT_STATS);
#endif
			
			

			m_state->disableAllNonEnabledModes();	// We need this for the top global state

			gzState::setGlobalState(getContext(),m_state);
			

			// -------------------- lights --------------------------------

			// Add some dynamic light
			m_spin=new gzLight;						// This is the lamp that we move around
			m_spin->setSpecularColor(0.2f,0.2f,0.2f);
			m_spin->setDiffuseColor(0.5f,0.5f,0.5f);
			m_spin->setAmbientColor(0.3f,0.3f,0.3f);
			m_spin->setPosition(26,10,90);

			// m_spin->invertShadowLight(TRUE); // Funny to invert shadow to be a spot instead..

			group->addLight(m_spin);

			// ------------------------ some global settings --------------

			gzEnvironment::setTwoSideLighting(getContext(),TRUE);


			// Add a sample recursive geometry

			
			gzNode *pTree=buildTree();

			gzTransform *trx=new gzTransform;
			gzCompileGroup *cg=new gzCompileGroup;

#ifdef USE_COMPILED_TREE
			cg->addNode(pTree);
			trx->addNode(cg);
#elif defined USE_IMPOSTOR


			gzImpostor *imp=new gzImpostor;

			imp->addNode(pTree);

			imp->setForceLocalIncludeBoundary(TRUE);

			
			for(gzUInt32 i=0;i<100;i++)
			{
				for(gzUInt32 j=0;j<100;j++)
				{
					gzTransform *tr=new gzTransform;

					tr->setForceLocalIncludeBoundary(TRUE);

					tr->setTranslation(i*300+300*gzRandom(),30*gzRandom(),j*300+400*gzRandom());
					tr->setHPR(270*gzRandom(),0.3*gzRandom(),0);

					tr->addNode(imp);
					trx->addNode(tr);
				}
			}

			//trx->addNode(imp);
			//trx->addNode(pTree);
#else
			trx->addNode(pTree);
#endif

			trx->scale(0.2f);
			
			group->addNode(trx);

			
			// Add a lamp to show position of light

			m_lamp=new gzTransform;

			gzGeometry *sphere=new gzGeometrySphere(1 , 20 ,gzVec4(1.0,1.0,1.0,1.0) );

			//sphere->useDisplayList(TRUE);

			//sphere->convertToIndexed();

			m_lamp->addNode( sphere );

			// Set mouse press state
			m_inMousePress=FALSE;


			// And add the group to the scene as well
			m_scene->addNode(group);

			// And add the lamp
			m_scene->addNode(m_lamp);

						
			// Add some textual info about frame rates
			m_text=new gzText("Screen Text");

			m_text->setColor(gzVec4(1,1,0,0.5));
			m_text->use2DPosition(TRUE);
			m_text->set2DPosition(10, -10);
			
			m_scene->addNode(m_text);

			// Now we want to look at the scene. Grab the default perspective camera from the window and set the scen as the
			// active one.
			getCamera()->setScene(m_scene);
			getCamera()->setNearClipPlane(1);
			getCamera()->setFarClipPlane(2000);

			// Lets add some movement to the scene

			m_input=new gzSimpleMouseViewControl(getCamera());

			addInputInterface(m_input);

			// Hmm. trivial
			setBackground(0.5f,0.5f,0.5f,1.0f);

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


	gzVoid onIdle()
	{
		// Hmm. Now i remember. The spinning lamp..

		angle+=0.003f;

		if(angle>2*GZ_PI)
			angle-=(gzFloat)(2*GZ_PI);

		m_spin->setPosition((gzFloat)(50*cos(5*angle)),5.0f,(gzFloat)(50*sin(angle/2)+20));

		m_lamp->setTranslation((gzFloat)(50*cos(5*angle)),5.0f,(gzFloat)(50*sin(angle/2)+20));

		gzSeedTransform::setGlobalStriveStrength((1+(0.3*cos(7*angle)*sin(3*angle)+0.4*cos(5*angle)+0.2*sin(8*angle)+0.1*sin(21*angle) ))*0.5);

		//gzSleep(10);
	}

	virtual ~MyWindow()
	{
		if(m_input)
			delete m_input;
	}

	gzRefPointer<gzState> m_state;

	gzLight *			m_spin;

	gzText *			m_text;

	gzFloat				angle;

	gzTransform	*		m_lamp;
	
	gzBool				m_inMousePress;

	gzScene				*m_scene;

	gzSwitch			*m_switch;

	gzSimpleMouseViewControl	*m_input;

};


class WindowApp : public gzApplication
{
public:
		
	WindowApp():m_win(NULL),m_format(NULL),m_heading(0),m_pitch(0),m_position(30,30,500)
	{
	};

    ~WindowApp()
	{
	}

    void Create()
	{
		m_win = new MyWindow;

		gzPerspCamera *camera=gzDynamic_Cast<gzPerspCamera>(m_win->getCamera());

		camera->setHPR(m_heading,m_pitch,0);
		camera->setPosition(m_position.v1,m_position.v2,m_position.v3);
		camera->lookAt(0,25,0);
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

			gzDouble delay=gzDynamicLoader::getWorkDelay();

			gzUInt32 pending=gzDynamicLoader::getPendingWork();

			gzString outputString;

			if(!gzDynamicLoader::isWorking())
			{
				outputString=gzString("FrameRate:")+gzString(m_win->getFrameRate());
				delay=0;
			}
			else
				outputString=gzString("FrameRate:")+gzString(m_win->getFrameRate())+"\n\nDynamic Loading -- Delay("+gzString(delay,"%.2f")+gzString("s) Pending(") + gzString((gzInt64)gzDynamicLoader::getPendingWork())+ gzString(")");

#ifdef USE_STATS
			outputString+=(gzString("\n\n\n")+gzGraphicsStats(m_win->getContext()));
#endif

			
			
			if((delay>0.8) || (pending>100))
			{
				gzDynamicLoader::boostLoaderWork();
				outputString+="\n\n BOOSTED LOADING !!";
			}
			else
				gzDynamicLoader::unBoostLoaderWork();

			

			m_win->m_text->setText(outputString);

		}

	}

private:

	friend class MyWindow;
	
	gzRefPointer<MyWindow> m_win;
	gzGraphicsFormat       m_format;

	gzFloat             m_heading;
	gzFloat             m_pitch;
	gzVec3              m_position;

};

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
