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
// File			: chkappend.cpp
// Module		: 
// Description	: An example of string append and serialization
// Author		: Anders Modén          
// Product		: Gizmo3D 2.11.48
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
#include "gzBaseLibrary.h"

#include <iostream>
using namespace std;


int main(int argc , char *argv[] )
{
	if(argc!=4)	// Check for 4 arguments
	{
		cout<<"syntax: "<<argv[0]<<" <word> <filename> <appendfilename>"<<endl;
		return 0;
	} 

	try
	{
		gzSerializeAdapterFile input(argv[2]);		// Open first file
		
		gzBool foundMatch=FALSE;
		
		gzSerializeString line('\n');
		
		gzQueue<gzString>	queue(GZ_QUEUE_FIFO);
		
		if(input.hasError())						// fault in opening ?
		{
			cout<<"Error: Couldn't open "<<argv[2]<<" for reading !!"<<endl;
			return 0;
		}
	
		while(input.hasData())						// read all data into a queue
		{
			line.read(&input);
			queue.push(line);
			if(((gzString)line).contains(argv[1],FALSE))
				foundMatch=TRUE;
		}
		
		if(!foundMatch)								// did we find the word we were looking for ?
		{
			gzSerializeAdapterFile output(argv[2],GZ_SERIALIZE_OUTPUT);
			
			if(output.hasError())
			{
				cout<<"Error: Couldn't open "<<argv[2]<<" for writing !!"<<endl;
				return 0;
			}
				
			while(queue.entries())					// write it back to new file
			{
				line=queue.pop();
				line.write(&output);
			}
			
			gzSerializeAdapterFile append(argv[3]);
			
			while(append.hasData())					// and append new data
			{
				line.read(&append);
				line.write(&output);
			}
		}
	}
	catch(gzBaseError &error)	// In case of exceptions thrown we want to print the message
	{
		error.reportError();
	}
	
	return 0;
}
