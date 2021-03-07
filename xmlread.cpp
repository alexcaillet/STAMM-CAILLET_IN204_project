#include <iostream>
#include "vecteur.hpp"
#include "XML/tinyxml2.h"
#include "objet.hpp"
#include "vector"
#include "camera.hpp"

//L'objet de ce fichier est de définir les fonctions de lecture de fichiers XML
using namespace tinyxml2;

void xyz(float *x, float *y, float *z, XMLElement *element)
{   
    if (element)
    {
        const char *cx = element->Attribute("x");
        const char *cy = element->Attribute("y");
        const char *cz = element->Attribute("z");
        //std :: cout << cz << std :: endl;
        if (cx)
            *x = atof(cx);
        if (cy)    
            *y = atof(cy);
        if (cz)    
            *z = atof(cz);
        }
}

void baseAttributes(float *xp, float *yp, float *zp,
                    float *xc, float *yc, float *zc,
                    float *xe, float *ye, float *ze,
                    float *re, float *tr, XMLElement *element)
{
    //POSITION DE LA SPHERE
    XMLElement *position = element->FirstChildElement( "position" );
    if (position)
        xyz(xp,yp,zp,position);

    //COULEUR DE LA SPHERE
    XMLElement *couleur = element->FirstChildElement( "couleur" );
    if ( couleur )
        xyz(xc,yc,zc,couleur);

    XMLElement *emission = element->FirstChildElement( "emission" );
    if ( emission )
        xyz(xe,ye,ze,emission);
    else
        *xe=0,*ye=0,*ze=0;

    XMLElement *reflec = element->FirstChildElement( "re");
    //std :: cout << reflec->GetText() << std :: endl;
    if (reflec)
        *re=atof(reflec->GetText());

    XMLElement *transp = element->FirstChildElement( "tr");
    if (transp)
        *tr=atof(transp->GetText());

}


std::vector<Sphere> readSpheres(const char* filename)
{
    XMLDocument doc;
    std::vector<Sphere> spheres; 
    doc.LoadFile(filename);
    
    XMLElement *scene = doc.RootElement();
    double iradius;
    float xp,yp,zp;
    float xc,yc,zc;
    float xe,ye,ze;
    float re,tr;
    
    if( scene )
    {
        XMLElement *objets = scene->FirstChildElement( "objets" );
 
        if ( objets )
        {   
            XMLElement *sphere = objets->FirstChildElement( "sphere" );

     
            while( sphere )
            {
                baseAttributes(&xp,&yp,&zp,&xc,&yc,&zc,&xe,&ye,&ze,&re,&tr,sphere);

                XMLElement *rayon = sphere->FirstChildElement( "rayon");
                if (rayon)
                    iradius=atof(rayon->GetText());

                spheres.push_back(Sphere(Vec(xp,yp,zp), Vec(xc,yc,zc), re, tr, Vec(xe,ye,ze),iradius)); 

                sphere = sphere->NextSiblingElement( "sphere" );
            }
        }
    }
    return spheres;
}



std::vector<Plan> readPlans(const char* filename)
{
    XMLDocument doc;
    std::vector<Plan> plans; 
    doc.LoadFile(filename);
    
    XMLElement *scene = doc.RootElement();

    float xp,yp,zp;
    float xc,yc,zc;
    float xe,ye,ze;
    float re,tr;
 
    if( scene )
    {
        XMLElement *objets = scene->FirstChildElement( "objets" );
 
        if ( objets )
        {   
            XMLElement *plan = objets->FirstChildElement( "plan" );

            while( plan )
            {
                baseAttributes(&xp,&yp,&zp,&xc,&yc,&zc,&xe,&ye,&ze,&re,&tr,plan);

                XMLElement *longueur = plan->FirstChildElement( "longueur");
                float xlo,ylo,zlo;
                if (longueur)
                    xyz(&xlo,&ylo,&zlo,longueur);

                XMLElement *largeur = plan->FirstChildElement( "largeur");
                float xla,yla,zla;
                if (largeur)
                    xyz(&xla,&yla,&zla,largeur);
				
               	plans.push_back(Plan(Vec(xp,yp,zp), Vec(xc, yc, zc), re, tr, Vec(xe,ye,ze),
                                     Vec(xlo,ylo,zlo), Vec(xla,yla,zla)));

                plan = plan->NextSiblingElement( "plan" );
            }
        }
    }
    return plans;
}



std::vector<Parallelepipede> readParalls(const char* filename)
{
    XMLDocument doc;
    std::vector<Parallelepipede> paralls; 
    doc.LoadFile(filename);
    
    XMLElement *scene = doc.RootElement();

    float xp,yp,zp;
    float xc,yc,zc;
    float xe,ye,ze;
    float re,tr;

 
    if( scene )
    {
        XMLElement *objets = scene->FirstChildElement( "objets" );
 
        if ( objets )
        {   
            XMLElement *parall = objets->FirstChildElement( "parallelepipede" );

            while( parall )
            {
                baseAttributes(&xp,&yp,&zp,&xc,&yc,&zc,&xe,&ye,&ze,&re,&tr,parall);

                XMLElement *longueur = parall->FirstChildElement( "longueur");
                float xlo,ylo,zlo;
                if (longueur)
                    xyz(&xlo,&ylo,&zlo,longueur);

                XMLElement *largeur = parall->FirstChildElement( "largeur");
                float xla,yla,zla;
                if (largeur)
                    xyz(&xla,&yla,&zla,largeur);

                XMLElement *hauteur = parall->FirstChildElement( "hauteur");
                float xh,yh,zh;
                if (hauteur)
                    xyz(&xh,&yh,&zh,hauteur);
                
                paralls.push_back(Parallelepipede(Vec(xp,yp,zp), Vec(xc, yc, zc), re, tr, Vec(xe,ye,ze),
                                                  Vec(xh,yh,zh), Vec(xlo,ylo,zlo), Vec(xla,yla,zla)));

                parall = parall->NextSiblingElement( "parallelepipede" );
            }
        }
    }
    return paralls;
}


