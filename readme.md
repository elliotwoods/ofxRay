ofxRay

# Introduction

A lightweight set of tools for intersection testings (e.g. does a ray intersect a plane).

# Objects

## Ray

A ray here is defined by the starting point __s__ and the transmission vector __t__ . Check equation (5) at http://mathworld.wolfram.com/Line.html

We define a ray as an infinite line (continuing infinitely forwards and backwards along the line __t__ and -__t__ starting at __s__ .

The line is defined by points __s__ + _u_ * __t__ .

A _Line segment_ is a line which has finite length. We denote a line segment  as a line between __s__ and __t__ where __infinite__ is set to _false_ .

## Plane

A plane is defined by a vector a position on the plane  __c__ and a normal vector __n__ .

We can also define the plane using the notation ax+by+cz+d=0, using the appropriate constructor.

For a non-infinite plane, we define the bounds of the plane section with the 2D vector __bounds__.