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
		ofVec3f direction(a,b,c);
		this->setCenter(direction * d);
		this->setNormal(direction.getNormalized());
		this->setScale(ofVec2f(1.0f, 1.0f));
		infinite = true;
		makeGrid();
	}

	Plane::Plane(ofVec3f center, ofVec3f normal) {
		setCenter(center);
		setNormal(normal);
		infinite = true;
		makeGrid();
	}

	Plane::Plane(ofVec3f center, ofVec3f normal, ofVec3f up, ofVec2f scale) {
		setCenter(center);
		setNormal(normal);
		setUp(up);
		setScale(scale);
		infinite = false;	
		makeGrid();
	}

	void Plane::draw() const {
		ofPushStyle();
		ofSetColor(color);
		ofPushMatrix();
	
		ofTranslate(center);
		ofQuaternion q, r;
		q.makeRotate(ofVec3f(0,0,1), normal);
		r.makeRotate(ofVec3f(0,1,0) * q, up);
		float angle, x, y, z;
		(q * r).getRotate(angle, x, y, z);
		ofRotate(angle, x, y, z);
		ofScale(this->scale.x, this->scale.y, 1.0f);
	
		viewGrid->draw();
	
		if (this->infinite) {
			ofPushStyle();
			ofEnableAlphaBlending();
			ofSetColor(color.r, color.g, color.b, 50);
			viewPlane->draw();	
			ofPopStyle();
		}
	
		ofPopMatrix();
	
		ofDrawArrow(getCenter(), getCenter() + getNormal());
		ofDrawArrow(getCenter(), getCenter() + getUp());
		ofDrawArrow(getCenter(), getCenter() + getRight());
		ofPopStyle();
	}

	void Plane::randomiseVectors(float amplitude) {
		center = ofVec3f(ofRandom(-1.0f, 1.0f),
					ofRandom(-1.0f, 1.0f),
					ofRandom(-1.0f, 1.0f)) * amplitude;
		normal = ofVec3f(ofRandom(-1.0f, 1.0f),
					ofRandom(-1.0f, 1.0f),
					ofRandom(-1.0f, 1.0f));
		up = ofVec3f(ofRandom(-1.0f, 1.0f),
					 ofRandom(-1.0f, 1.0f),
					 ofRandom(-1.0f, 1.0f));
		scale = ofVec2f(ofRandom(-1.0f, 1.0f),
						ofRandom(-1.0f, 1.0f)) * amplitude;
		normal.normalize();
		infinite = true;
	}

	bool Plane::intersect(const Ray &ray) const {
		ofVec3f position;
		return intersect(ray, position);
	}

	bool Plane::intersect(const Ray &ray, ofVec3f &position) const {
		//check if line is parallel to plane
		//if so, die young
		if (ray.t.dot(normal) == 0.0f)
			return false;
	
		float u = normal.dot(center - ray.s) / normal.dot(ray.t);	
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
			float y = sqrt((center - position).lengthSquared() - x*x);
		
			//if length along this ray < height and distance of point to ray < width then we're in the plane
			if (abs(x) <= scale.x && abs(y) <= scale.y)
				return true;
			else
				return false;
		
		} else
			return true;
	}

	const ofVec3f& Plane::getCenter() const{
		return this->center;
	}

	const ofVec3f& Plane::getNormal() const {
		return this->normal;
	}

	const ofVec3f& Plane::getUp() const {
		return this->up;
	}

	const ofVec2f& Plane::getScale() const{
		return this->scale;
	}

	ofVec4f Plane::getABCD() const {
		ofVec4f ABCD;
		ABCD.x = this->normal.x;
		ABCD.y = this->normal.y;
		ABCD.z = this->normal.z;
		ABCD.w = - this->normal.dot(this->center);
		return ABCD;
	}

	bool Plane::getInfinite() const {
		return this->infinite;
	}

	void Plane::setCenter(const ofVec3f& center) {
		this->center = center;
	}

	void Plane::setNormal(const ofVec3f& normal) {
		this->normal = normal.getNormalized();
		setUp(this->up); //reset up (to ensure orthogonal to normal)
	}

	void Plane::setUp(const ofVec3f& up) {
		this->up = up;
		//remove component of up which is parallel to the normal
		this->up = this->up - this->normal * this->up.dot(this->normal);
		this->up.normalize();
	}

	void Plane::setScale(const ofVec2f& scale) {
		this->scale = scale;
	}

	void Plane::setInfinite(const bool b) {
		infinite = b;
	}
	
	void Plane::setFrom(ofPlanePrimitive & planePrimitive) {
		const ofMatrix4x4 transform = planePrimitive.getLocalTransformMatrix();
		ofVec3f translation, scale;
		ofQuaternion rotation, so;
		transform.decompose(translation, rotation, scale, so);
		
		this->setScale(scale * ofVec2f(planePrimitive.getWidth(), planePrimitive.getHeight()) / 2.0f);
		this->setCenter(translation);
		
		this->setNormal(ofVec3f(0.0f, 0.0f, 1.0f) * rotation);
		this->setUp(ofVec3f(0.0f, 1.0f, 0.0f) * rotation);

		this->setInfinite(false);
	}

	void Plane::getCornerRaysTo(const ofVec3f &target, Ray* rays) const {
		ofVec3f up = this->up * scale.y;
		ofVec3f right = getRight() * scale.x;
		ofVec3f corner;
	
		corner = center - up - right;
		*rays++ = Ray(corner, target - corner, color, false);

		corner = center - up + right;
		*rays++ = Ray(corner, target - corner, color, false);
	
		corner = center + up + right;
		*rays++ = Ray(corner, target - corner, color, false);
	
		corner = center + up - right;
		*rays++ = Ray(corner, target - corner, color, false);
	}

	void Plane::getCornerRaysFrom(const ofVec3f &source, Ray* rays) const {
		ofVec3f up = this->up * scale.y;
		ofVec3f right = getRight() * scale.x;
		ofVec3f corner;
	
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
	float Plane::getDistanceTo(const ofVec3f & point) const {
		const auto ABCD = this->getABCD();
		const auto ABC = ofVec3f(ABCD.x, ABCD.y, ABCD.z);
		const auto numerator = abs(normal.dot(point) + ABCD.w);
		const auto denominator = ABC.length();
		return numerator / denominator;
	}

	void Plane::makeGrid() {
		if (viewGrid != 0)
			return;
	
		vector<ofVec3f> vertices((2.0f / 0.25 + 1)*4);
		int i=0;
		for (float y=-1.0f; y<=1.0f; y+=0.25f) {
			vertices[i++] = ofVec3f(-1.0f, y, 0.0f);
			vertices[i++] = ofVec3f(+1.0f, y, 0.0f);
		}
		cout << endl;
		for (float x=1.0f; x>=-1.0f; x-=0.25f) {
			vertices[i++] = ofVec3f(x, 1.0f, 0.0f);
			vertices[i++] = ofVec3f(x, -1.0f, 0.0f);
		}
	
		viewGrid = new ofMesh();
		viewGrid->addVertices(vertices);
		viewGrid->setMode(OF_PRIMITIVE_LINES);
	
		viewPlane = new ofMesh();
		vertices.resize(4);
		vertices[0] = ofVec3f(-1000.0f, -1000.0f, 0.0f);
		vertices[1] = ofVec3f(+1000.0f, -1000.0f, 0.0f);
		vertices[2] = ofVec3f(-1000.0f, +1000.0f, 0.0f);
		vertices[3] = ofVec3f(-1000.0f, +1000.0f, 0.0f);
		viewPlane->addVertices(vertices);
		viewPlane->setMode(OF_PRIMITIVE_TRIANGLE_FAN);	
	}


	ofVec3f Plane::getRight() const {
		return up.getCrossed(normal).normalize();
	}

	Ray Plane::getUpRay() const {
		return Ray(this->center, this->up);
	}

	Ray Plane::getRightRay() const {
		return Ray(this->center, getRight());
	}
}