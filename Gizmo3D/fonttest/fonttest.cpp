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
// File			: fonttest.cpp
// Module		: 
// Description	: Test app to virify font performance
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
// AMO  060214  Created file    
//
// ******************************************************************************

//#define GZ_MEM_DEBUG	//to enable mem debug

// The general include
#include "gzGizmo3DLibrary.h"


class MyWindow : public gzWindow
{
public:

	/*
	gzBool onMouseCursor()
	{
		SetCursor(LoadCursor(NULL, IDC_CROSS));
		return TRUE;
	}
	*/

	MyWindow(	gzInt32 swap,gzBool debug,gzGraphicsFormat *format , gzWindowHandle parent,
				gzBool useTextColor,gzBool useOutline,gzString font)
					:gzWindow(gzString("Gizmo3D : ")+gzTime::now(TRUE).asString(),parent,format,FALSE) /*:gzWindow("test",0,GetWindowDC(NULL),format) */
	{
			// Create a scene to render -------------------------------------
			// We must have a scene that the camera can "look at"
			m_scene=new gzScene("Font Example Scene");

					
			// Add some textual info about frame rates
			m_text=new gzText("Screen Text");

			m_scene->addNode(m_text);
	
			
			gzFont *fonten=gzFont::getFont(gzFontInfo(font,256,24,0,0,0,0,0,1));

			
			for(gzUInt32 i=0;i<256;i++)
			{
				gzText *text=new gzText;

				text->format("%c",i);

				text->useOverlay(FALSE);

				if(useTextColor)
					text->setColor(gzVec4(1,1,0,0.7));

				text->setFont(fonten);

				
				//text->useScreenFixFont(FALSE);

				//text->setScale(0.05);
				

				if(useOutline)
				{
					text->useMaskColor(TRUE);
					text->setMaskColor(gzVec4(0,0,1,1));
				}

				gzTransform *trans=new gzTransform;

				trans->setTranslation(10*gzRandom(),10*gzRandom(),10*gzRandom());

				trans->addNode(text);

				m_scene->addNode(trans);
			}
			

			// Set up the ordinary screen font		

			m_text->use2DPosition(TRUE);
			m_text->set2DPosition(10, -10);
			//m_text->setScale(40);
			
			/*
			m_text->setMaskColor(gzVec4(0,0,0,1));
			m_text->useMaskColor(TRUE);
			*/


			getCamera()->setScene(m_scene);
			getCamera()->setNearClipPlane(1);
			getCamera()->setFarClipPlane(2000);


			// Let us try to sync the refresh to the default monitor rate. Set zero value to go bezerk
			setSwapInterval(swap);

			// and show us ...
			show();

	};


	/*
		The following code snippet is a virtual function to catch the window messages
		for pressed, repeated and released keys
	*/


	gzVoid onIdle()
	{

	}


	virtual ~MyWindow()
	{
	}

	gzText				*m_text;

	gzScene				*m_scene;
};

// Definition of a sample application
// The application provides an initialisation and an onIdle loop manager
// to do the refresh of the window

class WindowApp : public gzApplication 
{
public:

	
	WindowApp():m_win(NULL),m_format(NULL)
	{
	};

    ~WindowApp()
	{
	}

    void Create(
					gzInt32 swap,
					gzBool debug,
					gzBool bFullScreen,
					gzInt32 ms,
					gzWindowHandle parent,
					gzBool useTextColor,
					gzBool useOutline,
					gzString font)
	{
		m_format = new gzGraphicsFormat;

		if(bFullScreen)
		{
			m_format->setFullScreenWidth(1280);
			m_format->setFullScreenFrequency(75);
			m_format->useFullScreen(TRUE);
			m_format->setDepthBits(32);
		}

		m_format->useStencil(TRUE);

		if(ms)
		{
			m_format->useMultisample(TRUE);
			m_format->setMultisamples(ms);
		}

		//m_format->useDoubleBuffer(FALSE);

		m_win = new MyWindow(	swap,
								debug,
								m_format,
								parent,
								useTextColor,
								useOutline,
								font);

		gzPerspCamera *camera=gzDynamic_Cast<gzPerspCamera>(m_win->getCamera());

		camera->setPosition(30,30,30);
		//camera->setPosition(0,0,30);

		camera->lookAt(0,0,0);

		camera->useInfiniteFarPlane(TRUE);

		//m_win->setBackground(0.5,0.7,0);

		//camera->flipVerticalView(TRUE);

		//camera->flipHorizontalView(TRUE);

	}

