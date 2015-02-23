#pragma once
//
//  Plane.h
//  ofxRay
//
//  Elliot Woods (C) 2012, MIT license
//	http://www.kimchiandchips.com
//

#include "Base.h"
#include "Ray.h"

namespace ofxRay {
	class Plane;
}
ostream& operator<<(ostream & os, const ofxRay::Plane &);
istream& operator>>(istream & is, ofxRay::Plane &);

namespace ofxRay {
	class Plane : public Base {
	public:
		Plane();
		Plane(float a, float b, float c, float d);
		Plane(ofVec3f center, ofVec3f normal);
		Plane(ofVec3f center, ofVec3f normal, ofVec3f up, ofVec2f scale);
	
		//-
		//ofGeometric
		void draw() const;
		void randomiseVectors(float amplitude=1.0f);
		//-
	
		bool intersect(const Ray &ray) const;
		bool intersect(const Ray &ray, ofVec3f &position) const;
	
		const ofVec3f& getCenter() const;
		const ofVec3f& getNormal() const;
		const ofVec3f& getUp() const;
		const ofVec2f& getScale() const;
		ofVec4f getABCD() const; ///< http://mathworld.wolfram.com/Point-PlaneDistance.html
		bool getInfinite() const;
	
		void setCenter(const ofVec3f& center);
		void setNormal(const ofVec3f& normal);
		void setUp(const ofVec3f& up);
		void setScale(const ofVec2f& scale);
		void setInfinite(const bool b);
		void setFrom(ofPlanePrimitive &);
	
		///fills rays{0..3} with rays starting from corners and going to target
		void getCornerRaysTo(const ofVec3f &target, Ray* rays) const;
		///fills rays{0..3} with rays starting from source and going to corners
		void getCornerRaysFrom(const ofVec3f &source, Ray* rays) const;
	
		float getDistanceTo(const ofVec3f &) const;
		bool fitToPoints(const vector<ofVec3f> &, float & residual);

		ofVec3f getRight() const;
	
		Ray getUpRay() const;
		Ray getRightRay() const;
	
		friend ostream & (::operator<<) (ostream &, const Plane &);
		friend istream & (::operator>>) (istream &, Plane &);

	protected:
		void makeGrid();
	
		ofVec3f center;
		ofVec3f normal;
	
		bool infinite;
		ofVec3f up;
		ofVec2f scale;
	
		static ofMesh* viewGrid;
		static ofMesh* viewPlane;
	};
}