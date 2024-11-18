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
// File			: PurlServer.cpp
// Module		: 
// Description	: Starts a pipe url server
// Author		: Anders Modén		
// Product		: GizmoBase 2.12.199
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
// AMO	040205	Created file 	
// AMO	230228	Added -machid to require machid in connect option (2.12.63)
//
// ******************************************************************************

#include "gzBaseLibrary.h"
#include "PurlServer.h"

gzLoggerPtr logger_save;
gzLoggerPtr logger_con;

// Product feature app mask
const gzUInt32 PRODUCT_PURLSERVER = (1 << 26);

const gzString service_name			= "PurlServer";
const gzString service_comment		= "PurlServer Pipe URL services";
const gzString EXT_XML				= "xml";

const gzString GZ_PURL_SERVICE_REG_KEY_SETTINGS = "purlserver/settings/";

// Installed probe ----------------------------------------------------------------------

gzString PurlServerProbeFunction()
{
	gzService* instance = gzService::getInstance();

	if (instance)	// We have a scenebuilder service
	{
		gzString result = "---------------- PurlServer status ----------------";

		result = gzString::formatString("%s\nUptime:%lf (s)", result, gzTime::uptimeSeconds());

		PurlService* service = (PurlService*)instance;

		gzPipeURLServer* server = service->getServer();

		if (server)
		{
			result = gzString::formatString("%s\nTopic: %s", result, server->getPipeName());

			result = gzString::formatString("%s\nDescription: %s", result, server->getDescription());

			//result = gzString::formatString("%s\nConnection URL: '%s'", result, server->getConnectionURL());

			//result = gzString::formatString("%s\nPending Work: %d", result, server->getPendingWork());

			//result = gzString::formatString("%s\nPending Errors: %d", result, server->getPendingErrors());
		}

		return result;
	}

	return GZ_EMPTY_STRING;
}
// ------------------- Graphics Settings ---------------------------------------------

#ifdef GZ_WINDOWS

