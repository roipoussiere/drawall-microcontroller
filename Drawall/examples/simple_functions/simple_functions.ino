/*

Drawall library - simple function tests

This example shows how use the svg function on the Drawall library.
Please see and edit the params.h file to set the drawall parameters, like the pins location.

This work is licensed under the Creative Commons Attribution
- Pas d’Utilisation Commerciale - Partage dans les Mêmes Conditions
3.0 France License. To view a copy of this license, visit
http://creativecommons.org/licenses/by-nc-sa/3.0/fr/.

Created on 5 july 2013 by Nathanael Jourdane
*/

// Include the drawall libraries and his dependencies
#include <SD.h>
#include <Servo.h>
#include <drawall.h>

// Initialise the library with :
// Distance between the motors, sheet position (X and Y) and sheet size (width and height)
Drawall d(1500, 1000, 500, 250, 250);

void setup() {
  d.begin(); // Start the library.
  d.setPosition(0, 0); // Set the initial position
}

void loop() {
  d.move(100, 100); // Move to the position [100,100].
  d.line(150, 100); // Draw a line from this point to the absolute position [150, 100].
  d._line(50, 50); // Draw a line from this point to the relative position [50, 50].
  d.circle(50); // Draw a circle of 50mm radius.
  d.cubicCurve(50, 50, 100, 100); // Draw a bezier cubic curve.
  d.end(); // Finish the drawing.
}
