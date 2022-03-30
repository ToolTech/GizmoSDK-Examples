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
// File			: dyndata.cpp
// Module		: 
// Description	: Example of dynamic data in GizmoBase
// Author		: Anders Modén		
// Product		: GizmoBase 2.11.76
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


class MyDynamicData : public gzSerializeData
{
public:

	// The data, One wide string and one number

	gzSerializeStringWide				string;

	gzSerializeBinary<gzDouble>			number;


	// The serializing methods (Format)

	gzVoid write(gzSerializeAdapter *adapter)	// Provide this method to write the data in the correct order
	{
		string.write(adapter);					// Format defined as a string first and then a number
		number.write(adapter);
	}

	gzVoid read(gzSerializeAdapter *adapter)	// Provide this method to read the data in the correct order
	{
		string.read(adapter);
		number.read(adapter);
	}

	gzVoid pushBack(gzSerializeAdapter *adapter) // Provide this method to pushback the data in the correct order
	{
		number.pushBack(adapter);				// Note that this is done in the reversed order !!!
		string.pushBack(adapter);
	}

	gzUInt32	getDataSize(gzSerializeAdapter *adapter=NULL) const	// Provide this method to get the correct size of the stored data (in bytes)
	{
		return string.getDataSize(adapter)+number.getDataSize(adapter);
	}


	// The type definition (either const char * or gzString)

	static const char * getDataTag()		// The custom class will be identified by this tag "MyData"
	{
		return "MyData";
	}

};

int main(int argc , char *argv[] )
{
	try
	{
		gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG);


		// Create the data structure

		MyDynamicData data;

		// set some values

		//data.string=gzString("Nisse Hult såg en CAN buss").convertAsciiToUTF8();
		data.string=L"Test";
		data.number=667;


		gzDynamicTypeContainer container;		// Can contain any data

		container.setAttribute("testdata",gzDynamicTypeCustom<MyDynamicData>(data));	// Set the attribute "testdata"


		// and do some stuff etc...



		MyDynamicData data2;

		data2=gzDynamic_Cast<MyDynamicData>(container.getAttribute("testdata"));

		// Check some data

		GZTRACE("String is %S",(const gzString &)data2.string);

		GZTRACE("Number is %d",(gzDouble)data2.number);


		// Some other checks

		gzDynamicType rawdata=container.getAttribute("testdata");

		GZTRACE("Type is %s",(const char *)rawdata.getDynamicType());


	}
	catch(gzBaseError &error)	// In case of exceptions thrown we want to print the message
	{
 		error.reportError();
	}

	return 0;
}

