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
#include "gzDebug.h"
#include "gzExceptions.h"
#include "gzSerialize.h"

#include <iostream>


class testSerializeData : public gzSerializeData
{
public:

	long	longdata;
	float	floatdata;

	GZ_DECLARE_SERIALIZE_2("testSerializeData", longdata, floatdata);
};


int main(int argc , char *argv[] )
{
	try		// To catch all Gizmo3D exceptions
	{
		gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG);

		testSerializeData data1,data2;

		data1.floatdata=1.23f;
		data1.longdata=12345678;

		gzSerializeAdapter  *adapter=gzSerializeAdapter::getURLAdapter("test.dat",GZ_SERIALIZE_OUTPUT);

		data1.write(adapter);
		delete adapter;

		adapter=gzSerializeAdapter::getURLAdapter("test.dat",GZ_SERIALIZE_INPUT);
		data2.read(adapter);
		delete adapter;

		GZMESSAGE(GZ_MESSAGE_DEBUG,"Float data %.3lf",data2.floatdata);
		GZMESSAGE(GZ_MESSAGE_DEBUG,"Long data %ld",data2.longdata);

	}
	catch(gzBaseError &error)	// In case of exceptions thrown we want to print the message
	{
		error.reportError();
	}

	return 0;
}
