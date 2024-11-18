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
// File			: smartpointer.cpp
// Module		: 
// Description	: An example of smart pointers for memory management
// Author		: Anders Moden          
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
// AMO  020227  Created file  
//
// ******************************************************************************
#define GZ_MEM_DEBUG	// Before all includes !

#include "gzBaseLibrary.h"


class MyRefData : public gzReference
{
public:
	MyRefData()
	{
		// init
	}
	
	virtual ~MyRefData()
	{
		// Don't forget the virtual destructor
	}

	// Some data
	gzInt32	nisse;
	
};

GZ_DECLARE_REFPTR(MyRefData);	// Add a smartp ointer declaration

class MyData 
{
public:
	MyData()
	{
		// init
	}
	
	virtual ~MyData()
	{
		// Don't forget the virtual destructor
	}

	// Some data
	gzInt32	nisse;
	
};

int main(int argc , char *argv[] )
{
	try
	{
		gzMessage::setMessageLevel(GZ_MESSAGE_MEM_DEBUG);
		
		gzMemoryControl::traceAlloc(TRUE);
	
		// Create a reference "smart" pointer to the class
		MyRefDataPtr pMyRefData;
	
		// Allocate an instance
		pMyRefData = new MyRefData;
		
		pMyRefData->nisse=10;

		//delete pMyRefData;
		
		// And some non referenced managed data, use RefData
		
		gzRefPointer< gzRefData< MyData> > pMyData;
		
		pMyData=new gzRefData< MyData >;
		
		pMyData->nisse=27;
		
		// No Need to clean up data. It is all managed by the RefPointers

		// Note that there are some default memory caches that need to be cleared

		//gzRecycleData_gzBase<gzString::disable();
	}
	catch(gzBaseError &error)
	{
		error.reportError();
	}
	catch(...)
	{
	}

	return 0;
}
