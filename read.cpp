/* Extrapolation of the read in functionality created by Ravi Ramamoorthi CSE167/homework 2 code*/

#include <iostream>
#include <string>
#include <GLUT/glut.h>
#include <fstream>
#include <sstream>

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
        cout << "open?" << endl;
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

                switch (cmd)
                {
                }

                if (cmd == "light")
                {
                    cout << "reading in light commmand" << endl;
                    if (ReadValues(s, 8, values))
                    {
                        //++numused;
                    }
                }
                else if (cmd == "test")
                {
                    if (ReadValues(s, 8, values)) // stream, argument length, inserted value
                    {
                        cout << "reading test with 8 arguments" << endl;
                    }
                    // cout << "reading in light commmand" << endl;
                    // validinput = ReadValues(s, 8, values); // Position/color for lts.
                    // if (validinput)
                    // {
                    //     //++numused;
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