//
//  Camera.cpp
//  ofxRay
//
//  Elliot Woods (C) 2012, MIT license
//	http://www.kimchiandchips.com
//

#include "ofxRay/Camera.h"

ostream& operator<<(ostream & os, const ofxRay::Camera & camera) {
	os << camera.distortion.size();
	os << ": ";
	for (auto parameter : camera.distortion) {
		os << parameter;
		os << ", ";
	}
	os << ";\n";

	os << (const ofxRay::Projector &) camera;

	return os;
}

istream& operator>>(istream & is, ofxRay::Camera & camera) {

	size_t distortionSize;

	is >> distortionSize;
	is.ignore(2);

	camera.distortion.resize(distortionSize);

	for (int i = 0; i < distortionSize; i++) {
		float parameter;
		
		is >> parameter;
		is.ignore(2);

		camera.distortion[i] = parameter;
	}
	is.ignore(2);

	is >> (ofxRay::Projector &)camera;

	return is;
}

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
		const int distortionLength = this->distortion.size();
		
		if (this->distortion.size() < 2) {
			return xy;
		}
		
		float r = xy.length();
		float rr = r*r;
		
		float rad_coeff = 1.0f + distortion[0] * rr + distortion[1] * rr * rr;
		if (distortionLength > 4) {
			rad_coeff += distortion[4] * rr * rr * rr;
		}
		
		float xn = xy.x * rad_coeff;
		float yn = xy.y * rad_coeff;
		
		xn += 2 * distortion[2] * xn * yn + distortion[3] * (rr + 2 * xn * xn);
		yn += distortion[2] * (rr + 2 * yn * yn) + 2 * distortion[3] * xn * yn;
		
		return ofVec2f(xn, yn);
	}
}
