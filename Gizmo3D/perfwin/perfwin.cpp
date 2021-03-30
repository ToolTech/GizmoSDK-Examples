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
// File			: perfwin.cpp
// Module		: 
// Description	: Test app for image rendering to a texture
// Author		: Anders Modén          
// Product		: Gizmo3D 2.10.9
//              
//
//                      
// NOTE:	Gizmo3D is a high performance 3D Scene Graph and effect visualisation 
//			C++ toolkit for Linux, Mac OS X, Windows (Win32) and IRIX® for  
//			usage in Game or VisSim development.
//
//
// Revision History...                                                  
//                                                                      
// Who  Date    Description                                             
//                                                                      
// AMO  050209  Created file    
//
// ******************************************************************************

#include "gzGizmo3DLibrary.h"


int main(int argc, char *argv[])
{
	gzTime::useHighSpeedClock(TRUE);

 	gzArray<gzString> sections(2);

	sections[0]="outer";
	sections[1]="inner";


	gzRefPointer<gzWindow> win=gzCreatePerformanceWindow(sections);

	gzApplication app;

	for(gzUInt32 i=0;i<10000;i++)
	{

		gzEnterPerformanceSection(sections[0]);

			gzEnterPerformanceSection(sections[1]);

			gzSleep(1);	// Make a small sleep in inner section

			gzLeavePerformanceSection();

		gzSleep(1);		// Make a small sleep only in outer section

		gzLeavePerformanceSection();

		app.poll();		// Will take a couple of percent 
	}

	return 0;
}
