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
 * Configuration file for the plotter, describing enabled components.
 */

/// Enable sending debug data through serial link, with 0 = disabled and 1 = enabled.
#define EN_DEBUG 0

/// Enable buzzer rings, with 0 = disabled and 1 = enabled.
#define EN_BUZZER 0

/// Enable pause button, with 0 = disabled and 1 = enabled.
#define EN_PAUSE_BUTTON 0

/// Enable motor step modes, with 0 = disabled and 1 = enabled.
#define EN_STEP_MODES 1

/// Enable limit sensors, with 0 = disabled and 1 = enabled.
#define EN_LIMIT_SENSORS 0

/// Enable remote support, with 0 = disabled and 1 = enabled.
#define EN_REMOTE_SUPPORT 0

/// Enable screen support, with 0 = disabled and 1 = enabled.
#define EN_SCREEN 0

/// Enable serial communication, with 0 = disabled and 1 = enabled.
#define EN_SERIAL 1

/// Serial speed, in bauds.
#define SERIAL_BAUDS 57600