std::vector<Disque> readDisques(const char* filename)
{
    XMLDocument doc;
    std::vector<Disque> disques; 
    doc.LoadFile( filename );
    
    XMLElement *scene = doc.RootElement();
    double iradius;
    float xp,yp,zp;
    float xc,yc,zc;
    float xe,ye,ze;
    float re,tr;
    
    if( scene )
    {
        XMLElement *objets = scene->FirstChildElement( "objets" );
 
        if ( objets )
        {   
            XMLElement *disque = objets->FirstChildElement( "disque" );

     
            while( disque)
            {
                baseAttributes(&xp,&yp,&zp,&xc,&yc,&zc,&xe,&ye,&ze,&re,&tr,disque);

                XMLElement *rayon = disque->FirstChildElement( "rayon");
                if (rayon)
                    iradius=atof(rayon->GetText());

                XMLElement *normale = disque->FirstChildElement( "normale");
                float xn,yn,zn;
                if (normale)
                    xyz(&xn,&yn,&zn,normale);

                disques.push_back(Disque(Vec(xp,yp,zp), Vec(xc,yc,zc), re, tr, Vec(xe,ye,ze),
                                         iradius, Vec(xn,yn,zn))); 

                disque = disque->NextSiblingElement( "disque" );
            }
        }
    }
    return disques;
}

/*
std::vector<Cylindre> readCylindres()
{
    XMLDocument doc;
    std::vector<Cylindre> cylindres; 
    doc.LoadFile( "scene.xml" );
    
    XMLElement *scene = doc.RootElement();
    double iradius;
    float xp,yp,zp;
    float xc,yc,zc;
    float xe,ye,ze;
    float re,tr;
    
    if( scene )
    {
        XMLElement *objets = scene->FirstChildElement( "objets" );
 
        if ( objets )
        {   
            XMLElement *cylindre = objets->FirstChildElement( "cylindre" );

     
            while(cylindre)
            {
                baseAttributes(&xp,&yp,&zp,&xc,&yc,&zc,&xe,&ye,&ze,&re,&tr,cylindre);

                XMLElement *rayon = cylindre->FirstChildElement( "rayon");
                if (rayon)
                    iradius=atof(rayon->GetText());

                XMLElement *normale = cylindre->FirstChildElement( "normale");
                float xn,yn,zn;
                if (normale)
                    xyz(&xn,&yn,&zn,normale);

                
                XMLElement *hauteur = cylindre->FirstChildElement( "hauteur");
                float xh,yh,zh;
                if (hauteur)
                    xyz(&xh,&yh,&zh,hauteur);

                cylindres.push_back(Cylindre(Disque(Vec(xp,yp,zp), Vec(xc,yc,zc), re, tr, Vec(xe,ye,ze),
                                         iradius, Vec(xn,yn,zn)),hauteur)); 

                cylindre = cylindre->NextSiblingElement( "cylindre" );
            }
        }
    }
    return cylindres;
}*/

/*
Camera readCamera()
{
    XMLDocument doc;
    doc.LoadFile( "scene.xml" );
    
    XMLElement *scene = doc.RootElement();
    double iradius;
    float xp,yp,zp;
    float xc,yc,zc;
    float xe,ye,ze;
    float re,tr;
    Camera camera;
    
    if( scene )
    {
        XMLElement *spectateur = scene->FirstChildElement( "spectateur" );
 
        if ( spectateur )
        {   
            XMLElement *camera = objets->FirstChildElement( "camera" );
     
            if(camera)
            {

                XMLElement *position = camera->FirstChildElement( "position");
                float xp,yp,zp;
                if (position)
                    xyz(&xp,&yp,&zp,position);

                XMLElement *axe = camera->FirstChildElement( "axe");
                float xa,ya,za;
                if (axe)
                    xyz(&xa,&ya,&za,axe);


                XMLElement *hauteur = camera->FirstChildElement( "hauteur");
                float xh,yh,zh;
                if (hauteur)
                    xyz(&xh,&yh,&zh,hauteur);

                cylindre.push_back(Cylindre(Disque(Vec(xp,yp,zp), Vec(xc,yc,zc), re, tr, Vec(xe,ye,ze),
                                         iradius, Vec(xn,yn,zn)),)); 

                cylindre = cylindre->NextSiblingElement( "cylindre" );
            }
        
    }
    return disques;
}*/


/*
int main(int argc, char const *argv[])
{

	std::vector<Sphere> spheres = readSpheres();
    std :: cout << spheres[1].transparence<< std :: endl;
	std::vector<Plan> plans = readPlans(); 
    std::vector<Parallelepipede> paralls = readParalls();
	return 0;
}
*/