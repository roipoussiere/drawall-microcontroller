/*
 * This file is part of DraWall, a vertical plotter (aka drawbot) - see http://drawall.cc
 * Drawall is free software and licenced under GNU GPL v3 : http://www.gnu.org/licenses/
 * Copyright (c) 2012-2015 Nathanaël Jourdane
 * 
 * This program tests the motors,
 * by rotating them in both directions and in all step modes.
 * You need to use a DRV8824 (or equivalent) motor driver.
 * See https://www.pololu.com/product/2131 for wiring.
 * You can open the serial monitor window to get informations during execution.
 *
 * To change pins allocation or other parameters,
 * edit pins.h or hardware.h files in the DraWallParameters folder.
 */

// Include parameter files.
#include "pins.h"
#include "hardware.h"

#define SPEED_RPM 30 // Speed in rotations by seconds
#define NB_TURNS 3 // Number of turns before switching direction

#define SERIAL_BAUDS 57600 // Serial baud rate

int initStepsByTurn = PLT_MOTORS_STEPS * SPEED_RPM / 60 * 2;
long stepsByTurn;
int stepMode = 0;

void setup() {
	Serial.begin(SERIAL_BAUDS);
	pinMode(PIN_LEFT_MOTOR_DIR, OUTPUT);
	pinMode(PIN_LEFT_MOTOR_STEPS, OUTPUT);

	pinMode(PIN_RIGHT_MOTOR_DIR, OUTPUT);
	pinMode(PIN_RIGHT_MOTOR_STEPS, OUTPUT);

	pinMode(PIN_STEP_MODE_0, OUTPUT);
	pinMode(PIN_STEP_MODE_1, OUTPUT);
	pinMode(PIN_STEP_MODE_2, OUTPUT);

	delay(3000);
}

void loop() {
	stepsByTurn = initStepsByTurn * pow(2, stepMode);

	digitalWrite(PIN_STEP_MODE_0, (stepMode & B1) > 0 ? HIGH : LOW);
	digitalWrite(PIN_STEP_MODE_1, (stepMode & B10) > 0 ? HIGH : LOW);
	digitalWrite(PIN_STEP_MODE_2, (stepMode & B100) > 0 ? HIGH : LOW);

	Serial.print("stepMode ");
	Serial.print(stepMode);
	Serial.print(" -> ");
	Serial.print(stepsByTurn);
	Serial.println(" steps/sec.\n");

	Serial.println("Left motor is moving up...");
	digitalWrite(PIN_LEFT_MOTOR_DIR, HIGH);
	move(PIN_LEFT_MOTOR_STEPS);

	Serial.println("Left motor is moving down...");
	digitalWrite(PIN_LEFT_MOTOR_DIR, LOW);
	move(PIN_LEFT_MOTOR_STEPS);

	Serial.println("Right motor is moving up...");
	digitalWrite(PIN_RIGHT_MOTOR_DIR, HIGH);
	move(PIN_RIGHT_MOTOR_STEPS);

	Serial.println("Right motor is moving down...\n");
	digitalWrite(PIN_RIGHT_MOTOR_DIR, LOW);
	move(PIN_RIGHT_MOTOR_STEPS);

	stepMode++;
	if (stepMode == 6)
		stepMode = 0;
}

void move(int stepPin) {
	long delayMicros = long(1000000.0/float(stepsByTurn));
	for(long i=0 ; i < stepsByTurn * NB_TURNS ; i++) {
		digitalWrite(stepPin, i%2);
		delayMicroseconds(delayMicros);
	}
}

