//
//  Scene.cpp
//  
//
//  Created by Henry Bradlow on 9/22/11.
//  Copyright (c) 2011 Westview High School. All rights reserved.
//
/*  Lecture Notes:
    Intersection: Ray vs Sphere (formula in book)     (Triangle Equation)    (Ray)
    Triangle: in barycentric coordinates: X(B,y) = V1 + B(V2-V1) + y(V3-V1) = A+tD B+y<=1 and B,y>=0
    
    Searching:
    BSP-Trees: Binary Space Partition Trees
    Axis-Aligned Bounding Boxes (AABB)
 
    Transformations: Transform Ray to test if it hits the object in the object space
 
    Trees:
        Group nodes
        Leaf nodes
    
 
 */
#ifndef _Scene_h
#define _Scene_h

#include <iostream>
#include <vector>
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
void addSphere(float x, float y, float z, float r){
    Eigen::Matrix4f *o2w = new Eigen::Matrix4f;
    Eigen::Matrix4f translate;
    Eigen::Matrix4f scale;
    translate << 1,0,0,x,
    0,1,0,y,
    0,0,1,z,
    0,0,0,1;
    scale << r,0,0,0,
    0,r,0,0,
    0,0,r,0,
    0,0,0,1;
    
    *o2w = translate*scale;
    Object *o = new Sphere();
    o->o2w = o2w;
    
    objects->push_back(o);
}
void addTriangle(){
    Eigen::Matrix4f *o2w = new Eigen::Matrix4f;
    Eigen::Matrix4f translate;
    Eigen::Matrix4f scale;
    Eigen::Matrix4f rotate;
    translate << 1,0,0,-3000,
    0,1,0,-3000,
    0,0,1,-1150,
    0,0,0,1;
    scale << 6000,0,0,0,
    0,6000,0,0,
    0,0,6000,0,
    0,0,0,1;
    float theta = 1.0f;
    rotate << 1,0,0,0,
    0,cos(theta),-sin(theta),0,
    0,sin(theta),cos(theta),0,
    0,0,0,1;
    
    
    *o2w = rotate*translate*scale;
    Object *o = new Triangle();
    o->o2w = o2w;
    
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
    Object *o = new Light();
    o->o2w = o2w;
    lights->push_back(o);
}
void initScene(){
    float r = 60;
    
    objects = new vector<Object*>;
    lights = new vector<Object*>;
    float z = -1000;
    
    /*addSphere(0,0,z,100);
    addSphere(150,150,z,100);
    addSphere(150,-150,z,100);
    addSphere(-150,150,z,100);
    addSphere(-150,-150,z,100);
    
    addSphere(300,0,z,100);
    addSphere(-300,0,z,100);
    addSphere(0,300,z,100);
    addSphere(0,-300,z,100);*/
    
    for(int i = -1200; i<1200; i+=200){
        for(int j = -500; j>-2100; j-=200){
            if(i%400==0)
                addSphere(i,j/2+530,j,90);
            else
                addSphere(i,(j+100)/2+530,j+100,90);
        }
    }
    
    
    
    addTriangle();
    
    //addPointLight(0,-300,100);
    addPointLight(-300,800,-800);
    addPointLight(800,800,-300);
    addPointLight(-300,-800,-300);
    
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
    Eigen::Matrix4f rotate;
    translate << 1,0,0,0,
            0,1,0,0,
            0,0,1,1500,
            0,0,0,1;
    scale << screenWidth/2,0,0,0,
            0,screenHeight/2,0,0,
            0,0,1,0,
    0,0,0,1;
    float theta = .0f;
    rotate << 1,0,0,0,
    0,cos(theta),-sin(theta),0,
    0,sin(theta),cos(theta),0,
    0,0,0,1;
    camera2World = translate*scale*rotate;
    
    Ray cameraRay;
    Eigen::Vector4f location;
    Eigen::Vector4f direction;
    location << 0,0,1,1;
    direction << 0,0,1,0;
    cameraRay.location = &location;
    cameraRay.direction = &direction;
    Camera *c = new Camera(&camera2World);
    c->ray = &cameraRay;

        
    Ray *r = new Ray;
    RayTracer *rt = new RayTracer;
    Color *color = new Color(0,0,0);

    float percentComplete = 0;
    float prev = 0;
    printf("\rPercent complete: %d%%", (int)(percentComplete*100));
    fflush(stdout);
    for(float i = 0; i<screenWidth; i+=1.0f/resolution){
        for (float j = 0; j<screenHeight; j+=1.0f/resolution) {
            c->getRayForWorldPosition(screenWidth/2.0f-i,screenWidth/2.0f-j,r);
            rt->trace(r,0,color,objects, lights);
            setPixelColor(i*resolution,j*resolution,color);
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