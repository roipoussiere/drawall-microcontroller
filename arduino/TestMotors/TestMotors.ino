/*
 * This file is part of Drawall, a vertical tracer (aka drawbot) - see http://drawall.fr/
 * Drawall is free software and licenced under GNU GPL v3 : http://www.gnu.org/licenses/
 * Copyright (c) 2012-2014 Nathanaël Jourdane
 */

// Left motor pins
#define PIN_LEFT_DIR 6
#define PIN_LEFT_STEPS 7

// Right motor pins
#define PIN_RIGHT_DIR 8
#define PIN_RIGHT_STEPS 9

// step mode pins
#define PIN_MODE_0 2
#define PIN_MODE_1 3
#define PIN_MODE_2 4

// Motor steps
#define MOTORS_STEPS 200

// Speed in rotations by seconds
#define SPEED_RPS 1

// Serial baud rate
#define SERIAL_BAUDS 9600

int initStepsBySec = MOTORS_STEPS * SPEED_RPS;
long stepsBySec;
int stepMode = 0;

void setup() {
	Serial.begin(SERIAL_BAUDS);
	pinMode(PIN_LEFT_DIR, OUTPUT);
	pinMode(PIN_LEFT_STEPS, OUTPUT);

	pinMode(PIN_RIGHT_DIR, OUTPUT);
	pinMode(PIN_RIGHT_STEPS, OUTPUT);

	pinMode(PIN_MODE_0, OUTPUT);
	pinMode(PIN_MODE_1, OUTPUT);
	pinMode(PIN_MODE_2, OUTPUT);

	delay(3000);
}

void loop() {
	stepsBySec = initStepsBySec * pow(2, stepMode);

	digitalWrite(PIN_MODE_0, (stepMode & B1) > 0 ? HIGH : LOW);
	digitalWrite(PIN_MODE_1, (stepMode & B10) > 0 ? HIGH : LOW);
	digitalWrite(PIN_MODE_2, (stepMode & B100) > 0 ? HIGH : LOW);

	Serial.print("stepMode ");
	Serial.print(stepMode);
	Serial.print(" -> ");
	Serial.print(stepsBySec);
	Serial.println(" steps/sec.\n");

	Serial.println("Left motor is moving up...");
	digitalWrite(PIN_LEFT_DIR, HIGH);
	move(PIN_LEFT_STEPS);

	Serial.println("Left motor is moving down...");
	digitalWrite(PIN_LEFT_DIR, LOW);
	move(PIN_LEFT_STEPS);

	Serial.println("Right motor is moving up...");
	digitalWrite(PIN_RIGHT_DIR, HIGH);
	move(PIN_RIGHT_STEPS);

	Serial.println("Right motor is moving down...\n");
	digitalWrite(PIN_RIGHT_DIR, LOW);
	move(PIN_RIGHT_STEPS);

	stepMode++;
	if (stepMode == 6)
		stepMode = 0;
}

void move(int stepPin) {
	long delayMicros = long(1000000.0/float(stepsBySec));
	for(long i=0 ; i < stepsBySec ; i++) {
		digitalWrite(stepPin, i%2);
		delayMicroseconds(delayMicros);
	}
}

