/*  ---------------------------------------------------------------------------
 *  filename    :   xampnm.cpp
 *  description :   IMPLEMENTATION de la classe XamPNM (PNM: Portable aNy Map)
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

#include 	<xamgraph.h>
#include	"xampnm.h"

string XamPNM::version() { return XAMPNM_VERSION ; }

XamPNM::XamPNM(const char* filename )
	: gr( NULL )
	, m_openFailed( false )
	, m_id( 0 )
	, m_maxval( 255 )
	, m_raster( NULL )
	, m_isModified( true )
{
	load(filename ) ;
}

XamPNM::XamPNM(int width, int height, bool isRgb )
	: gr( NULL )
	, m_openFailed( false )
	, m_width( width )
	, m_height( height )
	, m_maxval( 255 )
	, m_raster( NULL )
	, m_isModified( true )
{
	m_filename = "noname" ;
	m_id = ( isRgb ? 6 : 5 ) ;		// PPM | PGM raw

	int size = m_width * m_height * 3 ;
	m_raster = new Uint8[ size ] ;
	if ( m_raster == NULL ) {
		cerr << "XamPNM::XamPNM(): raster allocation failed..." << endl ;
		return ;
	}
	for ( int i = 0 ; i < size ; i++ )	m_raster[i] = 0 ;
}

XamPNM::~XamPNM()
{
	if ( m_raster != NULL )	delete [] m_raster ;
}

// file -> raster

int XamPNM::load(const char* filename )
{
	ifstream 	in ;
	char		buf[80] ;

	in.open( filename ) ;					// ouverture
	if ( !in ) {
		cerr << "XamPNM::load(): file open failed..." << endl ;
		m_openFailed = true ;
		return -1 ;
	}
	m_filename.assign( filename ) ;			// memorisation nom de fichier

	do { 									// recherche indicatif P..
		in.getline(buf,80) ;
	} while ( buf[0] == '#' ) ;

	if (( buf[0] != 'P' )&&( buf[0] != 'p' )) {
		cerr << "XamPNM::load(): bad PNM file format..." << endl ;
		m_openFailed = true ;
		in.close() ;
		return -2 ;
	}
	m_id = (int)( buf[1] - '0' ) ;			// magic number

	do { 									// dimensions de l'image
		in.getline(buf,80) ;
	} while ( buf[0] == '#' ) ;
	sscanf(buf, "%d %d", &m_width, &m_height ) ;

	if (( m_id != 1 )&&( m_id != 4 )) {
		do { 								// profondeur de l'image
			in.getline(buf,80) ;
		} while ( buf[0] == '#' ) ;
		sscanf(buf, "%d", &m_maxval ) ;
		if ( m_maxval > 255 ) {
			cerr << "XamPNM::load(): PNM depth not implemented..." << endl ;
			m_openFailed = true ;
			in.close() ;
			return -4 ;
		}
	}

	int size = width() * height() * 3 ;		// création 'raster'
	m_raster = new Uint8[ size ] ;
	if ( m_raster == NULL ) {
		cerr << "XamPNM::load(): raster allocation failed..." << endl ;
		m_openFailed = true ;
		in.close() ;
		return -5 ;
	}

	switch ( m_id ) {						// lecture de la 'map'

	case 1 : // PBM plain, 1 pixel = 1 valeur codée ASCII '0' ou '1' (sans séparateurs)
		for (int y = 0 ; y < height() ; y++ ) {
			for ( int x = 0 ; x < width() ; x++ ) {
				do {
					in.read( buf, 1 ) ;
				} while ( (buf[0] != '0')&&( buf[0] != '1') ) ;
				setGray(x, y, ( buf[0] == '1' ? 0 : m_maxval ) ) ;
			}
		}
		break ;
	case 2 : // PGM plain, 1 pixel = 1 valeur codée ASCII '0'..'255' (avec séparateurs)
		for (int y = 0 ; y < height() ; y++ ) {
			for ( int x = 0 ; x < width() ; x++ ) {
				in >> buf ;
				setGray(x, y, (Uint8)atoi( buf ) * 255 / (float)m_maxval ) ;
			}
		}
		break ;
	case 3 : // PPM plain, 1 pixel = 3 valeurs codées ASCII '0'..m_maxval (avec séparateurs)
		for ( int i = 0 ; i < size ; i++ ) {
			in >> buf ;
			m_raster[i] = (Uint8)atoi( buf ) * 255 / (float)m_maxval ;
		}
		break ;
	case 4 : // PBM raw, 1 pixel = 1 bit -> R = G = B = 0 | 255
		for (int y = 0 ; y < height() ; y++ ) {
			for ( int x = 0 ; x < width() ; x++ ) {
				if ( x % 8 == 0 )	in.read( buf, 1 ) ;
				setGray(x, y, ( (Uint8)buf[0] >> ( 7 - x % 8 ) ) & 1 ? 0 : m_maxval ) ;
			}
		}
		break ;
	case 5 : // PGM raw, 1 pixel = 1 octet -> R = G = B = 0..m_maxval
		for (int y = 0 ; y < height() ; y++ ) {
			for ( int x = 0 ; x < width() ; x++ ) {
				in.read( buf, 1 ) ;
				setGray(x, y, (Uint8)buf[0] * 255 / (float)m_maxval ) ;
			}
		}
		break ;
	case 6 : // PPM raw, 1 pixel = 3 octets -> R, G, B = 0..m_maxval
		in.read( (char *)m_raster, size ) ;
		if ( m_maxval != 255 ) {
			for ( int i = 0 ; i < size ; i++ )	m_raster[i] *= 255 / (float)m_maxval ;
		}
		break ;
	default :
		cerr << "XamPNM::load(): unexpected PNM magic number..." << endl ;
		m_openFailed = true ;
		in.close() ;
		return -3 ;
	}

	in.close() ;
	m_isModified = true ;
	return 0 ;
}

// raster -> file PGM | PPM raw

int XamPNM::save(const char* filename, bool isRgb )
{
	const char*	p = filename ;			// suppression des eventuels
	while ( isspace( *p ) )	p++ ;		// separateurs de tete

	string	file( p ) ;					// ajout de l'extension
	if ( file.empty() ) {
		cerr << "XamPNM::save(): file name required..." << endl ;
		return -1 ;
	}
	if ( isRgb )	file += ".ppm" ;
	else			file += ".pgm" ;

	ofstream	out ;					// ouverture
	out.open( file.c_str() ) ;
	if ( !out ) {
		cerr << "XamPNM::save(): open failed..." << endl ;
		return -1 ;
	}
										// ecriture de l'entete
	char buf[80] ;
	snprintf(buf, 79, "P%c\n", isRgb ? '6' : '5' ) ;
	out.write( buf, strlen(buf) ) ;
	snprintf(buf, 79, "# %s v%s\n", XAMPNM_TITLE, XAMPNM_VERSION ) ;
	out.write( buf, strlen(buf) ) ;
	snprintf( buf, 79, "%d %d\n%d\n", width(), height(), m_maxval ) ;
	out.write( buf, strlen(buf) ) ;
										// ecriture de la matrice

	int size = width() * height() * 3 ;
	if ( isRgb ) {
		out.write( (char *)m_raster, size ) ;
	}
	else {
		for (int y = 0 ; y < height() ; y++ ) {
			for ( int x = 0 ; x < width() ; x++ ) {
				char v = gray(x, y ) ;
				out.write( &v, 1 ) ;
			}
		}
	}
	out.put('\n') ;

	out.close() ;						// fermeture
	m_filename = file ;
	return 0 ;
}

// raster -> display

void XamPNM::write(int zoom )
{
	if ( gr == NULL )	return ;

	for (int y = 0 ; y < height() ; y++ ) {
		for ( int x = 0 ; x < width() ; x++ ) {
			int r = m_raster[ offset(x, y ) ] ;
			int g = m_raster[ offset(x, y ) + 1 ] ;
			int b = m_raster[ offset(x, y ) + 2 ] ;
			if ( zoom == 1 )
				gr->putPixel( x, y, gr->rgb(r, g, b ) ) ;
			else {
				gr->setColor( gr->rgb(r, g, b ) ) ;
				gr->filledRectangle(x * zoom, y * zoom, zoom, zoom ) ;
			}
		}
	}
}

// display -> raster

void XamPNM::read(int zoom )
{
	if ( gr == NULL )	return ;

	for (int y = 0 ; y < height() ; y++ ) {
		for ( int x = 0 ; x < width() ; x++ ) {
			int pix = gr->getPixel( x * zoom, y * zoom ) ;
			setRed(x, y, gr->red( pix ) ) ;
			setGreen(x, y, gr->green( pix ) ) ;
			setBlue(x, y, gr->blue( pix ) ) ;
		}
	}
}

// taille max. de l'image entre raster et autre image XamPNM
// ou entre raster et display qd mask nul (défaut)

bool XamPNM::surfaceSize(int& w, int& h, const XamPNM* mask )
{
	if ( !mask ) {
		if ( gr == NULL )	return false ;
		w = std::min( this->width(), gr->maxX() + 1 ) ;
		h = std::min( this->height(), gr->maxY() + 1 ) ;
		return true ;
	}
	w = std::min( this->width(), mask->width() ) ;
	h = std::min( this->height(), mask->height() ) ;
	return true ;
}

bool XamPNM::isRaw() const
{
	return ( magicNumber() > 3 ) ;
}

bool XamPNM::isRgb() const
{
	return ( magicNumber() == 3 )||( magicNumber() == 6 ) ;
}

// accesseurs composantes RGB du raster

Uint8 XamPNM::red(int x, int y ) const
{
	if ( isOutOfRange(x, y ) )	return 0 ;
	return m_raster[ offset(x, y ) ] ;
}

Uint8 XamPNM::green(int x, int y ) const
{
	if ( isOutOfRange(x, y ) )	return 0 ;
	return m_raster[ offset(x, y ) + 1 ] ;
}

Uint8 XamPNM::blue(int x, int y ) const
{
	if ( isOutOfRange(x, y ) )	return 0 ;
	return m_raster[ offset(x, y ) + 2 ] ;
}

void XamPNM::setRed(int x, int y, Uint8 r )
{
	if ( isOutOfRange(x, y ) )	return ;
	m_raster[ offset(x, y ) ] = normalize( r ) ;
	m_isModified = true ;
}

void XamPNM::setGreen(int x, int y, Uint8 g )
{
	if ( isOutOfRange(x, y ) )	return ;
	m_raster[ offset(x, y ) + 1 ] = normalize( g ) ;
	m_isModified = true ;
}

void XamPNM::setBlue(int x, int y, Uint8 b )
{
	if ( isOutOfRange(x, y ) )	return ;
	m_raster[ offset(x, y ) + 2 ] = normalize( b ) ;
	m_isModified = true ;
}

Uint32 XamPNM::pixel(int x, int y ) const
{
	if ( isOutOfRange(x, y ) )	return 0 ;
	return XAM_COLOR24(red(x,y), green(x,y), blue(x,y) ) ;

}

void XamPNM::setPixel(int x, int y, Uint8 r, Uint8 g, Uint8 b )
{
	if ( isOutOfRange(x, y ) )	return ;
	m_raster[ offset(x, y ) ] = normalize( r ) ;
	m_raster[ offset(x, y ) + 1 ] = normalize( g ) ;
	m_raster[ offset(x, y ) + 2 ] = normalize( b ) ;
	m_isModified = true ;
}

void XamPNM::setPixel(int x, int y, Uint32 color )
{
	if ( isOutOfRange(x, y ) )	return ;
	m_raster[ offset(x, y ) ] = XAM_R(color) ;
	m_raster[ offset(x, y ) + 1 ] = XAM_G(color) ;
	m_raster[ offset(x, y ) + 2 ] = XAM_B(color) ;
	m_isModified = true ;
}

// accesseurs niveau de gris : R = G = B = v

Uint8 XamPNM::gray(int x, int y ) const
{
	if ( isOutOfRange(x, y ) )	return 0 ;
	int r = m_raster[ offset(x, y ) ] ;
	int g = m_raster[ offset(x, y ) + 1 ] ;
	int b = m_raster[ offset(x, y ) + 2 ] ;
	return (Uint8)( ( r + g + b ) / 3 ) ;
}

void XamPNM::setGray(int x, int y, Uint8 v )
{
	if ( isOutOfRange(x, y ) )	return ;
	for ( int i = 0 ; i < 3 ; ++i ) m_raster[ offset(x, y ) + i ] = normalize( v ) ;
	m_isModified = true ;
}

// spectre de l'image (index = 0..m_maxval)

int XamPNM::spectrumMaxValue()
{
	if ( m_isModified )	initSpectrum() ;
	return m_spectrumMax ;
}

int XamPNM::spectrum(int index )
{
	if ( m_isModified )	initSpectrum() ;
	return ( rSpectrum(index) + gSpectrum(index) + bSpectrum(index ) ) / 3 ;
}

int XamPNM::rSpectrum(int index )
{
	if ( m_isModified )	initSpectrum() ;
	return m_rSpectrum[ normalize(index ) ] ;
}

int XamPNM::gSpectrum(int index )
{
	if ( m_isModified )	initSpectrum() ;
	return m_gSpectrum[ normalize(index ) ] ;
}

int XamPNM::bSpectrum(int index )
{
	if ( m_isModified )	initSpectrum() ;
	return m_bSpectrum[ normalize(index ) ] ;
}

void XamPNM::initSpectrum()
{
	// raz
	for ( int i = 0 ; i < 256 ; i++ ) {
		m_rSpectrum[i] = m_gSpectrum[i] = m_bSpectrum[i] = 0 ;
	}
	// calcul des spectres R, G et B
	for (int j = 0 ; j < height() ; j++ ) {
		for ( int i = 0 ; i < width() ; i++ ) {
			m_rSpectrum[ red(i,j) ] += 1 ;
			m_gSpectrum[ green(i,j) ] += 1 ;
			m_bSpectrum[ blue(i,j) ] += 1 ;
		}
	}
	// recherche valeur maximale
	m_spectrumMax = 0 ;
	for ( int i = 0 ; i < 256 ; i++ ) {
		if ( m_rSpectrum[i] > m_spectrumMax )	m_spectrumMax = m_rSpectrum[i] ;
		if ( m_gSpectrum[i] > m_spectrumMax )	m_spectrumMax = m_gSpectrum[i] ;
		if ( m_bSpectrum[i] > m_spectrumMax )	m_spectrumMax = m_bSpectrum[i] ;
	}

	m_isModified = false ;
}

// test de validité des coordonnées dans raster

bool XamPNM::isOutOfRange(int x, int y ) const
{
	if (( x < 0 )||( x >= width() ))	return true ;
	if (( y < 0 )||( y >= height() ))	return true ;
	return false ;
}

// accès pixel dans raster

int XamPNM::offset(int x, int y ) const
{
	return ( y * width() + x ) * 3 ;
}

// normalisation 0..m_maxval

Uint8 XamPNM::normalize(int v )
{
	if ( v < 0 )		return 0 ;
	if ( v > m_maxval )	return m_maxval ;
	return v ;
}
