#include "GizmoView.h"

#define GZ_ENABLE_NEW_HANDLER
#include "gzGizmo3DLibrary.h"

// ------------------ CGizmoWindow -------------------------

class CGizmoWindow : public gzWindow
{
public:

	CGizmoWindow(HWND hwnd,gzGraphicsFormat *format=NULL):gzWindow("CGizmoWindow",hwnd,format)
	{
		setRefreshRate(100);	// FrameLock down
		setSwapInterval(1);

		showDynamicLoadProgress(TRUE);

		// Setup camera

		gzPerspCamera *camera=gzDynamic_Cast<gzPerspCamera>(getCamera());

		camera->setPosition(0,10,10);
		camera->lookAt(0,0,0);
		camera->useInfiniteFarPlane(TRUE);

		// Setup generic top scene with lights

		m_propertyTopTransform=new gzTransform;

		m_propertyEnvironment=new gzEnvironment;
		m_propertyEnvironment->addNode(m_propertyTopTransform);

		m_propertyHeadLight=new gzLight("HeadLight");
		m_propertyEnvironment->addLight(m_propertyHeadLight);

		gzEnvironment::setGlobalAmbientColor(getContext(),0.5,0.5,0.5);
		gzEnvironment::setTwoSideLighting(getContext(),TRUE);
		gzEnvironment::setLocalViewer(getContext(),TRUE);

		m_propertyEnvironment->useFog(FALSE);
		m_propertyEnvironment->setFogStart(0);
		m_propertyEnvironment->setFogEnd(5000);
		m_propertyEnvironment->setFogMode(GZ_FOG_MODE_LINEAR);
		m_propertyEnvironment->setFogDensity(0.7f);
		m_propertyEnvironment->setFogColor(gzRGBA(0.5f,0.5f,0.5f,1.0f));

		gzScene *scene=new gzScene;
		scene->addNode(m_propertyEnvironment);

		camera->setScene(scene);
	}

	virtual gzBool refreshWindow()
	{
		// Do some custom stuff each rendering 

		triggerKeyEvaluation();
		
		m_propertyHeadLight->setPosition(getCamera()->getPosition());

		gzBool result=gzWindow::refreshWindow();

		return result;
	}

	GZ_PROPERTY(gzEnvironmentPtr,	Environment);
	GZ_PROPERTY(gzTransformPtr,		TopTransform);
	GZ_PROPERTY(gzLightPtr,			HeadLight);
};

gzKeyValue keyRemapper(unsigned int key)
{
	// just a small example of how to remap keys used by Gizmo

	switch(key)
	{
		case 'A' : return (gzKeyValue)'a';
		case 'Z' : return (gzKeyValue)'z';
		case 'Q' : return (gzKeyValue)'q';
		case 'W' : return (gzKeyValue)'w';
		case 'S' : return (gzKeyValue)'s';
		case 'X' : return (gzKeyValue)'x';
	}

	return GZ_KEY_INVALID;
}

// ------------------ CGizmoView -------------------------

CGizmoView * CGizmoView::createInstance(HWND hwnd)
{
	return new CGizmoView(hwnd);
}

void CGizmoView::destroyInstance(CGizmoView *instance)
{
	delete instance;
}

CGizmoView::CGizmoView(HWND hwnd)
{
	gzGraphicsFormat *format = new gzGraphicsFormat;
	format->useStencil(TRUE);
	format->useMultisample(TRUE);
	format->setMultisamples(4);

	m_win=new CGizmoWindow(hwnd,format);

	m_input=new gzSimpleMouseViewControl(m_win->getCamera());

	m_win->addInputInterface(m_input);
}

CGizmoView::~CGizmoView(void)
{
}

void CGizmoView::onKeyDown(unsigned int key)
{
	if(key==32)
	{
		if(gzDynamicLoaderManager::isManagerRunning())
			gzDynamicLoaderManager::stopManager();
		else
			gzDynamicLoaderManager::startManager();
	}

	m_win->onKey(keyRemapper(key),GZ_KEY_STATE_PRESSED,m_win->getMousePosition().x,m_win->getMousePosition().y);
}

void CGizmoView::onKeyUp(unsigned int key)
{
	m_win->onKey(keyRemapper(key),GZ_KEY_STATE_RELEASED,m_win->getMousePosition().x,m_win->getMousePosition().y);
}

void CGizmoView::resize(int cx, int cy )
{
	m_win->setSize(cx,cy);
}

