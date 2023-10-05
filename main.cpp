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

void GenerateImage(string type, BYTE *pixels)
{
    std::cout << "Saving screenshot: " << FILENAME + type << "\n";
    FreeImage_Save(
        FIF_PNG,
        FreeImage_ConvertFromRawBits(
            pixels, WIDTH, HEIGHT, WIDTH * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false),
        (FILENAME + type).c_str(), 0);
    delete[] pixels;
}
// GL_BGR
void SetPixel(BYTE *pixels, int i, int r, int g, int b)
{
    pixels[i + 0] = b;
    pixels[i + 1] = g;
    pixels[i + 2] = r;
}

void init()
{
    WIDTH = 0;
    HEIGHT = 0;
    DEPTH = 5;
    SPHERES = new sphere *[maxSpheres];

    // vertex array is created/sorted on read in
    TRIS = new tri *[maxTris];
}

int main()
{
    init();
    cout << "Eugene Martens RayTracer 2" << endl;
    int error = ReadCommands("test.txt");
    cout << "height: " << HEIGHT << ", width: " << WIDTH << ", depth: " << DEPTH << endl;
    cout << "fileName: " << FILENAME << endl;
    cout << "camera: "
         << "from <" << CAMLOOKFROM.x << ", " << CAMLOOKFROM.y << ", " << CAMLOOKFROM.z << "> "
         << "at <" << CAMLOOKAT.x << ", " << CAMLOOKAT.y << ", " << CAMLOOKAT.z << "> "
         << "up <" << CAMUP.x << ", " << CAMUP.y << ", " << CAMUP.z << "> "
         << "fovy " << FOVY << endl;

    for (int i = 0; i < numSpheres; i++)
    {
        cout << "sphere_" << i
             << " <" << SPHERES[i]->c.x << "," << SPHERES[i]->c.y << "," << SPHERES[i]->c.z << ","
             << ">, rad: " << SPHERES[i]->rad << endl;
    }

    for (int i = 0; i < MAXVERTS; i++)
    {
        cout << "vertex_" << i
             << " <" << VERTS[i]->x << "," << VERTS[i]->y << "," << VERTS[i]->z << ">" << endl;
    }

    for (int i = 0; i < numTris; i++)
    {
        cout << "tri_" << i
             << " <" << TRIS[i]->v1 << "," << TRIS[i]->v2 << "," << TRIS[i]->v3 << ">" << endl;
    }

    int pix = WIDTH * HEIGHT;
    BYTE *pixels = new BYTE[pix * 3];
    float __a = 0;
    float __b;
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            // for each pixel
            int pixel = 3 * ((HEIGHT - i - 1) * WIDTH + j);
            // trace primary eye ray, find intersection
            { // ray calculation
                vec3 w = glm::normalize(CAMLOOKFROM - CAMLOOKAT);
                vec3 u = glm::normalize(glm::cross(CAMUP, w));
                vec3 v = glm::cross(w, u);

                float aspect = WIDTH / HEIGHT;
                float fovy = FOVY * PI / 180.0;

                float tan_fovx = tan(fovy / 2.0) * aspect;
                float _a = tan_fovx * (j - WIDTH / 2.0) / (WIDTH / 2.0);
                float _b = tan(fovy / 2.0) * (HEIGHT / 2.0 - i) / (HEIGHT / 2.0);

                // cout << "alpha: " << _a << ", ";
                // cout << "beta: " << _b << ", ";

                ray r = ray(CAMLOOKFROM, glm::normalize(_a * u + _b * v - w));

                float dist = INFINITY;
                if (SPHERES[0]->intersecting(r, &dist))
                {
                    SetPixel(pixels, pixel, 255, 0, 100);
                }
                // if (pixel % 41 == 0)
                // {

                // }

                // cout << "ray: "
                //      << "<" << r.d.x << "," << r.d.y << "," << r.d.z << ">" << endl;
            }
            { // color time
              // V, visibility / intersections
            }
        }
    }
    cout << pix * 3 << endl;
    GenerateImage(".png", pixels);

    if (error == 2)
    {
        cout << "error reading information" << endl;
    }

    return error;
}