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
// File			: moduleTest.cpp
// Module		: 
// Description	: Test usage of compiled plugin module
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
// Who  Date    Description                                             
//                                                                      
// AMO  991002  Created file  
//
// ******************************************************************************

#include "gzBaseLibrary.h"

// Note !! Add "module1" as argument to debugger

gzVoid testModule(const gzString &moduleName)
{
	GZMESSAGE(GZ_MESSAGE_DEBUG,"----- Testing module '%s' --------",(const char *)moduleName);

	gzEnterPerformanceSection(moduleName);

	gzMemoryControl::updateState(2);

	gzModule *module=gzModule::getModule(moduleName);

	if(module)
	{
		module->unLoad(); 

		gzUInt64 a=gzMemoryControl::getAllocMem(2);

		if(a)
			gzMemoryControl::dumpAllocMem(2);
	}

	gzMemoryControl::cleanAllocMem();

	gzMemoryControl::updateState(1);

	gzLeavePerformanceSection();

}

int main(int argc , char *argv[] )
{
	try
	{ 
		gzMessage::setMessageLevel(GZ_MESSAGE_MEM_DEBUG);

		gzStartPerformanceThread();

		GZMESSAGE(GZ_MESSAGE_NOTICE,"Module testing software\n");

		gzMessageDatabase messageDataBase;

		gzMemoryControl::debugMem(TRUE);

		for(int i=1;i<argc;i++)
		{
			testModule(gzString(argv[i]));
		}

		gzMemoryControl::debugMem(FALSE);

		gzMemoryControl::cleanAllocMem();

		gzStopPerformanceThread();

		gzDumpPerformanceInfo(GZ_PERF_DUMP_ALL);	// If you want to dump all performance data
	}
	catch(gzBaseError &error)
	{
		error.reportError();
	}

	return 0;
}