void CGizmoView::load(const TCHAR *filename)
{
	gzNodePtr node=gzDbManager::loadDB(filename,GZ_EVALUATE_EXTENSION,GZ_DB_FLAGS_USE_MIPMAPS|GZ_DB_FLAGS_USE_ANIMATION);

	if(!node)
	{
		// Try an ordinary image

		gzImage *image=gzImageManager::loadImage(filename);
		
		if(image)
		{
			gzViewPlane *plane=new gzViewPlane(0.999f);

			plane->setColor(gzVec4(1,1,1,1));

			gzState *state=new gzState;

			gzTexture *tex=new gzTexture;

			tex->setImage(image);

			state->setTexture(tex);

			state->setMode(GZ_STATE_TEXTURE,GZ_STATE_ON);

			plane->setState(state);

			node=plane;

			// Check for video

			gzVideo *video=gzDynamic_Cast<gzVideo>(image);
			if(video)
			{
				video->invokeMethod("setLeftAudioSource",new gzAudioSource);
				video->invokeMethod("setRightAudioSource",new gzAudioSource);
			}
		}
	}
	else
	{
		// ---- check animation ------------

		gzFrame *frame=gzDynamic_Cast<gzFrame>(node);

		if(frame)
		{
			frame->setFrameDuration(0.05);
			frame->startFrameCounter();
		}

		// Add some auto scale feature

		gzRoi *roi=gzDynamic_Cast<gzRoi>(node);

		if(roi)
		{
			// Start possible roi management
			gzDynamicLoaderManager::startManager();

			// We use ROI. model is scaled in metres

			gzDouble x=0,y=0,z=0;

			gzTransform *transform=new gzTransform;

			transform->addNode(roi);

			if(gzHasNodeDbInfo(roi, GZ_DB_INFO_DB_SIZE))
			{
				gzAttribute_DBSize dbinfo=gzDynamic_Cast<gzAttribute_DBSize>(gzGetNodeDbInfo(roi, GZ_DB_INFO_DB_SIZE));

				x=(dbinfo.minX+dbinfo.maxX)/2;

				y=(dbinfo.minY+dbinfo.maxY)/2;

				z=(dbinfo.minZ+dbinfo.maxZ)/2;

				gzDouble scale=1;

				while(x>10000)
				{
					x=x/10;
					y=y/10;
					z=z/10;

					scale=scale/10;
				}

				transform->setScale(scale);
				transform->setTranslation(-(gzReal)x,-(gzReal)y,-(gzReal)z);

				// Activate the fog
				/*m_win->getEnvironment()->useFog(TRUE);
				m_win->setBackground(0.5f,0.5f,0.5f,1.0f);*/

				// Activate Sky layers

				gzSkyBox *box=new gzSkyBox;
				
				box->setImageSide(GZ_SKYBOX_SKY,gzImageManager::loadImage("../skybox/sky.png"));
				box->setImageSide(GZ_SKYBOX_EAST,gzImageManager::loadImage("../skybox/east.png"));
				box->setImageSide(GZ_SKYBOX_NORTH,gzImageManager::loadImage("../skybox/north.png"));
				box->setImageSide(GZ_SKYBOX_WEST,gzImageManager::loadImage("../skybox/west.png"));
				box->setImageSide(GZ_SKYBOX_SOUTH,gzImageManager::loadImage("../skybox/south.png"));
				
				box->setHorizonAngle(10);

				box->setStandardHeight(2000);

				box->setGroundPoint(0,0,0);
			
				box->useFixedHeight(TRUE);

				transform->addNode(box);


				gzSkyLayer *layer;

				layer=new gzSkyLayer;

				gzImage *clouds=gzImageManager::loadImage("../skybox/cloud2.png");

				layer->setLayerImage(clouds,100000,100000,2000,600,300);
			
				layer->setHorizonAngle(10);

				transform->addNode(layer);

			
				layer=new gzSkyLayer;

				layer->setHorizonAngle(10);
			
				layer->setLayerImage(clouds,70000,50000,3000,0,500);

				transform->addNode(layer);


			}

			/*
			transform->unitScale();

			if(gzHasNodeDbInfo(roi, GZ_DB_INFO_DB_MAX_LOD_RANGE))
			{
				gzFloat range=1/(gzFloat)gzGetNodeDbInfo(roi, GZ_DB_INFO_DB_MAX_LOD_RANGE).getNumber();

				transform->scale(range);
			}
			
			transform->scale(200.0f,200.0f,200.0f);

			*/

			node=transform;
		}
		else
		{
			// --- Unit scale it -------------

			gzTransform *scale=new gzTransform;
			scale->addNode(node);
			scale->unitScale();

			node=scale;
		}

	}
	
	if(node)
	{
		gzNodeOptimizer optimizer;

		node=optimizer.optimize(node);

		m_win->getTopTransform()->addNode(node);
	}
}

// ------------------ CGizmoControl -------------------------

void CGizmoControl::initialize()
{
	// This line will give you debug output
	//gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG);

	// Fire OpenGL up as default graphics engine
	// Fire OpenAL up as default audio engine
	gzStartUpGizmo();

	// initialise the generic db readers/writers
	gzInitializeDbManagers();

	// Enable some plugins
	gzModule::getModule("gzFFMPeg");
	gzModule::getModule("gzSvg");

}

void CGizmoControl::uninitialize()
{
	gzModule::unLoadModules();
	gzShutDownGizmo();
}
