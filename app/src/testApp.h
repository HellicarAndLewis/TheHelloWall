
#pragma once
#include "ofMain.h"
#include "ofxVectorMath.h"
#include "ofxBox2d.h"
#include "ofxTween.h"
#include "ofxTwitter.h"
#include "ofxXmlSettings.h"

	//for easy tokenisation of the twitter string
#include "Poco/StringTokenizer.h"
#include <iostream>
#include <queue>
#include "Poco/String.h"

using Poco::StringTokenizer;

#define TOPGRAPHICTYPETOPBORDER 40.f
#define TOPGRAPHICTYPEBOTTOMBORDER 30.f
#define MAXTIMEBETWEENPARTICLES 10.f
#define MINTIMEBETWEENPARTICLES 1.f
#define MAXPARTICLESIZE 2.f				//users can go up to twice as big as normal
#define MINPARTICLESIZE 0.5f			//or half...
#define NUMBEROFSHAKES	10
#define SCROLLMESSAGEDURATION 10000.f	//give the messages 10 seconds to scroll
#define SCROLLMESSAGEDELAY	500.f	//wait one second before a new message
#define LIFEOFPARTICLE 300.f //5 minute life span? 60*5 = 300

typedef enum {HELLO_CIRCLE, HELLO_SQUARE, HELLO_RECTANGLE, HELLO_TRIANGLE} ShapeState;


	// ------------------------------------------------- a simple extended box2d circle
class CustomParticleCircle : public ofxBox2dCircle {
	
public:
	CustomParticleCircle() {
	}
	float timeOfBirth;
	void setTimeOfBirth(float newBirthTime){
		timeOfBirth = newBirthTime;
	}
	ofColor colour;
	void setColour(ofColor newColour){
		colour = newColour;
	}
	void draw() {
		float radius = getRadius();
		
		glPushMatrix();
		glTranslatef(getPosition().x, getPosition().y, 0);
		
		ofEnableAlphaBlending();
		ofSetColor(colour.r, colour.g, colour.b,colour.a);
		ofFill();
		ofCircle(0, 0, radius);	
		ofDisableAlphaBlending();
		glPopMatrix();
		
	}
	
		
};

	// ------------------------------------------------- a simple extended box2d rect, with same width and height
class CustomParticleSquare : public ofxBox2dRect {
	
public:
	CustomParticleSquare() {
	}
	float timeOfBirth;
	void setTimeOfBirth(float newBirthTime){
		timeOfBirth = newBirthTime;
	}	
	ofColor colour;
	void setColour(ofColor newColour){
		colour = newColour;
	}
	void draw() {
			//stolen direct from ofxBox2dRect
		if(dead) return;
		
			//wow this is a pain
		b2Shape* s = body->GetShapeList();
		const b2XForm& xf = body->GetXForm();
		b2PolygonShape* poly = (b2PolygonShape*)s;
		int count = poly->GetVertexCount();
		const b2Vec2* localVertices = poly->GetVertices();
		b2Assert(count <= b2_maxPolygonVertices);
		b2Vec2 verts[b2_maxPolygonVertices];
		for(int32 i=0; i<count; ++i) {
			verts[i] = b2Mul(xf, localVertices[i]);
		}
		
		
		ofEnableAlphaBlending();
		ofSetColor(colour.r, colour.g, colour.b, colour.a);
		ofFill();
		ofBeginShape();
		for (int32 i = 0; i <count; i++) {
			ofVertex(verts[i].x*OFX_BOX2D_SCALE, verts[i].y*OFX_BOX2D_SCALE);
		}
		ofEndShape();
		
			//		ofSetColor(0, 255, 255); //bugger outline for now
//		ofNoFill();
//		ofBeginShape();
//		for (int32 i = 0; i <count; i++) {
//			ofVertex(verts[i].x*OFX_BOX2D_SCALE, verts[i].y*OFX_BOX2D_SCALE);
//		}
//		ofEndShape(true);	
		ofDisableAlphaBlending();
	}
	
	
};

	// ------------------------------------------------- a simple extended box2d rect, with same width and height
class CustomParticleRectangle : public ofxBox2dRect {
	
public:
	CustomParticleRectangle() {
	}
	float timeOfBirth;
	void setTimeOfBirth(float newBirthTime){
		timeOfBirth = newBirthTime;
	}	
	ofColor colour;
	void setColour(ofColor newColour){
		colour = newColour;
	}
	void draw() {
			//stolen direct from ofxBox2dRect
		if(dead) return;
		
			//wow this is a pain
		b2Shape* s = body->GetShapeList();
		const b2XForm& xf = body->GetXForm();
		b2PolygonShape* poly = (b2PolygonShape*)s;
		int count = poly->GetVertexCount();
		const b2Vec2* localVertices = poly->GetVertices();
		b2Assert(count <= b2_maxPolygonVertices);
		b2Vec2 verts[b2_maxPolygonVertices];
		for(int32 i=0; i<count; ++i) {
			verts[i] = b2Mul(xf, localVertices[i]);
		}
		
		
		ofEnableAlphaBlending();
		ofSetColor(colour.r, colour.g, colour.b, colour.a);
		ofFill();
		ofBeginShape();
		for (int32 i = 0; i <count; i++) {
			ofVertex(verts[i].x*OFX_BOX2D_SCALE, verts[i].y*OFX_BOX2D_SCALE);
		}
		ofEndShape();
		
			//		ofSetColor(0, 255, 255); //bugger outline for now
			//		ofNoFill();
			//		ofBeginShape();
			//		for (int32 i = 0; i <count; i++) {
			//			ofVertex(verts[i].x*OFX_BOX2D_SCALE, verts[i].y*OFX_BOX2D_SCALE);
			//		}
			//		ofEndShape(true);	
		ofDisableAlphaBlending();
	}
	
	
};

	// ------------------------------------------------- a simple extended box2d rect, with same width and height
