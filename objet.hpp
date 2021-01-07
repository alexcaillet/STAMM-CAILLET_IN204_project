#ifndef OBJETHPP
#define OBJETHPP

#include "vecteur.hpp"
class Objet{
    Vec couleur;

    //Tout objet a besoin d'être défini par sa position, 
    //même si elle ne désigne pas toujours le même point (centre du cercle, point d'intéret...)
    Vec position;
    double reflectivite;
    double transparence; 
    double reflection; 

    //Méthod normale()
    
    
};

//Définition d'un object classique : La sphère
class Sphere : public Objet {
public:
    double radius; //rayon du cercle
};


class Parallelepipede : public Objet{
public:
	double hauteur;
	double longueur;
	double larg;
	Vec orientation; 
	/* Un parallelepipède rectangle ne peut pas être simplement défini par des longueurs et une position
	Il faut également réfléchir à son orienation */
};

class Plan : public Objet
{
public:
	double longueur;
	double larg;

	//On définit aisément une surface par son vecteur normal
	Vec normalVector;
}

#endif