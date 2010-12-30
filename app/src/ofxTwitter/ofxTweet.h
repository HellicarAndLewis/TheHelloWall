/*
 *  ofxTweet.h
 *  openFrameworks
 *
 *  Created by Todd Vanderlin on 11/21/09.
 *  Copyright 2009 Interactive Design. All rights reserved.
 *
 */

/*
 <created_at>Fri Nov 20 03:14:18 +0000 2009</created_at>
 <id>5878424310</id>
 <text>@memotv this is so good!</text>
 <source>web</source>
 <truncated>false</truncated>
 <in_reply_to_status_id>5871502095</in_reply_to_status_id>
 <in_reply_to_user_id>12087912</in_reply_to_user_id>
 <favorited>false</favorited>
 <in_reply_to_screen_name>memotv</in_reply_to_screen_name>
 +
 <user>
 <id>14370619</id>
 <name>vanderlin</name>
 <screen_name>vanderlin</screen_name>
 <location/>
 <description/>
 −
 <profile_image_url>
 http://a3.twimg.com/profile_images/52719653/n34602390_30732958_5321_normal.jpg
 </profile_image_url>
 <url/>
 <protected>false</protected>
 <followers_count>155</followers_count>
 <profile_background_color>C6E2EE</profile_background_color>
 <profile_text_color>663B12</profile_text_color>
 <profile_link_color>1F98C7</profile_link_color>
 <profile_sidebar_fill_color>DAECF4</profile_sidebar_fill_color>
 <profile_sidebar_border_color>C6E2EE</profile_sidebar_border_color>
 <friends_count>28</friends_count>
 <created_at>Sat Apr 12 19:36:45 +0000 2008</created_at>
 <favourites_count>0</favourites_count>
 <utc_offset>-18000</utc_offset>
 <time_zone>Quito</time_zone>
 −
 <profile_background_image_url>
 http://s.twimg.com/a/1258674567/images/themes/theme2/bg.gif
 </profile_background_image_url>
 <profile_background_tile>false</profile_background_tile>
 <statuses_count>77</statuses_count>
 <notifications>false</notifications>
 <geo_enabled>false</geo_enabled>
 <verified>false</verified>
 <following>true</following>
 </user>
 <geo/>
 */
#pragma once
#include "ofMain.h"


class ofxTweet {
	
	
public:
	
	int		id;
	string  name;
	string  screen_name;
	string  profile_url;
	
	string	text;
	
	ofxTweet() {
		text = "";
	}
	
};
