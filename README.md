# STAMM-CAILLET_IN204_project

***Cette branche contient une version du projet adaptée à une compilation sans affichage graphique avec QT Creator***

Pour compiler le projet, il est recommandé d'utiliser la commande suivante :
```
g++ main_commandline.cpp  -fopenmp -O3 XML/tinyxml2.cpp xmlread.cpp -o raytracer.exe
```

Pour executer le programme, il suffit d'utiliser :
```
./raytracer.exe nomdelascene.xml
```
nomdelascene.xml est le nom du fichier de description de scène que l'on souhaite utiliser.
Par exemple, *beautiful_scene.xml* et *scene.xml* sont deux fichiers de description à disposition dans le resposit.
