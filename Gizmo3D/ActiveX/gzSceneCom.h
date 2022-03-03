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
// File			: gzSceneCom.h
// Module		: 
// Description	: Declaration of the CgzSceneCom
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

#ifndef __GZSCENECOM_H_
#define __GZSCENECOM_H_

#include "gzScene.h"

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CgzSceneCom
class ATL_NO_VTABLE CgzSceneCom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CgzSceneCom, &CLSID_gzSceneCom>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CgzSceneCom>,
	public IDispatchImpl<IgzSceneCom, &IID_IgzSceneCom, &LIBID_ACTIVEXLib>
{
public:
	CgzSceneCom()
	{
		m_pUnkMarshaler = NULL;
		m_scene = new gzScene;
		m_useAutoScale=FALSE;
		m_scale=1;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_GZSCENECOM)
DECLARE_GET_CONTROLLING_UNKNOWN()

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CgzSceneCom)
	COM_INTERFACE_ENTRY(IgzSceneCom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CgzSceneCom)
END_CONNECTION_POINT_MAP()


	HRESULT FinalConstruct()
	{
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}

	void FinalRelease()
	{
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IgzSceneCom
public:
	STDMETHOD(useAutoScale)(/*[in]*/ int on);
	STDMETHOD(setScale)(/*[in]*/ float scale);
	STDMETHOD(loadURL)(/*[in]*/ BSTR url);

	gzRefPointer<gzScene>	m_scene;

	gzBool					m_useAutoScale;

	gzFloat					m_scale;
};

#endif //__GZSCENECOM_H_
