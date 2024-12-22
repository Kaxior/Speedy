/*  ---------------------------------------------------------------------------
 *  filename    :   xamgraphfont.c
 *  description :   iMPLEMENTATION C/C++
 *
 *	project     :	Trivial 2D Graphic Library based on SDL
 *  start date  :   février 2007
 *  ---------------------------------------------------------------------------
 *  Copyright 2007-2017 by Alain Menu   <alain.menu@ac-creteil.fr>
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

// references externes (C)
// ----------------------------------------------------------------------------

#ifndef	XAMGRAPHCLASS

extern	SDL_Surface*	m_screen ;			// current video surface
extern	int				m_fgColor ;			// couleur courante d'avant-plan
extern	TTF_Font*		m_font ;			// fonte courante
extern	int				m_textAlignment ;	// alignement du texte
extern	int				m_textStyle ;		// style du texte

void	_log(const char* mess ) ;

#endif

// default font as char array (C/C++)
// ----------------------------------------------------------------------------

#include "xamdefaultfont.c"

// Chargement de la fonte par défaut (police à largeur fixe, taille 16)
// ----------------------------------------------------------------------------

#ifdef	XAMGRAPHCLASS
bool XamGraph::defaultFont()
{
#else
bool xamDefaultFont( void )
{
#endif
	if ( m_font != NULL ) {
		TTF_CloseFont( m_font ) ;
		m_font = NULL ;
	}

	SDL_RWops* rw = SDL_RWFromMem(xamdefaultfont_ttf, xamdefaultfont_ttf_len ) ;
	m_font = TTF_OpenFontIndexRW(rw, 1, 16, 0 ) ;

#ifdef XAM_DEBUG
	if ( m_font == NULL ) {
		_log("XAM_ERROR : Enable to load default font\n" ) ;
	}
#endif
	return ( m_font != NULL ) ;
}

// Chargement d'une police de caractères (fichier TTF ou OTF)
// ----------------------------------------------------------------------------
/*
	Chargement d'une nouvelle police de caractères. Le chemin spécifié doit
	pointer sur un fichier de format .ttf/.otf valide. L'argument size spécifie
	la taille de la police. La fonte chargée devient la fonte active.
	La fonction retourne true en cas de succès, ou false en cas d'échec.
*/

