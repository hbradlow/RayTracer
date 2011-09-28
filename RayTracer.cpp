//
//  RayTracer.cpp
//  
//
//  Created by Henry Bradlow on 9/25/11.
//  Copyright (c) 2011 Westview High School. All rights reserved.
//
#ifndef _RayTracer_h
#define _RayTracer_h
#include <iostream>
#include <vector>
#include <Eigen/Core>
#include "Object.cpp"
#include <math.h>
using namespace std;

class RayTracer {
public:
    void trace(Ray *ray, int depth, Color* color, vector<Object*>* objects, vector<Object*>* lights);
};

void shade(Ray *ray, float t, Color* color,Eigen::Vector4f* lightPos, Object *o){
    Eigen::Vector4f location = (*(ray->location) + (*(ray->direction)*(t)));
    
    Eigen::Vector4f lightDir = ((*lightPos)-(*(ray->location) + (*(ray->direction)*t)));
    lightDir = lightDir/(lightDir.norm());
    
    Eigen::Vector4f viewDir = *(ray->direction);
    viewDir = viewDir/(viewDir.norm());

    Eigen::Vector4f* normal = new Eigen::Vector4f;
    o->getNormalToWorldPoint(&location,normal);
    *normal = *normal/(normal->norm());
    
    Eigen::Vector4f r;
    r = (lightDir) - 2*(lightDir.dot(*normal))*(*normal);
    r = r/(r.norm());
    
    float kd = .2f;
    float ks = .8f;
    float dot = max(normal->dot(lightDir),0.0f)*kd + pow(max(r.dot(viewDir),0.0f),100)*ks;
    color->r += dot;
    
    delete normal;
}

void RayTracer::trace(Ray *ray, int depth, Color* color, vector<Object*>* objects, vector<Object*>* lights) {
    if(depth==0){
        color->r = 0;
        color->g = 0;
        color->b = 0;
    }
    if(depth>5){
        color->r += 0;
        color->g += 0;
        color->b += 0;
        return;
    }
    Object* o;
    float tmin = 999999999;
    bool found = false;
    for(int i = 0; i<objects->size(); i++){
        if((*objects)[i]->timeOfIntersection(ray)>1)
        {
            if((*objects)[i]->timeOfIntersection(ray)<tmin){
                tmin = (*objects)[i]->timeOfIntersection(ray);
                o = (*objects)[i];
                found = true;
            }
        }
    }
    if(found)
    {
        for(int k = 0; k<lights->size(); k++){
            Eigen::Vector4f *lightPos = new Eigen::Vector4f;
            (*lights)[k]->getWorldLocation(lightPos);
            int hit = 0;
            Ray *lightRay = new Ray;//ray of light that hits the point
            Eigen::Vector4f r1 = *(ray->direction) * tmin + *(ray->location);
            Eigen::Vector4f r2 = (*lightPos) - r1;
            r1 = r1 + r2*.01;
            lightRay->location = &r1;
            lightRay->direction = &r2;
            for(int j = 0; j<objects->size(); j++){
                if((*objects)[j]->timeOfIntersection(lightRay)>0)
                {
                    hit = 1;
                }
            }
            if(!hit){
                shade(ray,tmin,color,lightPos,o);
            }
            
            //Reflect ray and repeat
            Eigen::Vector4f bounce;
            Eigen::Vector4f location = (*(ray->location) + (*(ray->direction)*(tmin)));
            
            Eigen::Vector4f* normal = new Eigen::Vector4f;
            o->getNormalToWorldPoint(&location,normal);
            *normal = *normal/(normal->norm());
            
            bounce = (*ray->direction)/(ray->direction->norm());
            bounce = bounce-2*(bounce.dot(*normal)*(*normal));
            
            
            location = location + bounce*.01f;
            Ray *bRay = new Ray;
            bRay->direction = &bounce;
            bRay->location = &location;
            
            trace(bRay, depth+1, color, objects, lights);
            delete bRay;
            delete normal;

            delete lightRay;
            delete lightPos;
        }
        color->r += .2f;
        color->g += 0;
        color->b += 0;
    }
    else{
        color->r += 0;
        color->g += 0;
        color->b += 0;
    }
    color->r *= pow(.8f,depth);
    
    //set the max color to be 1
    if(color->r>1)
        color->r = 1;
    if(color->g>1)
        color->g = 1;
    if(color->b>1)
        color->b = 1;
    color->g = color->r;
    color->b = color->r;
}
#endif
