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
// File			: viewer.cpp
// Module		: 
// Description	: Test app shadow database viewer
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
// AMO  020901  Created file    
//
// ******************************************************************************

//#define GZ_MEM_DEBUG	//to enable mem debug

#define USE_STATS

// The general include
#include "gzGizmo3DLibrary.h"

// And here comes the Java look alike app. It is a matter of taste.
gzBaseGeometry *createPlane(gzUInt32 offset)
{
				gzViewPlane *plane=new gzViewPlane();

				plane->setTexCoordinates(gzVec2(0+offset*0.28,0+offset*0.28), gzVec2(3+offset*0.28,3+offset*0.28));

				gzState *transp=new gzState;

				plane->setState(transp);

				transp->setTransparency(0.2f);

				if(offset)
				{

					gzImage *image=gzImageManager::loadImage("brus.dds");

					gzTexture *tex=new gzTexture;

					tex->setImage(image);
					tex->setWrapS(GZ_TEXTURE_WRAP_REPEAT);
					tex->setWrapT(GZ_TEXTURE_WRAP_REPEAT);


					transp->setTexture(tex);
					transp->setMode(GZ_STATE_TEXTURE,GZ_STATE_ON);

					gzTexEnv *env=new gzTexEnv;

					env->setMode(GZ_TEXENV_MODE_BLEND);

					transp->setTexEnv(env);
					transp->setMode(GZ_STATE_TEXENV,GZ_STATE_ON);
				}

				return plane;

}

