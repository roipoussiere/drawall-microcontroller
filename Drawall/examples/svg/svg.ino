/*
This file is part of Drawall, a project for a robot which draws on walls.
See http://drawall.cc/ and https://github.com/roipoussiere/Drawall/.

Copyright (c) 2012-2013 Nathanaël Jourdane

Drawall is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
Drawall library - svg test

This example shows how use the svg function on the Drawall library.
Please see and edit the params.h file to set the drawall parameters, like the pins location.
Before uploading this sketch, copy the drawbot.svg file into your SD card.
*/

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
  d.area();
  d.drawingArea("drawbot.svg");
  d.svg("drawbot.svg"); // Draw the vectorial picture from the "drawbot.svg" file.
  d.end(); // Finish the drawing.
}
