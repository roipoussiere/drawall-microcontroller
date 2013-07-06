/*
Ce travail est sous licence Creative Commons Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 3.0 France License. Pour voir voir une copie de cette licence, ouvrez le fichier LIENCE.txt ou connectez-vous sur http://creativecommons.org/licenses/by-nc-sa/3.0/fr/.
*/

/**
 * \file    drawall.h
 * \author  Nathanaël Jourdane
 * \version 1.0
 * \date    28 septembre 2012
 * \brief   Fichier d'en-tête de la bibliothèque
 *
 * \mainpage Présentation

Drawall est un projet de robot qui dessine sur les murs.

Ce travail est sous licence Creative Commons Attribution - Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions 3.0 France License. Pour voir voir une copie de cette licence, ouvrez le fichier LIENCE.txt ou connectez-vous sur http://creativecommons.org/licenses/by-nc-sa/3.0/fr/.

Ce robot utilise une carte Arduino et nécessite par conséquent le logiciel Arduino pour fonctionner.
Je vous invite à vous documenter sur internet pour l'installation de celui-ci, vous y trouverez beaucoup d'information et d'aide.

La partie logicielle se compose en un programme principal (le fichier .ino) et une librairie (les fichiers drawall.cpp et drawall.h).
La librairie contient tous les calculs a executer pour faire fonctionner le robot.
Le fichier .ino ne sert qu'à initialiser le robot et appeler la librairie.
Le programme principal est paramétré pour que le robot déssine sur une feuille au format 65*50cm en mode paysage.
Lors de l'installation, il vous faut aussi respecter des distances précises : un petit schéma d'instalation est présent sur le dépot.
Toutefois vous pouvez facilement l'éditer pour modifier les paramètres que vous voulez.

Le projet comporte également un simulateur, qui permet de simuler le fonctionnement du robot sans le brancher.
Attention toutefois, le simulateur lis directement les impulsions électriques envoyées aux moteurs.
Il vous faut donc connecter à vote ordinateur une carte arduino munie d'un port sd, avec une carte contenant une image svg dessus pour faire fonctionner ce simulateur.
Ce simulteur utilise l'environnement de développement Processing, il vous faut donc l'installer.
Une aide à l'installation sur Linux est disponible sur le dépot.

Le logiciel et le matériel ont été intégralement développé par Nathanaël Jourdane.
Ce projet évoluera en fonction des retours des utilisateurs. N'hésitez pas à poser des questions, demander d'avantage d'information pour le montage et éventuellement proposer des évoluions.
Pour toute question vous pouvez me contacter par mail à nathaATjourdaneDOTnet.

Pour voir une démonstration de ce robot, rendez-vous sur : http://www.youtube.com/watch?v=ewhZ9wcrR2s
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
 * \bug Corriger Warnings pour les conversions de texte en char*
 */
class Drawall {
    public:

    /**
     * \brief Position sur la feuille
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
     * \param sheetWidth Largeur de la feuille.
     * \param sheetHeight Hauteur de la feuille.
     * \param sheetPositionX Position en X de la feuille par rapport au moteur de gauche.
     * \param sheetPositionY Position en Y de la feuille par rapport au moteur de gauche.
     * \bug Corriger Warning
     */
    Drawall(int distanceBetweenMotors, int sheetWidth, int sheetHeight, int sheetPositionX, int sheetPositionY);
    
    /**
     * \brief Démarre la librairie.
     * \details \b Nécessaire au fonctionnement de la librairie. Réalise les procédures d'initialisation du robot.
     */
    void begin();
   
    /**
     * \brief Finit le traçé.
     * \details Utilisé à la fin du programme. Cela positionne le crayon en bas de la feuille, désalimente les moteurs et met en pause le programme.
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
     * \brief Spécifie la vitesse du traçé.
     * \param speed La vitesse du traçé.
     */
    void setSpeed(float speed);
    
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
     * \brief Trace une ligne droite horizontale sur toute la largeur de la feuille, à la position absolue \a y.
     * \param y La position absolue verticale de la ligne.
     */
    void horizontal(float y);
    
    /**
     * \brief Trace une ligne droite horizontale sur toute la largeur de la feuille, à la position relative \a y.
     * \details Une position relative correspond à la position d'un point par rapport à la position actuelle.
     * \param y La position relative verticale de la ligne.
     */
    void _horizontal(float y);

    /**
     * \brief Trace une ligne droite verticale sur toute la hauteur de la feuille, à la position absolue \a x.
     * \param x La position absolue horizontale de la ligne.
     */
    void vertical(float x);
    
    /**
     * \brief Trace une ligne droite verticale sur toute la hauteur de la feuille, à la position relative \a x.
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
     * \brief Fini la figure SVG en cours.
     * \details Trâce une ligne droite jusqu'à la position du point cible du dernier déplacement, de manière a faire des formes "finies".
     */
    void endCurve();
    
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
     * \brief Trace un dessin vectoriel svg correspondant au fichier \a fileName sur la carte SD.
     * \details Le robot doit être muni d'un lecteur de carte SD et le fichier vectoriel doit être copié sur celle-ci.
     * \param fileName Le nom du fichier à dessiner.
     */
    void svg(char* fileName);
    
    private:

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
    void initRatio();

    /**
     * \brief Modifie l'échelle pour s'adapter à la largeur \a width et la hauteur \a height du dessin.
     * \details Les dimentions du dessin sont récupérées sur le fichier svg.
     * \todo Changer le nom et retourner l'échelle plutôt que de la modifier directement.
     */
    void setScale(int width, int height);
    
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
    void error(char* errNumber);

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

    /**
     * \brief Objet correspondant au servo-moteur, utilise la librairie Servo
     */
    Servo mServo;

    unsigned int mLeftLength, mRightLength;

    int mDistanceBetweenMotors;
    
    int mSheetPositionX, mSheetPositionY;
    
    // le fichier XML à lire
    File mFile;
    
    float mScale;
    
    float mPositionX,mPositionY; // position sur le plan
    
    // point d'arrivée fictif non modifié par les limites
    float mFictivePosX, mFictivePosY;

    float mSheetWidth, mSheetHeight; // taille du plan (en mm)
    
    float mRatio; // ratio entre le nb de pas et les mm
    
    // delai entre chaque pas, en micro-secondes
    float mDelay;
    
    bool mWriting;
    
    float mStartCurveX, mStartCurveY;
    
    float mCubicCurveX, mCubicCurveY;
    float mQuadraticCurveX, mQuadraticCurveY;    
};

#endif