class MyWindow : public gzWindow
{
public:
	MyWindow(gzInt32 swap,gzBool useLodSmooth,gzDouble fadeDist,gzBool useFog,gzBool useRoi,gzBool softShadow,gzBool debug, gzGraphicsFormat *format , gzBool useShadow, gzSerializeAdapterFlags flags , gzString filename , gzReal scale,gzBool skyLayer):gzWindow(gzString("Gizmo3D : ")+gzTime::now(TRUE).asString(),NULL,format,FALSE)
	{
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
			m_state->setDebugMode((gzStateDebugMode)(GZ_STATE_DEBUG_COLLECT_STATS /*|GZ_STATE_DEBUG_SHOW_BSPHERE*/));
#endif							   
			
			

			m_state->disableAllNonEnabledModes();	// We need this for the top global state

			gzState::setGlobalState(getContext(),m_state);
			

			// ---------------------- DB loading ------------------------------

			// Create a object node from a generic db file
			
			gzDouble secs=gzTime::systemSeconds();	// Some timing

			// Load the DB
			
			gizmo=gzDbManager::loadDB(filename,GZ_EVALUATE_EXTENSION,flags 
																		//	| GZ_DB_FLAGS_FLIP_DDS_IMAGES
																		//	| GZ_DB_FLAGS_USE_MIPMAPS
																		//	| GZ_DB_FLAGS_SORT_BINARY_ALPHA
																		//	| GZ_DB_FLAGS_GENERATE_LIGHT_MATERIAL
																		//	| GZ_DB_FLAGS_USE_INDEXED_GEOMETRY
																		//	| GZ_DB_FLAGS_USE_DISPLAYLIST_GEOMETRY
																		//	| GZ_DB_FLAGS_USE_ANIMATION
																		//	| GZ_DB_FLAGS_COMPRESS_ON_GPU 
																		//	| GZ_DB_FLAGS_USE_BUFFERED_GEOMETRY 
																		//	| GZ_DB_FLAGS_DISPOSE_BUFFERED_GEOMETRY 
																		);

			// How long
			gzDouble duration=gzTime::systemSeconds()-secs;

			gzNodeOptimizer opt;		// We let the optimizer take away redundant geometry etc.

			gizmo=opt.optimize(gizmo);

			// --------------------- animation ------------------------------

			if(gizmo)
			{
				if(gizmo->isOfType(gzFrame::getClassType()))	// Check if it is animated
				{
					gzFrame *frame=(gzFrame *)gizmo;

					frame->setFrameDuration(0.05);
					frame->startFrameCounter();
				}
			}

			// -------------------- lights --------------------------------

			// Add some dynamic light
			m_spin=new gzLight;						// This is the lamp that we move around
			m_spin->setSpecularColor(0.5f,0.5f,0.5f);
			m_spin->setDiffuseColor(0.7f,0.7f,0.7f);
			m_spin->setAmbientColor(0.2f,0.2f,0.2f);
			m_spin->setPosition(26,10,90);

			group->addLight(m_spin);

			// ------------------------ some global settings --------------

			gzEnvironment::setTwoSideLighting(getContext(),TRUE);

			gzEnvironment::setLocalViewer(getContext(),TRUE);


			m_lodFactor=1;
			// How much do we want to scale LOD details
			gzLod::setLodFactor(getContext(),m_lodFactor);

			// We want to enable an automatic calc of smoothness in lods
			if(useLodSmooth)
				gzLod::setGenericFadeDistance(getContext(),(gzFloat)fadeDist);

	
			if(skyLayer)
			{
				gzSkyBox *box=new gzSkyBox;

	//			box->setImageSide(GZ_SKYBOX_EAST,gzImageManager::loadImage("east.bmp"));
	//			box->setImageSide(GZ_SKYBOX_SOUTH,gzImageManager::loadImage("south.bmp"));
	//			box->setImageSide(GZ_SKYBOX_WEST,gzImageManager::loadImage("west.bmp"));
	//			box->setImageSide(GZ_SKYBOX_NORTH,gzImageManager::loadImage("north.bmp"));
	//			box->setImageSide(GZ_SKYBOX_SKY,gzImageManager::loadImage("sky.bmp"));
	//			box->setImageSide(GZ_SKYBOX_GROUND,gzImageManager::loadImage("ground.bmp"));

				//box->setImageSide(GZ_SKYBOX_SKY,gzImageManager::loadImage("clouds.png"));

				//box->setHorizonAngle(20);

				//box->setGroundPoint(0,-1000,0);

				//box->setStandardHeight(2000);

				m_scene->addNode(box);


				gzSkyLayer *layer;

				layer=new gzSkyLayer;

				gzImage *clouds=gzImageManager::loadImage("light_cloud.dds",GZ_EVALUATE_EXTENSION,GZ_IMAGE_FLAGS_NO_ALTERNATE_IMAGE_EXT);

				layer->setLayerImage(clouds,100000,100000,5000,600,300);

				m_scene->addNode(layer);

				
				layer=new gzSkyLayer;

				layer->setLayerImage(clouds,70000,50000,6000,0,500);

				m_scene->addNode(layer);

			}


			// -------- Ajustments for viewer ----------------------------

			// Lets make a node that scales down the loaded object to a unit size

			if(gizmo && gizmo->isOfType(gzRoi::getClassType()))
			{
				// We use ROI. model is scaled in metres

				gzTransform *trans=new gzTransform;

				trans->addNode(gizmo);

				gzDouble x=0,y=0,z=0;

				if(gzHasNodeDbInfo(gizmo,GZ_DB_INFO_DB_SIZE))
				{
					gzAttribute_DBSize dbinfo=gzDynamic_Cast<gzAttribute_DBSize>(gzGetNodeDbInfo(gizmo,GZ_DB_INFO_DB_SIZE));

					x=(dbinfo.minX+dbinfo.maxX)/2;

					y=(dbinfo.minY+dbinfo.maxY)/2;

					z=(dbinfo.minZ+dbinfo.maxZ)/2;

					trans->setTranslation(-(gzReal)x,-(gzReal)y,-(gzReal)z);
				}

				
				// Lets scale it a bit

				if(gzHasNodeDbInfo(gizmo,GZ_DB_INFO_DB_MAX_LOD_RANGE))
				{
					gzFloat range=1/(gzFloat)gzGetNodeDbInfo(gizmo,GZ_DB_INFO_DB_MAX_LOD_RANGE).getNumber();

					trans->scale(range);
				}

				trans->scale((gzReal)(30*scale),(gzReal)(30*scale),(gzReal)(30*scale));

				group->addNode(trans);

				//((gzRoi *)gizmo)->loadAtPosition(NULL,gzRoiPosition(x,y,z),TRUE);

			}
			else
			{
				// We don't use Region of Interest. lets autoscale it

				gzTransform *trans=new gzTransform;

				trans->addNode(gizmo);
				
				trans->unitScale();

				// Lets scale it a bit

				trans->scale((gzReal)(30*scale),(gzReal)(30*scale),(gzReal)(30*scale));

				group->addNode(trans);
			}


			
			// Add a lamp to show position of light

			m_lamp=new gzTransform;

			gzGeometry *sphere=new gzGeometrySphere(1 , 20 ,gzVec4(1.0,1.0,1.0,1.0) );

			sphere->setBind(TRUE,GZ_VBO_STATIC_DROP_LOCAL);

			m_lamp->addNode( sphere );

			m_scene->addNode(m_lamp);

			// Set mouse press state
			m_inMousePress=FALSE;

			// Add a line of sight object

			m_los=new gzLineOfSight;

			m_los->setNodeTree(m_scene);

			m_scene->addNode(m_los);

			// Add a visual coverage object
			m_cov = new gzVisualCoverage;

			m_cov->setNodeTree(group);

			m_scene->addNode(m_cov);

			// Add a visual coverage sampling object

			gzImageRender *renderer=new gzImageRender(this);

			//gzImage_DepthMap *image=new gzImage_DepthMap;
			gzImage *image=gzImage::createImage(GZ_IMAGE_TYPE_DEPTHMAP);

			renderer->setImage(image);

			m_covSample = new gzVisualCovSample(renderer,image);

			m_covSample->setNodeTree(group);

			m_scene->addNode(m_covSample);

			// Test a virtual pip display

			gzViewPlane *viewPlane=new gzViewPlane;

			viewPlane->setCoordinates(gzVec2(0,0),gzVec2(100,100));

			viewPlane->usePixelPosition(TRUE);

			gzState *viewPlaneState=new gzState;

			gzTexture *tex=new gzTexture;

			tex->setImage(image);

			viewPlaneState->setTexture(tex);

			viewPlaneState->setMode(GZ_STATE_TEXTURE,GZ_STATE_ON);

			viewPlaneState->setActionStage(GZ_ACTION_STAGE_OVERLAY);

			viewPlaneState->setMode(GZ_STATE_ACTION_STAGE,GZ_STATE_ON);

			viewPlane->setState(viewPlaneState);

			viewPlane->setAlignment(GZ_VIEWPLANE_ALIGN_BOTTOM_RIGHT);

			gzTexEnv *env=new gzTexEnv;

			env->setMode(GZ_TEXENV_MODE_DECAL);

			viewPlaneState->setMode(GZ_STATE_TEXENV,GZ_STATE_ON);

			viewPlaneState->setTexEnv(env);

			m_switch = new gzSwitch;

			m_switch->addNode(viewPlane);

			m_scene->addNode(m_switch);

			m_switch->setSwitchVal(GZ_SWITCH_NONE);


			// Add the logo PIP

			gzImage *logo=gzImageManager::loadImage("gizmo3d.png",GZ_EVALUATE_EXTENSION,GZ_IMAGE_FLAGS_NO_ALTERNATE_IMAGE_EXT);


			if(logo)
			{

				gzViewPlane *viewPlane2=new gzViewPlane;

				viewPlane2->setCoordinates(gzVec2(10,10),gzVec2(256+10,64+10));

				viewPlane2->usePixelPosition(TRUE);

				gzState *viewPlaneState2=new gzState;

				gzTexture *tex2=new gzTexture;

				tex2->setImage(logo);

				tex2->setComponents(4);

				viewPlaneState2->setTexture(tex2);

				viewPlaneState2->setMode(GZ_STATE_TEXTURE,GZ_STATE_ON);

				viewPlaneState2->setActionStage(GZ_ACTION_STAGE_OVERLAY);

				viewPlaneState2->setMode(GZ_STATE_ACTION_STAGE,GZ_STATE_ON);

				//viewPlaneState2->setMode(GZ_STATE_ALPHA_FUNC,GZ_STATE_ON);

				//viewPlaneState2->setAlphaFunc(GZ_ALPHA_FUNC_GREATER,0.5f);

				viewPlaneState2->setMode(GZ_STATE_BLENDING,GZ_STATE_ON);
				viewPlaneState2->setBlendFunction(GZ_BLEND_SRC_ALPHA,GZ_BLEND_ONE_MINUS_SRC_ALPHA);


				viewPlane2->setState(viewPlaneState2);

				viewPlane2->setAlignment(GZ_VIEWPLANE_ALIGN_BOTTOM_LEFT);

				viewPlaneState2->setMode(GZ_STATE_TEXENV,GZ_STATE_ON);

				gzTexEnv *env2=new gzTexEnv;

				env2->setMode(GZ_TEXENV_MODE_BLEND);

				viewPlaneState2->setTexEnv(env2);

				m_scene->addNode(viewPlane2);
			}

		
			// Shall we shadow
			if(useShadow)
			{

				// ------------------- SHADOWS !!! --------------------------

				// Add the shadow renderer. This is responisble for the volume rending

				gzShadowRender *vol=new gzShadowRender;

				//vol->showVolumes(TRUE);	// Try this ;-)

				// Our shadow renderer can make various shadows. Now lets make a "simple" shadow that just
				// darkens a region and take away the specular component of the light

					
				// mask that adds the trasnparend black color to the shadow

				if(softShadow)
				{

					vol->addGeometryMask(createPlane(0));			// Now the plane that cover the entire display is a 
					vol->addGeometryMask(createPlane(0),1);
					vol->addGeometryMask(createPlane(0),2);
					vol->addGeometryMask(createPlane(0),3);

					//vol->setPassOffset(1);	//Standard way to use smooth shadow corners

				}
				else
				{
					m_spin->excludeDiffuseInShadow(TRUE);
					//vol->setPassOffset(1);	//Standard way to use smooth shadow corners
				}

				vol->useClearing(FALSE);				// Don't bother to clean up stencil values	

				// What is in the shadow..

				// Lets add all the scene to the possible shadow receivers for occluded specular light. All nodes under the shadow renderer
				// receives the shadow with specular component disabled.

				vol->addNode(group);

				// and add the shadow render to the scene 
				m_scene->addNode(vol);
		
				// Oahh ! Lets add shadows to all objects in the scene.
				// It adds shadow generators to all geomtery under the scene node
				// The length of the shadow is 50 times the distance between the light and the occluder

				if(softShadow)
					gzShadowGenerator::addShadowGenerators(group,vol,m_spin,FALSE,50,3,0.98f);
				else
					gzShadowGenerator::addShadowGenerators(group,vol,m_spin,FALSE,50);

			}



			// And add the group to the scene as well
			m_scene->addNode(group);

									
			// Add some textual info about frame rates
			m_text=new gzText("Screen Text");

			//m_text->setColor(gzVec4(1,1,0,0.5));
			m_text->use2DPosition(TRUE);
			m_text->set2DPosition(10, -10);
			//m_text->useMaskColor(TRUE);
			
			m_scene->addNode(m_text);

			// Ok. How long did this take...

			GZMESSAGE(GZ_MESSAGE_NOTICE,"Loading time for DB = %d secs",duration);


			// Now we want to look at the scene. Grab the default perspective camera from the window and set the scen as the
			// active one.
			getCamera()->setScene(m_scene);
			getCamera()->setNearClipPlane(1);
			getCamera()->setFarClipPlane(2000);
			//((gzPerspCamera *)getCamera())->setVerticalFOV(3);
			//((gzPerspCamera *)getCamera())->setHorizontalFOV(3);


			// Lets add some movement to the scene

			m_input=new gzSimpleMouseViewControl(getCamera());

			addInputInterface(m_input);

			if(useFog)
			{
				// And hey. Why not toss in some fog as well...
				group->useFog(TRUE);
				group->setFogStart(0);
				group->setFogEnd(3000);
				group->setFogMode(GZ_FOG_MODE_LINEAR);
				group->setFogDensity(0.7f);
				group->setFogColor(gzRGBA(0.5f,0.5f,0.5f,1.0f));
			}

			// Hmm. trivial
			setBackground(0.0f,0.0f,1.0f,1.0f);

			// Let us try to sync the refresh to the default monitor rate. Set zero value to go bezerk
			setSwapInterval(swap);

			// and show us ...
			show();

			// hm. wonder why I put this one here ?
			angle=2.22f;

			if(debug)
				m_scene->debug(GZ_DEBUG_SHOW_ALL);

			gzDynamicLoaderManager::startManager();

			gzOrigin::addOriginAxis();
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
					state->setBackPolygonMode(GZ_POLYGON_MODE_LINE);
					state->setMode(GZ_STATE_POLYGON_MODE,GZ_STATE_ON);
					state->setOverride(GZ_STATE_POLYGON_MODE,GZ_STATE_ON);
					state->setOverride(GZ_STATE_TEXTURE,GZ_STATE_OFF);

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
					gzLogger logger;
					getCamera()->getScene()->debug(GZ_DEBUG_SHOW_ALL);
				}
				break;

