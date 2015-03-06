/*
 * This file is part of DraWall.
 * DraWall is free software: you can redistribute it and/or modify it under the terms of the GNU
 * General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * DraWall is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU
 * General Public License along with DraWall. If not, see <http://www.gnu.org/licenses/>.
 * © 2012–2015 Nathanaël Jourdane
 */

/**
 * Main library file.
 */

//#define I_AM_CODING
#include <drawall.h>

// TODO remove all public methods, they are useless and it will save space.
// TODO use all variables without Arduino.h defines, like true, false, etc.
// TODO Make a Parameter object
// TODO: Interrupt routine with ISR(INT0_vect){...}
// TODO: make a method to write data on serial, with #if EN_SERIAL test

void Drawall::start() {
	pinInitialization();

#if EN_SERIAL
	Serial.begin(SERIAL_BAUDS);
#endif

	if (!SD.begin(PIN_SD_CS)) {
		error(ERR_CARD_NOT_FOUND);
	}

	// Load all parameters from the configuration file
	loadParameters();

	drawingInsertConf = drawingInsertConf/float(100)*PLT_MAX_SERVO_ANGLE+PLT_MIN_SERVO_ANGLE;
	movingInsertConf = movingInsertConf/float(100)*PLT_MAX_SERVO_ANGLE+PLT_MIN_SERVO_ANGLE;
	drawingScale = (sheetWidthConf>sheetHeightConf) ? sheetWidthConf/float(65535) : sheetHeightConf/float(65535);

	servo.attach(PIN_SERVO);
	servo.write(movingInsertConf);

	isWriting = true; // to make write() works for the first time.

	stepLength = getStepLength();

	// Get the belts length
	leftLength = positionToLeftLength(initPosXConf, initPosYConf);
	rightLength = positionToRightLength(initPosXConf, initPosYConf);

	delayBetweenSteps = getDelay(maxSpeedConf);

#if EN_SERIAL
	// Send initialization data to computer
	Serial.println("READY");
	delay(100);

	Serial.write(DRAW_START_INSTRUCTIONS);
	Serial.println(spanConf);
	Serial.println(sheetPosXConf);
	Serial.println(sheetPosYConf);
	Serial.println(sheetWidthConf);
	Serial.println(sheetHeightConf);
	Serial.println(leftLength);
	Serial.println(rightLength);
	Serial.println(stepLength * 1000);
	Serial.write(DRAW_END_INSTRUCTIONS);
#endif

	digitalWrite(PIN_ENABLE_MOTORS, LOW); // power

#if EN_SERIAL
	Serial.write(DRAW_WAITING);
#endif

	delay(initDelayConf);
}

void Drawall::pinInitialization() {
	// Motors

	pinMode(PIN_ENABLE_MOTORS, OUTPUT);

	pinMode(PIN_LEFT_MOTOR_STEPS, OUTPUT);
	pinMode(PIN_RIGHT_MOTOR_STEPS, OUTPUT);

	pinMode(PIN_LEFT_MOTOR_DIR, OUTPUT);
	pinMode(PIN_RIGHT_MOTOR_DIR, OUTPUT);

	// Memory card
	pinMode(PIN_SD_CS, OUTPUT);

#if EN_LIMIT_SENSORS
	// Limit sensors

	pinMode(PIN_LEFT_CAPTOR, INPUT);
	pinMode(PIN_RIGHT_CAPTOR, INPUT);

	// Enable sensors internal pull-ups
	digitalWrite(PIN_LEFT_CAPTOR, HIGH);
	digitalWrite(PIN_RIGHT_CAPTOR, HIGH);
	// TODO: set sensors interrupt here
#endif

#if EN_REMOTE_SUPPORT
	pinMode(PIN_REMOTE, INPUT);
#endif

#if EN_SCREEN
	// Screen
	pinMode(PIN_SCREEN_SCE, OUTPUT);
	pinMode(PIN_SCREEN_RST, OUTPUT);
	pinMode(PIN_SCREEN_DC, OUTPUT);
	pinMode(PIN_SCREEN_SDIN, OUTPUT);
	pinMode(PIN_SCREEN_SCLK, OUTPUT);
#endif
}

// TODO use a Macro Expansion
float Drawall::getStepLength() {
	// PLT_STEPS * 2 because it is the rising edge which drive the motor steps.
	return (PI * PLT_PINION_DIAMETER / 1000)
			/ (PLT_MOTORS_STEPS * 2 * pow(2, PLT_STEP_MODE));
}

// TODO use a Macro Expansion
float Drawall::getDelay(unsigned int speed) {
	return 1000000 * stepLength / float(speed);
}

