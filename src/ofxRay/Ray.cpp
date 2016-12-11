//
//  Ray.cpp
//  ofxRay
//
//  Elliot Woods (C) 2012, MIT license
//	http://www.kimchiandchips.com
//

#include "ofxRay/Ray.h"

ostream& operator<<(ostream & os, const ofxRay::Ray & ray) {
	os << ray.s;
	os << "; ";
	os << ray.t;
	os << "; ";
	os << (const ofxRay::Base &)ray;
	return os;
}
istream& operator>>(istream & is, ofxRay::Ray & ray) {
	is >> ray.s;
	is.ignore(2);
	is >> ray.t;
	is.ignore(2);
	is >> (ofxRay::Base &)ray;
	return is;
}

namespace ofxRay {
	Ray::Ray() {
		this->defined = false;
		this->width = 2.0f;
	}

	Ray::Ray(glm::vec3 s, glm::vec3 t, bool infinite) {
		this->defined = true;
		this->s = s;
		this->t = t;
		this->infinite = infinite;
		this->width = 2.0f;
	}

	Ray::Ray(glm::vec3 s, glm::vec3 t, ofColor color, bool infinite) : ofxRay::Base(color) {
		this->defined = true;
		this->s = s;
		this->t = t;
		this->infinite = infinite;
		this->width = 2.0f;
	}

	void Ray::draw() const {
		if (!this->defined)
			return;
	
		ofPushStyle();
		ofEnableSmoothing();
		ofSetColor(color);
	
		ofPushMatrix();
		ofTranslate(s);
		ofDrawSphere(0.01);
		ofPopMatrix();
	
		if (infinite) {
			//'infinite' line
		
			//inner line
			if (width > 2.0f){
				ofPushStyle();
				ofSetLineWidth(1.0f);
				ofSetColor(255,255,255);
				ofDrawLine(s-100*t, s+100*t);
				ofPopStyle();
			}
		
		
			ofSetLineWidth(width==0.0f ? 1.0f : width);
			ofDrawLine(s-1000*t, s+1000*t);
		}
	
		//arrow
		ofDrawArrow(s, s+t);
	
		ofPopStyle();
	}

	void Ray::randomiseVectors(float amplitude) {
		infinite = true;
		s = glm::vec3(ofRandomf(), ofRandomf(), ofRandomf()) * amplitude;
		t = glm::vec3(ofRandomf(), ofRandomf(), ofRandomf()) * amplitude;
	}

	Ray& Ray::operator*=(const glm::mat4 &m) {
		glm::vec3 sOld = s;
		s = (glm::vec4(s, 1.0f) * m).xyz;
		t = (glm::vec4(t+sOld, 1.0f) * m).xyz - s;
		return *this;
	}

	Ray Ray::operator*(const glm::mat4 &m) const {
		Ray ray = *this;
		return ray *= m;
	}
	
	// from http://paulbourke.net/geometry/lineline3d/lineline.c
	//	int LineLineIntersect(
	//	XYZ p1,XYZ p2,XYZ p3,XYZ p4,XYZ *pa,XYZ *pb,
	//	double *mua, double *mub)
	Ray Ray::intersect(const Ray &other) const {
		Ray intersectRay;

		const glm::vec3 p1(s), p2(s+t), p3(other.s), p4(other.s+other.t);
		const float EPS(1.0E-15);
	
		glm::vec3 p13,p43,p21;
		float d1343,d4321,d1321,d4343,d2121;
		float numer,denom;
	
		p13.x = p1.x - p3.x;
		p13.y = p1.y - p3.y;
		p13.z = p1.z - p3.z;
		p43.x = p4.x - p3.x;
		p43.y = p4.y - p3.y;
		p43.z = p4.z - p3.z;
		if (fabs(p43.x) < EPS && fabs(p43.y) < EPS && fabs(p43.z) < EPS)
			return(Ray());
		p21.x = p2.x - p1.x;
		p21.y = p2.y - p1.y;
		p21.z = p2.z - p1.z;
		if (fabs(p21.x) < EPS && fabs(p21.y) < EPS && fabs(p21.z) < EPS)
			return(Ray());
	
		d1343 = p13.x * p43.x + p13.y * p43.y + p13.z * p43.z;
		d4321 = p43.x * p21.x + p43.y * p21.y + p43.z * p21.z;
		d1321 = p13.x * p21.x + p13.y * p21.y + p13.z * p21.z;
		d4343 = p43.x * p43.x + p43.y * p43.y + p43.z * p43.z;
		d2121 = p21.x * p21.x + p21.y * p21.y + p21.z * p21.z;
	
		denom = d2121 * d4343 - d4321 * d4321;
		if (fabs(denom) < EPS)
			return(Ray());
		numer = d1343 * d4321 - d1321 * d4343;
		
		float ma = numer / denom;
		float mb = (d1343 + d4321 * (ma)) / d4343;
	
		glm::vec3 s;
		glm::vec3 t;
	
		s.x = p1.x + ma * p21.x;
		s.y = p1.y + ma * p21.y;
		s.z = p1.z + ma * p21.z;
		t.x = p3.x + mb * p43.x;
		t.y = p3.y + mb * p43.y;
		t.z = p3.z + mb * p43.z;
		t = t - s;
	
		return Ray(s, t, false);
	}

	// using http://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html
	float Ray::distanceTo(const glm::vec3& point) const {
		return glm::length(glm::cross((point - s), point - (s+t))) / glm::length(t);
	}
	// untested but makes sense
	glm::vec3 Ray::closestPointOnRayTo(const glm::vec3& point) const {
		return s + glm::dot(t * (point - s), t) / glm::length2(t);
	}

	glm::vec3 Ray::getMidpoint() const {
		return s + t*0.5;
	}

	float Ray::getLength() const {
		return glm::length(t);
	}
	
	float Ray::getLengthSquared() const {
		return glm::length2(t);
	}

	void Ray::setStart(const glm::vec3 & start) {
		this->s = start;
	}

	void Ray::setEnd(const glm::vec3 & end) {
		this->setTranmissionVector(end - this->s);
	}

	void Ray::setTranmissionVector(const glm::vec3 & tranmissionVector) {
		this->t = tranmissionVector;
		this->defined = true;
	}

	const glm::vec3 & Ray::getStart() const {
		return this->s;
	}

	glm::vec3 Ray::getEnd() const {
		return this->t + this->s;
	}

	const glm::vec3 & Ray::getTransmissionVector() const {
		return this->t;
	}

	Ray Ray::operator*(float other) const {
		return Ray(s, t * other, color, infinite);
	}

	glm::vec3 Ray::operator()(float other) const {
		return s + other * t;
	}
}
