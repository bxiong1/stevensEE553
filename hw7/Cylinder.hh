#ifndef CYLINDER_HH_
#define CYLINDER_HH_
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Cylinder {
private:
       int x, y, z, r, h, num;
       ofstream f;
public:
       Cylinder(int x, int y, int z, int r, int h, int num); 
       void print(string filename);
};

#endif


