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
#include <fstream>
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

#include <omp.h>

using namespace std;

FIBITMAP * bitmap;
vector<Object*> *objects;
vector<Object*> *lights;
float resolution = 1.0f;
int depth = 9;
Eigen::Matrix4f camera2World;
char *outputPath = "test.png";


void addSphere(float x, float y, float z, float r, float ka, float kd, float ks, float krr, float krg, float krb){
    Eigen::Matrix4f *o2w = new Eigen::Matrix4f;
    Eigen::Matrix4f translate, scale, rotate;
    translate << 1,0,0,x,
    0,1,0,y,
    0,0,1,z,
    0,0,0,1;
    scale << r/2.0f,0,0,0,
    0,r/2.0f,0,0,
    0,0,r/1.0f,0,
    0,0,0,1;
    float theta = 1.0f;
    rotate << 1,0,0,0,
    0,cos(theta),-sin(theta),0,
    0,sin(theta),cos(theta),0,
    0,0,0,1;
    
    
    *o2w = translate*rotate*scale;
    Object *o = new Sphere();
    o->o2w = o2w;
    o->brdf.krs = ks;
    o->brdf.krd = kd;
    o->brdf.kra = ka;
    o->brdf.krr = krr;
    
    o->brdf.kgs = ks;
    o->brdf.kgd = kd;
    o->brdf.kga = ka;
    o->brdf.kgr = krg;
    
    o->brdf.kbs = ks;
    o->brdf.kbd = kd;
    o->brdf.kba = ka;
    o->brdf.kbr = krb;
    
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






void addPointLight(float x, float y, float z, float r, float g, float b){
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
    o->r = r;
    o->g = g;
    o->b = b;
    lights->push_back(o);
}
void addDirectionalLight(float x, float y, float z, float r, float g, float b){
    Eigen::Matrix4f *o2w = new Eigen::Matrix4f;
    Eigen::Matrix4f translate;
    Eigen::Matrix4f scale;
    translate << 1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,0,1;
    scale << 1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,0,1;
    
    *o2w = translate*scale;
    
    
    Eigen::Vector4f *direction = new Eigen::Vector4f;
    *direction << x,y,z,0;
    Light *o = new Light();
    o->o2w = o2w;
    o->r = r;
    o->g = g;
    o->b = b;
    o->direction = direction;
    lights->push_back(o);
}
void addSphere(float x, float y, float z, float radx, float rady, float radz, float rx, float ry, float rz, BRDF *brdf){
    Eigen::Matrix4f *o2w = new Eigen::Matrix4f;
    Eigen::Matrix4f translate, scale, rotatex,rotatey,rotatez;
    translate << 1,0,0,x,
    0,1,0,y,
    0,0,1,z,
    0,0,0,1;
    scale << radx,0,0,0,
    0,rady,0,0,
    0,0,radz,0,
    0,0,0,1;
    rotatex <<  1,0,0,0,
    0,cos(rx),-sin(rx),0,
    0,sin(rx),cos(rx),0,
    0,0,0,1;
    rotatey <<  cos(ry),0,sin(ry),0,
    0,1,0,0,
    -sin(ry),0,cos(ry),0,
    0,0,0,1;
    rotatez <<  cos(rz),-sin(rz),0,0,
    sin(rz),cos(rz),0,0,
    0,0,1,0,
    0,0,0,1;
    
    *o2w = translate*rotatez*rotatey*rotatex*scale;
    Object *o = new Sphere();
    o->o2w = o2w;
    o->brdf = *brdf;
    
    objects->push_back(o);
}
void addTriangle(float x, float y, float z, float width, float rx, float ry, float rz, BRDF *brdf){
    Eigen::Matrix4f *o2w = new Eigen::Matrix4f;
    Eigen::Matrix4f translate, scale, rotatex,rotatey,rotatez;
    translate << 1,0,0,x,
    0,1,0,y,
    0,0,1,z,
    0,0,0,1;
    scale << width,0,0,0,
    0,width,0,0,
    0,0,width,0,
    0,0,0,1;
    rotatex <<  1,0,0,0,
    0,cos(rx),-sin(rx),0,
    0,sin(rx),cos(rx),0,
    0,0,0,1;
    rotatey <<  cos(ry),0,sin(ry),0,
    0,1,0,0,
    -sin(ry),0,cos(ry),0,
    0,0,0,1;
    rotatez <<  cos(rz),-sin(rz),0,0,
    sin(rz),cos(rz),0,0,
    0,0,1,0,
    0,0,0,1;
    
    
    *o2w = translate*rotatez*rotatey*rotatex*scale;
    Object *o = new Triangle();
    o->o2w = o2w;
    o->brdf = *brdf;
    
    objects->push_back(o);
}

void addTriangle(Eigen::Vector4f p1, Eigen::Vector4f p2, Eigen::Vector4f p3){
    Eigen::Matrix4f *o2w = new Eigen::Matrix4f;
    *o2w << (p2-p1).x(),(p3-p1).x(),0,0,
            (p2-p1).y(),(p3-p1).y(),0,0,
            (p2-p1).z(),(p3-p1).z(),1,0,
            0,0,0,1;
    Eigen::Matrix4f translate;
    translate << 1,0,0,0,
    0,1,0,0,
    0,0,1,-3,
    0,0,0,1;
    Triangle *o = new Triangle();
    *o2w = translate*(*o2w);
    o->o2w = o2w;
    
    
    BRDF brdf;
    
    brdf.kra = .05;
    brdf.kga = .05;
    brdf.kba = .05;
    
    brdf.krd = .2;
    brdf.kgd = .2;
    brdf.kbd = .2;
    
    brdf.krs = .8;
    brdf.kgs = .8;
    brdf.kbs = .8;
    
    brdf.krr = 0;
    brdf.kgr = 0;
    brdf.kbr = 0;
    
    brdf.sp = 100;
    
    o->brdf = brdf;
    
    objects->push_back(o);
}














void initScene(){    
   /* addPointLight(0,-1,-2);
    addPointLight(0,-5,-2.2f);
    addPointLight(0,5,-2.2f);
    addSphere(0,0,-3,.4f,0,0,0,1.0f,0,0);
    addSphere(1,1,-3,.4f,0,0,0,1.0f,0,1.0f);
    addSphere(1,0,-3,.4f,0,0,0,1.0f,1.0f,0);
    addSphere(0,1,-3,.4f,0,0,0,1.0f,1.0f,1.0f);
    addSphere(0,-1,-3,.4f,0,0,0,.0f,1.0f,0);
    addSphere(-1,0,-3,.4f,0,0,0,.0f,1.0f,1.0f);
    addSphere(-1,-1,-3,.4f,0,0,0,.0f,0,1.0);
    addSphere(1,-1,-3,.4f,0,0,0,1.0f,.5f,.5f);
    addSphere(-1,1,-3,.4f,0,0,0,1.0f,0,.5f);
    addTriangle(-3.4f,1);
    addTriangle(-1,-1);*/
}

int setPixelColor(int x, int y, Color *color){
    RGBQUAD *c = new RGBQUAD;
    c->rgbRed = 255*color->r;
    c->rgbGreen = 255*color->g;
    c->rgbBlue = 255*color->b;
    FreeImage_SetPixelColor(bitmap,x,y,c);
    delete c;
}
void parseOption(char* item, int i, char *argv[]){
    if(strcmp(item,"-resolution")==0)// -resolution value
    {
        resolution = atof(argv[i+1])/500.0f;
    }
    if(strcmp(item,"-o")==0)// -resolution value
    {
        outputPath = argv[i+1];
    }
    if(strcmp(item,"-depth")==0)// -depth value
    {
        depth = atoi(argv[i+1]);
    }
    if(strcmp(item,"-pl")==0)// -pl x y z r g b
    {
        addPointLight(atof(argv[i+1]),atof(argv[i+2]),atof(argv[i+3]),atof(argv[i+4]),atof(argv[i+5]),atof(argv[i+6]));
    }
    if(strcmp(item,"-dl")==0)// -pl x y z r g b
    {
        addDirectionalLight(atof(argv[i+1]),atof(argv[i+2]),atof(argv[i+3]),atof(argv[i+4]),atof(argv[i+5]),atof(argv[i+6]));
    }
    else if(strcmp(item,"-sphere")==0)// -sphere x y z rx ry rz rotatex rotatey rotatez  kra kga kba   krd kgd kbd   krs kgs kbs   krr kgr kbr   sp
    {
        BRDF brdf;
        brdf.kra = atof(argv[i+10]);
        brdf.kga = atof(argv[i+11]);
        brdf.kba = atof(argv[i+12]);
        
        brdf.krd = atof(argv[i+13]);
        brdf.kgd = atof(argv[i+14]);
        brdf.kbd = atof(argv[i+15]);
        
        brdf.krs = atof(argv[i+16]);
        brdf.kgs = atof(argv[i+17]);
        brdf.kbs = atof(argv[i+18]);
        
        brdf.krr = atof(argv[i+19]);
        brdf.kgr = atof(argv[i+20]);
        brdf.kbr = atof(argv[i+21]);
        
        brdf.sp = atof(argv[i+22]);
        addSphere(atof(argv[i+1]),atof(argv[i+2]),atof(argv[i+3]),atof(argv[i+4]),atof(argv[i+5]),atof(argv[i+6]),atof(argv[i+7]),atof(argv[i+8]),atof(argv[i+9]),&brdf);
    }
    else if(strcmp(item,"-triangle")==0)// -triangle x y z rotatex rotatey rotatez width  kra kga kba   krd kgd kbd   krs kgs kbs   krr kgr kbr   sp
    {
        BRDF brdf;
        brdf.kra = atof(argv[i+8]);
        brdf.kga = atof(argv[i+9]);
        brdf.kba = atof(argv[i+10]);
        
        brdf.krd = atof(argv[i+11]);
        brdf.kgd = atof(argv[i+12]);
        brdf.kbd = atof(argv[i+13]);
        
        brdf.krs = atof(argv[i+14]);
        brdf.kgs = atof(argv[i+15]);
        brdf.kbs = atof(argv[i+16]);
        
        brdf.krr = atof(argv[i+17]);
        brdf.kgr = atof(argv[i+18]);
        brdf.kbr = atof(argv[i+19]);
        
        brdf.sp = atof(argv[i+20]);
        addTriangle(atof(argv[i+1]),atof(argv[i+2]),atof(argv[i+3]),atof(argv[i+4]),atof(argv[i+5]),atof(argv[i+6]),atof(argv[i+7]),&brdf);
    }
    else if(strcmp(item,"-camera")==0)// -camera x y z rotatex rotatey rotatez
    {
        Eigen::Matrix4f translate,rotatex,rotatey,rotatez;
        translate <<1,0,0,atof(argv[i+1]),
        0,1,0,atof(argv[i+2]),
        0,0,1,atof(argv[i+3]),
        0,0,0,1;
        rotatex <<  1,0,0,0,
        0,cos(atof(argv[i+4])),-sin(atof(argv[i+4])),0,
        0,sin(atof(argv[i+4])),cos(atof(argv[i+4])),0,
        0,0,0,1;
        rotatey <<  cos(atof(argv[i+5])),0,sin(atof(argv[i+5])),0,
        0,1,0,0,
        -sin(atof(argv[i+5])),0,cos(atof(argv[i+5])),0,
        0,0,0,1;
        rotatez <<  cos(atof(argv[i+6])),-sin(atof(argv[i+6])),0,0,
        sin(atof(argv[i+6])),cos(atof(argv[i+6])),0,0,
        0,0,1,0,
        0,0,0,1;
        camera2World = translate*rotatez*rotatey*rotatex;
    }
    else if(strcmp(item,"-batch")==0)
    {
        ifstream file;
        file.open(argv[i+1]);
        int count = 0;
        char output[20];
        char *first;
        char *args[100];

        if(file.is_open()){
            args[count] = (char*)malloc(20);
            file >> args[count];
            while(!file.eof()){
                //args[count] = output;
             //   cout << output << endl;
                if(strcmp(args[count],"\\n")==0)
                {
                  //  cout << args[0] << endl;
                    parseOption(args[0],0,args);
                    count = 0;
                    args[count] = (char*)malloc(20);
                    file >> args[count];
                }
                else{
                    count+=1;
                    args[count] = (char*)malloc(20);
                    file >> args[count];
                }
            }
        }
        file.close();
    }
            /*else if(strcmp(item,"-obj")==0){
             ifstream file;
             vector<Eigen::Vector4f> points;
             file.open(argv[i+1]);
             char output[100];
             float x,y,z;
             if(file.is_open()){
             while(!file.eof()){
             file >> output;
             
             if(strcmp(output,"v")==0)
             {
             x = y = z = 0;
             file >> output;
             x = atof(output);
             file >> output;
             y = atof(output);
             file >> output;
             z = atof(output);
             
             Eigen::Vector4f point;
             point << x,y,z,1;
             points.push_back(point);
             
             }
             else if(strcmp(output,"f")==0)
             {
             
             x = y = z = 0;
             file >> output;
             x = atof(output)-1;
             file >> output;
             y = atof(output)-1;
             file >> output;
             z = atof(output)-1;
             
             Eigen::Vector4f p1,p2,p3;
             p1 = points[(int)x];
             p2 = points[(int)y];
             p3 = points[(int)z];
             //cout << p1 << p2 << p3 << endl;
             addTriangle(p1,p2,p3);                        
             }        
             }
             }
             file.close();
             }*/

}
int main(int argc, char *argv[]){    
    if(argc==1 || strcmp(argv[1],"--help")==0 || strcmp(argv[1],"-h")==0)
    {
        printf("Usage: ");
        printf("\n\t ./Scene [option]");
        printf("\n\t Where options include:");
        printf("\n\t\t -sphere \t x y z rx ry rz \t rotatex rotatey rotatez \t kra kga kba   krd kgd kbd   krs kgs kbs   krr kgr kbr   sp");
        printf("\n\t\t -triangle \t x y z width \t\t rotatex rotatey rotatez \t kra kga kba   krd kgd kbd   krs kgs kbs   krr kgr kbr   sp");
        printf("\n\t\t -camera \t x y z \t\t\t rotatex rotatey rotatez");
        printf("\n\t\t -pl \t\t x y z \t\t\t r g b");
        printf("\n\t\t -dl \t\t x y z \t\t\t r g b");
        printf("\n\t\t -resolution imageSize (500 = 500px*500px image)");
        printf("\n\t\t -batch batchfile (file formatted with each option on a different line, and each line ending in \"\\n\")");
        printf("\n\t\t -depth value");
        printf("\n\t\t -o outputPath\n");
        return 0;
    }
    
    
    objects = new vector<Object*>;
    lights = new vector<Object*>;

    //initScene();
    
    //set up camera to world transformation
    camera2World << 1,0,0,0,
                    0,1,0,0,
                    0,0,1,0,
                    0,0,0,1;
    
    for(int i = 0; i<argc; i++){
        char *item = argv[i];
        parseOption(item,i,argv);
    }

    
    int screenWidth = 500, screenHeight = 500;
    
    FreeImage_Initialise();
    bitmap = FreeImage_Allocate(screenWidth*resolution,screenHeight*resolution,8*3);

    
    Camera *c = new Camera(&camera2World);
        
    Ray *r = new Ray;
    RayTracer *rt = new RayTracer;
    rt->depthLimit = depth;
    Color *color = new Color(0,0,0);

    float percentComplete = 0;
    float prev = 0;
    printf("\rPercent complete: %d%%", (int)(percentComplete*100));
    fflush(stdout);
#pragma omp parallel for      \  
      default(shared) private(i)
    for(float i = 0; i<screenWidth*resolution; i+=1.0f){
        for (float j = 0; j<screenHeight*resolution; j+=1.0f) {
            c->getRayForWorldPosition((screenWidth/2.0f-i/resolution)/(float)screenWidth,(screenHeight/2.0f-j/resolution)/(float)screenHeight,r);
            rt->trace(r,0,color,objects, lights);
            setPixelColor((screenWidth*resolution-i),(screenHeight*resolution-j),color);
        }
        
        
        percentComplete = (float)i/(screenWidth*resolution);
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
    
    FreeImage_Save(FIF_PNG,bitmap,outputPath,0);
    FreeImage_DeInitialise();
}

#endif
