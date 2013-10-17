
#line 113 "exp.g"

/*************************************************************************************
 *  Copyright (C) 2008 by Aleix Pol <aleixpol@kde.org>                               *
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



#include <QtCore/QDebug>
#include "expressionparser.h"
#include "abstractlexer.h"
#include "operator.h"
#include <QCoreApplication>

ExpressionParser::ExpressionParser()
{}

ExpressionParser::~ExpressionParser()
{}

QString funcToTag(const QString& name)
{
	if(Analitza::Operator::toOperatorType(name)!=Analitza::Operator::none)
		return QString("<%1 />").arg(name);
	else
		return "<ci type='function'>"+name+"</ci>";
}

void ExpressionParser::reallocateStack()
{
	int size = m_stateStack.size();
	if (size == 0)
		size = 128;
	else
		size <<= 1;

	m_symStack.resize(size);
	m_stateStack.resize(size);
}

bool ExpressionParser::parse(AbstractLexer *lexer)
{
  const int INITIAL_STATE = 0;
  int yytoken = -1;

  reallocateStack();

  m_tos = 0;
  m_stateStack[++m_tos] = INITIAL_STATE;

  for(;;) {
      const int state = m_stateStack.at(m_tos);
      if (yytoken == -1 && - TERMINAL_COUNT != action_index [state]) {
        yytoken = lexer->lex();
		while(yytoken==tComment) {
			//TODO: Add mathml comment
			m_comments.append(lexer->current.val);
			yytoken = lexer->lex();
		}
		
		if(!lexer->error().isEmpty()) {
			m_err += lexer->error();
			return false;
		}
	  }
      int act = t_action (state, yytoken);
      if (act == ACCEPT_STATE)
        return true;
      else if (act > 0) {
          if (++m_tos == m_stateStack.size())
            reallocateStack();
          m_stateStack[m_tos] = act;
          yytoken = -1;
      } else if (act < 0) {
          int r = - act - 1;

          m_tos -= rhs [r];
          act = m_stateStack.at(m_tos++);
          switch (r) {

#line 207 "exp.g"
 case 0: 
#line 208 "exp.g"
 case 1: 
#line 210 "exp.g"

case 2:
	m_exp = "<math>"+sym(1)+"</math>";
	break;

#line 216 "exp.g"
 case 3: sym(1) = "<declare><ci>"+sym(1)+"</ci>"+sym(3)+"</declare>"; break; 
#line 219 "exp.g"
 case 4: 
#line 220 "exp.g"
 case 5: 
#line 222 "exp.g"

case 6:
	sym(1) = lexer->current.val;
	break;

#line 231 "exp.g"

case 9:
	sym(1) = "<ci>"+sym(1)+"</ci>";
	break;

#line 240 "exp.g"

case 12:
	sym(1) = "<apply><selector />"+sym(3)+sym(1)+"</apply>";
	break;

#line 247 "exp.g"

case 13:
	sym(1) = sym(2);
	break;

#line 257 "exp.g"
 case 18: sym(1)=sym(2); break; 
#line 258 "exp.g"
 case 19: sym(1)=funcToTag(sym(1)); break; 
#line 260 "exp.g"
 case 20: sym(1) = "<apply>"+sym(1)+sym(2)+"</apply>"; break; 
#line 261 "exp.g"
 case 21: sym(1) = "<apply>"+sym(1)+sym(3)+"</apply>"; break; 
#line 262 "exp.g"
 case 22: sym(1) = "<apply>"+sym(1)+       "</apply>"; break; 
#line 264 "exp.g"
 case 23: sym(1) = "<apply>"+sym(3)+sym(1)+"</apply>"; break; 
#line 265 "exp.g"
 case 24: sym(1) = "<apply>"+sym(3)+sym(1)+"</apply>"; break; 
#line 272 "exp.g"

case 28:
	sym(1).prepend(sym(3));
	break;

#line 279 "exp.g"

case 29:
	sym(1)=sym(3)+sym(5)+sym(1);
	break;

#line 286 "exp.g"

case 30:
	sym(1)=sym(3)+"<domainofapplication>"+sym(5)+"</domainofapplication>"+sym(1);
	break;

#line 294 "exp.g"

case 31:
	sym(1) = '<'+sym(1)+" />";
	break;

#line 301 "exp.g"

case 32:
	sym(1) = '<'+sym(1)+'>'+sym(3)+"</"+sym(1)+'>';
	break;

#line 309 "exp.g"

case 33:
	sym(1) = "<lambda>"+sym(1)+sym(3)+"</lambda>";
	break;

#line 317 "exp.g"

case 34:
	sym(1) = "<apply><minus />"+sym(2)+"</apply>";
	break;

#line 324 "exp.g"

case 35:
	sym(1) = "<otherwise>"+sym(2)+"</otherwise>";
	break;

#line 331 "exp.g"
 case 36: sym(1) = "<apply><plus />"  +sym(1)+sym(3)+"</apply>"; break; 
#line 332 "exp.g"
 case 37: sym(1) = "<apply><minus />" +sym(1)+sym(3)+"</apply>"; break; 
#line 333 "exp.g"
 case 38: sym(1) = "<apply><times />" +sym(1)+sym(3)+"</apply>"; break; 
#line 334 "exp.g"
 case 39: sym(1) = "<apply><times />" +sym(1)+sym(2)+"</apply>"; break; 
#line 335 "exp.g"
 case 40: sym(1) = "<apply><divide />"+sym(1)+sym(3)+"</apply>"; break; 
#line 336 "exp.g"
 case 41: sym(1) = "<apply><power />" +sym(1)+sym(3)+"</apply>"; break; 
#line 337 "exp.g"
 case 42: sym(1) = "<apply><eq />"    +sym(1)+sym(3)+"</apply>"; break; 
#line 338 "exp.g"
 case 43: sym(1) = "<apply><leq />"   +sym(1)+sym(3)+"</apply>"; break; 
#line 339 "exp.g"
 case 44: sym(1) = "<apply><geq />"   +sym(1)+sym(3)+"</apply>"; break; 
#line 340 "exp.g"
 case 45: sym(1) = "<apply><lt />"    +sym(1)+sym(3)+"</apply>"; break; 
#line 341 "exp.g"
 case 46: sym(1) = "<apply><gt />"    +sym(1)+sym(3)+"</apply>"; break; 
#line 342 "exp.g"
 case 47: sym(1) = "<apply><neq />"   +sym(1)+sym(3)+"</apply>"; break; 
#line 344 "exp.g"
 case 48: sym(1) = "<piece>"+sym(3)+sym(1)+"</piece>"; break; 
#line 349 "exp.g"

case 50:
	sym(1) += sym(3);
	break;

#line 358 "exp.g"

case 52:
	sym(1) = sym(2);
	break;

#line 365 "exp.g"

case 53:
	sym(1) = "<bvar><ci>"+sym(1)+"</ci></bvar>";
	break;

#line 372 "exp.g"

case 54:
	sym(1) += sym(3);
	break;

#line 379 "exp.g"

case 55:
	sym(1) += sym(3);
	break;

#line 386 "exp.g"

case 56:
	sym(1) = "<uplimit>"+sym(3)+"</uplimit><downlimit>"+sym(1)+"</downlimit>";
	break;

#line 392 "exp.g"

		} // switch
		m_stateStack[m_tos] = nt_action(act, lhs[r] - TERMINAL_COUNT);
	} else {
		int ers = state;
		int shifts = 0;
		int reduces = 0;
		int expected_tokens[3];
		for (int tk = 0; tk < TERMINAL_COUNT; ++tk) {
			int k = t_action(ers, tk);

			if (! k)
				continue;
			else if (k < 0)
				++reduces;
			else if (spell[tk]) {
				if (shifts < 3)
				expected_tokens[shifts] = tk;
				++shifts;
			}
		}

		m_errorLineNumber = lexer->lineNumber();
		int tokFoundType=lexer->current.type;
		QString error;
		
		if (shifts && shifts<3) {
			QString tokFound(spell[tokFoundType]);
			QStringList expectedTokens;
			for (int s = 0; s < shifts; ++s) {
				expectedTokens += '\''+QLatin1String(spell[expected_tokens[s]])+'\'';
			}
			error=QCoreApplication::translate("error message", "Expected %1 instead of '%2'").arg(expectedTokens.join(QCoreApplication::tr(", ")), tokFound);
		} else if(tokFoundType==tLpr) {
			error=QCoreApplication::tr("Missing right parenthesis");
		} else if(tokFoundType==tRpr || tokFoundType==tRcb) {
			error=QCoreApplication::tr("Unbalanced right parenthesis");
		} else
			if(tokFoundType==tId)
				error=QCoreApplication::tr("Unexpected token identifier: %1").arg(lexer->current.val);
			else
				error=QCoreApplication::tr("Unexpected token %1").arg(spell[tokFoundType]);
		m_err.append(error);
		return false;
		}
	}

	return false;
}

