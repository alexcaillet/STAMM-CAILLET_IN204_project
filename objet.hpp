#ifndef OBJETHPP
#define OBJETHPP

#include "vecteur.hpp"
#include "ray.hpp"
#include <stdbool.h>

class Objet
{
public:
	//Tout objet a besoin d'être défini par sa position,
	//même si elle ne désigne pas toujours le même point (centre du cercle, point d'intéret...)
	Vec position;
	Vec couleur;
	double reflectivite;
	double transparence;

	Objet() : position(Vec(0, 0, 0)), couleur(Vec(0, 0, 0)), reflectivite(0.5), transparence(0.5)
	{
	}

	Objet(Vec pos, Vec col, double reflec, double transp) : position(pos), couleur(col), reflectivite(reflec), transparence(transp)
	{
	}

	virtual bool intersect(Ray rayon, double* t)
	{
		return false;
	}
	//Méthode normale(), virtuelle pure pour s'adapter à chaque type d'objet
	virtual Vec normale(Vec point_intersection)
	{
		return Vec(0.0, 0.0, 0.0);
	}

	virtual ~Objet(){}
};

//Définition d'un object classique : La sphère
class Sphere : public virtual Objet
{
public:
	double radius; //rayon du cercle

	Sphere() : Objet(), radius(1)
	{
	}

	Sphere(double rad) : Objet(), radius(rad)
	{
	}

	Sphere(Vec pos, Vec col, double reflec, double transp, double rad) : Objet(pos, col, reflec, transp), radius(rad)
	{
	}

	~Sphere(){};

	/*Calcul s'il y a une intersection du rayon avec la sphère
	La direction du rayon est un vecteur unitaire
	*/
	virtual bool intersect(Ray rayon, double* t)
	{
		auto a = rayon.direction.dot(rayon.direction);
		auto b = rayon.direction.dot(rayon.origine - position) * 2.0;
		auto c = (rayon.origine - position).norme2() - radius * radius;
		auto delta = b * b - a * c * 4.0;

		if (delta >= 0)
		{
			auto t1 = (-b + sqrt(delta)) / (2.0 * a);
			auto t2 = (-b - sqrt(delta)) / (2.0 * a);
			//On cherche le premier point d'intersection entre le rayon et la sphère, t doit être positif
			if (t1 * t2 >= 0)
			{
				if (t1 < 0)
				{
					return false;
				}
				else
				{
					*t = std::min(t1, t2);
					return true;
				}
			}
		}
		return false;
	}

	/*Retourne la normale à la surface au point d'intersection*/
	virtual Vec normale(Vec point_intersection)
	{
		Vec normale = point_intersection - position;
		normale.normalize();
		return normale;
	}
};

class Parallelepipede : public virtual Objet
{
public:
	double hauteur;
	double longueur;
	double larg;
	Vec orientation;
	/* Un parallelepipède rectangle ne peut pas être simplement défini par des longueurs et une position
	Il faut également réfléchir à son orientation */

	Parallelepipede() : Objet(), hauteur(1), longueur(1), larg(1), orientation(Vec(0, 0, 0))
	{
	}

	Parallelepipede(double h, double lo, double la, Vec ori) : Objet(), hauteur(h), longueur(lo), larg(la), orientation(ori)
	{
	}

	Parallelepipede(Vec pos, Vec col, double reflec, double transp,
					double h, double lo, double la, Vec ori) : Objet(pos, col, reflec, transp),
															   hauteur(h), longueur(lo), larg(la), orientation(ori)
	{
	}

	virtual bool intersect(Ray rayon, double* t)
	{
	}

	/*Retourne la normale à la surface au point d'intersection*/
	virtual Vec normale(Vec point_intersection)
	{
	}
};

class Plan : public virtual Objet
{
public:
	Vec longueur;
	Vec larg;
	//On définit aisément une surface par son vecteur normal
	Vec normalVector;

	Plan() : Objet(), longueur(Vec(1, 0, 0)), larg(Vec(0, 1, 0)), normalVector(Vec(0, 0, 1))
	{
	}

	Plan(Vec lo, Vec la, Vec norm) : Objet(), longueur(lo), larg(la), normalVector(norm)
	{
	}

	Plan(Vec pos, Vec col, double reflec, double transp, Vec lo, Vec la, Vec norm) : Objet(pos, col, reflec, transp),
																					 longueur(lo), larg(la), normalVector(norm)
	{
	}

	/*Calcul s'il y a une intersection du rayon avec le plan
	La direction du rayon est un vecteur unitaire
	*/
	virtual bool intersect(Ray rayon, double* t)
	{
		auto d = -normalVector.dot(position);
		if (normalVector.dot(rayon.direction) != 0)
		{
			//Il existe un point d'intersection avec le plan
			auto t1 = -(normalVector.dot(rayon.origine) + d) / normalVector.dot(rayon.direction);
			//vérifions que ce point appartient au plan fini
			Vec intersection = (rayon.origine + rayon.direction * t1) - position;
			if (t > 0 && intersection.dot(larg) <= larg.norme() && intersection.dot(longueur) <= longueur.norme())
			{
				*t = t1;
				return true;
			}
		}
		return false;
	}

	/*Retourne la normale à la surface au point d'intersection*/
	virtual Vec normale(Vec point_intersection)
	{
		return normalVector;
	}
};

#endif