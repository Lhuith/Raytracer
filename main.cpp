#include <iostream>
#include <string>
#include "variables.h" // global variables
#include "read.cpp"
#include <cstddef>

#include <glm/glm.hpp>
#include <FreeImage.h>

#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <ctype.h>

using namespace std;

#define IMAGE_LOCATION "images/"

void generate_image(BYTE *pixels)
{
    std::cout << "Saving screenshot: " << FILENAME + ".png"
              << "\n";
    FreeImage_Save(
        FIF_PNG,
        FreeImage_ConvertFromRawBits(
            pixels, WIDTH, HEIGHT, WIDTH * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false),
        (IMAGE_LOCATION + FILENAME + ".png").c_str(), 0);
    delete[] pixels;
}
// GL_BGR
void SetPixel(BYTE *pixels, int i, int r, int g, int b)
{
    pixels[i + 0] = b * 255;
    pixels[i + 1] = g * 255;
    pixels[i + 2] = r * 255;
}
void SetPixel(BYTE *pixels, int i, vec3 c)
{
    pixels[i + 0] = c.b * 255;
    pixels[i + 1] = c.g * 255;
    pixels[i + 2] = c.r * 255;
}

void init()
{
    WIDTH = 0;
    HEIGHT = 0;
    DEPTH = 5;

    OBJS = new obj *[MAX_OBJS];
    LIGHTS = new light *[MAX_LIGHTS];
}

void ReadOut()
{
    cout << "camera: "
         << "from <" << CAMLOOKFROM.x << ", " << CAMLOOKFROM.y << ", " << CAMLOOKFROM.z << "> "
         << "at <" << CAMLOOKAT.x << ", " << CAMLOOKAT.y << ", " << CAMLOOKAT.z << "> "
         << "up <" << CAMUP.x << ", " << CAMUP.y << ", " << CAMUP.z << "> "
         << "fovy " << FOVY << endl;
}

bool intersecting(ray r, obj &hit_obj, vec3)
{
    return false;
}

int trace(string scene)
{
    numObjs = 0;
    numLights = 0;

    int error = ReadCommands(scene);
    cout << "height: " << HEIGHT << ", width: " << WIDTH << ", depth: " << DEPTH << endl;
    cout << "fileName: " << FILENAME << endl;

    cout << "# of objects: " << numObjs << endl;
    cout << "# of lights: " << numLights << endl;

    int pix = WIDTH * HEIGHT;
    BYTE *pixels = new BYTE[pix * 3];
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            vec3 c = vec3(0, 0, 0);
            // for each pixel
            int pixel = 3 * ((HEIGHT - i - 1) * WIDTH + j);
            // trace primary eye ray, find intersection
            { // ray calculation
                vec3 w = glm::normalize(CAMLOOKFROM - CAMLOOKAT);
                vec3 u = glm::normalize(glm::cross(CAMUP, w));
                vec3 v = glm::cross(w, u);

                float fovy = FOVY * PI / 180.0;

                float tan_fovx = tan(fovy / 2.0) * WIDTH / HEIGHT;

                float b = tan(fovy / 2.0) * (HEIGHT / 2.0 - i) / (HEIGHT / 2.0);
                float a = tan_fovx * (j - WIDTH / 2.0) / (WIDTH / 2.0);

                ray r = ray(CAMLOOKFROM, (a * u + b * v - w));

                float nearest_dist = INFINITY;
                obj *hit_obj = NULL;
                vec3 hit_point;

                // V :
                if (numObjs != 0)
                {
                    for (int i = 0; i < numObjs; i++)
                    {
                        ray t_ray = r.transform_ray(OBJS[i]->inv_tr);
                        float obj_dist;
                        if (OBJS[i]->intersecting(t_ray, &obj_dist))
                        {
                            vec3 hit_trans = t_ray.o + t_ray.d * obj_dist;
                            vec4 hit_extend = OBJS[i]->tr * vec4(hit_trans, 1.0);

                            // dehomogeneous
                            vec3 hit = hit_extend / hit_extend.w;

                            obj_dist = glm::length(hit - r.o);
                            if (obj_dist < nearest_dist)
                            {
                                hit_point = hit;
                                hit_obj = OBJS[i];
                                nearest_dist = obj_dist;
                            }
                        }
                    }
                }
                if (hit_obj == NULL)
                    continue;

                c = hit_obj->mat.ambient + hit_obj->mat.emission;

                for (int i = 0; i < numLights; i++)
                {
                    if (LIGHTS[i]->type == "point")
                    {
                        ray l_ray = ray(LIGHTS[i]->pos, hit_point - LIGHTS[i]->pos);

                        obj *l_hit_obj = NULL;
                        vec3 l_hit;
                        float l_nearest_dist = INFINITY;
                        for (int i = 0; i < numObjs; i++)
                        {
                            ray lt_ray = l_ray.transform_ray(OBJS[i]->inv_tr);
                            float obj_dist;
                            if (OBJS[i]->intersecting(lt_ray, &obj_dist))
                            {
                                vec3 hit_trans = lt_ray.o + lt_ray.d * obj_dist;
                                vec4 hit_extend = OBJS[i]->tr * vec4(hit_trans, 1.0);

                                // dehomogeneous
                                vec3 hit = hit_extend / hit_extend.w;

                                obj_dist = glm::length(hit - r.o);
                                if (obj_dist < l_nearest_dist)
                                {
                                    l_hit = hit;
                                    l_hit_obj = OBJS[i];
                                    l_nearest_dist = obj_dist;
                                }
                            }
                        }

                        if (l_hit_obj != NULL)
                        {
                            if (glm::dot(l_hit - hit_point, l_hit - hit_point) < EPS)
                            {
                                c += LIGHTS[i]->calculate_light(*hit_obj, r, hit_point, ATTEN);
                            }
                        }
                    }
                    else
                    {
                        c += LIGHTS[i]->calculate_light(*hit_obj, r, hit_point, ATTEN);
                    }
                }

                SetPixel(pixels, pixel, c);
            }
        }
    }
    cout << pix * 3 << endl;
    generate_image(pixels);

    if (error == 2)
    {
        cout << "error reading information" << endl;
    }
    return error;
}

string scenes[] =
    {
        "scene1-camera1",
        // "scene1-camera2",
        // "scene1-camera3",
        // "scene1-camera4",
        // "scene2-camera1",
        // "scene2-camera2",
        // "scene2-camera3",
        // "scene3",
};

int main(int argc, char *argv[])
{
    init();
    cout << "Eugene Martens RayTracer" << endl;
    trace("scene3");
    // for (string s : scenes)
    // {
    //     cout << s << endl;
    //     trace(s);
    // }
    return 0;
}