			case 'f' :
				if(keystate == GZ_KEY_STATE_PRESSED)
				{
					GZMESSAGE(GZ_MESSAGE_NOTICE,"FrameRate %d",getFrameRate());
				}
				break;

			case 'p' :
				if(keystate == GZ_KEY_STATE_PRESSED)
				{
					GZMESSAGE(GZ_MESSAGE_NOTICE,"Position %s HPR %s",(const char *)getCamera()->getPosition().asString(),(const char *)getCamera()->getHPR().asString());
				}
				break;

			case '1' :
			case '2' :
				{
					gzVec3 startPosition;
					gzVec3 direction;


					gzGetMouseXYIntersector(startPosition,direction,getCamera(),mouse_x,mouse_y,getSize());

					gzIntersector isector;

					isector.setBaseTransform(getContext()->mainTransform);
					isector.setProjectionTransform(getContext()->mainProjTransform);

					isector.setStartPosition(startPosition);
					isector.setDirection(direction);

					if(isector.intersect(getCamera()->getScene(),(GZ_INTERSECTOR_NEAREST_POINT)))
					{
						gzVec3 pos=isector.getResult()[0].coordinate;

						if(key=='1')
							m_los->setStartPosition(pos);
						else
							m_los->setStopPosition(pos);
					}
				}
				break;

