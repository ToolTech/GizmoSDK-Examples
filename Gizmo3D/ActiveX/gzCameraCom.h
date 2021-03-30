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
// File			: gzCameraCom.h
// Module		: 
// Description	: Declaration of the CgzCameraCom
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

#ifndef __GZCAMERACOM_H_
#define __GZCAMERACOM_H_

#include "gzCamera.h"

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CgzCameraCom
class ATL_NO_VTABLE CgzCameraCom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CgzCameraCom, &CLSID_gzCameraCom>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CgzCameraCom>,
	public IDispatchImpl<IgzCameraCom, &IID_IgzCameraCom, &LIBID_ACTIVEXLib>
{
public:
	CgzCameraCom()
	{
		m_pUnkMarshaler = NULL;

		m_camera=new gzPerspCamera;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_GZCAMERACOM)
DECLARE_GET_CONTROLLING_UNKNOWN()

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CgzCameraCom)
	COM_INTERFACE_ENTRY(IgzCameraCom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CgzCameraCom)
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

// IgzCameraCom
public:
	STDMETHOD(setNearFar)(/*[in]*/ float near , /*[in]*/ float far);
	STDMETHOD(setPosition)(/*[in]*/ float x , /*[in]*/ float y , /*[in]*/ float z);
	STDMETHOD(setScene)(/*[in]*/ IgzSceneCom *scene);
	STDMETHOD(lookAt)(/*[in]*/ float x , /*[in]*/ float y , /*[in]*/ float z);

	gzRefPointer<gzCamera>	m_camera;

	CComPtr<IgzSceneCom>	m_comScene;
};

#endif //__GZCAMERACOM_H_
