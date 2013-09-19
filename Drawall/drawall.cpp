
/*
 * This file is part of Drawall, a project for a robot which draws on walls.
 * See http://drawall.cc/ and https://github.com/roipoussiere/Drawall/.
 * 
 * Copyright (c) 2012-2013 Nathanaël Jourdane
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
 * \file	drawall.cpp
 * \author  Nathanaël Jourdane
 * \brief   Fichier principal de la bibliothèque.
 */

#include <drawall.h>

Drawall::Drawall()
{
}

void Drawall::begin(
			const char *fileName)
{
	Serial.begin(SERIAL_BAUDS);

	// Gestion de l'interruption à l'appui sur le BP pause.
	SREG |= 128;				// Interruptions globales ok
	EIMSK = B01;				// Interruption INT0 déclenchée à l'appui sur le BP pause : INT0 (pin2) = B01 ou INT1 (pin3) = B10 sur Atmega328.
	EICRA = B0011;				// Interruption déclenchée sur le front montant pour INT0 : état bas = B00, changement d'état = B01, front descendant = B10, front montant = B11

	// pins d'entrée
	pinMode(PIN_LEFT_CAPTOR, INPUT);
	pinMode(PIN_RIGHT_CAPTOR, INPUT);
	// pinMode(PIN_REMOTE, INPUT);

	// Activation des pullup internes pour les boutons
	digitalWrite(2, HIGH);		// INT0 (pour BP pause) est sur pin 2
	digitalWrite(PIN_LEFT_CAPTOR, HIGH);
	digitalWrite(PIN_RIGHT_CAPTOR, HIGH);

	// pins de sortie
	pinMode(PIN_OFF_MOTORS, OUTPUT);
	pinMode(PIN_LEFT_MOTOR_STEP, OUTPUT);
	pinMode(PIN_LEFT_MOTOR_DIR, OUTPUT);
	pinMode(PIN_RIGHT_MOTOR_STEP, OUTPUT);
	pinMode(PIN_RIGHT_MOTOR_DIR, OUTPUT);
	pinMode(PIN_SCREEN_SCE, OUTPUT);
	pinMode(PIN_SCREEN_RST, OUTPUT);
	pinMode(PIN_SCREEN_DC, OUTPUT);
	pinMode(PIN_SCREEN_SDIN, OUTPUT);
	pinMode(PIN_SCREEN_SCLK, OUTPUT);
	pinMode(PIN_SD_CS, OUTPUT);

	mServo.attach(PIN_SERVO);

	if (!SD.begin(PIN_SD_CS)) {
		error(CARD_NOT_FOUND);
	}
	// pour que write() fonctionne la 1ere fois
	mWriting = true;

	// Chargement des paramètres à partir du fichier de configuration
	loadParameters(fileName);
	// printParameters();

	mDrawingScale = 1;
	mDrawingOffsetX = 0;
	mDrawingOffsetY = 0;

	if (mpSpan < mpAreaWidth + mpAreaPositionX) {
		error(TOO_LONG_SPAN);
	}

	initStepLength();

	setPosition(mpDefaultPosition);

	// Calcul de la longueur des fils au début
	mLeftLength = positionToLeftLength(mPositionX, mPositionY);
	mRightLength = positionToRightLength(mPositionX, mPositionY);

	// Le point de départ de la 1ere figure est le point courant
	mStartFigureX = mPositionX;
	mStartFigureY = mPositionY;

	setSpeed(mpDefaultSpeed);

	// *** envoi des données d'initialisation à Processing ***
	Serial.write('\n');			// Début d'init

	Serial.print(mpSpan);
	Serial.write(',');

	Serial.print(mpAreaPositionX);
	Serial.write(',');
	Serial.print(mpAreaPositionY);
	Serial.write(',');

	Serial.print(mpAreaWidth);
	Serial.write(',');
	Serial.print(mpAreaHeight);
	Serial.write(',');

	Serial.print(mLeftLength);
	Serial.write(',');
	Serial.print(mRightLength);
	Serial.write(',');

	Serial.print(mStepLength * 1000);

	Serial.write('\n');			// Fin d'init

	// Pause jusqu'à l'appui sur le BP.
	// Serial.println("_Appuyez sur le bouton pour commencer");
	// while(digitalRead(PIN_BP) == LOW) {}

	power(true);
	delay(mpInitialDelay);
}

// Routine d'interruption
ISR(INT0_vect)
{
	EIMSK = 0;					// Bloquer INT0

	while (PIND & 4) {
	};							// On attend que le pin soit à '1'
	while (!(PIND & 4)) {
	};							// On attend que le pin soit à '0'
	// Pour un front descendant, inverser les 2 lignes.

	EIMSK = 1;					// Réautorisation de INT0
	EIFR = 1;					// Flag de INT0 remis à '0'
};

