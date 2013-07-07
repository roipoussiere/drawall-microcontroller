/*
This file is part of Drawall, a project for a robot which draws on walls.
See http://drawall.cc/ and https://github.com/roipoussiere/Drawall/.

Copyright (c) 2012-2013 Nathanaël Jourdane

Drawall is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * \file    drawall.h
 * \author  Nathanaël Jourdane
 * \version 1.0
 * \date    28 septembre 2012
 * \brief   Fichier d'en-tête de la bibliothèque
 *
 * \mainpage Présentation

Drawall est un projet libre de robot autonomme qui dessine sur les murs.

Ce projet est libre : vous pouvez le redistribuer ou le modifier suivant les termes de la GNU GPL. L'ensemble du projet est publié sous cette licence, ce qui inclut l'intégralité du code-source, les schémas électroniques, les schémas du matériel et toute la documentation. Pour plus de détails, consultez la GNU General Public License, dont vous trouverez une copie sur le fichier COPYING.txt dans le dépot GitHub. La documentation détaillée du code source est disponible sur http://drawall.cc/. 

Ce robot utilise une carte Arduino et nécessite donc le logiciel Arduino pour fonctionner. Vous trouverez de l'aide pour son installation et utilisation sur le site officiel http://arduino.cc/fr/.

La partie logicielle est une librairie Arduino. Elle est composée d'un fichier principal drawall.cpp, d'un fichier header drawall.h et d'un fichier de paramètres params.h. Ce dernier permet de spécifier tous les paramètres concernant le robot. Vous devrez l'éditer avant d'utiliser la librairie.
La librairie est utilisée par l'intermédiaire d'un "sketch" Arduino, (fichier .ino), dont vous trouverez des exemples dans le répertoire de la librairie.

La librairie contient tous les calculs nécessaire à l'execution du robot, les sketchs ne servent qu'à le commander, ils sont très courts et simples à utiliser.

Il est possible de commander le robot par des fonctions simples (lignes, courbes, ...), ou par l'intermédiaire d'un fichier svg qu'il va interpréter.
Les fonctions svg ne sont pas encore toutes interprétées, certains dessins ne seront don pas correctement reproduits. Vous pouvez vous référer au fichier d'exemple drawbot.svg dans le dossier examples.

Le projet comporte également un simulateur, permetant de tester le bon
fonctionnement d'un programe de dessin avant de le reproduire et faciliter le développement du projet.
Il nécessite l'environnement de développement Processing : http://www.processing.org/. Ce simulateur reproduit le dessin que réalise le robot, en interpretant en temps réel les impulsions envoyées aux moteurs.

Pour le faire fonctionner il vous faut donc connecter à votre ordinateur au minimum une carte arduino munie d'un lecteur SD et y insérer une carte contenant une image svg valide. Toutes les fonctions svg ne sont pas encore interprétées. Pour plus d'informations sur la conformité du fichier svg, référez-vous au document documentation/valid_svg.txt du dépot GitHub. Une aide à l'installation sur Linux est également disponible sur le dépot.

Ce projet est libre et évoluera en grâce aux retours des utilisateurs. Questions, demande d'informations et suggestions sont donc les bienvenues.

Copyright (c) 2012-2013 Nathanaël Jourdane

Adresse de contact : nathanael[AT]jourdane[DOT]net.

Lien vers dépôt GitHub : https://github.com/roipoussiere/Drawall

Site web du projet : http://drawall.cc/

Une vidéo de démonstration du robot : http://www.youtube.com/watch?v=ewhZ9wcrR2s
 */

#ifndef drawbot
#define drawbot

#include "params.h"
#include <Arduino.h>
#include <math.h>
#include <SD.h>
#include <Servo.h> 

