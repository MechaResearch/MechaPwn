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

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <tamtypes.h>
#include <libpad.h>
#include <libmc.h>
#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <iopcontrol.h>
#include <sbv_patches.h>

#include "mecha.h"
#include "exploit.h"
#include "pad.h"
#include "binaries.h"
#include "ui.h"
#include "mass.h"

static void ResetIOP()
{
	SifInitRpc(0);
	SifIopReset("", 0);
	while(!SifIopSync());
	SifInitRpc(0);
	SifLoadFileInit();
}

char unlockNVM()
{
	gsKit_clear(gsGlobal, Black);
	
	uint8_t version[3];
	getMechaVersion(version);
	
	uint8_t build_date[5];
	getMechaBuildDate(build_date);
	
	struct GSTEXTURE_holder *versionTextures = ui_printf(10, 100, 42, 0xFFFFFF, "Mecha version: %d.%02d\n", version[1], version[2]);
	struct GSTEXTURE_holder *buildTextures = ui_printf(10, 150, 42, 0xFFFFFF, "Mecha build date: 20%02x/%02x/%02x %02x:%02x\n", build_date[0], build_date[1], build_date[2], build_date[3], build_date[4]);
	
	const uint8_t *patch = getPatch(build_date);
	
	struct GSTEXTURE_holder *exploitTextures = draw_text(10, 200, 42, 0xFFFFFF, "Press O exploit.\n");
	struct GSTEXTURE_holder *exitTextures = draw_text(10, 250, 42, 0xFFFFFF, "Press X to exit.\n");
	
	drawFrame();
	
	while (1)
	{
		u32 new_pad = ReadCombinedPadStatus();

		if(new_pad & PAD_CIRCLE)
		{
			break;
		}
		else if(new_pad & PAD_CROSS)
		{
			gsKit_clear(gsGlobal, Black);
			ResetIOP();
			LoadExecPS2("rom0:OSDSYS", 0, NULL);
			
			return 0;
		}
	}

	if (!installPatch(patch))
	{
		struct GSTEXTURE_holder *errorTextures = draw_text(10, 300, 42, 0xFFFFFF, "Failed to install the patch.\n");

		drawFrame();
	
		freeGSTEXTURE_holder(versionTextures);
		freeGSTEXTURE_holder(buildTextures);
		freeGSTEXTURE_holder(exploitTextures);
		freeGSTEXTURE_holder(exitTextures);
		freeGSTEXTURE_holder(errorTextures);
		
		return 0;
	}
	
	freeGSTEXTURE_holder(versionTextures);
	freeGSTEXTURE_holder(buildTextures);
	freeGSTEXTURE_holder(exploitTextures);
	freeGSTEXTURE_holder(exitTextures);
	
	return 1;
}

struct MENU
{
	const char *title;
	const char *x_text;
	const char *o_text;
	uint8_t option_count;
	const char *options[10];
};

int drawMenu(struct MENU *menu)
{
	int selected_menu = 0;

	while(1)
	{
		u32 new_pad = ReadCombinedPadStatus();

		if(new_pad & PAD_UP)
		{
			selected_menu--;
		}
		else if(new_pad & PAD_DOWN)
		{
			selected_menu++;
		}
		else if(new_pad & PAD_CIRCLE)
		{
			return -1;
		}
		else if(new_pad & PAD_CROSS)
		{
			return selected_menu;
		}
		
		if (selected_menu < 0)
			selected_menu = menu->option_count - 1;
		else if (selected_menu > menu->option_count - 1)
			selected_menu = 0;
		
		gsKit_clear(gsGlobal, Black);
	
		int title_x, title_y;
		getTextSize(52, menu->title, &title_x, &title_y);
	
		struct GSTEXTURE_holder *TitleTexture = draw_text((gsGlobal->Width - title_x) / 2, 10, 52, 0xFFFFFF, menu->title);
		struct GSTEXTURE_holder *oTexture = draw_text(10, -74, 42, 0xFFFFFF, menu->o_text);
		struct GSTEXTURE_holder *xTexture = draw_text(-10, -74, 42, 0xFFFFFF, menu->x_text);
		
		struct GSTEXTURE_holder *menus[256];
		memset(menus, 0, sizeof(menus));

		for (int i = 0; i < menu->option_count; i++)
		{
			int menu_x, menu_y;
			getTextSize(42, menu->options[i], &menu_x, &menu_y);
			
			menus[i] = draw_text((gsGlobal->Width - menu_x) / 2, 100 + i * 50, 42, selected_menu == i ? 0xFF6040 : 0xFFFFFF, menu->options[i]);
		}

		drawFrame();
		
		for (int i = 0; i < 256; i++)
			if (menus[i])
				freeGSTEXTURE_holder(menus[i]);
		
		freeGSTEXTURE_holder(xTexture);
		freeGSTEXTURE_holder(oTexture);
		freeGSTEXTURE_holder(TitleTexture);
	}
}

