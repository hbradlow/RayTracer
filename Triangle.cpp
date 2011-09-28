//
//  Triangle.cpp
//  
//
//  Created by Henry Bradlow on 9/27/11.
//  Copyright (c) 2011 Westview High School. All rights reserved.
//

#ifndef _Triangle_h
#define _Triangle_h
#include <iostream>
#include <math.h>
#include <Eigen/Core>
#include <Eigen/LU>
#include <Eigen/Geometry>
#include "Object.cpp"

using namespace std;

class Triangle: public Object {
public:
    Eigen::Vector4f normal;
    Eigen::Vector4f v0;
    Eigen::Vector4f v1;
    Eigen::Vector4f v2;
    Triangle(){
        normal << 0,0,1,0;
        v0 << 0,0,0,1;
        v1 << 1,0,0,1;
        v2 << 0,1,0,1;
    }
    virtual float timeOfIntersection(Ray *ray);
    virtual void getNormalToWorldPoint(Eigen::Vector4f* loc, Eigen::Vector4f* normal);
};
void Triangle::getNormalToWorldPoint(Eigen::Vector4f* loc, Eigen::Vector4f* normal){
    *normal = (*o2w) * this->normal;
}
float Triangle::timeOfIntersection(Ray *ray){
    Eigen::Vector3f    u, v, n, w;             // triangle vectors
    float     r, a, b;             // params to calc ray-plane intersect
    
    Eigen::Vector4f loc4 = (o2w->inverse()) * (*ray->location);
    Eigen::Vector4f dir4 = (o2w->inverse()) * (*ray->direction);
    Eigen::Vector3f w0(loc4.x(),loc4.y(),loc4.z());
    Eigen::Vector3f dir(dir4.x(),dir4.y(),dir4.z());
    
    Eigen::Vector3f v30(v0.x(), v0.y(), v0.z());
    Eigen::Vector3f v31(v1.x(), v1.y(), v1.z());
    Eigen::Vector3f v32(v2.x(), v2.y(), v2.z());
    
    
    // get triangle edge vectors and plane normal
    u = v31 - v30;
    v = v32 - v30;
    n = u.cross(v);             // cross product
    //cout << "Vectors: " << v32 << v31 << v30 << endl;
    if (n.norm() < 0.01f)            // triangle is degenerate
        return -1;                 // do not deal with this case
    
    w0 = w0 - v30;
    a = -n.dot(w0);
    b = n.dot(dir);
    if (abs(b) < .001f) {     // ray is parallel to triangle plane
        return -1;
    }
    
    // get intersect point of ray with triangle plane
    r = a / b;
 //   cout << r << endl;

    if (r < 0.0)                   // ray goes away from triangle
        return -1;                  // => no intersect
    // for a segment, also test if (r > 1.0) => no intersect
    
    Eigen::Vector3f I = w0 + r * dir;           // intersect point of ray and plane
    
    // is I inside T?
    float    uu, uv, vv, wu, wv, D;
    uu = u.dot(u);
    uv = u.dot(v);
    vv = v.dot(v);
    w = I - v30;
    wu = w.dot(u);
    wv = w.dot(v);
    D = uv * uv - uu * vv;
    
    // get and test parametric coords
    float s, t;
    s = (uv * wv - vv * wu) / D;
    if (s < 0.0 || s > 1.0)        // I is outside T
        return -1;
    t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0)  // I is outside T
        return -1;
    
    return r;
}


#endif
