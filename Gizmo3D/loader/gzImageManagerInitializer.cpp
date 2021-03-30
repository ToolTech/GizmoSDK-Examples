//*****************************************************************************
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
// File			: gzImageManagerInitializer.cpp
// Module		: 
// Description	: Utilities for image management
// Author		: Anders Modén		
// Product		: Gizmo3D 2.10.9
//		
//
//			
// NOTE:	The GIZMO package defines a general purpose API for large model
//			visualisation and advanced graphic behaviours. The package is similar
//			to Cosmo3D, Inventor, Performer etc. but adds automated behaviour
//			patterns to the graphics. 
//
//
// Revision History...							
//									
// Who	Date	Description						
//									
// AMO	990301	Created file 	
//
//******************************************************************************

#include "gzImageManagerInitializer.h"
#include "gzImage.h"

//--------------------- defined managers by default -------------------------------

#include "gzImageManager_bmp.h"

gzVoid gzInitializeImageManagers()
{
	static gzBool isInitialized=FALSE;

	//---------- add managers here -------------------------

	if(!isInitialized)
	{
		gzImageManager::registerManager(new gzImageManager_bmp);
		isInitialized=TRUE;
	}

}