			case '3' :
				{
					gzVec3 startPosition;
					gzVec3 direction;

					gzGetMouseXYIntersector(startPosition,direction,getCamera(),mouse_x,mouse_y,getSize());

					gzIntersector isector;

					isector.setBaseTransform(getContext()->mainTransform);
					isector.setProjectionTransform(getContext()->mainProjTransform);

					isector.setStartPosition(startPosition);
					isector.setDirection(direction);

					if(isector.intersect(getCamera()->getScene(),(GZ_INTERSECTOR_NEAREST_POINT)))
					{
						gzVec3 pos=isector.getResult()[0].coordinate;

						m_cov->setPosition(pos+gzVec3(0,2,0));
					}
				}
				break;

			case '4' :
			case '5' :
				{
					gzVec3 startPosition;
					gzVec3 direction;

					gzGetMouseXYIntersector(startPosition,direction,getCamera(),mouse_x,mouse_y,getSize());
					gzIntersector isector;

					isector.setBaseTransform(getContext()->mainTransform);
					isector.setProjectionTransform(getContext()->mainProjTransform);

					isector.setStartPosition(startPosition);
					isector.setDirection(direction);

					if(isector.intersect(getCamera()->getScene(),(GZ_INTERSECTOR_NEAREST_POINT)))
					{
						gzVec3 pos=isector.getResult()[0].coordinate;

						if(key=='4')
							m_covSample->setPosition(pos+gzVec3(0,2,0));
						else
							m_covSample->lookAt(pos);

						m_switch->setSwitchVal(GZ_SWITCH_ALL);
					}
				}
				break;

