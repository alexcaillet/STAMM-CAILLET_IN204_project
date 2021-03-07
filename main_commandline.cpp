#include "moteur_rendu.hpp"

int main(){
    moteur_rendu *moteur = new moteur_rendu();
    moteur->calcul("scene.xml");
    delete moteur;
}