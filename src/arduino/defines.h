/*
 * This file is part of Drawall, a vertical tracer (aka drawbot) - see http://drawall.fr/
 *
 * Copyright (c) 2012-2014 Nathanaël Jourdane
 *
 * Drawall is free software : you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file	config.h
 * \brief   Compilation modes, serial link speed and pin allocation.
 */

// *** Compilation modes ***
// Comment the line if you don't need the feature.

/// Enable buttons support
// #define BUTTONS

/// Enable sending data through serial link
#define SERIAL

/// Enable screen support
// #define SCREEN

/// Enable sending debug data through serial link
// #define DEBUG

// *** Misc ***

/// Serial link speed
#define SERIAL_BAUDS 57600

// *** Pin allocations ***

// Notes:
// pins 0 and 1 are used by serial link (RX, TX)
// Pins 11, 12 and 13 are used by SD card (MOSI, MISO, SCK)
// Pins 14-19 are respectively equivalent to A0-A5 on an Arduino UNO.

// ** Atmega328P (Arduino) **

typedef enum {
	PIN_PAUSE = 2,            	///< Pause button interruption
	PIN_LEFT_CAPTOR = 3,      	///< Left limit switch
	PIN_RIGHT_CAPTOR = 4,     	///< Right limit switch
	PIN_SERVO = 5,            	///< Servo-motor control
	PIN_LEFT_MOTOR_DIR = 6,   	///< Left motor direction
	PIN_LEFT_MOTOR_STEP = 7,  	///< Left motor steps
	PIN_RIGHT_MOTOR_DIR = 8,  	///< Right motor direction
	PIN_RIGHT_MOTOR_STEP = 9, 	///< Right motor steps
	PIN_SD_CS = 10,           	///< CS pin on the SD card reader (Snootlab, Adafruit : 10 - Sparkfun : 8).
	PIN_REMOTE = 14,		  	///< Remote control sensor
	PIN_ENABLE_LEFT_MOTOR = 15, ///< Left motor enabling
	PIN_ENABLE_RIGHT_MOTOR = 16,///< Right motor enabling
	PIN_BUZZER = 17,			///< Buzzer
} Pins;

// ** Atmega644 (DraWall official) ***

// TODO: Make pin allocation template for Atmega644
