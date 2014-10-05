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
 * Header library file.
 */

#ifndef _H_DRAWALL
#define _H_DRAWALL

#include "plotter.h"
#include <math.h>
#include <SD.h>
#include <Servo.h>
#include <Arduino.h>

#define START_WITH_DELAY 0
#define START_WITH_BUTTON 1
#define START_WITH_SERIAL 2

/**
 * Main library class.
 */
class Drawall {

public:

	/**
	 * CardinalPoint on the drawing area.
	 * The positions, corresponding to the cardinal points, plus the center.	 */
	// TODO: delete this, because configuration file will give positions in millimeters.
	typedef enum {
		LOWER_LEFT, LOWER_CENTER, LOWER_RIGHT,

		LEFT_CENTER, CENTER, RIGHT_CENTER,

		UPPER_LEFT, UPPER_CENTER, UPPER_RIGHT
	} CardinalPoint;

	/**
	 * Image width modes.
	 */
	// TODO delete this because the file dimensions are always the same (65535)
	typedef enum {
		ORIGINAL, ///< Matching to the drawing width.
		FULL,     ///< Matching to the sheet width.
	} DrawingSize;

	/**
	 * Initialize the library.
	 * required to use the library. Do the plotter initialization procedures.
	 */
	void start();

	/**
	 * Initialize all pins.
	 */
	void pinInitialization();

	/**
	 * End the drawing.
	 * Used in the end of the drawing:
	 * - position the plotter on the end position (at the bottom of the sheet by default);
	 * - disable the motors;
	 * - pause the program.
	 */
	void end();

	/********************
	 * Getters & setters *
	 ********************/

	/**
	 * Set the plotter speed (in mm/s).
	 * A next feature will support dynamic speed, then this parameter will be a 'base' speed.
	 * \param speed The plotter speed.
	 * \bug Speed growing if steep numbers growing.
	 */
	float getDelay(unsigned int speed);

	/**
	 * Set the step mode, on the motors drivers.
	 */
	void setStepMode();

	/**********************
	 * Fonctions de dessin *
	 **********************/

	/**
	 * Move the plotter to the absloute position [\a x ; \a y].
	 * \param x The horizontal absolute position of the destination point.
	 * \param y The vertical absolute position of the destination point.
	 */
	void move(float posX, float posY);

	/**
	 * Draw a straight line, from the actual position to the absolute position  [\a x; \a y].
	 * \param x The horizontal absolute position of the destination point.
	 * \param y The vertical absolute position of the destination point.
	 * \bug Fait des escaliers dans certains cas (?).
	 */
	void line(float x, float y);

	/**
	 * Draw a rectangle matching with the limits of the drawing.
	 */
	void drawingArea(DrawingSize size = FULL, CardinalPoint position = CENTER);

	/**
	 * Draw a drawing as descibed in the \a fileName file stored int the SD card.
	 * \param fileName Le nom du fichier gcode à dessiner.
	 * TODO Check the M02 presence (end of drawing) before the end of drawing.
	 */
	void draw(DrawingSize size = FULL, CardinalPoint position = CENTER);

private:

	/// Configuration file name
	const char *CONFIG_FILE_NAME = "config";

	/**
	 * The codes to send to the computer trought the serial link.
	 * The errors and warnings which should occurs during the program execution.
	 * The errors starts to 0 and the Warnings starts to 100.
	 * The warnings doesn't interfere in the drawing and the errors are critical anomalies which prevents the drawing.
	 */
	typedef enum {

		// Drawing messages

		DRAW_START_INSTRUCTIONS, ///< 0. Beginning of the initialization data;
		DRAW_END_INSTRUCTIONS,   ///< 1. End of the initialization data;
		DRAW_RELEASE_LEFT,       ///< 2. Release the left belt for one step;
		DRAW_PULL_LEFT,          ///< 3. Pull the left belt for one step;
		DRAW_RELEASE_RIGHT,      ///< 4. Release the left belt for one step;
		DRAW_PULL_RIGHT,         ///< 5. Pull the left belt for one step;
		DRAW_WRITING,            ///< 6. The plotter is drawing;
		DRAW_MOVING,             ///< 7. The plotter is moving (dot drawing);
		DRAW_WAITING,            ///< 8. The plotter waiting for something;
		DRAW_ENABLE_MOTORS,      ///< 9. Enable the motors and the servo;
		DRAW_DISABLE_MOTORS,     ///< 10. Disable the motors and the servo;
		DRAW_CHANGE_TOOL,        ///< 11. Pause the program to change the tool;
		DRAW_END_DRAWING,        ///< 12. The drawing is finished;
		DRAW_START_MESSAGE,      ///< 13. Beginning of the message to display on the screen (if any) or/and on the computer (if any);
		DRAW_END_MESSAGE,        ///< 14. End of the message to display on the screen (if any) or/and on the computer (if any);

		// Errors

		ERR_CARD_NOT_FOUND,      ///< 15. The SD card is not found or not readable;
		ERR_FILE_NOT_FOUND,      ///< 16. The file not exists;
		ERR_FILE_NOT_READABLE,   ///< 17. Error while opening the file;
		ERR_TOO_FEW_PARAMETERS,  ///< 18. One or several parameters have not been read;
		ERR_TOO_MANY_PARAMETERS, ///< 19. Too many parameters have been read;
		ERR_WRONG_CONFIG_LINE,   ///< 20. Incorrectly formatted line in the configuration file;
		ERR_TOO_LONG_CONFIG_LINE,///< 21. Too long line in the configuration file;
		ERR_UNKNOWN_CONFIG_KEY,  ///< 22. Unknown key in the configuration file;

		// Warnings

		WARN_UNKNOWN_GCODE_FUNCTION, ///< 23. Unknown GCode function in the drawing file;
		WARN_UNKNOWN_GCODE_PARAMETER,///< 24. Unknown GCode parameter;
	} SerialData;