/**
 * \brief Classe principale de la librairie
 * \todo Faire une simulation complète très rapide au début avant de commencer le traçé (possible ?) ou créer méthode test(bool testing) pour activer le mode de test.
 * \todo Ajouter méthode rect.
 * \todo Processing : Tracer les mouvement sur un calque séparé (derrière).
 * \todo Processing : Gestion des warnings.
 * \bug Processing : Affiche une mauvais position à la fin du traçé (même pendant ?)
 * \bug Processing : Afficher toujours déplacement en cours à la fin du traçé
 * \bug Système de limites très foireux.
 * \bug Processing : L'affichage de la position de tient pas compte de la position de la feuille.
 * \bug Le crayon va très en bas en fin de traçé (en dehors de limite basse).
 * \bug Moteurs gauche et droit non synchronisés pour les déplacements.
 * \bug Voir quel est le problème avec le dessin de Drawall, il fait n'importe quoi + non détection de la limite basse (cf. capture).
 * \bug Il y a une légère marge en haut du dessin.
 * \bug Pour les surfaces larges, pas de marge en bas du dessin, sans doute lié au bug ci-dessus.
 * \bug Dessin pas centré pour les surfaces larges.
 */
class Drawall {
    public:

    /**
     * \brief Position sur la zone de dessin
     * \details Les différentes position pour accès rapide, correspondant aux 8 points cardinaux, plus le centre.
     * \bug Ne fonctionne pas dans le sketch.
     */
    enum Position {
        CENTER,
        UPPER_CENTER,
        LOWER_CENTER,
        LEFT_CENTER,
        RIGHT_CENTER,
        UPPER_LEFT,
        UPPER_RIGHT,
        LOWER_LEFT,
        LOWER_RIGHT
    };

    /**
     * \brief Initialise la librairie.
     * \param distanceBetweenMotors Distance entre les deux moteurs.
     * \param areaWidth Largeur de la zone de dessin.
     * \param areaHeight Hauteur de la zone de dessin.
     * \param areaPositionX Position en X de la zone de dessin par rapport au moteur de gauche.
     * \param areaPositionY Position en Y de la zone de dessin par rapport au moteur de gauche.
     * \bug Corriger Warning
     */
    Drawall(unsigned int distanceBetweenMotors, unsigned int areaWidth, unsigned int areaHeight, unsigned int areaPositionX, unsigned int areaPositionY);
    
    /**
     * \brief Démarre la librairie.
     * \details \b Nécessaire au fonctionnement de la librairie. Réalise les procédures d'initialisation du robot.
     */
    void begin();
   
    /**
     * \brief Finit le traçé.
     * \details Utilisé à la fin du programme. Cela positionne le crayon en bas de la zone de dessin, désalimente les moteurs et met en pause le programme.
     */
    void end();
    
    /********************
    * Getters & setters *
    ********************/
    
    /**
     * \brief Renvoie la position horizontale actuelle du crayon.
     * \return La position horizontale du crayon.
     */
    float getPositionX();
    
    /**
     * \brief Renvoie la position verticale actuelle du crayon.
     * \return La position verticale du crayon.
     */
    float getPositionY();

    /**
     * \brief Spécifie la position initiale du crayon.
     * \details À utiliser avant de commencer à tracer.
     * \param x La position horizontale du crayon.
     * \param y La position verticale du crayon.
     */
    void setPosition(float x, float y);
    
    /**
     * \brief Spécifie la position initiale du crayon.
     * \details À utiliser avant de commencer à tracer.
     * \param position La position du crayon (voir type enum \a Position).
     * \bug Ne fonctionne pas sur le sketch.
     */
    void setPosition(Position position);
    
    /**
     * \brief Spécifie la vitesse du traçé (en mm/s).
     * \details Cette vitesse correspond à la vitesse de déplacement de la courroie qui effectue la plus grande distance entre 2 points (Cf. \a mDelay). La vitesse réelle du dessin sera donc plus lente.
     * \param speed La vitesse du traçé.
     * \bug La vitesse diminue si on augmente le nombre de pas.
     */
    void setSpeed(unsigned int speed);
    