float Drawall::getPositionX(
			)
{
	return mPositionX;
}

float Drawall::getPositionY(
			)
{
	return mPositionY;
}

void Drawall::setPosition(
			float positionX,
			float positionY)
{
	mPositionX = positionX;
	mPositionY = positionY;
}

void Drawall::setPosition(
			Position position)
{
	setPosition(positionToX(position), positionToY(position));
}

float Drawall::positionToX(
			Position position)
{
	float x = 0;

	switch (position) {
	case UPPER_LEFT:
	case LEFT_CENTER:
	case LOWER_LEFT:
		x = 0;
		break;

	case UPPER_CENTER:
	case CENTER:
	case LOWER_CENTER:
		x = (float) (mpAreaWidth) / 2;
		break;

	case UPPER_RIGHT:
	case RIGHT_CENTER:
	case LOWER_RIGHT:
		x = (float) (mpAreaWidth);
		break;

	default:
		break;
	}

	return x;
}

float Drawall::positionToY(
			Position position)
{
	float y = 0;

	switch (position) {
	case UPPER_LEFT:
	case UPPER_CENTER:
	case UPPER_RIGHT:
		y = 0;
		break;

	case LEFT_CENTER:
	case CENTER:
	case RIGHT_CENTER:
		y = (float) (mpAreaHeight) / 2;
		break;

	case LOWER_LEFT:
	case LOWER_CENTER:
	case LOWER_RIGHT:
		y = (float) (mpAreaHeight);
		break;

	default:
		break;
	}

	return y;
}

void Drawall::initStepLength(
			)
{
	// mpSteps*2 car c'est seulement le front montant qui contôle le moteur
	mStepLength = (PI * float (mpDiameter)) /float (
				mpSteps * 2);
}

void Drawall::setSpeed(
			unsigned int speed)
{
	mDelay = 1000000 * mStepLength / float (
				speed);
}

long Drawall::positionToLeftLength(
			float positionX,
			float positionY)
{
	float width = ((float) mpAreaPositionX + positionX) / mStepLength;
	float height = ((float) mpAreaPositionY + positionY) / mStepLength;

	return sqrt(pow(width, 2) + pow(height, 2));
}

long Drawall::positionToRightLength(
			float positionX,
			float positionY)
{
	float width =
				((float) mpSpan - (float) mpAreaPositionX -
				positionX) / mStepLength;
	float height = ((float) mpAreaPositionY + positionY) / mStepLength;

	return sqrt(pow(width, 2) + pow(height, 2));
}

void Drawall::power(
			bool alimenter)
{
	if (alimenter) {
		digitalWrite(PIN_OFF_MOTORS, LOW);
		Serial.write('a');		// Processing: a = alimenter
	} else {
		digitalWrite(PIN_OFF_MOTORS, HIGH);
		write(false);
		Serial.write('b');		// Processing: b = désalimenter
	}
}

void Drawall::write(
			bool write)
{
	// Si on souhaite écrire et que le stylo n'ecrit pas
	if (write && !mWriting) {
		delay(mpPreServoDelay);
		mServo.write(mpServoOn);
		delay(mpPostServoDelay);

		Serial.write('w');		// Processing: w = ecrire
		mWriting = true;
	}
	// si on ne veut pas ecrire et que le stylo ecrit
	else if (!write && mWriting) {
		delay(mpPreServoDelay);
		mServo.write(mpServoOff);
		delay(mpPostServoDelay);

		Serial.write('x');		// Processing: x = ne pas ecrire
		mWriting = false;
	}
}

bool Drawall::positionInsideArea(
			float x,
			float y)
{
	bool inside = false;

	if (x < 0) {
		x = 0;
		Serial.write('W');
		Serial.write(50);
	} else if (x > mpAreaWidth) {
		x = mpAreaWidth;
		Serial.write('W');
		Serial.write(51);
	} else if (y < 0) {
		y = 0;
		Serial.write('W');
		Serial.write(52);
	} else if (y > mpAreaHeight) {
		y = mpAreaHeight;
		Serial.write('W');
		Serial.write(53);
	} else {
		inside = true;
	}

	if (!inside) {
		mFictivePosX = x;
		mFictivePosY = y;
	}

	return inside;
}

