/*
This work is licensed under the Creative Commons Attribution
- Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions
3.0 France License. To view a copy of this license, see COPYING.txt
file or visit http://creativecommons.org/licenses/by-nc-sa/3.0/fr/.
*/

/**********************
* Définition des pins *
***********************/

// Pins 0 et 1 : utilisées par le port série (communication avec Processing)

// Capteurs fin de course
#define PIN_LEFT_CAPTOR 2 // gauche
#define PIN_RIGHT_CAPTOR 3 // droite

// desactivation des moteurs (pin /en sur pololu)
#define PIN_OFF_MOTORS 4

// pin de commande du servo-moteur
#define PIN_SERVO 5

// moteur gauche
#define PIN_LEFT_MOTOR_STEP 6 // vitesse
#define PIN_LEFT_MOTOR_DIR 7 // direction

// moteur droit
#define PIN_RIGHT_MOTOR_STEP 8 // vitesse
#define PIN_RIGHT_MOTOR_DIR 9 // direction

// pin CS de la carte SD
// Snootlab et Adafruit : 10 - Sparkfun : 8
#define PIN_SD_CS 10

// Pins 11, 12 et 13 : utilisés par la carte SD (MOSI, MISO, SCK)

// Diode IR pour télécommande
#define PIN_REMOTE A0

// Écran LCD
#define PIN_SCREEN_SCE A1
#define PIN_SCREEN_RST A2
#define PIN_SCREEN_DC A3
#define PIN_SCREEN_SDIN A4
#define PIN_SCREEN_SCLK A5

/*************
* paramètres *
**************/

// servo-moteur :

#define MIN_SERVO 30 // position lorsque le robot n'écrit pas (en degrés)
#define MAX_SERVO 90 // position lorsque le robot écrit (en degrés)

#define DELAY_BEFORE_SERVO 200 // Pause avant chaque déplacement (en ms)
#define DELAY_AFTER_SERVO 200 // Pause après chaque déplacement (en ms)

// moteurs :

// nombre de pas
#define STEPS 400

// Diamètre du pignon
#define DIAMETER 12

// Direction des moteurs
#define LEFT_DIRECTION true
#define RIGHT_DIRECTION true

/*********************
* Valeurs par défaut *
**********************/

#define SCALE 1.0
#define SPEED_SCALE 1.0

#define DEFAULT_SPEED 10
