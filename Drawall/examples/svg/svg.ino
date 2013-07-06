/*

Drawall library - svg test

This example shows how use the svg function on the Drawall library.
Please see and edit the params.h file to set the drawall parameters, like the pins location.

This work is licensed under the Creative Commons Attribution
- Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions
3.0 France License. To view a copy of this license, visit
http://creativecommons.org/licenses/by-nc-sa/3.0/fr/.

Created on 5 july 2013 by Nathanael Jourdane
*/

// Before uploading this sketch, copy the drawbot.svg file into your SD card.

// Include the drawall libraries and his dependencies
#include <SD.h>
#include <Servo.h>
#include <drawall.h>

// Initialise the library with :
// Distance between the motors, sheet size (width and height) and sheet position (X and Y)
Drawall d(1500, 1000, 500, 250, 250);

void setup() {
  d.begin(); // Start the library.
  d.setSpeed(500); // Set speed.
}

void loop() {
  d.svg("drawbot.svg"); // Draw the vectorial picture.
  d.end(); // Finish the drawing.
}
