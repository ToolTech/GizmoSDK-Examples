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
// File			: txfUnpack.cpp
// Module		: 
// Description	: Convrter for Terrapage txf packed files
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
// AMO  050615  Created file    
//
// ******************************************************************************

#include "gzBaseLibrary.h"

#define SYNC(x)		readAdapter(in,x,FALSE);if(out)writeAdapter(out,x,FALSE);eaten+=getDataSize(in,x,FALSE);
#define SYNCBIN(x)	data.setSize(x);in->read(data.getAddress(),x);if(out)out->write(data.getAddress(),x);eaten+=x;

gzDynamicArray<gzUByte>	data;

gzSerializeAdapter *in=NULL;
gzSerializeAdapter *out=NULL;

class TxpInfo
{
public:
};

gzArray<TxpInfo> txpInfo;

enum ArchiveMode
{
	MODE_INFO,
	MODE_UNPACK

} arc_mode=MODE_INFO;

const gzUInt32 TXP_MAGIC=	0x0090a8b4;


// Tokens

const gzUInt16 TXP_PUSH						=100;
const gzUInt16 TXP_POP						=101;
const gzUInt16 TXP_HEADER					=200;
const gzUInt16 TXP_HEAD_LODINFO				=201;

const gzUInt16 TXP_MATTABLE					=300;
const gzUInt16 TXP_MATTABLE2				=301;
const gzUInt16 TXP_SHORTMATTABLE			=302;

const gzUInt16 TXP_MATERIAL					=400;
const gzUInt16 TXP_MAT_BASIC				=401;
const gzUInt16 TXP_MAT_SHADE				=402;
const gzUInt16 TXP_MAT_SIZES				=403;
const gzUInt16 TXP_MAT_CULL					=404;
const gzUInt16 TXP_MAT_ALPHA				=405;
const gzUInt16 TXP_MAT_NORMAL				=406;
const gzUInt16 TXP_MAT_TEXTURE				=407;
const gzUInt16 TXP_MAT_BUMP					=408;
const gzUInt16 TXP_MAT_ATTR					=409;

const gzUInt16 TXP_MAT_TEXENV				=500;
const gzUInt16 TXP_MAT_TXENV_MODE			=501;
const gzUInt16 TXP_MAT_TXENV_FILTER			=502;
const gzUInt16 TXP_MAT_TXENV_WRAP			=503;
const gzUInt16 TXP_MAT_TXENV_BORDER			=504;

const gzUInt16 TXP_TEXTABLE					=600;
const gzUInt16 TXP_TEXTABLE2				=601;
const gzUInt16 TXP_TEXTURE					=650;

const gzUInt16 TXP_MODELREF					=700;

const gzUInt16 TXP_MODELTABLE				=800;

const gzUInt16 TXP_TILETABLE				=900;
const gzUInt16 TXP_TILE_ENTRY				=901;
const gzUInt16 TXP_TILETABLE2				=902;

const gzUInt16 TXP_TILEHEADER				=1000;
const gzUInt16 TXP_TILE_MATLIST				=1001;
const gzUInt16 TXP_TILE_MODELLIST			=1002;
const gzUInt16 TXP_TILE_DATE				=1003;
const gzUInt16 TXP_LOCALMATERIAL			=1004;
const gzUInt16 TXP_TILE_LOCMATLIST			=1005;


const gzUInt16 TXP_LIGHTTABLE				=1100;
const gzUInt16 TXP_LIGHTATTR				=1150;
const gzUInt16 TXP_LIGHTATTR_BASIC			=1151;
const gzUInt16 TXP_LIGHTATTR_RASCAL			=1152;
const gzUInt16 TXP_LIGHTATTR_CALLIGRAPHIC	=1153;
const gzUInt16 TXP_LIGHTATTR_PFB			=1154;
const gzUInt16 TXP_LIGHTATTR_ANIMATION		=1155;
const gzUInt16 TXP_LIGHT					=1160;

const gzUInt16 TXP_RANGETABLE				=1200;
const gzUInt16 TXP_RANGE					=1201;


const gzUInt16 TXP_GROUP					=2001;
const gzUInt16 TXP_BILLBOARD				=2002;
const gzUInt16 TXP_LOD						=2003;
const gzUInt16 TXP_TRANSFORM				=2004;
const gzUInt16 TXP_MODELREF2				=2005;
const gzUInt16 TXP_LAYER					=2006;

const gzUInt16 TXP_GEOMETRY					=3000;
const gzUInt16 TXP_GEOM_PRIM				=3001;
const gzUInt16 TXP_GEOM_MATERIAL			=3002;
const gzUInt16 TXP_GEOM_VERT32				=3003;
const gzUInt16 TXP_GEOM_VERT64				=3004;
const gzUInt16 TXP_GEOM_NORM32				=3005;
const gzUInt16 TXP_GEOM_NORM64				=3006;
const gzUInt16 TXP_GEOM_COLOR				=3007;
const gzUInt16 TXP_GEOM_TEX32				=3008;
const gzUInt16 TXP_GEOM_TEX64				=3009;
const gzUInt16 TXP_GEOM_EFLAG				=3010;

const gzUInt16 TXP_ATTACH					=4000;


class TxpTileInfo
{
public:

	int fileid;

	int offset;

};

gzArray<gzMatrix<TxpTileInfo> > tileInfo;

