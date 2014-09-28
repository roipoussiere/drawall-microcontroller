/*
 * This file is part of Drawall, a vertical tracer (aka drawbot) - see http://drawall.fr/
 * Drawall is free software and licenced under GNU GPL v3 : http://www.gnu.org/licenses/
 * Copyright (c) 2012-2014 Nathanaël Jourdane
 */

#include <Servo.h>

#define PIN_SERVO 5

#define MIN_SERVO 10
#define MAX_SERVO 80

Servo s;

void setup()
{
  s.attach(PIN_SERVO);
  Serial.begin(57600);
}

void loop()
{
  Serial.println("Mooving...");
  s.write(MIN_SERVO);
  delay(5000);

  Serial.println("Writing...");
  s.write(MAX_SERVO);
  delay(5000);
}
