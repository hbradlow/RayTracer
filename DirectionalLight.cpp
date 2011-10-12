//
//  Light.cpp
//  
//
//  Created by Henry Bradlow on 9/26/11.
//  Copyright (c) 2011 Westview High School. All rights reserved.
//

#ifndef _DLight_h
#define _DLight_h
#include <iostream>
#include <Eigen/Core>

class DirectionalLight: public Light {
public:
    Eigen::Vector4f *direction;
};


#endif