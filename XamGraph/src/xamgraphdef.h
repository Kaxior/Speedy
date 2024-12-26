/*  ---------------------------------------------------------------------------
 *  filename    :   xamgraphdef.h
 *  description :   Définitions communes
 *
 *	project     :	Trivial 2D Graphic Library based on SDL
 *  start date  :   février 2007
 *  ---------------------------------------------------------------------------
 *  Copyright 2007-2024 by Alain Menu   <alain.menu@ac-creteil.fr>
 *
 *  This file is part of "XamGraph"
 *
 *  This program is free software ;  you can  redistribute it and/or  modify it
 *  under the terms of the  GNU General Public License as published by the Free
 *  Software Foundation ; either version 3 of the License, or  (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY ; without even the  implied  warranty  of  MERCHANTABILITY  or
 *  FITNESS FOR  A PARTICULAR PURPOSE. See the  GNU General Public License  for
 *  more details.
 *
 *	You should have  received  a copy of the  GNU General Public License  along
 *	with this program. If not, see <http://www.gnu.org/licenses/>.
 *  ---------------------------------------------------------------------------
 */

#ifndef XAMGRAPHDEF_H
#define XAMGRAPHDEF_H

#include "../_ABOUT"

// modes d'affichage
// ----------------------------------------------------------------------------

// SDL2
//#define	XAM_DEPTH			24
#define	XAM_DEPTH			32

#define	XAM_DRAWING_SCREEN	0
#define	XAM_DRAWING_BITMAP	1

#define	XAM_MODE_COPY		0
#define	XAM_MODE_XOR		1

// couleurs predefinies
// ----------------------------------------------------------------------------

//#define	XAM_COLOR24(R,G,B)	(int)( (R) * 65536 + (G) * 256 + (int)(B) )
//#define	XAM_R(V)			( ( (V) & 0x00FF0000 ) >> 16 )
//#define	XAM_G(V)			( ( (V) & 0x0000FF00 ) >> 8 )
//#define	XAM_B(V)			( ( (V) & 0x000000FF ) )

#define	XAM_COLOR24(R,G,B)	(Uint32)( (Uint8)(R) << 24 | (Uint8)(G) << 16 | (Uint8)(B) << 8 | 0xFF )
#define	XAM_COLOR32(R,G,B,A)	(Uint32)( (Uint8)(R) << 24 | (Uint8)(G) << 16 | (Uint8)(B) << 8 | (Uint8)(A) )
#define	XAM_R(V)			(Uint8)( ( (V) & 0xFF000000 ) >> 24 )
#define	XAM_G(V)			(Uint8)( ( (V) & 0x00FF0000 ) >> 16 )
#define	XAM_B(V)			(Uint8)( ( (V) & 0x0000FF00 ) >> 8 )
#define	XAM_A(V)			(Uint8)( ( (V) & 0x000000FF ) )


#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define XAM_RMASK			(Uint32)(0xFF000000)
#define XAM_GMASK			(Uint32)(0x00FF0000)
#define XAM_BMASK			(Uint32)(0x0000FF00)
#define XAM_AMASK			(Uint32)(0x000000FF)
//#define	XAM_COLOR24(R,G,B)	(Uint32)( (Uint8)(R) << 24 | (Uint8)(G) << 16 | (Uint8)(B) << 8 | 0xFF )
//#define	XAM_R(V)			(Uint8)( ( (V) & 0xFF000000 ) >> 24 )
//#define	XAM_G(V)			(Uint8)( ( (V) & 0x00FF0000 ) >> 16 )
//#define	XAM_B(V)			(Uint8)( ( (V) & 0x0000FF00 ) >> 8 )
//#define	XAM_A(V)			(Uint8)( ( (V) & 0x000000FF ) )
#else
#define XAM_AMASK			(Uint32)(0xFF000000)
#define XAM_BMASK			(Uint32)(0x00FF0000)
#define XAM_GMASK			(Uint32)(0x0000FF00)
#define XAM_RMASK			(Uint32)(0x000000FF)
//#define	XAM_COLOR24(R,G,B)	(Uint32)( 0xFF000000 | (Uint8)(B) << 16 | (Uint8)(G) << 8 | (Uint8)(R) )
//#define	XAM_A(V)			(Uint8)( ( (V) & 0xFF000000 ) >> 24 )
//#define	XAM_B(V)			(Uint8)( ( (V) & 0x00FF0000 ) >> 16 )
//#define	XAM_G(V)			(Uint8)( ( (V) & 0x0000FF00 ) >> 8 )
//#define	XAM_R(V)			(Uint8)( ( (V) & 0x000000FF ) )
#endif

