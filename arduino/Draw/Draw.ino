/*
 * This file is part of DraWall.
 * DraWall is free software: you can redistribute it and/or modify it under the terms of the GNU
 * General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * DraWall is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU
 * General Public License along with DraWall. If not, see <http://www.gnu.org/licenses/>.
 * © 2012–2014 Nathanaël Jourdane
 * © 2014 Victor Adam
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
	// d.setSpeed(5);
	d.drawingArea("lyra.ngc", d.FULL);
	d.draw("lyra.ngc", d.FULL); // Draw the GCode file
	d.end(); // Go to the final position (on the floor by default) and stop the robot.
	while (true)
		;
}
