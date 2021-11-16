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
// File			: pipes.cpp
// Module		: 
// Description	: Test app pipe communication between two serializers
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
// AMO  030919  Created file    
//
// ******************************************************************************
#include "gzBaseLibrary.h"

int main()
{
	// Create one pipe server that multiplexes data between pipes named "gizmo3d"
	gzPipeServer server("gizmo3d");

	server.run();

	gzSleep(1000);	// Wait for pipe creation


	// Create two serializers
	gzSerializeAdapter *adapter1=gzSerializeAdapter::getURLAdapter("pipe:gizmo3d");
	gzSerializeAdapter *adapter2=gzSerializeAdapter::getURLAdapter("pipe:gizmo3d");

	if(adapter1 && adapter2)
	{
		// write a string to pipe
		gzSerializeString a=gzString("test");

		a.write(adapter1);

		// wait for data
		while(!adapter2->hasData())
			gzSleep(0);

		// read data
		gzSerializeString b;

		b.read(adapter2);

		GZMESSAGE(GZ_MESSAGE_NOTICE,"Got data '%s' from pipe",(const char *)(gzString)b);

		delete adapter1;

		delete adapter2;

	}

	// terminate and wait for all pipes to shut down
	server.stop(TRUE);

	return 0;
}
