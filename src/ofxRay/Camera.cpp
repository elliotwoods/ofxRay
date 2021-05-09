//
//  Camera.cpp
//  ofxRay
//
//  Elliot Woods (C) 2012, MIT license
//	http://www.kimchiandchips.com
//

#include "ofxRay/Camera.h"

ostream& operator<<(ostream & os, const ofxRay::Camera & camera) {
	os << (const ofxRay::Projector &) camera;
	return os;
}

istream& operator>>(istream & is, ofxRay::Camera & camera) {
	is >> (ofxRay::Projector &)camera;
	return is;
}

namespace ofxRay {
	Camera::Camera() :
		Projector(1.0f, glm::vec2(0.0f, 0.0f), 1024, 768) {

	}

	Camera::Camera(const glm::vec2 & focalLength, const glm::vec2 & center, float w, float h)
		: Projector(w, h) {
		ofMatrix4x4 projection;

		projection(0, 0) = focalLength.x / w * 2.0f;
		projection(1, 1) = focalLength.y / h * 2.0f;

		projection(2, 0) = (center.x - w / 2) / w;
		projection(2, 1) = (center.y - h / 2) / h;
		projection(2, 2) = -1.0f;
		projection(2, 3) = -1.0f;

		projection(3, 3) = 0.0f;

		setProjection(projection);
	}
}