// TODO use a Macro Expansion
long Drawall::positionToLeftLength(float posX, float posY) {
	return sqrt(
			pow(((float) sheetPosXConf + posX) / stepLength, 2)
					+ pow(
							((float) sheetPosYConf + sheetHeightConf - posY)
									/ stepLength, 2));
}

// TODO use a Macro Expansion
long Drawall::positionToRightLength(float posX, float posY) {
	return sqrt(
			pow(((float) spanConf - (float) sheetPosXConf - posX) / stepLength,
					2)
					+ pow(
							((float) sheetPosYConf + sheetHeightConf - posY)
									/ stepLength, 2));
}

void Drawall::writingPen(bool shouldWrite) {
	if (shouldWrite && !isWriting) {
		// If pen is not writing and should write

		delay(PLT_PRE_SERVO_DELAY);
		for(int pos = movingInsertConf; pos>=drawingInsertConf; pos-=1)
		{                                
			servo.write(pos);
			delay(15);
		}
		delay(PLT_POST_SERVO_DELAY);

#if EN_SERIAL
		Serial.write(DRAW_WRITING);
#endif
		isWriting = true;
	} else if (!shouldWrite && isWriting) {
		// If pen is writing and shouldn't

		delay(PLT_PRE_SERVO_DELAY);
		for(int pos = drawingInsertConf; pos < movingInsertConf; pos += 1)
		{
			servo.write(pos);
			delay(15);
		} 
		delay(PLT_POST_SERVO_DELAY);

#if EN_SERIAL
		Serial.write(DRAW_MOVING);
#endif
		isWriting = false;
	}
}

void Drawall::leftStep(bool shouldPull) {
	if (shouldPull) {
		leftLength--;
#if EN_SERIAL
		Serial.write(DRAW_PULL_LEFT);
#endif
	} else {
		leftLength++;
#if EN_SERIAL
		Serial.write(DRAW_RELEASE_LEFT);
#endif
	}

	digitalWrite(
	PLT_REVERSE_MOTORS ? PIN_RIGHT_MOTOR_STEPS : PIN_LEFT_MOTOR_STEPS,
			leftLength % 2);
}

void Drawall::rightStep(bool shouldPull) {
	if (shouldPull) {
		rightLength--;
#if EN_SERIAL
		Serial.write(DRAW_PULL_RIGHT);
#endif
	} else {
		rightLength++;
#if EN_SERIAL
		Serial.write(DRAW_RELEASE_RIGHT);
#endif
	}

	digitalWrite(
	PLT_REVERSE_MOTORS ? PIN_LEFT_MOTOR_STEPS : PIN_RIGHT_MOTOR_STEPS,
			rightLength % 2);
}

void Drawall::line(float x, float y) {
	writingPen(true);
	int longmax = 5;

	float longX = abs(x - plotterPosX);
	float longY = abs(y - plotterPosY);

	float miniX;
	float miniY;
	int boucle;

	if (longX > longmax || longY > longmax) {
		boucle = ceil((longX > longY ? longX : longY) / longmax);
		miniX = (x - plotterPosX) / boucle;
		miniY = (y - plotterPosY) / boucle;

		for (int i = 0; i < boucle; i++) {
			segment(plotterPosX + miniX, plotterPosY + miniY);
		}
	}
	segment(x, y);
}

void Drawall::move(float x, float y) {
	writingPen(false);
	segment(x, y);
}

void Drawall::processSDLine() {
#define PARAM_MAX_LENGTH 5
#define FUNC_NAME_MAX_LENGTH 2

	byte i, j;
	char functionName[FUNC_NAME_MAX_LENGTH + 1];
	char car;
	char parameter[PARAM_MAX_LENGTH + 1];
	unsigned int parameters[2];

	// Get function name
	car = file.read();

	if (car == ';') {
		while(file.read() != '\n');
		return;
	}

	for (i = 0; car != ' ' && car != '\n' && i < FUNC_NAME_MAX_LENGTH + 1;
			i++) {
		functionName[i] = car;
		car = file.read();
	}
	functionName[i] = '\0';

	// Get parameters
	// The first char has been already read.
	for (i = 0; car != '\n'; i++) {
		car = file.read(); // first parameter char (X, Y or Z)
		for (j = 0; car != ' ' && car != '\n' && j < PARAM_MAX_LENGTH + 1;
				j++) {
			if (j == 0) {
				car = file.read(); // pass first parameter char
			}
			parameter[j] = car;
			car = file.read();
		}
		parameter[j] = '\0';
		parameters[i] = atoi(parameter);
	}

	// Process the GCode function
	if (!strcmp(functionName, "G0")) {
		move(parameters[0], parameters[1]); // move
	} else if (!strcmp(functionName, "G1")) {
		line(parameters[0], parameters[1]); // draw
	} else if (!strcmp(functionName, "G4")) {
		delay(1000 * parameters[0]); // drink some coffee
		Serial.write(DRAW_WAITING);
	} else {
		warning(WARN_UNKNOWN_GCODE_FUNCTION); // raise warning
	}
}

