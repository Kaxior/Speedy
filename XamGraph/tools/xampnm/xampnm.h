/*  ---------------------------------------------------------------------------
 *  filename    :   xampnm.h
 *  description :   INTERFACE de la classe XamPNM (PNM: Portable aNy Map)
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

#ifndef XAMPNM_H
#define XAMPNM_H

/*
	PNM (Portable aNy Map)
	---------------------------------------------------------------------------
	PBM (Portable BitMap) : P1 (ASCII), P4 (binaire)
	PGM (Portable GrayMap): P2 (ASCII), P5 (binaire)
	PPM (Portable Pixmap) : P3 (ASCII), P6 (binaire)
*/

#include <iostream>
#include <fstream>
#include <string>
using namespace std ;

#include 	"_ABOUT"

#include <xamgraph.h>

class XamPNM {

  public:
	static string version() ;

	XamPNM(const char* filename ) ;
	XamPNM(int width, int height, bool isRgb = true ) ;
	~XamPNM() ;

	bool openFailed() const { return m_openFailed ; }

	void setDisplay(XamGraph* gr ) { this->gr = gr ; }
	int magicNumber() const { return m_id ; }
    string fileName() const { return m_filename ; }
    bool isRaw() const ;
    bool isRgb() const ;
    int maxVal() const { return m_maxval ; }
	int width() const { return m_width ; }
	int height() const { return m_height ; }

    Uint8 red(int x, int y ) const ;
	Uint8 green(int x, int y ) const ;
	Uint8 blue(int x, int y ) const ;
	Uint8 gray(int x, int y ) const ;
    Uint32 pixel(int x, int y )  const ;
	void setRed(int x, int y, Uint8 r ) ;
	void setGreen(int x, int y, Uint8 g ) ;
	void setBlue(int x, int y, Uint8 b ) ;
	void setGray(int x, int y, Uint8 v ) ;
    void setPixel(int x, int y, Uint8 r, Uint8 g, Uint8 b ) ;
    void setPixel(int x, int y, Uint32 color ) ;

    Uint8 normalize(int v ) ;

	void write(int zoom = 1 ) ;
	void read(int zoom = 1 ) ;

	int save(const char* filename, bool isRgb = true ) ;

	int spectrumMaxValue() ;
	int spectrum(int index ) ;
	int rSpectrum(int index ) ;
	int gSpectrum(int index ) ;
	int bSpectrum(int index ) ;

  protected:
	int load(const char* filename ) ;
	bool surfaceSize(int& w, int& h, const XamPNM* mask = 0 ) ;

  protected:
	XamGraph*	gr ;				// fenetre graphique de projection

  private:
    void initSpectrum() ;
    bool isOutOfRange(int x, int y ) const ;
  	int offset(int x, int y ) const ;

  private:
	string		m_filename ;		// nom du fichier image source
	bool		m_openFailed ;		// status chargement image
	int			m_id ;				// type PNM 1, 2, 3, 4, 5 ou 6
	int			m_width ;			// largeur
	int			m_height ;			// hauteur
	int			m_maxval ;			// profondeur

    Uint8*		m_raster ;			// matrice de pixels
	bool		m_isModified ;		// indique modif. dans raster

	int			m_spectrumMax ;		// valeur de spectre max.
    int		    m_rSpectrum[256] ;	// spectre composante R
	int		    m_gSpectrum[256] ;	// spectre composante G
	int         m_bSpectrum[256] ;	// spectre composante B
} ;

#endif
