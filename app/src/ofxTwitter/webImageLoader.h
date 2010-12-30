/*
 *  WebImage.h
 *  openFrameworks
 *
 *  Created by Todd Vanderlin on 11/21/09.
 *  Copyright 2009 Interactive Design. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"

#include "Poco/URIStreamOpener.h"
#include "Poco/StreamCopier.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/Net/HTTPStreamFactory.h"
#include <memory>
#include <iostream>

using Poco::URIStreamOpener;
using Poco::StreamCopier;
using Poco::Path;
using Poco::URI;
using Poco::Exception;
using Poco::Net::HTTPStreamFactory;

static bool factoryLoaded = false;

class webImageLoader : public ofImage {
	
	
	public :
	
	void loadFromUrl(string url){
		printf("%s\n", url.c_str());
		//poco is not happy if we register the factory more than once
		if(!factoryLoaded){
			HTTPStreamFactory::registerFactory();
			factoryLoaded = true;
		}
		
		//specify out url and open stream
		URI uri(url);     
		std::auto_ptr<std::istream> pStr(URIStreamOpener::defaultOpener().open(uri));
		
		//copy to our string
		string str;       
		StreamCopier::copyToString(*pStr.get(), str);
		
		//figure out how many bytes the image is and allocate
		int bytesToRead = str.size();
		char buff[bytesToRead];
		memset(buff, 0, bytesToRead);
		
		//copy the bytes from the string to our buffer
		for(int i = 0; i < bytesToRead; i++){
			buff[i] = str[i];
		}
		
		//printf("numBytes copied is %i \n", sizeof(buff));
		
		//if we already have a loaded image clear it
		// if(isValid()){
		clear();     
		// }
		
		//create a freeimage memory handle from the buffer address
		FIMEMORY *hmem = NULL;
		hmem = FreeImage_OpenMemory((BYTE *)buff, bytesToRead);
		if (hmem == NULL){ printf("couldn't create memory handle! \n"); return; }
		
		//get the file type!
		FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromMemory(hmem);
		
		//make the image!!
		putBmpIntoPixels(FreeImage_LoadFromMemory(fif, hmem, 0), myPixels);
		//  bmp = FreeImage_LoadFromMemory(fif, hmem, 0);
		
		//free our memory
		FreeImage_CloseMemory(hmem);
		
		if (getBmpFromPixels(myPixels) == NULL){ printf("couldn't create bmp! \n"); return; }
		
		//flip it!
		FreeImage_FlipVertical(getBmpFromPixels(myPixels));
		
		if (myPixels.bAllocated == true && bUseTexture == true){
			tex.allocate(myPixels.width, myPixels.height, myPixels.glDataType);
		}   
		
		swapRgb(myPixels);
		
		update();
	}   
	
}; 