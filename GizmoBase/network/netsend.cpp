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
// File			: netsend.cpp
// Module		: 
// Description	: Test app for network communication
// Author		: Anders Modén          
// Product		: GizmoBase 2.11.76
//              
//
//                      
// NOTE:	Gizmo3D is a high performance 3D Scene Graph and effect visualisation 
//			C++ toolkit for Linux, Mac OS X, Windows (Win32) and IRIX® for  
//			usage in Game or VisSim development.
//
//
// Revision History...                                                  
//                                                                      
// Who  Date    Description                                             
//                                                                      
// AMO  030420  Created file    
//
// ******************************************************************************

#include "gzBaseLibrary.h"
#include <iostream>

int main(int argc, char *argv[])
{
    gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG);

	gzArgumentParser arg(argc,argv);

	arg.setSyntaxString("{-p portnumber : port to send on} {-m : multicast} {-t time : time between each send}");

	gzBool useMulticast=arg.hasOption("m");

	gzUInt32 port=arg.getOptionValue("p",1234);

	gzDouble tick=arg.getOptionValue("t",1.0);


	try
	{
		gzSocket socket;

		socket.useExceptions(TRUE);

		gzSocketAddress destination(gzHostAddress(224,56,78,90), port);

		if(useMulticast)
		{
			std::cout << "Multicast network sender" << std::endl;
			socket.enableMulticast(destination.getHostAddress(), destination.getPort()); // Multicast
		}
		else
		{
			std::cout << "Broadcast network sender" << std::endl;
			socket.enableBroadcast(port);	// Broadcast
		}

		gzUInt32 id=0;

		while(TRUE)
		{
			id++;

			gzUInt32 networkID=gzSwapNetworkBytesL(id);

			if(useMulticast)
			{
				// Send multicast
		 		socket.sendTo(destination,(gzUByte *)&networkID,sizeof(gzUInt32));
			}
			else
			{
				// Send broadcast on 1234
		 		socket.sendTo(gzSocketAddress(socket.getBroadcastAddress(),port),(gzUByte *)&networkID,sizeof(gzUInt32));
			}

			std::cout<<'\r'<<id<<std::flush;

			gzSleep(GZ_SLEEP_SECOND*tick);
		}

	}
	catch(gzBaseError &error)       // In case of exceptions thrown we want to print the message
	{
		error.reportError();
		gzSleep(GZ_SLEEP_SECOND);
	}

	return 0;
}

