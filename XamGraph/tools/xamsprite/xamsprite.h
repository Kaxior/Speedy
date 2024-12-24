/*  ---------------------------------------------------------------------------
 *  filename    :   xamsprite.h
 *  description :   INTERFACE de la classe XamSprite
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

#ifndef XAM_SPRITE
#define XAM_SPRITE

#include "_VERSION"
#include <xamgraph.h>
#include <string>
#include <vector>
#include <algorithm>

class XamSprite
{
  public :
    XamSprite(XamGraph* gr, std::string name = "sprite" ) 
        : m_gr( gr ) 
        , m_name( name )
        , m_texture( nullptr )
        , m_x( 0 )
        , m_y( 0 )
        , m_speed( 1.0 )
        , m_dx( 0 )
        , m_dy( 0 )
        , m_lockedUp( false )
        , m_inCollection( true )
    {
    }
    ~XamSprite() ;

    std::string name() const { return m_name ; }

    bool loadSheet(const char* fileName, int nRow = 1, int nColumn = 1, bool inCollection = true ) ;

    void select(int row, int column = -1 ) ;
    void next() ;
    int  numRow() const { return m_numRow ; }
    int  numColumn() const { return m_numColumn ; }
    int  currentRow() const { return m_currentRow ; }
    int  currentColumn() const { return m_currentColumn ; }
    void draw() ;
    void draw(int x, int y ) ;

    static void drawCollection() ;
    void setInCollection(bool inCollection = true ) { m_inCollection = inCollection ; } ;

    int  x() const { return m_x ; }
    int  y() const { return m_y ; }
    void setX(int x ) { m_x = x ; }
    void setY(int y ) { m_y = y ; }
    void setPosition(int x, int y ) { setX(x) ; setY(y) ; }
    int  xSheet() const { return m_clip.x ; }
    int  ySheet() const { return m_clip.y ; }
    int  width() const { return m_clip.w ; }
    int  height() const { return m_clip.h ; }

    void  setSpeed(float speed ) { m_speed = speed ; }
    float speed() const { return m_speed ; }

    void setMove(int dx, int dy ) { m_dx = dx ; m_dy = dy ; }
    bool isMoving() const { return ( ( m_dx != 0 )||( m_dy != 0 ) ? true : false ) ; }

    void setLockedUp(bool lockedUp = true ) { m_lockedUp = lockedUp ; }
    bool isLockedUp() const { return m_lockedUp ; }

    void flip(int flags ) ;
    void turn(int flags ) ;
    enum { FLIP_VERTICAL = 1, FLIP_HORIZONTAL, TURN_LEFT, TURN_RIGHT } ;

    static std::string version() { return XAMSPRITE_VERSION ; }

  private :
    Uint32 getPixel(SDL_Surface* surface, int x, int y ) ;
    void   putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel ) ;
    SDL_Surface* flipSurface( SDL_Surface* surface, int flags ) ;
    SDL_Surface* turnSurface( SDL_Surface* surface, int flags ) ;

  private :
    XamGraph*       m_gr ;
    std::string     m_name ;
    int             m_numRow ;      // nombre de lignes du spritesheet
    int             m_numColumn ;   // nombre de colonnes du spritesheet
    int             m_currentRow ;
    int             m_currentColumn ;
    SDL_Surface*    m_spriteSheet ;
    SDL_Texture*	m_texture ;
    SDL_Rect		m_clip ;        // aire à découper sur le spritesheet

    int             m_x, m_y ;      // position courante
    float           m_speed ;       // facteur de vitesse de déplacement
    int             m_dx, m_dy ;    // indicateurs de direction (défaut: -1,0,+1)
    bool            m_lockedUp ;    // vrai = prisonnier de la fenêtre

    bool            m_inCollection ;    // vrai = sprite dans la collection (défaut)

    static std::vector<XamSprite*> m_order ;    // ordonnancement des sprites
} ;

#endif
