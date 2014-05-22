/*
 * This file is part of Drawall, a vertical tracer (aka drawbot) - see http://drawall.fr/
 * 
 * Copyright (c) 2012-2014 Nathanaël Jourdane
 * 
 * Drawall is free software : you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file	drawall.h
 * \author  Nathanaël Jourdane
 * \brief   Fichier d'en-tête de la bibliothèque
 */

#ifndef drawall
#define drawall

#include "defines.h"
#include <Arduino.h>
#include <math.h>
#include <SD.h>
#include <Servo.h>

/**
 * \brief Classe principale de la librairie
 *
 * \todo Tester le fichier GCode avant de commencer le dessin.
 * \todo Possibilité d'insérer une variable dans les codes d'erreurs.
 * \todo Création d'un fichier de log contenant les erreurs et warnings.
 * \todo Envoyer signal de pause sur le port série lors d'une pause.
 *
 * \bug Système de limites à corriger.
 * \bug Le crayon va très en bas en fin de traçé (en dehors de limite basse).
 * \bug Moteurs gauche et droit non synchronisés pour les déplacements.
 * \bug Il y a une légère marge en haut du dessin.
 * \bug Pour les surfaces larges, pas de marge en bas du dessin, sans doute lié au bug ci-dessus.
 * \bug Dessin pas centré pour les surfaces larges.
 * \bug Lorsqu'on change la vitesse, le traçé se fait en 2 segments.
 */
class Drawall {

  public:

	/**
	 * \brief Position sur la zone de dessin
	 * \details Les différentes position pour accès rapide, correspondant aux 8 points cardinaux, plus le centre.
	 */
	typedef enum Position {
		LOWER_LEFT,
		LOWER_CENTER,
		LOWER_RIGHT,

		LEFT_CENTER,
		CENTER,
		RIGHT_CENTER,

		UPPER_LEFT,
		UPPER_CENTER,
		UPPER_RIGHT
	};

	/**
	 * \brief modes de taille de l'image
	 */
	typedef enum DrawingSize {
		ORIGINAL,				///< Image de la taille du dessin
		FULL					///< Image de la taille de la zone de dessin
	};

	/**
	 * \brief Initialise la librairie.
	 * \bug Corriger Warning
	 */
	 Drawall(
				);

	/**
	 * \brief Démarre la librairie.
	 * \details \b Nécessaire au fonctionnement de la librairie. Réalise les procédures d'initialisation du robot.
	 * \param fileName Nom du fichier de configuration à charger.
	 */
	void begin(
				char *fileName);

	/**
	 * \brief Finit le traçé.
	 * \details Utilisé à la fin du programme. Cela positionne le crayon en bas de la zone de dessin, désalimente les moteurs et met en pause le programme.
	 */
	void end(
				);

	/********************
	* Getters & setters *
	********************/

	/**
	 * \brief Spécifie la position initiale du crayon.
	 * \details À utiliser avant de commencer à tracer.
	 * \param x La position horizontale du crayon.
	 * \param y La position verticale du crayon.
	 */
	void setPosition(
				float x,
				float y);

	/**
	 * \brief Spécifie la position initiale du crayon.
	 * \details À utiliser avant de commencer à tracer.
	 * \param position La position du crayon (voir type enum \a Position).
	 */
	void setPosition(
				Position position);

	/**
	 * \brief Spécifie la vitesse du traçé (en mm/s).
	 * \details Cette vitesse correspond à la vitesse de déplacement de la courroie qui effectue la plus grande distance entre 2 points (Cf. \a mDelay). La vitesse réelle du dessin sera donc plus lente.

	 * \param speed La vitesse du traçé.
	 * \bug La vitesse diminue si on augmente le nombre de pas.
	 */
	void setSpeed( unsigned int speed);

	/**********************
	* Fonctions de dessin *
	**********************/

