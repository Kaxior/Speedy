/*  ---------------------------------------------------------------------------
 *  filename    :   xamsprite.cpp
 *  description :   IMPLEMENTATION de la classe XamSprite
 *
 *	project     :	XamGraph tools
 *  start date  :   octobre 2024
 *  ---------------------------------------------------------------------------
 *  Copyright 2007-2024 by Alain Menu   <alain.menu@ac-creteil.fr>
 *
 *  This file is part of "XamGraph tools"
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

#include <xamsprite.h>

#include <string>
#include <iostream>
using namespace std ;

std::vector<XamSprite*> XamSprite::m_order ;

bool XamSprite::loadSheet(const char* fileName, int nRow, int nColumn, bool inCollection )
{
  		m_numRow = nRow ;
  		m_numColumn = nColumn ;

  		m_spriteSheet = IMG_Load( fileName ) ;
  		if ( m_spriteSheet == nullptr )	return false ;

		m_texture = SDL_CreateTextureFromSurface(m_gr->_renderer(), m_spriteSheet ) ;

		m_clip.w = m_spriteSheet->w / m_numColumn ;
		m_clip.h = m_spriteSheet->h / m_numRow ;

		select(0, 0 ) ;

        m_inCollection = inCollection ;
        if ( m_inCollection ) m_order.push_back( this ) ;

		return true ; 
}

XamSprite::~XamSprite() 
{
    if ( m_texture != nullptr ) SDL_DestroyTexture( m_texture ) ;
}

void XamSprite::select(int row, int column )
{
    m_currentRow = row % m_numRow ;
    if ( column != -1 ) m_currentColumn = column % m_numColumn ;
    m_clip.x = m_currentColumn * m_clip.w ;
    m_clip.y = m_currentRow * m_clip.h ;
}

void XamSprite::next() 
{
    m_currentColumn++ ;
    m_currentColumn %= m_numColumn ;
    m_clip.x = m_currentColumn * m_clip.w ;
}

void XamSprite::draw()
{
    draw(m_x + m_dx * m_speed, m_y + m_dy * m_speed ) ;
}

void XamSprite::draw(int x, int y ) 
{
    if ( m_lockedUp ) {
        if ( x < 0 )    x = 0 ;
        else if ( x > ( m_gr->maxX() - m_clip.w ) )    x = m_gr->maxX() - m_clip.w ;
        if ( y < 0 )    y = 0 ;
        else if ( y > ( m_gr->maxY() - m_clip.h ) )    y = m_gr->maxY() - m_clip.h ;
    }
    m_x = x ;
    m_y = y ;
    SDL_Rect dest = { m_x, m_y, m_clip.w, m_clip.h } ;

    if ( !m_inCollection ) {
        SDL_UpdateTexture(m_gr->_texture(), NULL, m_gr->_video()->pixels, m_gr->_video()->pitch ) ;
        SDL_RenderCopy(m_gr->_renderer(), m_gr->_texture(), NULL, NULL ) ;
        SDL_RenderCopy(m_gr->_renderer(), m_texture, &m_clip, &dest ) ;
        SDL_RenderPresent(m_gr->_renderer() ) ;
        return ;
    }

    // projection suivant ordre dans la collection
    // premier de la liste au premeir plan
    if ( m_order.front() == this ) {    // premier de la liste ?
        SDL_UpdateTexture(m_gr->_texture(), NULL, m_gr->_video()->pixels, m_gr->_video()->pitch ) ;
        SDL_RenderCopy(m_gr->_renderer(), m_gr->_texture(), NULL, NULL ) ;
    }
    SDL_RenderCopy(m_gr->_renderer(), m_texture, &m_clip, &dest ) ;
    if ( m_order.back() == this ) {     // dernier de la liste ?
        SDL_RenderPresent(m_gr->_renderer() ) ;
    }
}

void XamSprite::drawCollection()
{
    for ( unsigned i = 0 ; i < m_order.size() ; i++ ) {
        m_order[i]->draw() ;
    }
}

void XamSprite::flip(int flags )
{
    m_spriteSheet = flipSurface( m_spriteSheet, flags ) ;
    SDL_DestroyTexture( m_texture ) ;
    m_texture = SDL_CreateTextureFromSurface(m_gr->_renderer(), m_spriteSheet ) ;
}

void XamSprite::turn(int flags )
{
    m_spriteSheet = turnSurface( m_spriteSheet, flags ) ;
    SDL_DestroyTexture( m_texture ) ;
    m_texture = SDL_CreateTextureFromSurface(m_gr->_renderer(), m_spriteSheet ) ;
}

// méthodes privées

Uint32 XamSprite::getPixel(SDL_Surface* surface, int x, int y )
{
    Uint32* pixels = (Uint32*)surface->pixels ;
    return pixels[ ( y * surface->w ) + x ] ;
}

void   XamSprite::putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel )
{
    Uint32* pixels = (Uint32*)surface->pixels ;
    pixels[ ( y * surface->w ) + x ] = pixel ;
}

SDL_Surface* XamSprite::flipSurface( SDL_Surface* surface, int flags )
{
    SDL_Surface* flipped = nullptr ;
    flipped = SDL_CreateRGBSurface( SDL_SWSURFACE, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask );

    if ( SDL_MUSTLOCK( surface ) )   SDL_LockSurface( surface ) ;

    for ( int x = 0, rx = flipped->w - 1 ; x < flipped->w ; x++, rx-- ) {
        for ( int y = 0, ry = flipped->h - 1 ; y < flipped->h ; y++, ry-- ) {
            Uint32 pixel = getPixel(surface, x, y ) ;
            if (( flags & FLIP_VERTICAL )&&( flags & FLIP_HORIZONTAL ))  putPixel(flipped, rx, ry, pixel ) ;
            else if ( flags & FLIP_HORIZONTAL )                          putPixel(flipped, rx, y, pixel ) ;
            else if ( flags & FLIP_VERTICAL )                            putPixel(flipped, x, ry, pixel ) ;
        }
    }

    if ( SDL_MUSTLOCK( surface ) )   SDL_UnlockSurface( surface ) ;

    return flipped ;
}

SDL_Surface* XamSprite::turnSurface( SDL_Surface* surface, int flags )
{
    SDL_Surface* turned = nullptr ;
    int size = ( surface->w > surface->h ? surface->h : surface->w ) ;
    turned = SDL_CreateRGBSurface( SDL_SWSURFACE, size, size, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask );

    if ( SDL_MUSTLOCK( surface ) )   SDL_LockSurface( surface ) ;
    
    int xmin = ( surface->w - size ) / 2 ;
    int xmax = xmin + size ;
    int ymin = ( surface->h - size ) / 2 ;
    int ymax = ymin + size ;

    for ( int x = xmin, rx = 0 ; x < xmax ; x++, rx++ ) {
        for ( int y = ymin, ry = 0 ; y < ymax ; y++, ry++ ) {
            Uint32 pixel = getPixel(surface, x, y ) ;
            if (  flags & TURN_LEFT )       putPixel(turned, ry, rx, pixel ) ;
            else if (  flags & TURN_RIGHT ) putPixel(turned, ry, turned->w - 1 - rx, pixel ) ;
        }
    }

    if ( SDL_MUSTLOCK( surface ) )   SDL_UnlockSurface( surface ) ;

    return turned ;
}
