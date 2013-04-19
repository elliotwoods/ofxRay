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

	Camera::Camera(const ofVec2f & focalLength, const ofVec2f & center, float w, float h)
	:Projector(w,h){
		ofMatrix4x4 projection;
		
		projection(0,0) = focalLength.x / w * 2.0f;
		projection(1,1) = focalLength.y / h * 2.0f;
		
		projection(2,0) = (center.x - w / 2) / w;
		projection(2,1) = (center.y - h / 2) / h;
		projection(2,2) = -1.0f;
		projection(2,3) = -1.0f;
		
		projection(3,3) = 0.0f;

		setProjection(projection);
	}

	ofVec2f Camera::undistortCoordinate(const ofVec2f & xy) const {
		float theta = atan2(xy.y, xy.x);
		float r = xy.length();
		
		r = r + 
			distortion.x * r * r + 
			distortion.y * r * r * r + 
			distortion.z * r * r * r * r + 
			distortion.w * r * r * r * r * r;
		
		return ofVec2f(r * cos(theta), r * sin(theta));
	}
}
