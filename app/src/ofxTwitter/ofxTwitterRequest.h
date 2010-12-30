/*
 *  ofxTwitterRequest.h
 *  openFrameworks
 *
 *  Created by Todd Vanderlin on 11/22/09.
 *  Copyright 2009 Interactive Design. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
#include "ofxTweet.h"
#include "ofxTwitterProfile.h"

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/StreamCopier.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/BasicEvent.h"
#include "Poco/Delegate.h"

#include <iostream>
#include <vector>
#include <istream.h>

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTMLForm;
using Poco::Net::HTTPMessage;
using Poco::StreamCopier;
using Poco::Path;
using Poco::URI;
using Poco::Exception;
using Poco::BasicEvent;
using Poco::Delegate;

class ofxTwitterRequest {

public:
	
	int		type;
	int		id;
	bool	bDone;
	bool	bRequestMade;
	bool	bHasResponse;
	int		status;
	string	url;
	string  content;
		
	vector <ofxTweet>			tweets;
	vector <ofxTwitterProfile>	profiles;
	
	
	ofxTwitterRequest() {
		type		 = -1;
		id			 = -1;
		bDone		 = false;
		bRequestMade = false;
		bHasResponse = false;
		
		status		 = -1;
		content		 = "";
		url			 = "";
	}
	
	void setupResponse(HTTPResponse &pocoResponse, istream &bodyStream){
		
		status = pocoResponse.getStatus();
		
		if(status == 200) {
			//timestamp = pocoResponse.getDate();
			//reasonForStatus = pocoResponse.getReasonForStatus(pocoResponse.getStatus());
			//contentType = pocoResponse.getContentType();
			
			StreamCopier::copyToString(bodyStream, content);
			
		}
		else {
			bDone = true;
		}
	}
	
};








