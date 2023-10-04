#include <iostream>
#include <string>
#include "variables.h" // global variables
#include "read.cpp"
#include <cstddef>

#include <glm/glm.hpp>
#include <FreeImage.h>

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <ctype.h>

using namespace std;

void screenshot(string type)
{
    int w = WIDTH;
    int h = HEIGHT;
    string fName = FILENAME + type;

    int pix = w * h;
    BYTE *pixels = new BYTE[pix * 3];

    for (int i = 0; i < pix; i += 3)
    {
        int index = i;
        pixels[index + 0] = 0;   // B
        pixels[index + 1] = 0;   // G
        pixels[index + 2] = 255; // R
        cout << pixels[i] << endl;
    }

    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, w, h, GL_BGR, GL_UNSIGNED_BYTE, pixels);

    FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, w, h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);

    std::cout << "Saving screenshot: " << fName << "\n";

    FreeImage_Save(FIF_PNG, img, fName.c_str(), 0);
    delete[] pixels;
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
             << " <" << SPHERES[i]->x << "," << SPHERES[i]->y << "," << SPHERES[i]->z << "," << SPHERES[i]->rad << ">" << endl;
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

    for (int i = 0; i < WIDTH; i++)
    {
        for (int j = 0; j < HEIGHT; j++)
        {
            for (int s = 0; s < maxSpheres; s++)
            {
                // TODO sphere intersection/drawing I think?
            }
            // TODO ray tracing here!
        }
    }

    if (error == 2)
    {
        cout << "error reading information" << endl;
    }
    screenshot(".png");
    return error;
}