/*  ---------------------------------------------------------------------------
 *  filename    :   othello.c
 *  description :   Ressources de gestion affichages / souris
 *
 *	project     :	XamGraph demo : Reversi like
 *  start date  :   septembre 2006
 *  ---------------------------------------------------------------------------
 *  Copyright 2006-2017 by Alain Menu   <alain.menu@ac-creteil.fr>
 *
 *  This file is part of "XamGraph apps"
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

#include	"othello.h"

// prototypes des ressources privées

bool	testInCell( int mx, int my, int x, int y ) ;
void	grille( int couleur ) ;
void	afficheTexte( char* s, int x, int y, int couleur ) ;
void	pion( int x, int y, int couleur ) ;
void	cell( int x, int y, int couleur ) ;
void	affichePions( Othellier* oth ) ;


// ------------------------ ressources publiques ------------------------------

// initialisation de l'othellier ( début de partie )

void	InitPlateau( Othellier* oth )
{
	int	x , y ;

	for ( x = 0 ; x < 10 ; x++ )
		for ( y = 0 ; y < 10 ; y++ ) {		// nettoyage du plateau
			oth->jeu[x][y] = VIDE ;
	}
	oth->jeu[4][4] = BLANC ;				// placement des pions de départ
	oth->jeu[5][5] = BLANC ;
	oth->jeu[5][4] = NOIR ;
	oth->jeu[4][5] = NOIR ;

	oth->joueur = HUMAIN ;					// la machine laisse la main...

	oth->jeu[4][3] = JOUABLE ;				// le tableau cj[] n'est pas
	oth->jeu[3][4] = JOUABLE ;				// utilisé par le joueur humain...
	oth->jeu[5][6] = JOUABLE ;				// il n'est donc pas initialisé ici
	oth->jeu[6][5] = JOUABLE ;

	oth->nbPi[HUMAIN]  = 2 ;
	oth->nbPi[MACHINE] = 2 ;
	oth->nbCj[HUMAIN]  = 4 ;
	oth->nbCj[MACHINE] = 0 ;

}

// affichage du plateau de jeu

void	AffichePlateau( Othellier* oth )
{
	int		i ;
	char	buf[10] ;

	grille( XAM_LIGHT_GRAY ) ;

	for ( i = 1 ; i < 9 ; i++ )	{
		sprintf(buf,"%d", i ) ;
		afficheTexte(buf, 0, i, XAM_LIGHT_GRAY ) ;
		sprintf(buf,"%c", 'A' + i - 1 ) ;
		afficheTexte(buf, i, 0, XAM_LIGHT_GRAY ) ;
	}

	affichePions( oth ) ;

	xamUpdateScreen() ;
}

// traitement d'une requête de pose de pion sur la case n

void	MajPlateau( Othellier* oth, int n )
{
	int		x = N2X(n) ;
	int		y = N2Y(n) ;
	Etat	etat = COULEUR( oth->joueur ) ;

	if ( oth->jeu[x][y] != JOUABLE )	return ;

	cell( x, y, CFOND ) ;				//	animation ...

	pion( x, y, etat == NOIR ? XAM_BLACK : XAM_WHITE ) ;
	xamDelay(150) ;
	cell( x, y, CFOND ) ;
	xamDelay(150) ;
	pion( x, y, etat == NOIR ? XAM_BLACK : XAM_WHITE ) ;
	xamDelay(150) ;
	cell( x, y, CFOND ) ;

	oth->jeu[x][y] = etat ;				// pose du nouveau pion

	oth->nbPi[ oth->joueur ] += 1 ;		// màj compteur

	IaRetournePions( oth, n ) ;			// retournement des pions adverses

	affichePions( oth ) ;				// et màj affichage
}

// retourne le n de case [1..64] survolée par la souris, 0 sinon

int	Survol(int mx, int my )
{
	int	i ;
	for ( i = 1 ; i <= 64 ; i++ ) {
		if ( testInCell( mx, my, N2X(i), N2Y(i) ) )	return i ;
	}
	return 0 ;
}

// teste de requête de pose d'un pion par le joueur courant

int	PosePion( Othellier* oth, int niv, int mx, int my, bool clic )
{
	// la machine a-t-elle la main ?
	// si oui, le calcul de la case à jouer est traité par IA

	if ( ( niv )&&( oth->joueur == MACHINE ) ) return IaCalculCoup( oth, niv ) ;

	// sinon, joueur humain : retourne le n de case sous clic gauche souris
	// ou 0 si en dehors du plateau

	if (!clic )	return 0 ;
	return Survol(mx, my ) ;
}

// mise en valeur d'une case jouable

void	AideSiJouable( Othellier* oth, int n )
{
	static int mem = 0 ;
	int	x = N2X(mem) ;
	int	y = N2Y(mem) ;

	// même case que précédemment, rien à faire
	if ( n == mem )	return ;

	// annuler mise en valeur précédente
	if ( mem )	switch ( oth->jeu[x][y] ) {
		case VIDE :
		case JOUABLE :	cell( N2X(mem), N2Y(mem), CFOND ) ; break ;
		default :	break ;
	}

	// mise en valeur nouvelle case
	x = N2X(n) ;
	y = N2Y(n) ;
	if ( oth->jeu[x][y] == JOUABLE ) {
		cell( x, y, XAM_DARK_BLUE ) ;
	}
	mem = n ;
}

// teste si la case spécifiée est jouable

bool	CaseJouable( Othellier* oth, int n )
{
	return ( oth->jeu[ N2X(n) ][ N2Y(n) ] == JOUABLE ) ;
}

// changement de joueur, retourne le nb. de cases jouables

int		ChangeJoueur( Othellier* oth )
{
	oth->joueur = ADVERSAIRE( oth->joueur ) ;

	return IaChercheCasesJouables( oth ) ;
}

// retourne le nombre de pions du joueur courant

int		NbrPions( Othellier* oth )
{
	return oth->nbPi[ oth->joueur ] ;
}

// nombre de cases disponibles

int		CasesDispo( Othellier* oth )
{
	int	i , result = 0 ;

	for ( i = 1 ; i <= 64 ; i++ ) {
		if ( oth->jeu[ N2X(i) ][ N2Y(i) ] == VIDE )		result++ ;
		if ( oth->jeu[ N2X(i) ][ N2Y(i) ] == JOUABLE )	result++ ;
	}
	return result ;
}

// affiche le score courant

Joueur	AfficheScore( Othellier* oth )
{
	char	buf[10] ;

	sprintf(buf, "%02d", oth->nbPi[HUMAIN] ) ;	// nombres de pions
	cell( 10, 2, CFOND ) ;
	afficheTexte(buf, 10, 2, XAM_BLACK ) ;
	sprintf(buf, "%02d", oth->nbPi[MACHINE] ) ;
	cell( 10, 6, CFOND ) ;
	afficheTexte(buf, 10, 6, XAM_WHITE ) ;

	if ( oth->joueur == HUMAIN ) {				// indique à qui le tour...
												// et nb. de cases jouables
		cell( 10, 7, CFOND ) ;
		pion( 10, 3, XAM_BLACK ) ;
		sprintf(buf, "%d", oth->nbCj[HUMAIN] ) ;
		cell( 10, 8, CFOND ) ;
		afficheTexte(buf, 10, 4, XAM_LIGHT_RED ) ;
	}
	else {
		cell( 10, 3, CFOND ) ;
		pion( 10, 7, XAM_WHITE ) ;
		sprintf(buf, "%d", oth->nbCj[MACHINE] ) ;
		cell( 10, 4, CFOND ) ;
		afficheTexte(buf, 10, 8, XAM_LIGHT_RED ) ;
	}

	xamUpdateScreen() ;

	return oth->joueur ;
}

int		Evaluation( Othellier* oth )
{
	return	IaEvaluation( oth ) ;
}

// ------------------------- ressources privées -------------------------------

// teste si la souris se trouve sur la case de coord. x,y

bool	testInCell( int mx, int my, int x, int y )
{
	if ( mx < ORGX + ( x - 1 ) * CELLSIZE + 2 )	return false ;
	if ( mx > ORGX + x * CELLSIZE - 2 )			return false ;
	if ( my < ORGY + ( y - 1 ) * CELLSIZE + 2 )	return false ;
	if ( my > ORGY + y * CELLSIZE - 2 )			return false ;

	return true ;
}

// dessin de la grille de l'othellier

void	grille( int couleur )
{
	int	len = 8 * CELLSIZE ;
	int	i ;

	xamSetColor( couleur ) ;
	xamRectangle( ORGX, ORGY, len, len ) ;
	for ( i = ORGX + CELLSIZE ; i < ORGX + len ; i += CELLSIZE ) {
		xamMoveTo( i, ORGY ) ;
		xamLineTo( i, ORGX + len ) ;
	}
	for ( i = ORGY + CELLSIZE ; i < ORGY + len ; i += CELLSIZE ) {
		xamMoveTo( ORGX, i ) ;
		xamLineTo( ORGX + len, i ) ;
	}
}

// affichage de texte sur cellule

void	afficheTexte( char* s, int x, int y, int couleur )
{
	int	posx  = ORGX + (int)( ( (double)x - 0.5 ) * CELLSIZE ) ;
	int	posy  = ORGY + (int)( ( (double)y - 0.5 ) * CELLSIZE ) ;

	xamSetTextAlignment( XAM_ALIGNMENT_CENTER ) ;
	xamSetColor( couleur ) ;
	xamTextOut( posx, posy, s ) ;
}

// dessin d'un pion en x,y

void	pion( int x, int y, int couleur )
{
	int	rayon = ( CELLSIZE - 6 ) / 2 ;
	int	posx  = ORGX + (int)( ( (double)x - 0.5 ) * CELLSIZE ) ;
	int	posy  = ORGY + (int)( ( (double)y - 0.5 ) * CELLSIZE ) ;

	xamSetColor( couleur ) ;
	xamFilledCircle( posx, posy, rayon ) ;
	xamSetColor( couleur == XAM_WHITE ? XAM_BLACK : XAM_WHITE ) ;
	xamCircle( posx, posy, rayon ) ;

	xamUpdateScreen() ;
}

// coloration de la case x,y

void	cell( int x, int y, int couleur )
{
	int	posx  = ORGX + ( x - 1 ) * CELLSIZE ;
	int	posy  = ORGY + ( y - 1 ) * CELLSIZE ;

	xamSetColor( couleur ) ;
	xamFilledRectangle( posx + 1, posy + 1, CELLSIZE - 2, CELLSIZE - 2 ) ;

	xamUpdateScreen() ;
}

// affichage des pions

void	affichePions( Othellier* oth )
{
	int		x , y ;

	for ( x = 1 ; x < 9 ; x++ )	{
		for ( y = 1 ; y < 9 ; y++ ) {
			switch( oth->jeu[x][y] ) {
				case BLANC :	pion( x, y, XAM_WHITE ) ;	break ;
				case NOIR :		pion( x, y, XAM_BLACK ) ;	break ;
				default : break ;
			}
		}
	}
}

// ----------------------------------------------------------------------------
