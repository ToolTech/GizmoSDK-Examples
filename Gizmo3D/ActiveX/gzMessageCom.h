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
// File			: gzMessageCom.h
// Module		: 
// Description	: Declaration of the CgzMessageCom
// Author		: Anders Modén          
// Product		: Gizmo3D 2.12.199
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


#ifndef __GZMESSAGECOM_H_
#define __GZMESSAGECOM_H_

#include "gzDebug.h"

#include "resource.h"       // main symbols
#include "ActiveXCP.h"

/////////////////////////////////////////////////////////////////////////////
// CgzMessageCom
class ATL_NO_VTABLE CgzMessageCom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CgzMessageCom, &CLSID_gzMessageCom>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CgzMessageCom>,
	public IDispatchImpl<IgzMessageCom, &IID_IgzMessageCom, &LIBID_ACTIVEXLib>,
	public gzMessageReceiverInterface,
	public CProxy_IgzMessageComEvents< CgzMessageCom >
{
public:
	CgzMessageCom()
	{
		m_pUnkMarshaler = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_GZMESSAGECOM)
DECLARE_GET_CONTROLLING_UNKNOWN()

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CgzMessageCom)
	COM_INTERFACE_ENTRY(IgzMessageCom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
	COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CgzMessageCom)
CONNECTION_POINT_ENTRY(DIID__IgzMessageComEvents)
END_CONNECTION_POINT_MAP()


	HRESULT FinalConstruct()
	{
		gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG);
		gzMessage::addMessageReceiver(this);

		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}

	void FinalRelease()
	{
		gzMessage::removeMessageReceiver(this);
		m_pUnkMarshaler.Release();
	}

	virtual gzVoid onMessage(const gzString& sender , gzMessageLevel level , const char *message);


	CComPtr<IUnknown> m_pUnkMarshaler;

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IgzMessageCom
public:
};

#endif //__GZMESSAGECOM_H_
