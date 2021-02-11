#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <fstream>
#include <vector>
#include "vecteur.hpp"

class Picture
{
 public:
    const int w; //largeur
    const int h; //hauteur
    std::vector<Vec> pixels; //valeurs comprises entre 0 et 255

    Picture(int largeur, int hauteur) : w(largeur), h(hauteur){
        pixels.resize(w*h);
    };
    ~Picture(){};

    /** sauvegarde l'image finale **/
    void savePicture(const std::string &filename)
    {
        //double scaleCol = 1. / maxIter; //16777216

        std::ofstream ofs(filename.c_str(), std::ios::out | std::ios::binary);
        ofs << "P3\n" << w << " " << h << "\n255\n";
        for (int i = 0; i < w * h; ++i)
        {
            int r = (int)pixels[i].x;
            int b = (int)pixels[i].y;
            int g = (int)pixels[i].z;
            ofs << r << ' ' << b << ' ' << g << std::endl;
        }
        ofs.close();
    }
};

#endif