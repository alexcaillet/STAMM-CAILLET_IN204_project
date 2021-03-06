#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <math.h>

#include "vecteur.hpp"
#include "ray.hpp"

#define PI 3.141592653589793

class Camera{
    private:
        double aspect_ratio;
        double focal_length;
        int fov;
        const double viewport_height = 2.0;
        double viewport_width;

        Vec origine;
        Vec largeur;
        Vec hauteur;
        Vec coin_haut_gauche;

    public:
        Camera(): aspect_ratio(16.0/9.0), fov(80){
            double viewport_width = aspect_ratio * viewport_height;
            double focal_length = viewport_width/tan(PI*fov/180*0.5);
            origine = Vec(0.0, 0.0, 0.0);
            largeur = Vec(viewport_width, 0.0, 0.0);
            hauteur = Vec(0.0, viewport_height, 0.0);
            coin_haut_gauche = origine - largeur*0.5 + hauteur*0.5 - Vec(0.0, 0.0, focal_length);
        }

        Camera(int image_width, int image_height): aspect_ratio((double)image_width/(double)image_height), fov(80){
            double viewport_width = aspect_ratio * viewport_height;
            double focal_length = viewport_width/tan(PI*fov/180*0.5);
            origine = Vec(0.0, 0.0, 0.0);
            largeur = Vec(viewport_width, 0.0, 0.0);
            hauteur = Vec(0.0, viewport_height, 0.0);
            coin_haut_gauche = origine - largeur*0.5 + hauteur*0.5 - Vec(0.0, 0.0, focal_length);
        }

        Camera(int image_width, int image_height, int fieldOfView): aspect_ratio((double)image_width/(double)image_height), fov(fieldOfView){
            double viewport_width = aspect_ratio * viewport_height;
            double focal_length = viewport_width/tan(PI*fov/180*0.5);
            origine = Vec(0.0, 0.0, 0.0);
            largeur = Vec(viewport_width, 0.0, 0.0);
            hauteur = Vec(0.0, viewport_height, 0.0);
            coin_haut_gauche = origine - largeur*0.5 + hauteur*0.5 - Vec(0.0, 0.0, focal_length);
        }

        /*
        Camera(Vec position, Vec direction): aspect_ratio(16.0/9.0), fov(80) {
            double viewport_width = aspect_ratio * viewport_height;
            double focal_length = viewport_width/tan(PI*fov/180*0.5);
            origine = position;

        }*/

        Ray getRay(double u, double v){
            Vec dir = coin_haut_gauche+largeur*u-hauteur*v-origine;
            dir.normalize();
            return Ray(origine, dir);
        }


};

#endif