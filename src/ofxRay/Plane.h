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
		Plane(glm::vec3 center, glm::vec3 normal);
		Plane(glm::vec3 center, glm::vec3 normal, glm::vec3 up, glm::vec2 scale);
	
		//-
		//ofGeometric
		void draw() const;
		void randomiseVectors(float amplitude=1.0f);
		//-
	
		bool intersect(const Ray &ray) const;
		bool intersect(const Ray &ray, glm::vec3 &position) const;
	
		const glm::vec3& getCenter() const;
		const glm::vec3& getNormal() const;
		const glm::vec3& getUp() const;
		const glm::vec2& getScale() const;
		glm::vec4 getABCD() const; ///< http://mathworld.wolfram.com/Point-PlaneDistance.html
		bool getInfinite() const;
	
		void setCenter(const glm::vec3& center);
		void setNormal(const glm::vec3& normal);
		void setUp(const glm::vec3& up);
		void setScale(const glm::vec2& scale);
		void setInfinite(const bool b);
		void setFrom(ofPlanePrimitive &);
	
		///fills rays{0..3} with rays starting from corners and going to target
		void getCornerRaysTo(const glm::vec3 &target, Ray* rays) const;
		///fills rays{0..3} with rays starting from source and going to corners
		void getCornerRaysFrom(const glm::vec3 &source, Ray* rays) const;
	
		float getDistanceTo(const glm::vec3 &) const;
		bool fitToPoints(const vector<glm::vec3> &, float & residual);

		glm::vec3 getRight() const;
	
		Ray getUpRay() const;
		Ray getRightRay() const;
	
		friend ostream & (::operator<<) (ostream &, const Plane &);
		friend istream & (::operator>>) (istream &, Plane &);

	protected:
		void makeGrid();
	
		glm::vec3 center;
		glm::vec3 normal;
	
		bool infinite;
		glm::vec3 up;
		glm::vec2 scale;
	
		static ofMesh* viewGrid;
		static ofMesh* viewPlane;
	};
}