void Drawall::line(
			float bX,
			float bY,
			bool writing)
{
	/*
	 * aX, aY: Position du point courant
	 * aG, aD: Longueur actuelle des fils.
	 * 
	 * bX, bY: position du point de destination.
	 * bG, bD: Longueur des fils à la destination.
	 * 
	 * nbPasG, nbPasD: Nombre de pas à faire pour aller à destination.
	 * 
	 * delaiG, delaiD: Delai avant chaque pas.
	 * 
	 * 1) on calcule la longueur des fils à la destination
	 * 2) on fait la différence par rapport à la longueur actuelle
	 * 3) Calcule le délai en fonction du rapport de longeur des fils
	 * 
	 * Stoque la position voulue avant qu'elle soit modifiée par les limites
	 */

	// si le crayon est dans la zone de dessin et qu'il doit écrire, écrit.
	if (positionInsideArea(bX, bY)) {
		if (writing) {
			write(true);
		} else {
			write(false);
		}
	}
	// Calcul de la longueur fils à la destination (en pas)

	// échelle
	float sX = (float) mpScaleX * mDrawingScale;
	float sY = (float) mpScaleY * mDrawingScale;

	// offset
	int oX = (int) mpOffsetX + mDrawingOffsetX;
	int oY = (int) mpOffsetY + mDrawingOffsetY;

	unsigned long bG = positionToLeftLength(bX * sX + oX, bY * sY + oY);
	unsigned long bD = positionToRightLength(bX * sX + oX, bY * sY + oY);

	// nombre de pas à faire
	long nbPasG = bG - mLeftLength;
	long nbPasD = bD - mRightLength;

	bool sensGHaut = true;
	bool sensDHaut = true;

	float delaiG;
	float delaiD;

	unsigned long dernierTempsG;
	unsigned long dernierTempsD;

	// calcul de la direction
	if (nbPasG < 0) {
		sensGHaut = false;
	}

	if (nbPasD < 0) {
		sensDHaut = false;
	}
	// On a le sens, donc on peut retirer le signe pour simplifier les calculs
	nbPasG = fabs(nbPasG);
	nbPasD = fabs(nbPasD);

	if (nbPasG > nbPasD) {
		delaiG = mDelay;
		delaiD = mDelay * ((float) nbPasG / (float) nbPasD);
	} else {
		delaiD = mDelay;
		delaiG = mDelay * ((float) nbPasD / (float) nbPasG);
	}

	dernierTempsG = micros();
	dernierTempsD = micros();

	if (sensGHaut) {
		digitalWrite(PIN_LEFT_MOTOR_DIR, mpLeftDirection);
	} else {
		digitalWrite(PIN_LEFT_MOTOR_DIR, !mpLeftDirection);
	}

	if (sensDHaut) {
		digitalWrite(PIN_RIGHT_MOTOR_DIR, mpRightDirection);
	} else {
		digitalWrite(PIN_RIGHT_MOTOR_DIR, !mpRightDirection);
	}

	while (nbPasG > 0 || nbPasD > 0) {
		// si le delai est franchi et qu'il reste des pas à faire
		if ((nbPasG > 0) && (micros() - dernierTempsG >= delaiG)) {
			// stoque le temps actuel dans lastTimer
			dernierTempsG = micros();

			// incremente ou decremente (en fonction de la direction)
			if (sensGHaut) {
				mLeftLength++;
				Serial.write('L');
			} else {
				mLeftLength--;
				Serial.write('l');
			}

			// decremente le nb de pas restants
			nbPasG--;

			// Effectue le pas
			leftStep();
		}

		if ((nbPasD > 0) && (micros() - dernierTempsD >= delaiD)) {
			// stoque le temps actuel dans lastTimer
			dernierTempsD = micros();

			// incremente ou decremente (en fonction de la direction)
			if (sensDHaut) {
				mRightLength++;
				Serial.write('R');
			} else {
				mRightLength--;
				Serial.write('r');
			}

			// decremente le nb de pas restants
			nbPasD--;

			// Effectue le pas
			rightStep();
		}
	}

	mPositionX = bX;
	mPositionY = bY;
}

void Drawall::leftStep(
			)
{
	digitalWrite(PIN_LEFT_MOTOR_STEP, mLeftLength % 2);
}

void Drawall::rightStep(
			)
{
	digitalWrite(PIN_RIGHT_MOTOR_STEP, mRightLength % 2);
}

void Drawall::lineAbs(
			float x,
			float y)
{
	mQuadraticCurveX = 0;
	mQuadraticCurveY = 0;
	mCubicCurveX = 0;
	mCubicCurveY = 0;

	int longmax = 5;

	float longX = abs(x - mPositionX);
	float longY = abs(y - mPositionY);

	float miniX;
	float miniY;
	int boucle;

	if (longX > longmax || longY > longmax) {

		if (longX > longY) {
			boucle = ceil(longX / longmax);
		} else {
			boucle = ceil(longY / longmax);
		}

		miniX = ((x - mPositionX) / boucle);
		miniY = ((y - mPositionY) / boucle);

		for (int i = 0; i < boucle; i++) {
			line(mPositionX + miniX, mPositionY + miniY, true);
		}
	}
	line(x, y, true);
}

