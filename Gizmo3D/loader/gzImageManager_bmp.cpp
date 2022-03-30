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
// File			: gzImageManager_bmp.cpp
// Module		: 
// Description	: Class implementation of the bmp format reader
// Author		: Anders Modén		
// Product		: Gizmo3D 2.11.76
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
#include "gzImageManager_bmp.h"
#include "gzSerialize.h"

gzImageManager_bmp::gzImageManager_bmp()
{
	m_version=1;
	m_extension="bmp";
}


gzBool gzImageManager_bmp::saveImage(gzImage *image , gzSerializeAdapter *adapter)
{
	GZNOTIFY(GZ_NOTIFY_FATAL,"Save Image not yet implemented for '%s' Image Manager version %ld.",(const char *)m_extension,m_version);
	return FALSE;
}

gzImage	*gzImageManager_bmp::loadImage(gzSerializeAdapter *adapter)
{
	gzImage *image;

	gzSerializeBinary<gzUInt16> bmpVersion(FALSE);

	bmpVersion.read(adapter);
	bmpVersion.pushBack(adapter);

	if(bmpVersion==0x4d42)
		image=getImageVersion3(adapter);
	else if(bmpVersion==0x0)
		image=getImageVersion1(adapter);
	
	return image;
}

gzVoid gzImageManager_bmp::registerManager()
{
	GZNOTIFY(GZ_NOTIFY_DEBUG,"Registered '%s' Image Manager version %ld.",(const char *)m_extension,m_version);
}

