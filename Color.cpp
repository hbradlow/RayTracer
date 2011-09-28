//
//  Color.cpp
//  
//
//  Created by Henry Bradlow on 9/25/11.
//  Copyright (c) 2011 Westview High School. All rights reserved.
//

#ifndef _Color_h
#define _Color_h
#include <iostream>

class Color {
public:
    float r,g,b;
    Color(float red,float green, float blue){
        r = red;
        g = green;
        b = blue;
    }
};

#endif