void Drawall::lineRel(
			float x,
			float y)
{
	lineAbs(mPositionX + x, mPositionY + y);
}

void Drawall::rectangle(
			float x,
			float y)
{
	lineRel(x, 0);
	lineAbs(0, y);
	lineAbs(-x, 0);
	lineAbs(0, -y);
}

void Drawall::area(
			)
{
	moveAbs(0, 0);
	rectangle(mpAreaWidth, mpAreaHeight);
}

void Drawall::drawingArea(
			const char *fileName)
{
	sdInit(fileName);
	float width = getNumericAttribute("width");
	float height = getNumericAttribute("height");

	setDrawingScale(width, height);
	moveAbs(0, 0);
	rectangle(width, height);

	mFile.close();
}

void Drawall::endFigure(
			)
{
	lineAbs(mStartFigureX, mStartFigureY);
}

void Drawall::moveAbs(
			float x,
			float y)
{
	line(x, y, false);

	mStartFigureX = x;
	mStartFigureY = y;
}

void Drawall::move(
			Position position)
{
	moveAbs(positionToX(position), positionToY(position));
}

void Drawall::moveRel(
			float x,
			float y)
{
	moveAbs(mPositionX + x, mPositionY + y);
}

void Drawall::cubicCurveAbs(
			float x1,
			float y1,
			float x2,
			float y2,
			float x,
			float y)
{
	float x0 = mPositionX;
	float y0 = mPositionY;
	float ptx, pty;
	float t;

	for (t = 0; t <= 1; t += 0.01) {
		ptx = x0 * pow((1 - t), 3) + 3 * x1 * t * pow(1 - t,
					2) + 3 * x2 * pow(t, 2) * (1 - t) + x * pow(t, 3);
		pty = y0 * pow((1 - t), 3) + 3 * y1 * t * pow(1 - t,
					2) + 3 * y2 * pow(t, 2) * (1 - t) + y * pow(t, 3);
		line(ptx, pty, true);
	}

	// finit le dernier trajet au cas ou ça ne tombe pas juste
	line(x, y, true);

	mQuadraticCurveX = 0;
	mQuadraticCurveY = 0;
	mCubicCurveX = x2;
	mCubicCurveY = y2;
}

void Drawall::cubicCurveRel(
			float x1,
			float y1,
			float x2,
			float y2,
			float x,
			float y)
{
	cubicCurveAbs(mPositionX + x1, mPositionY + y1, mPositionX + x2,
				mPositionY + y2, mPositionX + x, mPositionY + y);
}

void Drawall::cubicCurveAbs(
			float x2,
			float y2,
			float x,
			float y)
{
	float x1, y1;

	if (mCubicCurveX == 0 && mCubicCurveY == 0) {
		x1 = x;
		y1 = y;
	} else {
		x1 = 2 * mFictivePosX - mCubicCurveX;
		y1 = 2 * mFictivePosY - mCubicCurveY;
	}

	cubicCurveAbs(x1, y1, x2, y2, x, y);
}

void Drawall::cubicCurveRel(
			float x2,
			float y2,
			float x,
			float y)
{
	cubicCurveAbs(mPositionX + x2, mPositionY + y2, mPositionX + x,
				mPositionY + y);
}

void Drawall::quadraticCurveAbs(
			float x1,
			float y1,
			float x,
			float y)
{
	float x0 = mPositionX;
	float y0 = mPositionY;
	float ptx, pty;

	for (float t = 0; t <= 1; t += 0.01) {
		ptx = x0 * pow(1 - t, 2) + 2 * x1 * t * (1 - t) + x * pow(t, 2);
		pty = y0 * pow(1 - t, 2) + 2 * y1 * t * (1 - t) + y * pow(t, 2);
		line(ptx, pty, true);
	}
	line(x, y, true);

	mCubicCurveX = 0;
	mCubicCurveY = 0;
	mQuadraticCurveX = x1;
	mQuadraticCurveY = y1;
}

void Drawall::quadraticCurveRel(
			float x1,
			float y1,
			float x,
			float y)
{
	quadraticCurveAbs(mPositionX + x1, mPositionY + y1, mPositionX + x,
				mPositionY + y);
}

