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
// File			: datagen.cpp
// Module		: 
// Description	: An example of string append and serialization
// Author		: Anders Modén          
// Product		: GizmoBase 2.12.199
//              
//
// NOTE:	GizmoBase is a platform abstraction utility layer for C++. It contains 
//			design patterns and C++ solutions for the advanced programmer.
//
//
// Revision History...                                                  
//                                                                      
// Who  Date    Description                                             
//                                                                      
// AMO  991002  Created file  
//
// ******************************************************************************
#include "gzBaseLibrary.h"

#include <iostream>


int main(int argc , char *argv[] )
{
	gzArgumentParser args(argc,argv);

	if(args.getArgumentCount()!=3)	// Check for 3 arguments
	{
		std::cout<<"syntax: "<<args.getArgument(0).getString()<<" <inputfile> <outputfile>"<<std::endl;
		return 0;
	} 

	try
	{
		gzSerializeAdapterPtr input=gzSerializeAdapter::getURLAdapter(args.getArgument(1),GZ_SERIALIZE_INPUT);

		gzSerializeAdapterPtr output=gzSerializeAdapter::getURLAdapter(args.getArgument(2),GZ_SERIALIZE_OUTPUT);

		gzUByte itemCount=0;

		if(input && output)
		{
			gzUByte data;

			gzString format;

			while(readAdapter(input,data))
			{
				if(!(itemCount=(itemCount+1)%80))
					format.format("%d,\r\n",data);
				else
					format.format("%d,",data);
				
				output->write((gzUByte *)(const char *)format,format.length());
			}
		}
		
	}
	catch(gzBaseError &error)	// In case of exceptions thrown we want to print the message
	{
		error.reportError();
	}
	
	return 0;
}
