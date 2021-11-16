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
// File			: gzgetenv.cpp
// Module		:
// Description	: Example that creates a getenv command for GizmoSDK registry
// Author		: Anders Modén
// Product		: GizmoBase 2.11.48
//
//
//
// NOTE:	GizmoBase is a platform abstraction utility layer for C++. It contains 
//			design patterns and C++ solutions for the advanced programmer.
//
//
// Revision History...
//
// Who	Date	Description
//
// AMO	010720	Created file
//
// ******************************************************************************
#include "gzBaseLibrary.h"
#include <iostream>


int main(int argc , char *argv[] )
{
	try		// To catch all Gizmo3D exceptions
	{
		gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG);

		if(argc!=2)
		{
		    std::cout<<"Usage: "<<argv[0]<<" <envname>"<<std::endl;
			exit(0);
		}

		std::cout<<(const char *)gzKeyDatabase::getUserKey(argv[1])<<std::endl;

	}
	catch(gzBaseError &error)	// In case of exceptions thrown we want to print the message
	{
		error.reportError();
	}

	return 0;
}
