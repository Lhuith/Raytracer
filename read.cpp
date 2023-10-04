/* Extrapolation of the read in functionality created by Ravi Ramamoorthi CSE167/homework 2 code*/
#include <iostream>
#include <string>
#include <GLUT/glut.h>
#include <fstream>
#include <sstream>
#include "variables.h"

#include <glm/glm.hpp>

using namespace std;

bool ReadValues(stringstream &s, const int numvals, GLfloat *values)
{
    for (int i = 0; i < numvals; i++)
    {
        s >> values[i];
        if (s.fail())
        {
            cout << "Failed reading value " << i << " will skip\n";
            return false;
        }
    }
    return true;
}

int readfile(const char *filename)
{
    string str, cmd;
    ifstream in;
    in.open(filename);
    if (in.is_open())
    {
        getline(in, str);
        while (in)
        {
            if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#'))
            {
                // Ruled out comment and blank lines
                stringstream s(str);
                s >> cmd;
                int i;
                GLfloat values[10]; // Position and color for light, colors for others
                // Up to 10 params for cameras.
                // bool validinput; // Validity of input

                // size width height
                if (cmd == "size")
                {
                    if (ReadValues(s, 2, values))
                    {
                        WIDTH = values[0];
                        HEIGHT = values[1];
                    }
                }
                // maxdepth depth
                else if (cmd == "maxdepth")
                {
                    if (ReadValues(s, 1, values))
                    {
                        DEPTH = values[0];
                    }
                }
                // output filename
                else if (cmd == "output")
                {
                    string name = "";
                    s >> name;
                    if (!s.fail())
                    {
                        FILENAME = name;
                    }
                    else
                    {
                        cout << "file name grab failed!" << endl;
                    }
                }
                // camera lookfromx lookfromy lookfromz lookatx lookaty lookatz upx upy upz fov
                else if (cmd == "camera" && ReadValues(s, 10, values))
                {
                    CAMLOOKFROM.x = values[0];
                    CAMLOOKFROM.y = values[1];
                    CAMLOOKFROM.z = values[2];

                    CAMLOOKAT.x = values[3];
                    CAMLOOKAT.y = values[4];
                    CAMLOOKAT.z = values[5];

                    CAMUP.x = values[6];
                    CAMUP.y = values[7];
                    CAMUP.z = values[8];

                    FOVY = values[9];
                }
                // object material coloring, need to store objects to array and reference later ... somehow?
                // TODO: diffuse r g b
                else if (cmd == "diffuse" && ReadValues(s, 3, values))
                {
                    cout << "implement diffuse" << endl;
                }
                // TODO: specular r g b
                else if (cmd == "specular" && ReadValues(s, 3, values))
                {
                    cout << "implement specular" << endl;
                }
                // TODO: shininess r g b
                else if (cmd == "shininess" && ReadValues(s, 3, values))
                {
                    cout << "implement shininess" << endl;
                }
                // TODO: emission r g b
                else if (cmd == "emission" && ReadValues(s, 3, values))
                {
                    cout << "implement emission" << endl;
                }
                // sphere x y z radius
                // ++ to objects
                else if (cmd == "sphere" && ReadValues(s, 4, values))
                {
                    // sphere x y z radius
                    SPHERES[numSpheres++] = new sphere(
                        values[0],
                        values[1],
                        values[2],
                        values[3]);
                    // cout << "number of spheres: " << numSpheres << endl;
                }
                // maxverts number
                else if (cmd == "maxverts" && ReadValues(s, 1, values))
                {
                    MAXVERTS = values[0];
                    VERTS = new glm::vec3 *[MAXVERTS];
                    currentVertex = 0;
                }
                // vertex x y z
                else if (cmd == "vertex" && ReadValues(s, 3, values))
                {
                    if (currentVertex == MAXVERTS)
                    {
                        cerr << "vertex overflow" << endl;
                        return 2;
                    }
                    VERTS[currentVertex++] = new glm::vec3(
                        values[0],
                        values[1],
                        values[2]);
                }
                // vertex x y z
                else if (cmd == "vertex" && ReadValues(s, 3, values))
                {
                    if (currentVertex == MAXVERTS)
                    {
                        cerr << "vertex overflow" << endl;
                        return 2;
                    }
                    VERTS[currentVertex++] = new glm::vec3(
                        values[0],
                        values[1],
                        values[2]);
                }
                // tri v1 v2 v3
                else if (cmd == "tri" && ReadValues(s, 3, values))
                {
                    // sphere x y z radius
                    TRIS[numTris++] = new tri(
                        values[0],
                        values[1],
                        values[2]);
                    // cout << "number of spheres: " << numSpheres << endl;
                }
                // maxvertnorms number
                else if (cmd == "maxvertnorms" && ReadValues(s, 1, values))
                {
                    cout << "impliment max vert normals" << endl;
                }
                // vertexnormal x y z nx ny nz
                else if (cmd == "vertexnormal" && ReadValues(s, 1, values))
                {
                    cout << "impliment max vert normals" << endl;
                }
                // trinormal v1 v2 v3
                else if (cmd == "trinormal" && ReadValues(s, 1, values))
                {
                    cout << "impliment max vert normals" << endl;
                }
                // TODO: translate x y z
                else if (cmd == "translate" && ReadValues(s, 3, values))
                {
                    cout << "impliment translate" << endl;
                }
                // TODO: rotate x y z angle
                else if (cmd == "rotate" && ReadValues(s, 4, values))
                {
                    cout << "impliment rotate" << endl;
                }
                // TODO: scale x y z
                else if (cmd == "scale" && ReadValues(s, 3, values))
                {
                    cout << "impliment scale" << endl;
                }
                // TODO: pushTransform
                else if (cmd == "pushTransform" && ReadValues(s, 3, values))
                {
                    cout << "impliment push transform" << endl;
                }
                // TODO: popTransform
                else if (cmd == "popTransform" && ReadValues(s, 3, values))
                {
                    cout << "impliment pop transform" << endl;
                }
                // Lighting Commands
                // TODO: directional x y z r g b
                else if (cmd == "directional" && ReadValues(s, 6, values))
                {
                    cout << "impliment directional light" << endl;
                }
                // TODO: point x y z r g b
                else if (cmd == "point" && ReadValues(s, 6, values))
                {
                    cout << "impliment point light" << endl;
                }
                // TODO: attenuation const linear quadratic
                else if (cmd == "attenuation" && ReadValues(s, 3, values))
                {
                    cout << "impliment attenuation" << endl;
                }
                // TODO: ambient r g b
                else if (cmd == "ambient" && ReadValues(s, 3, values))
                {
                    cout << "impliment ambient" << endl;
                }
                else
                {
                    cerr << "Unknown Command: " << cmd << " Skipping \n";
                }
            }
            getline(in, str);
        }
    }
    else
    {
        cerr << "Unable to Open Input Data File " << filename << "\n";
        return 2;
    }
    return 0;
}

int ReadCommands(const char *filename)
{
    return readfile(filename);
}