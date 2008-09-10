/*************************************************************************************
 *  Copyright (C) 2008 by Aleix Pol <aleixpol@gmail.com>                             *
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

#ifndef MATHMLEXPRESSIONWRITTER_H
#define MATHMLEXPRESSIONWRITTER_H

#include "expressionwritter.h"

/**
 *	This class represents the mathml expression writter.
 *
 *	@author Aleix Pol <aleixpol@gmail.com>
 */

class MathMLExpressionWritter : public ExpressionWritter
{
	public:
		MathMLExpressionWritter(const Object* o);
		
		virtual QString accept(const Ci* var);
		virtual QString accept(const Cn* var);
		virtual QString accept(const Container* var);
		virtual QString accept(const Operator* var);
		
		QString result() const { return m_result; }
		
	private:
		QString m_result;
};

#endif