			case GZ_KEY_ADD :
				{
					m_lodFactor+=0.1f;
					gzLod::setLodFactor(getContext(),m_lodFactor);
					GZMESSAGE(GZ_MESSAGE_DEBUG,"Lod Factor:%f",m_lodFactor);
				}
				break;

			case GZ_KEY_SUBTRACT :
				{
					if(m_lodFactor>0)
						m_lodFactor-=0.1f;
					gzLod::setLodFactor(getContext(),m_lodFactor);
					GZMESSAGE(GZ_MESSAGE_DEBUG,"Lod Factor:%f",m_lodFactor);
				}
				break;

			case 'h' :
				{
					if(keystate == GZ_KEY_STATE_PRESSED)
					{
				
						gzVec3 startPosition;
						gzVec3 direction;

						gzGetMouseXYIntersector(startPosition,direction,getCamera(),mouse_x,mouse_y,getSize());
						gzIntersector isector;

					isector.setBaseTransform(getContext()->mainTransform);
					isector.setProjectionTransform(getContext()->mainProjTransform);

					isector.setStartPosition(startPosition);
					isector.setDirection(direction);

						if(isector.intersect(getCamera()->getScene(),(GZ_INTERSECTOR_NEAREST_POINT|GZ_INTERSECTOR_HISTORY|GZ_INTERSECTOR_NODE)))
						{
							gzVec3 pos=isector.getResult()[0].coordinate;

							GZMESSAGE(GZ_MESSAGE_DEBUG,"Node %s",(const char *)isector.getResult()[0].node->getName());
						}
					}
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

		m_spin->setPosition((gzReal)(30*cos(5*angle)),(gzReal)(10*cos(angle))+30,(gzReal)(30*sin(angle/2)+20));

		m_lamp->setTranslation((gzReal)(30*cos(5*angle)),(gzReal)(10*cos(angle))+30,(gzReal)(30*sin(angle/2)+20));

		gzSeedTransform::setGlobalStriveStrength((1+(0.3*cos(7*angle)*sin(3*angle)+0.4*cos(5*angle)+0.2*sin(8*angle)+0.1*sin(21*angle) ))*0.5);

	}

