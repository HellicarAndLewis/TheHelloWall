
#pragma once
#include "ofMain.h"
#include "Box2D.h"

class ofxBox2dContactListener : public b2ContactListener {
	
public:
	
	ofxBox2dContactListener() {
	}

	void Add(const b2ContactPoint* point) {
		
		b2Vec2 p = point->position;
		p *= OFX_BOX2D_SCALE;
		
		
		contactAdd(ofPoint(p.x, p.y));
		
		//b2Body * body_1 = point->shape1->GetBody();
		//MonsterData * theData = (MonsterData*)body_1->GetUserData();
		
	}
	
	void Remove(const b2ContactPoint* point) {
	}
	virtual void contactAdd(ofPoint p) {
	}
	virtual void contactRemove(ofPoint p) {
	}
};