	/*************
	 * Attributes *
	 *************/

	/// Instance of the servo, used to drive it with the \a Servo library.
	Servo servo;

	/// The GCode file of the drawing.
	// TODO: use in local variable
	File file;

	/// Left belt length, in steps.
	unsigned long leftLength;

	/// Right belt length, in steps.
	unsigned long rightLength;

	/// Horizontal offset. Can be used to calibrate the drawing in a accurate position.
	unsigned int offsetX;

	/// Vertical offset. Can be used to calibrate the drawing in a accurate position.
	unsigned int offsetY;

	/// Drawing scale. Can be used to calibrate the drawing in a accurate scale.
	float drawingScale;

	/// Width of the running drawing.
	int drawingWidth;

	/// Hight of the running drawing.
	int drawingHeight;

	/// Step length. This is the distance traveled by the belt in one step, in mm.
	/// \TODO Use micro or more accurate unit to use int instead of floats ?
	float stepLength;

	/// Initial delay between each motor step (in ms). It concerns the motor which have the longger distance to travel.
	/// The delay concerning the other motor is calculaed in such a way as to the 2 motors are synchronised, that is, they finishes the line on the same time.
	float delayBetweenSteps;

	/// The robot is currently writing (\a true) or not (\a false).
	bool isWriting;

	/************
	 * Positions *
	 ************/

	/// Current horizontal coordinate of the plotter on the sheet, in millimeters.
	// TODO use integers
	float plotterPosX;

	/// Current vertical coordinate of the plotter on the sheet, in millimeters.
	// TODO use integers
	float plotterPosY;

	//-------------------------------------------------------------------------

	/*************
	 * Parameters *
	 *************/

	// *** 1. Drawing ***
	/**
	 * Drawing file name
	 * Name of the file stored on the memory card which will drawn.
	 * Default value: "drawing"
	 * Range: [1 char, 20 chars]
	 * TODO [v1+] The plotter should draw more than one file.
	 */
	char drawingNameConf[15];

	/**
	 * Drawing width
	 * Width of this drawing, as will be on the wall.
	 * Unit: millimeters
	 * Default value: Max
	 * Range: [10 mm, sheetWidth]
	 */
	unsigned int drawingWidthConf;

	/**
	 * Drawing horizontal position
	 * The drawing’s bottom left corner, according to the sheet’s bottom left corner.
	 * Unit: millimeters
	 * Default value: Center
	 * Range: [0 mm, sheetWidth - drawingWidth]
	 */
	unsigned int drawingPosXConf;

	/**
	 * Drawing vertical position
	 * The drawing’s bottom left corner, according to the sheet’s bottom left corner.
	 * Unit: millimeters
	 * Default value: Center
	 * Range: [0 mm, sheetHeight - drawingHeight]
	 */
	unsigned int drawingPosYConf;

	// *** 2. Installation ***

	// * 2.1 General *

	/**
	 * Span
	 * Distance between the belt extremities.
	 * Unit: millimeters
	 * Default value: 3000 mm
	 * Range: [500 mm, 30,000 mm]
	 */
	unsigned int spanConf;

	/**
	 * Start-up mode
	 * Specify which event starts the drawing.
	 * Unit: -
	 * Default value: Delay
	 * Range: [0 = Delay, 1 = pushButton, 2 = serial]
	 * TODO unused yet
	 */
	byte startupEventConf;

	/**
	 * Initial delay
	 * Delay before the drawing starts after started the program.
	 * Unit: milliseconds
	 * Default value: 2000 ms
	 * Range: [0 ms, 10,000 ms]
	 */
	unsigned int initDelayConf;

