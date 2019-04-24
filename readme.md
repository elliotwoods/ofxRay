# ofxRay

## Introduction

A lightweight set of tools for intersection testings (e.g. does a ray intersect a plane).

## Note

The current code in this repository is designed for openFrameworks 0.10.0. 
If you are looking for a version of ofxRay which is compatible with previous versions of openFrameworks, please check [https://github.com/elliotwoods/ofxRay/tree/0.9.8](https://github.com/elliotwoods/ofxRay/tree/0.9.8) .

## Objects

### ofxRay::Ray

A _Ray_ is defined by the starting point __s__ and the transmission vector __t__ . Check equation (5) at http://mathworld.wolfram.com/Line.html

We define a ray as an infinite line (continuing infinitely forwards and backwards along the line __t__ and - __t__ starting at __s__ .

The line is defined by points __s__ + __u__ * __t__ , where {-inf < _u_ < +inf}.

A _Line segment_ is a line which has finite length. We denote a line segment as a ofRay between __s__ and __t__ where __infinite__ is set to _false_ , i.e. a _Line segment_ is a finite section along a ray

### ofxRay::Plane

A _Plane_ is defined by a position on the plane  __c__ and a normal vector __n__ .

We can also define the plane using the notation _a_ x + _b_ y + _c_ z + _d_ = 0 (by using the appropriate constructor).

For a non-infinite plane, we define the bounds of the _Plane segment_ with the 2D vector __bounds__, and the up vector __up__. We presume that the __up__ vector and __n__ vectors are perpendicular.

### ofxRay::Projector

A _Projector_ is defined by __projection__ matrix and its pose (defined by ofNode which it inherits from).

We can use a projector to preview a projector or camera in a scene, and to generate pixel _ofRay_s.

### ofxRay::Camera

A _Camera_ is defined the same as a _Projector_, but has distortion coefficients also.

### ofxRay::Base

All objects within ofxRay inherit from ofxRay::Base. 
