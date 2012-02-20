#pragma once
//
//  ofProjector.h
//  ofxRay
//
//  Elliot Woods (C) 2012, MIT license
//	http://www.kimchiandchips.com
//
#include "ofRay.h"

///A class to generate rays given parameters for a projector or camera
///NB: ofProjector inherits from ofNode, and therefore ofNode stores its view matrix
class ofProjector : public ofGeometric, public ofNode {
public:
	ofProjector(int width=1024, int height=768);
	ofProjector(float throwRatio, const ofVec2f& lensOffset, int width, int height);
	ofProjector(const ofMatrix4x4& projection, int width, int height);

	////
	//ofGeometric
	////
	//
	void draw() const;
	void randomiseVectors(float amplitude=1.0f);
	////
	
	///Choose a random pose, for the projector with scale factor
	void randomisePose(float scale=1.0f);
	
	///Generate a ray for the given pixel coordinates x,y within the projector's image
	ofRay castPixel(int x, int y) const;
	ofRay castPixel(const ofVec2f& xy) const;
	void castPixels(const vector<ofVec2f>& xy, vector<ofRay>& rays) const;

	///Generate a ray for the given normalised coordinate x,y where {-1.0f<x,y<1.0f}
	ofRay castCoordinate(const ofVec2f& xy) const;
	void castCoordinates(const vector<ofVec2f>& xy, vector<ofRay>& rays) const;

	void setProjection(float throwRatio, const ofVec2f& lensOffset);
	void setProjection(const ofMatrix4x4& projection);

	ofMatrix4x4 getViewMatrix() const;
	ofMatrix4x4 getProjectionMatrix() const;
	int width;
	int height;

protected:
	ofMatrix4x4 projection;
	static ofMesh* drawBox;

private:
	static void makeBox();
};