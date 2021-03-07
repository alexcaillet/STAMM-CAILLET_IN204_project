#include <iostream>
#include <QString>
#include "vecteur.hpp"
#include "XML/tinyxml2.h"
#include "objet.hpp"
#include "vector"
#include "camera.hpp"
#include "xmlread.hpp"

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
        if (cx){
            QString x_str = QString::fromUtf8(cx);
            *x = x_str.toFloat();}
        if (cy){
            QString y_str = QString::fromUtf8(cy);
            *y = y_str.toFloat();}
        if (cz){
            QString z_str = QString::fromUtf8(cz);
            *z = z_str.toFloat();}
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
    if (reflec){
        QString refl = reflec->GetText();
        *re = refl.toFloat();
        }

    XMLElement *transp = element->FirstChildElement( "tr");
    if (transp){
        QString trans = transp->GetText();
        *tr = trans.toFloat();}

}


std::vector<Sphere *> readSpheres(const char* filename)
{
    XMLDocument doc;
    std::vector<Sphere*> spheres; 
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
                if (rayon){
                    QString rad_str = rayon->GetText();
                    iradius = rad_str.toFloat();}

                Sphere *sphere_t = new Sphere(Vec(xp,yp,zp), Vec(xc,yc,zc), re, tr, Vec(xe,ye,ze),iradius);

                spheres.push_back(sphere_t);

                sphere = sphere->NextSiblingElement( "sphere" );
            }
        }
    }
    return spheres;
}



std::vector<Plan *> readPlans(const char* filename)
{
    XMLDocument doc;
    std::vector<Plan*> plans; 
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

				Plan *plan_t = new Plan(Vec(xp,yp,zp), Vec(xc, yc, zc), re, tr, Vec(xe,ye,ze),
                                     Vec(xlo,ylo,zlo), Vec(xla,yla,zla));
               	plans.push_back(plan_t);

                plan = plan->NextSiblingElement( "plan" );
            }
        }
    }
    return plans;
}



std::vector<Parallelepipede *> readParalls(const char* filename)
{
    XMLDocument doc;
    std::vector<Parallelepipede*> paralls; 
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
                
                Parallelepipede *parall_t = new Parallelepipede(Vec(xp,yp,zp), Vec(xc, yc, zc), re, tr, Vec(xe,ye,ze),
                                                  Vec(xh,yh,zh), Vec(xlo,ylo,zlo), Vec(xla,yla,zla));
                paralls.push_back(parall_t);
                parall = parall->NextSiblingElement( "parallelepipede" );
            }
        }
    }
    return paralls;
}


std::vector<Disque *> readDisques(const char* filename)
{
    XMLDocument doc;
    std::vector<Disque*> disques; 
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
            XMLElement *disque = objets->FirstChildElement( "disque" );

     
            while( disque)
            {
                baseAttributes(&xp,&yp,&zp,&xc,&yc,&zc,&xe,&ye,&ze,&re,&tr,disque);

                XMLElement *rayon = disque->FirstChildElement( "rayon");
                if (rayon){
                    QString rad_str = rayon->GetText();
                    iradius = rad_str.toFloat();}

                XMLElement *normale = disque->FirstChildElement( "normale");
                float xn,yn,zn;
                if (normale)
                    xyz(&xn,&yn,&zn,normale);

                Disque *disque_t = new Disque(Vec(xp,yp,zp), Vec(xc,yc,zc), re, tr, Vec(xe,ye,ze),
                                    iradius, Vec(xn,yn,zn));
                disques.push_back(disque_t); 


                disque = disque->NextSiblingElement( "disque" );
            }
        }
    }
    return disques;
}


std::vector<Cylindre *> readCylindres(const char* filename)
{
    XMLDocument doc;
    std::vector<Cylindre*> cylindres; 
    doc.LoadFile(filename);
    
    XMLElement *scene = doc.RootElement();
    double iradius, ihauteur;
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
                if (rayon){
                    QString rad_str = rayon->GetText();
                    iradius = rad_str.toFloat();}

                XMLElement *normale = cylindre->FirstChildElement( "normale");
                float xn,yn,zn;
                if (normale)
                    xyz(&xn,&yn,&zn,normale);


                XMLElement *hauteur = cylindre->FirstChildElement( "hauteur");
                if (hauteur){
                    QString h_str = hauteur->GetText();
                    ihauteur = h_str.toFloat();}

                Cylindre *cylindre_t = new Cylindre(Disque(Vec(xp,yp,zp), Vec(xc,yc,zc), re, tr, Vec(xe,ye,ze),
                                         iradius, Vec(xn,yn,zn)),ihauteur); 
                cylindres.push_back(cylindre_t); 

                cylindre = cylindre->NextSiblingElement( "cylindre" );
            }
        }
    }
    return cylindres;
}


Camera readCamera(const char* filename)
{
    XMLDocument doc;
    doc.LoadFile( filename );
    
    XMLElement *scene = doc.RootElement();
    double itilt;
    float xp,yp,zp;
    float xc,yc,zc;
    float xe,ye,ze;
    float re,tr;
    int iwidth, iheight, ifieldOfView;
    Camera cameraRes;
    
    if( scene )
    {
        XMLElement *spectateur = scene->FirstChildElement( "spectateur" );
 
        if ( spectateur )
        {   
            XMLElement *camera = spectateur->FirstChildElement( "camera" );
     
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


                XMLElement *image_width = camera->FirstChildElement( "image_width");
                if (image_width)
                    iwidth=atoi(image_width->GetText());

                XMLElement *image_height = camera->FirstChildElement( "image_height");
                if (image_height)
                    iheight=atoi(image_height->GetText());

                XMLElement *fieldOfView = camera->FirstChildElement("fieldOfView");
                if (fieldOfView)
                    ifieldOfView=atoi(fieldOfView->GetText());

                XMLElement *tilt = camera->FirstChildElement( "tilt");
                if (tilt){
                    QString tilt_str = tilt->GetText();
                    itilt = tilt_str.toFloat();}

                Camera cameraRes_t = Camera(Vec(xp,yp,zp),Vec(xa,ya,za),iwidth,iheight,ifieldOfView,itilt);

            }
        
        }
    }
    return cameraRes;
}


std :: vector <Objet *> read(const char* filename)
{   
    std :: vector <Objet *> objets;

	std::vector<Sphere *> spheres = readSpheres(filename);
    //std :: cout <<"Test sphere : "<< spheres[0]->position.z << std :: endl;
    objets.insert(objets.end(), spheres.begin(), spheres.end());

	std::vector<Plan *> plans = readPlans(filename); 
    objets.insert(objets.end(), plans.begin(), plans.end());

    std::vector<Parallelepipede *> paralls = readParalls(filename);
    objets.insert(objets.end(), paralls.begin(), paralls.end());

    std::vector<Disque *> disques = readDisques(filename);
    objets.insert(objets.end(), disques.begin(), disques.end());

    std::vector<Cylindre *> cylindres = readCylindres(filename);
    objets.insert(objets.end(), cylindres.begin(), cylindres.end());

    return objets;
	
}
