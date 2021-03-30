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
// File			: encoder.cpp
// Module		: encoder
// Description	: encoder (example application)
// Author		: Christian Andersson
// Product		: GizmoDistribution
//
//
//
// NOTE:	GizmoDistribution is a toolkit used for implementing distributed 
//			objects and events in C++
//
//
// Revision History...
//
// Who	Date	Description
//
// CAN	040803	Created file
//
//*****************************************************************************

// Includes
#include "gzDistributionLibrary.h"

//------------------------- myCustomEncoderClass -----------------------------
/*	
	This class illustrates how to create a customized data encoder/decoder
	using GizmoDistribution. The example shows a simple byte substitution
	i.e. it just swithes a character to another.
*/

class myCustomEncoderClass : public gzDistEncoderInterface
{
	public:
		// Constructor. By putting a gzDistEncoderInterface* as in argument it's possible to cascade connect several data scramblers.
		myCustomEncoderClass(gzDistEncoderInterface* parent = NULL) : gzDistEncoderInterface(parent) {}

		// Destructor
		virtual ~myCustomEncoderClass() {}

		// My custom encoding.
		// Here you can put any scramble algorithm.
		gzUInt32 onEncode(const gzUByte* source, gzInt32 sourceLength, gzArray<gzUByte>& destination)
		{
			// Set size of destination buffer.
			destination.setSize(sourceLength);

			// Go through data and make a simple substitution.
			for (gzUInt32 i = 0; i < sourceLength; ++i)
			{
				destination[i] = source[i] ^ 123;	// Substitute!
			}

			// Return the length of my coded data. In this case the same as the source length.
			return destination.getSize();
		}

		// My custom decoding.
		gzUInt32 onDecode(const gzUByte* source, gzInt32 sourceLength, gzArray<gzUByte>& destination)
		{
			// Set size of destination buffer.
			destination.setSize(sourceLength);

			for (gzUInt32 i = 0; i < sourceLength; ++i)
			{
				destination[i] = source[i] ^ 123;	// Substitute back to original.
			}

			// Return the length of my decoded data. In this case the same as the source length.
			return destination.getSize();
		}
};


//------------------------ myMessageTransceiver ------------------------------

// A client that send events
class myMessageTransceiver : public gzDistClientInterface
{
	public:

		// Constructor.
		myMessageTransceiver();

		// Destructor.
		virtual ~myMessageTransceiver();

		// Just send a message.
		gzVoid sendMessage();
	
	protected:

		// Reimplemented from gzDistClientInterface
		gzVoid onEvent(gzDistEvent* event);

	private:

		// members

		gzDistSessionPtr m_session;
};


myMessageTransceiver::myMessageTransceiver() : gzDistClientInterface("Transceiver")
{
	// Initialize client.
	initialize();

	// Create a global session.
	m_session = getSession("session", TRUE, TRUE);

	// Join the session
	joinSession(m_session);

	// Subscribe all events in session
	subscribeEvents(gzDistEvent::getClassType(), m_session);
}

myMessageTransceiver::~myMessageTransceiver()
{
	// Uninitialize client
	uninitialize();
}

gzVoid myMessageTransceiver::onEvent(gzDistEvent* event)
{
	// Get the attribute value.
	gzString attributeValue = event->getAttributeString("message");

	// Add some information about where the message comes from.
	attributeValue += " From ";
	attributeValue += event->getAttributeString("id");
	// Note: It's possible to get the number as a string event if it's stored as a number.
	// If you want to use the number, use getAttributeNumber(...) instead.
	
	// Display it!
	GZMESSAGE(GZ_MESSAGE_NOTICE, attributeValue);
}

gzVoid myMessageTransceiver::sendMessage()
{
	// Create a event
	gzDistEvent* event = new gzDistEvent;

	// Put some data in it.
	event->setAttributeValue("message", "Hello world!");

	// Put an id in the message.
	event->setAttributeValue("id", gzDistGetCurrentProcessID());

	// Send the event!
	sendEvent(event, m_session);
}

//--------------------------------- main --------------------------------------

// Application entry point
int main(int argc, char* argv[])
{
	// Create the manager.
	gzDistManagerPtr manager = gzDistManager::getManager(TRUE);

	// Create your customized encoder....
	gzDistEncoderInterface* encoder = new myCustomEncoderClass();

	// ...or use a default encoder provided by GizmoDistribution.
//	gzDistEncoderInterface* encoder = new gzDistDefaultEncoder(12345);

	// You can stack any type of data scramblers etc. by putting them as parent to each other.
	// For example you can add a compressor after the encoder.
	// In this case, the message will first go through the encoder class, and after that it will be compressed by the gzDistDefaultCompressor.
//	encoder = new gzDistDefaultCompressor(encoder); /* Just remove this line if you don't want the compressor. */

	// Create session channel using my customized encoder.
	gzDistRemoteChannel* sessionChannel = gzDistCreateDefaultSessionChannel(0, GZ_DIST_MULTICAST, encoder);
	
	// Create server channel. You don't have to decode the server channel if you don't want to.
	// All relevant data is transmitted on the session channel anyway.
	gzDistRemoteChannel* serverChannel = gzDistCreateDefaultServerChannel(0, GZ_DIST_MULTICAST);

	// Start the manager.
	manager->start(sessionChannel, serverChannel);

	// Create transceiver client
	myMessageTransceiver* transceiver = new myMessageTransceiver;

	// Print my ID.
	GZMESSAGE(GZ_MESSAGE_NOTICE, "My ID is %d", gzDistGetCurrentProcessID());
	
	// Sleep a while...
	gzSleep(5 * GZ_SLEEP_SECOND);

	// Send some messages...
	for (gzUInt32 i = 0; i < 10; i++)
	{
		transceiver->sendMessage();
		gzSleep(GZ_SLEEP_SECOND);
	}

	// Delete the transceiver
	delete transceiver;

	// shut down and release manager
	manager->shutDown();
	manager = NULL;

	return 0;
}

