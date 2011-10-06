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
    Eigen::Vector4f location;
    Camera(Eigen::Matrix4f *object2World){
        o2w = object2World;
        location << 0,0,0,1;
    }
    void getRayForWorldPosition(float x, float y, Ray *r){
        Eigen::Vector4f v;
        v << x,y,-1,1;
       // cout << o2w->inverse() << endl;
        v = v-location;
        v = o2w->inverse() * (v);
        
        
        Eigen::Vector4f l;
        l = (*o2w) * location;
        r->location = new Eigen::Vector4f;
        *r->location = l;
        
        
        r->direction = new Eigen::Vector4f;
        *r->direction = v;
    }
};

#endif
