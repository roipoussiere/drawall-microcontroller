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

int initStepsByTurn = PLT_MOTORS_STEPS * SPEED_RPM / 60 * 2;
long stepsByTurn;

void setup() {
	Serial.begin(PLT_SERIAL_BAUDS);
	pinMode(PIN_LEFT_MOTOR_DIR, OUTPUT);
	pinMode(PIN_LEFT_MOTOR_STEPS, OUTPUT);

	pinMode(PIN_RIGHT_MOTOR_DIR, OUTPUT);
	pinMode(PIN_RIGHT_MOTOR_STEPS, OUTPUT);

	delay(3000);
}

void loop() {
	stepsByTurn = initStepsByTurn * 32;

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
}

void move(int stepPin) {
	long delayMicros = long(1000000.0/float(stepsByTurn));
	for(long i=0 ; i < stepsByTurn * NB_TURNS ; i++) {
		digitalWrite(stepPin, i%2);
		delayMicroseconds(delayMicros);
	}
}