    /**********************
    * Fonctions de dessin *
    **********************/
    
    /**
     * \brief Déplace le crayon à la position absolue [\a x ; \a y].
     * \param x La position absolue horizontale du point de destination.
     * \param y La position absolue verticale du point de destination.
     */
    void move(float x, float y);
    
    /**
     * \brief Déplace le crayon à la position absolue \a position.
     * \param position La position absolue du point de destination (Cf type enum \a Position)
     */
    void move(Position position);
    
    /**
     * \brief Déplace le crayon à la position relative [\a x ; \a y].
     * \details Une position relative correspond à la position d'un point par rapport à la position actuelle.
     * \param x La position relative horizontale du point de destination.
     * \param y La position relative verticale du point de destination.
     */
    void _move(float x, float y);
    
    /**
     * \brief Trace une ligne droite, de la position actuelle à la position absolue [\a x; \a y].
     * \param x La position absolue horizontale du point de destination.
     * \param y La position absolue verticale du point de destination.
     * \bug Fait des escaliers dans certains cas (?).
     */
    void line(float x, float y);

    /**
     * \brief Trace une ligne droite, de la position actuelle à la position relative [\a x; \a y].
     * \details Une position relative correspond à la position d'un point par rapport à la position actuelle.
     * \param x La position relative horizontale du point de destination.
     * \param y La position relative verticale du point de destination.
     */
    void _line(float x, float y);

    /**
     * \brief Trace un rectangle, avec pour point haut-gauche la position actuelle et point bas-droit absolue [\a x; \a y].
     * \param x La position absolue horizontale du point bas-droit du rectangle.
     * \param y La position absolue verticale du point bas-droit du rectangle.
     */
    void rect(float x, float y);

    /**
     * \brief Trace un rectangle, avec pour point haut-gauche la position actuelle et point bas-droit relative [\a x; \a y].
     * \details Une position relative correspond à la position d'un point par rapport à la position actuelle.
     * \param x La position relative horizontale du point bas-droit du rectangle.
     * \param y La position relative verticale du point bas-droit du rectangle.
     */
    void _rect(float x, float y);
    
    /**
     * \brief Trace un rectangle représentant les limites de la zone de dessin.
     */
    void area();
    
    /**
     * \brief Trace une ligne droite horizontale sur toute la largeur de la zone de dessin, à la position absolue \a y.
     * \param y La position absolue verticale de la ligne.
     */
    void horizontal(float y);
    
    /**
     * \brief Trace une ligne droite horizontale sur toute la largeur de la zone de dessin, à la position relative \a y.
     * \details Une position relative correspond à la position d'un point par rapport à la position actuelle.
     * \param y La position relative verticale de la ligne.
     */
    void _horizontal(float y);

    /**
     * \brief Trace une ligne droite verticale sur toute la hauteur de la zone de dessin, à la position absolue \a x.
     * \param x La position absolue horizontale de la ligne.
     */
    void vertical(float x);
    
    /**
     * \brief Trace une ligne droite verticale sur toute la hauteur de la zone de dessin, à la position relative \a x.
     * \details Une position relative correspond à la position d'un point par rapport à la position actuelle.
     * \param x La position absolue horizontale de la ligne.
     */
    void _vertical(float x);

    /**
     * \brief Trace une courbe de Bézier cubique en position absolue.
     * \details La courbe est composée de 100 segments.
     * Avec en point de départ la position actuelle, en point de destination [\a pX ; \a pY] et en points de contrôle P1 [\a p1X ; \a p1Y] et P2 [\a p2X ; \a p2Y].
     * Cf l'article Wikipédia sur les courbes de Bézier : \a http://fr.wikipedia.org/wiki/Courbe_de_B%C3%A9zier .
     * Équation de la courbe : B(t) = P0(1-t)^3 + 3*P1*t(1-t)^2 + 3*P2*t^2*(1-t) + P3*t^3. Avec t allant de 0 à 1.
     * \param pX La position absolue horizontale du point de destination.
     * \param pY La position absolue verticale du point de destination.
     * \param p1X La position absolue horizontale du point de contrôle P1.
     * \param p1Y La position absolue verticale du point de contrôle P1.
     * \param p2X La position absolue horizontale du point de contrôle P2.
     * \param p2Y La position absolue verticale du point de contrôle P2.
     */
    void cubicCurve(float pX, float pY, float p1X, float p1Y, float p2X, float p2Y);