#ifdef	XAMGRAPHCLASS
bool XamGraph::loadFont(const char* fileName, int size )
{
#else
bool xamLoadFont(const char* fileName, int size )
{
#endif
	if ( m_font != NULL ) {
		TTF_CloseFont( m_font ) ;
		m_font = NULL ;
	}
	m_font = TTF_OpenFont(fileName, size ) ;
#ifdef XAM_DEBUG
	if ( m_font == NULL ) {
		_log("XAM_ERROR : Enable to load font" ) ;
		_log(fileName ) ;
		_log("\n") ;
	}
#endif
	return ( m_font != NULL ) ;
}

// Affichage de texte
// ----------------------------------------------------------------------------
/*
	Affiche le texte passé par l'argument text aligné par rapport au point (x,y)
	spécifié. L'aspect du texte est tributaire de la fonte active, de la couleur
	d'avant-plan, et des contraintes d'alignement en cours.
*/

#ifdef	XAMGRAPHCLASS
void XamGraph::textOut( int x, int y, const char* text )
{
#else
void xamTextOut( int x, int y, const char* text )
{
#endif
	SDL_Color 		co ;
    SDL_Surface* 	surf ;
    int 			w, h ;
    SDL_Rect		rect ;

	if ( m_font == NULL ) {
#ifdef XAM_DEBUG
		puts("XAM_ERROR : No font loaded" ) ;
#endif
		return ;
	}

	co.r = XAM_R(m_fgColor) ;
	co.g = XAM_G(m_fgColor) ;
	co.b = XAM_B(m_fgColor) ;

    TTF_SizeText(m_font, text, &w, &h ) ;

    if ( m_textAlignment & XAM_ALIGNMENT_HCENTER )		rect.x = x - w / 2 ;
    else if ( m_textAlignment & XAM_ALIGNMENT_RIGHT )	rect.x = x - w ;
    else												rect.x = x ;

    if ( m_textAlignment & XAM_ALIGNMENT_VCENTER )		rect.y = y - h / 2 ;
    else if ( m_textAlignment & XAM_ALIGNMENT_BOTTOM )	rect.y = y - h ;
    else												rect.y = y ;

    TTF_SetFontStyle(m_font, m_textStyle ) ;

    surf = TTF_RenderText_Blended(m_font, text, co ) ;
    if ( surf != NULL ) {
        SDL_BlitSurface(surf, NULL, m_screen, &rect ) ;
        SDL_FreeSurface(surf ) ;
    }
}

// Affichag de texte formaté (standard printf() like...)
// ----------------------------------------------------------------------------
/*
	Fabrique une chaîne de caractères de la même manière que la fonction
	standard printf() puis affiche le texte aligné par rapport au point (x,y)
	spécifié. L'aspect du texte est tributaire de la fonte active, de la couleur
	d'avant-plan, et des contraintes d'alignement en cours. La taille de la
	chaîne fabriquée est retournée en résultat, elle ne doit pas dépasser
	255 caractères.
*/

#ifdef	XAMGRAPHCLASS
int XamGraph::printf( int x, int y, const char* format, ... )
{
#else
int xamPrintf( int x, int y, const char* format, ... )
{
#endif
	va_list	argptr ;
	char 	str[255] ;
	int 	cnt ;

	va_start( argptr, format ) ;

	cnt = vsnprintf( str, 255, format, argptr ) ; // vsprintf( str, format, argptr ) ;

#ifdef	XAMGRAPHCLASS
	textOut( x, y, str ) ;
#else
	xamTextOut( x, y, str ) ;
#endif

	va_end( argptr ) ;

	return cnt ;
}

// Alignement horizontal/vertical de texte
// ----------------------------------------------------------------------------
/*
	Détermine le type d'alignement du texte par rapport à la position
	d'affichage. L'argument alignment peut prendre une des valeurs :

	XAM_ALIGNMENT_DEFAULT : équivalent à XAM_ALIGNMENT_LEFT | XAM_ALIGNMENT_TOP
    XAM_ALIGNMENT_LEFT : justification à gauche
    XAM_ALIGNMENT_HCENTER : justification centrée horizontalement
    XAM_ALIGNMENT_RIGHT : justification à droite
    XAM_ALIGNMENT_TOP : alignement en haut
    XAM_ALIGNMENT_VCENTER : centrale vertical
    XAM_ALIGNMENT_BOTTOM : alignement en bas
    XAM_ALIGNMENT_CENTER : équ. à XAM_ALIGNMENT_HCENTER | XAM_ALIGNMENT_VCENTER
*/

#ifdef	XAMGRAPHCLASS
void XamGraph::setTextAlignment( int alignment )
{
#else
void xamSetTextAlignment( int alignment )
{
#endif
	m_textAlignment = alignment ;
}

// Style de texte normal, gras ou italique
// ----------------------------------------------------------------------------
/*
	Fixe le style d'affichage de la police courante. L'argument style est à
	choisir parmi :

	XAM_NORMAL_TEXT : texte normal (défaut)
	XAM_BOLD_TEXT : texte gras
	XAM_ITALIC_TEXT : texte en italique
*/

#ifdef	XAMGRAPHCLASS
void XamGraph::setTextStyle(int style )
{
#else
void xamSetTextStyle(int style )
{
#endif
	if ( style > 3 )	style = XAM_NORMAL_TEXT ;
	m_textStyle = style ;
}

// Hauteur de la fonte courante, en pixels
// ----------------------------------------------------------------------------
/*
	Retourne la hauteur maximale en pixels de la police de caractères courante.
*/

#ifdef	XAMGRAPHCLASS
//int XamGraph::getTextHeight( void )
int XamGraph::textHeight( void )
{
#else
//int xamGetTextHeight( void )
int xamTextHeight( void )
{
#endif
	if ( m_font == NULL )	return 0 ;
	return TTF_FontHeight(m_font ) ;
}

// Largeur du texte spécifié pour la fonte courante, en pixels
// ----------------------------------------------------------------------------
/*
	Retourne la largeur en pixels du texte spécifié, en fonction de la police
	de caractères active.
*/

#ifdef	XAMGRAPHCLASS
//int XamGraph::getTextWidth( const char* text )
int XamGraph::textWidth( const char* text )
{
#else
//int xamGetTextWidth( const char* text )
int xamTextWidth( const char* text )
{
#endif
	int w, h ;
	if ( m_font == NULL )	return 0 ;
	if ( TTF_SizeText(m_font, text, &w, &h ) == 0 )	return w ;
	return 0 ;
}
