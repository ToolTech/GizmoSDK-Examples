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
// File			: machid.cpp
// Module		: 
// Description	: Generates machine id for GIZMO license
// Author		: Anders Modén		
// Product		: GizmoBase 2.11.70
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
// AMO	990610	Created file 	
//
// ******************************************************************************

#include "gzBaseLibrary.h"

#include <iostream>

// This is it ! No more ! No Less !

int main(int argc , char *argv[] )
{
	try
	{
		gzMessage::setMessageLevel(GZ_MESSAGE_NOTICE);

		gzLicense::notifyDefaultLicense();

		char buffer[30];

		gzULtoA(buffer,gzLicense::getMachineID(),10); 

		std::cout<<std::endl<<"Machine id : "<<buffer<<std::endl<<std::endl;
	
	}
	catch(gzBaseError &error)	// In case of exceptions thrown we want to print the message
	{
 		error.reportError();
	}

	return 0;
}

// Use this id in request for license key

