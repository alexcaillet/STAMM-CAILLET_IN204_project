#ifndef VECTEURHPP
#define VECTEURHPP

#include <cmath>

class Vec{
public:
    double x,y,z;
    Vec(double x1, double y1, double z1): x(x1), y(y1), z(z1){}
    ~Vec(){}

    void normalize(){
        double norme = this->norme();
        if (norme>0){
            x = x/norme;
            y = y/norme;
            z = z/norme;
        }
    }
    double norme2(){
        return x*x+y*y+z*z;
    }
    double norme(){
        return sqrt(norme2());
    }

    Vec operator * (const double &f) const { return Vec(x * f, y * f, z * f); }
    Vec operator * (const Vec &v) const { return Vec(x * v.x, y * v.y, z * v.z); }
    Vec operator - (const Vec &v) const { return Vec(x - v.x, y - v.y, z - v.z); }
    Vec operator + (const Vec &v) const { return Vec(x + v.x, y + v.y, z + v.z); }
    Vec& operator += (const Vec &v) { x += v.x, y += v.y, z += v.z; return *this; }
    Vec operator *= (const Vec &v) { x *= v.x, y *= v.y, z *= v.z; return *this; }
    Vec operator - () const { return Vec(-x, -y, -z); }

};

#endif