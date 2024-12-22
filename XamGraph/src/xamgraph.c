/*  ---------------------------------------------------------------------------
 *  filename    :   xamgraph.c
 *  description :   iMPLEMENTATION C/C++
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

#include "xamgraph.h"

// déclarations globales (C) 
// ----------------------------------------------------------------------------

#ifndef XAMGRAPHCLASS
 
#ifdef XAMGRAPH_SDL2
SDL_Window*		m_window ;
SDL_Renderer*	m_renderer ;
SDL_Texture*	m_texture ;
#endif

bool			m_isInit = false ;
bool			m_quit = false ;

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

int				m_fgColor ;				// couleur courante d'avant-plan
int				m_bgColor ;				// couleur courante d'arriere-plan

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

#endif

// déclarations globales (C/C++)
// ----------------------------------------------------------------------------

static XamTimerHandler	m_timerHandler = NULL ;
static SDL_TimerID		m_timerId = 0 ;

// ressources privées (C/C++)
// ----------------------------------------------------------------------------

// journalisation (canal std d'erreur)

#ifdef	XAMGRAPHCLASS
void XamGraph::_log(const char* mess )								// private
{
	std::cerr << mess ;
#else
void 	_log(const char* mess )
{
	fprintf(stderr, "%s", mess ) ;
#endif
}

// digit hexa -> valeur décimale, cf. stringToColor()

#ifdef	XAMGRAPHCLASS
int	XamGraph::_hexDigitValue( char c )								// private
{
#else
int	_hexDigitValue( char c )
{
#endif
	if ( ( c >= '0' )&&( c <= '9' ) )	return c - '0' ;
	if ( ( c >= 'a' )&&( c <= 'f' ) )	return c - 'a' + 10 ;
	if ( ( c >= 'A' )&&( c <= 'F' ) )	return c - 'A' + 10 ;
	return 0 ;
}

// ressources privées (C)
// ----------------------------------------------------------------------------

#ifndef XAMGRAPHCLASS												// private

bool	_isInit( void ) { if ( !m_isInit ) _log("Init required !\n") ; return m_isInit ; }

#endif

// ----------------------------------------------------------------------------
// gestion de la fenêtre graphique
// ----------------------------------------------------------------------------

// constructeur
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
XamGraph::XamGraph( int width, int height, const char* title )
{
#else
void	xamCreateGraph( int width, int height, const char* title )
{
#endif

	m_quit = false ;

	m_video = NULL ;
	m_bmp = NULL ;
	m_save = NULL ;
	m_width = width ;
	m_height = height ;
	m_writeMode = XAM_MODE_COPY ;
	m_fgColor = XAM_BLACK ;
	m_bgColor = XAM_WHITE ;
	m_x = 0 ;
	m_y = 0 ;
	m_thickness = 1 ;
	m_style = XAM_SOLID_LINE ;
	m_keyUpHandler = NULL ;
	m_keyDownHandler = NULL ;
	m_motionHandler = NULL ;
	m_buttonDownHandler = NULL ;
	m_buttonUpHandler = NULL ;

	if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) == -1 ) {
		_log("SDL_Init() failed !\n" ) ; return ;
	}

	// surface physique (écran)

#ifdef XAMGRAPH_SDL2
	m_window = SDL_CreateWindow(title,
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             width, height,
                             SDL_WINDOW_SHOWN ) ;

//	SDL_SetWindowIcon(m_window, SDL_LoadBMP("icon.bmp") ) ;

	m_renderer = SDL_CreateRenderer(m_window, -1, 0 ) ;

 	m_texture = SDL_CreateTexture(m_renderer,
                                  SDL_PIXELFORMAT_RGBA32,
                                  SDL_TEXTUREACCESS_STREAMING,
                                  width, height ) ;

	m_video = SDL_CreateRGBSurface( 0, width, height, XAM_DEPTH, XAM_RMASK, XAM_GMASK, XAM_BMASK, XAM_AMASK ) ;
	m_bmp   = SDL_CreateRGBSurface( 0, width, height, XAM_DEPTH, XAM_RMASK, XAM_GMASK, XAM_BMASK, XAM_AMASK ) ;
	m_save  = SDL_CreateRGBSurface( 0, width, height, XAM_DEPTH, XAM_RMASK, XAM_GMASK, XAM_BMASK, XAM_AMASK ) ;
#else
	m_video = SDL_SetVideoMode( width, height, 0, SDL_HWSURFACE | SDL_DOUBLEBUF ) ;
	SDL_WM_SetCaption( title, NULL ) ;
//	SDL_WM_SetIcon( SDL_LoadBMP("icon.bmp"), NULL ) ;
	// surface mémoire
	m_bmp = SDL_CreateRGBSurface( SDL_SWSURFACE, width, height, XAM_DEPTH, XAM_RMASK, XAM_GMASK, XAM_BMASK, XAM_AMASK ) ;
	// plan de sauvegarde de l'écran
	m_save = SDL_CreateRGBSurface( SDL_SWSURFACE, width, height, XAM_DEPTH, XAM_RMASK, XAM_GMASK, XAM_BMASK, XAM_AMASK ) ;
#endif

	if ( m_video == NULL )	{ _log("VIDEO surface creation failed !\n" ) ; return ; }
	if ( m_bmp == NULL )	{ _log("SYSTEM surface creation failed !\n" ) ; return ; }
	if ( m_save == NULL )	{ _log("SAVE surface creation failed !\n" ) ; return ; }

	// surface courante de travail
	m_screen = m_video ;

	// fonte par défaut
	TTF_Init() ;
	m_font = NULL ;
	m_textAlignment = XAM_ALIGNMENT_DEFAULT ;
	m_textStyle = XAM_NORMAL_TEXT ;
#ifndef	XAMGRAPHCLASS
	xamDefaultFont() ;
#else
	defaultFont() ;
#endif

#ifdef XAM_DEBUG
//	SDL_PixelFormat* fmt = m_video->format ;
//	printf("video %d bits/pixel, ", fmt->BitsPerPixel ) ;
//	fmt = m_bmp->format ;
//	printf("bitmap %d bits/pixel\n", fmt->BitsPerPixel ) ;
//	printf("byte order : %s\n", SDL_BYTEORDER == SDL_BIG_ENDIAN ? "BigEndian" : "LittleEndian" ) ;
#endif

#ifndef	XAMGRAPHCLASS
	m_isInit = true ;
#endif
}

// destructeur
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
XamGraph::~XamGraph()
#else
int		xamCloseGraph( void )
#endif
{
	if ( m_bmp != NULL )	SDL_FreeSurface( m_bmp ) ;
	m_bmp = NULL ;
	if ( m_save != NULL )	SDL_FreeSurface( m_save ) ;
	m_save = NULL ;
	if ( m_video != NULL )	SDL_FreeSurface( m_video ) ;
	m_video = NULL ;

#ifdef XAMGRAPH_SDL2
	SDL_DestroyTexture( m_texture ) ;
	SDL_DestroyRenderer( m_renderer ) ;
	SDL_DestroyWindow( m_window ) ;
#endif

#ifdef	XAMGRAPHCLASS
	killTimer() ;
#else
	xamKillTimer() ;
	m_isInit = false ;
	return 0 ;
#endif
}

#ifdef	XAMGRAPHCLASS
void XamGraph::version()
{
#else
void xamVersion()
{
#endif
	_log("XamGraph version " ) ; _log( XAMGRAPH_VERSION ) ;
	_log(" - ") ; _log( XAMGRAPH_COPYRIGHT ) ; _log("\n" ) ;
}

// titre de la fenêtre
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::setTitle( const char* title )
{
#else
void xamSetTitle( const char* title )
{
	if ( !_isInit() )	return ;
#endif
#ifdef XAMGRAPH_SDL2
	SDL_SetWindowTitle( m_window, title ) ;
#else
	SDL_WM_SetCaption( title, NULL ) ;
#endif
}

// position de la fenêtre
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::setWindowPosition(int x, int y )
{
#else
void xamSetWindowPosition(int x, int y )
{
	if ( !_isInit() )	return ;
#endif
	SDL_SetWindowPosition(m_window, x, y ) ;
}

// taille de la fenêtre
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::resize(int width, int height )
{
#else
void xamResize(int width, int height )
{
	if ( !_isInit() )	return ;
#endif
#ifdef XAMGRAPH_SDL2
	SDL_SetWindowSize( m_window, width, height ) ;

	SDL_DestroyTexture( m_texture ) ;
	m_texture = SDL_CreateTexture(m_renderer,
                                  SDL_PIXELFORMAT_RGBA32,
                                  SDL_TEXTUREACCESS_STREAMING,
                                  width, height ) ;
#endif
	SDL_FreeSurface( m_bmp ) ;
 	m_bmp = SDL_CreateRGBSurface( 0, width, height, XAM_DEPTH, XAM_RMASK, XAM_GMASK, XAM_BMASK, XAM_AMASK ) ;
	SDL_FreeSurface( m_video ) ;
#ifdef XAMGRAPH_SDL2
	m_video = SDL_CreateRGBSurface( 0, width, height, XAM_DEPTH, XAM_RMASK, XAM_GMASK, XAM_BMASK, XAM_AMASK ) ;
#else
	m_video = SDL_SetVideoMode( width, height, 0, SDL_HWSURFACE | SDL_DOUBLEBUF ) ;
#endif

//	SDL_Event event ;
//	SDL_PollEvent(&event) ;
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
int	 XamGraph::maxX() const
{
#else
int	xamMaxX( void )
{
#endif
	return m_width - 1 ;
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
int XamGraph::maxY() const
{
#else
int xamMaxY( void )
{
#endif
	return m_height - 1 ;
}

// gestion de la surface de dessin active
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::setDrawingArea( int drawing )
{
#else
void xamSetDrawingArea( int drawing )
{
	if ( !_isInit() )	return ;
#endif
	if ( drawing == XAM_DRAWING_BITMAP )	m_screen = m_bmp ;
	else									m_screen = m_video ;

	m_drawingColor = SDL_MapRGBA( m_screen->format, XAM_R(m_fgColor), XAM_G(m_fgColor), XAM_B(m_fgColor), XAM_A(m_fgColor) ) ;
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
int	 XamGraph::drawingArea() const
{
#else
int	xamDrawingArea( void )
{
	if ( !_isInit() )	return -1 ;
#endif
	if ( m_screen == m_bmp )	return XAM_DRAWING_BITMAP ;
	return XAM_DRAWING_SCREEN ;
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::clearScreen(Uint32 color )
{
#else
void xamClearScreen(Uint32 color )
{
	if ( !_isInit() )	return ;
#endif
	m_bgColor = color | 0x000000FF ;	// opaque
	Uint32 c = SDL_MapRGBA( m_screen->format, XAM_R(color), XAM_G(color), XAM_B(color), XAM_A(color) ) ;
	SDL_FillRect( m_screen, NULL, c ) ;

	if ( m_screen == m_video ) {
#ifdef XAMGRAPH_SDL2
		SDL_UpdateTexture(m_texture, NULL, m_video->pixels, m_video->pitch ) ;
		SDL_RenderClear(m_renderer ) ;
		SDL_RenderCopy(m_renderer, m_texture, NULL, NULL ) ;
		SDL_RenderPresent(m_renderer ) ;
#else
		SDL_Flip( m_video ) ;
#endif
	}
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::updateScreen()
{
#else
void xamUpdateScreen( void )
{
	if ( !_isInit() )	return ;
#endif
	if ( m_screen == m_bmp )	SDL_BlitSurface( m_bmp, NULL, m_video, NULL ) ;

#ifdef XAMGRAPH_SDL2
	SDL_UpdateTexture(m_texture, NULL, m_video->pixels, m_video->pitch ) ;
	SDL_RenderClear(m_renderer ) ;
	SDL_RenderCopy(m_renderer, m_texture, NULL, NULL ) ;
	SDL_RenderPresent(m_renderer ) ;
#else
	SDL_Flip( m_video ) ;
#endif
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::saveScreen()
{
#else
void xamSaveScreen( void )
{
	if ( !_isInit() )	return ;
#endif
	SDL_BlitSurface( m_video, NULL, m_save, NULL ) ;
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::restoreScreen()
{
#else
void xamRestoreScreen( void )
{
	if ( !_isInit() )	return ;
#endif
	SDL_BlitSurface( m_save, NULL, m_video, NULL ) ;
#ifdef XAMGRAPH_SDL2
	SDL_UpdateTexture(m_texture, NULL, m_video->pixels, m_video->pitch ) ;
	SDL_RenderClear(m_renderer ) ;
	SDL_RenderCopy(m_renderer, m_texture, NULL, NULL ) ;
	SDL_RenderPresent(m_renderer ) ;
#else
	SDL_Flip( m_video ) ;
#endif
}

// mode d'écriture
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::setWriteMode( int mode )
{
#else
void xamSetWriteMode( int mode )
{
#endif
	if ( mode != XAM_MODE_XOR )	mode = XAM_MODE_COPY ;
	m_writeMode = mode ;
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
int XamGraph::writeMode() const
{
#else
int xamWriteMode( void )
{
#endif
	return m_writeMode ;
}

// ----------------------------------------------------------------------------
// gestion des couleurs
// ----------------------------------------------------------------------------

// manipulations de couleurs 32 bits, codage 0xRRGGBBAA
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
Uint32 XamGraph::rgb( Uint8 r, Uint8 g, Uint8 b )
{
#else
Uint32 xamRgb( Uint8 r, Uint8 g, Uint8 b )
{
#endif
	return XAM_COLOR24(r, g, b ) ;
}

#ifdef	XAMGRAPHCLASS
Uint32 XamGraph::rgba( Uint8 r, Uint8 g, Uint8 b, Uint8 a )
{
#else
Uint32 xamRgba( Uint8 r, Uint8 g, Uint8 b, Uint8 a )
{
#endif
	return XAM_COLOR32(r, g, b, a ) ;
}

// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
Uint8 XamGraph::red( Uint32 color ) const
{
#else
Uint8 xamRed( Uint32 color )
{
#endif
	return XAM_R( color ) ;
}

#ifdef	XAMGRAPHCLASS
Uint8	XamGraph::green( Uint32 color ) const
{
#else
Uint8 xamGreen( Uint32 color )
{
#endif
	return XAM_G( color ) ;
}

#ifdef	XAMGRAPHCLASS
Uint8	XamGraph::blue( Uint32 color ) const
{
#else
Uint8 xamBlue( Uint32 color )
{
#endif
	return XAM_B( color )  ;
}

#ifdef	XAMGRAPHCLASS
Uint8 XamGraph::alpha( Uint32 color ) const
{
#else
Uint8 xamAlpha( Uint32 color )
{
#endif
	return XAM_A( color ) ;
}

// formats "#rrggbb" (opaque) ou "#rrggbbaa" --> codage 32 bits 0xRRGGBBAA
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
Uint32 XamGraph::stringToColor( const char* str )
{
#else
Uint32 xamStringToColor( const char* str )
{
#endif
	Uint8 r, g, b, a = 0xFF ;

	if ( str[0] != '#' )		return 0 ;
	if ( strlen( str ) < 7 )	return 0 ;

	r = _hexDigitValue( str[1] ) * 16 + _hexDigitValue( str[2] ) ;
	g = _hexDigitValue( str[3] ) * 16 + _hexDigitValue( str[4] ) ;
	b = _hexDigitValue( str[5] ) * 16 + _hexDigitValue( str[6] ) ;
	if ( strlen( str ) == 9 ) {
		a = _hexDigitValue( str[7] ) * 16 + _hexDigitValue( str[8] ) ;
	}

#ifdef	XAMGRAPHCLASS
	return rgba(r, g, b, a ) ;
#else
	return xamRgba(r, g, b, a ) ;
#endif
}

// couleur courante d'avant-plan codée 0xRRGGBBAA
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::setColor( Uint32 color )
{
#else
void xamSetColor( Uint32 color )
{
	if ( !_isInit() )	return ;
#endif
	m_fgColor = color ;
	m_drawingColor = SDL_MapRGBA( m_screen->format, XAM_R(m_fgColor), XAM_G(m_fgColor), XAM_B(m_fgColor), XAM_A(m_fgColor) ) ;
}

#ifdef	XAMGRAPHCLASS
Uint32 XamGraph::color() const
{
#else
Uint32 xamColor( void )
{
#endif
	return m_fgColor ;
}

// couleur courante d'arrière-plan codée 0xRRGGBBAA avec AA = 0xFF (opaque)
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
Uint32 XamGraph::backgroundColor() const
{
#else
Uint32 xamBackgroundColor( void )
{
#endif
	return m_bgColor ;
}

// ----------------------------------------------------------------------------
// primitives de dessin, styles, textes et images
// ----------------------------------------------------------------------------

// cf. xamgraphdraw.c

// ----------------------------------------------------------------------------
// boucle d'événements clavier/souris
// ----------------------------------------------------------------------------

// intercepteurs
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::connectKeyUpEvent( XamKeyboardHandler handler )
#else
void xamConnectKeyUpEvent( XamKeyboardHandler handler )
#endif
{
	m_keyUpHandler = handler ;
}

#ifdef	XAMGRAPHCLASS
void XamGraph::connectKeyDownEvent( XamKeyboardHandler handler )
#else
void xamConnectKeyDownEvent( XamKeyboardHandler handler )
#endif
{
	m_keyDownHandler = handler ;
}

#ifdef	XAMGRAPHCLASS
void XamGraph::connectMouseMotionEvent( XamMouseHandler handler )
#else
void xamConnectMouseMotionEvent( XamMouseHandler handler )
#endif
{
	m_motionHandler = handler ;
}

#ifdef	XAMGRAPHCLASS
void XamGraph::connectMouseButtonDownEvent( XamMouseButtonHandler handler )
#else
void xamConnectMouseButtonDownEvent( XamMouseButtonHandler handler )
#endif
{
	m_buttonDownHandler = handler ;
}

#ifdef	XAMGRAPHCLASS
void XamGraph::connectMouseButtonUpEvent( XamMouseButtonHandler handler )
#else
void xamConnectMouseButtonUpEvent( XamMouseButtonHandler handler )
#endif
{
	m_buttonUpHandler = handler ;
}

// boucle d'événements
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::run()
{
#else
void xamRun( void )
{
	if ( !_isInit() )	return ;
#endif

	bool running = true ;
	m_quit = false ;		// new 0.61 (pb sous Ubuntu...)

	while ( running ) {

		SDL_Event event ;

		if ( m_quit )
			running = false ;

		else if ( SDL_WaitEvent(&event) ) {
			switch ( event.type ) {

			case SDL_KEYDOWN :			
#ifdef	XAMGRAPHCLASS
				onKeyDown(event.key.keysym.sym, event.key.keysym.mod ) ;				// new v0.5
#endif				
				if ( m_keyDownHandler == NULL )	break ;
				(*m_keyDownHandler)(event.key.keysym.sym, event.key.keysym.mod ) ;
				break ;
				
			case SDL_KEYUP :
#ifdef	XAMGRAPHCLASS				
				onKeyUp(event.key.keysym.sym, event.key.keysym.mod ) ;					// new v0.5
#endif				
				if ( m_keyUpHandler == NULL )	break ;
				(*m_keyUpHandler)(event.key.keysym.sym, event.key.keysym.mod ) ;
				break ;

			case SDL_MOUSEMOTION :
#ifdef	XAMGRAPHCLASS
				onMouseMove(event.motion.x, event.motion.y ) ;							// new v0.5
#endif
				if ( m_motionHandler == NULL )	break ;
				(*m_motionHandler)(event.motion.x, event.motion.y ) ;
				break ;

			case SDL_MOUSEBUTTONDOWN :
#ifdef	XAMGRAPHCLASS				
				onMouseButtonDown(event.button.x, event.button.y, event.button.button ) ;	//new v0.5
#endif				
				if ( m_buttonDownHandler == NULL )	break ;
				(*m_buttonDownHandler)(event.button.x, event.button.y, event.button.button ) ;
				break ;

			case SDL_MOUSEBUTTONUP :
#ifdef	XAMGRAPHCLASS				
				onMouseButtonUp(event.button.x, event.button.y, event.button.button ) ;	//new v0.5
#endif				
				if ( m_buttonUpHandler == NULL )	break ;
				(*m_buttonUpHandler)(event.button.x, event.button.y, event.button.button ) ;
				break ;
				
			case SDL_QUIT :
				running = false ;
				break ;
			}
		}
	}
}

// sortie de la boucle d'événements
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::quit()
{
#else
void xamQuit( void )
{
	if ( !_isInit() )	return ;
#endif
	m_quit = true ;
}

// ----------------------------------------------------------------------------
// gestion du temps
// ----------------------------------------------------------------------------

// horloge périodique
// ----------------------------------------------------------------------------

Uint32 timerCallback(Uint32 interval, void* param )
{
#ifdef	XAMGRAPHCLASS
	if ( param != nullptr )((XamGraph*)param)->onTimeout() ;	// new v 0.5
#endif	
	if ( m_timerHandler != NULL )(	// new v 0.5
		*m_timerHandler)() ;
    return interval ;
}

#ifdef	XAMGRAPHCLASS
void XamGraph::connectTimerEvent( XamTimerHandler handler, int period )
#else
void xamConnectTimerEvent( XamTimerHandler handler, int period )
#endif
{
	m_timerHandler = handler ;
	if ( m_timerId != 0 )	SDL_RemoveTimer( m_timerId ) ;
	m_timerId = SDL_AddTimer(period, timerCallback, NULL ) ;
}

#ifdef	XAMGRAPHCLASS
void XamGraph::startTimer(XamGraph* obj, int period )		// new v0.5
{
	if ( m_timerId != 0 )	SDL_RemoveTimer( m_timerId ) ;
	m_timerId = SDL_AddTimer(period, timerCallback, obj ) ;
}
#endif

#ifdef	XAMGRAPHCLASS
void XamGraph::killTimer()
#else
void xamKillTimer( void )
#endif
{
	if ( m_timerId != 0 )	SDL_RemoveTimer( m_timerId ) ;
	m_timerId = 0 ;
}

// temporisation
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
void XamGraph::delay( int msec )
{
#else
void xamDelay( int msec )
{
	if ( !_isInit() )	return ;
#endif
	SDL_Delay( msec ) ;
}
