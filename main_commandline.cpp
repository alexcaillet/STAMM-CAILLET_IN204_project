#include "moteur_rendu.hpp"

int main(int argc, char const * argv[]){
    moteur_rendu *moteur = new moteur_rendu();
    moteur->calcul(argv[1]);
    delete moteur;
}