    /**
     * \brief Trace une courbe de Bézier cubique en position relative.
     * \details La courbe est composée de 100 segments.
     * Avec en point de départ la position actuelle, en point de destination [\a pX ; \a pY] et en points de contrôle P1 [\a p1X ; \a p1Y] et P2 [\a p2X ; \a p2Y].
     * Cf l'article Wikipédia sur les courbes de Bézier : \a http://fr.wikipedia.org/wiki/Courbe_de_B%C3%A9zier .
     * \param pX La position relative horizontale du point de destination.
     * \param pY La position relative verticale du point de destination.
     * \param p1X La position relative horizontale du point de contrôle P1.
     * \param p1Y La position relative verticale du point de contrôle P1.
     * \param p2X La position relative horizontale du point de contrôle P2.
     * \param p2Y La position relative verticale du point de contrôle P2.
     */
    void _cubicCurve(float pX, float pY, float p1X, float p1Y, float p2X, float p2Y);

    /**
     * \brief Trace une courbe de Bézier cubique en position absolue.
     * \details La courbe est composée de 100 segments.
     * Avec en point de départ la position actuelle, en point de destination [\a pX ; \a pY] et point P2 [\a p2X ; \a p2Y].
     * Le point de contrôle P1 est l'image du point P2 de la dernière courbe de Bézier. Si la dernière figure n'en était pas une, il correspond au point courant.
     * Cf l'article Wikipédia sur les courbes de Bézier : \a http://fr.wikipedia.org/wiki/Courbe_de_B%C3%A9zier .
     * \param pX La absolue relative horizontale du point de destination.
     * \param pY La absolue relative verticale du point de destination.
     * \param p2X La absolue relative horizontale du point de contrôle P2.
     * \param p2Y La absolue relative verticale du point de contrôle P2.
     */
    void cubicCurve(float pX, float pY, float p2X, float p2Y);

    /**
     * \brief Trace une courbe de Bézier cubique en position relative.
     * \details La courbe est composée de 100 segments.
     * Avec en point de départ la position actuelle, en point de destination [\a pX ; \a pY] et point P2 [\a p2X ; \a p2Y].
     * Le point de contrôle P1 est l'image du point P2 de la dernière courbe de Bézier. Si la dernière figure n'en était pas une, il correspond au point courant.
     * Cf l'article Wikipédia sur les courbes de Bézier : \a http://fr.wikipedia.org/wiki/Courbe_de_B%C3%A9zier .
     * \param pX La position relative horizontale du point de destination.
     * \param pY La position relative verticale du point de destination.
     * \param p2X La position relative horizontale du point de contrôle P2.
     * \param p2Y La position relative verticale du point de contrôle P2.
     */
    void _cubicCurve(float pX, float pY, float p2X, float p2Y);

    /**
     * \brief Trace une courbe de Bézier quadratique en position absolue.
     * \details La courbe est composée de 100 segments.
     * Avec en point de départ la position actuelle, en point de destination [\a pX ; \a pY] et en point de contrôle P1 [\a p1X ; \a p1Y].
     * Cf l'article Wikipédia sur les courbes de Bézier : \a http://fr.wikipedia.org/wiki/Courbe_de_B%C3%A9zier .
     * Équation de la courbe : B(t) = P0*(1-t)^2 + 2*P1*t(1-t) + P2*t^2. Avec t allant de 0 à 1.
     * \param pX La position absolue horizontale du point de destination.
     * \param pY La position absolue verticale du point de destination.
     * \param p1X La position absolue horizontale du point de contrôle P1.
     * \param p1Y La position absolue verticale du point de contrôle P1.
     */
    void quadraticCurve(float pX, float pY, float p1X, float p1Y);

