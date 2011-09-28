//
//  Sphere.cpp
//  
//
//  Created by Henry Bradlow on 9/25/11.
//  Copyright (c) 2011 Westview High School. All rights reserved.
//

#ifndef _Sphere_h
#define _Sphere_h
#include <iostream>
#include "Object.cpp"
#include <Eigen/LU>
#include <Eigen/Core>

using namespace std;

class Sphere: public Object {
public:
    float radius;
    Eigen::Vector4f center;
    Sphere(){
        radius = 1;
        center << 0,0,0,1;
    }
    virtual float timeOfIntersection(Ray *ray);
    virtual void getNormalToWorldPoint(Eigen::Vector4f* loc, Eigen::Vector4f* normal);
};
float Sphere::timeOfIntersection(Ray *ray){
    Ray r;
    Eigen::Vector4f v1 = (o2w->inverse() * (*(ray->direction)));
    Eigen::Vector4f v2 = (o2w->inverse() * (*(ray->location)));
    r.direction = &v1;
    r.location = &v2;
    
    
    float A = ((r.direction)->dot(*r.direction));
    float B = 2*(r.direction->dot(*r.location-center));
    float C = (*r.location-center).dot(*r.location-center)-(radius*radius);
    
    
    float descriminant = B*B-4*A*C;
    if (descriminant<.00001f) {
        return -1;
    }
    float ans1 = (-B+sqrt(descriminant))/(2*A);
    float ans2 = (-B-sqrt(descriminant))/(2*A);
    float ret = min(ans1,ans2);
    return ret>0 ? ret : max(ans1,ans2);
}
void Sphere::getNormalToWorldPoint(Eigen::Vector4f* loc, Eigen::Vector4f* normal){
    *normal = *loc-(*o2w * center);
}
#endif
