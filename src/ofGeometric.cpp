//
//  ofGeometric.cpp
//  ofxRay
//
//  (C) 2012 http://www.kimchiandchips.com
//
#include "ofGeometric.h"

void ofGeometric::randomiseColor() {
	color = ofColor(150.0f + 100.0f * ofRandomuf(), 150.0f + 100.0f * ofRandomuf(), 150.0f + 100.0f * ofRandomuf(), 255);
}