void Drawall::quadraticCurveAbs(
			float x,
			float y)
{
	float x1, y1;

	if (mQuadraticCurveX == 0 && mQuadraticCurveY == 0) {
		x1 = x;
		y1 = y;
	} else {
		x1 = 2 * mFictivePosX - mQuadraticCurveX;
		y1 = 2 * mFictivePosY - mQuadraticCurveY;
	}

	quadraticCurveAbs(x1, y1, x, y);
}

void Drawall::quadraticCurveRel(
			float x,
			float y)
{
	quadraticCurveAbs(mPositionX + x, mPositionY + y);
}

void Drawall::getAttribute(
			const char *attribute,
			char *value)
{
	int pos = mFile.position();

	mFile.seek(0);

	// att : requette avec =" , ex : svg="
	// crée une chaine de la meme taille que valeur (+2 pour (=") )
	char att[strlen(attribute) + 2];

	char car = ' ';
	int i = 0;

	// copie la chaine cherchée et ajoute le (=")  
	strcpy(att, attribute);
	strcat(att, "=\"");

	// se positionne à la fin de la requette cherchée + (=")
	// pour récupérer la valeur de la requette
	sdFind(att);

	// tant qu'il y a qqch à lire
	// et qu'on n'est pas rendu au (") correspondant à la fin de la valeur
	while (mFile.available() && car != '"') {
		car = mFile.read();
		value[i] = car;
		i++;
	}

	// Quand on a fini, remplace le (") par le car. de fin de chaine.
	value[i - 1] = '\0';

	mFile.seek(pos);
}

bool Drawall::isNumber(
			char car)
{
	int i;
	const char *chiffres = "-.0123456789";

	for (i = 0; chiffres[i] != '\0'; i++) {
		if (car == chiffres[i]) {
			return true;
		}
	}

	// Aucun chiffre n'a été trouvé, renvoie faux
	return false;
}

float Drawall::getNumericAttribute(
			const char *attribute)
{
	// nombre de max 20 caractères
	char chaine[20 + 1];

	// se positionne en début de fichier
	mFile.seek(0);

	// recuperation des valeurs sous forme de chaine
	getAttribute(attribute, chaine);

	// unité de mesure de la valeur, ex cm ou mm
	char unite[3];

	// chaine contenant le nombre, ex : -3456.345
	char nbCh[strlen(chaine)];

	// chaine convertie en float
	float nbFl;

	int i;

	// taille des 2 chaines
	int tNbCh = 0;
	int tUnite = 0;

	// parcourt toute la chaine.
	for (i = 0; chaine[i] != '\0'; i++) {
		if (isNumber(chaine[i])) {
			// remplit nbCh avec les chiffres
			nbCh[tNbCh] = chaine[i];

			tNbCh++;
		}

		else {
			unite[tUnite] = chaine[i];
			tUnite++;
		}
	}

	// met la fin de chaine
	nbCh[tNbCh] = '\0';
	unite[tUnite] = '\0';

	// Conversion de la chaine en float
	nbFl = atof(nbCh);

	// Conversion des unités en mm :

	if (!strcmp(unite, "px")) {
		nbFl *= 1;
	}

	else if (!strcmp(unite, "pt")) {
		nbFl *= 1.25;
	}

	else if (!strcmp(unite, "pc")) {
		nbFl *= 15;
	}

	else if (!strcmp(unite, "mm")) {
		nbFl *= 3.543307;
	}

	else if (!strcmp(unite, "cm")) {
		nbFl *= 35.43307;
	}

	else if (!strcmp(unite, "in")) {
		nbFl *= 90;
	}

	else {
		nbFl *= 1;
	}

	return nbFl;
}

bool Drawall::sdFind(
			const char *word)
{
	int i = 0;
	char car;

	// tant qu'il y a qqch à lire
	while (mFile.available()) {
		car = mFile.read();

		// si le caractère correspond, passe au suivant
		if (car == word[i]) {
			i++;

			// si fin du mot (mot trouvé)
			if (word[i] == '\0') {
				// on s'est bien positionné, opération réeussie! :)
				return true;
			}
		}
		// si un des car. n'est pas bon on reprends depuis le debut
		else {
			i = 0;
		}
	}

	// si on est là c'est qu'on a pas trouvé le mot, fail!
	return false;
}

