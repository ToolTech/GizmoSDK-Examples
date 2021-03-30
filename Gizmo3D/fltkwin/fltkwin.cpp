
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Widget.H>
#include <FL/x.H>

// Gizmo Includes
#include "gzGizmo3DLibrary.h"

// A special rendered node
class MyNodeClass : public gzNode
{
public:

	GZ_DECLARE_TYPE_INTERFACE;		// provide RTTI typed interface

	MyNodeClass(const gzString & name="MyNodeClass"):gzNode(name)
	{

	}

	virtual ~MyNodeClass()
	{
	}

	// ---------- Clone interface ---------------------------------------

	virtual gzReference* clone() const	// Provide a clone method for factories
	{
		return new MyNodeClass(*this);
	}

	// ---------- Action Interface --------------------------------------

	virtual gzVoid preTraverseAction( gzTraverseAction *actionclass , gzContext *context)
	{
		if(actionclass->isExactType(gzRenderAction::getClassType()))	// Exact a graphic action
		{
			gzBegin(GZ_QUADS);
			gzColor4d(1,1,1,1);
			gzVertex3d(0,0,0);
			gzColor4d(1,0,1,1);
			gzVertex3d(0,1,0);
			gzColor4d(1,1,0,1);
			gzVertex3d(1,1,0);
			gzColor4d(0,1,1,1);
			gzVertex3d(1,0,0);
			gzEnd();
		}
	}

	virtual gzVoid postTraverseAction( gzTraverseAction *actionclass , gzContext *context)
	{
		// Here we could add some cleanup if we were a group etc.
	}

	virtual gzActionStage useActionStage( gzTraverseAction *actionclass , gzContext *context) 
	{ 
		return GZ_ACTION_STAGE_SORTED_TEXTURE; 
	}

	virtual gzVoid updateNode()		// Note new code since Gizmo3D 1.1 Beta 24
	{
		gzVec3 center(0.5,0.5,0);		// Her is your own code to calculate the boundary sphere

		gzReal radius=2;

		resetBoundary();				// And set it
		includeBoundary(center,radius);	

		adjustBoundary();				// We must apply to this rule
	}
};

// Add some type info inheritance
GZ_DECLARE_TYPE_CHILD(gzNode,MyNodeClass,"MyNodeClassName");



class Test : public Fl_Widget
{
public:

	Test(int X, int Y, int W, int H, const char *l=0):Fl_Widget(X, Y, W,H,l)
	{
	};

	virtual int handle(int command)
	{
		switch(command)
		{
			case FL_SHOW:
				{
					if(!m_win)
					{
						if(window())
						{
							Fl_X* i=Fl_X::i(window());

							m_win=new gzWindow("test",i->xid);

							m_win->setSize(w(),h());
							m_win->setPosition(x(),y());

							gzScene *scene=new gzScene;

							scene->addNode(new MyNodeClass("A test quad"));

							gzCamera *cam=m_win->getCamera();

							cam->setScene(scene);

							cam->setPosition(0.5,0.5,10);
							cam->lookAt(0.5,0.5,0);

							m_win->refreshWindow();
						
						}
					}
				}
				break;
		}
		return Fl_Widget::handle(command);
	}

	virtual void resize(int X, int Y, int W, int H)
	{
		Fl_Widget::resize(X,Y,W,H);
		if(m_win)
		{
			m_win->setSize(w(),h());
			m_win->setPosition(x(),y());
		}

	}

	virtual void draw()
	{
	}

private:

	gzRefPointer<gzWindow>	m_win;
};

int main(int argc, char **argv) {
  Fl_Window *window = new Fl_Window(300,200);
   Test *t=new Test(50,50,200,100);
  window->end();
  window->show(argc, argv);
  return Fl::run();
}

