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
#define EPS std::numeric_limits<double>::epsilon()

// SCENE
int WIDTH, HEIGHT, DEPTH;
std::string FILENAME;

// CAMERA
vec3 CAMLOOKFROM, CAMLOOKAT, CAMUP;
float FOVY;

// MATERIAL
vec3 EMISSION;
vec3 DIFFUSE;
vec3 SPECULAR;
vec3 AMBIENT;
float SHINY;

class material
{
public:
    vec3 diffuse;
    vec3 specular;
    vec3 emission;
    vec3 ambient;
    float shiny;
    material() {}
    material(vec3 d, vec3 spc, vec3 e, vec3 a, float s)
    {
        diffuse = d;
        specular = spc;
        emission = e;
        ambient = a;
        shiny = s;
    }
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
    ray transform_ray(mat4 t)
    {
        ray t_ray = ray(o, d);

        vec4 o_transform = (t * vec4(o.x, o.y, o.z, 0));
        t_ray.o = vec3(o_transform.x, o_transform.y, o_transform.z);

        vec4 d_transform = glm::normalize(t * vec4(d.x, d.y, d.z, 1));
        t_ray.d = vec3(d_transform.x, d_transform.y, d_transform.z);

        return t_ray;
    }
};

class light
{
};

int MAX_LIGHTS = 10;
int numLights;
light **LIGHTS;

class obj
{
public:
    mat4 transform;
    material mat;
    obj() {}
    virtual string type() { return "default"; }
    virtual bool intersecting(const ray &r, float *dist_to_ray)
    {
        cout << "default intersection" << endl;
        return false;
    }
    virtual void print() {}
};

int MAX_OBJS = 10;
int numObjs;
obj **OBJS;

class sphere : public obj
{
public:
    vec3 c;
    float rad;
    sphere(float _x, float _y, float _z, float r) : obj()
    {
        c = vec3(_x, _y, _z);
        rad = r;
    };
    string type() { return "sphere"; }
    bool intersecting(const ray &r, float *dist_to_ray)
    {
        return true;
    }
};

int MAXVERTS;
glm::vec3 **VERTS;

int currentVertex;

class tri : public obj
{
public:
    vec3 A, B, C, n; // store index of vert

    tri(int _1, int _2, int _3)
    {
        cout << _1 << ", " << _2 << ", " << _3 << endl;
        A = *VERTS[_1];
        B = *VERTS[_2];
        C = *VERTS[_3];
        n = glm::normalize(glm::cross(glm::normalize(B - A), glm::normalize(C - A)));
    }
    bool intersecting(const ray &r, float *dist_to_ray)
    {
        // t, length, reduced to fall into place space
        float t = glm::dot(A, n) - glm::dot(r.o, n) / glm::dot(r.d, n);
        vec3 P = r.o + (r.d * t);

        if (t < 1e-2)
        {
            return false;
        }

        // compute barycentric coordinates (u, w, v | alpha, beta, gamma)
        vec3 v0 = B - A;
        vec3 v1 = C - A;
        vec3 v2 = P - A;

        float d00 = dot(v0, v0);
        float d01 = dot(v0, v1);
        float d11 = dot(v1, v1);
        float d20 = dot(v2, v0);
        float d21 = dot(v2, v1);
        float denm = d00 * d11 - d01 * d01;
        float _alpha = (d11 * d20 - d01 * d21) / denm;
        float _beta = (d00 * d21 - d01 * d20) / denm;
        float _gamma = 1.0f - _alpha - _beta;

        if (_beta > -EPS && _beta < 1.0 + EPS &&
            _gamma > -EPS && _gamma < 1.0 + EPS &&
            _alpha > -EPS && _alpha < 1.0 + EPS)
        {
            return true;
        }
        return false;
    }
    string type() { return "tri"; }
    void print() {}
};

#endif