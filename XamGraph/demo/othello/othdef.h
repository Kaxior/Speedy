/*  ---------------------------------------------------------------------------
 *  filename    :   othdef.h
 *  description :   Définitions globales
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

#ifndef	OTHDEF_H
#define	OTHDEF_H

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<time.h>

#include 	"_VERSION"
#include 	<xamgraph.h>

// dimensions du plateau de jeu

#define		CELLSIZE	42
#define		ORGX		CELLSIZE
#define		ORGY		CELLSIZE
#define		WINSIZEX	( 10 * CELLSIZE + ORGX + CELLSIZE )
#define		WINSIZEY	( 8 * CELLSIZE + ORGY + CELLSIZE / 2 )

// couleur du tapis de jeu

#define		CFOND		XAM_DARK_GREEN

// conversion de coordonnées : N [ 1..64 ] <---> X , Y [ 1..8 ]

#define		N2X(N)		( ( (N) - 1 ) % 8 + 1 )
#define		N2Y(N)		( ( (N) - 1 ) / 8 + 1 )
#define		XY2N(X,Y)	( ( (Y) - 1 ) * 8 + (X) )

// modélisation de l'othellier

typedef enum { HUMAIN , MACHINE }	Joueur ;
typedef enum { NOIR , BLANC , VIDE , JOUABLE } Etat ;

#define		ADVERSAIRE(J)	( (J) == HUMAIN ? MACHINE : HUMAIN )
#define		COULEUR(J)		( (J) == HUMAIN ? NOIR : BLANC )

#define		CASELIBRE(C)	( ( (C) != NOIR )&&( (C) != BLANC ) )

typedef struct {
	Etat	jeu[10][10] ;
	Joueur	joueur ;
	int		nbPi[2] ;	// nb. de pions, indice HOMME | MACHINE
	int		nbCj[2] ;	// nb. de cases jouables, indice HOMME | MACHINE
	int		cj[64] ;	// indices 1..64 des cases jouables (terminateur 0)
						// pour le joueur en cours
} Othellier ;

#endif