	/**
	 * \brief Déplace le crayon à la position absolue [\a x ; \a y].
	 * \param x La position absolue horizontale du point de destination.
	 * \param y La position absolue verticale du point de destination.
	 */
	void move(
				float x,
				float y);

	/**
	 * \brief Déplace le crayon à la position absolue \a position.
	 * \param position La position absolue du point de destination (Cf type enum \a Position)
	 */
	void move(
				Position position);

	/**
	 * \brief Trace une ligne droite, de la position actuelle à la position absolue [\a x; \a y].
	 * \param x La position absolue horizontale du point de destination.
	 * \param y La position absolue verticale du point de destination.
	 * \bug Fait des escaliers dans certains cas (?).
	 */
	void line(
				float x,
				float y);

	/**
	 * \brief Trace un rectangle représentant les limites du dessin.
	 */
	void drawingArea(
				char *fileName, DrawingSize size = ORIGINAL, Position position = CENTER);

	/**
	 * \brief Trace un dessin correspondant au fichier gcode \a fileName de la carte SD.
	 * \details Le robot doit être muni d'un lecteur de carte SD contenant un fichier gcode.
	 * \param fileName Le nom du fichier gcode à dessiner.
	 * \todo Tester la présence du code M02 (fin du dessin) avant la fin du fichier.
	 */
	void draw(char *fileName, DrawingSize size = ORIGINAL, Position position = CENTER);

  private:

	/**
	 * \brief Liste des données envoyées au pc via le port série.
	*/
	typedef enum SerialData {
		PUSH_LEFT,				///< Relâche la courroie gauche d'un cran.
		PULL_LEFT,				///< Tire la courroie gauche d'un cran.
		PUSH_RIGHT,				///< Relâche la courroie droite d'un cran.
		PULL_RIGHT,				///< Tire la courroie droite d'un cran.
		WRITING,				///< Le stylo dessine.
		MOVING,					///< Le stylo se déplace (ne dessine pas).
		START_MESSAGE,			///< Début d'envoie d'un message à afficher.
		END_MESSAGE,			///< Début d'envoie d'un message à afficher.
		ENABLE_MOTORS,			///< Alimentation des moteurs.
		DISABLE_MOTORS,			///< Désalimentation des moteurs.
		SLEEP,					///< Mise en pause du programme.
		CHANGE_TOOL,			///< Pause pour changement d'outil
		END_DRAWING,			///< Fin du dessin.
		WARNING,				///< Warning (suivi du code de warning).
		END_WARNING,			///< Fin du message de warning.
		ERROR,					///< Erreur (suivi du code d'erreur).
		END_ERROR,				///< Fin du message d'erreur.

		START = 100,			///< Amorçage par liason série
		START_INSTRUCTIONS,		///< Début d'envoi des données d'initialisation.
		END_INSTRUCTIONS,		///< Fin d'envoi des données d'initialisation.
	};

