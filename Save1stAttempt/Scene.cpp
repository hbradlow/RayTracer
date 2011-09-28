//
//  Scene.h
//  
//
//  Created by Henry Bradlow on 9/20/11.
//  Copyright (c) 2011 Westview High School. All rights reserved.
//
#ifndef _Scene_h
#include "Vector3D.cpp"
#include "Object.cpp"
#include "Light.cpp"
#include "Sphere.cpp"
#include "FreeImage.h"
#include <iostream>
#define _Scene_h

using namespace std;

class Scene{
public:
    vector<Object*> objects;
    vector<Light*> lights;
    void computeColorForRay(Ray ray,RGBQUAD* color);
};

void Scene::computeColorForRay(Ray ray,RGBQUAD* color){
    for(int i = 0; i<objects.size(); i++){
        float t = objects[i]->intersectsRay(ray);
        if(t != -1){
           
            for(int j = 0; j<lights.size(); j++)
            {
                Ray lightRay;
                Vector3D loc = *ray.location+*ray.direction*t;
                lightRay.location = &loc;
                
                Vector3D dir = *(lights[j]->location)-*lightRay.location;
                lightRay.direction = &dir;
                
                Vector3D loc2 = *lightRay.location+((*lightRay.direction)*.0001f);
                lightRay.location = &loc2;
                
                for(int k = 0; k<objects.size(); k++){
                    float t2 = objects[k]->intersectsRay(lightRay);
                   //  printf("t: %f\n",t2);
                    if(t2 < 0){
                        color->rgbRed += objects[i]->r*lights[j]->r;
                        color->rgbGreen += objects[i]->g*lights[j]->g;
                        color->rgbBlue += objects[i]->b*lights[j]->b;
                    }
                }
            }
            return;
        }
    }
    return;
}

#endif