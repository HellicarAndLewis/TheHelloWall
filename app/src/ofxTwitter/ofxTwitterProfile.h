/*
 *  ofxTwitterProfile.h
 *  openFrameworks
 *
 *  Created by Todd Vanderlin on 11/21/09.
 *  Copyright 2009 Interactive Design. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
	//#include "webImageLoader.h"

class ofxTwitterProfile {

public:
	
	int		id;
	string  name;
	string  screen_name;
	string  profile_image_url;
	int		followersCount;
	int		friendsCount;
	int		statusCount;
	
	ofImage profileImage;
	
	ofxTwitterProfile() {
		id = -1;
		name = "";
		screen_name = "";
		profile_image_url = "";
		followersCount = -1;
		friendsCount = -1;
		statusCount = -1;
	}
	
	void loadProfileImage() {
//		if(profile_image_url.compare("") != -1) {
//			webImageLoader temp;
//			temp.loadFromUrl(profile_image_url);
//			profileImage = temp;
//		}
//		else {
			printf("JGL took images out. No users yet\n");
//		}

	}
	
	void print() {
		printf("name:%s\n", name.c_str());
		printf("screen name:%s\n", screen_name.c_str());
		printf("id:%i\n", id);
		printf("img url:%s\n", profile_image_url.c_str());
		printf("Followers:%i\n", followersCount);
		printf("friends cnt:%i\n", friendsCount);
		printf("status cnt:%i\n", statusCount);
	}

};