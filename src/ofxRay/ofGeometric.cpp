//
//  ofGeometric.cpp
//  ofxRay
//
//  Elliot Woods (C) 2012, MIT license
//	http://www.kimchiandchips.com
//
#include "ofGeometric.h"

ofGeometric::ofGeometric() {
	randomiseColor();
}

ofGeometric::ofGeometric(ofColor color) {
	this->color = color;
}

void ofGeometric::randomiseColor() {
	color = ofColor(150.0f + 100.0f * ofRandomuf(), 150.0f + 100.0f * ofRandomuf(), 150.0f + 100.0f * ofRandomuf(), 255);
}

void ofGeometric::randomise(float amplitude) {
	randomiseVectors(amplitude);
	randomiseColor();
}