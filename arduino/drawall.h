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

#define BOARD ATMEGA328

#ifndef _H_DRAWALL
#define _H_DRAWALL

#include "models.h"
#include <math.h>
#include <SD.h>
#include <Servo.h>
#include <Arduino.h>

/// Serial link speed
#define SERIAL_BAUDS 57600

/// Enable sending data through serial link
#define SERIAL

/// Enable sending debug data through serial link
// #define DEBUG

/**
 * Main library class.
 */
class Drawall {

	public:

	/**
	 * CardinalPoint on the drawing area.
	 * The positions, corresponding to the cardinal points, plus the center.	 */
	typedef enum {
		LOWER_LEFT,
		LOWER_CENTER,
		LOWER_RIGHT,

		LEFT_CENTER,
		CENTER,
		RIGHT_CENTER,

		UPPER_LEFT,
		UPPER_CENTER,
		UPPER_RIGHT
	} CardinalPoint;

	/**
	 * Image width modes.
	 */
	typedef enum  {
		ORIGINAL, ///< Matching to the drawing width.
		FULL,     ///< Matching to the sheet width.
	} DrawingSize;

	/**
	 * Initialise the library.
	 * required to use the library. Do the plotter initialisation procedures.
	 * \param fileName The name of the file to load.
	 */
	void begin(char *fileName);

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
	 * Set the initial plotter position. Use before drawing.
	 * \param x The horizontal plotter position.
	 * \param y The horizontal plotter position.
	 */
	void setCardinalPoint(float x, float y);

	/**
	 * Set the initial plotter position. Use before drawing.
	 * \param position The plotter position (@see CardinalPoint).
	 */
	void setCardinalPoint(CardinalPoint position);

	/**
	 * Set the plotter speed (in mm/s).
	 * A next feature will support dynamic speed, then this parameter will be a 'base' speed.
	 * \param speed The plotter speed.
	 * \bug Speed growing if steep numbers growing.
	 */
	void setSpeed(unsigned int speed);

	/**********************
	* Fonctions de dessin *
	**********************/

	/**
	 * Move the plotter to the absloute position [\a x ; \a y].
	 * \param x The horizontal absolute position of the destination point.
	 * \param y The vertical absolute position of the destination point.
	 */
	void move(float x, float y);

	/**
	 * Move the plotter to the absloute position \a position.
	 * \param position The absolue position of the destination point (@see CardinalPoint)
	 */
	void move(CardinalPoint position);

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
	void drawingArea(char *fileName, DrawingSize size = ORIGINAL, CardinalPoint position = CENTER);

	/**
	 * Draw a drawing as descibed in the \a fileName file stored int the SD card.
	 * \param fileName Le nom du fichier gcode à dessiner.
	 * \TODO Check the M02 presence (end of drawing) before the end of drawing.
	 */
	void draw(char *fileName, DrawingSize size = ORIGINAL, CardinalPoint position = CENTER);

  private:

	/**
	 * The codes to send to the computer trought the serial link.
	 * \TODO Optimize the warning and errors messages: maybe use only one enum ?
	*/
	typedef enum {
		RELEASE_LEFT,       ///< Release the left belt for one step;
		PULL_LEFT,          ///< Pull the left belt for one step;
		RELEASE_RIGHT,      ///< Release the left belt for one step;
		PULL_RIGHT,         ///< Pull the left belt for one step;
		WRITING,            ///< The plotter is drawing;
		MOVING,             ///< The plotter moving (dot drawing);
		START_MESSAGE,      ///< Begining of the message to display on the sreen (if any) or/and on the computer (if any);
		END_MESSAGE,        ///< End of the message to display on the sreen (if any) or/and on the computer (if any);
		ENABLE_MOTORS,      ///< Enable the motors and the servo;
		DISABLE_MOTORS,     ///< Disable the motors and the servo;
		SLEEP,              ///< Pause the program;
		CHANGE_TOOL,        ///< Pause the program to change the tool;
		END_DRAWING,        ///< End of the drawing;
		START_WARNING,      ///< Begining of a warning (followed by te warning code);
		END_WARNING,        ///< End of warning message;
		START_ERROR,        ///< Begining of an error (followed by te error code);
		END_ERROR,          ///< End of error message;
		START = 100,        ///< Sart-up with the serial link;
		START_INSTRUCTIONS, ///< Begining of the initialization data;
		END_INSTRUCTIONS,   ///< End of the initialization data;
	} SerialData;