class CustomParticleTriangle : public ofxBox2dPolygon {
	
public:
	CustomParticleTriangle() {
	}
	float timeOfBirth;
	void setTimeOfBirth(float newBirthTime){
		timeOfBirth = newBirthTime;
	}	
	ofColor colour;
	void setColour(ofColor newColour){
		colour = newColour;
	}
	void draw() {
			//stolen direct from ofxBox2dPolygon, with customisations
		if(body != NULL ) {
				//wow this is a pain
			b2Shape* s = body->GetShapeList();
			const b2XForm& xf = body->GetXForm();
			b2PolygonShape* poly = (b2PolygonShape*)s;
			int count = poly->GetVertexCount();
			const b2Vec2* localVertices = poly->GetVertices();
			b2Assert(vertexCount <= b2_maxPolygonVertices);
			b2Vec2 verts[b2_maxPolygonVertices];
			for(int i=0; i<vertexCount; i++) {
				verts[i] = b2Mul(xf, localVertices[i]);
			}
			
			ofEnableAlphaBlending();
			ofSetColor(colour.r, colour.g, colour.b, colour.a);
			ofFill();
			ofBeginShape();
			for(int i=0; i<count; i++) {
				ofVertex(verts[i].x*OFX_BOX2D_SCALE, verts[i].y*OFX_BOX2D_SCALE);
			}
			ofEndShape();
			
				//			ofNoFill(); //bugger outline for now
//			ofSetColor(20, 20, 20);
//			ofBeginShape();
//			for(int i=0; i<count; i++) {
//				ofVertex(verts[i].x*OFX_BOX2D_SCALE, verts[i].y*OFX_BOX2D_SCALE);
//			}
//			ofEndShape(true);
			ofDisableAlphaBlending();
		}
	}
	
	
};

struct ColourNameAndColour{
	string colourName;
	ofColor colourValue;
};

	// -------------------------------------------------

class testApp : public ofBaseApp {
	
public:
	
	testApp();
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void resized(int w, int h);
	
private:
		// from todds simple twitter example
	
	void twitterResponse(TwitterResponseArgs &e);
	void readInColours(); //reads in all the colours from the data file
	
		//newJGL
	
	void updateTopGraphicPositions();
	void updateTopGraphic();
	void drawTopGraphic();
	void loadAndParseXMLColourSettings();
	
	vector<string> tokeniseString(string inputString, string deliminators=" "); //defaults to the " " deliminator
	
	vector<string> tokeniseTwitterMessage(ofxTweet tweet); //breaks up a tweet message string around " " 's for processing
	
	void processTweet(ofxTweet tweet); //processes a tweet by splitting it up via tokeniszeTwitterMessage and then runs the relevant commands if there are
									   // more than one

	void popAndProcessATweet();
	void checkAndRunWallCommand(string const wallCommand);
	void checkAndRunColourCommand(string const wallCommand);
	
	float getStartX(); //returns a valid position on screen as an xco-ordinate
	void addHelloCircle();
	void addHelloSquare();
	void addHelloRectangle();
	void addHelloTriangle();	
	void addParticle(); //adds a custom particle to the world
	void increaseRateOfParticles();
	void decreaseRateOfParticles();
	void increaseSizeOfParticles();
	void decreaseSizeOfParticles();
	float getRandomSizeScaled();	//get a random size, scaled by particleSize
	
	void startNewScrollNewInstruction(); //start a new message scrolling accross the screen
	
	void judgeParticles();
	
		//from ofxOFbox2D example 
	
	float px, py;
	bool							bMouseForce;
	
	ofxBox2d						box2d;								//	the box2d world
	vector		<CustomParticleCircle>		customParticleCircles;		//	this is a vec of custom particles that extend a circle
	vector		<CustomParticleSquare>		customParticleSquares;		//	this is a vec of custom particles that extend a square
	vector		<CustomParticleRectangle>	customParticleRectangles;	//	this is a vec of custom particles that extend a rectangle
	vector		<CustomParticleTriangle>	customParticleTriangles;	//	this is a vec of custom particles that extend a triangle	
	
		//from font example
	ofTrueTypeFont	helloFont;	
	
		//from ofxTweenxample
	
	float length_float;
	
	ofxTween tweenlinear;
	ofxEasingLinear 	easinglinear;
	
		// OF Twitter
	ofxTwitter			twitter;
	ofxTwitterProfile	profile;
	queue <ofxTweet>	tweets;	
	
		//new JGL
	
	ofColor backgroundColour;
	ofColor foregroundColour;
	
	bool bDebugDraw;
	
	ofPoint pt_topTopGraphic;
	ofPoint pt_bottomTopGraphic;
	
	float heightOfTopGraphic;
	
	vector <ColourNameAndColour> allColourValues; //a list would be faster I think
	
	ofxXmlSettings xmlColourValues;
	
	bool bGetNewResponse;
	
	float timeOfLastNewParticle;
	
	float timeBetweenTwitterUpdateAttempts;
	
	float timeOfLastAttemptedTwitterUpdate;
	
	float timeBetweenParticles;
	
	ShapeState currentShapeState;
	
	float particleSize;
	
	bool bDoShake; //if we should shake the screen on the next frame
	int numberOfShakesRemaining;
	
	vector <string> allWallInstructions;
	int currentInstruction;
	
	string lastText;
	string lastScreenName;
	
	float timeOfLastMessageScroll;
};