// Use discrete GPU by default.
extern "C"
{
	// http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;

	// http://developer.amd.com/community/blog/2015/10/02/amd-enduro-system-for-developers/
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

#endif

gzVoid setLowPerformanceGraphics()
{
#ifdef GZ_WINDOWS

	NvOptimusEnablement = 0;
	AmdPowerXpressRequestHighPerformance = 0;

#endif
}

// --------------------- CleanUp structure -------------------------------------------

class CleanUpService : public gzReference
{
public:

	virtual ~CleanUpService()
	{

	}

	virtual gzBool	releaseRefs() override 
	{
		gzModule::unLoadModules();
		return TRUE; 
	}
};

gzCleanupReference _cleanUp = gzCleanupReference(new CleanUpService,GZ_CLEANUP_MODULES);


// ------------------- PurlService --------------------------------------------------

PurlService::PurlService():gzService(service_name, service_comment)
{

}

PurlService::~PurlService()
{

}

gzUInt32 PurlService::getExitCode() const
{
	// 99 will trigger exception in service

	// Override if we want to get different exit behaviour

	return gzService::getExitCode();
}

gzVoid PurlService::process(const gzArgumentParser& args, gzBool runAsService)
{
	gzAttachConsole();
		
	// --------------- Debug enabling -----------------------------------------------

	if (args.hasOption("debug"))
		gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG | GZ_MESSAGE_API_INTERNAL);

	// Check application arguments --------------------------------------------------

	gzString applicationName = args.getArgument(0);

	GZMESSAGE(GZ_MESSAGE_DEBUG, "App Name:%s", applicationName);

	if (applicationName.contains(GZ_STRING_SLASH))
		applicationName = applicationName.rightOf(GZ_STRING_SLASH);

	if (applicationName.contains(GZ_STRING_BACKSLASH))
		applicationName = applicationName.rightOf(GZ_STRING_BACKSLASH);

	if (applicationName.contains(GZ_STRING_DOT))
		applicationName = applicationName.leftOf(GZ_STRING_DOT, FALSE);

	gzString buildExt = GZ_LIB_EXT;

	if (applicationName.right(buildExt.length()) == buildExt)
		applicationName = applicationName.left(applicationName.length() - buildExt.length());

	gzString defaultResourceName = applicationName + GZ_STRING_DOT + EXT_XML;

	// Setup configuration ---------------------------------------------------------

#if defined GZ_WINDOWS

// Set current dir on unicode windows

	GZMESSAGE(GZ_MESSAGE_DEBUG, "Current Path:%s", getPath());
	SetCurrentDirectoryW(getPath().getWideString());

#else

	chdir(getPath());

	GZ_XXX;

#endif

	// config activation ---------------------------------------------------------

	gzString configURL = args.getOptionValue("config", defaultResourceName);

	GZMESSAGE(GZ_MESSAGE_NOTICE, "Using config file '%s'", configURL);

	gzString errorString;
	gzSerializeAdapterError error;

	if (gzFileExist(configURL))
	{
		if (gzFeature::readFeatures(configURL, "//feature_data"))
		{
			if (!gzKeyDatabase::setLocalRegistry(configURL, args.getOptionValue("password", GZ_EMPTY_STRING), &errorString, &error, gzFeature::hasAllowedFeature("XML_Config_Checksum")))
			{
				GZMESSAGE(GZ_MESSAGE_FATAL, "Failed to set config file '%s' : (%s)", configURL, errorString);
				return;
			}
		}
		else
		{
			GZMESSAGE(GZ_MESSAGE_NOTICE, "Failed to use features in file '%s'", configURL);
		}
	}
	else
	{
		GZMESSAGE(GZ_MESSAGE_NOTICE, "Couldn't find config file '%s'.", configURL);

		if (configURL.contains(GZ_STRING_SLASH))
			configURL = configURL.rightOf(GZ_STRING_SLASH);

		if (configURL.contains(GZ_STRING_BACKSLASH))
			configURL = configURL.rightOf(GZ_STRING_BACKSLASH);

		configURL = gzString("c:/windows/") + configURL;

		GZMESSAGE(GZ_MESSAGE_NOTICE, "Using alternative config file '%s'.", configURL);

		if (gzFeature::readFeatures(configURL, "//feature_data"))
		{
			if (!gzKeyDatabase::setLocalRegistry(configURL, args.getOptionValue("password", GZ_EMPTY_STRING), &errorString, &error, gzFeature::hasAllowedFeature("XML_Config_Checksum")))
			{
				GZMESSAGE(GZ_MESSAGE_FATAL, "Failed to set config file '%s' : (%s)", configURL, errorString);
				return;
			}
		}
		else
		{
			GZMESSAGE(GZ_MESSAGE_NOTICE, "Failed to use features in file '%s'", configURL);
		}
	}


	// Arg Settings ----------------------------------------------------------------

	if (args.hasOption("logg"))
	{
		logger_save = new gzLogger(gzRegKeyExpandedString(args.getOptionValue("logg", gzKeyDatabase::getDefaultUserKey(GZ_PURL_SERVICE_REG_KEY_SETTINGS + "logg", "udp::45455?nic=127.0.0.1"))));
		logger_con = new gzLogger("con:");
	}
	
	// Setup probe

	gzProgramExecution::setProbeFunction(PurlServerProbeFunction);

	// Check options ---------------------------------------------------------------

	gzString options = getSetting(args, "options", GZ_EMPTY_STRING);

	gzString basePath = getSetting(args, "base", GZ_EMPTY_STRING);

	gzString plugins = getSetting(args, "plugins", GZ_EMPTY_STRING);

	gzString urlBases = getSetting(args, "urlbases", GZ_EMPTY_STRING);

	gzString description = getSetting(args, "description", "Saab PurlServer Service");

	gzString whiteList = getSetting(args, "whitelist", GZ_EMPTY_STRING);

	gzString blackList = getSetting(args, "blacklist", GZ_EMPTY_STRING);

	gzString topic= getSetting(args, "topic", GZ_EMPTY_STRING);

	// Check arguments for service ------------------------------------------------

	if (!topic)
	{
		args.checkArgumentCount(2);
		topic = args.getArgument(1);
	}

	GZMESSAGE(GZ_MESSAGE_NOTICE, "* Starting purl:%s %s\tProtocol:%d\tGizmoSDK:%s", topic, options, GZ_PURL_PROTOCOL_VERSION, GZ_VERSION_STR);

	m_propertyServer = new gzPipeURLServer(topic, options);

	m_propertyServer->setBasePath(basePath);

	m_propertyServer->setAllowAbsoluteURL(getSetting(args, "absolute", GZ_EMPTY_STRING).yesNo());

	m_propertyServer->setAllowBackstepURL(getSetting(args, "backstep", GZ_EMPTY_STRING).yesNo());

	m_propertyServer->setAllowWrite(getSetting(args, "write", GZ_EMPTY_STRING).yesNo());

	m_propertyServer->setRequireMachID(getSetting(args, "machid", GZ_EMPTY_STRING).yesNo());

	m_propertyServer->setAllowRegistryURL(getSetting(args, "registry", GZ_EMPTY_STRING).yesNo());

	if (urlBases.length())
		m_propertyServer->setAllowedURLBases(urlBases);

	if (plugins.length())
		gzModule::loadModules(plugins, FALSE, TRUE, TRUE);

	m_propertyServer->setDescription(description);

	m_propertyServer->setBlackListMach(blackList);
	m_propertyServer->setWhileListMach(whiteList);

	if (getSetting(args, "low3d", GZ_EMPTY_STRING).yesNo())
		setLowPerformanceGraphics();

	m_propertyServer->run();

	while(isRunning())
		gzSleep(100);
}

// -------------------------- main exec ---------------------------------------------

int main(int argc , char *argv[] )
{
	PurlService service;

	gzArgumentParser args(argc, argv);

	args.setSyntaxString("<pipename> {-logg} {-debug} {-absolute yes/no} {-machid yes/no} {-write yes/no} {-registry yes/no} {-urlbases <base;base>} {-options <optionstring>} {-base <base path>} {-plugins <a;b;c>} {-description <descriptionstring>} {-whitelist <mac;mac>} {-blacklist <mac;mac>} {-config <config_url>} ");

	service.exec(args);

	int retval = service.getExitCode();

	return retval;
}

// ------------------------------ Utilities ----------------------------------------

gzString getSetting(const gzArgumentParser& args,const gzString& settingName, const gzString& defaultValue)
{
	return gzRegKeyExpandedString(args.getOptionValue(settingName, gzKeyDatabase::getDefaultUserKey(GZ_PURL_SERVICE_REG_KEY_SETTINGS + settingName, defaultValue)));
}