void Drawall::segment(float x, float y) {
	unsigned long leftTargetLength = positionToLeftLength(
			drawingScale * x, drawingScale * y);
	unsigned long rightTargetLength = positionToRightLength(
			drawingScale * x, drawingScale * y);

	// get the number of steps to do
	long nbPasG = leftTargetLength - leftLength;
	long nbPasD = rightTargetLength - rightLength;

	bool pullLeft;
	bool pullRight;

	float delaiG;
	float delaiD;

	unsigned long dernierTempsG;
	unsigned long dernierTempsD;

	// get the direction
	pullLeft = nbPasG < 0;
	pullRight = nbPasD < 0;

	// Since we have the direction, we can leave the sign
	nbPasG = abs(nbPasG);
	nbPasD = abs(nbPasD);

	if (nbPasG > nbPasD) {
		delaiG = delayBetweenSteps;
		delaiD = delayBetweenSteps * ((float) nbPasG / (float) nbPasD);
	} else {
		delaiD = delayBetweenSteps;
		delaiG = delayBetweenSteps * ((float) nbPasD / (float) nbPasG);
	}

	dernierTempsG = micros();
	dernierTempsD = micros();

	// TODO digitalWrite() should be called only when the direction is changing

	if (pullLeft) {
		digitalWrite(
		PLT_REVERSE_MOTORS ? PIN_RIGHT_MOTOR_DIR : PIN_LEFT_MOTOR_DIR,
		PLT_LEFT_DIRECTION);
	} else {
		digitalWrite(
		PLT_REVERSE_MOTORS ? PIN_RIGHT_MOTOR_DIR : PIN_LEFT_MOTOR_DIR,
				!PLT_LEFT_DIRECTION);
	}

	if (pullRight) {
		digitalWrite(
		PLT_REVERSE_MOTORS ? PIN_LEFT_MOTOR_DIR : PIN_RIGHT_MOTOR_DIR,
		PLT_RIGHT_DIRECTION);
	} else {
		digitalWrite(
		PLT_REVERSE_MOTORS ? PIN_LEFT_MOTOR_DIR : PIN_RIGHT_MOTOR_DIR,
				!PLT_RIGHT_DIRECTION);
	}

	while (nbPasG > 0 || nbPasD > 0) {
		// if delay is reached and there are steps to do
		if ((nbPasG > 0) && (micros() - dernierTempsG >= delaiG)) {
			dernierTempsG = micros(); // save current time
			leftStep(pullLeft); // do the step
			nbPasG--; // decrement the remaining steps
		}

		if ((nbPasD > 0) && (micros() - dernierTempsD >= delaiD)) {
			dernierTempsD = micros();
			rightStep(pullRight);
			nbPasD--;
		}
	}

	plotterPosX = x;
	plotterPosY = y;
}

void Drawall::error(SerialData errorNumber) {
#if EN_SERIAL
	Serial.print((byte) errorNumber);
#endif
	// TODO ring buzzer
	delay(1000);
	writingPen(false);
	while (true)
		;
}

void Drawall::warning(SerialData warningNumber) {
#if EN_SERIAL
	Serial.print((byte) warningNumber);
#endif
	// TODO ring buzzer
}

// TODO: do not use CardinalPoint
void Drawall::drawArea() {
	file = SD.open(drawingNameConf);

	if (!file) {
		error(ERR_FILE_NOT_FOUND);
	}

	for(int pos = movingInsertConf; pos < PLT_MAX_SERVO_ANGLE; pos += 1)
	{
		servo.write(pos);
		delay(15);
	}

	segment(0, 0);
	delay(1000);
	segment(65535, 0);
	delay(1000);
	segment(65535, 65535);
	delay(1000);
	segment(0, 65535);
	delay(1000);
	segment(0, 0);

	drawingHeight = sheetHeightConf; // do not subtract the picture height

	file.close();
#if EN_SERIAL
	Serial.print(DRAW_END_DRAWING);
#endif
}

