#include "consoleident.h"
#include <stdlib.h>
#include <stdio.h>


#define arraysizeof(a) (sizeof(a) / sizeof(a[0]))

static const console_record_t DB[] = {
    {0xd200, "DTL-H10000", DEX | PCMCIA | PROTOKERNEL, BLACK},
    {0xd201, "SCPH-10000", PCMCIA | PROTOKERNEL, BLACK},
    {0xd202, "SCPH-15000/18000", PCMCIA, BLACK},
    {0xd203, "SCPH-30001", 0, BLACK},
    {0xd204, "SCPH-30002/R", 0, BLACK},
    {0xd205, "SCPH-30003/R", 0, BLACK},
    {0xd206, "SCPH-30004/R", 0, BLACK},
    {0xd207, "DTL-H30001", DEX, BLACK},
    {0xd208, "DTL-H30002", DEX, BLACK},
    {0xd209, "COH-H30000", COH, METAL},
    {0xd20a, "SCPH-18000", 0, BLACK},
    {0xd20b, "COH-H31000", COH, METAL},
    {0xd20c, "SCPH-30000", 0, BLACK},
    {0xd20d, "DTL-H30000", DEX, BLACK},
    {0xd20e, "COH-H31100", COH, METAL},
    {0xd20f, "SCPH-35001 GT", 0, BLACK},
    {0xd210, "SCPH-35002 GT", 0, BLACK},
    {0xd211, "SCPH-35003 GT", 0, BLACK},
    {0xd212, "SCPH-35004 GT", 0, BLACK},
    {0xd213, "SCPH-35000 GT", 0, BLACK},
    {0xd214, "SCPH-30001/R", 0, BLACK},
    {0xd215, "SCPH-30005 R", 0, BLACK},
    {0xd216, "SCPH-30006 R", 0, BLACK},
    {0xd217, "SCPH-39000", 0, BLACK},
    {0xd218, "SCPH-39001", 0, BLACK},
    {0xd219, "SCPH-39002", 0, BLACK},
    {0xd21a, "SCPH-39003", 0, BLACK},
    {0xd21b, "SCPH-39004", 0, BLACK},
//  {0xd21c, "SCPH-30007 R", 0, BLACK},
    {0xd21d, "SCPH-37000 L", 0, OCEAN_BLUE},
    {0xd21e, "SCPH-37000 B", 0, ZEN_BLACK},
    {0xd21f, "SCPH-39008", 0, BLACK},
    {0xd220, "SCPH-39000 TB", 0, BLUE},
    {0xd221, "SCPH-39000 RC", 0, BLACK},
    {0xd222, "SCPH-39006", 0, BLACK},
//  {0xd223, "SCPH-39005", 0, BLACK},
//  {0xd224, "SCPH-39007", 0, BLACK},
    {0xd225, "DTL-H10100", DEX | PROTOKERNEL, BLACK},
    {0xd226, "DTL-H30100", DEX, BLACK},
    {0xd227, "DTL-H30101", DEX, BLACK},
    {0xd228, "DTL-H30102", DEX, BLACK},
    {0xd229, "DTL-H30105", DEX, BLACK},
    {0xd22a, "SCPH-39000 S", 0, SATIN_SILVER},
    {0xd22b, "SCPH-39000 AQ", 0, AQUA_BLUE},
    {0xd22c, "SCPH-39000 SA", 0, PINK},
    {0xd22d, "SCPH-39010/N", 0, BLACK},
    // Deckard
    // d300 = SCPH-50000, whats that?
    {0xd301, "DTL-H50000", DEX, BLACK},
    {0xd302, "DTL-H50001", DEX, BLACK},
    {0xd303, "DTL-H50002", DEX, BLACK},
    {0xd304, "DTL-H50009", DEX, SATIN_SILVER},
    // d305 - d31d ??
    {0xd31e, "DTL-H70002", DEX, BLACK},
//  {0xd31f, "DTL-H70011S ???", DEX, BLACK},
//  {0xd320, "???", DEX, BLACK},
//  {0xd321, "???", DEX, BLACK},
    {0xd322, "DTL-H75000A", DEX, BLACK},
//  {0xd323, "???", DEX, BLACK},
//  {0xd324, "???", DEX, BLACK},
//  {0xd325, "???", DEX, BLACK},
    {0xd326, "DTL-H90000(a)", DEX, BLACK},
    // d327-d37f
    // X-chassis
    {0xd380, "DESR-7000", PSX, WHITE},
    {0xd381, "DESR-5000", PSX, WHITE},
    {0xd382, "DESR-7100", PSX, WHITE},
    {0xd383, "DESR-5100", PSX, WHITE},
    {0xd384, "DESR-5100/S", PSX, SATIN_SILVER},
    {0xd385, "DESR-7500", PSX, WHITE},
    {0xd386, "DESR-5500", PSX, WHITE},
    {0xd387, "DESR-7700", PSX, WHITE},
    {0xd388, "DESR-5700", PSX, WHITE},
    // d389 - d400 ??
    // H, I, J -chassis
//  {0xd400, "SCPH-50000", 0, BLACK},               // 35: PX1, PX2, 27203531, 27203532
    {0xd401, "SCPH-50001/N", 0, BLACK},             // 37: PX7, PX8, 27203731, 27203732
    {0xd402, "SCPH-50010/N", 0, BLACK},             // ??: PU6
    {0xd403, "SCPH-50000", 0, BLACK},               // 35: 27203500, 27203502
    {0xd404, "SCPH-50000 MB/NH", 0, MIDNIGHT_BLUE}, // 39, 43, 60: 27203900, 27203902, 27204302, 27206002
    {0xd405, "SCPH-50002", 0, BLACK},               // 35: 27203591
    {0xd406, "SCPH-50003", 0, BLACK},               // 35: 227203561
    {0xd407, "SCPH-50004", 0, BLACK},               // 35: 227203551, 27203552
    {0xd408, "SCPH-50002 SS", 0, SATIN_SILVER},     // 40: 27204091, Silver Prestige Line
    {0xd409, "SCPH-50003 SS", 0, SATIN_SILVER},     // 40: 27204061, Silver Prestige Line
    {0xd40a, "SCPH-50004 SS", 0, SATIN_SILVER},     // 40: 27204051, 27204052, Silver Prestige Line
    {0xd40b, "SCPH-50001", 0, BLACK},               // 41: PX3, PX4, 27204131, 27204132
    {0xd40c, "SCPH-50005/N", 0, BLACK},             // 37: 27203725
    {0xd40d, "SCPH-50006", 0, BLACK},               // 35: 27203512, 27203518
    {0xd40e, "SCPH-50007", 0, BLACK},               // 35: 27203516
    {0xd40f, "SCPH-50008", 0, BLACK},               // 35: 27203581
/*  {0xd410, "SCPH-50009", 0, BLACK}, */
    {0xd411, "SCPH-50000 NB", 0, BLACK},            // 45, 46, 54: 27204501, 27204502, 27204604, 27205402
    {0xd412, "SCPH-50000 TSS", 0, SATIN_SILVER},    // 40: 27204004
    {0xd413, "SCPH-55000 GU", 0, GOLD},             // 67: 27206702
    {0xd414, "SCPH-55000 GT", 0, WHITE},            // 47, 49: 27204704, 27204904
    {0xd415, "SCPH-50009 SS", 0, SATIN_SILVER},     // 37, 40: 27203720, 27204020
    {0xd416, "SCPH-50003 AQ", 0, AQUA_BLUE},        // 51: 27205161
/*  {0xd417, "SCPH-55005 GT/N", 0, WHITE},          // 50: 27205025 */
    {0xd418, "SCPH-55006 GT", 0, WHITE},            // 47: 27204718
    {0xd419, "SCPH-55007 GT", 0, WHITE},            // 47: 27204716
    {0xd41a, "SCPH-50008 SS", 0, SATIN_SILVER},     // 40: 27204081
    {0xd41b, "SCPH-50004 AQ", 0, AQUA_BLUE},        // 51: 27205151
    {0xd41c, "SCPH-50005 SS/N", 0, SATIN_SILVER},   // 55: 27205525
    {0xd41d, "SCPH-50005 AQ/N", 0, AQUA_BLUE},      // 56: 27205625
    {0xd41e, "SCPH-50000 CW", 0, WHITE},            // 44: 27204402
    {0xd41f, "SCPH-50000 SA", 0, PINK},             // 61: 27206101
    {0xd420, "SCPH-50004 ESS", 0, SATIN_SILVER},    // 57, 62, 66: 27205751, 27206251, 27206651 Satin Silver
 /* {0xd421, "???", 0, UNKNOWN}, */
    {0xd422, "SCPH-50002 ESS", 0, SATIN_SILVER},    // 62: 27206291 Satin Silver
    {0xd423, "SCPH-50003 ESS", 0, SATIN_SILVER},    // 66: 27206661 Satin Silver
    {0xd424, "SCPH-50000 PW", 0, WHITE},            // 65: 27206501
    {0xd425, "SCPH-50011", 0, BLACK},               // ??: SM1 (box not found)
                                                    // K-chassis
    {0xd426, "SCPH-70004", 0, BLACK},               // 63, 70: 27207051, 27207052, 27206351, 27206352
    {0xd427, "SCPH-70003", 0, BLACK},               // 63, 70: 27207061, 27207062, ????????, 27206362
    {0xd428, "SCPH-70002", 0, BLACK},               // 63, 70: 27207091, 27206392
    {0xd429, "SCPH-70011", 0, BLACK},               // 63: PK1, 27206331
    {0xd42a, "SCPH-70012", 0, BLACK},               // 63: PK2, 27206341
    {0xd42b, "SCPH-70000", 0, BLACK},               // 63, 78: 27206301, 27206302, 27207802
    {0xd42c, "SCPH-70005", 0, BLACK},               // 63: 27206325
    {0xd42d, "SCPH-70006", 0, BLACK},               // 63: 27206318
    {0xd42e, "SCPH-70007", 0, BLACK},               // 63: 27206316
    {0xd42f, "SCPH-70000 GT", 0, BLACK},            // 72: 27207202
    {0xd430, "SCPH-70008", 0, BLACK},               // 63: 27206381
    {0xd431, "SCPH-70002 SS", 0, SATIN_SILVER},     // 69: 27206991
    {0xd432, "SCPH-70003 SS", 0, SATIN_SILVER},     // 69: 27206961
    {0xd433, "SCPH-70004 SS", 0, SATIN_SILVER},     // 69: 27206951
    {0xd434, "SCPH-70008 SS", 0, SATIN_SILVER},     // 69: 27206981
    {0xd435, "SCPH-70001", 0, BLACK},               // 77: PK3, 27207731
    {0xd436, "SCPH-70010", 0, BLACK},               // sticker and box not found
    {0xd437, "SCPH-70000 CW", 0, WHITE},            // 79: 27207902
    {0xd438, "SCPH-70003 SS", 0, SATIN_SILVER},     // 80: 27208063
    {0xd439, "SCPH-70004 SS", 0, SATIN_SILVER},     // 80: 27208053
    {0xd43a, "SCPH-70008 SS", 0, SATIN_SILVER},     // 80: 27208081
                                                    // L-chassis
    {0xd43b, "SCPH-75001", 0, BLACK},               // 81, 82: PK4, PK5, 27208131, 27208232
    {0xd43c, "SCPH-75002", 0, BLACK},               // 81: 27208191, ????????
    {0xd43d, "SCPH-75003", 0, BLACK},               // 81, 82: 27208161, 27208262
    {0xd43e, "SCPH-75004", 0, BLACK},               // 81, 82: 27208151, 27208252
    {0xd43f, "SCPH-75000 SSS", 0, SATIN_SILVER},    // 85: 27208501
    {0xd440, "SCPH-75002 SS", 0, SATIN_SILVER},     // 87: 27208791
    {0xd441, "SCPH-75003 SS", 0, SATIN_SILVER},     // 87: 27208761
    {0xd442, "SCPH-75004 SS", 0, SATIN_SILVER},     // 87: 27208751
    {0xd443, "SCPH-75000", 0, BLACK},               // 82: 27208202
    {0xd444, "SCPH-75000 CW", 0, WHITE},            // 83: 27208302
    {0xd445, "SCPH-75006", 0, BLACK},               // 82: 27208218
    {0xd446, "SCPH-75007", 0, BLACK},               // 82: 27208216
    {0xd447, "SCPH-75005", 0, BLACK},               // 82: 27208225
    {0xd448, "SCPH-75010", 0, BLACK},               // PK6
    {0xd449, "SCPH-75000 FF", 0, BLACK},            // 89: 27208901
//  {0xd44a, "???", 0, UNKNOWN},
//  {0xd44b, "???", 0, UNKNOWN},
    {0xd44c, "SCPH-75008", 0, BLACK},               // 82: 27208281
    {0xd44d, "SCPH-75008 SS", 0, SATIN_SILVER},     // 88: 27208881
                                                    // M-chassis
    {0xd44e, "SCPH-77001", 0, BLACK},               // 91: PK7, PK8: 27209131
    {0xd44f, "SCPH-77002", 0, BLACK},               // 91: 27209191
    {0xd450, "SCPH-77003", 0, BLACK},               // 91: 27209161
    {0xd451, "SCPH-77004", 0, BLACK},               // 91: 27209151, 27209152
    {0xd452, "SCPH-77002 SS", 0, SATIN_SILVER},     // 95: 27209591
    {0xd453, "SCPH-77003 SS", 0, SATIN_SILVER},     // 95: 27209561
    {0xd454, "SCPH-77004 SS", 0, SATIN_SILVER},     // 95, 16: 27209551, 27201651
    {0xd455, "SCPH-77000", 0, BLACK},               // 91: 27209102
    {0xd456, "SCPH-77000 CW", 0, WHITE},            // 93: 27209302
    {0xd457, "SCPH-77005", 0, BLACK},               // 91: 27209125
    {0xd458, "SCPH-77006", 0, BLACK},               // 91: 27209118
    {0xd459, "SCPH-77007", 0, BLACK},               // 91: 27209116
    {0xd45a, "SCPH-77008", 0, BLACK},               // 91: 27209181
    {0xd45b, "SCPH-77008 SS", 0, SATIN_SILVER},     // 95: 27209581
//  {0xd45c, "??? SCPH-77010", 0, UNKNOWN},         // ??? PK9 not confirmed
    {0xd45d, "SCPH-77001 SS", 0, SATIN_SILVER},     // PL1
    {0xd45e, "SCPH-77003 PK", 0, PINK},             // 97: 27209761
    {0xd45f, "SCPH-77004 PK", 0, PINK},             // 97: 27209751
//  {0xd460, "SCPH-77010 SS", 0, SATIN_SILVER},     // PL2
    {0xd461, "SCPH-77000 SS", 0, SATIN_SILVER},     // 96: 27209601
    {0xd462, "SCPH-77000 PK", 0, PINK},             // 97: 27209701
    {0xd463, "SCPH-77008 PK", 0, PINK},             // 97: 27209781
    {0xd464, "SCPH-77002 PK", 0, PINK},             // 97: 27209791
                                                    // N-chassis
    {0xd465, "SCPH-79001", 0, BLACK},               // 240, PA0, PA9: 27224031
    {0xd466, "SCPH-79000", 0, BLACK},               // 240: 27224001
    {0xd467, "SCPH-79000 CW", 0, WHITE},            // 242: 27224201
    {0xd468, "SCPH-79002", 0, BLACK},               // 240: 27224091
    {0xd469, "SCPH-79001 SS", 0, SATIN_SILVER},     // 241: 27224131
//  {0xd46a, "??? SCPH-79005", 0, UNKNOWN},          // 240: 27224025
    {0xd46b, "SCPH-79006", 0, BLACK},               // 240: 27224018
//  {0xd46c, "??? SCPH-79007", 0, UNKNOWN},          // 240: 27224016
    {0xd46d, "SCPH-79000 SS", 0, SATIN_SILVER},     // 241: 27224101
    {0xd46e, "SCPH-79003", 0, BLACK},               // 240: 27224061
    {0xd46f, "SCPH-79004", 0, BLACK},               // 240: 27224051, 27224052
    {0xd470, "SCPH-79010", 0, BLACK},               // 240: SN0, 27224035
    {0xd471, "SCPH-79003 SS", 0, SATIN_SILVER},     // 241: 27224161
//  {0xd472, "SCPH-79004 SS", 0, SATIN_SILVER},     // 241: 27224151
    {0xd473, "SCPH-79008", 0, BLACK},               // 240: 27224081
    {0xd474, "SCPH-79001 CW", 0, WHITE},            // PW0
                                                    // P/R-chassis
    {0xd475, "SCPH-90000", 0, BLACK},               // 244, 248: 27224401, 27224801
    {0xd476, "SCPH-90000 CW", 0, WHITE},            // 245, 249: 27224501, 27224901
    {0xd477, "SCPH-90000 SS", 0, SATIN_SILVER},     // 246: 27224601        // 4990: 27499001
    {0xd478, "SCPH-90006", 0, BLACK},               // 244: 27224418
    {0xd479, "SCPH-90006 CW", 0, WHITE},            // 245: 27224518
//  {0xd47a, "SCPH-90006 SS", 0, SATIN_SILVER},     // 246: 27224618 // needs sticker confirmation
    {0xd47b, "SCPH-90005", 0, BLACK},               // 244: 27224425        // 4994: 27499475
//  {0xd47c, "SCPH-90005 CW", 0, WHITE},            // 245: 27224525
//  {0xd47d, "SCPH-90005 SS", 0, SATIN_SILVER},      // 246: 27224625
    {0xd47e, "SCPH-90007", 0, BLACK},               // 244: 27224416        // 4245: 27424581
    {0xd47f, "SCPH-90007 CW", 0, WHITE},            // 245: 27224516        // 4246: 27424681
//  {0xd480, "SCPH-90007 SS", 0, SATIN_SILVER},      // 246: 27224616        // 4247: 27424731
    {0xd481, "SCPH-90001", 0, BLACK},               // 4090: PA2, 27409054
    {0xd482, "SCPH-90001 SS", 0, SATIN_SILVER},     // 246: PA7, 27224631   // 4090, 4991: PA7, 27499104, PA8, 27409054
    {0xd483, "SCPH-90004", 0, BLACK},               // 244: 27224451        // 4993: 27499372
    {0xd484, "SCPH-90004 SS", 0, SATIN_SILVER},     // 246: 27224651
    {0xd485, "SCPH-90002", 0, BLACK},               // 244: 27224491       // 4091: 27409166
    {0xd486, "SCPH-90003", 0, BLACK},               // 244: 27224461
//  {0xd487, "SCPH-90005 CR", 0, CINNABAR_RED},     // 247: 27224725        // 4242: 27424225
    {0xd488, "SCPH-90006 CR", 0, CINNABAR_RED},     // 4244: 27424428
    {0xd489, "SCPH-90007 CR", 0, CINNABAR_RED},     // 4246: 27424631
    {0xd48a, "SCPH-90010", 0, BLACK},               // 4248: SP9, 27424810
    {0xd48b, "SCPH-90000 CR", 0, CINNABAR_RED},     // 4249: 27424901
    {0xd48c, "SCPH-90008", 0, BLACK},               // 244: 27224481        // 4247: 27424784
    {0xd48d, "SCPH-90008 SS", 0, SATIN_SILVER},     // 246: 27224681
    {0xd48f, "PX300-1", PS2TV, BLACK},
    {0xd490, "PX300-2", PS2TV, BLACK},
    {0xd491, "SCPH-90010 CR", 0, CINNABAR_RED}, // SM3
};


const console_record_t *ConsoleModel_GetDataByID(uint16_t ID)
{
    size_t records = arraysizeof(DB);

    for (size_t i = 0; i < records; i++)
    {
        if (ID == DB[i].id)
            return &DB[i];
    }
    printf("end of DB reached. unknown console 0x%04u\n", ID);
    return NULL;
}

static const char *console_colors[COLORCOUNT] = {
    "UNKNOWN",
    "Black",
    "White",
    "Silver",
    "Pink",
    "Cinnabar Red",
    "Blue",
    "Zen black",
    "Ocean Blue",
    "Midnight blue",
    "Sakura Pink",
    "Aqua Blue",
    "Gold",
    "Metal",
};

const char *ConsoleModel_GetColorName(int color)
{
    if (color >= UNKNOWN && color < COLORCOUNT)
        return console_colors[color];
    return console_colors[UNKNOWN];
}