	/**
	* The errors and warnings which should occurs during the program execution.
	* The errors starts to 0 and the Warnings starts to 100.
	* The warnings doesn't interfere in the drawing and the errors are critical anomalies which prevents the drawing.
	*/
	typedef enum {
		CARD_NOT_FOUND,     ///< The SD card is not found or not readable;
		FILE_NOT_FOUND,     ///< The file not exists;
		FILE_NOT_READABLE,  ///< Error while opening the file;
		TOO_SHORT_SPAN,     ///< The distance between the belt extremities is lower than the sheet width plus his horizontal coordinate;
		TOO_FEW_PARAMETERS, ///< One or several parameters have not been read;

		// Warnings
		UNKNOWN_SERIAL_CODE = 100, ///< Character sent throught serial link has not been recognised (used only in the compunter side);
		WRONG_CONFIG_LINE,         ///< Incorrectly formatted line in the configuration file. Param : the line number;
		TOO_LONG_CONFIG_LINE,      ///< Too long line in the configuration file. Param : the line number;
		UNKNOWN_CONFIG_KEY,        ///< Unknown key in the configuration file. Param : the key, the line number;
		UNKNOWN_CONFIG_POSITION,   ///< Can not read the position string in the configuration file;
		UNKNOWN_GCODE_FUNCTION,    ///< Unknown GCode function in the drawing file;
		UNKNOWN_GCODE_PARAMETER,   ///< Unknown GCode parameter;
		WRONG_GCODE_PARAMETER,     ///< Error while reader a parameter. Param: The parameter, the line number.
		LEFT_LIMIT,                ///< The plotter reached the left limit of the sheet;
		RIGHT_LIMIT,               ///< The plotter reached the right limit of the sheet;
		UPPER_LIMIT,               ///< The plotter reached the upper limit of the sheet;
		LOWER_LIMIT,               ///< The plotter reached the lower limit of the sheet;
	} Error;

	/*************
	* Attributes *
	*************/

	/// Instance of the servo, used to drive it with the \a Servo library.
	Servo mServo;

	/// The GCode file of the drawing.
	File mFile;

	/// Left belt length, in steps.
	unsigned long mLeftLength;

	/// Right belt length, in steps.
	unsigned long mRightLength;

	/// Horizontal offset. Can be used to calibrate the drawing in a accurate position.
	unsigned int mOffsetX;

	/// Vertical offset. Can be used to calibrate the drawing in a accurate position.
	unsigned int mOffsetY;

	/// Drawing scale. Can be used to calibrate the drawing in a accurate scale.
	float mDrawingScale;

	/// Width of the running drawing.
	int mDrawingWidth;

	/// Hight of the running drawing.
	int mDrawingHeight;

	/// Step length. This is the distance traveled by the belt in one step, in mm.
	/// \TODO Use micro or more accurate unit to use int instead of floats ?
	float mStepLength;

	/// Initial delay between each motor step (in mm). It concerns the motor which have the longger distance to travel.
	/// The delay concerning the other motor is calculaed in such a way as to the 2 motors are synchronised, that is, they finishes the line on the same time.
	float mDelay;

	/// The robot is currently writing (\a true) or not (\a false).
	bool mIsWriting;

	/************
	* Positions *
	************/

	/// Current horizontal coordinate of the plotter on the sheet.
	/// \TODO Use floats
	float mPositionX;

	/// Current vertical coordinate of the plotter on the sheet.
	/// \TODO Use floats
	float mPositionY;

	/// Current vertical coordinate of the plotter on the sheet, that it, the distance between the pen and the sheet (can be negative).
	/// \TODO Use floats
	float mPositionZ;

	/*************
	* Parameters *
	*************/

	// Commencent par mp, pour "member" et "parameter".
	/// \TODO: Virer les préfices 'mp'

	// * Dessin *

	/// Distance between the belt extremities.
	unsigned int mpSpan;

	/// Sheet width, in mm.
	unsigned int mpSheetWidth;

	/// Sheet height (in mm).
	unsigned int mpSheetHeight;

	/// Horizontal position of the upper left corner of the sheet, according to the left belt extremity.
	unsigned int mpSheetPositionX;

	/// Vertical position of the upper left corner of the sheet, according to the left belt extremity.
	unsigned int mpSheetPositionY;

	// * Servo-motor *

	/// Servo angle while the plotter is writing (in degrees).
	unsigned int mpServoWritingAngle;

