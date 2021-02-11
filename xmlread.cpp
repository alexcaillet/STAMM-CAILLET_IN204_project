#include <iostream>
#include "vecteur.hpp"
#include "XML/tinyxml2.h"
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


                XMLElement *position = sphere->FirstChildElement( "position" );
                if ( position )
                {
                    const char *x = position->Attribute("x");
                    const char *y = position->Attribute("y");
                    const char *z = position->Attribute("z");

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
				
                spheres.push_back(Sphere(Vec(ix,iy,iz), Vec(ia, ib, ic), ireflec, itransp, iradius)); 

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
    double ireflec, itransp;
 
    if( scene )
    {
        XMLElement *objects = scene->FirstChildElement( "objects" );
 
        if ( objects )
        {   
            XMLElement *plan = objects->FirstChildElement( "plan" );

            while( plan )
            {


                XMLElement *position = plan->FirstChildElement( "position" );
                if ( position )
                {
                    const char *x = position->Attribute("x");
                    const char *y = position->Attribute("y");
                    const char *z = position->Attribute("z");

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

                XMLElement *largeur = plan->FirstChildElement( "largeur");
                if (largeur)
                    ilargeur=atof(largeur->GetText());
                    //std::cout << radius->GetText() << std::endl;
				
				XMLElement *normal = plan->FirstChildElement( "normal" );
                if ( normal )
                {
                    const char *x = normal->Attribute("x");
                    const char *y = normal->Attribute("y");
                    const char *z = normal->Attribute("z");

                    if (x)
                        inormalx=atof(x);
                        //std::cout << "a : " << a << "\n";
                    if (y)
                        inormaly=atof(y);
                        //std::cout << "b : " << b << "\n";
                    if (z)
                        inormalz=atof(z);
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

                XMLElement *reflec = plan->FirstChildElement( "re");
                if (reflec)
                    ireflec=atof(reflec->GetText());
                    //std::cout << reflec->GetText() << std::endl;


                XMLElement *transp = plan->FirstChildElement( "tr");
                if (transp)
                    itransp=atof(transp->GetText());
                    //std::cout << transp->GetText() << std::endl;
				
               	plans.push_back(Plan(Vec(ix,iy,iz), Vec(ia, ib, ic), ireflec, itransp,
                                ilongueur, ilargeur, Vec(inormalx, inormaly, inormalz)));

                plan = plan->NextSiblingElement( "plan" );
            }
        }
    }
    return plans;
}


std::vector<Parallelepipede> readParall()
{
    using namespace tinyxml2;
    XMLDocument doc;
    std::vector<Parallelepipede> paralls; 
    doc.LoadFile( "scene.xml" );
    
    XMLElement *scene = doc.RootElement();

    double ix, iy, iz;
    double ilongueur, ilargeur, ihauteur;
    double ia, ib, ic;
    double ireflec, itransp;
    double iorix, ioriy, ioriz;

 
    if( scene )
    {
        XMLElement *objects = scene->FirstChildElement( "objects" );
 
        if ( objects )
        {   
            XMLElement *parall = objects->FirstChildElement( "parallelepipede" );

            while( parall )
            {


                XMLElement *position = parall->FirstChildElement( "position" );
                if ( position )
                {
                    const char *x = position->Attribute("x");
                    const char *y = position->Attribute("y");
                    const char *z = position->Attribute("z");

                    if (x)
                        ix=atof(x);
                    if (y)
                        iy=atof(y);
                    if (z)
                        iz=atof(z);
                }

                XMLElement *longueur = parall->FirstChildElement( "longueur");
                if (longueur)
                    ilongueur=atof(longueur->GetText());
                    //std::cout << radius->GetText() << std::endl;

                XMLElement *largeur = parall->FirstChildElement( "largeur");
                if (largeur)
                    ilargeur=atof(largeur->GetText());
                    //std::cout << radius->GetText() << std::endl;
                
                XMLElement *hauteur = parall->FirstChildElement( "hauteur");
                if (hauteur)
                    ihauteur=atof(hauteur->GetText());
                    //std::cout << radius->GetText() << std::endl;

                XMLElement *orientation = parall->FirstChildElement( "orientation" );
                if ( orientation )
                {
                    const char *x = orientation->Attribute("x");
                    const char *y = orientation->Attribute("y");
                    const char *z = orientation->Attribute("z");

                    if (x)
                        iorix=atof(x);
                        //std::cout << "a : " << a << "\n";
                    if (y)
                        ioriy=atof(y);
                        //std::cout << "b : " << b << "\n";
                    if (z)
                        ioriz=atof(z);
                        //std::cout << "c : " << c << "\n";
                    
                }

                XMLElement *scolor = parall->FirstChildElement( "surfaceColor" );
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

                XMLElement *reflec = parall->FirstChildElement( "re");
                if (reflec)
                    ireflec=atof(reflec->GetText());
                    //std::cout << reflec->GetText() << std::endl;


                XMLElement *transp = parall->FirstChildElement( "tr");
                if (transp)
                    itransp=atof(transp->GetText());
                    //std::cout << transp->GetText() << std::endl;
                
                paralls.push_back(Parallelepipede(Vec(ix,iy,iz), Vec(ia, ib, ic), ireflec, itransp,
                                ihauteur, ilongueur, ilargeur, Vec(iorix, ioriy, ioriz)));

                parall = parall->NextSiblingElement( "parallelepipede" );
            }
        }
    }
    return paralls;
}


int main(int argc, char const *argv[])
{

	std::vector<Sphere> spheres = readSpheres();
	std::vector<Plan> plans = readPlans(); 
    std::vector<Parallelepipede> paralls = readParall();
	return 0;
}