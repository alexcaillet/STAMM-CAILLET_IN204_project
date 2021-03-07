#ifndef MOTEUR_RENDU_HPP
#define MOTEUR_RENDU_HPP

#include <iostream>
#include <stdlib.h>
#include <stdbool.h>
#include <cmath>
#include <memory>
#include <random>
#include <omp.h>
//#include "/usr/local/Cellar/gcc/10.2.0/lib/gcc/10/gcc/x86_64-apple-darwin19/10.2.0/include/omp.h"
#include <chrono>

#include "vecteur.hpp"
#include "image.hpp"
#include "objet.hpp"
#include "ray.hpp"
#include "camera.hpp"
#include "xmlread.hpp"

#define PI 3.141592653589793
#define INFINI 1e8

#define rouge Vec(255.0, 0.0, 0.0)
#define bleu Vec(0, 0.0, 255.0)
#define vert Vec(0, 255.0, 0.0)
#define gris Vec(50.0, 50.0, 50.0)
#define grisclair Vec(200, 200, 200)
#define blanc Vec(255, 255, 255)
#define noir Vec(0, 0, 0)

#define max_depth 100

class moteur_rendu
{
public:
    moteur_rendu(){};
    ~moteur_rendu(){};

    /*Calcul de la couleur d'un pixel sur l'image*/
    Vec calcul_pixel(Ray rayon, std::vector<Objet *> &objets, int depth)
    {
        //On commence par chercher s'il y a un point d'intersection entre le rayon
        //et un des objets de la scène.
        double tmin = INFINI;
        int closest_object_index = -1;
        double t = INFINI;
        Vec normale_temp;
        Vec normale;
        for (unsigned int i = 0; i < objets.size(); i++)
        {
            if (objets[i]->intersect(rayon, &t, &normale_temp) && t < tmin)
            {
                tmin = t;
                closest_object_index = i;
                normale = normale_temp;
            }
        }

        //Si pas d'objet sur le parcours du rayon, on renvoie la couleur d'arrière-plan
        //C'est la condition de terminaison de la fct recursive
        if (closest_object_index == -1)
        {
            return blanc;
        }

        Objet *closest_object = objets[closest_object_index];
        Vec point_intersection = rayon.origine + rayon.direction * tmin;
        Vec pixel_color(0, 0, 0);

        //On adapte la normale pour la réflexion et l'effet fresnel
        if (rayon.direction.dot(normale) < 0)
        {
            normale = -normale;
        }
        //Pour contrer l'acnée de reflexion, on a besoin d'une petite valeur à ajouter
        float epsilon = 1e-4;
        //On gère l'effet fresnel
        float facingratio = rayon.direction.dot(normale);
        static float fresneleffect = mix(pow(1 - facingratio, 3), 1, 0.1);

        if (closest_object->reflectivite > 0 && depth < max_depth)
        {

            //On s'occupe de la réflexion (direction + recursivité)
            Vec reflecdir = rayon.direction - normale * 2 * rayon.direction.dot(normale);
            reflecdir.normalize();
            Ray new_reflec_rayon(point_intersection - normale * epsilon, reflecdir);
            Vec reflection = calcul_pixel(new_reflec_rayon, objets, depth + 1);

            pixel_color += (closest_object->couleur * (1 - closest_object->reflectivite) + reflection * closest_object->reflectivite) * fresneleffect;

            if (!closest_object->transparence)
            {
                pixel_color += closest_object->couleur * (1 - fresneleffect);
            }
        }

        if (closest_object->transparence > 0 && depth < max_depth)
        {

            float epsilon = 1e-4;
            //On gère l'effet fresnel
            float facingratio = rayon.direction.dot(normale);
            static float fresneleffect = mix(pow(1 - facingratio, 3), 1, 0.1);
            bool inside = false;

            normale = -normale;

            //On adapte la normale pour la réfraction
            if (rayon.direction.dot(normale) > 0)
            {
                normale = -normale;
                inside = true;
            }

            float ior = 1.1, eta = (inside) ? ior : 1 / ior;
            float cosi = -normale.dot(rayon.direction);
            float k = 1 - eta * eta * (1 - cosi * cosi);
            Vec refracdir = rayon.direction * eta + normale * (eta * cosi - sqrt(k));
            refracdir.normalize();
            Ray new_refrac_rayon(point_intersection - normale * epsilon, refracdir);
            Vec refraction = calcul_pixel(new_refrac_rayon, objets, depth + 1);
            pixel_color += (refraction * closest_object->transparence +
                            closest_object->couleur * (1 - closest_object->transparence)) *
                           (1 - fresneleffect);

            if (!closest_object->reflectivite)
            {
                pixel_color += closest_object->couleur * fresneleffect;
            }
        }

        else
        {
            pixel_color = closest_object->couleur; // *abs(normale.dot(rayon.direction));
            for (int i = 0; i < objets.size(); i++)
            {
                if (objets[i]->emission.x > 0)
                {
                    // this is a light
                    Vec transmission(1, 1, 1);
                    Vec lumDir = objets[i]->position - point_intersection;
                    lumDir.normalize();
                    for (int j = 0; j < objets.size(); ++j)
                    {
                        if (i != j)
                        {
                            Ray lighting_rayon(point_intersection + normale * epsilon, lumDir);

                            if (objets[j]->intersect(lighting_rayon, &t, &normale_temp))
                            {
                                transmission = Vec(0, 0, 0);
                                break;
                            }
                        }
                    }
                    pixel_color += closest_object->couleur * transmission *
                                   std::max(double(0), normale.dot(lumDir)) * objets[i]->emission;
                }
            }
        }

        //eclaircissement de l'image temporaire
        double facteur_eclaircissement = 1.7;
        //pixel_color.x = std::min(255.0, pixel_color.x*facteur_eclaircissement);
        //pixel_color.y = std::min(255.0, pixel_color.y*facteur_eclaircissement);
        //pixel_color.z = std::min(255.0, pixel_color.z*facteur_eclaircissement);

        return pixel_color*0.8 + closest_object->emission ;//*abs(normale.dot(rayon.direction));
    }

