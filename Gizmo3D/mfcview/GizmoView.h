#pragma once

#include "windows.h"

class CGizmoWindow;
class gzSimpleMouseViewControl;

class CGizmoView
{
public:

	// To separate the MFC mem handler from GZ mem handler
	static CGizmoView *	createInstance(HWND hwnd);
	static void			destroyInstance(CGizmoView *instance);

	// Size, position manip
	void resize(int cx, int cy );

	// Utilities
	void load(const TCHAR *filename);

	// Callbacks
	void onKeyDown(unsigned int key);
	void onKeyUp(unsigned int key);

private:

	CGizmoView(HWND hwnd);
	~CGizmoView(void);


	CGizmoWindow *				m_win;
	gzSimpleMouseViewControl *	m_input;
};


class CGizmoControl
{
public:
	void static initialize();
	void static uninitialize();
};
