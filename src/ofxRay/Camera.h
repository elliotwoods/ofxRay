#pragma once
//
//  Camera.h
//  ofxRay
//
//  Elliot Woods (C) 2012, MIT license
//	http://www.kimchiandchips.com
//
#include "Ray.h"
#include "Projector.h"

namespace ofxRay {
	class Camera;
}
ostream& operator<<(ostream &, const ofxRay::Camera &);
istream& operator>>(istream & is, ofxRay::Camera &);

namespace ofxRay {
	class Camera : public Projector {
	public:
		Camera();
		Camera(const glm::vec2 & focalLength, const glm::vec2 & center, float w, float h);
		
		friend ostream & (::operator<<) (ostream &, const Camera &);
		friend istream & (::operator>>) (istream &, Camera &);
	};
}
