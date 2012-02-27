#pragma once
//
//  ofRay.h
//  ofxRay
//
//  Elliot Woods (C) 2012, MIT license
//	http://www.kimchiandchips.com
//

#include "ofMain.h"
#include "ofGeometric.h"

class ofRay : public ofGeometric {
public:
	ofRay();
	ofRay(ofVec3f s, ofVec3f t, bool infinite=true);
	ofRay(ofVec3f s, ofVec3f t, ofColor color, bool infinite=true);
	
	//-
	//ofGeometric
	void draw() const;
	void randomiseVectors(float amplitude = 1.0f);
	//-
	
	ofRay intersect(const ofRay &other) const;
	
	float distanceTo(const ofVec3f& point) const;
	ofVec3f closestPointOnRayTo(const ofVec3f& point) const;
	
	ofRay& operator*=(const ofMatrix4x4 &m);
	ofRay operator*(const ofMatrix4x4 &m) const;
	
	ofVec3f	getMidpoint() const;
	float	getLength() const;
	
	ofRay operator*(float other) const;
	ofVec3f operator()(float other) const;
	
	//vectors
	ofVec3f	s, t;
	bool	infinite;
	bool	defined;
	
	//draw styles
	float	width;
};