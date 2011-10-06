//
//  Object.cpp
//  
//
//  Created by Henry Bradlow on 9/22/11.
//  Copyright (c) 2011 Westview High School. All rights reserved.
//
#ifndef _Object_h
#define _Object_h
#include <iostream>
#include <Eigen/Core>

using namespace std;

struct Ray {
    Eigen::Vector4f *location;
    Eigen::Vector4f *direction;
};
struct BRDF {
    float kra,kga,kba;
    float krd,kgd,kbd;
    float krs,kgs,kbs;
    float krr,kgr,kbr;
};

class Object{
public:
    Eigen::Matrix4f *o2w;
    BRDF brdf;
    virtual float timeOfIntersection(Ray *ray);
    virtual void getWorldLocation(Eigen::Vector4f* loc);
    virtual void getNormalToWorldPoint(Eigen::Vector4f* loc, Eigen::Vector4f* normal);
};
float Object::timeOfIntersection(Ray *ray){
    return 0;
}
void Object::getWorldLocation(Eigen::Vector4f* loc){
    *loc << 0,0,0,1;
    *loc = *o2w * (*loc);
}
void Object::getNormalToWorldPoint(Eigen::Vector4f* loc, Eigen::Vector4f* normal){
}
#endif