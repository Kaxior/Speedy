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

#include "myled.h"

// constructeurs

MyLed::MyLed(XamGraph* gr, int x, int y, int radius )
	: m_gr( gr )
	, m_radius( radius)
	, m_lineColor(XAM_BLACK)
	, m_fillColor(XAM_RED)
	, m_offColor(XAM_LIGHT_GRAY)
	, m_state(false)
{
	m_pos.set(x, y ) ;
}

MyLed::MyLed(XamGraph* gr, Point2D pos, int radius )
	: m_gr( gr )
	, m_pos( pos )
	, m_radius( radius)
	, m_lineColor(XAM_BLACK)
	, m_fillColor(XAM_RED)
	, m_offColor(XAM_LIGHT_GRAY)
	, m_state(false)
{
}

// projection sur la surface graphique

void MyLed::show()
{
	m_gr->setColor( m_state ? m_fillColor : m_offColor ) ;
	m_gr->filledCircle(m_pos.x(), m_pos.y(), m_radius ) ;
	m_gr->setColor( m_lineColor ) ;
	m_gr->circle(m_pos.x(), m_pos.y(), m_radius ) ;
}

