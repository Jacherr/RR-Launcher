/*
    crash.c - crash handler when the main game throws an exception

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

#include "crash.h"
#include "console.h"
#include "prompt.h"
#include "util.h"
#include "time.h"

void rrc_crash_handle(void *xfb)
{
    char* lines[] = {
        "- - - Retro Rewind crashed! - - -",
        "",
        "This could have been caused by faulty My Stuff",
        "files, online cheaters, a bug in the pack,",
        "or a corrupted installation.",
        "",
        "Would you like to upload the crash file",
        "for analysis?"
    };

    enum rrc_prompt_result res = rrc_prompt_yes_no(xfb, lines, 8);

    if(res == RRC_PROMPT_RESULT_YES)
    {
        // upload Crash.pul
    }
} 