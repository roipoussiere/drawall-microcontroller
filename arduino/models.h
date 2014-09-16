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

/**
 * \brief Configuration file for each plotter model, describing enabled components and pin allocation.
 * \details
 * Note about enabled components : comment the #define line if you don't need the feature.
 * Available modes:
 * - BUTTONS: Enable buttons support;
 * - SCREEN: Enable screen support;
 * - SERIAL: Enable sending data through serial link to use the simulator;
 * - SERIAL_BAUDS: Serial link speed (baud rate).
 * Notes about pin allocation:
 * - pins 0 and 1 are used by serial link (RX, TX);
 * - pins 11, 12 and 13 are used by SD card (MOSI, MISO, SCK);
 * - pins 14-19 are respectively equivalent to A0-A5 on an Arduino UNO.
 */

#if BOARD == ATMEGA328

// #define BUTTONS
// #define SCREEN

typedef enum {
	PIN_PAUSE = 2,            	///< Pause button interruption
	PIN_LEFT_CAPTOR = 3,      	///< Left limit switch
	PIN_RIGHT_CAPTOR = 4,     	///< Right limit switch

	PIN_SERVO = 5,            	///< Servo-motor control

	PIN_LEFT_MOTOR_DIR = 6,   	///< Left motor direction
	PIN_LEFT_MOTOR_STEP = 7,  	///< Left motor steps
	PIN_RIGHT_MOTOR_DIR = 8,  	///< Right motor direction
	PIN_RIGHT_MOTOR_STEP = 9, 	///< Right motor steps

	PIN_SD_CS = 10, ///< CS pin on the SD card reader (Snootlab, Adafruit : 10 - Sparkfun : 8).

	PIN_STEP_MODE_0 = 14,            ///< Step Mode 0 on the motor driver
	PIN_STEP_MODE_1 = 15,            ///< Step Mode 1 on the motor driver
	PIN_STEP_MODE_2 = 16,            ///< Step Mode 2 on the motor driver

	PIN_REMOTE = 17,		  	///< Remote control sensor
	PIN_ENABLE_MOTORS = 18,     ///< Right motor enabling
	PIN_BUZZER = 19,			///< Buzzer
} Pins;

#elif BOARD == ATMEGA644

// TODO: Make good pin allocation template for Atmega644

#define BUTTONS
#define SCREEN

typedef enum {
	PIN_PAUSE = 2,            	///< Pause button interruption
	PIN_LEFT_CAPTOR = 3,///< Left limit switch
	PIN_RIGHT_CAPTOR = 4,///< Right limit switch

	PIN_SERVO = 5,///< Servo-motor control

	PIN_LEFT_MOTOR_DIR = 6,///< Left motor direction
	PIN_LEFT_MOTOR_STEP = 7,///< Left motor steps
	PIN_RIGHT_MOTOR_DIR = 8,///< Right motor direction
	PIN_RIGHT_MOTOR_STEP = 9,///< Right motor steps

	PIN_SD_CS = 10,///< Chip Select pin on the SD card reader (Snootlab, Adafruit : 10 - Sparkfun : 8).

	PIN_STEP_MODE_0 = 14,///< Step Mode 0 on the motor driver
	PIN_STEP_MODE_1 = 15,///< Step Mode 1 on the motor driver
	PIN_STEP_MODE_2 = 16,///< Step Mode 2 on the motor driver

	PIN_REMOTE = 17,///< Remote control sensor
	PIN_ENABLE_MOTORS = 18,///< Right motor enabling
	PIN_BUZZER = 19,///< Buzzer
}Pins;

#elif BOARD == PERSONALISED

// Write here your personalised configuration

#endif
