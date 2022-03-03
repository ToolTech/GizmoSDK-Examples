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
// File			: gzMessageCom.cpp
// Module		: 
// Description	: Implementation of CgzMessageCom
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
#include "gzMessageCom.h"
#include "gzBase.h"

/////////////////////////////////////////////////////////////////////////////
// CgzMessageCom

STDMETHODIMP CgzMessageCom::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IgzMessageCom
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

gzVoid CgzMessageCom::onMessage(const gzString& sender , gzMessageLevel level , const char *message)
{
	CComBSTR bsender=(const char *)sender;
	CComBSTR bmessage=message;

	Fire_onMessage(bmessage,bsender,(gzInt32)level);
}
