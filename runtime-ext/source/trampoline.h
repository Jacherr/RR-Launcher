/*
    trampoline.h - Declares "trampoline" functions, which are small stubs
    that contain the original 4 instructions that were overwritten
    with a jump to our custom functions, as well as 4 instructions
    to jump back to the original function after said overwritten instructions.
    These are intended to be called if you want the original behavior of the DVD functions
    (e.g. you want to fall back to the original logic after trying the custom logic).

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

#ifndef RRC_RUNTIME_EXT_TRAMPOLINE
#define RRC_RUNTIME_EXT_TRAMPOLINE

#include <types.h>
#include "dvd.h"

extern u8 _rrc_trampoline_scratch_space[5][32];

s32 dvd_convert_path_to_entrynum_trampoline(const char *);
s32 dvd_open_trampoline(const char *, FileInfo *);
s32 dvd_fast_open_trampoline(s32, FileInfo *);
s32 dvd_read_prio_trampoline(FileInfo *, void *, s32, s32, s32);

#endif
