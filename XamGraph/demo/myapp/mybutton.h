/*  ---------------------------------------------------------------------------
 *  filename    :   mybutton.h
 *  description :   C++ library header - INTERFACE classe MyButton
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

#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <string>
#include <xamgraph.h>
#include "tools.h"

class MyButton
{
  private :
  	XamGraph*	m_gr ;			// surface de dessin
	Point2D		m_pos ;			// position
	Size2D		m_size ;		// taille
	Uint32		m_lineColor ;	// couleur du cadre
	Uint32		m_fillColor ;	// couleur de remplissage
	Uint32		m_focusColor ;	// couleur de remplissage si activé
	Uint32		m_textColor ;	// couleur du texte
	std::string	m_text ;		// label
	bool		m_focus ;		// indicateur de focus
  	
  public :
  	MyButton(XamGraph* gr, int x, int y, int w, int h ) ;
  	MyButton(XamGraph* gr, Point2D pos, Size2D size ) ;

  	void setColor(Uint32 color ) 	 { m_fillColor = color ; }
  	void setLineColor(Uint32 color ) { m_lineColor = color ; }
  	void setTextColor(Uint32 color ) { m_textColor = color ; }
  	void setFocusColor(Uint32 color ){ m_focusColor = color ; }

  	void setText(std::string text )  { m_text = text ; }
  	void setText(const char* text )  { m_text = std::string(text) ; }
  	void show() ;
  	bool contains(Point2D pos ) ;
  	
  	void setFocus(bool focus = true ) { m_focus = focus ; }
  	bool hasFocus() const { return m_focus ; }
} ;

#endif
