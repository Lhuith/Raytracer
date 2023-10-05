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
#define PI 3.1415926535897931159979634685442

int WIDTH, HEIGHT, DEPTH;
std::string FILENAME;
vec3 CAMLOOKFROM, CAMLOOKAT, CAMUP;
float FOVY;

class material
{
public:
    vec3 diffuse;
    vec3 specular;
    vec3 emission;
    vec3 ambient;
    float shiny;
};

class ray
{
public:
    vec3 o;
    vec3 d;
    ray(vec3 _o, vec3 _d)
    {
        o = _o;
        d = _d;
    }
};

class obj
{
    mat4 transform;
    material mat;

public:
    obj() {}
    virtual string type() { return "default"; }
    virtual bool intersecting(const ray &r, float *dist_to_ray) { return false; }
};

bool solveQuad(const float &a, const float &b, const float &c, float &x0, float &x1)
{
    float discr = b * b - 4 * a * c;
    if (discr < 0)
        return false;
    else if (discr == 0)
        x0 = x1 = -0.5 * b / a;
    else
    {
        float q = (b > 0) ? -0.5 * (b + sqrt(discr)) : -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }

    if (x0 > x1)
    {
        std::swap(x0, x1);
    }
    return true;
}

class sphere : obj
{
public:
    vec3 c;
    float rad;
    sphere(float _x, float _y, float _z, float r)
    {
        c = vec3(_x, _y, _z);
        rad = r;
    };
    string type() { return "sphere"; }
    bool intersecting(const ray &r, float *dist_to_ray)
    {
        float t0, t1;

        vec3 L = r.o - c;
        float a = glm::dot(r.d, r.d);
        float b = 2 * glm::dot(r.d, L);
        float c = glm::dot(L, L) - glm::pow(rad, 2);
        if (!solveQuad(a, b, c, t0, t1))
        {
            return false;
        }

        if (t0 > t1)
        {
            std::swap(t0, t1);
        }

        if (t0 < 0)
        {
            t0 = t1;
            if (t0 < 0)
                return false;
        }

        return true;
    }
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
    int v1, v2, v3; // store index of vert
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
    bool intersecting(const ray &r, float *dist_to_ray)
    {
        // do the math
        return false;
    }
    string type() { return "sphere"; }
};

int maxTris = 10;
int numTris;
tri **TRIS;

#endif