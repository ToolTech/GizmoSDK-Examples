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
// File			: PurlServer.cpp
// Module		: 
// Description	: Starts a pipe url server
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
// AMO	241114	Created file 								(2.12.199)
//
// ******************************************************************************
#pragma once

class PurlService : public gzService
{
public:

	PurlService();

	virtual ~PurlService();

	gzUInt32 getExitCode() const override;

	virtual gzVoid process(const gzArgumentParser& args, gzBool runAsService) override;

	GZ_PROPERTY(gzPipeURLServerPtr, Server);
};

gzString getSetting(const gzArgumentParser &args, const gzString& settingName,const gzString &defaultValue);