gzImage *gzImageManager_bmp::getImageVersion3(gzSerializeAdapter *adapter)
{
	gzWin3xBitmapHeader			bmpHeader;
	gzWin3xBitmapInfoHeader		bmpInfo;	
	gzWin3xPalette				bmpPaletteInfo;
	gzWin3xData					bmpDataInfo;

	bmpHeader.read(adapter);

	// Check to see if reserved words are zero.
	if(bmpHeader.bmpReserved1!=0 || bmpHeader.bmpReserved2!=0)
		return NULL;

	bmpInfo.read(adapter);

	// Check to see if OS2 bitmap
	if(bmpInfo.bmpHeaderSize!=40)	// 40 bytes in MS bitmaps
		return NULL;

	if(bmpInfo.bmpSizeOfBitmap==0)	// if sloppy software engineer written bmp saver is used
		bmpInfo.bmpSizeOfBitmap = bmpInfo.bmpImageHeight * bmpInfo.bmpImageWidth;

	if( ( (bmpInfo.bmpBitsPerPixel==1) || (bmpInfo.bmpBitsPerPixel==4) || (bmpInfo.bmpBitsPerPixel==8)) && ( bmpInfo.bmpNumColorsUsed==0 ))
	{
		bmpInfo.bmpNumColorsUsed=1<<bmpInfo.bmpBitsPerPixel;
	}

	if((bmpInfo.bmpBitsPerPixel==16)||(bmpInfo.bmpBitsPerPixel==32))
	{
		GZNOTIFY(GZ_NOTIFY_WARNING,"16 or 32 bits bmp files not yet supported");
		return NULL;
	}
	
	if(bmpInfo.bmpNumColorsUsed)	// no support for palette in 0(JPEG) ,16 or 32 bits mode
	{
		bmpPaletteInfo.bmpPalette.setSize(bmpInfo.bmpNumColorsUsed);
		bmpPaletteInfo.read(adapter);
	}

	if(adapter->length()+bmpHeader.bmpImageDataOffset!=bmpHeader.bmpFileSize)
		GZNOTIFY(GZ_NOTIFY_DEBUG,"Minor size error in bmp file");
	

	//bmpDataInfo.bmpData.setSize(bmpInfo.bmpSizeOfBitmap);
	bmpDataInfo.bmpData.setSize(adapter->length());	// remaining file
	bmpDataInfo.read(adapter);

	if(bmpInfo.bmpCompressionMethod)
	{
		if(!bmpDataInfo.uncompress(bmpInfo.bmpCompressionMethod,bmpInfo.bmpImageWidth,bmpInfo.bmpImageHeight))
			return NULL;
	}

	// Each row shall be align on a 4 byte basis
	gzInt32 alignment=(4-(bmpInfo.bmpImageWidth*3)%4)%4;

	gzImage	* newImage= new gzImage_RGB_8(adapter->getName());

	gzArray<gzUByte> &imagedata=newImage->getArray();
	
	// And align it to bytepad
	
	newImage->setAlignment(4);
		
	// The size must then be
	imagedata.setSize(newImage->getArraySize(bmpInfo.bmpImageWidth,bmpInfo.bmpImageHeight)); // rgb storage

	newImage->setWidth(bmpInfo.bmpImageWidth);
	newImage->setHeight(bmpInfo.bmpImageHeight);

	gzUInt32 j,i;
	
	if(bmpInfo.bmpBitsPerPixel==24)
	{
		// Fix BGR to RGB
		for(j=0;j<bmpInfo.bmpImageHeight;j++)
		{
			for(i=0;i<bmpInfo.bmpImageWidth;i++)
			{
				imagedata[j*(bmpInfo.bmpImageWidth * 3 + alignment)+i*3+0]=bmpDataInfo.bmpData[j*(bmpInfo.bmpImageWidth * 3 + alignment)+i*3+2]; // Red
				imagedata[j*(bmpInfo.bmpImageWidth * 3 + alignment)+i*3+1]=bmpDataInfo.bmpData[j*(bmpInfo.bmpImageWidth * 3 + alignment)+i*3+1]; // Green
				imagedata[j*(bmpInfo.bmpImageWidth * 3 + alignment)+i*3+2]=bmpDataInfo.bmpData[j*(bmpInfo.bmpImageWidth * 3 + alignment)+i*3+0]; // Blue
			}
		}
	}
	else
	{
		gzUInt16 	pixelperbyte=8/bmpInfo.bmpBitsPerPixel;
		gzUInt32 	rowlen=(bmpInfo.bmpImageWidth+pixelperbyte-1)/pixelperbyte;
		
		gzUInt32 	bytepad=(4-(rowlen)%4)%4;
		
		gzInt32 k;
		gzInt32 maskvalue;
		gzInt32 mask;
		gzInt32 value;
				
		gzInt32 index;

		for(j=0;j<bmpInfo.bmpImageHeight;j++)
		{
			for(i=0;i<bmpInfo.bmpImageWidth;i++)
			{
				k=i%pixelperbyte;
				maskvalue=(1<<bmpInfo.bmpBitsPerPixel)-1;
				mask=maskvalue<<(bmpInfo.bmpBitsPerPixel*(pixelperbyte-k-1));
				value=bmpDataInfo.bmpData[j*(rowlen+bytepad)+i/pixelperbyte];
				
				index=(value&mask)>>(bmpInfo.bmpBitsPerPixel*(pixelperbyte-k-1));
				
				imagedata[j*(bmpInfo.bmpImageWidth * 3 + alignment)+i*3+0]=(gzUByte)bmpPaletteInfo.bmpPalette[index].red;
				imagedata[j*(bmpInfo.bmpImageWidth * 3 + alignment)+i*3+1]=(gzUByte)bmpPaletteInfo.bmpPalette[index].green;
				imagedata[j*(bmpInfo.bmpImageWidth * 3 + alignment)+i*3+2]=(gzUByte)bmpPaletteInfo.bmpPalette[index].blue;
						
			}
		}
	}
	return newImage;
}


gzImage *gzImageManager_bmp::getImageVersion1(gzSerializeAdapter *adapter)
{
	return NULL;
}

gzVoid gzWin3xBitmapHeader::write(gzSerializeAdapter *adapter)
{
	bmpImageFileType.write(adapter);
	bmpFileSize.write(adapter);
	bmpReserved1.write(adapter);
	bmpReserved2.write(adapter);
	bmpImageDataOffset.write(adapter);
}

