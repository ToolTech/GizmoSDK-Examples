//***************************************************************************
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
// File			: tcp.cpp
// Module		: tcp
// Description	: tcp (example application)
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
// XAA	040823	Created file
//
//***************************************************************************

//---------------------------------------------------------------------------
//
// This example application illustrates how to use the TCP transport class,
// and the socket server class.
//
// The transport classes can be used independantly from the common
// event and object distribution as shown here, but normally you do not
// need to bother.
//
// The application can be configured to be either a server or a client.
// If no configuration is given on the command line, the application
// will be a server listening for connections on port 2774.
// 
// The server repeatedly send the current time to all connected clients.
//
// Command line: tcp [-server <port> | -client <address> <port>]
//
//---------------------------------------------------------------------------


// Includes
#include "gzDistributionLibrary.h"


// Common constants
static const gzUInt32 MAX_MESSAGE_LENGTH = 256;
static const gzUInt16 DEFAULT_PORT = 2774;
static const gzHostAddress DEFAULT_ADDRESS = gzHostAddress(127,0,0,1);
static const gzUInt32 SEND_INTERVAL = 5;


//-------------------------------- TcpClient --------------------------------

// TCP client class
class TcpClient : public gzThread
{
	public:

		TcpClient();
		virtual ~TcpClient();

		gzBool run(const gzHostAddress& address, gzUInt16 port);
		gzVoid stop(gzBool wait = TRUE);

		gzBool isConnected()  const;
		

	private:

		gzVoid process();
		gzVoid processClient();

		gzUInt16 m_port;
		gzHostAddress m_address;
		gzBool m_connected;

		gzDistTransportTCP m_transport;

};


// Constructor
TcpClient::TcpClient()
 :	m_port(DEFAULT_PORT),
	m_address(DEFAULT_ADDRESS),
	m_connected(FALSE)
{
}


// Destructor
TcpClient::~TcpClient()
{
	stop(TRUE);
}


// Run client
gzBool TcpClient::run(const gzHostAddress& address, gzUInt16 port)
{
	if (isRunning())
	{
		return FALSE;
	}

	if (!m_transport.createClient(gzSocketAddress(address, port)))
	{
		gzDistReportLastError(TRUE);
		return FALSE;
	}

	m_address = address;
	m_port = port;
	m_connected = FALSE;

	gzThread::run();

	return TRUE;
}


// Stop client
gzVoid TcpClient::stop(gzBool wait)
{
	gzThread::stop(wait);
}


// Connected?
gzBool TcpClient::isConnected() const
{
	return m_connected;
}


// Thread entry point
gzVoid TcpClient::process()
{
	GZTRACE("Entering TCP client thread");

	GZ_EXCEPTION(try)
	{
		processClient();
	}
	GZ_EXCEPTION(catch(gzBaseError& err)
	{
		GZMESSAGE(GZ_MESSAGE_FATAL, "Exception in TCP client thread: %s", (const char*)err.getErrorString());
		gzSleep(100);
	})
	GZ_EXCEPTION(catch(...)
	{
		GZMESSAGE(GZ_MESSAGE_FATAL, "Exception in TCP client thread");
		gzSleep(100);
	})

	GZTRACE("Leaving TCP client thread");
}


// Thread implementation
gzVoid TcpClient::processClient()
{
	// setup receive buffer	
	gzUInt32 length(0);
	gzArray<gzUByte> buffer(MAX_MESSAGE_LENGTH);
	memset(buffer.getAddress(), 0, buffer.getSize());

	// send server time flag
	gzBool sendTime(TRUE);

	// loop
	while (!isStopping())
	{
		if (!m_connected)
		{
			// try to connect client-side
			if (!m_transport.open())
			{
				// connection failed
				gzDistError error = gzDistGetLastError(TRUE);
				if (error.getErrorCode() != GZ_DIST_SOCKET_CONNECTION_REFUSED)
				{
					error.reportError();
					gzThread::stop(FALSE); // quit
				}
			}
			else
			{
				// connection succeeded
				m_connected = TRUE;
				GZMESSAGE(GZ_MESSAGE_NOTICE, "Connected to %s:%u", (const char*)m_address.asString(), m_port);
			}
		}

		// check connection
		if (m_transport.isConnected())
		{
			// connected

			// receive (non-blocking)
			gzInt32 result = m_transport.receive(buffer.getAddress(), buffer.getSize());

			if (result > 0)
			{
					// time was received
				GZMESSAGE(GZ_MESSAGE_NOTICE, "Server time is %s (utc)", (const char*)buffer.getAddress());
			}
			else if (result < 0)
			{
				// there was an error
				gzDistReportLastError(TRUE);
			}

		}
		else
		{
			// not connected

			if (m_connected)
			{
				// connection lost
				m_connected = FALSE;
				GZMESSAGE(GZ_MESSAGE_NOTICE, "Connection to %s:%u was lost", (const char*)m_address.asString(), m_port);
			}
		
		}

		gzSleep(100);

	}

	if (m_transport.isOpen())
	{
		m_transport.close();
	}

}


//-------------------------------- TimeServer --------------------------------

class TimeServer : public gzThread
{
	public:

		TimeServer();

		virtual ~TimeServer();

		gzVoid addConnection(gzDistTransportTCP* connection);


	private:

		gzVoid process();

		gzVoid processConnections();


		gzList<gzDistTransportTCP> m_connectionList;

