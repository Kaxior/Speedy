/*  ---------------------------------------------------------------------------
 *  filename    :   myapp.h
 *  description :   C++ library header - INTERFACE classe MyApp
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

#ifndef MYAPP_H
#define MYAPP_H

#include <xamgraph.h>
#include <xamsprite.h>
#include "myled.h"
#include "mybutton.h"

#define PERIOD	25	// période en ms
#define NUMBTN	2	// nombre de bouton-poussoirs
enum { GO, QUIT } ;	// mnémoniques des bouton-poussoirs

class MyApp : public XamGraph
{
  private :
  	MyLed*		m_led ;
	MyButton*	m_btn[NUMBTN] ;

	XamSprite*	m_stickman ;
	XamSprite*	m_pacman ;

  public :
	MyApp() : XamGraph() {}
	MyApp(int width, int height, const char* title ) ;
	~MyApp() ;
	
	void onMouseMove(int x, int y ) ;
	void onMouseButtonUp(int x, int y, int button ) ;
	void onKeyDown(int key, int modifier ) ;
	void onKeyUp(int key, int modifier ) ;
	void onTimeout() ;
	
	void onBtnQuitClicked() ;
} ;

#endif
