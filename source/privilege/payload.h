/*
    privilege.h - IOS privilege escalation ARM payload

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

    The exploit used in this program to elevate privileges is adapted from
    wfc-patcher-wii, which is licenced via GPL-2-or-later.
    See <https://www.gnu.org/licenses/> for license details.

    See https://github.com/WiiLink24/wfc-patcher-wii/blob/main/README.md
*/

#ifndef RRC_PAYLOAD_H
#define RRC_PAYLOAD_H

#include <gctypes.h>

/**
 * This is the stub THUMB code to be copied to the beginning of MEM1 (should
 * be max 32 bytes).
 */
static u32 armstage0[8] = {
    // .thumb
    // @ Immediately switch to ARM mode. This instruction would in theory be
    // @ ignored if in ARM mode anyway, due to it requiring the N==1
    // condition.
    // bx      pc
    // nop
    0x477846C0, //
    // .arm
    0xE59F1010, // ldr     r1, =ENTRY_POINT
    0xEE071F36, // mcr     p15, 0, r1, c7, c6, 1 @ Invalidate DCache
    0xE3A00000, // mov     r0, #0
    0xEE070F9A, // mcr     p15, 0, r0, c7, c10, 4 @ Drain write buffer
    0xEE071F35, // mcr     p15, 0, r1, c7, c5, 1 @ Invalidate ICache
    0xE12FFF11, // bx      r1
    0x00000002, // .long   ENTRY_POINT @ Set on runtime
};

static u32 armstage1[] __attribute__((aligned(32))) = {
    0xE59F2014, // ldr     r2, =STAGE1_END
                // L_CacheLoop:
    0xEE071F36, // mcr     p15, 0, r1, c7, c6, 1 @ Invalidate DCache
    0xEE071F35, // mcr     p15, 0, r1, c7, c5, 1 @ Invalidate ICache
    0xE2811020, // add     r1, r1, #32
    0xE1510002, // cmp     r1, r2
    0xBAFFFFFA, // blt     L_CacheLoop
    0xEA000001, // b       Entry
    0x00000000, // STAGE1_END @ Set on runtime
    0x00000000, // MESSAGE_VALUE
    // Entry:
    // @ Flush the entire data cache
    // L_FlushLoop:
    0xEE17FF7A, // mrc     p15, 0, pc, c7, c10, 3
    0x1AFFFFFD, // bne     L_FlushLoop
    // @ Give PPC full bus access
    // See EXPLOIT.md
    0xE3A02536, // mov     r2, #0x0D800000
    0xE5921060, // ldr     r1, [r2, #0x60]
    0xE3811008, // orr     r1, #0x08
    0xE5821060, // str     r1, [r2, #0x60]
    0xE5921064, // ldr     r1, [r2, #0x64]
    0xE381113A, // orr     r1, #0x8000000E
    0xE3811EDF, // orr     r1, #0x00000DF0
    0xE5821064, // str     r1, [r2, #0x64]
    // @ Wait for PPC to finish patching
    0xE24F3034, // adr     r3, MESSAGE_VALUE
    // L_PPCWaitLoop:
    0xEE073F36, // mcr     p15, 0, r3, c7, c6, 1 @ Invalidate DCache
    0xE5932000, // ldr     r2, [r3]
    0xE3520001, // cmp     r2, #1
    0x1AFFFFFB, // bne     L_PPCWaitLoop
    // @ Invalidate the entire ICache
    0xEE070F15, // mcr     p15, 0, r0, c7, c5, 0
    // @ Flush the entire data cache (again)
    // L_FlushLoop2:
    0xEE17FF7A, // mrc     p15, 0, pc, c7, c10, 3
    0x1AFFFFFD, // bne     L_FlushLoop2
    // @ Send acknowledge back to PPC
    0xE3A00002, // mov     r0, #2
    0xE5830000, // str     r0, [r3]
    0xEE073F3A, // mcr     p15, 0, r3, c7, c10, 1 @ Flush DCache
    // @ Set NL=0 to skip the first instruction in case the exploit runs
    // again
    0xE3A00000, // mov     r0, #0
    0xE3500000, // cmp     r0, #0
    // @ Overwrite the reserved handler to infinitely loop
    0xE59F1004, // ldr     r1, =0xFFFF0014
    0xE5811020, // str     r1, [r1, #0x20]
    0xE12FFF11, // bx      r1
    // Data pool;
    0xFFFF0014,
};

#endif