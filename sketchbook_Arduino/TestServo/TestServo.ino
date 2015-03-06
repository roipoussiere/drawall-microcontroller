/*
 * This file is part of DraWall, a vertical plotter (aka drawbot) - see http://drawall.cc
 * Drawall is free software and licenced under GNU GPL v3 : http://www.gnu.org/licenses/
 * Copyright (c) 2012-2015 Nathanaël Jourdane
 *
 * This program tests the servo-motor, by rotating it in several angles.
 * It doesn't use the Arduino Servo library
 * to prevent timer conflicts and save memory space.
 *
 * To change pins allocation or other parameters,
 * edit pins.h or hardware.h files in the DraWallParameters folder.
 */

// Include parameter files.
#include "pins.h"
#include "hardware.h"

int currentPulseWidth;

void setup() {
  pinMode(PIN_SERVO, OUTPUT);
  currentPulseWidth = PLT_MIN_SERVO_PULSE_WIDTH;
  delay(2000);
}

void loop()
{
  delay(500);
  moveServo(PIN_SERVO, 0);
  delay(500);
  moveServo(PIN_SERVO, 50);
  delay(500);
  moveServo(PIN_SERVO, 100);
}

void moveServo(int servoPin, int position) {
  int pulseWidth = position*(PLT_MAX_SERVO_PULSE_WIDTH-PLT_MIN_SERVO_PULSE_WIDTH)/100 + PLT_MIN_SERVO_PULSE_WIDTH;

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