	/**
	* \brief Liste des erreurs et warnings pouvant survenir pendant l'execution du programme.
	* \details Les erreurs commencent à l'indice 0 tandis que les warnings commencent à l'indice 100.
	* Les warnings sont des simples avertissement qui n'interfèrent pas la course du robot, tandis que
	* les erreurs sont des anomalies critiques qui empèchent ou stoppent la course du robot.
	*/
	typedef enum Error {
		CARD_NOT_FOUND,			///< La carte SD n'a pas été trouvée ou est illisible.
		FILE_NOT_FOUND,			///< Le fichier n'existe pas.
		FILE_NOT_READABLE,		///< Erreur d'ouverture du fichier.
		TOO_SHORT_SPAN,			///< La distance entre les 2 moteurs est inférieure à la largeur de la feuille et sa position horizontale.
		TOO_FEW_PARAMETERS,		///< Certains paramètres n'ont pas été lus dans le fichier de configuration.

		// * Warnings *

		UNKNOWN_SERIAL_CODE = 100,	///< Caractère envoyé au port série non reconnu. Utilisé uniquement sur le programme PC, jamais sur l'Arduino (mais il est préférable que les listes soient identiques).
		WRONG_CONFIG_LINE,		///< Ligne mal formée dans le fichier de configuration. Param : n° de la ligne.
		TOO_LONG_CONFIG_LINE,	///< Ligne trop longue dans le fichier de configuration. Param : n° de la ligne.
		UNKNOWN_CONFIG_KEY,		///< Clé inconnue dans le fichier de configuration. Params : clé, n° de la ligne.
		UNKNOWN_CONFIG_POSITION,///< Position inconnue dans le fichier de configuration.
		UNKNOWN_GCODE_FUNCTION,	///< Fonction gcode inconnue dans le fichier.
		UNKNOWN_GCODE_PARAMETER,	///< Paramètre gcode inconnu.
		WRONG_GCODE_PARAMETER,	///< Erreur lors de la lecture d'un paramètre. Params : paramètre, n° de la ligne.
		LEFT_LIMIT,				///< Le traceur a atteint la limite gauche.
		RIGHT_LIMIT,			///< Le traceur a atteint la limite droite.
		UPPER_LIMIT,			///< Le traceur a atteint la limite haute.
		LOWER_LIMIT,			///< Le traceur a atteint la limite basse.
	};

	/************
	* Attributs *
	************/
	// Commencent par m, pour "member".

	/// Objet pour manipuler le servo-moteur, utilisé avec la librairie \a Servo.
	Servo mServo;

	/// Le fichier svg contenant le dessin vectoriel à reproduire.
	File mFile;

	/// Longueur de la courroie gauche, en pas.
	unsigned long mLeftLength;

	/// Longueur de la courroie droite, en pas.
	unsigned long mRightLength;

	/// Offset horizontal pour placer le dessin au bon endroit dans la zone de dessin
	unsigned int mOffsetX;

	/// Offset vertical pour placer le dessin au bon endroit dans la zone de dessin
	unsigned int mOffsetY;

	/// Scale du dessin
	float mDrawingScale;

	/// Largeur du dessin en cours
	int mDrawingWidth;

	/// Hauteur du dessin en cours
	int mDrawingHeight;

	/// Longueur d'un pas (distance parcourue par la courroie en 1 pas, en mm).
	float mStepLength;

	/// Delai initial entre chaque pas du moteur qui a la plus grande distance à parcourir, (en micro-secondes).
	/// \details Le délai de l'autre moteur est calculé pour que les 2 moteurs arrivent au point de destination simultanément.
	float mDelay;

	/// Indique si le robot est en train d'écrire (\a true) ou non (\a false).   
	bool mIsWriting;

	/// Fonction actuellement en cours d'execution dans le fichier Gcode, utile lorsque le nom de fonction n'est pas spécifié de nouveau (ex : G1 X10 X20). Lorsque aucune fonction n'est en cours, la valeur est 255.
	byte mFunction;

	/************
	* Positions *
	************/

	/// Position horizontale actuelle du crayon sur le plan.
	float mPositionX;

	/// Position verticale actuelle du crayon sur le plan.
	float mPositionY;

	/// Position du servo-moteur.
	float mPositionZ;

	/*************
	* Paramètres *
	*************/

	// Commencent par mp, pour "member" et "parameter".

	// * Dessin *

	/// Distance entre les 2 moteurs.
	unsigned int mpSpan;

	/// Largeur de la feuille.
	unsigned int mpSheetWidth;

	/// Hauteur de la feuille.
	unsigned int mpSheetHeight;

	/// Position horizontale du coin supérieur gauche de la feuille par rapport au moteur gauche.
	unsigned int mpSheetPositionX;

	/// Position verticale du coin supérieur gauche de la feuille par rapport au moteur gauche.
	unsigned int mpSheetPositionY;

	// * Servo-moteur *

	/// Angle du servo-moteur lorsque le traceur écrit (en degrés).
	unsigned int mpServoWritingAngle;

