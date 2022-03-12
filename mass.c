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

#include <stdio.h>
#include <string.h>
#include <kernel.h>
#include <sifrpc.h>

#include "mass.h"
#include "masswatcher.h"

static SifRpcClientData_t SifRpcClientMass;

static unsigned char RpcBuffer[0x1000] ALIGNED(64);

#define _printf(args...)

int MassInit()
{
    SifInitRpc(0);

    nopdelay();
    while (SifBindRpc(&SifRpcClientMass, MASSWATCHER_HEADER, 0) < 0 || SifRpcClientMass.server == NULL)
    {
        _printf("libsecr: bind failed\n");
    }

    return 1;
}

void MassDeinit()
{
    memset(&SifRpcClientMass, 0, sizeof(SifRpcClientData_t));
}

char MassCheck()
{
    char *params = (char *)RpcBuffer;

    if (SifCallRpc(&SifRpcClientMass, 1, 0, RpcBuffer, sizeof(RpcBuffer), RpcBuffer, sizeof(RpcBuffer), NULL, NULL) < 0)
    {
        _printf("MassScmd: rpc error\n");
        return 0;
    }

    return *params;
}