		gzMutex m_connectionLocker;

};


TimeServer::TimeServer()
{
}


TimeServer::~TimeServer()
{
	stop(TRUE);

	m_connectionLocker.waitLock();
	m_connectionList.clearAndDestroy();
	m_connectionLocker.unLock();
}


gzVoid TimeServer::addConnection(gzDistTransportTCP* connection)
{
	m_connectionLocker.waitLock();
	m_connectionList.insert(connection);
	GZTRACE("%u connections", m_connectionList.entries());
	m_connectionLocker.unLock();
}

		
gzVoid TimeServer::process()
{
	GZTRACE("Entering time server thread");

	GZ_EXCEPTION(try)
	{
		processConnections();
	}
	GZ_EXCEPTION(catch(gzBaseError& err)
	{
		GZMESSAGE(GZ_MESSAGE_FATAL, "Exception in time server thread: %s", (const char*)err.getErrorString());
		gzSleep(100);
	})
	GZ_EXCEPTION(catch(...)
	{
		GZMESSAGE(GZ_MESSAGE_FATAL, "Exception in time server thread");
		gzSleep(100);
	})

	GZTRACE("Leaving time server thread");
}


gzVoid TimeServer::processConnections()
{
	gzBool send(FALSE);
	gzUInt32 lastSend(0);
	gzUInt32 systemSeconds = (gzUInt32)gzTime::systemSeconds();
	gzString now = gzTime::now().asString();

	while (!isStopping())
	{
		m_connectionLocker.waitLock();

		systemSeconds = (gzUInt32)gzTime::systemSeconds();

		if (systemSeconds % SEND_INTERVAL == 0)
		{
			if (systemSeconds > lastSend)
			{
				send = TRUE;
				lastSend = systemSeconds;
				now = gzTime::now().asString();
			}
			else
			{
				send = FALSE;
			}
		}
		else
		{
			send = FALSE;
		}

		gzListIterator<gzDistTransportTCP> iter(m_connectionList);
		gzDistTransportTCP* connection = NULL;

		while (connection = iter())
		{
			if (!connection->isConnected())
			{
				gzSocketAddress address = connection->getClientConnectAddress();
				GZMESSAGE(GZ_MESSAGE_NOTICE, "Connection to %s:%u was lost", (const char*)address.getHostAddress().asString(), address.getPort());
				
				iter.remove();
				delete connection;

				GZTRACE("%u connections", m_connectionList.entries());

			}
			else if (send)
			{
				if (!connection->send((gzUByte*)now.getString(), now.length()+1))
				{
					// there was an error
					gzDistReportLastError(TRUE);
				}
			}
		}

		m_connectionLocker.unLock();

		gzSleep(100);
	}
}


//-------------------------------- TcpServer --------------------------------

class TcpServer : public gzDistSocketServer
{
	public:

		TcpServer();

		virtual ~TcpServer();


	private:

		gzVoid onConnect(gzSocket* socket);

		TimeServer m_timeServer;

};


// Constructor
TcpServer::TcpServer()
{
	// run time server
	m_timeServer.run();
}


// Destructor
TcpServer::~TcpServer()
{
	// stop server
	stop(TRUE);
	
	// stop time server
	m_timeServer.stop(TRUE);
}


// Handle new connections
gzVoid TcpServer::onConnect(gzSocket* socket)
{
	// create transport
	gzDistTransportTCP* transport = new gzDistTransportTCP; 
	
	if (!transport->createClient(socket))
	{
		GZMESSAGE(GZ_MESSAGE_WARNING, "Failed to create transport");
		delete transport;
		delete socket;
	}
	else
	{
		gzSocketAddress address = socket->getConnectAddress();
		GZMESSAGE(GZ_MESSAGE_NOTICE, "Connection from %s:%u", (const char*)address.getHostAddress().asString(), address.getPort());

		// add connection to time server
		m_timeServer.addConnection(transport);
	}

}


//------------------------- Application entry point -------------------------

int main(int argc, char* argv[])
{
	// set message level
#ifdef GZ_DEBUG
	gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG);
#else
	gzMessage::setMessageLevel(GZ_MESSAGE_NOTICE);
#endif

	// default parameters
	gzBool server(TRUE);
	gzUInt16 port(DEFAULT_PORT);
	gzHostAddress address(DEFAULT_ADDRESS);

	
	// parse command line
	gzArgumentParser args(argc, argv);

	if (args.hasOption("server"))
	{
		server = TRUE;
		port = args.getOptionValue("server", DEFAULT_PORT, 0);
	}
	else if (args.hasOption("client"))
	{
		server = FALSE;
		gzString addressString = args.getOptionValue("client", DEFAULT_ADDRESS.asString(), 0);
		address = gzHostAddress(addressString);
		port = args.getOptionValue("client", DEFAULT_PORT, 1);
	}


	// run
	GZMESSAGE(GZ_MESSAGE_NOTICE, "Hit return to quit!");
	
	if (server)
	{		
		GZMESSAGE(GZ_MESSAGE_NOTICE, "Server listening on port %u", port);

		TcpServer server;
		server.run(port);

		getchar(); // wait
		
		server.stop();
	}
	else
	{
		GZMESSAGE(GZ_MESSAGE_NOTICE, "Client connecting to port %u on host %s", port, (const char*)address.asString());

		TcpClient client;
		client.run(address, port);
		
		getchar(); // wait

		client.stop();
	}

	return 0;

}