gzVoid unPackTiles()
{
	gzString filename;
	gzUInt32 sx,sy;

	for(gzUInt32 i=0;i<tileInfo.getSize();i++)
	{
		tileInfo[i].getSize(sx,sy);

		printf("--- Creating Archive Tiles for level %d(%d): (%dx%d) ---\n",i,tileInfo.getSize()-1,sx-1,sy-1);

		for(gzUInt32 x=0;x<sx;x++)
		{
			for(gzUInt32 y=0;y<sy;y++)
			{
				filename.format("%s%stileFile_%d.tpf",in->getURLBase().getString(),in->getDefaultPath().getString(),tileInfo[i][x][y].fileid);
				gzSerializeAdapter *file_in=gzSerializeAdapter::getURLAdapter(filename);

				if(!file_in)
					gzThrowFatal("Couldn't open archive tile URL '%s' for reading",(const char *)filename);

				filename.format("tile_%d_%d_%d.tpt",x,y,i);

				printf("Creating Archive Tile: %s\n",(const char *)filename);

				gzSerializeAdapter *file_out=gzSerializeAdapter::getURLAdapter(filename,GZ_SERIALIZE_OUTPUT);

				if(!file_out)
					gzThrowFatal("Couldn't open archive tile URL '%s' for writing",(const char *)filename);

				data.setSize(tileInfo[i][x][y].offset);
				file_in->read(data.getAddress(),tileInfo[i][x][y].offset);

				gzUInt32 filesize;

				readAdapter(file_in,filesize,FALSE);

				data.setSize(filesize);
				file_in->read(data.getAddress(),filesize);
				file_out->write(data.getAddress(),filesize);

				delete file_out;
				delete file_in;

			}
		}
	}
}

gzUInt32 processRecords(gzUInt32 length,const gzString &prefix=GZ_EMPTY_STRING);

gzUInt32 processTileTable2(gzUInt32 length,const gzString &prefix)
{
	if(arc_mode==MODE_INFO)
		printf("%sRecord:TILETABLE2 , Length:%ld\n",(const char *)prefix,length);

	gzUInt32 eaten(0);

	gzUInt32 tilemode;

	readAdapter(in,tilemode,FALSE);
	if(out)
		writeAdapter(out,gzUInt32(1),FALSE);
	eaten+=4;

	if(!tilemode)	// Local archive
	{
		int numLod;

		SYNC(numLod);

		tileInfo.setSize(numLod);

		int sx,sy;

		float elevMin,elevMax;

		for(int i=0;i<numLod;i++)
		{
			SYNC(sx);
			SYNC(sy);

			tileInfo[i].setSize(sx,sy);

			for(int x=0;x<sx;x++)
			{
				for(int y=0;y<sy;y++)
				{
					SYNC(tileInfo[i][x][y].fileid);
					SYNC(tileInfo[i][x][y].offset);
				}
			}
			for(int x=0;x<sx;x++)
			{
				for(int y=0;y<sy;y++)
				{
					SYNC(elevMin);
					SYNC(elevMax);
				}
			}
		}

		if(arc_mode==MODE_UNPACK)
			unPackTiles();
	}
	else
	{
	}

	

	return eaten;
}


gzUInt32 processHeader(gzUInt32 length,const gzString &prefix)
{
	if(arc_mode==MODE_INFO)
		printf("%sRecord:HEADER , Length:%ld\n",(const char *)prefix,length);

	gzUInt32 eaten(0);

	SYNCBIN(73);

	gzInt32 numLods;

	SYNC(numLods);

	if(numLods<0)
		return eaten;

	txpInfo.setSize(numLods);

	eaten+=processRecords(length-eaten-4,prefix+"    ");

	return eaten;
}

gzUInt32 processRecords(gzUInt32 length,const gzString &prefix)
{
	gzUInt16 recordType;
	gzUInt32 recordLength;

	gzUInt32 eaten(0);

	while(length)
	{
		SYNC(recordType);
		SYNC(recordLength);

		if(arc_mode==MODE_INFO)
			printf("%s---------------------------------------------\n",(const char *)prefix);

		length-=6;

		gzUInt32 eatenBytes=0;

		switch(recordType)
		{
			case TXP_HEADER:
				eatenBytes=processHeader(recordLength,prefix);
				break;

			case TXP_TILETABLE2:
				eatenBytes=processTileTable2(recordLength,prefix);
				break;

			default:
				if(arc_mode==MODE_INFO)
					printf("%sRecord:%d , Length:%ld\n",(const char *)prefix,recordType,recordLength);
				break;

		}

		eaten+=eatenBytes;

		if(recordLength>eatenBytes)
		{
			SYNCBIN(recordLength-eatenBytes);
		}

		length-=recordLength;
	}

	return eaten;
}



int main(int argc, char *argv[])
{
	gzMessage::setMessageLevel(GZ_MESSAGE_FATAL);

	gzUInt32 eaten(0);

	try
	{
		gzArgumentParser arg(argc,argv);

		gzString archiveName=arg.getOptionValue("url","archive.txp");

		if(arg.hasOption("unpack"))
			arc_mode=MODE_UNPACK;

		in=gzSerializeAdapter::getURLAdapter(archiveName);

		if(!in)
			gzThrowFatal("Couldn't open archive URL '%s'",(const char *)archiveName);

		gzUInt32 magic;

		if(arc_mode==MODE_UNPACK)
		{
			out=gzSerializeAdapter::getURLAdapter("tmp.txp",GZ_SERIALIZE_OUTPUT);

			if(!out)
				gzThrowFatal("Couldn't open temprary txp archive 'tmp.txp' for writing");
		}

		SYNC(magic);
		
		if(magic!=TXP_MAGIC)
			gzThrowFatal("File '%s' is not a valid txp archive",(const char *)archiveName);


		gzUInt32 length;

		SYNC(length);

		processRecords(length);

		if(in->hasData())
			GZTRACE("Oah");

		delete in;

		if(out)
			delete out;

		in=NULL;
		out=NULL;

	}
	catch(gzBaseError &error)
	{
		error.reportError();
	}
	return 0;
}

