/*
 * PrBoomX: PrBoomX is a fork of PrBoom-Plus with quality-of-play upgrades. 
 *
 * Copyright (C) 2023  JadingTsunami
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __C_CMD__
#define __C_CMD__

#include "doomtype.h"
#include "d_event.h"
#include "hu_stuff.h"

#define CONSOLE_LINE_LENGTH_MAX (HU_MAXLINELENGTH)

void C_ConsoleCommand(char* cmd);
const char* C_NavigateCommandHistory(int direction);
void C_ResetCommandHistoryPosition();
void C_ConsolePrintf(const char *s, ...);
const char* C_GetMessage();
void C_ClearMessage();
dboolean C_HasMessage();
dboolean C_Responder();
dboolean C_RegisterBind(int keycode, char* cmd, evtype_t type);
dboolean C_UnregisterBind(int keycode, evtype_t type);
void C_SaveSettings();
void C_LoadSettings();
const char* C_CommandComplete(const char* partial);
void C_Ticker();

typedef struct _command {
    const char* name;
    void (*func)(char*);
} command;

void C_SchedulePlayerMessage(char* msg);

#endif
