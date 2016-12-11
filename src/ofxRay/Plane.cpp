//
//  Plane.cpp
//  ofxRay
//
//  Elliot Woods (C) 2012, MIT license
//	http://www.kimchiandchips.com
//

#include "Plane.h"

ostream& operator<<(ostream & os, const ofxRay::Plane & plane) {
	os << plane.center;
	os << "; ";
	os << plane.normal;
	os << "; ";
	os << plane.infinite;
	os << "; ";
	os << plane.up;
	os << "; ";
	os << plane.scale;
	os << "; ";
	os << (const ofxRay::Base &)plane;
	return os;
}

istream& operator>>(istream & is, ofxRay::Plane & plane) {
	is >> plane.center;
	is.ignore(2);
	is >> plane.normal;
	is.ignore(2);
	is >> plane.infinite;
	is.ignore(2);
	is >> plane.up;
	is.ignore(2);
	is >> plane.scale;
	is.ignore(2);
	is >> (ofxRay::Base &)plane;
	return is;
}

namespace ofxRay {

	ofMesh* Plane::viewGrid = 0;
	ofMesh* Plane::viewPlane = 0;

	Plane::Plane() {
		infinite = true;
		makeGrid();
		randomiseVectors();
	}

	Plane::Plane(float a, float b, float c, float d) {
		glm::vec3 direction(a,b,c);
		this->setCenter(direction * d);
		this->setNormal(glm::normalize(direction));
		this->setScale(glm::vec2(1.0f, 1.0f));
		infinite = true;
		makeGrid();
	}

	Plane::Plane(glm::vec3 center, glm::vec3 normal) {
		setCenter(center);
		setNormal(normal);
		infinite = true;
		makeGrid();
	}

	Plane::Plane(glm::vec3 center, glm::vec3 normal, glm::vec3 up, glm::vec2 scale) {
		setCenter(center);
		setNormal(normal);
		setUp(up);
		setScale(scale);
		infinite = false;	
		makeGrid();
	}

	void Plane::draw() const {
		ofPushStyle();
		{
			ofSetColor(color);
			ofPushMatrix();
			{
				ofTranslate(center);

				/*
				{
					glm::quat q, r;
					glm::angleAxis(
						q.makeRotate(glm::vec3(0, 0, 1), normal);
					r.makeRotate(glm::vec3(0, 1, 0) * q, up);
					float angle, x, y, z;
					(q * r).getRotate(angle, x, y, z);
					ofRotateRad(angle, x, y, z);
				}*/
				ofLogWarning("ofxRay") << "HACK : since glm update we don't draw planes correctly. needs a fix";
				

				ofScale(this->scale.x, this->scale.y, 1.0f);

				viewGrid->draw();

				if (this->infinite) {
					ofPushStyle();
					ofEnableAlphaBlending();
					ofSetColor(color.r, color.g, color.b, 50);
					viewPlane->draw();
					ofPopStyle();
				}
			}
			ofPopMatrix();

			ofDrawArrow(getCenter(), getCenter() + getNormal());
			ofDrawArrow(getCenter(), getCenter() + getUp());
			ofDrawArrow(getCenter(), getCenter() + getRight());
		}
		ofPopStyle();
	}
	
	void Plane::randomiseVectors(float amplitude) {
		center = glm::vec3(ofRandom(-1.0f, 1.0f),
					ofRandom(-1.0f, 1.0f),
					ofRandom(-1.0f, 1.0f)) * amplitude;
		normal = glm::vec3(ofRandom(-1.0f, 1.0f),
					ofRandom(-1.0f, 1.0f),
					ofRandom(-1.0f, 1.0f));
		up = glm::vec3(ofRandom(-1.0f, 1.0f),
					 ofRandom(-1.0f, 1.0f),
					 ofRandom(-1.0f, 1.0f));
		scale = glm::vec2(ofRandom(-1.0f, 1.0f),
						ofRandom(-1.0f, 1.0f)) * amplitude;
		normal = glm::normalize(normal);
		infinite = true;
	}

	bool Plane::intersect(const Ray &ray) const {
		glm::vec3 position;
		return intersect(ray, position);
	}

	bool Plane::intersect(const Ray &ray, glm::vec3 &position) const {
		//check if line is parallel to plane
		//if so, die young
		if (glm::dot(ray.t, normal) == 0.0f)
			return false;
	
		float u = glm::dot(normal, center - ray.s) / glm::dot(normal, ray.t);	
		position = ray.s + u * ray.t;
	
	
		if (!ray.infinite) {
			//check within line segment
			if (u < 0.0f || u > 1.0f)
				return false;
		}
	
		if (!this->infinite) {
			//check within plane segment
		
			//define up as ray, find distance between this ray and ray<> plane intersection point to get x position in plane
			float x = getUpRay().distanceTo(position);
			float y = sqrt(glm::length2(center - position) - x*x);
		
			//if length along this ray < height and distance of point to ray < width then we're in the plane
			if (abs(x) <= scale.x && abs(y) <= scale.y)
				return true;
			else
				return false;
		
		} else
			return true;
	}

	const glm::vec3& Plane::getCenter() const{
		return this->center;
	}

	const glm::vec3& Plane::getNormal() const {
		return this->normal;
	}

	const glm::vec3& Plane::getUp() const {
		return this->up;
	}