    /**
     * \brief Trace une courbe de Bézier quadratique en position relative.
     * \details La courbe est composée de 100 segments.
     * Avec en point de départ la position actuelle, en point de destination [\a pX ; \a pY] et en point de contrôle P1 [\a p1X ; \a p1Y].
     * Cf l'article Wikipédia sur les courbes de Bézier : \a http://fr.wikipedia.org/wiki/Courbe_de_B%C3%A9zier .
     * \param pX La position relative horizontale du point de destination.
     * \param pY La position relative verticale du point de destination.
     * \param p1X La position relative horizontale du point de contrôle P1.
     * \param p1Y La position relative verticale du point de contrôle P1.
     */
    void _quadraticCurve(float pX, float pY, float p1X, float p1Y);

    /**
     * \brief Trace une courbe de Bézier quadratique en position absolue.
     * \details La courbe est composée de 100 segments.
     * Avec en point de départ la position actuelle et en point de destination [\a pX ; \a pY].
     * Le point de contrôle P1 est l'image du point P2 de la dernière courbe de Bézier. Si la dernière figure n'en était pas une, il correspond au point courant.
     * Cf l'article Wikipédia sur les courbes de Bézier : \a http://fr.wikipedia.org/wiki/Courbe_de_B%C3%A9zier .
     * \param pX La position absolue horizontale du point de destination.
     * \param pY La position absolue verticale du point de destination.
     */
    void quadraticCurve(float pX, float pY);

    /**
     * \brief Trace une courbe de Bézier quadratique en position relative.
     * \details La courbe est composée de 100 segments.
     * Avec en point de départ la position actuelle, en point de destination [\a pX ; \a pY].
     * Le point de contrôle P1 est l'image du point P2 de la dernière courbe de Bézier. Si la dernière figure n'en était pas une, il correspond au point courant.
     * Cf l'article Wikipédia sur les courbes de Bézier : \a http://fr.wikipedia.org/wiki/Courbe_de_B%C3%A9zier .
     * \param pX La position relative horizontale du point de destination.
     * \param pY La position relative verticale du point de destination.
     */
    void _quadraticCurve(float pX, float pY);
        
    /**
     * \brief Trace un arc de cercle en position absolue.
     * \todo Remplir la fonction.
     */
    void arc(float a1, float a2, float a3, float a4, float a5, float a6, float a7);

    /**
     * \brief Trace un arc de cercle en position relative.
     * \todo Remplir la fonction.
     */
    void _arc(float a1, float a2, float a3, float a4, float a5, float a6, float a7);
    
    /**
     * \brief Trace une ellipse dont le centre est le point courant, le rayon horizontal est \a rX et le rayon vertical est \a rY.
     * \param rX Le rayon horizontal de l'ellipse.
     * \param rY Le rayon vertical de l'ellipse.
     */
    void ellipse(float rX, float rY);

    /**
     * \brief Trace un cercle dont le centre est le point courant et le rayon est \a r.
     * \param r Le rayon du cercle.
     */
    void circle(float r);
    
    /**
     * \brief Fini la figure SVG en cours.
     * \details Trâce une ligne droite jusqu'à la position du point cible du dernier déplacement, de manière a faire des formes "finies".
     */
    void endFigure();

    /**
     * \brief Trace un rectangle correspondant à la zone de dessin du fichier svg \a fileName.
     * \details Le robot doit être muni d'un lecteur de carte SD et le fichier vectoriel doit être copié sur celle-ci.
     * \param fileName Le nom du fichier.
     * \bug Le rectangle sort de la zone de dessin dans certains cas (testé aux valeurs 4810, 3950, 2830, 430, 640) + limite droite non-détectée.
     */
    void drawingArea(const char* fileName);

