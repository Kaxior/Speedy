/*  ---------------------------------------------------------------------------
 *  filename    :   main.c
 *  description :   point d'entrée de l'application 'Othello'
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

/*	niveaux de difficulté
	---------------------
	0 : pas d'IA, permet à 2 joueurs humains de s'affronter
	1 : jeu contre l'ordinateur, pose aléatoire
	2 : id., IA avec recherche sur 3 coups futurs (méthode Minimax)
	3 : id., IA avec recherche sur 5 coups futurs (méthode AlphaBeta)
*/

#include	<unistd.h>				// pour getopt()
#include	"othello.h"

Othellier	oth ;					// plateau de jeu
Joueur		joueur ;				// joueur courant
int			iaNiveau = 2 ;			// niveau de difficulté

void	Help(const char* appName )
{
	printf("\nsyntaxe : %s [ -n <niveau> ] [-?]", appName ) ;
	puts("\n\t<niveau> : 0..3 ( defaut = 2 )" ) ;
	puts("\t0 : pas d'IA, 2 joueurs humains doivent s'affronter..." ) ;
	puts("\t1 : jeu contre l'ordinateur, sans IA (pose aleatoire)" ) ;
	puts("\t2 : IA avec recherche sur 3 coups futurs (methode Minimax)" ) ;
	puts("\t3 : IA avec recherche sur 5 coups futurs (methode AlphaBeta) " ) ;
}

// traitement (récursif) d'une partie

void Jeu(int mx, int my, bool clic )
{
	static bool	fin = false ;		// fin de partie
	static bool	passe = false ;		// joueur passe son tour
	static bool	change = false ;	// changement de joueur

	int	n ;							// numéro de case 1..64

	if ( fin )	return ;

	if ( !CasesDispo( &oth ) ) {	// reste-t-il des cases libres ?
		fin = true ;
	}
	else {

		// le joueur courant doit-il passer son tour ?

		if ( passe ) {
			if ( iaNiveau )	printf("%7s", ( joueur == HUMAIN ? "Humain" : "Machine" ) ) ;
			else			printf("%5s", ( joueur == HUMAIN ? "Noir" : "Blanc" ) ) ;
			printf(" PASSE !\n" ) ;

			change = true ;
		}
		else {
			// assistance si souris sur case jouable

			n = Survol(mx, my ) ;
			if ( n )	AideSiJouable( &oth, n ) ;

			// requête de pose d'un nouveau pion ? (case jouable ou non)

			n = PosePion( &oth, iaNiveau, mx, my, clic ) ;
			if ( n ) {

				if ( iaNiveau )	printf("%7s", ( joueur == HUMAIN ? "Humain" : "Machine" ) ) ;
				else			printf("%5s", ( joueur == HUMAIN ? "Noir" : "Blanc" ) ) ;
				printf(" joue en %c%d", N2X(n)+'A'-1, N2Y(n) ) ;

				if ( !CaseJouable( &oth, n ) )	printf(", TOUR PERDU !") ;
				putchar('\n') ;

				MajPlateau( &oth, n ) ;		// prise en compte du nouveau pion

				change = true ;
			}
		}

		// changement de joueur ?

		if ( change ) {
			if ( !ChangeJoueur( &oth ) ) {	// aucune case jouable ?

				if ( NbrPions( &oth ) ) {	// encore des pions sur le jeu ?

					if ( passe )			// les 2 joueurs doivent passer ?
							fin = true ;	// alors jeu bloqué
					else	passe = true ;	// sinon passe son tour
				}
				else	fin = true ;		// score nul pour le joueur !
			}
			else	passe = false ;			// le joueur peut jouer...

			joueur = AfficheScore( &oth ) ;	// màj score + joueur qui à la main

			change = false ;
		}
	}

	if ( fin ) {
		printf("PARTIE TERMINEE !\n" ) ;
		xamKillTimer() ;
		return ;
	}

	// la machine joue automatiquement...

	if (( iaNiveau )&&( joueur == MACHINE ))	Jeu(mx, my, false ) ;
}

// intercepteurs des actions souris

void PositionSouris(int x, int y )
{
	Jeu(x, y, false ) ;
}

void ClicSouris(int x, int y, int button )
{
	if ( button != 1 ) return ;		// clic gauche attendu
	Jeu(x, y, true ) ;
}

// point d'entrée

int	main(int argc, char** argv )
{
	int			opt ;

	printf("\n%s\nwith %s\n", OTHELLO_VERSION, XAMGRAPH_SHORTDESC ) ;

	while ( ( opt = getopt(argc, argv, ":?n:") ) != -1 ) {
		int	i ;
		switch( opt ) {
			case 'n' :	i = atoi( optarg ) ;
						if ( ( i >= 0 )&&( i <= 3 ) ) {
							iaNiveau = i ;
							break ;
						}
			case '?' :	Help( argv[0] ) ;
						return -1 ;
		}
	}

	xamCreateGraph( WINSIZEX, WINSIZEY, OTHELLO_VERSION ) ;

	xamLoadFont("../resources/Arial Black.ttf", 20 ) ;
	xamClearScreen( CFOND ) ;

	xamLoadImage(CELLSIZE * 10.5 - 50, CELLSIZE / 4, "../resources/XamGraph_logo_100pix.png" ) ;

	InitPlateau( &oth ) ;
	AffichePlateau( &oth ) ;
	joueur = AfficheScore( &oth ) ;

	xamConnectMouseButtonUpEvent( ClicSouris ) ;
	xamConnectMouseMotionEvent( PositionSouris ) ;

	xamRun() ;

	xamCloseGraph() ;

	return 0 ;
}