	/**
	 * Maximum speed
	 * Maximum drawing speed. The plotter will slow down in curves (release 1.0+).
	 * Unit: millimeters by second
	 * Default value: 20 mm/s
	 * Range: [10 mm/s, 1000 mm/s]
	 * TODO [v1.0+] Dynamically manage the speed. The speed parameter will be a percentage of the maximum value.
	 */
	unsigned int maxSpeedConf;

	// * 2.2 Sheet position and dimensions *

	/**
	 * Sheet width
	 * Unit: millimeters
	 * Default value: 650 mm x 500 mm
	 * Range: [100 mm, span - sheetPosX] x [100 mm, 30,000 mm]
	 */
	unsigned int sheetWidthConf;

	/**
	 * Sheet height
	 * Unit: millimeters
	 * Default value: 650 mm x 500 mm
	 * Range: [100 mm, span - sheetPosX] x [100 mm, 30,000 mm]
	 */
	unsigned int sheetHeightConf;

	/*
	 * Sheet horizontal position
	 * Position of the sheet’s top left corner, according to the left belt extremity.
	 * Unit: millimeters
	 * Default value: Center
	 * Range: [0, span - sheetWidth]
	 */
	unsigned int sheetPosXConf;

	/*
	 * Sheet vertical position
	 * Position of the sheet’s top left corner, according to the left belt extremity.
	 * Unit: millimeters
	 * Default value: 1,000 mm
	 * Range: [0, 30,000]
	 */
	unsigned int sheetPosYConf;

	// * 2.3 Pen depressing *

	/**
	 * Drawing insertion level
	 * The pen insertion level when the plotter is drawing.
	 * Unit: 0.1 percents
	 * Default value: 0%
	 * Range: [0%, 100%]
	 * TODO unused yet
	 */
	unsigned int drawingInsertConf;

	/**
	 * Moving insertion level
	 * The pen insertion level when the plotter is moving.
	 * Unit: 0.1 percents
	 * Default value: 0%
	 * Range: [0%, 100%]
	 * TODO unused yet
	 */
	unsigned int movingInsertConf;

	// *** 3. Advanced parameters ***

	// * 3.1 Plotter positions *

	/**
	 * Initial plotter horizontal position
	 * Initial position of the pen, according to the lower-left sheet corner.
	 * If values are 65535, that means the initial position is set automatically.
	 * Unit: millimeters
	 * Default value: Center
	 * Range: [0 mm, sheetWidth]
	 * TODO v1.0: set up the initial position automatically.
	 */
	unsigned int initPosXConf;

	/**
	 * Initial plotter horizontal position
	 * Initial position of the pen, according to the sheet’s bottom left corner.
	 * If values are 65535, that means the initial position is set automatically.
	 * Unit: millimeters
	 * Default value: Bottom
	 * Range: [0 mm, sheetHeight]
	 * TODO v1.0: set up the initial position automatically.
	 */
	unsigned int initPosYConf;

	/**
	 * Final plotter horizontal position
	 * Final position of the pen, according to the sheet’s bottom left corner.
	 * Unit: millimeters
	 * Default value: Center
	 * Range: [0 mm, sheetWidth]
	 */
	unsigned int endPosXConf;

	/**
	 * Final plotter vertical position
	 * Final position of the pen, according to the sheet’s bottom left corner.
	 * Unit: millimeters
	 * Default value: Bottom
	 * Range: [0 mm, sheetHeight]
	 */
	unsigned int endPosYConf;

	// * 3.2 Adjusting *

	/**
	 * Horizontal scale
	 * Scale applied to the drawing, used to calibrate it according to his width and height. Should be used rarely.
	 * Unit: percents
	 * Default value: 100%
	 * Range: [70.0%, 150.0%]
	 */
	unsigned int scaleXConf;
	// TODO /!\ I just changed this to unsigned integer

	/**
	 * Vertical scale
	 * Scale applied to the drawing, used to calibrate it according to his width and height. Should be used rarely.
	 * Unit: percents
	 * Default value: 100%
	 * Range: [70.0%, 150.0%]
	 */
	unsigned int scaleYConf;
	// TODO /!\ I just changed this to unsigned integer

	/**
	 * Horizontal offset
	 * Offset applied to the drawing, used to shift it according to his width and height. Should be used rarely.
	 * Unit: millimeters
	 * Default value: 0 mm
	 * Range: [-500mm, +500mm]
	 */
	int offsetXConf;

	/**
	 * Vertical offset
	 * Offset applied to the drawing, used to shift it according to his width and height. Should be used rarely.
	 * Unit: millimeters
	 * Default value: 0 mm
	 * Range: [-500mm, +500mm]
	 */
	int offsetYConf;

