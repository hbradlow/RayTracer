//
//  Sphere.cpp
//  
//
//  Created by Henry Bradlow on 9/20/11.
//  Copyright (c) 2011 Westview High School. All rights reserved.
//

#ifndef _Light_h
#include <iostream>
#include "Object.cpp"
#include "Vector3D.cpp"
#define _Light_h

class Light: public Object{
public:
    Vector3D *location;
    
    Light(float red, float green, float blue, Vector3D c) {
        r = red;
        g = green;
        b = blue;
        location = new Vector3D(c);
    }
    ~Light(){
        delete location;
    }
};

#endif