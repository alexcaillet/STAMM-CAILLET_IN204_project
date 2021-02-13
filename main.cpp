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

/*Calcul de la couleur d'un pixel sur l'image*/
Vec calcul_pixel(Ray rayon, std::vector<std::unique_ptr<Objet>>& objets){
    //On commence par chercher s'il y a un point d'intersection entre le rayon et un des objets de la scène
    double tmin = INFINI;
    int closest_object = -1;
    double t = INFINI;
    for(int i=0; i<objets.size(); i++){
        if (objets[i].intersect(rayon, &t) && t<tmin){ //encore un pb pour appeler la methode intersect
            tmin = t;
            closest_object = i;
        }
    }

    //si pas d'objet sur le parcours du rayon, on renvoie la couleur d'arrière-plan
    if(closest_object==-1){
        return Vec(0.0, 0.0, 0.0); //on renvoie du noir
    }
    else{
        return objets[closest_object].couleur;
    }
}


int main()
{

    //Test de la classe image
    // Image

    const int image_width = 500;
    const int image_height = 400;

    Picture image(image_width, image_height);
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
    std::cerr << "\nDone.\n";


    Picture scene(image_width, image_height);
    double aspect_ratio = (double)image_width/(double)image_height;

    //ajout d'objets
    std::vector<std::unique_ptr<Objet>> objets;
    objets.push_back(std::make_unique<Sphere> (Sphere(Vec(0.0, 0.0, 10.0), Vec(1.0, 0.0, 0.0), 0.0, 0.0, 1.0)));

    // Camera

    double viewport_height = 2.0;
    double viewport_width = aspect_ratio * viewport_height;
    double fov = 50; //angle de vue en degré
    double focal_length = viewport_width/tan(PI*fov/180*0.5);

    Vec origine(0.0, 0.0, 0.0);
    Vec largeur(viewport_width, 0.0, 0.0);
    Vec hauteur(0.0, viewport_height, 0.0);
    Vec coin_haut_gauche = origine - largeur*0.5 + hauteur*0.5 + Vec(0.0, 0.0, focal_length);

    for(int j=0; j<image_height; j++){
        for(int i=0; i<image_width; i++){
            double u = i/(double)(image_width-1);
            double v = j/(double)(image_height-1);
            Vec dir = coin_haut_gauche+largeur*u+hauteur*v-origine;
            dir.normalize();
            Ray rayon_incident(origine, dir);
            scene.pixels[j*image_width + i] = calcul_pixel(rayon_incident, objets);
        }
    }

    scene.savePicture("premier_test.ppm");


    //fin du test
}