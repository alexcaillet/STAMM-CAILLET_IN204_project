#ifndef XMLREAD_HPP
#define XMLREAD_HPP

#include <iostream>
#include "vecteur.hpp"
#include "XML/tinyxml2.h"
#include "objet.hpp"
#include "vector"
#include "camera.hpp"

//L'objet de ce fichier est de définir les fonctions de lecture de fichiers XML
using namespace tinyxml2;

void xyz(float *x, float *y, float *z, XMLElement *element);

void baseAttributes(float *xp, float *yp, float *zp,
                    float *xc, float *yc, float *zc,
                    float *xe, float *ye, float *ze,
                    float *re, float *tr, XMLElement *element);


std::vector<Sphere> readSpheres(const char* filename);



std::vector<Plan> readPlans(const char* filename);



std::vector<Parallelepipede> readParalls(const char* filename);


std::vector<Disque> readDisques(const char* filename);


std::vector<Cylindre> readCylindres(const char* filename);

/*
Camera readCamera();*/



std :: vector <Objet> read(const char* filename);

#endif