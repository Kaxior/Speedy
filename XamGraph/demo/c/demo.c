/* 	programme de démonstration d'usage de XamGraph
	les ressources suivantes doivent être disponibles :

		../resources/XamGraph_logo.png
		../resources/SDL_logo.png
		../resources/Arial Black.ttf
		../resources/Comic Sans MS.ttf

	le Makefile utilisé est celui fourni par XamGraph.
*/

#include <xamgraph.h>

// effacement de la zone d'affichage d'info clavier

void eraseKeyInfo( void )
{
	xamSetLineThickness( 1 ) ;
	int wmax = xamTextWidth("mod/key : XXXX/XXXXXXXX") ;
	int hmax = xamTextHeight() ;
	xamSetColor( xamBackgroundColor() ) ;
	xamFilledRectangle(200, 350, wmax, hmax ) ;
}

// intercepteurs d'événements liés au clavier

void myKeyDownHandler(int key, int modifier )
{
	eraseKeyInfo() ;
	xamSetColor( XAM_BLACK ) ;
	xamPrintf(200, 350, "mod/key : %04X/%08X", modifier, key ) ;
	xamUpdateScreen() ;
}

void myKeyUpHandler(int key, int modifier )
{
	if ( key == 0x1b )	xamQuit() ;

	eraseKeyInfo() ;
	xamUpdateScreen() ;
}

// effacement de la zone d'affichage d'info souris/touchpad

void eraseMouseInfo( void )
{
	int wmax = xamTextWidth("button N (NNNN,NNNN)") ;
	int hmax = xamTextHeight() ;
	xamSetColor( xamBackgroundColor() ) ;
	xamFilledRectangle(200, 320, wmax, hmax ) ;
}

// intercepteurs d'événements souris/touchpad

void myMouseButtonDownHandler(int x, int y, int button )
{
	if ( button == 3 ) {
		xamRestoreScreen() ;
		return ;
	}

	eraseMouseInfo() ;
	xamSetColor( XAM_BLACK ) ;
	xamPrintf(200, 320, "button %d (%04d,%04d)", button, x, y ) ;
	xamUpdateScreen() ;
}

void myMouseButtonUpHandler(int x, int y, int button )
{
	eraseMouseInfo() ;
	xamUpdateScreen() ;
}


void myMouseMotionHandler(int x, int y )
{
	static int xmem = -1 , ymem = -1 ;
	int w = 64 ;
	int h = 32 ;

	xamSetWriteMode( XAM_MODE_XOR ) ;
	xamSetColor( XAM_WHITE ) ;

	if ( xmem != -1 )	xamFilledEllipse( xmem, ymem, w, h ) ;
	xamFilledEllipse( x, y, w, h ) ;

	xamSetWriteMode( XAM_MODE_COPY ) ;

	xmem = x ;
	ymem = y ;

	xamUpdateScreen() ;
}

// témoin temporel
/*
void myTimerHandle()
{
	static char s[] = "-\\|/X" ;
	static int  i = 0 ;
	s[4] = s[ i++ % 4 ] ;
	int wmax = xamTextWidth("X") ;
	int hmax = xamTextHeight() ;
	xamSetColor( xamBackgroundColor() ) ;
	xamFilledRectangle(5, 5, wmax, hmax ) ;
	xamSetColor( XAM_BLACK ) ;
	xamTextOut(5, 5, s + 4 ) ;
	xamUpdateScreen() ;
}
*/
// dessin initial

