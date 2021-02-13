#ifndef RAY_HPP
#define RAY_HPP
#include "vecteur.hpp"

class Ray{
    public:
        Vec origine;
        Vec direction;

        Ray(): origine(Vec(0.0, 0.0, 0.0)), direction(Vec(0.0, 0.0, 0.0)){}
        Ray(Vec ori, Vec dir) : origine(ori), direction(dir){}
        ~Ray(){}

};

#endif