	//-------------------------------------------------------------------------

	/***********
	 * Methods *
	 ***********/

	/**
	 * Read a variable in the GCode file formated like this VARNAME = VALUE.
	 */
	// int processVar();
	/**
	 * Initialise the X et Y offsets according to the configuration file and the desired position of the drawing.
	 */
	void initOffset(CardinalPoint position);

	/**
	 * Initialise the scale according to the desired drawing width.
	 */
	void initScale(DrawingSize size);

	/**
	 * Initialize the ratio of number of steps to distance.
	 * Calculated with the pinion diameter and the number of steps.
	 * - xx(mm)*ratio --> xx(pas)
	 * - xx(pas)/ratio --> xx(mm)
	 * \TODO Invert the ratio, then the ratio will be an integer value, so that prevents working with floats.
	 */
	float getStepLength();

	/**
	 * Set the drawing scale, to adapt the drawing to his \a width and \a height.
	 * Drawing dimentions are got from the GCode file header.
	 * \TODO Return the new scale instead of modify it directly.
	 */
	void setDrawingScale(int width, int height);

	/**
	 * Function called when an error appends.
	 * \TODO Optimize the errors and warning management, maybe use only one function for errors and warnings, and find something better for the parameter.
	 * - Keep away the pen from the sheet;
	 * - Stop the program;
	 * \TODO Displays the error on the screen, if any;
	 * - Send the error code \a to the computer trought the serial link, if any.
	 * All the errors should appends before the begining of the drawing.
	 * \param errorNumber The error number (See Drawall::Error);
	 * \param msg An optional error message which adds an information about the error (empty string if there is no message to send);
	 */
	void error(SerialData errorNumber);

	/**
	 * Function called when a warning appends.
	 * \TODO Displays the error on the screen, if any;
	 * - Send the error code \a to the computer trought the serial link, if any.
	 * \param warningNumber The warning number (See Drawall::Error);
	 * \param msg An optional warning message which adds an information about the about (empty string if there is no message to send);
	 */
	void warning(SerialData warningNumber);

	/*******************
	 * Hardware driving *
	 *******************/

	/**
	 * Rotate the left motor for one step.
	 * \param pull direction of rotation: \a true to pull the belt, \a false to release the belt.
	 */
	void leftStep(bool shouldPull);

	/**
	 * Rotate the right motor for one step.
	 * \param pull direction of rotation: \a true to pull the belt, \a false to release the belt.
	 */
	void rightStep(bool shouldPull);

	/**
	 * Enable or disable the motors.
	 * Keep away the pen from the sheet to prevent an ugly drawing during any plotter fall.
	 * \param power \a true to enable the motors, \a false to disable the motors.
	 * \TODO Separate the left motor disable and the right motor disable.
	 */
	void power(bool shouldPower);

	/**************
	 * Convertions *
	 **************/

	/**
	 * Calculate the left belt length for the position [\a x ; \a y].
	 * \param x The horizontal absolute coordinate of the point.
	 * \param y The vertical absolute coordinate of the point.
	 * \return The left belt length for the given position (in steps number).
	 */
	long positionToLeftLength(float x, float y);

	/**
	 * Calculate the right belt length for the position [\a x ; \a y].
	 * \param x The horizontal absolute coordinate of the point.
	 * \param y The vertical absolute coordinate of the point.
	 * \return The right belt length for the given position (in steps number).
	 */
	long positionToRightLength(float x, float y);

	/******************
	 * SD card reading *
	 ******************/

	/**
	 * Initialise the SD card.
	 * Could throw error CARD_NOT_FOUND (see Drawall::Error)
	 * \param fileName The file name to read.
	 * \TODO : If an error append and the file name is longer than 8 characters, suggest to format the card in fat16.
	 */
	void sdInit(char *fileName);

	/**
	 * Interpret the current GCode function.
	 * The cursor need to be just before a GCode function. Ignore white spaces before the function name.
	 */
	void processSDLine();

	/**
	 * Draw a straight line from the current point to the point [\a x ; \a y].
	 * \param shouldWrite: the pen state, that is, \true to write and \false to move.
	 */
	void segment(float x, float y, bool shouldWrite);

	/**
	 * Come close or keep away the pen from the sheet.
	 * \param shouldWrite \a true to come close the pen to the sheet (writing), \a false to keep away (moving).
	 */
	void writingPen(bool shouldWrite);

	/**
	 * Send a message to the GUI
	 */
	void message(char* message);

	/**
	 * Load the parameters from the configuration file on the SD card.
	 * \param fileName The name of the configuration file to load.
	 */
	void loadParameters();

};

#endif
