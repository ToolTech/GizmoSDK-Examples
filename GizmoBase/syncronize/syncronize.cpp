// *****************************************************************************
//
// Copyright (C) SAAB AB
//
// All rights, including the copyright, to the computer program(s)
// herein belong to SAAB AB. The program(s) may be used and/or
// copied only with the written permission of SAAB AB, or in
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
// File			: syncronize.cpp
// Module		: gzBase
// Description	: Class implementation of interpreter utilities
// Author		: Anders Modén		
// Product		: GizmoBase 2.12.199
//		
// NOTE:	GizmoBase is a platform abstraction utility layer for C++. It contains 
//			design patterns and C++ solutions for the advanced programmer.
//
//
// Revision History...							
//									
// Who	Date	Description						
//									
// AMO	241115	Created file 									(2.12.199)
//
// ******************************************************************************
#include "gzBaseLibrary.h"

gzMutex s_global_errorLock;

gzString					s_errorString;
gzSerializeAdapterError		s_errorType = GZ_SERIALIZE_ADAPTER_ERROR_NO_ERROR;
gzUInt32					s_errorID = 0;


class PurlSyncer : public gzURLSyncronizer, public gzBatchWork
{
public:

	PurlSyncer(gzUInt32 id, const gzString& source, const gzString& destination, gzBool verbose = TRUE, gzBool useCRC = FALSE, gzString* errorString = NULL, gzSerializeAdapterError* errorType = NULL)
	{
		m_connectionID = id;
		m_source = source;
		m_destination = destination;
		m_verbose = verbose;
		m_useCRC = useCRC;
		m_errorString = errorString;
		m_errorType = errorType;
	}

	virtual gzVoid work() override
	{
		gzString errorString;
		gzSerializeAdapterError errorType;

		if (!syncronize(m_source, m_destination, m_verbose, m_useCRC, &errorString, &errorType))
		{
			if (m_errorString || m_errorType)
			{
				// We will report errors one at a time

				while (TRUE)
				{
					gzSleep(100);

					GZ_BODYGUARD(s_global_errorLock);

					// Check if we have pending errors not handled

					if (m_errorString && m_errorString->length())
						continue;

					if (m_errorType && *m_errorType)
						continue;

					// Write errors

					if (m_errorString)
						*m_errorString = errorString;

					if (m_errorType)
						*m_errorType = errorType;

					s_errorID = m_connectionID;

					break;
				}
			}
		}
	}

	virtual gzVoid onStatus(const gzUByte& progress, const gzString& item, const gzUInt32& syncronised, const gzUInt32& already_syncronised, const gzFloat& speed, gzBool verbose) override
	{
		if (verbose)
			GZMESSAGE(GZ_MESSAGE_NOTICE, "(%d:%d%%) Same:%d Upload:%d Speed:%.1f (Mbyte/sek) Source:%s", m_connectionID, progress, already_syncronised, syncronised, speed / 1024 / 1024, (const char*)item);
	}

private:

	gzUInt32	m_connectionID;
	gzString	m_source;
	gzString	m_destination;
	gzBool		m_verbose;
	gzBool		m_useCRC;
	gzString* m_errorString;
	gzSerializeAdapterError* m_errorType;

};

gzDynamicArray<gzString> s_errorMessages;

gzVoid checkErrors()
{
	GZ_BODYGUARD(s_global_errorLock);

	if (s_errorString.length())
	{
		// Handle

		s_errorMessages += gzString::formatString("ID:%d Error:%s", s_errorID, s_errorString);

		s_errorString.clear();
	}

	if (s_errorType)
	{
		// Handle

		s_errorType = GZ_SERIALIZE_ADAPTER_ERROR_NO_ERROR;
	}
}



int main(int argc, char* argv[])
{
	gzArgumentParser args(argc, argv);

	args.setSyntaxString("<source> <destination> <topic_name> <description_search_tag> {-debug} {-nic <iface>} {-crc <no>} {-check}");


	if (args.hasOption("debug"))
		gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG | GZ_MESSAGE_API_INTERNAL);

	gzString iface = gzRegKeyExpandedString(args.getOptionValue("nic", GZ_EMPTY_STRING));

	gzBool crc = gzRegKeyExpandedString(args.getOptionValue("crc", GZ_STRING_TRUE)).yesNo();


	args.checkArgumentCount(5);

	gzString source = args.getArgument(1);

	gzString destination = args.getArgument(2);

	gzString topic = args.getArgument(3);

	gzString searchTag = args.getArgument(4);

	gzString result = gzPipe::queryNamedPipe(iface, 10, 1, GZ_PIPE_QUERY_DESCRIPTION);

	gzDynamicType dyn = gzDynamicType::fromJSON(result);

	gzArray<gzDynamicType> array;

	if (!gzDynamic_Cast(dyn, array))
	{
		GZMESSAGE(GZ_MESSAGE_FATAL, "Failed to convert query response (%s)", result);
		return 99;
	}

	GZMESSAGE(GZ_MESSAGE_NOTICE, "Number of connected clients (%d)", array.getSize());

	if (args.hasOption("check"))
	{
		GZMESSAGE(GZ_MESSAGE_NOTICE, "Check result \n%s", result);
		return 0;
	}

	PurlSyncer* sync;

	gzBatchManager manager(16);

	for (gzUInt32 i = 0; i < array.getSize(); i++)
	{
		gzDynamicTypeContainer cont;

		if (!gzDynamic_Cast(array[i], cont))
			return 99;

		gzString description = cont.getAttribute("Description").asString();
		gzString host = cont.getAttribute("ProcessID").asString();
		gzString topic_name = cont.getAttribute("Name").asString();


		if (topic_name == topic && description.contains(searchTag))
		{
			GZMESSAGE(GZ_MESSAGE_NOTICE, "Connected to '%s' with id %s", (const char*)description, (const char*)host);

			if (!crc)
				sync = new PurlSyncer(i, source, gzString::formatString("purl:%s?nic=%s,%s?host=%s&acc=no", topic, iface, destination, host), TRUE, FALSE, &s_errorString, &s_errorType);
			else
				sync = new PurlSyncer(i, source, gzString::formatString("purl:%s?nic=%s,%s?host=%s&nodelay=true", topic, iface, destination, host), TRUE, TRUE, &s_errorString, &s_errorType);

			manager.addWork(sync);
		}
	}

	// Wait for non scheduled work

	while (manager.getPendingWork())
	{
		checkErrors();

		gzSleep(100);
	}

	// Wait for work in progress
	while (manager.getWorkInProgress())
	{
		checkErrors();

		// Stop work not in progress
		manager.stop(FALSE);

		gzSleep(100);
	}


	manager.stop();

	checkErrors();

	gzSleep(GZ_SLEEP_SECOND * 5);

}
