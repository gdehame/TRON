# TRON
## Architecture du répertoire application

  Fichiers en-têtes dans le répertoire "Headers".
  Fichiers objets dans le répertoire "obj".
  Le fichier "Build" est un exécutable servant à compiler l'application et
             créer un exécutable "TRON"
  Le fichier "TRON Compilation and Launch" sert à compiler l'application et
             l'exécuter puis supprimer l'exécutable

  Liste des fichiers en-têtes et structure du code:

    TRON.cpp: fichier principal contenant la fonction main de l'application
    TRON.h: fichier en-tête principal contenant les déclarations des fonctions,
    les classes, les variables globales
    INI.hpp: fichier en-tête contenant les définitions des fonctions
             d'initialisation de l'application, l'initialisation de SDL et les
             définitions des variables globales
    Texture.hpp: fichier en-tête contenant les définitions des méthodes de la
                 classe "LTexture"
    Timer.hpp: fichier en-tête contenant les définitions des méthodes de la
               classe "LTimer"
    Moto.hpp: fichier en-tête contenant les définitions des méthodes des classes
              "Moto" et "Enemy" ainsi que des fonctions auxiliaires à certaines
              de ces méthodes
    Menus.hpp: fichier en-tête contenant les définitions des fonctions relatives
               aux menus, affichage et gestion des menus
    Fruits.hpp: fichier en-tête contenant les définitions des fonctions
                relatives aux fruits
    Game.hpp: fichier en-tête définissant la fonction de boucle de jeu
    SB.hpp: fichier en-tête contenant les défintions des fonctions relatives aux
            tableaux des scores
    END.hpp: fichier en-tête contenant les défintions des fonctions de fin de
             partie et de fermeture de l'application

## Informations sur l'application elle-même
  Le jeu TRON est un jeu jouable à 1 ou 2 joueurs, un joueur contrôle une moto
  qui peut grandir en roulant sur des fruits qui apparaissent de façon aléatoire
  et doit battre son adversaire en le forcant à rentrer dans le corps de sa moto.
  Il est possible de choisir la couleur des motos via le menu "inputs andoptions".
  Pour jouer en multijoueur il faut connaître à l'avance l'adresse IP de l'autre
  joueur et aller dans le menu "multijoueur".