    void rendu(std::vector<Objet *> objets, int image_width, int image_height, Camera camera, const char *filename)
    {
        Picture scene(image_width, image_height);

        int sample_per_pixel = 10; //pour l'anti-aliasing

        auto start = std::chrono::system_clock::now();

        int i, j, s;
        //calcul des pixels
        for (j = 0; j < image_height; j++)
        {
            std::cerr << "\rLignes restantes: " << image_height - 1 - j << ' ' << std::flush;
            #pragma omp parallel for schedule(dynamic, 100) private(i, s) num_threads(omp_get_max_threads())
            for (i = 0; i < image_width; i++)
            {
                Vec pixel_color = Vec();
                for (s = 0; s < sample_per_pixel; s++)
                {
                    double u = (i + random_double()) / (double)(image_width - 1);
                    double v = (j + random_double()) / (double)(image_height - 1);
                    Ray rayon_incident = camera.getRay(u, v);
                    pixel_color += calcul_pixel(rayon_incident, objets, 0);
                }
                pixel_color = pixel_color * (1.0 / sample_per_pixel);
                //pixel_color.x = std ::min(double(255), pixel_color.x);
                //pixel_color.y = std ::min(double(255), pixel_color.y);
                //pixel_color.z = std ::min(double(255), pixel_color.z);
                scene.pixels[j * image_width + i] = pixel_color;
            }
    }

    std::chrono::duration<double> temps_calcul = std::chrono::system_clock::now() - start;
    std::cerr << std::endl
              << "Temps de calcul : " << temps_calcul.count() << "s\n";

    scene.savePicture(filename, 2);
}

void calcul(const char* filename)
{
    //const int image_width = 640;
    //const int image_height = 360;

    //const int image_width = 1920;
    //const int image_height = 1080;

    //16K
    //const int image_width = 15360;
    //const int image_height = 8640;

    int image_height, image_width;
    readDimension( filename, &image_width, &image_height);

    Camera camera = readCamera(filename);

    //ajout d'objets

    std::vector<Objet *> objets = read(filename);

    //objets.push_back( new Cylindre(Disque(Vec(2, 1, -15), Vec(20,20,255), 0.5, 0.5, 1.0, Vec(0.0, -1.0, -1.0)), 3.0));


    /*
    //objets.push_back(new Sphere(Vec(4, 20.0, -45.0), Vec(255, 255, 255), 0.0, 0.0, Vec(0, 0, 0), 10.0));
    objets.push_back(new Sphere(Vec(-3.0, 1.0, -35.0), rouge, 0.8, 0.2, 2.0));
    objets.push_back(new Sphere(Vec(3.0, -1.0, -35.0), vert, 0.8, 0.2, 0.5));
    objets.push_back(new Sphere(Vec(1.0, 2, -30.0), bleu, 0.8, 0.2, 1.0));
    //objets.push_back( new Sphere(Vec(0.0, 5, -20.0), blanc, 0.5, 0.5, 2.0));
    //objets.push_back( new Sphere(Vec(0.0, -100.5, 0.0), vert, 0.5, 0.0, 100.0));
    //objets.push_back( new Sphere(Vec(-3.0, 10.0, -20.0), Vec(255.0, 0.0, 150.0), 0.0, 0.0, 1.0));

    //objets.push_back( new Plan(Vec(0.0, 1.0, -35.0), vert, 0.0, 0.0, Vec(3.0, 0.0, -5.0), Vec(0.0, 3.0, 0.0)));
    //objets.push_back( new Plan(Vec(0.0, 0.0, -35.0), bleu, 0.0, 0.0, Vec(-3.0, 0.0, -5.0), Vec(0.0, 3.0, 0.0)));
    objets.push_back(new Plan(Vec(0.0, -2.0, -40.0), Vec(0, 150, 150), 1, 0.5, Vec(15.0, 0.0, 10.0), Vec(0.0, 5.0, 0.0)));
    objets.push_back(new Plan(Vec(-100.0, -5.0, 0.0), vert, 0.9, 0.1, Vec(200, 0.0, 0.0), Vec(0.0, 0.0, -300.0)));
    //objets.push_back( new Plan);
    //objets.push_back( new Plan(Vec(0.0, 0.0, -25.0), rouge, 0.9, 0.9, Vec(0.0, 2.0, -2.0), Vec(2.0, 0.0, 0.0)));

    objets.push_back(new Parallelepipede(Vec(-10.0, 2.0, -15.0), bleu, 0.4, 0.6, Vec(2.5 * sqrt(2), 0.0, -2.5 * sqrt(2)).prod_vec(Vec(-2.5 * sqrt(2), -3.0, -2.5 * sqrt(2))) * 0.25, Vec(-2.5 * sqrt(2), -3.0, -2.5 * sqrt(2)), Vec(2.5 * sqrt(2), 0.0, -2.5 * sqrt(2))));

    //objets.push_back( new Disque(Vec(-4.0, 2.0, -25.0), Vec(255,255,0) ,0.9, 0.2, 3.0, Vec(1.0, 0, 0.2)));

    //objets.push_back( new Cylindre(Disque(Vec(-3, 1, -25), bleu, 0.5, 0.0, 1.0, Vec(-1.0, 0.0, 0.0)), 5.0));

    //objets.push_back( new Cylindre(Disque(Vec(0, 0, -25), bleu, 0.7, 0.5, Vec(3.0, 0.0, 0.0), Vec(0.0, -1.0, 0.0)), 3.0));
    */

    int fov = 90;
    rendu(objets, image_width, image_height, camera, "premier_test.png");
    for (unsigned int i = 0; i < objets.size(); i++)
    {
        delete objets[i];
    }
    std::cerr << "fin \n";
}

private:
    float mix(const float &a, const float &b, const float &mix)
    {
        return b * mix + a * (1 - mix);
    }

    inline double random_double()
    {
        static std::uniform_real_distribution<double> distribution(0.0, 1.0);
        static std::mt19937 generator;
        return distribution(generator);
    }
}
;

#endif
