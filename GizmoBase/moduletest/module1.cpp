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
// File			: module1.cpp
// Module		: 
// Description	: A sample plugin module
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
// Who  Date    Description                                             
//                                                                      
// AMO  991002  Created file  
//
// ******************************************************************************

#define GZ_INSTRUMENT_CODE	// if we want to check some results..
#define GZ_MEM_DEBUG

#include "gzBaseLibrary.h"

const gzString MODULE_ID="test";

// First test 
// allocate some memory that we don't free
gzVoid test1()
{
	int *a=new int;	// Do a memory leak

	gzSleep(40);
}

// Second test 
// simulate to fail allocating some memory 
gzVoid test2()
{
	int *a=0; //new int;	// Simulate result memory allocation failure

	GZ_MODULE_LEVEL_ASSERT_TEXT(MODULE_ID,GZ_MESSAGE_FATAL,a!=0,"Memory allocation failure");

	gzSleep(20);
}

// Main module code
class MinModul : public gzDynamicMethodChain<MinModul,gzModule>
{
public:
	GZ_DECLARE_TYPE_INTERFACE;

	MinModul()
	{
		// Register a version info
		registerMethod(IID_VERSION,IIDS_VERSION,&MinModul::version);
		registerMethod("mupp",&MinModul::mupp);
		setArgCheck("mupp",GZ_DYNAMIC_TYPE_NUMBER);
	}

	gzDynamicType version(GZ_DYNAMIC_ATTRIBUTE_LIST_IMP)
	{
		return gzString("This is the version 3.0");
	}

	gzDynamicType mupp(GZ_DYNAMIC_ATTRIBUTE_LIST_IMP)
	{
		return gzString("This is mupp %f",a0.getNumber());
	}

	gzVoid onEvent( gzModuleEvent event)
	{
		switch(event)
		{
			case GZ_MODULE_ADD:

				GZ_ENTER_PERFORMANCE_SECTION(MODULE_ID);	// Set the module id


				//--------------------------------------------------------------

				GZ_ENTER_PERFORMANCE_SECTION(MODULE_ID+"_test1");

				test1();	// Perform test at loading

				GZ_LEAVE_PERFORMANCE_SECTION;


				//--------------------------------------------------------------


				GZ_ENTER_PERFORMANCE_SECTION(MODULE_ID+"_test2");

				test2();

				GZ_LEAVE_PERFORMANCE_SECTION;

				
				//--------------------------------------------------------------


				GZ_LEAVE_PERFORMANCE_SECTION;			// end module performance tests


				// Do some additional tests

				gzSectionResult res=gzGetSectionResult(MODULE_ID+"_test1");

				GZ_MODULE_LEVEL_ASSERT_TEXT(MODULE_ID,GZ_MESSAGE_FATAL,res.execTotTime<0.03,"Exec time for test1 is too large");

				//gzClearPerformanceSection();

				break;

		}
	};
	
		
	//----------- Clone interface ---------------------------------------

	gzReference *clone() const
	{
		return (gzReference *)new MinModul(*this);
	}
};

GZ_DECLARE_TYPE_CHILD(gzModule,MinModul,"MyModule");

GZ_DECLARE_MODULE(MinModul);

