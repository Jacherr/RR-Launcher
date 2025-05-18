/*
    privilege.h - IOS privilege escalation exploit headers

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

#ifndef RRC_PRIVILEGE_H
#define RRC_PRIVILEGE_H

#include "../result.h"

/*
    Attempts to perform the /dev/sha Init exploit.

    See EXPLOIT.md for details.
*/
struct rrc_result rrc_privilege_send_exploit();

#endif