	/// Angle du servo-moteur lorsque le traceur se déplace (en degrés).
	unsigned int mpServoMoovingAngle;

	/// Pause avant le déplacement du servo (en ms).
	unsigned int mpPreServoDelay;

	/// Pause après le déplacement du servo (en ms).
	unsigned int mpPostServoDelay;

	// * Moteurs *

	/// Nombre de pas (prendre en compte les micros-pas effectués par le driver moteur).
	unsigned int mpSteps;

	/// Diamètre du pignon (en microns).
	int mpDiameter;

	/// Direction du moteur gauche : \a true pour relâcher la courroie lorsque le moteur tourne dans le sens horaire et \a false dans le cas contraire.
	bool mpLeftDirection;

	/// Direction du moteur droit : \a true pour relâcher la courroie lorsque le moteur tourne dans le sens horaire et \a false dans le cas contraire.
	bool mpRightDirection;

	// * Divers *

	/// Pause avant de commencer à desiner (en ms)
	unsigned int mpInitialDelay;

	/// Échelle horizontale appliquée au dessin, permetant de le calibrer.
	float mpScaleX;
	/// Échelle verticale appliquée au dessin, permetant de le calibrer.
	float mpScaleY;

	/// Offset horizontal appliqué au dessin, permetant de le décaler.
	int mpOffsetX;

	/// Offset vertical appliqué au dessin, permetant de le décaler.
	int mpOffsetY;

	/// Vitesse par défaut du crayon, avant l'appel éventuel de setSpeed() (en m/s).
	unsigned int mpDefaultSpeed;

	/// Position par défaut du point de départ du crayon, avant l'appel éventuel de setPosition().
	/// Todo Prendre en charge dans le fichier de paramètres.
	Position mpInitPosition;

	/// Position du point d'arrivée du crayon
	/// Todo Prendre en charge dans le fichier de paramètres.
	Position mpEndPosition;

	/***********
	* Méthodes *
	***********/

	/**
	 *
	 */
	int processVar();

	/**
	 * \brief initialise les offset X et Y en fonction du fichier de config et de la position désirée du dessin.
	 */
	void initOffset(Position position);

	/**
	 * \brief initialise le scale en fonction de la taille désirée
	 */
	void initScale(DrawingSize size);

	/**
	 */
	void waitUntil(
				char msg);

	/**
	 * \brief Initialise le ratio entre le nombre de pas et la distance.
	 * \details Le ratio est calculé en fonction du diametre moteur et du nombre de pas.
	 * xx(mm)*ratio --> xx(pas)
	 * xx(pas)/ratio --> xx(mm)
	 * \todo Inverser le ratio car le nombre de pas est une valeur entière, éviter de le diviser.
	 */
	void initStepLength(
				);

	/**
	 * \brief Modifie l'échelle pour s'adapter à la largeur \a width et la hauteur \a height du dessin.
	 * \details Les dimentions du dessin sont récupérées sur le fichier svg.
	 * \todo Changer le nom et retourner l'échelle plutôt que de la modifier directement.
	 */
	void setDrawingScale(
				int width,
				int height);

	/**
	 * \brief Fonction appelée lorsque une erreur se produit.
	 * \details Éloigne le stylo de la paroi et stoppe le programme. Envoie le code d'erreur \a errNumber au PC, qui se charge d'afficher sa description. 
	 * \param p1 1er paramètre du warning (facultatif).
	 * \param p2 2eme paramètre du warning (facultatif).
	 * \todo Mettre en pause le traçé, quand la pause sera opérationnelle.
	 */
	void error(
				Error errorNumber,
				char *msg = "");

	/**
	 * \brief Fonction appelée lorsque un warning se produit.
	 * \details Envoie le code de warning \a errNumber à Processing, qui se charge d'afficher sa description sans affecter le déroulement du programme.
	 * \param p1 1er paramètre du warning (facultatif).
	 * \param p2 2eme paramètre du warning (facultatif).
	 */
	void warning(
				Error warningNumber,
				char *msg = "");

