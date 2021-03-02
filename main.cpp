#include <iostream>
#include <stdbool.h>
#include <math.h>
#include <memory>
#include "vecteur.hpp"
#include "image.hpp"
#include "objet.hpp"
#include "ray.hpp"

#define PI 3.141592653589793
#define INFINI 1e8
#define rouge Vec(255.0, 0.0, 0.0)
#define bleu Vec(0, 0.0, 255.0)
#define vert Vec(0, 255.0, 0.0)
#define gris Vec(50.0, 50.0, 50.0)

/*Calcul de la couleur d'un pixel sur l'image*/
Vec calcul_pixel(Ray rayon, std::vector<Objet*>& objets){
    //On commence par chercher s'il y a un point d'intersection entre le rayon et un des objets de la scène
    double tmin = INFINI;
    int closest_object = -1;
    double t = INFINI;
    Vec normale_temp;
    Vec normale;
    for(unsigned int i=0; i<objets.size(); i++){
        if (objets[i]->intersect(rayon, &t, &normale_temp) && t<tmin){
            tmin = t;
            closest_object = i;
            normale = normale_temp;
        }
    }

    //si pas d'objet sur le parcours du rayon, on renvoie la couleur d'arrière-plan
    if(closest_object==-1){
        return gris;
    }
    else{
        return objets[closest_object]->couleur * abs(normale.dot(rayon.direction));
    }
}

void rendu(std::vector<Objet *> objets, int image_width, int image_height, int fov, const std::string &filename){
    Picture scene(image_width, image_height);

    //camera
    double aspect_ratio = (double)image_width/(double)image_height;
    double viewport_height = 2.0;
    double viewport_width = aspect_ratio * viewport_height;
    double focal_length = viewport_width/tan(PI*fov/180*0.5);

    Vec origine(0.0, 0.0, 0.0);
    Vec largeur(viewport_width, 0.0, 0.0);
    Vec hauteur(0.0, viewport_height, 0.0);
    Vec coin_haut_gauche = origine - largeur*0.5 + hauteur*0.5 - Vec(0.0, 0.0, focal_length);

    //calcul des pixels
    for(int j=0; j<image_height; j++){
        for(int i=0; i<image_width; i++){
            double u = i/(double)(image_width-1);
            double v = j/(double)(image_height-1);
            Vec dir = coin_haut_gauche+largeur*u-hauteur*v-origine;
            dir.normalize();
            Ray rayon_incident(origine, dir);
            scene.pixels[j*image_width + i] = calcul_pixel(rayon_incident, objets);
        }
    }
    scene.savePicture("test_savePicture.ppm", 0);
    scene.savePicture("test_savePicture.jpeg", 1);
}


int main()
{

    //Test de la classe image
    // Image

    const int image_width = 640;
    const int image_height = 360;

    /*const int image_width = 1920;
    const int image_height = 1080;*/


    /*Picture image(image_width, image_height);
    // Render
    for (int j = 0; j<image_height; ++j)
    {
        std::cerr << "\rScanlines remaining: " << image_height-j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i)
        {
            auto r = double(i) / (image_width - 1);
            auto g = double(j) / (image_height - 1);
            auto b = 0.25;

            image.pixels[i + j*image_width].x = static_cast<int>(255.999 * r);
            image.pixels[i + j*image_width].y = static_cast<int>(255.999 * g);
            image.pixels[i + j*image_width].z = static_cast<int>(255.999 * b);

        }
    }
    image.savePicture("image.ppm");
    std::cerr << "\nDone.\n";*/

    //ajout d'objets
    std::vector<Objet *> objets;
    //objets.push_back( new Sphere(Vec(-1.5, 0.0, -20.0), Vec(0.0, 0.0, 255.0), 0.0, 0.0, 2.0));
    objets.push_back( new Sphere(Vec(3.0, 0.0, -25.0), Vec(255.0, 160.0, 0.0), 0.0, 0.0, 2.0));
    //objets.push_back( new Sphere(Vec(0.0, 0.0, -30.0), Vec(0.0, 0.0, 150.0), 0.0, 0.0, 2.0));
    //objets.push_back( new Sphere(Vec(0.0, 0.0, -20.0), Vec(255.0, 0.0, 150.0), 0.0, 0.0, 1.0));
    //objets.push_back( new Plan(Vec(0.0, 0.0, -35.0), vert, 0.0, 0.0, Vec(3.0, 0.0, -5.0), Vec(0.0, 3.0, 0.0)));
    //objets.push_back( new Plan(Vec(0.0, 0.0, -35.0), bleu, 0.0, 0.0, Vec(-3.0, 0.0, -5.0), Vec(0.0, 3.0, 0.0)));
    //objets.push_back( new Plan(Vec(0.0, 0.0, -40.0), rouge, 0.0, 0.0, Vec(-5.0, 0.0, -5.0), Vec(0.0, 5.0, 0.0)));
    objets.push_back( new Plan(Vec(-5.0, -3.0, -15.0), vert, 0.0, 0.0, Vec(10, 0.0, 0.0), Vec(0.0, 0.0, -20.0)));
    //objets.push_back( new Parallelepipede());
    objets.push_back( new Parallelepipede(Vec(0.0, 0.0,-35.0), rouge, 0.0, 0.0, Vec(2.5*sqrt(2), 0.0, -2.5*sqrt(2)).prod_vec(Vec(-2.5*sqrt(2), -3.0, -2.5*sqrt(2)))*0.25, Vec(-2.5*sqrt(2), -3.0, -2.5*sqrt(2)), Vec(2.5*sqrt(2), 0.0, -2.5*sqrt(2))));

    int fov = 90;
    rendu(objets, image_width, image_height, fov, "premier_test.ppm");
    for(unsigned int i=0; i<objets.size(); i++){
        delete objets[i];
    }

    //fin du test
}