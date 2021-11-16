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
// Product		: Gizmo3D 2.11.48
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
//
// ******************************************************************************
#include "gzGizmo3DLibrary.h"

#include <iostream>

int main(int argc , char *argv[] )
{
	gzStartUpGizmo();	// Needed for some systems to install external graphics engines

	try		// To catch all Gizmo3D exceptions
	{
		if((argc!=3) && (argc!=4) && (argc!=5))
		{
			std::cout<<"Syntax: gzconv [infile] [outfile] {loadflags=0} {saveflags=0}"<<std::endl;
			exit(0);
		}

		gzInt32 loadflags=0,saveflags=0;

		if(argc>=4)
			loadflags=atol(argv[3]);

		if(argc==5)
			saveflags=atol(argv[4]);

		gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG);

		gzInitializeDbManagers();

		gzModule::loadModules();

		gzNode *node=gzDbManager::loadDB(argv[1],GZ_EVALUATE_EXTENSION , GZ_DB_FLAGS_DEFAULT, loadflags);

		if(node)
			gzDbManager::saveDB(node , argv[2] , GZ_EVALUATE_EXTENSION , GZ_DB_FLAGS_DEFAULT, saveflags );

	}
	catch(gzBaseError &error)	// In case of exceptions thrown we want to print the message
	{
		error.reportError();
	}

	gzShutDownGizmo();

	return 0;
}
