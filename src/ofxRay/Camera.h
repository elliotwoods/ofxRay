#pragma once
//
//  Camera.h
//  ofxRay
//
//  Elliot Woods (C) 2012, MIT license
//	http://www.kimchiandchips.com
//
#include "ofxRay/Ray.h"
#include "ofxRay/Projector.h"

namespace ofxRay {
	class Camera : public Projector {
	public:
		Camera();
		
		ofVec2f undistortCoordinate(const ofVec2f & xy) const;
		ofVec4f distortion;
	};
}
