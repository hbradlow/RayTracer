//
//  Header.h
//  
//
//  Created by Henry Bradlow on 9/20/11.
//  Copyright (c) 2011 Westview High School. All rights reserved.
//

#ifndef _Object_h
#include "Vector3D.cpp"
#define _Object_h

class Object{
public:
    float r,g,b;
    virtual float intersectsRay(Ray r){
        return -1;
    }
};

#endif
