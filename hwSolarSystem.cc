
#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <random>
#include <cmath>
#include <ctime>
#include <random>
using namespace std;
default_random_engine e(time(0));
const double G = 6.667E-11;



// put your Vec3d implementation here



class Vec3d {
public:
    double x;
    double y;
    double z;
    double angle;

    Vec3d(double _x = 0, double _y = 0, double _z = 0) :x(_x), y(_y), z(_z) {}

    Vec3d operator-(const Vec3d& a) const {
        return Vec3d(x - a.x, y - a.y, z - a.z);
    }

    Vec3d operator+(const Vec3d& a) const {
        return Vec3d(x + a.x, y + a.y, z + a.z);
    }


    Vec3d& operator=(const Vec3d& a) {
        x = a.x;
        y = a.y;
        z = a.z;
        angle = a.angle;
        return *this;
    }

    double operator *(const Vec3d& a) const{

        return x * a.x + y * a.y + z * a.z;
    }

    Vec3d operator * (const double a) const {
        return Vec3d(x * a, y * a, z * a);
    }

    double calDistance(const Vec3d& a) const {
        return sqrt((x - a.x) * (x - a.x) + (y - a.y) * (y - a.y));
    }

    double calMagSq() {
        return x * x + y * y + z * z;
    }
    


    Vec3d normalize()
    {
        Vec3d vec = *this;
        if (calMagSq() > 0)
        {
            double val = 1.0 / sqrt(vec.calMagSq());
            vec.x *= val;
            vec.y *= val;
            vec.z *= val;
            return vec;
        }
        return Vec3d();
    }

    Vec3d calDpos(const Vec3d& a) {
        return (*this - a).normalize();
    }

};

inline double calVect3Mag(const Vec3d& a)
{
    return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

// put your Body implementation here
class Body {
public:
    string m_name;
    string m_orbitis;
    double m_mass;
    double m_diam;
    double m_perihelion;
    double m_aphelion;
    double m_orbPeriod;
    double m_rotationalPeriod;
    double m_axialtilt;
    double m_orbinclin;
    Vec3d m_pos;
    Vec3d m_a;
    Vec3d m_v;
    Body() = default;


    bool operator==(const Body& b) const {
        return m_name == b.m_name;
    }
    friend istream& operator>>(istream& in, Body& b) {

        string mass;
        string diam;
        string perihelion;
        string aphelion;
        string orbPeriod;
        string rotationalPeriod;
        string axialtilt;
        string orbinclin;
        //read data as a string
        in >> b.m_name >> b.m_orbitis >> mass >> diam >> perihelion >> aphelion >> orbPeriod >> rotationalPeriod >> axialtilt >> orbinclin;
        //cast string to double
        b.m_mass = stod(mass);
        b.m_diam = stod(diam);
        b.m_perihelion = stod(perihelion);
        b.m_aphelion = stod(aphelion);
        b.m_orbPeriod = stod(orbPeriod);
        b.m_rotationalPeriod = stod(rotationalPeriod);
        b.m_axialtilt = stod(axialtilt);
        b.m_orbinclin = stod(orbinclin);

        //random angle
        uniform_real_distribution<double> u(0, 360);
        b.m_pos.angle = u(e);

        std::cout << b.m_name << "    " << b.m_pos.angle << "\n";
        b.m_pos.x = (b.m_aphelion + b.m_aphelion) / 2 * sin(b.m_pos.angle * M_PI / 180);
        b.m_pos.y = (b.m_aphelion + b.m_aphelion) / 2 * cos(b.m_pos.angle * M_PI / 180);
        return in;
    }

    friend ostream& operator <<(ostream& out, Body& b) {
        out << b.m_name << "    " << "pos( " << b.m_pos.x << " , " << b.m_pos.y << " , " << b.m_pos.z << " )    " << "v( " << b.m_v.x << " , " << b.m_v.y << " , " << b.m_v.z << " )\n";
        return out;
    }

    //     b.pos = b.pos + b.v * dt + 0.5 * b.a * dt*dt
    //     b.v = b.v + b.a * dt;
    void calPos(double dt) {
        m_pos = m_pos + m_v * dt +  m_a * dt * dt*0.5;
    }

    void calV(double dt) {
        m_v = m_v + m_a * dt;
    }

};

class SolarSystem {
private:
    vector<Body> bodies;
public:
    explicit SolarSystem(const char* path) {
        ifstream inFile(path);
        string buffTmp;
        getline(inFile, buffTmp);
        while (!inFile.eof()) {

            getline(inFile, buffTmp);
            stringstream stringIn(buffTmp);
            Body body;
            stringIn >> body;
            bodies.emplace_back(body);
            stringIn.clear();
        }
    }

    void calAcceleration() {
        for (auto& i : bodies) {
            for (auto j : bodies) {
                if (i == j) {
                    continue;
                }
                else {
                    double dis = i.m_pos.calDistance(j.m_pos);
                    double a = G * j.m_mass / (dis * dis);
                    Vec3d dpos = i.m_pos.calDpos(j.m_pos);
                    double r = calVect3Mag(dpos);
                    i.m_a = i.m_a + Vec3d(a * dpos.x / r, a * dpos.y / r, a * dpos.z / r);
                }
            }
        }
    }

    void stepForward(double dt) {
        calAcceleration();
        for (auto& i : bodies) {
            i.calV(dt);
            i.calPos(dt);
        }
    }

    friend ostream& operator<<(ostream& out, SolarSystem& s) {
        for (auto i : s.bodies) {
            out << i<<"\n";
        }
        return out;
    }



};



int main() {
    /*
    1. read in the solar system from the file
  2. Your SolarSystem class should have a list (vector) of Body objects
  3. for each row in the file, add a body. Calculate the initial position for
     each body as last week. Note that for now we will not be giving you moons
     because they orbit not the sun but other planets. This will be added later.
  4. Each body should calculate the acceleration due to the gravitational pull
     of all the others bodies. This requires vector math, which you should write
     in Vec3d.
     For any body m, the accelation due to another body m2 is:
     a = Gm2/rsq,   rsq = (pos2 - pos).magsq()
     G = universal gravitational constant 6.667E-11
     To calculate the vector acceleration, you must break down a into components
     dpos = (pos2 - pos).normalize
     (compute a unit vector in the direction of body2)
     ax = a * dpos.x/r, ay = a * dpos.y/r, az = a * dpos.z/r
     r = magnitude of dpos.
         If you can do this calculation within vector, it is a generically useful
         one but you can also just compute it in Body if you want.
         Sum up all accelerations.
         for each body i
       set acceleration of body i = 0
           for each body j
         add acceleration of body j on i unless j == i
       end
     end
     when you have the acceleration of all bodies, step forward in time for a timestep dt with:
    for each body b
     b.pos = b.pos + b.v * dt + 0.5 * b.a * dt*dt
     b.v = b.v + b.a * dt;
    dt is delta time (double precision) and should not be huge.
    Start with 1000 seconds and see what happens. If
    one body is in a sharply changing orbit, dt may have to be reduced.
    For the above to work you will have to implement some operators in Vec3d.
    v^2/r = a
        v = sqrt(ar)
    */
    SolarSystem s("solarsystem.dat");

    cout << s << "=========\n\n\n"; // print the solar system at load time
    for (int i = 0; i < 31556; i++) {
        s.stepForward(1000); // step forward in time by 1000 seconds
    }
    // the loop should step forward in time by about a year. We are simplifying
    // the orbit to a circle, so the numbers will be off a bit.
    cout << s << "=========\n\n\n"<< (1.25414e+14 - 1.51532e+11)<< 1.50448e+10; // print the solar system after the year

}