void Drawall::draw(
			)
{
	char car;

	// tableau contenant les valeurs à lançer dans la requette
	// (max 7 valeurs avec la fonction arc() )
	float tNb[7];

	// tant qu'il y a qqch à lire
	while (mFile.available()) {
		car = mFile.read();

		// Appelle les fonctions de dessin svg en fonction du caractère détecté
		switch (car) {

		case 'M':				// deplacerABS (2 args)
			// lis les paramètres tant qu'il y a des nombres
			do {
				// la fonction deplacerABS() attends 2 paramètres
				// on les récupère sous forme de tableau (tNb)
				getParameters(tNb, 2);

				// appel de la fonction
				moveAbs(tNb[0], tNb[1]);
			}
			// teste le prochain car (le curseur ne bouge pas)
			while (isNumber(mFile.peek()));

			break;

		case 'm':
			do {
				getParameters(tNb, 2);
				moveRel(tNb[0], tNb[1]);
			}
			while (isNumber(mFile.peek()));
			break;

		case 'Z':
		case 'z':
			endFigure();
			break;

		case 'L':
			do {
				getParameters(tNb, 2);
				lineAbs(tNb[0], tNb[1]);
			}
			while (isNumber(mFile.peek()));

			break;

		case 'l':
			do {
				getParameters(tNb, 2);
				lineRel(tNb[0], tNb[1]);
			}
			while (isNumber(mFile.peek()));
			break;

		case 'C':
			do {
				getParameters(tNb, 6);
				cubicCurveAbs(tNb[0], tNb[1], tNb[2], tNb[3], tNb[4],
							tNb[5]);
			}
			while (isNumber(mFile.peek()));
			break;

		case 'c':
			do {
				getParameters(tNb, 6);
				cubicCurveRel(tNb[0], tNb[1], tNb[2], tNb[3], tNb[4],
							tNb[5]);
			}
			while (isNumber(mFile.peek()));
			break;

		case 'S':
			do {
				getParameters(tNb, 4);
				cubicCurveAbs(tNb[0], tNb[1], tNb[2], tNb[3]);
			}
			while (isNumber(mFile.peek()));
			break;

		case 's':
			do {
				getParameters(tNb, 4);
				cubicCurveRel(tNb[0], tNb[1], tNb[2], tNb[3]);
			}
			while (isNumber(mFile.peek()));
			break;

		case 'Q':
			do {
				getParameters(tNb, 4);
				quadraticCurveAbs(tNb[0], tNb[1], tNb[2], tNb[3]);
			}
			while (isNumber(mFile.peek()));
			break;

		case 'q':
			do {
				getParameters(tNb, 4);
				quadraticCurveRel(tNb[0], tNb[1], tNb[2], tNb[3]);
			}
			while (isNumber(mFile.peek()));
			break;

		case 'T':
			do {
				getParameters(tNb, 2);
				quadraticCurveAbs(tNb[0], tNb[1]);
			}
			while (isNumber(mFile.peek()));
			break;

		case 't':
			do {
				getParameters(tNb, 2);
				quadraticCurveRel(tNb[0], tNb[1]);
			}
			while (isNumber(mFile.peek()));
			break;

			// si on détecte la fin du contenu de "d"
			// on a parcouru toute les données, ça a reeussi !!
		case '"':
			return;
			break;

			// Caractères inutiles
		case ' ':
		case '\n':
			break;

			// Warning si la lettre n'est pas reconnue
		default:
			Serial.write('W');
			Serial.write(60);
			Serial.print("_'");
			Serial.write(car);
			Serial.println("'.");
			break;
		}

	}

	// si on est là c'est qu'on a parcouru tout le fichier
	// sans trouver le (") de fin de la balise (d) : fail

	error(INCOMPLETE_SVG);
}

void Drawall::error(
			Error errNumber)
{
	if (errNumber < 100) {		// Gestion Erreurs
		Serial.write('E');
		Serial.write((byte) errNumber);
		delay(1000);
		write(false);
		while (true) ;
	} else {					// Gestion Warnings
		Serial.write('W');
		Serial.write((byte) errNumber);
	}
}

void Drawall::setDrawingScale(
			int width,
			int height)
{
	float scaleX = float (
				mpAreaWidth) / float (
				width);
	float scaleY = float (
				mpAreaHeight) / float (
				height);

	if (scaleX > scaleY) {
		mDrawingScale = scaleY;
		mDrawingOffsetX = float (
					mpAreaWidth) / 2 - float (
					width) * scaleY / 2;
	} else {
		mDrawingScale = scaleX;
		mDrawingOffsetY = float (
					mpAreaHeight) / 2 - float (
					height) * scaleX / 2;
	}
}

void Drawall::sdInit(
			const char *fileName)
{
	mFile = SD.open(fileName);

	if (!mFile) {
		// Err. 02 : Erreur d'ouverture de fichier.
		error(FILE_NOT_FOUND);
	}
	// se positionne en début de fichier
	mFile.seek(0);

	// Se positionne jusqu'à la balise SVG
	// Si on ne la trouve pas, on renvoie une erreur
	if (!sdFind("<svg")) {
		// Err. 12 : Le fichier n'est pas un fichier svg.
		error(NOT_SVG_FILE);
	}
}

