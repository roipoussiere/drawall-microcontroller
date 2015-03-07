/*
 * This file is part of DraWall, a vertical plotter (aka drawbot) - see http://drawall.cc
 * Drawall is free software and licenced under GNU GPL v3 : http://www.gnu.org/licenses/
 * Copyright (c) 2012-2015 Nathanaël Jourdane
 * 
 * You can open the serial monitor window to get informations during execution.
 *
 * To change pins allocation or other parameters,
 * edit pins.h or hardware.h files in the DraWallParameters folder.
 */

// Include parameter files.
#include "pins.h"
#include "hardware.h"

void setup() {
  Serial.begin(PLT_SERIAL_BAUDS);
  pinMode(PIN_PAUSE, INPUT);
  digitalWrite(PIN_PAUSE, HIGH); // set internal pull-up
}

void loop(){
  Serial.println("Waiting for button press...");
  while (digitalRead(PIN_PAUSE) == HIGH);
  Serial.println("Button is pressed");
  delay(PLT_ANTIBOUNCE_BUTTON_DELAY);
  if(digitalRead(PIN_PAUSE) == LOW) {
    delay(PLT_ANTIBOUNCE_BUTTON_DELAY);
    Serial.println("Long press");
  }
  
  Serial.print("\nReloading");
  delay(500);
  Serial.print('.');
  delay(500);
  Serial.print('.');
  delay(500);
  Serial.println('.');
  delay(500);
}
