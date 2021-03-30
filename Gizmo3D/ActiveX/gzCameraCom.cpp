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
// File			: gzCameraCom.cpp
// Module		: 
// Description	: Implementation of CgzCameraCom
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
// AMO  030420  Created file    
//
// ******************************************************************************

#include "stdafx.h"
#include "ActiveX.h"
#include "gzCameraCom.h"
#include "gzSceneCom.h"

/////////////////////////////////////////////////////////////////////////////
// CgzCameraCom

STDMETHODIMP CgzCameraCom::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IgzCameraCom
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CgzCameraCom::lookAt(float x, float y, float z)
{
	if(m_camera.get())
		m_camera.get()->lookAt(x,y,z);


	return S_OK;
}

STDMETHODIMP CgzCameraCom::setScene(IgzSceneCom *scene)
{
	m_comScene=scene;

	if(m_camera.get())
	{
		if(scene)
			m_camera.get()->setScene(((CgzSceneCom *)scene)->m_scene.get());
		else
			m_camera.get()->setScene(NULL);
	}


	return S_OK;
}

STDMETHODIMP CgzCameraCom::setPosition(float x, float y, float z)
{
	// TODO: Add your implementation code here

	if(m_camera.get())
		m_camera.get()->setPosition(x,y,z);

	return S_OK;
}

STDMETHODIMP CgzCameraCom::setNearFar(float nearDist, float farDist)
{
	if(m_camera.get())
	{
		m_camera.get()->setNearClipPlane(nearDist);

		m_camera.get()->setFarClipPlane(farDist);
	}

	return S_OK;
}


