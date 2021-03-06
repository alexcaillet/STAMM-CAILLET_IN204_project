#include <iostream>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <memory>
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

float mix(const float &a, const float &b, const float &mix) 
{ 
    return b * mix + a * (1 - mix); 
} 

#define max_depth 10
/*Calcul de la couleur d'un pixel sur l'image*/
Vec calcul_pixel(Ray rayon, std::vector<Objet*>& objets, int depth){
    //On commence par chercher s'il y a un point d'intersection entre le rayon et un des objets de la scène
    double tmin = INFINI;
    int closest_object_index = -1;
    double t = INFINI;
    Vec normale_temp;
    Vec normale;
    for(unsigned int i=0; i<objets.size(); i++){
        if (objets[i]->intersect(rayon, &t, &normale_temp) && t<tmin){
            tmin = t;
            closest_object_index = i;
            normale = normale_temp;
        }
    }



    //si pas d'objet sur le parcours du rayon, on renvoie la couleur d'arrière-plan
    if(closest_object_index == -1){
        return grisclair;
    }

    Objet *closest_object = objets[closest_object_index]; 
    Vec point_intersection = rayon.origine + rayon.direction*tmin;
    Vec pixel_color(0,0,0);

    //pour contrer l'"acnée de reflexion"
    float bias = 1e-4;
    if ((closest_object->transparence > 0 || closest_object->reflectivite > 0) && depth < max_depth) { 
        
        if(rayon.direction.dot(normale)<0){
            normale = -normale;
        }

        /*if (depth >0){
        std :: cout << "reflexion de profondeur " << depth << std :: endl;}*/
        float facingratio = rayon.direction.dot(normale); 
        // change the mix value to tweak the effect
        float fresneleffect = mix(pow(1 - facingratio, 3), 1, 0.1);
        //std :: cout << fresneleffect << std :: endl;
        // compute reflection direction (not need to normalize because all vectors
        // are already normalized)
        Vec reflecdir = rayon.direction - normale * 2 * rayon.direction.dot(normale); 
        reflecdir.normalize(); 
        Ray new_reflec_rayon(point_intersection-normale*bias, reflecdir);
        Vec reflection = calcul_pixel(new_reflec_rayon, objets, depth+1); 

        if (closest_object->reflectivite > 0)
            pixel_color +=  (closest_object->couleur*(1-closest_object->reflectivite) + reflection * closest_object->reflectivite)*fresneleffect;
        
        if (closest_object->transparence > 0)
        {
            bool inside = false;
            normale = - normale;
            if(rayon.direction.dot(normale)>0){
                normale = -normale;
                inside = true;
            }

            float ior = 1.1, eta = (inside) ? ior : 1 / ior;
            float cosi = -normale.dot(rayon.direction); 
            float k = 1 - eta * eta * (1 - cosi * cosi); 
            Vec refracdir = rayon.direction * eta + normale * (eta *  cosi - sqrt(k)); 
            refracdir.normalize();
            Ray new_refrac_rayon(point_intersection-normale*bias, refracdir);
            Vec refraction = calcul_pixel(new_refrac_rayon, objets, depth+1);
            pixel_color += (refraction * closest_object->transparence + closest_object->couleur*(1-closest_object->transparence))* (1 - fresneleffect) ;
        }
    }

    else{
        pixel_color = closest_object->couleur;// *abs(normale.dot(rayon.direction));
    }

    //eclaircissement de l'image temporaire
    double facteur_eclaircissement = 1.7;
    //pixel_color.x = std::min(255.0, pixel_color.x*facteur_eclaircissement);
    //pixel_color.y = std::min(255.0, pixel_color.y*facteur_eclaircissement);
    //pixel_color.z = std::min(255.0, pixel_color.z*facteur_eclaircissement);

    return pixel_color; //*abs(normale.dot(rayon.direction));
}

