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

#ifndef __C_SCORE__
#define __C_SCORE__

#include "doomtype.h"

#define SCORE_MSG_SIZE (256)

typedef enum {
    SCORE_EVT_NONE = 0,
    SCORE_EVT_ENEMY_DAMAGED,
    SCORE_EVT_ZOMBIE_DAMAGED,
    SCORE_EVT_PLAYER_DAMAGED,
    SCORE_EVT_ITEM_GOT,
    SCORE_EVT_SECRET_FOUND,
    SCORE_EVT_STREAK_TIMEOUT,
    SCORE_EVT_LEVEL_DONE,
    SCORE_EVT_LAST
} g_score_event_t;

typedef enum {
    SCORE_CFG_NONE = 0,
    SCORE_CFG_TIMEOUT,
    SCORE_CFG_MIN_BREAK,
    SCORE_CFG_SECRET_POINTS,
    SCORE_CFG_SECRET_STREAK_EXTENSION,
    SCORE_CFG_LAST
} g_score_config_t;

void G_ScoreTicker();
void G_ScoreInit();
void G_ScoreReset(dboolean clear_total);

dboolean G_ShouldKeepScore();
long long int G_GetLevelScore();
long long int G_GetTotalScore();
int G_GetStreakTimeLeft();
void G_RegisterScoreEvent(g_score_event_t event, int arg);
const char* G_GetScoreMessage();
int G_GetScoreColor();

#endif
