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
// File			: messages.cpp
// Module		: 
// Description	: Test implementation of messaging
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
// AMO	010822	Created file 	
//
// ******************************************************************************
#include "gzDebug.h"
#include "gzExceptions.h"
#include "gzReference.h"

#include <iostream>

class MyMessageReceiver : public gzMessageReceiverInterface
{
public:

	MyMessageReceiver()
	{
		gzMessage::addMessageReceiver(this);
	}

	virtual ~MyMessageReceiver()
	{
		gzMessage::removeMessageReceiver(this);
	}

	gzVoid onMessage(const gzString& sender , gzMessageLevel level , const char *message)
	{
			std::cout<<message<<std::endl;
	}

};

// Defined your own message offsets
enum gzMyMessageOffsets { MY_OFFSET_DEBUG=1 };

// Define gzMessagelevel to be combined with +/- gzMyMessageOffsets types
GZ_USE_BIT_LOGIC_ADD_MIX(gzMessageLevel,gzMyMessageOffsets);

// Create a new gzMessagelevel
gzMessageLevel GZ_MESSAGE_DEBUG_CUSTOM=GZ_MESSAGE_DEBUG+MY_OFFSET_DEBUG;



int main(int argc , char *argv[] )
{
	try		// To catch all Gizmo3D exceptions
	{
		gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG);


		// Instantiate our custom receiver

		MyMessageReceiver theReceiver;




		GZMESSAGE(GZ_MESSAGE_DEBUG_CUSTOM,GZ_T_UNICODE(gzString((gzWideChar *)L"Hejsan")));	// Outputs a debug level message

		// Note that the previous line creates a unicode string and if the UNICODE def is set , the string will be used as a Wide String


		GZTRACE("Debug string");				// Outputs a TRACE message, automatically removed when GZ_DEBUG is undefined

	}
	catch(gzBaseError &error)	// In case of exceptions thrown we want to print the message
	{
		error.reportError();
	}

	return 0;
}