void rendu(std::vector<Objet *> objets, int image_width, int image_height, int fov, const std::string &filename){
    Picture scene(image_width, image_height);

    Camera camera(image_width, image_height, fov);

    //calcul des pixels
    for(int j=0; j<image_height; j++){
        for(int i=0; i<image_width; i++){
            double u = i/(double)(image_width-1);
            double v = j/(double)(image_height-1);
            Ray rayon_incident = camera.getRay(u,v);
            scene.pixels[j*image_width + i] = calcul_pixel(rayon_incident, objets,0);
        }
    }
    scene.savePicture("test_savePicture.png", 2);
}


int main()
{

    //Test de la classe image
    // Image
    
    //const int image_width = 640;
    //const int image_height = 360;

    const int image_width = 1920;
    const int image_height = 1080;

    //16K
    //const int image_width = 15360;
    //const int image_height = 8640;



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
    //objets.push_back( new Sphere(Vec(-3.0, 0.0, -20.0), rouge, 0.5, 0.5, 2.0));
    //objets.push_back( new Sphere(Vec(3.0, -1.0, -5.0), Vec(255,0,187), 0.5, 0.5, 0.5));
    //objets.push_back( new Sphere(Vec(1.0, 2, -35.0), bleu, 0.5, 0.5, 2.0));
    //objets.push_back( new Sphere(Vec(0.0, 5, -20.0), blanc, 0.5, 0.5, 2.0));
    //objets.push_back( new Sphere(Vec(0.0, -100.5, 0.0), vert, 0.5, 0.0, 100.0));
    //objets.push_back( new Sphere(Vec(-3.0, 10.0, -20.0), Vec(255.0, 0.0, 150.0), 0.0, 0.0, 1.0));

    //objets.push_back( new Plan(Vec(0.0, 1.0, -35.0), vert, 0.0, 0.0, Vec(3.0, 0.0, -5.0), Vec(0.0, 3.0, 0.0)));
    //objets.push_back( new Plan(Vec(0.0, 0.0, -35.0), bleu, 0.0, 0.0, Vec(-3.0, 0.0, -5.0), Vec(0.0, 3.0, 0.0)));
    objets.push_back( new Plan(Vec(0.0, -2.0, -30.0), Vec(0, 150, 150), 0.5, 0.5, Vec(15.0, 0.0, 10.0), Vec(0.0, 5.0, 0.0)));
    objets.push_back( new Plan(Vec(-8.0, -3.0, -2.0), vert, 0.5, 0.5, Vec(15, 0.0, 0.0), Vec(0.0, 0.0, -25.0)));
    //objets.push_back( new Plan);
    //objets.push_back( new Plan(Vec(0.0, 0.0, -25.0), rouge, 0.9, 0.9, Vec(0.0, 2.0, -2.0), Vec(2.0, 0.0, 0.0)));


    //objets.push_back( new Parallelepipede(Vec(-4.0, 2.0,-5.0), bleu, 0.2, 0.8, Vec(2.5*sqrt(2), 0.0, -2.5*sqrt(2)).prod_vec(Vec(-2.5*sqrt(2), -3.0, -2.5*sqrt(2)))*0.25, Vec(-2.5*sqrt(2), -3.0, -2.5*sqrt(2)), Vec(2.5*sqrt(2), 0.0, -2.5*sqrt(2))));
    
    objets.push_back( new Disque(Vec(-3.0, 2.0, -25.0), rouge, 0.9, 0.2, 3.0, Vec(1.0, 0, 0.2)));

    objets.push_back( new Cylindre(Disque(Vec(2, 1, -25), bleu, 0.5, 0.01, 3.0, Vec(0.0, -1.0, -1.0)), 3.0));
    //objets.push_back( new Cylindre(Disque(Vec(-3, 1, -25), bleu, 0.5, 0.0, 1.0, Vec(-1.0, 0.0, 0.0)), 5.0));

    //objets.push_back( new Cylindre(Disque(Vec(0, 0, -25), bleu, 0.7, 0.5, Vec(3.0, 0.0, 0.0), Vec(0.0, -1.0, 0.0)), 3.0));
    

    int fov = 90;
    rendu(objets, image_width, image_height, fov, "premier_test.ppm");
    for(unsigned int i=0; i<objets.size(); i++){
        delete objets[i];
    }

    //fin du test
}