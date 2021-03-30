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
// File			: evaluators.cpp
// Module		: evaluators
// Description	: evaluators (example application)
// Author		: Anders Sandblad		
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
// XAA	040421	Created file 	
//
//*****************************************************************************

// Includes
#include <stdio.h>
#include "gzBaseLibrary.h"
#include "gzDistLibrary.h"


// Common constants
static const gzString EXAMPLE_SESSION = "example";


//-----------------------------------------------------------------------------

// A client
class Client : public gzDistClientInterface
{
	public:

		Client();

		virtual ~Client();


	private:

		// callbacks

		gzVoid onClientTick();

		gzVoid onNewObject(gzDistObject* object, gzDistSession* session);

		gzVoid onRemoveObject(gzDistObject* object, gzDistSession* session);

		gzVoid onSystemShutdown();


	private:

		// members

		gzDistSessionPtr m_session;

		gzDistObjectPtr m_object;

		gzUInt32 m_tickCount;


		enum actions
		{
			ADD		= 0,
			REMOVE	= 9
		};

};


Client::Client()
 :	gzDistClientInterface("example_client"),
	m_tickCount(0)
{

	// initialize client
	initialize(2);

	// create an join session
	m_session = getSession(EXAMPLE_SESSION, TRUE);
	joinSession(m_session);

	// subscribe for objects
	subscribeObjects(gzDistObject::getClassType(), m_session);

}


Client::~Client()
{
	// unsubscribe objects
	unsubscribeObjects(gzDistObject::getClassType(), m_session);

	// resign the session
	resignSession(m_session);

	// uninitialize client
	uninitialize();
}


gzVoid Client::onNewObject(gzDistObject* object, gzDistSession* session)
{
	m_object = object;
}


gzVoid Client::onRemoveObject(gzDistObject* object, gzDistSession* session)
{
	m_object = NULL;
}


gzVoid Client::onClientTick()
{
	if (!m_session)
	{
		return;
	}

	gzUInt32 action = m_tickCount%10;

	switch (action)
	{
		case ADD:
		{
			GZMESSAGE(GZ_MESSAGE_NOTICE, "Adding object");
			if (addObject( new gzDistObject(gzString(m_tickCount)), m_session, GZ_DIST_SYNCHRONOUS))
			{
				GZMESSAGE(GZ_MESSAGE_NOTICE, "Accepted");
			}
			else
			{
				gzDistReportLastError(TRUE);
			}

		}
		break;

		case REMOVE:
		{
			GZMESSAGE(GZ_MESSAGE_NOTICE, "Removing object");
			if (removeObject(m_object))
			{
				GZMESSAGE(GZ_MESSAGE_NOTICE, "Accepted");
			}
			else
			{
				gzDistReportLastError(TRUE);
			}
		}
		break;

		default:
		{
			GZMESSAGE(GZ_MESSAGE_NOTICE, "Updating object");
			if (updateObject(gzString(action%5), gzString(m_tickCount), m_object))
			{
				GZMESSAGE(GZ_MESSAGE_NOTICE, "Accepted");
			}
			else
			{
				gzDistReportLastError(TRUE);
			}
		}
		break;
	}
	
	++m_tickCount;
}


gzVoid Client::onSystemShutdown()
{
	// in case of pre-mature shutdown
	m_object = NULL;
	m_session = NULL;	
}


//-----------------------------------------------------------------------------

// An evaluator
class Evaluator : public gzDistEvaluatorInterface
{
	public:

		Evaluator();

		virtual ~Evaluator();


	private:

		// callbacks

		gzBool onNewObject(gzDistObject* object, gzDistSession* session);

		gzBool onRemoveObject(gzDistObject* object);

		gzBool onUpdateObject(gzDistTransaction* transaction, gzDistObject* object);

		gzBool onRemoveAttributes(gzDistTransaction* transaction, gzDistObject* object);

};


Evaluator::Evaluator()
:	gzDistEvaluatorInterface(gzDistObject::getClassType())
{
}


Evaluator::~Evaluator()
{
}

		
gzBool Evaluator::onNewObject(gzDistObject* object, gzDistSession* session)
{
	GZMESSAGE(GZ_MESSAGE_NOTICE, "Evaluating new object");
	
	return TRUE; // accept new object (return FALSE to reject)
}


gzBool Evaluator::onRemoveObject(gzDistObject* object)
{
	GZMESSAGE(GZ_MESSAGE_NOTICE, "Evaluating remove object");

	return TRUE; // accept object remove (return FALSE to reject)
}


gzBool Evaluator::onUpdateObject(gzDistTransaction* transaction, gzDistObject* object)
{
	GZMESSAGE(GZ_MESSAGE_NOTICE, "Evaluating update object");

	return TRUE; // accept object update (return FALSE to reject)
}


gzBool Evaluator::onRemoveAttributes(gzDistTransaction* transaction, gzDistObject* object)
{
	GZMESSAGE(GZ_MESSAGE_NOTICE, "Evaluating remove attributes");

	return TRUE; // accept attribute remove (return FALSE to reject)
}

		
//-----------------------------------------------------------------------------

// Application entry point
int main(int argc, char* argv[])
{
	// setup default message receiver
	gzMessage::setMessageLevel(GZ_MESSAGE_NOTICE);

	// create manager and start
	gzDistManagerPtr manager = gzDistManager::getManager(TRUE);
	manager->start();

	GZMESSAGE(GZ_MESSAGE_NOTICE, "Hit return to quit!\n");	
	
	// create session
	gzDistSessionPtr session = manager->getSession(EXAMPLE_SESSION, TRUE, FALSE, GZ_DIST_SERVER_PRIO_NORMAL);

	// add evaluator
	gzDistEvaluatorInterfacePtr evaluator = new Evaluator;
	session->registerEvaluator(evaluator);
	
	// create client
	Client* client = new Client;


	// wait for return
	getchar();
	

	// delete the client
	delete client;

	// unregister evaluator
	session->unregisterEvaluator(evaluator);
	evaluator = NULL;

	// release session
	session = NULL;
	
	// shut down and release manager
	manager->shutDown();
	manager = NULL;

	gzSleep(GZ_SLEEP_SECOND);

	return 0;

}

