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

#include "irx_imports.h"
#include "mechaproxy.h"

#define MODNAME "mechaproxy"
IRX_ID(MODNAME, 1, 2);

extern struct irx_export_table _exp_mechaproxy;

static SifRpcDataQueue_t SifMechaScmdQD;
static SifRpcServerData_t SifMechaScmdData;
static int SifMechaScmdThreadID;

static unsigned char SifServerBuffer[0x1000];

static void *MechaScmdHeader(int function, void *buffer, int nbytes)
{
	struct MechaScmdParams *params = (struct MechaScmdParams *) buffer;

	params->result = sceCdApplySCmd(params->cmd, params->input, params->inputlength, params->output);

	return buffer;
}

static void SifMechaScmdThread(void *parameters)
{
	if(!sceSifCheckInit()){
		printf("yet sif hasn't been init\n");
		sceSifInit();
	}

	sceSifInitRpc(0);
	sceSifSetRpcQueue(&SifMechaScmdQD, GetThreadId());
	sceSifRegisterRpc(&SifMechaScmdData, MECHAPROXY_SCMD_HEADER, &MechaScmdHeader, SifServerBuffer, NULL, NULL, &SifMechaScmdQD);
	sceSifRpcLoop(&SifMechaScmdQD);
}

int _start(int argc, char *argv[])
{
	if(RegisterLibraryEntries(&_exp_mechaproxy) != 0)
		return MODULE_NO_RESIDENT_END;

	iop_thread_t thread;
	thread.attr = TH_C;
	thread.priority = 0x28;
	thread.stacksize = 0x800;

	thread.thread=&SifMechaScmdThread;
	SifMechaScmdThreadID = CreateThread(&thread);
	if(SifMechaScmdThreadID == 0)
		return MODULE_NO_RESIDENT_END;
	StartThread(SifMechaScmdThreadID, NULL);

	return MODULE_RESIDENT_END;
}
