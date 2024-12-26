/*  ---------------------------------------------------------------------------
 *  filename    :   mybutton.cpp
 *  description :   C++ library header - IMPLEMENTATION classe MyButton
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

#include "mybutton.h"

// constructeur par défaut

MyButton::MyButton(XamGraph* gr, int x, int y, int w, int h )
	: m_gr( gr )
	, m_lineColor(XAM_BLACK)
	, m_fillColor(XAM_WHITE)
	, m_focusColor(XAM_LIGHT_GRAY)
	, m_textColor(XAM_BLACK)
{
	m_pos.set(x, y ) ;
	m_size.set(w, h ) ;
}

// constructeur

MyButton::MyButton(XamGraph* gr, Point2D pos, Size2D size )
	: m_gr( gr )
	, m_pos( pos )
	, m_size( size )
	, m_lineColor(XAM_BLACK)
	, m_fillColor(XAM_WHITE)
	, m_focusColor(XAM_LIGHT_GRAY)
	, m_textColor(XAM_BLACK)
{
}

// projection sur la surface graphique

void MyButton::show()
{
	m_gr->setColor( hasFocus() ? m_focusColor : m_fillColor ) ;
	m_gr->filledRectangle(m_pos.x(), m_pos.y(), m_size.width(), m_size.height() ) ;

	m_gr->setColor( m_lineColor ) ;
	m_gr->rectangle(m_pos.x(), m_pos.y(), m_size.width(), m_size.height() ) ;
	
	m_gr->setColor( m_textColor ) ;
	m_gr->setTextAlignment( XAM_ALIGNMENT_CENTER ) ; 
	int x = m_pos.x() + m_size.width() / 2 ;
	int y = m_pos.y() + m_size.height() / 2 ;
	m_gr->textOut(x, y, m_text.c_str() ) ;
}

// teste si le point 'pos' appartient au rectangle d'inscription

bool MyButton::contains(Point2D pos )
{
	if ( pos.x() < m_pos.x() )	return false ;
	if ( pos.x() > ( m_pos.x() + m_size.width() ) )	return false ;
	if ( pos.y() < m_pos.y() )	return false ;
	if ( pos.y() > ( m_pos.y() + m_size.height() ) )	return false ;
	return true ;
}
