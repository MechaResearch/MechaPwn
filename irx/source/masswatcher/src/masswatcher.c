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
#include "masswatcher.h"

#define MODNAME "masswatcher"
IRX_ID(MODNAME, 1, 2);

extern struct irx_export_table _exp_masswatcher;

static SifRpcDataQueue_t SifMassQD;
static SifRpcServerData_t SifMassData;
static int SifMassThreadID;

static unsigned char SifServerBuffer[0x1000];

static char massState[2] = { 0, 0 };

static void *MassHeader(int function, void *buffer, int nbytes)
{
	char *params = (char *) buffer;

	*params = massState[0] || massState[1];

	return buffer;
}

static void SifMassThread(void *parameters)
{
	if(!sceSifCheckInit()){
		printf("yet sif hasn't been init\n");
		sceSifInit();
	}

	sceSifInitRpc(0);
	sceSifSetRpcQueue(&SifMassQD, GetThreadId());
	sceSifRegisterRpc(&SifMassData, MASSWATCHER_HEADER, &MassHeader, SifServerBuffer, NULL, NULL, &SifMassQD);
	sceSifRpcLoop(&SifMassQD);
}

static void usbmass_cb0(int cause)
{
	if (cause == USBMASS_DEV_EV_CONN)
		massState[0] = 1;
	else if (cause == USBMASS_DEV_EV_DISCONN)
		massState[0] = 0;
}

static void usbmass_cb1(int cause)
{
	if (cause == USBMASS_DEV_EV_CONN)
		massState[1] = 1;
	else if (cause == USBMASS_DEV_EV_DISCONN)
		massState[1] = 0;
}

int _start(int argc, char *argv[])
{
	if(RegisterLibraryEntries(&_exp_masswatcher) != 0)
		return MODULE_NO_RESIDENT_END;
	
	UsbMassRegisterCallback(0, usbmass_cb0);
	UsbMassRegisterCallback(1, usbmass_cb1);

	iop_thread_t thread;
	thread.attr = TH_C;
	thread.priority = 0x28;
	thread.stacksize = 0x800;

	thread.thread=&SifMassThread;
	SifMassThreadID = CreateThread(&thread);
	if(SifMassThreadID == 0)
		return MODULE_NO_RESIDENT_END;
	StartThread(SifMassThreadID, NULL);

	return MODULE_RESIDENT_END;
}
