/*
 *  ofxTwitterEvents.h
 *  openFrameworks
 *
 *  Created by Todd Vanderlin on 11/22/09.
 *  Copyright 2009 Interactive Design. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"

class TwitterResponseArgs : public ofEventArgs {
public:

	int		type;		// the type of twitter request
	int		id;			// web id called
	string  content;	// the body <- temp

	vector <ofxTweet>			tweets;
	vector <ofxTwitterProfile>	users;
};


class TwitterErrorArgs : public ofEventArgs {
public:
	int id;
};