    /**
     * \brief Trace un dessin vectoriel svg correspondant au fichier \a fileName sur la carte SD.
     * \details Le robot doit être muni d'un lecteur de carte SD et le fichier vectoriel doit être copié sur celle-ci.
     * \param fileName Le nom du fichier à dessiner.
     */
    void svg(const char* fileName);
    
    private:

    /**
     * \brief Détermine si la position désirée se trouve dans la zone de dessin (\a true) ou non (\a false).
     * \details Envoie un warning si c'est le cas.
     * \return \a true si la position désirée se trouve dans la zone de dessin, \a false sinon
     */
    bool positionInsideArea(float x, float y);

    /**
     * \brief Trace une ligne à bas niveau, de la position actuelle à la position absolue [\a x; \a y], en écrivant (\a writing) ou pas.
     * \details Ne compense pas le courbure naturelle de la ligne. N'initialise pas les variables pour les courbes Bezier, à la différence de line(float x, float y).
     * \param pX La position absolue horizontale du point de destination.
     * \param pY La position absolue verticale du point de destination.
     * \param writing \a true pour écrire pendant le déplacement, \a false pour ne pas écrire.
     * \todo Trop long, éclater la méthode.
     */
    void line(float pX, float pY, bool writing);

    /**
     * \brief Initialise le ratio entre le nombre de pas et la distance.
     * \details Le ratio est calculé en fonction du diametre moteur et du nombre de pas.
     * xx(mm)*ratio --> xx(pas)
     * xx(pas)/ratio --> xx(mm)
     * \todo Inverser le ratio car le nombre de pas est une valeur entière, éviter de le diviser.
     */
    void initStepLength();

    /**
     * \brief Modifie l'échelle pour s'adapter à la largeur \a width et la hauteur \a height du dessin.
     * \details Les dimentions du dessin sont récupérées sur le fichier svg.
     * \todo Changer le nom et retourner l'échelle plutôt que de la modifier directement.
     */
    void setDrawingScale(float width, float height);
    
    /**
     * \brief Renvoie la longueur actuelle du câble gauche.
     * \return La longueur actuelle du câble gauche, en nombre de pas.
     */
    long getLeftLength();

    /**
     * \brief Renvoie la longueur actuelle du câble droit.
     * \return La longueur actuelle du câble droit, en nombre de pas.
     */
    long getRightLength();
    
    /**
     * \brief À appeler lorsque une erreur se produit.
     * \details Envoie le code d'erreur \a errNumber à Processing, qui se charge d'afficher sa description. Éloigne le stylo de la paroi et stoppe le programme.
     * \todo Mettre en pause le traçé, quand la pause sera opérationnelle.
     */
    void error(const char* errNumber);

    /***********************
    * Commande du matériel *
    ***********************/
    
    /**
     * \brief Rotation du moteur gauche d'un pas.
     * \todo La direction en paramètre ou en argument de classe.
     * \todo Il faudrait ne pas faire de pause pour ne pas influencer sur le traçé
     */
    void leftStep();

    /**
     * \brief Rotation du moteur droit d'un pas.
     * \todo La direction en paramètre ou en argument de classe.
     * \todo Il faudrait ne pas faire de pause pour ne pas influencer sur le traçé
     */
    void rightStep();

    /**
     * \brief Alimente ou désalimente les moteurs.
     * \details Éloigne le stylo de la paroi avant la désalimentation pour éviter de dessiner pendant la chute éventuelle du moteur.
     * \param power \a true pour alimenter le moteur, \a false pour le désalimenter.
     */
    void power(bool power);
    
    /**************
    * Convertions *
    **************/

    /**
     * \brief Récupère la position horizontale de \a position.
     * \details Cf. type enum \a Position.
     * \param position La position a convertir, de type \a Position.
     * \return La position verticale de \a position.
     */
    float positionToX(Position position);

