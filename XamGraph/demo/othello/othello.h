/*  ---------------------------------------------------------------------------
 *  filename    :   othello.h
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

#ifndef		OTHELLO_H
#define		OTHELLO_H

#include	"othdef.h"
#include	"ia.h"

extern int mouseX ;
extern int mouseY ;
extern bool mouseUp ;

void	InitPlateau( Othellier* oth ) ;
void	AffichePlateau( Othellier* oth ) ;

int		Survol( int mx, int my ) ;
void	AideSiJouable( Othellier* oth, int n ) ;

int		PosePion( Othellier* oth, int niv, int mx, int my, bool clic ) ;
bool	CaseJouable( Othellier* oth, int n ) ;
void	MajPlateau( Othellier* oth, int n ) ;

int		ChangeJoueur( Othellier* oth ) ;
int		NbrPions( Othellier* oth ) ;
int		CasesDispo( Othellier* oth ) ;

Joueur	AfficheScore( Othellier* oth ) ;

int		Evaluation( Othellier* oth ) ;

#endif
