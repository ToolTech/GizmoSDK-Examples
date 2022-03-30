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
// File			: gzWindowCom.h
// Module		: 
// Description	: Declaration of the CgzWindowCom
// Author		: Anders Modén          
// Product		: Gizmo3D 2.11.76
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


#ifndef __GZWINDOWCOM_H_
#define __GZWINDOWCOM_H_

// Gizmo Includes
#include "gzGizmo3DLibrary.h"



#include "resource.h"       // main symbols
#include <atlctl.h>



/////////////////////////////////////////////////////////////////////////////
// CgzWindowCom
class ATL_NO_VTABLE CgzWindowCom : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IgzWindowCom, &IID_IgzWindowCom, &LIBID_ACTIVEXLib>,
	public CComControl<CgzWindowCom>,
	public IPersistStreamInitImpl<CgzWindowCom>,
	public IOleControlImpl<CgzWindowCom>,
	public IOleObjectImpl<CgzWindowCom>,
	public IOleInPlaceActiveObjectImpl<CgzWindowCom>,
	public IViewObjectExImpl<CgzWindowCom>,
	public IOleInPlaceObjectWindowlessImpl<CgzWindowCom>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CgzWindowCom>,
	public IPersistStorageImpl<CgzWindowCom>,
	public ISpecifyPropertyPagesImpl<CgzWindowCom>,
	public IQuickActivateImpl<CgzWindowCom>,
	public IDataObjectImpl<CgzWindowCom>,
	public IProvideClassInfo2Impl<&CLSID_gzWindowCom, &DIID__IgzWindowComEvents, &LIBID_ACTIVEXLib>,
	public IPropertyNotifySinkCP<CgzWindowCom>,
	public CComCoClass<CgzWindowCom, &CLSID_gzWindowCom>
{
public:
	CgzWindowCom()
	{
		m_pUnkMarshaler = NULL;
		m_bWindowOnly = TRUE;
		m_window=NULL;
	}

	~CgzWindowCom()
	{
	}

DECLARE_GET_CONTROLLING_UNKNOWN()
DECLARE_REGISTRY_RESOURCEID(IDR_GZWINDOWCOM)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CgzWindowCom)
	COM_INTERFACE_ENTRY(IgzWindowCom)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
	COM_INTERFACE_ENTRY(IQuickActivate)
	COM_INTERFACE_ENTRY(IPersistStorage)
	COM_INTERFACE_ENTRY(IDataObject)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

BEGIN_PROP_MAP(CgzWindowCom)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// Example entries
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CgzWindowCom)
	CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CgzWindowCom)
	CHAIN_MSG_MAP(CComControl<CgzWindowCom>)

	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_CREATE,OnCreate)
	MESSAGE_HANDLER(WM_DESTROY,OnDestroy)
	MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)

	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);



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
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
	{
		static const IID* arr[] = 
		{
			&IID_IgzWindowCom,
		};
		for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
		{
			if (InlineIsEqualGUID(*arr[i], riid))
				return S_OK;
		}
		return S_FALSE;
	}

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IgzWindowCom
public:
	STDMETHOD(activateGizmoWindow)();
	STDMETHOD(setRefreshRate)(/*[in]*/ float hz);
	STDMETHOD(setCamera)(/*[in]*/ IgzCameraCom *camera);


private:

	gzWindow				*m_window;

	CComPtr<IgzCameraCom>	m_comCamera;
};

#endif //__GZWINDOWCOM_H_
