/*
 * This file is part of DraWall.
 * DraWall is free software: you can redistribute it and/or modify it under the terms of the GNU
 * General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * DraWall is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU
 * General Public License along with DraWall. If not, see <http://www.gnu.org/licenses/>.
 * © 2012–2015 Nathanaël Jourdane
 */

/*
PLEASE UPLOAD THIS PROGRAM ONCE ON THE PLOTTER MICROCONTROLLER.
ALL THE OPTIONS FOR THE DRAWING MUST BE SET IN THE CONFIGURATION FILE, STORED IN THE SD CARD.
YOU SHOULD NOT MODIFY THIS FILE, EXCEPT IF YOU WANT TO HACK THE DRAWALL.
*/

#include <SD.h>
#include <Servo.h>
#include <drawall.h>

Drawall d;

void setup() {
}

void loop() {
  d.start();
}
