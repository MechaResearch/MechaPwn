/*
 * Copyright (c) 2021 MechaResearch
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <kernel.h>
#include <libpad.h>
#include <stdio.h>
#include <string.h>
#include <tamtypes.h>

#include "pad.h"

static unsigned char padArea[2][256] ALIGNED(64);
static unsigned int old_pad[2] = {0, 0};

void PadInitPads(void)
{
    padInit(0);
    padPortOpen(0, 0, padArea[0]);
    padPortOpen(1, 0, padArea[1]);

    old_pad[0] = 0;
    old_pad[1] = 0;
}

void PadDeinitPads(void)
{
    padPortClose(0, 0);
    padPortClose(1, 0);
    padEnd();
}

int ReadPadStatus_raw(int port, int slot)
{
    struct padButtonStatus buttons;
    u32 paddata;

    paddata = 0;
    if (padRead(port, slot, &buttons) != 0)
    {
        paddata = 0xffff ^ buttons.btns;
    }

    return paddata;
}

int ReadCombinedPadStatus_raw(void)
{
    return (ReadPadStatus_raw(0, 0) | ReadPadStatus_raw(1, 0));
}

int ReadPadStatus(int port, int slot)
{
    struct padButtonStatus buttons;
    u32 new_pad, paddata;

    new_pad = 0;
    if (padRead(port, slot, &buttons) != 0)
    {
        paddata       = 0xffff ^ buttons.btns;

        new_pad       = paddata & ~old_pad[port];
        old_pad[port] = paddata;
    }

    return new_pad;
}

int ReadCombinedPadStatus(void)
{
    return (ReadPadStatus(0, 0) | ReadPadStatus(1, 0));
}
