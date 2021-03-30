//*****************************************************************************
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
// File			: server.cpp
// Module		: server
// Description	: server (example application)
// Author		: Anders Sandblad		
// Product		: GizmoDistribution
//		
//
//			
// NOTE:	GizmoDistribution is a toolkit that is used to implement
//			network distributed objects and events in C++
//
//
// Revision History...							
//									
// Who	Date	Description						
//									
// XAA	031030	Created file 	
//
//*****************************************************************************

// Includes
#include "gzDistributionLibrary.h"

//-----------------------------------------------------------------------------

// Message receiver
class MessageReceiver : public gzMessageReceiverInterface
{
	public:
	
		gzVoid onMessage(const gzString& sender, gzMessageLevel level, const char* message);

};

// Message receiver callback
gzVoid MessageReceiver::onMessage(const gzString& sender, gzMessageLevel level, const char* message)
{
	gzString output;
	
	if (level >= GZ_MESSAGE_NOTICE)
	{
		if (level >= GZ_MESSAGE_ASSERT)
		{
			output.format("[ASSERT] %s\n", message);
		} // endif
		if (level >= GZ_MESSAGE_FATAL)
		{
			output.format("[FATAL] %s\n", message);
		} // endif
		else if (level >= GZ_MESSAGE_WARNING)
		{
			output.format("[WARNING] %s\n", message);
		} // endif
		else
		{
			output.format("%s\n", message);
		} // endelse

		printf((const char*)output);

	} // endif
	else
	{
		output.format("%s\n", message);
	} // endelse

	#ifdef GZ_DEBUG		
		#ifdef GZ_WIN32
			gzString debug;
			debug.format("%s: %s", (const char*)sender, (const char*)output);
			OutputDebugStringA((const char*)debug); // WIN32 debug output
		#else
			// ??
		#endif
	#endif

}


//-----------------------------------------------------------------------------

// Simple example server instance 
class SimpleServer: public gzDistServerInterface, public gzThread
{
	public:

		explicit SimpleServer(gzDistServerPriority priority);
		
		virtual ~SimpleServer();


		//----- server callbacks -----

		gzVoid onStatus(gzDistServerStatus status);

		gzVoid onNewInstance(const gzDistInstanceID& id);

		gzVoid onRemoveInstance(const gzDistInstanceID& id, gzDistServerRemoveReason reason);

		gzVoid onActiveInstance(const gzDistInstanceID& id);


		//----- thread -----
		
		gzVoid process();


	private:

		gzDistServerStatus m_status;



}; // SimpleServer


// Constructor
SimpleServer::SimpleServer(gzDistServerPriority priority)
 :	gzDistServerInterface("example", priority), // The function ID of the server is "example"
	m_status(GZ_DIST_SERVER_STATUS_IDLE)
{
}


// Destructor
SimpleServer::~SimpleServer()
{
	if (isRunning())
	{
		gzThread::stop(TRUE);
	}
}


// Local server status callback
gzVoid SimpleServer::onStatus(gzDistServerStatus status)
{
	m_status = status;

	switch (status)
	{
		case GZ_DIST_SERVER_STATUS_ACTIVE:
		{
			// Start the thread when activated
			if (!isRunning())
			{
				gzThread::run();
			}
		}
		break;

		default:
		{
			// Stop the thread when not active
			if (isRunning())
			{	
				gzThread::stop(TRUE);
			}
		}
		break;
	}
}


// New remote server instance callback
gzVoid SimpleServer::onNewInstance(const gzDistInstanceID& id)
{
	GZMESSAGE("SimpleServer", GZ_MESSAGE_NOTICE, "New instance: %s", (const char*)id.asString());
}


// Remove remote server instance callback
gzVoid SimpleServer::onRemoveInstance(const gzDistInstanceID& id, gzDistServerRemoveReason reason)
{
	switch (reason)
	{
		case GZ_DIST_SERVER_REMOVE_SHUTDOWN:
			GZMESSAGE("SimpleServer", GZ_MESSAGE_NOTICE, "Instance shutdown: %s", (const char*)id.asString());
			break;
			
		case GZ_DIST_SERVER_REMOVE_TIMEOUT:
			GZMESSAGE("SimpleServer", GZ_MESSAGE_DEBUG, "Instance timeout: %s", (const char*)id.asString());
			break;		
	}

}


// Actice remote server instance callback
gzVoid SimpleServer::onActiveInstance(const gzDistInstanceID& id)
{
	if (id.isNull())
	{
		GZMESSAGE("SimpleServer", GZ_MESSAGE_NOTICE, "There is no active server instance");
	}
	else
	{
		GZMESSAGE("SimpleServer", GZ_MESSAGE_NOTICE, "Active instance: %s", (const char*)id.asString());
	}
}


// Example server thread
gzVoid SimpleServer::process()
{	
	while (!isStopping())
	{
		gzSleep(GZ_SLEEP_SECOND);

		if (m_status == GZ_DIST_SERVER_STATUS_ACTIVE)
		{
			GZMESSAGE("SimpleServer", GZ_MESSAGE_NOTICE, "I am the server");
		}
	}
}


