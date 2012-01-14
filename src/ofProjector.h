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
class ofProjector : public ofGeometric {
public:
	ofProjector();
	ofProjector(int width, int height);
	ofProjector(const ofMatrix4x4& viewProjection, int width, int height);
	
	//-
	//ofGeometric
	void draw() const;
	void randomiseVectors(float amplitude=1.0f);
	//-
	
	///Choose a random pose, fov, lens properties for the camera with position scale parameter
	void randomisePose(float scale=1.f);
	
	///Generate a ray for the given pixel coordinates x,y within the projector's image
	ofRay castPixel(int x, int y) const;
	///Generate a ray for the given normalised coordinate x,y where {-1.0f<x,y<1.0f}
	ofRay castCoordinate(float x, float y) const;
	
	///
	
	ofMatrix4x4 getViewMatrix() const;
	ofMatrix4x4 getProjectionMatrix() const;
	int width;
	int height;
	float throwRatio;
	float aspectRatio;
	ofVec2f lensOffset;
	ofVec3f position;
	ofQuaternion rotation;
	
protected:
	void makeBox();
	
	static ofMesh* drawBox;
};