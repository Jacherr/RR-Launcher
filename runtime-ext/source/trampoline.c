/*
    trampoline.c - Trampoline function scratch space

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

#include <types.h>

/**
 * Scratch space for the machine code of 5 trampoline functions.
 * The last one (DVDClose) is currently copied into here anyway, but unused.
 *
 * This is given a fixed address in the linker script, and we initialize it in the launcher DOL
 * since the first 16 bytes (4 instructions) are copied from the original DVD functions we overwrite.
 */
__attribute__((section(".dvd_trampolines"), used)) u8 _rrc_trampoline_scratch_space[32][5];
