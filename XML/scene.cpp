#include "scene.hpp"

int main()
{
	 
	using namespace tinyxml2;
	//std::vector<Sphere> spheres;
    std::vector<Sphere> spheres = readSpheres();
    std::cout << spheres.size() << std::endl;
    //spheres.push_back(Sphere(Vec3f( 0.0, -10004, -20), 10000, Vec3f(0.20, 0.20, 0.20), 0, 0.0)); 
    //spheres.push_back(Sphere(Vec3f( 0.0,      0, -20),     4, Vec3f(1.00, 0.32, 0.36), 1, 0.5)); 
    //spheres.push_back(Sphere(Vec3f( 5.0,     -1, -15),     2, Vec3f(0.90, 0.76, 0.46), 1, 0.0)); 
    //spheres.push_back(Sphere(Vec3f( 5.0,      0, -25),     3, Vec3f(0.65, 0.77, 0.97), 1, 0.0)); 
    //spheres.push_back(Sphere(Vec3f(-5.5,      0, -15),     3, Vec3f(0.90, 0.90, 0.90), 1, 0.0)); 
    spheres.push_back(Sphere(Vec3f( 0.0,     20, -20),     3, Vec3f(0.00, 0.00, 0.00), 0, 0.0, Vec3f(10))); 
    render(spheres);
    return 0;
}

/*


^
| y  ->x  z vers l'arriere*/
