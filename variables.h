/*
    Global variables to be used by raytracer
*/
#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <FreeImage.h>

using namespace std;
using namespace glm;

#ifndef VARIABLES_FILE_H
#define VARIABLES_FILE_H
int WIDTH, HEIGHT, DEPTH;
std::string FILENAME;
vec3 CAMLOOKFROM, CAMLOOKAT, CAMUP;
float FOVY;

class trnsfrm
{
public:
};

class geo
{
    vec3 diffuse;
    vec3 specular;
    vec3 shiny;
    vec3 emission;
    trnsfrm t;

public:
    geo() {}
    geo(vec3 d, vec3 spc, vec3 shy, vec3 e)
    {
        diffuse = d;
        specular = spc;
        shiny = shy;
        emission = e;
    }
    virtual string type() { return "default"; }
};

class sphere : geo
{
public:
    float x, y, z, rad;
    sphere(float _x, float _y, float _z, float r)
    {
        x = _x;
        y = _y;
        z = _z;
        rad = r;
    };
    string type() { return "sphere"; }
};
int maxSpheres = 10;
int numSpheres;
sphere **SPHERES;

int MAXVERTS;
glm::vec3 **VERTS;

int currentVertex;

class tri
{
public:
    int v1, v2, v3; // store index of vert ezpz
    tri(int _1, int _2, int _3)
    {
        v1 = _1;
        v2 = _2;
        v3 = _3;
    }
    void calculateNormal()
    {
        cout << "calculate normal" << endl;
    }
};

int maxTris = 10;
int numTris;
tri **TRIS;

#endif