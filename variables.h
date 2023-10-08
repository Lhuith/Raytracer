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
vec3 AMBIENT = vec3(.2, .2, .2);
float SHINY;

vec3 ATTEN = vec3(1, 0, 0);
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
        vec4 o_tran = vec4(o, 1) * t;
        vec4 d_tran = vec4(d, 0) * t;

        return ray(
            vec3(o_tran.x, o_tran.y, o_tran.z) / o_tran.w,
            vec3(d_tran.x, d_tran.y, d_tran.z));
    }
};

class obj
{
public:
    mat4 tr, inv_tr;
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

int MAX_OBJS = 100;
int numObjs;
obj **OBJS;

class sphere : public obj
{
public:
    vec3 c;
    float r;
    sphere(float _x, float _y, float _z, float _r) : obj()
    {
        c = vec3(_x, _y, _z);
        r = _r;
    };
    string type() { return "sphere"; }

    // https : // www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection.html
    bool intersecting(const ray &ray, float *dist_to_ray)
    {
        // t^2 * (dir⋅dir) + 2t*dir ⋅ (origin - centre) + (origin - centre)⋅(origin - centre) - r^2 = 0
        float t0, t1;

        vec3 L = ray.o - c;

        float a = dot(ray.d, ray.d);
        float b = 2 * dot(ray.d, L);
        float c = dot(L, L) - pow(r, 2);

        // quadratic solve
        float disc = b * b - 4 * a * c;
        if (disc < 0)
            return false;
        else if (disc == 0)
            t0 = t1 = -0.5 * b / a;
        else
        {
            float q = (b > 0) ? -0.5 * (b + sqrt(disc)) : -0.5 * (b - sqrt(disc));
            t0 = q / a;
            t1 = c / q;
        }
        if (t0 > t1)
            swap(t0, t1);

        if (t0 < 0)
        {
            t0 = t1;
            if (t0 < 0)
                return false;
        }
        *dist_to_ray = t0;

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
        A = *VERTS[_1];
        B = *VERTS[_2];
        C = *VERTS[_3];
        n = glm::normalize(glm::cross(B - A, C - A));
    }
    bool intersecting(const ray &r, float *dist_to_ray)
    {
        if (abs(glm::dot(r.d, n)) == 0)
        {
            return false;
        }

        // t, length, reduced to fall into place space
        float t = (glm::dot(A, n) - glm::dot(r.o, n)) / glm::dot(r.d, n);
        vec3 P = r.o + (r.d * t);

        if (t < 1e-2)
        {
            return false;
        }

        // compute barycentric coordinates (u, w, v | alpha, beta, gamma)
        // https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
        // Thanks John Calsbeek
        vec3 v0 = B - A;
        vec3 v1 = C - A;
        vec3 v2 = P - A;

        float d00 = dot(v0, v0);
        float d01 = dot(v0, v1);
        float d11 = dot(v1, v1);
        float d20 = dot(v2, v0);
        float d21 = dot(v2, v1);

        float denm = d00 * d11 - d01 * d01;
        float v = (d11 * d20 - d01 * d21) / denm;
        float w = (d00 * d21 - d01 * d20) / denm;
        float u = 1.0 - v - w;

        if (v > -EPS && v < 1.0 + EPS &&
            w > -EPS && w < 1.0 + EPS &&
            u > -EPS && u < 1.0 + EPS)
        {
            *dist_to_ray = t;
            return true;
        }
        return false;
    }
    string type() { return "tri"; }
    void print() {}
};

class light
{
public:
    vec3 pos;
    vec3 col;
    string type;
    light(vec3 p, vec3 c, string t)
    {
        pos = p;
        col = c;
        type = t;
    }
    vec3 calculate_light(obj &o, ray &r, vec3 &hp, const float *atten)
    {
        return vec3(1.0, 1.0, 1.0);
    }
};

int MAX_LIGHTS = 10;
int numLights;
light **LIGHTS;

#endif