#pragma once
//
//  ofxRay.h
//  ofxRay
//
//  (C) 2012 http://www.kimchiandchips.com
//

#include "ofMain.h"

class ofxRay {
public:
	ofxRay(); ///< random initialisation
	ofxRay(float f);
	
	void draw(float f=0.0f);
	
	void randomise();
	
	ofxRay intersect(Ray &other);
	void transform(ofMatrix4x4 m);
	
	ofVec3f	getMidpoint() const;
	float	getLength() const;
	
	ofVec3f		s, t;
	ofColor		color;
	
	bool	infinite;
};