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
                    CAMLOOKFROM = vec3(values[0], values[1], values[2]);
                    CAMLOOKAT = vec3(values[3], values[4], values[5]);
                    CAMUP = vec3(values[6], values[7], values[8]);
                    FOVY = values[9];
                }
                // object material coloring, need to store objects to array and reference later ... somehow?
                // diffuse r g b
                else if (cmd == "diffuse" && ReadValues(s, 3, values))
                {
                    DIFFUSE = vec3(values[0], values[1], values[2]);
                }
                // specular r g b
                else if (cmd == "specular" && ReadValues(s, 3, values))
                {
                    SPECULAR = vec3(values[0], values[1], values[2]);
                }
                // emission r g b
                else if (cmd == "emission" && ReadValues(s, 3, values))
                {
                    EMISSION = vec3(values[0], values[1], values[2]);
                }
                // shininess r g b
                else if (cmd == "shininess" && ReadValues(s, 1, values))
                {
                    SHINY = values[0];
                }
                // sphere x y z radius
                // ++ to objects
                else if (cmd == "sphere" && ReadValues(s, 4, values))
                {
                    OBJS[numObjs] = new sphere(
                        values[0], values[1], values[2], values[3]);
                    OBJS[numObjs]->mat = material(
                        DIFFUSE,
                        SPECULAR,
                        EMISSION,
                        AMBIENT,
                        SHINY);
                    numObjs++;
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
                    OBJS[numObjs] = new tri(
                        values[0],
                        values[1],
                        values[2]);
                    OBJS[numObjs]->mat = material(
                        DIFFUSE,
                        SPECULAR,
                        EMISSION,
                        AMBIENT,
                        SHINY);
                    numObjs++;
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
                // directional 0 0 1 .5 .5 .5
                // point 4 0 4 .5 .5 .5
                else if ((cmd == "directional" || cmd == "point") && ReadValues(s, 6, values))
                {
                    cout << "impliment light" << endl;
                }
                // TODO: attenuation const linear quadratic
                else if (cmd == "attenuation" && ReadValues(s, 3, values))
                {
                    cout << "impliment attenuation" << endl;
                }
                // ambient r g b
                else if (cmd == "ambient" && ReadValues(s, 3, values))
                {
                    AMBIENT = vec3(values[0], values[1], values[2]);
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