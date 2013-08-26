#pragma once
//
//  Ray.h
//  ofxRay
//
//  Elliot Woods (C) 2012, MIT license
//	http://www.kimchiandchips.com
//

#include "ofMain.h"
#include "ofxRay/Base.h"

namespace ofxRay {
	class Ray : public Base {
	public:
		Ray();
		Ray(ofVec3f s, ofVec3f t, bool infinite=true);
		Ray(ofVec3f s, ofVec3f t, ofColor color, bool infinite=true);
	
		//-
		//ofGeometric
		void draw() const;
		void randomiseVectors(float amplitude = 1.0f);
		//-

		Ray intersect(const Ray &other) const;
	
		float distanceTo(const ofVec3f& point) const;
		ofVec3f closestPointOnRayTo(const ofVec3f& point) const;
	
		Ray& operator*=(const ofMatrix4x4 &m);
		Ray operator*(const ofMatrix4x4 &m) const;
	
		ofVec3f	getMidpoint() const;
		float	getLength() const;
		float	getLengthSquared() const;
	
		Ray operator*(float other) const;
		ofVec3f operator()(float other) const;
	
		//vectors
		ofVec3f	s, t;
		bool	infinite;
		bool	defined;
	
		//draw styles
		float	width;
	};
}