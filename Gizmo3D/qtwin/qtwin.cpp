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
// File			: qtwin.cpp
// Module		:
// Description  : Test app to show how to interface qt (minimal app!) or any other
//				  equal GUI toolkit that exports a window handle.
// Author		: Anders Modén
// Product		: Gizmo3D 2.11.1
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
// AMO  990611  Created file
//
// ******************************************************************************


// These QT includes are just an example of how to interface QT
// Other toolkits can be used as well.

#include <QWidget>
#include <QSplitter>
#include <QResizeEvent>
#include <QApplication>
#include <QLayout>

#include "gzGizmo3DLibrary.h"  // include all Gizmo3D functionality

// Sample class to show how to get resize events and refreshs...


class MyQtWindow : public QWidget
{
public:
	MyQtWindow(QWidget *parent) : QWidget(parent)
	{
		#ifdef GZ_GLX
			m_gizmoWin=new gzWindow("QtWidget",(gzWindowContext)winId(),qt_xdisplay());
		#elif defined GZ_WIN32
			m_gizmoWin=new gzWindow("QtWidget",(gzWindowContext)winId());
		#elif defined GZ_MAC
			m_gizmoWin=new gzWindow("QtWidget",(gzWindowContext)topLevelWidget()->winId(),NULL);
		#endif
	}

	void resizeEvent ( QResizeEvent * event)
	{
		m_gizmoWin->setSize(event->size().width() , event->size().height());
		
		#ifdef GZ_MAC
		
			QPoint t=mapTo(topLevelWidget(),QPoint(0,0));
		
			m_gizmoWin->setPosition(t.x(),t.y());
			
		#endif
		
		m_gizmoWin->refreshWindow();
	}

	void refresh ()
	{
		m_gizmoWin->refreshWindow();
	}

	gzWindow * getGizmoWin() { return m_gizmoWin; }

private:

	gzWindow	*m_gizmoWin;
};

// the very main loop

int main(int argc , char *argv[] )
{
	gzStartUpGizmo();	// Needed for some systems to install external graphics engines

	try
	{
		QApplication *a=new QApplication(argc,argv);

		QWidget *w=new QWidget;

		w->resize(300,300);

		QSplitter *splitter=new QSplitter(w);


		QBoxLayout *sizer = new QHBoxLayout(w);

		sizer->addWidget(splitter);
		sizer->activate();

		gzInitializeDbManagers();
		gzGraphicsEngine::useEngine(GZ_ENGINE_OPENGL);			// These two lines are all to be added
		gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG);

		// Create a scene to render
		gzScene *scene=new gzScene("3ds Scene");

		// Create some light environment group
		gzEnvironment *group=new gzEnvironment;

		gzKeyDatabase::setGlobalKey("GIZMO_PATH","../dbviewer");

		gzNode *node=gzDbManager::loadDB("butt.3ds",GZ_EVALUATE_EXTENSION);

		gzTransform *scale=new gzTransform;

		scale->addNode(node);
		scale->unitScale();
		scale->scale(30,30,30);

		gzFrame *frame = new gzFrame;
		frame->setFrameDuration(1/30.0);
        frame->setNumberOfFrames(50);
		frame->selectChildPerFrame(TRUE);


        for(int i=0;i<50;i++)
		{
			gzTransform *transform=new gzTransform;
			transform->setHPR(360.0/50.0*i,0,0);
			transform->addNode(scale);
			frame->addNode(transform);
		}

		scene->addNode(frame);

		frame->startFrameCounter();

		// Add some light to environment
		gzLight *light=new gzLight;
		group->addLight(light);

		light->setSpecularColor(0,0,0);	// No shiny white
		light->setPosition(20,20,20);



        MyQtWindow *win_a=new MyQtWindow(splitter);   // if you want to use GIZMO in your app

		win_a->getGizmoWin()->getCamera()->setScene(scene);
		win_a->getGizmoWin()->getCamera()->setPosition(100,100,100);
		win_a->getGizmoWin()->getCamera()->lookAt(0,0,0);
		win_a->getGizmoWin()->setBackground(1.0f,0.6f,0.6f,1.0f);

        MyQtWindow *win_b=new MyQtWindow(splitter);   // if you want to use GIZMO in your app
		win_b->getGizmoWin()->getCamera()->setScene(scene);
		win_b->getGizmoWin()->getCamera()->setPosition(100,100,100);
		win_b->getGizmoWin()->getCamera()->lookAt(0,0,0);
		win_b->getGizmoWin()->setBackground(1,0.6f,1.0f,1.0f);

		QApplication::setActiveWindow( w );
		w->show();

		while(QApplication::activeWindow()->isVisible())
		{

			win_a->refresh();
			win_b->refresh();

			a->processEvents();
		}

		delete w;

		delete a;
	}
	catch(gzBaseError &error)       // In case of exceptions thrown we want to print the message
	{
		error.reportError();
	}

	gzShutDownGizmo();

	return 0;
}
