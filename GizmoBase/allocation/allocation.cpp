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
// File			: allocations.cpp
// Module		: 
// Description	: Example of different allocations
// Author		: Anders Modén		
// Product		: GizmoBase 2.11.70
//		
//
//			
// NOTE:	GizmoBase is a platform abstraction utility layer for C++. It contains 
//			design patterns and C++ solutions for the advanced programmer.
//
//
// Revision History...							
//									
// Who	Date	Description						
//									
// AMO	040205	Created file 	
//
// ******************************************************************************

//#define GZ_MEM_DEBUG			// use this to debug new/delete operators

#include "gzBaseLibrary.h"

// --------------------------- Custom memory allocator

// In this class you can design your own memory allocating scheme to fit e.g. XBox, playstation etc.

class CustomAllocator : public gzMemoryManagerInterface
{
	virtual gzVoid *malloc(size_t size)
	{
		return ::malloc(size);
	}

	virtual gzVoid *calloc(size_t num,size_t size)
	{
		return ::calloc(num,size);
	}

	virtual gzVoid free(gzVoid *address)
	{
		::free(address);
	}

	virtual gzVoid *realloc(gzVoid *address,size_t size)
	{
		return ::realloc(address,size);
	}
} theCustomAllocator;

// --------------------------- MEMBASE allocations -----------------------------------

// Define a membase allocatior for the class "CustomMemoryClass" in the header file
GZ_DECLARE_MEMBASE(CustomMemoryClass);


// Define the actual class derived from base class "GZ_MEMBASE(CustomMemoryClass)"
class CustomMemoryClass : public GZ_MEMBASE(CustomMemoryClass)
{
public:

	CustomMemoryClass(){};

	virtual ~CustomMemoryClass(){};

	gzString	data;

};

// define the actual storage and item count in the .cpp file
#define CUSTOM_MEMBASE_SIZE 1000
GZ_DECLARE_MEMBASE_IMP(CustomMemoryClass,CUSTOM_MEMBASE_SIZE);	// We create CUSTOM_MEMBASE_SIZE items in the local allocator storage


// -------------------------- RECYCLE DATA allocations -------------------------------

// Create a class for the actual compound data to be recycled

class MyData
{
public:

	MyData()
	{
		GZTRACE( "Created Mydata");
	}

	virtual ~MyData()
	{
		GZTRACE( "Destroyed Mydata");
	}

	gzString string;
};


// Create a container and controller class for the recycled data

class CustomRecycleClass : public gzRecycleData<MyData>
{
public:


	virtual gzVoid onNewItem(MyData & data) const 
	{
		GZTRACE( "First New");
		data.string="New";
	}									
		
	virtual gzVoid onInitItem(MyData & data) const 
	{
		GZTRACE( "Initialize Recycled data");
		data.string="Recycled";
	}
};

// Create a local storage
GZ_DECLARE_RECYCLE_DATA_IMP;

int main(int argc , char *argv[] )
{
	try
	{
		//CustomMemoryClass::s_allocator_CustomMemoryClass.disable();

		// Just try a custom allocatio
		//gzMemoryControl::setMemoryManager(&theCustomAllocator);

		//CustomMemoryClass::s_allocator_CustomMemoryClass.enable();



		//gzMemoryControl::debugMem(TRUE);		// Enables debugger of memory
		//gzMemoryControl::traceAlloc(TRUE);	// Enables trace of allocated memory


		gzMessage::setMessageLevel(GZ_MESSAGE_MEM_DEBUG);

		gzTimer timer;

		for (gzUInt32 i=0;i<CUSTOM_MEMBASE_SIZE;i++)		// Lets allocate 100%
		{
			CustomMemoryClass *object=new CustomMemoryClass;
		}

		timer.notifyTime();	// Lets see some timing
		timer.reset();

		for (gzUInt32 i=0;i<CUSTOM_MEMBASE_SIZE;i++)		// Lets overallocate 200%
		{
			CustomMemoryClass *object=new CustomMemoryClass;
		}

		timer.notifyTime();	// Lets see some timing
		timer.reset();

		// Note that the over allocation slows down the allocation to a normal heap alloc speed


		// Recycled data

		CustomRecycleClass::setMaxStorageCount(100);		// Don't recycle more than 100 items

		CustomRecycleClass *data=new CustomRecycleClass;


		GZTRACE("Type:%s address:%x",(const char *)(*data)->string,&(MyData &)*data);

		delete data;

		data=new CustomRecycleClass;

		GZTRACE( "Type:%s address:%x",(const char *)(*data)->string,&(MyData &)*data);

		delete data;

	}
	catch(gzBaseError &error)	// In case of exceptions thrown we want to print the message
	{
 		error.reportError();
	}

	return 0;
}

