#pragma once
//
//  Ray.h
//  ofxRay
//
//  Elliot Woods (C) 2012, MIT license
//	http://www.kimchiandchips.com
//

#include "Base.h"

namespace ofxRay {
	class Ray;
}
ostream& operator<<(ostream & os, const ofxRay::Ray &);
istream& operator>>(istream & is, ofxRay::Ray &);

namespace ofxRay {
	class Ray : public Base {
	public:
		Ray();
		Ray(glm::vec3 s, glm::vec3 t, bool infinite=true);
		Ray(glm::vec3 s, glm::vec3 t, ofColor color, bool infinite=true);
	
		//-
		//ofGeometric
		void draw() const override;
		void randomiseVectors(float amplitude = 1.0f) override;
		//-
		
		///NOTE : does not account for non-infinite lines
		Ray intersect(const Ray &other) const;
	
		float distanceTo(const glm::vec3& point) const;
		glm::vec3 closestPointOnRayTo(const glm::vec3& point) const;
	
		Ray& operator*=(const glm::mat4 &m);
		Ray operator*(const glm::mat4 &m) const;
	
		glm::vec3	getMidpoint() const;
		float	getLength() const;
		float	getLengthSquared() const;
	
		void setStart(const glm::vec3 &);
		void setEnd(const glm::vec3 &);
		void setTransmissionVector(const glm::vec3 &);

		const glm::vec3 & getStart() const;
		glm::vec3 getEnd() const;
		const glm::vec3 & getTransmissionVector() const;

		Ray operator*(float other) const;
		glm::vec3 operator()(float other) const;

		friend ostream& (::operator<<) (ostream&, const Ray &);
		friend istream& (::operator>>) (istream&, Ray &);
	
		//vectors
		glm::vec3	s, t;
		bool	infinite;
		bool	defined;
	
		//draw styles
		float	width;
	};
}
