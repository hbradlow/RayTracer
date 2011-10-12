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
#include "Light.cpp"
#include "DirectionalLight.cpp"

#define SELF_SHADOW_BIAS .01f
using namespace std;

class RayTracer {
public:
    void trace(Ray *ray, int depth, Color* color, vector<Object*>* objects, vector<Object*>* lights);
};

void shade(Ray *ray, float t, Color* color,Light *light, Object *o){
    Eigen::Vector4f *lightPos = new Eigen::Vector4f;
    light->getWorldLocation(lightPos);
    
    Eigen::Vector4f location = (*(ray->location) + (*(ray->direction)*(t)));
    
    
    Eigen::Vector4f lightDir = ((*lightPos)-(*(ray->location) + (*(ray->direction)*t)));
    lightDir = lightDir/(lightDir.norm());
    
    if((light)->direction)
    {
        lightDir = *(light->direction);
    }
    
    Eigen::Vector4f viewDir = *(ray->direction);
    viewDir = viewDir/(viewDir.norm());

    Eigen::Vector4f* normal = new Eigen::Vector4f;
    o->getNormalToWorldPoint(&location,normal);
    *normal = *normal/(normal->norm());
    
    Eigen::Vector4f r;
    r = (lightDir) - 2*(lightDir.dot(*normal))*(*normal);
    r = r/(r.norm());
    
    float kd = o->brdf.krd;
    float ks = o->brdf.krs;
    float ka = o->brdf.kra;
    float sp = o->brdf.sp;
    float dot = max(normal->dot(lightDir),0.0f)*kd*light->r + pow(max(r.dot(viewDir),0.0f),sp)*ks*light->r + ka;
    color->r += dot;
    
    kd = o->brdf.kgd;
    ks = o->brdf.kgs;
    ka = o->brdf.kga;
    dot = max(normal->dot(lightDir),0.0f)*kd*light->g + pow(max(r.dot(viewDir),0.0f),sp)*ks*light->g + ka;
    color->g += dot;
    
    kd = o->brdf.kbd;
    ks = o->brdf.kbs;
    ka = o->brdf.kba;
    dot = max(normal->dot(lightDir),0.0f)*kd*light->b + pow(max(r.dot(viewDir),0.0f),sp)*ks*light->b + ka;
    color->b += dot;
    
    delete normal;
}

void RayTracer::trace(Ray *ray, int depth, Color* color, vector<Object*>* objects, vector<Object*>* lights) {
    if(depth==0){
        color->r = 0;
        color->g = 0;
        color->b = 0;
    }
    if(depth>9){
        return;
    }
    Object* o;
    float tmin = 999999999;
    bool found = false;
    for(int i = 0; i<objects->size(); i++){
        if((*objects)[i]->timeOfIntersection(ray)>0)
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
        //Reflect ray and repeat
        if(o->brdf.krr !=0 || o->brdf.kgr !=0 || o->brdf.kbr !=0){
            Eigen::Vector4f bounce;
            Eigen::Vector4f location = (*(ray->location) + (*(ray->direction)*(tmin)));
            
            Eigen::Vector4f* normal = new Eigen::Vector4f;
            o->getNormalToWorldPoint(&location,normal);
            *normal = *normal/(normal->norm());
            
            bounce = (*ray->direction)/(ray->direction->norm());
            bounce = bounce-2*(bounce.dot(*normal)*(*normal));
            
            
            location = location + bounce*SELF_SHADOW_BIAS;
            Ray *bRay = new Ray;
            bRay->direction = &bounce;
            bRay->location = &location;
            
            trace(bRay, depth+1, color, objects, lights);
            color->r *= pow(o->brdf.krr,depth+1);
            color->g *= pow(o->brdf.kgr,depth+1);
            color->b *= pow(o->brdf.kbr,depth+1);
            
            delete bRay;
            delete normal;
        }
        
        for(int k = 0; k<lights->size(); k++){
            if(((Light*)(*lights)[k])->direction == NULL){
                Eigen::Vector4f *lightPos = new Eigen::Vector4f;
                (*lights)[k]->getWorldLocation(lightPos);
                int hit = 0;
                
                Ray *lightRay = new Ray;//ray of light that hits the point
                Eigen::Vector4f r1 = *(ray->direction) * tmin + *(ray->location);
                Eigen::Vector4f r2 = (*lightPos) - r1;
                r1 = r1 + r2*SELF_SHADOW_BIAS;
                lightRay->location = &r1;
                lightRay->direction = &r2;
                for(int j = 0; j<objects->size(); j++){
                    if((*objects)[j]->timeOfIntersection(lightRay)>0 && (*objects)[j]->timeOfIntersection(lightRay)<1.0f)//preventing shadowing TODO
                    {
                        hit = 1;
                        break;
                    }
                }
                if(!hit){
                    shade(ray,tmin,color,(Light*)(*lights)[k],o);
                }
                else{
                    color->r += o->brdf.kra;
                    color->g += o->brdf.kga;
                    color->b += o->brdf.kba;
                }
                delete lightRay;
                delete lightPos;
            }
            else{
                shade(ray,tmin,color,(Light*)(*lights)[k],o);
            }
                

           
        }
    }
    
    //set the max color to be 1
    if(color->r>1)
        color->r = 1;
    if(color->g>1)
        color->g = 1;
    if(color->b>1)
        color->b = 1;
}
#endif
