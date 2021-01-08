#include <iostream>
#include "vecteur.hpp"
#include "tinyxml2.h"
#include "objet.hpp"
#include "vector"

//L'objet de ce fichier est de définir les fonctions de lecture de fichiers XML
std::vector<Sphere> readSpheres()
{
	using namespace tinyxml2;
    XMLDocument doc;
    std::vector<Sphere> spheres; 
    doc.LoadFile( "scene.xml" );
    
    XMLElement *scene = doc.RootElement();

    double ix, iy, iz;
    double iradius;
    double ia, ib, ic;
    double ireflec, itransp;
 
    if( scene )
    {
        XMLElement *objects = scene->FirstChildElement( "objects" );
 
        if ( objects )
        {   
            XMLElement *sphere = objects->FirstChildElement( "sphere" );

     
            while( sphere )
            {


                XMLElement *centre = sphere->FirstChildElement( "centre" );
                if ( centre )
                {
                    const char *x = centre->Attribute("x");
                    const char *y = centre->Attribute("y");
                    const char *z = centre->Attribute("z");

                    if (x)
                        ix=atof(x);
                    if (y)
                        iy=atof(y);
                    if (z)
                        iz=atof(z);
                }

                XMLElement *radius = sphere->FirstChildElement( "radius");
                if (radius)
                    iradius=atof(radius->GetText());
                	//std::cout << radius->GetText() << std::endl;


                XMLElement *scolor = sphere->FirstChildElement( "surfaceColor" );
                if ( scolor )
                {
                    const char *a = scolor->Attribute("a");
                    const char *b = scolor->Attribute("b");
                    const char *c = scolor->Attribute("c");

                    if (a)
                        ia=atof(a);
                        //std::cout << "a : " << a << "\n";
                    if (b)
                        ib=atof(b);
                        //std::cout << "b : " << b << "\n";
                    if (c)
                        ic=atof(c);
                        //std::cout << "c : " << c << "\n";
                }

                XMLElement *reflec = sphere->FirstChildElement( "re");
                if (reflec)
                    ireflec=atof(reflec->GetText());
                	//std::cout << reflec->GetText() << std::endl;


                XMLElement *transp = sphere->FirstChildElement( "tr");
                if (transp)
                    itransp=atof(transp->GetText());
                	//std::cout << transp->GetText() << std::endl;
				
                //spheres.push_back(Sphere(Vec(ix,iy,iz), iradius, Vec(ia, ib, ic), ireflec, itransp)); 

                sphere = sphere->NextSiblingElement( "sphere" );
            }
        }
    }
    return spheres;
}


std::vector<Plan> readPlans()
{
	using namespace tinyxml2;
    XMLDocument doc;
    std::vector<Plan> plans; 
    doc.LoadFile( "scene.xml" );
    
    XMLElement *scene = doc.RootElement();

    double ix, iy, iz;
    double ilongueur, ilargeur;
    double inormalx, inormaly, inormalz;
    double ia, ib, ic;
 
    if( scene )
    {
        XMLElement *objects = scene->FirstChildElement( "objects" );
 
        if ( objects )
        {   
            XMLElement *plan = objects->FirstChildElement( "plan" );

            while( plan )
            {


                XMLElement *point = plan->FirstChildElement( "point" );
                if ( point )
                {
                    const char *x = point->Attribute("x");
                    const char *y = point->Attribute("y");
                    const char *z = point->Attribute("z");

                    if (x)
                        ix=atof(x);
                    if (y)
                        iy=atof(y);
                    if (z)
                        iz=atof(z);
                }

                XMLElement *longueur = plan->FirstChildElement( "longueur");
                if (longueur)
                    ilongueur=atof(longueur->GetText());
                	//std::cout << radius->GetText() << std::endl;
				
				XMLElement *normal = plan->FirstChildElement( "normal" );
                if ( normal )
                {
                    const char *normalx = normal->Attribute("normalx");
                    const char *normaly = normal->Attribute("normaly");
                    const char *normalz = normal->Attribute("normalz");

                    if (normalx)
                        inormalx=atof(normalx);
                        //std::cout << "a : " << a << "\n";
                    if (normaly)
                        inormaly=atof(normaly);
                        //std::cout << "b : " << b << "\n";
                    if (normalz)
                        inormalz=atof(normalz);
                        //std::cout << "c : " << c << "\n";
                }

                XMLElement *scolor = plan->FirstChildElement( "surfaceColor" );
                if ( scolor )
                {
                    const char *a = scolor->Attribute("a");
                    const char *b = scolor->Attribute("b");
                    const char *c = scolor->Attribute("c");

                    if (a)
                        ia=atof(a);
                        //std::cout << "a : " << a << "\n";
                    if (b)
                        ib=atof(b);
                        //std::cout << "b : " << b << "\n";
                    if (c)
                        ic=atof(c);
                        //std::cout << "c : " << c << "\n";
                }
				
               	//plans.push_back(Sphere(Vec(ix,iy,iz), iradius, Vec(ia, ib, ic), ireflec, itransp)); 

                plan = plan->NextSiblingElement( "plan" );
            }
        }
    }
    return plans;
}


int main(int argc, char const *argv[])
{
	readSpheres();
	readPlans();
	return 0;
}