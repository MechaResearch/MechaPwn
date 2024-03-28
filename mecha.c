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

#include <debug.h>
#include <stdio.h>
#include <string.h>
#include <kernel.h>
#include <sifrpc.h>

#include "mecha.h"
#include "mechaproxy.h"

static SifRpcClientData_t SifRpcClientMechaScmd;

static unsigned char RpcBuffer[0x1000] ALIGNED(64);

#define _printf(args...) // scr_printf(args)

int MechaInit()
{
    SifInitRpc(0);

    nopdelay();
    while (SifBindRpc(&SifRpcClientMechaScmd, MECHAPROXY_SCMD_HEADER, 0) < 0 || SifRpcClientMechaScmd.server == NULL)
    {
        _printf("libsecr: bind failed\n");
    }

    return 1;
}

void MechaDeinit()
{
    memset(&SifRpcClientMechaScmd, 0, sizeof(SifRpcClientData_t));
}

int MechaScmd(u8 cmd, void *input, u8 inputlength, void *output)
{
    struct MechaScmdParams *params = (struct MechaScmdParams *)RpcBuffer;

    params->cmd                    = cmd;
    params->inputlength            = inputlength;
    memcpy(params->input, input, inputlength);

    if (SifCallRpc(&SifRpcClientMechaScmd, 1, 0, RpcBuffer, sizeof(RpcBuffer), RpcBuffer, sizeof(RpcBuffer), NULL, NULL) < 0)
    {
        _printf("MechaScmd: rpc error\n");
        return 0;
    }

    memcpy(output, params->output, 16);

    return params->result;
}

char getMechaVersion(uint8_t *data)
{
    u8 input[1];
    u8 output[16];
    input[0] = 0x00;

    if (MechaScmd(0x03, input, sizeof(input), output) != 1)
    {
        return 0;
    }

    memcpy(data, &output[0], 4);

    return 1;
}

char getMechaBuildDate(uint8_t *data)
{
    u8 input[1];
    u8 output[16];
    input[0] = 0xfd;
    if (MechaScmd(0x03, input, sizeof(input), output) != 1 || output[0] != 0)
    {
        _printf("Failed to read the build date!\n");
        return 0;
    }

    memcpy(data, &output[1], 5);

    return 1;
}

char OpenConfig(uint8_t id, char write, uint8_t blocks)
{
    u8 input[3];
    u8 output[16];

    input[0] = write;  // 0 = read, 1 = write
    input[1] = id;     // id
    input[2] = blocks; // blocks

    if (MechaScmd(0x40, input, sizeof(input), output) != 1)
    {
        if (output[0] != 0)
        {
            CloseConfig();

            if (MechaScmd(0x40, input, sizeof(input), output) != 1 || output[0] != 0)
            {
                _printf("Failed to cmd OpenConfig!\n");
                return 0;
            }
        }
    }

    return 1;
}

char ReadConfig(uint8_t *data)
{
    u8 input[0];
    u8 output[16];

    if (MechaScmd(0x41, input, sizeof(input), output) != 1)
    {
        _printf("Failed to cmd ReadConfig!\n");
        return 0;
    }

    memcpy(data, output, 16);

    return 1;
}

char WriteConfig(const uint8_t *data)
{
    u8 input[16];
    u8 output[16];

    memcpy(input, data, 16);

    if (MechaScmd(0x42, input, sizeof(input), output) != 1 || output[0] != 0)
    {
        _printf("Failed to cmd WriteConfig!\n");
        return 0;
    }

    return 1;
}

char CloseConfig()
{
    u8 input[0];
    u8 output[16];
    output[0] = 1;

    while (output[0])
    {
        if (MechaScmd(0x43, input, sizeof(input), output) != 1 || (output[0] != 0 && output[0] != 1))
        {
            _printf("Failed to cmd CloseConfig!\n");
            return 0;
        }
    }

    return 1;
}

char ReadNVM(uint16_t offset, uint16_t *data)
{
    u8 input[2];
    u8 output[16];
    input[0] = offset >> 8;
    input[1] = offset;
    if (MechaScmd(0x0A, input, sizeof(input), output) != 1 || output[0] != 0)
    {
        _printf("Failed to cmd 0x0A!\n");
        return 0;
    }

    *data = output[1];
    *data <<= 8;
    *data |= output[2];

    return 1;
}

char WriteNVM(uint16_t offset, uint16_t data)
{
    u8 input[4];
    u8 output[16];
    input[0]  = offset >> 8;
    input[1]  = offset;
    input[2]  = data >> 8;
    input[3]  = data;
    output[0] = 1;

    while (output[0])
    {
        if (MechaScmd(0x0B, input, sizeof(input), output) != 1 || (output[0] != 0 && output[0] != 1))
        {
            _printf("Failed to cmd WriteNVM! (%d)\n", offset);
            return 0;
        }
    }

    return 1;
}

char IsNVMUnlocked()
{
    uint16_t data;
    if (!ReadNVM(400, &data))
        return 0;

    if (!WriteNVM(400, data))
        return 0;

    return 1;
}

char getSerial(uint32_t *serial)
{
    uint16_t part1;
    uint16_t part2;
    uint8_t version[4];
    if (!ReadNVM(0xFA, &part1))
        return 0;
    if (!ReadNVM(0xFB, &part2))
        return 0;

    if (getMechaVersion(version))
    {
        if (version[1] < 4)
        {
            if (!ReadNVM(0xE6, &part1))
                return 0;
            if (!ReadNVM(0xE7, &part2))
                return 0;
        }
    }

    *serial = ((part2 & 0xff) << 16) | part1;
    return 1;
}
