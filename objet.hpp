#ifndef OBJETHPP
#define OBJETHPP

#include "vecteur.hpp"
class Objet{

    //Tout objet a besoin d'être défini par sa position, 
    //même si elle ne désigne pas toujours le même point (centre du cercle, point d'intéret...)
    Vec position;
    Vec couleur;
    double reflectivite;
    double transparence; 

    //Méthode normale(), virtuelle pure pour s'adapter à chaque type d'objet
    virtual Vec normale(Vec) = 0;



    /*~Objet()
    {}*/
    
    
};

//Définition d'un object classique : La sphère
class Sphere : public virtual Objet {
public:
    double radius; //rayon du cercle
    virtual Vec normale(Vec collidePosition) = 0;

};


class Parallelepipede : public virtual Objet{
public:
	double hauteur;
	double longueur;
	double larg;
	Vec orientation; 
	/* Un parallelepipède rectangle ne peut pas être simplement défini par des longueurs et une position
	Il faut également réfléchir à son orientation */

	virtual Vec normale(Vec collidePosition) = 0;
};

class Plan : public virtual Objet
{
public:
	double longueur;
	double larg;

	//On définit aisément une surface par son vecteur normal
	Vec normalVector;

	//Ici, pas besoin de calcul, on connaît toujours la normale au plan
	virtual Vec normale(Vec collidePosition)
	{
		return normalVector;
	}
};

#endif