	/// Servo angle while the plotter is moving (in degrees).
	unsigned int mpServoMoovingAngle;

	/// Delay before the servo moves (en ms).
	unsigned int mpPreServoDelay;

	/// Delay after the servo moves (en ms).
	unsigned int mpPostServoDelay;

	// * Motors *

	/// Step motor numbers. Consider the motor driver multiplier.
	unsigned int mpSteps;

	/// Pinion diameter (in micrometers).
	int mpDiameter;

	/// Direction of the left motor. \a true to release the belt when the motor rotates clockwise, \a false if counter clockwise.
	bool mpLeftDirection;

	/// Direction of the right motor. \a true to release the belt when the motor rotates clockwise, \a false if counter clockwise.
	bool mpRightDirection;

	// * Misc *

	/// Delay before start the drawing (en milliseconds).
	unsigned int mpInitialDelay;

	/// Horizontal scale applied to the drawing. Used to calibrate it according to his width.
	float mpScaleX;

	/// Vertical scale applied to the drawing. Used to calibrate it according to his height.
	float mpScaleY;

	/// Horizontal offset applied to the drawing. Used to shift it according to his width.
	int mpOffsetX;

	/// Vertical offset applied to the drawing. Used to shift it according to his height.
	int mpOffsetY;

	/// Default plotter speed. Can be changed with setSpeed().
	/// \TODO Dynamically manage the speed.
	unsigned int mpDefaultSpeed;

	/// Default starting position of the plotter. Can be changed with setPosition().
	CardinalPoint mpInitPosition;

	/// Default final position of plotter.
	CardinalPoint mpEndPosition;

	/***********
	* Methods *
	***********/

	int processVar();

	/**
	 * Initialise the X et Y offsets according to the configuration file and the desired position of the drawing.
	 */
	void initOffset(CardinalPoint position);

	/**
	 * Initialise the scale according to the desired drawing width.
	 */
	void initScale(DrawingSize size);

	/**
	 * Wait until the \a msg appears on the serial link.
	 * \param msg The msg which wake up the plotter.
	 */
	void waitUntil(char msg);

	/**
	 * Initialise the ratio of number of steps to distance.
	 * Calculated with the pinion diameter and the number of steps.
	 * - xx(mm)*ratio --> xx(pas)
	 * - xx(pas)/ratio --> xx(mm)
	 * \TODO Invert the ratio, then the ratio will be an integer value, so that prevents working with floats.
	 */
	void initStepLength();

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
	void error(Error errorNumber, char *msg = (char *) "");

	/**
	 * Function called when a warning appends.
	 * \TODO Displays the error on the screen, if any;
	 * - Send the error code \a to the computer trought the serial link, if any.
	 * \param warningNumber The warning number (See Drawall::Error);
	 * \param msg An optional warning message which adds an information about the about (empty string if there is no message to send);
	 */
	void warning(Error warningNumber, char *msg = (char *) "");

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
	 * Get the horizontal coordinate from the \a position. See Drawall::CardinalPoint.
	 * \param position The position to convert.
	 * \return The horizontal coordinate of the input \a position.
	 */
	int positionToX(CardinalPoint position);

	/**
	 * Get the vertical coordinate from the \a position. See Drawall::CardinalPoint.
	 * \param position The position to convert.
	 * \return The vertical coordinate of the input \a position.
	 */
	int positionToY(CardinalPoint position);

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
	 * \param shouldWrite \a true to come close the pen to the sheet (writing), \a false to keep away (mooving).
	 */
	void writingPen(bool shouldWrite);

	/**
	 * Converts a string into a boolean value.
	 * \param value A string containing the word "true", or "false".
	 * \return \a true if the String is "true", \a false if not.
	 */
	bool atob(char *value);

	/**
	 * Converts a string into a position.
	 * \param str_pos A string containing the word "LOWER_LEFT", "LOWER_CENTER", "LOWER_RIGHT", "LEFT_CENTER", "CENTER", "RIGHT_CENTER", "UPPER_LEFT", "UPPER_CENTER" or "UPPER_RIGHT". Throw the warning UNKNOWN_CONFIG_POSITION (see Drawall::Error) if the string doesn't match with any of these words.
	 */
	Drawall::CardinalPoint atop(char *str_pos);

	/**
	 * Load the parameters from the configuration file on the SD card.
	 * \param fileName The name of the configuration file to load.
	 */
	void loadParameters(char *fileName);

};

#endif
