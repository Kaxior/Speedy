/*  ---------------------------------------------------------------------------
 *  filename    :   myapp.cpp
 *  description :   C++ library header - IMPLEMENTATION classe MyApp
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

#include "myapp.h"

// constructeur

MyApp::MyApp(int width, int height, const char* title )
	: XamGraph(width, height, title )
//	, spritePos(20,20)
{
	// création d'un témoin lumineux
	
	m_led = new MyLed(this, maxX() - 30, 30, 8 ) ;
	
	// fabrication des bouton-poussoirs
	
	Point2D pos ;				// position bouton
	Size2D	size(100, 40 ) ;	// taille bouton
	int 	gap = 200 ;			// entraxe
	
	pos.set( ( maxX() - gap - size.width() ) / 2, 0.9 * maxY() - size.height() ) ;
	m_btn[GO] = new MyButton(this, pos, size ) ;
	m_btn[GO]->setText("Go!") ;
	
	pos.setX( pos.x() + gap ) ;
	m_btn[QUIT] = new MyButton(this, pos, size ) ;
	m_btn[QUIT]->setText("Quit") ;
	m_btn[QUIT]->setTextColor(XAM_DARK_BLUE) ;
	
	// sprites ! ordre déterminé par appel à loadSheet()
	// (le premier au premier plan...)

	m_pacman = new XamSprite(this, "pacman" ) ;
	m_pacman->loadSheet("../resources/pacman.png" ) ;
	m_pacman->setY( (maxY() - m_pacman->height()) / 2 ) ;
	m_pacman->setSpeed( 4.5 ) ;
	m_pacman->setMove(1,0) ;

	m_stickman = new XamSprite(this) ;
	m_stickman->loadSheet("../resources/stickman.bmp", 4, 9 ) ;
	m_stickman->setPosition(40, 40 ) ;
	m_stickman->setLockedUp() ;
	m_stickman->setSpeed( 2 ) ;

	// projections sur l'écran
	
	setDrawingArea(XAM_DRAWING_SCREEN ) ;
	clearScreen( XAM_IVORY ) ;
	
	m_led->show() ;
	for (int i = 0 ; i < NUMBTN ; ++i )	m_btn[i]->show() ;

	setColor(XAM_DARK_GRAY) ;
	setTextAlignment(XAM_ALIGNMENT_LEFT ) ;
	textOut(10, 10, XamSprite::version().c_str() ) ;

	// démarrage de l'horloge
		
	startTimer(this, PERIOD ) ;
}

// destructeur

MyApp::~MyApp()
{
	for (int i = 0 ; i < NUMBTN ; ++i )	delete m_btn[i] ;
	delete m_led ;
}

// exemple d'interception des mouvements de la souris
// pour mettre en évidence le survol des boutons de la fenêtre

void MyApp::onMouseMove(int x, int y )
{
	for (int i = 0 ; i < NUMBTN ; ++i ) {
		bool redrawNeeded = false ;
		// est-ce que le pointeur souris entre dans la surface du bouton ?
		// autrement dit, les coordonnées sont dans le rectangle du bouton
		// et celui-ci n'a pas encore le focus...
		if ( m_btn[i]->contains( Point2D(x,y) ) ) {
			if ( !m_btn[i]->hasFocus() )	redrawNeeded = true ;
		}
		// sinon, le pointeur souris sort-il de la surface du bouton ?
		// les coordonnées ne sont pas dans le rectangle du bouton
		// et le bouton avait le focus...
		else {
			if ( m_btn[i]->hasFocus() )	redrawNeeded = true ;
		}
		// mise à jour du bouton le cas échéant
		if ( redrawNeeded ) {
			m_btn[i]->setFocus( !m_btn[i]->hasFocus() ) ;
			m_btn[i]->show() ;
		}
	}
}

// exemple d'interception des relachements des boutons de la souris
// --> action liée au bouton 'Quit'

void MyApp::onMouseButtonUp(int x, int y, int button ) 
{
	if ( button != BUTTON_LEFT )	return ;	// bouton gauche attendu...

	if ( m_btn[QUIT]->contains( Point2D(x,y) ) )	onBtnQuitClicked() ;
}

// exemple d'interception de l'horloge périodique
// --> clignotement de la led + déplacement des sprites

void MyApp::onTimeout()
{
	static unsigned cpt = 0 ;

	if ( cpt++ % 16 == 0 ) {
		m_led->setState( !m_led->state() ) ;
		m_led->show() ;
	}
	
	// animation sprite...
	if ( m_stickman->isMoving() ) {
		if ( cpt % 5 == 0 )	m_stickman->next() ;
	}

//	updateScreen() ; 				// version sans sprite...
	XamSprite::drawCollection() ;	// avec sprites !

	// animation sprite...
	if ( m_pacman->x() <= -m_pacman->width() ) {
		m_pacman->setMove(+1,0) ;
		m_pacman->flip(XamSprite::FLIP_HORIZONTAL ) ;
	}
	if ( m_pacman->x() >= maxX() ){
		m_pacman->setMove(-1,0) ;
		m_pacman->flip(XamSprite::FLIP_HORIZONTAL ) ;
	}
}

// exemple d'interception des appuis/relâchements des touches du clavier
// --> changement de couleur de la led
// --> déplacement d'un sprite
// --> fin de l'application

void MyApp::onKeyDown(int key, int modifier )
{
	switch( key ) {
		case 'V' :
		case 'v' :	m_led->setColor(XAM_LIGHT_GREEN ) ;
					break ;
		case 'O' :
		case 'o' :	m_led->setColor(XAM_ORANGE ) ;
					break ;
		case 'R' :
		case 'r' :	m_led->setColor(XAM_LIGHT_RED ) ;
					break ;

		case SDLK_RIGHT :	m_stickman->select(2) ;
							m_stickman->setMove(2, -1 ) ;
							break ;
		case SDLK_LEFT :	m_stickman->select(1) ;
							m_stickman->setMove(-2, 0 ) ;
							break ;
		case SDLK_DOWN :	m_stickman->select(3) ;
							m_stickman->setMove(0, 1 ) ;
							break ;
		case SDLK_UP :		m_stickman->select(0) ;
							m_stickman->setMove(0, -1 ) ;
							break ;

		case SDLK_ESCAPE :	quit() ; 
							break ;
	}
}

void MyApp::onKeyUp(int key, int modifier ) 
{
	if ( m_stickman->isMoving() ) {
		m_stickman->setMove(0, 0 ) ;
		m_stickman->select(0, 0 ) ;
	}
}

// action associée au bouton 'Quit'
// --> fin de l'application

void MyApp::onBtnQuitClicked()
{
	quit() ;
}
