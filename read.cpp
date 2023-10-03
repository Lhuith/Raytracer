/* Extrapolation of the read in functionality created by Ravi Ramamoorthi CSE167/homework 2 code*/
#include <iostream>
#include <string>
#include <GLUT/glut.h>
#include <fstream>
#include <sstream>

#include "variables.h"

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

void readfile(const char *filename)
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

                // Process the light, add it to database.
                // Lighting Command
                if (cmd == "light")
                {
                    cout << "reading in light commmand" << endl;
                    if (ReadValues(s, 8, values))
                    {
                    }
                }
                else if (cmd == "size")
                {
                    if (ReadValues(s, 2, values))
                    {
                        WIDTH = values[0];
                        HEIGHT = values[1];
                    }
                }
                else if (cmd == "maxdepth")
                {
                    if (ReadValues(s, 1, values))
                    {
                        DEPTH = values[0];
                    }
                }
                else if (cmd == "output")
                {
                    string fileName = "";
                    s >> fileName;
                    if (!s.fail())
                    {
                        FILENAME = fileName;
                    }
                    else
                    {
                        cout << "file name grab failed!" << endl;
                    }

                    // if (ReadValues(s, 1, values))
                    // {

                    // }
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
        throw 2;
    }
}

void ReadCommands(const char *filename)
{
    readfile(filename);
}