void myDrawing( void )
{
	int	i, x, y, yline ;

	// logo

	xamLoadImage(20,20,"../resources/XamGraph_logo.png" ) ;

	// version

	xamLoadFont("../resources/Arial Black.ttf", 20 ) ;
	xamSetTextAlignment( XAM_ALIGNMENT_CENTER ) ;
	xamSetColor( XAM_BLACK ) ;
	xamPrintf(320, 180, "version %s", XAMGRAPH_VERSION ) ;
	xamLoadFont("../resources/Arial Black.ttf", 16 ) ;
	xamSetColor( XAM_DARK_RED ) ;
	xamTextOut(320, 200, XAMGRAPH_SHORTDESC ) ;

	// lignes horizontales

	yline = 200 ;
	for ( i = 0 ; i < 5 ; i++ , yline += 10 ) {
		xamSetLineStyle( i ? XAM_SOLID_LINE : XAM_DASHED_LINE ) ;
		xamSetColor( xamRgb( 255 - 32 * i, 0, 0 ) ) ;
		xamLine( 10 , yline, 630, yline ) ;
		xamSetLineThickness( 1 + i ) ;
	}

	// copyright

	xamLoadFont("../resources/Comic Sans MS.ttf", 14 ) ;
	xamSetColor( xamStringToColor("#108010") ) ;
	xamSetTextStyle( XAM_ITALIC_TEXT ) ;
	xamTextOut(320, 280, XAMGRAPH_COPYRIGHT ) ;

	xamSetTextAlignment( XAM_ALIGNMENT_DEFAULT ) ;
	xamSetTextStyle( XAM_NORMAL_TEXT ) ;
	xamDefaultFont() ;

	// figures à gauche

	x = 80 ;
	y = 320 ;

	xamSetLineThickness( 2 ) ;
	xamSetColor( XAM_DARK_CYAN ) ;
	xamRegularPolygon(x, y, 80, 4, 45 ) ;
	xamSetColor( XAM_DARK_RED ) ;
	xamRegularPolygon(x, y, 50, 6, 60 ) ;

	xamFloodFill(x, y, XAM_GOLD ) ;

	// figures à droite

	x = 540 ;
	y = 320 ;

	xamSetColor( XAM_IVORY ) ;
	xamFilledCircle( x, y, 50 ) ;
	xamSetLineThickness( 2 ) ;
	xamSetColor( XAM_RED ) ;
	xamCircle( x, y, 50 ) ;

	xamSetColor( XAM_BLUE ) ;
	xamSetLineThickness( 1 ) ;
	xamCenteredRectangle(x, y, 150, 120 ) ;

	xamSetColor( XAM_RED ) ;
	xamLine(x, y - 70, x, y + 70 ) ;
	xamLine(x - 80, y - 2, x + 80, y - 2 ) ;
	xamLine(x - 80, y, x + 80, y ) ;
	xamLine(x - 80, y + 2, x + 80, y + 2 ) ;

	xamSetColor( XAM_YELLOW ) ;
	xamLine(x - 80, y - 1, x + 80, y - 1 ) ;
	xamLine(x - 80, y + 1, x + 80, y + 1 ) ;

	// mise à jour de la surface

	xamUpdateScreen() ;
}

int main(/*int argc, char** argv*/)
{
	// initialisation

	xamCreateGraph(640, 400, "XamGraph C test program" ) ;

	// préparation d'une deuxième image (logo SDL)

	xamSetDrawingArea( XAM_DRAWING_SCREEN ) ;
	xamClearScreen( XAM_BLACK ) ;
	xamSetColor( XAM_WHITE ) ;
	xamTextOut(20, 20, "XamGraph is based on and powered by" ) ;
	xamLoadImage(90, 60, "../resources/SDL_logo.png" ) ;
	xamSaveScreen() ;

	// couleur d'arrière-plan

	xamSetDrawingArea( XAM_DRAWING_BITMAP ) ;
	xamClearScreen( XAM_LAVENDER ) ;
	xamUpdateScreen() ;

	// dessin initial

	myDrawing() ;

	// boucle d'événements

	xamConnectKeyDownEvent( myKeyDownHandler ) ;
	xamConnectKeyUpEvent( myKeyUpHandler ) ;
	xamConnectMouseMotionEvent( myMouseMotionHandler ) ;
	xamConnectMouseButtonDownEvent( myMouseButtonDownHandler ) ;
	xamConnectMouseButtonUpEvent( myMouseButtonUpHandler ) ;

//	xamConnectTimerEvent( myTimerHandle, 500 ) ;

	xamRun() ;

	// fermeture

	xamCloseGraph() ;
	return 0 ;
}
