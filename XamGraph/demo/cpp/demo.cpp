/* 	programme de démonstration d'usage de XamGraph
	les ressources suivantes doivent être disponibles :

		../resources/XamGraph_logo.png
		../resources/SDL_logo.png
		../resources/Arial Black.ttf
		../resources/Comic Sans MS.ttf

	le Makefile utilisé est celui fourni par XamGraph.
*/

#include <xamgraph.h>

XamGraph*	gr = NULL ;

// effacement de la zone d'affichage d'info clavier

void eraseKeyInfo( void )
{
	int wmax = gr->textWidth("mod/key : XXXX/XXXXXXXX") ;
	int hmax = gr->textHeight() ;
	gr->setColor( gr->backgroundColor() ) ;
	gr->filledRectangle(200, 350, wmax, hmax ) ;
}

// intercepteurs d'événements liés au clavier

void myKeyDownHandler(int key, int modifier )
{
	eraseKeyInfo() ;
	gr->setColor( XAM_BLACK ) ;
	gr->printf(200, 350, "mod/key : %04X/%08X", modifier, key ) ;
	gr->updateScreen() ;
}

void myKeyUpHandler(int key, int modifier )
{
	if ( key == 0x1b )	gr->quit() ;

	eraseKeyInfo() ;
	gr->updateScreen() ;
}

// effacement de la zone d'affichage d'info souris/touchpad

void eraseMouseInfo( void )
{
	int wmax = gr->textWidth("button N (NNNN,NNNN)") ;
	int hmax = gr->textHeight() ;
	gr->setColor( gr->backgroundColor() ) ;
	gr->filledRectangle(200, 320, wmax, hmax ) ;
}

// intercepteurs d'événements souris/touchpad

void myMouseButtonDownHandler(int x, int y, int button )
{
	if ( button == 3 ) {
		gr->restoreScreen() ;
		return ;
	}

	eraseMouseInfo() ;
	gr->setColor( XAM_BLACK ) ;
	gr->printf(200, 320, "button %d (%04d,%04d)", button, x, y ) ;
	gr->updateScreen() ;
}

void myMouseButtonUpHandler(int x, int y, int button )
{
	eraseMouseInfo() ;
	gr->updateScreen() ;
}


void myMouseMotionHandler(int x, int y )
{
	static int xmem = -1 , ymem = -1 ;
	int w = 64 ;
	int h = 32 ;

	gr->setWriteMode( XAM_MODE_XOR ) ;
	gr->setColor( XAM_WHITE ) ;

	if ( xmem != -1 )	gr->filledEllipse( xmem, ymem, w, h ) ;
	gr->filledEllipse( x, y, w, h ) ;

	gr->setWriteMode( XAM_MODE_COPY ) ;

	xmem = x ;
	ymem = y ;

	gr->updateScreen() ;
}

// témoin temporel
/*
void myTimerHandle()
{
	static char s[] = "-\\|/X" ;
	static int  i = 0 ;
	s[4] = s[ i++ % 4 ] ;
	int wmax = gr->textWidth("X") ;
	int hmax = gr->textHeight() ;
	gr->setColor( gr->backgroundColor() ) ;
	gr->filledRectangle(5, 5, wmax, hmax ) ;
	gr->setColor( XAM_BLACK ) ;
	gr->textOut(5, 5, s + 4 ) ;
	gr->updateScreen() ;
}
*/
// dessin initial

