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
// File			: terminal.cpp
// Module		: 
// Description	: Starts a terminal console utility
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
// Who	Date	Description						
//									
// AMO	210617	Created file 		(2.11.19)
//
// ******************************************************************************

#include "gzBaseLibrary.h"


gzLoggerPtr logger_save;
gzLoggerPtr logger_con;

static class cleaner 
{
public:

	virtual ~cleaner()
	{
		gzModule::unLoadModules();
	}

} _the_cleaner;

int main(int argc , char *argv[] )
{
	try
	{
		gzAttachConsole();

		gzArgumentParser args(argc, argv);

		args.setSyntaxString("{-in <url to open>} {-debug} {-duplex} {-out <url output>} {-plugins <a;b;c>} {-ping} {-command <plugin command>} {-config <config url>}");

		if(args.hasOption("debug"))
			gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG | GZ_MESSAGE_API_INTERNAL);

		gzString config = args.getOptionString("config");

		if (config.length())
			gzKeyDatabase::setLocalRegistry(config);

		gzString plugins = args.getOptionString("plugins");

		gzString in = gzRegKeyExpandedString(args.getOptionValue("in", "udp::45455?nic=127.0.0.1&blocking=no&echo=no"));

		gzString out = gzRegKeyExpandedString(args.getOptionValue("out", "con:?lf=yes&echo=yes"));

		gzString command = gzRegKeyExpandedString(args.getOptionValue("command", ""));

		args.checkArgumentCount(1);
		
		gzString welcome = gzString::formatString("* Starting terminal: in:\"%s\"\t out:\"%s\"\r\n", in, out, GZ_VERSION_STR);
		
		if (plugins.length())
			gzModule::loadModules(plugins, FALSE, TRUE, TRUE, command);


		gzSerializeAction in_action = GZ_SERIALIZE_INPUT;

		if (args.hasOption("duplex"))
			in_action = GZ_SERIALIZE_DUPLEX;

		gzSerializeAdapterPtr	adapter_in = gzSerializeAdapter::getURLAdapter(in, in_action);

		if (!adapter_in || adapter_in->hasError())
		{
			GZMESSAGE(GZ_MESSAGE_FATAL, "Error in input adapter %s", in);
			exit(99);
		}

		gzSerializeAdapterPtr	adapter_out = gzSerializeAdapter::getURLAdapter(out, GZ_SERIALIZE_DUPLEX);

		if (!adapter_out || adapter_out->hasError())
		{
			GZMESSAGE(GZ_MESSAGE_FATAL, "Error in output adapter %s", out);
			exit(99);
		}

		if (args.hasOption("ping"))
		{
			printf("--------------- Sending PING --------------\n");

			gzSerializeString ping = gzSerializeString("Ping\r", '\n');

			while (TRUE)
			{
				ping.write(adapter_in);
				gzSleep(1000);
			}
		}

		adapter_out->write((gzUByte*)welcome.getString(), welcome.length());

		adapter_out->flush();

		while (adapter_in && adapter_out && !adapter_in->hasError())
		{
			if (adapter_in->length())
			{
				gzUInt32 len = adapter_in->length();

				gzArray<gzUByte> data(len);

				len = adapter_in->read(data.getAddress(), len);

				if (len)
				{

					adapter_out->write(data.getAddress(), len);

					adapter_out->flush();
				}
			}

			if (adapter_out->length())
			{
				gzUInt32 len = adapter_out->length();

				gzArray<gzUByte> data(len);

				len = adapter_out->read(data.getAddress(), len);

				if (len)
				{
					adapter_in->write(data.getAddress(), len);

					adapter_in->flush();
				}
			}
		}

	}
	catch(gzBaseError &error)	// In case of exceptions thrown we want to print the message
	{
 		error.reportError();
	}

	
	return 0;
}

