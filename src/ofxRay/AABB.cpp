//
//  AABB.cpp
//
//  Created by Franck Youdine on 24/10/2019.
//

#include "AABB.h"

ofxRay::AABB::AABB(){
    this->randomiseVectors();
}
ofxRay::AABB::AABB( const glm::vec3 &min, const glm::vec3 &max ){
    this->set(min, max);
}
ofxRay::AABB::AABB( const glm::vec3 &min, const glm::vec3 &max, const ofColor &color ){
    this->set(min, max);
    this->color = color;
}
void ofxRay::AABB::set( const glm::vec3 &min, const glm::vec3 &max ) {
    for (int a = 0; a < 3; ++a){
        bool b = min[a] < max[a];
        this->min[a] = b ? min[a] : max[a];
        this->max[a] = b ? max[a] : min[a];
    }
    
    this->scale = this->max - this->min;
    this->center = this->min + (this->scale * .5f);
}


void ofxRay::AABB::setCenter( const glm::vec3 &center ) {
    this->center = center;
    _calcMinMax();
}
void ofxRay::AABB::setSize( const glm::vec3 &size ) {
    this->scale = size;
    _calcMinMax();
}


//ofGeometric
void ofxRay::AABB::draw() const {
    // draw a cube
    
    ofPushStyle();
    ofSetColor(color);
    ofPushMatrix();
    
    ofTranslate(this->center);
    ofScale(this->scale.x, this->scale.y, this->scale.z);
    
    auto & viewGrid = AABB::getView();
    viewGrid.draw();
    
    ofPopMatrix();
    ofPopStyle();
    
}

void ofxRay::AABB::randomiseVectors( float amplitude ) {
    this->set(
        glm::vec3( ofRandom(-1.f, 1.f), ofRandom(-1.f, 1.f), ofRandom(-1.f, 1.f) ) * amplitude,
        glm::vec3( ofRandom(-1.f, 1.f), ofRandom(-1.f, 1.f), ofRandom(-1.f, 1.f) ) * amplitude
    );
}

ofMesh & ofxRay::AABB::getView() {
    static unique_ptr<ofMesh> viewGrid;
    if (!viewGrid) {
        vector<glm::vec3> vertices(8);
        int i = 0;
        
        vertices[i++] = glm::vec3(-.5f, -.5f, .5f);
        vertices[i++] = glm::vec3( .5f, -.5f, .5f);
        vertices[i++] = glm::vec3( .5f,  .5f, .5f);
        vertices[i++] = glm::vec3(-.5f,  .5f, .5f);
        
        vertices[i++] = glm::vec3(-.5f, -.5f, -.5f);
        vertices[i++] = glm::vec3( .5f, -.5f, -.5f);
        vertices[i++] = glm::vec3( .5f,  .5f, -.5f);
        vertices[i++] = glm::vec3(-.5f,  .5f, -.5f);
        
        //     4 - 5
        //     | / |
        // 4 - 0 - 1 - 5 - 4
        // | / | \ | / | \ |
        // 7 - 3 - 2 - 6 - 7
        //     | / |
        //     7 - 6
        
        viewGrid = make_unique<ofMesh>();
        viewGrid->addVertices(vertices);
        //top
        viewGrid->addTriangle(0, 1, 2);
        viewGrid->addTriangle(0, 2, 3);
        //left
        viewGrid->addTriangle(0, 3, 7);
        viewGrid->addTriangle(0, 7, 4);
        //rear
        viewGrid->addTriangle(0, 4, 5);
        viewGrid->addTriangle(0, 5, 1);
        //right
        viewGrid->addTriangle(1, 5, 2);
        viewGrid->addTriangle(2, 5, 6);
        //front
        viewGrid->addTriangle(2, 6, 7);
        viewGrid->addTriangle(3, 2, 7);
        //bottom
        viewGrid->addTriangle(5, 4, 7);
        viewGrid->addTriangle(5, 7, 6);
        
        viewGrid->setMode(OF_PRIMITIVE_LINES);
    }
    
    return *viewGrid;
}

//-


bool ofxRay::AABB::intersect(const ofxRay::Ray &ray) const {
    float tmin = std::numeric_limits<float>::min();
    float tmax = std::numeric_limits<float>::max();
    
    return this->intersect(ray, tmin, tmax);
}

bool ofxRay::AABB::intersect(const ofxRay::Ray &ray, float tmin, float tmax) const {
    
    for (int a = 0; a < 3; ++a)
    {
        float invD = 1.0f / ray.t[a];
        float t0 = (this->min[a] - ray.s[a]) * invD;
        float t1 = (this->max[a] - ray.s[a]) * invD;
        
        if (invD < 0.0f) {
            float temp = t1;
            t1 = t0;
            t0 = temp;
        }
        
        tmin = t0 > tmin ? t0 : tmin;
        tmax = t1 < tmax ? t1 : tmax;
        
        if (tmax <= tmin)
            return false;
    }
    
    return true;
}

bool ofxRay::AABB::intersect(const AABB &other) const
{
    return  min.x < other.max.x &&
            min.y < other.max.y &&
            min.z < other.max.z &&
            other.min.x < max.x &&
            other.min.y < max.y &&
            other.min.z < max.z;
}

void ofxRay::AABB::_calcMinMax()
{
    this->min = this->center - (this->scale*.5);
    this->max = this->min + this->scale;
}