	/***********************
	* Commande du matériel *
	***********************/

	/**
	 * \brief Rotation du moteur gauche d'un pas.
	 * \param pull Sens du pas a effectuer : \a true pour tirer, \a false pour relacher.
	 */
	void leftStep(
				bool shouldPull);

	/**
	 * \brief Rotation du moteur droit d'un pas.
	 * \param pull Sens du pas a effectuer : \a true pour tirer, \a false pour relacher.
	 */
	void rightStep(
				bool shouldPull);

	/**
	 * \brief Alimente ou désalimente les moteurs.
	 * \details Éloigne le stylo de la paroi avant la désalimentation pour éviter de dessiner pendant la chute éventuelle du moteur.
	 * \param power \a true pour alimenter le moteur, \a false pour le désalimenter.
 	* \todo Séparer la désactivation moteur gauche et moteur droit.
	 */
	void power(
				bool shouldPower);

	/**************
	* Convertions *
	**************/

	/**
	 * \brief Récupère la position horizontale de \a position.
	 * \details Cf. type enum \a Position.
	 * \param position La position a convertir, de type \a Position.
	 * \return La position verticale de \a position.
	 */
	int positionToX(
				Position position);

	/**
	 * \brief Récupère la position verticale de \a position.
	 * \details Cf. type enum \a Position.
	 * \param position La position a convertir, de type \a Position.
	 * \return La position verticale de \a position.
	 */
	int positionToY(
				Position position);

	/**
	 * \brief Calcule la longueur de la courroie gauche pour la position [\a x ; \a y]
	 * \param x La position absolue horizontale du point.
	 * \param y La position absolue horizontale du point.
	 * \return La longueur de la courroie gauche pour cette position, en nombre de pas.
	 */
	long positionToLeftLength(
				float x,
				float y);

	/**
	 * \brief Calcule la longueur de la courroie droite pour la position [\a x ; \a y]
	 * \param x La position absolue horizontale du point.
	 * \param y La position absolue horizontale du point.
	 * \return La longueur de la courroie droite pour cette position, en nombre de pas.
	 */
	long positionToRightLength(
				float x,
				float y);

	/*******************
	* Lecture carte SD *
	*******************/

	/**
	 * \brief Initialise de la carte SD.
	 * \details Peut générer l'erreur 01 : Carte absente ou non reconnue.
	 * \param fileName Le nom du fichier à lire.
	 * \todo : si erreur et nom fichier > 8 car, proposer de vérifer si carte formatée en fat16.
	 */
	void sdInit(
				char *fileName);

	/**
	 * \brief Interprète la fonction gcode passée en paramètre.
	 * \detail Le curseur doit-être devant une fonction g-code, sinon envoie un warning. Ignore les espaces avant la fonction.
	 */
	void processSDLine(
				);

	/**

	*/
	void segment(
				float x,
				float y,
				bool shouldWrite);

	/**
	 * \brief Approche ou éloigne le crayon de la paroi.
	 * \param write \a true pour plaquer le crayon contre la paroi (traçé), \a false pour l'éloigner (déplacement)
	 */
	void writingPen(
				bool shouldWrite);

	/**
	 * \brief Convertit une chaine en valeur booléenne.
	 * \param value Chaine contenant soit "true", soit "false".
	 * \return \a true si la chaine est "true", \a false si la chaine est "false".
	 */
	bool atob(
				char *value);

	/**
	 * \brief Convertit une chaine en position
	 */
	Drawall::Position atop(char *str_pos);


	/**
	 * \Brief Charge les paramètres à partir d'un fichier de configuration présent sur la carte SD.
	 * \param fileName Le nom du fichier de configuration à charger.
	 */
	void loadParameters(
				char *fileName);

	// void printParameters(
	// 			);

};

#endif
