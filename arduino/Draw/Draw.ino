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
PLEASE UPLOAD THIS PROGRAM ONCE ON THE PLOTTER MICROCONTROLLER.
ALL THE OPTIONS FOR THE DRAWING MUST BE SET IN THE CONFIGURATION FILE, STORED IN THE SD CARD.
YOU SHOULD NOT MODIFY THIS FILE, EXCEPT IF YOU WANT TO HACK THE DRAWALL.
*/

// Include drawall library and his dependencies.
#include <SD.h>
#include <Servo.h>
#include <drawall.h>

// Declare the library.
Drawall d;

void setup() {
	d.begin("config"); // Start the library and loads the configuration file.
}

void loop() {
	// d.setSpeed(20);
	// d.drawingArea(d.FULL, d.CENTER);
	d.draw(d.FULL, d.CENTER); // Draw the GCode file
	d.end(); // Go to the final position (on the floor by default) and stop the robot.
}