	virtual ~MyWindow()
	{
		if(m_input)
			delete m_input;

		if(m_los)
			m_los->setNodeTree(NULL);	// Recursive deadlock of scene
	}

	gzRefPointer<gzState> m_state;

	gzLight *			m_spin;

	gzText *			m_text;

	gzReal				angle;

	gzTransform	*		m_lamp;
	
	gzBool				m_inMousePress;

	gzNode				*gizmo;

	gzLineOfSight		*m_los;

	gzVisualCoverage	*m_cov;

	gzVisualCovSample	*m_covSample;

	gzScene				*m_scene;

	gzSwitch			*m_switch;

	gzSimpleMouseViewControl	*m_input;


	gzFloat				m_lodFactor;
};

// Definition of a sample application
// The application provides an initialisation and an onIdle loop manager
// to do the refresh of the window

class WindowApp : public gzApplication
{
public:
		
	WindowApp(){}

    ~WindowApp(){}

    void Create(gzInt32 swap,gzBool useLodSmooth,gzDouble fadeDist,gzBool useFog,gzBool softShadow,gzBool useRoi,gzBool debug,gzBool bFullScreen,gzBool useShadow, gzSerializeAdapterFlags flags,gzString filename,gzReal scale,gzBool skyLayer, gzVec3 eyePos,gzVec3 hpr)
	{
		gzGraphicsFormat *format = new gzGraphicsFormat;

		if(bFullScreen)
		{
			format->setFullScreenWidth(1280);
			format->setFullScreenFrequency(75);
			format->useFullScreen(TRUE);
			format->setDepthBits(32);
		}

		format->useStencil(TRUE);

		m_win = new MyWindow(	swap,
								useLodSmooth,
								fadeDist,
								useFog,
								useRoi,
								softShadow,
								debug,
								format,useShadow,flags,filename,scale,skyLayer);

		gzPerspCamera *camera=gzDynamic_Cast<gzPerspCamera>(m_win->getCamera());

		camera->setHPR(hpr);
		camera->setPosition(eyePos);

		if(hpr==gzVec3(0,0,0))
			camera->lookAt(0,0,0);

		camera->useInfiniteFarPlane(TRUE);

		//camera->flipVerticalView(TRUE);

		//camera->flipHorizontalView(TRUE);

		if(useRoi)
			m_win->getCamera()->useRoiPosition(TRUE);


		gzULongXY size=m_win->getSize();


	}

	void onIdle()
	{
		if(m_win)
		{
			m_win->onIdle();

			m_win->triggerKeyEvaluation();

			if(!m_win->refreshWindow())	// Yield some time if no rendering
				gzSleep(100);
			else
				gzSleep(1);				// Yield so we can use some dynamic loading in the background as well.

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

};

#include "gzDebug.h"




//---------------------------------------------------------------

gzVoid outputHelp()
{
	GZMESSAGE(GZ_MESSAGE_NOTICE,"---------------------- Help -----------------------");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use keys 'a' and 'z' to move forward/backwards");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use keys 'q' and 'w' to stride left/right");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use keys 's' and 'x' to move up/down");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use arrow keys to rotate viewer");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- or press left mouse button to fly with mouse");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"-");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use argument -x xxx to enable xxx func.. see below");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"-");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-a' to enable animation in files");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-b' to smooth alpha mask");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-c' to enable fog");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-d' to get debug tree output");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-e' to enable smooth lod ranges (200 m)");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-f' to enable full screen");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-g xxx' to set swap sync to xxx");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-h' to print this help");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-i xxx' to set lod fade distance to xxx");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-j' to convert to grayscale images");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-k' to generate indexed geometry");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-l' to enable generation of light materials");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-m xxxx' to load model xxxx");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-p' to enable mipmap usage");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-q' to enable shadows for scene");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-r' to enable ROI (Region Of Interest) position");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-s xxx' to set scale xxx");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-t' to enable soft shadows on some HW");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-u' to enable sky Layers");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-v x,y,z' to set camera position");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-w' to enable prefix encoding to enhance some DB formats");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-x h,p,r' to set camera heading");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-y' to enable VBO");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-z' to enable DisplayLists");
	
}


