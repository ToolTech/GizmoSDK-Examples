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
// File			: templates.cpp
// Module		: 
// Description	: Test implementation of templates usage
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
// AMO	990919	Created file 	
//
// ******************************************************************************
#include "gzBaseLibrary.h"


// A test class that shows a reference based class + virtual cloning
class testClass : public gzReference
{
public:

	virtual ~testClass(){};				// To ensure virtual destructor

	gzInt32 data;							// Some integer data

	virtual gzReference	* clone() const	// a copy of instance
	{
		return (gzReference *)new testClass(*this);
	}
};


int main(int argc , char *argv[] )
{
	try		// To catch all Gizmo3D exceptions
	{
		gzMessage::setMessageLevel(GZ_MESSAGE_NOTICE);

		// Adding objects to lists.

		gzRefList<testClass> list_A,list_B;		// Two reference lists

		testClass *object;

		object=new testClass;					// Ref count = 0 at creation

		list_A.insert(object);					// Adds object to lists

		list_B.insert(object);


		object=new testClass;					// An other instance

		list_B.insert(object);
		list_B.insert(object);					// Insert two instances of object in List B


		// Iterating of objects in list

		gzListIterator<testClass>	iterator(list_B);

		while(object=iterator())
		{
			GZMESSAGE(GZ_MESSAGE_NOTICE,"Object address %lx",object);
		}

		// Copy of referenced list where instances are copied
		// If the object implemented the method "useDeepCopy()" and returned FALSE
		// they would have been shared instead

		gzRefList<testClass> list_C(list_B);

		list_C.insert(new testClass);
	
		GZMESSAGE(GZ_MESSAGE_NOTICE,"List C contains %ld elements",list_C.entries());

		// When the destructor of this try-catch scope is called the auto vars list_A
		// list_B and list_C are automatically destroyed !!
		// as well as the objects in the lists
	}
	catch(gzBaseError &error)	// In case of exceptions thrown we want to print the message
	{
		error.reportError();
	}

	return 0;
}
