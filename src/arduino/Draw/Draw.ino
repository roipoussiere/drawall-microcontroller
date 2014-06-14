/*
 * This file is part of Drawall, a vertical tracer (aka drawbot) - see http://drawall.fr/
 * Drawall is free software and licenced under GNU GPL v3 : http://www.gnu.org/licenses/
 * Copyright (c) 2012-2014 Nathanaël Jourdane
 */

/*
Drawall library - drawing test

This example shows how to use the svg function on the Drawall library.
Please see and edit the params.h file to set the drawall parameters, like the pins location.
Before uploading this sketch, copy the drawbot.svg file into your SD card.
*/

// Include drawall library and his dependencies.
#include <SD.h>
#include <Servo.h>
#include <drawall.h>

// Initialise the library.
Drawall d;

void setup() {
  d.begin("drawall.dcf"); // Start the library and loads the configuration file.
}

void loop() {
  d.drawingArea("gnu.lgc", d.FULL); // Draw the limits of the drawing area
  d.draw("gnu.lgc", d.FULL); // Draw the GCode file
  d.end(); // Go to the final position (on the floor by default) and stop the robot.
  while(true);
}
