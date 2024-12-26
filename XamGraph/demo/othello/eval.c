/*  ---------------------------------------------------------------------------
 *  filename    :   eval.c
 *  description :   Evaluation de la valeur de l'othellier
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

#include	"eval.h"

// prototypes des ressources privées

int		iaEvalLigne( Othellier* oth, int y ) ;
int		iaEvalColonne( Othellier* oth, int x ) ;
int		iaEvalCase( Othellier* oth, int x, int y, int val ) ;

// ------------------------ ressources publiques ------------------------------

// Processus d'évaluation de la valeur de l'othellier
// ( toujours du point de vue de la machine... )

int		IaEvaluation( Othellier* oth )
{
	int	n , mat , mob ;
	int	eval = 0 ;
	int	diff = oth->nbPi[MACHINE] - oth->nbPi[HUMAIN] ; ;

	// cas d'une partie terminée (aucune case jouable quel que soit le joueur)

	if ( ( !oth->nbCj[HUMAIN] )&&( !oth->nbCj[MACHINE] ) ) {

		if ( diff > 0 )	return  EVALMAX ;	// la machine gagne !
		if ( diff < 0 )	return -EVALMAX ;	// la machine perd !
		return 0 ;							// match nul...
	}

	// poids des pions imprenables (bords de l'othellier sauf coins)

	eval += iaEvalLigne( oth, 1 ) ;			// ligne haute
	eval += iaEvalLigne( oth, 8 ) ;			// ligne basse
	eval += iaEvalColonne( oth, 1 ) ;		// colonne gauche
	eval += iaEvalColonne( oth, 8 ) ;		// colonne droite

	// poids des coins

	eval += iaEvalCase( oth, 1, 1, EVALCOIN ) ;
	eval += iaEvalCase( oth, 1, 8, EVALCOIN ) ;
	eval += iaEvalCase( oth, 8, 1, EVALCOIN ) ;
	eval += iaEvalCase( oth, 8, 8, EVALCOIN ) ;

	// poids des cases C et X

	if ( CASELIBRE( oth->jeu[1][1] ) ) {
		eval += iaEvalCase( oth, 1, 2, -EVALCASEC ) ;
		eval += iaEvalCase( oth, 2, 2, -EVALCASEX ) ;
		eval += iaEvalCase( oth, 2, 1, -EVALCASEC ) ;
	}
	if ( CASELIBRE( oth->jeu[1][8] ) ) {
		eval += iaEvalCase( oth, 1, 7, -EVALCASEC ) ;
		eval += iaEvalCase( oth, 2, 7, -EVALCASEX ) ;
		eval += iaEvalCase( oth, 2, 8, -EVALCASEC ) ;
	}
	if ( CASELIBRE( oth->jeu[8][1] ) ) {
		eval += iaEvalCase( oth, 7, 1, -EVALCASEC ) ;
		eval += iaEvalCase( oth, 7, 2, -EVALCASEX ) ;
		eval += iaEvalCase( oth, 8, 2, -EVALCASEC ) ;
	}
	if ( CASELIBRE( oth->jeu[8][8] ) ) {
		eval += iaEvalCase( oth, 7, 8, -EVALCASEC ) ;
		eval += iaEvalCase( oth, 7, 7, -EVALCASEX ) ;
		eval += iaEvalCase( oth, 8, 7, -EVALCASEC ) ;
	}

	// poids des cases centrales

	eval += iaEvalCase( oth, 4, 4, EVALCENTRE ) ;
	eval += iaEvalCase( oth, 5, 4, EVALCENTRE ) ;
	eval += iaEvalCase( oth, 4, 5, EVALCENTRE ) ;
	eval += iaEvalCase( oth, 5, 5, EVALCENTRE ) ;

	// prépondérance des critères

	n = oth->nbPi[HUMAIN] + oth->nbPi[MACHINE] - 4 ;		// n = 0..60

	mat = EVALMAT * diff ;
	mob = EVALMOB * ( oth->nbCj[ MACHINE ] - oth->nbCj[ HUMAIN ] ) ;

	if ( n < 15 )	return 	mat     + mob  * 2 + eval     ;
	if ( n < 45 )	return 	mat     + mob      + eval * 3 ;
	return					mat * 3            + eval * 2 ;
}

// ------------------------- ressources privées -------------------------------

// évaluation d'une ligne ( y = 1..8 )

int		iaEvalLigne( Othellier* oth, int y )
{
	int	x , n , eval = 0 ;

	// comptage du pions du joueur (hors coins)

	for ( x = 1, n = 0 ; x <= 8 ; x++ ) {
		if ( CASELIBRE( oth->jeu[x][y] ) )	break ;
		if ( ( x == 1 )||( x == 8 ) )		continue ;
		if ( oth->jeu[x][y] == BLANC )		n++ ;
	}

	// ligne entièrement occupée ? comptabilisation en +/-

	if ( x > 8 ) {
		eval += EVALBORD * ( 2 * n - 6 ) ;
		return eval ;
	}

	// au moins une case libre, recherche à partir de coins

	n = 0 ;

	if ( oth->jeu[1][y] == BLANC )
		for ( x = 2 ; x <= 8 ; x++ ) {
			if ( oth->jeu[x][y] != BLANC ) break ;
			n++ ;
		}

	if ( oth->jeu[8][y] == BLANC )
		for ( x = 7 ; x >= 1 ; x-- ) {
			if ( oth->jeu[x][y] != BLANC ) break ;
			n++ ;
		}

	eval += EVALBORD * n ;

	return eval ;
}

// évaluation d'une colonne ( x = 1..8 )

int		iaEvalColonne( Othellier* oth, int x )
{
	int	y , n , eval = 0 ;

	// comptage du pions du joueur (hors coins)

	for ( y = 1, n = 0 ; y <= 8 ; y++ ) {
		if ( CASELIBRE( oth->jeu[x][y] ) )	break ;
		if ( ( y == 1 )||( y == 8 ) )		continue ;
		if ( oth->jeu[x][y] == BLANC )		n++ ;
	}

	// colonne entièrement occupée ? comptabilisation en +/-

	if ( y > 8 ) {
		eval += EVALBORD * ( 2 * n - 6 ) ;
		return eval ;
	}

	// au moins une case libre, recherche à partir de coins

	n = 0 ;

	if ( oth->jeu[1][y] == BLANC )
		for ( y = 2 ; y <= 8 ; y++ ) {
			if ( oth->jeu[x][y] != BLANC ) break ;
			n++ ;
		}

	if ( oth->jeu[1][y] == BLANC )
		for ( y = 7 ; y >= 1 ; y-- ) {
			if ( oth->jeu[x][y] != BLANC ) break ;
			n++ ;
		}

	eval += EVALBORD * n ;

	return eval ;
}

// évaluation d'une case en +/- suivant la couleur de son pion

int		iaEvalCase( Othellier* oth, int x, int y, int val )
{
	if ( CASELIBRE( oth->jeu[x][y] ) )	return 0 ;
	if ( oth->jeu[x][y] == BLANC )		return val ;
	return -val ;
}

// ----------------------------------------------------------------------------
