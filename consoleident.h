#ifndef CONSOLEIDENT_H
#define CONSOLEIDENT_H

#include <stdint.h>

typedef enum CONSOLE_COLOR {
    UNKNOWN = 0,
    BLACK,
    WHITE,
    SATIN_SILVER,
    PINK,
    CINNABAR_RED,
    BLUE,
    ZEN_BLACK,
    OCEAN_BLUE,
    MIDNIGHT_BLUE,
    SAKURA_PINK,
    AQUA_BLUE,
    GOLD,
	METAL, // metal casing. arcade ps2
    COLORCOUNT, // ammount of colors in enum
}ConsoleColor_t;


typedef struct consolerecord {
    uint16_t id;
    const char* name;
    int flags;
    int color;
}console_record_t;

#define BSH(x) (1 << x)
enum FLG {
    DEX = BSH(1),
    COH = BSH(2),
    PCMCIA = BSH(3),
    PSX = BSH(4),
    PROTOKERNEL = BSH(5),
    PS2TV = BSH(6),
};
const console_record_t* ConsoleModel_GetDataByID(uint16_t ID);
const char* ConsoleModel_GetColorName(int color);

#endif