	void onIdle()
	{
		static gzString LF="\n\n\n";

		static gzFloat rot=0;

		if(m_win)
		{
			m_win->onIdle();

			if(!m_win->refreshWindow())	// Yield some time if no rendering
				gzSleep(30);
			
			gzString outputString;

			outputString=gzString("FrameRate:")+gzString((gzUInt64)m_win->getFrameRate())+LF+gzGraphicsStats(m_win->getContext());

			m_win->m_text->setText(outputString);

			//m_win->m_text->setHPR(0,0,rot);

			//m_win->m_text->setScale(2);

			//rot+=0.05;

		}

	}

private:

	friend class MyWindow;
	
	gzRefPointer<MyWindow>   m_win;
	gzGraphicsFormat        *m_format;
};


gzVoid outputHelp()
{
	GZMESSAGE(GZ_MESSAGE_NOTICE,"---------------------- Help -----------------------");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use argument -x xxx to enable xxx func.. see below");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"-");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-d' to get debug tree output");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-f' to enable full screen");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-g xx' to set swap sync to xx");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-h' to print this help");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-ms xx' to get multsample xx");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-color' to enable text color and transparency");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-outline' to enable text outline");
	GZMESSAGE(GZ_MESSAGE_NOTICE,"- Use '-font xx' to enable font xx");
}


int main(int argc, char *argv[])
{
	gzStartUpGizmo();	// Needed for some systems to install external graphics engines

	gzMessage::setMessageLevel(GZ_MESSAGE_MEM_DEBUG);

	/* // To update the font compiled data file

	gzSerializeAdapter *adapter=gzSerializeAdapter::getURLAdapter("fonts.gzf",GZ_SERIALIZE_OUTPUT);

	adapter=new gzSerializeAdapterCompress(adapter);

	gzFont *font=gzFont::getFont(gzFontInfo("mapinfo cartographic",256,24,0,FALSE,FALSE,FALSE,FALSE,TRUE));
	font->write(adapter);

	delete adapter;

	*/

	try
	{ 
		gzArgumentParser args(argc,argv);

		args.exitOnError(FALSE);	// We want to print the help

		gzBool		fullscreen		= args.hasOption("f") | args.hasOption("fullscreen");

		gzBool		debug			= args.hasOption("d");

		gzInt32		swap			= args.getOptionValue("g",1);

		gzInt32		multisample		= args.getOptionValue("ms",0);

		gzBool		useTextColor	= args.hasOption("color");

		gzBool		useOutline		= args.hasOption("outline");

		gzString	font			= args.getOptionValue("font",gzString("courier"));

		gzLicense::notifyDefaultLicense();
	
		gzGraphicsEngine::useEngine(GZ_ENGINE_OPENGL);

		gzInitializeDbManagers();

		if(args.hasOption("h") || args.hasOption("help") || args.hasError())
		{
			if(args.hasError())
				GZMESSAGE(GZ_MESSAGE_WARNING,"Syntax Error:%s",(const char *)args.getError());
			outputHelp();

			exit(99);
		}

		// Make the application
		WindowApp *app=new WindowApp;
	      
		// Create the scene and the window
		app->Create(swap,
					debug,
					fullscreen,
					multisample,
					NULL,
					useTextColor,
					useOutline,
					font);
		
		// run the application

		gzExecuteProgram(app);

		delete app;

	}
	catch(gzBaseError &error)       // In case of exceptions thrown we want to print the message
	{
		error.reportError();
	}

	gzShutDownGizmo();

	return 0;
}



