#ifndef SCENE_HPP
#define SCENE_HPP

#include <iostream>
#include "tinyxml2.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cassert> 
#include <cstdlib> 
#include <cstdio> 
#include <cmath> 
#include <fstream> 
#include <vector> 
#include <iostream> 
 
template<typename T> 
class Vec3 
{ 
public: 
    T x, y, z; 
    Vec3() : x(T(0)), y(T(0)), z(T(0)) {} 
    Vec3(T xx) : x(xx), y(xx), z(xx) {} 
    Vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {} 
    Vec3& normalize() 
    { 
        T nor2 = length2(); 
        if (nor2 > 0) { 
            T invNor = 1 / sqrt(nor2); 
            x *= invNor, y *= invNor, z *= invNor; 
        } 
        return *this; 
    } 
    Vec3<T> operator * (const T &f) const { return Vec3<T>(x * f, y * f, z * f); } 
    Vec3<T> operator * (const Vec3<T> &v) const { return Vec3<T>(x * v.x, y * v.y, z * v.z); } 
    T dot(const Vec3<T> &v) const { return x * v.x + y * v.y + z * v.z; } 
    Vec3<T> operator - (const Vec3<T> &v) const { return Vec3<T>(x - v.x, y - v.y, z - v.z); } 
    Vec3<T> operator + (const Vec3<T> &v) const { return Vec3<T>(x + v.x, y + v.y, z + v.z); } 
    Vec3<T>& operator += (const Vec3<T> &v) { x += v.x, y += v.y, z += v.z; return *this; } 
    Vec3<T>& operator *= (const Vec3<T> &v) { x *= v.x, y *= v.y, z *= v.z; return *this; } 
    Vec3<T> operator - () const { return Vec3<T>(-x, -y, -z); } 
    T length2() const { return x * x + y * y + z * z; } 
    T length() const { return sqrt(length2()); } 
    friend std::ostream & operator << (std::ostream &os, const Vec3<T> &v) 
    { 
        os << "[" << v.x << " " << v.y << " " << v.z << "]"; 
        return os; 
    } 
}; 
 
typedef Vec3<float> Vec3f; 
 
class Sphere 
{ 
public: 
    Vec3f center;                           /// position of the sphere 
    float radius, radius2;                  /// sphere radius and radius^2 
    Vec3f surfaceColor, emissionColor;      /// surface color and emission (light) 
    float transparency, reflection;         /// surface transparency and reflectivity 
    Sphere( 
        const Vec3f &c, 
        const float &r, 
        const Vec3f &sc, 
        const float &refl = 0, 
        const float &transp = 0, 
        const Vec3f &ec = 0) : 
        center(c), radius(r), radius2(r * r), surfaceColor(sc), emissionColor(ec), 
        transparency(transp), reflection(refl) 
    { /* empty */ } 


    bool intersect(const Vec3f &rayorig, const Vec3f &raydir, float &t0, float &t1) const 
    { 
        Vec3f l = center - rayorig; 
        float tca = l.dot(raydir); 
        if (tca < 0) return false; 
        float d2 = l.dot(l) - tca * tca; 
        if (d2 > radius2) return false; 
        float thc = sqrt(radius2 - d2); 
        t0 = tca - thc; 
        t1 = tca + thc; 
 
        return true; 
    } 
}; 
 
#define MAX_RAY_DEPTH 5 
 
float mix(const float &a, const float &b, const float &mix) 
{ 
    return b * mix + a * (1 - mix); 
} 
 