gzVoid gzWin3xBitmapHeader::read(gzSerializeAdapter *adapter)
{
	bmpImageFileType.read(adapter);
	bmpFileSize.read(adapter);
	bmpReserved1.read(adapter);
	bmpReserved2.read(adapter);
	bmpImageDataOffset.read(adapter);
}

gzUInt32 gzWin3xBitmapHeader::getDataSize()
{
	return bmpImageFileType.getDataSize()+
	bmpFileSize.getDataSize()+
	bmpReserved1.getDataSize()+
	bmpReserved2.getDataSize()+
	bmpImageDataOffset.getDataSize();
}

gzVoid gzWin3xBitmapHeader::pushBack(gzSerializeAdapter *adapter)
{

	bmpImageDataOffset.pushBack(adapter);
	bmpReserved2.pushBack(adapter);
	bmpReserved1.pushBack(adapter);
	bmpFileSize.pushBack(adapter);
	bmpImageFileType.pushBack(adapter);
}

gzVoid gzWin3xBitmapInfoHeader::write(gzSerializeAdapter *adapter)
{

	bmpHeaderSize.write(adapter);
	bmpImageWidth.write(adapter);
	bmpImageHeight.write(adapter);
	bmpNumberOfImagePlanes.write(adapter);
	bmpBitsPerPixel.write(adapter);
	bmpCompressionMethod.write(adapter);
	bmpSizeOfBitmap.write(adapter);
	bmpHorizResolution.write(adapter);
	bmpVertresolution.write(adapter);
	bmpNumColorsUsed.write(adapter);
	bmpNumSignificantColors.write(adapter);

}

gzVoid gzWin3xBitmapInfoHeader::read(gzSerializeAdapter *adapter)
{
	bmpHeaderSize.read(adapter);
	bmpImageWidth.read(adapter);
	bmpImageHeight.read(adapter);
	bmpNumberOfImagePlanes.read(adapter);
	bmpBitsPerPixel.read(adapter);
	bmpCompressionMethod.read(adapter);
	bmpSizeOfBitmap.read(adapter);
	bmpHorizResolution.read(adapter);
	bmpVertresolution.read(adapter);
	bmpNumColorsUsed.read(adapter);
	bmpNumSignificantColors.read(adapter);
}

gzUInt32 gzWin3xBitmapInfoHeader::getDataSize()
{
	return bmpHeaderSize.getDataSize()+
	bmpImageWidth.getDataSize()+
	bmpImageHeight.getDataSize()+
	bmpNumberOfImagePlanes.getDataSize()+
	bmpBitsPerPixel.getDataSize()+
	bmpCompressionMethod.getDataSize()+
	bmpSizeOfBitmap.getDataSize()+
	bmpHorizResolution.getDataSize()+
	bmpVertresolution.getDataSize()+
	bmpNumColorsUsed.getDataSize()+
	bmpNumSignificantColors.getDataSize();
}

gzVoid gzWin3xBitmapInfoHeader::pushBack(gzSerializeAdapter *adapter)
{
	bmpNumSignificantColors.pushBack(adapter);
	bmpNumColorsUsed.pushBack(adapter);
	bmpVertresolution.pushBack(adapter);
	bmpHorizResolution.pushBack(adapter);
	bmpSizeOfBitmap.pushBack(adapter);
	bmpCompressionMethod.pushBack(adapter);
	bmpBitsPerPixel.pushBack(adapter);
	bmpNumberOfImagePlanes.pushBack(adapter);
	bmpImageHeight.pushBack(adapter);
	bmpImageWidth.pushBack(adapter);
	bmpHeaderSize.pushBack(adapter);
}

gzVoid gzWin3xPalette::write(gzSerializeAdapter *adapter)
{
	for(gzUInt32 i=0;i<bmpPalette.getSize();i++)
	{
		bmpPalette[i].write(adapter);
	}
}

