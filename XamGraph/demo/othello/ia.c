/*  ---------------------------------------------------------------------------
 *  filename    :   ia.c
 *  description :   Module d'intelligence artificielle
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

#include	"ia.h"

// prototypes des ressources privées

int		iaTestDirection( Othellier* oth, int n, int dirx, int diry ) ;
void	iaBoussole( int dir, int* dirx, int* diry ) ;
void	iaMinimax( Othellier* oth, int niv, int* score, int* ind ) ;
void	iaAlphaBeta( Othellier* oth, int niv, int* score, int* ind ) ;

// ------------------------ ressources publiques ------------------------------

// retourne les pions adverses suite à une pose sur la case n [1..64]

void	IaRetournePions( Othellier* oth, int n )
{
	int 	dir, dirx, diry ;
	int		x , y ;
	int		borne ;

	for ( dir = 0 ; dir < 8 ; dir++ ) {

		iaBoussole(dir, &dirx, &diry ) ;
		borne = iaTestDirection( oth, n, dirx, diry ) ;

		if ( !borne )	continue ;

		x = N2X(n) + dirx ;
		y = N2Y(n) + diry ;
		while ( XY2N(x,y) != borne ) {
			oth->jeu[x][y] = COULEUR( oth->joueur ) ;
			oth->nbPi[ oth->joueur ] += 1 ;
			oth->nbPi[ ADVERSAIRE( oth->joueur ) ] -= 1 ;
			x += dirx ;
			y += diry ;
		}
	}
}

int		IaChercheCasesJouables( Othellier* oth )
{
	int		i ;
	int		x, y ;
	int 	dir, dirx, diry ;
	bool	jouable ;
	int		nbJouables = 0 ;

	for ( i = 1 ; i <= 64 ; i++ ) {		// pour toutes les cases du jeu

		jouable = false ;
		x = N2X(i) ;
		y = N2Y(i) ;
		if ( oth->jeu[x][y] == JOUABLE )	oth->jeu[x][y] = VIDE ;
		if ( oth->jeu[x][y] != VIDE )		continue ;

		for ( dir = 0 ; dir < 8 ; dir++ ) {
			iaBoussole(dir, &dirx, &diry ) ;
			if ( iaTestDirection( oth, i, dirx, diry ) )	jouable = true ;
		}

		if ( jouable ) {
			oth->jeu[x][y] = JOUABLE ;
			oth->cj[ nbJouables ] = XY2N( x, y ) ;
			nbJouables++ ;
		}
	}

	oth->cj[ nbJouables ] = 0 ;

	oth->nbCj[ oth->joueur ] = nbJouables ;

	return nbJouables ;
}

int	IaCalculCoup( Othellier* oth, int niv )		// recherche meilleur coup
{												// suivant niv. de difficulté
	int		nbCj = 0 ;
	int		ind = 0 ;							// indice à calculer
	int*	pCj = oth->cj ;

	while ( *pCj++ )	nbCj++ ;				// nb. de cases jouables

	if ( niv == 1 )	{							// choix aléatoire ?
		srand( time(NULL) ) ;
		ind = rand() % nbCj ;
	}
	else if ( niv == 2 ) {

		int	score ;
		iaMinimax( oth, 3, &score, &ind ) ;
	}
	else if ( niv == 3 ) {
		int	score ;
		iaAlphaBeta( oth, 5, &score, &ind ) ;
	}

	return oth->cj[ ind ] ;
}

// ------------------------- ressources privées -------------------------------

// teste la présence de pions adverses autour de la case n avec dir 0..7
// retourne le n de case du pion final du joueur ou 0 sinon

int	iaTestDirection( Othellier* oth, int n, int dirx, int diry )
{
	int		x , y ;
	Etat	etatJoueur = 	COULEUR( oth->joueur ) ;
	Etat	etatAdverse = 	COULEUR( ADVERSAIRE( oth->joueur ) ) ;

	// première case adjacente

	x = N2X(n) + dirx ;
	y = N2Y(n) + diry ;

	// si ce n'est pas un pion adverse, abandon

	if ( oth->jeu[x][y] != etatAdverse )	return 0 ;

	// sinon, on continue dans la même direction

	while ( oth->jeu[x][y] == etatAdverse ) {
		x += dirx ;
		y += diry ;
	}

	// si un pion du joueur trouvé, ok

	if ( oth->jeu[x][y] == etatJoueur )	return XY2N( x, y ) ;

	return 0 ;
}

// affecte dirx,diry avec des valeurs parmi -1,0,+1
// à partir d'une direction de déplacement codée 0..7

void	iaBoussole( int dir, int* dirx, int* diry )
{
	switch ( dir ) {
		case 0 :	*dirx =  0 ; *diry = -1 ; break ;		// N
		case 1 :	*dirx =  1 ; *diry = -1 ; break ;		// NE
		case 2 :	*dirx =  1 ; *diry =  0 ; break ;		// E
		case 3 :	*dirx =  1 ; *diry =  1 ; break ;		// SE
		case 4 :	*dirx =  0 ; *diry =  1 ; break ;		// S
		case 5 :	*dirx = -1 ; *diry =  1 ; break ;		// SO
		case 6 :	*dirx = -1 ; *diry =  0 ; break ;		// O
		default :	*dirx = -1 ; *diry = -1 ; break ;		// NO
	}
}

// algorithme "Minimax" (récursif) sur une profondeur 'niv'
// toutes les feuilles de l'arbre sont évaluées...
// le numéro 1..64 de la "meilleure" case à jouer est placé dans 'ind'

void	iaMinimax( Othellier* oth, int niv, int* score, int* ind )
{
	Othellier	otmp ;
	int			x , y , i = 0 ;
	int			_score , _ind ;

	// jeu bloqué ? (aucune case jouable)

	if ( ( oth->nbCj[HUMAIN] == 0 )&&( oth->nbCj[MACHINE] == 0 ) ) {
		*score = IaEvaluation( oth ) ;
		return ;
	}

	*score = ( oth->joueur == HUMAIN ? EVALMAX : -EVALMAX ) ;
	niv-- ;

	while ( oth->cj[i] ) {			// pour toutes les cases jouables

		// construction d'un nouvel othellier avec le pion en + ...

		memmove( &otmp, oth, sizeof( Othellier ) ) ;
		x = N2X( otmp.cj[i] ) ;
		y = N2Y( otmp.cj[i] ) ;
		otmp.jeu[x][y] = COULEUR( otmp.joueur ) ;	// joue la case
		IaRetournePions( &otmp, otmp.cj[i] ) ;		// retourne les pions adv.
		otmp.joueur = ADVERSAIRE( otmp.joueur ) ;	// change de joueur
		IaChercheCasesJouables( &otmp ) ;			// cherche les cases jouables

		if ( niv )	iaMinimax(&otmp, niv, &_score, &_ind ) ;	// branche
		else		_score = IaEvaluation( &otmp ) ;			// feuille

		if ( oth->joueur == MACHINE ) {
			if ( _score >= *score ) {
				*score = _score ;
				*ind = i ;
				if ( *score == EVALMAX )	break ;
			}
		}
		else {
			if ( _score <= *score ) {
				*score = _score ;
				*ind = i ;
				if ( *score == -EVALMAX )	break ;
			}
		}
		i++ ;		// case jouable suivante...
	}
}

// algorithme "AlphaBeta" (récursif) sur une profondeur 'niv'
// optimisation de l'algorithme "Minimax" par coupure des branches
// sans intèrêt par rapport à l'exploration déjà effectuée...
// le numéro 1..64 de la "meilleure" case à jouer est placé dans 'ind'

void	iaAlphaBeta( Othellier* oth, int niv, int* score, int* ind )
{
	Othellier	otmp ;
	int			x , y , i = 0 ;
	int			_score , _ind ;

	static int		memScore = 0 ;
	static Joueur	memJoueur = HUMAIN /*oth->joueur*/ ;

	if ( ( oth->nbCj[HUMAIN] == 0 )&&( oth->nbCj[MACHINE] == 0 ) ) {
		*score = IaEvaluation( oth ) ;
		return ;
	}

	*score = ( oth->joueur == HUMAIN ? EVALMAX : -EVALMAX ) ;
	niv-- ;

	while ( oth->cj[i] ) {			// pour toutes les cases jouables

		// construction d'un nouvel othellier avec le pion en + ...

		memmove( &otmp, oth, sizeof( Othellier ) ) ;
		x = N2X( otmp.cj[i] ) ;
		y = N2Y( otmp.cj[i] ) ;
		otmp.jeu[x][y] = COULEUR( otmp.joueur ) ;	// joue la case
		IaRetournePions( &otmp, otmp.cj[i] ) ;		// retourne les pions adv.
		otmp.joueur = ADVERSAIRE( otmp.joueur ) ;	// change de joueur
		IaChercheCasesJouables( &otmp ) ;			// cherche les cases jouables

		if ( niv ) {											// branche
			memScore = *score ;
			memJoueur = oth->joueur ;
			iaMinimax(&otmp, niv, &_score, &_ind ) ;
		}
		else		_score = IaEvaluation( &otmp ) ;			// feuille

		if ( oth->joueur == MACHINE ) {

			if ( ( memJoueur != MACHINE )&&( _score >= memScore ) ) {
				*score = EVALMAX ;
				break ;
			}
			else if ( _score >= *score ) {
				*score = _score ;
				*ind = i ;
				if ( *score == EVALMAX )	break ;
			}
		}
		else {

			if ( ( memJoueur != HUMAIN )&&( _score <= memScore ) ) {
				*score = -EVALMAX ;
				break ;
			}
			else if ( _score <= *score ) {
				*score = _score ;
				*ind = i ;
				if ( *score == -EVALMAX )	break ;
			}
		}
		i++ ;		// case jouable suivante...
	}
}

// ----------------------------------------------------------------------------
