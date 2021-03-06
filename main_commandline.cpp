#include "moteur_rendu.hpp"

#include <iostream>
#include <stdlib.h>
#include <stdbool.h>
#include <cmath>
#include <memory>
#include <random>
#include <omp.h>
#include <chrono>

#include "vecteur.hpp"
#include "image.hpp"
#include "objet.hpp"
#include "ray.hpp"
#include "camera.hpp"

int main(){
    moteur_rendu moteur = moteur_rendu();
    return moteur.calcul();
}