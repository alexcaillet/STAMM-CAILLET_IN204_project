#ifndef OBJETHPP
#define OBJETHPP

#include "vecteur.hpp"
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

	virtual bool intersect(Vec origine_rayon, Vec dir_rayon, Vec *point_intersection)
	{
		return false;
	}
	//Méthode normale(), virtuelle pure pour s'adapter à chaque type d'objet
	virtual Vec normale(Vec point_intersection)
	{
		return Vec(0.0, 0.0, 0.0);
	}

	/*~Objet()
    {}*/
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

	/*Calcul s'il y a une intersection du rayon avec la sphère
	La direction du rayon est un vecteur unitaire
	*/
	bool intersect(Vec origine_rayon, Vec dir_rayon, Vec *point_intersection) override
	{
		auto a = dir_rayon.dot(dir_rayon);
		auto b = dir_rayon.dot(origine_rayon - position) * 2.0;
		auto c = (origine_rayon - position).norme2() - radius * radius;
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
					auto t = std::min(t1, t2);
					*point_intersection = origine_rayon + dir_rayon * t;
					return true;
				}
			}
		}
		return false;
	}

	/*Retourne la normale à la surface au point d'intersection*/
	Vec normale(Vec point_intersection) override
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

	bool intersect(Vec origine_rayon, Vec dir_rayon, Vec *point_intersection) override
	{
	}

	/*Retourne la normale à la surface au point d'intersection*/
	Vec normale(Vec point_intersection) override
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
	bool intersect(Vec origine_rayon, Vec dir_rayon, Vec *point_intersection) override
	{
		auto d = -normalVector.dot(position);
		if (normalVector.dot(dir_rayon) != 0)
		{
			//Il existe un point d'intersection avec le plan
			auto t = -(normalVector.dot(origine_rayon) + d) / normalVector.dot(dir_rayon);
			//vérifions que ce point appartient au plan fini
			Vec intersection = (origine_rayon + dir_rayon * t) - position;
			if (t > 0 && intersection.dot(larg) <= larg.norme() && intersection.dot(longueur) <= longueur.norme())
			{
				*point_intersection = intersection;
				return true;
			}
		}
		return false;
	}

	/*Retourne la normale à la surface au point d'intersection*/
	Vec normale(Vec point_intersection) override
	{
		return normalVector;
	}
};

#endif