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
// File			: gzImageManager_bmp.h
// Module		: 
// Description	: Class definition of the bmp format reader
// Author		: Anders Modén		
// Product		: Gizmo3D 2.11.1
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
// AMO	990301	Created file 	
//
//******************************************************************************
#ifndef __GZ_IMAGE_BMP_H__
#define __GZ_IMAGE_BMP_H__

/*!	\file 
	\brief Base definitions of a BMP bitmap reader

*/
#include "gzImage.h"
#include "gzSerialize.h"

class gzWin3xBitmapHeader : public gzSerializeData
{
public:

	gzWin3xBitmapHeader():	bmpImageFileType(FALSE),bmpFileSize(FALSE),
							bmpReserved1(FALSE),bmpReserved2(FALSE),
							bmpImageDataOffset(FALSE){};


	virtual gzVoid write(gzSerializeAdapter *adapter);

	virtual gzVoid read(gzSerializeAdapter *adapter);

	virtual gzVoid pushBack(gzSerializeAdapter *adapter);

	virtual gzUInt32	getDataSize();

	gzSerializeBinary<gzUInt16> bmpImageFileType;
	gzSerializeBinary<gzUInt32>	bmpFileSize;
	gzSerializeBinary<gzUInt16> bmpReserved1;
	gzSerializeBinary<gzUInt16> bmpReserved2;
	gzSerializeBinary<gzUInt32>	bmpImageDataOffset;
};

class gzWin3xBitmapInfoHeader : public gzSerializeData
{
public:

	gzWin3xBitmapInfoHeader():	bmpHeaderSize(FALSE),bmpImageWidth(FALSE),bmpImageHeight(FALSE),
								bmpNumberOfImagePlanes(FALSE),bmpBitsPerPixel(FALSE),bmpCompressionMethod(FALSE),
								bmpSizeOfBitmap(FALSE),bmpHorizResolution(FALSE),bmpVertresolution(FALSE),
								bmpNumColorsUsed(FALSE),bmpNumSignificantColors(FALSE){};


	virtual gzVoid write(gzSerializeAdapter *adapter);

	virtual gzVoid read(gzSerializeAdapter *adapter);

	virtual gzVoid pushBack(gzSerializeAdapter *adapter);

	virtual gzUInt32	getDataSize();

	gzSerializeBinary<gzUInt32>	bmpHeaderSize;
	gzSerializeBinary<gzUInt32>	bmpImageWidth;
	gzSerializeBinary<gzUInt32>	bmpImageHeight;
	gzSerializeBinary<gzUInt16>	bmpNumberOfImagePlanes;
	gzSerializeBinary<gzUInt16>	bmpBitsPerPixel;
	gzSerializeBinary<gzUInt32>	bmpCompressionMethod;
	gzSerializeBinary<gzUInt32>	bmpSizeOfBitmap;
	gzSerializeBinary<gzUInt32>	bmpHorizResolution;
	gzSerializeBinary<gzUInt32>	bmpVertresolution;
	gzSerializeBinary<gzUInt32>	bmpNumColorsUsed;
	gzSerializeBinary<gzUInt32>	bmpNumSignificantColors;
};

class gzWin3xPaletteEntry
{
public:
	virtual gzVoid write(gzSerializeAdapter *adapter);

	virtual gzVoid read(gzSerializeAdapter *adapter);

	virtual gzVoid pushBack(gzSerializeAdapter *adapter);

	virtual gzUInt32	getDataSize();

	gzSerializeBinary<gzUByte>	blue;
	gzSerializeBinary<gzUByte>	green;
	gzSerializeBinary<gzUByte>	red;
	gzSerializeBinary<gzUByte>	reserved;
};

class gzWin3xPalette : public gzSerializeData
{
public:
	virtual gzVoid write(gzSerializeAdapter *adapter);

	virtual gzVoid read(gzSerializeAdapter *adapter);

	virtual gzVoid pushBack(gzSerializeAdapter *adapter);

	virtual gzUInt32	getDataSize();

	gzArray<gzWin3xPaletteEntry> bmpPalette;
};

class gzWin3xData : public gzSerializeData
{
public:
	virtual gzVoid write(gzSerializeAdapter *adapter);

	virtual gzVoid read(gzSerializeAdapter *adapter);

	virtual gzVoid pushBack(gzSerializeAdapter *adapter);

	virtual gzUInt32	getDataSize();

	gzBool uncompress(gzInt32 method , gzUInt32 width , gzUInt32 height);
	gzBool compress(gzInt32 method, gzUInt32 width , gzUInt32 height);

	gzArray< gzUByte > bmpData;
};

class gzImageManager_bmp : public gzImageManager	
{
public:

	gzImageManager_bmp();

	virtual gzImage	*loadImage(gzSerializeAdapter *adapter);

	virtual gzBool  saveImage(gzImage *image , gzSerializeAdapter *adapter);

protected:

	gzImage *getImageVersion3(gzSerializeAdapter *adapter);
	gzImage *getImageVersion1(gzSerializeAdapter *adapter);

	virtual gzVoid	registerManager();
};




#endif
