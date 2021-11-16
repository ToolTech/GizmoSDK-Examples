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
// File			: rtti.cpp
// Module		: 
// Description	: Example of specialised rtti in GizmoBase
// Author		: Anders Modén		
// Product		: GizmoBase 2.11.48
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
// AMO	040209	Created file 	
//
// ******************************************************************************

#include "gzBaseLibrary.h"

class MyBaseClass : public gzTypeInterface
{
public:
	GZ_DECLARE_TYPE_INTERFACE;		// You need this line to add type info to your class

	MyBaseClass():a(10){}			// Create the member with a default value of 10

	int a;

};


class MyDerivedClass : public MyBaseClass
{
public:

	GZ_DECLARE_TYPE_INTERFACE;		// You need this line to add type info to your class

};

class MySecondDerivedClass : public MyBaseClass
{
public:

	GZ_DECLARE_TYPE_INTERFACE;		// You need this line to add type info to your class

};


class MyVeryDerivedClass : public MyDerivedClass
{
public:

	GZ_DECLARE_TYPE_INTERFACE;		// You need this line to add type info to your class

};




// You need this in the implementation file


GZ_DECLARE_TYPE_BASE(MyBaseClass,"MyBaseClass");
GZ_DECLARE_TYPE_CHILD(MyBaseClass,MyDerivedClass,"MyDerivedClass");
GZ_DECLARE_TYPE_CHILD(MyBaseClass,MySecondDerivedClass,"MySecondDerivedClass");
GZ_DECLARE_TYPE_CHILD(MyDerivedClass,MyVeryDerivedClass,"MyVeryDerivedClass");

gzVoid printInfo(gzTypeInterface *pClass)
{
	// The standard way to do it...

	MyBaseClass *pMyBaseClass=NULL;

	if(pClass->isOfType(MyBaseClass::getClassType()))
	{
		pMyBaseClass=(MyBaseClass *)pClass;		// Now the pointer has a value
	}

	// The neat way to do it

	pMyBaseClass=gzDynamic_Cast<MyBaseClass>(pClass);

	if(pMyBaseClass)
	{
		GZMESSAGE(GZ_MESSAGE_NOTICE,"The instance is a '%s' type",pClass->getTypeName());

		if(pMyBaseClass->isExactType(MyVeryDerivedClass::getClassType()))
		{
			GZMESSAGE(GZ_MESSAGE_NOTICE,"The instance is really a very very '%s' type",pClass->getTypeName());
		}
	}

}

int main(int argc , char *argv[] )
{
	try
	{
	
		MyDerivedClass *pDerivedClass=new MyDerivedClass;		// Lets make a derived class

		printInfo(pDerivedClass);

		delete pDerivedClass;



		MyVeryDerivedClass *pVeryDerivedClass=new MyVeryDerivedClass;		// Lets make a very derived class

		printInfo(pVeryDerivedClass);

		delete pVeryDerivedClass;
		
	}
	catch(gzBaseError &error)	// In case of exceptions thrown we want to print the message
	{
 		error.reportError();
	}

	return 0;
}

