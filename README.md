# Projet Raytracing - Without-GUI

***Cette branche contient une version du projet adaptée à une compilation sans affichage graphique léger avec QT Creator***


Il est nécessaire d'avoir installé au préalable la librairie *openmp* pour compiler ce projet.

### Pour compiler le projet 
Il est recommandé d'utiliser la commande suivante :
```
g++ main_commandline.cpp  -fopenmp -O3 XML/tinyxml2.cpp xmlread.cpp -o raytracer.exe
```

### Pour executer le programme
Il suffit d'utiliser, après compilation :
```
./raytracer.exe nomdelascene.xml
```
*nomdelascene.xml* est le nom du fichier de description de scène que l'on souhaite utiliser.
Par exemple, *beautiful_scene.xml* et *scene.xml* sont deux fichiers de description à disposition dans le resposit.

### Remarques 
- Cette version du projet est à jour avec la branche principale du reposit
- Les fichiers *beautiful_scene.xml* et *scene.xml* décrivent d'ores et déjà des scènes prêtes à la compilation
- La création d'une image peut prendre du temps, en fonction des dimensions demandées. Un affichage interactif dans le terminal vous tiendra avancé de l'avancement du calcul.
