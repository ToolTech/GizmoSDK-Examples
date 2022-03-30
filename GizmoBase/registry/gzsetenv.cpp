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
// File			: gzsetenv.cpp
// Module		:
// Description	: Example that creates a setenv command for GizmoSDK registry
// Author		: Anders Modén
// Product		: GizmoBase 2.11.76
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

		if(argc!=3)
		{
		    std::cout<<"Usage: "<<argv[0]<<" <envname> <value>"<<std::endl;
			exit(0);
		}

		gzKeyDatabase::setUserKey(argv[1],gzString(argv[2]).strip('"'));

		std::cout<<"Reg Value set Ok !"<<std::endl;

	}
	catch(gzBaseError &error)	// In case of exceptions thrown we want to print the message
	{
		error.reportError();
	}

	return 0;
}
