//
//  RayTracer.cpp
//  
//
//  Created by Henry Bradlow on 9/20/11.
//  Copyright (c) 2011 Westview High School. All rights reserved.
//
/*
 Section notes:
 Scale a circle to get an ellipse
 
 NEED HOMOGENEOUS COORDINATES - need to find better matrix library
 
 To find intersection with transformed shape, transform ray back to object space, then
 find where it intersects the object, then transform the point back to world space.
 Object space - sphere
 World space - ellipsoid
 
 Generate a ray:    (eye at the origin)     (vector has homogenous value of 0)
 In camera space: ray  = [0,0,0,1]      +       [x,y,-1,0]*t
Camera to World transformation = C2W = TR (some matrix)
 Ray in world space = C2W*ray
 
 
 
 
 */




#ifndef _RayTracer_h
#include <iostream>
#include <Eigen/Core>
#include "Scene.cpp"
#define _RayTracer_h


#define WIDTH 500
#define HEIGHT 500
#define BITS_PER_COLOR 8

using namespace std;

FIBITMAP * bitmap;

int setPixelColor(int x, int y, RGBQUAD *color){
    FreeImage_SetPixelColor(bitmap,x,y,color);
}
int main(int argc, char* argv[]){
    FreeImage_Initialise();
    bitmap = FreeImage_Allocate(WIDTH,HEIGHT,BITS_PER_COLOR*3);
    Scene *s = new Scene;
    Vector3D *eye = new Vector3D(WIDTH/2.0f,HEIGHT/2.0f,-WIDTH/2.0f);
    
    Sphere *shape = new Sphere(255,0,0,300,Vector3D(200,200,600));
    Light *light = new Light(1,0,0,Vector3D(-200,200,100));
    
    Eigen::Matrix4f m;
    m << 2,0,0,0,
         0,2,0,0,
         0,0,2,0,
         0,0,0,2;
    Eigen::Matrix4f m2;
    m2 << 1,0,0,6,
            1,1,0,0,
            3,4,1,0,
            0,5,6,1;
    cout << m+m2;
    
    
    s->objects.push_back(shape);
    s->lights.push_back(light);
    
    for(int i = 0; i<WIDTH; i++){
        for(int j = 0; j<HEIGHT; j++){//for each pixel
            //compute the ray:
            Ray r;
            r.location = eye;
            Vector3D dir = Vector3D(i,j,0)-*(r.location);
            r.direction = &dir;
            
            RGBQUAD *color = new RGBQUAD;
            color->rgbRed = 0;
            color->rgbGreen = 0;
            color->rgbBlue = 0;
            s->computeColorForRay(r,color);
            setPixelColor(i,j, color);
            delete color;
        }
    }
    
    delete shape;
    delete light;
    delete s;
    FreeImage_Save(FIF_PNG,bitmap,"test.png",0);
    FreeImage_DeInitialise();
    return 0;
}

#endif