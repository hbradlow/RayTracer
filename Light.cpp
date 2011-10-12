//
//  Light.cpp
//  
//
//  Created by Henry Bradlow on 9/26/11.
//  Copyright (c) 2011 Westview High School. All rights reserved.
//

#ifndef _Light_h
#define _Light_h
#include <iostream>
#include <Eigen/Core>

class Light: public Object {
public:
    float r,g,b;
    Eigen::Vector4f *direction;
};


#endif