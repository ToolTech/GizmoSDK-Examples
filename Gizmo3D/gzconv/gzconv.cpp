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
// File			: gzconv.cpp
// Module		: 
// Description	: Test implementation of messaging
// Author		: Anders Modén		
// Product		: Gizmo3D 2.12.199
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
// AMO	010910	Created file 	
// AMO	230309	Updated example to help converting to .gzd and .gzl files	(2.12.67)
//
// ******************************************************************************
#include "gzGizmo3DLibrary.h"

const char syntax[] =
"'infileURL' 'outfileURL' {-optimize} {-plugins <a;b;c>} {-config cfg_url} {-loadflags nn} {-saveflags nn} {-optimizeflags nn} {-exclude aa;bb;cc}\n"	\
"\t-optimize		: Run NodeOptimizer before file is saved\n"				\
"\t-plugins			: Load list of plugins before converting\n"				\
"\t-config			: Load xml config into registry\n"						\
"\t-saveflags		: set of gzSerializeAdapterFlags for saving\n"			\
"\t-loadflags		: set of gzSerializeAdapterFlags for loading\n"			\
"\t-optimizeflags   : set of gzNodeOptimizeLevel for optimizing\n"			\
"\t-exclude			: list of nodenames to be excluded from opt\n"			\
"\t-path			: list of paths to be added\n"							\
;

int main(int argc , char *argv[] )
{
	gzStartUpGizmo();	// Needed for some systems to install external graphics engines

	try		// To catch all Gizmo3D exceptions
	{
		gzArgumentParser args(argc, argv);

		args.setSyntaxString(syntax);
		args.exitOnError();
				
		if(args.hasOption("debug"))
			gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG|GZ_MESSAGE_API_INTERNAL);

		gzString config_url = args.getOptionValue("config", GZ_EMPTY_STRING);

		if (config_url.length())
			gzKeyDatabase::setLocalRegistry(config_url);

		gzString path= args.getOptionValue("path", GZ_EMPTY_STRING);

		if (path.length())
		{
			gzString currentPath = gzKeyDatabase::getUserKey("GIZMO_PATH");

			gzString newPath = gzString::formatString("%s;%s", path, currentPath);

			gzPutEnv("GIZMO_PATH", newPath);
		}

		gzInitializeImageManagers();
		gzInitializeDbManagers();


		gzInt32 loadflags		= args.getOptionValue("loadflags", gzKeyDatabase::getDefaultUserKey("gzConv/loadflags", GZ_DB_FLAGS_FLIP_FLIPPED_IMAGES)).num32();
		gzInt32 saveflags		= args.getOptionValue("saveflags", gzKeyDatabase::getDefaultUserKey("gzConv/saveflags", GZ_DB_FLAGS_DEFAULT)).num32();

		gzString plugins		= args.getOptionValue("plugins",gzKeyDatabase::getUserKey("gzConv/plugins"));
		gzString excludedNodes	= args.getOptionValue("exclude", gzKeyDatabase::getUserKey("gzConv/exclude"));

		gzNodeOptimizeLevel level = (gzNodeOptimizeLevel)args.getOptionValue("optimizeflags", gzKeyDatabase::getDefaultUserKey("gzConv/optimizeflags", GZ_NODE_OPTIMIZE_DEFAULT)).num32();

		if (plugins.length())
			gzModule::loadModules(plugins, FALSE, TRUE, TRUE);

		args.checkArgumentCount(3);

		gzNodePtr node=gzDbManager::loadDB(args.getArgument(1),GZ_EVALUATE_EXTENSION ,loadflags);

		if (node)
		{
			gzDbInfoStatusBits status;

			if (gzDynamic_Cast(gzGetNodeDbInfo(node, GZ_DB_INFO_STATUS), status))
			{
				if (status & GZ_DB_INFO_STATUS_MISSING_TEXTURE)
					printf("Detected Missing Texture\n");

				if (status & GZ_DB_INFO_STATUS_UNKNOW_DATA)
					printf("Detected Unknown Data\n");
			}

			if (args.hasOption("optimize"))
			{
				gzNodeOptimizer optimizer;

				optimizer.exclude(excludedNodes);

				node = optimizer.optimize(node,level);
			}

			if (node)
			{
				if (!gzDbManager::saveDB(node, args.getArgument(2), GZ_EVALUATE_EXTENSION, saveflags))
					printf("!!!! Failed to save '%s'  !!!!\n", (const char*)args.getArgument(2));
				else
					printf("Ok! Saved '%s'\n", (const char *)args.getArgument(2));
			}
		}

	}
	catch(gzBaseError &error)	// In case of exceptions thrown we want to print the message
	{
		error.reportError();
	}

	gzShutDownGizmo();

	gzPutEnv("GIZMO_PATH", GZ_EMPTY_STRING);	// Clean path

	return 0;
}
