#pragma once
//
//  ofPlane.h
//  ofxRay
//
//  (C) 2012 http://www.kimchiandchips.com
//

#include "ofMain.h"
#include "ofGeometric.h"
#include "ofRay.h"

class ofPlane : ofGeometric {
public:
	ofPlane();
	ofPlane(float a, float b, float c, float d);
	ofPlane(ofVec3f center, ofVec3f normal);
	ofPlane(ofVec3f center, ofVec3f normal, ofVec3f up, ofVec2f scale);
	
	void draw() const;
	
	void randomise(float amplitude=1.0f);
	void randomiseVectors(float amplitude=1.0f);
	
	bool intersect(const ofRay &ray, ofVec3f &position) const;
	
	const ofVec3f& getC() const;
	const ofVec3f& getN() const;
	bool getInfinite() const;
	
	void setC(const ofVec3f& c);
	void setN(const ofVec3f& c);
	void setInfinite(const bool b);
	
protected:
	void makeGrid();
	
	ofVec3f c;
	ofVec3f n;
	
	bool infinite;
	ofVec3f up;
	ofVec2f scale;
	
	static ofMesh* viewGrid;
	static ofMesh* viewPlane;
};