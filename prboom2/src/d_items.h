/* Emacs style mode select   -*- C++ -*-
 *-----------------------------------------------------------------------------
 *
 *
 *  PrBoom: a Doom port merged with LxDoom and LSDLDoom
 *  based on BOOM, a modified and improved DOOM engine
 *  Copyright (C) 1999 by
 *  id Software, Chi Hoang, Lee Killough, Jim Flynn, Rand Phares, Ty Halderman
 *  Copyright (C) 1999-2000 by
 *  Jess Haas, Nicolas Kalkhof, Colin Phipps, Florian Schulze
 *  Copyright 2005, 2006 by
 *  Florian Schulze, Colin Phipps, Neil Stevens, Andrey Budko
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 *  02111-1307, USA.
 *
 * DESCRIPTION:
 *  Items: key cards, artifacts, weapon, ammunition.
 *
 *-----------------------------------------------------------------------------*/


#ifndef __D_ITEMS__
#define __D_ITEMS__

#include "doomdef.h"

#ifdef __GNUG__
#pragma interface
#endif

// jds - mbf21 weapon flags
#define WF_NOTHRUST           0x001
#define WF_SILENT             0x002
#define WF_NOAUTOFIRE         0x004
#define WF_FLEEMELEE          0x008
#define WF_AUTOSWITCHFROM     0x010
#define WF_NOAUTOSWITCHTO     0x020

// jds - mbf21 ammo per shot must be non-negative,
// so -1 is a safe "invalid" default value to use
#define WP_DEFAULT_AMMO_PER_SHOT (-1)

/* Weapon info: sprite frames, ammunition use. */
typedef struct
{
  ammotype_t  ammo;
  int         upstate;
  int         downstate;
  int         readystate;
  int         atkstate;
  int         flashstate;
  int         mbf21weaponflags;
  int         ammopershot;
} weaponinfo_t;

extern  weaponinfo_t    weaponinfo[NUMWEAPONS+2];
extern int ammopershot[NUMWEAPONS+2];

#endif
