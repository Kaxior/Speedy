/*  ---------------------------------------------------------------------------
 *  filename    :   main.cpp
 *  description :   programme utilitaire : PNM (Portable aNy Map) Viewer
 *
 *	project     :	XamGraph tools
 *  start date  :   octobre 2007
 *  ---------------------------------------------------------------------------
 *  Copyright 2007-2023 by Alain Menu   <alain.menu@ac-creteil.fr>
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

#include <iostream>
#include <iomanip>
using namespace std ;

#include <xamgraph.h>
#include "xampnm.h"

#define	ESC	0x1b

XamGraph*	graph = NULL ;
XamPNM*		image = NULL ;
int 		zoom  = 1 ;

// fonctions d'interception d'événements

void keyUpHandler(int key, int modifier )
{
	if ( key == ESC )	graph->quit() ;
}
	
void mouseButtonDownHandler(int x, int y, int button )
{
	x /= zoom ;
	y /= zoom ;
	cout << '(' << setfill(' ') << setw(4) << x ;
	cout << ',' << setfill(' ') << setw(4) << y << ')' ;
	cout << " \x1b[31m" << setfill(' ') << setw(3) << (int)image->red(x, y ) ;
	cout << " \x1b[32m" << setfill(' ') << setw(3) << (int)image->green(x, y ) ;
	cout << " \x1b[34m" << setfill(' ') << setw(3) << (int)image->blue(x, y ) ;
	cout << "\x1b[0m"<< endl ;
}

// point d'entrée de l'application

int main(int argc, char** argv )
{
	char* fn = NULL ;
	
	for ( int i = 1 ; i < argc ; ++i ) {
		char* p = argv[i] ;
		if (( p[0] == '-' )&&( p[1] == 'z' )) {
			zoom = atoi( p + 2 ) ;
			if ( zoom == 0 )	zoom = 1 ;
		}
		else {
			fn = p ;
		}
	}

	string title = XAMPNM_TITLE + string(" v") + XAMPNM_VERSION ;
	
	if ( fn == NULL ) {
		cerr << "\tPortable aNy Map Viewer - " << title.c_str() ;
		cerr << " - " << XAMPNM_COPYRIGHT << endl ;
		cerr << "\tusage:\x1b[1m xampnm [-z<zoomfactor>] <filename>\x1b[0m" << endl ;
		return -1 ;
	}
	
	image = new XamPNM( fn ) ;
	if ( image->openFailed() ) {
		delete image ;
		return -2 ;
	}
	
	graph = new XamGraph(image->width() * zoom, image->height() * zoom, title.c_str() ) ;

	cout << "PNM type P" << image->magicNumber() << ", " ;
	cout << image->width() << " x " << image->height() << " pixels" ;
	if ( zoom > 1 )	cout << " (zoom x" << zoom << ')' ;
	cout << endl ;

	image->setDisplay( graph ) ;
	image->write( zoom ) ;
	graph->updateScreen() ;
	
	graph->connectKeyUpEvent( keyUpHandler ) ;
	graph->connectMouseButtonDownEvent( mouseButtonDownHandler ) ;

	graph->run() ;

	delete graph ;
	delete image ;
	return 0 ;
}
