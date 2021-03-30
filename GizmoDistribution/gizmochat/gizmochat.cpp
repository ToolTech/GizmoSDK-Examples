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
// File         : gizmochat.cpp
// Module       : GizmoChat
// Description  : A simple network chat program. (example application)
// Author       : Christian Andersson
// Product      : GizmoDistribution
//      
//
//          
// NOTE:    GizmoDistribution is a toolkit used for implementing distributed
//          objects and events in C++
//
//
// Revision History...
//
// Who  Date    Description
//
// CAN  040513  Created file
//
//*****************************************************************************

// Include all necessary files.
#include "gzDistributionLibrary.h"


// --------------------- The chat client class declaration ---------------------

class CChatClient : public gzDistClientInterface
{
    public:
        // Constructor declaration.
        CChatClient();

        // Destructor declaration.
        virtual ~CChatClient();

        // The run method.
        gzVoid run();

    protected:

        // Virtual callback reimplemented from gzDistClientInterface.
        gzVoid onEvent(gzDistEvent* event);
};


// --------------------- The code ---------------------

// Constructor
CChatClient::CChatClient() : gzDistClientInterface("ChatClient")
{
}

// Destructor
CChatClient::~CChatClient()
{
}

gzVoid CChatClient::run()
{
    // Create the distribution manager.
    gzDistManagerPtr manager = gzDistManager::getManager(TRUE);

    // Start the manager and use default channels.
    manager->start(gzDistCreateDefaultSessionChannel(), gzDistCreateDefaultServerChannel());

    // Initialize my distribution interface.
    initialize();

    // Create a global session where all chat messages will be sent.
    gzDistSessionPtr myChatSession = getSession("ChatSession", TRUE, TRUE);

    // Join the session.
    joinSession(myChatSession);

    // Subscribe all events on myChatSession.
    subscribeEvents(gzDistEvent::getClassType(), myChatSession);

    // Alloc a buffer for alias.
    gzChar alias[128];
    
    // Tell the user to enter his/her alias.
    printf("Enter your alias:");

    // Get user alias.
    gets(alias);

    // Print some information and a '>' at cursor position.
    printf("Write your messages here. Type 'exit' to exit.\r\n>");

    // Alloc a message buffer.
    gzChar message[1024];

    // Repeat until 'exit' is entered.
    while (gets(message))
    {
        // Quit if the user type 'exit'.
        if (gzString(message) == "exit")
        {
            // Break the 'while loop'.
            break;
        }

        // Don't send an empty message.
        if (strlen(message) == 0)
        {
            // Empty message. Put a cursor at cursor position and wait for new input.
            printf(">");
            continue;
        }

        // Put a cursor at cursor position.
        printf(">");

        // Create a new event.
        gzDistEvent* event = new gzDistEvent;

        // Put the alias in the event.
        event->setAttributeValue("Sender", alias);

        // Put the message in the event.
        event->setAttributeValue("Message", message);

        // Put an id for current process in the event.
        event->setAttributeValue("Id", gzDistGetCurrentProcessID());

        // Send my message on myChatSession.
        sendEvent(event, myChatSession);
    }

    // Release my reference to the chat session.
    myChatSession = NULL;

    // Uninitialize my distribution interface.
    uninitialize();

    // Shut down the manager.
    manager->shutDown();
}

gzVoid CChatClient::onEvent(gzDistEvent* event)
{
    gzDouble id;

    // Get the id from the event
    if (event->getAttributeNumber("Id", id))
    {
        // Don't receive my own messages.
        if (id == gzDistGetCurrentProcessID())
        {
            // I'm the sender -> Don't display.
            return;
        }
    }

    // Get the sender name
    gzString sender = event->getAttributeString("Sender");
    
    // Get the message
    gzString message = event->getAttributeString("Message");
    
    // Display the message.
#ifdef GZ_WIN32
    // Change this line to make the example use
	// message box output on other platforms than Win32.
    MessageBox(NULL, message, sender, MB_OK);
#else
	// Default output to console.
	printf("\n");
	printf("%s: %s\n", (const char*)sender, (const char*)message);
#endif

}


// --------------------- The main routine. ---------------------

int main(int argc, char* argv[])
{
    // Create the chat client.
    CChatClient chatClient;

    // Run it!
    chatClient.run();

    return 0;
}
