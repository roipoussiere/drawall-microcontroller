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

/**
 * Configuration file for the plotter, describing plotter physical attributes.
 */

// *** Servo parameters ***

/// Pulse width for minimum angle, in ms
#define PLT_MIN_SERVO_PULSE_WIDTH 500

/// Pulse width for maximum angle, in ms
#define PLT_MAX_SERVO_PULSE_WIDTH 2300

/// Period width, in ms
#define PLT_PERIOD_MS 25000

/// Increment value for the rotation loop, in ms
#define PLT_INCREMENT_MS 15

// *** Motors parameters ***

/// Number of motor steps.
#define PLT_MOTORS_STEPS 200

/// Motor step mode, from 0 (full step) to 5 (1/32 step).
#define PLT_STEP_MODE 5

/// Pinion diameter, in micrometers.
#define PLT_PINION_DIAMETER 12730

/// Direction of the left motor. True to release the belt when the motor rotates clockwise, false if counter clockwise.
#define PLT_LEFT_DIRECTION false

/// Direction of the right motor. True to release the belt when the motor rotates clockwise, false if counter clockwise.
#define PLT_RIGHT_DIRECTION true

/// Virtual reversing state of the motors. True to reverse the motors, false to don't reverse them.
#define PLT_REVERSE_MOTORS true

/// Minimum servo-motor angle (as close as possible to the wall).
#define PLT_MIN_SERVO_ANGLE 55

/// Maximum servo-motor angle (as far away as possible to the wall).
#define PLT_MAX_SERVO_ANGLE 95

/// Delay before the servo moves, in milliseconds.
#define PLT_PRE_SERVO_DELAY 750

/// Delay after the servo moves, in milliseconds.
#define PLT_POST_SERVO_DELAY 750