//-----------------------------------------------------------------------------

// Get help from command line
gzBool getHelp(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		gzString arg = gzString(argv[i]).toLower();

		if (arg == "-help") return TRUE;
	
		if (arg == "-h") return TRUE;
	
		if (arg == "-?") return TRUE;
	
	}

	return FALSE;

}


// Get priority from command line
gzDistServerPriority getPriority(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		gzString arg = gzString(argv[i]).toLower();

		if (arg == "-never")
		{
			printf("\nServer priority: never\n\n");
			return GZ_DIST_SERVER_PRIO_NEVER;
		}
		else if (arg == "-verylow")
		{
			printf("\nServer priority: very low\n\n");
			return GZ_DIST_SERVER_PRIO_VERY_LOW;
		}
		else if (arg == "-low")
		{
			printf("\nServer priority: low\n\n");
			return GZ_DIST_SERVER_PRIO_LOW;
		}
		else if (arg == "-normal")
		{
			printf("\nServer priority: normal\n\n");
			return GZ_DIST_SERVER_PRIO_NORMAL;
		}
		else if (arg == "-high")
		{
			printf("\nServer priority: high\n\n");
			return GZ_DIST_SERVER_PRIO_HIGH;
		}
		else if (arg == "-veryhigh")
		{
			printf("\nServer priority: very high\n\n");
			return GZ_DIST_SERVER_PRIO_VERY_HIGH;
		}
		else if (arg == "-max")
		{
			printf("\nServer priority: max\n\n");
			return GZ_DIST_SERVER_PRIO_MAX;
		}

	}

	printf("\nServer priority: normal\n\n");
	return GZ_DIST_SERVER_PRIO_NORMAL;

}


// Get message level from command line
gzMessageLevel getMessageLevel(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		gzString arg = gzString(argv[i]).toLower();

		if (arg == "-memdebug")
		{
			return GZ_MESSAGE_MEM_DEBUG;
		}
		else if (arg == "-debug")
		{
			return GZ_MESSAGE_DEBUG;
		}
		else if (arg == "-notice")
		{
			return GZ_MESSAGE_NOTICE;
		}
		else if (arg == "-warning")
		{
			return GZ_MESSAGE_WARNING;
		}
		else if (arg == "-fatal")
		{
			return GZ_MESSAGE_FATAL;
		}

	}

	#ifdef GZ_DEBUG
		return GZ_MESSAGE_DEBUG;
	#else
		return GZ_MESSAGE_NOTICE;
	#endif

}


// Print help
gzVoid printHelp()
{
	printf("Command line argumets:\n\n");

	printf("  Priority levels:\n");
	printf("    -never\n");
	printf("    -verylow\n");
	printf("    -low\n");
	printf("    -normal (default)\n");
	printf("    -high\n");
	printf("    -veryhigh\n");
	printf("    -max\n\n");

	printf("  Message output levels:\n");
	printf("    -debug\n");
	printf("    -notice (default)\n");
	printf("    -warning\n\n");

	printf("  Help (this text):\n");
	printf("    -help\n\n");

}


//-----------------------------------------------------------------------------

// Application entry point
int main(int argc, char* argv[])
{
	// Print example info
	printf("\nThis is a GizmoSDK example application that illustrates how to create\n");
	printf("a simple priority based distributed server using GizmoDistribution.\n\n");
	printf("(c) 2004- Saab Training Systems AB, Sweden\n\n");

	// Look for help arg on the command line
	if ( getHelp(argc, argv) )
	{
		printHelp();
		return 0;
	}

	// Add message receiver (with level from command line)
	MessageReceiver messageReceiver;
	gzMessage::setMessageLevel( getMessageLevel(argc, argv) );
	gzMessage::addMessageReceiver(&messageReceiver);
	
	// Create manager
	gzDistManagerPtr manager = gzDistManager::getManager(TRUE);
	
	// Setup session channel
	gzDistRemoteChannel* sessionChannel = new gzDistRemoteChannel;
	gzDistTransportUDP* sessionTransport = new gzDistTransportUDP;
	sessionTransport->createMulticast(gzSocketAddress(gzHostAddress(234,56,78,90), 8888));
	sessionChannel->setTransport(sessionTransport);
	
	// Setup server channel
	gzDistRemoteChannel* serverChannel = new gzDistRemoteChannel;
	gzDistTransportUDP* serverTransport = new gzDistTransportUDP;
	serverTransport->createMulticast(gzSocketAddress(gzHostAddress(234,56,78,90), 9999));
	serverChannel->setTransport(serverTransport);

	// Start manager
	manager->start(sessionChannel, serverChannel);

	// Create the server instance (with priority from the command line)
	SimpleServer server( getPriority(argc, argv) );

	printf("Hit return key to quit!\n\n\n");

	// Install the instance
	server.install();
	
	// Wait for return
	getchar();

	// Uninstall the instance
	server.uninstall();

	// Shutdown remote distribution
	manager->shutDown();
	manager = NULL;

	// Remove message receiver
	gzMessage::removeMessageReceiver(&messageReceiver);

	return 0;

}

