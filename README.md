# STAMM-CAILLET_IN204_project

#Idées 
* Sphere 
* Cube
* Pland (damier ?)
* Verre
* Paille ?

[Beacoup de choses intéressantes ici](https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing)

Pour la prochaine séance : Présenter l'analyse d'usage

## Analyse d'usage
# Contexte d'utilisation
Le logiciel prend un fichier de description (de type BLENDER, ou XML, formats standard) de scène en entrée et crée un rendu.
Ce genre de moteurs de rendu peut être utile pour le rendu d'images de synthèse (films d'animation, effets spéciaux, jeux vidéo, modélisation  et design).

# Fonctions souhaitées par les utilisateurs de la bibliothèque


# Definition des intéraction
Comment fournir les consignes au logiciel ?
* Par le biais du fichier de scène défini préalablement par l'utilisateur
* Par le biais d'une interface graphique (pas si compliqué à programmer --> usage de la SDL)
* Affichage du rendu dans l'interface ?

# Definition des fonctions de base du programme
* Fonction de rendu final et exportation
* Fonction de tracé de rayon 
* Constructeurs/Destructeur des différents objets/rayons

# CdC format de fichier
* .XML ou .Json
* Description de la position de l'objet dans la scène
* Prise en charge des sphères, des parallèlepipèdes, des plans
* //Description de la texture de l'objet (reflectivité et diffusivité)
* //Objet composé ?
