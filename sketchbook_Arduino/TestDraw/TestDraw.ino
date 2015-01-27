/*
 * This file is part of DraWall, a vertical tracer (aka drawbot) - see http://drawall.cc
 * Drawall is free software and licenced under GNU GPL v3 : http://www.gnu.org/licenses/
 * Copyright (c) 2012-2015 Nathanaël Jourdane
 * 
 * This program tests the DraWall,
 * - by rotating the 2 motors in both directions,
 * - by rotating the servo-motor in several angles.
 * It doesn't use the Arduino Servo library
 * to prevent timer conflicts and save memory space.
 *
 * To change pins allocation or other parameters,
 * edit pins.h or hardware.h files in the DraWallParameters folder.
 */

// Include parameter files.
#include "pins.h"
#include "hardware.h"

#define SPEED_RPM 30 // Motor speed in rotations by seconds
#define NB_TURNS 3 // Number of turns before switching direction

int currentPulseWidth;

long stepsByTurn = PLT_MOTORS_STEPS * SPEED_RPM / 60 * 2;

void setup() {
	pinMode(PIN_SERVO, OUTPUT);
	currentPulseWidth = PLT_MIN_SERVO_PULSE_WIDTH;

	pinMode(PIN_LEFT_MOTOR_DIR, OUTPUT);
	pinMode(PIN_LEFT_MOTOR_STEPS, OUTPUT);

	pinMode(PIN_RIGHT_MOTOR_DIR, OUTPUT);
	pinMode(PIN_RIGHT_MOTOR_STEPS, OUTPUT);

	delay(2000);
}

void loop() {
	delay(250);
	moveServo(PIN_SERVO, 0);
	delay(250);

	digitalWrite(PIN_LEFT_MOTOR_DIR, HIGH);
	moveMotor(PIN_LEFT_MOTOR_STEPS);

	digitalWrite(PIN_LEFT_MOTOR_DIR, LOW);
	moveMotor(PIN_LEFT_MOTOR_STEPS);

	delay(250);
	moveServo(PIN_SERVO, 100);
	delay(250);

	digitalWrite(PIN_RIGHT_MOTOR_DIR, HIGH);
	moveMotor(PIN_RIGHT_MOTOR_STEPS);

	digitalWrite(PIN_RIGHT_MOTOR_DIR, LOW);
	moveMotor(PIN_RIGHT_MOTOR_STEPS);
}

void moveMotor(int stepPin) {
	long delayMicros = long(1000000.0/float(stepsByTurn));
	for(long i=0 ; i < stepsByTurn * NB_TURNS ; i++) {
		digitalWrite(stepPin, i%2);
		delayMicroseconds(delayMicros);
	}
}

void moveServo(int servoPin, int position) {
	int pulseWidth = position *
		(PLT_MAX_SERVO_PULSE_WIDTH-PLT_MIN_SERVO_PULSE_WIDTH)/100 +
		PLT_MIN_SERVO_PULSE_WIDTH;

	pulseWidth = pulseWidth < PLT_MIN_SERVO_PULSE_WIDTH ? PLT_MIN_SERVO_PULSE_WIDTH : pulseWidth;
	pulseWidth = pulseWidth > PLT_MAX_SERVO_PULSE_WIDTH ? PLT_MAX_SERVO_PULSE_WIDTH : pulseWidth;

	if (currentPulseWidth > pulseWidth) {
		for (; currentPulseWidth >= pulseWidth; currentPulseWidth-=PLT_INCREMENT_MS) {
			digitalWrite(servoPin, HIGH);
			delayMicroseconds(currentPulseWidth);
			digitalWrite(servoPin, LOW);
			delayMicroseconds(PLT_PERIOD_MS - currentPulseWidth);
		}
	} else {
		for (; currentPulseWidth <= pulseWidth; currentPulseWidth+=PLT_INCREMENT_MS) {
			digitalWrite(servoPin, HIGH);
			delayMicroseconds(currentPulseWidth);
			digitalWrite(servoPin, LOW);
			delayMicroseconds(PLT_PERIOD_MS - currentPulseWidth);
		}    
	}
}
