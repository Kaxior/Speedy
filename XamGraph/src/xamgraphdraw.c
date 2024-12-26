/*  ---------------------------------------------------------------------------
 *  filename    :   xamgraphdraw.c
 *  description :   iMPLEMENTATION C/C++
 *
 *	project     :	Trivial 2D Graphic Library based on SDL
 *  start date  :   février 2007
 *  ---------------------------------------------------------------------------
 *  Copyright 2007-2017 by Alain Menu   <alain.menu@ac-creteil.fr>
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

#include "xamgraph.h"

// references externes (C)
// ----------------------------------------------------------------------------

#ifndef	XAMGRAPHCLASS

extern	SDL_Surface*	m_screen ;				// current video surface
extern	SDL_Surface*	m_transparentSurf ;		// temporary transparent layer
extern	Uint32			m_transparentColor ;	// SDL transparent color for temporary layer
extern	Uint32			m_drawingColor ;		// SDL current foreground color for active surface

extern	int				m_width ;				// largeur utile
extern	int				m_height ;				// hauteur utile

extern	int				m_x ;					// abscisse courante de dessin
extern	int				m_y ;					// ordonnee courante de dessin

extern	int				m_writeMode ;			// mode d'affichage ( copy | xor )

extern	Uint32			m_fgColor ;				// couleur courante d'avant-plan

extern	int				m_thickness ;			// epaisseur de trace des lignes
extern	int				m_style ;				// type de trace des lignes

#endif

// prototypes des ressources privées (C)
// ----------------------------------------------------------------------------

#ifndef XAMGRAPHCLASS

int*	_allocate(int n ) ;
void 	_free(int* p ) ;

SDL_Surface* _createTransparentLayer(int w, int h ) ;
void 	_blitAndFreeTransparentLayer(int x, int y ) ;

int 	_lock(SDL_Surface* surf ) ;
void 	_unlock(SDL_Surface* surf ) ;

Uint32 	_getPixel(SDL_Surface* surf, int x, int y ) ;
void 	_putPixel(SDL_Surface* surf, int x, int y, Uint32 c ) ;

void 	_floodFill(SDL_Surface* surf, int x, int y, Uint32 oldc, Uint32 newc ) ;

SDL_Rect _getPolygonRect(int n, int* p ) ;

void 	_drawPoint(SDL_Surface* surf, int x, int y, Uint32 c ) ;
void 	_drawLine(SDL_Surface* surf, int x1, int y1, int x2, int y2, Uint32 c ) ;
void 	_drawPolygon(SDL_Surface* surf, int n, int* p, Uint32 c, bool filled ) ;
void 	_drawRectangle(SDL_Surface* surf, int x, int y, int w, int h, Uint32 c, bool filled ) ;
void 	_drawCircle(SDL_Surface* surf, int x, int y, int r, Uint32 c, bool filled ) ;
void 	_drawEllipse(SDL_Surface* surf, int x, int y, int a, int b, Uint32 c, bool filled ) ;

void	_log(const char* mess ) ;
bool	_isInit( void ) ;

#endif

// ----------------------------------------------------------------------------
// allocation dynamique de mémoire pour tableaux de points (x,y)
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
int* XamGraph::_allocate(int n )
{
	return new int[ n ] ;
#else
int* _allocate(int n )
{
	return (int*)malloc( n * sizeof(int) ) ;
#endif
}

#ifdef	XAMGRAPHCLASS
void XamGraph::_free(int* p )
{
	delete [] p ;
#else
void _free(int* p )
{
	free( p ) ;
#endif
}

// ----------------------------------------------------------------------------
// Transparent layer
// ----------------------------------------------------------------------------

// création d'un calque apte à recevoir un dessin de dimensions (w,h)
// fixe les couleurs de dessin et de transparence à partir de m_fgColor

#ifdef	XAMGRAPHCLASS
SDL_Surface* XamGraph::_createTransparentLayer(int w, int h )
{
#else
SDL_Surface* _createTransparentLayer(int w, int h )
{
#endif
	w += XAM_MAX_THICKNESS ;	//// tenir compte de thickness !
	h += XAM_MAX_THICKNESS ;

	m_transparentSurf = SDL_CreateRGBSurface( SDL_SWSURFACE, w, h, XAM_DEPTH, XAM_RMASK, XAM_GMASK, XAM_BMASK, XAM_AMASK ) ;

	// remplissage avec complément de la couleur courante

	Uint8	r = XAM_R(m_fgColor) ;
	Uint8	g = XAM_G(m_fgColor) ;
	Uint8	b = XAM_B(m_fgColor) ;

	m_transparentColor = SDL_MapRGB( m_transparentSurf->format, ~r, ~g, ~b ) ;

	SDL_FillRect(m_transparentSurf, NULL, m_transparentColor ) ;

	return m_transparentSurf ;
}

// projection du calque sur la surface SDL courante (m_screen), CSG (x,y)
// libère le calque courant

#ifdef	XAMGRAPHCLASS
void XamGraph::_blitAndFreeTransparentLayer(int x, int y )
{
#else
void _blitAndFreeTransparentLayer(int x, int y )
{
#endif
	int i, j ;

	if ( _lock( m_screen ) == 0 ) {
		for ( j = 0 ; j < m_transparentSurf->h ; ++j ) {
			for ( i = 0 ; i < m_transparentSurf->w ; ++i ) {
				Uint32 c = _getPixel(m_transparentSurf, i, j ) ;
				if ( c != m_transparentColor ) {
					_putPixel(m_screen, x - XAM_MAX_THICKNESS / 2 + i, y - XAM_MAX_THICKNESS / 2 + j, c ) ;
				}
			}
		}
		_unlock(m_screen) ;
	}

	SDL_FreeSurface( m_transparentSurf ) ;
}

// ----------------------------------------------------------------------------
// [dé]verrouillage d'une surface de dessin
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
int XamGraph::_lock(SDL_Surface* surf )
{
#else
int _lock(SDL_Surface* surf )
{
	if ( !_isInit() )	return -1 ;
#endif
	if ( SDL_MUSTLOCK(surf) )	return SDL_LockSurface(surf) ;
	return 0 ;
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::_unlock(SDL_Surface* surf )
{
#else
void _unlock(SDL_Surface* surf )
{
	if ( !_isInit() )	return ;
#endif
	if ( SDL_MUSTLOCK(surf) )	SDL_UnlockSurface(surf) ;
}

// ----------------------------------------------------------------------------
// lecture du pixel de coordonnées (x,y) --> couleur 0xRRGGBBAA (bigEndian)
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
Uint32 XamGraph::_getPixel(SDL_Surface* surf, int x, int y )		// private
{
#else
Uint32 _getPixel(SDL_Surface* surf, int x, int y )
{
	if ( !_isInit() )	return 0 ;
#endif
	if (( x < surf->clip_rect.x )||( x > surf->clip_rect.x + surf->clip_rect.w - 1 ))	return 0 ;
	if (( y < surf->clip_rect.y )||( y > surf->clip_rect.y + surf->clip_rect.h - 1 ))	return 0 ;

	int		bpp = surf->format->BytesPerPixel ;
	Uint8* 	p = (Uint8*)surf->pixels + y * surf->pitch + x * bpp ;

//	assert( bpp == 4 ) ;

	return *(Uint32*)p ;	// big or LittleEndian...
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
Uint32 XamGraph::getPixel( int x, int y )
{
#else
Uint32 xamGetPixel( int x, int y )
{
#endif
	Uint32 c = 0 ;
	Uint8 r, g, b, a ;

	if ( !_lock( m_screen ) ) {
		c = _getPixel(m_screen, x, y ) ;	// big or LittleEndian...
		_unlock( m_screen ) ;
		SDL_GetRGBA( c, m_screen->format, &r, &g, &b, &a ) ;
		c = ( r << 24 ) | ( g << 16 ) | ( b << 8 ) | a ;
	}
	return c ;
}

// ----------------------------------------------------------------------------
// écriture du pixel de coordonnées (x,y), couleur codée 0xRRGGBBAA
// avec prise en compte du mode d'écriture COPY | XOR
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::_putPixel(SDL_Surface* surf, int x, int y, Uint32 c ) // private
{
#else
void _putPixel(SDL_Surface* surf, int x, int y, Uint32 c )
{
	if ( !_isInit() )	return ;
#endif
	if (( x < surf->clip_rect.x )||( x > surf->clip_rect.x + surf->clip_rect.w - 1 ))	return ;
	if (( y < surf->clip_rect.y )||( y > surf->clip_rect.y + surf->clip_rect.h - 1 ))	return ;

	int		bpp = surf->format->BytesPerPixel ;
	Uint8* 	p = (Uint8*)surf->pixels + y * surf->pitch + x * bpp ;

//	assert( bpp == 4 ) ;

	// prise en compte du mode XOR (sauf sur m_transparentSurf)

	if (( m_writeMode == XAM_MODE_XOR )&&( surf != m_transparentSurf )) {
		c ^= *(Uint32*)p ;
		c |= 0xFF000000 ;
	}

	*(Uint32*)p = c ;
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void	XamGraph::putPixel( int x, int y, Uint32 color )
{
#else
void	xamPutPixel( int x, int y, Uint32 color )
{
#endif
	Uint32 c = SDL_MapRGBA( m_screen->format, XAM_R(color), XAM_G(color), XAM_B(color), XAM_A(color) ) ;

	if ( !_lock( m_screen ) ) {
		_putPixel(m_screen, x, y, c ) ;
		_unlock( m_screen ) ;
	}
}

// ----------------------------------------------------------------------------
// position courante de dessin, pour move...() et line...()
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
int	XamGraph::currentX() const
{
#else
int	xamCurrentX( void )
{
#endif
	return m_x ;
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
int	XamGraph::currentY() const
{
#else
int	xamCurrentY( void )
{
#endif
	return m_y ;
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::moveRel( int dx, int dy )
{
#else
void xamMoveRel( int dx, int dy )
{
#endif
	m_x += dx ;
	m_y += dy ;
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::moveTo( int x, int y )
{
#else
void xamMoveTo( int x, int y )
{
#endif
	m_x = x ;
	m_y = y ;
}

// ----------------------------------------------------------------------------
// dessin d'un point aux coordonnées (x,y)
// aspect : couleur courante 'm_fgColor' et style 'm_thickness'
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::_drawPoint(SDL_Surface* surf, int x, int y, Uint32 c )	// private
{
#else
void _drawPoint(SDL_Surface* surf, int x, int y, Uint32 c )
{
#endif
	if ( m_thickness <= 1 )	_putPixel(surf, x, y, c ) ;
	else					_drawRectangle(surf, x, y, m_thickness, m_thickness, c, true ) ;
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void	XamGraph::point( int x, int y )
{
#else
void	xamPoint( int x, int y )
{
#endif
	if ( !_lock( m_screen ) ) {
		_drawPoint(m_screen, x, y, m_drawingColor ) ;
		_unlock( m_screen ) ;
	}
}

// ----------------------------------------------------------------------------
// dessin d'une ligne
// aspect : couleur courante 'm_fgColor', styles 'm_thickness' et 'm_style'
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::_drawLine(SDL_Surface* surf, int x1, int y1, int x2, int y2, Uint32 c )	// private
{
#else
void _drawLine(SDL_Surface* surf, int x1, int y1, int x2, int y2, Uint32 c )
{
#endif
	int 	dx, dy ;
	int 	sx, sy ;
	int 	x = 0 ;
	int		y = 0 ;
	int		a = 0 ;

	dx = x2 - x1 ;
	dy = y2 - y1 ;
	sx = ( dx >= 0 ? 1 : -1 ) ;
	sy = ( dy >= 0 ? 1 : -1 ) ;
	dx = sx * dx + 1 ;
	dy = sy * dy + 1 ;

	// m_style est pris en compte ici,
	// _drawPoint() gère m_thickness et la couleur ...

	if ( dx >= dy ) {
		for ( ; x < dx ; ++x ) {
			if (( m_style == XAM_SOLID_LINE )||( ( x  % m_style ) < ( m_style / 2 ) ))
				_drawPoint(surf, x1 + x * sx, y1 + y * sy, c ) ;
			a += dy ;
			if ( a >= dx ) { a -= dx ; y++ ; }
		}
	}
	else {
		for ( ; y < dy ; ++y ) {
			if (( m_style == XAM_SOLID_LINE )||( ( y  % m_style ) < ( m_style / 2 ) ))
			_drawPoint(surf, x1 + x * sx, y1 + y * sy, c ) ;
			a += dx ;
			if ( a >= dy ) { a -= dy ; x++ ; }
		}
	}
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void	XamGraph::line( int x1, int y1, int x2, int y2 )
{
#else
void	xamLine( int x1, int y1, int x2, int y2 )
{
#endif
	if ( !_lock( m_screen ) ) {
		_drawLine(m_screen, x1, y1, x2, y2, m_drawingColor ) ;
		_unlock( m_screen ) ;
	}
	m_x = x2 ;
	m_y = y2 ;
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void	XamGraph::lineRel( int dx, int dy )
{
#else
void	xamLineRel( int dx, int dy )
{
#endif
	if ( !_lock( m_screen ) ) {
		_drawLine(m_screen, m_x, m_y, m_x + dx, m_y + dy, m_drawingColor ) ;
		_unlock( m_screen ) ;
	}
	m_x += dx ;
	m_y += dy ;
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void	XamGraph::lineTo( int x, int y )
{
#else
void	xamLineTo( int x, int y )
{
#endif
	if ( !_lock( m_screen ) ) {
		_drawLine(m_screen, m_x, m_y, x, y, m_drawingColor ) ;
		_unlock( m_screen ) ;
	}
	m_x = x ;
	m_y = y ;
}

// ----------------------------------------------------------------------------
// dessin d'un polygone, avec/sans remplissage
// ----------------------------------------------------------------------------

// recherche du rectangle d'inscription

#ifdef	XAMGRAPHCLASS
SDL_Rect XamGraph::_getPolygonRect(int n, int* p )				// private
{
#else
SDL_Rect _getPolygonRect(int n, int* p )
{
#endif
	int 		i, xmin, xmax, ymin, ymax ;
	SDL_Rect	rect ;

	xmin = xmax = *p ;
	ymin = ymax = *( p + 1 ) ;

	for ( i = 0 ; i < n ; ++i ) {
		int x = *( p + i * 2 ) ;
		int y = *( p + i * 2 + 1 ) ;
		if ( x < xmin )	xmin = x ;
		if ( x > xmax )	xmax = x ;
		if ( y < ymin )	ymin = y ;
		if ( y > ymax ) ymax = y ;
	}
	rect.x = xmin ;
	rect.y = ymin ;
	rect.w = xmax - xmin + 1 ;
	rect.h = ymax - ymin + 1 ;

	return rect ;
}

// dessin du polygone, doit toujours recevoir un contour fermé
// si contour uniquement (sans remplissage), tient compte des styles de ligne
// 'm_thickness' et 'm_style'

#ifdef	XAMGRAPHCLASS
void XamGraph::_drawPolygon(SDL_Surface* surf, int n, int* p, Uint32 c, bool filled )	// private
{
#else
void _drawPolygon(SDL_Surface* surf, int n, int* p, Uint32 c, bool filled )
{
#endif
	int i, j, k ;

	if ( !filled ) {
		for ( i = 0 ; i < ( n - 1 ) ; ++i ) {
			int* p1 = p + i * 2 ;
			int* p2 = p + ( i + 1 ) * 2 ;
			_drawLine(surf, *p1, *(p1 + 1), *p2, *(p2 + 1), c ) ;
		}
		return ;
	}

#define POLX(i)	((float)(p[ i * 2 ] - rect.x + 1 ))
#define	POLY(i) ((float)(p[ i * 2 + 1 ] - rect.y ))

	int numNodes ;
	int numCorners ;
	int nodes[100] ;	// max 100 nodes par ligne ?????

	SDL_Rect	rect = _getPolygonRect(n, p ) ;

	for ( j = 0 ; j < rect.h ; ++j ) {	// pour chaque ligne...

		// construction de la liste des points d'intersection (nodes) avec le polygone

		numNodes = 0 ;
		numCorners = n - 2 ;

		for ( i = 0 ; i < n - 1 ; ++i ) {
			if ( ( POLY(i) < (double)j && POLY(numCorners) >= (double)j )
			  || ( POLY(numCorners) < (double)j && POLY(i) >= (double)j ) ) {

				nodes[numNodes++] = (int)(
					POLX(i) + ( j - POLY(i) )
				/ ( POLY(numCorners) - POLY(i) )
				* ( POLX(numCorners) - POLX(i) ) ) ;
			}
			numCorners = i ;
		}

		// tri à bulles de la liste

		i = 0 ;
		while ( i < numNodes - 1 ) {
			if ( nodes[i] > nodes[i+1] ) {
				int swap = nodes[i] ;
				nodes[i] = nodes[i+1] ;
				nodes[i+1] = swap ;
				if ( i ) i-- ;
			}
			else {
				i++ ;
			}
		}

		// remplissage entre les paires de nodes

		for ( i = 0 ; i < numNodes ; i += 2 ) {
			for ( k = nodes[i] ; k <= nodes[i+1] ; ++k )
				_putPixel(surf, k + rect.x, j + rect.y, c ) ;
		}
	}
}

// polygone défini par liste de sommets
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::polygon( int numPoints, int* points )
#else
void xamPolygon( int numPoints, int* points )
#endif
{
	int		i ;
	int*	p = NULL ;

	p = _allocate( 2 * ( numPoints + 1 ) ) ;

	for ( i = 0 ; i < numPoints ; i++ ) {
		*(p + i * 2) = m_x = points[ i * 2 ] ;
		*(p + i * 2 + 1) = m_y = points[ i * 2 + 1 ] ;
	}
	// fermeture du contour
	if (( m_x != points[0] )||( m_y != points[1] )) {
		*(p + i * 2) = m_x = points[ 0 ] ;
		*(p + i * 2 + 1) = m_y = points[ 1 ] ;
		numPoints++ ;
	}

	if ( !_lock( m_screen ) ) {
		_drawPolygon(m_screen, numPoints, p, m_drawingColor, false ) ;
		_unlock( m_screen ) ;
	}

	_free(p) ;
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::filledPolygon( int numPoints, int* points )
#else
void xamFilledPolygon( int numPoints, int* points )
#endif
{
	int		i ;
	int*	p = NULL ;

	p = _allocate( 2 * ( numPoints + 1 ) ) ;

	for ( i = 0 ; i < numPoints ; i++ ) {
		*(p + i * 2) = m_x = points[ i * 2 ] ;
		*(p + i * 2 + 1) = m_y = points[ i * 2 + 1 ] ;
	}
	if (( m_x != points[0] )||( m_y != points[1] )) {
		*(p + i * 2) = m_x = points[ 0 ] ;
		*(p + i * 2 + 1) = m_y = points[ 1 ] ;
		numPoints++ ;
	}

	if ( !_lock( m_screen ) ) {
		_drawPolygon(m_screen, numPoints, p, m_drawingColor, true ) ;
		_unlock( m_screen ) ;
	}

	_free(p) ;
}

// polygone régulier (min. 3 faces)
// centre, rayon ext., nombre de faces, angle trigo (en degrés) du 1er point
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::regularPolygon( int x, int y, int radius, int faces, int angle )
#else
void xamRegularPolygon( int x, int y, int radius, int faces, int angle )
#endif
{
	int 	i ;
	int*	p = NULL ;

	if ( faces < 3 )	faces = 3 ;

	p = _allocate( 2 * ( faces + 1 ) ) ;

	for ( i = 0 ; i < faces ; ++i ) {
		double a = 2 * M_PI * ( i / (double)faces - angle / 360.0 ) ;
		*(p + i * 2) = 	x + radius * cos(a) ;
		*(p + i * 2 + 1) = y + radius * sin(a) ;
	}
	m_x = *(p + i * 2) 		= *p ;
	m_y = *(p + i * 2 + 1) 	= *(p + 1) ;

	if ( !_lock( m_screen ) ) {
		_drawPolygon(m_screen, faces + 1, p, m_drawingColor, false ) ;
		_unlock( m_screen ) ;
	}

	_free(p) ;
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::filledRegularPolygon( int x, int y, int radius, int faces, int angle )
#else
void xamFilledRegularPolygon( int x, int y, int radius, int faces, int angle )
#endif
{
	int 	i ;
	int*	p = NULL ;

	if ( faces < 3 )	faces = 3 ;

	p = _allocate( 2 * ( faces + 1 ) ) ;

	for ( i = 0 ; i < faces ; ++i ) {
		double a = 2 * M_PI * ( i / (double)faces - angle / 360.0 ) ;
		*(p + i * 2) = 	x + radius * cos(a) ;
		*(p + i * 2 + 1) = y + radius * sin(a) ;
	}
	m_x = *(p + i * 2) 		= *p ;
	m_y = *(p + i * 2 + 1) 	= *(p + 1) ;

	if ( !_lock( m_screen ) ) {
		_drawPolygon(m_screen, faces + 1, p, m_drawingColor, true ) ;
		_unlock( m_screen ) ;
	}

	_free(p) ;
}

// ----------------------------------------------------------------------------
// dessin d'un rectangle, avec/sans remplissage
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::_drawRectangle(SDL_Surface* surf, int x, int y, int w, int h, Uint32 c, bool filled )
{
#else
void _drawRectangle(SDL_Surface* surf, int x, int y, int w, int h, Uint32 c, bool filled )
{
#endif
	int i, j ;

	if ( surf == m_transparentSurf ) {	//// tenir compte de thickness !
		x += XAM_MAX_THICKNESS / 2 ;
		y += XAM_MAX_THICKNESS / 2 ;
	}

	for ( j = 0 ; j < h ; ++j ) {
		if ( ( filled )||( j == 0 )||( j == h - 1 ) ) {
			for ( i = 0 ; i < w ; ++i )	_putPixel(surf, x - w / 2 + i, y - h / 2 + j, c ) ;
		}
		else {
			_putPixel(surf, x - w / 2, y - h / 2 + j, c ) ;
			_putPixel(surf, x + w / 2 - 1, y - h / 2 + j, c ) ;
		}
	}
}

// rectangle de centre (x,y), dimensions (w,h)
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::centeredRectangle( int x, int y, int w, int h )
{
#else
void xamCenteredRectangle( int x, int y, int w, int h )
{
#endif
	// épaisseur de trait = 1 -> dessin direct sur m_screen
	if ( m_thickness <= 1 ) {
		if ( _lock( m_screen ) != 0 ) return ;
		_drawRectangle(m_screen, x, y, w, h, m_drawingColor, false ) ;
		_unlock( m_screen ) ;
	}
	// sinon, construction via calque transparent
	else {
		int eInt = m_thickness / 2 ;
		int eExt = m_thickness - eInt ;
		SDL_Surface* surf = _createTransparentLayer( w, h ) ;
		_drawRectangle(surf, w / 2, h / 2, w + 2 * eExt, h + 2 * eExt, m_drawingColor, true ) ;
		_drawRectangle(surf, w / 2, h / 2, w - 2 * eInt, h - 2 * eInt, m_transparentColor, true ) ;
		_blitAndFreeTransparentLayer( x - w / 2, y - h / 2 ) ;
	}
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::filledCenteredRectangle( int x, int y, int w, int h )
{
#else
void xamFilledCenteredRectangle( int x, int y, int w, int h )
{
#endif
	SDL_Surface* surf = _createTransparentLayer( w, h ) ;
	_drawRectangle(surf, w / 2, h / 2, w, h, m_drawingColor, true ) ;
	_blitAndFreeTransparentLayer( x - w / 2, y - h / 2 ) ;
}

// rectangle d'origine (x,y), dimensions (w,h)
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::rectangle( int x, int y, int w, int h )
{
	this->centeredRectangle( x + w / 2, y + h / 2, w, h ) ;
#else
void xamRectangle( int x, int y, int w, int h )
{
	xamCenteredRectangle( x + w / 2, y + h / 2, w, h ) ;
#endif
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::filledRectangle( int x, int y, int w, int h )
{
	this->filledCenteredRectangle( x + w / 2, y + h / 2, w, h ) ;
#else
void xamFilledRectangle( int x, int y, int w, int h )
{
	xamFilledCenteredRectangle( x + w / 2, y + h / 2, w, h ) ;
#endif
}

// ----------------------------------------------------------------------------
// dessin d'un cercle sur surf, centre (x,y), rayon r, couleur c, avec/sans remplissage
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::_drawCircle(SDL_Surface* surf, int x, int y, int r, Uint32 c, bool filled )
{
#else
void _drawCircle(SDL_Surface* surf, int x, int y, int r, Uint32 c, bool filled )
{
#endif
	int	cx ;
	int cy = ++r ;	// r = 0 -> marquage du centre
	int df = 1 - r ;
	int xx ;

	if ( surf == m_transparentSurf ) {	//// tenir compte de thickness !
		x += XAM_MAX_THICKNESS / 2 ;
		y += XAM_MAX_THICKNESS / 2 ;
	}

	for ( cx = 0 ; cx < cy ; cx++ ) {
		if ( df < 0 ) {
			df += 2 * cx + 3 ;
		}
		else {
			df += 2 * ( cx - cy ) + 5 ;
			cy-- ;
		}
		if ( filled ) {
			for ( xx = -cx ; xx <= cx ; ++xx ) {
				_putPixel(surf, x + xx, y - cy, c ) ;
				_putPixel(surf, x + xx, y + cy, c ) ;
			}
			for ( xx = -cy ; xx <= cy ; ++xx ) {
				_putPixel(surf, x + xx, y - cx, c ) ;
				_putPixel(surf, x + xx, y + cx, c ) ;
			}
		}
		else {
			_putPixel(surf, x + cx, y - cy, c ) ;
			_putPixel(surf, x + cy, y - cx, c ) ;
			_putPixel(surf, x + cy, y + cx, c ) ;
			_putPixel(surf, x + cx, y + cy, c ) ;
			_putPixel(surf, x - cx, y + cy, c ) ;
			_putPixel(surf, x - cy, y + cx, c ) ;
			_putPixel(surf, x - cy, y - cx, c ) ;
			_putPixel(surf, x - cx, y - cy, c ) ;
		}
	}
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::circle( int x, int y, int radius )
{
#else
void xamCircle( int x, int y, int radius )
{
#endif
	// épaisseur de trait = 1 -> dessin direct sur m_screen
	if ( m_thickness <= 1 ) {
		if ( _lock( m_screen ) != 0 ) return ;
		_drawCircle(m_screen, x, y, radius, m_drawingColor, false ) ;
		_unlock( m_screen ) ;
	}
	// sinon, construction via calque transparent
	else {
		int eInt = m_thickness / 2 ;
		int eExt = m_thickness - eInt ;
		SDL_Surface* surf = _createTransparentLayer( 2 * radius, 2 * radius ) ;
		_drawCircle(surf, radius, radius, radius + eExt, m_drawingColor, true ) ;
		_drawCircle(surf, radius, radius, radius - eInt, m_transparentColor, true ) ;
		_blitAndFreeTransparentLayer(x - radius, y - radius ) ;
	}
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::filledCircle( int x, int y, int radius )
{
#else
void xamFilledCircle( int x, int y, int radius )
{
#endif
	SDL_Surface* surf = _createTransparentLayer( 2 * radius, 2 * radius ) ;
	_drawCircle(surf, radius, radius, radius, m_drawingColor, true ) ;
	_blitAndFreeTransparentLayer(x - radius, y - radius ) ;
}

// ----------------------------------------------------------------------------
// dessin d'une ellipse sur surf, centre (x,y), rayons a et b, couleur c, avec/sans remplissage
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::_drawEllipse(SDL_Surface* surf, int x, int y, int a, int b, Uint32 c, bool filled )
{
#else
void _drawEllipse(SDL_Surface* surf, int x, int y, int a, int b, Uint32 c, bool filled )
{
#endif
	int aa = a * a, bb = b * b ;
	int xx, xe = 0, ye = b ;
	int p, px = 0, py = 2 * aa * b ;

	if ( surf == m_transparentSurf ) {	//// tenir compte de thickness !
		x += XAM_MAX_THICKNESS / 2 ;
		y += XAM_MAX_THICKNESS / 2 ;
	}

	_putPixel( surf, x + xe, y + ye, c ) ;
	_putPixel( surf, x + xe, y - ye, c ) ;

	p = round( bb - b * aa + 0.25 * aa ) ;
	while ( px < py ) {
		xe++ ;
		px += 2 * bb ;
		if ( p >= 0 ) {
			ye-- ;
			py -= 2 * aa ;
			p -= py ;
		}
		p += bb + px ;
		if ( filled ) {
			for ( xx = -xe ; xx <= xe ; ++xx ) {
				_putPixel( surf, x + xx, y + ye, c ) ;
				_putPixel( surf, x + xx, y - ye, c ) ;
			}
		}
		else {
			_putPixel( surf, x + xe, y + ye, c ) ;
			_putPixel( surf, x + xe, y - ye, c ) ;
			_putPixel( surf, x - xe, y + ye, c ) ;
			_putPixel( surf, x - xe, y - ye, c ) ;
		}
	}

	p = round( bb * ( xe + 0.5 ) * (xe + 0.5 ) + aa * ( ( ye - 1 ) * ( ye - 1 ) - bb ) ) ;
	while ( ye > 0 ) {
		ye-- ;
		py -= 2 * aa ;
		if ( p <= 0 ) {
			xe++ ;
			px += 2 * bb ;
			p += px ;
		}
		p += aa - py ;
		if ( filled ) {
			for ( xx = -xe ; xx <= xe ; ++xx ) {
				_putPixel( surf, x + xx, y + ye, c ) ;
				_putPixel( surf, x + xx, y - ye, c ) ;
			}
		}
		else {
			_putPixel( surf, x + xe, y + ye, c ) ;
			_putPixel( surf, x + xe, y - ye, c ) ;
			_putPixel( surf, x - xe, y + ye, c ) ;
			_putPixel( surf, x - xe, y - ye, c ) ;
		}
	}
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::ellipse( int x, int y, int xRadius, int yRadius )
{
#else
void xamEllipse( int x, int y, int xRadius, int yRadius )
{
#endif
	// épaisseur de trait = 1 -> dessin direct sur m_screen
	if ( m_thickness <= 1 ) {
		if ( _lock( m_screen ) != 0 ) return ;
		_drawEllipse(m_screen, x, y, xRadius, yRadius, m_drawingColor, false ) ;
		_unlock( m_screen ) ;
	}
	// sinon, construction via calque transparent
	else {
		int eInt = m_thickness / 2 ;
		int eExt = m_thickness - eInt ;
		SDL_Surface* surf = _createTransparentLayer( 2 * xRadius, 2 * yRadius ) ;
		_drawEllipse(surf, xRadius, yRadius, xRadius + eExt, yRadius + eExt, m_drawingColor, true ) ;
		_drawEllipse(surf, xRadius, yRadius, xRadius - eInt, yRadius - eInt, m_transparentColor, true ) ;
		_blitAndFreeTransparentLayer(x - xRadius, y - yRadius ) ;
	}
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::filledEllipse( int x, int y, int xRadius, int yRadius )
{
#else
void xamFilledEllipse( int x, int y, int xRadius, int yRadius )
{
#endif
	// l'algorithme de remplissage peut tracer plusieurs fois la même ligne
	// ce qui annule le mécanisme XOR si affichage direct sur m_screen
	// il faut donc passer par le calque transparent dans lequel le mode XOR est traité
	// seulement au moment du blit...

	SDL_Surface* surf = _createTransparentLayer( 2 * xRadius, 2 * yRadius ) ;
	_drawEllipse(surf, xRadius, yRadius, xRadius, yRadius, m_drawingColor, true ) ;
	_blitAndFreeTransparentLayer(x - xRadius, y - yRadius ) ;
}

// ----------------------------------------------------------------------------
// remplissage de zone
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::_floodFill(SDL_Surface* surf, int x, int y, Uint32 oldc, Uint32 newc )
{
#else
void _floodFill(SDL_Surface* surf, int x, int y, Uint32 oldc, Uint32 newc )
{
#endif
	int yy ;

	if ( _getPixel(surf, x, y ) != oldc )	return ;

	// vers le bas
	yy = y ;
	while ( _getPixel(surf, x, yy ) == oldc ) {
		_putPixel(surf, x, yy, newc ) ;
		if ( ++yy >= m_height ) break ;
	}
	// vers le haut
	yy = y - 1 ;
	while ( _getPixel(surf, x, yy ) == oldc ) {
		_putPixel(surf, x, yy, newc ) ;
		if ( --yy < 0 ) break ;
	}
	// vers la gauche
	yy = y ;
	while ( _getPixel(surf, x, yy ) == newc ) {
		if ( x > 0 && _getPixel(surf, x - 1, yy ) == oldc )	_floodFill(surf, x - 1, yy, oldc, newc ) ;
		if ( ++yy >= m_height ) break ;
	}
	yy = y - 1 ;
	while ( _getPixel(surf, x, yy ) == newc ) {
		if ( x > 0 && _getPixel(surf, x - 1, yy ) == oldc )	_floodFill(surf, x - 1, yy, oldc, newc ) ;
		if ( --yy < 0 ) break ;
	}
	// vers la droite
	yy = y ;
	while ( _getPixel(surf, x, yy ) == newc ) {
		if ( x < m_width && _getPixel(surf, x + 1, yy ) == oldc )	_floodFill(surf, x + 1, yy, oldc, newc ) ;
		if ( ++yy >= m_height ) break ;
	}
	yy = y - 1 ;
	while ( _getPixel(surf, x, yy ) == newc ) {
		if ( x < m_width && _getPixel(surf, x + 1, yy ) == oldc )	_floodFill(surf, x + 1, yy, oldc, newc ) ;
		if ( --yy < 0 ) break ;
	}
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
//void XamGraph::floodFill( int x, int y, int color )
void XamGraph::floodFill( int x, int y, Uint32 color )
{
#else
//void xamFloodFill( int x, int y, int color )
void xamFloodFill( int x, int y, Uint32 color )
{
#endif
	Uint32 oldColor, newColor ;

	if ( _lock( m_screen ) != 0 ) return ;

	oldColor = _getPixel(m_screen, x, y ) ;
	newColor = SDL_MapRGBA(m_screen->format, XAM_R(color), XAM_G(color), XAM_B(color), XAM_A(color) ) ;
	if ( oldColor != newColor )	_floodFill(m_screen, x, y, oldColor, newColor ) ;

	_unlock( m_screen ) ;
}

// ----------------------------------------------------------------------------
// épaisseur de ligne
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::setLineThickness( int thickness )
{
#else
void xamSetLineThickness( int thickness )
{
#endif
	if ( thickness > XAM_MAX_THICKNESS )	thickness = XAM_MAX_THICKNESS ;
	if ( thickness < 1 ) 	thickness = 1 ;
	m_thickness = thickness ;
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
int XamGraph::lineThickness() const
{
#else
int xamLineThickness( void )
{
#endif
	return m_thickness ;
}

// ----------------------------------------------------------------------------
// style de ligne (affecte seulement lignes et polygones)
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::setLineStyle( int style )
{
#else
void xamSetLineStyle( int style )
{
#endif
	if (( style != XAM_DASHED_LINE )&&( style != XAM_DOTTED_LINE ))	style = XAM_SOLID_LINE ;
	m_style = style ;
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
int XamGraph::lineStyle() const
{
#else
int xamLineStyle( void )
{
#endif
	return m_style ;
}

// ----------------------------------------------------------------------------
// affichage de textes
// ----------------------------------------------------------------------------

// cf. xamgraphfont.c

// ----------------------------------------------------------------------------
// chargement d'images
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
bool XamGraph::loadImage( int x, int y, const char* fileName )
{
#else
bool xamLoadImage( int x, int y, const char* fileName )
{
	if ( !_isInit() )	return false ;
#endif
	SDL_Surface*	bmp = IMG_Load( fileName ) ;
	if ( bmp == NULL )	return false ;

	SDL_Rect	rect = { (Sint16)x, (Sint16)y, 0, 0 } ;
	SDL_BlitSurface(bmp, NULL, m_screen, &rect ) ;
	SDL_FreeSurface( bmp ) ;
	return true ;
}
