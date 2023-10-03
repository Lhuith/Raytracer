#include <iostream>
#include <string>
#include "variables.h" // global variables
#include "read.cpp"

using namespace std;

void init()
{
    WIDTH = 0;
    HEIGHT = 0;
    DEPTH = 5;
}

int main()
{
    init();
    cout << "hello test world" << endl;
    ReadCommands("test.txt");
    cout << "height: " << HEIGHT << ", width: " << WIDTH << ", depth: " << DEPTH << endl;
    cout << "fileName: " << FILENAME << endl;
    return 0;
}