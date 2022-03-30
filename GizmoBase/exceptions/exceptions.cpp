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
// File			: exceptions.cpp
// Module		: 
// Description	: Example of different allocations
// Author		: Anders Modén		
// Product		: Gizmo3D 2.11.76
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
// AMO	040209	Created file 	
//
// ******************************************************************************

#include "gzBaseLibrary.h"

// If the system doesn't support exceptions the GZ_NO_EXCEPTIONS is defined as 1.
// You can use the macros GZ_EXCEPTION and GZ_EXCPETION_ELSE to setup your code

gzBool theTestExceptionProcedure1()
{
	gzWarningError error("test",127);

	throw error;

	// If we have exceptions in the system, we will not reach this line.
	if(GZ_NO_EXCEPTIONS)
		return FALSE;

	// or you can use

	GZ_EXCEPTION_ELSE(return FALSE);

	return TRUE;
}

gzVoid theTestExceptionProcedure2()
{
	try
	{
		theTestExceptionProcedure1();
	}
	catch(gzBaseError &error)
	{
		// Get some data
		int a=error.getErrorCode();
		gzString b=error.getErrorString();

		GZMESSAGE(GZ_MESSAGE_WARNING,"Intercepted exception::%s err:%d",b,a);

		// pass it on
		throw;
	}
}


// Custom report function for Warning messages

static gzVoid customReportFunctionForWarnings(const gzWarningError & warning)
{
	GZMESSAGE(GZ_MESSAGE_WARNING,"My Special Warning Reporter::%s err:%d",(const char *)warning.getErrorString(),warning.getErrorCode());
}


int main(int argc , char *argv[] )
{
	try
	{
		gzWarningError::setReportFunction(customReportFunctionForWarnings);

		theTestExceptionProcedure2();
	}
	catch(gzBaseError &error)	// In case of exceptions thrown we want to print the message
	{
 		error.reportError();
	}

	return 0;
}

