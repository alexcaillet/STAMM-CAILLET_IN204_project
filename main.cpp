#include <iostream>
#include <stdbool.h>
#include "vecteur.hpp"
#include "image.hpp"
#include "objet.hpp"

int main()
{

    //Test de la classe image
    // Image

    const int image_width = 256;
    const int image_height = 256;

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


    Sphere test_sphere;
    Vec dir_rayon(-1.0,0.0,0.0);
    Vec origine_rayon(5.0, 0.0, 0.0);
    Vec point_intersection(0.0, 0.0, 0.0);
    bool intersection = test_sphere.intersect(origine_rayon, dir_rayon, &point_intersection);

    std::cout << "intersection sphere : " << intersection << "   point : " << point_intersection.x << " " << point_intersection.y << " " << point_intersection.z <<std::endl;

    Plan test_plan;
    Vec dir_rayon2(0.0, 0.0, 1.0);
    Vec origine_rayon2(0.5, 0.0, -1.0);
    Vec point_intersection2(0.0, 0.0, 0.0);
    bool intersection2 = test_plan.intersect(origine_rayon2, dir_rayon2, &point_intersection2);
    
    std::cout << "intersection plan : " << intersection2 << "   point : " << point_intersection2.x << " " << point_intersection2.y << " " << point_intersection2.z << std::endl;
    //fin du test
}