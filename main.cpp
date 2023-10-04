#include <iostream>
#include <string>
#include "variables.h" // global variables
#include "read.cpp"

#include <glm/glm.hpp>

using namespace std;

void init()
{
    WIDTH = 0;
    HEIGHT = 0;
    DEPTH = 5;
    SPHERES = new sphere *[maxSpheres];

    // vertex array is created/sorted on read in
    TRIS = new tri *[maxTris];
}
  int screenshot()
{
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

    return error;
}