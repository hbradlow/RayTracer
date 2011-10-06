//
//  Scene.cpp
//  
//
//  Created by Henry Bradlow on 9/22/11.
//  Copyright (c) 2011 Westview High School. All rights reserved.
//
/*  
    Midterm Review:
 
    -CHECK OUT SP11 REVIEW ON TRANSFORMATIONS
    -CHECK OUT DEPTH DISTORTION
    -CHECK DRAWING A LINE AND FILLING A POLYGON
 
 
    Lecture Notes:
    Intersection: Ray vs Sphere (formula in book)     (Triangle Equation)    (Ray)
    Triangle: in barycentric coordinates: X(B,y) = V1 + B(V2-V1) + y(V3-V1) = A+tD B+y<=1 and B,y>=0
    
    Searching:
    BSP-Trees: Binary Space Partition Trees
    Axis-Aligned Bounding Boxes (AABB)
 
    Transformations: Transform Ray to test if it hits the object in the object space
 
    Trees:
        Group nodes
        Leaf nodes
    
 
    
    Section Notes - 9/29/2011:
        
        for every pixel{
            Ray w = c2w*Ray_Camera;
            Color c = trace(w);
            colorPixel(i,j,c);
        }
 
 
        Color trace(Ray w){
            1. intersection with objects
                - take intersection with min t
            2. for every light
                - if light is not bocked
                    shade
            3. reflect and repeat
        }
 
 
    Path tracing: advanced and used to model difhuse objects
 
    Transformations:
        Camera:
            Ray starts at origin;
            Film is at plane z=-1;
        World:
        Object:
 
 
    Ellipsoid Normal:
        Transform for point p - M
            to transform N - (M^(-t)) (inverse transpose)
 */
#ifndef _Scene_h
#define _Scene_h

#include <iostream>
#include <vector>
#include <math.h>
#include <Eigen/Core>
#include "Object.cpp"
#include "FreeImage.h"
#include "Camera.cpp"
#include "Sphere.cpp"
#include "Color.cpp"
#include "RayTracer.cpp"
#include "Light.cpp"
#include "Triangle.cpp"

using namespace std;

FIBITMAP * bitmap;
vector<Object*> *objects;
vector<Object*> *lights;
void addSphere(float x, float y, float z, float r, float ka, float kd, float ks, float kr){
    Eigen::Matrix4f *o2w = new Eigen::Matrix4f;
    Eigen::Matrix4f translate, scale, rotate;
    translate << 1,0,0,x,
    0,1,0,y,
    0,0,1,z,
    0,0,0,1;
    scale << r/1.0f,0,0,0,
    0,r/1.0f,0,0,
    0,0,r/1.0f,0,
    0,0,0,1;
    float theta = 1.0f;
    rotate << 1,0,0,0,
    0,cos(theta),-sin(theta),0,
    0,sin(theta),cos(theta),0,
    0,0,0,1;
    
    
    *o2w = translate*scale;
    Object *o = new Sphere();
    o->o2w = o2w;
    o->brdf.krs = ks;
    o->brdf.krd = kd;
    o->brdf.kra = ka;
    o->brdf.krr = kr;
    
    o->brdf.kgs = ks;
    o->brdf.kgd = kd;
    o->brdf.kga = ka;
    o->brdf.kgr = kr;
    
    o->brdf.kbs = ks;
    o->brdf.kbd = kd;
    o->brdf.kba = ka;
    o->brdf.kbr = kr;
    
    objects->push_back(o);
}
void addTriangle(float z, int up){
    Eigen::Matrix4f *o2w = new Eigen::Matrix4f;
    Eigen::Matrix4f translate;
    Eigen::Matrix4f scale;
    Eigen::Matrix4f rotate;
    translate << 1,0,0,-15,
    0,1,0,-15,
    0,0,1,z,
    0,0,0,1;
    scale << 60,0,0,0,
    0,60,0,0,
    0,0,up*60,0,
    0,0,0,1;
    float theta = 0.0f;
    rotate << 1,0,0,0,
    0,cos(theta),-sin(theta),0,
    0,sin(theta),cos(theta),0,
    0,0,0,1;
    
    
    *o2w = rotate*translate*scale;
    Object *o = new Triangle();
    o->o2w = o2w;
    o->brdf.krs = 0.8f;
    o->brdf.krd = 0.2f;
    o->brdf.kra = 0.05f;
    o->brdf.krr = 0.1f;
    
    o->brdf.kgs = 0.8f;
    o->brdf.kgd = 0.2f;
    o->brdf.kga = 0.05f;
    o->brdf.kgr = 0.1f;
    
    o->brdf.kbs = 0.8f;
    o->brdf.kbd = 0.2f;
    o->brdf.kba = 0.05f;
    o->brdf.kbr = 0.1f;
    
    objects->push_back(o);
}
void addPointLight(float x, float y, float z){
    Eigen::Matrix4f *o2w = new Eigen::Matrix4f;
    Eigen::Matrix4f translate;
    Eigen::Matrix4f scale;
    translate << 1,0,0,x,
    0,1,0,y,
    0,0,1,z,
    0,0,0,1;
    scale << 1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,0,1;
    
    *o2w = translate*scale;
    Light *o = new Light();
    o->o2w = o2w;
    o->r = 1.0f;
    o->g = 1.0f;
    o->b = 1.0f;
    lights->push_back(o);
}
void initScene(){    
    objects = new vector<Object*>;
    lights = new vector<Object*>;
    
    addPointLight(0,-1,-2);
    addPointLight(0,-5,-2.2f);
    addPointLight(0,5,-2.2f);
    addSphere(0,0,-3,.4f,0,0,0,1.0f);
    addSphere(1,1,-3,.4f,0,0,0,1.0f);
    addSphere(1,0,-3,.4f,0,0,0,1.0f);
    addSphere(0,1,-3,.4f,0,0,0,1.0f);
    addSphere(0,-1,-3,.4f,0,0,0,1.0f);
    addSphere(-1,0,-3,.4f,0,0,0,1.0f);
    addSphere(-1,-1,-3,.4f,0,0,0,1.0f);
    addSphere(1,-1,-3,.4f,0,0,0,1.0f);
    addSphere(-1,1,-3,.4f,0,0,0,1.0f);
    addTriangle(-3.4f,1);
    addTriangle(-1,-1);
}

