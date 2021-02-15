#ifndef OBJETHPP
#define OBJETHPP

#include "vecteur.hpp"
#include "ray.hpp"
#include <stdbool.h>

#define INFINI 1e8

class Objet
{
public:
	//Tout objet a besoin d'être défini par sa position,
	//même si elle ne désigne pas toujours le même point (centre du cercle, point d'intéret...)
	Vec position;
	Vec couleur;
	double reflectivite;
	double transparence;

	Objet() : position(Vec(0, 0, 0)), couleur(Vec(255.0, 0.0, 0.0)), reflectivite(0.5), transparence(0.5)
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

class Plan : public virtual Objet
{
public:
	Vec longueur;
	Vec largeur;
	//On définit aisément une surface par son vecteur normal
	Vec normalVector;

	Plan() : Objet(), longueur(Vec(1, 0, 0)), largeur(Vec(0, 1, 0)), normalVector(Vec(0, 0, 1)) {}

	Plan(Vec lo, Vec la) : Objet(), longueur(lo), largeur(la){
		normalVector = longueur.prod_vec(largeur);
		normalVector.normalize();
	}

	Plan(Vec pos, Vec col, double reflec, double transp, Vec lo, Vec la) : Objet(pos, col, reflec, transp),longueur(lo), largeur(la){
		normalVector = longueur.prod_vec(largeur);
		normalVector.normalize();
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
			//Vérifions que ce point appartient au plan fini
			Vec intersection = (rayon.origine + rayon.direction * t1) - position;
			if (*t > 0 && intersection.dot(largeur) <= largeur.norme() && intersection.dot(longueur) <= longueur.norme())
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
		return -normalVector;
	}
};

class Parallelepipede : public virtual Objet
{
public:
	Vec hauteur;
	Vec longueur;
	Vec largeur;
	/* Un parallelepipède rectangle ne peut pas être simplement défini par des longueurs et une position
	Il faut également réfléchir à son orientation */

	Parallelepipede() : Objet(), hauteur(Vec(0.0, 1.0, 0.0)), longueur(Vec(0.0, 0.0, 1.0)), largeur(Vec(1.0, 0.0, 0.0)) {}

	Parallelepipede(Vec h, Vec lo, Vec la) : Objet(), hauteur(h), longueur(lo), largeur(la) {}

	Parallelepipede(Vec pos, Vec col, double reflec, double transp, Vec h, Vec lo, Vec la) : Objet(pos, col, reflec, transp),hauteur(h), longueur(lo), largeur(la) {}


	/*Renvoie s'il existe une intersection avec le parallelepipe et renvoie le parametre permettant de calculer le point d'intersection*/
	virtual bool intersect(Ray rayon, double* t)
	{
		//On définit les 6 plans formant le parallelepipede
		Plan dessous(position, couleur, reflectivite, transparence, longueur, largeur);
		Plan dessus(position+hauteur, couleur, reflectivite, transparence, longueur, largeur);
		Plan face_lat1(position, couleur, reflectivite, transparence, longueur, hauteur);
		Plan face_lat2(position+longueur, couleur, reflectivite, transparence, largeur, hauteur);
		Plan face_lat3(position+longueur+largeur, couleur, reflectivite, transparence, -longueur, hauteur);
		Plan face_lat4(position, couleur, reflectivite, transparence, largeur, hauteur);

		std::vector<Plan *> faces;
		faces.push_back(&dessous);
		faces.push_back(&dessus);
		faces.push_back(&face_lat1);
		faces.push_back(&face_lat2);
		faces.push_back(&face_lat3);
		faces.push_back(&face_lat4);

		//On regarde s'il existe des intersections avec l'extéieur de ces plans
		double tmin = INFINI;
    	int closest_face = -1;
    	double tloc = INFINI;
		for(unsigned int i=0; i<faces.size(); i++){
			if (faces[i]->intersect(rayon, &tloc) && tloc<tmin){
				tmin = tloc;
				closest_face = i;
			}
		}
		if (tmin != INFINI){
			*t = tmin;
			return true;
		}
		return false;
	}

	/*Retourne la normale à la surface au point d'intersection*/
	virtual Vec normale(Vec point_intersection){

	}
};

#endif