Vec3f trace( 
    const Vec3f &rayorig, 
    const Vec3f &raydir, 
    const std::vector<Sphere> &spheres, 
    const int &depth) 
{ 
    //if (raydir.length() != 1) std::cerr << "Error " << raydir << std::endl;
    float tnear = INFINITY; 
    const Sphere* sphere = NULL; 
    // find intersection of this ray with the sphere in the scene
    for (unsigned i = 0; i < spheres.size(); ++i) { 
        float t0 = INFINITY, t1 = INFINITY; 
        if (spheres[i].intersect(rayorig, raydir, t0, t1)) { 
            if (t0 < 0) t0 = t1; 
            if (t0 < tnear) { 
                tnear = t0; 
                sphere = &spheres[i]; 
            } 
        } 
    } 
    // if there's no intersection return black or background color
    if (!sphere) return Vec3f(2); 
    Vec3f surfaceColor = 0; // color of the ray/surfaceof the object intersected by the ray 
    Vec3f phit = rayorig + raydir * tnear; // point of intersection 
    Vec3f nhit = phit - sphere->center; // normal at the intersection point 
    nhit.normalize(); // normalize normal direction 
    // If the normal and the view direction are not opposite to each other
    // reverse the normal direction. That also means we are inside the sphere so set
    // the inside bool to true. Finally reverse the sign of IdotN which we want
    // positive.
    float bias = 1e-4; // add some bias to the point from which we will be tracing 
    bool inside = false; 
    if (raydir.dot(nhit) > 0) nhit = -nhit, inside = true; 
    if ((sphere->transparency > 0 || sphere->reflection > 0) && depth < MAX_RAY_DEPTH) { 
        float facingratio = -raydir.dot(nhit); 
        // change the mix value to tweak the effect
        float fresneleffect = mix(pow(1 - facingratio, 3), 1, 0.1); 
        // compute reflection direction (not need to normalize because all vectors
        // are already normalized)
        Vec3f refldir = raydir - nhit * 2 * raydir.dot(nhit); 
        refldir.normalize(); 
        Vec3f reflection = trace(phit + nhit * bias, refldir, spheres, depth + 1); 
        Vec3f refraction = 0; 
        // if the sphere is also transparent compute refraction ray (transmission)
        if (sphere->transparency) { 
            float ior = 1.1, eta = (inside) ? ior : 1 / ior; // are we inside or outside the surface? 
            float cosi = -nhit.dot(raydir); 
            float k = 1 - eta * eta * (1 - cosi * cosi); 
            Vec3f refrdir = raydir * eta + nhit * (eta *  cosi - sqrt(k)); 
            refrdir.normalize(); 
            refraction = trace(phit - nhit * bias, refrdir, spheres, depth + 1); 
        } 
        // the result is a mix of reflection and refraction (if the sphere is transparent)
        surfaceColor = ( 
            reflection * fresneleffect + 
            refraction * (1 - fresneleffect) * sphere->transparency) * sphere->surfaceColor; 
    } 
    else { 
        // it's a diffuse object, no need to raytrace any further
        for (unsigned i = 0; i < spheres.size(); ++i) { 
            if (spheres[i].emissionColor.x > 0) { 
                // this is a light
                Vec3f transmission = 1; 
                Vec3f lightDirection = spheres[i].center - phit; 
                lightDirection.normalize(); 
                for (unsigned j = 0; j < spheres.size(); ++j) { 
                    if (i != j) { 
                        float t0, t1; 
                        if (spheres[j].intersect(phit + nhit * bias, lightDirection, t0, t1)) { 
                            transmission = 0; 
                            break; 
                        } 
                    } 
                } 
                surfaceColor += sphere->surfaceColor * transmission * 
                std::max(float(0), nhit.dot(lightDirection)) * spheres[i].emissionColor; 
            } 
        } 
    } 
 
    return surfaceColor + sphere->emissionColor; 
} 
 

void render(const std::vector<Sphere> &spheres) 
{ 
    unsigned width = 1920, height = 1080; 
    Vec3f *image = new Vec3f[width * height], *pixel = image; 
    float invWidth = 1 / float(width), invHeight = 1 / float(height); 
    float fov = 30, aspectratio = width / float(height); 
    float angle = tan(M_PI * 0.5 * fov / 180.); 
    // Trace rays
    for (unsigned y = 0; y < height; ++y) { 
        for (unsigned x = 0; x < width; ++x, ++pixel) { 
            float xx = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectratio; 
            float yy = (1 - 2 * ((y + 0.5) * invHeight)) * angle; 
            Vec3f raydir(xx, yy, -1); 
            raydir.normalize(); 
            *pixel = trace(Vec3f(0), raydir, spheres, 0); 
        } 
    } 
    // Save result to a PPM image (keep these flags if you compile under Windows)
    std::ofstream ofs("./untitled.ppm", std::ios::out | std::ios::binary); 
    ofs << "P6\n" << width << " " << height << "\n255\n"; 
    for (unsigned i = 0; i < width * height; ++i) { 
        ofs << (unsigned char)(std::min(float(1), image[i].x) * 255) << 
               (unsigned char)(std::min(float(1), image[i].y) * 255) << 
               (unsigned char)(std::min(float(1), image[i].z) * 255); 
    } 
    ofs.close(); 
    delete [] image; 
} 
 

std::vector<Sphere> readSpheres()
{
	using namespace tinyxml2;
    XMLDocument doc;
    std::vector<Sphere> spheres; 
    doc.LoadFile( "scene.xml" );
    
    XMLElement *scene = doc.RootElement();

    double ix;
    double iy;
    double iz;
    double iradius;
    double ia;
    double ib;
    double ic;
    double ireflec;
    double itransp;
 
    if( scene )
    {
    // set of &lt;person&gt; tags
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
				
                spheres.push_back(Sphere(Vec3f(ix,iy,iz), iradius, Vec3f(ia, ib, ic), ireflec, itransp)); 

                sphere = sphere->NextSiblingElement( "sphere" );
            }
        }
    }
    return spheres;
}

#endif