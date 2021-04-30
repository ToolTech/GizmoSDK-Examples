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
// File			: purl.cpp
// Module		: 
// Description	: Starts a pipe url server
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
// AMO	040205	Created file 	
//
// ******************************************************************************

#include "gzBaseLibrary.h"


gzLoggerPtr logger_save;
gzLoggerPtr logger_con;

static class cleaner 
{
public:

	virtual ~cleaner()
	{
		gzModule::unLoadModules();
	}

} _the_cleaner;

int main(int argc , char *argv[] )
{
	try
	{
		gzAttachConsole();

		gzArgumentParser args(argc, argv);

		args.setSyntaxString("<pipename> {-logg} {-debug} {-absolute} {-write} {-urlbases <base;base>} {-options <optionstring>} {-base <base path>}");

		if(args.hasOption("debug"))
			gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG | GZ_MESSAGE_API_INTERNAL);

		gzString options = gzRegKeyExpandedString(args.getOptionValue("options", GZ_EMPTY_STRING));

		gzString basePath = args.getOptionValue("base", GZ_EMPTY_STRING);

		gzString plugins = args.getOptionString("plugins");

		gzString urlBases=args.getOptionString("urlbases");

		args.checkArgumentCount(2);
		
		GZMESSAGE(GZ_MESSAGE_NOTICE, "* Starting purl:%s %s\tVersion:%f", args.getArgument(1),options, GZ_PURL_PROTOCOL_VERSION/100.0);

		gzPipeURLServer server(args.getArgument(1),options);

		server.setBasePath(basePath);

		if (args.hasOption("absolute"))
			server.setAllowAbsoluteURL(TRUE);

		if (args.hasOption("write"))
			server.setAllowWrite(TRUE);

		if (urlBases.length())
		{
			server.setAllowedURLBases(urlBases);
		}

		if (args.hasOption("logg"))
		{
			logger_save = new gzLogger;
			logger_con = new gzLogger("con:");
		}
		
		if (plugins.length())
			gzModule::loadModules(plugins, FALSE, TRUE, TRUE);

		server.run();

		gzSleep(GZ_SLEEP_INFINITE);
	}
	catch(gzBaseError &error)	// In case of exceptions thrown we want to print the message
	{
 		error.reportError();
	}

	
	return 0;
}