void Drawall::svg(
			const char *fileName)
{
	mDrawingScale = 1;
	mDrawingOffsetX = 0;
	mDrawingOffsetY = 0;

	sdInit(fileName);

	setDrawingScale(getNumericAttribute("width"),
				getNumericAttribute("height"));

	// Se positionne jusqu'à la balise PATH
	// Si on ne la trouve pas, on renvoie une erreur
	if (!sdFind("<path")) {
		error(NOT_SVG_PATH);
	}
	// tant que l'on trouve le début des données d'un traçé, on dessine
	while (sdFind("d=\"")) {
		draw();
	}

	// Fin du dessin svg
	mFile.close();
	Serial.write('n');
}

void Drawall::getParameters(
			float *tNb,
			int nbParams)
{
	// chaine qui va contenir le nombre à convertir en float
	// ex: (23456.43532)
	char valeur[20];

	char car;

	int i, j;

	for (i = 0; i < nbParams; i++) {
		// passe les espaces et virgules
		do {
			car = mFile.read();
		}
		while (car == ' ' || car == ',');

		// tant que les car. sont des chiffres, lis et stoque dans une chaine
		for (j = 0; isNumber(car); j++) {
			valeur[j] = car;
			car = mFile.read();
		}
		valeur[j] = '\0';

		tNb[i] = atof(valeur);
	}
}

void Drawall::end(
			)
{
	move(mpEndPosition);
	power(false);
	while (true) ;
}

void Drawall::loadParameters(
			const char *fileName)
{
	const byte BUFFER_SIZE = 32;	// Taille buffer

	char buffer[BUFFER_SIZE];	// Stocke une ligne du fichier
	char *key;					// Chaine pour la clé
	char *value;				// Chaine pour la valeur

	byte i;						// Itérateur
	byte line_lenght;			// Longueur de la ligne
	byte line_counter = 0;		// Compteur de lignes

	// Initialise la carte SD
	pinMode(PIN_SD_CS, OUTPUT);

	// Ici, problème avec la communication avec Processing !!!

	// Test existence fichier
	/*if(!SD.exists(fileName)) {
	 * Serial.print("Le fichier '");
	 * Serial.print(fileName);
	 * Serial.println("' n'existe pas.");
	 * while(true){};
	 * } */

	// Ouvre le fichier de configuration
	File configFile = SD.open(fileName, FILE_READ);

	if (!configFile) {
		error(FILE_NOT_READABLE);
	}
	// Tant que non fin de fichier
	while (configFile.available() > 0) {
		// Récupère une ligne entière dans le buffer
		i = 0;
		while ((buffer[i++] = configFile.read()) != '\n') {

			// Si la ligne dépasse la taille du buffer
			if (i == BUFFER_SIZE) {
				// On finit de lire la ligne mais sans stocker les données
				while (configFile.read() != '\n') {
				};
				break;			// Et on arrête la lecture de cette ligne
			}
		}

		// On garde de côté le nombre de char stocké dans le buffer
		line_lenght = i;

		// Finalise la chaine de caractéres ASCIIZ en supprimant le \n au passage
		buffer[--i] = '\0';

		// Incrémente le compteur de lignes
		++line_counter;

		// Ignore les lignes vides ou les lignes de commentaires
		if (buffer[0] == '\0' || buffer[0] == '#') {
			continue;
		}
		// Gestion des lignes trop grande
		if (i == BUFFER_SIZE) {
			error(W_TOO_LONG_LINE);
			// Serial.println(line_counter, DEC);
		}
		// Cherche l'emplacement de la clef en ignorant les espaces et les tabulations en début de ligne
		i = 0;
		while (buffer[i] == ' ' || buffer[i] == '\t') {
			if (++i == line_lenght) {
				break;
			}
			// Ignore les lignes contenant uniquement des espaces et/ou des tabulations
		}

		if (i == line_lenght) {
			continue;			// Gère les lignes contenant uniquement des espaces et/ou des tabulations
		}
		key = &buffer[i];

		// Cherche l'emplacement du séparateur = en ignorant les espaces et les tabulations apres la clé
		while (buffer[i] != ' ' && buffer[i] != '\t') {
			if (++i == line_lenght) {
				error(W_WRONG_LINE);
				// Serial.println(line_counter, DEC);
				break;			// Ignore les lignes mal formées
			}
		}

		if (i == line_lenght) {
			continue;			// Gère les lignes mal formé
		}
		// Transforme le séparateur en \0 et continue
		buffer[i++] = '\0';

		// Cherche l'emplacement de la valeur en ignorant les espaces et les tabulations âpres le séparateur
		while (buffer[i] == ' ' || buffer[i] == '\t') {
			if (++i == line_lenght) {
				error(W_WRONG_LINE);
				// Serial.println(line_counter, DEC);
				break;			// Ignore les lignes mal formées
			}
		}

		if (i == line_lenght) {
			continue;			// Gère les lignes mal formées
		}

		value = &buffer[i];

		// Transforme les données texte en valeur utilisable
		if (!strcmp(key, "fileName")) {
			strcpy(this->mpFileName, value);
		} else if (!strcmp(key, "span")) {
			mpSpan = atoi(value);
		} else if (!strcmp(key, "areaWidth")) {
			mpAreaWidth = atoi(value);
		} else if (!strcmp(key, "areaHeight")) {
			mpAreaHeight = atoi(value);
		} else if (!strcmp(key, "areaPositionX")) {
			mpAreaPositionX = atoi(value);
		} else if (!strcmp(key, "areaPositionY")) {
			mpAreaPositionY = atoi(value);
		} else if (!strcmp(key, "servoOff")) {
			mpServoOff = atoi(value);
		} else if (!strcmp(key, "servoOn")) {
			mpServoOn = atoi(value);
		} else if (!strcmp(key, "preServoDelay")) {
			mpPreServoDelay = atoi(value);
		} else if (!strcmp(key, "postServoDelay")) {
			mpPostServoDelay = atoi(value);
		} else if (!strcmp(key, "steps")) {
			mpSteps = atoi(value);
		} else if (!strcmp(key, "diameter")) {
			mpDiameter = atof(value);
		} else if (!strcmp(key, "leftDirection")) {
			mpLeftDirection = false;
			if (!strcmp(value, "true")) {
				mpLeftDirection = true;
			}
		} else if (!strcmp(key, "rightDirection")) {
			mpRightDirection = false;
			if (!strcmp(value, "true")) {
				mpRightDirection = true;
			}
		} else if (!strcmp(key, "initialDelay")) {
			mpInitialDelay = atoi(value);
		} else if (!strcmp(key, "scaleX")) {
			mpScaleX = atoi(value);
		} else if (!strcmp(key, "scaleY")) {
			mpScaleY = atoi(value);
		} else if (!strcmp(key, "offsetX")) {
			mpOffsetX = atoi(value);
		} else if (!strcmp(key, "offsetY")) {
			mpOffsetY = atoi(value);
		} else if (!strcmp(key, "defaultSpeed")) {
			mpDefaultSpeed = atoi(value);
		} else {
			Serial.print("Cle inconnue : ");
			Serial.println(key);
		}

	}

	// Ferme le fichier de configuration
	configFile.close();
}