void myDrawing( void )
{
	int	i, x, y, yline ;

	// logo

	gr->loadImage(20,20,"../resources/XamGraph_logo.png" ) ;

	// version

	gr->loadFont("../resources/Arial Black.ttf", 20 ) ;
	gr->setTextAlignment( XAM_ALIGNMENT_CENTER ) ;
	gr->setColor( XAM_BLACK ) ;
	gr->printf(320, 180, "version %s", XAMGRAPH_VERSION ) ;
	gr->loadFont("../resources/Arial Black.ttf", 16 ) ;
	gr->setColor( XAM_DARK_RED ) ;
	gr->textOut(320, 200, XAMGRAPH_SHORTDESC ) ;

	// lignes horizontales

	yline = 200 ;
	for ( i = 0 ; i < 5 ; i++ , yline += 10 ) {
		gr->setLineStyle( i ? XAM_SOLID_LINE : XAM_DASHED_LINE ) ;
		gr->setColor( gr->rgb( 255 - 32 * i, 0, 0 ) ) ;
		gr->line( 10 , yline, 630, yline ) ;
		gr->setLineThickness( 1 + i ) ;
	}

	// copyright

	gr->loadFont("../resources/Comic Sans MS.ttf", 14 ) ;
	gr->setColor( gr->stringToColor("#108010") ) ;
	gr->setTextStyle( XAM_ITALIC_TEXT ) ;
	gr->textOut(320, 280, XAMGRAPH_COPYRIGHT ) ;

	gr->setTextAlignment( XAM_ALIGNMENT_DEFAULT ) ;
	gr->setTextStyle( XAM_NORMAL_TEXT ) ;
	gr->defaultFont() ;

	// figures à gauche

	x = 80 ;
	y = 320 ;

	gr->setLineThickness( 2 ) ;
	gr->setColor( XAM_DARK_CYAN ) ;
	gr->regularPolygon(x, y, 80, 4, 45 ) ;
	gr->setColor( XAM_DARK_RED ) ;
	gr->regularPolygon(x, y, 50, 6, 60 ) ;

	gr->floodFill(x, y, XAM_GOLD ) ;

	// figures à droite

	x = 540 ;
	y = 320 ;

	gr->setColor( XAM_IVORY ) ;
	gr->filledCircle( x, y, 50 ) ;
	gr->setLineThickness( 2 ) ;
	gr->setColor( XAM_RED ) ;
	gr->circle( x, y, 50 ) ;

	gr->setColor( XAM_BLUE ) ;
	gr->setLineThickness( 1 ) ;
	gr->centeredRectangle(x, y, 150, 120 ) ;

	gr->setColor( XAM_RED ) ;
	gr->line(x, y - 70, x, y + 70 ) ;
	gr->line(x - 80, y - 2, x + 80, y - 2 ) ;
	gr->line(x - 80, y, x + 80, y ) ;
	gr->line(x - 80, y + 2, x + 80, y + 2 ) ;

	gr->setColor( XAM_YELLOW ) ;
	gr->line(x - 80, y - 1, x + 80, y - 1 ) ;
	gr->line(x - 80, y + 1, x + 80, y + 1 ) ;

	// mise à jour de la surface

	gr->updateScreen() ;
}

int main(/*int argc, char** argv*/)
{
	// initialisation

	gr = new XamGraph(640, 400, "XamGraph C++ test program" ) ;

	// préparation d'une deuxième image (logo SDL)

	gr->setDrawingArea( XAM_DRAWING_SCREEN ) ;
	gr->clearScreen( XAM_BLACK ) ;
	gr->setColor( XAM_WHITE ) ;
	gr->textOut(20, 20, "XamGraph is based on and powered by" ) ;
	gr->loadImage(90,60,"../resources/SDL_logo.png" ) ;
	gr->saveScreen() ;

	// couleur d'arrière-plan

	gr->setDrawingArea( XAM_DRAWING_BITMAP ) ;
	gr->clearScreen( XAM_LAVENDER ) ;
	gr->updateScreen() ;

	// dessin initial

	myDrawing() ;

	// boucle d'événements

	gr->connectKeyDownEvent( myKeyDownHandler ) ;
	gr->connectKeyUpEvent( myKeyUpHandler ) ;
	gr->connectMouseMotionEvent( myMouseMotionHandler ) ;
	gr->connectMouseButtonDownEvent( myMouseButtonDownHandler ) ;
	gr->connectMouseButtonUpEvent( myMouseButtonUpHandler ) ;

//	gr->connectTimerEvent( myTimerHandle, 500 ) ;

	gr->run() ;

	return 0 ;
}
