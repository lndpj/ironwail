/*
Copyright (C) 1996-2001 Id Software, Inc.
Copyright (C) 2002-2009 John Fitzgibbons and others
Copyright (C) 2007-2008 Kristian Duske
Copyright (C) 2010-2014 QuakeSpasm developers

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#ifndef __VID_DEFS_H
#define __VID_DEFS_H

// vid.h -- video driver defs

#define VID_CBITS	6
#define VID_GRADES	(1 << VID_CBITS)

#define GAMMA_MAX	3.0

// moved here for global use -- kristian
typedef enum { MS_UNINIT, MS_WINDOWED, MS_FULLSCREEN } modestate_t;

extern modestate_t modestate;

// a pixel can be one, two, or four bytes
typedef byte pixel_t;

typedef struct vrect_s
{
	int32_t x;
	int32_t y;
	int32_t width;
	int32_t height;
} vrect_t;

typedef struct
{
	pixel_t	*colormap;       // 256 * VID_GRADES size
	int32_t  fullbright;     // index of first fullbright color
	int32_t  rowbytes;       // may be > width if displayed in a window
	int32_t  width;
	int32_t  height;
	qboolean resized;
	int32_t  maxscale;       // maximum r_scale value, based on height
	int32_t  refreshrate;
	int32_t  numpages;
	int32_t  recalc_refdef;	 // if true, recalc vid-based stuff
	int32_t  conrowbytes;
	int32_t  conwidth;
	int32_t  conheight;
	float    guipixelaspect; // 2D pixel aspect ratio (1 = square)
	int32_t  guiwidth;       // 2D width
	int32_t  guiheight;      // 2D height
} viddef_t;

extern	viddef_t vid;     // global video state

typedef struct
{
	int32_t width;
	int32_t height;
	int32_t refreshrate;
	int32_t bpp;
} vmode_t;

#define MAX_MODE_LIST	600 //johnfitz -- was 30

extern	vmode_t	modelist[MAX_MODE_LIST];
extern	int     nummodes;

void	VID_Init (void); //johnfitz -- removed palette from argument list
void	VID_Shutdown (void); // Called at shutdown

void	VID_SyncCvars (void);
void	VID_Toggle (void);

typedef enum
{
	MOUSECURSOR_DEFAULT,
	MOUSECURSOR_HAND,
	MOUSECURSOR_IBEAM,
} mousecursor_t;

void		*VID_GetWindow (void);
qboolean	VID_HasMouseOrInputFocus (void);
qboolean	VID_IsMinimized (void);
void		VID_Lock (void);
void		VID_SetWindowTitle (const char *title);
void		VID_SetMouseCursor (mousecursor_t cursor);
void		VID_RecalcConsoleSize (void);
void		VID_RecalcInterfaceSize (void);

#endif	/* __VID_DEFS_H */

