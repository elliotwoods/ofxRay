#pragma once
//
//  ofRay.h
//  ofxRay
//
//  (C) 2012 http://www.kimchiandchips.com
//

#include "ofMain.h"
#include "ofGeometric.h"

class ofRay : ofGeometric {
public:
	ofRay();
	ofRay(ofVec3f s, ofVec3f t, bool infinite=true);
	
	void draw(float width=0.0f) const;
	
	void randomise(float amplitude=1.0f);
	void randomiseVectors(float amplitude=1.0f);
	
	ofRay intersect(const ofRay &other) const;
	void transform(ofMatrix4x4 m);
	
	const ofVec3f& getS() const;
	const ofVec3f& getT() const;
	ofVec3f	getMidpoint() const;
	float	getLength() const;
	bool	getInfinite() const;
	
protected:
	ofVec3f& getS();
	ofVec3f& getT();
	
	ofVec3f		s, t;
	
	bool	infinite;
	bool	defined;
};