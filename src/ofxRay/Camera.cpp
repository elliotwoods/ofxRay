//
//  Camera.cpp
//  ofxRay
//
//  Elliot Woods (C) 2012, MIT license
//	http://www.kimchiandchips.com
//

#include "ofxRay/Camera.h"

namespace ofxRay {
	Camera::Camera() :
	Projector(1.0f, ofVec2f(0.0f, 0.0f), 1024, 768) {
		
	}
}