int setPixelColor(int x, int y, Color *color){
    RGBQUAD *c = new RGBQUAD;
    c->rgbRed = 255*color->r;
    c->rgbGreen = 255*color->g;
    c->rgbBlue = 255*color->b;
    FreeImage_SetPixelColor(bitmap,x,y,c);
    delete c;
}
int main(int argc, char *argv[]){
    
    int screenWidth = 500, screenHeight = 500;
    float resolution = 1.0f;

    FreeImage_Initialise();
    bitmap = FreeImage_Allocate(screenWidth*resolution,screenHeight*resolution,8*3);

    initScene();
    
    //set up camera to world transformation
    Eigen::Matrix4f camera2World;
    Eigen::Matrix4f translate;
    Eigen::Matrix4f scale;
    Eigen::Matrix4f rotatex,rotatez;
    translate << 1,0,0,.8f,
            0,1,0,-1,
            0,0,1,-2.5f,
            0,0,0,1;
    scale << screenWidth/2,0,0,0,
            0,screenHeight/2,0,0,
            0,0,1,0,
    0,0,0,1;
    float theta = -3.14159f/3.0f;
    rotatex << 1,0,0,0,
    0,cos(theta),-sin(theta),0,
    0,sin(theta),cos(theta),0,
    0,0,0,1; 
    theta = -3.14159f/5.5f;
    rotatez << cos(theta),-sin(theta),0,0,
    sin(theta),cos(theta),0,0,
    0,0,1,0,
    0,0,0,1; 
    
    //cout << rotate << endl;
    camera2World = translate*rotatex*rotatez;
    
    Camera *c = new Camera(&camera2World);
    //c->location = location;

        
    Ray *r = new Ray;
    RayTracer *rt = new RayTracer;
    Color *color = new Color(0,0,0);

    float percentComplete = 0;
    float prev = 0;
    printf("\rPercent complete: %d%%", (int)(percentComplete*100));
    fflush(stdout);
    for(float i = 0; i<screenWidth; i+=1.0f/resolution){
        for (float j = 0; j<screenHeight; j+=1.0f/resolution) {
            c->getRayForWorldPosition((screenWidth/2.0f-i)/(float)screenWidth,(screenHeight/2.0f-j)/(float)screenHeight,r);
            rt->trace(r,0,color,objects, lights);
            setPixelColor((screenWidth-i)*resolution,(screenHeight-j)*resolution,color);
        }
        
        
        percentComplete = (float)i/(screenWidth);
        if(percentComplete-prev>0.01f)
        {
            prev = percentComplete;
            printf("\rPercent complete: %d%% [", (int)(percentComplete*100));
            int i;
            for(i = 1; i<(int)(percentComplete*100)+1; i++)
            {
                printf("=");
            }
            printf(">");
            for(int j = i; j<100; j++)
            {
                printf(" ");
            }
            printf("]");
            fflush(stdout);
        }
    }
    
    FreeImage_Save(FIF_PNG,bitmap,"test.png",0);
    FreeImage_DeInitialise();
}

#endif