#define	XAM_WHITE			XAM_COLOR24( 255, 255, 255 )
#define	XAM_BLACK			XAM_COLOR24(   0,   0,   0 )
#define	XAM_RED				XAM_COLOR24( 255,   0,   0 )
#define	XAM_GREEN			XAM_COLOR24(   0, 255,   0 )
#define	XAM_BLUE			XAM_COLOR24(   0,   0, 255 )
#define	XAM_MAGENTA			XAM_COLOR24( 255,   0, 255 )
#define	XAM_CYAN			XAM_COLOR24(   0, 255, 255 )
#define	XAM_YELLOW			XAM_COLOR24( 255, 255,   0 )

#define	XAM_GRAY			XAM_COLOR24( 190, 190, 190 )
#define	XAM_LIGHT_GRAY		XAM_COLOR24( 211, 211, 211 )
#define	XAM_DARK_GRAY		XAM_COLOR24( 105, 105, 105 )

#define	XAM_DARK_RED		XAM_COLOR24( 139,   0,   0 )
#define	XAM_DARK_GREEN		XAM_COLOR24(   0, 139,   0 )
#define	XAM_DARK_BLUE		XAM_COLOR24(   0,   0, 139 )
#define	XAM_LIGHT_RED		XAM_COLOR24( 255,  99,  71 )
#define	XAM_LIGHT_GREEN		XAM_COLOR24( 144, 238, 144 )
#define	XAM_LIGHT_BLUE		XAM_COLOR24( 173, 216, 255 )

#define	XAM_DARK_MAGENTA	XAM_COLOR24( 139,   0, 129 )
#define	XAM_DARK_CYAN		XAM_COLOR24(   0, 139, 139 )
#define	XAM_DARK_YELLOW		XAM_COLOR24( 139, 139,   0 )

#define	XAM_ORANGE			XAM_COLOR24( 255, 140,   0 )
#define	XAM_PINK			XAM_COLOR24( 255, 105, 180 )
#define	XAM_VIOLET			XAM_COLOR24( 238, 130, 238 )
#define	XAM_GOLD			XAM_COLOR24( 255, 215,   0 )
#define	XAM_BROWN			XAM_COLOR24( 184, 134,  11 )
#define	XAM_IVORY			XAM_COLOR24( 255, 255, 240 )
#define	XAM_LAVENDER		XAM_COLOR24( 230, 230, 250 )

// style de ligne
// ----------------------------------------------------------------------------

#define	XAM_MAX_THICKNESS		10
#define	XAM_SOLID_LINE			0
#define	XAM_DASHED_LINE			15
#define	XAM_DOTTED_LINE			5

// style de texte
// ----------------------------------------------------------------------------

#define	XAM_ALIGNMENT_LEFT		1
#define	XAM_ALIGNMENT_HCENTER	2
#define	XAM_ALIGNMENT_RIGHT		4
#define	XAM_ALIGNMENT_TOP		8
#define	XAM_ALIGNMENT_VCENTER	16
#define	XAM_ALIGNMENT_BOTTOM	32
#define	XAM_ALIGNMENT_DEFAULT	( XAM_ALIGNMENT_LEFT | XAM_ALIGNMENT_TOP )
#define	XAM_ALIGNMENT_CENTER	( XAM_ALIGNMENT_HCENTER | XAM_ALIGNMENT_VCENTER )

#define	XAM_NORMAL_TEXT			0
#define	XAM_BOLD_TEXT			1
#define	XAM_ITALIC_TEXT			2

// souris
// ----------------------------------------------------------------------------

#define BUTTON_LEFT		1
#define BUTTON_MIDDLE	2
#define BUTTON_RIGHT	3

// types
// ----------------------------------------------------------------------------

#ifndef XAMGRAPHCLASS

typedef enum { false, true } bool ;

#endif

typedef void (*XamKeyboardHandler)(int,int) ;
typedef void (*XamMouseHandler)(int,int) ;
typedef void (*XamMouseButtonHandler)(int,int,int) ;
typedef void (*XamTimerHandler)() ;

#endif