/*void Drawall::printParameters()
 * {
 * Serial.print("fileName = ");
 * Serial.println(mpFileName);
 * 
 * Serial.print("span = ");
 * Serial.println(mpSpan);
 * 
 * Serial.print("areaWidth = ");
 * Serial.println(mpAreaWidth);
 * 
 * Serial.print("areaHeight = ");
 * Serial.println(mpAreaHeight);
 * 
 * Serial.print("areaPositionX = ");
 * Serial.println(mpAreaPositionX);
 * 
 * Serial.print("areaPositionY = ");
 * Serial.println(mpAreaPositionY);
 * 
 * Serial.print("servoOff = ");
 * Serial.println(mpServoOff);
 * 
 * Serial.print("servoOn = ");
 * Serial.println(mpServoOn);
 * 
 * Serial.print("preServoDelay = ");
 * Serial.println(mpPreServoDelay);
 * 
 * Serial.print("postServoDelay = ");
 * Serial.println(mpPostServoDelay);
 * 
 * Serial.print("steps = ");
 * Serial.println(mpSteps);
 * 
 * Serial.print("mpDiameter = ");
 * Serial.println(mpDiameter);
 * 
 * Serial.print("leftDirection = ");
 * Serial.println(mpLeftDirection);
 * 
 * Serial.print("rightDirection = ");
 * Serial.println(mpRightDirection);
 * 
 * Serial.print("initialDelay = ");
 * Serial.println(mpInitialDelay);
 * 
 * Serial.print("scaleX = ");
 * Serial.println(mpScaleX);
 * 
 * Serial.print("scaleY = ");
 * Serial.println(mpScaleY);
 * 
 * Serial.print("offsetX = ");
 * Serial.println(mpOffsetX);
 * 
 * Serial.print("offsetY = ");
 * Serial.println(mpOffsetY);
 * 
 * Serial.print("defaultSpeed = ");
 * Serial.println(mpDefaultSpeed);
 * } */
