#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <cmath>

#include "vecteur.hpp"
#include "ray.hpp"

#define PI 3.141592653589793

class Camera
{
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
    Camera() : aspect_ratio(16.0 / 9.0), fov(80)
    {
        double viewport_width = aspect_ratio * viewport_height;
        double focal_length = viewport_width / tan(PI * fov / 180 * 0.5);
        origine = Vec(0.0, 0.0, 35.0);
        largeur = Vec(viewport_width, 0.0, 0.0);
        hauteur = Vec(0.0, viewport_height, 0.0);
        coin_haut_gauche = origine - largeur * 0.5 + hauteur * 0.5 - Vec(0.0, 0.0, focal_length);
    }

    Camera(int image_width, int image_height) : aspect_ratio((double)image_width / (double)image_height), fov(80)
    {
        double viewport_width = aspect_ratio * viewport_height;
        double focal_length = viewport_width / tan(PI * fov / 180 * 0.5);
        origine = Vec(0.0, 0.0, 35.0);
        largeur = Vec(viewport_width, 0.0, 0.0);
        hauteur = Vec(0.0, viewport_height, 0.0);
        coin_haut_gauche = origine - largeur * 0.5 + hauteur * 0.5 - Vec(0.0, 0.0, focal_length);
    }

    Camera(int image_width, int image_height, int fieldOfView) : aspect_ratio((double)image_width / (double)image_height), fov(fieldOfView)
    {
        double viewport_width = aspect_ratio * viewport_height;
        double focal_length = viewport_width / tan(PI * fov / 180 * 0.5);
        origine = Vec(0.0, 0.0, 35.0);
        largeur = Vec(viewport_width, 0.0, 0.0);
        hauteur = Vec(0.0, viewport_height, 0.0);
        coin_haut_gauche = origine - largeur * 0.5 + hauteur * 0.5 - Vec(0.0, 0.0, focal_length);
    }

    Camera(Vec position, Vec axe) : aspect_ratio(16.0 / 9.0), fov(80)
    {
        double viewport_width = aspect_ratio * viewport_height;
        double focal_length = viewport_width / tan(PI * fov / 180 * 0.5);
        axe.normalize();
        origine = position;
        if (axe.x == 0 && axe.z == 0)
        {
            largeur = Vec(1, 0, 0);
        }
        else
        {
            largeur = Vec(-axe.z, 0.0, axe.x);
            largeur.normalize();
        }

        hauteur = largeur.prod_vec(axe);
        hauteur.normalize();
        largeur = largeur * viewport_width;
        hauteur = hauteur * viewport_height;

        coin_haut_gauche = origine - largeur * 0.5 + hauteur * 0.5 + axe * focal_length;
    }

    Camera(Vec position, Vec axe, int fieldOfView) : aspect_ratio(16.0 / 9.0), fov(fieldOfView)
    {
        double viewport_width = aspect_ratio * viewport_height;
        double focal_length = viewport_width / tan(PI * fov / 180 * 0.5);
        axe.normalize();
        origine = position;
        if (axe.x == 0 && axe.z == 0)
        {
            largeur = Vec(1, 0, 0);
        }
        else
        {
            largeur = Vec(-axe.z, 0.0, axe.x);
            largeur.normalize();
        }

        hauteur = largeur.prod_vec(axe);
        hauteur.normalize();
        largeur = largeur * viewport_width;
        hauteur = hauteur * viewport_height;

        coin_haut_gauche = origine - largeur * 0.5 + hauteur * 0.5 + axe * focal_length;
    }

    Camera(Vec position, Vec axe, int image_width, int image_height, int fieldOfView) : aspect_ratio((double)image_width / (double)image_height), fov(fieldOfView)
    {
        double viewport_width = aspect_ratio * viewport_height;
        double focal_length = viewport_width / tan(PI * fov / 180 * 0.5);
        axe.normalize();
        origine = position;
        if (axe.x == 0 && axe.z == 0)
        {
            largeur = Vec(1, 0, 0);
        }
        else
        {
            largeur = Vec(-axe.z, 0.0, axe.x);
            largeur.normalize();
        }

        hauteur = largeur.prod_vec(axe);
        hauteur.normalize();
        largeur = largeur * viewport_width;
        hauteur = hauteur * viewport_height;

        coin_haut_gauche = origine - largeur * 0.5 + hauteur * 0.5 + axe * focal_length;
    }

    //Constructeur avec rotation de la camera autour de l'axe (tilt en degrée)
    Camera(Vec position, Vec axe, int image_width, int image_height, int fieldOfView, double tilt) : aspect_ratio((double)image_width / (double)image_height), fov(fieldOfView)
    {
        double viewport_width = aspect_ratio * viewport_height;
        double focal_length = viewport_width / tan(PI * fov / 180 * 0.5);
        axe.normalize();
        origine = position;
        if (axe.x == 0 && axe.z == 0)
        {
            largeur = Vec(1, 0, 0);
        }
        else
        {
            largeur = Vec(-axe.z, 0.0, axe.x);
            largeur.normalize();
        }

        //Rotation de la caméra
        double tilt_rad = (double)tilt * PI / 180.0; //conversion de l'angle en radian
        //Calcul des lignes de la matrice de rotation autour de axe
        Vec R1(axe.x * axe.x * (1 - cos(tilt_rad)) + cos(tilt_rad), axe.x * axe.y * (1 - cos(tilt_rad)) - axe.z * sin(tilt_rad), axe.x * axe.z * (1 - cos(tilt_rad)) + axe.y * sin(tilt_rad));
        Vec R2(axe.x * axe.y * (1 - cos(tilt_rad)) + axe.z * sin(tilt_rad), axe.y * axe.y * (1 - cos(tilt_rad)) + cos(tilt_rad), axe.y * axe.z * (1 - cos(tilt_rad)) - axe.x * sin(tilt_rad));
        Vec R3(axe.x * axe.z * (1 - cos(tilt_rad)) - axe.y * sin(tilt_rad), axe.y * axe.z * (1 - cos(tilt_rad)) + axe.x * sin(tilt_rad), axe.z * axe.z * (1 - cos(tilt_rad)) + cos(tilt_rad));
        //Rotation du vecteur largeur
        largeur = Vec(R1.dot(largeur), R2.dot(largeur), R3.dot(largeur));

        hauteur = largeur.prod_vec(axe);
        hauteur.normalize();
        largeur = largeur * viewport_width;
        hauteur = hauteur * viewport_height;

        coin_haut_gauche = origine - largeur * 0.5 + hauteur * 0.5 + axe * focal_length;
    }

    Ray getRay(double u, double v)
    {
        Vec dir = coin_haut_gauche + largeur * u - hauteur * v - origine;
        dir.normalize();
        return Ray(origine, dir);
    }
};

#endif