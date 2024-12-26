/*  ---------------------------------------------------------------------------
 *  filename    :   xamgraphclass.h
 *  description :   C++ library header - INTERFACE classe XamGraph
 *
 *	project     :	Trivial 2D Graphic Library based on SDL
 *  start date  :   février 2007
 *  ---------------------------------------------------------------------------
 *  Copyright 2007-2024 by Alain Menu   <alain.menu@ac-creteil.fr>
 *
 *  This file is part of "XamGraph"
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

#ifndef XAMGRAPHCLASS_H
#define XAMGRAPHCLASS_H

class XamGraph
{
  public:

    // gestion de la fenêtre graphique

  	XamGraph(int width = 640, int height = 480, const char* title = XAMGRAPH_VERSION ) ;
  	virtual ~XamGraph() ;

	static void	version() ;
	
    void    setTitle( const char* title ) ;
    void    resize(int width, int height ) ;
    int     maxX() const ;
    int     maxY() const ;

	void 	setWindowPosition(int x, int y ) ;
	
    void	setDrawingArea( int drawing = XAM_DRAWING_SCREEN ) ;
    int		drawingArea() const ;
    void	clearScreen(Uint32 color = XAM_WHITE ) ;
	void	updateScreen() ;
	void	saveScreen() ;
	void	restoreScreen() ;
    void	setWriteMode( int mode = XAM_MODE_COPY ) ;
    int     writeMode() const ;

    // gestion des couleurs

    Uint32	rgb( Uint8 r, Uint8 g, Uint8 b ) ;
    Uint32  rgba( Uint8 r, Uint8 g, Uint8 b, Uint8 a ) ;
    Uint8	red( Uint32 color ) const ;
	Uint8 	green( Uint32 color ) const ;
	Uint8 	blue( Uint32 color ) const ;
    Uint8 	alpha( Uint32 color ) const ;
    Uint32	stringToColor( const char* str ) ;

    void	setColor( Uint32 color ) ;         // foreground color
    Uint32	color() const ;
    Uint32	backgroundColor() const ;          // background color

    // primitives de dessin

    void	putPixel( int x, int y, Uint32 color ) ;
	Uint32	getPixel( int x, int y ) ;

    int		currentX() const ;
	int		currentY() const ;
	void	moveRel( int dx, int dy ) ;
	void	moveTo( int x, int y ) ;

	void	point(int x, int y ) ;

	void	line( int x1, int y1, int x2, int y2 ) ;
	void	lineRel( int dx, int dy ) ;
	void	lineTo( int x, int y ) ;

	void	polygon( int numPoints, int* points ) ;
	void	filledPolygon( int numPoints, int* points ) ;

	void	regularPolygon( int x, int y, int radius, int faces, int angle = 0 ) ;
	void	filledRegularPolygon( int x, int y, int radius, int faces, int angle = 0 ) ;

	void 	rectangle( int x, int y, int w, int h ) ;
	void 	filledRectangle( int x, int y, int w, int h ) ;

	void 	centeredRectangle( int x, int y, int w, int h ) ;
	void 	filledCenteredRectangle( int x, int y, int w, int h ) ;

	void	circle( int x, int y, int radius ) ;
	void	filledCircle( int x, int y, int radius ) ;

	void	ellipse( int x, int y, int xRadius, int yRadius ) ;
	void	filledEllipse( int x, int y, int xRadius, int yRadius ) ;

    void 	floodFill( int x, int y, Uint32 color ) ;

    // styles pour points, lignes et polygones

	void	setLineThickness( int thickness = 1 ) ;
	int 	lineThickness() const ;

	void	setLineStyle( int style = XAM_SOLID_LINE ) ;
	int 	lineStyle() const ;

    // affichage de textes

	bool 	loadFont(const char* fileName, int size ) ;
	bool 	defaultFont() ;
	void 	textOut( int x, int y, const char* text ) ;
	int 	printf( int x, int y, const char* format, ... ) ;
	void 	setTextAlignment( int alignment = XAM_ALIGNMENT_DEFAULT ) ;
	void 	setTextStyle(int style = XAM_NORMAL_TEXT ) ;
    int 	textHeight() ;
	int 	textWidth( const char* text ) ;

    // chargement d'images

	bool	loadImage( int x, int y, const char* fileName ) ;

    // boucle d'événements clavier/souris

	void	connectKeyDownEvent( XamKeyboardHandler handler ) ;
	void	connectKeyUpEvent( XamKeyboardHandler handler ) ;
	void	connectMouseMotionEvent( XamMouseHandler handler ) ;
	void	connectMouseButtonDownEvent( XamMouseButtonHandler handler ) ;
	void	connectMouseButtonUpEvent( XamMouseButtonHandler handler ) ;
    void	run() ;
	void	quit() ;

	// new v0.5
	virtual void onMouseMove(int x, int y ) {}
	virtual void onKeyDown(int key, int modifier ) {}
	virtual void onKeyUp(int key, int modifier ) {}
	virtual void onMouseButtonDown(int x, int y, int button ) {}
	virtual void onMouseButtonUp(int x, int y, int button ) {}
	
    // gestion du temps

	void 	connectTimerEvent( XamTimerHandler handler, int period ) ;
	void 	killTimer() ;
	void	delay( int msec ) ;

	// new v0.5
	void	startTimer(XamGraph* obj, int period ) ;
	virtual void onTimeout(void) {}
	
  private:

  	int*	_allocate(int n ) ;
  	void 	_free(int* p ) ;

  	SDL_Surface* _createTransparentLayer(int w, int h ) ;
  	void 	_blitAndFreeTransparentLayer(int x, int y ) ;

  	int 	_lock(SDL_Surface* surf ) ;
  	void 	_unlock(SDL_Surface* surf ) ;

  	Uint32 	_getPixel(SDL_Surface* surf, int x, int y ) ;
  	void 	_putPixel(SDL_Surface* surf, int x, int y, Uint32 c ) ;

  	void 	_floodFill(SDL_Surface* surf, int x, int y, Uint32 oldc, Uint32 newc ) ;

  	SDL_Rect _getPolygonRect(int n, int* p ) ;

  	void 	_drawPoint(SDL_Surface* surf, int x, int y, Uint32 c ) ;
  	void 	_drawLine(SDL_Surface* surf, int x1, int y1, int x2, int y2, Uint32 c ) ;
  	void 	_drawPolygon(SDL_Surface* surf, int n, int* p, Uint32 c, bool filled ) ;
  	void 	_drawRectangle(SDL_Surface* surf, int x, int y, int w, int h, Uint32 c, bool filled ) ;
  	void 	_drawCircle(SDL_Surface* surf, int x, int y, int r, Uint32 c, bool filled ) ;
  	void 	_drawEllipse(SDL_Surface* surf, int x, int y, int a, int b, Uint32 c, bool filled ) ;

  	int		_hexDigitValue( char c ) ;

  	static void	_log(const char* mess ) ;

  private:

#ifdef XAMGRAPH_SDL2
  	SDL_Window*		m_window ;              // graphic window
  	SDL_Renderer*	m_renderer ;            // window renderer
  	SDL_Texture*	m_texture ;             // active GPU surface
#endif

  	SDL_Surface*	m_video ;				// surface in video memory
  	SDL_Surface*	m_bmp ;					// surface in system memory
  	SDL_Surface*	m_save ;				// surface in system memory
  	SDL_Surface*	m_screen ;				// current video surface
  	SDL_Surface*	m_transparentSurf ;		// temporary transparent layer
  	Uint32			m_transparentColor ;	// SDL transparent color for temporary layer
  	Uint32			m_drawingColor ;		// SDL current foreground color for active surface

  	int				m_width ;				// largeur utile
  	int				m_height ;				// hauteur utile

  	int				m_x ;					// abscisse courante de dessin
  	int				m_y ;					// ordonnee courante de dessin

  	int				m_writeMode ;			// mode d'affichage ( copy | xor )

    Uint32			m_fgColor ;				// couleur courante d'avant-plan
  	Uint32			m_bgColor ;				// couleur courante d'arriere-plan

  	int				m_thickness ;			// epaisseur de trace des lignes
  	int				m_style ;				// type de trace des lignes

  	XamKeyboardHandler		m_keyUpHandler ;
  	XamKeyboardHandler		m_keyDownHandler ;
  	XamMouseHandler			m_motionHandler ;
  	XamMouseButtonHandler	m_buttonDownHandler ;
  	XamMouseButtonHandler	m_buttonUpHandler ;

  	TTF_Font*		m_font ;				// fonte courante
  	int				m_textAlignment ;		// alignement du texte
  	int				m_textStyle ;			// style du texte

  	bool			m_quit ;
  	
	// new v0.6 : undocumented
  public :
#ifdef XAMGRAPH_SDL2
  	SDL_Window*		_window() const { return m_window ; }		// graphic window
  	SDL_Renderer*	_renderer() const { return m_renderer ; }	// window renderer
  	SDL_Texture*	_texture() const { return m_texture ; }		// active GPU surface
#endif
  	SDL_Surface*	_video() { return m_video ; }				// surface in video memory

} ;

#endif
