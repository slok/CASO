/*
Copyright (C) 2011 Iraide Diaz (Sharem)
Copyright (C) 2011 Xabier Larrakoetxea (slok)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _COLORS_H
#define _COLORS_H

#define COL_RESET "\x1b[0m"
// Foreground colors are in form of 3x
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define WHITE    "\x1b[37m"

// Background colors are in form of 4x

#define RED_BK      "\x1b[41m"
#define GREEN_BK    "\x1b[42m"
#define YELLOW_BK   "\x1b[43m"
#define BLUE_BK     "\x1b[44m"
#define MAGENTA_BK  "\x1b[45m"
#define CYAN_BK     "\x1b[46m"
#define WHITE_BK    "\x1b[47m"

// Bold format is 1;
#define RED_BOLD     "\x1b[1;31m"
#define GREEN_BOLD   "\x1b[1;32m"
#define YELLOW_BOLD  "\x1b[1;33m"
#define BLUE_BOLD    "\x1b[1;34m"
#define MAGENTA_BOLD "\x1b[1;35m"
#define CYAN_BOLD    "\x1b[1;36m"
#define WHITE_BOLD   "\x1b[1;37m"

//examples of combination
#define WHITE_BK_RED_FG     "\x1b[47;31m"
#define BLUE_BK_WHITE_FG    "\x1b[44;37m"
#define GREEN_BK_YELLOW_FG  "\x1b[42;33m"

#endif
