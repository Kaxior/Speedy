/* ---------------------------------------------------------------------------+
 |    Modèle C++ de point d'entrée d'une application basée sur XamGraph       |
 + ---------------------------------------------------------------------------+
 |
 |	Nom :			main.cpp
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

// classe d'application

class MyApp : public XamGraph
{
  public :

  	// constructeur
  	MyApp(int w, int h, const char* title ) : XamGraph(w, h, title )
  	{
		setDrawingArea( XAM_DRAWING_BITMAP ) ;
		clearScreen() ;
		myDrawing() ;
  	}

	// contenu initial de la fenêtre d'application
	void myDrawing()
	{
		setColor( XAM_BLACK ) ;
		textOut(10,10, "Welcome on board !") ;
		updateScreen() ;
	}
} ;

// instance d'application

MyApp*	myApp = NULL ;

// fonctions d'interception d'événements
// ( à déclarer amies de MyApp si l'accès au domaine privé est nécessaire)

void myKeyDownHandler(int key, int modifier )
{
}

void myKeyUpHandler(int key, int modifier )
{
	if ( key == ESC )	myApp->quit() ;
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

// point d'entrée de l'application

int main(int argc, char** argv )
{
	// initialisation
	myApp = new MyApp(640, 400, "My XamGraph C++ program" ) ;

	// installation des intercepteurs d'événements
	myApp->connectKeyDownEvent( myKeyDownHandler ) ;
	myApp->connectKeyUpEvent( myKeyUpHandler ) ;
	myApp->connectMouseMotionEvent( myMouseMotionHandler ) ;
	myApp->connectMouseButtonDownEvent( myMouseButtonDownHandler ) ;
	myApp->connectMouseButtonUpEvent( myMouseButtonUpHandler ) ;

	// boucle d'événements
	myApp->run() ;

	// fermeture
	delete myApp ;
	return 0 ;
}
