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
    int w = WIDTH;
    int h = HEIGHT;
    string fName = FILENAME + type;

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

    int pix = WIDTH * HEIGHT;
    BYTE *pixels = new BYTE[pix * 3];

    for (int i = 0; i < WIDTH; i++)
    {
        for (int j = 0; j < HEIGHT; j++)
        {
            int index = ((i * HEIGHT) + j) * 3;
            pixels[index + 0] = 255; // BLUE
            pixels[index + 1] = 125; // GREEN
            pixels[index + 2] = 255; // RED
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