int main(int argc, char *argv[])
{

	gzStartUpGizmo();	// Needed for some systems to install external graphics engines

	gzMessage::setMessageLevel(GZ_MESSAGE_MEM_DEBUG);


	try
	{ 
		gzArgumentParser args(argc,argv);

		args.exitOnError(FALSE);	// We want to print the help

		gzBool		fullscreen	= args.hasOption("f") | args.hasOption("fullscreen");

		gzString	filename	= args.getOptionValue("m","gizmo.3ds");

		gzFloat		scale		= args.getOptionValue("s",1.0);

		gzBool		useShadow	= args.hasOption("q");

		gzBool		debug		= args.hasOption("d");

		gzBool		useRoi		= args.hasOption("r");

		gzBool		softShadow	= args.hasOption("q");

		gzBool		useLodSmooth= args.hasOption("e");

		gzBool		useFog		= args.hasOption("c");

		gzDouble	fadeDist	= args.getOptionValue("i",500);

		gzInt32		swap		= args.getOptionValue("g",1);

		gzBool		skyLayer	= args.hasOption("u");

		gzSerializeAdapterFlags		flags=GZ_DB_FLAGS_DEFAULT;

		gzVec3		eyePos		= gzVec3(args.getOptionValue("v",50.0,0),args.getOptionValue("v",50.0,1),args.getOptionValue("v",120.0,2));

		gzVec3		hpr			= gzVec3(args.getOptionValue("x",0.0f,0),args.getOptionValue("x",0.0f,1),args.getOptionValue("v",0.0f,2));


		gzLicense::notifyDefaultLicense();
	
		gzGraphicsEngine::useEngine(GZ_ENGINE_OPENGL);

		gzInitializeDbManagers();

		flags=flags|(args.hasOption("a") ? GZ_DB_FLAGS_USE_ANIMATION : GZ_DB_FLAGS_DEFAULT);

		flags=flags|(args.hasOption("b") ? GZ_DB_FLAGS_USE_TWO_PASS_ALPHA : GZ_DB_FLAGS_DEFAULT);

		flags=flags|(args.hasOption("j") ? GZ_DB_FLAGS_CONVERT_IMAGES_TO_GRAYSCALE : GZ_DB_FLAGS_DEFAULT);

		flags=flags|(args.hasOption("k") ? GZ_DB_FLAGS_USE_INDEXED_GEOMETRY : GZ_DB_FLAGS_DEFAULT);

		flags=flags|(args.hasOption("l") ? GZ_DB_FLAGS_GENERATE_LIGHT_MATERIAL : GZ_DB_FLAGS_DEFAULT);

		flags=flags|(args.hasOption("p") ? GZ_DB_FLAGS_USE_MIPMAPS : GZ_DB_FLAGS_DEFAULT);

		flags=flags|(args.hasOption("w") ? GZ_DB_FLAGS_USE_NAME_ENCODED_NODES : GZ_DB_FLAGS_DEFAULT);

		flags=flags|(args.hasOption("y") ? GZ_DB_FLAGS_USE_BUFFERED_GEOMETRY : GZ_DB_FLAGS_DEFAULT);

		flags=flags|(args.hasOption("z") ? GZ_DB_FLAGS_USE_DISPLAYLIST_GEOMETRY : GZ_DB_FLAGS_DEFAULT);

		if(args.hasOption("h") || args.hasOption("help") || args.hasError())
		{
			if(args.hasError())
				GZMESSAGE(GZ_MESSAGE_WARNING,"Syntax Error:%s",(const char *)args.getError());
			outputHelp();

			exit(99);
		}
				
		gzModule *module=gzModule::getModule("gzFFMPeg");

		// Make the application
		WindowApp app;
	      
		// Create the scene and the window
		app.Create(	swap,
					useLodSmooth,
					fadeDist,
					useFog,
					softShadow,
					useRoi,
					debug,
					fullscreen,
					useShadow,
					flags,
					filename,
					scale,
					skyLayer,
					eyePos,
					hpr);
		
		// run the application
		app.run();

	}
	catch(gzBaseError &error)       // In case of exceptions thrown we want to print the message
	{
		error.reportError();
	}

	gzModule::unLoadModules();
	
	gzShutDownGizmo();

	return 0;
}