// TODO: do not use CardinalPoint
void Drawall::draw() {
	file = SD.open(drawingNameConf);

	if (!file) {
		error(ERR_FILE_NOT_FOUND);
	}

	// process line until we can read the file
	while (file.available()) {
		processSDLine();
	}

	drawingHeight = sheetHeightConf; // do not subtract the picture height

	file.close();
#if EN_SERIAL
	Serial.print(DRAW_END_DRAWING);
#endif
}

void Drawall::end() {
	for(int pos = movingInsertConf; pos < PLT_MAX_SERVO_ANGLE; pos += 1)
	{
		servo.write(pos);
		delay(15);
	}

	segment(endPosXConf/drawingScale, endPosYConf/drawingScale);
	// TODO ring buzzer

	digitalWrite(PIN_ENABLE_MOTORS, HIGH);

	while (true)
		;
}

void Drawall::message(char* message) {
	Serial.write(DRAW_START_MESSAGE);
	Serial.println(message);
	Serial.write(DRAW_END_MESSAGE);
}

void Drawall::loadParameters() {
#define LINE_MAX_LENGTH 32
#define NB_PARAMETERS 17

	char buffer[LINE_MAX_LENGTH + 1];
	char *key;
	char *value;

	byte i;
	byte line_length;
	int nb_parsed = 0;

	// Check if the file exists
	// TODO Something wrong here with serial communication
	// if(!SD.exists(fileName)) {
	// 	error(ERR_FILE_DONT_EXISTS);
	// }

	File configFile = SD.open(CONFIG_FILE_NAME, FILE_READ);
	if (!configFile) {
		error(ERR_FILE_NOT_READABLE);
	}

	// Until the EOF is not reached
	while (configFile.available() > 0) {
		// Store the full line in buffer
		i = 0;
		while ((buffer[i] = configFile.read()) != '\n') {
			if (i == LINE_MAX_LENGTH) {
				configFile.close();
				error(ERR_TOO_LONG_CONFIG_LINE);
			}
			i++;
		}
		buffer[i] = '\0';

		// Ignore empty or commented lines
		if (buffer[0] == '\0' || buffer[0] == '#') {
			continue;
		}

		for (i = 0; buffer[i] != '='; i++) {
			if (buffer[i] == '\0') {
				error(ERR_WRONG_CONFIG_LINE);
			}
		}
		key = &buffer[0];
		key[i] = '\0';
		value = &buffer[i + 1];

		Serial.print(key);
		Serial.print("=");
		Serial.println(value);

		// TODO use constants for the parameters names

		// * Convert text data into usable data *

		if (!strcmp(key, "drawingName")) {
			strcpy(drawingNameConf, value);
		} else if (!strcmp(key, "drawingWidth")) {
			drawingWidthConf = atoi(value);
			// note : never used yet.
		} else if (!strcmp(key, "drawingPosX")) {
			drawingPosXConf = atoi(value);
		} else if (!strcmp(key, "drawingPosY")) {
			drawingPosYConf = atoi(value);
		} else if (!strcmp(key, "span")) {
			spanConf = atoi(value);
		} else if (!strcmp(key, "initDelay")) {
			initDelayConf = atoi(value);
		} else if (!strcmp(key, "maxSpeed")) {
			maxSpeedConf = atoi(value);
		} else if (!strcmp(key, "sheetWidth")) {
			sheetWidthConf = atoi(value);
		} else if (!strcmp(key, "sheetHeight")) {
			sheetHeightConf = atoi(value);
		} else if (!strcmp(key, "sheetPosX")) {
			sheetPosXConf = atoi(value);
		} else if (!strcmp(key, "sheetPosY")) {
			sheetPosYConf = atoi(value);
		} else if (!strcmp(key, "initPosX")) {
			initPosXConf = atoi(value);
		} else if (!strcmp(key, "initPosY")) {
			initPosYConf = atoi(value);
		} else if (!strcmp(key, "endPosX")) {
			endPosXConf = atoi(value);
		} else if (!strcmp(key, "endPosY")) {
			endPosYConf = atoi(value);
		} else if (!strcmp(key, "drawingInsert")) {
			drawingInsertConf = atoi(value);
		} else if (!strcmp(key, "movingInsert")) {
			movingInsertConf = atoi(value);
		} else {
			warning(ERR_UNKNOWN_CONFIG_KEY);
		}
		nb_parsed++;
	}

	configFile.close();

	if (nb_parsed < NB_PARAMETERS) {
		error(ERR_TOO_FEW_PARAMETERS);
	} else if (nb_parsed > NB_PARAMETERS) {
		error(ERR_TOO_MANY_PARAMETERS);
	}

}
