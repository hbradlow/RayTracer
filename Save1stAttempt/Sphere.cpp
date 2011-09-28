//
//  Sphere.cpp
//  
//
//  Created by Henry Bradlow on 9/20/11.
//  Copyright (c) 2011 Westview High School. All rights reserved.
//

#ifndef _SPHERE_h
#include <iostream>
#include "Object.cpp"
#include "Vector3D.cpp"
#define _SPHERE_h

class Sphere: public Object{
public:
    float radius;
    Vector3D *center;
    
    Sphere(float red, float green, float blue, float rad, Vector3D c) {
        r = red;
        g = green;
        b = blue;
        radius = rad;
        center = new Vector3D(c);
    }
    ~Sphere(){
        delete center;
    }
    virtual float intersectsRay(Ray r);
};

float Sphere::intersectsRay(Ray r){//returns time of the first intersection    
    float A = r.direction->dot(*r.direction);
    float B = 2*(r.direction->dot(*r.location-*center));
    float C = (*r.location-*center).dot((*r.location-*center))-radius*radius;
    float descriminant = B*B-4*A*C;
    if (descriminant<0) {
        return -1;
    }
    float ans1 = (-B+sqrt(descriminant))/(2*A);
    float ans2 = (-B-sqrt(descriminant))/(2*A);
    float ret = min(ans1,ans2);
    return ret>0 ? ret : max(ans1,ans2);
}

#endif