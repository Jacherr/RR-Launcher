/*
    util.h - utility constants and macro definitions

    Copyright (C) 2025  Retro Rewind Team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef RRC_UTIL_H
#define RRC_UTIL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define _RRC_STRING(s) #s

#define FATAL(...) printf(__VA_ARGS__); \
    usleep(1000000); \
    exit(1);

#define RRC_ASSERT(condition, what)                                     \
    do                                                                  \
    {                                                                   \
        int st = condition;                                             \
        char *st_str = _RRC_STRING(condition);                          \
        if (!st)                                                        \
        {                                                               \
            FATAL("%s: assert failed: %s (got %i)", what, st_str, st);  \
        }                                                               \
    } while (0);
                                        
#define RRC_ASSERTEQ(lhs, rhs, what)                                                      \
    do                                                                                    \
    {                                                                                     \
        int st = lhs == rhs;                                                              \
        char *st_str = _RRC_STRING(lhs) " == " _RRC_STRING(rhs);                          \
        if (!st)                                                                          \
        {                                                                                 \
            FATAL("%s: assert failed: %s (lhs = %i, rhs = %i)", what, st_str, lhs, rhs);  \
        }                                                                                 \
    } while (0);

#endif
