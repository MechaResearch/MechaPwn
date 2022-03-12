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

#include <ft2build.h>
#include FT_FREETYPE_H

#include "ui.h"
#include "binaries.h"

GSGLOBAL *gsGlobal;
FT_Library library;
FT_Face face;

extern int gsKit_texture_finish(GSGLOBAL *gsGlobal, GSTEXTURE *Texture);
void draw_bitmap(int x, int y, FT_Bitmap *bitmap, GSTEXTURE *Texture, u32 color)
{
    memset(Texture, 0, sizeof(GSTEXTURE));

    Texture->Delayed  = 1;
    Texture->PSM      = GS_PSM_CT32;
    Texture->Filter   = GS_FILTER_NEAREST;
    Texture->VramClut = 0;
    Texture->Clut     = NULL;

    Texture->Width    = bitmap->width;
    Texture->Height   = bitmap->rows;
    u32 TextureSize   = gsKit_texture_size_ee(Texture->Width, Texture->Height, Texture->PSM);
    Texture->Mem      = memalign(128, TextureSize);

    u32 *buff         = (u32 *)Texture->Mem;

    memset(buff, 0, TextureSize);

    if (bitmap->pixel_mode == FT_PIXEL_MODE_GRAY)
    {
        for (FT_Int i = 0; i < bitmap->rows; i++)
        {
            for (FT_Int j = 0; j < bitmap->width; j++)
            {
                u8 cl                       = 0xFF - bitmap->buffer[i * bitmap->width + j];
                buff[i * bitmap->width + j] = cl << 24 | color;
            }
        }
    }

    gsKit_texture_finish(gsGlobal, Texture);

    gsKit_TexManager_bind(gsGlobal, Texture);

    gsKit_prim_sprite_texture(gsGlobal, Texture, x, y, 0.0f, 0.0f, x + Texture->Width, y + Texture->Height, Texture->Width, Texture->Height, 3, GS_SETREG_RGBAQ(0xFF, 0xFF, 0xFF, 0xFF, 0x00));
}

struct GSTEXTURE_holder *drawImage(int x, int y, int width, int height, u8 *texture)
{
    struct GSTEXTURE_holder *TextureHolder = malloc(sizeof(struct GSTEXTURE_holder));
    memset(TextureHolder, 0, sizeof(struct GSTEXTURE_holder));
    GSTEXTURE *Texture = &TextureHolder->curr;

    memset(Texture, 0, sizeof(GSTEXTURE));

    Texture->Delayed  = 1;
    Texture->PSM      = GS_PSM_CT32;
    Texture->Filter   = GS_FILTER_NEAREST;
    Texture->VramClut = 0;
    Texture->Clut     = NULL;

    Texture->Width    = width;
    Texture->Height   = height;
    u32 TextureSize   = gsKit_texture_size_ee(Texture->Width, Texture->Height, Texture->PSM);
    Texture->Mem      = memalign(128, TextureSize);

    uint32_t pos      = 0;
    uint32_t tx       = 0;
    uint32_t ty       = 0;

    uint32_t hw       = 0;
    while (hw < height * width)
    {
        uint8_t count = texture[pos];
        pos += 1;

        char repeat_run = count & 0x80;
        count           = (count & 0x7f) + 1;

        for (int i = 0; i < count; i++)
        {
            Texture->Mem[ty * width + tx] = (texture[pos] << 16) | (texture[pos + 1] << 8) | texture[pos + 2];
            hw += 1;
            tx += 1;

            if (!repeat_run)
                pos += 3;
        }

        if (repeat_run)
            pos += 3;

        if (tx == width)
        {
            ty += 1;
            tx = 0;
        }
    }

    gsKit_texture_finish(gsGlobal, Texture);

    gsKit_TexManager_bind(gsGlobal, Texture);

    gsKit_prim_sprite_texture(gsGlobal, Texture, x, y, 0.0f, 0.0f, x + Texture->Width, y + Texture->Height, Texture->Width, Texture->Height, 3, GS_SETREG_RGBAQ(0xFF, 0xFF, 0xFF, 0xFF, 0x00));

    return TextureHolder;
}

void getTextSize(int size, const char *text, int *x, int *y)
{
    FT_GlyphSlot slot = face->glyph;
    u32 text_len      = strlen(text);

    FT_Set_Pixel_Sizes(face, 0, size);

    *x = 0;
    *y = 0;

    for (int n = 0; n < text_len; n++)
    {
        int error = FT_Load_Char(face, text[n], FT_LOAD_RENDER);
        if (error)
            continue;

        *x += slot->advance.x >> 6;
        *y += slot->advance.y >> 6;
    }
}

struct GSTEXTURE_holder *draw_text(int x, int y, int size, u32 color, const char *text)
{
    FT_GlyphSlot slot = face->glyph;
    u32 text_len      = strlen(text);

    FT_Set_Pixel_Sizes(face, 0, size);

    if (x < 0 || y < 0)
    {
        int size_x, size_y;
        getTextSize(size, text, &size_x, &size_y);

        if (x < 0)
            x += gsGlobal->Width - size_x;
        if (y < 0)
            y += gsGlobal->Height - size_y;
    }

    y += size;

    struct GSTEXTURE_holder *Textures = 0;
    for (int n = 0; n < text_len; n++)
    {
        int error = FT_Load_Char(face, text[n], FT_LOAD_RENDER);
        if (error)
            continue;

        struct GSTEXTURE_holder *Texture = malloc(sizeof(struct GSTEXTURE_holder));
        memset(Texture, 0, sizeof(struct GSTEXTURE_holder));
        draw_bitmap(x + slot->bitmap_left, y - slot->bitmap_top, &slot->bitmap, &Texture->curr, color);

        Texture->next = Textures;
        Textures      = Texture;

        x += slot->advance.x >> 6;
        y += slot->advance.y >> 6;
    }

    return Textures;
}

struct GSTEXTURE_holder *ui_printf(int x, int y, int size, u32 color, const char *format, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    struct GSTEXTURE_holder *Textures = draw_text(x, y, size, color, buffer);
    va_end(args);
    return Textures;
}

void freeGSTEXTURE_holder(struct GSTEXTURE_holder *Textures)
{
    while (Textures)
    {
        struct GSTEXTURE_holder *next = Textures->next;
        gsKit_TexManager_free(gsGlobal, &Textures->curr);
        if (Textures->curr.Mem)
            free(Textures->curr.Mem);
        free(Textures);
        Textures = next;
    }
}

void drawFrame()
{
    gsKit_queue_exec(gsGlobal);

    gsKit_sync_flip(gsGlobal);

    gsKit_TexManager_nextFrame(gsGlobal);
}

void init_ui()
{
    gsGlobal                  = gsKit_init_global();

    gsGlobal->PSM             = GS_PSM_CT24;
    gsGlobal->PSMZ            = GS_PSMZ_16S;
    gsGlobal->PrimAlphaEnable = GS_SETTING_ON;

    dmaKit_init(D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC,
                D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);

    dmaKit_chan_init(DMA_CHANNEL_GIF);

    gsKit_init_screen(gsGlobal);

    int error = FT_Init_FreeType(&library);

    error     = FT_New_Memory_Face(library, &IndieFlower, size_IndieFlower, 0, &face);
}
