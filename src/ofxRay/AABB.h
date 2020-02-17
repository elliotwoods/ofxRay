#pragma once
//
//  AABB.hpp
//
//  Created by Franck Youdine on 24/10/2019.
//


#include "Base.h"
#include "Ray.h"


namespace ofxRay {
    class AABB : public Base {
        public:
            AABB();
            AABB( const glm::vec3 &min, const glm::vec3 &max );
            AABB( const glm::vec3 &min, const glm::vec3 &max, const ofColor &color );
        
            void set( const glm::vec3 &min, const glm::vec3 &max );
            void setCenter( const glm::vec3 &center );
            void setSize( const glm::vec3 &size );
            
            //ofGeometric
            void draw() const override;
            void randomiseVectors(float amplitude=1.0f);
            //-
            
            bool intersect(const Ray &ray) const;
            bool intersect(const Ray &ray, float tmin, float tmax) const;
            bool intersect(const AABB &other) const;
            
            glm::vec3 min;
            glm::vec3 max;
            glm::vec3 scale;
            glm::vec3 center;
        protected:
            void _calcMinMax();
            static ofMesh & getView();
    };
}
