/*
This work is licensed under the Creative Commons Attribution
- Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions
3.0 France License. To view a copy of this license, visit
http://creativecommons.org/licenses/by-nc-sa/3.0/fr/.
*/

/**********************
* Définition des pins *
***********************/

// Ne pas prendre les pins 0 et 1 qui sont utilisées par le port série (communication avec Processing)

// pin de commande du servo-moteur
#define PIN_SERVO 3

// pin pour le bouton play/pause
#define PIN_BP 4

// desactivation des moteurs (pin /en sur pololu)
#define PIN_OFF_MOT 5

// direction des moteurs
#define PIN_DIR_MOT_G 6 // moteur gauche
#define PIN_DIR_MOT_D 7 // moteur droit

// vitesse des moteurs
#define PIN_CLK_MOT_G 8 // moteur gauche
#define PIN_CLK_MOT_D 9 // moteur droit

// pin CS de la carte sd
#define PIN_CS 10
// shield Ethernet Arduino : pin 4
// shield et module SD Adafruit : pin 10
// shield SD Snootlab : pin 10
// shield SD Sparkfun : pin 8


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
#define NBPAS 200

// Diamètre du pignon
#define DIAMETRE 12
