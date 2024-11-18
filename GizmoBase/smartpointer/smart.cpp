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
// File			: serialize.cpp
// Module		: 
// Description	: Test implementation of serialization
// Author		: Anders Modén		
// Product		: GizmoBase 2.12.199
//		
// NOTE:	GizmoBase is a platform abstraction utility layer for C++. It contains 
//			design patterns and C++ solutions for the advanced programmer.
//
//
// Revision History...							
//									
// Who	Date	Description						
//									
// AMO	990919	Created file 	
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

GZ_DECLARE_REFPTR(MyRefData);	// Add a smart-pointer declaration

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
		// Adjust the level so we get output from memory traces
		gzMessage::setMessageLevel(GZ_MESSAGE_MEM_DEBUG| GZ_MESSAGE_API_INTERNAL);

		// Enable trace of allocation
		gzMemoryControl::traceAlloc(TRUE);

		// Initiate a memory allocation block so we can count allocations
		gzMemoryControl::dumpAllocMem();
					
		// Create a reference "smart" pointer to the class
		MyRefDataPtr pMyRefData;
	
		// Allocate an instance
		pMyRefData = new MyRefData;
		
		pMyRefData->nisse=10;
		
		// And some non referenced managed data, use RefData
		
		gzRefPointer< gzRefData< MyData> > pMyData;
		
		pMyData=new gzRefData< MyData >;
		
		pMyData->nisse=27;
		
		// No Need to clean up data. It is all managed by the RefPointers


	}
	catch(gzBaseError &error)
	{
		error.reportError();
	}
	catch(...)
	{
	}

	gzMemoryControl::dumpAllocMem();

	return 0;
}