	const glm::vec2& Plane::getScale() const{
		return this->scale;
	}

	glm::vec4 Plane::getABCD() const {
		glm::vec4 ABCD;
		ABCD.x = this->normal.x;
		ABCD.y = this->normal.y;
		ABCD.z = this->normal.z;
		ABCD.w = - glm::dot(this->normal, this->center);
		return ABCD;
	}

	bool Plane::getInfinite() const {
		return this->infinite;
	}

	void Plane::setCenter(const glm::vec3& center) {
		this->center = center;
	}

	void Plane::setNormal(const glm::vec3& normal) {
		this->normal = glm::normalize(normal);
		setUp(this->up); //reset up (to ensure orthogonal to normal)
	}

	void Plane::setUp(const glm::vec3& up) {
		this->up = up;
		//remove component of up which is parallel to the normal
		this->up = this->up - this->normal * glm::dot(this->up, this->normal);
		this->up = glm::normalize(this->up);
	}

	void Plane::setScale(const glm::vec2& scale) {
		this->scale = scale;
	}

	void Plane::setInfinite(const bool b) {
		infinite = b;
	}
	
	void Plane::setFrom(ofPlanePrimitive & planePrimitive) {
		const glm::mat4 transform = planePrimitive.getLocalTransformMatrix();
		glm::vec3 translation, scale, skew;
		glm::quat rotation;
		glm::vec4 perspective;
		//glm::decompose(transform, scale, rotation, translation, skew, perspective);
		ofLogError("ofxRay") << "HACK : ofxRay::Plane does not support setFrom since upgrade to glm";

		this->setScale(scale.xy * glm::vec2(planePrimitive.getWidth(), planePrimitive.getHeight()) / 2.0f);
		this->setCenter(translation);
		
		this->setNormal(glm::vec3(0.0f, 0.0f, 1.0f) * rotation);
		this->setUp(glm::vec3(0.0f, 1.0f, 0.0f) * rotation);

		this->setInfinite(false);
	}

	void Plane::getCornerRaysTo(const glm::vec3 &target, Ray* rays) const {
		glm::vec3 up = this->up * scale.y;
		glm::vec3 right = getRight() * scale.x;
		glm::vec3 corner;
	
		corner = center - up - right;
		*rays++ = Ray(corner, target - corner, color, false);

		corner = center - up + right;
		*rays++ = Ray(corner, target - corner, color, false);
	
		corner = center + up + right;
		*rays++ = Ray(corner, target - corner, color, false);
	
		corner = center + up - right;
		*rays++ = Ray(corner, target - corner, color, false);
	}

	void Plane::getCornerRaysFrom(const glm::vec3 &source, Ray* rays) const {
		glm::vec3 up = this->up * scale.y;
		glm::vec3 right = getRight() * scale.x;
		glm::vec3 corner;
	
		corner = center - up - right;
		*rays++ = Ray(source, corner - source, color, false);
	
		corner = center - up + right;
		*rays++ = Ray(source, corner - source, color, false);
	
		corner = center + up + right;
		*rays++ = Ray(source, corner - source, color, false);
	
		corner = center + up - right;
		*rays++ = Ray(source, corner - source, color, false);
	}

	//http://mathinsight.org/distance_point_plane
	float Plane::getDistanceTo(const glm::vec3 & point) const {
		const auto ABCD = this->getABCD();
		const auto ABC = glm::vec3(ABCD.x, ABCD.y, ABCD.z);
		const auto numerator = abs(glm::dot(normal, point) + ABCD.w);
		const auto denominator = glm::length(ABC);
		return numerator / denominator;
	}

	void Plane::makeGrid() {
		if (viewGrid != 0)
			return;
	
		vector<glm::vec3> vertices((2.0f / 0.25 + 1)*4);
		int i=0;
		for (float y=-1.0f; y<=1.0f; y+=0.25f) {
			vertices[i++] = glm::vec3(-1.0f, y, 0.0f);
			vertices[i++] = glm::vec3(+1.0f, y, 0.0f);
		}
		
		for (float x=1.0f; x>=-1.0f; x-=0.25f) {
			vertices[i++] = glm::vec3(x, 1.0f, 0.0f);
			vertices[i++] = glm::vec3(x, -1.0f, 0.0f);
		}
	
		viewGrid = new ofMesh();
		viewGrid->addVertices(vertices);
		viewGrid->setMode(OF_PRIMITIVE_LINES);
	
		viewPlane = new ofMesh();
		vertices.resize(4);
		vertices[0] = glm::vec3(-1000.0f, -1000.0f, 0.0f);
		vertices[1] = glm::vec3(+1000.0f, -1000.0f, 0.0f);
		vertices[2] = glm::vec3(-1000.0f, +1000.0f, 0.0f);
		vertices[3] = glm::vec3(-1000.0f, +1000.0f, 0.0f);
		viewPlane->addVertices(vertices);
		viewPlane->setMode(OF_PRIMITIVE_TRIANGLE_FAN);	
	}


	glm::vec3 Plane::getRight() const {
		return glm::normalize(glm::cross(up, normal));
	}

	Ray Plane::getUpRay() const {
		return Ray(this->center, this->up);
	}

	Ray Plane::getRightRay() const {
		return Ray(this->center, getRight());
	}
}