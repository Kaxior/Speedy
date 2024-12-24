/*  ---------------------------------------------------------------------------
 *  filename    :   xamgraphlib.h
 *  description :   C library header
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

#ifndef XAMGRAPHLIB_H
#define XAMGRAPHLIB_H

#ifdef __cplusplus
extern "C" {
#endif

// interface publique
// ----------------------------------------------------------------------------

void 	xamVersion() ;

// gestion de la fenêtre graphique

void	xamCreateGraph( int width, int height, const char* title ) ;
int		xamCloseGraph( void ) ;

void    xamSetTitle( const char* title ) ;
void	xamSetWindowPosition(int x, int y ) ;
void    xamResize(int width, int height ) ;
int		xamMaxX( void ) ;
int		xamMaxY( void ) ;

void	xamSetDrawingArea( int drawing ) ;
int		xamDrawingArea( void ) ;
void	xamClearScreen( Uint32 color ) ;
void	xamUpdateScreen( void ) ;
void	xamSaveScreen( void ) ;
void	xamRestoreScreen( void ) ;
void	xamSetWriteMode( int mode ) ;
int     xamWriteMode( void ) ;

// gestion des couleurs

Uint32	xamRgb( Uint8 r, Uint8 g, Uint8 b ) ;
Uint32	xamRgba( Uint8 r, Uint8 g, Uint8 b, Uint8 a ) ;
Uint8	xamRed( Uint32 color ) ;
Uint8 	xamGreen( Uint32 color ) ;
Uint8 	xamBlue( Uint32 color ) ;
Uint8 	xamAlpha( Uint32 color ) ;
Uint32	xamStringToColor( const char* str ) ;

void	xamSetColor( Uint32 color ) ;              // foreground color
Uint32	xamColor( void ) ;
Uint32	xamBackgroundColor( void ) ;               // background color

// primitives de dessin

void	xamPutPixel( int x, int y, Uint32 color ) ;
Uint32	xamGetPixel( int x, int y ) ;

int		xamCurrentX( void ) ;
int		xamCurrentY( void ) ;
void	xamMoveRel( int dx, int dy ) ;
void	xamMoveTo( int x, int y ) ;

void	xamPoint(int x, int y ) ;

void	xamLine( int x1, int y1, int x2, int y2 ) ;
void	xamLineRel( int dx, int dy ) ;
void	xamLineTo( int x, int y ) ;

void	xamPolygon( int numPoints, int* points ) ;
void	xamFilledPolygon( int numPoints, int* points ) ;

void	xamRegularPolygon( int x, int y, int radius, int faces, int angle ) ;
void	xamFilledRegularPolygon( int x, int y, int radius, int faces, int angle ) ;

void 	xamRectangle( int x, int y, int w, int h ) ;
void 	xamFilledRectangle( int x, int y, int w, int h ) ;

void 	xamCenteredRectangle( int x, int y, int w, int h ) ;
void 	xamFilledCenteredRectangle( int x, int y, int w, int h ) ;

void	xamCircle( int x, int y, int radius ) ;
void	xamFilledCircle( int x, int y, int radius ) ;

void	xamEllipse( int x, int y, int xRadius, int yRadius ) ;
void	xamFilledEllipse( int x, int y, int xRadius, int yRadius ) ;

void 	xamFloodFill( int x, int y, Uint32 color ) ;

// styles pour points, lignes et polygones

void	xamSetLineThickness( int thickness ) ;
int 	xamLineThickness( void ) ;

void	xamSetLineStyle( int style ) ;
int 	xamLineStyle( void ) ;

// affichage de textes

bool 	xamLoadFont(const char* fileName, int size ) ;
bool	xamDefaultFont( void ) ;
void 	xamTextOut( int x, int y, const char* text ) ;
int 	xamPrintf( int x, int y, const char* format, ... ) ;
void 	xamSetTextAlignment( int alignment ) ;
void 	xamSetTextStyle(int style ) ;
int 	xamTextHeight( void ) ;
int 	xamTextWidth( const char* text ) ;
// chargement d'images

bool	xamLoadImage( int x, int y, const char* fileName ) ;

// boucle d'événements clavier/souris

void	xamConnectKeyDownEvent( XamKeyboardHandler handler ) ;
void	xamConnectKeyUpEvent( XamKeyboardHandler handler ) ;
void	xamConnectMouseMotionEvent( XamMouseHandler handler ) ;
void	xamConnectMouseButtonDownEvent( XamMouseButtonHandler handler ) ;
void	xamConnectMouseButtonUpEvent( XamMouseButtonHandler handler ) ;
void	xamRun( void ) ;
void	xamQuit( void ) ;

// gestion du temps

void	xamConnectTimerEvent( XamTimerHandler handler, int period ) ;
void 	xamKillTimer( void ) ;
void	xamDelay( int msec ) ;

#ifdef __cplusplus
}
#endif

#endif
