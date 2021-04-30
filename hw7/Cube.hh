#ifndef CUBE_HH_
#define CUBE_HH_
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Cube {
private:
       int x, y, z, l;
       ofstream f;
public:
       Cube(int x, int y, int z, int l); 
       void print(string filename);
};

#endif