uint8_t region_keyseed[] = { 0x4d, 0x65, 0x63, 0x68, 0x61, 0x50, 0x77, 0x6e, 0x00, 0xec };
uint8_t region_ciphertext_dex[] = { 0x1a, 0x74, 0xac, 0xb2, 0xb0, 0xae, 0x15, 0xdf, 0x00, 0xc1 };

// 0 - Japan
uint8_t region_params_japan[] = { 0x4a, 0x4a, 0x6a, 0x70, 0x6e, 0x4a, 0x4a, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t region_ciphertext_japan_cex[] = { 0xf1, 0xef, 0x50, 0xec, 0x1c, 0x2e, 0xc5, 0x69, 0x00, 0x6b };

// 1 - USA
uint8_t region_params_usa[] = { 0x41, 0x41, 0x65, 0x6e, 0x67, 0x41, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t region_ciphertext_usa_cex[] = { 0xb8, 0x42, 0xd5, 0xbc, 0x53, 0xa1, 0x96, 0xe6, 0x00, 0x04 };

// 2 - Oceania
uint8_t region_params_oceania[] = { 0x45, 0x45, 0x65, 0x6e, 0x67, 0x45, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t region_ciphertext_oceania_cex[] = { 0x64, 0xaa, 0x23, 0x25, 0x28, 0x5c, 0x14, 0xf3, 0x00, 0x1e };

// 3 - UK
// same as europe

// 4 - Europe
uint8_t region_params_europe[] = { 0x45, 0x45, 0x65, 0x6e, 0x67, 0x45, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t region_ciphertext_europe_cex[] = { 0x54, 0x88, 0x4f, 0x90, 0x76, 0x4d, 0xcb, 0xeb, 0x00, 0xcb };

// 5 - Korea
uint8_t region_params_korea[] = { 0x48, 0x4b, 0x6b, 0x6f, 0x72, 0x4a, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00 };
int8_t region_ciphertext_korea_cex[] = { 0x2a, 0xf0, 0x38, 0x2d, 0xef, 0xe5, 0x48, 0xa4, 0x00, 0xc0 };

// 6 - Asia
uint8_t region_params_asia[] = { 0x48, 0x48, 0x65, 0x6e, 0x67, 0x4a, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t region_ciphertext_asia_cex[] = { 0x2a, 0xf0, 0x38, 0x2d, 0xef, 0xe5, 0x48, 0xa4, 0x00, 0xc0 };

// 7 - Tawain
uint8_t region_params_tawain[] = { 0x48, 0x48, 0x74, 0x63, 0x68, 0x4a, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00 };
// region_ciphertext is same as asia

// 8 - Russia
uint8_t region_params_russia[] = { 0x45, 0x52, 0x65, 0x6e, 0x67, 0x45, 0x52, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t region_ciphertext_russia_cex[] = { 0x3e, 0x11, 0xc1, 0xbb, 0xfb, 0x26, 0x2b, 0x4c, 0x00, 0x9c };

// 9 - China
uint8_t region_ciphertext_china_cex[] = { 0x0f, 0x5d, 0xb8, 0xa2, 0x9d, 0x85, 0xcc, 0x76, 0x00, 0xd5 };

// 10 - ???

// 11 - Mexico / ???
uint8_t region_ciphertext_mexico_cex[] = { 0x35, 0x1a, 0x92, 0x9a, 0x05, 0x5b, 0xdc, 0x40, 0x00, 0x08 };

// 12 - ???

void sum_buffer(uint8_t *buffer, int length)
{
	uint8_t sum = 0;
	for (int i = 0; i < length - 2; i++)
		sum += buffer[i];
	buffer[length - 2] = 0;
	buffer[length - 1] = ~sum;
}

char write_region(uint8_t *region_params, uint8_t *model_number, uint8_t *region_ciphertext)
{
	if (region_params)
	{
		for (int i = 0; i < 12; i += 2)
			if (!WriteNVM(192 + i / 2, *(uint16_t *) &region_params[i]))
				break;
	}
	
	if (model_number)
	{
		for (int i = 0; i < 18; i += 2)
			if (!WriteNVM(216 + i / 2, *(uint16_t *) &model_number[i]))
				break;
	}
	
	if (region_ciphertext)
	{
		for (int i = 0; i < 10; i += 2)
			if (!WriteNVM(227 + i / 2, *(uint16_t *) &region_keyseed[i]))
				break;
	
		for (int i = 0; i < 10; i += 2)
			if (!WriteNVM(232 + i / 2, *(uint16_t *) &region_ciphertext[i]))
				break;
	}
			
	return 1;
}

void selectCexDex(char *isDex)
{
	gsKit_clear(gsGlobal, Black);
	
	struct MENU menu;
	menu.title = "Select console type";
	menu.x_text = "X Select";
	menu.o_text = "O Exit";
	menu.option_count = 2;
	menu.options[0] = "CEX (retail)";
	menu.options[1] = "Retail-DEX";
	int selected = drawMenu(&menu);

	gsKit_clear(gsGlobal, Black);

	drawFrame();
	
	if (selected == -1)
	{
		ResetIOP();
		LoadExecPS2("rom0:OSDSYS", 0, NULL);
		SleepThread();
	}
	
	*isDex = selected == 1;
}


void selectModel(char isDex, char *isSlim, char *model)
{
	if (isDex)
		strcpy(model, "DTL-H");
	else
		strcpy(model, "SCPH-");
		
	gsKit_clear(gsGlobal, Black);
	
	struct MENU menu;
	menu.title = "Select model";
	menu.x_text = "X Select";
	menu.o_text = "O Exit";
	menu.option_count = 5;
	
	for (int i = 0; i < 5; i++)
	{
		char *option = malloc(20);
		strcpy(option, model);
		menu.options[i] = option;
	}
	
	strcat(menu.options[0], "50xxx");
	strcat(menu.options[1], "70xxx");
	strcat(menu.options[2], "75xxx");
	strcat(menu.options[3], "77xxx");
	strcat(menu.options[4], "90xxx");
	
	int selected = drawMenu(&menu);
	
	for (int i = 0; i < 5; i++)
		free(menu.options[i]);
	
	if (selected == -1)
	{
		ResetIOP();
		LoadExecPS2("rom0:OSDSYS", 0, NULL);
		SleepThread();
	}
	else if(selected == 0)
	{
		strcat(model, "50");
		*isSlim = 0;
	}
	else if(selected == 1)
	{
		strcat(model, "70");
		*isSlim = 1;
	}
	else if(selected == 2)
	{
		strcat(model, "75");
		*isSlim = 1;
	}
	else if(selected == 3)
	{
		strcat(model, "77");
		*isSlim = 1;
	}
	else if(selected == 4)
	{
		strcat(model, "90");
		*isSlim = 1;
	}
}

void selectRegion2(char isDex, char isSlim, char *model, uint8_t **region_params, uint8_t **region_ciphertext);
void selectRegion(char isDex, char isSlim, char *model, uint8_t **region_params, uint8_t **region_ciphertext)
{
	gsKit_clear(gsGlobal, Black);
	
	struct MENU menu;
	menu.title = "Select model";
	menu.x_text = "X Select";
	menu.o_text = "O Exit";
	menu.option_count = 7;
	
	for (int i = 0; i < 6; i++)
	{
		char *option = malloc(20);
		strcpy(option, model);
		menu.options[i] = option;
	}
	
	strcat(menu.options[0], "000 - Japan");
	strcat(menu.options[1], "001 - USA");
	strcat(menu.options[2], "002 - Oceania");
	strcat(menu.options[3], "003 - UK");
	strcat(menu.options[4], "004 - Europe");
	strcat(menu.options[5], "005 - Korea");
	menu.options[6] = "More";
	
	int selected = drawMenu(&menu);
	
	for (int i = 0; i < 6; i++)
		free(menu.options[i]);
	
	if (selected == -1)
	{
		ResetIOP();
		LoadExecPS2("rom0:OSDSYS", 0, NULL);
		SleepThread();
	}
	else if(selected == 0)
	{
		strcat(model, "000");
		sum_buffer(model, 18);
		if (isSlim)
			*region_params = region_params_japan;
		if (isDex)
			*region_ciphertext = region_ciphertext_dex;
		else
			*region_ciphertext = region_ciphertext_japan_cex;
	}
	else if(selected == 1)
	{
		strcat(model, "001");
		sum_buffer(model, 18);
		if (isSlim)
			*region_params = region_params_usa;
		if (isDex)
			*region_ciphertext = region_ciphertext_dex;
		else
			*region_ciphertext = region_ciphertext_usa_cex;
	}
	else if(selected == 2)
	{
		strcat(model, "002");
		sum_buffer(model, 18);
		if (isSlim)
			*region_params = region_params_oceania;
		if (isDex)
			*region_ciphertext = region_ciphertext_dex;
		else
			*region_ciphertext = region_ciphertext_oceania_cex;
	}
	else if(selected == 3)
	{
		strcat(model, "003");
		sum_buffer(model, 18);
		if (isSlim)
			*region_params = region_params_europe;
		if (isDex)
			*region_ciphertext = region_ciphertext_dex;
		else
			*region_ciphertext = region_ciphertext_europe_cex;
	}
	else if(selected == 4)
	{
		strcat(model, "004");
		sum_buffer(model, 18);
		if (isSlim)
			*region_params = region_params_europe;
		if (isDex)
			*region_ciphertext = region_ciphertext_dex;
		else
			*region_ciphertext = region_ciphertext_europe_cex;
	}
	else if(selected == 5)
	{
		strcat(model, "005");
		sum_buffer(model, 18);
		if (isSlim)
			*region_params = region_params_korea;
		if (isDex)
			*region_ciphertext = region_ciphertext_dex;
		else
			*region_ciphertext = region_ciphertext_korea_cex;
	}
	else if(selected == 6)
	{
		return selectRegion2(isDex, isSlim, model, region_params, region_ciphertext);
	}
}

void selectRegion2(char isDex, char isSlim, char *model, uint8_t **region_params, uint8_t **region_ciphertext)
{
	gsKit_clear(gsGlobal, Black);
	
	struct MENU menu;
	menu.title = "Select model";
	menu.x_text = "X Select";
	menu.o_text = "O Exit";
	menu.option_count = (isSlim ? 7 : 6);
	
	for (int i = 1; i < (isSlim ? 7 : 6); i++)
	{
		char *option = malloc(20);
		strcpy(option, model);
		menu.options[i] = option;
	}
	
	menu.options[0] = "Back";
	strcat(menu.options[1], "006 - Asia");
	strcat(menu.options[2], "007 - Tawain");
	strcat(menu.options[3], "008 - Russia");
	if (!isSlim)
	{
		strcat(menu.options[4], "009 - China");
		strcat(menu.options[5], "011 - Mexico");
	}
	else
	{
		strcat(menu.options[4], "010 - ???");
		strcat(menu.options[5], "011 - ???");
		strcat(menu.options[6], "012 - ???");
	}
	
	int selected = drawMenu(&menu);

	for (int i = 1; i < (isSlim ? 7 : 6); i++)
		free(menu.options[i]);
		
	if (selected == -1)
	{
		ResetIOP();
		LoadExecPS2("rom0:OSDSYS", 0, NULL);
		SleepThread();
	}
	else if(selected == 0)
	{
		return selectRegion(isDex, isSlim, model, region_params, region_ciphertext);
	}
	else if(selected == 1)
	{
		strcat(model, "006");
		sum_buffer(model, 18);
		if (isSlim)
			*region_params = region_params_asia;
		if (isDex)
			*region_ciphertext = region_ciphertext_dex;
		else
			*region_ciphertext = region_ciphertext_asia_cex;
	}
	else if(selected == 2)
	{
		strcat(model, "007");
		sum_buffer(model, 18);
		if (isSlim)
			*region_params = region_params_tawain;
		if (isDex)
			*region_ciphertext = region_ciphertext_dex;
		else
			*region_ciphertext = region_ciphertext_asia_cex;
	}
	else if(selected == 3)
	{
		strcat(model, "008");
		sum_buffer(model, 18);
		if (isSlim)
			*region_params = region_params_russia;
		if (isDex)
			*region_ciphertext = region_ciphertext_dex;
		else
			*region_ciphertext = region_ciphertext_russia_cex;
	}
	else if(selected == 4)
	{
		if (!isSlim)
		{
			strcat(model, "009");
			sum_buffer(model, 18);
			if (isDex)
				*region_ciphertext = region_ciphertext_dex;
			else
				*region_ciphertext = region_ciphertext_china_cex;
		}
		else
		{
			strcat(model, "010");
			sum_buffer(model, 18);
			*region_params = region_params_usa;
			if (isDex)
				*region_ciphertext = region_ciphertext_dex;
			else
				*region_ciphertext = region_ciphertext_usa_cex;
		}
	}
	else if(selected == 5)
	{
		if (!isSlim)
		{
			strcat(model, "011");
			sum_buffer(model, 18);
			if (isDex)
				*region_ciphertext = region_ciphertext_dex;
			else
				*region_ciphertext = region_ciphertext_mexico_cex;
		}
		else
			{
			strcat(model, "011");
			sum_buffer(model, 18);
			*region_params = region_params_usa;
			if (isDex)
				*region_ciphertext = region_ciphertext_dex;
			else
				*region_ciphertext = region_ciphertext_usa_cex;
		}
	}
	else if(selected == 6)
	{
		strcat(model, "012");
		sum_buffer(model, 18);
		*region_params = region_params_usa;
		if (isDex)
			*region_ciphertext = region_ciphertext_dex;
		else
			*region_ciphertext = region_ciphertext_usa_cex;
	}
}

void setRegion()
{
	char isDex = 0;	
	selectCexDex(&isDex);
	char isSlim = 0;
	char model[18];
	selectModel(isDex, &isSlim, model);
	uint8_t *region_params = 0;
	uint8_t *region_ciphertext = 0;
	selectRegion(isDex, isSlim, model, &region_params, &region_ciphertext);
	
	write_region(region_params, model, region_ciphertext);
}

uint8_t *frames[] = {
	&frame_001, &frame_002, &frame_003, &frame_004, &frame_005, &frame_006, &frame_007, &frame_008,
	&frame_009, &frame_010, &frame_011, &frame_012, &frame_013, &frame_014, &frame_015, &frame_016,
	&frame_017, &frame_018, &frame_019, &frame_020, &frame_021, &frame_022, &frame_023, &frame_024,
	&frame_025, &frame_026, &frame_027, &frame_028, &frame_029, &frame_030, &frame_031, &frame_032,
	&frame_033, &frame_034, &frame_035, &frame_036, &frame_037, &frame_038, &frame_039, &frame_040,
	&frame_041, &frame_042, &frame_043, &frame_044, &frame_045, &frame_046, &frame_047, &frame_048,
	&frame_049, &frame_050, &frame_051, &frame_052, &frame_053, &frame_054, &frame_055, &frame_056,
	&frame_057, &frame_058, &frame_059, &frame_060, &frame_061, &frame_062
};

void drawLogoFrame(uint8_t frame, char *text2)
{
	gsKit_clear(gsGlobal, Black);
	
	struct GSTEXTURE_holder *logoTextures = drawImage((gsGlobal->Width - 480) / 2, (gsGlobal->Height - 270) / 2, 480, 270, frames[frame]);

	char text[] = "MechaPwn";			
	int x, y;
	getTextSize(42, text, &x, &y);
	y += 42;
	struct GSTEXTURE_holder *textTextures = draw_text((gsGlobal->Width - x) / 2, (gsGlobal->Height - 270) / 2 - 50, 42, 0xFFFFFF, text);
		
	int x2, y2;
	getTextSize(42, text2, &x2, &y2);
	y2 += 42;
	struct GSTEXTURE_holder *text2Textures = draw_text((gsGlobal->Width - x2) / 2, (gsGlobal->Height - 270) / 2 + 270, 42, 0xFFFFFF, text2);

	drawFrame();
	
	freeGSTEXTURE_holder(text2Textures);
	freeGSTEXTURE_holder(textTextures);
	freeGSTEXTURE_holder(logoTextures);
}

void drawLogo()
{
	uint8_t frame = 0;
	while(!MassCheck())
	{
		drawLogoFrame(frame++, "Waiting for USB drive...");
	
		if (frame >= 62)
			frame = 0;
	}
	
	while (1)
	{
		drawLogoFrame(frame++, "Press X to continue.");
	
		if (frame >= 62)
			frame = 0;
			
		u32 new_pad = ReadCombinedPadStatus();

		if(new_pad & PAD_CROSS)
		{
			break;
		}
	}
}

char backupNVM()
{
	FILE *f = fopen("mass:/nvm.bin", "wb");
	if (!f)
	{
		gsKit_clear(gsGlobal, Black);
	
		char text[] = "Failed to open nvm.bin!";
			
		int x, y;
		getTextSize(42, text, &x, &y);
		y += 42;
		
		struct GSTEXTURE_holder *textTextures = draw_text((gsGlobal->Width - x) / 2, (gsGlobal->Height - y) / 2, 42, 0xFFFFFF, text);
		drawFrame();
		freeGSTEXTURE_holder(textTextures);
		return 0;
	}
			
	int x, y;
	getTextSize(42, "Backing up to nvm.bin...", &x, &y);
	y += 42;

	int last = 0;
	for (int i = 0; i < 0x200; i++)
	{
		int new = (i * 100) / 0x200;
		if (new != last)
		{
			gsKit_clear(gsGlobal, Black);
			struct GSTEXTURE_holder *textTextures = ui_printf((gsGlobal->Width - x) / 2, (gsGlobal->Height - y) / 2, 42, 0xFFFFFF, "Backing up to nvm.bin... %d%%", new);
			drawFrame();
			freeGSTEXTURE_holder(textTextures);
			last = new;
		}
	
		uint16_t data;
		if (!ReadNVM(i, &data))
			break;
		fwrite(&data, 1, 2, f);
	}
	
	fclose(f);
	
	return 1;
}

char restorePatches()
{
	FILE *f = fopen("mass:/nvm.bin", "rb");
	if (!f)
	{
		gsKit_clear(gsGlobal, Black);
	
		char text[] = "Failed to open nvm.bin!";
			
		int x, y;
		getTextSize(42, text, &x, &y);
		y += 42;
		
		struct GSTEXTURE_holder *textTextures = draw_text((gsGlobal->Width - x) / 2, (gsGlobal->Height - y) / 2, 42, 0xFFFFFF, text);
		drawFrame();
		freeGSTEXTURE_holder(textTextures);
		return 0;
	}
			
	int x, y;
	getTextSize(42, "Restoring patches...", &x, &y);
	y += 42;
	
	int last = 0;
	fseek(f, 400 * 2, SEEK_SET);
	for (int i = 0; i < 112; i++)
	{
		int new = (i * 100) / 112;
		if (new != last)
		{
			gsKit_clear(gsGlobal, Black);
			struct GSTEXTURE_holder *textTextures = ui_printf((gsGlobal->Width - x) / 2, (gsGlobal->Height - y) / 2, 42, 0xFFFFFF, "Restoring patches... %d%%", new);
			drawFrame();
			freeGSTEXTURE_holder(textTextures);
			last = new;
		}
		
		uint16_t data;
		fread(&data, 1, 2, f);
		if (!WriteNVM(400, data))
			break;
	}
	
	fclose(f);
	return 1;
}

uint8_t *getPowerTexture()
{
	char model_number[18];
	for (int i = 0; i < 18; i += 2)
		if (!ReadNVM(216 + i / 2, (uint16_t *) &model_number[i]))
			break;

	if (
		memcmp(model_number, "SCPH-9", 6) == 0 ||
		memcmp(model_number, "DTL-H9", 6) == 0
	)
		return  &pwr90k;

	if (
		memcmp(model_number, "SCPH-7", 6) == 0 ||
		memcmp(model_number, "DTL-H7", 6) == 0
	)
		return  &pwr70k;
	
	return &pwr50k;
}

void checkUnsupportedVersion()
{
	uint8_t version[3];
	uint8_t build_date[5];
	if (!getMechaVersion(version) || !getMechaBuildDate(build_date))
	{
		struct GSTEXTURE_holder *errorTextures = draw_text(10, 200, 42, 0xFFFFFF, "This MechaCon isn't supported!\n");

		drawFrame();
		
		freeGSTEXTURE_holder(errorTextures);
		
		SleepThread();
		return 0;
	}
	

	struct GSTEXTURE_holder *versionTextures = ui_printf(10, 100, 42, 0xFFFFFF, "Mecha version: %d.%02d\n", version[1], version[2]);
	struct GSTEXTURE_holder *buildTextures = ui_printf(10, 150, 42, 0xFFFFFF, "Mecha build date: 20%02x/%02x/%02x %02x:%02x\n", build_date[0], build_date[1], build_date[2], build_date[3], build_date[4]);
	
	const uint8_t *patch = getPatch(build_date);
	if (patch == 0)
	{
		struct GSTEXTURE_holder *errorTextures = draw_text(10, 200, 42, 0xFFFFFF, "This MechaCon isn't supported!\n");

		drawFrame();
		
		freeGSTEXTURE_holder(versionTextures);
		freeGSTEXTURE_holder(buildTextures);
		freeGSTEXTURE_holder(errorTextures);
		
		SleepThread();
		return 0;
	}
}

int main()
{
	init_ui();

	gsKit_mode_switch(gsGlobal, GS_ONESHOT);

	gsKit_clear(gsGlobal, Black);

	struct GSTEXTURE_holder *loadingTextures = draw_text(-10, -74, 42, 0xFFFFFF, "Loading...");
	drawFrame();
	freeGSTEXTURE_holder(loadingTextures);
	
	ResetIOP();
	
	sbv_patch_enable_lmb();
	sbv_patch_disable_prefix_check();
	sbv_patch_fileio();

	SifExecModuleBuffer(&iomanX, size_iomanX, 0, NULL, NULL);
	SifExecModuleBuffer(&fileXio, size_fileXio, 0, NULL, NULL);
	SifExecModuleBuffer(&freesio2, size_freesio2, 0, NULL, NULL);
	SifExecModuleBuffer(&freepad, size_freepad, 0, NULL, NULL);
	SifExecModuleBuffer(&mcman, size_mcman, 0, NULL, NULL);
	SifExecModuleBuffer(&mcserv, size_mcserv, 0, NULL, NULL);
	SifExecModuleBuffer(&USBD, size_USBD, 0, NULL, NULL);
	SifExecModuleBuffer(&USBHDFSD, size_USBHDFSD, 0, NULL, NULL);
	SifExecModuleBuffer(&MECHAPROXY_irx, size_MECHAPROXY_irx, 0, NULL, NULL);
	SifExecModuleBuffer(&MASSWATCHER_irx, size_MASSWATCHER_irx, 0, NULL, NULL);

	MechaInit();
	MassInit();
	
	mcInit(MC_TYPE_XMC);
	
	PadInitPads();
	
	// ---
	
	drawLogo();
	
	checkUnsupportedVersion();
	
	if (!IsNVMUnlocked())
	{
		if (backupNVM())
			unlockNVM();
	}
	else
	{
		setRegion();
		restorePatches();
	}

	gsKit_clear(gsGlobal, Black);

	struct GSTEXTURE_holder *imageTextures = drawImage((gsGlobal->Width - 400) / 2, (gsGlobal->Height - (225 + 60)) / 2, 400, 225, getPowerTexture());
		
	char text[] = "Unplug the power cord.";

	int x2, y2;
	getTextSize(42, text, &x2, &y2);
	y2 += 42;
	
	struct GSTEXTURE_holder *unplugTextures = draw_text((gsGlobal->Width - x2) / 2, ((gsGlobal->Height - (225 + 60)) / 2) + 225, 42, 0xFFFFFF, text);
	drawFrame();
	freeGSTEXTURE_holder(unplugTextures);
	freeGSTEXTURE_holder(imageTextures);
	
	// ---
	
	SleepThread();
	
	return 0;
}
