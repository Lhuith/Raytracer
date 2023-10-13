/* Extrapolation of the read in functionality created by Ravi Ramamoorthi CSE167/homework 2 code*/
#include <iostream>
#include <string>
#include <GLUT/glut.h>
#include <fstream>
#include <sstream>
#include <stack>
#include "variables.h"

#include <glm/glm.hpp>
#include "transform.h"

using namespace std;

#define READIN_LOCATION "testscenes/"
#define READIN_FILETYPE ".test"

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

void rightmult(const mat4 &M, stack<mat4> &t_stack)
{
    mat4 &T = t_stack.top();
    T = T * M;
}

int ReadCommands(const string filename)
{
    stack<mat4> t_stack;
    t_stack.push(mat4(1.0));

    string str, cmd, readInFileName;
    ifstream in;
    in.open(READIN_LOCATION + filename + READIN_FILETYPE);
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
                    if (!s.fail() && s.rdbuf()->in_avail() != 0)
                    {
                        readInFileName = name;
                    }
                }
                // Lighting Commands
                // directional 0 0 1 .5 .5 .5
                // point 4 0 4 .5 .5 .5
                else if ((cmd == "directional" || cmd == "point") && ReadValues(s, 6, values))
                {
                    LIGHTS[numLights++] = new light(
                        vec3(values[0], values[1], values[2]),
                        vec3(values[3], values[4], values[5]),
                        cmd);
                }
                // attenuation const linear quadratic
                else if (cmd == "attenuation" && ReadValues(s, 3, values))
                {
                    ATTEN = vec3(values[0], values[1], values[2]);
                }
                // ambient r g b
                else if (cmd == "ambient" && ReadValues(s, 3, values))
                {
                    AMBIENT = vec3(values[0], values[1], values[2]);
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
                    OBJS[numObjs]->tr = t_stack.top();
                    OBJS[numObjs]->inv_tr = glm::inverse(OBJS[numObjs]->tr);
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
                    OBJS[numObjs]->tr = t_stack.top();
                    OBJS[numObjs]->inv_tr = glm::inverse(OBJS[numObjs]->tr);
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
                else if (cmd == "translate" && ReadValues(s, 3, values))
                {

                    rightmult(transform::translate(values[0], values[1], values[2]), t_stack);
                }
                else if (cmd == "scale" && ReadValues(s, 3, values))
                {
                    rightmult(transform::scale(values[0], values[1], values[2]), t_stack);
                }
                else if (cmd == "rotate" && ReadValues(s, 4, values))
                {
                    mat3 rot = transform::rotate(values[3], vec3(values[0], values[1], values[2]));
                    rightmult(mat4(rot), t_stack);
                }
                // pushTransform
                else if (cmd == "pushTransform")
                {
                    t_stack.push(t_stack.top());
                }
                // popTransform
                else if (cmd == "popTransform")
                {
                    if (t_stack.size() <= 1)
                    {
                        cerr << "cant pop stack" << endl;
                    }
                    else
                    {
                        t_stack.pop();
                    }
                }
                else
                {
                    cerr << "Unknown Command: " << cmd << endl;
                    return 3;
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

    // no ouput filename was given
    if (readInFileName == "")
    {
        cout << "output not in file, using filename" << endl;
        FILENAME = filename;
    }

    return 0;
}