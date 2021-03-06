#ifndef MOTEUR_RENDU_HPP
#define MOTEUR_RENDU_HPP

#include <iostream>
#include <stdlib.h>
#include <stdbool.h>
#include <cmath>
#include <memory>
#include <random>
#include <omp.h>
#include <chrono>

#include "vecteur.hpp"
#include "image.hpp"
#include "objet.hpp"
#include "ray.hpp"
#include "camera.hpp"

#define PI 3.141592653589793
#define INFINI 1e8

#define rouge Vec(255.0, 0.0, 0.0)
#define bleu Vec(0, 0.0, 255.0)
#define vert Vec(0, 255.0, 0.0)
#define gris Vec(50.0, 50.0, 50.0)
#define grisclair Vec(200,200,200)
#define blanc Vec(255, 255, 255)
#define noir Vec(0,0,0)

#define max_depth 10

class moteur_rendu{
    public:
        moteur_rendu(){};
        ~moteur_rendu(){};
        /*Calcul de la couleur d'un pixel sur l'image*/
        Vec calcul_pixel(Ray rayon, std::vector<Objet*>& objets, int depth);
        void rendu(std::vector<Objet *> objets, int image_width, int image_height, int fov, const std::string &filename);
        int calcul();
    private:
        float mix(const float &a, const float &b, const float &mix);
        inline double random_double();
    
};

#endif