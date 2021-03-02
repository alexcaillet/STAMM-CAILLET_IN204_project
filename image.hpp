#ifndef IMAGE_HPP
#define IMAGE_HPP

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <fstream>
#include <vector>
#include "vecteur.hpp"
#include "stb-master/stb_image_write.h"

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

    /** Sauvegarde de l'image finale au format choisi
     * ppm | 0
     * jpeg | 1
     */
    void savePicture(char const *filename, unsigned int format){
        switch (format)
        {
        case 0:
            savePicturePPM(filename);
            break;
        case 1:
            savePictureJpeg(filename);
            break;
        default:
            std::cerr << "Erreur : le format demandé est incorrect. Choisissez ppm, jpeg ou png \n";
            break;
        }
    }

private:
    /** sauvegarde l'image finale au format .ppm**/
    void savePicturePPM(const std::string &filename)
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

    /**Sauvegarde l'image au format jpeg **/
    void savePictureJpeg(char const *filename){
        unsigned char* img = (unsigned char*)malloc(w*h*3*sizeof(char));
        for(int i=0; i<w*h; i++){
            img[3*i]=(char)(int)pixels[i].x;
            img[3*i+1]=(char)(int)pixels[i].y;
            img[3*i+2]=(char)(int)pixels[i].z;
        }
        stbi_write_jpg(filename, w, h, 3, img, 100);
        free(img);
    }


};

#endif