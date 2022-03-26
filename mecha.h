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

int MechaInit();
void MechaDeinit();
char getMechaVersion(uint8_t *data);
char getMechaBuildDate(uint8_t *data);
char OpenConfig(uint8_t id, char write, uint8_t blocks);
char ReadConfig(uint8_t *data);
char WriteConfig(const uint8_t *data);
char CloseConfig();
char ReadNVM(uint16_t offset, uint16_t *data);
char WriteNVM(uint16_t offset, uint16_t data);
char getModelName(char *model_number[18])
char IsNVMUnlocked();