    /**
     * \brief Récupère la position verticale de \a position.
     * \details Cf. type enum \a Position.
     * \param position La position a convertir, de type \a Position.
     * \return La position verticale de \a position.
     */
    float positionToY(Position position);

    /**
     * \brief Calcule la longueur du câble gauche pour la position [\a x ; \a y]
     * \param x La position absolue horizontale du point.
     * \param y La position absolue horizontale du point.
     * \return La longueur du câble gauche pour cette position, en nombre de pas.
     */
    long positionToLeftLength(float x, float y);
    
    /**
     * \brief Calcule la longueur du câble droit pour la position [\a x ; \a y]
     * \param x La position absolue horizontale du point.
     * \param y La position absolue horizontale du point.
     * \return La longueur du câble droit pour cette position, en nombre de pas.
     */
    long positionToRightLength(float x, float y);

    /*******************
    * Lecture carte SD *
    *******************/
    
    /**
     * \brief initialise de la carte SD.
     * \details Peut générer l'erreur 01 : Carte absente ou non reconnue.
     */
    void sdInit();

    /**
     * \brief Récupere la valeur de l'attribut \a attribute du fichier svg sous forme textuelle.
     * \details Récupère le prochain mot correspondant à la chaîne spécifiée.
     * Ex.1 : "width" --> "21cm"
     * Ex.2 : "style" --> "fill:none;stroke:#000000"
     * \param attribute Le nom de l'attribut à lire, ex : "width".
     * \param value Chaîne dans laquelle copier la valeur de l'attribut.
     * \todo Détecter le '>' pour chercher uniquement dans la balise.
     * \todo Voir pourquoi pas de char* en sortie.
     */
    void getAttribute(const char* attribute, char* value);

    /**
     * \brief Récupere la valeur de l'attribut \a attribute du fichier svg sous forme numérique (flotant).
     * \details Récupère le prochain mot correspondant à la chaîne spécifiée et le convertit en flotant en prenant en compte son unité (cm, mm, ...).
     * Ex.1 : width --> 210
     * \param attribute Le nom de l'attribut à lire.
     * \todo Convertir les unités en mm, pas en px.
     */
    float getNumericAttribute(const char* attribute);

    /**
     * \brief Informe si le caractère \a car est un chiffre.
     * \param car Le caractère à tester.
     * \return Renvoie \a true si le caractère est un chiffre, \a false sinon.
     */
    bool isNumber(char car);

    /**
     * \brief Cherche le mot \a word dans le fichier.
     * \details Une fois trouvé, le curseur sera positionné à la fin du mot.
     * \param word Le mot à chercher.
     * \return Renvoie \a false si le mot n'a pas été trouvé.
     */
    bool sdFind(const char* word);

    /**
     * \brief Récupère \a nbParams paramètres, les convertissent en float et les stoquent dans le tableau \a tabParams.
     * \details Les paramètres sont des nombres séparés par un espace ou une virgule.
     * Ex : d="m 150.0 10.0, l 20 20"
     * \param tabParams Le tableau de paramètres dans lequel seront stoqué les nombres.
     * \param nbParams Le nombre de paramètres.
     * \todo Voir si on peut renvoyer le tableau.
     */
    void getParameters(float* tabParams, int nbParams);

    /**
     * \brief Dessine une figure vectorielle correspondant au contenu de l'attribut d="..." d"une balise \a path.
     * \details Le curseur doit être positionné au début du contenu de d"..."
     */
    void draw();

    /**
     * \brief Approche ou éloigne le crayon de la paroi.
     * \param write \a true pour plaquer le crayon contre la paroi (traçé), \a false pour l'éloigner (déplacement)
     */
    void write(bool write);

    /************
    * Attributs *
    ************/

    /// Objet pour manipuler le servo-moteur, utilisé avec la librairie \a Servo.
    Servo mServo;

    /// Longueur du câble gauche, en pas.
    unsigned long mLeftLength;
    