gzVoid gzWin3xPalette::read(gzSerializeAdapter *adapter)
{
	for(gzUInt32 i=0;i<bmpPalette.getSize();i++)
	{
		bmpPalette[i].read(adapter);
	}
}

gzUInt32 gzWin3xPalette::getDataSize()
{
	gzUInt32 size=0;

	for(gzUInt32 i=0;i<bmpPalette.getSize();i++)
	{
		size+=bmpPalette[i].getDataSize();
	}
	return size;
}

gzVoid gzWin3xPalette::pushBack(gzSerializeAdapter *adapter)
{
	for(gzUInt32 i=bmpPalette.getSize();i>0;i--)
	{
		bmpPalette[i].pushBack(adapter);
	}
}

gzVoid gzWin3xData::write(gzSerializeAdapter *adapter)
{
	gzUByte *addr=bmpData.getAddress();

	gzUInt32 size=bmpData.getSize();

	adapter->write(addr,size);
}

gzVoid gzWin3xData::read(gzSerializeAdapter *adapter)
{
	gzUByte *addr=bmpData.getAddress();

	gzUInt32 size=bmpData.getSize();

	if(adapter->read(addr,size)!=size)
		GZTRACE("End of file reached !!\n");
}

gzUInt32 gzWin3xData::getDataSize()
{
	return bmpData.getSize();
}


gzVoid gzWin3xData::pushBack(gzSerializeAdapter *adapter)
{
	gzUByte *addr=bmpData.getAddress();

	gzUInt32 size=bmpData.getSize();

	for(gzUInt32 i=size;i>0;i--)
		adapter->pushBack(*(addr+i));
}

gzBool gzWin3xData::uncompress(gzInt32 method, gzUInt32 width , gzUInt32 height)
{
	if(method!=1)
		return FALSE;

	gzUInt32 srcindex=0,dstindex=0,i,runlength;
	gzArray<gzUByte> resbmpData(width*height);

	while(TRUE)
	{
		switch(bmpData[srcindex])
		{
			case 0 :
				srcindex++;
				switch(bmpData[srcindex])
				{
					case 0:
						srcindex++;
						break;
					
					case 1:
						srcindex++;
						bmpData=resbmpData;
						return TRUE;

					case 2:
						srcindex++;
						dstindex+=bmpData[srcindex++];
						dstindex+=bmpData[srcindex++]*width;
						break;

					default:
						runlength=bmpData[srcindex++];
						for(i=0;i<runlength;i++)
						{
							resbmpData[dstindex++]=bmpData[srcindex++];
						}
						if(srcindex%2)
							srcindex++;
						break;
				}
				break;

			default:
				runlength=bmpData[srcindex++];
				for(i=0;i<runlength;i++)
				{
					resbmpData[dstindex++]=bmpData[srcindex];
				}
				srcindex++;
				break;
		}
	}
}

gzBool gzWin3xData::compress(gzInt32 method, gzUInt32 width , gzUInt32 height)
{
	if(method!=1)
		return FALSE;

	return FALSE;
}

gzVoid gzWin3xPaletteEntry::write(gzSerializeAdapter *adapter)
{
	blue.write(adapter);
	green.write(adapter);
	red.write(adapter);
	reserved.write(adapter);
}
		
gzVoid gzWin3xPaletteEntry::read(gzSerializeAdapter *adapter)
{
	blue.read(adapter);
	green.read(adapter);
	red.read(adapter);
	reserved.read(adapter);
}

gzUInt32 gzWin3xPaletteEntry::getDataSize()
{
	return 	blue.getDataSize()+
			green.getDataSize()+
			red.getDataSize()+
			reserved.getDataSize();
}

gzVoid gzWin3xPaletteEntry::pushBack(gzSerializeAdapter *adapter)
{
	reserved.read(adapter);
	red.read(adapter);
	green.read(adapter);
	blue.read(adapter);
}
