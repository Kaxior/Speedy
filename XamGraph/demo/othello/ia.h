/*  ---------------------------------------------------------------------------
 *  filename    :   ia.h
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

#ifndef	IA_H
#define	IA_H

#include	"othdef.h"
#include	"eval.h"

void	IaRetournePions( Othellier* oth, int n ) ;
int		IaChercheCasesJouables( Othellier* oth ) ;
int		IaCalculCoup( Othellier* oth, int niv ) ;

#endif
