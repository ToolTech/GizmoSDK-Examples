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
// File			: stdafx.h
// Module		: 
// Description	: include file for standard system include files,
//				  or project specific include files that are used frequently,
//				  but are changed infrequently
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

#if !defined(AFX_STDAFX_H__C9C2FEE8_2A45_4EEF_9B26_AD5737044DB0__INCLUDED_)
#define AFX_STDAFX_H__C9C2FEE8_2A45_4EEF_9B26_AD5737044DB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include <atlctl.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__C9C2FEE8_2A45_4EEF_9B26_AD5737044DB0__INCLUDED)
