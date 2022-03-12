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

#include <gsKit.h>
#include <dmaKit.h>
#include <malloc.h>

#define White      GS_SETREG_RGBAQ(0xFF, 0xFF, 0xFF, 0x00, 0x00)
#define Black      GS_SETREG_RGBAQ(0x00, 0x00, 0x00, 0x00, 0x00)
#define Red        GS_SETREG_RGBAQ(0xFF, 0x00, 0x00, 0x00, 0x00)
#define Green      GS_SETREG_RGBAQ(0x00, 0xFF, 0x00, 0x00, 0x00)
#define Blue       GS_SETREG_RGBAQ(0x00, 0x00, 0xFF, 0x00, 0x00)

#define BlueTrans  GS_SETREG_RGBAQ(0x00, 0x00, 0xFF, 0x40, 0x00)
#define RedTrans   GS_SETREG_RGBAQ(0xFF, 0x00, 0x00, 0x60, 0x00)
#define GreenTrans GS_SETREG_RGBAQ(0x00, 0xFF, 0x00, 0x50, 0x00)
#define WhiteTrans GS_SETREG_RGBAQ(0xFF, 0xFF, 0xFF, 0x50, 0x00)

struct GSTEXTURE_holder
{
    GSTEXTURE curr;
    struct GSTEXTURE_holder *next;
};

extern GSGLOBAL *gsGlobal;

struct GSTEXTURE_holder *drawImage(int x, int y, int width, int height, u8 *texture);

void getTextSize(int size, const char *text, int *x, int *y);

struct GSTEXTURE_holder *draw_text(int x, int y, int size, u32 color, const char *text);

struct GSTEXTURE_holder *ui_printf(int x, int y, int size, u32 color, const char *format, ...);

void freeGSTEXTURE_holder(struct GSTEXTURE_holder *Textures);

void drawFrame();

void init_ui();
