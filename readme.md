ofxRay

# Introduction

A lightweight set of tools for intersection testings (e.g. does a ray intersect a plane).

# Objects

## ofRay

A _Ray_ is defined by the starting point __s__ and the transmission vector __t__ . Check equation (5) at http://mathworld.wolfram.com/Line.html

We define a ray as an infinite line (continuing infinitely forwards and backwards along the line __t__ and -__t__ starting at __s__ .

The line is defined by points __s__ + _u_ * __t__ , where {-inf < _u_ < +inf}.

A _Line segment_ is a line which has finite length. We denote a line segment as a ofRay between __s__ and __t__ where __infinite__ is set to _false_ , i.e. a _Line segment_ is a finite section along a ray

## ofPlane

A _Plane_ is defined by a position on the plane  __c__ and a normal vector __n__ .

We can also define the plane using the notation _a_ x + _b_ y + _c_ z + _d_ = 0 (by using the appropriate constructor).

For a non-infinite plane, we define the bounds of the _Plane segment_ with the 2D vector __bounds__, and the up vector __up__. We presume that the __up__ vector and __n__ vectors are perpendicular.

## ofGeometric

All objects within ofxRay inherit from ofGeometric. 