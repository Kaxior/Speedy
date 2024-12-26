/*  ---------------------------------------------------------------------------
 *  filename    :   tools.h
 *  description :   C++ library header - classes utilitaires
 *
 *	project     :	Trivial 2D Graphic Library based on SDL
 *  start date  :   octobre 2024
 *  ---------------------------------------------------------------------------
 *  Copyright 2024 by Alain Menu   <alain.menu@ac-creteil.fr>
 *
 *  This file is part of "XamGraph" demo
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

#ifndef TOOLS_H
#define TOOLS_H

// coordonnées (x,y)

class Point2D
{
  private :
  	int m_x ;
  	int m_y ;
  	
  public :
  	Point2D() : m_x(0), m_y(0) {}
  	Point2D(int x, int y ) : m_x(x), m_y(y) {}
  	void setX(int x ) { m_x = x ; }
  	void setY(int y ) { m_y = y ; }
  	void set(int x, int y ) { setX(x); setY(y) ; }
  	int x() const { return m_x ; }
  	int y() const { return m_y ; }
} ;

// dimensions (largeur,hauteur)

class Size2D
{
  private :
	int m_w ;
	int m_h ;
	
  public :
  	Size2D() : m_w(0), m_h(0) {}
	Size2D(int w, int h ) : m_w(w) , m_h(h) {}
  	void setWidth(int w ) { m_w = w ; }
  	void setHeight(int h ) { m_h = h ; }
  	void set(int w, int h ) { setWidth(w); setHeight(h) ; }
  	int width() const { return m_w ; }
  	int height() const { return m_h ; }
} ;

#endif
