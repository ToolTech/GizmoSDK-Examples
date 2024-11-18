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
// File			: fontgen.cpp
// Module		: 
// Description	: Test app to virify font performance
// Author		: Anders Modén          
// Product		: Gizmo3D 2.12.199
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
// AMO  060214  Created file    
//
// ******************************************************************************

#include "gzGizmo3DLibrary.h"

int main(int argc, char *argv[])
{
	gzArgumentParser args(argc,argv);

	gzMessage::setMessageLevel(GZ_MESSAGE_MEM_DEBUG);

	gzXmlDom fonts;

	gzSerializeAdapter *xmladapter=gzSerializeAdapter::getURLAdapter(args.getOptionValue("xml","fontgen.xml"));

	if(!xmladapter)
		return 99;

	fonts.read(xmladapter);

	if(fonts.hasError())
		return 99;

	gzSerializeAdapter *adapter=gzSerializeAdapter::getURLAdapter("fonts.gzf",GZ_SERIALIZE_OUTPUT);

	adapter=new gzSerializeAdapterCompress(adapter);

	gzXmlItem *top=fonts.findItem("//fonts");

	if(top)
	{
		while(gzXmlItem	*item=top->getNextChild())
		{
			gzXmlElement *element=gzDynamic_Cast<gzXmlElement>(item);

			if(element)
			{
				gzFont *font=gzFont::getFont(gzFontInfo(	element->getAttribute("fontName"),
															element->getAttribute("charcterCount").num(),
															element->getAttribute("genericHeight").num(),
															element->getAttribute("genericWidth").num(),
															element->getAttribute("italic").yesNo(),
															element->getAttribute("bold").yesNo(),
															element->getAttribute("underline").yesNo(),
															element->getAttribute("strikeout").yesNo(),
															element->getAttribute("antialias").yesNo()),FALSE);

				if(font)
					font->write(adapter);
			}
		}
	}

	delete adapter;


	return 0;
}
