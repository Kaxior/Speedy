/*  ---------------------------------------------------------------------------
 *  filename    :   myled.h
 *  description :   C++ library header - INTERFACE classe MyLed
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

#ifndef MYLED_H
#define MYLED_H

#include <xamgraph.h>
#include "tools.h"

class MyLed
{
  private :
  	XamGraph*	m_gr ;			// surface de dessin
	Point2D		m_pos ;			// position
	int			m_radius ;		// rayon
	Uint32		m_lineColor ;	// couleur du contour
	Uint32		m_fillColor ;	// couleur de remplissage 'on'
	Uint32		m_offColor ;	// couleur de remplissage 'off'
	bool		m_state ;		// état courant (on/off)

  public :
	MyLed(XamGraph* gr, int x, int y, int radius ) ;
	MyLed(XamGraph* gr, Point2D pos, int radius ) ;

  	void setColor(Uint32 color ) 	 { m_fillColor = color ; }
  	void setLineColor(Uint32 color ) { m_lineColor = color ; }

	void setState(bool state ) { m_state = state ; }
	bool state() const { return m_state ; }
	
  	void show() ;
} ;

#endif
