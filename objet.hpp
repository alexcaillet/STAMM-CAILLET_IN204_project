#ifndef OBJETHPP
#define OBJETHPP

#include "vecteur.hpp"
class Objet{
public:

    //Tout objet a besoin d'être défini par sa position, 
    //même si elle ne désigne pas toujours le même point (centre du cercle, point d'intéret...)
	Vec position;
    Vec couleur;
    double reflectivite;
    double transparence; 

	Objet (): 
		position(Vec(0,0,0)), couleur(Vec(0,0,0)), reflectivite(0.5), transparence(0.5)
		{}

	Objet (Vec pos, Vec col,double reflec, double transp):
		position(pos), couleur(col), reflectivite(reflec), transparence(transp)
		{}

    //Méthode normale(), virtuelle pure pour s'adapter à chaque type d'objet
    //virtual Vec normale(Vec) = 0;

    /*~Objet()
    {}*/
    
    
};

//Définition d'un object classique : La sphère
class Sphere : public virtual Objet {
public:
    double radius; //rayon du cercle

	Sphere():
		Objet(), radius(1)
		{}

	Sphere(double rad):
		Objet(), radius(rad)
		{}

	Sphere(Vec pos, Vec col,double reflec, double transp, double rad):
		Objet (pos, col, reflec, transp), radius(rad)
		{}


   // virtual Vec normale(Vec collidePosition) = 0;

};

class Parallelepipede : public virtual Objet{
public:
	double hauteur;
	double longueur;
	double larg;
	Vec orientation; 
	/* Un parallelepipède rectangle ne peut pas être simplement défini par des longueurs et une position
	Il faut également réfléchir à son orientation */

	Parallelepipede():
		Objet(), hauteur(1), longueur(1), larg(1), orientation(Vec(0,0,0))
		{}

	Parallelepipede(double h, double lo, double la, Vec ori):
		Objet(), hauteur(h), longueur(lo), larg(la), orientation(ori)
		{}

	Parallelepipede(Vec pos, Vec col, double reflec, double transp,
			double h, double lo, double la, Vec ori):
		Objet (pos, col, reflec, transp),
		hauteur(h), longueur(lo), larg(la), orientation(ori)
		{}


	//virtual Vec normale(Vec collidePosition) = 0;
};

class Plan : public virtual Objet
{
public:
	double longueur;
	double larg;
	//On définit aisément une surface par son vecteur normal
	Vec normalVector;

	Plan():
		Objet(), longueur(1), larg(1), normalVector(Vec(0,1,0))
		{}

	Plan(double lo, double la, Vec norm):
		Objet(), longueur(lo), larg(la), normalVector(norm)
		{}

	Plan(Vec pos, Vec col, double reflec, double transp, double lo, double la, Vec norm):
		Objet (pos, col, reflec, transp),
		longueur(lo), larg(la), normalVector(norm) 
		{}	


	//Ici, pas besoin de calcul, on connaît toujours la normale au plan
	/*virtual Vec normale(Vec collidePosition)
	{
		return normalVector;
	}*/
};

#endif