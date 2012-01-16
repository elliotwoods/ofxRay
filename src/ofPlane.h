#pragma once
//
//  ofPlane.h
//  ofxRay
//
//  Elliot Woods (C) 2012, MIT license
//	http://www.kimchiandchips.com
//

#include "ofMain.h"
#include "ofGeometric.h"
#include "ofRay.h"

class ofPlane : public ofGeometric {
public:
	ofPlane();
	ofPlane(float a, float b, float c, float d);
	ofPlane(ofVec3f center, ofVec3f normal);
	ofPlane(ofVec3f center, ofVec3f normal, ofVec3f up, ofVec2f scale);
	
	//-
	//ofGeometric
	void draw() const;
	void randomiseVectors(float amplitude=1.0f);
	//-
	
	bool intersect(const ofRay &ray) const;
	bool intersect(const ofRay &ray, ofVec3f &position) const;
	
	const ofVec3f& getC() const;
	const ofVec3f& getN() const;
	bool getInfinite() const;
	
	void setC(const ofVec3f& c);
	void setN(const ofVec3f& c);
	void setInfinite(const bool b);
	
	///fills rays{0..3} with rays starting from corners and going to target
	void getCornerRaysTo(const ofVec3f &target, ofRay* rays) const;
	///fills rays{0..3} with rays starting from source and going to corners
	void getCornerRaysFrom(const ofVec3f &source, ofRay* rays) const;
	
	ofVec3f getUp() const;
	ofVec3f getRight() const;
	
	ofRay getUpRay() const;
	ofRay getRightRay() const;
	
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