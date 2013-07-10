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
 * \file    params.h
 * \author  Nathanaël Jourdane
 * \version 1.0
 * \date    1er juillet 2013
 * \brief   Fichier de paramètres
 */

/**********************
* Définition des pins *
**********************/

// Pins 0 et 1 : utilisées par le port série (communication avec Processing)

/// Pin du capteur fin de course du moteur gauche.
#define PIN_LEFT_CAPTOR 2

/// Pin du capteur fin de course du moteur droit.
#define PIN_RIGHT_CAPTOR 3

/// Pin pour desactivation des moteurs (pin /en sur pololu).
#define PIN_OFF_MOTORS 4

/// Pin de commande du servo-moteur.
#define PIN_SERVO 5

/// Pin STEP (les pas) du moteur gauche.
#define PIN_LEFT_MOTOR_STEP 6

/// Pin DIR (la direction) du moteur gauche.
#define PIN_LEFT_MOTOR_DIR 7

/// Pin STEP (les pas) du moteur droit.
#define PIN_RIGHT_MOTOR_STEP 8 //

/// Pin DIR (la direction) du moteur droit.
#define PIN_RIGHT_MOTOR_DIR 9

/// Pin CS de la carte SD.
/// Snootlab et Adafruit : 10 - Sparkfun : 8
#define PIN_SD_CS 10

// Pins 11, 12 et 13 : utilisés par la carte SD (MOSI, MISO, SCK)

/// Pin de la diode infra-rouge pour télécommande.
#define PIN_REMOTE A0

/// Pin SCE de l'écran LCD
#define PIN_SCREEN_SCE A1

/// Pin RESET de l'écran LCD
#define PIN_SCREEN_RST A2

/// Pin DC de l'écran LCD
#define PIN_SCREEN_DC A3

/// Pin SDIN de l'écran LCD
#define PIN_SCREEN_SDIN A4

/// Pin SCLK de l'écran LCD
#define PIN_SCREEN_SCLK A5

/*************
* paramètres *
*************/

/***** du servo-moteur *****/

/// position lorsque le robot n'écrit pas (en degrés)
#define MIN_SERVO 30

/// position lorsque le robot écrit (en degrés)
#define MAX_SERVO 90

/// Pause avant chaque déplacement (en ms)
#define DELAY_BEFORE_SERVO 100

/// Pause après chaque déplacement (en ms)
#define DELAY_AFTER_SERVO 1000

/***** des moteurs *****/

/// nombre de pas (prendre en compte les micros-pas effectués par le driver-moteur)
#define STEPS 800

/// Diamètre du pignon (en mm)
#define DIAMETER 17.51

/// Direction du moteur gauche
#define LEFT_DIRECTION true

/// Direction du moteur droit
#define RIGHT_DIRECTION false

/*******************
* Autres variables *
*******************/

/// Pause avant de commencer à desiner (en ms)
#define DELAY_BEFORE_STARTING 5000

/// Vitesse de la communication série (en bauds)
#define SERIAL_BAUDS 57600

/// Échelle horizontale appliquée au dessin, permetant de le calibrer.
#define SCALE_X 1

/// Échelle verticale appliquée au dessin, permetant de le calibrer.
#define SCALE_Y 1.0256

/// Offset horizontal appliqué au dessin, permetant de le décaler.
#define OFFSET_X -65

/// Offset vertical appliqué au dessin, permetant de le décaler.
#define OFFSET_Y -38

/// Vitesse par défaut du crayon, avant l'appel éventuel de setSpeed() (en m/s).
#define DEFAULT_SPEED 10

/// Position par défaut du point de départ du crayon, avant l'appel éventuel de setPosition().
#define DEFAULT_POSITION CENTER

/// Position du point d'arrivée du crayon
#define END_POSITION CENTER
