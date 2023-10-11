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
#define PI 3.14159
#define EPS std::numeric_limits<float>::epsilon()

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
float SHINY = 0;

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
    ray transform(mat4 t)
    {
        ray t_ray = ray(o, d);

        vec4 o_transform = (t * vec4(o.x, o.y, o.z, 1));
        t_ray.o = vec3(o_transform.x, o_transform.y, o_transform.z);

        vec4 d_transform = glm::normalize(t * vec4(d.x, d.y, d.z, 0));
        t_ray.d = vec3(d_transform.x, d_transform.y, d_transform.z);

        return t_ray;
    }

    ray reflect(vec3 &h, vec3 &n)
    {
        vec3 p = d - 2 * glm::dot(n, d) * n;
        return ray(h + (p * 0.001f), p);
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
    virtual vec3 interpolateNormal(const vec3 &p) { return vec3(0, 1, 0); }
    virtual void print() {}
};

int MAX_OBJS = 10000;
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
    vec3 interpolateNormal(const vec3 &p)
    {
        vec4 t_p = inv_tr * vec4(p, 1.0f);
        vec3 pt = vec3(t_p.x, t_p.y, t_p.z) / t_p.w;

        return vec3(vec4(pt - c, 0.0) * transpose(inv_tr));
    }
};

int MAXVERTS;
glm::vec3 **VERTS;

int currentVertex;

class tri : public obj
{
public:
    vec3 A, B, C, n; // store index of vert
    vec3 na, nb, nc;
    tri(int _1, int _2, int _3)
    {
        A = *VERTS[_1];
        B = *VERTS[_2];
        C = *VERTS[_3];
        na = nb = nc = n = glm::normalize(glm::cross(B - A, C - A));
    }
    void normals(int n1, int n2, int n3)
    {
    }
    // alpha, beta, gamma
    vec3 barycentric(vec3 p)
    {
        // compute barycentric coordinates (u, w, v | alpha, beta, gamma)
        // https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
        // Thanks John Calsbeek
        vec3 v0 = B - A;
        vec3 v1 = C - A;
        vec3 v2 = p - A;

        float d00 = dot(v0, v0);
        float d01 = dot(v0, v1);
        float d11 = dot(v1, v1);
        float d20 = dot(v2, v0);
        float d21 = dot(v2, v1);

        float denm = d00 * d11 - d01 * d01;
        float beta = (d11 * d20 - d01 * d21) / denm;
        float gamma = (d00 * d21 - d01 * d20) / denm;
        float alpha = 1.0 - beta - gamma;
        return vec3(alpha, beta, gamma);
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

        vec3 bary = barycentric(P);

        if (bary.x > -EPS && bary.x < 1.0 + EPS &&
            bary.y > -EPS && bary.y < 1.0 + EPS &&
            bary.z > -EPS && bary.z < 1.0 + EPS)
        {
            *dist_to_ray = t;
            return true;
        }
        return false;
    }
    string type() { return "tri"; }
    vec3 interpolateNormal(const vec3 &p)
    {
        vec3 t_p = inv_tr * vec4(p, 1.0);
        vec3 bary = barycentric(t_p);

        return vec3(
            transpose(inv_tr) * vec4((na * bary.x) + (nb * bary.y) + (nc * bary.z), 0.0f));
    }
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
    vec3 calculate_light(obj &o, ray &r, vec3 &hp, const vec3 &atten)
    {
        vec3 l;
        if (type == "directional")
        {
            l = normalize(pos);
        }
        else if (type == "point")
        {
            l = normalize(pos - hp);
        }

        vec3 n = normalize(o.interpolateNormal(hp));
        float n_l = glm::max(dot(n, l), 0.0f);
        vec3 d = o.mat.diffuse * col * n_l;

        vec3 halfv = normalize(l + normalize(-r.d));
        float n_h = glm::max(dot(n, halfv), 0.0f);
        vec3 s = o.mat.specular * col * pow(n_h, o.mat.shiny);

        vec3 light_c = d + s;
        if (type == "point")
        {
            float r = length(pos - hp);
            light_c *= (1.0f / (ATTEN.z * r * r + ATTEN.y * r + ATTEN.x));
        }
        return light_c;
    }
};

int MAX_LIGHTS = 10;
int numLights;
light **LIGHTS;

#endif