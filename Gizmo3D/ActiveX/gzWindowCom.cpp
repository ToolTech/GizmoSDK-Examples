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
// File			: gzWindowCom.cpp
// Module		: 
// Description	: Implementation of CgzWindowCom
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
// AMO  030420  Created file    
//
// ******************************************************************************

#include "stdafx.h"
#include "ActiveX.h"
#include "gzGizmo3DLibrary.h"
#include "gzWindowCom.h"
#include "gzCameraCom.h"

/////////////////////////////////////////////////////////////////////////////
// CgzWindowCom

LRESULT CgzWindowCom::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// Previously the startup code was located here but we moved the code so the active x view doesn't conflict with VB  reg on certian drivers
	return S_OK;
}

LRESULT CgzWindowCom::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(m_window)
		delete m_window;

	return S_OK;
}

LRESULT CgzWindowCom::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(m_window)
		::SetFocus((HWND)m_window->getHandle());

	return S_OK;
}

LRESULT CgzWindowCom::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	WORD cx = LOWORD(lParam);
	WORD cy = HIWORD(lParam);

	if(m_window)
	{
		m_window->setSize(cx,cy);
	}

	return S_OK;
}

STDMETHODIMP CgzWindowCom::setCamera(IgzCameraCom *camera)
{
	// TODO: Add your implementation code here

	m_comCamera=camera;


	if(m_window)
	{
		if(camera)
			m_window->setCamera(((CgzCameraCom *)camera)->m_camera.get());
		else
			m_window->setCamera(NULL);
	}

	return S_OK;
}

STDMETHODIMP CgzWindowCom::setRefreshRate(float hz)
{
	if(m_window)
		m_window->setRefreshRate(hz);


	return S_OK;
}

STDMETHODIMP CgzWindowCom::activateGizmoWindow()
{
	gzGraphicsEngine::useEngine(GZ_ENGINE_OPENGL);

	gzInitializeDbManagers();

	m_window=new gzWindow("gzWindowCom",m_hWnd);

	m_window->setRefreshRate(100);

	return S_OK;
}
