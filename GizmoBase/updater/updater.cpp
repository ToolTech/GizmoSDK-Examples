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
// File			: updater.cpp
// Module		: 
// Description	: Updates files
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
// AMO	200402	Created file 	
//
// ******************************************************************************

#include "gzBaseLibrary.h"

gzBool copyFile(const gzString& purl, const gzString &nic,const gzString& sourcePath, const gzString& fileName, const gzString& destPath=".",gzBool overWrite = FALSE)
{
	gzString destFileName = gzString::formatString("%s/%s", destPath, fileName);

	if (!overWrite && gzFileExist(destFileName))
		return TRUE;

	gzSerializeAdapterPtr from = gzSerializeAdapter::getURLAdapter(gzString::formatString("purl:%s?nic=%s,%s/%s", purl, nic,sourcePath, fileName));

	if (!from)
		return FALSE;


	gzSerializeAdapterPtr to = gzSerializeAdapter::getURLAdapter(destFileName,GZ_SERIALIZE_OUTPUT);

	if (!to)
		return FALSE;

	gzString error;

	if (!copyAdapters(to, from, &error)) 
		return FALSE;

	return TRUE;
}

int main(int argc , char *argv[] )
{
	try
	{
		gzAttachConsole();

		gzArgumentParser args(argc, argv);

		if(args.hasOption("debug"))
			gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG | GZ_MESSAGE_API_INTERNAL);

		gzString purl = args.getOptionValue("purl", "maps");			// Default to "maps" purl

		gzString nic = args.getOptionValue("nic", getLocalHostAddress().asString());

		gzString destPath = args.getOptionValue("dest", "./");			// Default to current directory

		copyFile(purl, nic, "SceneBuilder", "gzBase64.dll", destPath,TRUE);
		copyFile(purl, nic, "SceneBuilder", "SceneBuilderClient64.exe", destPath,TRUE);
		copyFile(purl, nic, "SceneBuilder", "run_me_as_admin.bat", destPath,FALSE);
		copyFile(purl, nic, "SceneBuilder", "SceneBuilderClient.xml", destPath,FALSE);
		copyFile(purl, nic, "SceneBuilder", "vcruntime140.dll", destPath,FALSE);
		copyFile(purl, nic, "SceneBuilder", "vcruntime140_1.dll", destPath,FALSE);
		copyFile(purl, nic, "SceneBuilder", "updater.exe", destPath, TRUE);
	}
	catch(gzBaseError &error)	// In case of exceptions thrown we want to print the message
	{
 		error.reportError();
	}

	return 0;
}

