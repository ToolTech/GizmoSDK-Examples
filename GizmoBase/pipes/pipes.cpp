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
// Product		: GizmoBase 2.12.199
//              
//
// NOTE:	GizmoBase is a platform abstraction utility layer for C++. It contains 
//			design patterns and C++ solutions for the advanced programmer.
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
	// Create one pipe server that multiplexes data between pipes named "gizmo"
	gzPipeServer server("gizmo");

	server.run();

	gzSleep(100);	// Wait for pipe creation as we only try once to connect

	// Create two serializers
	gzSerializeAdapterPtr adapter1=gzSerializeAdapter::getURLAdapter("pipe:gizmo");
	gzSerializeAdapterPtr adapter2=gzSerializeAdapter::getURLAdapter("pipe:gizmo");

	gzSleep(100);	// Wait for pipe connection as we in this test expect a received data

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
	}

	// terminate and wait for all pipes to shut down
	server.stop(TRUE);

	return 0;
}
