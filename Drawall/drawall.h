/*
This work is licensed under the Creative Commons Attribution
- Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions
3.0 France License. To view a copy of this license, visit
http://creativecommons.org/licenses/by-nc-sa/3.0/fr/.
*/

/**
 * \file    drawall.h
 * \author  Nathanaël Jourdane
 * \version 1.0
 * \date    28 septembre 2012
 * \brief   Fichier d'en-tête de la bibliothèque
 */
 
#ifndef drawbot
#define drawbot

#include "params.h"
#include <Arduino.h>
#include <math.h>
#include <SD.h>
#include <Servo.h> 

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
    
    /**
     * \brief Déplace le crayon à un point donné absolu.
     * \param x La position horizontale du point de destination du crayon.
     * \param y La position verticale du point de destination du crayon.
     */
    void move(float x, float y);
    
    /**
     * \brief Déplace le crayon à un point donné absolu.
     * \param position La position du point de destination du crayon (voir type enum \a Position)
     */
    void move(Position position);
    
    /**
     * \brief Déplace le crayon à un point donné relatif.
     * \Details Une position relative correspond à la position d'un point par rapport à la position actuelle.
     * \param x La position horizontale du point de destination du crayon.
     * \param y La position verticale du point de destination du crayon.
     */
    void _move(float x, float y);
    
    // trace une ligne droite, du point courant au point [x, y]
    void line(float x, float y);
    void _line(float x, float y);
    
    // lignes horizontales/verticales

    // trace une ligne horizontale sur toute la largeur de la surface, à la coordonée y.
    void horizontal(float y);
    void _horizontal(float y);

    // trace une ligne verticale sur toute la hauteur de la surface,
    // à la coordonée x.
    void vertical(float x);
    void _vertical(float x);

    // courbes de Bezier cubiques
    
    // trace une courbe de Bézier cubique
    // dont les points de controle P1 et P2 sont respectivement
    // en [x1, y1] et [x2, y2],
    // et dont le point de destination est en [x, y].
    void cubicCurve(float x, float y, float x1, float y1, float x2, float y2);
    void _cubicCurve(float x, float y, float x1, float y1, float x2, float y2);

    // idem, le point de controle P1 est l'image du point P2 de la dernière courbe de Bézier
    // (cubique ou quadratique) par symétrie avec le point courant.
    // Si la dernière figure n'était pas une courbe de Bézier, il correspond au point courant.
    void cubicCurve(float x, float y, float x2, float y2);
    void _cubicCurve(float x, float y, float x2, float y2);

    //courbes de Bezier quadratiques

    // trace une courbe de Bézier quadratique dont le point de controle P1 est en [x1, y1]
    // et dont le point de destination est en [x, y].
    void quadraticCurve(float x, float y, float x1, float y1);
    void _quadraticCurve(float x, float y, float x1, float y1);

    // idem, le point de controle est l'image du point de controle de la dernière courbe de Bézier
    // (cubique ou quadratique) par symétrie avec le point courant.
    // Si la dernière figure n'était pas une courbe de Bézier, il correspond au point courant.
    void quadraticCurve(float x, float y);
    void _quadraticCurve(float x, float y);
    
    // arcs et cercles
    
    // trace un arc de cercle
    void arc(float a1, float a2, float a3, float a4, float a5, float a6, float a7); // non fini
    void _arc(float a1, float a2, float a3, float a4, float a5, float a6, float a7);
    
    // trace une ellipse dont le centre est les point courant,
    // le rayon horizontal est rx et le rayon vertical est ry.
    void ellipse(float rx, float ry);

    // trace un cercle dont le centre est les point courant et de rayon r.
    void circle(float r);
    
    void svg(char* fileName);
    
    private:

    // ligne(float x, float y, bool write) n'initialise pas les variables pour les courbes Bezier, à la différence de line(float x, float y)
    void line(float x, float y, bool writing);
    
    // déplacement d'un pas moteur
    void leftStep();
    void rightStep();
    
    float positionToX(Position position);
    float positionToY(Position position);

    // fini le traçé courant:
    // Revient au point cible du dernier déplacement.
    // Ceci permet de faire des formes "finies"
    void endCurve();

    // renvoie la longueur courante des câbles
    long getLeftLength();
    long getRightLength();
    
    void initRatio();

    void setScale(int width, int height);
    
    // alimente ou désalimente le moteur (peut entrainer la chute du robot)
    void power(bool alimenter);

    void error(char* errNumber);
    
    /*********************
    ** lecture carte SD **
    *********************/
    
    // initialisation de la carte SD
    int sdInit();

    // recupere une valeur du fichier XML
    // EX: width --> (21cm) OU style --> (fill:none;stroke:#000000)
    void getAttribute(const char *attribute, char *value);

    // recupere une valeur du fichier XML qui est un nombre et le converti en float,
    // en prenant en compte les unités (cm, mm, ...)
    // EX: width --> 210 OU height --> 320
    float getNumericAttribute(const char *attribute);

    // teste si le car est un chiffre ou non
    bool isNumber(char car);

    // trouve un mot dans le fichier
    // renvoie faux si non trouvé (le curseur sera à la fin du mot)
    boolean sdFind(const char *mot);

    // récupère nbParams paramètres, les convertis en float et les stoquent dans le tableau tNb
    void getParameters(float *tNb, int nbParams);

    // dessine les traçés en lisant le contenu du fichier .svg
    void draw();

    long positionToLeftLength(float x, float y);
    long positionToRightLength(float x, float y);

    // si ecrire est vrai, le stylo est contre la paroi, sinon il est éloigné
    void write(bool write);

    /************
    * Attributs *
    *************/

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
    
    float mDelay;
    
    bool mWriting;
    
    float mStartCurveX, mStartCurveY;
    
    float mCubicCurveX, mCubicCurveY;
    float mQuadraticCurveX, mQuadraticCurveY;    
};

#endif
