/*  ---------------------------------------------------------------------------
 *  filename    :   eval.h
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

#ifndef	EVAL_H
#define	EVAL_H

/* 	Critères d'évaluation
	---------------------

	  L'EVALUATION EST TOUJOURS FAITE DU POINT DE VUE DE LA MACHINE ...

	- matériel : différence des nombres de pions en place
	- mobilité : différence des nombres de cases jouables
	- force    : positions respectives des pions sur l'othellier

	  nota : la technique retenue ci-dessous n'engage que son auteur (AM)...

	Calcul du critère de force (pour le joueur actif)
	--------------------------

	+ poids des pions imprenables (bords de l'othellier sauf coins)

	. si la ligne est totalement occupée, tous les pions de la ligne
	  (sauf coins) sont comptabilisés en +/- suivant leur couleur
	. sinon, les pions adjacents au coin, et de la même couleur que
	  celui-ci, sont retenus en +/- suivant la couleur

	+ poids des coins

	. la valeur d'un coin doit être supérieure à la somme max. des valeurs
	  des pions d'un bord hors coins

	+ poids des cases C et X

	. ce sont les cases adjacentes à un coin, de valeur négative si celui-ci
	  n'est pas occupé.

	+ poids des cases centrales

	. elles sont importantes surtout au début (favorisent la mobilité)

	Prépondérance des critères (fonction de l'avancement de la partie)
	--------------------------
	. début de partie  (15 coups) : matériel (+)   , mobilité (++) , force (+)
	. milieu de partie (30 coups) : matériel (+)   , mobilité (+)  , force (+++)
	. fin de partie    (15 coups) : matériel (+++) , mobilité (0)  , force (++)
*/

#include	"othdef.h"

#define	EVALMAX		20000					// note maximale
#define	EVALBORD	75						// pion en bordure
#define	EVALCOIN	500						// pion dans un coin
#define	EVALCASEX	200						// pion sur case X
#define	EVALCASEC	40						// pion sur case C
#define	EVALCENTRE	15						// pion sur les 4 cases centrales

#define	EVALMAT		5						// coefficient 'matériel'
#define	EVALMOB		20						// coefficient 'mobilité'

int		IaEvaluation( Othellier* oth ) ;

#endif
