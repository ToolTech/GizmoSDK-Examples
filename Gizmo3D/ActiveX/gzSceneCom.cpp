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
// File			: gzSceneCom.cpp
// Module		: 
// Description	: Implementation of CgzSceneCom
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
// AMO  030420  Created file    
//
// ******************************************************************************

// gzSceneCom.cpp : 
#include "stdafx.h"
#include "ActiveX.h"
#include "gzSceneCom.h"
#include "gzGizmo3DLibrary.h"

/////////////////////////////////////////////////////////////////////////////
// CgzSceneCom

STDMETHODIMP CgzSceneCom::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IgzSceneCom
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CgzSceneCom::loadURL(BSTR url)
{
	gzString urlName(url);

	gzNode *gizmo=NULL;

	try
	{
		gizmo=gzDbManager::loadDB(urlName,GZ_EVALUATE_EXTENSION,GZ_DB_FLAGS_USE_ANIMATION|GZ_DB_FLAGS_USE_MIPMAPS	);
	}
	catch(gzBaseError &err)
	{
		err.reportError();
		return S_OK;
	}
	
	if(gizmo)
	{
	
		gzEnvironment *env=new gzEnvironment;

		gzLight *light=new gzLight;

		light->setPosition(0,0,100);

		light->setSpecularColor(0.4f,0.4f,0.5f);

		env->addLight(light);

		gzTransform *transform=new gzTransform;


		if(gizmo->isOfType(gzFrame::getClassType()))
		{
			gzFrame *frame=(gzFrame *)gizmo;

			frame->setFrameDuration(0.05);
			frame->startFrameCounter();
		}
	
		transform->addNode(gizmo);

		if(m_useAutoScale)
			transform->unitScale();

		transform->scale(m_scale,m_scale,m_scale);

		env->addNode(transform);

		m_scene.get()->addNode(env);
	}
	
	return S_OK;
}

STDMETHODIMP CgzSceneCom::setScale(float scale)
{
	m_scale=scale;
	return S_OK;
}

STDMETHODIMP CgzSceneCom::useAutoScale(int on)
{
	m_useAutoScale=on;

	return S_OK;
}
