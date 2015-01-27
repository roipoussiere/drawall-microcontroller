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
 * Configuration file for the plotter, describing pins allocation.
 *
 * Notes about pin allocation:
 * - pins 0 and 1 are used by serial link (respectively RX, TX);
 * - pins 11, 12 and 13 are used by ICSP (SD card communication) : respectively MOSI, MISO, SCK ;
 * - pin 14 = pin A0, 15 = A1, 16 = A2, 17 = A3, 18 = A4 and 19 = A5 (on Arduino UNO board).
 */

/// Pause button interruption pin.
#define PIN_PAUSE A2

/// Limit switch pin.
#define PIN_LIMIT_SWITCH A3

/// Servo-motor control pin.
#define PIN_SERVO A5

/// Left motor direction pin.
#define PIN_LEFT_MOTOR_DIR 5

/// Left motor steps pin.
#define PIN_LEFT_MOTOR_STEPS 6

/// Right motor direction pin.
#define PIN_RIGHT_MOTOR_DIR 7

/// Right motor steps pin.
#define PIN_RIGHT_MOTOR_STEPS 8

/// CS pin on the SD card reader pin.
#define PIN_SD_CS 10

/// Step Mode 0 on the motor driver pin.
#define PIN_STEP_MODE_0 2

/// Step Mode 1 on the motor driver pin.
#define PIN_STEP_MODE_1 3

/// Step Mode 2 on the motor driver pin.
#define PIN_STEP_MODE_2 4

/// Remote control sensor pin.
#define PIN_REMOTE A4

/// Right motor enabling pin.
#define PIN_ENABLE_MOTORS A7

/// Buzzer pin.
#define PIN_BUZZER A6

/// SCE screen pin.
#define PIN_SCREEN_SCE -1

/// RST screen pin.
#define PIN_SCREEN_RST -1

/// DC screen pin.
#define PIN_SCREEN_DC -1

/// SDIN screen pin.
#define PIN_SCREEN_SDIN -1

/// SCLK screen pin.
#define PIN_SCREEN_SCLK -1
