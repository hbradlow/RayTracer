//
//  Camera.cpp
//  
//
//  Created by Henry Bradlow on 9/22/11.
//  Copyright (c) 2011 Westview High School. All rights reserved.
//
#ifndef _Camera_h
#define _Camera_h
#include <iostream>
#include <Eigen/Core>
#include <Eigen/LU>
using namespace std;

class Camera: public Object{
public:
    Ray *ray;
    Camera(Eigen::Matrix4f *object2World){
        o2w = object2World;
    }
    void getRayForWorldPosition(float x, float y, Ray *r){
        Eigen::Vector4f *v = new Eigen::Vector4f;
        *v << x,y,0,1;
        
      /*  Eigen::Vector4f w,v,u;
        w << 0,0,1,0;
        v << 0,1,0,0;
        u << 1,0,0,0;
        w = *o2w * w;
        v = *o2w * v;
        u = *o2w * u;*/
        
        *v = o2w->inverse() * (*v) - *(ray->location);
        Eigen::Vector4f *l = new Eigen::Vector4f;
        *l = *(o2w) * *(ray->location);
        r->location = l;
        
        *v = *(o2w) * (*v);

        r->direction = v;
    }
};

#endif
