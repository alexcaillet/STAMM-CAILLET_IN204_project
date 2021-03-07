#ifndef IMAGE_HPP
#define IMAGE_HPP



#include <fstream>
#include <vector>
#include "vecteur.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb-master/stb_image_write.h"
//#if defined(QT_VERSION)
//#include <QString>
//include <QImage>
//#endif

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
     * png | 2
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
        case 2:
            savePicturePNG(filename);
            break;
        default:
            std::cout << "Erreur : le format demandé est incorrect. Choisissez ppm(0), jpeg(1) ou png(2) \n";
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
        //stbi_write_jpg(filename, w, h, 3, img, 100);
        QImage *monimage = new QImage(img, w, h, QImage::Format_RGB888);
        monimage->save(QString(filename), "jpeg");
        free(img);
    }

    /**Sauvegarde l'image au format png **/
    void savePicturePNG(char const *filename){
        unsigned char* img = (unsigned char*)malloc(w*h*3*sizeof(char));
        for(int i=0; i<w*h; i++){
            img[3*i]=(char)(int)pixels[i].x;
            img[3*i+1]=(char)(int)pixels[i].y;
            img[3*i+2]=(char)(int)pixels[i].z;
        }
        stbi_write_png(filename, w, h, 3, img, w*3);
        //#if defined(QT_VERSION)
       // QImage *monimage = new QImage(img, w, h, QImage::Format_RGB888);
        //monimage->save(QString(filename), "png");
        //#endif
        free(img);
    }


};

#endif
