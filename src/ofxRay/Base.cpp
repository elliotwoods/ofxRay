//
//  Base.cpp
//  ofxRay
//
//  Elliot Woods (C) 2012, MIT license
//	http://www.kimchiandchips.com
//
#include "ofxRay/Base.h"

namespace ofxRay {
	Base::Base() {
		randomiseColor();
	}

	Base::Base(ofColor color) {
		this->color = color;
	}

	void Base::randomiseColor() {
		color = ofColor(150.0f + 100.0f * ofRandomuf(), 150.0f + 100.0f * ofRandomuf(), 150.0f + 100.0f * ofRandomuf(), 255);
	}

	void Base::randomise(float amplitude) {
		randomiseVectors(amplitude);
		randomiseColor();
	}
}