    /// Longueur du câble droit, en pas.
    unsigned long mRightLength;

    /// Distance entre les 2 moteurs, en mm.
    unsigned int mDistanceBetweenMotors;
       
    /// Le fichier svg contenant le dessin vectoriel à reproduire.
    File mFile;
    
    /// Échelle générée par les attributs width et height du fichier svg, afin que le dessin s'adapte aux dimentions de la zone de dessin.
    float mDrawingScale;
    
    /// Offset horizontal généré par l'attribut width du fichier svg, afin que le dessin soit centré sur la zone de dessin.
    unsigned int mDrawingOffsetX;

    /// Offset vertical généré par l'attribut height du fichier svg, afin que le dessin soit centré sur la zone de dessin.
    unsigned int mDrawingOffsetY;

    /// Largeur de la zone de dessin, en mm.
    unsigned int mAreaWidth;
    
    /// Hauteur de la zone de dessin, en mm.
    unsigned int mAreaHeight;
    
    /// Longueur d'un pas (distance parcourue par le câble en 1 pas, en mm.
    float mStepLength;
    
    /// Delai initial entre chaque pas du moteur qui a la plus grande distance à parcourir, (en µs).
    /// \details Le délai de l'autre moteur est calculé pour que les 2 moteurs arrivent au point de destination simultanément.
    float mDelay;

    /// Permet de déterminer si le robot est en train d'écrire (\a true) ou non (\a false).    
    bool mWriting;
    
    /************
    * Positions *
    ************/    

    /// Position horizontale du point [0 ; 0] (haut-gauche) de la zone de dessin par rapport au moteur gauche, en mm.
    unsigned int mAreaPositionX;

    /// Position verticale du point  [0 ; 0] (haut-gauche) de la zone de dessin par rapport au moteur gauche, en mm.
    unsigned int mAreaPositionY;

    /// Position horizontale actuelle du crayon sur le plan.
    float mPositionX;
    
    /// Position verticale actuelle du crayon sur le plan.
    float mPositionY;
        
    /// Position horizontale du point d'arrivée fictif qui n'est pas modifié par les limites, de manière à ce que le traçé ne subisse pas de transformation.
    float mFictivePosX;
    
    /// Position verticale du point d'arrivée fictif qui n'est pas modifié par les limites, de manière à ce que le traçé ne subisse pas de transformation.    
    float mFictivePosY;

    /// Position horizontale du point de départ d'une figure (ligne, courbe, ...).
    /// \details Nécessaire pour finir le traçé avec endFigure().
    float mStartFigureX;

    /// Position verticale du point de départ d'une figure (ligne, courbe, ...).
    /// \details Nécessaire pour finir le traçé avec endFigure().
    float mStartFigureY;
    
    /// Position horizontale du point de contrôle P2 de la dernière courbe de Bézier cubique traçée.
    /// \details Nécessaire pour tracer une courbe de Bézier cubique qui dépend de la précédente (appel de la fonction avec un seul point de contrôle).
    float mCubicCurveX;
    
    /// Position horizontale du point de contrôle P2 de la dernière courbe de Bézier cubique traçée.
    /// \details Nécessaire pour tracer une courbe de Bézier cubique qui dépend de la précédente (avec un point de contrôle en moins).
    float mCubicCurveY;

    /// Position horizontale du point de contrôle P1 de la dernière courbe de Bézier quadratique traçée.
    /// \details Nécessaire pour tracer une courbe de Bézier quadratique qui dépend de la précédente (appel de la fonction avec aucun point de contrôle).
    float mQuadraticCurveX;
    
    /// Position verticale du point de contrôle P1 de la dernière courbe de Bézier quadratique traçée.
    /// \details Nécessaire pour tracer une courbe de Bézier quadratique qui dépend de la précédente (appel de la fonction avec aucun point de contrôle).
    float mQuadraticCurveY;
};

#endif
