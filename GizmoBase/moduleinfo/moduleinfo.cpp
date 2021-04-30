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
// File			: moduleinfo.cpp
// Module		: 
// Description	: Shows info about a specific module
// Author		: Anders Modén		
// Product		: Gizmo3D 2.11.1
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
// AMO	040303	Created file 	
//
// ******************************************************************************
#include "gzBaseLibrary.h"

int main(int argc , char *argv[] )
{
	try		// To catch all Gizmo3D exceptions
	{
		gzArgumentParser arg(argc,argv);

		arg.setSyntaxString("<modulename>");

		arg.checkArgumentCount(2);

		gzModule *pModule=gzModule::getModule(arg.getArgument(1));

		if(!pModule)
		{
			GZMESSAGE(GZ_MESSAGE_NOTICE,"Module <%s> couldn't be loaded.",(const char *)arg.getArgument(1));
			return 0;
		}

		GZMESSAGE(GZ_MESSAGE_NOTICE,"---- Info for Module < %s > ----",(const char *)arg.getArgument(1));

		gzArray<gzDynamicMethodID> methodInfo=pModule->queryAllMethodIID();

		GZMESSAGE(GZ_MESSAGE_NOTICE,"Module has %d registered methods",methodInfo.getSize());

		if(pModule->supportMethod(IID_VERSION))
		{
			gzDynamicType dyn_version=pModule->invokeMethod(IID_VERSION);

			gzString version="No Proper Version Info";

			if(dyn_version.is(GZ_DYNAMIC_TYPE_STRING))
				version=dyn_version.getString();

			GZMESSAGE(GZ_MESSAGE_NOTICE,"Module has a VERSION method with info [ %s ]",(const char *)version);
		}

		for(gzUInt32 i=0;i<methodInfo.getSize();i++)
		{
			GZMESSAGE(GZ_MESSAGE_NOTICE,"-----------------------------------------------------------");
			GZMESSAGE(GZ_MESSAGE_NOTICE,"Method Name    : %s",(const char *)methodInfo[i].IIDS_method);
			GZMESSAGE(GZ_MESSAGE_NOTICE,"Method ID      : %d",methodInfo[i].IID_method);
			GZMESSAGE(GZ_MESSAGE_NOTICE,"Method RAW     : 0x%x",gzPtr2Val((const gzVoid *)methodInfo[i].method));
			GZMESSAGE(GZ_MESSAGE_NOTICE,"Method Arg Chk : %s,%s,%s,%s,%s,%s,%s,%s,%s,%s",	(const char *)methodInfo[i].argTypeCheck[0],
																							(const char *)methodInfo[i].argTypeCheck[1],
																							(const char *)methodInfo[i].argTypeCheck[2],
																							(const char *)methodInfo[i].argTypeCheck[3],
																							(const char *)methodInfo[i].argTypeCheck[4],
																							(const char *)methodInfo[i].argTypeCheck[5],
																							(const char *)methodInfo[i].argTypeCheck[6],
																							(const char *)methodInfo[i].argTypeCheck[7],
																							(const char *)methodInfo[i].argTypeCheck[8],
																							(const char *)methodInfo[i].argTypeCheck[9]);
		}

		GZMESSAGE(GZ_MESSAGE_NOTICE,"---- End of Module   < %s > ----",argv[1]);


		gzModule::unLoadModules();
	}
	catch(gzBaseError &error)	// In case of exceptions thrown we want to print the message
	{
		error.reportError();
	}

	return 0;
}
