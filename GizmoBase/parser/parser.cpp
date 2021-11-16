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
// File			: parser.cpp
// Module		: 
// Description	: An example of a generic parser implementing a calculator
// Author		: Anders Modén          
// Product		: Gizmo3D 2.11.48
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
// Who  Date    Description                                             
//                                                                      
// AMO  020102  Created file  
//
// ******************************************************************************

#include "gzBaseLibrary.h"

char bnf[]="																		\
				signed_integer	::= '+'? $ integer | '-' $ integer fNeg ;			\
				group			::= '(' expr ')';									\
				expr1			::= signed_integer | group;							\
				expr2			::= expr1 (('*' expr1 fMul) | ('/' expr1 fDiv))*;	\
				expr			::= expr2 (('+' expr2 fAdd) | ('-' expr2 fSub))*;	\
";

class gzGenericParser : public gzParserFunction<gzGenericParser,gzBasicParser>
{
public:

	gzGenericParser()
	{
		registerFunction("integer",&gzGenericParser::parseInteger);

		registerFunction("fMul",&gzGenericParser::fMul);
		registerFunction("fDiv",&gzGenericParser::fDiv);
		registerFunction("fAdd",&gzGenericParser::fAdd);
		registerFunction("fSub",&gzGenericParser::fSub);
		registerFunction("fNeg",&gzGenericParser::fNeg);

		gzString bnfURL;
		bnfURL.format("mem:%d,%d",bnf,sizeof(bnf)-1);
		registerRulesFromURL(bnfURL);
	}

	gzParseResult fMul()
	{
		gzTrace::TraceMessage("Mul:%ld\n",getItemID());
		m_stack.push(m_stack.pop()*m_stack.pop());
		return GZ_PARSE_OK;
	}
	
	gzParseResult fSub()
	{
		gzTrace::TraceMessage("Sub:%ld\n",getItemID());
		m_stack.push(-(m_stack.pop()-m_stack.pop()));
		return GZ_PARSE_OK;
	}

	gzParseResult fDiv()
	{
		gzTrace::TraceMessage("Div:%ld\n",getItemID());
		m_stack.push(1/(m_stack.pop()/m_stack.pop()));
		return GZ_PARSE_OK;
	}

	gzParseResult fAdd()
	{
		gzTrace::TraceMessage("Add:%ld\n",getItemID());
		m_stack.push(m_stack.pop()+m_stack.pop());
		return GZ_PARSE_OK;
	}

	gzParseResult fNeg()
	{
		gzTrace::TraceMessage("Neg:%ld\n",getItemID());

		m_stack.push(-m_stack.pop());

		return GZ_PARSE_OK;
	}



	gzParseResult parseInteger()
	{
		gzParseResult result=gzBasicParser::parseInteger();

		if(result==GZ_PARSE_OK)
		{
			gzString number=getPushedTokenData();

			gzTrace::TraceMessage("%ld:%ld\n",(gzUInt32)number.num(),getItemID());

			m_stack.push(number.num());
		}

		return result;
	}


	gzVoid virtual onRejectItem(gzUInt32 id)
	{
		//printf("reject %ld\n",getItemID());
	}

	gzQueue<gzDouble>	m_stack;
};

 
GZ_DECLARE_RECYCLE_DATA_IMP;

int main()
{
	gzMessage::setMessageLevel(GZ_MESSAGE_PERF_DEBUG);

	gzGenericParser puff;

	gzString expression="(2-3*5)/6+3*4";

	gzSerializeAdapter *adapter=new gzSerializeAdapterString(&expression);

	puff.setAdapter(adapter);

	puff.setEnableTrace(TRUE);

	//puff.setEnablePerfMon(TRUE);

	gzParseResult result=puff.parseRule("expr");

	if(result==GZ_PARSE_ERROR)
		GZMESSAGE(GZ_MESSAGE_DEBUG,"Error in parser '%s'",puff.getError());

	gzDouble val=puff.m_stack.pop();

	GZMESSAGE(GZ_MESSAGE_DEBUG,"Evaluated:%s=%f",expression,val);

	gzUInt32 len=adapter->length();

	//gzDumpPerformanceInfo();

	return 0;
}
