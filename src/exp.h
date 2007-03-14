/*************************************************************************************
 *  Copyright (C) 2007 by Aleix Pol <aleixpol@gmail.com>                             *
 *                                                                                   *
 *  This program is free software; you can redistribute it and/or                    *
 *  modify it under the terms of the GNU General Public License                      *
 *  as published by the Free Software Foundation; either version 2                   *
 *  of the License, or (at your option) any later version.                           *
 *                                                                                   *
 *  This program is distributed in the hope that it will be useful,                  *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                    *
 *  GNU General Public License for more details.                                     *
 *                                                                                   *
 *  You should have received a copy of the GNU General Public License                *
 *  along with this program; if not, write to the Free Software                      *
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA   *
 *************************************************************************************/

#ifndef EXP_H
#define EXP_H

#include <QtCore>

#include <cstdlib>
#include <iostream>
/**
	@author Aleix Pol i Gonzalez
*/

#define i18n QString


typedef enum {
	S,
	R,
	A,
	E1,
	E2,
	E3,
	E,
	K
} actEnum;

typedef enum {
	tAssig, 	//:=
	tLambda,	//->
	tLimits,	//..
	tAdd,		//+
	tSub,		//-
	tMul,		//*
	tDiv,		// /
	tPow,		//^
	tUmi,		//-a
	tFunc,		//f(x)
	tComa,		//,
	tLpr,		//(
	tRpr,		//)
	tEof,
	tMaxOp,
	tVal,
	tVar
} tokEnum;

struct TOKEN {
	QString val;
	tokEnum tipus;
};

// void printPilaOpr(QValueStack<int> opr);
// QString opr2str(int);

class Exp
{
public:
	Exp(QString);
	~Exp();
	
	int parse();
	QStringList error();
	QString mathML();
	
	static TOKEN pillatoken(QString &a, int &l);
private:
	QString mml;
	QString str; //Auxiliar pel parsing
	QStringList err;
	
	tokEnum tok;
	QString tokval;
	
	QStack<int> opr;
	QStack<QString> val;
	QStack<QString> func;
	
	int valTop, oprTop;
	bool firsttok;
	tokEnum antnum;
	
	int getTok();
	int shift();
	int reduce();
};

#endif
