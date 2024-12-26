/* ---------------------------------------------------------------------------+
 |     Modèle C de point d'entrée d'une application basée sur XamGraph        |
 + ---------------------------------------------------------------------------+
 |
 |	Nom :			main.c
 |
 |	Sujet :
 |
 |  Auteur :
 |	Version :		0.1
 |	Creation :
 |	Mise a jour :
 |	Fabrication :	Makefile dédié XamGraph
 + ------------------------------------------------------------------------- */

#include <xamgraph.h>

#define	ESC	0x1b

/* fonctions d'interception d'événements */

void myKeyDownHandler(int key, int modifier )
{
}

void myKeyUpHandler(int key, int modifier )
{
	if ( key == ESC )	xamQuit() ;
}

void myMouseMotionHandler(int x, int y )
{
}

void myMouseButtonDownHandler(int x, int y, int button )
{
}

void myMouseButtonUpHandler(int x, int y, int button )
{
}

/* contenu initial de la fenêtre d'application */

void myDrawing( void )
{
	xamSetColor( XAM_BLACK ) ;
	xamTextOut(10,10, "Welcome on board !") ;
	xamUpdateScreen() ;
}

/* point d'entrée de l'application */

int main(int argc, char** argv )
{
	/* initialisation */
	xamCreateGraph(640, 400, "My XamGraph C program" ) ;
	xamSetDrawingArea( XAM_DRAWING_BITMAP ) ;
	xamClearScreen( XAM_WHITE ) ;
	xamUpdateScreen() ;

	myDrawing() ;

	/* installation des intercepteurs d'événements */
	xamConnectKeyDownEvent( myKeyDownHandler ) ;
	xamConnectKeyUpEvent( myKeyUpHandler ) ;
	xamConnectMouseMotionEvent( myMouseMotionHandler ) ;
	xamConnectMouseButtonDownEvent( myMouseButtonDownHandler ) ;
	xamConnectMouseButtonUpEvent( myMouseButtonUpHandler ) ;

	/* boucle d'événements */
	xamRun() ;

	/* fermeture */
	xamCloseGraph() ;
	return 0 ;
}
